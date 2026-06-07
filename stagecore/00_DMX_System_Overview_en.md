# SuperStage DMX System — User Manual Overview

> **Version**: SuperStage 26Q2  
> **Target Users**: Stage lighting designers, lighting programmers, virtual production technicians  
> **Last Updated**: 2026-04-14

---

## 1. What is SuperStage

SuperStage is a **professional stage visualization plugin** running in Unreal Engine 5. Its core mission is to enable lighting designers to control virtual fixtures in real-time using actual DMX signals within a virtual 3D scene, achieving lighting effect previews that are fully consistent with real performances.

In simple terms: **When you push a fader on the MA console, the virtual light in UE responds accordingly**.

### Core Capabilities at a Glance

| Capability | Description |
|------|------|
| **DMX Real-time Input** | Art-Net (UDP 6454) / sACN E1.31 (UDP 5568), millisecond-level response |
| **DMX Real-time Output** | Reverse output of Art-Net / sACN to real fixtures during Sequencer playback |
| **Full Range of Fixtures** | Moving lights / Wash lights / Matrix lights / Profile lights / LED strips / Lasers / Effects machines / Lift matrices / Stage machinery / Cameras / Projectors / NDI screens |
| **Professional Color System** | RGB / RGBW / HSV / ColorWheel / CMY / CTO / CoolWarm / ColorMix (arbitrary channel color mixing) |
| **Gobo & Prism** | Up to 3 Gobo wheels + 3 Prism wheels, static/infinite rotation |
| **Cutting System** | 4-blade 8-channel Shaper with superimposed rotation |
| **Matrix Control** | Pixel-level independent color/brightness/strobe, supports matrices of any scale |
| **Sequencer Integration** | Record DMX → Sequencer tracks, playback/export supported |
| **MA Console Export** | One-click generation of fixture configuration files importable into MA2/MA3 |
| **Multi-Platform Protocols** | Art-Net / sACN input/output + Pangolin Beyond (Laser) + NDI (Video) |

---

## 2. What is DMX (Quick Primer)

DMX512 is the standard control protocol for the stage lighting industry. You need to understand the following core concepts:

| Concept | Description |
|------|------|
| **Universe** | A single DMX line containing 512 channels. Can be understood as "one signal cable" |
| **Channel** | A single control value in each Universe, range 0-255 (8-bit). Each channel controls one function of a fixture (e.g., brightness, color, position) |
| **Address** | The starting channel number of a fixture within a Universe (1-512). Similar to a "house number" |
| **Fixture** | A single light. One fixture typically occupies multiple consecutive channels (e.g., a moving light may occupy 30 channels) |
| **Fixture ID** | The unique identifier of a fixture, used to correspond with the MA console |
| **Art-Net** | A protocol for transmitting DMX signals over Ethernet. SuperStage uses this protocol by default, port 6454 |
| **sACN (E1.31)** | ANSI E1.31 standard streaming DMX protocol, using multicast/unicast UDP port 5568 |

### DMX Channel Precision

| Precision | Bytes | Value Range | Use Case | SuperStage Support |
|------|--------|--------|------|-----------------|
| **8-bit (Coarse)** | 1 channel | 0 - 255 | Basic control (Color, Gobo, etc.) | ✅ `GetAttributeRaw8ByIndex` |
| **16-bit (Fine)** | 2 channels | 0 - 65,535 | Fine control (Pan/Tilt and other functions requiring smooth movement) | ✅ `GetAttributeRaw16ByIndex` |
| **24-bit (Ultra)** | 3 channels | 0 - 16,777,215 | Ultra-fine control (rarely used) | ✅ `GetAttributeRaw24ByIndex` |

---

## 3. Complete Class Inheritance System

All SuperStage Actor classes form a clear inheritance tree. Understanding this tree is fundamental to using this system correctly.

### 3.1 Inheritance Tree Overview

```
AActor (UE5 Engine Base Class)
  │
  └── ASuperBaseActor ·················· Root class for all SuperStage Actors
        │                                 ├ SceneBase (root component)
        │                                 ├ FAssetMetaData (UUID/Manufacturer/Group metadata)
        │                                 └ Direction preview arrows (ForwardArrow / UpArrow)
        │
        ├── ASuperDmxActorBase ·········· DMX fixture base class (adds DMX reading capability)
        │     │                            ├ FSuperDMXFixture (Universe / StartAddress / FixtureID)
        │     │                            ├ USuperFixtureLibrary* (fixture library reference)
        │     │                            ├ GetChannelValue / GetAttributeRaw8/16/24ByIndex
        │     │                            ├ GetMatrixAttributeRaw / Raw16 (batch matrix read)
        │     │                            ├ GetSuperDmxAttributeValue (normalized read 0~1)
        │     │                            ├ SuperDMXTick (blueprint per-frame event)
        │     │                            ├ ForceRefreshDMX (editor force refresh)
        │     │                            └ AddressLabel (address code text display)
        │     │
        │     ├── ASuperLightBase ······· Moving light base class (adds Pan/Tilt control layer)
        │     │     │                      ├ SceneRocker → SceneHard (arm → head rotation axes)
        │     │     │                      ├ Pan/Tilt range control (default ±270°/±135°)
        │     │     │                      ├ Infinite rotation (PanRot/TiltRot continuous rotation mode)
        │     │     │                      ├ PTSpeed speed control
        │     │     │                      ├ Matrix Pan/Tilt (YMatrixPan/YMatrixTilt)
        │     │     │                      ├ Z-axis lift (SetLiftZ, LiftRange default 500cm)
        │     │     │                      └ Lamp angle (LampAngle manual fixed mode)
        │     │     │
        │     │     ├── ASuperStageLight · Full-featured moving light (complete fixture implementation) → Document 15
        │     │     │                      ├ Beam: Dimmer/Strobe/Zoom/Focus/Frost/Iris
        │     │     │                      ├ Color: RGB/RGBW/HSV/ColorWheel/CMY/CTO/CoolWarm/ColorMix
        │     │     │                      ├ Gobo: Up to 3 Gobo wheels + rotation
        │     │     │                      ├ Prism: Up to 3 Prism wheels + rotation
        │     │     │                      ├ Cutting: 4-blade Shaper (A1/B1~A4/B4) + rotation
        │     │     │                      ├ Effects: Effect LUT system (10 built-in effects)
        │     │     │                      ├ Matrix: SuperMatrixComponent (pixel-level RGB/White/Strobe)
        │     │     │                      └ Spot fill light: SuperSpotComponent (real SpotLight fill)
        │     │     │
        │     │     ├── ASuperStageVFXActor Stage VFX → Document 14
        │     │     │                      ├ UNiagaraComponent (particle emitter)
        │     │     │                      ├ DMX → SpawnCount / Color
        │     │     │                      └ Inherits Pan/Tilt (controllable emission direction)
        │     │     │
        │     │     └── ASuperLiftMatrix ·· Lift matrix light array → Document 12
        │     │                            ├ 5-layer lift components + 4 steel cables
        │     │                            ├ 10 SuperEffectComponents
        │     │                            ├ DMX lift + matrix effect/color control
        │     │                            └ Inherits Pan/Tilt
        │     │
        │     ├── ASuperDMXCamera ········ DMX Camera → Document 05
        │     │                            ├ UCineCameraComponent (cine camera)
        │     │                            ├ USceneCaptureComponent2D (render to RT)
        │     │                            ├ 6-axis motion: PosX/Y/Z + RotX/Y/Z
        │     │                            └ Lens parameters: FOV / Aperture / FocusDistance
        │     │
        │     ├── ASuperLiftingMachinery · Lifting machinery (6-axis) → Document 10
        │     │                            ├ 3-axis translation + 3-axis rotation
        │     │                            ├ Absolute mode / Infinite rotation mode
        │     │                            └ BootRefresh initialize ranges
        │     │
        │     ├── ASuperRailMachinery ···· Rail machinery (7-axis) → Document 10
        │     │                            ├ USplineComponent (spline rail path)
        │     │                            ├ RailMountPoint + OffsetComponent
        │     │                            ├ Rail position + 3-axis offset + 3-axis rotation
        │     │                            └ Closed loop / Orientation lock
        │     │
        │     └── ASuperLightStripEffect · LED strip effects → Document 13
        │                                  ├ GPU material effects (10 built-in effects)
        │                                  ├ 9-channel DMX control
        │                                  └ Multi-target StaticMesh batch application
        │
        ├── ASuperLaserActor ·············· Laser (texture projection mode) → Document 11
        │                                  └ RenderTarget texture projection
        │
        ├── ASuperLaserProActor ··········· Laser (point data precise mode) → Document 11
        │                                  └ Laser point coordinates → line rendering + collision detection
        │
        ├── ASuperMediaBase ··············· Media playback base class (NDI / Static Texture dual mode)
        │     │                            ├ SourceMode (NDI / StaticTexture)
        │     │                            └ OnActiveTextureChanged (texture update callback)
        │     │
        │     ├── ASuperProjector ········· Projector → Document 05(stageassets)
        │     │                            └ Video/Image projection (SpotLight real illumination)
        │     │
        │     └── ASuperScreen ··········· Screen (formerly SuperNDIScreen)
        │                                  └ NDI / Static texture → Material + Keystone correction
        │
        ├── ASuperScaffold ················ Scaffolding (parametric generation)
        ├── ASuperCurvedScaffold ·········· Curved scaffolding (spline-driven)
        ├── ASuperTruss ··················· Truss/gantry (parametric generation)
        ├── ASuperCircularTruss ·········· Circular truss (polar coordinate segments)
        ├── ASuperCurvedTruss ············ Curved truss (spline-driven)
        ├── ASuperTrussGrid ·············· Truss grid (double-layer horizontal grid)
        ├── ASuperTrussTower ············· Truss tower (vertical column)
        ├── ASuperStageFloor ············· Stage floor (parametric platform)
        ├── ASuperDrape ··················· Drape (procedural fold generation)
        └── ASuperCrowd ··················· Procedural crowd (Poisson sampling)
```

### 3.2 Responsibility of Each Level

| Level | Class Name | Responsibility | Added Capabilities |
|------|------|------|----------|
| **L0** | `AActor` | UE5 engine base class | Tick, Transform, Component system |
| **L1** | `ASuperBaseActor` | SuperStage root class | SceneBase root component, asset metadata (UUID/Manufacturer/Group), direction preview arrows |
| **L2** | `ASuperDmxActorBase` | DMX data reading layer | DMX Fixture address (Universe/Address/FixtureID), fixture library reference, channel value reading (8/16/24-bit), batch matrix reading, normalized reading, blueprint event SuperDMXTick, address label display |
| **L3** | `ASuperLightBase` | Pan/Tilt control layer | SceneRocker→SceneHard rotation axes, Pan/Tilt range mapping, infinite rotation mode, PTSpeed speed control, matrix Pan/Tilt, Z-axis lift, lamp angle fixation |
| **L4** | `ASuperStageLight` | Full-featured fixture implementation | Full implementation of Beam/Color/Gobo/Prism/Cutting/Effects/Matrix/Spot fill light |

> **Design Principle**: Each layer only adds the functionality it is responsible for, without overstepping boundaries. For example, `ASuperDmxActorBase` only handles DMX data reading and has no knowledge of Pan/Tilt; `ASuperLightBase` only handles Pan/Tilt rotation and has no knowledge of color/gobo. This layered design allows you to derive new Actor types at any level.

### 3.3 "Which Class Should I Use?" — Selection Decision Table

| What You Want to Do | Choose Base Class | Typical Scenario |
|------------|-----------|---------|
| Need full moving light functionality (Color/Gobo/Prism/Cutting/Effects/Matrix) | **Use ASuperStageLight directly** | Martin MAC Viper, Robe MegaPointe, ClayPaky Sharpy |
| Only need DMX control + Pan/Tilt rotation | **Inherit ASuperLightBase** | Simple follow spot, rotating desk lamp |
| Only need DMX data reading (no rotation) | **Inherit ASuperDmxActorBase** | LED bar screen, DMX relay, custom devices |
| No DMX needed, only SuperStage basic features | **Inherit ASuperBaseActor** | Stage props, trusses, drapes |
| Laser visualization | **ASuperLaserActor / ASuperLaserProActor** | Pangolin Beyond integration |
| Niagara particle effects | **ASuperStageVFXActor** | Smoke machine, flame, CO2, snow |
| Virtual camera | **ASuperDMXCamera** | Studio/LED virtual production |
| Stage lift/rotation | **ASuperLiftingMachinery** | Lift platform, rotating stage |
| Rail movement | **ASuperRailMachinery** | Rail car, moving boom |
| LED strip effects | **ASuperLightStripEffect** | Light strips, contour lights |
| Lift matrix light array | **ASuperLiftMatrix** | LED lift balls/tubes |

---

## 4. Full Actor Feature Comparison Matrix

The following matrix lists the feature support for all DMX-controlled Actors, helping you quickly compare and select:

| Feature | StageLight | LightBase | VFXActor | LiftMatrix | DMXCamera | LiftMach | RailMach | StripEffect |
|------|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| **DMX Data Reading** | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| **Fixture Library** | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| **Pan/Tilt Rotation** | ✅ | ✅ | ✅ | ✅ | — | — | — | — |
| **Infinite Rotation** | ✅ | ✅ | ✅ | ✅ | — | ✅ | ✅ | — |
| **Z-Axis Lift** | ✅ | ✅ | ✅ | ✅ | — | — | — | — |
| **Dimmer** | ✅ | — | — | ✅ | — | — | — | ✅ |
| **Strobe** | ✅ | — | — | — | — | — | — | ✅ |
| **RGB Color** | ✅ | — | ✅ | ✅ | — | — | — | ✅ |
| **RGBW Color** | ✅ | — | — | — | — | — | — | — |
| **HSV Color** | ✅ | — | — | — | — | — | — | — |
| **ColorWheel** | ✅ (1~3 wheels) | — | — | — | — | — | — | — |
| **CMY Color Mixing** | ✅ | — | — | — | — | — | — | — |
| **CTO Color Temperature** | ✅ | — | — | — | — | — | — | — |
| **Cool/Warm Light Mixing** | ✅ | — | — | — | — | — | — | — |
| **Dynamic Multi-Channel ColorMix** | ✅ | — | — | — | — | — | — | — |
| **Zoom** | ✅ | — | — | — | — | — | — | — |
| **Focus** | ✅ | — | — | — | — | — | — | — |
| **Frost** | ✅ | — | — | — | — | — | — | — |
| **Iris** | ✅ | — | — | — | — | — | — | — |
| **Gobo Wheel** | ✅ (1~3 wheels) | — | — | — | — | — | — | — |
| **Prism** | ✅ (1~3 wheels) | — | — | — | — | — | — | — |
| **Cutting (Shaper)** | ✅ | — | — | — | — | — | — | — |
| **Effect LUT** | ✅ | — | — | ✅ | — | — | — | ✅ |
| **Matrix** | ✅ | — | — | — | — | — | — | — |
| **Spot Fill Light (SpotLight)** | ✅ | — | — | — | — | — | — | — |
| **3-Axis Translation** | — | — | — | — | ✅ | ✅ | ✅(offset) | — |
| **3-Axis Rotation** | — | — | — | — | ✅ | ✅ | ✅ | — |
| **Rail Movement (Spline)** | — | — | — | — | — | — | ✅ | — |
| **FOV / Aperture / Focus** | — | — | — | — | ✅ | — | — | — |
| **Render to RenderTarget** | — | — | — | — | ✅ | — | — | — |
| **Niagara Particles** | — | — | ✅ | — | — | — | — | — |
| **GPU Material Effects** | — | — | — | — | — | — | — | ✅ |
| **Multi-Target Batch Application** | — | — | — | — | — | — | — | ✅ |

---

## 5. Overall System Architecture

SuperStage's DMX system consists of the following parts working together:

```
┌─────────────────────────────────────────────────────────────────────────┐
│                           External Device Layer                          │
│   MA2/MA3 Console │ grandMA3 onPC │ Other Consoles │ Pangolin Beyond    │
└───────┬──────────────────┬──────────────────────────┬───────────────────┘
        │ Art-Net / sACN    │ Art-Net / sACN           │ Beyond Laser Data
        │ UDP 6454 / 5568   │ UDP 6454 / 5568          │ (TCP/Shared Memory)
        ▼                   ▼                          ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                      SuperDMX Engine Subsystem                           │
│                                                                          │
│  ┌──────────┐    ┌───────────────────┐    ┌───────────────────┐        │
│  │ UDP Rx    │───▶│ Art-Net / sACN    │───▶│  Universe Buffer   │       │
│  │  (Input)  │    │  Protocol Parsing  │    │  (512ch × N Univs) │       │
│  └──────────┘    └───────────────────┘    └─────────┬─────────┘        │
│                                                      │                   │
│  ┌──────────┐    ┌───────────────────┐               │ GetDMXValue()     │
│  │ UDP Tx    │◀───│ Art-Net / sACN    │◀──────────────┘ SendDMXBuffer()  │
│  │  (Output) │    │  Frame Packaging  │  (Sequencer Playback / SuperConsolePro)
│  └──────────┘    └───────────────────┘                                   │
└────────────────────────┬────────────────────────────────────────────────┘
                         │ Per-frame Tick → Read DMX channel values
                         ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                      Virtual Fixture Actor Layer                          │
│                                                                          │
│  ┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐           │
│  │  Moving Light     │ │  DMX Camera      │ │  Stage Machinery  │          │
│  │  SuperStageLight  │ │  SuperDMXCamera  │ │  LiftingMach      │          │
│  │  ─────────────── │ │  ─────────────── │ │  RailMachinery    │          │
│  │  Beam/Color/Gobo  │ │  6-axis Move/Rot │ │  6/7-axis Motion  │          │
│  │  Prism/Cut/Effect │ │  FOV/Apt/Focus   │ │  Abs/Infinite Mode│          │
│  │  Matrix/Spot Fill │ │  Render to RT    │ │  Spline Rail       │          │
│  └─────────────────┘ └─────────────────┘ └─────────────────┘           │
│                                                                          │
│  ┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐           │
│  │  LED Strip Effect │ │  Stage VFX       │ │  Lift Matrix      │          │
│  │  LightStripEffect│ │  StageVFXActor   │ │  LiftMatrix       │          │
│  │  ─────────────── │ │  ─────────────── │ │  ─────────────── │          │
│  │  10 GPU Matl FX  │ │  Niagara Particles│ │  5-Layer+Cables  │          │
│  │  9ch DMX Control  │ │  SpawnCount+RGB  │ │  Matrix FX/Color │          │
│  │  Multi-Target     │ │  Pan/Tilt Inherit│ │  Pan/Tilt Inherit│          │
│  └─────────────────┘ └─────────────────┘ └─────────────────┘           │
│                                                                          │
│  ┌─────────────────┐ ┌─────────────────┐                                │
│  │  Laser (Tex Proj) │ │  Laser (Point)   │                                │
│  │  SuperLaserActor  │ │  LaserProActor   │                                │
│  │  ─────────────── │ │  ─────────────── │                                │
│  │  RenderTarget    │ │  Line+Collision  │                                │
│  │  Beyond Integration│ │  Beyond Integration│                                │
│  └─────────────────┘ └─────────────────┘                                │
│                                                                          │
│  Each fixture defines its channel table via "Fixture Library"            │
│  Specifies Universe + Start Address + FixtureID via "DMX Patch"          │
└─────────────────────────────────────────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                         Toolchain                                          │
│                                                                          │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐ │
│  │Patch Tool │  │ Monitor   │  │DMX Record │  │MA Export  │  │DMX Config│ │
│  │Batch Addr │  │Live Ch Val│  │Sequencer │  │MA2/MA3   │  │Art-Net   │ │
│  │Assignment │  │Viewing    │  │Playback   │  │Fixture Cfg│  │Net Setup  │ │
│  └──────────┘  └──────────┘  └──────────┘  └──────────┘  └──────────┘ │
└─────────────────────────────────────────────────────────────────────────┘
```

### Core Data Flow (How signals travel from console to fixtures)

```
① Console sends DMX signal
   ↓ Art-Net UDP packets (port 6454) or sACN UDP packets (port 5568)
② SuperDMX subsystem receives → Parses Art-Net / sACN protocol → Extracts Universe number + 512 channel values
   ↓ Stores in Universe buffers (thread-safe, readable by game thread at any time)
③ Fixture Actor Tick
   ↓ Reads channel values from buffer according to its Universe + StartAddress
④ Fixture Library channel mapping
   ↓ FSuperDMXModuleInstance → FSuperDMXAttributeDef → Coarse/Fine/Ultra
⑤ Parameter application
   ↓ DMX values → Pan angle / Color / Brightness / Gobo / Position etc.
⑥ Unreal Engine real-time rendering
```

### Module Dependencies

```
┌──────────────────┐     ┌──────────────────┐
│    SuperDMX      │     │   SuperStage     │
│  (Runtime Module)│◀────│  (Runtime Module)│
│  ─────────────── │     │  ─────────────── │
│  USuperDMXSubsystem │  │  All Fixture Actors │
│  Art-Net Tx/Rx   │     │  Fixture Library Assets│
│  Universe Buffers│     │  Lighting Components│
└──────────────────┘     └──────────────────┘
                                 ▲
                         ┌───────┴──────────┐
                         │ SuperConsolePro  │
                         │  (Editor Module) │
                         │  ─────────────── │
                         │  Virtual Console UI│
                         │  Phaser / Cue    │
                         └──────────────────┘
```

---

## 6. Module Navigation

The SuperStage DMX system includes the following functional modules, each with independent detailed documentation:

### Core Fixture Modules

| Document | Module | Description |
|------|------|------|
| [03 - DMX Fixture Base](03_DMX_Actor_Base.md) | `ASuperDmxActorBase` | **Common foundation for all DMX fixtures**: Universe, Address, FixtureID, fixture library reference, DMX channel reading (8/16/24-bit), batch matrix reading, address label display |
| [04 - Moving Light Base Class](04_Computer_Light.md) | `ASuperLightBase` | **Pan/Tilt control layer**: horizontal/vertical rotation, infinite rotation, speed control, matrix heads, Z-axis lift, lamp angle |
| [15 - Full-Featured Moving Light](15_SuperStageLight.md) | `ASuperStageLight` | **Complete fixture implementation**: Beam (Dimmer/Strobe/Zoom/Focus/Frost/Iris), Color system (RGB/RGBW/HSV/ColorWheel/CMY/CTO/CoolWarm/ColorMix), Gobo (3x), Prism (3x), Cutting (Shaper 4-blade), Effects (Effect LUT 10 types), Matrix (pixel-level control), Spot fill light |

### Specialized Fixture Modules

| Document | Module | Description |
|------|------|------|
| [05 - DMX Camera](05_DMX_Camera.md) | `ASuperDMXCamera` | 6-axis translation/rotation + FOV/Aperture/Focus + Render to RenderTarget |
| [10 - Stage Machinery](10_Stage_Machinery.md) | `ASuperLiftingMachinery` / `ASuperRailMachinery` | Lift platform/rotating stage (6-axis), rail car (7-axis, spline path) |
| [11 - Laser System](11_Laser_System.md) | `ASuperLaserActor` / `ASuperLaserProActor` | Pangolin Beyond integration: texture projection mode / point data precise line mode |
| [12 - Lift Matrix](12_Lift_Matrix.md) | `ASuperLiftMatrix` | 5-layer lift components + 4 steel cables, matrix effect/color control |
| [13 - LED Strip Effect](13_Light_Strip_Effect.md) | `ASuperLightStripEffect` | 10 GPU material effects, 9-channel DMX control, multi-target batch application |
| [14 - Stage VFX](14_Stage_VFX.md) | `ASuperStageVFXActor` | DMX-controlled Niagara particle systems: smoke/flame/snow/CO2/fireworks |

### System Configuration & Tools

| Document | Module | Description |
|------|------|------|
| [01 - DMX Network Configuration](01_DMX_Network_Configuration.md) | DMX Configuration Panel | Art-Net / sACN protocol settings, IP address, port, input/output toggle, Universe start offset |
| [02 - Fixture Library](02_Fixture_Library.md) | Fixture Library | Create and edit fixture channel definition tables (attribute name→channel offset→precision→sub-attributes) |
| [06 - DMX Activity Monitor](06_DMX_Activity_Monitor.md) | Activity Monitor | Real-time viewing of current values for each channel in each Universe |
| [07 - Patch Tools](07_Patch_Tools.md) | Patch Tool & Preview | Batch DMX address assignment, preview and edit patch info for all fixtures |
| [08 - DMX Recording & Playback](08_DMX_Recording_Playback.md) | Sequencer & Take Recorder | Record external DMX input as Sequencer animation, support playback output |
| [09 - MA Console Export](09_Export_To_MA.md) | Export to MA / CSV | Export virtual fixture configurations as MA2/MA3 importable fixture configuration files |

---

## 7. Quick Start (5-Minute Setup)

### Step 1: Open the DMX Configuration Panel

Click the **DMX** button in the SuperStage toolbar at the top of the UE editor to open the DMX configuration panel.

### Step 2: Configure Network

- **Protocol**: Select Art-Net (default) or sACN (E1.31)
- **Input**: Check "Enable", Art-Net port 6454 / sACN port 5568
- **Local IP**: Select the NIC IP on the same subnet as the console (leave blank if only one NIC)
- Click "Apply"

### Step 3: Place Fixtures

Drag fixture blueprints from the Content Browser into the scene. Each fixture inherits from `ASuperDmxActorBase` and comes with built-in DMX control capability.

### Step 4: Assign DMX Address (Patch)

Select the fixture and set the following in the Details panel:
- **Universe**: The DMX universe the fixture belongs to (must match the console, range 1-256)
- **Start Address**: Starting address (must match the fixture's address in the console, range 1-512)
- **Fixture ID**: Fixture number (matching the console, used for MA export)

> **Tip**: For multiple fixtures, use the "Patch Tool" for batch address assignment. See [07 - Patch Tools](07_Patch_Tools.md).

### Step 5: Confirm Signal

Open **DMX Activity Monitor** and send signals from the console. If configured correctly, you'll see channel values changing in real-time. Virtual fixtures in the scene will also respond synchronously.

### Typical Workflow

```
Design Phase                    Programming Phase              Performance Phase
───────────                    ───────────                   ───────────
1. Build virtual stage         2. Connect MA console         5. Real-time light preview
   ├ Place fixture blueprints     ├ Configure Art-Net        6. Adjust lighting programming
   ├ Set fixture positions        ├ Assign DMX addresses     7. Record DMX → Sequencer
   └ Configure fixture library    └ Confirm signal           8. Export to MA console
3. Batch address assignment
4. Export fixture config to MA
```

---

## 8. System Requirements & Network Topology

### Hardware Requirements

| Item | Minimum | Recommended |
|------|---------|---------|
| **CPU** | Intel i5 / AMD Ryzen 5 | Intel i7 / AMD Ryzen 7 or above |
| **GPU** | NVIDIA GTX 1660 / AMD RX 5600 | NVIDIA RTX 3070 / AMD RX 6800 or above |
| **RAM** | 16 GB | 32 GB or above |
| **Network** | 100 Mbps Ethernet | Gigabit Ethernet (required for multi-Universe scenarios) |
| **Engine** | Unreal Engine 5.7 | Unreal Engine 5.8 |

### Recommended Network Topology

```
┌──────────────────┐                    ┌──────────────────┐
│   MA2/MA3 Console  │                    │  UE5 + SuperStage │
│   IP: 2.x.x.x    │◀──── Gigabit Switch ──▶│  IP: 2.x.x.x     │
│ Art-Net/sACN Output│         │          │ Art-Net/sACN Input│
└──────────────────┘         │          └──────────────────┘
                              │
                     ┌────────┴────────┐
                     │ Art-Net Node    │  (Optional: connect real fixtures)
                     │ IP: 2.x.x.x    │
                     └─────────────────┘
```

### Port Reference

| Port | Protocol | Direction | Description |
|------|------|------|------|
| **6454** | Art-Net | Input/Output | DMX signal transmit/receive (UDP) |
| **5568** | sACN (E1.31) | Input/Output | DMX signal transmit/receive (UDP multicast/unicast) |

### Universe Number Alignment

SuperStage provides a "Start Universe" offset setting for aligning with different consoles' numbering conventions:

| Console | Console Universe 1 | SuperStage Start Universe Set To | SuperStage Internal Universe |
|------|-----------------|------------------------------|------------------------|
| MA2/MA3 | Universe 1 | 0 (default) | 1 |
| MA2/MA3 | Universe 1 | 1 | 1 |

Default setting (Start Universe = 0) is already compatible with common MA2/MA3 configurations; no changes needed in most cases.

---

## 9. Performance Guide

### Fixture Count & Performance

| Scenario Scale | Fixture Count | Universe Count | Expected FPS (RTX 3070) |
|---------|---------|--------------|-------------------|
| Small show | < 50 fixtures | 1-4 | 60+ FPS |
| Medium show | 50-200 fixtures | 4-16 | 45-60 FPS |
| Large show | 200-500 fixtures | 16-64 | 30-45 FPS |
| Extra large | 500+ fixtures | 64+ | Depends on optimization |

### Performance Optimization Suggestions

| Optimization | Method | Effect |
|--------|------|------|
| **Reduce beam rendering** | Lower `BeamQuality` (default 75, can go to 50) | GPU load ↓ 20-30% |
| **Disable shadows** | `FLightLightSpotDefaultValue.bLightShadow = false` | GPU load ↓ 15-25% |
| **Reduce volumetric fog** | `VolumetricScattering = 0` | GPU load ↓ 10-15% |
| **Reduce light distance** | Lower `MaxLightDistance` (default 2345cm) | GPU load ↓ |
| **Spot fill light on demand** | Only enable SpotLight when real light interaction is needed | Significantly reduces lighting computation |
| **Matrix light optimization** | Reduce matrix scale or lower update frequency | CPU load ↓ |

---

## 10. FAQ

### Q1: Virtual fixtures don't respond to console signals?

**Troubleshooting steps (check in order)**:

1. **Check DMX input toggle** — DMX Configuration Panel → Input → Confirm "Enable" is checked
2. **Check network connectivity** — Confirm UE host and console are on the same subnet (ping test)
3. **Check ports** — Confirm UDP 6454 (Art-Net) or 5568 (sACN) ports are not blocked by firewall
4. **Check Activity Monitor** — Open DMX Activity Monitor, confirm signals are being received
5. **Check Universe alignment** — Confirm fixture's Universe number matches the console (note Start Universe offset setting)
6. **Check address match** — Confirm fixture's StartAddress matches the fixture's address in the console
7. **Check fixture library** — Confirm fixture is associated with a fixture library, and attribute names in the library match those called in blueprints
8. **Check blueprint logic** — Confirm correct control functions are called in the blueprint's SuperDMXTick event

### Q2: Address conflicts between multiple fixtures?
Use the "Patch Tool" to automatically calculate non-conflicting address assignments. See [07 - Patch Tools](07_Patch_Tools.md).

### Q3: How to output UE light effects to real fixtures?
Enable "Output" in the DMX configuration panel, set the remote IP to the fixture/node's address. During Sequencer playback, DMX signals are automatically output via Art-Net / sACN.

### Q4: How many Universes does SuperStage support?
Theoretically supports 32,767 Universes (Art-Net specification limit). Actual performance depends on hardware and scene complexity; typically 256 Universes or less runs without issue.

### Q5: Can DMX be received and sent simultaneously?
Yes. Input and output are independent and can be enabled simultaneously. For example: receive signals from MA console to control virtual fixtures, while outputting other Universes from Sequencer playback to real fixtures.

### Q6: Light effects appear dim / beams not visible?

| Issue | Solution |
|------|---------|
| Light too dim | Increase `MaxLightIntensity` (default 100, can go to 200-500) |
| Beam invisible | Confirm fog (ExponentialHeightFog) in scene, increase `BeamFogIntensity` (default 20) |
| Beam too short | Increase `MaxLightDistance` (default 2345cm, can go to 5000-10000cm for large scenes) |
| No shadows | Enable `LightSpotDefaultValue.bLightShadow = true` |
| Color inaccurate | Check Post Process Volume tone mapping settings, recommend ACES tone mapping |

### Q7: How to create custom fixtures?

1. Create a new Blueprint in Content Browser, parent class set to `ASuperStageLight` (or other suitable base class)
2. Replace fixture 3D models (four mesh components: Hook/Base/Rocker/Hard)
3. Create a Fixture Library defining the channel table
4. Call control functions in the blueprint's `SuperDMXTick` event
5. Adjust default parameters (beam range, color mode, etc.)

See the "Blueprint Integration" sections in each module's documentation for detailed steps.

---

## 11. Glossary

| Term | Chinese | Description |
|------|------|------|
| Universe | 域 | A 512-channel DMX signal line |
| Channel | 通道 | A control value within a DMX Universe (0-255) |
| Address | 地址 | Starting channel number of a fixture within a Universe |
| Patch | 补丁 | The process of assigning Universe + Start Address to a fixture |
| Fixture Library | 灯具库 | Data asset defining fixture channel function mapping |
| Fixture ID | 灯具 ID | Unique fixture number, corresponding to the fixture number in the console |
| Coarse | 粗调 | 8-bit (1 channel) precision control, value range 0-255 |
| Fine | 精调 | 16-bit (2 channel) precision control, value range 0-65535 |
| Ultra | 超精调 | 24-bit (3 channel) precision control, value range 0-16777215 |
| Attribute | 属性 | A controllable function of a fixture (e.g., Dimmer, Pan, Tilt, Color, etc.) |
| Module Instance | 模块实例 | Functional module in the fixture library (for multi-head definitions of matrix lights), containing Patch offset and attribute list |
| Channel Set | 通道集 | Named functional ranges for specific value ranges within a channel (similar to MA2 Channel Set) |
| Sub-Attribute | 子属性 | Functional intervals within an attribute subdivided by value range (e.g., in Gobo1 attribute: 0-10=White, 11-20=Gobo1) |
| Rocker | 灯臂 | The mechanical arm of a moving light responsible for Pan (horizontal rotation) (corresponds to `SceneRocker` component) |
| Hard | 灯头 | The head portion of a moving light responsible for Tilt (vertical rotation) (corresponds to `SceneHard` component) |
| Hook | 灯钩 | The hook part of a moving light hanging on a truss (corresponds to `SM_Hook` component) |
| Strobe | 频闪 | Rapid on/off flashing effect of light, controlled via Strobe channel |
| Frost | 雾化 | Optical effect that softens beam edges |
| Iris | 光圈 | Mechanical aperture controlling spot size |
| ColorWheel | 色轮 | Rotating disc with multiple fixed color filters |
| Gobo Wheel | 图案轮 | Rotating disc with multiple gobo templates |
| Prism | 棱镜 | Optical element that splits a single beam into multiple beams |
| Shaper/Blade | 切割 | Blades that block beam edges to achieve rectangular/trapezoidal spots |

---

> **Next Steps**: Read [01 - DMX Network Configuration](01_DMX_Network_Configuration.md) to learn how to set up network connections, or jump directly to [15 - Full-Featured Moving Light](15_SuperStageLight.md) to learn about the most commonly used fixture type.
