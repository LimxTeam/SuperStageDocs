# Component Classes API Documentation

> **Document Version**: 1.0  
> **Applicable Version**: SuperStage Plugin 2025/2026  
> **Target Audience**: Blueprint Developers / C++ Plugin Integration Developers (without source code modification permissions)  
> **Last Updated**: 2026-03

---

## Table of Contents

1. [Component Inheritance Hierarchy](#1-component-inheritance-hierarchy)
2. [USuperLightingComponent — Lighting Component Base Class](#2-usuperlightingcomponent--lighting-component-base-class)
3. [USuperSpotComponent — Spotlight Component](#3-usuperspotcomponent--spotlight-component)
4. [USuperBeamComponent — Volumetric Beam Component](#4-usuperbeamcomponent--volumetric-beam-component)
5. [USuperCuttingComponent — Cutting Beam Component](#5-usupercuttingcomponent--cutting-beam-component)
6. [USuperRectComponent — Rectangular Area Light Component](#6-usuperrectcomponent--rectangular-area-light-component)
7. [USuperEffectComponent — Effect Plane Component](#7-usupereffectcomponent--effect-plane-component)
8. [USuperMatrixComponent — Matrix Light Component](#8-usupermatrixcomponent--matrix-light-component)
9. [USuperLiftComponent — Lift Component](#9-usuperliftcomponent--lift-component)
10. [USuperLaserProComponent — Laser Pro Component](#10-usuperlaserprocomponent--laser-pro-component)
11. [Auxiliary Light Classes](#11-auxiliary-light-classes)
12. [API Quick Reference](#12-api-quick-reference)

---

## 1. Component Inheritance Hierarchy

```
USceneComponent
 ├─ USuperLightingComponent (Lighting Base Class)
 │   ├─ USuperSpotComponent (Spotlight = SpotLight + Lens + Spot Material)
 │   │   └─ USuperBeamComponent (Volumetric Beam = SpotLight + Beam Mesh + Beam Material)
 │   │       └─ USuperCuttingComponent (Cutting Beam = Volumetric Beam + Cutting Material)
 │   └─ USuperRectComponent (Rectangular Area Light = RectLight + Lens)
 ├─ USuperEffectComponent (Effect Plane = Static Mesh + Effect Material)
 ├─ USuperMatrixComponent (Matrix Light = Static Mesh + Segmented Material + Segmented Lights)
 ├─ USuperLiftComponent (Lift Control)
 └─ USuperLaserProComponent (Laser Line = Procedural Mesh)
```

**Design principles**:
- Each component encapsulates one physical light source or visual effect
- The Actor (e.g., `ASuperStageLight`) combines multiple components to form a complete fixture
- Components are uniformly controlled through virtual function interfaces, the Actor layer doesn't need to care about specific implementations

---

## 2. USuperLightingComponent — Lighting Component Base Class

**Header File**: `LightComponent/SuperLightingComponent.h`  
**Base Class**: `USceneComponent`  
**Export Macro**: `SUPERSTAGE_API`  
**Blueprint Identifier**: `BlueprintSpawnableComponent`, `DisplayName = "SuperLightComponent"`

Base class for all lighting components, providing common interfaces for lens mesh, spot material, intensity/color/strobe, etc.

### Sub-components

| Component | Type | Description |
|------|------|------|
| `YStaticMeshLens` | `UStaticMeshComponent*` | Lens mesh (created at runtime) |
| `YArrowComponent` | `UArrowComponent*` | Direction arrow (editor aid) |

### Default Parameters

| Property | Type | Default | Description |
|------|------|--------|------|
| `StaticMeshLens` | `UStaticMesh*` | — | Lens model |
| `LensTransform` | `FTransform` | `Identity` | Lens transform |
| `Angle` | `float` | `1.0` | Spot angle |
| `DimmerCurveExponent` | `float` | `2.0` | Intensity response curve exponent (1-3) |
| `MaxLightIntensity` | `float` | `1.0` | Component-level intensity control (0-1) |

> **Initialization note**: All default parameters are automatically initialized by component `OnRegister()`. On component registration, `SetLightingMaterial()` and `SetLightingDefaultValue()` are automatically called, no need to manually call in the Actor.

#### DimmerCurveExponent Description

| Value | Curve | Recommended Scenario |
|----|------|---------|
| `1.0` | Linear | Not recommended (too much variation in low brightness range) |
| `2.0` | Quadratic | **Recommended** (simulates real dimming consoles) |
| `2.2` | Gamma | Monitor standard calibration |
| `3.0` | Cubic | Strong non-linear response |

**Formula**: `Output = pow(Input, DimmerCurveExponent)`

#### MaxLightIntensity Description

Component-level intensity control (formerly ComponentDimmer), multiplicatively stacked with Actor-level Dimmer:

```
FinalIntensity = ActorDimmer × MaxLightIntensity × StrobeMultiplier
```

### Materials

| Property | Type | Description |
|------|------|------|
| `LensMaterial` | `UMaterialInstance*` | Lens source material |
| `LightSpotMaterial` | `UMaterialInstance*` | Light spot source material |
| `DynamicMaterialLens` | `UMaterialInstanceDynamic*` | Lens dynamic material |
| `DynamicMaterialLightSpot` | `UMaterialInstanceDynamic*` | Light spot dynamic material |

### Ray Detection Parameters (Internal)

| Property | Default | Description |
|------|--------|------|
| `MinReturnDistance` | `100.0` cm | Minimum ray return distance |
| `bSweepFallback` | `true` | Sweep fallback on miss |
| `SweepRadius` | `2.0` cm | Sweep radius |
| `TraceChannel` | `ECC_Visibility` | Collision channel |

### Functions

#### Initialization

```cpp
virtual void SetLightingMaterial();
```
Creates dynamic material instances and binds to sub-components.

```cpp
virtual void SetLightingDefaultValue(
    float NewMaxLightDistance,
    float NewLightMaxIntensity,
    float VolumetricScattering,
    bool LightShadow,
    float NewLightSpotIntensity = 100.0f,
    float LensIntensity = 1.0f);
```
Sets light default parameters.

---

#### Intensity and Strobe

```cpp
virtual void SetLightingIntensity(float NewLightIntensity = 1.0f);
virtual void SetLightingStrobe(float NewStrobe = 0.0f);
virtual void SetLightingStrobeMode(float NewStrobeMode = 1.0f);
virtual void SetRandomSeed(float NewSeed);
```

**Strobe modes** (`StrobeMode` values):

| Value | Mode | Description |
|----|------|------|
| 0 | Close | Always off |
| 1 | Open | Always on (default) |
| 2 | Strobe | Standard strobe |
| 3 | Pulse | Pulse |
| 4 | RampUp | Fade in |
| 5 | RampDown | Fade out |
| 6 | Sine | Sine wave |
| 7 | Random | Random |

**Strobe implementation**: `TickComponent` calls `CalculateStrobeMultiplier()` to compute the current frame's strobe multiplier, then applies to the light source via `UpdateLightIntensity()`.

```cpp
UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetComponentDimmer"))
void SetComponentDimmer(float NewDimmer);
```
Sets component-level intensity control.

---

#### Color and Optics

```cpp
virtual void SetLightingColor(FLinearColor NewColor = FLinearColor(1,1,1));
virtual void SetLightingZoom(float NewZoom = 0.0f);
virtual void SetLightingFrost(float NewFrost = 0.0f);
virtual void SetLightingIris(float NewIris = 1.0f);
```

---

#### Textures

```cpp
virtual void SetLightingTexture(UTexture2D* NewBeamTexture);

// Color wheel textures (base class empty implementation, subclasses override)
virtual void SetColorTexture(UTexture2D*, int32 NumColors, float ColorIndex, float ColorSpeed);
virtual void SetColorTexture2(...);
virtual void SetColorTexture3(...);
```

---

#### Rotation and Visibility

```cpp
UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetLightingRotate"))
virtual void SetLightingRotate(float NewRotate = 0, float NewInfiniteRotation = 0);

UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetLightingVisibility"))
virtual void SetLightingVisibility(bool bNewVisibility = false);

virtual void SetLightingLensVisibility(bool bLensVisibility = true);
```

---

#### Advanced Lighting Settings

```cpp
UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetLightingChannels"))
virtual void SetLightingChannels(bool bChannel0 = true, bool bChannel1 = false, bool bChannel2 = false);

UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetLightingTransmission"))
virtual void SetLightingTransmission(bool bAffectTransmission = true);

virtual void SetSpecularScale(float NewSpecularScale = 1.0f);
virtual void SetSourceRadius(float NewSourceRadius = 0.0f);
```

---

#### Ray Detection

```cpp
float GetRayDetectionDistance(float NewMaxLightDistance) const;
virtual void UpdateBeamBlockDistance(float NewMaxLightDistance);
```

`GetRayDetectionDistance`: Performs ray detection from component position along forward direction for collision, returns hit distance. Supports Sweep fallback and minimum return distance limit.

`UpdateBeamBlockDistance`: Updates beam occlusion distance (called by `ASuperStageLight::UpdateBeamOcclusion`).

---

## 3. USuperSpotComponent — Spotlight Component

**Header File**: `LightComponent/SuperSpotComponent.h`  
**Base Class**: `USuperLightingComponent`  
**Export Macro**: `SUPERSTAGE_API`  
**Blueprint Identifier**: `DisplayName = "SuperSpotComponent"`

Inherits the lighting base class, adding an actual SpotLight light source. Suitable for fixtures that need real light projection (fill light, supplementary light).

### Sub-components

| Component | Type | Description |
|------|------|------|
| `YSpotLight` | `USilentSpotLightComponent*` | Spotlight light source (silent version, bypasses GPU Scene synchronization issues) |

### Default Parameters

| Property | Type | Default | Description |
|------|------|--------|------|
| `ZoomRange` | `FVector2D` | `(1, 10)` | Zoom angle range (degrees) |
| `bDisableLightFunction` | `bool` | `false` | Disable light function (use Tick for direct intensity control) |

### Overridden Functions

All `USuperLightingComponent` virtual functions are overridden to synchronously update `YSpotLight` parameters:

- `SetLightingMaterial()` — Create SpotLight and set materials
- `SetLightingDefaultValue(...)` — Configure SpotLight parameters (attenuation, shadows, volumetric fog, etc.)
- `SetLightingIntensity(float)` — Update SpotLight intensity
- `SetLightingColor(FLinearColor)` — Update SpotLight color
- `SetLightingZoom(float)` — Update OuterConeAngle (via ZoomRange mapping)
- `SetLightingFrost(float)` — Adjust InnerConeAngle to simulate frost
- `SetLightingIris(float)` — Adjust spot size
- `SetLightingRotate(float, float)` — Rotate light function
- `SetLightingVisibility(bool)` — SpotLight visibility
- `SetLightingChannels(...)` — Lighting channel settings
- `SetLightingTransmission(bool)` — Transmission toggle
- `SetSpecularScale(float)` — Specular scale
- `SetSourceRadius(float)` — Soft shadow radius
- `SetColorTexture/2/3(...)` — Color wheel textures

### Internal Cache

| Property | Description |
|------|------|
| `LastSegIdx` | Last frame's Zoom segment index (accelerates lookup) |
| `LastConeDeg` | Last dispatched cone angle (debounce) |
| `LastAttenuationRadius` | Last dispatched attenuation radius (debounce) |
| `BaseOuterConeAngle` | Base outer cone angle (unaffected by Frost/Iris) |
| `CurrentZoomValue` | Current Zoom value |
| `CurrentIrisValue` | Current Iris value |
| `CurrentFrostValue` | Current Frost value |

### USilentSpotLightComponent

```cpp
UCLASS()
class SUPERSTAGE_API USilentSpotLightComponent : public USpotLightComponent
```

Safe version of SpotLight, provides `SetAttenuationRadiusSafe(float)`:
- Bypasses `PushRadiusToRenderThread()` to avoid GPU Scene stale assertions
- Only sets properties + `MarkRenderStateDirty()`, letting the engine update uniformly at frame end

---

## 4. USuperBeamComponent — Volumetric Beam Component

**Header File**: `LightComponent/SuperBeamComponent.h`  
**Base Class**: `USuperSpotComponent`  
**Export Macro**: `SUPERSTAGE_API`  
**Blueprint Identifier**: `DisplayName = "SuperBeamComponent"`

Adds a volumetric beam mesh on top of SpotLight, achieving visible light column effects through beam materials.

### Inheritance Chain

```
USceneComponent → USuperLightingComponent → USuperSpotComponent → USuperBeamComponent
```

### Sub-components

| Component | Type | Description |
|------|------|------|
| `YStaticMeshBeam` | `UStaticMeshComponent*` | Beam mesh |
| (inherited) `YSpotLight` | `USilentSpotLightComponent*` | SpotLight light source |

### Materials

| Property | Type | Description |
|------|------|------|
| `BeamMaterial` | `UMaterialInstance*` | Beam source material |
| `DynamicMaterialBeam` | `UMaterialInstanceDynamic*` | Beam dynamic material |

### Functions

#### Beam Initialization

```cpp
void SetBeamDefaultValue(
    float NewMaxLightDistance,
    float NewLightMaxIntensity,
    float AtmosphericDensity,
    float LensRadius,
    float NewBeamQuality,
    float NewFogInfluence,
    float NewFogSpeed,
    float NewBeamIntensity = 1.0f) const;
```

Sets default parameters for the beam material.

**Parameter descriptions**:

| Parameter | Description |
|------|------|
| `AtmosphericDensity` | Atmospheric attenuation density (0-1) |
| `LensRadius` | Lens size (%), affects beam width |
| `NewBeamQuality` | Beam render quality (%) |
| `NewFogInfluence` | Beam fog intensity (%) |
| `NewFogSpeed` | Beam fog flow speed (%) |
| `NewBeamIntensity` | Beam base intensity |

---

#### Beam Texture (Gobo)

```cpp
void SetBeamTexture(
    UTexture2D* NewBeamTexture = nullptr,
    int32 NewNumGobos = 1,
    float GoboIndex = 0.f,
    float GoboSpeed = 0.f,
    float ShakeSpeed = 0.f) const;
```

Sets beam texture/gobo wheel parameters.

| Parameter | Description |
|------|------|
| `NewBeamTexture` | Gobo atlas texture |
| `NewNumGobos` | Number of gobos |
| `GoboIndex` | Current gobo index |
| `GoboSpeed` | Gobo rotation speed |
| `ShakeSpeed` | Gobo shake speed |

---

#### Prism

```cpp
void SetBeamPrism(
    int32 PrismFacets = 0,
    float PrismRadius = 0.3f,
    float PrismScale = 0.15f,
    float PrismRotation = 0.f,
    float PrismRotationSpeed = 0.f) const;
```

| Parameter | Description |
|------|------|
| `PrismFacets` | Number of prism facets (0=off, 8/16/24 etc.) |
| `PrismRadius` | Prism radius |
| `PrismScale` | Prism scale |
| `PrismRotation` | Prism rotation angle |
| `PrismRotationSpeed` | Prism rotation speed |

---

#### Focus

```cpp
void SetBeamFocus(float Focus = 0.f) const;
```

`0` = sharp, `1` = blurry.

---

### Overridden Functions

All `USuperSpotComponent` virtual functions are overridden to synchronously update beam material parameters. For example, `SetLightingIntensity` updates both the SpotLight intensity and the beam material's `Brightness` parameter.

---

## 5. USuperCuttingComponent — Cutting Beam Component

**Header File**: `LightComponent/SuperCuttingComponent.h`  
**Base Class**: `USuperBeamComponent`  
**Export Macro**: `SUPERSTAGE_API`  
**Blueprint Identifier**: `DisplayName = "SuperCuttingComponent"`

Adds four-blade cutting control on top of volumetric beam. Loads cutting-specific material in constructor.

### Inheritance Chain

```
USuperLightingComponent → USuperSpotComponent → USuperBeamComponent → USuperCuttingComponent
```

### Functions

#### SetCuttingValue

```cpp
void SetCuttingValue(
    float NewUpperLeftY,  float NewLowerLeftY,
    float NewTopRightY,   float NewBottomRightY,
    float NewTopLeftX,    float NewBottomLeftX,
    float NewTopRightX,   float NewBottomRightX) const;
```

Sets four-corner eight-point cutting parameters.

**Material parameter mapping**:

| Parameter | Material Param Name | Description |
|------|-----------|------|
| `NewUpperLeftY` | `UpperLeftY` | Upper-left Y position |
| `NewLowerLeftY` | `LowerLeftY` | Lower-left Y position |
| `NewTopRightY` | `TopRightY` | Upper-right Y position |
| `NewBottomRightY` | `BottomRightY` | Lower-right Y position |
| `NewTopLeftX` | `TopLeftX` | Upper-left X position |
| `NewBottomLeftX` | `BottomLeftX` | Lower-left X position |
| `NewTopRightX` | `TopRightX` | Upper-right X position |
| `NewBottomRightX` | `BottomRightX` | Lower-right X position |

---

#### SetLightingRotate (Override)

```cpp
virtual void SetLightingRotate(float NewRotate, float NewInfiniteRotation = 0.f) override;
```

Overrides parent rotation, also sets the rotation parameter of the LightSpot material (cutting and spot rotate synchronously).

---

## 6. USuperRectComponent — Rectangular Area Light Component

**Header File**: `LightComponent/SuperRectComponent.h`  
**Base Class**: `USuperLightingComponent`  
**Export Macro**: `SUPERSTAGE_API`  
**Blueprint Identifier**: `DisplayName = "SuperRectComponent"`

Rectangular area light component, bridges `URectLightComponent` with the lighting base class interface. Suitable for LED panel lights, Wash lights, and other area light sources.

### Sub-components

| Component | Type | Description |
|------|------|------|
| `YRectLight` | `USilentRectLightComponent*` | Rectangular area light source |

### Default Parameters

| Property | Type | Default | Range | Description |
|------|------|--------|------|------|
| `SourceWidth` | `float` | `20.0` | 0-200 cm | Light source width |
| `SourceHeight` | `float` | `20.0` | 0-200 cm | Light source height |
| `BarnDoorAngle` | `float` | `20.0` | 0-80° | Barn door angle |
| `BarnDoorLength` | `float` | `20.0` | 0-1000 cm | Barn door length |

### Overridden Functions

- `SetLightingMaterial()` — Create RectLight and set materials
- `SetLightingDefaultValue(...)` — Configure RectLight parameters
- `SetLightingIntensity(float)` — Update RectLight intensity
- `SetLightingColor(FLinearColor)` — Update RectLight color
- `SetLightingVisibility(bool)` — RectLight visibility
- `SetLightingChannels(...)` — Lighting channels
- `SetLightingTransmission(bool)` — Transmission toggle

### USilentRectLightComponent

```cpp
UCLASS()
class SUPERSTAGE_API USilentRectLightComponent : public URectLightComponent
```

Empty stub subclass, used to hide editor icons.

---

## 7. USuperEffectComponent — Effect Plane Component

**Header File**: `LightComponent/SuperEffectComponent.h`  
**Base Class**: `USceneComponent`  
**Export Macro**: `SUPERSTAGE_API`  
**Blueprint Identifier**: `DisplayName = "SuperEffectComponent"`

Effect plane component, using static mesh + dynamic material to achieve LED effects (flow, chase, pulse, etc.). Supports opaque/transparent material switching.

### Sub-components

| Component | Type | Description |
|------|------|------|
| `YStaticMeshEffect` | `UStaticMeshComponent*` | Effect mesh |

### Default Parameters

| Property | Type | Default | Description |
|------|------|--------|------|
| `StaticMeshEffect` | `UStaticMesh*` | — | Effect mesh model |
| `EffectMaterialNo` | `int` | `0` | Material number |
| `bTransparent` | `bool` | `false` | Transparent material toggle |
| `EffectTransform` | `FTransform` | `Identity` | Effect mesh transform |
| `MaxLightIntensity` | `float` | `1.0` | Component-level intensity control |

### Materials

| Property | Type | Description |
|------|------|------|
| `EffectMaterial` | `UMaterialInstance*` | Opaque effect material |
| `EffectMaterialTransparent` | `UMaterialInstance*` | Transparent effect material |
| `DynamicMaterialEffect` | `UMaterialInstanceDynamic*` | Current dynamic material |

### Functions

#### Initialization

```cpp
void SetEffectMaterial(float NewIntensity = 1.0f);
```

Creates dynamic material instance and applies to mesh. Selects source material based on `bTransparent`.

---

#### Intensity and Strobe

```cpp
void SetEffectIntensity(float NewIntensity);
void SetEffectStrobe(float NewStrobe = 0.f);
void SetEffectStrobeMode(float NewStrobeMode = 1.f);
void SetRandomSeed(float NewSeed);
```

```cpp
UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetComponentDimmer"))
void SetComponentDimmer(float NewDimmer);
```

---

#### Color and Effect

```cpp
void SetEffectColor(FLinearColor NewColor = FLinearColor(1,1,1)) const;
void SetEffectControl(float NewEffect = 0.f, float NewSpeed = 1.f, float NewWidth = 0.5f) const;
```

---

#### Combined Control

```cpp
void SetEffectsControl(
    float NewIntensity = 1.f,
    float NewStrobe = 0.f,
    FLinearColor NewColor = FLinearColor(1,1,1),
    float NewDirection = 0.f,
    float NewEffect = 0.f,
    float NewSpeed = 1.f,
    float NewWidth = 0.5f) const;
```

Sets all effect parameters at once (intensity / strobe / color / direction / effect / speed / width).

---

## 8. USuperMatrixComponent — Matrix Light Component

**Header File**: `LightComponent/SuperMatrixComponent.h`  
**Base Class**: `USceneComponent`  
**Export Macro**: `SUPERSTAGE_API`  
**Blueprint Identifier**: `DisplayName = "SuperMatrixComponent"`

Matrix light component, controlling color, strobe, and intensity by segments. Each segment corresponds to one pixel. Supports segment lights (PointLight/SpotLight) to simulate real lighting.

### Sub-components

| Component | Type | Description |
|------|------|------|
| `YStaticMeshMatrix` | `UStaticMeshComponent*` | Matrix mesh |
| `SegmentPointLights` | `TArray<USilentPointLightComponent*>` | Segment point light array |
| `SegmentSpotLights` | `TArray<USilentMatrixSpotLightComponent*>` | Segment spotlight array |

### Default Parameters

| Property | Type | Default | Description |
|------|------|--------|------|
| `SegCount` | `int32` | `5` | Segment count (≤200) |
| `bUseUAxis` | `bool` | `true` | Segment direction (true=U axis, false=V axis) |
| `StaticMeshMatrix` | `UStaticMesh*` | — | Matrix mesh model |
| `bTransparent` | `bool` | `false` | Transparent material toggle |
| `MatrixTransform` | `FTransform` | `Identity` | Mesh transform |
| `MaxLightIntensity` | `float` | `1.0` | Component-level intensity control |

### Segment Light Configuration

| Property | Type | Default | Description |
|------|------|--------|------|
| `SegmentLightType` | `ESegmentLightType` | `None` | Light source type |
| `SegmentLightIntensity` | `float` | `1000.0` | Light source intensity (lumens) |
| `SegmentLightRadius` | `float` | `2400.0` | Attenuation radius |
| `SpotOuterConeAngle` | `float` | `90.0°` | SpotLight outer cone angle |

#### ESegmentLightType

| Value | Description |
|----|------|
| `None` | No light source (material glow only) |
| `PointLight` | Point light |
| `SpotLight` | Spotlight |

### Materials

| Property | Type | Description |
|------|------|------|
| `MatrixMaterial` | `UMaterialInstance*` | Opaque matrix material |
| `MatrixMaterialTransparent` | `UMaterialInstance*` | Transparent matrix material |
| `DynamicMaterialMatrix` | `UMaterialInstanceDynamic*` | Current dynamic material |

**Material parameter naming**: Segment color parameter names are `C00`, `C01`, ..., `C99` (consistent with SuperLightMatrix material).

### Functions

#### Initialization

```cpp
UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetMatrixMaterial"))
void SetMatrixMaterial(float NewIntensity = 1.0f);
```

Creates dynamic material instance, pushes `SegCount`/`UseUAxis` core parameters.

---

#### Color

```cpp
UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetSegmentColor"))
void SetSegmentColor(int32 Index = 0, FLinearColor RGB = FLinearColor::White);
```

Sets single segment color. `Index` starts from 0.

---

#### Strobe

```cpp
UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetMatrixStrobe"))
void SetMatrixStrobe(float NewStrobe = 255.f);

UFUNCTION(BlueprintCallable)
void SetMatrixStrobeMode(float NewStrobeMode = 1.f);
```

---

#### Intensity

```cpp
UFUNCTION(BlueprintCallable)
void SetMatrixIntensity(float NewIntensity);

UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetComponentDimmer"))
void SetComponentDimmer(float NewDimmer);
```

---

#### Segment Light Management

```cpp
UFUNCTION(BlueprintCallable)
void RebuildSegmentLights();    // Rebuild segment lights

UFUNCTION(BlueprintCallable)
void DestroyAllSegmentLights(); // Destroy all segment lights

UFUNCTION(BlueprintCallable)
void UpdateLightParameters();   // Update light parameters (no rebuild needed)
```

---

## 9. USuperLiftComponent — Lift Component

**Header File**: `LightComponent/SuperLiftComponent.h`  
**Base Class**: `USceneComponent`  
**Export Macro**: `SUPERSTAGE_API`  
**Blueprint Identifier**: `DisplayName = "SuperLiftComponent"`

Z-axis lift control component. Does not auto-update via Tick; actively called by the parent Actor.

### Functions

```cpp
void SetLiftZ(float InPosZ = 0.f, float LiftRange = 100.f, float LiftSpeed = 1.f);
```

| Parameter | Description |
|------|------|
| `InPosZ` | Normalized lift position `[0, 1]` |
| `LiftRange` | Lift range (cm) |
| `LiftSpeed` | Interpolation speed |

**Behavior**: `Clamp(InPosZ, 0, 1) → Lerp(0, LiftRange) → FInterpTo(CurrentZ, Target, DeltaTime, LiftSpeed) → SetRelativeLocation(0, 0, NewZ)`

### Cache

| Property | Type | Description |
|------|------|------|
| `CurrentZ` | `float` | Current interpolated Z position |

---

## 10. USuperLaserProComponent — Laser Pro Component

**Header File**: `LightComponent/SuperLaserProComponent.h`  
**Base Class**: `USceneComponent`  
**Export Macro**: `SUPERSTAGE_API`  
**Blueprint Identifier**: `DisplayName = "SuperLaserProComponent"`

Implements point data-driven laser line visualization via procedural mesh. Each point corresponds to one laser line from the source to the target point.

### Sub-components (Internal)

| Component | Type | Description |
|------|------|------|
| `YBeamMeshComponent` | `UProceduralMeshComponent*` | Laser line mesh |
| `DynamicMaterialInstance` | `UMaterialInstanceDynamic*` | Laser material |

### Default Parameters

| Property | Type | Default | Range | Description |
|------|------|--------|------|------|
| `BeamLength` | `float` | — | 100-50000 cm | Laser projection distance |
| `ProjectionAngle` | `float` | — | 1-90° | Projection angle range |
| `LaserWidth` | `float` | — | 0.1-20 cm | Laser line width |
| `CoreSharpness` | `float` | — | 1-10 | Core sharpness (concentration) |
| `DepthFade` | `float` | — | 0-1 | Depth fade (0=no fade) |
| `Dim` | `float` | — | 1-10 | Self-illumination intensity (exponential curve) |
| `OpacityScale` | `float` | — | 0-1 | Overall transparency |
| `FogSpeed` | `float` | — | 0-2 | Fog flow speed |
| `FogInfluence` | `float` | — | 0-1 | Fog influence intensity |
| `SpotDimmer` | `float` | — | 0-5 | Spot brightness (on surface hit) |
| `MaxLightIntensity` | `float` | `1.0` | 0-1 | Component-level intensity control |
| `bEnableCollision` | `bool` | — | — | Collision occlusion detection |
| `BeamMaterial` | `UMaterialInterface*` | — | — | Laser line material |

### Functions

#### Core Interface

```cpp
UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetLaserPoints"))
void SetLaserPoints(const TArray<FLaserPoint>& InPoints);
```

Sets laser point data (obtained from Subsystem each frame). Automatically triggers mesh rebuild.

```cpp
UFUNCTION(BlueprintCallable, meta = (DisplayName = "RebuildMesh"))
void RebuildMesh();
```

Forces mesh rebuild.

```cpp
UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetLaserVisibility"))
void SetLaserVisibility(bool bNewVisibility);
```

Sets visibility.

```cpp
UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetComponentDimmer"))
void SetComponentDimmer(float NewDimmer);
```

Sets component-level intensity control.

### Performance Optimization

- **Mesh cache**: `CachedVertexCount` + `bMeshNeedsRebuild` to avoid rebuilding every frame
- **Material parameter cache**: Change detection, skip when value unchanged
- **Collision distance cache**: `HitDistances` array, ray detection only when `bEnableCollision`
- **Angle cache**: `CachedTanAngle` to avoid per-frame `tan(ProjectionAngle)` computation

### Render Flow

```
SetLaserPoints(InPoints)
 → CachedLaserPoints = InPoints
 → bMeshNeedsRebuild = true
 → TraceAllCollisions()  // Collision detection (optional)
 → BuildLaserMesh()      // Procedural mesh generation
 → UpdateMaterialParameters() // Material parameter synchronization
```

---

## 11. Auxiliary Light Classes

### USilentSpotLightComponent

```cpp
class SUPERSTAGE_API USilentSpotLightComponent : public USpotLightComponent
```

**Defined in**: `SuperSpotComponent.h`

Safe version of SpotLight. Provides `SetAttenuationRadiusSafe(float)` to bypass `PushRadiusToRenderThread()` and avoid GPU Scene synchronization assertions.

### USilentRectLightComponent

```cpp
class SUPERSTAGE_API USilentRectLightComponent : public URectLightComponent
```

**Defined in**: `SuperRectComponent.h`

Empty stub subclass, used to hide editor icons.

### USilentPointLightComponent

```cpp
class SUPERSTAGE_API USilentPointLightComponent : public UPointLightComponent
```

**Defined in**: `SuperMatrixComponent.h`

Silent version of point light, used for matrix segment lights.

### USilentMatrixSpotLightComponent

```cpp
class SUPERSTAGE_API USilentMatrixSpotLightComponent : public USpotLightComponent
```

**Defined in**: `SuperMatrixComponent.h`

Silent version of spotlight, used for matrix segment lights.

---

## 12. API Quick Reference

### Component Selection Guide

| Fixture Type | Recommended Component |
|---------|---------|
| Profile / Follow Spot | `USuperCuttingComponent` |
| Standard Moving Light (Spot/Beam) | `USuperBeamComponent` |
| Wash Light / LED Area Light | `USuperRectComponent` or `USuperSpotComponent` |
| LED Matrix Light | `USuperMatrixComponent` |
| Effect Light (LED Strip/Panel) | `USuperEffectComponent` |
| Laser (Beyond) | `USuperLaserProComponent` |
| Fixture Lift | `USuperLiftComponent` |
| Fill Light / Supplementary Light | `USuperSpotComponent` |

### Common Interface (USuperLightingComponent and Subclasses)

| Function | Description |
|------|------|
| `SetLightingMaterial()` | Initialize material |
| `SetLightingDefaultValue(...)` | Set default parameters |
| `SetLightingIntensity(float)` | Intensity |
| `SetLightingStrobe(float)` | Strobe speed |
| `SetLightingStrobeMode(float)` | Strobe mode |
| `SetLightingColor(FLinearColor)` | Color |
| `SetLightingZoom(float)` | Zoom |
| `SetLightingFrost(float)` | Frost |
| `SetLightingIris(float)` | Iris |
| `SetLightingTexture(UTexture2D*)` | Texture |
| `SetColorTexture/2/3(...)` | Color wheel texture |
| `SetLightingRotate(float, float)` | Rotation |
| `SetLightingVisibility(bool)` | Visibility |
| `SetLightingLensVisibility(bool)` | Lens visibility |
| `SetLightingChannels(...)` | Lighting channels |
| `SetLightingTransmission(bool)` | Transmission |
| `SetSpecularScale(float)` | Specular scale |
| `SetSourceRadius(float)` | Soft shadow radius |
| `SetComponentDimmer(float)` | Component intensity control |
| `GetRayDetectionDistance(float)` | Ray detection distance |
| `UpdateBeamBlockDistance(float)` | Beam occlusion distance |

### Beam-specific Interface (USuperBeamComponent)

| Function | Description |
|------|------|
| `SetBeamDefaultValue(...)` | Beam default parameters |
| `SetBeamTexture(...)` | Gobo texture |
| `SetBeamPrism(...)` | Prism |
| `SetBeamFocus(float)` | Focus |

### Cutting-specific Interface (USuperCuttingComponent)

| Function | Description |
|------|------|
| `SetCuttingValue(8 floats)` | Four-corner eight-point cutting |

### Effect-specific Interface (USuperEffectComponent)

| Function | Description |
|------|------|
| `SetEffectMaterial(float)` | Initialize |
| `SetEffectIntensity(float)` | Intensity |
| `SetEffectStrobe(float)` | Strobe |
| `SetEffectStrobeMode(float)` | Strobe mode |
| `SetEffectColor(FLinearColor)` | Color |
| `SetEffectControl(float,float,float)` | Effect / Speed / Width |
| `SetEffectsControl(...)` | Combined control |

### Matrix-specific Interface (USuperMatrixComponent)

| Function | Description |
|------|------|
| `SetMatrixMaterial(float)` | Initialize |
| `SetSegmentColor(int32, FLinearColor)` | Segment color |
| `SetMatrixStrobe(float)` | Strobe |
| `SetMatrixStrobeMode(float)` | Strobe mode |
| `SetMatrixIntensity(float)` | Intensity |
| `RebuildSegmentLights()` | Rebuild segment lights |
| `DestroyAllSegmentLights()` | Destroy segment lights |
| `UpdateLightParameters()` | Update light parameters |

### Laser-specific Interface (USuperLaserProComponent)

| Function | Description |
|------|------|
| `SetLaserPoints(TArray<FLaserPoint>)` | Set point data |
| `RebuildMesh()` | Rebuild mesh |
| `SetLaserVisibility(bool)` | Visibility |
