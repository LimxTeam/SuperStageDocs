# SuperDMX Module API Documentation

> **Document Version**: 1.0  
> **Applicable Version**: SuperStage Plugin 2025/2026  
> **Target Audience**: Blueprint Developers / C++ Plugin Integration Developers (without source code modification permissions)  
> **Last Updated**: 2026-03

---

## Table of Contents

1. [Module Overview](#1-module-overview)
2. [Data Types](#2-data-types)
   - [ESuperDMXProtocol](#21-esuperdmxprotocol)
   - [FSuperDMXEndpoint](#22-fsuperdmxendpoint)
   - [FSuperDMXIOConfig](#23-fsuperdmxioconfig)
3. [USuperDMXSubsystem — DMX Communication Subsystem](#3-usuperdmxsubsystem--dmx-communication-subsystem)
   - [Getting the Instance](#31-getting-the-instance)
   - [Lifecycle](#32-lifecycle)
   - [Configuration Management](#33-configuration-management)
   - [Data Reception and Query](#34-data-reception-and-query)
   - [Data Transmission](#35-data-transmission)
   - [Diagnostics and Monitoring](#36-diagnostics-and-monitoring)
   - [sACN Multicast Management](#37-sacn-multicast-management)
4. [Sequencer Integration](#4-sequencer-integration)
   - [UMovieSceneSuperDMXTrack](#41-umoviescenesuperdmxtrack)
   - [UMovieSceneSuperDMXSection](#42-umoviescenesuperdmxsection)
5. [Thread Safety Model](#5-thread-safety-model)
6. [Network Protocol Details](#6-network-protocol-details)
7. [Universe Numbering and StartUniverse Mapping](#7-universe-numbering-and-startuniverse-mapping)
8. [Usage Examples](#8-usage-examples)
9. [FAQ and Troubleshooting](#9-faq-and-troubleshooting)

---

## 1. Module Overview

**SuperDMX** is the underlying communication module of the SuperStage plugin suite, responsible for **DMX over IP** data transmission, reception, and caching.

| Item | Description |
|------|------|
| **Module Type** | Runtime |
| **Export Macro** | `SUPERDMX_API` |
| **Module Entry** | `FSuperDMXModule` (`SuperDMX.h`) |
| **Core Subsystem** | `USuperDMXSubsystem` (`UEngineSubsystem` global singleton) |
| **Protocol Support** | Art-Net (UDP 6454), sACN/E1.31 (UDP 5568, reserved in code) |
| **Header Directory** | `Source/SuperDMX/Public/` |

### Module Dependencies

Add to `.Build.cs`:

```csharp
PublicDependencyModuleNames.Add("SuperDMX");
```

### Public Header File List

| Header File | Content |
|--------|------|
| `SuperDMX.h` | Module interface (`FSuperDMXModule`) |
| `SuperDMXTypes.h` | Data types (protocol enum, endpoint, I/O config) |
| `SuperDMXSubsystem.h` | DMX subsystem (send/receive/cache/query) |
| `Sequencer/MovieSceneSuperDMXTrack.h` | Sequencer DMX track |
| `Sequencer/MovieSceneSuperDMXSection.h` | Sequencer DMX section |

---

## 2. Data Types

### 2.1 ESuperDMXProtocol

**Header File**: `SuperDMXTypes.h`

DMX network protocol enum.

```cpp
UENUM(BlueprintType)
enum class ESuperDMXProtocol : uint8
{
    ArtNet  UMETA(DisplayName = "Art-Net"),
};
```

| Value | Description | Default Port |
|----|------|----------|
| `ArtNet` | Art-Net protocol (OpDmx 0x5000) | 6454 |

> **Note**: sACN/E1.31 protocol is reserved in code (port 5568), current version only supports Art-Net.

---

### 2.2 FSuperDMXEndpoint

**Header File**: `SuperDMXTypes.h`

DMX network endpoint configuration struct, defines network parameters for one input or output endpoint.

```cpp
USTRUCT(BlueprintType)
struct FSuperDMXEndpoint
```

#### Properties

| Property | Type | Default | Description |
|------|------|--------|------|
| `bEnabled` | `bool` | `true` | Whether the endpoint is enabled. When disabled, receiver/transmitter will not be created |
| `LocalIp` | `FString` | `""` | Local bind IP. When empty or `"0.0.0.0"`, binds all network interfaces |
| `RemoteIp` | `FString` | `""` | Remote target IP. For output: unicast/broadcast address; sACN uses `239.255.*.*` multicast |
| `Port` | `int32` | `6454` | Network port. Art-Net standard port 6454, sACN standard port 5568 |
| `StartUniverse` | `int32` | `0` | Starting Universe offset (see [Section 7](#7-universe-numbering-and-startuniverse-mapping) for details) |

#### Property Details

##### bEnabled

Controls whether the endpoint is active. For **input** (`FSuperDMXIOConfig.Input`):
- `true`: Create UDP receiver, begin listening for DMX data
- `false`: Do not create receiver, no network resource consumption

For **output** (`FSuperDMXIOConfig.Output`):
- `true`: Create UDP sender, `SendDMXBuffer()` sends data to network
- `false`: `SendDMXBuffer()` only updates local cache, does not send to network

##### LocalIp

Specifies the local network interface to bind to. In multi-NIC environments, a specific IP can be specified to ensure DMX data is only sent/received through that interface.

| Value | Behavior |
|----|------|
| `""` or `"0.0.0.0"` | Listen on all network interfaces |
| `"192.168.1.100"` | Listen only on specified interface |
| `"10.0.0.1"` | Used to isolate DMX network segment |

##### RemoteIp

Target address for the output endpoint.

| Scenario | Value |
|------|-----|
| Broadcast (all devices) | `"255.255.255.255"` or `"2.255.255.255"` (Art-Net subnet broadcast) |
| Unicast (specific device) | `"192.168.1.200"` |
| sACN multicast | `"239.255.0.1"` (Universe 1) |

##### StartUniverse

Used to align different consoles' Universe numbering conventions. See [Section 7](#7-universe-numbering-and-startuniverse-mapping) for details.

---

### 2.3 FSuperDMXIOConfig

**Header File**: `SuperDMXTypes.h`

Complete DMX input/output configuration entity.

```cpp
USTRUCT(BlueprintType)
struct FSuperDMXIOConfig
```

#### Properties

| Property | Type | Default | Description |
|------|------|--------|------|
| `Protocol` | `ESuperDMXProtocol` | `ArtNet` | Network protocol |
| `Input` | `FSuperDMXEndpoint` | See Endpoint defaults | Input configuration (receive DMX) |
| `Output` | `FSuperDMXEndpoint` | See Endpoint defaults | Output configuration (send DMX) |

#### Usage Example

```cpp
FSuperDMXIOConfig Config;
Config.Protocol = ESuperDMXProtocol::ArtNet;

// Input configuration: listen for Art-Net on all interfaces
Config.Input.bEnabled = true;
Config.Input.LocalIp = TEXT("");       // 0.0.0.0
Config.Input.Port = 6454;
Config.Input.StartUniverse = 0;        // MA2 default alignment

// Output configuration: broadcast to entire subnet
Config.Output.bEnabled = true;
Config.Output.RemoteIp = TEXT("255.255.255.255");
Config.Output.Port = 6454;
Config.Output.StartUniverse = 0;
```

---

## 3. USuperDMXSubsystem — DMX Communication Subsystem

**Header File**: `SuperDMXSubsystem.h`  
**Base Class**: `UEngineSubsystem`  
**Export Macro**: `SUPERDMX_API`

The core singleton service for DMX communication, **automatically created on engine startup**, providing the following capabilities:
- Network configuration and lifecycle management
- Art-Net/sACN data reception and parsing
- Multi-Universe DMX data caching
- DMX data query (thread-safe)
- DMX data transmission (Sequencer playback / programmatic output)
- Signal activity diagnostics

---

### 3.1 Getting the Instance

`USuperDMXSubsystem` is a `UEngineSubsystem`, globally unique, **no manual creation needed**.

**C++ access method**:

```cpp
#include "SuperDMXSubsystem.h"

USuperDMXSubsystem* DMX = GEngine->GetEngineSubsystem<USuperDMXSubsystem>();
```

**Blueprint access method**:

> In the current version, the UFUNCTION macros have been commented out (`//UFUNCTION`), therefore these functions **cannot be called directly** in Blueprint. They need to be bridged through custom Blueprint function libraries or C++ plugins.

---

### 3.2 Lifecycle

| Phase | Behavior |
|------|------|
| **Engine Startup** | `Initialize()` automatically called → start receiver and sender with default configuration |
| **Runtime** | Receive thread continuously parses DMX packets → writes to cache |
| **Engine Shutdown** | `Deinitialize()` automatically called → stop receiver → destroy sender → release network resources |

> **Important**: On engine startup, the system automatically begins listening with default configuration (ArtNet, port 6454, bind 0.0.0.0). To change configuration, call `ApplyConfig()`.

---

### 3.3 Configuration Management

#### ApplyConfig

```cpp
void ApplyConfig(const FSuperDMXIOConfig& InConfig);
```

Apply I/O configuration and restart receiver and sender.

**Parameters**:

| Parameter | Type | Description |
|------|------|------|
| `InConfig` | `const FSuperDMXIOConfig&` | Complete I/O configuration |

**Behavior**:
1. Save configuration to internal `Config` member
2. Stop current receiver → recreate receiver with new configuration
3. Destroy current sender → recreate sender with new configuration

**Notes**:
- Takes effect **immediately** after calling, cached DMX data is **not cleared**
- If `Input.bEnabled = false`, receiver will not start
- If `Output.bEnabled = false`, sender will not be created
- Can be called multiple times at runtime to switch network configuration

**Example**:

```cpp
USuperDMXSubsystem* DMX = GEngine->GetEngineSubsystem<USuperDMXSubsystem>();

FSuperDMXIOConfig Config;
Config.Protocol = ESuperDMXProtocol::ArtNet;
Config.Input.bEnabled = true;
Config.Input.Port = 6454;
Config.Output.bEnabled = true;
Config.Output.RemoteIp = TEXT("192.168.1.255");
Config.Output.Port = 6454;

DMX->ApplyConfig(Config);
```

---

#### Stop

```cpp
void Stop();
```

Stop all network activity (receiver and sender).

**Behavior**:
- Stop receive thread
- Close receive socket
- Destroy send socket
- Cached DMX data is retained unchanged

**Use cases**:
- Temporarily pause DMX communication
- Cleanup before switching network environments

---

### 3.4 Data Reception and Query

#### GetDMXBuffer

```cpp
bool GetDMXBuffer(int32 Universe, TArray<uint8>& OutBuffer) const;
```

Get the **complete DMX buffer** (latest frame) for a specified Universe.

**Parameters**:

| Parameter | Type | Direction | Description |
|------|------|------|------|
| `Universe` | `int32` | Input | Target Universe number (internal numbering, starting from 1) |
| `OutBuffer` | `TArray<uint8>&` | Output | Receive buffer, length equals actual frame data size (usually 512 bytes) |

**Return value**:

| Value | Description |
|----|------|
| `true` | Successfully retrieved data |
| `false` | This Universe has not received any data yet |

**Thread-safe**: ✅ Internally protected with `FCriticalSection`

**Notes**:
- Returns a **copy of the data**, modifying `OutBuffer` after the call does not affect the internal cache
- Buffer length depends on the Length field of the received Art-Net packet (usually 512)
- Channel indexing: `OutBuffer[0]` corresponds to DMX channel 1, `OutBuffer[511]` corresponds to channel 512

**Example**:

```cpp
TArray<uint8> Buffer;
if (DMX->GetDMXBuffer(1, Buffer))  // Universe 1
{
    // Buffer[0] = Channel 1 value (0-255)
    // Buffer[1] = Channel 2 value
    // ...
    uint8 DimmerValue = Buffer[0];  // Assuming Dimmer is on channel 1
}
```

---

#### GetDMXValue

```cpp
bool GetDMXValue(int32 Universe, int32 Address, uint8& OutValue) const;
```

Get a **single DMX value** for a specified Universe and channel address.

**Parameters**:

| Parameter | Type | Direction | Description |
|------|------|------|------|
| `Universe` | `int32` | Input | Target Universe number (internal numbering, starting from 1) |
| `Address` | `int32` | Input | DMX channel address (**1-512**, note: starts from 1) |
| `OutValue` | `uint8&` | Output | Channel value (0-255) |

**Return value**:

| Value | Description |
|----|------|
| `true` | Successfully retrieved |
| `false` | Address out of bounds (<1 or >512), Universe has no data, or address exceeds received data range |

**Thread-safe**: ✅

**Notes**:
- Address starts from **1**, consistent with DMX industry standards
- If Address < 1 or Address > 512, returns `false` directly
- If Universe exists but Address exceeds received data length, returns `false`

**Example**:

```cpp
uint8 Value = 0;
if (DMX->GetDMXValue(1, 1, Value))  // Universe 1, Channel 1
{
    float Normalized = Value / 255.0f;  // Normalize to 0.0-1.0
}
```

---

#### GetKnownUniverses

```cpp
void GetKnownUniverses(TArray<int32>& OutUniverses) const;
```

Get a list of all Universe numbers that currently have cached data.

**Parameters**:

| Parameter | Type | Direction | Description |
|------|------|------|------|
| `OutUniverses` | `TArray<int32>&` | Output | List of Universe numbers (**sorted ascending**) |

**Thread-safe**: ✅

**Use cases**:
- Building a DMX Activity Monitor
- Dynamically discovering active Universes

**Example**:

```cpp
TArray<int32> Universes;
DMX->GetKnownUniverses(Universes);
for (int32 Uni : Universes)
{
    UE_LOG(LogTemp, Log, TEXT("Active Universe: %d"), Uni);
}
```

---

#### GetActiveUniverses

```cpp
void GetActiveUniverses(TArray<int32>& OutUniverses) const;
```

Get a list of all active Universes (same functionality as `GetKnownUniverses`, but without sorting).

**Parameters**:

| Parameter | Type | Direction | Description |
|------|------|------|------|
| `OutUniverses` | `TArray<int32>&` | Output | List of Universe numbers (**unspecified order**) |

**Thread-safe**: ✅

> **Difference from GetKnownUniverses**: `GetKnownUniverses` returns results sorted ascending, `GetActiveUniverses` returns unsorted results (slightly better performance).

---

#### ClearAllDMXBuffers

```cpp
void ClearAllDMXBuffers();
```

Clear DMX buffer data for all Universes.

**Behavior**:
- Clears internal `UniverseToSlots` map
- Does not affect receiver/sender running state
- After clearing, `GetDMXBuffer()` / `GetDMXValue()` will return `false` until new data is received

**Use cases**:
- Reset DMX state
- Cleanup before switching scenes/shows

---

#### ClearDMXBufferForUniverse

```cpp
void ClearDMXBufferForUniverse(int32 Universe);
```

Clear DMX buffer data for a specified Universe.

**Parameters**:

| Parameter | Type | Description |
|------|------|------|
| `Universe` | `int32` | The Universe number to clear |

**Behavior**:
- Removes data for the specified Universe from the cache
- Other Universes are unaffected

---

### 3.5 Data Transmission

#### SendDMXBuffer

```cpp
bool SendDMXBuffer(int32 Universe, const TArray<uint8>& Buffer);
```

Send one frame of DMX data to a specified Universe.

**Parameters**:

| Parameter | Type | Direction | Description |
|------|------|------|------|
| `Universe` | `int32` | Input | Target Universe number (internal numbering, ≥1) |
| `Buffer` | `const TArray<uint8>&` | Input | DMX data (length 1-512 bytes) |

**Return value**:

| Value | Description |
|----|------|
| `true` | Send successful (or output not enabled but local cache updated successfully) |
| `false` | Invalid parameters, engine not initialized, or network send failed |

**Behavior flow**:
1. **Parameter validation**: `GEngine` not null, `Universe > 0`, `Buffer.Num() > 0`
2. **Universe mapping**: Calculate `NetworkUniverse = Universe + StartUniverse - 1`
3. **Update local cache**: Write Buffer to `UniverseToSlots[Universe]` (for real-time activity monitor reflection)
4. **Network send** (only when `Output.bEnabled = true`):
   - Build Art-Net OpDmx packet (18-byte header + DMX data)
   - Send via UDP to `Output.RemoteIp:Output.Port`
5. If sender not yet created, automatically attempt `SetupSender()` initialization

**Art-Net packet structure** (built internally by SendDMXBuffer):

| Offset | Bytes | Content |
|------|--------|------|
| 0-7 | 8 | `"Art-Net\0"` magic number |
| 8-9 | 2 | OpCode `0x5000` (little-endian) |
| 10-11 | 2 | Protocol version `0x000E` (14) |
| 12 | 1 | Sequence number (fixed 0) |
| 13 | 1 | Physical port (fixed 0) |
| 14-15 | 2 | Universe (little-endian) |
| 16-17 | 2 | Data length (big-endian) |
| 18+ | 1-512 | DMX data |

**Notes**:
- Buffer length will be clamped to 1-512 bytes
- Even if `Output.bEnabled = false`, local cache will be updated
- Send socket enables broadcast mode (`SetBroadcast(true)`)

**Example**:

```cpp
// Build and send DMX frame
TArray<uint8> Buffer;
Buffer.SetNumZeroed(512);
Buffer[0] = 255;  // Channel 1 = full
Buffer[1] = 128;  // Channel 2 = 50%
Buffer[2] = 0;    // Channel 3 = off

DMX->SendDMXBuffer(1, Buffer);  // Send to Universe 1
```

---

### 3.6 Diagnostics and Monitoring

#### GetSecondsSinceLastInput

```cpp
float GetSecondsSinceLastInput() const;
```

Get the number of seconds since the last DMX packet was received from **any Universe**.

**Return value**:

| Value | Description |
|----|------|
| `float` | Seconds since last input |
| `BIG_NUMBER` | No input has ever been received |

**Thread-safe**: ✅

**Use cases**:
- Signal loss detection
- Network connection status indicator
- Auto-sleep/wake mechanism

**Example**:

```cpp
float Elapsed = DMX->GetSecondsSinceLastInput();
if (Elapsed > 5.0f)
{
    // No DMX data received for over 5 seconds, possible signal loss
    UE_LOG(LogTemp, Warning, TEXT("DMX signal lost for %.1f seconds"), Elapsed);
}
```

---

#### GetSecondsSinceLastInputForUniverse

```cpp
float GetSecondsSinceLastInputForUniverse(int32 Universe) const;
```

Get the number of seconds since the last DMX packet was received for a **specified Universe**.

**Parameters**:

| Parameter | Type | Description |
|------|------|------|
| `Universe` | `int32` | Target Universe number |

**Return value**:

| Value | Description |
|----|------|
| `float` | Seconds since last input for this Universe |
| `BIG_NUMBER` | This Universe has never received input |

**Thread-safe**: ✅

---

#### GetProtocol

```cpp
ESuperDMXProtocol GetProtocol() const;
```

Get the currently configured DMX protocol.

**Return value**: Current `ESuperDMXProtocol` value (`ArtNet`)

---

### 3.7 sACN Multicast Management

#### SetSACNMulticastUniverses

```cpp
void SetSACNMulticastUniverses(const TArray<int32>& InUniverses);
```

Set the list of multicast Universes to join in sACN mode.

**Parameters**:

| Parameter | Type | Description |
|------|------|------|
| `InUniverses` | `const TArray<int32>&` | List of Universe numbers (>0) |

**Behavior** (in sACN mode):
1. Save the Universe list
2. Stop receiver
3. Restart receiver with new multicast groups (automatically computes `239.255.{Hi}.{Lo}` multicast addresses)

> **Current status**: sACN functionality is reserved but **not activated** in the current version (code is commented out). Calling this function has no effect.

---

## 4. Sequencer Integration

SuperDMX provides deep integration with UE5 Sequencer, allowing DMX animations to be recorded and played back on a timeline.

### 4.1 UMovieSceneSuperDMXTrack

**Header File**: `Sequencer/MovieSceneSuperDMXTrack.h`  
**Base Class**: `UMovieSceneNameableTrack`, `IMovieSceneTrackTemplateProducer`  
**Export Macro**: `SUPERDMX_API`

DMX track container in Sequencer that manages DMX sections for multiple Universes.

#### Class Structure

```
UMovieSceneSuperDMXTrack (DMX Track)
 ├── Section[0]: DMX section for Universe 1
 ├── Section[1]: DMX section for Universe 2
 └── Section[N]: DMX section for Universe N
```

#### Public Methods

##### AddNewSection

```cpp
UMovieSceneSuperDMXSection* AddNewSection(int32 Universe);
```

Add a section for the specified Universe. If a section for that Universe already exists, returns the existing one.

**Parameters**:

| Parameter | Type | Description |
|------|------|------|
| `Universe` | `int32` | Target Universe number |

**Return value**: Pointer to newly created or existing `UMovieSceneSuperDMXSection`

##### UMovieSceneTrack Interface

| Method | Description |
|------|------|
| `GetAllSections()` | Return array of all sections |
| `CreateNewSection()` | Create a blank section |
| `AddSection(Section)` | Add section to track |
| `RemoveSection(Section)` | Remove section |
| `HasSection(Section)` | Check if section exists |
| `IsEmpty()` | Whether track is empty |
| `RemoveAllAnimationData()` | Clear all sections |
| `SupportsType(SectionClass)` | Type support check |
| `CreateTemplateForSection(Section)` | Create evaluation template (`FMovieSceneSuperDMXTemplate`) |

---

### 4.2 UMovieSceneSuperDMXSection

**Header File**: `Sequencer/MovieSceneSuperDMXSection.h`  
**Base Class**: `UMovieSceneSection`  
**Export Macro**: `SUPERDMX_API`

DMX animation section for a single Universe, storing keyframe curves for 1-512 channels.

#### Properties

| Property | Type | Description |
|------|------|------|
| `Universe` | `int32` | Target Universe number |
| `ChannelCurves` | `TMap<int32, FRichCurve>` | Channel curve mapping (channel number 1-512 → animation curve) |
| `bIsRecording` | `bool` | Recording status flag |

#### Public Methods

##### GetOrCreateCurve

```cpp
FRichCurve& GetOrCreateCurve(int32 Channel);
```

Get or create the animation curve for a specified channel.

**Parameters**:

| Parameter | Type | Description |
|------|------|------|
| `Channel` | `int32` | DMX channel number (1-512) |

**Return value**: Reference to the corresponding channel's `FRichCurve` (creates empty curve if not exists)

##### GetCurve

```cpp
const FRichCurve* GetCurve(int32 Channel) const;
```

Get the animation curve for a specified channel (read-only).

**Return value**: Pointer to curve, returns `nullptr` if not exists

##### EvaluateAndSendDMX

```cpp
void EvaluateAndSendDMX(const FMovieSceneContext& Context) const;
```

Evaluate all channel curves based on the current time of the Sequencer context, and send via `USuperDMXSubsystem::SendDMXBuffer()`.

**Behavior**:
1. Check `bIsRecording`, skip sending during recording (avoid echo conflicts)
2. Read the last state of the current Universe as base buffer
3. Iterate through `ChannelCurves`, evaluate the current value of each curve based on time
4. Only overwrite channels that have curve definitions (preserve channel values from other sources)
5. Call `SendDMXBuffer()` to send the merged 512-byte frame

##### Recording Control

```cpp
bool GetIsRecording() const;
void SetIsRecording(bool bIn);
```

Control recording status. During recording, `EvaluateAndSendDMX()` will not send data, avoiding echo/conflict during recording.

#### Data Flow

```
Sequencer playback engine
  │
  ▼
UMovieSceneSuperDMXTrack::CreateTemplateForSection()
  │ Create FMovieSceneSuperDMXTemplate
  ▼
FMovieSceneSuperDMXTemplate::Evaluate()
  │ Pass FMovieSceneContext (contains current time)
  ▼
UMovieSceneSuperDMXSection::EvaluateAndSendDMX()
  │ Iterate ChannelCurves → evaluate curve values → build 512B buffer
  ▼
USuperDMXSubsystem::SendDMXBuffer()
  │ Update local cache + build Art-Net packet + UDP send
  ▼
External DMX devices / Light Actors read
```

---

## 5. Thread Safety Model

`USuperDMXSubsystem` operates in a multi-threaded environment with the following protection mechanisms:

### Thread Roles

| Thread | Responsibility |
|------|------|
| **Receive Thread** (`FUdpSocketReceiver`) | Runs continuously in background, receives UDP packets → parses → writes to cache |
| **Game Thread** | Calls `GetDMXValue`/`GetDMXBuffer`/`SendDMXBuffer` to read/write cache |

### Mutex Protection

| Mutex | Protected Data | Users |
|--------|----------|--------|
| `DataMutex` (`FCriticalSection`) | `UniverseToSlots` (DMX data cache) | Read: `GetDMXBuffer/Value/Active/KnownUniverses`, `ClearAll/ClearForUniverse`<br>Write: `HandleDatagram`, `SendDMXBuffer` |
| `TimeMutex` (`FCriticalSection`) | `LastAnyInputSeconds`, `UniverseToLastInputSeconds` | Read: `GetSecondsSinceLastInput*`<br>Write: `HandleDatagram` |

### Lock Usage Pattern

All locks use the `FScopeLock` RAII pattern, ensuring exception safety:

```cpp
// Typical read pattern
bool USuperDMXSubsystem::GetDMXValue(...) const
{
    FScopeLock Lock(&DataMutex);
    // Read UniverseToSlots under lock protection
    // FScopeLock destructor automatically releases the lock
}
```

### Performance Considerations

- Receiver uses **zero throttling** (`FTimespan::FromMicroseconds(0)`), maximizing throughput
- Receive buffer **2 MB**, send buffer **256 KB**
- Lock granularity is at **operation level** (not channel level), simple and efficient
- `GetDMXBuffer` returns a **data copy**, lock hold time is extremely short

---

## 6. Network Protocol Details

### 6.1 Art-Net OpDmx Packet Format

Art-Net packets sent and received by SuperDMX follow the Art-Net 4 protocol:

```
Offset  Size  Field          Value / Description
─────────────────────────────────────
0       8     Magic          "Art-Net\0" (ASCII + NULL)
8       2     OpCode         0x5000 (little-endian, DMX data packet)
10      2     ProtVer        0x000E (14, big-endian)
12      1     Sequence       0 (sequence number, currently fixed)
13      1     Physical       0 (physical port, currently fixed)
14      2     Universe       Universe number (little-endian)
16      2     Length         DMX data length (big-endian, 1-512)
18      N     Data           DMX channel data (N = Length)
```

### 6.2 sACN E1.31 Packet Format (Reserved)

```
Offset  Size  Field                  Value / Description
──────────────────────────────────────────
4       12    ACN Packet ID          "ACN-E1.17\0\0\0"
113     2     Universe               Universe number (big-endian)
123     2     Property Value Count   DMX data length + 1 (big-endian, includes start code)
125     1     Start Code             0x00
126     N     Data                   DMX channel data (N = Count - 1)
```

### 6.3 Parsing Key Points

| Item | Art-Net | sACN |
|------|---------|------|
| **Minimum packet length** | 18 bytes | 126 bytes |
| **Magic number verification** | `"Art-Net\0"` 8 bytes | ACN ID 12 bytes |
| **Universe byte order** | Little-endian (LE) | Big-endian (BE) |
| **Data length byte order** | Big-endian (BE) | Big-endian (BE) |
| **OpCode verification** | `0x5000` | None (identified by ACN ID) |

---

## 7. Universe Numbering and StartUniverse Mapping

### Problem Background

Different consoles have different Universe numbering conventions:
- **MA2/MA3 default**: Network Universe starts from 0 (Art-Net standard)
- **Some consoles**: Network Universe starts from 1
- **SuperStage internal**: Universe starts from 1

### Mapping Formula

`FSuperDMXEndpoint.StartUniverse` is used to unify numbering:

**Input direction** (receive):

```
InternalUniverse = NetworkUniverse - StartUniverse + 1
```

**Output direction** (send):

```
NetworkUniverse = InternalUniverse + StartUniverse - 1
```

### Configuration Examples

| Scenario | StartUniverse | Network Universe 0 | Network Universe 1 |
|------|---------------|-----------------|-----------------|
| **MA2 default** | `0` | → Internal 1 | → Internal 2 |
| **Console starting from 1** | `1` | → Discard (<1) | → Internal 1 |
| **Offset alignment** | `10` | → Discard | → Discard (<1) |

> **Important**: If the calculated `InternalUniverse < 1`, the packet will be **discarded**.

### Best Practices

| Console | Recommended StartUniverse |
|------|-------------------|
| grandMA2 | `0` |
| grandMA3 | `0` |
| Hog4 | `0` |
| ETC Eos | `0` |
| Custom / Starting from 1 | `1` |

---

## 8. Usage Examples

### 8.1 Basic Reception Example

```cpp
#include "SuperDMXSubsystem.h"

void AMyActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    USuperDMXSubsystem* DMX = GEngine->GetEngineSubsystem<USuperDMXSubsystem>();
    if (!DMX) return;

    // Read Universe 1, Channel 1 value
    uint8 DimmerRaw = 0;
    if (DMX->GetDMXValue(1, 1, DimmerRaw))
    {
        float Dimmer = DimmerRaw / 255.0f;
        // Apply brightness...
    }

    // Read entire frame
    TArray<uint8> Buffer;
    if (DMX->GetDMXBuffer(1, Buffer))
    {
        uint8 Red   = Buffer[1];  // Channel 2
        uint8 Green = Buffer[2];  // Channel 3
        uint8 Blue  = Buffer[3];  // Channel 4
    }
}
```

### 8.2 Configuration Switching Example

```cpp
void AMyController::SwitchToArtNet(const FString& BindIp, int32 Port)
{
    USuperDMXSubsystem* DMX = GEngine->GetEngineSubsystem<USuperDMXSubsystem>();
    if (!DMX) return;

    FSuperDMXIOConfig Config;
    Config.Protocol = ESuperDMXProtocol::ArtNet;

    // Input: bind to specific network interface
    Config.Input.bEnabled = true;
    Config.Input.LocalIp = BindIp;
    Config.Input.Port = Port;
    Config.Input.StartUniverse = 0;

    // Output: broadcast
    Config.Output.bEnabled = true;
    Config.Output.RemoteIp = TEXT("255.255.255.255");
    Config.Output.Port = Port;
    Config.Output.StartUniverse = 0;

    DMX->ApplyConfig(Config);
}
```

### 8.3 DMX Send Example (Programmatic Control)

```cpp
void AMyController::SendDMXFrame(int32 Universe)
{
    USuperDMXSubsystem* DMX = GEngine->GetEngineSubsystem<USuperDMXSubsystem>();
    if (!DMX) return;

    TArray<uint8> Frame;
    Frame.SetNumZeroed(512);

    // Set channel values
    Frame[0] = 255;   // Channel 1: Dimmer full
    Frame[1] = 128;   // Channel 2: Pan center
    Frame[2] = 64;    // Channel 3: Tilt 1/4
    Frame[3] = 200;   // Channel 4: Red
    Frame[4] = 100;   // Channel 5: Green
    Frame[5] = 50;    // Channel 6: Blue

    bool bSuccess = DMX->SendDMXBuffer(Universe, Frame);
    // bSuccess is true indicating successful send
}
```

### 8.4 Signal Monitoring Example

```cpp
void AMyMonitor::CheckDMXHealth()
{
    USuperDMXSubsystem* DMX = GEngine->GetEngineSubsystem<USuperDMXSubsystem>();
    if (!DMX) return;

    // Global signal check
    float GlobalElapsed = DMX->GetSecondsSinceLastInput();
    bool bSignalActive = (GlobalElapsed < 2.0f);

    // Per-Universe check
    TArray<int32> Universes;
    DMX->GetKnownUniverses(Universes);
    for (int32 Uni : Universes)
    {
        float UniElapsed = DMX->GetSecondsSinceLastInputForUniverse(Uni);
        if (UniElapsed > 5.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("Universe %d: signal lost (%.1fs)"), Uni, UniElapsed);
        }
    }
}
```

---

## 9. FAQ and Troubleshooting

### Q1: Cannot receive DMX data

| Check Item | Action |
|--------|------|
| Firewall | Ensure UDP port 6454 is not blocked |
| IP Binding | Try `LocalIp = ""` to bind all interfaces |
| Console Output | Confirm console has Art-Net output enabled and correct subnet |
| StartUniverse | Check if offset causes InternalUniverse < 1 (data discarded) |
| Input.bEnabled | Ensure set to `true` |
| Diagnostics | `GetSecondsSinceLastInput()` returning `BIG_NUMBER` means no data has ever been received |

### Q2: High data latency

| Check Item | Action |
|--------|------|
| Network Topology | DMX devices and UE host should be on the same subnet/VLAN |
| Switch | Use gigabit switch, avoid IGMP snooping interference with broadcasts |
| Frame Rate | Ensure UE frame rate ≥ DMX refresh rate (usually 44fps) |

### Q3: SendDMXBuffer returns false

| Cause | Solution |
|------|------|
| `Universe ≤ 0` | Universe must be ≥ 1 |
| `Buffer` is empty | Must contain at least 1 byte |
| `Output.bEnabled = false` | In this case, only local cache is updated (returns `true`) |
| Sender not initialized | Check if `Output.RemoteIp` is valid |

### Q4: Multi-Universe performance

- Each Universe independently cached, queries do not interfere
- Lock granularity at operation level, no significant contention under high concurrency
- Recommend using `GetDMXBuffer()` for batch reading rather than per-channel `GetDMXValue()`

### Q5: Sequencer recording and playback conflict

- During recording, `bIsRecording = true`, playback sending is automatically skipped
- After recording, call `SetIsRecording(false)` to resume playback
- Buffer merge strategy during playback: only overwrite channels with curves, preserve other channel values

---

## Appendix: API Quick Reference

### USuperDMXSubsystem

| Function | Category | Thread-Safe | Description |
|------|------|----------|------|
| `ApplyConfig(Config)` | Config | — | Apply I/O config and restart transceiver |
| `Stop()` | Config | — | Stop all network activity |
| `GetDMXBuffer(Uni, Out)` | Query | ✅ | Get full DMX frame buffer |
| `GetDMXValue(Uni, Addr, Out)` | Query | ✅ | Get single channel value |
| `GetKnownUniverses(Out)` | Query | ✅ | Get known Universe list (sorted) |
| `GetActiveUniverses(Out)` | Query | ✅ | Get active Universe list (unsorted) |
| `ClearAllDMXBuffers()` | Management | ✅ | Clear all buffers |
| `ClearDMXBufferForUniverse(Uni)` | Management | ✅ | Clear single Universe buffer |
| `SendDMXBuffer(Uni, Buffer)` | Send | ✅ | Send DMX frame |
| `GetSecondsSinceLastInput()` | Diagnostic | ✅ | Global last input time |
| `GetSecondsSinceLastInputForUniverse(Uni)` | Diagnostic | ✅ | Specified Universe last input time |
| `GetProtocol()` | Query | — | Current protocol |
| `SetSACNMulticastUniverses(Universes)` | sACN | — | Set multicast Universes (reserved) |

### UMovieSceneSuperDMXTrack

| Function | Description |
|------|------|
| `AddNewSection(Universe)` | Add Universe section |
| `GetAllSections()` | Get all sections |

### UMovieSceneSuperDMXSection

| Function/Property | Description |
|-----------|------|
| `Universe` | Target Universe |
| `ChannelCurves` | Channel → Curve mapping |
| `GetOrCreateCurve(Channel)` | Get/create channel curve |
| `GetCurve(Channel)` | Get channel curve (read-only) |
| `EvaluateAndSendDMX(Context)` | Evaluate and send DMX |
| `SetIsRecording(bool)` | Set recording status |
| `GetIsRecording()` | Query recording status |
