# SuperLaser Laser System — User Manual

> **Version**: SuperStage 26Q1
> **Last Updated**: 2026-03-05
> **Engine Version**: Unreal Engine 5.6 – 5.7
> **Target Platform**: Windows 64-bit (depends on Win32 native Socket API and third-party DLLs)
> **Target Audience**: Lighting designers, virtual production operators, technical directors

---

## Table of Contents

1. [System Overview](#1-system-overview)
2. [Prerequisites & Environment Requirements](#2-prerequisites--environment-requirements)
3. [Quick Start (5-Minute Intro)](#3-quick-start-5-minute-intro)
4. [Laser Actor Details](#4-laser-actor-details)
   - 4.1 [SuperLaserActor (Texture Projection Mode)](#41-superlaseractor-texture-projection-mode)
   - 4.2 [SuperLaserProActor (Procedural Mesh Mode)](#42-superlaserproactor-procedural-mesh-mode)
5. [SuperLaserProComponent Parameter Details](#5-superlaserprocomponent-parameter-details)
6. [Network Connection & Beyond Software Integration](#6-network-connection--beyond-software-integration)
7. [Scanner Simulation & Quality Settings](#7-scanner-simulation--quality-settings)
8. [Beam Detection & Rendering Parameters](#8-beam-detection--rendering-parameters)
9. [Sequencer Integration & Laser Playback](#9-sequencer-integration--laser-playback)
10. [Blueprint API & C++ Interface](#10-blueprint-api--c-interface)
11. [Multi-Device Management](#11-multi-device-management)
12. [Performance Optimization Guide](#12-performance-optimization-guide)
13. [FAQ & Troubleshooting](#13-faq--troubleshooting)
14. [Appendix: Parameter Quick Reference](#14-appendix-parameter-quick-reference)

---

## 1. System Overview

SuperLaser is the **laser visualization module** within the SuperStage plugin for receiving and rendering laser point data in real time from **Pangolin Beyond** laser control software within Unreal Engine 5.

### Core Capabilities

| Capability | Description |
|------------|-------------|
| **Real-Time Reception** | Receives laser point data in real time from Beyond software via UDP multicast |
| **Dual-Mode Rendering** | Provides both "Texture Projection" and "Procedural Mesh" rendering modes |
| **Scanner Simulation** | Physics-level laser galvanometer simulation reproducing real laser scanning effects |
| **Multi-Device Support** | Default support for 4 Beyond laser devices (Device 0–3); Actor property DeviceID configurable 1–12; expand via FLaserSettings::MaxLaserDevices |
| **Sequencer Playback** | Stores laser keyframe data in Level Sequences for playback and editing via Sequencer |
| **Editor Real-Time Preview** | Real-time laser preview in editor viewport without running the game |
| **Collision Occlusion** | Pro mode supports collision detection between laser lines and scene objects, auto-truncation when blocked |

### System Architecture Overview

```
Beyond Laser Software
      │
      │ UDP Multicast (239.255.x.x:5568)
      ▼
┌─────────────────────────────────────────────┐
│           SuperLaser Subsystem               │
│  ┌──────────────┐  ┌──────────────────────┐  │
│  │ Network Rcvr │→ │ Data Processor (per  │  │
│  │ (UDP Thread) │  │ device): Interp→Smooth│  │
│  └──────────────┘  │ →Downsample→Beam     │  │
│                    └──────────┬───────────┘  │
│                               │              │
│          ┌────────────────────┼──────────┐   │
│          ▼                    ▼          │   │
│  ┌──────────────┐   ┌──────────────┐    │   │
│  │ Tex Renderer │   │ Point Data   │    │   │
│  │ (RenderTgt)  │   │ Ref (0-copy) │    │   │
│  └──────┬───────┘   └──────┬───────┘    │   │
└─────────┼──────────────────┼────────────┘   │
          ▼                  ▼                │
  SuperLaserActor    SuperLaserProActor       │
  (Texture Proj)     (Procedural Mesh)        │
                                              │
          ┌───────────────────────────────────┘
          ▼
   Sequencer (Keyframe Recording & Playback)
```

---

## 2. Prerequisites & Environment Requirements

### 2.1 Software Requirements

| Software | Version | Purpose |
|----------|---------|---------|
| Unreal Engine | **5.6 – 5.7** | Rendering engine (only these two versions supported) |
| SuperStage Plugin | 26Q1 and above | Laser visualization |
| Pangolin Beyond | Any version | Laser control software |
| OS | **Windows 10/11 64-bit** | Depends on Winsock2, IP_PKTINFO, WSARecvMsg and other Win32 APIs |

> **Platform Limitation**: SuperLaser's network receiving layer is based on Win32 native Socket API (Winsock2) and depends on Windows DLLs, therefore **only supports Windows 64-bit platform**. macOS/Linux are not supported.

### 2.2 DLL Files

SuperLaser requires the following two DLL files to parse Beyond protocol data:

| File Name | Purpose |
|-----------|---------|
| `linetD2_x64.dll` | Beyond protocol data parsing library |
| `matrix64.dll` | Matrix computation dependency library (must be loaded before linetD2) |

**Auto Deployment**: These two DLLs are built into the `Source/SuperLaser/ThirdParty/BeyondLink/bin/` directory and are **automatically copied** to the plugin's binary output directory via `RuntimeDependencies` rules in `SuperLaser.Build.cs`. **No manual copying** needed during normal development and packaging.

If auto-deployment fails, the runtime searches for DLLs in the following order:

1. `Plugins/SuperStage/Binaries/Win64/` (plugin binary directory, located via IPluginManager)
2. `<ProjectDir>/Binaries/Win64/` (packaging fallback)
3. `FPlatformProcess::GetModulesDirectory()` (module directory, last fallback)

> **Important**: If DLL files are missing, network reception can start normally but laser data cannot be parsed. The Output Log will show:
> - `matrix64.dll not found in any search path` — matrix library not found
> - `Failed to load linetD2_x64.dll` — parsing library load failed
> - `Failed to get function pointers from linetD2_x64.dll` — DLL function export anomaly

### 2.3 Network Requirements

| Item | Value |
|------|-------|
| Protocol | UDP Multicast |
| Port | **5568** (Beyond default port) |
| Multicast Address Range | `239.255.0.0` ~ `239.255.{MaxDevices-1}.30` (default MaxDevices=4, i.e. `239.255.0.0` ~ `239.255.3.30`) |
| Multicast Group Count | MaxDevices x 31 (default 4 x 31 = **124 groups**) |
| Firewall | UDP 5568 inbound must be allowed |

**Beyond and UE must be on the same LAN**, and the network switch/router must support multicast forwarding (IGMP).

---

## 3. Quick Start (5-Minute Intro)

The following steps help you see laser effects in the shortest time:

### Step 1: Confirm DLL Files in Place

DLLs are typically auto-deployed by Build.cs. If using for the first time, compile the plugin once, then check if `linetD2_x64.dll` and `matrix64.dll` exist in `Plugins/SuperStage/Binaries/Win64/`.

### Step 2: Start Beyond and Begin Output

1. Open Pangolin Beyond software
2. Select a laser effect/pattern
3. Confirm Beyond is outputting data over the network (default port 5568)

### Step 3: Place Laser Actor in UE

1. Open the UE Editor
2. In the level viewport, click the **Place Actors** panel, or go to **Edit > Place Actor**, search for `SuperLaserProActor` (recommended) or `SuperLaserActor`
3. Place the Actor in the scene
4. In the **Details** panel, set **DeviceID** to `1` (corresponds to Beyond's first device)

### Step 4: View Live Laser

If the network is connected and DLLs are in place, you should immediately see real-time laser effects in the editor viewport. **No need to click Play**; real-time preview is available in editor mode.

> **Not seeing effects?** See [Section 13 FAQ & Troubleshooting](#13-faq--troubleshooting).

---

## 4. Laser Actor Details

SuperStage provides two types of laser Actors for different scenarios:

### 4.1 SuperLaserActor (Texture Projection Mode)

**Class Name**: `ASuperLaserActor` (inherits from `ASuperBaseActor`)

**Working Principle**: Renders laser point data via `FLaserTextureRenderer` to an HDR texture (`UTextureRenderTarget2D`, format `PF_FloatRGBA`), then projects it into the scene via **static mesh + dynamic material**, similar to a projection light effect. Rendering uses **additive blending** (`SE_BLEND_Additive`), supporting HDR color stacking beyond 1.0.

**Applicable Scenarios**:
- Volumetric light effects (laser beams in fog)
- Atmospheric attenuation effects
- Higher rendering quality requirements

#### Parameter List

| Parameter | Display Name | Range | Default | Description |
|-----------|-------------|-------|---------|-------------|
| **DeviceID** | DeviceID | 1–12 | 1 | Beyond device number. Device 1 in Beyond corresponds to 1 here. Internally auto-converted to 0-based index |
| **LaserIntensity** | LaserIntensity | 1–5000 | 100 | Controls maximum laser brightness intensity. Higher values = brighter beams. Recommended range 50–500 |
| **MaxLaserDistance** | MaxLaserDistance | 100–10000 cm | 2345 | Maximum distance the laser can reach (cm). Adjust based on scene size |
| **AtmosphericDensity** | AtmosphericDensity | 0–1 | 0.03 | Atmospheric attenuation coefficient. Controls how much the laser dims with distance. 0 = no attenuation, 1 = fast attenuation |
| **FogIntensity** | BeamFogIntensity | 0–100 | 20 | Fog effect intensity within the laser beam. Higher values = thicker fog |
| **AtmosFogSpeed** | AtmosBeamFogSpeed | 0–100 | 10 | Fog flow speed. Higher values = faster fog drift |

#### Usage Tips

- **Brightness Adjustment**: First adjust `LaserIntensity` to a visible level (usually 100–300), then fine-tune `AtmosphericDensity`
- **Fog Effects**: Best combined with Exponential Height Fog in the scene
- **Visibility**: When `LaserIntensity` is 0, the beam mesh is automatically hidden to save rendering overhead

---

### 4.2 SuperLaserProActor (Procedural Mesh Mode)

**Class Name**: `ASuperLaserProActor` (inherits from `ASuperBaseActor`)

**Working Principle**: Directly reads laser point data and generates a **procedural mesh line from Actor position to target point** for each visible laser point via the built-in `USuperLaserProComponent`, achieving per-line rendering.

**Applicable Scenarios** (Recommended):
- Precise per-line laser rendering
- Collision occlusion detection (laser lines auto-truncate when blocked by objects)
- More flexible material control (Fresnel, depth attenuation, fog, etc.)

#### Actor-Level Parameters

| Parameter | Display Name | Range | Default | Description |
|-----------|-------------|-------|---------|-------------|
| **DeviceID** | DeviceID | 1–12 | 1 | Beyond device number, same rules as SuperLaserActor |
| **bEnableCollision** | EnableCollision | On/Off | Off | Enables collision occlusion detection. When on, laser lines truncate when hitting scene objects |

#### Editor Features

- **Arrow Indicator**: Green arrow displayed in scene pointing in -Z direction (laser projection direction)
- **Icon**: Spotlight icon displayed in editor for easy selection
- **Real-Time Preview**: Auto-updates in editor mode, no Play needed

> **All laser rendering parameters in Pro mode are set on the LaserComponent**, see next section for details.

---

## 5. SuperLaserProComponent Parameter Details

`SuperLaserProComponent` is the core rendering component of SuperLaserProActor. After selecting the Actor, expand the **LaserComponent** node in the Details panel to see all parameters.

### 5.1 Basic Geometry Parameters

| Parameter | Display Name | Range | Default | Description |
|-----------|-------------|-------|---------|-------------|
| **BeamLength** | BeamLength | 100–50000 cm | — | Laser projection distance. Maximum length from Actor position to laser line end (cm). Set based on scene size; e.g., 30000 for stadiums |
| **ProjectionAngle** | ProjectionAngle | 1–90 | — | Laser projection angle range. Controls how large a cone angle the X/Y coordinates [-1, 1] in laser point data map to. Larger angle = wider laser scan range |
| **LaserWidth** | LaserWidth | 0.1–20 cm | — | Laser line width (thickness). Higher values = thicker lines. Real lasers are usually thin (0.5–2.0) |

### 5.2 Material & Visual Effect Parameters

| Parameter | Display Name | Range | Default | Description |
|-----------|-------------|-------|---------|-------------|
| **CoreSharpness** | CoreSharpness | 1–10 | — | Core sharpness. Controls how concentrated the laser line center core is. Higher = brighter center, darker edges, sharper look. Recommended 2–5 |
| **DepthFade** | DepthFade | 0–1 | — | Depth attenuation. Controls how much the far end of the laser line dims. 0 = uniform brightness throughout, 1 = far end completely disappears. Recommended 0.2–0.5 |
| **Dim** | Dim | 1–10 | — | Emissive intensity. Controls overall laser brightness using an exponential curve. 1 = normal brightness, 3 = bright, 5 = very bright, 10 = maximum |
| **OpacityScale** | OpacityScale | 0–1 | — | Overall opacity scale. 0 = fully transparent, 1 = fully opaque. Can be used for fade-in/out effects |
| **ComponentDimmer** | ComponentDimmer | 0–1 | 1.0 | Component-level brightness trim. Multiplicative overlay with Dim: `final brightness = Dim x ComponentDimmer`. For independently adjusting laser component brightness in multi-module fixtures. 0 = off, 1 = full |

### 5.3 Fog Effect Parameters

| Parameter | Display Name | Range | Default | Description |
|-----------|-------------|-------|---------|-------------|
| **FogSpeed** | FogSpeed | 0–2 | — | Fog flow speed. Controls the scrolling speed of the noise texture inside the laser beam. 0 = static, 2 = fast flow |
| **FogInfluence** | FogInfluence | 0–1 | — | Fog influence intensity. Controls how much the fog texture affects the laser line appearance. 0 = no fog (pure laser line), 1 = fully controlled by fog (like real fog laser effects) |

### 5.4 Hotspot Parameters

| Parameter | Display Name | Range | Default | Description |
|-----------|-------------|-------|---------|-------------|
| **SpotDimmer** | SpotDimmer | 0–5 | — | Hotspot brightness. Controls the brightness of the hotspot produced when the laser line hits scene surfaces. 0 = no hotspot, 5 = very bright hotspot |

### 5.5 Collision Parameters

| Parameter | Display Name | Range | Default | Description |
|-----------|-------------|-------|---------|-------------|
| **bEnableCollision** | EnableCollision | On/Off | Off | Collision occlusion detection toggle. When on, each laser line performs ray detection and auto-truncates when hitting objects. **Note**: Enabling collision increases performance overhead; use cautiously with many laser lines |

### 5.6 Parameter Tuning Recommendations

**Realism-Focused** settings:
```
BeamLength      = 15000 (150m)
ProjectionAngle = 30
LaserWidth      = 1.0
CoreSharpness   = 3.0
DepthFade       = 0.3
Dim             = 4.0
OpacityScale    = 0.8
FogSpeed        = 0.3
FogInfluence    = 0.4
SpotDimmer      = 1.5
bEnableCollision = On
```

**Performance-Focused** settings:
```
BeamLength      = 10000 (100m)
ProjectionAngle = 45
LaserWidth      = 0.5
CoreSharpness   = 2.0
DepthFade       = 0.0
Dim             = 3.0
OpacityScale    = 1.0
FogSpeed        = 0.0
FogInfluence    = 0.0
SpotDimmer      = 0.0
bEnableCollision = Off
```

---

## 6. Network Connection & Beyond Software Integration

### 6.1 Beyond-Side Configuration

1. Open Pangolin Beyond software
2. Confirm your laser effect is playing
3. Beyond automatically broadcasts laser data via UDP multicast on port **5568**
4. By default no additional configuration is needed; Beyond's network output is enabled by default

### 6.2 Network Protocol Details

| Item | Value |
|------|-------|
| Transport Protocol | UDP Multicast |
| Default Port | 5568 |
| Multicast Address Format | `239.255.{DeviceID}.{SubnetID}` |
| DeviceID Range | 0 ~ MaxLaserDevices-1 (default 0–3, i.e., 4 devices) |
| SubnetID Range | 0–30 (31 subnets per device) |
| Total Multicast Groups | MaxLaserDevices x 31 (default 4 x 31 = **124** multicast addresses) |

The system automatically joins all multicast groups on startup to receive data. The number of multicast groups depends on `FLaserSettings::MaxLaserDevices` configuration (default 4).

### 6.3 DeviceID Mapping

There is an **offset relationship** between Beyond and UE DeviceIDs:

| Beyond Device Number | DeviceID in UE Actor | Internal System Index |
|:---:|:---:|:---:|
| Device 1 | **1** | 0 |
| Device 2 | **2** | 1 |
| Device 3 | **3** | 2 |
| Device 4 | **4** | 3 |

> In simple terms: **Just fill in the DeviceID property of the Actor with the device number shown in Beyond**; the system internally handles the -1 conversion automatically.

### 6.4 Firewall Settings

If laser data cannot be received, check Windows Firewall:

1. Open **Windows Defender Firewall** > **Advanced Settings**
2. Click **Inbound Rules** > **New Rule**
3. Select **Port** > **UDP** > Enter `5568`
4. Select **Allow the connection**
5. Name it `SuperLaser Beyond`

### 6.5 Multi-NIC Environment

If your computer has multiple NICs (e.g., WiFi + Ethernet), the system defaults to binding to `0.0.0.0` (all NICs). Usually no additional configuration is needed.

---

## 7. Scanner Simulation & Quality Settings

SuperLaser includes a physics-level **laser galvanometer simulator** that reproduces the physical characteristics of real laser scanners. These settings are globally configured via the `FLaserSettings` struct and can be modified through `USuperLaserSubsystem::SetSettings()` in C++ or Blueprints. All active devices share the same set of settings.

### 7.1 Scanner Simulation Parameters

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **bScannerSimulation** | On/Off | On | Master switch. When off, laser points are displayed directly without physical simulation |
| **SampleCount** | 1–16 | 8 | Point interpolation sample count. How many intermediate points are inserted between two adjacent laser points. Higher = smoother lines but more computation |
| **EdgeFade** | 0–1 | 0.1 | Edge fade factor. Simulates the physical phenomenon of real lasers dimming at high-speed corners. 0 = no fade, 1 = strong fade |
| **VelocitySmoothing** | 0–1 | 0.83 | Velocity smoothing factor. Simulates galvanometer inertia. 0 = no inertia (instant response), 1 = maximum inertia (slow tracking). Recommended 0.7–0.9 |

### 7.2 Quality Levels

| Quality Level | Downsample Factor | Description |
|---------------|:---:|-------------|
| **Low** | 8x | Performance priority. Point count reduced to 1/8. Suitable for previewing many devices simultaneously |
| **Medium** | 4x | Balanced mode. Point count reduced to 1/4. Recommended for most scenes |
| **High** | 2x | High quality. Point count reduced to 1/2. Default level |
| **Ultra** | 1x (no downsampling) | Maximum quality. No downsampling. Only use when per-point precision is needed |

### 7.3 Texture Resolution (Texture Projection Mode Only)

| Parameter | Options | Default | Description |
|-----------|---------|---------|-------------|
| **TextureSize** | 256 / 512 / 1024 / 2048 | 512 | Laser texture rendering resolution. Higher = clearer patterns but more VRAM usage |
| **bEnableMipmaps** | On/Off | Off | Whether to generate Mipmaps for the texture. Off improves performance |

---

## 8. Beam Detection & Rendering Parameters

### 8.1 Beam Detection

Beam detection identifies **static beam points** in laser data (e.g., multiple consecutive points at the same location when a laser is aimed in one direction).

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **BeamRepeatThreshold** | 1 | 8 | Beam repeat point threshold. When consecutive points at the same location exceed this count, it is identified as a beam. Lower = more sensitive |
| **BeamIntensityCount** | 1 | 160 | Additional point count generated for high-intensity beams. After detecting a beam, this many points are stacked at that location to enhance brightness |

### 8.2 Rendering Parameters (Texture Projection Mode)

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **LineWidth** | 0.01–10 | 0.1 | Laser point drawing size on texture (pixels). Higher = thicker points |
| **MaxBeamBrush** | 0.1–10 | 2.0 | Maximum beam brush size. Controls the drawing range of beam areas |
| **bEnableBeamBrush** | On/Off | Off | Beam brush toggle. When on, duplicate points at beam locations are removed and drawn with a larger brush, producing more concentrated hotspot effects |

---

## 9. Sequencer Integration & Laser Playback

SuperLaser provides complete **Sequencer track infrastructure** for storing laser keyframe data in Level Sequences and playing back.

### 9.1 Track Architecture

```
Level Sequence
   UMovieSceneSuperLaserTrack ("Super Laser")
       UMovieSceneSuperLaserSection
           DeviceID: 0 (internal index, 0-based)
           Keyframes: TArray<FLaserKeyframe>
                Per frame: Time + TArray<FCompressedLaserPoint>
           bIsRecording: Recording/playback mutual exclusion flag
```

### 9.2 Data Compression

Keyframe data uses `FCompressedLaserPoint` automatic quantization compression:

| Data | Original Precision | Compressed Precision | Size |
|------|-------------------|---------------------|------|
| X/Y Coordinates | float32 (4 bytes) | int16 (2 bytes) | Precision loss < 0.003% |
| R/G/B Color | float32 (4 bytes) | uint8 (1 byte) | Precision loss < 0.4% |
| Focus + Z Flag | float32 x 2 (8 bytes) | uint8 (1 byte) | Focus 7bit, Z boolean |
| **Total Per Point** | **28 bytes** | **8 bytes** | **71.4% compression** |

Recorded files are only about **29%** of the original data size with virtually no visual difference.

### 9.3 Playback Mechanism

1. Open the **Level Sequence** asset containing laser tracks
2. Ensure there is a SuperLaserActor or SuperLaserProActor with matching DeviceID in the scene
3. Press the **Play** button in Sequencer
4. Laser data is automatically distributed to scene laser Actors via `USuperLaserSubsystem::SetDevicePoints()`

**Interpolation Strategy**: During playback, **constant interpolation** (stepped) is used rather than linear interpolation:
- Between two keyframes, the data shown is always from the **previous keyframe** (binary search positioning)
- This matches the physical characteristics of lasers - laser patterns switch instantaneously without "gradient" processes

### 9.4 Recording & Playback Mutual Exclusion

- **Recording** (`bIsRecording = true`): The Section is skipped during Template evaluation to prevent new/old data conflicts
- **Playing**: Live network data is still being received and processed, but Sequencer data overrides the display via `SetDevicePoints`

### 9.5 C++ Recording Interface

```cpp
// Get or create Section
UMovieSceneSuperLaserSection* Section = ...;
Section->SetIsRecording(true);

// Add keyframes per frame (auto-compressed)
Section->AddKeyframe(CurrentTime, LaserPoints);
Section->ExpandToFrame(CurrentFrame);

// Recording complete
Section->SetIsRecording(false);
```

> **Note**: DeviceID in the Section uses **0-based internal indexing** (0 = Beyond Device 1), different from the Actor property's 1-based DeviceID.

---

## 10. Blueprint API & C++ Interface

### 10.1 USuperLaserSubsystem (Engine Subsystem)

`USuperLaserSubsystem` is the laser system's core manager, globally unique as a `UEngineSubsystem`.

```cpp
USuperLaserSubsystem* LaserSys = GEngine->GetEngineSubsystem<USuperLaserSubsystem>();
```

**Main Interfaces**:

| Method | Description |
|--------|-------------|
| `StartReceiving()` | BlueprintCallable. Start network reception (auto-join multicast groups, load DLLs) |
| `StopReceiving()` | BlueprintCallable. Stop network reception and clean up resources |
| `IsReceiving()` | BlueprintPure. Query whether network reception is running |
| `GetDeviceTexture(int32 DeviceID)` | BlueprintCallable. Get RenderTarget for specified device (texture projection mode) |
| `GetDevicePoints(int32 DeviceID)` | BlueprintCallable. Get copy of device point data |
| `GetDevicePointsRef(int32 DeviceID)` | C++ only. Get **const reference** to device point data (zero-copy, high performance) |
| `SetDevicePoints(int32 DeviceID, Points)` | Set device point data (for Sequencer playback) |
| `GetSettings()` / `SetSettings()` | BlueprintCallable. Read/modify global `FLaserSettings` configuration |
| `GetNetworkStats()` | BlueprintPure. Get network reception statistics (`FLaserNetworkStats`) |
| `ClearDeviceResources(int32 DeviceID)` | BlueprintCallable. Release processor and renderer for specified device |
| `ClearAllDeviceResources()` | BlueprintCallable. Release all device resources |

### 10.2 ASuperLaserProActor Blueprint Interfaces

| Method | Category | Description |
|--------|----------|-------------|
| `SetLaserEnabled(bool)` | BlueprintCallable | Set laser visibility toggle |

### 10.3 USuperLaserProComponent Blueprint Interfaces

| Method | Category | Description |
|--------|----------|-------------|
| `SetLaserPoints(TArray<FLaserPoint>)` | BlueprintCallable | Set laser point data |
| `RebuildMesh()` | BlueprintCallable | Force rebuild procedural mesh |
| `SetLaserVisibility(bool)` | BlueprintCallable | Set laser visibility |
| `SetComponentDimmer(float)` | BlueprintCallable | Set component-level brightness trim (multiplicative with Dim) |

---

## 11. Multi-Device Management

### 11.1 Multiple Beyond Devices

SuperLaser defaults to supporting **4 Beyond devices** (internal index 0–3), expandable to up to 12 by modifying `FLaserSettings::MaxLaserDevices`. Each device independently has:
- One data processor
- One texture renderer
- Independent point data buffers

### 11.2 Placing Multiple Laser Actors in a Scene

You can place any number of laser Actors in the scene; they can point to the **same or different** DeviceIDs:

| Scenario | Configuration |
|----------|--------------|
| 4 different laser devices | Place 4 Actors, set DeviceID = 1, 2, 3, 4 respectively (Actor property supports 1–12) |
| Multiple views of the same device | Place multiple Actors, all with the same DeviceID |
| Mixed use of both modes | Same device can be displayed simultaneously with both SuperLaserActor and SuperLaserProActor |

### 11.3 Resource Release

The system auto-creates processor and renderer for each device as needed. If a device is no longer in use, memory can be released via Blueprint calls:

- **`ClearDeviceResources(DeviceID)`**: Release processor and renderer for specified device (BlueprintCallable)
- **`ClearAllDeviceResources()`**: Release all device resources (BlueprintCallable)

---

## 12. Performance Optimization Guide

### 12.1 Performance Impact Factors

| Factor | Impact | Optimization Suggestion |
|--------|:---:|------|
| Quality Level | | Drop from Ultra to Medium for 75% point processing reduction |
| Scanner Simulation | | Turning off bScannerSimulation significantly reduces CPU overhead |
| Collision Detection | | Turning off bEnableCollision avoids per-line ray detection |
| SampleCount | | Drop from 16 to 4 for 75% interpolation reduction |
| Texture Resolution | | Drop from 2048 to 512 for 93% VRAM reduction |
| Fog Effects | | Set FogInfluence to 0 to skip noise sampling |
| Hotspot Effects | | Set SpotDimmer to 0 to simplify shading |

### 12.2 Recommended Configuration

**High-Performance Scenarios** (4+ devices, many laser lines):
- Quality Level: **Low** or **Medium**
- Scanner Simulation: **Off**
- Collision Detection: **Off**
- Texture Resolution: **256** or **512**
- Fog/Hotspot: **Off**

**High-Quality Scenarios** (1–2 devices, final render):
- Quality Level: **High** or **Ultra**
- Scanner Simulation: **On**, SampleCount = 8–16
- Collision Detection: **On**
- Texture Resolution: **1024** or **2048**
- Fog/Hotspot: On as needed

### 12.3 Performance Monitoring

SuperLaser registers the following performance counters in UE Editor's **Stat** system:

- **Stat SuperLaser** > `STAT_SuperLaserTick`: Shows subsystem per-frame processing time

In **Output Log**, information can be filtered by the following log categories:

| Log Category | Module | Description |
|-------------|--------|-------------|
| `LogSuperLaser` | Subsystem/Module Entry | Core system state, initialization, DLL verification |
| `LogLaserNetwork` | Network Receiver | Multicast group join/leave, DLL loading, packet parsing, network stats |
| `LogLaserProcessor` | Data Processor | Scanner simulation, interpolation, downsampling, beam detection |
| `LogLaserRenderer` | Texture Renderer | RenderTarget creation, Canvas drawing, texture clearing |

---

## 13. FAQ & Troubleshooting

### Q1: Placed an Actor but see no laser effects

**Troubleshooting steps**:

1. **Check DLL files**: Look for error messages in Output Log
2. **Check if Beyond is outputting**: Confirm laser effects are playing in Beyond
3. **Check network connection**: Beyond and UE must be on the same LAN
4. **Check firewall**: Confirm UDP port 5568 is allowed
5. **Check DeviceID**: Confirm Actor's DeviceID matches Beyond's device number (Actor set to 1 = Beyond Device 1)
6. **Check brightness parameters**: SuperLaserActor's `LaserIntensity` must not be 0; SuperLaserProActor's `Dim` and `ComponentDimmer` must not be 0

### Q2: Noticeable laser effect lag

**Possible causes and solutions**:
- **Network latency**: Ensure Beyond and UE are on the same machine or gigabit LAN
- **Quality too high**: Lower quality from Ultra to High or Medium
- **Scanner SampleCount too large**: Reduce SampleCount (recommended 4–8)
- **CPU overload**: Check Tick time in Stat SuperLaser

### Q3: Cannot preview in editor mode

1. Confirm the Actor's `ShouldTickIfViewportsOnly` is enabled (default enabled)
2. Confirm the editor viewport's **Realtime** button is on (clock icon in the top-left corner of the viewport)

### Q4: No laser track data in Sequencer

1. Confirm `UMovieSceneSuperLaserTrack` and `UMovieSceneSuperLaserSection` have been correctly created
2. Confirm Section's `DeviceID` corresponds to the scene Actor's device (Section uses 0-based index)
3. Confirm `AddKeyframe()` was called correctly and keyframe data is non-empty

### Q5: Laser not showing during Sequencer playback

1. Confirm there is a SuperLaserActor or SuperLaserProActor in the scene with matching DeviceID
2. Confirm the Level Sequence's laser track contains keyframe data
3. Confirm not in recording mode (recording blocks playback)

### Q6: SuperLaserActor vs. SuperLaserProActor?

| Dimension | SuperLaserActor | SuperLaserProActor |
|-----------|:---:|:---:|
| Rendering | Texture projection | Procedural mesh per-line |
| Volumetric/Fog | | |
| Per-Line Precision | | |
| Collision Occlusion | Not supported | |
| Material Control | | |
| Performance Overhead | Lower | Medium |

**Recommendation**: Use **SuperLaserProActor** for most scenarios; it provides more precise laser rendering and collision occlusion capability.

### Q7: Can multiple UE computers receive from the same Beyond simultaneously?

Yes. Since UDP multicast is used, multiple computers on the same LAN can simultaneously receive laser data from the same Beyond device, enabling multi-machine synchronized preview.

---

## 14. Appendix: Parameter Quick Reference

### A. SuperLaserActor Parameters

| Parameter | Type | Range | Default | Category |
|-----------|------|-------|---------|----------|
| DeviceID | int32 | 1–12 | 1 | SuperLaser |
| LaserIntensity | float | 1–5000 | 100 | SuperLaser |
| MaxLaserDistance | float | 100–10000 cm | 2345 | SuperLaser |
| AtmosphericDensity | float | 0–1 | 0.03 | SuperLaser |
| FogIntensity | float | 0–100 | 20 | SuperLaser |
| AtmosFogSpeed | float | 0–100 | 10 | SuperLaser |

### B. SuperLaserProActor Parameters

| Parameter | Type | Range | Default | Category |
|-----------|------|-------|---------|----------|
| DeviceID | int32 | 1–12 | 1 | A.DefaultParameter |
| bEnableCollision | bool | On/Off | Off | A.DefaultParameter |

### C. SuperLaserProComponent Parameters

| Parameter | Type | Range | Default | Category |
|-----------|------|-------|---------|----------|
| BeamLength | float | 100–50000 cm | — | A.DefaultParameter |
| ProjectionAngle | float | 1–90 | — | A.DefaultParameter |
| LaserWidth | float | 0.1–20 cm | — | A.DefaultParameter |
| CoreSharpness | float | 1–10 | — | A.DefaultParameter |
| DepthFade | float | 0–1 | — | A.DefaultParameter |
| Dim | float | 1–10 | — | A.DefaultParameter |
| OpacityScale | float | 0–1 | — | A.DefaultParameter |
| ComponentDimmer | float | 0–1 | 1.0 | A.DefaultParameter |
| FogSpeed | float | 0–2 | — | A.DefaultParameter |
| FogInfluence | float | 0–1 | — | A.DefaultParameter |
| SpotDimmer | float | 0–5 | — | A.DefaultParameter |
| bEnableCollision | bool | On/Off | Off | A.DefaultParameter |

### D. System Global Settings (FLaserSettings)

| Parameter | Type | Range | Default | Category |
|-----------|------|-------|---------|----------|
| NetworkPort | int32 | — | 5568 | Network |
| MaxLaserDevices | int32 | — | 4 | Network |
| TextureSize | int32 | 256/512/1024/2048 | 512 | Rendering |
| bEnableMipmaps | bool | On/Off | Off | Rendering |
| LineWidth | float | 0.01–10 | 0.1 | Rendering |
| MaxBeamBrush | float | 0.1–10 | 2.0 | Rendering |
| bEnableBeamBrush | bool | On/Off | Off | Rendering |
| bScannerSimulation | bool | On/Off | On | Scanner Simulation |
| SampleCount | int32 | 1–16 | 8 | Scanner Simulation |
| EdgeFade | float | 0–1 | 0.1 | Scanner Simulation |
| VelocitySmoothing | float | 0–1 | 0.83 | Scanner Simulation |
| BeamRepeatThreshold | int32 | 1 | 8 | Beam Detection |
| BeamIntensityCount | int32 | 1 | 160 | Beam Detection |
| LaserQuality | enum | Low/Medium/High/Ultra | High | Quality |
| RecordingDownsample | int32 | 1–10 | 1 | Recording |
| bRemoveBlankPointsInRecording | bool | On/Off | On | Recording |
| bEnableParallelProcessing | bool | On/Off | Off | Performance |

### E. Sequencer Track Types

| Class | Description |
|-------|-------------|
| `UMovieSceneSuperLaserTrack` | Sequencer track, display name "Super Laser" |
| `UMovieSceneSuperLaserSection` | Keyframe data storage, DeviceID (0-based) + TArray<FLaserKeyframe> |
| `FMovieSceneSuperLaserTemplate` | Evaluation template, generates execution Token during playback |

---

> **Technical Support**: For issues not covered in this manual, check the UE Editor's Output Log using the following log category filters: `LogSuperLaser`, `LogLaserNetwork`, `LogLaserProcessor`, `LogLaserRenderer`. They typically contain detailed error causes and solution hints.
