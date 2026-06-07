# SuperStage Fixture Development Guide

> **Document Version**: 1.0  
> **Applicable Version**: SuperStage Plugin 2025/2026  
> **Target Audience**: C++ Fixture Developers / Blueprint Fixture Producers  
> **Last Updated**: 2026-03

---

## Table of Contents

1. [Overview](#1-overview)
2. [Core Concepts](#2-core-concepts)
3. [Fixture Library System](#3-fixture-library-system)
4. [Actor Inheritance Hierarchy](#4-actor-inheritance-hierarchy)
5. [Component System](#5-component-system)
6. [C++ Fixture Development Workflow](#6-c-fixture-development-workflow)
7. [Blueprint Fixture Development Workflow](#7-blueprint-fixture-development-workflow)
8. [DMX Channel Reading API](#8-dmx-channel-reading-api)
9. [Color System Details](#9-color-system-details)
10. [Gobo and Prism System](#10-gobo-and-prism-system)
11. [Cutting System](#11-cutting-system)
12. [Effect System](#12-effect-system)
13. [Matrix System](#13-matrix-system)
14. [Motion Control](#14-motion-control)
15. [Strobe System](#15-strobe-system)
16. [Complete Example: C++ Beam Moving Light](#16-complete-example-c-beam-moving-light)
17. [Complete Example: Blueprint Wash Light](#17-complete-example-blueprint-wash-light)
18. [Complete Example: Blueprint Strobe Light](#18-complete-example-blueprint-strobe-light)
19. [Complete Example: Blueprint Profile Moving Light](#19-complete-example-blueprint-profile-moving-light)
20. [Complete Example: Blueprint Matrix Light (with Fill Light)](#20-complete-example-blueprint-matrix-light-with-fill-light)
21. [Complete Example: Blueprint Moving Head Matrix Light](#21-complete-example-blueprint-moving-head-matrix-light)
22. [Complete Example: Blueprint Multi-function Moving Light](#22-complete-example-blueprint-multi-function-moving-light)
23. [Detailed Guide for Each Fixture Type](#23-detailed-guide-for-each-fixture-type)
24. [Fixture Library Editor Usage Guide](#24-fixture-library-editor-usage-guide)
25. [FAQ and Troubleshooting](#25-faq-and-troubleshooting)
26. [Appendix: Standard Attribute Name Reference](#26-appendix-standard-attribute-name-reference)

---

## 1. Overview

SuperStage's fixture development system provides a complete pipeline from data definition to visual rendering:

```
Fixture Library Asset (USuperFixtureLibrary)     ← Defines DMX channel mapping
    ↓ referenced by
Fixture Actor (ASuperStageLight etc.)            ← Reads DMX data + drives components
    ↓ owns
Lighting Components (USuperBeamComponent etc.)   ← Renders beam/spot/effect
    ↓ writes to
Dynamic Material (UMaterialInstanceDynamic)      ← Visualized on GPU
```

**Two development paths**:

| Path | Applicable Scenarios | Technical Requirements |
|------|---------|---------|
| **C++ Subclass** | Complex fixtures, need extreme performance, new fixture types | C++ + UE Reflection System |
| **Blueprint Subclass** | Rapid prototyping, standard fixture variants, user customization | Blueprint Event Graph |

---

## 2. Core Concepts

### 2.1 Data Flow

```
DMX Signal (Art-Net/sACN)
    ↓ USuperDMXSubsystem receives
DMX Buffer (uint8[512] × 256 Universes)
    ↓ ASuperDmxActorBase::Tick() reads
Normalized value [0,1] or raw value [0-255]
    ↓ Control functions (SetLightingIntensity etc.)
Component parameters (material parameters / light source properties / mesh transforms)
    ↓ Rendering
Visual output
```

### 2.2 Normalization Convention

**All** DMX attribute values are normalized to `[0, 1]` after reading at the Actor level:

| DMX Precision | Raw Range | Normalization Formula |
|----------|---------|-----------|
| 8-bit | `0-255` | `value / 255.0` |
| 16-bit | `0-65535` | `value / 65535.0` |
| 24-bit | `0-16777215` | `value / 16777215.0` |

### 2.3 FSuperDMXAttribute — DMX Attribute Reference

```cpp
USTRUCT(BlueprintType)
struct FSuperDMXAttribute
{
    int32 InstanceIndex = 0;    // Module instance index (0-based)
    FName AttribName;           // Attribute name (must exactly match fixture library definition)
    EDMXChannelType DMXChannelType; // Precision: Conarse(8bit) / Fine(16bit) / Ultra(24bit)
};
```

This is the **unified struct** for referencing DMX channels in both Blueprint and C++.

### 2.4 FSuperDMXFixture — DMX Address

```cpp
USTRUCT(BlueprintType)
struct FSuperDMXFixture
{
    int32 FixtureID = 1;     // Fixture number (user identifier)
    int32 Universe = 1;      // DMX Universe (1-256)
    int32 StartAddress = 1;  // Start address (1-512)
};
```

Each fixture Actor's `SuperDMXFixture` property defines its position in the DMX network.

---

## 3. Fixture Library System

### 3.1 Data Model

```
USuperFixtureLibrary (UPrimaryDataAsset)
├── FixtureName: "Acme XP-380 Beam 17CH"
├── Manufacturer: "Acme"
├── Source: Custom / GDTF / MA2 / MA3
├── PowerConsumption: 540.0 W
├── Weight: 21.2 kg
└── Modules[]: FSuperDMXModuleInstance
    ├── ModuleName: "Mode 1"
    ├── Patch: 1 (channel offset, 0-based)
    └── AttributeDefs[]: FSuperDMXAttributeDef
        ├── AttribName: "Dimmer"
        ├── Category: EDMXAttributeCategory::Dimmer
        ├── Coarse: 1 (coarse byte channel, 1-based)
        ├── Fine: 2 (fine byte channel, 0=unused)
        ├── Ultra: 0 (ultra byte, 0=unused)
        ├── DefaultValue: 0 (default DMX value)
        ├── HighlightValue: 255 (highlight DMX value)
        └── SubAttributes[]: FSubAttribute
            ├── StrobeMode / RotationMode
            ├── DmxMin / DmxMax
            ├── PhysicalRange: FVector2D
            └── ChannelSets[]: FChannelSet
                ├── Name / GoboMode
                ├── DmxMin / DmxMax
                ├── PhysicalRange
                ├── Texture / Color / ColorIndex
                └── PrismFacets / PrismRadius / PrismScale
```

### 3.2 Fixture Library Hierarchy

```
USuperFixtureLibrary
 └── Modules[] — Module instances (corresponding to different control units of the fixture)
      └── AttributeDefs[] — Attribute definitions (each DMX channel)
           └── SubAttributes[] — Sub-attributes (DMX value range segments)
                └── ChannelSets[] — Channel sets (such as each gobo in a gobo wheel)
```

**Key concepts**:
- **Module**: Corresponds to one control unit of a fixture. Single-head fixtures typically have only 1 module; matrix lights/multi-head lights have multiple modules, each corresponding to one pixel/head.
- **AttributeDef**: Corresponds to one DMX channel group (Coarse + optional Fine + optional Ultra).
- **SubAttribute**: Segments the DMX value range of a channel, each segment can have different behavior (e.g., Dimmer channel: 0-7=off, 8-255=intensity).
- **ChannelSet**: Finer granularity segmentation within sub-attributes, used for each gobo of a gobo wheel, each color of a color wheel, etc.

### 3.3 Channel Address Calculation

```
Absolute Address = StartAddress + Module.Patch + AttributeDef.Coarse - 1
```

**Example**: StartAddress=101, Patch=0, Coarse=3 → Absolute Address=103

### 3.4 Channel Span

```cpp
int32 USuperFixtureLibrary::GetChannelSpan() const;
```

Returns total channels occupied by the fixture (Max address - Min address + 1).

### 3.5 Creating Fixture Library Assets

**Method 1 — Content Browser**:
1. Content Browser → Right-click → SuperStage → `SuperFixtureLibrary`
2. Double-click to open the fixture library editor
3. Fill in fixture name, manufacturer, source
4. Add modules and attribute definitions
5. Ctrl+S to save

**Method 2 — C++ Runtime Creation**:
```cpp
USuperFixtureLibrary* Lib = NewObject<USuperFixtureLibrary>(GetTransientPackage());
Lib->FixtureName = TEXT("My Custom Beam");
Lib->Manufacturer = TEXT("MyBrand");

FSuperDMXModuleInstance& Mod = Lib->Modules.AddDefaulted_GetRef();
Mod.ModuleName = FName(TEXT("Mode 1"));
Mod.Patch = 1;

// Add Dimmer attribute
FSuperDMXAttributeDef& Dimmer = Mod.AttributeDefs.AddDefaulted_GetRef();
Dimmer.AttribName = FName(TEXT("Dimmer"));
Dimmer.Category = EDMXAttributeCategory::Dimmer;
Dimmer.Coarse = 1;
Dimmer.Fine = 2; // 16bit
```

### 3.6 Fixture Library Enum Types Quick Reference

#### EDMXChannelType

| Value | Precision | Range |
|----|------|------|
| `Conarse` | 8-bit | 0-255 |
| `Fine` | 16-bit | 0-65535 |
| `Ultra` | 24-bit | 0-16777215 |

#### EDMXAttributeCategory

| Value | Meaning | Typical Attribute Names |
|----|------|-----------|
| `Dimmer` | Intensity | Dimmer, Intensity |
| `Position` | Position | Pan, Tilt, PanRot |
| `Gobo` | Gobo | Gobo1, Gobo2 |
| `Color` | Color | ColorR, ColorG, ColorWheel |
| `Beam` | Beam | Zoom, Iris |
| `Focus` | Focus | Focus |
| `Control` | Control | Control, Reset |
| `Shapers` | Shapers | A1, B1, ShaperRot |
| `Strobe` | Strobe | Strobe, StrobeMode |
| `Prism` | Prism | Prism1, Prism2 |
| `Frost` | Frost | Frost |
| `Effects` | Effects | EffectValue, EffectSpeed |
| `Other` | Other | — |

#### EStrobeMode

| Value | Mode | Waveform |
|----|------|------|
| 0 | Closed | Always off |
| 1 | Open | Always on |
| 2 | Linear | Linear (triangle wave) |
| 3 | Pulse | Square wave |
| 4 | RampUp | Sawtooth up |
| 5 | RampDown | Sawtooth down |
| 6 | Sine | Sine wave |
| 7 | Random | Random |

#### EInfiniteRotationMode

| Value | Mode | Behavior |
|----|------|------|
| `Off` | Off | Normal position control |
| `Stop` | Stop | Infinite rotation paused |
| `Position` | Position | Rotate from current position to target |
| `Infinite` | Infinite | Continuous rotation |

---

## 4. Actor Inheritance Hierarchy

```
AActor
 └─ ASuperBaseActor
     │  ├─ SceneBase (root component)
     │  ├─ ForwardArrow / UpArrow (direction preview)
     │  └─ AssetData (FAssetMetaData: UUID/Manufacturer/Thumbnail)
     └─ ASuperDmxActorBase
         │  ├─ SuperDMXFixture (Universe/StartAddress/FixtureID)
         │  ├─ FixtureLibrary → USuperFixtureLibrary*
         │  ├─ AddressLabel (address text label)
         │  ├─ GetChannelValue() / GetSuperDmxAttributeValue()
         │  ├─ GetAttributeRaw8/16/24ByIndex()
         │  ├─ GetMatrixAttributeRaw/Raw16()
         │  └─ SuperDMXTick (Blueprint-implementable event)
         └─ ASuperLightBase
             │  ├─ SceneRocker (Pan rotation axis)
             │  ├─ SceneHard (Tilt rotation axis)
             │  ├─ YPan() / YTilt() / YPTSpeed()
             │  ├─ YPanRot() / YTiltRot() (infinite rotation)
             │  └─ YLampAngle() / SetLiftZ()
             └─ ASuperStageLight
                 ├─ SetLightingDefaultValue() / SetLightingIntensity()
                 ├─ SetLightingStrobe() / SetLightingColorRGB()
                 ├─ SetLightingColorWheel() / SetLightingColorTemperature()
                 ├─ SetLightingZoom() / SetLightingFrost() / SetLightingIris()
                 ├─ SetBeamGobo1/2/3() / SetBeamPrism() / SetBeamFocus()
                 ├─ SetBeamCutting() / SetBeamCuttingRotate()
                 ├─ SetEffect*() / SetMatrix*()
                 └─ 80+ BlueprintCallable control functions
```

### Choosing the Base Class

| Fixture Type | Recommended Base Class | Reason |
|---------|---------|------|
| Standard Moving Light (Beam/Spot/Wash/Profile) | `ASuperStageLight` | Provides full set of control functions |
| Camera | `ASuperLightBase` | Needs Pan/Tilt, not lighting components |
| Lift Devices | `ASuperDmxActorBase` | Only needs DMX reading |
| LED Strip / Panel | `ASuperDmxActorBase` | Custom effect control |
| Laser | `ASuperDmxActorBase` | Custom laser component |

---

## 5. Component System

### 5.1 Component Selection Guide

| Component | Rendering Method | Applicable Scenario |
|------|---------|---------|
| `USuperBeamComponent` | SpotLight + beam mesh + spot material | Beam/Spot light (visible light column) |
| `USuperCuttingComponent` | Same as above + cutting material | Profile light (needs cutting) |
| `USuperSpotComponent` | SpotLight + spot material | Fill light / supplementary light (no visible light column) |
| `USuperRectComponent` | RectLight + spot material | LED panel / Wash light |
| `USuperEffectComponent` | Static mesh + effect material | LED effect light / strip |
| `USuperMatrixComponent` | Static mesh + segmented material | Matrix light / pixel light |
| `USuperLiftComponent` | No rendering (motion only) | Lift control |
| `USuperLaserProComponent` | Procedural mesh | Beyond laser |

### 5.2 Component Creation Patterns

**Created in C++ constructor**:
```cpp
// In fixture Actor constructor
SuperLighting = CreateDefaultSubobject<USuperBeamComponent>(TEXT("SuperLighting"));
SuperLighting->SetupAttachment(SceneHard);
SuperLighting->StaticMeshLens = LoadObject<UStaticMesh>(...);
```

**Created in Blueprint**:
1. Add components in the Blueprint editor's Components panel
2. Attach components to the correct parent (usually SceneHard)
3. Set default parameters in the Details panel

### 5.3 USuperLightingComponent Details (Base Lighting Component)

The **base class** for all lighting components, providing lens mesh, spot material, and strobe calculation.

**Inheritance chain**: `USceneComponent → USuperLightingComponent`

**Key properties**:

| Property | Type | Description |
|------|------|------|
| `StaticMeshLens` | `UStaticMesh*` | Lens mesh model |
| `LensTransform` | `FTransform` | Lens transform offset |
| `Angle` | `float` | Spot angle |
| `DimmerCurveExponent` | `float` | Intensity response curve (1.0=linear, 2.0=recommended quadratic) |
| `ComponentDimmer` | `float` | Component-level intensity control [0,1] |

**Initialization functions** (call in OnConstruction / BeginPlay):

```cpp
// Initialize material (automatically called internally, usually no manual call needed)
SetLightingMaterial();

// Set default light parameters (MUST be called!)
SetLightingDefaultValue(
    float NewMaxLightDistance,       // Maximum light distance (cm)
    float NewLightMaxIntensity,      // Maximum intensity
    float VolumetricScattering,      // Volumetric scattering intensity
    bool  LightShadow,              // Enable shadows
    float NewLightSpotIntensity,     // Spot intensity (default 100)
    float LensIntensity              // Lens glow intensity (default 1)
);
```

**Control functions** (call in Tick / SuperDMXTick):

```cpp
SetLightingIntensity(float NewIntensity);          // Intensity [0,1]
SetLightingStrobe(float NewStrobe);                // Strobe speed [0,1]
SetLightingStrobeMode(float NewStrobeMode);        // Strobe mode (0-7 enum)
SetRandomSeed(float NewSeed);                      // Random seed (different per light)
SetLightingColor(FLinearColor NewColor);            // Color
SetLightingZoom(float NewZoom);                    // Zoom [0,1]
SetLightingFrost(float NewFrost);                  // Frost [0,1]
SetLightingIris(float NewIris);                    // Iris [0,1] (1=fully open)
SetLightingTexture(UTexture2D* Texture);           // Set texture
SetLightingRotate(float Rotate, float Infinite);   // Rotation
SetLightingVisibility(bool bVisible);              // Visibility
SetComponentDimmer(float NewDimmer);               // Component intensity control
```

**Intensity calculation formula**:
```
FinalIntensity = pow(DMXDimmer, DimmerCurveExponent) × ComponentDimmer × StrobeMultiplier
```

### 5.4 USuperSpotComponent Details (Spotlight Component)

Inherits `USuperLightingComponent`, adds UE SpotLight real illumination.

**Inheritance chain**: `USuperLightingComponent → USuperSpotComponent`

**New properties**:

| Property | Type | Description |
|------|------|------|
| `YSpotLight` | `USilentSpotLightComponent*` | Internal SpotLight (auto-created) |
| `ZoomRange` | `FVector2D` | Beam angle range (default 1°~10°) |
| `bDisableLightFunction` | `bool` | Disable light function (use Tick for strobe control) |

**Zoom mapping**: `DMX[0,1] → Lerp(ZoomRange.X, ZoomRange.Y)` → SpotLight OuterConeAngle

**Applicable fixtures**: Wash lights, fill lights, fixtures without visible light columns

**Blueprint initialization**:
```
OnConstruction / BeginPlay:
    SetLightingDefaultValue(MaxDistance, MaxIntensity, Scatter, Shadow, SpotIntensity, LensIntensity)
```

**Blueprint Tick control**:
```
SuperDMXTick:
    SetLightingIntensity(DmxDimmer)
    SetLightingColor(Color)
    SetLightingZoom(DmxZoom)
    SetLightingFrost(DmxFrost)
```

### 5.5 USuperBeamComponent Details (Beam Component)

Inherits `USuperSpotComponent`, adds visible volumetric beam (light column).

**Inheritance chain**: `USuperSpotComponent → USuperBeamComponent`

**New properties**:

| Property | Type | Description |
|------|------|------|
| `YStaticMeshBeam` | `UStaticMeshComponent*` | Beam cone mesh |
| `BeamMaterial` / `DynamicMaterialBeam` | Material | Beam volume material |

**Initialization** (OnConstruction / BeginPlay):
```cpp
// Call base class initialization first
SetLightingDefaultValue(MaxDist, MaxIntensity, Scatter, Shadow, SpotIntensity, LensIntensity);

// Then set beam parameters
SetBeamDefaultValue(
    float MaxLightDistance,    // Beam length (cm)
    float MaxIntensity,        // Max intensity
    float AtmosphericDensity,  // Atmospheric density (beam attenuation, 0.03 recommended)
    float LensRadius,          // Lens radius (spot size, default 10)
    float BeamQuality,         // Beam quality (step count, higher = smoother)
    float FogInfluence,        // Fog intensity
    float FogSpeed,            // Fog flow speed
    float BeamIntensity        // Beam intensity multiplier (default 1)
);
```

**Beam-specific control**:
```cpp
SetBeamTexture(Texture, NumGobos, GoboIndex, GoboSpeed, ShakeSpeed);  // Gobo texture
SetBeamPrism(Facets, Radius, Scale, Rotation, RotSpeed);              // Prism
SetBeamFocus(Focus);                                                   // Focus [0=sharp, 1=blurry]
SetColorTexture(Texture, NumColors, Index, Speed);                     // Color wheel 1
SetColorTexture2(...);                                                  // Color wheel 2
SetColorTexture3(...);                                                  // Color wheel 3
```

**Applicable fixtures**: Beam lights, Spot lights (fixtures that need visible light columns)

### 5.6 USuperCuttingComponent Details (Cutting Component)

Inherits `USuperBeamComponent`, adds four-blade cutting (shutter).

**Inheritance chain**: `USuperBeamComponent → USuperCuttingComponent`

**Cutting control** (8 parameters, corresponding to both ends of 4 blades):
```cpp
SetCuttingValue(
    float UpperLeftY,     // Upper left Y
    float LowerLeftY,     // Lower left Y
    float TopRightY,      // Upper right Y
    float BottomRightY,   // Lower right Y
    float TopLeftX,       // Upper left X
    float BottomLeftX,    // Lower left X
    float TopRightX,      // Upper right X
    float BottomRightX    // Lower right X
);

// Cutting system rotation (synchronized with spot rotation)
SetLightingRotate(float Rotate, float InfiniteRotation);
```

**Cutting channel naming convention**: `A1, A2, A3, A4` (A side) + `B1, B2, B3, B4` (B side) + `ShaperRot`

**Applicable fixtures**: Profile lights (fixtures that need shutter cutting)

### 5.7 USuperRectComponent Details (Rectangular Area Light Component)

Inherits `USuperLightingComponent`, uses UE RectLight for area light sources.

**Inheritance chain**: `USuperLightingComponent → USuperRectComponent`

**New properties**:

| Property | Type | Description |
|------|------|------|
| `YRectLight` | `USilentRectLightComponent*` | Internal rectangular light source |
| `SourceWidth` | `float` | Area light width (cm) |
| `SourceHeight` | `float` | Area light height (cm) |
| `BarnDoorAngle` | `float` | Barn door angle (0-80°) |
| `BarnDoorLength` | `float` | Barn door length (cm) |

**Applicable fixtures**: LED panel lights, Wash lights (soft effect), area light sources

**Initialization and control** (consistent with `USuperLightingComponent`):
```
OnConstruction / BeginPlay:
    SetLightingDefaultValue(MaxDist, MaxIntensity, Scatter, Shadow)

SuperDMXTick:
    SetLightingIntensity(DmxDimmer)
    SetLightingColor(Color)
```

### 5.8 USuperEffectComponent Details (Effect Component)

Independent effect plane component, using material-driven LED effects.

**Inheritance chain**: `USceneComponent → USuperEffectComponent`

**Key properties**:

| Property | Type | Description |
|------|------|------|
| `YStaticMeshEffect` | `UStaticMeshComponent*` | Effect display mesh |
| `StaticMeshEffect` | `UStaticMesh*` | Effect mesh model (set in Blueprint) |
| `EffectMaterialNo` | `int` | Material number |
| `bTransparent` | `bool` | Whether to use transparent material |
| `EffectTransform` | `FTransform` | Effect component transform |
| `ComponentDimmer` | `float` | Component-level intensity control |

**Initialization** (OnConstruction / BeginPlay):
```cpp
SetEffectMaterial(float NewIntensity = 1.0f);  // Create dynamic material and bind
```

**Control functions**:
```cpp
// Individual control (recommended for fine control)
SetEffectIntensity(float Intensity);
SetEffectStrobe(float Strobe);
SetEffectStrobeMode(float StrobeMode);
SetEffectColor(FLinearColor Color);
SetEffectControl(float Effect, float Speed, float Width);  // Effect selection + speed + width
SetComponentDimmer(float NewDimmer);

// One-shot control (convenient method)
SetEffectsControl(
    float Intensity,       // Intensity
    float Strobe,          // Strobe
    FLinearColor Color,    // Color
    float Direction,       // Direction
    float Effect,          // Effect index (0-10)
    float Speed,           // Speed (-10 to 10)
    float Width            // Width (0-10)
);
```

**Effect index mapping** (Effect parameter 0-10):
| Value | Effect |
|----|------|
| 0 | No effect (solid color) |
| 1-10 | Built-in GPU effects (flow, chase, breathing, etc.) |

**Applicable fixtures**: LED strips, effect lights, decorative lights

### 5.9 USuperMatrixComponent Details (Matrix Component)

Segmented control matrix pixel component, independent color per segment.

**Inheritance chain**: `USceneComponent → USuperMatrixComponent`

**Key properties**:

| Property | Type | Description |
|------|------|------|
| `SegCount` | `int32` | Segment count (≤200) |
| `bUseUAxis` | `bool` | true=slice by U direction, false=by V direction |
| `StaticMeshMatrix` | `UStaticMesh*` | Matrix mesh model |
| `bTransparent` | `bool` | Transparent material |
| `MatrixTransform` | `FTransform` | Matrix transform |
| `SegmentLightType` | `ESegmentLightType` | Segment light type (None/PointLight/SpotLight) |
| `SegmentLightIntensity` | `float` | Segment light intensity |
| `SegmentLightRadius` | `float` | Segment light attenuation radius |
| `ComponentDimmer` | `float` | Component-level intensity control |

**Initialization** (OnConstruction / BeginPlay):
```cpp
SetMatrixMaterial(float NewIntensity = 1.0f);  // Create material + rebuild segment lights
```

**Control functions**:
```cpp
SetSegmentColor(int32 Index, FLinearColor RGB);  // Set single pixel color
SetMatrixStrobe(float Strobe);                    // Matrix strobe
SetMatrixStrobeMode(float StrobeMode);            // Matrix strobe mode
SetMatrixIntensity(float Intensity);              // Matrix intensity
SetComponentDimmer(float NewDimmer);              // Component intensity control

// Light source management
RebuildSegmentLights();     // Rebuild when type or count changes
DestroyAllSegmentLights();  // Destroy all segment lights
UpdateLightParameters();    // Update light parameters (no rebuild needed)
```

**Matrix pixel color setting modes** (called at Actor level):
```cpp
// Single pixel setting (specified by Index)
SetMatrixColorRGBSingle(DmxR, DmxG, DmxB, Index, MatrixComp);

// Multi-pixel batch setting (GET_SUPER_DMX_MATRIX_VALUE macro iteration)
SetMatrixColorRGBMultiple(DmxR, DmxG, DmxB, MatrixComp);
```

### 5.10 USuperLiftComponent Details (Lift Component)

Concise Z-axis lift control component, actively called by parent Actor.

**Inheritance chain**: `USceneComponent → USuperLiftComponent`

**Control function**:
```cpp
void SetLiftZ(
    float InPosZ,       // DMX normalized value [0,1]
    float LiftRange,    // Lift range (cm)
    float LiftSpeed     // Interpolation speed (0-10)
);
```

**Internal calculation**: `DMX[0,1] → Lerp(0, LiftRange) → FInterpTo smoothing → SetRelativeLocation.Z`

**Usage** (Blueprint/C++):
```
SuperDMXTick:
    SetLiftZ(SuperLift, DmxLiftZ)  // Actor-level function wrapper
```

### 5.11 USuperLaserProComponent Details (Laser Pro Component)

Implements point data-driven laser line visualization via procedural mesh.

**Inheritance chain**: `USceneComponent → USuperLaserProComponent`

**Key properties**:

| Property | Type | Description |
|------|------|------|
| `BeamLength` | `float` | Laser projection distance (100-50000 cm) |
| `ProjectionAngle` | `float` | Projection angle range (1-90°) |
| `LaserWidth` | `float` | Laser line width (0.1-20 cm) |
| `CoreSharpness` | `float` | Core sharpness [1,10] |
| `DepthFade` | `float` | Depth fade [0,1] |
| `Dim` | `float` | Self-illumination intensity [1,10] |
| `OpacityScale` | `float` | Opacity scale [0,1] |
| `FogSpeed` / `FogInfluence` | `float` | Fog effect |
| `SpotDimmer` | `float` | Spot brightness [0,5] |
| `bEnableCollision` | `bool` | Collision occlusion detection |
| `ComponentDimmer` | `float` | Component-level intensity control |

**Control functions**:
```cpp
SetLaserPoints(const TArray<FLaserPoint>& Points);  // Set laser point data (per frame)
RebuildMesh();                                        // Force mesh rebuild
SetLaserVisibility(bool bVisible);                   // Visibility
SetComponentDimmer(float NewDimmer);                 // Intensity control
```

**Point data format** (`FLaserPoint`):
```
X, Y: Position [-1,1] (normalized to ProjectionAngle range)
R, G, B: Color [0,1]
Z: Beam marker (>0 means beam point)
```

---

## 6. C++ Fixture Development Workflow

### 6.1 Step Overview

1. **Create fixture library asset** — Define DMX channel mapping
2. **Create C++ Actor class** — Inherit appropriate base class
3. **Add components** — Create lighting/effect components in constructor
4. ~~**Implement initialization** — Call SetLightingDefaultValue in OnConstruction + BeginPlay (once each)~~ → **Auto component initialization**: On component registration, `OnRegister()` automatically calls `SetLightingMaterial()` and `SetLightingDefaultValue()`
5. **Implement DMX control** — Call control functions in Tick
6. **Create Blueprint subclass** — Set models and materials
7. **Test** — Place in scene and connect DMX

### 6.2 C++ Actor Skeleton

```cpp
// MyCustomLight.h
#pragma once
#include "CoreMinimal.h"
#include "LightActor/SuperStageLight.h"
#include "MyCustomLight.generated.h"

UCLASS(meta = (DisplayName = "MyCustomLight"))
class SUPERSTAGE_API AMyCustomLight : public ASuperStageLight
{
    GENERATED_BODY()
    
public:
    AMyCustomLight();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // DMX attribute references (configurable in Blueprint subclass)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E.DMXMapping")
    FSuperDMXAttribute DmxDimmer;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E.DMXMapping")
    FSuperDMXAttribute DmxStrobe;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E.DMXMapping")
    FSuperDMXAttribute DmxColorR;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E.DMXMapping")
    FSuperDMXAttribute DmxColorG;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E.DMXMapping")
    FSuperDMXAttribute DmxColorB;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E.DMXMapping")
    FSuperDMXAttribute DmxZoom;
};
```

```cpp
// MyCustomLight.cpp
#include "MyCustomLight.h"

AMyCustomLight::AMyCustomLight()
{
    // Components already created in ASuperStageLight constructor
    // Custom components or default value modifications can be added here
    
    // Enable needed channel switches
    bDimmer = true;
    bStrobe = true;
    bColor = true;
    bZoom = true;
}

void AMyCustomLight::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    
    // Initialize beam component (executes when placed in editor)
    SetLightingDefaultValue(SuperLighting);
}

void AMyCustomLight::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize again at runtime (ensures material correctness)
    SetLightingDefaultValue(SuperLighting);
}

void AMyCustomLight::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // DMX control (do not call SetLightingDefaultValue here)
    SetLightingIntensity(DmxDimmer);
    SetLightingStrobe(DmxStrobe);
    SetLightingColorRGB(DmxColorR, DmxColorG, DmxColorB);
    SetLightingZoom(DmxZoom);
}
```

### 6.3 Channel Switch System

`ASuperStageLight` uses `bChannelEdit` + individual channel switches to control property visibility in the editor:

```cpp
// Channel switches — EditDefaultsOnly, set in Blueprint subclass Class Defaults
bChannelEdit = true;   // Master switch (must be true to edit other switches)
bDimmer = true;        // Intensity channel
bStrobe = true;        // Strobe channel
bColor = true;         // RGB Color
bColorWheel = true;    // Color wheel
bZoom = true;          // Zoom
bFrost = true;         // Frost
bIris = true;          // Iris
bGobo1 = true;         // Gobo wheel 1
bGobo2 = true;         // Gobo wheel 2
bGobo_Rot = true;      // Gobo rotation
bPrism1 = true;        // Prism 1
bPrism_Rot = true;     // Prism rotation
bEffect = true;        // Effect
bCuttingChannel = true;// Cutting
bPan = true;           // Pan
bTilt = true;          // Tilt
bPTSpeed = true;       // PT speed
bPanRot = true;        // Pan infinite rotation
bTiltRot = true;       // Tilt infinite rotation
bInPosZ = true;        // Lift
```

---

## 7. Blueprint Fixture Development Workflow

### 7.1 Simplest Blueprint Fixture (5 steps)

**Step 1: Create fixture library**
1. Content Browser → Right-click → SuperStage → SuperFixtureLibrary
2. Double-click to open, add attributes (Dimmer ch1, Pan ch3/4, Tilt ch5/6, R ch7, G ch8, B ch9, Zoom ch10)
3. Save

**Step 2: Create Blueprint Actor**
1. Content Browser → Right-click → Blueprint Class
2. Select parent class `SuperStageLight`
3. Name it `BP_MyWashLight`

**Step 3: Set components and models**
1. Open Blueprint editor
2. Select existing components in Components panel
3. Set SM_Hook/SM_Base/SM_Rocker/SM_Hard's Static Mesh
4. Select SuperLighting component (USuperBeamComponent or its subclass), set lens model etc.

**Step 4: Configure channel switches and DMX mapping**
1. Open Class Defaults
2. `ChannelEdit = true`
3. Check needed channels: Dimmer, Pan, Tilt, Color, Zoom etc.
4. Set `FixtureLibrary` property pointing to fixture library asset from step 1

**Step 5: Implement SuperDMXTick**
1. Add `Super DMX Tick` event in Event Graph
2. Wire up calls to control functions:

```
Event OnConstruction
    └─ SetLightingDefaultValue (SuperLighting)  ← Initialize when placed in editor

Event BeginPlay
    └─ SetLightingDefaultValue (SuperLighting)  ← Initialize at runtime

Event SuperDMXTick
    ├─ Pan (DmxPan)
    ├─ Tilt (DmxTilt)
    │
    ├─ SetLightingIntensity (DmxDimmer)
    ├─ SetLightingStrobe (DmxStrobe)
    ├─ SetLightingColorRGB (DmxR, DmxG, DmxB)
    └─ SetLightingZoom (DmxZoom)
```

### 7.2 FSuperDMXAttribute Configuration in Blueprint

In Blueprint, each DMX attribute variable needs three fields configured:

- **InstanceIndex**: Module index (usually 0)
- **AttribName**: Attribute name (must exactly match fixture library)
- **DMXChannelType**: Precision selection (8-Bit / 16-Bit / 24-Bit)

**Example configuration** (set in Blueprint variable default values):

```
DmxDimmer:
  InstanceIndex = 0
  AttribName = "Dimmer"
  DMXChannelType = Fine (16-bit)

DmxPan:
  InstanceIndex = 0
  AttribName = "Pan"
  DMXChannelType = Fine (16-bit)

DmxColorR:
  InstanceIndex = 0
  AttribName = "ColorR"
  DMXChannelType = Conarse (8-bit)
```

### 7.3 Blueprint Initialization Pattern

It is recommended to call initialization functions once each in `OnConstruction` (construction script) and `BeginPlay` (game start):

```
Event OnConstruction
    ├─ SetLightingDefaultValue(SuperLighting)    ← Editor placement/property modification
    ├─ SetLightingDefaultTexture(GoboTexture)    ← Optional
    └─ SetEffectDefaultValue(SuperEffect)        ← If effect component exists

Event BeginPlay
    ├─ SetLightingDefaultValue(SuperLighting)    ← Runtime
    ├─ SetLightingDefaultTexture(GoboTexture)    ← Optional
    └─ SetEffectDefaultValue(SuperEffect)        ← If effect component exists
```

**Key**:
- ~~`SetLightingDefaultValue` must be called before control functions in `SuperDMXTick`, otherwise material is not initialized~~
- > **26Q2.2 Update**: Component default parameters have been migrated into components, auto-initialized by `OnRegister()`. On component registration, `SetLightingMaterial()` is automatically called to create material, then `SetLightingDefaultValue()` to set defaults. **No need to manually call initialization functions in the Actor**.
- If custom default parameter values are needed, simply set properties like `MaxLightIntensity`, `ZoomRange` on the component, and the component will auto-apply.

---

## 8. DMX Channel Reading API

### 8.1 High-level Interface (Recommended)

These functions automatically handle normalization, multi-precision, and permission checks:

```cpp
// Read normalized value [0,1] (auto-selects precision based on DMXChannelType)
void GetSuperDmxAttributeValue(const FSuperDMXAttribute& DmxAttribute, float& InOutDefault) const;

// Read normalized value (without [0,1] conversion)
void GetSuperDmxAttributeValueNoConversion(const FSuperDMXAttribute& DmxAttribute, float& InOutDefault) const;

// Read RGB color
void GetSuperDMXColorValue(const FSuperDMXAttribute& DmxRed, const FSuperDMXAttribute& DmxGreen, 
    const FSuperDMXAttribute& DmxBlue, FLinearColor& OutColor) const;

// Read DMX raw value [0-255]
void GetSuperDmxAttributeRawValue(const FSuperDMXAttribute& DmxAttribute, int32& OutRawValue) const;
```

### 8.2 Reading by Instance Index (Mid-level)

```cpp
// 8-bit [0..255]
int32 GetAttributeRaw8ByIndex(int32 InstanceIndex, FName AttribName, int32 DefaultValue = 0) const;

// 16-bit [0..65535]
int32 GetAttributeRaw16ByIndex(int32 InstanceIndex, FName AttribName, int32 DefaultValue = 0) const;

// 24-bit [0..16777215]
int32 GetAttributeRaw24ByIndex(int32 InstanceIndex, FName AttribName, int32 DefaultValue = 0) const;

// Query bit depth
int32 GetAttributeBitDepthByIndex(int32 InstanceIndex, FName AttribName) const;
```

### 8.3 Raw Channel Reading (Low-level)

```cpp
// Directly read 8-bit value by channel offset (1-based)
float GetChannelValue(int32 Address = 1, float DefaultValue = 1) const;
```

### 8.4 Matrix Batch Reading

```cpp
// Iterate same-named attribute across all modules (8-bit)
void GetMatrixAttributeRaw(FName AttribName, TArray<float>& OutValues, 
    float DefaultValue = 0, bool bSortByPatch = true) const;

// Iterate same-named attribute across all modules (16-bit)
void GetMatrixAttributeRaw16(FName AttribName, TArray<float>& OutValues, 
    float DefaultValue = 0, bool bSortByPatch = true) const;
```

### 8.5 Matrix Macros

In C++, macros are recommended to simplify matrix iteration:

```cpp
// Basic matrix iteration (provides LightIndex and DefaultValue)
GET_SUPER_DMX_MATRIX_VALUE(DmxAttribute, DefaultValue,
{
    // LightIndex: current pixel index
    // DefaultValue: current pixel's normalized value
    SomeComponent->SetIntensity(LightIndex, DefaultValue);
});

// Matrix iteration with sub-attributes (additionally provides DmxRawValue, AttrDef, SubAttr)
GET_SUPER_DMX_MATRIX_VALUE_WITH_SUBATTR(DmxAttribute, DefaultValue,
{
    // DmxRawValue: DMX raw value [0-255]
    // AttrDef: attribute definition pointer
    // SubAttr: matched sub-attribute pointer
    if (SubAttr && SubAttr->StrobeMode == EStrobeMode::Linear)
    {
        // Handle strobe
    }
});

// RGB matrix iteration
GET_SUPER_DMX_MATRIX_RGB(DmxR, DmxG, DmxB, OutColor,
{
    // LightIndex: current pixel index
    // OutColor: FLinearColor
    MatrixComp->SetSegmentColor(LightIndex, OutColor);
});
```

### 8.6 Sub-attribute Queries

```cpp
// Find attribute definition
const FSuperDMXAttributeDef* AttrDef = FindAttributeDef(InstanceIndex, AttribName);

// Find matching sub-attribute by DMX raw value
const FSubAttribute* SubAttr = AttrDef->FindSubAttribute(DmxRawValue);

// Find matching channel set by DMX raw value
const FChannelSet* ChannelSet = AttrDef->FindChannelSet(DmxRawValue);

// Normalized position within sub-attribute
float NormPos = SubAttr->GetNormalizedPosition(DmxRawValue);

// Physical value mapping within sub-attribute
float Physical = SubAttr->GetMappedPhysical(DmxRawValue);
```

---

## 9. Color System Details

### 9.1 Direct RGB Control

```cpp
// Single light
SetLightingColorRGB(FSuperDMXAttribute DmxR, FSuperDMXAttribute DmxG, FSuperDMXAttribute DmxB);

// Matrix (per-pixel independent)
SetLightingColorRGBMatrix(FSuperDMXAttribute DmxR, FSuperDMXAttribute DmxG, FSuperDMXAttribute DmxB);
```

### 9.2 RGBW (White Channel Overlay)

```cpp
SetLightingColorRGBW(DmxR, DmxG, DmxB, DmxW);
SetLightingColorRGBWMatrix(DmxR, DmxG, DmxB, DmxW);
```

White channel added to RGB: `FinalColor = RGB + W * White`

### 9.3 HSV / HS

```cpp
SetLightingColorHSV(DmxH, DmxS, DmxV);  // Hue / Saturation / Value
SetLightingColorHS(DmxH, DmxS);          // Hue / Saturation (value controlled by Dimmer)
```

### 9.4 Color Wheel

```cpp
// Single color wheel
SetLightingColorWheel(DmxAttribute, ColorAtlas);

// Color wheel + CMY mixing
SetLightingColorWheelAndCMY(DmxWheel, DmxC, DmxM, DmxY, ColorAtlas);

// Dual/triple color wheel
SetLightingColorWheel2(Dmx1, Dmx2, Atlas1, Atlas2);
SetLightingColorWheel3(Dmx1, Dmx2, Dmx3, Atlas1, Atlas2, Atlas3);

// Color wheel + CMY + CTO
SetLightingColorWheel2AndCMYAndCTO(Dmx1, Dmx2, DmxC, DmxM, DmxY, DmxCTO, 
    WarmTemp, CoolTemp, Atlas1, Atlas2);
```

**Color wheel working principle**:
- Color-category attribute sub-attributes in the fixture library define each color slot
- Each `FChannelSet` contains `Color` and `ColorIndex`
- Actor finds matching ChannelSet by DMX raw value
- Uses ColorAtlas texture + ColorIndex for precise color mixing

### 9.5 Color Temperature

```cpp
// Single channel color temperature
SetLightingColorTemperature(DmxAttribute, WarmTemperature=1700, CoolTemperature=12000);

// Cool-warm dual channel
SetLightingCoolWarmMix(DmxCool, DmxWarm, CoolTemperature=6500, WarmTemperature=3200);
```

### 9.6 Dynamic Multi-channel Color Mixing

```cpp
// Supports arbitrary color channel combinations (RGBW/RGBA/RGBL/RGBLAM etc.)
TArray<FSuperColorChannel> Channels;
Channels.Add(FSuperColorChannel(DmxR, FLinearColor::Red));
Channels.Add(FSuperColorChannel(DmxG, FLinearColor::Green));
Channels.Add(FSuperColorChannel(DmxB, FLinearColor::Blue));
Channels.Add(FSuperColorChannel(DmxW, FLinearColor::White));
Channels.Add(FSuperColorChannel(DmxAmber, FLinearColor(1, 0.75, 0))); // Amber

SetLightingColorMix(Channels);
SetLightingColorMixWithCTO(Channels, DmxCTO, CoolTemp, WarmTemp);
```

### 9.7 Spot Fill Light Color

All main light source color functions have corresponding Spot fill light versions:

```cpp
SetSpotColorRGB(DmxR, DmxG, DmxB);
SetSpotColorRGBW(DmxR, DmxG, DmxB, DmxW);
SetSpotCoolWarmMix(DmxCool, DmxWarm, CoolTemp, WarmTemp);
// ... and Matrix versions
```

---

## 10. Gobo Wheel, Color Wheel, and Prism System

### 10.1 Core Data Structure: FChannelSet

`FChannelSet` is the **shared data carrier** for gobo wheel, color wheel, and prism systems, defined within a fixture library attribute's sub-attribute (`FSubAttribute`).

```
FSuperDMXAttributeDef (attribute)
 └─ SubAttributes[] (sub-attributes)
     └─ ChannelSets[] (channel set list)
         ├─ Name            — Slot name (e.g., "Open", "Gobo1", "Red")
         ├─ DmxMin/DmxMax   — DMX range
         ├─ PhysicalRange   — Physical value mapping range
         ├─ GoboMode        — Gobo mode (Static/Scrolling/Shake)
         ├─ Texture         — Gobo texture (UTexture2D*)
         ├─ Color           — Color value (FLinearColor)
         ├─ ColorIndex      — Color index (for Color Wheel positioning)
         └─ PrismSelection  — Prism selection (EPrismLayerSelect, points to USuperPrismPreset layer index)
```

**Each attribute type only uses a subset of fields**:

| Attribute Type | Used FChannelSet Fields | Unused Fields |
|---------|----------------------|------------|
| **Gobo** | GoboMode, Texture, DmxMin/Max | Color, PrismSelection |
| **Color** | Color, ColorIndex, DmxMin/Max | Texture, GoboMode, PrismSelection |
| **Prism** | PrismSelection, DmxMin/Max | Texture, Color, GoboMode |

---

### 10.2 Gobo Wheel

#### 10.2.1 Gobo Configuration in Fixture Library

Each gobo occupies one `FChannelSet` slot and needs to configure:

```
Attribute: Gobo1, Category: Gobo, Coarse: 7
 └─ SubAttribute[0]: DmxMin=0, DmxMax=127 (static gobo selection)
     ├─ ChannelSet[0]: Name="Open",   DmxMin=0,  DmxMax=9,  GoboMode=Static, Texture=null
     ├─ ChannelSet[1]: Name="Gobo1",  DmxMin=10, DmxMax=19, GoboMode=Static, Texture=T_Gobo_Star
     ├─ ChannelSet[2]: Name="Gobo2",  DmxMin=20, DmxMax=29, GoboMode=Static, Texture=T_Gobo_Circle
     ├─ ChannelSet[3]: Name="Gobo3",  DmxMin=30, DmxMax=39, GoboMode=Static, Texture=T_Gobo_Dot
     ├─ ChannelSet[4]: Name="Gobo4",  DmxMin=40, DmxMax=49, GoboMode=Static, Texture=T_Gobo_Line
     ├─ ChannelSet[5]: Name="Gobo5",  DmxMin=50, DmxMax=59, GoboMode=Static, Texture=T_Gobo_Flower
     └─ ChannelSet[6]: Name="Gobo6",  DmxMin=60, DmxMax=69, GoboMode=Static, Texture=T_Gobo_Cross
 └─ SubAttribute[1]: DmxMin=128, DmxMax=189 (shake gobo)
     ├─ ChannelSet[0]: Name="Shake1", DmxMin=128, DmxMax=138, GoboMode=Shake, Texture=T_Gobo_Star
     ├─ ... (shake gobos, GoboMode=Shake)
 └─ SubAttribute[2]: DmxMin=190, DmxMax=255 (scrolling mode)
     └─ (no ChannelSet, PhysicalRange=(-1,1) representing speed range)
```

#### 10.2.2 EGoboMode Enum

| Mode | Behavior | ChannelSet Requirement |
|------|------|----------------|
| `Static` | Fixed display of specified gobo | Each slot contains Texture |
| `Scrolling` | Gobo continuously scrolls | No ChannelSet, use PhysicalRange for speed mapping |
| `Shake` | Gobo shakes at current position | Each slot contains Texture, DMX value maps to shake frequency (1-10Hz) |

#### 10.2.3 Gobo Atlas

At runtime, individual Gobo textures are **not used directly**; instead, multiple gobos are merged into a single **horizontal atlas texture** passed to the material.

**Atlas specifications**:
- **Size**: 4096 × 256 pixels
- **Arrangement**: Horizontally divided, each gobo occupies `4096 / N` pixels width
- **Format**: BGRA8, no Mipmap, Linear (sRGB=false)

**Naming convention**: `LTA_{library name without SL_ prefix}_{attribute name}.uasset`
- Example: Library `SL_Acme_XP-380Beam` + attribute `Gobo1` → `LTA_Acme_XP-380Beam_Gobo1`

**Build method** (recommended to use editor tool):
1. Open **GOBO Atlas Builder** (`SGoboAtlasBuilder`)
2. Select fixture library asset → input attribute name (e.g., `Gobo1`) → search
3. Tool automatically extracts all `Texture` from library's `SubAttribute[0].ChannelSets`
4. Click "Generate Atlas" → output to library's same directory

**Build flow (internal)**:
```
1. Iterate Modules[].AttributeDefs[], find attribute matching AttribName
2. Take first SubAttribute's ChannelSets[]
3. Extract each ChannelSet.Texture (skip null)
4. Scale and draw all textures into corresponding positions on 4096×256 atlas
5. Create UTexture2D asset and save
```

**Runtime data flow**:
```
DMX raw value → FindSubAttribute(DmxValue) → FindChannelSet(DmxValue)
  → Compute GoboIndex (index of ChannelSet in array)
  → SuperBeam->SetBeamTexture(GoboAtlas, NumGobos, GoboIndex, Speed, ShakeSpeed)
  → Material: floor(GoboIndex) positions sub-gobo in atlas
```

#### 10.2.4 Gobo Rotation (GoboRot)

Gobo rotation channel uses `CalcGoboRotation` calculation, supporting two modes:

| Mode | Condition | Mapping |
|------|------|------|
| **Static rotation** | SubAttribute.PhysicalRange == (0,0) | DMX normalized × 360° |
| **Infinite rotation** | SubAttribute.PhysicalRange != (0,0) | DMX → Lerp(PhysicalRange.X, PhysicalRange.Y) → rotation speed |

Fixture library configuration example:
```
Attribute: Gobo1Rot, Category: Gobo, Coarse: 8
 └─ SubAttribute[0]: DmxMin=0,   DmxMax=127, PhysicalRange=(0,0)     → Static rotation 0-360°
 └─ SubAttribute[1]: DmxMin=128, DmxMax=189, PhysicalRange=(-360,0)  → CCW infinite rotation
 └─ SubAttribute[2]: DmxMin=190, DmxMax=194, PhysicalRange=(0,0)     → Stop
 └─ SubAttribute[3]: DmxMin=195, DmxMax=255, PhysicalRange=(0,360)   → CW infinite rotation
```

#### 10.2.5 Using Gobo in Blueprint

**Wiring in SuperDMXTick**:
```
// Single gobo wheel
SetBeamGobo1(DmxGobo1, DmxGobo1Rot, Gobo1Atlas)

// Dual gobo wheel (auto priority selection: non-zero preferred, all zero uses Gobo1)
SetBeamGobo2(DmxGobo1, DmxGobo1Rot, DmxGobo2, DmxGobo2Rot, Atlas1, Atlas2)

// Triple gobo wheel
SetBeamGobo3(DmxGobo1, DmxGobo1Rot, DmxGobo2, DmxGobo2Rot,
    DmxGobo3, DmxGobo3Rot, Atlas1, Atlas2, Atlas3)
```

**Parameter descriptions**:
- `DmxGoboN` — Gobo selection channel (`FSuperDMXAttribute`, points to GoboN attribute in library)
- `DmxGoboNRot` — Gobo rotation channel (`FSuperDMXAttribute`, points to GoboNRot attribute)
- `GoboNAtlas` — Gobo atlas texture (`UTexture2D*`, generated by GoboAtlasBuilder)

**Blueprint variable setup**: In the fixture Blueprint's Class Defaults, create `UTexture2D*` type variables named `Gobo1Atlas` / `Gobo2Atlas`, assigned to atlas assets generated by the builder.

---

### 10.3 Color Wheel

#### 10.3.1 Color Configuration in Fixture Library

Each color occupies one `FChannelSet` slot and needs `Color` field configured:

```
Attribute: ColorWheel1, Category: Color, Coarse: 9
 └─ SubAttribute[0]: DmxMin=0, DmxMax=127 (static color selection)
     ├─ ChannelSet[0]: Name="Open",    DmxMin=0,  DmxMax=9,  Color=(1,1,1,1)        → White
     ├─ ChannelSet[1]: Name="Red",     DmxMin=10, DmxMax=19, Color=(1,0,0,1)        → Red
     ├─ ChannelSet[2]: Name="Orange",  DmxMin=20, DmxMax=29, Color=(1,0.5,0,1)      → Orange
     ├─ ChannelSet[3]: Name="Yellow",  DmxMin=30, DmxMax=39, Color=(1,1,0,1)        → Yellow
     ├─ ChannelSet[4]: Name="Green",   DmxMin=40, DmxMax=49, Color=(0,1,0,1)        → Green
     ├─ ChannelSet[5]: Name="Cyan",    DmxMin=50, DmxMax=59, Color=(0,1,1,1)        → Cyan
     ├─ ChannelSet[6]: Name="Blue",    DmxMin=60, DmxMax=69, Color=(0,0,1,1)        → Blue
     ├─ ChannelSet[7]: Name="Magenta", DmxMin=70, DmxMax=79, Color=(1,0,1,1)        → Magenta
     └─ ChannelSet[8]: Name="CTO",     DmxMin=80, DmxMax=89, Color=(1,0.8,0.6,1)    → Warm white
 └─ SubAttribute[1]: DmxMin=128, DmxMax=189, PhysicalRange=(-1,1) (forward flow)
 └─ SubAttribute[2]: DmxMin=190, DmxMax=255, PhysicalRange=(1,-1) (reverse flow)
```

#### 10.3.2 Color Atlas

Similar to Gobo Atlas, the color wheel also uses atlas textures.

**Atlas specifications**:
- **Size**: 256 × 16 pixels (much smaller than Gobo atlas)
- **Arrangement**: Horizontally divided, each color occupies `256 / N` pixels width
- **Format**: BGRA8, no Mipmap, sRGB=true, point sampling (TF_Nearest)
- **Compression**: TC_VectorDisplacementmap (lossless)

**Naming convention**: `CLA_{library name without SL_ prefix}_{attribute name}.uasset`
- Example: Library `SL_Robe_T1Profile` + attribute `ColorWheel1` → `CLA_Robe_T1Profile_ColorWheel1`

**Build method** (recommended to use editor tool):
1. Open **Color Atlas Builder** (`SColorAtlasBuilder`)
2. Select fixture library asset → input attribute name (e.g., `ColorWheel1`) → search
3. Tool automatically extracts all `Color` from library's `SubAttribute[0].ChannelSets`
4. Preview color arrangement → click "Generate Atlas"

**Build flow (internal)**:
```
1. Iterate Modules[].AttributeDefs[], find attribute matching AttribName
2. Take first SubAttribute's ChannelSets[]
3. Extract each ChannelSet.Color (FLinearColor)
4. Fill each color into corresponding columns of 256×16 atlas
5. Linear color → sRGB FColor conversion
6. Create UTexture2D asset and save
```

**Runtime data flow**:
```
DMX raw value → FindSubAttribute(DmxValue) → FindChannelSet(DmxValue)
  → Compute ColorIndex (index of ChannelSet in array)
  → NumColors = SubAttributes[0].ChannelSets.Num()
  → SuperLighting->SetColorTexture(ColorAtlas, NumColors, ColorIndex, ColorSpeed)
  → Material samples atlas texture by ColorIndex
```

#### 10.3.3 Color Wheel vs RGB Mixing

| Feature | Color Wheel | RGB Mixing |
|------|---------------------|---------|
| Channel count | 1 channel | 3-4 channels (R/G/B/W) |
| Color count | Limited (8-14 types) | Continuous (16.7M colors) |
| Principle | Mechanical color wheel / atlas sampling | Electronic color mixing |
| Applicable | Beam/Spot/Profile | Wash/Matrix |
| Supports flow | ✅ (SubAttribute PhysicalRange controls speed) | ❌ |

#### 10.3.4 Using Color Wheel in Blueprint

**Wiring in SuperDMXTick**:
```
// Single color wheel
SetLightingColorWheel(DmxColorWheel, ColorAtlas)

// Dual color wheel
SetLightingColorWheel2(DmxCW1, DmxCW2, ColorAtlas1, ColorAtlas2)

// Triple color wheel
SetLightingColorWheel3(DmxCW1, DmxCW2, DmxCW3, Atlas1, Atlas2, Atlas3)

// Color wheel + CMY
SetLightingColorWheelAndCMY(DmxCW, DmxC, DmxM, DmxY, ColorAtlas)

// Dual color wheel + CMY
SetLightingColorWheel2AndCMY(DmxCW1, DmxCW2, DmxC, DmxM, DmxY, Atlas1, Atlas2)

// Dual color wheel + CMY + CTO
SetLightingColorWheel2AndCMYAndCTO(DmxCW1, DmxCW2, DmxC, DmxM, DmxY, DmxCTO,
    WarmTemperature, CoolTemperature, Atlas1, Atlas2)

// Triple color wheel + CMY
SetLightingColorWheel3AndCMY(DmxCW1, DmxCW2, DmxCW3, DmxC, DmxM, DmxY, Atlas1, Atlas2, Atlas3)
```

**Multi color wheel priority rules**:
- Color wheel with non-zero DMX value takes priority
- Multiple non-zero: parameter order priority (CW1 > CW2 > CW3)
- All zero: uses first color wheel (usually white/Open)

---

### 10.4 Prism

#### 10.4.1 Prism Configuration in Fixture Library

Prism attributes use the `PrismSelection` (`EPrismLayerSelect`) field of `FChannelSet`, pointing to a layer index in the `USuperPrismPreset` data asset:

```
Attribute: Prism1, Category: Prism, Coarse: 12
 └─ SubAttribute[0]: DmxMin=0, DmxMax=127
     ├─ ChannelSet[0]: Name="Open",    DmxMin=0,  DmxMax=7,  PrismSelection=None     → Prism off
     ├─ ChannelSet[1]: Name="3-Facet", DmxMin=8,  DmxMax=39, PrismSelection=Prism1   → Prism layer 0
     ├─ ChannelSet[2]: Name="5-Facet", DmxMin=40, DmxMax=69, PrismSelection=Prism2   → Prism layer 1
     └─ ChannelSet[3]: Name="Linear",  DmxMin=70, DmxMax=127,PrismSelection=Prism3   → Prism layer 2
```

#### 10.4.2 EPrismLayerSelect Enum and USuperPrismPreset

**EPrismLayerSelect enum**:

| Value | Description |
|------|------|
| `None` (0) | Prism off (Open) |
| `Prism1` (1) | Points to `USuperPrismPreset::PrismLayers[0]` |
| `Prism2` (2) | Points to `USuperPrismPreset::PrismLayers[1]` |
| `Prism3` (3) | Points to `USuperPrismPreset::PrismLayers[2]` |

**USuperPrismPreset** is a prism data asset, created visually in the editor. Each layer (PrismLayer) defines a set of prism facets (`FSuperPrismFacet`) positions/scales, and at runtime `GetPositionTexture()` generates a 48×1 RGBA16F position lookup texture pushed to the material.

**Creation**: In Content Browser, right-click → SuperStage category → Prism Preset, double-click to open the visual editor and drag-edit facet positions.

**Built-in preset templates**: Circle / Line / Triangle / Square

#### 10.4.3 Prism Rotation (PrismRot)

Prism rotation shares the `CalcGoboRotation` logic with gobo rotation:

```
Attribute: PrismRot, Category: Prism, Coarse: 13
 └─ SubAttribute[0]: DmxMin=0,   DmxMax=127, PhysicalRange=(-360,0) → CCW
 └─ SubAttribute[1]: DmxMin=128, DmxMax=132, PhysicalRange=(0,0)    → Stop
 └─ SubAttribute[2]: DmxMin=133, DmxMax=255, PhysicalRange=(0,360)  → CW
```

#### 10.4.4 Runtime Lookup Flow

```
ASuperStageLight::SetBeamPrism(DmxPrism1, DmxPrism2, DmxPrismRot, InPrismPreset)
│
├─ 1. Read DMX normalized values
│   GetSuperDmxAttributeValue(DmxPrism1, Prism1)
│   GetSuperDmxAttributeValue(DmxPrism2, Prism2)
│   GetSuperDmxAttributeValue(DmxPrismRot, Prism_Rot)
│
├─ 2. Prism rotation (reuses CalcGoboRotation logic)
│   CalcGoboRotation(DmxPrismRot, Prism_Rot * 255)
│   → PrismRotation (static angle) + PrismRotationSpeed (infinite rotation speed)
│
├─ 3. Prism selection (priority: Prism1 > Prism2)
│   FindAttributeDef(InstanceIndex, "Prism1")
│     → FindSubAttribute(DmxValue)
│       → Iterate ChannelSets, find slot with Contains(DmxValue)
│         → Read PrismSelection (EPrismLayerSelect)
│           → PrismSelection == None → Prism off
│           → PrismSelection == Prism1/2/3 → LayerIndex = PrismSelection - 1
│
└─ 4. Push to component
   ├─ Valid layer found → SuperBeam->SetBeamPrism(InPrismPreset, LayerIndex, PrismRotation, PrismRotationSpeed)
   └─ All invalid → SuperBeam->SetBeamPrism(nullptr, 0, 0, 0)  // Prism off
```

**Dual prism priority**:
- First check Prism1, if corresponding `PrismSelection != None` then use it
- Otherwise check Prism2
- Both None → pass `nullptr` to disable prism effect

#### 10.4.5 Using Prism in Blueprint

**Function signature**:

```cpp
void SetBeamPrism(
    FSuperDMXAttribute DmxPrism1,
    FSuperDMXAttribute DmxPrism2,
    FSuperDMXAttribute DmxPrismRot,
    USuperPrismPreset* InPrismPreset = nullptr);
```

**Wiring in SuperDMXTick**:
```
SetBeamPrism(DmxPrism1, DmxPrism2, DmxPrismRot, MyPrismPreset)
```

| Parameter | Description |
|------|------|
| `DmxPrism1` | First prism selection channel |
| `DmxPrism2` | Second prism selection channel (pass empty FSuperDMXAttribute if none) |
| `DmxPrismRot` | Prism rotation channel |
| `InPrismPreset` | Prism preset asset (`USuperPrismPreset*`, optional, default nullptr) |

**Notes**:
- Prism does not need an atlas texture, but needs `USuperPrismPreset` data asset passed in
- Facet count/position/scale are defined by the preset asset; the fixture library only selects layer index via `PrismSelection`
- When `InPrismPreset` is `nullptr`, prism will be disabled (regardless of library configuration)

---

### 10.5 Atlas Builder Tools

#### 10.5.1 GOBO Atlas Builder (SGoboAtlasBuilder)

**How to open**: UE editor main menu → **Tools** → **GOBO Atlas Builder** (in SuperStageTools section)

**Interface**:
```
┌──────────────────────────────────────┐
│ Fixture Library: [select library]     │
│ Attribute Name: [Gobo1    ] [Search]  │
│ Found 7 gobo textures                 │
│ ┌──────────────────────────────────┐ │
│ │ [0] T_Gobo_Open (256x256)       │ │
│ │ [1] T_Gobo_Star (256x256)       │ │
│ │ [2] T_Gobo_Circle (256x256)     │ │
│ │ ...                              │ │
│ └──────────────────────────────────┘ │
│                        [Generate Atlas] │
└──────────────────────────────────────┘
```

**Operation flow**:
1. Select fixture library asset (`USuperFixtureLibrary`)
2. Input gobo attribute name (e.g., `Gobo1`, `Gobo2`)
3. Click "Search" — auto-extract all textures from library's first sub-attribute
4. Confirm gobo list is correct
5. Click "Generate Atlas" — output `LTA_xxx.uasset` to library's same directory

**Output texture settings**:

| Property | Value | Description |
|------|---|------|
| SRGB | false | Linear space |
| Compression | TC_Default | Default compression |
| MipGenSettings | NoMipmaps | No Mip |
| LODGroup | UI | UI group |
| NeverStream | true | Keep in memory |

#### 10.5.2 Color Atlas Builder (SColorAtlasBuilder)

**How to open**: UE editor main menu → **Tools** → **Color Atlas Builder** (in SuperStageTools section)

**Interface**:
```
┌──────────────────────────────────────┐
│ Fixture Library: [select library]     │
│ Attribute Name: [ColorWheel1] [Search]│
│ Found 9 colors (atlas size: 256x16)   │
│ ┌──────────────────────────────────┐ │
│ │ [■][■][■][■][■][■][■][■][■]     │ │ ← Color preview swatches
│ │  0  1  2  3  4  5  6  7  8      │ │
│ └──────────────────────────────────┘ │
│                        [Generate Atlas] │
└──────────────────────────────────────┘
```

**Operation flow**:
1. Select fixture library asset
2. Input color attribute name (e.g., `ColorWheel1`, `ColorWheel2`)
3. Click "Search" — auto-extract all `FLinearColor`
4. Preview color arrangement
5. Click "Generate Atlas" — output `CLA_xxx.uasset` to library's same directory

**Output texture settings**:

| Property | Value | Description |
|------|---|------|
| SRGB | true | sRGB space |
| Compression | VectorDisplacementmap | Lossless |
| MipGenSettings | NoMipmaps | No Mip |
| Filter | TF_Nearest | Point sampling, prevents color interpolation |
| NeverStream | true | Keep in memory |

#### 10.5.3 Atlas Builder Comparison

| Comparison Item | GOBO Atlas | Color Atlas |
|-------|-----------|---------|
| Data source | ChannelSet.Texture | ChannelSet.Color |
| Output size | 4096 × 256 | 256 × 16 |
| Output prefix | `LTA_` | `CLA_` |
| Color space | Linear (sRGB=false) | sRGB (sRGB=true) |
| Compression | TC_Default | TC_VectorDisplacementmap (lossless) |
| Sampling mode | Default (bilinear) | TF_Nearest (point sampling) |

---

### 10.6 Complete Configuration Example: Beam Light Gobo + Color + Prism

Using a 17-channel Beam light as an example, showing complete gobo/color/prism library configuration and Blueprint wiring.

**Library channel table** (gobo/color/prism related parts):

| Channel | Attribute Name | Category | Description |
|------|--------|------|------|
| 9 | ColorWheel1 | Color | Color wheel |
| 10 | Gobo1 | Gobo | Gobo wheel 1 selection |
| 11 | Gobo1Rot | Gobo | Gobo wheel 1 rotation |
| 12 | Prism1 | Prism | Prism selection |
| 13 | PrismRot | Prism | Prism rotation |

**Library configuration steps**:

1. **Color wheel**: Edit `ColorWheel1` attribute → enter SubAttribute[0] → add 9 ChannelSets, each set `Color` (FLinearColor) and `DmxMin/Max`
2. **Gobo wheel**: Edit `Gobo1` attribute → enter SubAttribute[0] → add 7 ChannelSets, each set `Texture` and `GoboMode=Static`, optionally add SubAttribute[1] with `GoboMode=Shake` shake segments
3. **Prism**: Edit `Prism1` attribute → enter SubAttribute[0] → add 4 ChannelSets, ChannelSet[0] set `PrismFacets=0` (Open), subsequent ones set different facet counts/radius/scale

**Atlas building**:
1. Open GOBO Atlas Builder → select library → input `Gobo1` → search → generate → get `LTA_xxx_Gobo1`
2. Open Color Atlas Builder → select library → input `ColorWheel1` → search → generate → get `CLA_xxx_ColorWheel1`

**Blueprint variables**:
- `Gobo1Atlas` (UTexture2D*) = `LTA_xxx_Gobo1`
- `ColorAtlas` (UTexture2D*) = `CLA_xxx_ColorWheel1`

**SuperDMXTick wiring**:
```
SetLightingColorWheel(DmxColorWheel1, ColorAtlas)      // Color wheel
SetBeamGobo1(DmxGobo1, DmxGobo1Rot, Gobo1Atlas)        // Gobo wheel
SetBeamPrism(DmxPrism1, None, DmxPrismRot, PrismPreset) // Prism (no second prism, pass empty, pass prism preset asset)
```

---

## 11. Cutting System

```cpp
// Four-blade cutting (8 DMX channels)
SetBeamCutting(DmxA1, DmxB1, DmxA2, DmxB2, DmxA3, DmxB3, DmxA4, DmxB4);

// Cutting rotation (Shaper rotation + Gobo rotation overlay)
SetBeamCuttingRotate(DmxShaperRot, DmxGoboRot);
```

**Cutting mapping** (distinguished by blade pair direction, A/B in same pair share same mapping range):
- Blade pair 1 (A1,B1) and blade pair 3 (A3,B3): `[0,1] → [0, 0.4]` (from open to half-closed)
- Blade pair 2 (A2,B2) and blade pair 4 (A4,B4): `[0,1] → [1, 0.6]` (from opposite side open to half-closed)
- Rotation overlay: `ShaperRot[defined by sub-attribute physical range] + GoboRot[0°, 360°]`

**Requires `USuperCuttingComponent`** (inherits from `USuperBeamComponent`).

---

## 12. Effect System

### 12.1 Effect Component Initialization

```cpp
// Single effect
SetEffectDefaultValue(USuperEffectComponent* NewSuperEffect);

// Effect matrix
SetEffectMatrixDefaultValue(TArray<USuperEffectComponent*> NewSuperEffectArray);
```

### 12.2 Effect Control

```cpp
SetEffectIntensity(DmxAttribute);   // Intensity
SetEffectStrobe(DmxAttribute);      // Strobe
SetEffectColor(DmxR, DmxG, DmxB);  // Color

// Effect selection (via LUT lookup)
SetEffect(DmxAttribute);            // Single effect component
SetEffectMatrix(DmxAttribute);      // Effect matrix

// Effect + speed separation
SetEffectWithSpeed(DmxEffect, DmxSpeed);
SetEffectMatrixWithSpeed(DmxEffect, DmxSpeed);
```

**Effect LUT**: DMX value [0-255] maps to `{Effect, Speed, Width}` parameter combination.

---

## 13. Matrix System

### 13.1 Matrix Component Control

```cpp
// Combined control (RGB + Strobe)
SetMatrixComponent(DmxR, DmxG, DmxB, DmxStrobe, USuperMatrixComponent*);

// White (single pixel / multi pixel)
SetMatrixWhiteSingle(DmxAttribute, Index, USuperMatrixComponent*);
SetMatrixWhiteMultiple(DmxAttribute, USuperMatrixComponent*);

// RGB Color (single pixel / multi pixel)
SetMatrixColorSingle(DmxR, DmxG, DmxB, Index, USuperMatrixComponent*);
SetMatrixColorMultiple(DmxR, DmxG, DmxB, USuperMatrixComponent*);

// Strobe
SetMatrixStrobe(DmxAttribute, USuperMatrixComponent*);

// Intensity
SetMatrixIntensity(DmxAttribute, USuperMatrixComponent*);

// Cool-warm mix
SetMatrixCoolWarmMixSingle(DmxCool, DmxWarm, CoolTemp, WarmTemp, Index, Matrix);
SetMatrixCoolWarmMixMultiple(DmxCool, DmxWarm, CoolTemp, WarmTemp, Matrix);

// RGBW (single pixel / multi pixel)
SetMatrixColorRGBWSingle(DmxR, DmxG, DmxB, DmxW, Index, Matrix);
SetMatrixColorRGBWMultiple(DmxR, DmxG, DmxB, DmxW, Matrix);

// RGB + CTO (single pixel / multi pixel)
SetMatrixColorRGBWithCTOSingle(DmxR, DmxG, DmxB, DmxCTO, CoolTemp, WarmTemp, Index, Matrix);
SetMatrixColorRGBWithCTOMultiple(DmxR, DmxG, DmxB, DmxCTO, CoolTemp, WarmTemp, Matrix);

// RGBW + CTO (single pixel / multi pixel)
SetMatrixColorRGBWWithCTOSingle(DmxR, DmxG, DmxB, DmxW, DmxCTO, CoolTemp, WarmTemp, Index, Matrix);
SetMatrixColorRGBWWithCTOMultiple(DmxR, DmxG, DmxB, DmxW, DmxCTO, CoolTemp, WarmTemp, Matrix);

// RGB + CoolWarm (single pixel / multi pixel)
SetMatrixColorRGBWithCoolWarmSingle(DmxR, DmxG, DmxB, DmxCool, DmxWarm, CoolTemp, WarmTemp, Index, Matrix);
SetMatrixColorRGBWithCoolWarmMultiple(DmxR, DmxG, DmxB, DmxCool, DmxWarm, CoolTemp, WarmTemp, Matrix);

// RGBW + CoolWarm (single pixel / multi pixel)
SetMatrixColorRGBWWithCoolWarmSingle(DmxR, DmxG, DmxB, DmxW, DmxCool, DmxWarm, CoolTemp, WarmTemp, Index, Matrix);
SetMatrixColorRGBWWithCoolWarmMultiple(DmxR, DmxG, DmxB, DmxW, DmxCool, DmxWarm, CoolTemp, WarmTemp, Matrix);

// Dynamic multi-color mixing
SetMatrixColorMix(ColorChannels, Matrix);
```

### 13.2 Single Pixel vs Multi Pixel

- **Single**: All modules share one channel value, target pixel specified by Index
- **Multiple**: Each module has independent channels, iterates using `GET_SUPER_DMX_MATRIX_VALUE`

---

## 14. Motion Control

### 14.1 Pan/Tilt

```cpp
// Blueprint calls
YPan(DmxPan);     // Horizontal rotation
YTilt(DmxTilt);   // Vertical rotation
YPTSpeed(DmxPT);  // PT speed control
```

**Mapping**: `DMX[0,1] → Lerp(PanRange.X, PanRange.Y)` + `FInterpTo` smoothing

**Default ranges**:
- Pan: `[-270°, 270°]`
- Tilt: `[-135°, 135°]`

### 14.2 Infinite Rotation

```cpp
YPanRot(DmxInfinity, DmxPan);   // Pan infinite rotation
YTiltRot(DmxInfinity, DmxTilt); // Tilt infinite rotation
```

- Position attributes' sub-attributes in the fixture library define `RotationMode`
- `EInfiniteRotationMode::Infinite` → continuous rotation
- `EInfiniteRotationMode::Position` → rotate from current position to target
- Speed = `DMX[0,1] → Lerp(-5, 5) × InfiniteRotationalSpeed`

### 14.3 Matrix Motion

```cpp
YMatrixPan(DmxPan, MatrixHard);     // Matrix Pan
YMatrixTilt(DmxTilt, MatrixRocker); // Matrix Tilt
YMatrixPanRot(DmxInfinity, DmxPan, MatrixRocker);  // Matrix Pan infinite
YMatrixTiltRot(DmxInfinity, DmxTilt, MatrixHard);   // Matrix Tilt infinite
```

### 14.4 Lift

```cpp
SetLiftZ(USuperLiftComponent* NewSuperLift, FSuperDMXAttribute DmxLiftZ);
```

- `DMX[0,1] → Lerp(0, LiftRange)` + `FInterpTo(CurrentZ, Target, DeltaTime, LiftSpeed)`

### 14.5 Fixture Angle

```cpp
YLampAngle();  // Apply PanAngle / TiltAngle (manual angle mode)
```

---

## 15. Strobe System

### 15.1 Strobe Control Functions

```cpp
// Main light source
SetLightingStrobe(DmxStrobe);         // Single light
SetLightingStrobeMatrix(DmxStrobe);   // Matrix

// Spot fill light
SetSpotStrobe(DmxStrobe);
SetSpotStrobeMatrix(DmxStrobe);

// Effect
SetEffectStrobe(DmxStrobe);

// Matrix component
SetMatrixStrobe(DmxStrobe, MatrixComp);
```

### 15.2 Strobe Modes (Sub-attribute-driven)

The Dimmer/Strobe attribute's sub-attributes in the fixture library can define different DMX ranges corresponding to different strobe modes:

```
SubAttribute[0]: DmxMin=0,   DmxMax=7,   StrobeMode=Closed   (off)
SubAttribute[1]: DmxMin=8,   DmxMax=255, StrobeMode=Open      (always on)
SubAttribute[2]: DmxMin=64,  DmxMax=95,  StrobeMode=Linear    (linear strobe)
SubAttribute[3]: DmxMin=96,  DmxMax=127, StrobeMode=Pulse     (pulse)
SubAttribute[4]: DmxMin=128, DmxMax=159, StrobeMode=RampUp    (fade in)
SubAttribute[5]: DmxMin=160, DmxMax=191, StrobeMode=RampDown  (fade out)
SubAttribute[6]: DmxMin=192, DmxMax=223, StrobeMode=Sine      (sine)
SubAttribute[7]: DmxMin=224, DmxMax=255, StrobeMode=Random    (random)
```

### 15.3 ComponentDimmer

Each lighting/effect/matrix component has a `ComponentDimmer` property:

```
FinalIntensity = ActorDimmer × ComponentDimmer × StrobeMultiplier
```

Used for independently controlling the intensity of each component in multi-module fixtures.

---

## 16. Complete Example: C++ Moving Light

The following shows the complete C++ implementation concept for a 17-channel Beam light.

### 16.1 Fixture Library Definition

| Channel | Attribute Name | Category | Bit Depth |
|------|--------|------|------|
| 1-2 | Pan | Position | 16-bit |
| 3-4 | Tilt | Position | 16-bit |
| 5 | PTSpeed | Control | 8-bit |
| 6-7 | Dimmer | Dimmer | 16-bit |
| 8 | Strobe | Strobe | 8-bit |
| 9 | ColorWheel | Color | 8-bit |
| 10 | Gobo1 | Gobo | 8-bit |
| 11 | Gobo1Rot | Gobo | 8-bit |
| 12 | Prism1 | Prism | 8-bit |
| 13 | PrismRot | Prism | 8-bit |
| 14 | Focus | Focus | 8-bit |
| 15 | Zoom | Beam | 8-bit |
| 16 | Frost | Frost | 8-bit |
| 17 | Control | Control | 8-bit |

### 16.2 Blueprint SuperDMXTick Logic

```
Event OnConstruction / BeginPlay
    └─ SetLightingDefaultValue(SuperLighting)

Event SuperDMXTick(DeltaSeconds)
    │
    ├─ Pan(DmxPan)                              // ch1-2
    ├─ Tilt(DmxTilt)                            // ch3-4
    ├─ PTSpeed(DmxPTSpeed)                      // ch5
    │
    ├─ SetLightingIntensity(DmxDimmer)          // ch6-7
    ├─ SetLightingStrobe(DmxStrobe)             // ch8
    │
    ├─ SetLightingColorWheel(DmxColorWheel, ColorAtlas) // ch9
    │
    ├─ SetBeamGobo1(DmxGobo1, DmxGobo1Rot, GoboAtlas) // ch10-11
    ├─ SetBeamPrism(DmxPrism1, None, DmxPrismRot, PrismPreset) // ch12-13
    ├─ SetBeamFocus(DmxFocus)                                   // ch14
    │
    ├─ SetLightingZoom(DmxZoom)                 // ch15
    └─ SetLightingFrost(DmxFrost)               // ch16
```

---

## 17. Complete Example: Blueprint Wash Light

### 17.1 Channel Table

| Channel | Attribute Name | Category |
|------|--------|------|
| 1-2 | Pan | Position |
| 3-4 | Tilt | Position |
| 5 | Dimmer | Dimmer |
| 6 | Strobe | Strobe |
| 7 | ColorR | Color |
| 8 | ColorG | Color |
| 9 | ColorB | Color |
| 10 | ColorW | Color |
| 11 | Zoom | Beam |
| 12 | CTO | Color |

### 17.2 Blueprint Node Wiring

```
Event LightInitialization
    └─ SetLightingDefaultValue(SuperLighting)

Event SuperDMXTick
    ├─ Pan(DmxPan)                                          // 16-bit
    ├─ Tilt(DmxTilt)                                        // 16-bit
    ├─ SetLightingIntensity(DmxDimmer)                      // 8-bit
    ├─ SetLightingStrobe(DmxStrobe)                         // 8-bit
    ├─ SetLightingColorRGBWWithCTO(DmxR, DmxG, DmxB, DmxW, DmxCTO, 6500, 3200) // 8-bit ×5
    └─ SetLightingZoom(DmxZoom)                             // 8-bit
```

### 17.3 Component Configuration

- **SuperLighting** uses `USuperSpotComponent` (Wash light without visible light column)
- **ZoomRange**: `(10, 60)` representing 10°-60° beam angle
- No Gobo/Prism/Cutting channels

---

## 18. Complete Example: Blueprint Strobe Light

Strobe lights are one of the simplest fixture types, without Pan/Tilt, only with intensity and strobe control.

### 18.1 Fixture Library Definition

| Channel | Attribute Name | Category | Bit Depth | Description |
|------|--------|------|------|------|
| 1-2 | Dimmer | Dimmer | 16-bit | Intensity (high precision) |
| 3 | Strobe | Strobe | 8-bit | Strobe speed |
| 4 | StrobeMode | Strobe | 8-bit | Strobe waveform |
| 5 | Duration | Control | 8-bit | Flash duration |
| 6 | ColorR | Color | 8-bit | Red |
| 7 | ColorG | Color | 8-bit | Green |
| 8 | ColorB | Color | 8-bit | Blue |
| 9 | ColorW | Color | 8-bit | White |

**Strobe sub-attribute configuration** (Strobe channel SubAttribute):
```
SubAttr[0]: DmxMin=0,   DmxMax=7   → StrobeMode=Closed    (off)
SubAttr[1]: DmxMin=8,   DmxMax=15  → StrobeMode=Open      (always on)
SubAttr[2]: DmxMin=16,  DmxMax=95  → StrobeMode=Linear    (linear strobe slow→fast)
SubAttr[3]: DmxMin=96,  DmxMax=127 → StrobeMode=Pulse     (pulse)
SubAttr[4]: DmxMin=128, DmxMax=159 → StrobeMode=RampUp    (fade in)
SubAttr[5]: DmxMin=160, DmxMax=191 → StrobeMode=RampDown  (fade out)
SubAttr[6]: DmxMin=192, DmxMax=223 → StrobeMode=Sine      (sine)
SubAttr[7]: DmxMin=224, DmxMax=255 → StrobeMode=Random    (random)
```

### 18.2 Component Selection

- **Base class**: `ASuperStageLight` (no Pan/Tilt, but needs lighting control functions)
- **Core component**: `USuperSpotComponent` (SpotLight illumination effect)
- **Auxiliary component**: `USuperEffectComponent` (light body glow effect, optional)

### 18.3 Component Hierarchy

```
SceneBase (Root)
 └─ SM_Body (light body model)
     ├─ SuperLighting (USuperSpotComponent — illumination light source)
     └─ SuperEffect (USuperEffectComponent — body self-illumination, optional)
```

### 18.4 Blueprint Configuration

- `PanRange`: `(0, 0)` / `TiltRange`: `(0, 0)` — No motion
- `bDimmer=true`, `bStrobe=true`, `bColor=true`
- `bPan=false`, `bTilt=false`, `bZoom=false`, `bGobo1=false`
- `ZoomRange`: `(40, 40)` — Fixed beam angle
- `DimmerCurveExponent`: `1.0` (strobe lights usually use linear response)

### 18.5 Blueprint Node Wiring

```
Event OnConstruction / BeginPlay
    ├─ SetLightingDefaultValue(SuperLighting)
    └─ SetEffectDefaultValue(SuperEffect)     // If body self-illumination exists

Event SuperDMXTick(DeltaSeconds)
    │
    ├─ SetLightingIntensity(DmxDimmer)            // ch1-2  16-bit intensity
    ├─ SetLightingStrobe(DmxStrobe)               // ch3    strobe speed
    │
    ├─ SetLightingColorRGBW(DmxR, DmxG, DmxB, DmxW)  // ch6-9  RGBW mixing
    │
    │  // If body self-illumination effect component exists
    ├─ SetEffectIntensity(DmxDimmer)              // Sync intensity
    ├─ SetEffectStrobe(DmxStrobe)                 // Sync strobe
    └─ SetEffectColor(DmxR, DmxG, DmxB)          // Sync color
```

### 18.6 Key Points

- Strobe light `DimmerCurveExponent` is usually set to `1.0` (linear) as fast response is needed
- Strobe mode is driven by the fixture library SubAttribute `StrobeMode` field, no additional processing needed in Blueprint
- `SetRandomSeed` allows multiple strobe lights to produce different random sequences (pass different seed values per light)
- The body self-illumination component can share Dimmer/Strobe/Color channels with the main light source

---

## 19. Complete Example: Blueprint Profile Moving Light

Profile (imaging/cutting light) is the most complex fixture type, with a full optical system.

### 19.1 Fixture Library Definition

| Channel | Attribute Name | Category | Bit Depth | Description |
|------|--------|------|------|------|
| 1-2 | Pan | Position | 16-bit | Horizontal rotation |
| 3-4 | Tilt | Position | 16-bit | Vertical rotation |
| 5 | PTSpeed | Control | 8-bit | PT speed |
| 6-7 | Dimmer | Dimmer | 16-bit | Intensity |
| 8 | Strobe | Strobe | 8-bit | Strobe |
| 9 | Color1 | Color | 8-bit | Color wheel 1 |
| 10 | Color2 | Color | 8-bit | Color wheel 2 |
| 11 | CyanMix | Color | 8-bit | Cyan mixing (CMY) |
| 12 | MagentaMix | Color | 8-bit | Magenta mixing (CMY) |
| 13 | YellowMix | Color | 8-bit | Yellow mixing (CMY) |
| 14 | CTO | Color | 8-bit | Color temperature adjustment |
| 15 | Gobo1 | Gobo | 8-bit | Gobo wheel 1 |
| 16 | Gobo1Rot | Gobo | 8-bit | Gobo 1 rotation |
| 17 | Gobo2 | Gobo | 8-bit | Gobo wheel 2 |
| 18 | Gobo2Rot | Gobo | 8-bit | Gobo 2 rotation |
| 19 | Prism1 | Prism | 8-bit | Prism |
| 20 | PrismRot | Prism | 8-bit | Prism rotation |
| 21 | Focus | Focus | 8-bit | Focus |
| 22 | Zoom | Beam | 8-bit | Zoom |
| 23 | Frost | Frost | 8-bit | Frost |
| 24 | Iris | Beam | 8-bit | Iris |
| 25 | A1 | Cutting | 8-bit | Cutting blade A1 |
| 26 | B1 | Cutting | 8-bit | Cutting blade B1 |
| 27 | A2 | Cutting | 8-bit | Cutting blade A2 |
| 28 | B2 | Cutting | 8-bit | Cutting blade B2 |
| 29 | A3 | Cutting | 8-bit | Cutting blade A3 |
| 30 | B3 | Cutting | 8-bit | Cutting blade B3 |
| 31 | A4 | Cutting | 8-bit | Cutting blade A4 |
| 32 | B4 | Cutting | 8-bit | Cutting blade B4 |
| 33 | ShaperRot | Cutting | 8-bit | Cutting system rotation |

### 19.2 Component Selection

- **Base class**: `ASuperStageLight`
- **Core component**: `USuperCuttingComponent` (beam component supporting cutting, replaces USuperBeamComponent)
- **Auxiliary component**: `USuperSpotComponent` (fill light, optional)

### 19.3 Component Hierarchy

```
SceneBase (Root)
 └─ SM_Hook
     └─ SM_Base
         └─ SceneRocker (Pan)
             └─ SM_Rocker
                 └─ SceneHard (Tilt)
                     └─ SM_Hard
                         ├─ SuperLighting (USuperCuttingComponent — main light source + cutting)
                         └─ SpotLighting (USuperSpotComponent — fill light, optional)
```

### 19.4 Channel Switch Configuration

```
bChannelEdit=true (check first to see switches below)

bDimmer=true, bStrobe=true, bPan=true, bTilt=true, bPTSpeed=true,
bColorWheel=true, bColor=true,
bGobo1=true, bGobo2=true, bGobo_Rot=true,
bPrism1=true, bPrism_Rot=true,
bFocus=true, bZoom=true, bFrost=true, bIris=true,
bCuttingChannel=true
```

### 19.5 Blueprint Default Parameters

- `ZoomRange`: `(7, 50)` — Profile light typical angle range
- `LightDefaultValue.MaxLightDistance`: `8000` — Light distance
- `LightDefaultValue.MaxLightIntensity`: `500`
- `LightDefaultValue.bLightShadow`: `true`
- `BeamDefaultValue.AtmosphericDensity`: `0.03`
- `BeamDefaultValue.BeamQuality`: `64`
- `DimmerCurveExponent`: `2.0`

### 19.6 Blueprint Node Wiring

```
Event OnConstruction / BeginPlay
    ├─ SetLightingDefaultValue(SuperLighting)
    └─ SetSpotDefaultValue(SpotLighting)       // If fill light exists

Event SuperDMXTick(DeltaSeconds)
    │
    │  ── Motion ──
    ├─ Pan(DmxPan)                                                     // ch1-2
    ├─ Tilt(DmxTilt)                                                   // ch3-4
    ├─ PTSpeed(DmxPTSpeed)                                             // ch5
    │
    │  ── Intensity/Strobe ──
    ├─ SetLightingIntensity(DmxDimmer)                                 // ch6-7
    ├─ SetLightingStrobe(DmxStrobe)                                    // ch8
    │
    │  ── Color (Color wheel + CMY + CTO) ──
    ├─ SetLightingColorWheel2AndCMYAndCTO(                             // ch9-14
    │      DmxColor1, DmxColor2,
    │      DmxCyanMix, DmxMagentaMix, DmxYellowMix,
    │      DmxCTO, 2700, 6500,
    │      ColorAtlas1, ColorAtlas2)
    │
    │  ── Gobo ──
    ├─ SetBeamGobo2(DmxGobo1, DmxGobo1Rot,                            // ch15-18
    │              DmxGobo2, DmxGobo2Rot,
    │              Gobo1Atlas, Gobo2Atlas)
    │
    │  ── Prism ──
    ├─ SetBeamPrism(DmxPrism1, None, DmxPrismRot, PrismPreset)       // ch19-20
    │
    │  ── Optics ──
    ├─ SetBeamFocus(DmxFocus)                                          // ch21
    ├─ SetLightingZoom(DmxZoom)                                        // ch22
    ├─ SetLightingFrost(DmxFrost)                                      // ch23
    ├─ SetLightingIris(DmxIris)                                        // ch24
    │
    │  ── Cutting ──
    ├─ SetBeamCutting(DmxA1, DmxB1, DmxA2, DmxB2,                    // ch25-32
    │                DmxA3, DmxB3, DmxA4, DmxB4)
    └─ SetBeamCuttingRotate(DmxShaperRot, DmxGobo1Rot)               // ch33 + ch16
```

### 19.7 Key Points

- `USuperCuttingComponent` inherits from `USuperBeamComponent`, has all beam functions + cutting
- `SetBeamCuttingRotate` second parameter `DmxGoboRot` is used to sync Gobo rotation with cutting rotation
- CMY mixing is subtractive: C=1 means fully remove red, M=1 remove green, Y=1 remove blue
- CTO color temperature direction: `DmxCTO=0` corresponds to `CoolTemperature`, `DmxCTO=1` corresponds to `WarmTemperature`
- Two color wheels need separate `ColorAtlas` textures provided

---

## 20. Complete Example: Blueprint Matrix Light (with Fill Light)

LED matrix light, 7×7=49 pixels with independent RGB control, plus one SpotLight fill light for illumination.

### 20.1 Fixture Library Definition (Multi-module)

**Module 0 (main module, 7 channels)**:

| Channel Offset | Attribute Name | Category | Bit Depth | Description |
|---------|--------|------|------|------|
| 0-1 | Dimmer | Dimmer | 16-bit | Master intensity |
| 2 | Strobe | Strobe | 8-bit | Master strobe |
| 3 | SpotDimmer | Dimmer | 8-bit | Fill light intensity |
| 4 | SpotColorR | Color | 8-bit | Fill light red |
| 5 | SpotColorG | Color | 8-bit | Fill light green |
| 6 | SpotColorB | Color | 8-bit | Fill light blue |

**Modules 1-49 (pixel modules, 3 channels per module)**:

| Channel Offset | Attribute Name | Category | Description |
|---------|--------|------|------|
| 0 | MatrixR | Color | Pixel red |
| 1 | MatrixG | Color | Pixel green |
| 2 | MatrixB | Color | Pixel blue |

**Total channels**: 7 + 49×3 = **154 channels**

**Module Patch settings**:
```
Module 0:  Patch = 0    → Channels 1-7
Module 1:  Patch = 7    → Channels 8-10
Module 2:  Patch = 10   → Channels 11-13
...
Module 49: Patch = 151  → Channels 152-154
```

### 20.2 Component Selection

- **Base class**: `ASuperStageLight` (no Pan/Tilt, but needs Matrix control functions)
- **Core component**: `USuperMatrixComponent` (49 pixel matrix)
- **Auxiliary component**: `USuperSpotComponent` (fill light illumination)

### 20.3 Component Hierarchy

```
SceneBase (Root)
 └─ SM_Body (light body model)
     ├─ SuperMatrix (USuperMatrixComponent — 49 pixel panel)
     └─ SpotLighting (USuperSpotComponent — fill light illumination)
```

### 20.4 Channel Switch Configuration

```
bDimmer=true, bStrobe=true, bColor=true,
bPan=false, bTilt=false
```

### 20.5 Blueprint Default Parameters

- `PanRange`: `(0, 0)` / `TiltRange`: `(0, 0)` — No motion
- **USuperMatrixComponent**:
  - `SegCount`: `49`
  - `bUseUAxis`: `true` (slice in U direction, 7 columns)
  - `SegmentLightType`: `PointLight` (add point light per pixel)
  - `SegmentLightIntensity`: `100`
  - `SegmentLightRadius`: `50`
- **USuperSpotComponent (fill light)**:
  - `ZoomRange`: `(15, 60)` — Fill light angle

### 20.6 Blueprint Node Wiring

```
Event OnConstruction / BeginPlay
    ├─ SetMatrixMaterial(SuperMatrix)           // Initialize matrix material + segment lights
    └─ SetSpotDefaultValue(SpotLighting)        // Initialize fill light

Event SuperDMXTick(DeltaSeconds)
    │
    │  ── Main module control ──
    ├─ SetMatrixIntensity(DmxDimmer, SuperMatrix)            // Master intensity
    ├─ SetMatrixStrobe(DmxStrobe, SuperMatrix)               // Master strobe
    │
    │  ── Fill light control ──
    ├─ SetSpotIntensity(DmxSpotDimmer)                       // Fill light intensity
    ├─ SetSpotColorRGB(DmxSpotR, DmxSpotG, DmxSpotB)        // Fill light color
    │
    │  ── Matrix pixels (49 pixel batch setting) ──
    └─ SetMatrixColorMultiple(DmxMatrixR, DmxMatrixG, DmxMatrixB, SuperMatrix)
```

### 20.7 Key Points

- `SetMatrixColorMultiple` internally uses `GET_SUPER_DMX_MATRIX_VALUE` macro to auto-iterate modules 1-49
- Matrix fixture library **must** ensure all pixel modules' attribute names are exactly the same (all named `MatrixR/G/B`)
- Each pixel module's `Patch` value must be correctly offset, otherwise pixel addresses will be misaligned
- `SegmentLightType=PointLight` makes each pixel produce real lighting, but adds performance overhead
- Fill light uses `SetSpotIntensity` / `SetSpotColorRGB` independent control, unaffected by matrix intensity
- `SetMatrixMaterial` must be called in OnConstruction/BeginPlay, not in Tick

---

## 21. Complete Example: Blueprint Moving Head Matrix Light

Moving head matrix light = Pan/Tilt motion + matrix pixel control. Using 3×3=9 pixels as example.

### 21.1 Fixture Library Definition (Multi-module)

**Module 0 (main module, 8 channels)**:

| Channel Offset | Attribute Name | Category | Bit Depth | Description |
|---------|--------|------|------|------|
| 0-1 | Pan | Position | 16-bit | Horizontal rotation |
| 2-3 | Tilt | Position | 16-bit | Vertical rotation |
| 4 | PTSpeed | Control | 8-bit | PT speed |
| 5-6 | Dimmer | Dimmer | 16-bit | Master intensity |
| 7 | Strobe | Strobe | 8-bit | Master strobe |

**Modules 1-9 (pixel modules, 4 channels per module)**:

| Channel Offset | Attribute Name | Category | Description |
|---------|--------|------|------|
| 0 | MatrixR | Color | Pixel red |
| 1 | MatrixG | Color | Pixel green |
| 2 | MatrixB | Color | Pixel blue |
| 3 | MatrixW | Color | Pixel white |

**Total channels**: 8 + 9×4 = **44 channels**

### 21.2 Component Hierarchy

```
SceneBase (Root)
 └─ SM_Hook
     └─ SM_Base
         └─ SceneRocker (Pan)
             └─ SM_Rocker
                 └─ SceneHard (Tilt)
                     └─ SM_Hard
                         └─ SuperMatrix (USuperMatrixComponent — 9 pixels)
```

### 21.3 Channel Switch Configuration

```
bDimmer=true, bStrobe=true, bPan=true, bTilt=true, bPTSpeed=true
```

### 21.4 Blueprint Default Parameters

- `PanRange`: `(-270, 270)`
- `TiltRange`: `(-135, 135)`
- **USuperMatrixComponent**:
  - `SegCount`: `9`
  - `bUseUAxis`: `true` (3 column slicing)
  - `SegmentLightType`: `SpotLight` (matrix pixels use spotlights pointing outward)
  - `SegmentLightIntensity`: `200`
  - `SegmentLightRadius`: `100`

### 21.5 Blueprint Node Wiring

```
Event OnConstruction / BeginPlay
    └─ SetMatrixMaterial(SuperMatrix)

Event SuperDMXTick(DeltaSeconds)
    │
    │  ── Motion ──
    ├─ Pan(DmxPan)                                       // ch1-2
    ├─ Tilt(DmxTilt)                                     // ch3-4
    ├─ PTSpeed(DmxPTSpeed)                               // ch5
    │
    │  ── Master control ──
    ├─ SetMatrixIntensity(DmxDimmer, SuperMatrix)        // ch6-7
    ├─ SetMatrixStrobe(DmxStrobe, SuperMatrix)           // ch8
    │
    │  ── Matrix pixels (9 pixels batch RGBW) ──
    └─ SetMatrixColorRGBWMultiple(DmxMatrixR, DmxMatrixG, DmxMatrixB, DmxMatrixW, SuperMatrix)
```

### 21.6 Key Points

- Moving head matrix light's matrix component **must** be attached under `SceneHard` to follow Tilt rotation
- `SetMatrixColorRGBWMultiple` auto-iterates modules 1-9, handling RGBW four channels
- If the fixture also needs Pan/Tilt **infinite rotation**, add `PanRot`/`TiltRot` channels
- When matrix component `SegmentLightType` is `SpotLight`, each pixel casts a spotlight outward

---

## 22. Complete Example: Blueprint Multi-function Moving Light

High-end moving light = Beam/Spot main light + effect ring + matrix pixels + fill light + lift. Demonstrates multi-component collaboration.

### 22.1 Fixture Library Definition

**Module 0 (main module, 30 channels)**:

| Channel Offset | Attribute Name | Category | Bit Depth | Description |
|---------|--------|------|------|------|
| 0-1 | Pan | Position | 16-bit | Horizontal |
| 2-3 | Tilt | Position | 16-bit | Vertical |
| 4 | PTSpeed | Control | 8-bit | PT speed |
| 5-6 | Dimmer | Dimmer | 16-bit | Main light intensity |
| 7 | Strobe | Strobe | 8-bit | Main light strobe |
| 8 | ColorWheel | Color | 8-bit | Color wheel |
| 9 | Gobo1 | Gobo | 8-bit | Gobo wheel |
| 10 | Gobo1Rot | Gobo | 8-bit | Gobo rotation |
| 11 | Prism1 | Prism | 8-bit | Prism |
| 12 | PrismRot | Prism | 8-bit | Prism rotation |
| 13 | Focus | Focus | 8-bit | Focus |
| 14 | Zoom | Beam | 8-bit | Zoom |
| 15 | Frost | Frost | 8-bit | Frost |
| 16 | SpotDimmer | Dimmer | 8-bit | Fill light intensity |
| 17 | SpotStrobe | Strobe | 8-bit | Fill light strobe |
| 18 | SpotColorR | Color | 8-bit | Fill light red |
| 19 | SpotColorG | Color | 8-bit | Fill light green |
| 20 | SpotColorB | Color | 8-bit | Fill light blue |
| 21 | SpotZoom | Beam | 8-bit | Fill light zoom |
| 22 | EffectDimmer | Effect | 8-bit | Effect ring intensity |
| 23 | EffectStrobe | Effect | 8-bit | Effect ring strobe |
| 24 | EffectValue | Effect | 8-bit | Effect selection |
| 25 | EffectColorR | Color | 8-bit | Effect ring red |
| 26 | EffectColorG | Color | 8-bit | Effect ring green |
| 27 | EffectColorB | Color | 8-bit | Effect ring blue |
| 28 | MatrixDimmer | Dimmer | 8-bit | Matrix intensity |
| 29 | MatrixStrobe | Strobe | 8-bit | Matrix strobe |

**Modules 1-12 (matrix pixels, 3 channels per module)**:

| Channel Offset | Attribute Name | Category | Description |
|---------|--------|------|------|
| 0 | MatrixR | Color | Pixel red |
| 1 | MatrixG | Color | Pixel green |
| 2 | MatrixB | Color | Pixel blue |

**Total channels**: 30 + 12×3 = **66 channels**

### 22.2 Component Hierarchy

```
SceneBase (Root)
 └─ SM_Hook
     └─ SM_Base
         └─ SceneRocker (Pan)
             └─ SM_Rocker
                 └─ SceneHard (Tilt)
                     └─ SM_Hard
                         ├─ SuperLighting (USuperBeamComponent — main light source + beam column)
                         ├─ SpotLighting (USuperSpotComponent — fill light wash)
                         ├─ SuperEffect (USuperEffectComponent — effect ring)
                         └─ SuperMatrix (USuperMatrixComponent — 12 pixel ring)
```

### 22.3 Channel Switch Configuration

```
bDimmer=true, bStrobe=true, bPan=true, bTilt=true, bPTSpeed=true,
bColorWheel=true,
bGobo1=true, bGobo_Rot=true,
bPrism1=true, bPrism_Rot=true,
bFocus=true, bZoom=true, bFrost=true,
bEffect=true
```

### 22.4 Blueprint Default Parameters

- `ZoomRange`: `(3, 45)` — Main beam angle
- **SpotLighting**: `ZoomRange`: `(10, 60)` — Fill light angle
- **SuperEffect**: `bTransparent`: `true` — Effect ring transparent material
- **SuperMatrix**:
  - `SegCount`: `12` (circular arrangement)
  - `bUseUAxis`: `true`
  - `SegmentLightType`: `PointLight`
  - `SegmentLightIntensity`: `80`

### 22.5 Blueprint Node Wiring

```
Event OnConstruction / BeginPlay
    ├─ SetLightingDefaultValue(SuperLighting)    // Main light init
    ├─ SetSpotDefaultValue(SpotLighting)         // Fill light init
    ├─ SetEffectDefaultValue(SuperEffect)        // Effect ring init
    └─ SetMatrixMaterial(SuperMatrix)            // Matrix init

Event SuperDMXTick(DeltaSeconds)
    │
    │  ══════════ Motion Control ══════════
    ├─ Pan(DmxPan)                                                // ch1-2
    ├─ Tilt(DmxTilt)                                              // ch3-4
    ├─ PTSpeed(DmxPTSpeed)                                        // ch5
    │
    │  ══════════ Main Light Source ══════════
    ├─ SetLightingIntensity(DmxDimmer)                            // ch6-7
    ├─ SetLightingStrobe(DmxStrobe)                               // ch8
    ├─ SetLightingColorWheel(DmxColorWheel, ColorAtlas)           // ch9
    ├─ SetBeamGobo1(DmxGobo1, DmxGobo1Rot, GoboAtlas)            // ch10-11
    ├─ SetBeamPrism(DmxPrism1, None, DmxPrismRot, PrismPreset)  // ch12-13
    ├─ SetBeamFocus(DmxFocus)                                     // ch14
    ├─ SetLightingZoom(DmxZoom)                                   // ch15
    ├─ SetLightingFrost(DmxFrost)                                 // ch16
    │
    │  ══════════ Fill Light (Independent Control) ══════════
    ├─ SetSpotIntensity(DmxSpotDimmer)                            // ch17
    ├─ SetSpotStrobe(DmxSpotStrobe)                               // ch18
    ├─ SetSpotColorRGB(DmxSpotR, DmxSpotG, DmxSpotB)             // ch19-21
    ├─ SetSpotZoom(DmxSpotZoom)                                   // ch22
    │
    │  ══════════ Effect Ring ══════════
    ├─ SetEffectIntensity(DmxEffectDimmer)                        // ch23
    ├─ SetEffectStrobe(DmxEffectStrobe)                           // ch24
    ├─ SetEffect(DmxEffectValue)                                  // ch25
    ├─ SetEffectColor(DmxEffectR, DmxEffectG, DmxEffectB)        // ch26-28
    │
    │  ══════════ Matrix Pixels ══════════
    ├─ SetMatrixIntensity(DmxMatrixDimmer, SuperMatrix)           // ch29
    ├─ SetMatrixStrobe(DmxMatrixStrobe, SuperMatrix)              // ch30
    └─ SetMatrixColorMultiple(DmxMatrixR, DmxMatrixG, DmxMatrixB, SuperMatrix)
```

### 22.6 Key Points

- **Four-component collaboration**: Main light / fill light / effect ring / matrix each have independent intensity, strobe, and color control
- Main light source uses `USuperBeamComponent` (with beam column), fill light uses `USuperSpotComponent` (no beam column)
- Effect ring and matrix pixels are both attached under `SceneHard` to follow head rotation
- Fill light `SetSpot*` series functions are completely independent from main light source `SetLighting*`
- Matrix `SetMatrixIntensity` / `SetMatrixStrobe` control master intensity/strobe for all pixels
- Effect ring can use `SetEffectWithSpeed(DmxEffect, DmxSpeed)` to separate effect selection and speed control
- All components' `DefaultValue` / `Material` functions **must be called** during initialization; missing one means that component won't work

---

## 23. Detailed Guide for Each Fixture Type

This chapter provides complete creation workflows for each fixture type, including base class selection, component configuration, fixture library definition, initialization, and control logic.

---

### 23.1 Beam Light

**Characteristics**: Narrow beam, visible light column, Gobo/Prism/Color wheel

**Base class**: `ASuperStageLight`  
**Core component**: `USuperBeamComponent` (SuperLighting)  
**Auxiliary component** (optional): `USuperSpotComponent` (SpotLighting fill light)

**Component hierarchy**:
```
SceneBase (Root)
 └─ SM_Hook (hook model)
     └─ SM_Base (base model)
         └─ SceneRocker (Pan rotation axis)
             └─ SM_Rocker (arm model)
                 └─ SceneHard (Tilt rotation axis)
                     └─ SM_Hard (head model)
                         ├─ SuperLighting (USuperBeamComponent)
                         └─ SpotLighting (USuperSpotComponent, optional fill light)
```

**Fixture library definition** (typical 17 channels):

| Channel | Attribute Name | Category | Bit Depth |
|------|--------|------|------|
| 1-2 | Pan | Position | 16-bit |
| 3-4 | Tilt | Position | 16-bit |
| 5 | PTSpeed | Control | 8-bit |
| 6-7 | Dimmer | Dimmer | 16-bit |
| 8 | Strobe | Strobe | 8-bit |
| 9 | ColorWheel | Color | 8-bit |
| 10 | Gobo1 | Gobo | 8-bit |
| 11 | Gobo1Rot | Gobo | 8-bit |
| 12 | Prism1 | Prism | 8-bit |
| 13 | PrismRot | Prism | 8-bit |
| 14 | Focus | Focus | 8-bit |
| 15 | Zoom | Beam | 8-bit |
| 16 | Frost | Frost | 8-bit |
| 17 | Control | Control | 8-bit |

**Channel switch configuration** (after bChannelEdit=true):
```
bDimmer=true, bStrobe=true, bPan=true, bTilt=true, bPTSpeed=true,
bColor=true, bGobo=true, bPrism=true, bFocus=true, bZoom=true, bFrost=true
```

**Initialization** (OnConstruction + BeginPlay):
```
SetLightingDefaultValue(SuperLighting)
```

**SuperDMXTick control logic**:
```
Pan(DmxPan)                              // Horizontal rotation
Tilt(DmxTilt)                            // Vertical rotation
PTSpeed(DmxPTSpeed)                      // PT speed

SetLightingIntensity(DmxDimmer)          // Intensity
SetLightingStrobe(DmxStrobe)             // Strobe

SetLightingColorWheel(DmxColorWheel, ColorAtlas)  // Color wheel

SetBeamGobo1(DmxGobo1, DmxGobo1Rot, GoboAtlas)   // Gobo wheel 1
SetBeamPrism(DmxPrism1, None, DmxPrismRot, PrismPreset) // Prism
SetBeamFocus(DmxFocus)                                    // Focus

SetLightingZoom(DmxZoom)                 // Zoom
SetLightingFrost(DmxFrost)               // Frost
```

**Blueprint default parameter configuration**:
- `ZoomRange`: `(1, 8)` — 1°~8° beam angle
- `PanRange`: `(-270, 270)` — Default is fine
- `TiltRange`: `(-135, 135)` — Default is fine
- `LightDefaultValue.MaxLightDistance`: `5000` — Light distance (cm)
- `LightDefaultValue.MaxLightIntensity`: `1000` — Maximum intensity

---

### 23.2 Spot Light

**Characteristics**: Similar to Beam light but larger beam angle, usually has color wheel and Gobo

**Base class**: `ASuperStageLight`  
**Core component**: `USuperBeamComponent` (with beam column) or `USuperSpotComponent` (no beam column)

**Differences from Beam light**:
- `ZoomRange`: `(10, 45)` — Larger beam angle range
- Usually has Iris channel
- Optional second gobo wheel

**Additional channels** (on top of Beam):

| Channel | Attribute Name | Category |
|------|--------|------|
| Extra | Iris | Beam |
| Extra | Gobo2 / Gobo2Rot | Gobo |

**Additional SuperDMXTick nodes**:
```
SetLightingIris(DmxIris)                          // Iris
SetBeamGobo2(DmxGobo2, DmxGobo2Rot, Gobo2Atlas)  // Second gobo wheel
```

---

### 23.3 Wash Light

**Characteristics**: Soft uniform beam, RGB/RGBW mixing, large angle Zoom

**Base class**: `ASuperStageLight`  
**Core component**: `USuperSpotComponent` (no visible light column, soft effect)

**Fixture library definition** (typical 12 channels):

| Channel | Attribute Name | Category | Bit Depth |
|------|--------|------|------|
| 1-2 | Pan | Position | 16-bit |
| 3-4 | Tilt | Position | 16-bit |
| 5 | Dimmer | Dimmer | 8-bit |
| 6 | Strobe | Strobe | 8-bit |
| 7 | ColorR | Color | 8-bit |
| 8 | ColorG | Color | 8-bit |
| 9 | ColorB | Color | 8-bit |
| 10 | ColorW | Color | 8-bit |
| 11 | Zoom | Beam | 8-bit |
| 12 | CTO | Color | 8-bit |

**Channel switches**:
```
bDimmer=true, bStrobe=true, bPan=true, bTilt=true,
bColor=true, bZoom=true
```

**SuperDMXTick**:
```
Pan(DmxPan)
Tilt(DmxTilt)
SetLightingIntensity(DmxDimmer)
SetLightingStrobe(DmxStrobe)
SetLightingColorRGBWWithCTO(DmxR, DmxG, DmxB, DmxW, DmxCTO, 6500, 3200)
SetLightingZoom(DmxZoom)
```

**Blueprint default parameters**:
- `ZoomRange`: `(10, 60)` — 10°~60° soft light angle
- No Gobo/Prism/Cutting components

---

### 23.4 Profile Light (Imaging/Cutting Light)

**Characteristics**: Visible light column + four-blade cutting system + Gobo + Prism

**Base class**: `ASuperStageLight`  
**Core component**: `USuperCuttingComponent` (replaces USuperBeamComponent)

**Key differences from Beam light**:
- Uses `USuperCuttingComponent` as main light source (supports cutting)
- Adds 8 cutting channels (A1-A4, B1-B4) + ShaperRot

**Additional fixture library channels**:

| Channel | Attribute Name | Category | Description |
|------|--------|------|------|
| Extra | A1 | Cutting | Blade A1 position |
| Extra | A2 | Cutting | Blade A2 position |
| Extra | A3 | Cutting | Blade A3 position |
| Extra | A4 | Cutting | Blade A4 position |
| Extra | B1 | Cutting | Blade B1 position |
| Extra | B2 | Cutting | Blade B2 position |
| Extra | B3 | Cutting | Blade B3 position |
| Extra | B4 | Cutting | Blade B4 position |
| Extra | ShaperRot | Cutting | Cutting system rotation |

**Channel switches**: In addition to Beam light switches, also check `bCutting=true`

**SuperDMXTick** (cutting section):
```
SetBeamCutting(DmxA1, DmxB1, DmxA2, DmxB2, DmxA3, DmxB3, DmxA4, DmxB4)
SetBeamCuttingRotate(DmxShaperRot, DmxGoboRot)
```

**Component replacement in C++ constructor**:
```cpp
// Profile light uses CuttingComponent instead of BeamComponent
SuperLighting = CreateDefaultSubobject<USuperCuttingComponent>(TEXT("SuperLighting"));
SuperLighting->SetupAttachment(SceneHard);
```

---

### 23.5 Matrix Light / Pixel Light

**Characteristics**: Multi-pixel independent color control, multi-module fixture library

**Base class**: `ASuperStageLight`  
**Core component**: `USuperMatrixComponent` (matrix pixels) + `USuperBeamComponent`/`USuperSpotComponent` (optional main light source)

**Fixture library definition** (multi-module structure):

For a 5×5 matrix light, the fixture library needs 26 modules:
- **Module 0 (main module)**: Dimmer(16-bit) + Strobe + Pan + Tilt + Zoom = 6 channels
- **Modules 1-25 (pixel modules)**: Each module ColorR + ColorG + ColorB = 3 channels
- **Total channels**: 6 + 25×3 = 81 channels

**Channel switches**:
```
bDimmer=true, bStrobe=true, bPan=true, bTilt=true,
bZoom=true, bMatrix=true
```

**Initialization** (OnConstruction + BeginPlay):
```
SetLightingDefaultValue(SuperLighting)    // Main light source init
SetMatrixMaterial()                        // Called on USuperMatrixComponent
```

**SuperDMXTick**:
```
// Master control
Pan(DmxPan)
Tilt(DmxTilt)
SetLightingIntensity(DmxDimmer)

// Matrix pixels (using Multiple batch functions + GET_SUPER_DMX_MATRIX_VALUE macro)
SetMatrixColorRGBMultiple(DmxR, DmxG, DmxB, MatrixComp)

// Or per-pixel setting
for Index in 0..24:
    SetMatrixColorRGBSingle(DmxR, DmxG, DmxB, Index, MatrixComp)
```

**USuperMatrixComponent configuration**:
- `SegCount` = 25 (matches pixel module count)
- `bUseUAxis` = true/false (based on matrix direction)
- `SegmentLightType` = PointLight (optional, adds real light source per pixel)

---

### 23.6 LED Strip / Strip Effect Light

**Characteristics**: GPU material-driven effects, can be batch-applied to multiple meshes

**Base class**: `ASuperLightStripEffect` (inherits `ASuperDmxActorBase`, **not** ASuperStageLight)  
**Built-in component**: Effect material system (no manual component addition needed)

**Fixture library definition** (typical 9 channels):

| Channel | Attribute Name | Category | Description |
|------|--------|------|------|
| 1 | Dimmer | Dimmer | Intensity |
| 2 | Strobe | Strobe | Strobe |
| 3 | Effect | Effect | Effect selection (0-10) |
| 4 | Speed | Effect | Effect speed |
| 5 | Width | Effect | Effect width |
| 6 | Direction | Effect | Effect direction |
| 7 | ColorR | Color | Red |
| 8 | ColorG | Color | Green |
| 9 | ColorB | Color | Blue |

**Blueprint configuration**:
- `TargetMeshActors`: Select StaticMeshActors in the scene to apply effects to
- `MaxLightIntensity`: Maximum intensity
- `MaterialIndex`: Material slot index

**Initialization** (OnConstruction + BeginPlay):
```
SetLightStripDefault()
```

**SuperDMXTick**:
```
SetLightStripEffect(DmxDimmer, DmxStrobe, DmxEffect, DmxSpeed, DmxWidth, DmxDirection, DmxR, DmxG, DmxB)
```

**Effect mapping**:
| DMX Normalized | Effect |
|-----------|------|
| 0.0 | No effect (solid color) |
| 0.1~1.0 | 10 built-in effects (flow/chase/breathe/flicker etc.) |

---

### 23.7 VFX Effect Light (Smoke/Fire/Snow etc.)

**Characteristics**: Niagara particle system-driven, supports Pan/Tilt

**Base class**: `ASuperStageVFXActor` (inherits `ASuperLightBase`, has Pan/Tilt)  
**Built-in component**: `UNiagaraComponent*` Niagara

**Fixture library definition** (typical 8 channels):

| Channel | Attribute Name | Category | Description |
|------|--------|------|------|
| 1-2 | Pan | Position | Horizontal rotation |
| 3-4 | Tilt | Position | Vertical rotation |
| 5 | SpawnCount | Control | Particle spawn count |
| 6 | ColorR | Color | Particle red |
| 7 | ColorG | Color | Particle green |
| 8 | ColorB | Color | Particle blue |

**Blueprint default parameters**:
- `MaxSpawnCount`: Maximum spawn count
- `MaxSpeed`: Particle maximum speed
- `LoopTime`: Loop time
- `Minimum` / `Maximum`: Particle range

**Initialization** (OnConstruction + BeginPlay):
```
SetNiagaraDefaultValue()   // Activate Niagara + set Speed/LoopTime/Min/Max
```

**SuperDMXTick**:
```
Pan(DmxPan)
Tilt(DmxTilt)
SetNiagaraSpawnCount(DmxSpawnCount)              // DMX→particle count
SetNiagaraColor(DmxColorR, DmxColorG, DmxColorB) // DMX→particle color
```

**Niagara variable mapping**:
| Function | Niagara Variable | Mapping |
|------|-------------|------|
| `SetNiagaraDefaultValue` | Speed, LoopTime, Minimum, Maximum | Directly write float |
| `SetNiagaraSpawnCount` | SpawnCount | DMX[0,1] → Round(Lerp(0, MaxSpawnCount)) → int |
| `SetNiagaraColor` | Color | RGB → FLinearColor |

**Setting Niagara asset in Blueprint**: In Components panel select Niagara component → Details → Niagara System Asset → select particle system

---

### 23.8 DMX Camera

**Characteristics**: 6-axis motion (XYZ displacement + XYZ rotation) + camera parameters (FOV/Aperture/Focus)

**Base class**: `ASuperDMXCamera` (inherits `ASuperDmxActorBase`, **not** ASuperLightBase)  
**Built-in components**: `UCineCameraComponent` + `USceneCaptureComponent2D`

**Fixture library definition** (typical 9 channels):

| Channel | Attribute Name | Category | Description |
|------|--------|------|------|
| 1 | PosX | Position | X-axis displacement |
| 2 | PosY | Position | Y-axis displacement |
| 3 | PosZ | Position | Z-axis displacement |
| 4 | RotX | Rotation | X-axis rotation (Pitch) |
| 5 | RotY | Rotation | Y-axis rotation (Yaw) |
| 6 | RotZ | Rotation | Z-axis rotation (Roll) |
| 7 | FOV | Camera | Field of view |
| 8 | Aperture | Camera | Aperture |
| 9 | FocusDistance | Camera | Focus distance |

**Blueprint configuration**:
- `Start`: Start toggle
- `BootRefresh`: Click to initialize (records current position/rotation as baseline)
- `MovingRange`: XYZ movement range (cm)
- `RotRange`: Pitch/Yaw/Roll rotation range (°)
- `FOVRange`: FOV range (default 15°~120°)
- `ApertureRange`: Aperture range (default f/1.2~f/22)
- `FocusDistanceRange`: Focus distance range (default 50~100000 cm)
- `RenderTarget`: Render target (for LED screen output)
- `bEnableSceneCapture`: Enable scene capture

**Must do before use**:
1. Place Actor at correct position in scene
2. Click `BootRefresh` (auto-computes InitialPosition/EndPosition, InitialRotation/EndRotation)
3. Set `Start = true`

**SuperDMXTick**:
```
DMXCameraControl(DmxPosX, DmxPosY, DmxPosZ, DmxRotX, DmxRotY, DmxRotZ)
DMXCameraParams(DmxFOV, DmxAperture, DmxFocusDistance)
```

**DMX mapping**:
```
Position: DMX[0,1] → Lerp(InitialPosition, EndPosition) + FInterpTo smoothing
Rotation: DMX[0,1] → Lerp(InitialRotation, EndRotation) + FInterpTo smoothing
FOV:      DMX[0,1] → Lerp(FOVRange.X, FOVRange.Y)
Aperture: DMX[0,1] → Lerp(ApertureRange.X, ApertureRange.Y)
Focus:    DMX[0,1] → Lerp(FocusDistanceRange.X, FocusDistanceRange.Y)
```

---

### 23.9 Lifting Machinery

**Characteristics**: 6-axis motion (3-axis displacement + 3-axis rotation), supports absolute/infinite rotation

**Base class**: `ASuperLiftingMachinery` (inherits `ASuperDmxActorBase`)

**Fixture library definition** (typical 6 channels):

| Channel | Attribute Name | Category | Description |
|------|--------|------|------|
| 1 | PosX | Position | X-axis displacement |
| 2 | PosY | Position | Y-axis displacement |
| 3 | PosZ | Position | Z-axis displacement |
| 4 | RotX | Rotation | X-axis rotation |
| 5 | RotY | Rotation | Y-axis rotation |
| 6 | RotZ | Rotation | Z-axis rotation |

**Blueprint configuration**:
- `Start`: Start toggle
- `BootRefresh`: Click to initialize
- `MovingRange`: XYZ movement range (cm)
- `RotRange`: Rotation range (°) (absolute mode)
- `PosSpeed`: Displacement interpolation speed (0-10)
- `RotSpeed`: Rotation interpolation speed (0-10)
- `PolarRotation`: Infinite rotation mode toggle
- `PolarRotationSpeed`: Infinite rotation speed (0-10)

**Must do before use**:
1. Place Actor at correct position
2. Click `BootRefresh`
3. Set `Start = true`

**SuperDMXTick**:
```
LiftingMachinery(DmxPosX, DmxPosY, DmxPosZ, DmxRotX, DmxRotY, DmxRotZ)
```

**Two rotation modes**:
- **Absolute mode** (PolarRotation=false): DMX[0,1] → Lerp(InitialRotation, EndRotation)
- **Infinite mode** (PolarRotation=true): DMX[0,1] → Speed[-PolarRotationSpeed, +PolarRotationSpeed]

---

### 23.10 Rail Machinery

**Characteristics**: Spline curve path movement + 6-axis offset/rotation, 7 DMX control axes total

**Base class**: `ASuperRailMachinery` (inherits `ASuperDmxActorBase`)

**Fixture library definition** (typical 7 channels):

| Channel | Attribute Name | Category | Description |
|------|--------|------|------|
| 1 | RailPos | Position | Rail position [0=start, 1=end] |
| 2 | PosX | Position | X-axis offset |
| 3 | PosY | Position | Y-axis offset |
| 4 | PosZ | Position | Z-axis offset |
| 5 | RotX | Rotation | X-axis rotation |
| 6 | RotY | Rotation | Y-axis rotation |
| 7 | RotZ | Rotation | Z-axis rotation |

**Built-in components**:
- `RailSplineComponent` (USplineComponent): Defines rail path
- `RailMountPoint`: Mount point moving along spline
- `OffsetComponent`: Local 6-axis offset/rotation

**Blueprint configuration**:
- `BootRefresh`: Initialize offset/rotation baseline
- `bLockOrientationToRail`: Lock orientation along rail tangent direction
- `bClosedLoop`: Closed loop rail (end-to-end cyclic)
- `OffsetRange`: Offset range (cm)
- `RailSpeed` / `OffsetSpeed` / `RotSpeed`: Per-axis interpolation speed

**Usage flow**:
1. Place Actor → edit Spline control points to define rail path
2. Click `BootRefresh`
3. Set `Start = true`
4. Child actors attached to `GetDefaultAttachComponent()` (i.e., OffsetComponent)

**SuperDMXTick**:
```
// Full 7-axis control
RailMachinery(DmxRailPos, DmxPosX, DmxPosY, DmxPosZ, DmxRotX, DmxRotY, DmxRotZ)

// Or rail position only
RailPositionOnly(DmxRailPos)
```

**DMX mapping**:
```
Rail:    DMX[0,1] → SplineLength × t → position along spline
Offset:  DMX[0,1] → Lerp(InitialOffset, EndOffset) + FInterpTo
Closed loop: Shortest arc path interpolation (avoids going the long way)
```

---

### 23.11 NDI Video Screen

**Characteristics**: Receives NDI video streams and displays on screen mesh

**Base class**: `ASuperNDIScreen` (inherits `ASuperBaseActor`, **no DMX control**)  
**No fixture library needed**: NDI screen is not controlled via DMX, directly receives network video streams

**Blueprint configuration**:
- `InputName`: NDI input source name (dropdown selection)
- `TargetStaticMeshActors`: Output screen mesh Actors (supports multi-screen)
- `bTransparent`: Transparent mode
- `Transparency`: Transparency [0,1]
- `Brightness`: Brightness
- `Color`: Color filter
- `Contrast`: Contrast
- `Deformation`: Keystone correction (four-corner UV offset)

**Usage flow**:
1. Place `ASuperNDIScreen` in scene
2. Place `AStaticMeshActor` (plane mesh) to use as screen
3. Add screen Actor to `TargetStaticMeshActors` array
4. Select `InputName` (NDI source name)
5. Run to automatically receive and display video

**Data flow**:
```
NDI network stream → SuperNDISubsystem → OnFrame callback → HandleNDIFrame →
  EnsureTexture (create/recreate texture) → UpdateTextureGPU (RHI upload) →
  DynamicMaterial.SetTextureParameterValue → Apply to TargetStaticMeshActors
```

---

### 23.12 Projector / Mapping

**Characteristics**: Three-channel RGB SpotLight achieving Projection Mapping

**Base class**: `ASuperProjector` (inherits `ASuperBaseActor`, **no DMX control**)  
**Built-in components**: 3 SpotLights (R/G/B split-channel projection)

**Blueprint configuration**:
- `MappingTexture`: Projection texture
- `MappingScale`: Projection resolution (default 1920×1080)
- `Dimmer`: Intensity (%)
- `MaxLightDistance`: Projection distance (cm)
- `Zoom`: Projection angle (10-100°)
- `DilutionFactor`: Edge softening [0,1]
- `MappingDeformation`: Keystone correction (four-corner offset)

**Projection principle**:
```
SpotLightR/G/B each set LightFunctionMaterial
  → Light function material contains texture
  → Keystone correction implemented in Shader
  → Three-channel RGB overlay synthesizes full-color projection
```

**Usage flow**:
1. Place `ASuperProjector` Actor
2. Set `MappingTexture` (projection image/video)
3. Adjust `Zoom` and `MaxLightDistance` to cover target with projection
4. Use `MappingDeformation` for keystone correction
5. Adjust `Dimmer` to control brightness

---

### 23.13 Laser (Texture Mode)

**Characteristics**: Gets laser texture from Beyond software and visualizes in UE

**Base class**: `ASuperLaserActor` (inherits `ASuperBaseActor`, **no DMX**)

**Blueprint configuration**:
- `DeviceID`: Beyond device number (1-12)
- `LaserIntensity`: Laser intensity (1-5000)
- `MaxLaserDistance`: Projection distance (100-10000 cm)
- `AtmosphericDensity`: Atmospheric attenuation [0,1]
- `FogIntensity`: Fog intensity (0-100)
- `AtmosFogSpeed`: Fog flow speed (0-100)

**Data flow**:
```
Beyond device → SuperLaserSubsystem → GetLaserTexture(DeviceID) →
  SetLaserTexture → DynamicMaterialLaser → StaticMeshLaser
```

---

### 23.14 Laser Pro (Point Data Mode)

**Characteristics**: Gets point data from Beyond, procedural mesh draws laser lines

**Base class**: `ASuperLaserProActor` (inherits `ASuperBaseActor`, **no DMX**)  
**Built-in component**: `USuperLaserProComponent`

**Blueprint configuration**:
- `DeviceID`: Beyond device number (1-12)
- `bEnableCollision`: Collision occlusion detection
- LaserComponent parameters: BeamLength, ProjectionAngle, LaserWidth, CoreSharpness etc.

**Data flow**:
```
Beyond device → SuperLaserSubsystem → GetLaserPoints(DeviceID) →
  LaserComponent.SetLaserPoints → BuildLaserMesh → ProceduralMeshComponent
```

**Usage flow**:
1. Place `ASuperLaserProActor`
2. Set `DeviceID`
3. Adjust LaserComponent parameters (BeamLength, ProjectionAngle etc.)
4. Ensure Beyond software is running and outputting to the corresponding device

---

### 23.15 Lift Matrix

**Characteristics**: 5 groups of lift effect components + wire rope visualization

**Base class**: `ASuperLiftMatrix` (inherits `ASuperLightBase`, has Pan/Tilt)

**Fixture library definition** (typical channels):

| Channel | Attribute Name | Category | Description |
|------|--------|------|------|
| 1-2 | Pan | Position | Horizontal rotation |
| 3-4 | Tilt | Position | Vertical rotation |
| 5 | Lift | Position | Lift position |
| 6 | EffectSelect | Effect | Effect selection |
| 7 | EffectColorR | Color | Effect red |
| 8 | EffectColorG | Color | Effect green |
| 9 | EffectColorB | Color | Effect blue |

**Built-in component structure**:
```
SceneHard
 ├─ LiftComponents[0..4] (5 scene components, 5cm spacing)
 │   └─ EffectComponents[0,1] (2 SuperEffectComponents per, 10 total)
 ├─ CableOrigin (wire rope origin)
 └─ CableMeshes[0..3] (4 wire rope cylinders, four corners)
```

**Initialization** (OnConstruction + BeginPlay):
```
SetEffectMatrixDefaultValue()   // Initialize all effect component materials
```

**SuperDMXTick**:
```
Pan(DmxPan)
Tilt(DmxTilt)
LiftMatrix(DmxLift)                                          // Lift control
SetEffectMatrix(DmxEffectSelect)                              // Effect selection
SetEffectColorMatrix(DmxColorR, DmxColorG, DmxColorB)        // Effect color
```

---

### 23.16 Fixture Type Quick Reference

| Fixture Type | Base Class | Core Component | Has DMX | Has Pan/Tilt | Typical Channel Count |
|---------|------|---------|--------|------------|-----------|
| Beam light | ASuperStageLight | USuperBeamComponent | ✅ | ✅ | 15-20 |
| Spot light | ASuperStageLight | USuperBeamComponent | ✅ | ✅ | 18-25 |
| Wash light | ASuperStageLight | USuperSpotComponent | ✅ | ✅ | 10-15 |
| Profile light | ASuperStageLight | USuperCuttingComponent | ✅ | ✅ | 25-35 |
| Matrix light | ASuperStageLight | USuperMatrixComponent | ✅ | ✅ | 30-200+ |
| LED strip | ASuperLightStripEffect | Built-in effect material | ✅ | ❌ | 8-10 |
| VFX effect | ASuperStageVFXActor | UNiagaraComponent | ✅ | ✅ | 5-10 |
| DMX Camera | ASuperDMXCamera | UCineCameraComponent | ✅ | ❌ | 6-9 |
| Lifting machinery | ASuperLiftingMachinery | None (motion only) | ✅ | ❌ | 6 |
| Rail machinery | ASuperRailMachinery | USplineComponent | ✅ | ❌ | 7 |
| NDI screen | ASuperNDIScreen | Built-in texture system | ❌ | ❌ | 0 |
| Projector | ASuperProjector | 3×SpotLight | ❌ | ❌ | 0 |
| Laser (texture) | ASuperLaserActor | Built-in material | ❌ | ❌ | 0 |
| Laser (Pro) | ASuperLaserProActor | USuperLaserProComponent | ❌ | ❌ | 0 |
| Lift matrix | ASuperLiftMatrix | USuperEffectComponent×10 | ✅ | ✅ | 8-12 |

---

## 24. Fixture Library Editor Usage Guide

### 24.1 Creating Fixture Library Assets

In Content Browser, **right-click** → find **SuperStage** category → click **SuperFixtureLibrary** to create a new fixture library asset.

> Assets are created by the `UFixtureLibraryFactory` factory, default containing one empty module named `Mode 1`.

### 24.2 Opening the Fixture Library Editor

In Content Browser, **double-click** an existing `SuperFixtureLibrary` asset to open the fixture library editor.

> Underlying flow: `FAssetTypeActions_FixtureLibrary::OpenAssetEditor` → creates `FFixtureLibraryEditorToolkit` → embeds `SFixtureLibraryEditor` widget.

### 24.3 Editor Layout

```
┌──────────────────────────────────────────────────┐
│ Fixture Name: [___] Manufacturer: [___] Source: [▼] Power: [___] Weight: [___] │
├──────────────────────────────────────────────────┤
│ Breadcrumb nav: Module list > Mode 1 > Dimmer > SubAttr 1 > ChannelSet │
├────────┬───────────────────────────────────────┤
│ Module │ Attribute / Sub-attribute / Channel set list (switches by nav level) │
│ List   │ No | Attrib | Category | Coarse | Fine | Ultra | Default │
│ Mode 1 │  1 | Dimmer |  Dimmer   |   1    |  2   |  0    |   0     │
│ Mode 2 │  2 | Pan    |  Position |   3    |  4   |  0    |  128    │
│ [+][-]  │  3 | Tilt   |  Position |   5    |  6   |  0    |  128    │
│         │  4 | ColorR |  Color    |   7    |  0   |  0    |   0     │
│         │ [+ Attribute] [- Attribute]                           │
├──────────────────────────────────────────────────┤
│ [Total channels: 12] [Save] [Save As]                              │
└──────────────────────────────────────────────────┘
```

### 24.4 Operation Flow

1. **Add module** — Click `[+]` in the module list on the left
2. **Select module** — Click module name to enter attribute list
3. **Add attribute** — Click `[+ Attribute]`
4. **Edit attribute** — Modify name, category, channel number directly in the table
5. **Enter sub-attribute** — Click attribute row to enter sub-attribute level
6. **Edit sub-attribute** — Set DmxMin/DmxMax/PhysicalRange
7. **Enter channel sets** — Click sub-attribute row to enter channel set level
8. **Edit channel sets** — Different edit controls shown based on attribute category:
   - **Gobo type**: GoboMode + Texture selector
   - **Color type**: Color picker + ColorIndex
   - **Prism type**: PrismFacets + PrismRadius + PrismScale

### 24.5 Attribute Naming Convention

The attribute name `AttribName` is the **only link** between the fixture library and the Actor. Must:
- Match `FSuperDMXAttribute.AttribName` exactly (case-sensitive)
- Not be duplicated within the same module
- Recommended to use English naming (see appendix)

---

## 25. FAQ and Troubleshooting

### Q1: Fixture not responding

**Troubleshooting steps**:
1. Confirm `SuperDMXFixture` Universe and StartAddress are correct
2. Confirm `FixtureLibrary` is assigned
3. Confirm attribute names match (case-sensitive)
4. Confirm `SetLightingDefaultValue` called before control functions
5. Confirm DMX data source (Art-Net/sACN) is sending

### Q2: Light flickering

**Possible causes**:
- Strobe channel not correctly configured (StrobeMode sub-attribute missing → default Open)
- DimmerCurveExponent set to 1.0 (linear response causes low-intensity jitter)
- SetLightingDefaultValue called repeatedly in Tick/SuperDMXTick (should only be called once each in OnConstruction and BeginPlay)

### Q3: Color inaccurate

**Troubleshooting steps**:
1. Confirm RGB channel DMXChannelType is correct (usually 8-bit)
2. In color wheel mode, confirm ColorAtlas texture is set
3. In CMY mixing, confirm formula direction (C=1 means full cyan, subtracts red)
4. In CTO channel, confirm temperature range direction

### Q4: Matrix light only some pixels working

**Troubleshooting steps**:
1. Confirm each module's `Patch` in library is correct (distinguishes address offset for different pixels)
2. Confirm all modules' attribute names are exactly the same
3. Confirm `USuperMatrixComponent.SegCount` matches module count
4. Use `GetChannelSpan()` to verify total channel count

### Q5: Can't see channel parameters in editor

Confirm `bChannelEdit = true` (check in Blueprint subclass's Class Defaults).

### Q6: Material not initialized (component solid black/transparent)

> **26Q2.2 Update**: Components now auto-initialize! On component registration, `OnRegister()` automatically calls initialization functions, no need to manually call in the Actor.

If issues still occur, check:
- Whether component's `StaticMeshLens` / `StaticMeshEffect` / `StaticMeshMatrix` are correctly assigned
- Whether material assets (`LensMaterial` / `LightSpotMaterial` / `BeamMaterial` etc.) are correctly set
- Whether component is correctly attached to the head transform component

Older versions (26Q2.1 and earlier) need manual calls:
- Beam component: `SetLightingDefaultValue(SuperLighting)`
- Effect component: `SetEffectDefaultValue(SuperEffect)`
- Matrix component: `SetMatrixMaterial()` (called on `USuperMatrixComponent`)

---

## 26. Appendix: Standard Attribute Name Reference

### Position Type

| Attribute Name | Description | Typical Precision | Notes |
|--------|------|---------|------|
| `Pan` | Horizontal rotation | 16-bit | Default ±270° |
| `Tilt` | Vertical rotation | 16-bit | Default ±135° |
| `PTSpeed` | PT speed | 8-bit | 0=fastest |
| `PanRot` | Pan infinite rotation | 8-bit | Requires sub-attribute RotationMode |
| `TiltRot` | Tilt infinite rotation | 8-bit | Requires sub-attribute RotationMode |
| `InPosZ` | Lift position | 8-bit | 0=lowest, 1=highest |

### Intensity/Strobe Type

| Attribute Name | Description | Typical Precision |
|--------|------|---------|
| `Dimmer` | Intensity | 16-bit |
| `Strobe` | Strobe | 8-bit |
| `StrobeMode` | Strobe mode | 8-bit |

### Color Type

| Attribute Name | Description | Typical Precision |
|--------|------|---------|
| `ColorR` | Red | 8-bit |
| `ColorG` | Green | 8-bit |
| `ColorB` | Blue | 8-bit |
| `ColorW` | White | 8-bit |
| `ColorWheel` / `Color1` | Color wheel 1 | 8-bit |
| `Color2` / `Color3` | Color wheel 2/3 | 8-bit |
| `CyanMix` / `MagentaMix` / `YellowMix` | CMY mixing | 8-bit |
| `CTO` | Color temperature adjustment | 8-bit |
| `Cool` / `Warm` | Cool-warm mixing | 8-bit |
| `Hue` / `Saturation` | Hue/Saturation | 8-bit |

### Beam Type

| Attribute Name | Description | Typical Precision |
|--------|------|---------|
| `Zoom` | Zoom | 8-bit or 16-bit |
| `Focus` | Focus | 8-bit |
| `Frost` | Frost | 8-bit |
| `Iris` | Iris | 8-bit |

### Gobo Type

| Attribute Name | Description | Typical Precision |
|--------|------|---------|
| `Gobo1` | Gobo wheel 1 | 8-bit |
| `Gobo1Rot` | Gobo wheel 1 rotation | 8-bit |
| `Gobo2` / `Gobo2Rot` | Gobo wheel 2 | 8-bit |
| `Gobo3` / `Gobo3Rot` | Gobo wheel 3 | 8-bit |

### Prism Type

| Attribute Name | Description | Typical Precision |
|--------|------|---------|
| `Prism1` | Prism 1 | 8-bit |
| `Prism2` | Prism 2 | 8-bit |
| `PrismRot` | Prism rotation | 8-bit |

### Cutting Type

| Attribute Name | Description | Typical Precision |
|--------|------|---------|
| `A1`~`A4` | Blade A side (1-4) | 8-bit |
| `B1`~`B4` | Blade B side (1-4) | 8-bit |
| `ShaperRot` | Cutting system rotation | 8-bit |

### Effect Type

| Attribute Name | Description | Typical Precision |
|--------|------|---------|
| `EffectDimmer` | Effect intensity | 8-bit |
| `EffectStrobe` | Effect strobe | 8-bit |
| `EffectValue` | Effect selection | 8-bit |
| `EffectColorR/G/B` | Effect color | 8-bit |

### Matrix Type

| Attribute Name | Description | Typical Precision |
|--------|------|---------|
| `MatrixDimmer` | Matrix intensity | 8-bit |
| `MatrixStrobe` | Matrix strobe | 8-bit |
| `MatrixR/G/B` | Matrix color | 8-bit |
| `MatrixW` | Matrix white | 8-bit |

> **Note**: The above attribute names are only recommended conventions and can be freely named. The only requirement is that `AttribName` in the fixture library exactly matches `FSuperDMXAttribute.AttribName` in the Actor.
