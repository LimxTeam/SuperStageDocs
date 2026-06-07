# SuperStage & SuperDMX Plugin Architecture Overview

> **Document Version**: 1.0  
> **Applicable Version**: SuperStage Plugin 2025/2026  
> **Target Audience**: Blueprint Developers / C++ Plugin Integration Developers (without source code modification permissions)  
> **Last Updated**: 2026-03

---

## 1. Plugin Overview

SuperStage is a professional stage lighting and performance visualization plugin suite for **Unreal Engine 5**, consisting of two closely collaborating runtime modules:

| Module | Export Macro | Responsibility |
|------|--------|------|
| **SuperDMX** | `SUPERDMX_API` | DMX protocol communication layer: Art-Net/sACN send/receive, data caching, Sequencer integration |
| **SuperStage** | `SUPERSTAGE_API` | Stage device layer: fixture Actors, lighting components, fixture libraries, mechanical control, laser, NDI, VFX |

The dependency relationship is **SuperStage → SuperDMX** (SuperStage depends on SuperDMX, no reverse dependency).

### 1.1 Core Capability Matrix

| Capability Domain | Description |
|--------|------|
| **DMX Communication** | Art-Net send/receive, sACN multicast receive, multi-universe parallel, thread-safe caching |
| **Fixture Simulation** | Moving lights (Beam/Spot/Wash/Profile), LED matrix lights, light strips, projectors |
| **Motion Control** | Pan/Tilt rotation, infinite rotation, lifting machinery, rail machinery (Spline splines) |
| **Color System** | RGB/RGBW/HSV/CMY/Color Temperature/Cool-Warm Mix/Color Wheel/Multi-channel dynamic color mixing |
| **Optical System** | Zoom/Focus/Frost/Iris, Gobo wheel (Gobo×3), Prism (Prism×3), Shaper cutting |
| **Effects System** | LED effect LUT (10 built-in effects), Niagara particle VFX |
| **Video Integration** | NDI video stream receiving and screen display |
| **Laser Integration** | Beyond laser device texture and point data visualization |
| **Sequencer** | DMX Track/Section recording and playback |
| **Fixture Library System** | GDTF/MA2/MA3 compatible DataAsset fixture library configuration |

---

## 2. System Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────────┐
│                    External Console / Software                        │
│                  (grandMA, Hog, ETC Eos, etc.)                        │
└───────────────┬─────────────────────────────────┬───────────────────┘
                │ Art-Net / sACN                  │ NDI / Beyond
                ▼                                 ▼
┌───────────────────────────┐   ┌─────────────────────────────────────┐
│      SuperDMX Module      │   │       Third-party Subsystems          │
│  ┌─────────────────────┐  │   │  SuperNDISubsystem                  │
│  │ USuperDMXSubsystem   │  │   │  SuperLaserSubsystem                │
│  │  ├─ ApplyConfig()   │  │   └─────────────────────────────────────┘
│  │  ├─ GetDMXBuffer()  │  │                 │
│  │  ├─ GetDMXValue()   │  │                 │
│  │  ├─ SendDMXBuffer() │  │                 │
│  │  └─ Thread-Safe Cache│  │                 │
│  └─────────────────────┘  │                 │
│  ┌─────────────────────┐  │                 │
│  │ Sequencer Integration│  │                 │
│  │  UMovieSceneSuperDMX │  │                 │
│  │  Track / Section     │  │                 │
│  └─────────────────────┘  │                 │
└───────────┬───────────────┘                 │
            │ GetDMXBuffer / SendDMXBuffer    │
            ▼                                 ▼
┌─────────────────────────────────────────────────────────────────────┐
│                      SuperStage Module                                │
│                                                                     │
│  ┌─────────────────────────────────────────────────────────────┐   │
│  │              Fixture Library System (Data Layer)              │   │
│  │  USuperFixtureLibrary → FSuperDMXModuleInstance              │   │
│  │                       → FSuperDMXAttributeDef               │   │
│  │                       → FSubAttribute / FChannelSet         │   │
│  └─────────────────────────────────────────────────────────────┘   │
│                              │                                     │
│                              ▼                                     │
│  ┌─────────────────────────────────────────────────────────────┐   │
│  │                 Actor Inheritance Hierarchy                   │   │
│  │                                                             │   │
│  │  AActor                                                     │   │
│  │    └─ ASuperBaseActor        (Metadata / Direction Preview)  │   │
│  │        ├─ ASuperDmxActorBase   (DMX Read Core)               │   │
│  │        │   ├─ ASuperLightBase  (Pan/Tilt Motion Control)     │   │
│  │        │   │   ├─ ASuperStageLight     (Full-function Moving Light)│
│  │        │   │   ├─ ASuperStageVFXActor  (Niagara VFX)        │   │
│  │        │   │   └─ ASuperLiftMatrix     (Lift Matrix)         │   │
│  │        │   ├─ ASuperLiftingMachinery   (Lifting Machinery 6-axis)│
│  │        │   ├─ ASuperRailMachinery      (Rail Machinery 7-axis)│   │
│  │        │   ├─ ASuperLightStripEffect   (LED Strip Effect)    │   │
│  │        │   └─ ASuperDMXCamera          (DMX Camera)          │   │
│  │        ├─ ASuperLaserActor       (Laser Texture Display)     │   │
│  │        ├─ ASuperLaserProActor    (Laser Point Data Display)  │   │
│  │        ├─ ASuperNDIScreen        (NDI Video Screen)          │   │
│  │        ├─ ASuperProjector        (Projection Mapping)        │   │
│  │        ├─ ASuperTruss            (Truss)                     │   │
│  │        ├─ ASuperScaffold         (Scaffold)                  │   │
│  │        ├─ ASuperCurvedScaffold   (Curved Scaffold)           │   │
│  │        └─ ASuperDrape            (Drape)                     │   │
│  └─────────────────────────────────────────────────────────────┘   │
│                              │                                     │
│                              ▼                                     │
│  ┌─────────────────────────────────────────────────────────────┐   │
│  │                 Component System (Components)                 │   │
│  │                                                             │   │
│  │  USceneComponent                                            │   │
│  │    └─ USuperLightingComponent   (Beam Base Component)        │   │
│  │        ├─ USuperSpotComponent   (SpotLight Fill Light)       │   │
│  │        │   └─ USuperBeamComponent   (Beam Light)             │   │
│  │        │       └─ USuperCuttingComponent (Cutting/Shaper)    │   │
│  │        └─ USuperRectComponent   (RectLight Area Light)       │   │
│  │    └─ USuperEffectComponent     (LED Effect Component)       │   │
│  │    └─ USuperMatrixComponent     (LED Matrix Pixel Component) │   │
│  │    └─ USuperLiftComponent       (Lift Component)             │   │
│  │    └─ USuperLaserProComponent   (Laser Pro Render Component) │   │
│  └─────────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 3. Module Details

### 3.1 SuperDMX Module

SuperDMX is the underlying communication engine, providing **global singleton** access in the form of `UEngineSubsystem`, automatically created on engine initialization, available to all modules/levels.

#### 3.1.1 USuperDMXSubsystem — DMX Communication Subsystem

**Access method** (C++):
```cpp
USuperDMXSubsystem* DMX = GEngine->GetEngineSubsystem<USuperDMXSubsystem>();
```

**Core responsibilities**:
- **Network I/O**: Configure Art-Net/sACN endpoints via `ApplyConfig()`, automatically listening for and parsing DMX packets
- **Data caching**: Internally maintains `TMap<int32, TArray<uint8>>` caching the latest 512-byte DMX frame per Universe
- **Thread safety**: Receive thread and game thread isolated via `FCriticalSection`, read/write operations thread-safe
- **Data output**: Send DMX data to the network via `SendDMXBuffer()` (Sequencer playback, programmatic control)
- **Diagnostics**: `GetSecondsSinceLastInput()` tracks signal activity

**Key data types**:

| Type | Description |
|------|------|
| `ESuperDMXProtocol` | Protocol enum: `ArtNet` |
| `FSuperDMXEndpoint` | Network endpoint configuration (LocalIp, RemoteIp, Port, StartUniverse) |
| `FSuperDMXIOConfig` | Complete I/O configuration (Protocol + Input/Output endpoints) |

#### 3.1.2 Sequencer Integration

| Class | Description |
|----|------|
| `UMovieSceneSuperDMXTrack` | Sequencer track, inherits from `UMovieSceneNameableTrack`, contains multiple Sections |
| `UMovieSceneSuperDMXSection` | Sequencer section, each Section corresponds to one Universe containing 512 channel animation curves |

During Sequencer playback, DMX data is output via `USuperDMXSubsystem::SendDMXBuffer()`, enabling **timeline-driven DMX programming**.

---

### 3.2 SuperStage Module

SuperStage is built on top of SuperDMX, providing complete stage device modeling and control capabilities.

#### 3.2.1 Fixture Library System

The fixture library bridges DMX channel numbers to device functions. Each fixture Actor references a `USuperFixtureLibrary` data asset that defines all DMX channel layouts for the fixture.

**Data hierarchy**:

```
USuperFixtureLibrary (DataAsset)
 ├── FixtureName          "Acme XP-380 Beam 17CH"
 ├── Manufacturer         "Acme"
 ├── Source               EFixtureLibrarySource (Custom/GDTF/MA2/MA3)
 ├── PowerConsumption     540.0W
 ├── Weight               21.2kg
 └── Modules[]            TArray<FSuperDMXModuleInstance>
      ├── [0] ModuleName = "Main"
      │    ├── Patch = 0     (offset relative to StartAddress)
      │    └── AttributeDefs[]
      │         ├── [0] AttribName="Dimmer", Coarse=1, Fine=2
      │         │        └── SubAttributes[] → FSubAttribute
      │         │             └── ChannelSets[] → FChannelSet
      │         ├── [1] AttribName="Pan", Coarse=3, Fine=4
      │         ├── [2] AttribName="Tilt", Coarse=5, Fine=6
      │         └── ...
      ├── [1] ModuleName = "Pixel1"  (sub-module for matrix light)
      │    ├── Patch = 17
      │    └── AttributeDefs[] ...
      └── ...
```

**Core struct overview**:

| Struct | Description |
|--------|------|
| `FSuperDMXModuleInstance` | Module instance (each pixel/head of a matrix light is a module) |
| `FSuperDMXAttributeDef` | Attribute definition: name, category, Coarse/Fine/Ultra channel offsets, default value |
| `FSubAttribute` | Sub-attribute/function (corresponds to MA2 ChannelFunction), includes strobe mode, rotation mode |
| `FChannelSet` | Channel set (corresponds to MA2 ChannelSet), includes gobo texture, color, prism parameters |
| `FSuperDMXAttribute` | Attribute lookup key (InstanceIndex + AttribName + ChannelType) |
| `FSuperDMXFixture` | Fixture DMX address configuration (Universe + StartAddress) |

**Attribute category enum `EDMXAttributeCategory`**:

| Value | Meaning | Typical Attributes |
|----|------|----------|
| `Dimmer` | Brightness | Dimmer, MasterDimmer |
| `Position` | Position | Pan, Tilt, PanRot |
| `Gobo` | Gobo | Gobo1, Gobo2, GoboRot |
| `Color` | Color | ColorWheel, Red, Green, Blue |
| `Beam` | Beam | Zoom, Iris |
| `Focus` | Focus | Focus |
| `Control` | Control | Reset, Lamp |
| `Shapers` | Shapers | ShapA1, ShapB1 |
| `Strobe` | Strobe | Strobe |
| `Prism` | Prism | Prism1, PrismRot |
| `Frost` | Frost | Frost |
| `Effects` | Effects | Effect |
| `Other` | Other | — |

#### 3.2.2 Actor Inheritance Hierarchy

##### Layer 1: ASuperBaseActor — Base Asset Layer

The base class for all SuperStage Actors, providing:
- **Metadata management** (`FAssetMetaData`): UUID, grouping, manufacturer, DMX mode name, thumbnail
- **Direction preview**: Visualize +X forward and +Z up directions via arrow components in the editor
- **Root component** `SceneBase`: Mounting point for all child components

##### Layer 2: ASuperDmxActorBase — DMX Data Reading Layer

**Core responsibility**: Read DMX data from `USuperDMXSubsystem` and convert it to usable values.

**Key properties**:
- `SuperDMXFixture` (`FSuperDMXFixture`): Universe + StartAddress
- `FixtureLibrary` (`USuperFixtureLibrary*`): Channel configuration reference

**DMX value reading API** (sorted by usage frequency):

| Function | Returns | Description |
|------|------|------|
| `GetSuperDmxAttributeValue()` | `float [0,1]` | Read attribute value and **normalize** to 0-1 range |
| `GetSuperDmxAttributeValueNoConversion()` | `float` | Read attribute value, **without** normalization |
| `GetSuperDMXColorValue()` | `FLinearColor` | Read RGB three channels and combine into color |
| `GetSuperDmxAttributeRawValue()` | `int32 [0,255]` | Read raw DMX value |
| `GetAttributeRaw8ByIndex()` | `int32` | Read 8-bit raw value by module index |
| `GetAttributeRaw16ByIndex()` | `int32` | Read 16-bit raw value by module index (Coarse×256+Fine) |
| `GetAttributeRaw24ByIndex()` | `int32` | Read 24-bit raw value by module index |
| `GetChannelValue()` | `float [0,1]` | Read single channel value by relative address |
| `GetMatrixAttributeRaw()` | `TArray<float>` | Matrix mode: read same-named attribute from all module instances |
| `GetMatrixAttributeRaw16()` | `TArray<float>` | Matrix mode: 16-bit version |

**Blueprint events**:
- `SuperDMXTick(float DeltaSeconds)`: Blueprint-implementable event triggered **every frame**; users write DMX control logic in this event
- `LightInitialization`: Fixture initialization delegate

##### Layer 3: ASuperLightBase — Pan/Tilt Motion Control Layer

Adds **moving head motion control** on top of the DMX reading layer:

**Component structure**:
```
SceneBase
  └─ SceneRocker  (Pan rotation axis — horizontal rotation)
      └─ SceneHard (Tilt rotation axis — vertical rotation)
          └─ Light head components (beam/model, etc.)
```

**Motion control API**:

| Function | Description |
|------|------|
| `YPan()` | Horizontal rotation (Pan), DMX→angle range mapping |
| `YTilt()` | Vertical rotation (Tilt) |
| `YPTSpeed()` | Pan/Tilt speed control |
| `YPanRot()` / `YTiltRot()` | Infinite rotation (continuous rotation mode) |
| `YMatrixPan()` / `YMatrixTilt()` | Matrix multi-head independent Pan/Tilt |
| `YMatrixPanRot()` / `YMatrixTiltRot()` | Matrix multi-head infinite rotation |
| `YLampAngle()` | Manually set fixture angle |
| `SetLiftZ()` | Lift control (cooperates with `USuperLiftComponent`) |

**Motion parameters**:
- `PanRange` / `TiltRange`: Angle range (default ±270° / ±135°)
- `InfiniteRotationalSpeed`: Infinite rotation speed multiplier
- `PTSpeed`: Pan/Tilt interpolation speed (`FInterpTo` smoothing)

##### Layer 4: ASuperStageLight — Full-function Moving Light

**The richest fixture class**, providing complete professional fixture control capabilities, with **90+ BlueprintCallable functions** covering the following functional domains:

**1. Main Light Control (Lighting)**

| Function | Single Light Function | Matrix Function |
|------|---------|---------|
| Intensity | `SetLightingIntensity` | `SetLightingIntensityMatrix` |
| Strobe | `SetLightingStrobe` | `SetLightingStrobeMatrix` |
| RGB | `SetLightingColorRGB` | `SetLightingColorRGBMatrix` |
| RGBW | `SetLightingColorRGBW` | `SetLightingColorRGBWMatrix` |
| HSV | `SetLightingColorHSV` | `SetLightingColorHSVMatrix` |
| HS | `SetLightingColorHS` | `SetLightingColorHSMatrix` |
| Color Wheel | `SetLightingColorWheel` | — |
| Color Wheel + CMY | `SetLightingColorWheelAndCMY` | — |
| Multi Color Wheel | `SetLightingColorWheel2/3` | — |
| Color Temperature | `SetLightingColorTemperature` | — |
| Cool-Warm Mix | `SetLightingCoolWarmMix` | — |
| Multi-channel Color Mix | `SetLightingColorMix` | `SetLightingColorMixMatrix` |
| Zoom | `SetLightingZoom` | `SetLightingZoomMatrix` |
| Frost | `SetLightingFrost` | — |
| Iris | `SetLightingIris` | — |

**2. Spot Fill Light Control**

| Function | Single Light Function | Matrix Function |
|------|---------|---------|
| Intensity | `SetSpotIntensity` | `SetSpotIntensityMatrix` |
| Strobe | `SetSpotStrobe` | `SetSpotStrobeMatrix` |
| RGB | `SetSpotColorRGB` | `SetSpotColorRGBMatrix` |
| RGBW | `SetSpotColorRGBW` | `SetSpotColorRGBWMatrix` |
| Cool-Warm Mix | `SetSpotCoolWarmMix` | `SetSpotCoolWarmMixMatrix` |
| Zoom | `SetSpotZoom` | `SetSpotZoomMatrix` |
| Frost | `SetSpotFrost` | `SetSpotFrostMatrix` |
| Color Temperature Matrix | — | `SetSpotColorTemperatureMatrix` |

**3. Gobo / Prism / Shaper**

| Function | Description |
|------|------|
| `SetBeamGobo1/2/3()` | 1~3 gobo wheels + rotation control (includes atlas textures) |
| `SetBeamPrism()` | Prism selection + rotation |
| `SetBeamFocus()` | Focus control |
| `SetBeamCutting()` | Four-blade cutting (A1/B1 ~ A4/B4, 8 channels) |
| `SetBeamCuttingRotate()` | Cutting system rotation |

**4. Effects Layer**

| Function | Description |
|------|------|
| `SetEffectDefaultValue()` | Initialize effect component |
| `SetEffectIntensity()` | Effect intensity |
| `SetEffectStrobe()` | Effect strobe |
| `SetEffectColor()` / `SetEffectColorMatrix()` | Effect color |
| `SetEffect()` / `SetEffectMatrix()` | Effect selection (LUT lookup, 10 built-in effects) |
| `SetEffectWithSpeed()` | Effect + independent speed channel |

**5. Matrix Pixel Control**

| Function | Description |
|------|------|
| `SetMatrixComponent()` | Matrix RGB + Strobe |
| `SetMatrixWhiteSingle/Multiple()` | Matrix white (single point/multi point) |
| `SetMatrixColorSingle/Multiple()` | Matrix RGB (single pixel/multi pixel) |
| `SetMatrixIntensity()` | Matrix intensity |
| `SetMatrixStrobe()` | Matrix strobe |
| `SetMatrixCoolWarmMixSingle/Multiple()` | Matrix cool-warm mix |
| `SetMatrixColorMix()` | Matrix multi-channel color mix |
| `SetMatrixColorRGBW*()` | Matrix RGBW series |
| `SetMatrixColorRGBWithCTO*()` | Matrix RGB+CTO series |

---

### 3.3 Other Actor Types

#### 3.3.1 ASuperLiftingMachinery — Lifting Machinery

Controls **6-axis motion** (XYZ displacement + XYZ rotation) of stage devices via DMX, supporting absolute position mode and infinite rotation mode.

| Function | Description |
|------|------|
| `LiftingMachinery()` | Main control function (6 DMX attribute parameters) |
| `BootRefresh()` | Initialize motion range baseline |

**Control parameters**: PosX/Y/Z (displacement [0,1]), RotX/Y/Z (rotation [0,1]), PosSpeed/RotSpeed (interpolation speed)

#### 3.3.2 ASuperRailMachinery — Rail Machinery

Adds **spline curve rail axis** on top of 6-axis motion, totaling **7-axis DMX control**.

| Function | Description |
|------|------|
| `RailMachinery()` | 7-axis main control (RailPos + XYZ offset + XYZ rotation) |
| `RailPositionOnly()` | Rail position control only |
| `BootRefresh()` | Initialize range baseline |

**Featured capabilities**:
- `USplineComponent` defines rail path (visual editing in editor)
- `bLockOrientationToRail`: Lock orientation along the rail tangent
- `bClosedLoop`: Closed loop rail cyclic motion

#### 3.3.3 ASuperDMXCamera — DMX Camera

Controls position, rotation, and camera parameters of `UCineCameraComponent` via DMX.

| Function | Description |
|------|------|
| `DMXCameraControl()` | 6-axis motion control (XYZ displacement + XYZ rotation) |
| `DMXCameraParams()` | Camera parameters (FOV + Aperture + Focus Distance) |
| `GetCineCamera()` | Get camera component |
| `GetRenderTarget()` | Get render target (for LED screen output) |

#### 3.3.4 ASuperStageVFXActor — Niagara VFX

Controls Niagara particle systems via DMX (smoke / fire / snow / CO2, etc.).

| Function | Description |
|------|------|
| `SetNiagaraDefaultValue()` | Initialize Niagara parameters |
| `SetNiagaraSpawnCount()` | DMX-controlled particle spawn count |
| `SetNiagaraColor()` | DMX-controlled particle color |

**Niagara variable mapping**: Speed, LoopTime, Minimum, Maximum, SpawnCount, Color

#### 3.3.5 ASuperLightStripEffect — LED Light Strip

Achieves programmable LED strip effects via materials.

| Function | Description |
|------|------|
| `SetLightStripDefault()` | Initialize material |
| `SetLightStripEffect()` | Full control (intensity / strobe / effect / speed / width / direction / RGB) |

**Feature**: Supports batch application of effect materials to multiple `AStaticMeshActor`.

#### 3.3.6 ASuperLiftMatrix — Lift Matrix

Contains 5 vertically arranged scene components + 10 effect components + 4 wire ropes.

| Function | Description |
|------|------|
| `LiftMatrix()` | Lift control |
| `SetEffectMatrixDefaultValue()` | Initialize effect components |
| `SetEffectMatrix()` | Effect selection |
| `SetEffectColorMatrix()` | Effect color |

#### 3.3.7 ASuperNDIScreen — NDI Video Screen

Receives real-time video streams from the NDI network and displays them on a screen mesh.

**Key properties**:
- `InputName`: NDI source name (with dropdown selection)
- `TargetStaticMeshActors`: Multi-screen output targets
- `bTransparent`: Transparency mode toggle
- `Deformation`: Four-point keystone correction (projection blending)

#### 3.3.8 ASuperLaserActor / ASuperLaserProActor — Laser

| Class | Description |
|----|------|
| `ASuperLaserActor` | RenderTarget texture-based laser display |
| `ASuperLaserProActor` | Point data-based laser line drawing (`USuperLaserProComponent`) |

#### 3.3.9 ASuperProjector — Projection Mapping

Implements projection mapping using Light Functions, supporting RGB three-channel overlay projection + keystone correction.

#### 3.3.10 Stage Construction Assets

| Class | Description |
|----|------|
| `ASuperTruss` | Truss (straight/arc) |
| `ASuperScaffold` | Scaffold (configurable layers and dimensions) |
| `ASuperCurvedScaffold` | Curved scaffold |
| `ASuperDrape` | Drape (controllable folds and color) |

---

## 4. Component System

SuperStage's lighting rendering achieves high reusability through **component-based design**:

### 4.1 USuperLightingComponent — Beam Base Component

The base class for all light source components, providing:
- Material management (dynamic material instances)
- Intensity / Color / Strobe control
- Zoom / Frost / Iris control
- Beam rotation
- Ray occlusion detection (Ray Detection)

**Derived components**:

| Component | Description |
|------|------|
| `USuperSpotComponent` | SpotLight fill light component, manages `USpotLightComponent`, supports Zoom/Iris/Frost/Color Temperature |
| `USuperBeamComponent` | Beam light component (spotlight/beam light effects) |
| `USuperRectComponent` | RectLight area light component (Wash light / panel light effects) |

### 4.2 Independent Function Components

| Component | Description |
|------|------|
| `USuperEffectComponent` | LED effect component: 10 built-in effects + speed/width/direction control |
| `USuperCuttingComponent` | Cutting system component: four-blade Shaper + rotation |
| `USuperMatrixComponent` | LED matrix pixel component: per-pixel color / intensity / strobe |
| `USuperLiftComponent` | Lift component: smooth lift motion |
| `USuperLaserProComponent` | Laser Pro render component: point data → procedural mesh |

---

## 5. Data Flow and Execution Model

### 5.1 DMX Data Flow

```
┌──────────────────┐
│  External DMX    │
│  Console         │
│  (Art-Net output)│
└────────┬─────────┘
         │ UDP packets
         ▼
┌──────────────────────────────────┐
│   USuperDMXSubsystem             │
│   ┌──────────────────────────┐  │
│   │   Receive Thread         │  │
│   │   Parse Art-Net packets  │  │
│   │   Write to UniverseToSlots│  │
│   └──────────┬───────────────┘  │
│              │ FCriticalSection  │
│   ┌──────────▼───────────────┐  │
│   │   UniverseToSlots        │  │
│   │   TMap<int32, uint8[512]>│  │
│   └──────────┬───────────────┘  │
└──────────────┼──────────────────┘
               │ GetDMXBuffer / GetDMXValue
               ▼
┌──────────────────────────────────┐
│   ASuperDmxActorBase::Tick()     │
│   ├─ Get Universe snapshot       │
│   ├─ Call SuperDMXTick() Blueprint event│
│   │   └─ User Blueprint logic    │
│   │       ├─ SetLightingIntensity│
│   │       ├─ YPan / YTilt        │
│   │       ├─ SetLightingColorRGB │
│   │       └─ ...                 │
│   └─ Update component render state│
└──────────────────────────────────┘
```

### 5.2 Blueprint Development Workflow

Typical Blueprint development process:

1. **Place Fixture Actor** — Drag `ASuperStageLight` (or its subclass Blueprint) into the scene
2. **Configure Fixture Library** — Set `FixtureLibrary` (fixture library data asset)
3. **Configure DMX Address** — Set `SuperDMXFixture` (Universe + StartAddress)
4. **Set component default parameters** (optional) — Adjust `MaxLightIntensity`, `ZoomRange`, etc. in the component panel
5. **Write control logic** — Implement `SuperDMXTick` event in Blueprint:
   ```
   Event SuperDMXTick
     → SetLightingIntensity (Dimmer attribute)
     → SetLightingStrobe (Strobe attribute)
     → SetLightingColorRGB (Red/Green/Blue attributes)
     → YPan (Pan attribute)
     → YTilt (Tilt attribute)
     → SetLightingZoom (Zoom attribute)
     → ...
   ```
6. **Start DMX reception** — Call `USuperDMXSubsystem::ApplyConfig()` via editor tool or Blueprint

> **26Q2.2 Update**: Component default parameters are automatically initialized by `OnRegister()`. When a component registers, it automatically calls `SetLightingMaterial()` to create a material instance, then calls `SetLightingDefaultValue()` to set default values. **No need to manually call initialization functions in the Actor**.

### 5.3 FSuperDMXAttribute Lookup Mechanism

All fixture control functions accept the `FSuperDMXAttribute` struct as a parameter:

```
FSuperDMXAttribute
 ├── InstanceIndex : int32    → Module instance index (0 = main module, 1+ = matrix sub-modules)
 ├── AttribName    : FName    → Attribute name (must match what's defined in the fixture library)
 └── DMXChannelType: EDMXChannelType → Channel precision (8-Bit / 16-Bit / 24-Bit)
```

**Lookup flow**:
1. Locate `FSuperDMXModuleInstance` based on `InstanceIndex`
2. Find `FSuperDMXAttributeDef` based on `AttribName`
3. Calculate absolute DMX address = `StartAddress + Patch + Coarse - 1`
4. Read corresponding bytes from `USuperDMXSubsystem` cache
5. Combine Coarse/Fine/Ultra bytes based on `DMXChannelType`
6. Normalize to [0,1] range (if using normalized API)

---

## 6. Permission System

SuperStage has a built-in **subscription permission check** that controls DMX reading and feature usage:

```cpp
// Macros defined in SuperStagePermission.h
SUPER_REQUIRE_PERMISSION_VOID()    // No permission → return;
SUPER_REQUIRE_PERMISSION_RET(val)  // No permission → return val;
SUPER_IF_PERMISSION { ... }        // Conditional execution
```

**Permission rules**:
- ✅ **Standalone** (packaged runtime): All features unrestricted
- ✅ **Preview** (editor preview): All features unrestricted
- 🔒 **PIE / GamePreview / Editor**: Requires valid subscription

> **Impact on integration developers**: DMX data reading functions already include permission checks internally, no manual handling needed. When unauthorized, these functions will return default values (0 or no-op).

---

## 7. Color System Details

SuperStage supports multiple color control modes, covering all mainstream industry solutions:

### 7.1 Direct Control Modes

| Mode | API | Description |
|------|-----|------|
| **RGB** | `SetLightingColorRGB` | Three-channel independent control (most common) |
| **RGBW** | `SetLightingColorRGBW` | Four-channel, white channel added to RGB |
| **HSV** | `SetLightingColorHSV` | Hue / Saturation / Value |
| **HS** | `SetLightingColorHS` | Hue / Saturation (value controlled by Dimmer) |

### 7.2 Color Wheel Modes

| API | Description |
|-----|------|
| `SetLightingColorWheel` | Single color wheel (pass in color atlas texture) |
| `SetLightingColorWheel2/3` | Multi color wheel overlay |
| `SetLightingColorWheelAndCMY` | Color wheel + CMY subtractive color mixing |
| `SetLightingColorWheel2AndCMYAndCTO` | Multi wheel + CMY + CTO color temperature adjustment |

### 7.3 Color Temperature Modes

| API | Description |
|-----|------|
| `SetLightingColorTemperature` | Single channel color temperature (Warm→Cool, 1700K-12000K) |
| `SetLightingCoolWarmMix` | Dual channel cool-warm mix (independent Cool/Warm control) |

### 7.4 Dynamic Multi-channel Color Mixing

| API | Description |
|-----|------|
| `SetLightingColorMix` | Arbitrary multi-channel color mixing (RGBW/RGBA/RGBLAM etc.) |
| `SetLightingColorMixWithCTO` | Multi-channel color mixing + CTO color temperature adjustment |

Multi-channel color mixing uses the `FSuperColorChannel` struct:
```
FSuperColorChannel
 ├── DmxAttribute : FSuperDMXAttribute  → DMX channel
 └── ChannelColor : FLinearColor         → Color corresponding to this channel
```
**Color mixing algorithm**: All channels' `(DMX value × channel color)` additive overlay then Clamp.

---

## 8. Matrix Control Mode

Matrix control allows independent control of each pixel / head in a fixture.

### 8.1 Working Principle

1. Define multiple `FSuperDMXModuleInstance` in fixture library (one module per pixel)
2. Each module distinguishes channel range via `Patch` offset
3. Matrix APIs (such as `SetLightingIntensityMatrix`) automatically iterate through all module instances
4. `bSortByPatch` parameter controls whether to sort output by Patch

### 8.2 Single Point vs Multi Point API Comparison

| Category | Single | Multiple |
|------|---------------|-----------------|
| Parameter | Requires `int32 Index` | Automatically iterates all modules |
| Use Case | Precise control of a specific pixel | Batch control of all pixels |
| Example | `SetMatrixColorSingle(R,G,B,0,Matrix)` | `SetMatrixColorMultiple(R,G,B,Matrix)` |

---

## 9. C++ Plugin Integration Guide

### 9.1 Module Dependency Configuration

Add the following to your plugin `.Build.cs`:

```csharp
PublicDependencyModuleNames.AddRange(new string[]
{
    "SuperDMX",     // DMX Communication
    "SuperStage",   // Fixture Control
});
```

### 9.2 Getting the DMX Subsystem

```cpp
#include "SuperDMXSubsystem.h"

USuperDMXSubsystem* DMX = GEngine->GetEngineSubsystem<USuperDMXSubsystem>();
if (DMX)
{
    // Configure Art-Net reception
    FSuperDMXIOConfig Config;
    Config.Protocol = ESuperDMXProtocol::ArtNet;
    Config.Input.bEnabled = true;
    Config.Input.Port = 6454;
    DMX->ApplyConfig(Config);

    // Read DMX data
    TArray<uint8> Buffer;
    if (DMX->GetDMXBuffer(1, Buffer))  // Universe 1
    {
        uint8 DimmerValue = Buffer[0]; // Channel 1
    }
}
```

### 9.3 Accessing Fixture Attributes

```cpp
#include "SuperDmxActorBase.h"
#include "AssetTool/SuperFixtureLibrary.h"

// Get fixture Actor in scene
ASuperDmxActorBase* Light = /* FindActor... */;

// Read DMX attribute value (normalized 0-1)
FSuperDMXAttribute DimmerAttr;
DimmerAttr.InstanceIndex = 0;
DimmerAttr.AttribName = FName("Dimmer");
DimmerAttr.DMXChannelType = EDMXChannelType::Conarse;

float DimmerValue = 1.0f;
Light->GetSuperDmxAttributeValue(DimmerAttr, DimmerValue);
// DimmerValue now contains the normalized intensity value
```

---

## 10. Public API Class Index

### SuperDMX Module

| Class | Header File | Description |
|----|--------|------|
| `USuperDMXSubsystem` | `SuperDMXSubsystem.h` | DMX communication subsystem (global singleton) |
| `UMovieSceneSuperDMXTrack` | `Sequencer/MovieSceneSuperDMXTrack.h` | Sequencer DMX track |
| `UMovieSceneSuperDMXSection` | `Sequencer/MovieSceneSuperDMXSection.h` | Sequencer DMX section |

### SuperStage Module — Actor

| Class | Header File | Description |
|----|--------|------|
| `ASuperBaseActor` | `SuperBaseActor.h` | Base asset Actor |
| `ASuperDmxActorBase` | `SuperDmxActorBase.h` | DMX reading base class |
| `ASuperLightBase` | `LightActor/SuperLightBase.h` | Pan/Tilt motion base class |
| `ASuperStageLight` | `LightActor/SuperStageLight.h` | Full-function moving light |
| `ASuperStageVFXActor` | `LightActor/SuperStageVFXActor.h` | Niagara VFX Actor |
| `ASuperLiftMatrix` | `LightActor/SuperLiftMatrix.h` | Lift matrix |
| `ASuperLiftingMachinery` | `LightActor/SuperLiftingMachinery.h` | Lifting machinery (6-axis) |
| `ASuperRailMachinery` | `LightActor/SuperRailMachinery.h` | Rail machinery (7-axis) |
| `ASuperDMXCamera` | `LightActor/SuperDMXCamera.h` | DMX Camera |
| `ASuperLightStripEffect` | `LightActor/SuperLightStripEffect.h` | LED strip effect |
| `ASuperNDIScreen` | `LightActor/SuperNDIScreen.h` | NDI video screen |
| `ASuperLaserActor` | `LightActor/SuperLaserActor.h` | Laser texture display |
| `ASuperLaserProActor` | `LightActor/SuperLaserProActor.h` | Laser point data display |
| `ASuperProjector` | `LightActor/SuperProjector.h` | Projection Mapping |
| `ASuperTruss` | `StageAssets/SuperTruss.h` | Truss/gantry (4 styles) |
| `ASuperCircularTruss` | `StageAssets/SuperCircularTruss.h` | Circular/ring truss |
| `ASuperCurvedTruss` | `StageAssets/SuperCurvedTruss.h` | Spline-driven curved truss |
| `ASuperTrussGrid` | `StageAssets/SuperTrussGrid.h` | Horizontal truss grid (lighting rig) |
| `ASuperTrussTower` | `StageAssets/SuperTrussTower.h` | Vertical truss tower |
| `ASuperScaffold` | `StageAssets/SuperScaffold.h` | Straight scaffold |
| `ASuperCurvedScaffold` | `StageAssets/SuperCurvedScaffold.h` | Spline-driven curved scaffold |
| `ASuperDrape` | `StageAssets/SuperDrape.h` | Procedural drape (4 folds + 4 opening styles) |
| `ASuperCrowd` | `StageAssets/SuperCrowd.h` | Procedural crowd (Poisson sampling + spline area) |
| `ASuperStageFloor` | `StageAssets/SuperStageFloor.h` | Stage floor |

### SuperStage Module — Components

| Class | Header File | Description |
|----|--------|------|
| `USuperLightingComponent` | `LightComponent/SuperLightingComponent.h` | Beam base component |
| `USuperSpotComponent` | `LightComponent/SuperSpotComponent.h` | SpotLight fill light component |
| `USuperBeamComponent` | `LightComponent/SuperBeamComponent.h` | Beam light component |
| `USuperRectComponent` | `LightComponent/SuperRectComponent.h` | RectLight area light component |
| `USuperEffectComponent` | `LightComponent/SuperEffectComponent.h` | LED effect component |
| `USuperCuttingComponent` | `LightComponent/SuperCuttingComponent.h` | Cutting system component |
| `USuperMatrixComponent` | `LightComponent/SuperMatrixComponent.h` | LED matrix pixel component |
| `USuperLiftComponent` | `LightComponent/SuperLiftComponent.h` | Lift component |
| `USuperLaserProComponent` | `LightComponent/SuperLaserProComponent.h` | Laser Pro render component |

### SuperStage Module — Data Types

| Type | Header File | Description |
|------|--------|------|
| `USuperFixtureLibrary` | `AssetTool/SuperFixtureLibrary.h` | Fixture library data asset |
| `FSuperDMXModuleInstance` | `AssetTool/SuperFixtureLibrary.h` | Module instance definition |
| `FSuperDMXAttributeDef` | `AssetTool/SuperFixtureLibrary.h` | Attribute channel definition |
| `FSuperDMXAttribute` | `AssetTool/SuperFixtureLibrary.h` | Attribute lookup key |
| `FSubAttribute` | `AssetTool/SuperFixtureLibrary.h` | Sub-attribute/function definition |
| `FChannelSet` | `AssetTool/SuperFixtureLibrary.h` | Channel set |
| `FSuperDMXFixture` | `LightActor/SuperLightTypes.h` | Fixture DMX address |
| `FAssetMetaData` | `SuperBaseActor.h` | Asset metadata |
| `FSuperColorChannel` | `LightActor/SuperStageLight.h` | Dynamic color mixing channel |
| `ESuperDMXProtocol` | `SuperDMXTypes.h` | DMX protocol enum |
| `FSuperDMXIOConfig` | `SuperDMXTypes.h` | DMX I/O configuration |
| `FSuperDMXEndpoint` | `SuperDMXTypes.h` | DMX network endpoint |
| `EDMXChannelType` | `AssetTool/SuperFixtureLibrary.h` | Channel precision enum |
| `EDMXAttributeCategory` | `AssetTool/SuperFixtureLibrary.h` | Attribute category enum |
| `EStrobeMode` | `AssetTool/SuperFixtureLibrary.h` | Strobe mode enum |
| `EGoboMode` | `AssetTool/SuperFixtureLibrary.h` | Gobo mode enum |
| `EPrismLayerSelect` | `AssetTool/SuperFixtureLibrary.h` | Prism layer selection enum (None/Prism1/2/3) |
| `EInfiniteRotationMode` | `AssetTool/SuperFixtureLibrary.h` | Infinite rotation mode enum |
| `EFixtureLibrarySource` | `AssetTool/SuperFixtureLibrary.h` | Fixture library source enum |
| `USuperPrismPreset` | `AssetTool/SuperPrismPreset.h` | Prism preset data asset |

---

## 11. Glossary

| Term (Chinese) | English | Description |
|------|------|------|
| 灯库 | Fixture Library | Data asset defining fixture DMX channel configuration |
| Universe | Universe | DMX domain, each domain contains 512 channels |
| 通道 | Channel | DMX address (1-512), each channel carries a value of 0-255 |
| Coarse/Fine/Ultra | — | 8/16/24-bit precision control, Fine is the low byte |
| Pan/Tilt | — | Horizontal/vertical rotation, basic motion axes of moving lights |
| Gobo | — | Gobo wheel, projects patterns through rotating metal or glass discs |
| Prism | — | Prism, splits the beam into multiple beams |
| CMY | Cyan/Magenta/Yellow | Subtractive color mixing system (Cyan/Magenta/Yellow filters) |
| CTO | Color Temperature Orange | Color temperature adjustment filter (orange, lowers color temperature) |
| Frost | — | Diffusion filter, softens beam edges |
| Iris | — | Iris, controls beam diameter |
| Shaper | — | Cutting system (four blades, also called Framing System) |
| Art-Net | — | UDP-based DMX over IP protocol, default port 6454 |
| sACN | E1.31 | ANSI E1.31 Streaming ACN protocol, uses IP multicast |
| NDI | Network Device Interface | NewTek network video transmission protocol |
| Sequencer | — | UE5 built-in timeline editor |
| LUT | Look-Up Table | Used for effect index → parameter mapping |
| Patch | — | Channel offset for fixture module |

---

> **Next Steps**: Detailed API documentation for each class will be covered one by one in subsequent chapters, including parameter descriptions, return values, usage examples, and notes for each public function.
