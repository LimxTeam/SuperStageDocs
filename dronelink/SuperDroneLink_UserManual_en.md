# SuperDroneLink User Manual

**Version:** v1.0 | **Last Updated:** 2026-03-04 | **Applicable Version:** SuperStage 26Q1.2+ | **Author:** LimxTeam

---

## Table of Contents

- [Chapter 1 Overview](#chapter-1-overview)
- [Chapter 2 Quick Start](#chapter-2-quick-start)
- [Chapter 3 Network Configuration Details](#chapter-3-network-configuration-details)
- [Chapter 4 Geo Anchor System](#chapter-4-geo-anchor-system)
- [Chapter 5 DroneSwarmManager Formation Manager](#chapter-5-droneswarmmanager-formation-manager)
- [Chapter 6 SuperDroneActor Single Drone](#chapter-6-superdroneactor-single-drone)
- [Chapter 7 Sequencer Timeline Integration](#chapter-7-sequencer-timeline-integration)
- [Chapter 8 LDLink Protocol Description](#chapter-8-ldlink-protocol-description)
- [Chapter 9 Drone States & IDs](#chapter-9-drone-states--ids)
- [Chapter 10 Coordinate Systems & Conversion](#chapter-10-coordinate-systems--conversion)
- [Chapter 11 Blueprint Node Reference](#chapter-11-blueprint-node-reference)
- [Chapter 12 FAQ & Troubleshooting](#chapter-12-faq--troubleshooting)
- [Appendix A Glossary](#appendix-a-glossary)
- [Appendix B Performance Reference Metrics](#appendix-b-performance-reference-metrics)

---

# Chapter 1 Overview

## 1.1 What is SuperDroneLink

**SuperDroneLink** is the drone formation real-time visualization module within the SuperStage plugin. It receives real-time data from drone formation design software (such as LimxDroneStudio) and renders thousands of drones' positions, orientations, and LED light colors in three dimensions within Unreal Engine, achieving a "digital twin" previz of drone shows.

**In simple terms:** The formation design software handles "choreography," and SuperDroneLink handles "real-time 3D preview."

### Core Capabilities

| Capability | Description |
|------------|-------------|
| Real-Time Data Reception | Receives LDLink protocol data via UDP network with extremely low latency |
| Large-Scale Rendering | Supports 10,000+ drones displayed simultaneously, maintaining 60FPS+ |
| LED Light Synchronization | Real-time synchronization of RGB color and brightness for each drone |
| Smooth Position Interpolation | Smooth visuals even with network data at 10-25Hz |
| GPS Coordinate Conversion | Automatically converts GPS latitude/longitude to UE 3D coordinates |
| Sequencer Integration | Record and playback drone trajectories in the timeline |
| Blueprint Support | All core functionality callable from Blueprints |

## 1.2 Application Scenarios

- **Drone Show Previs:** Send formation paths to SuperStage via LDLink protocol for real-time preview of flight trajectories and lighting effects
- **Lighting + Drone Joint Programming:** Simultaneously display stage lighting and drone formations, reviewing the overall visual effect in a unified 3D environment
- **Proposal Review Presentations:** Play back drone show effects to clients in real time with free camera rotation
- **Sequencer Offline Programming:** Directly edit drone positions, orientations, and LED curves within the UE timeline

## 1.3 System Requirements

| Item | Requirement |
|------|-------------|
| Unreal Engine | 5.3 or higher |
| SuperStage Plugin | 26Q1.2 or higher |
| Operating System | Windows 10/11 (64-bit) |
| Network | UDP port 14555 available (configurable) |
| GPU | GTX 1060+ recommended for 1,000 drones; RTX 3060+ for 10,000 drones |
| External Software (Optional) | LimxDroneStudio or other ground station supporting LDLink protocol |

## 1.4 Core Concepts

### DroneLink Subsystem
The "brain" of the entire module, automatically runs on UE engine startup as a global singleton. Responsible for listening on network ports, managing drone states, and providing data query interfaces. **No manual creation needed.**

### Formation Manager (DroneSwarmManager)
An Actor placed in the scene that uses HISM technology to render drone formations with high performance. **Suitable for large formations of 100-10,000+ drones.**

### Single Drone Actor (SuperDroneActor)
An independent Actor bound to a specific Drone ID that follows its movement. **Suitable for a small number of drones that need mounted cameras, effects, etc.**

### Geo Anchor (GeoAnchor)
The conversion origin between GPS and UE coordinates. The GPS position of the first connected drone automatically becomes the anchor (UE origin 0,0,0).

### LDLink Protocol
A proprietary communication protocol that transmits drone position and LED data via UDP.

### DroneId
The unique identifier for each drone (0-65535).

---

# Chapter 2 Quick Start

## 2.1 Five-Minute Quick Start

### Step 1: Confirm Plugin is Enabled
Edit > Plugins, search for SuperStage, confirm it is enabled and restart the editor. SuperDroneLink loads automatically as a submodule.

### Step 2: Place Formation Manager
Search for `DroneSwarmManager` in the Super Browser panel and drag it into the level.

### Step 3: Configure (Optional)
Select the DroneSwarmManager and adjust in the Details panel:
- **Auto Sync** = On (default)
- **Drone Scale** = 50 (default, controls display size)
- **Interp Speed** = 0.9 (default, controls smoothness)

### Step 4: Configure Output in LimxDroneStudio
- Target IP: The IP of the computer running UE (use `127.0.0.1` for local)
- Target Port: `14555`
- Protocol: LDLink
- Click to start output

### Step 5: View Results
Play the formation animation in LimxDroneStudio, and drones will immediately appear and move in the UE editor.

> **Important:** There's no need to press UE's Play button; real-time display is available in editor mode.

## 2.2 Connection Methods

| Method | Target IP Setting |
|--------|-------------------|
| Same Computer | `127.0.0.1` |
| Two Computers on LAN | UE computer's LAN IP |
| Cross-Subnet | UDP 14555 firewall rule must be opened |

---

# Chapter 3 Network Configuration Details

Default configuration works for most scenarios; adjustments may be needed in special network environments.

## 3.1 Configuration Parameters Overview

| Parameter | Category | Default | Range | Description |
|-----------|----------|---------|-------|-------------|
| Port | Network | 14555 | 1-65535 | UDP listening port |
| BindIP | Network | Empty (all interfaces) | Valid IP | Local binding IP |
| bEnableInput | Network | Off | On/Off | Enable data reception (reserved) |
| bEnableOutput | Network | Off | On/Off | Enable data sending (reserved) |
| ProtocolVersion | Protocol | MAVLink 2.0 | V1/V2/None | Protocol version (reserved) |
| HeartbeatTimeoutSeconds | Protocol | 5.0 | 1.0-30.0 | Heartbeat timeout (seconds) |
| PositionInterpSpeed | Rendering | 0.8 | 0.0-1.0 | Position interpolation smoothing coefficient |

## 3.2 Listening Port (Port)

Default 14555. Modify when: port is occupied, different from external software port, or firewall restrictions.

Modify via the Blueprint `ApplyConfig` node; the receiver will automatically restart. Use port numbers above 1024.

## 3.3 Binding IP (BindIP)

| Scenario | Setting | Description |
|----------|---------|-------------|
| General (Recommended) | Leave empty | Accept data from all NICs |
| Multi-NIC Computer | `192.168.1.100` | Only accept data from specified NIC |
| Local Debug Only | `127.0.0.1` | Only accept local data |

Filling in a non-existent IP will cause receiver startup failure.

## 3.4 Protocol Version (ProtocolVersion)

The current version primarily uses the LDLink protocol; this parameter is reserved for future MAVLink native support. Keep default.

## 3.5 Heartbeat Timeout (HeartbeatTimeoutSeconds)

Drones without any data within this time are considered offline and automatically removed.

| Network Environment | Recommended Value |
|---------------------|-------------------|
| Local/LAN | 3.0 - 5.0 |
| Cross-Subnet/WiFi | 8.0 - 15.0 |
| Large Live Event | 10.0 - 20.0 |

After timeout, drones are removed from the state table; receiving data again automatically re-adds them.

## 3.6 Position Interpolation Smoothing (PositionInterpSpeed)

| Value | Effect |
|-------|--------|
| 0.0 | No smoothing, directly jumps to latest position |
| 0.5 | Medium smoothing |
| 0.8 (default) | High smoothing, suitable for most previews |
| 1.0 | Maximum smoothing (may feel laggy) |

DroneSwarmManager and SuperDroneActor each have their own interpolation parameters; effects are cumulative.

## 3.7 Input/Output Toggles

bEnableInput and bEnableOutput are reserved for future expansion; no modification needed in current version. LDLink reception is independent of these two toggles.

## 3.8 Apply Configuration

The subsystem **automatically listens** on port 14555 when the engine starts; usually no additional action is needed.

If modification is needed: In Blueprints, get DroneLinkSubsystem > Create FDroneLinkConfig struct > Call ApplyConfig. After calling, the receiver briefly restarts (approximately 100ms interruption).

---

# Chapter 4 Geo Anchor System

## 4.1 What is a Geo Anchor

The "bridge" between GPS coordinates and UE coordinates. Specifies a GPS coordinate as the UE origin (0,0,0), and all drone GPS coordinates are converted to relative offsets (centimeters).

**Example:** Anchor set to latitude 39.9042, longitude 116.4074, altitude 50m. A drone 100m north, 50m east, and 200m high from the anchor has UE coordinates approximately X=10000, Y=5000, Z=15000 (centimeters).

## 4.2 Auto Anchor

Default behavior: The first valid GPS coordinate received is automatically set as the anchor. Conditions: anchor not set + latitude non-zero + longitude non-zero.

If sending **local coordinates** (non-GPS), auto-anchor is not triggered; coordinates are used directly.

## 4.3 Manual Anchor Setting

Blueprint call `SetGeoAnchor`, passing:
- **Latitude:** degrees, e.g. 39.9042
- **Longitude:** degrees, e.g. 116.4074
- **Altitude:** meters (MSL), e.g. 50.0

After setting, all connected drone coordinates are immediately recalculated.

## 4.4 Use First Drone as Anchor

Blueprint call `SetGeoAnchorFromFirstDrone`, automatically uses the GPS position of the drone with the smallest DroneId as anchor.

## 4.5 Reset Anchor

Blueprint call `ResetGeoAnchor`, clears the anchor. The next GPS data will re-trigger automatic setting.

## 4.6 Anchor Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| Latitude | double | WGS84 latitude, degrees. North positive, south negative. -90~+90 |
| Longitude | double | WGS84 longitude, degrees. East positive, west negative. -180~+180 |
| Altitude | double | MSL altitude, meters |
| bIsSet | bool (read-only) | Whether the anchor has been set |

---

# Chapter 5 DroneSwarmManager Formation Manager

## 5.1 Function Overview

Uses HISM (Hierarchical Instanced Static Mesh) technology for large-scale drone formation rendering. **Preferred for formations of 100+ drones.**

Features: Single DrawCall, GPU-driven colors, smooth interpolation, dirty-check optimization, editor real-time preview.

## 5.2 Placing in Scene

Search for `DroneSwarmManager` in the Super Browser panel and drag into the viewport.

Placement position significance:
- LDLink local coordinates: Drone coordinates = manager position + received offset
- GPS coordinates: Converted by anchor, independent of manager position

## 5.3 Configuration Properties Details

### Auto Sync
- Category: DroneSwarm > Config
- Default: On
- Description: When on, automatically syncs with DroneLinkSubsystem. When off, manually call StartSync via Blueprint.

### Drone Scale
- Category: DroneSwarm > Config
- Default: 50.0
- Description: Display size. Value divided by 100 is the scale factor.

| Value | Scale | Suitable For |
|-------|-------|--------------|
| 10 | 0.1x | Long-distance overview |
| 25 | 0.25x | Medium distance |
| 50 (default) | 0.5x | Regular preview |
| 100 | 1.0x | Close inspection |
| 200 | 2.0x | Close-up presentation |

### Interp Speed
- Category: DroneSwarm > Config
- Default: 0.9
- Range: 0.0 - 1.0
- Description: Position smooth transition speed. Internally updates at 30fps.

| Value | Effect |
|-------|--------|
| 0.0 | No smoothing, directly jump |
| 0.3 | Light smoothing |
| 0.9 (default) | High smoothing |
| 1.0 | Maximum smoothing |

### Drone Timeout Seconds
- Category: DroneSwarm > Config
- Default: 5.0
- Range: 1.0 - 60.0
- Description: Hides render instance after data timeout

### Preallocated Instance Count
- Category: DroneSwarm > Config
- Default: 1000
- Range: 0 - 10000
- Description: Pre-created render instances to avoid first-appearance stutter

| Expected Count | Recommended Value |
|---------------|-------------------|
| 1-100 | 100 |
| 100-500 | 500 |
| 500-2000 | 1000 |
| 2000-5000 | 3000 |
| 5000-10000 | 5000 |

## 5.4 Visual Properties

### Drone Mesh
- Category: DroneSwarm > Visual
- Default: Engine built-in sphere
- Recommendation: Use low-poly models (< 100 triangles) for large formations

### Drone Material
- Category: DroneSwarm > Visual
- Default: MI_DroneLed_Inst (built-in LED material)
- Requirement: Must support PerInstanceCustomData

CustomData channel assignment:

| Channel | Data | Range |
|---------|------|-------|
| [0] | Red | 0-1 |
| [1] | Green | 0-1 |
| [2] | Blue | 0-1 |
| [3] | Brightness | 0-1 |

Custom materials must read these 4 channels for color output.

## 5.5 Sync Mechanism

### Editor Mode
On construction: Clear old instances > Connect subsystem > Subscribe events > Sync existing drones.
Per frame (30fps): Get state > Interpolate > Dirty check > Batch GPU update.

### PIE Mode
BeginPlay: Re-acquire subsystem > Re-subscribe events > Refresh instances. Tick behavior same as above.

## 5.6 Blueprint Control Nodes

| Node | Function |
|------|----------|
| Start Sync | Start synchronization |
| Stop Sync | Stop synchronization |
| Refresh All Instances | Force rebuild all instances |
| Get Drone Count | Get managed drone count |
| Get Drone World Location | Get world coordinates of specified drone |

## 5.7 Performance Optimization

- **30fps Update Rate:** Not per-frame; visually imperceptible difference
- **Dirty Detection:** Position threshold 0.5cm, color threshold 0.01; stationary drones incur zero overhead
- **Instance Pool:** Removed instances placed in pool for reuse, avoiding frequent memory allocation
- **Disabled Non-Essential Features:** Collision, shadows, decals, distance culling, density scaling all disabled

---

# Chapter 6 SuperDroneActor Single Drone

## 6.1 Function Overview

An independent Actor that syncs with subsystem data via DroneId, automatically following position, orientation, and LED color. **Suitable for a small number of drones needing mounted components or Blueprint interaction.**

## 6.2 Comparison with DroneSwarmManager

| Feature | DroneSwarmManager | SuperDroneActor |
|---------|-------------------|-----------------|
| Rendering Method | HISM batch instancing | Independent Actor |
| Suitable Count | 100-10000+ | 1-50 |
| DrawCall | 1 | 1 per drone |
| Mount Child Components | Not supported | Supported |
| Blueprint Interaction | Limited | Full support |
| Material | PerInstanceCustomData | Dynamic material instance |

Selection advice: Use SwarmManager for formation preview, DroneActor for mounted cameras/effects; they can be mixed.

## 6.3 Placement & Setup

Search `SuperDroneActor` (display name Super Drone Actor), drag into viewport, set Drone Id. Each Actor tracks only one drone.

## 6.4 Basic Properties

### Drone Id
- Category: SuperDroneLink
- Default: 0
- Range: 0-65535
- Must match the ID sent by external software

### Auto Sync
- Category: SuperDroneLink
- Default: On
- Automatically starts data sync

## 6.5 Sync Options (SuperDroneLink > Sync)

### Sync Position
- Default: On
- When on, Actor follows drone position movement

### Sync Rotation
- Default: On
- When on, Actor reflects drone Roll/Pitch/Yaw attitude

### Sync LED Color
- Default: On
- When on, material color follows drone LED changes

### Position Interp Speed
- Default: 15.0
- Range: >=0
- Set to 0 for instant movement to target position
- Larger values mean faster movement; default 15 is moderate smoothness

### Rotation Interp Speed
- Default: 10.0
- Range: >=0
- Set to 0 for instant rotation; default 10 is moderate smoothness

## 6.6 Material Parameters (SuperDroneLink > Material)

### LED Color Parameter Name
- Default: `LedColor`
- Vector Parameter name in the material that receives LED color

### LED Brightness Parameter Name
- Default: `LedBrightness`
- Scalar Parameter name in the material that receives LED brightness (0-1)

Custom materials must include parameter nodes with corresponding names, or modify these two properties to match your material.

## 6.7 Mounting Child Components

- **Camera Follow-Shot:** Add Component > Camera, adjust position and angle, use with Sequencer Camera Cut
- **Point Light:** Add Component > Point Light, bind to Get Led Color for environmental lighting
- **Particle Effects:** Add Component > Niagara System, add trail flame, trajectory effects, etc.

## 6.8 Blueprint Nodes

| Node | Type | Description |
|------|------|-------------|
| Get Drone Id | Pure | Returns the bound drone number |
| Set Drone Id | Callable | Modifies the number at runtime (0-65535) |
| Get Led Color | Pure | Returns current LED color |
| Start Sync | Callable | Start synchronization |
| Stop Sync | Callable | Stop synchronization |

---

# Chapter 7 Sequencer Timeline Integration

## 7.1 Feature Overview

Deep integration with UE Sequencer: Record real-time data as keyframes, playback trajectories, manually edit curves, multi-drone programming.

## 7.2 Add DroneLink Track

Open Sequencer > Click + Track > Select DroneLink Track. One track can contain multiple drone sections.

## 7.3 Add Drone Section

Right-click track > Add Section > DroneLink Section, set Drone Id (0-65535). No duplicate DroneId sections in the same track; each section is automatically placed on a different row to avoid overlap.

## 7.4 Curve Channels (10 curves per section)

### Position Curves (3)

| Channel | Unit | Description |
|---------|------|-------------|
| Position X | cm | UE X-axis (North) |
| Position Y | cm | UE Y-axis (East) |
| Position Z | cm | UE Z-axis (Up) |

### Attitude Curves (3)

| Channel | Unit | Range |
|---------|------|-------|
| Roll | degrees | -180 ~ 180 |
| Pitch | degrees | -180 ~ 180 |
| Yaw | degrees | 0 ~ 360 (North = 0) |

### LED Curves (4)

| Channel | Range | Description |
|---------|-------|-------------|
| LED R | 0-255 | Red |
| LED G | 0-255 | Green |
| LED B | 0-255 | Blue |
| LED Brightness | 0-255 | Brightness |

Editing: Double-click section to enter curve editor, right-click Add Key to add keyframes; linear/smooth/stepped interpolation available.

## 7.5 Recording Mode

Section's **Is Recording** flag:
- **true (recording):** Only records data, does not play back. Prevents recording and playback from interfering with each other
- **false (normal):** Updates drone state based on curves during playback

Recording workflow: Set Is Recording = true > Receive real-time data > Write curve keyframes > Set Is Recording = false > Playback and review

## 7.6 Playback Behavior

During Sequencer playback, non-recording sections will: Evaluate all curves > Write position/attitude/LED to subsystem.

- Curve data **overrides** live received data during Sequencer playback
- Live data works normally when Sequencer stops
- If the same drone has two data sources simultaneously, the last write takes precedence

## 7.7 Multi-Drone Programming

Add independent sections for each drone in one track, each with different DroneId; all sections evaluate simultaneously during playback.

> For 100+ drones, it's recommended to batch-generate curve data via external tools for import rather than manually editing drone by drone.

---

# Chapter 8 LDLink Protocol Description

## 8.1 Protocol Overview

LDLink (Limx Drone Link) is a protocol specifically designed for large-scale formation data transmission.

| Feature | Value |
|---------|-------|
| Transport | UDP |
| Default Port | 14555 |
| Byte Order | Little-endian |
| Magic Number | `LDLK` (4-byte ASCII) |
| Version | 1.0 |

## 8.2 Packet Structure

Header (12 bytes) + Payload (Length bytes)

| Field | Offset | Size | Description |
|-------|--------|------|-------------|
| Magic | 0 | 4B | ASCII `LDLK` |
| Version | 4 | 1B | Protocol version |
| OpCode | 5 | 1B | Operation code |
| Universe | 6 | 2B | Universe number (little-endian) |
| Sequence | 8 | 1B | Sequence number (0-255 cyclic) |
| Length | 9 | 2B | Payload length (little-endian) |
| Reserved | 11 | 1B | Reserved |

## 8.3 Operation Codes

| Hex | Name | Description |
|-----|------|-------------|
| 0x10 | LedData | LED color |
| 0x20 | PositionData | Position (local coordinates) |
| 0x30 | StateData | Position + LED (recommended) |
| 0x40 | Sync | Frame sync marker |
| 0x50 | Poll | Poll request |
| 0x61 | PollReply | Poll reply |
| 0x70 | Command | Control command |

## 8.4 LED Data (0x10)

### Universe Mode (Universe > 0) — 5 bytes per drone

| Offset | Size | Description |
|--------|------|-------------|
| 0 | 1B | Channel (0-255) |
| 1 | 1B | R (0-255) |
| 2 | 1B | G (0-255) |
| 3 | 1B | B (0-255) |
| 4 | 1B | Brightness (0-255) |

DroneId = Universe x 256 + Channel

### Global Mode (Universe = 0) — 6 bytes per drone

| Offset | Size | Description |
|--------|------|-------------|
| 0 | 2B | DroneId (little-endian) |
| 2 | 1B | R |
| 3 | 1B | G |
| 4 | 1B | B |
| 5 | 1B | Brightness |

## 8.5 Position Data (0x20) — 14 bytes per drone

| Offset | Size | Description |
|--------|------|-------------|
| 0 | 2B | DroneId (little-endian) |
| 2 | 4B | X (float, meters) |
| 6 | 4B | Y (float, meters) |
| 10 | 4B | Z (float, meters) |

Coordinate unit is meters; internally auto-converted to centimeters (x100). Always uses global mode.

## 8.6 Full State (0x30)

### Universe Mode (Universe > 0) — 17 bytes per drone

| Offset | Size | Description |
|--------|------|-------------|
| 0 | 1B | Channel |
| 1 | 4B | X (meters) |
| 5 | 4B | Y (meters) |
| 9 | 4B | Z (meters) |
| 13 | 4B | Color (RGBA packed, little-endian: R,G,B,Brightness) |

### Global Mode (Universe = 0) — 18 bytes per drone

| Offset | Size | Description |
|--------|------|-------------|
| 0 | 2B | DroneId (little-endian) |
| 2 | 4B | X (meters) |
| 6 | 4B | Y (meters) |
| 10 | 4B | Z (meters) |
| 14 | 4B | Color (RGBA packed) |

## 8.7 Universe Addressing

A Universe concept similar to DMX, grouping large numbers of drones:

| Universe | DroneId Range |
|----------|--------------|
| 0 | Global mode |
| 1 | 256 - 511 |
| 2 | 512 - 767 |
| ... | ... |
| 255 | 65280 - 65535 |

Formula: DroneId = Universe x 256 + Channel

Determination logic: Universe > 0 uses Universe mode; Universe = 0 auto-detects mode based on whether data length is divisible by entry size.

## 8.8 Integration with LimxDroneStudio

LimxDroneStudio settings:
1. Target address: UE computer IP
2. Target port: 14555
3. Protocol: LDLink
4. Data content: Recommended "Full State" (StateData 0x30)
5. Send frequency: Recommended 25-30Hz

### Bandwidth Estimation (StateData Global Mode, 25Hz)

| Drone Count | Per Frame Size | 25Hz Bandwidth |
|-------------|---------------|----------------|
| 100 | 1.8 KB | 45 KB/s |
| 1000 | 18 KB | 450 KB/s |
| 5000 | 90 KB | 2.2 MB/s |
| 10000 | 180 KB | 4.5 MB/s |

---

# Chapter 9 Drone States & IDs

## 9.1 Connection States

| State | Description |
|-------|-------------|
| Disconnected | Never received data, or has been cleaned up |
| Connecting | Just created, hasn't received complete data yet |
| Connected | Receiving data normally |
| Lost | Timeout (directly removed in current version) |

Flow: Non-existent > Data received > Connecting > More data received > Connected > Timeout > Removed (back to non-existent)

## 9.2 DroneId Numbering Rules

Range 0-65535, supporting up to 65536 drones.

### Extended Addressing Formula

`DroneId = (ComponentId - 1) x 256 + SystemId`

| Numbering Method | DroneId Range | Description |
|------------------|--------------|-------------|
| LDLink Direct | 0-65535 | Use DroneId directly |
| MAVLink Standard | 0-255 | ComponentId=1, DroneId=SystemId |
| MAVLink Extended | 0-65535 | Extended via ComponentId |

Reverse: SystemId = DroneId % 256, ComponentId = DroneId / 256 + 1

## 9.3 Position Data

| Field | Description |
|-------|-------------|
| Latitude/Longitude/Altitude | Raw GPS data (degrees/meters) |
| RelativeAltitude | Relative altitude from takeoff point (meters) |
| LocalPosition | Current displayed position (cm, interpolated) |
| TargetPosition | Latest target position (cm) |

UE coordinates: X=North, Y=East, Z=Up

## 9.4 Attitude Data

| Field | Range | Description |
|-------|-------|-------------|
| Roll | -180~180 degrees | Roll angle |
| Pitch | -180~180 degrees | Pitch angle |
| Yaw | 0~360 degrees | Yaw angle (North = 0) |
| LocalRotation | FRotator | Converted UE rotation |

## 9.5 LED Data

| Field | Range | Description |
|-------|-------|-------------|
| LedColor | Each component 0-1 | RGB color (normalized from 0-255) |
| LedBrightness | 0-1 | Brightness (normalized from 0-255) |

## 9.6 Statistics (GetStats)

| Field | Description |
|-------|-------------|
| ConnectedDrones | Number of connected drones |
| PacketsPerSecond | Packets received per second |
| BytesPerSecond | Bytes received per second |
| ParseErrors | Number of parse errors |
| LostPacketCount | Number of lost packets |
| PacketLossRate | Packet loss rate (0-100%) |
| AverageLatencyMs | Average latency (milliseconds) |

---

# Chapter 10 Coordinate Systems & Conversion

## 10.1 Two Coordinate Systems

| Coordinate System | Axis Definition | Unit |
|-------------------|----------------|------|
| WGS84 (GPS) | Longitude/Latitude/Altitude | degrees/meters |
| UE Local | X=North, Y=East, Z=Up | centimeters |

## 10.2 Conversion Principle

1. **Set Anchor:** Choose a GPS coordinate as UE origin
2. **Calculate Offset:** dLat/dLon converted to radians
3. **Ellipsoid Projection:** Calculate meter-level offset using WGS84 parameters
   - North = dLat x Meridian radius of curvature (M)
   - East = dLon x Prime vertical radius of curvature (N) x cos(anchor latitude)
   - Height = Target altitude - Anchor altitude
4. **Convert to cm:** x100

WGS84 parameters: Semi-major axis 6,378,137m, flattening 1/298.257. The same latitude/longitude difference corresponds to different actual distances at different latitudes; SuperDroneLink uses precise ellipsoid parameters rather than simple linear approximation.

## 10.3 Unit Summary

| Data Type | External Input | Internal Storage | Notes |
|-----------|---------------|-----------------|-------|
| GPS Lat/Lon | degrees | degrees | Stored directly |
| GPS Altitude | meters | meters | Stored directly |
| LDLink Position | meters | centimeters | x100 on reception |
| UE Coordinates | centimeters | centimeters | Engine standard |
| MAVLink Attitude | radians | degrees | Converted on reception |
| Sequencer Attitude | degrees | degrees | Stored directly |
| LED Color/Brightness | 0-255 | 0-1 | Normalized on reception |

---

# Chapter 11 Blueprint Node Reference

## 11.1 DroneLinkSubsystem Nodes

### Configuration

| Node | Type | Input | Output | Description |
|------|------|-------|--------|-------------|
| Apply Config | Callable | FDroneLinkConfig | None | Apply config and restart receiver |
| Get Config | Pure | None | FDroneLinkConfig | Get current config |

### Anchor

| Node | Type | Input | Description |
|------|------|-------|-------------|
| Set Geo Anchor | Callable | Lat, Lon, Alt | Manually set anchor |
| Set Geo Anchor From First Drone | Callable | None | Use first drone position as anchor |
| Get Geo Anchor | Pure | None | Get current anchor |
| Reset Geo Anchor | Callable | None | Reset anchor |

### Query

| Node | Type | Input | Output | Description |
|------|------|-------|--------|-------------|
| Get All Drone Ids | Callable | None | TArray uint16 | All drone IDs (sorted) |
| Get Drone State | Callable | DroneId | FDroneState, bool | Get specified drone state |
| Get All Drone States | Callable | None | TArray FDroneState | All drone states |
| Get Connected Drone Count | Pure | None | int32 | Number of connected drones |
| Get Stats | Pure | None | FDroneLinkStats | Statistics |

### Coordinate Conversion

| Node | Input | Output | Description |
|------|-------|--------|-------------|
| Convert Geo To Local | Lat, Lon, Alt | FVector | GPS to UE coordinates (cm) |

## 11.2 DroneSwarmManager Nodes

| Node | Type | Description |
|------|------|-------------|
| Start Sync | Callable | Start synchronization |
| Stop Sync | Callable | Stop synchronization |
| Refresh All Instances | Callable | Force rebuild all instances |
| Get Drone Count | Pure | Current managed count |
| Get Drone World Location | Pure | World coordinates of specified drone |

## 11.3 SuperDroneActor Nodes

| Node | Type | Description |
|------|------|-------------|
| Get Drone Id | Pure | Get bound drone number |
| Set Drone Id | Callable | Modify bound drone number |
| Get Led Color | Pure | Get current LED color |
| Start Sync | Callable | Start synchronization |
| Stop Sync | Callable | Stop synchronization |

---

# Chapter 12 FAQ & Troubleshooting

## 12.1 Drones Not Displaying

**Checklist:**
1. Confirm SuperStage plugin is enabled and editor has been restarted
2. Confirm DroneSwarmManager is placed in the scene and Auto Sync is on
3. Confirm LimxDroneStudio target IP and port are correct (default 14555)
4. Check if Windows Firewall has allowed UDP port 14555
5. Confirm LimxDroneStudio has started outputting data
6. Try increasing Drone Scale (default 50 may be invisible in large scenes)

## 12.2 Severe Drone Position Offset

**Possible causes and solutions:**
1. **Anchor setting error:** Call ResetGeoAnchor to reset and let system auto-set
2. **Coordinate system mismatch:** Confirm external software is outputting correct coordinate format (GPS or local coordinates)
3. **Unit error:** LDLink position data unit is meters; system internally auto-converts to cm. Confirm sender unit is correct
4. **DroneSwarmManager position:** When using local coordinates, the manager's world position is the offset origin

## 12.3 Drone Flickering or Jittering

**Possible causes and solutions:**
1. **Severe network packet loss:** Increase HeartbeatTimeoutSeconds (e.g., 10-15 seconds)
2. **Data frequency too low:** Increase LimxDroneStudio send frequency (recommended 25Hz+)
3. **Insufficient interpolation:** Increase Interp Speed (DroneSwarmManager or SuperDroneActor interpolation speed)
4. **Drones repeatedly timeout and reconnect:** Increase timeout duration

## 12.4 LED Color Not Changing

**Checklist:**
1. Confirm external software is sending LED data (OpCode 0x10 or 0x30)
2. DroneSwarmManager: Confirm material supports PerInstanceCustomData and channel assignment is correct
3. SuperDroneActor: Confirm Sync LED Color is on
4. SuperDroneActor: Confirm material has parameters named `LedColor` (Vector) and `LedBrightness` (Scalar)
5. Test with default built-in material MI_DroneLed_Inst to check if it's a custom material issue

## 12.5 Performance Issues

**Optimization suggestions:**
1. Use DroneSwarmManager (single DrawCall) rather than many SuperDroneActors
2. Use low-poly models (default sphere is optimal)
3. Reasonably set Preallocated Instance Count to avoid runtime dynamic creation
4. Confirm Drone Scale isn't too large, causing excessive polygon rendering
5. Reduce other high-overhead rendering elements in the viewport

## 12.6 Network Connection Issues

**Troubleshooting steps:**
1. Run `netstat -an | findstr 14555` on the UE computer to confirm port is listening
2. From LimxDroneStudio computer, `ping` the UE computer to confirm network reachability
3. Check firewall settings on both ends
4. Try leaving BindIP empty (accept all NICs)
5. Check if subnet masks of the two computers match
6. If using WiFi, try switching to wired connection

---

# Appendix A Glossary

| Term | Description |
|------|-------------|
| **DroneId** | Unique drone identifier, 0-65535 |
| **DroneLink Subsystem** | Engine-level subsystem, global singleton, manages all drone data |
| **DroneSwarmManager** | Formation manager Actor, HISM batch rendering |
| **SuperDroneActor** | Single drone Actor, independent tracking |
| **GeoAnchor** | Geo anchor, GPS to UE coordinate conversion origin |
| **HISM** | Hierarchical Instanced Static Mesh |
| **LDLink** | Limx Drone Link protocol |
| **MAVLink** | Micro Air Vehicle Link, standard drone communication protocol |
| **WGS84** | World Geodetic System 1984, GPS coordinate system standard |
| **MSL** | Mean Sea Level |
| **Universe** | Grouping concept similar to DMX Universe, 256 drones per group |
| **Channel** | Channel number within a Universe, 0-255 |
| **PerInstanceCustomData** | UE instanced rendering per-instance data passing mechanism |
| **PIE** | Play In Editor |
| **DrawCall** | GPU draw call; fewer is better for performance |
| **Interpolation** | Computing intermediate values between two known values for smooth transitions |
| **Heartbeat** | Periodic liveness signal for detecting connection status |
| **Dirty Check** | Checking if data has changed; only update changed portions |

---

# Appendix B Performance Reference Metrics

The following data is based on RTX 3060 GPU, i7-12700 CPU test environment, for reference only:

| Metric | 1000 Drones | 5000 Drones | 10000 Drones |
|--------|-------------|-------------|--------------|
| FPS (DroneSwarmManager) | 120+ | 90+ | 60+ |
| DrawCall | 1 | 1 | 1 |
| CPU Per-Frame Cost | <0.5ms | <1ms | <2ms |
| GPU VRAM Increment | ~10MB | ~40MB | ~80MB |
| Network Bandwidth (25Hz StateData) | 45 KB/s | 2.2 MB/s | 4.5 MB/s |
| Recommended Send Frequency | 25-30Hz | 20-25Hz | 15-20Hz |

### Performance Optimization Priority

1. **Prefer DroneSwarmManager** over many SuperDroneActors
2. **Use low-poly models** (default sphere < 100 faces is optimal)
3. **Reasonable pre-allocation** of Preallocated Instance Count
4. **Enable dirty detection** (enabled by default, no extra action needed)
5. **Control send frequency** for balance between bandwidth and smoothness

---

*This manual covers all user-operable features of the SuperDroneLink module. For technical issues, please contact LimxTeam technical support.*
