# 11 - Laser System

> **Module**: SuperStage Runtime (ASuperLaserActor / ASuperLaserProActor)  
> **Target Users**: Lighting designers, laser programmers, virtual production technicians  
> **Prerequisites**: [00 - DMX System Overview](/docs/stage-core/overview)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

SuperStage provides real-time integration with **Pangolin Beyond** laser control software, enabling **real-time visualization** of laser projection effects in UE scenes. The system provides two Laser Actors for different rendering modes:

| Type | Class Name | Rendering Method | Description |
|------|------|---------|------|
| **Texture Projection Laser** | SuperLaserActor | Dynamic material + texture mapping | Drives beam material via RenderTarget texture, simulating laser projection effects |
| **Point Data Laser (Pro)** | SuperLaserProActor | Procedural mesh rendering | Draws precise laser lines from laser point coordinate data, supports collision occlusion |

Both obtain real-time data from Beyond devices via **SuperLaserSubsystem** (laser engine subsystem).

### Class Inheritance Chain

```
AActor (UE5 Engine Base Class)
  │
  └── ASuperBaseActor ·············· SuperStage Root Class (L1)
        │                           ├ SceneBase / ForwardArrow / UpArrow
        │                           └ AssetMetaData
        │
        ├── ASuperLaserActor ······· Texture Projection Laser (L2)
        │     ├ DeviceID → SuperLaserSubsystem gets RenderTarget
        │     ├ Dynamic material parameters (LaserIntensity/MaxDistance/Fog)
        │     └ Beam static mesh rendering
        │
        └── ASuperLaserProActor ···· Point Data Laser Pro (L2)
              ├ DeviceID → SuperLaserSubsystem gets point data
              ├ SuperLaserProComponent → Procedural mesh
              ├ EnableCollision → Collision occlusion detection
              └ Sprite + Arrow editor helpers
```

> **Design Note**: Both Laser Actors directly inherit `ASuperBaseActor` (not `ASuperDmxActorBase`) because they **do not use DMX protocol control** — data comes directly from the real-time stream of Pangolin Beyond software, not through DMX channels. Therefore they have no Universe/Address configuration and no fixture library.

### Data Flow

```
Pangolin Beyond Software
       │
       │ Real-time data stream (texture / point data)
       ▼
SuperLaserSubsystem (engine subsystem)
       │
       ├──→ SuperLaserActor (texture mode)
       │      └─ RenderTarget → Dynamic material → Beam mesh
       │
       └──→ SuperLaserProActor (point data mode)
              └─ Point coordinate array → SuperLaserProComponent → Procedural mesh
```

---

# Part 1: Texture Projection Laser (SuperLaserActor)

## 2. Basic Principles

SuperLaserActor uses the **RenderTarget texture** output from a Beyond device as a laser pattern, projecting it onto a beam-shaped static mesh to simulate the effect of laser projection in the air.

**Rendering Pipeline**:
```
Per-frame Tick
  ↓
Get device texture (RenderTarget) from SuperLaserSubsystem
  ↓
Set to dynamic material's LightTexture parameter
  ↓
Control beam visibility based on intensity
  ↓
Render beam static mesh
```

---

## 3. Property Details

### 3.1 Device Configuration

| Parameter | Location | Description | Range | Default |
|------|------|------|------|--------|
| **DeviceID** | SuperLaser | Beyond device number. Corresponds to the device number in Beyond software | 1 - 12 | 1 |

**DeviceID Notes**:
- Each DeviceID corresponds to one laser device in Beyond
- Supports up to **12** Beyond devices connected simultaneously
- DeviceID starts numbering from **1** (consistent with Beyond interface)
- Different SuperLaserActors can have different DeviceIDs, displaying laser effects from different devices

### 3.2 Beam Appearance Parameters

| Parameter | Description | Range | Default | Unit |
|------|------|------|--------|------|
| **LaserIntensity** | Laser brightness intensity | 1 - 5,000 | 100 | Percentage |
| **MaxLaserDistance** | Maximum beam projection distance | 100 - 10,000 | 2,345 | cm |
| **AtmosphericDensity** | Atmospheric attenuation coefficient (degree of beam fading with distance) | 0.0 - 1.0 | 0.03 | — |
| **FogIntensity** | Fog effect intensity in beam | 0 - 100 | 20 | Percentage |
| **AtmosFogSpeed** | Fog effect flow speed | 0 - 100 | 10 | Percentage |

#### LaserIntensity

Controls overall beam brightness:
- Higher value → brighter beam
- Set to **0** automatically hides the beam (saves render performance)
- Recommended range: 50 ~ 500 (too high may cause overexposure)

#### MaxLaserDistance

Controls how far the beam extends in the scene:
- Unit is centimeters (100cm = 1 meter)
- Default 2,345cm ≈ 23.45 meters
- Adjust based on scene size — larger venues need larger values

#### AtmosphericDensity

Simulates beam scattering and attenuation in air:
- **0.0** → No attenuation (beam equally bright from start to end)
- **0.03** (default) → Slight attenuation (realistic effect)
- **1.0** → Extremely strong attenuation (beam disappears quickly)

#### FogIntensity

Controls volumetric fog effect in the beam (simulating laser scattering in smoke):
- **0** → No fog effect (clean beam line)
- **20** (default) → Light fog
- **100** → Heavy fog (like the effect in stage smoke)

#### AtmosFogSpeed

Controls the flow animation speed of the fog texture:
- **0** → Static
- **10** (default) → Slow flow
- **100** → Fast flow

### 3.3 Parameter Tuning Suggestions

| Scenario | LaserIntensity | MaxLaserDistance | AtmosphericDensity | FogIntensity | FogSpeed | Description |
|------|---------------|-----------------|-------------------|-------------|---------|------|
| **Small Studio** | 50 - 150 | 1000 - 1500 | 0.05 - 0.1 | 10 - 30 | 5 - 10 | Close-range projection, avoid overexposure |
| **Medium Theater** | 100 - 300 | 2000 - 3000 | 0.03 | 20 - 40 | 10 | Standard stage effect |
| **Large Arena** | 200 - 500 | 5000 - 8000 | 0.01 - 0.02 | 30 - 60 | 10 - 20 | Long distance needs high brightness, low attenuation |
| **Outdoor Festival** | 300 - 800 | 8000 - 10000 | 0.005 - 0.01 | 5 - 15 | 5 | Open space, sparse fog |
| **Immersive Experience** | 50 - 100 | 500 - 1000 | 0.1 - 0.3 | 50 - 100 | 15 - 30 | Close-range heavy fog for atmosphere |

> **Performance Tip**: Pro mode (SuperLaserProActor) `EnableCollision` performs ray tests for every laser line segment, increasing CPU load in dense laser shows. Recommended to enable only when occlusion effects are needed.

---

# Part 2: Point Data Laser Pro (SuperLaserProActor)

## 5. Basic Principles

SuperLaserProActor uses the **laser point coordinate data** output from a Beyond device to **precisely draw laser lines** as procedural meshes via SuperLaserProComponent.

### Point Data Format

Each laser point contains:

| Field | Description | Range |
|------|------|------|
| **X** | Horizontal position | -1.0 ~ 1.0 (normalized to projection range) |
| **Y** | Vertical position | -1.0 ~ 1.0 (normalized to projection range) |
| **Z** | Beam marker | > 0 means beam point (laser on), ≤ 0 means blank point |
| **R** | Red component | 0.0 ~ 1.0 |
| **G** | Green component | 0.0 ~ 1.0 |
| **B** | Blue component | 0.0 ~ 1.0 |

---

## 6. Property Details

### 6.1 Device Configuration

| Parameter | Location | Description | Range | Default |
|------|------|------|------|--------|
| **DeviceID** | A.DefaultParameter | Beyond device number | 1 - 12 | 1 |
| **EnableCollision** | A.DefaultParameter | Enable collision occlusion detection | On/Off | Off |

#### EnableCollision

- **Off (default)** — Laser lines penetrate all objects (pure visual effect)
- **On** — Laser lines are **blocked and truncated** when encountering scene objects, simulating real laser hitting object surfaces

### 6.2 Editor Helpers

In the editor, SuperLaserProActor provides the following helper visualization:

| Component | Description |
|------|------|
| **Sprite** | Displays a light icon in the scene for locating the laser projector |
| **Arrow** | Green arrow pointing in the -Z direction (downward), indicating the default laser projection direction |

---

## 7. Texture Mode vs Pro Mode Comparison

| Feature | Texture Mode (SuperLaserActor) | Pro Mode (SuperLaserProActor) |
|------|--------------------------|------------------------------|
| **Rendering Method** | Texture mapped to beam mesh | Procedural mesh drawing laser lines |
| **Precision** | Medium (limited by texture resolution) | **High** (based on precise coordinate data) |
| **Appearance** | Beam volumetric effect (like projection) | Precise laser lines |
| **Collision Occlusion** | Not supported | **Supported** |
| **Fog Control** | **Supported** (5 parameters) | Depends on component settings |
| **Performance Overhead** | Lower (static mesh + material) | Medium (procedural mesh updated per frame) |
| **Use Case** | Distant/atmosphere laser effects | Close-up/precise laser lines |
| **Data Source** | RenderTarget texture | Point coordinate array |

### Selection Suggestions

- **Pursuing visual atmosphere** → Use Texture mode (has fog, beam volume feel)
- **Pursuing accurate reproduction** → Use Pro mode (precise lines, collision occlusion)
- **Mix both** → Same DeviceID can simultaneously place both Actor types, effects overlay

---

## 8. Multi-Device Configuration

SuperStage supports up to **12** Beyond devices connected simultaneously. Each device can output independent laser content.

### Multi-Device Scenario Example

```
Beyond Device 1 (DeviceID=1) → SuperLaserProActor_1 (Stage left laser)
Beyond Device 2 (DeviceID=2) → SuperLaserProActor_2 (Stage right laser)
Beyond Device 3 (DeviceID=3) → SuperLaserActor_1   (Background projection laser)
Beyond Device 4 (DeviceID=4) → SuperLaserProActor_3 (Audience scanning laser)
```

Each Actor independently configured with DeviceID; system auto-retrieves the corresponding device's data from the Subsystem.

---

## 9. FAQ

### Q: Cannot see laser in the scene?
1. Confirm **Beyond software is running** and playing laser content
2. Confirm **SuperLaserSubsystem** has established connection with Beyond
3. Check if **DeviceID** matches the device number in Beyond
4. (Texture mode) Check if **LaserIntensity** > 0
5. Confirm Actor **rotation direction** is correct (laser should point to expected direction)

### Q: Laser visible in editor but disappears at runtime?
Check for permission-related restrictions. System uses `SUPER_REQUIRE_PERMISSION` for authorization checks.

### Q: Laser penetrates walls?
- **Texture Mode**: Collision occlusion not supported, this is normal behavior
- **Pro Mode**: Set **EnableCollision** to **True** to enable collision occlusion

### Q: Laser effect stuttering/not smooth?
1. Confirm Beyond data output frequency is normal
2. (Pro mode) Many laser points may cause performance issues; try reducing point density in Beyond
3. Confirm UE editor frame rate is normal (large scenes may cause FPS drops)

### Q: How to point laser in a specific direction?
Adjust Actor **rotation**:
- **Texture Mode**: Beam mesh defaults to projecting along the Actor's forward direction
- **Pro Mode**: Default projection direction is -Z (downward) indicated by green arrow. Rotate Actor to change projection direction

### Q: Can both modes be used simultaneously?
Yes. Same DeviceID can use both SuperLaserActor (texture mode) and SuperLaserProActor (Pro mode); both will display laser content from the same device with effects layered.

---

> **Return to Overview**: [00 - DMX System Overview](/docs/stage-core/overview)
