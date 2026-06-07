# ASuperLightBase & ASuperStageLight API Documentation

> **Document Version**: 1.0  
> **Applicable Version**: SuperStage Plugin 2025/2026  
> **Target Audience**: Blueprint Developers / C++ Plugin Integration Developers (without source code modification permissions)  
> **Last Updated**: 2026-03

---

## Table of Contents

1. [ASuperLightBase — Pan/Tilt Motion Control Layer](#1-asuperlightbase--pantilt-motion-control-layer)
   - [Component Structure](#11-component-structure)
   - [Default Parameters](#12-default-parameters)
   - [Control Parameters](#13-control-parameters)
   - [Channel Switches](#14-channel-switches)
   - [Position Control Functions](#15-position-control-functions)
   - [Matrix Position Control](#16-matrix-position-control)
   - [Infinite Rotation Control](#17-infinite-rotation-control)
   - [Matrix Infinite Rotation Control](#18-matrix-infinite-rotation-control)
   - [Auxiliary Control](#19-auxiliary-control)
2. [ASuperStageLight — Full-function Moving Light](#2-asuperstagelight--full-function-moving-light)
   - [Component Structure](#21-component-structure)
   - [Default Parameters](#22-default-parameters)
   - [Control Parameters](#23-control-parameters)
   - [Channel Switches](#24-channel-switches)
   - [Initialization Functions](#25-initialization-functions)
   - [Intensity Control](#26-intensity-control)
   - [Strobe Control](#27-strobe-control)
   - [Color Control](#28-color-control)
   - [Optical Control](#29-optical-control)
   - [Gobo Wheel Control](#210-gobo-wheel-control)
   - [Prism Control](#211-prism-control)
   - [Cutting Control](#212-cutting-control)
   - [Effect Control](#213-effect-control)
   - [Matrix Component Control](#214-matrix-component-control)
   - [Spot Fill Light Control](#215-spot-fill-light-control)
3. [FSuperColorChannel — Dynamic Color Mixing Channel](#3-fsupercolorchannel--dynamic-color-mixing-channel)
4. [FLightDefaultValue — Light Default Parameters](#4-flightdefaultvalue--light-default-parameters)
5. [Pan/Tilt Motion System Details](#5-pantilt-motion-system-details)
6. [Color System Details](#6-color-system-details)
7. [Usage Examples](#7-usage-examples)
8. [API Quick Reference](#8-api-quick-reference)

---

## 1. ASuperLightBase — Pan/Tilt Motion Control Layer

**Header File**: `LightActor/SuperLightBase.h`  
**Base Class**: `ASuperDmxActorBase`  
**Export Macro**: `SUPERSTAGE_API`

Provides basic horizontal (Pan) and vertical (Tilt) rotation control functionality. All moving light Actors with motion capability inherit from this class.

### Inheritance Chain

```
AActor → ASuperBaseActor → ASuperDmxActorBase → ASuperLightBase ← This class
```

---

### 1.1 Component Structure

```
SceneBase (inherited from ASuperDmxActorBase)
 └─ SceneRocker (Pan rotation axis, Yaw)
     └─ SceneHard (Tilt rotation axis, Roll)
         └─ Light head components (beam/model, created by subclasses)
```

| Component | Type | Category | Description |
|------|------|------|------|
| `SceneRocker` | `USceneComponent*` | `Z.Components` | Pan rotation axis, rotates on Yaw |
| `SceneHard` | `USceneComponent*` | `Z.Components` | Tilt rotation axis, rotates on Roll |

> **Rotation axis mapping**: Pan → `SceneRocker.Yaw`, Tilt → `SceneHard.Roll`. Other axes remain unchanged.

---

### 1.2 Default Parameters

| Property | Type | Default | Range | Description |
|------|------|--------|------|------|
| `PanRange` | `FVector2D` | `(-270, 270)` | — | Pan angle mapping range (degrees) |
| `TiltRange` | `FVector2D` | `(-135, 135)` | — | Tilt angle mapping range (degrees) |
| `InfiniteRotationalSpeed` | `float` | `1.0` | 0.01-10 | Infinite rotation speed multiplier |
| `PanAngle` | `float` | `0` | -180~180 | Manual Pan angle (degrees) |
| `TiltAngle` | `float` | `0` | -180~180 | Manual Tilt angle (degrees) |
| `LiftSpeed` | `float` | `1.0` | 0-10 | Lift interpolation speed |
| `LiftRange` | `float` | `500.0` | — | Lift range (cm) |

**Pan/Tilt mapping formula**:

```
Target Angle = Lerp(Range.X, Range.Y, DMX normalized value)

Example (default PanRange):
  DMX = 0.0 → -270°
  DMX = 0.5 → 0° (center)
  DMX = 1.0 → +270°
```

---

### 1.3 Control Parameters

These properties are automatically updated by DMX functions and can also be manually adjusted in the Details panel. All values are `[0, 1]` normalized.

| Property | Type | Default | Description |
|------|------|--------|------|
| `Pan` | `float` | `0.5` | Current Pan normalized value |
| `Tilt` | `float` | `0.5` | Current Tilt normalized value |
| `PTSpeed` | `float` | `5.0` | Pan/Tilt interpolation speed (`FInterpTo` parameter) |
| `PanRot` | `float` | `0.5` | Infinite Pan rotation control value |
| `TiltRot` | `float` | `0.5` | Infinite Tilt rotation control value |
| `InPosZ` | `float` | `0.0` | Lift position value |

---

### 1.4 Channel Switches

Channel switches control the visibility of corresponding control parameters in the Details panel. `bChannelEdit = true` must be enabled first for visibility.

| Switch | Default | Controlled Parameters |
|------|--------|-----------|
| `bChannelEdit` | `false` | Show/hide all channel switches |
| `bPan` | `false` | Pan control parameters |
| `bTilt` | `false` | Tilt control parameters |
| `bPTSpeed` | `false` | PTSpeed control parameters |
| `bPanRot` | `false` | PanRot control parameters |
| `bTiltRot` | `false` | TiltRot control parameters |
| `bLampAngle` | `false` | PanAngle/TiltAngle manual angles |
| `bYPolarRotation` | `false` | InfiniteRotationalSpeed infinite rotation speed |
| `bInPosZ` | `false` | InPosZ lift position |

---

### 1.5 Position Control Functions

#### YPan

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Base|Position",
    meta = (DisplayName = "Pan"))
void YPan(FSuperDMXAttribute DmxPan);
```

Control fixture horizontal rotation (Pan).

**Behavior**:
1. Call `GetSuperDmxAttributeValue()` to read normalized DMX value into `Pan`
2. Compute target angle: `Lerp(PanRange.X, PanRange.Y, Pan)`
3. Use `FInterpTo` to smoothly interpolate `CurrentPan` (speed = `PTSpeed`)
4. Set `SceneRocker` Yaw rotation

**Parameters**:

| Parameter | Type | Description |
|------|------|------|
| `DmxPan` | `FSuperDMXAttribute` | DMX attribute for Pan channel |

---

#### YTilt

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Base|Position",
    meta = (DisplayName = "Tilt"))
void YTilt(FSuperDMXAttribute DmxTilt);
```

Control fixture vertical rotation (Tilt).

**Behavior**: Symmetric with `YPan`, uses `TiltRange` mapping, sets `SceneHard` Roll rotation.

---

#### YPTSpeed

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Base|Position",
    meta = (DisplayName = "PTSpeed"))
void YPTSpeed(FSuperDMXAttribute DmxAttribute);
```

Dynamically adjust Pan/Tilt interpolation speed.

**Behavior**:
1. Read DMX raw value (`GetSuperDmxAttributeValueNoConversion`)
2. If fixture library defines sub-attributes (`SubAttribute`), use physical range mapping
3. Otherwise normalize: `PTSpeed /= 255.0`

**Scope**: All rotation functions using `FInterpTo` (`YPan`, `YTilt`, matrix Pan/Tilt)

---

### 1.6 Matrix Position Control

#### YMatrixPan

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Base|Matrix",
    meta = (DisplayName = "MatrixPan"))
void YMatrixPan(FSuperDMXAttribute DmxPan, TArray<USceneComponent*> MatrixHard);
```

Control Pan rotation of multiple light heads in matrix mode. Each head interpolates independently.

**Parameters**:

| Parameter | Type | Description |
|------|------|------|
| `DmxPan` | `FSuperDMXAttribute` | Pan channel attribute (matrix mode reading) |
| `MatrixHard` | `TArray<USceneComponent*>` | Light head component array (each element corresponds to one pixel) |

**Behavior**:
- Uses `GET_SUPER_DMX_MATRIX_VALUE` macro to iterate matrix data
- Each light head uses `FInterpTo` smooth interpolation to target angle
- Automatically manages `CurrentMatrixPan` internal array size

---

#### YMatrixTilt

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Base|Matrix",
    meta = (DisplayName = "MatrixTilt"))
void YMatrixTilt(FSuperDMXAttribute DmxTilt, TArray<USceneComponent*> MatrixRocker);
```

Control Tilt rotation of multiple light heads in matrix mode. Symmetric with `YMatrixPan`.

---

### 1.7 Infinite Rotation Control

Infinite rotation supports four modes (defined by fixture library `SubAttribute.RotationMode`):

| RotationMode | Behavior |
|-------------|------|
| `Off` | Fall back to normal position control (`YPan`/`YTilt`) |
| `Stop` | Keep current angle frozen |
| `Position` | Start from current angle, offset rotate by physical range |
| `Infinite` | Continuous rotation, speed determined by physical range mapped value |

#### YPanRot

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Base|Infinite",
    meta = (DisplayName = "PanRot"))
void YPanRot(FSuperDMXAttribute DmxInfinity, FSuperDMXAttribute DmxPan);
```

Pan infinite rotation control.

**Parameters**:

| Parameter | Type | Description |
|------|------|------|
| `DmxInfinity` | `FSuperDMXAttribute` | Infinite rotation control channel |
| `DmxPan` | `FSuperDMXAttribute` | Pan position channel (used in `Off` mode) |

**Behavior**:
1. Read `DmxInfinity` raw value
2. Find corresponding sub-attribute's `RotationMode`
3. No sub-attribute or `Off` → call `YPan(DmxPan)` position control
4. `Stop` → do nothing
5. `Position` → offset rotate from entry angle
6. `Infinite` → `SceneRocker->AddRelativeRotation(Speed * InfiniteRotationalSpeed)`

---

#### YTiltRot

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Base|Infinite",
    meta = (DisplayName = "TiltRot"))
void YTiltRot(FSuperDMXAttribute DmxInfinity, FSuperDMXAttribute DmxTilt);
```

Tilt infinite rotation control. Symmetric with `YPanRot`.

---

### 1.8 Matrix Infinite Rotation Control

#### YMatrixPanRot / YMatrixTiltRot

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Base|Infinite",
    meta = (DisplayName = "MatrixPanRot"))
void YMatrixPanRot(FSuperDMXAttribute DmxInfinity, FSuperDMXAttribute DmxPan,
    TArray<USceneComponent*> MatrixRocker);

UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Base|Infinite",
    meta = (DisplayName = "MatrixTiltRot"))
void YMatrixTiltRot(FSuperDMXAttribute DmxInfinity, FSuperDMXAttribute DmxTilt,
    TArray<USceneComponent*> MatrixHard);
```

Infinite rotation control in matrix mode. Each light head independently determines `RotationMode` and executes.

---

### 1.9 Auxiliary Control

#### YLampAngle

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Base|Angle",
    meta = (DisplayName = "LampAngle"))
void YLampAngle();
```

Set `SceneRocker`/`SceneHard` to `PanAngle`/`TiltAngle` fixed angles. Used for manual fixture positioning (non-DMX control).

---

#### SetLiftZ

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Base|Lift",
    meta = (DisplayName = "SetLiftZ"))
void SetLiftZ(USuperLiftComponent* NewSuperLift, FSuperDMXAttribute DmxLiftZ);
```

Control fixture lift.

**Parameters**:

| Parameter | Type | Description |
|------|------|------|
| `NewSuperLift` | `USuperLiftComponent*` | Lift component reference |
| `DmxLiftZ` | `FSuperDMXAttribute` | DMX attribute for lift channel |

---

## 2. ASuperStageLight — Full-function Moving Light

**Header File**: `LightActor/SuperStageLight.h`  
**Base Class**: `ASuperLightBase`  
**Export Macro**: `SUPERSTAGE_API`

Complete moving light implementation class, covering beam, color, gobo, prism, cutting, effect, and matrix control. Suitable for professional moving lights from brands such as Martin, Robe, ClayPaky, etc.

### Inheritance Chain

```
AActor → ASuperBaseActor → ASuperDmxActorBase → ASuperLightBase → ASuperStageLight ← This class
```

---

### 2.1 Component Structure

```
SceneBase
 ├─ SM_Hook (UStaticMeshComponent, hook)
 ├─ SM_Base (UStaticMeshComponent, base)
 └─ SceneRocker (Pan axis)
     ├─ SM_Rocker (UStaticMeshComponent, arm)
     └─ SceneHard (Tilt axis)
         ├─ SM_Hard (UStaticMeshComponent, head)
         ├─ SuperLighting (USuperLightingComponent, main light source)
         ├─ SuperLightingArray[] (USuperLightingComponent[], matrix light sources)
         ├─ SuperEffect (USuperEffectComponent, effect light source)
         ├─ SuperEffectArray[] (USuperEffectComponent[], effect matrix)
         ├─ SpotLighting (USuperSpotComponent, Spot fill light)
         └─ SuperSpotArray[] (USuperSpotComponent[], Spot matrix)
```

> **Note**: `SuperLighting`, `SuperLightingArray`, `SuperEffect`, `SuperEffectArray`, `SpotLighting`, `SuperSpotArray` are all `Transient`, set by calling initialization functions in the Blueprint `SuperDMXTick`.

---

### 2.2 Default Parameters

| Property | Type | Category | Description |
|------|------|------|------|
| `LightDefaultValue` | `FLightDefaultValue` | `B.DefaultParameter` | Light default parameter collection |
| `bHookVisibility` | `bool` | `F.Preview` | Hook model display toggle |
| `bModelVisibility` | `bool` | `F.Preview` | Fixture model display toggle |

---

### 2.3 Control Parameters

All control parameters support `Interp` (Sequencer animation). Parameters with `EditCondition` are only displayed when the corresponding channel switch is enabled.

#### Basic Parameters

| Property | Type | Default | Range | Description |
|------|------|--------|------|------|
| `Dimmer` | `float` | `1.0` | 0-1 | Intensity (0=off, 1=full) |
| `Strobe` | `float` | `1.0` | 0-1 | Strobe speed |
| `Zoom` | `float` | `0.0` | 0-1 | Beam angle/spot size |
| `Focus` | `float` | `0.0` | 0-1 | Focus |
| `Frost` | `float` | `0.0` | 0-1 | Frost effect intensity |
| `Iris` | `float` | `1.0` | 0-1 | Iris size (1=fully open, 0=fully closed) |

#### Color Parameters

| Property | Type | Default | Description |
|------|------|--------|------|
| `Color` | `FLinearColor` | `(1,1,1)` | RGB color |
| `ColorWheel` | `float` | `0` | Color wheel position (0-255) |
| `ColorTemperature` | `float` | `0` | Color temperature control value |
| `Cool` | `float` | `0` | Cool light channel |
| `Warm` | `float` | `0` | Warm light channel |

#### Gobo/Prism Parameters

| Property | Type | Default | Range | Description |
|------|------|--------|------|------|
| `Gobo1` | `float` | `0` | 0-255 | Gobo wheel 1 selection |
| `Gobo2` | `float` | `0` | 0-255 | Gobo wheel 2 selection |
| `Gobo_Rot` | `float` | `0` | 0-255 | Gobo rotation |
| `Prism1` | `float` | `0` | 0-255 | Prism 1 selection |
| `Prism2` | `float` | `0` | 0-255 | Prism 2 selection |
| `Prism_Rot` | `float` | `0` | 0-255 | Prism rotation |

#### Cutting Parameters

| Property | Description |
|------|------|
| `A1`, `B1` | Blade pair 1 (0-1) |
| `A2`, `B2` | Blade pair 2 |
| `A3`, `B3` | Blade pair 3 |
| `A4`, `B4` | Blade pair 4 |
| `ShaperRot` | Cutting system rotation (0-1) |

#### Effect Parameters

| Property | Type | Default | Description |
|------|------|--------|------|
| `EffectDimmer` | `float` | `0` | Effect layer intensity |
| `EffectStrobe` | `float` | `0` | Effect layer strobe |
| `EffectValue` | `float` | `0` | Effect selection (0-255, LUT lookup) |
| `EffectColor` | `FLinearColor` | `(1,1,1)` | Effect layer color |

---

### 2.4 Channel Switches

| Switch | Function |
|------|------|
| `bDimmer` | Intensity channel |
| `bStrobe` | Strobe channel |
| `bZoom` | Zoom channel |
| `bFocus` | Focus channel |
| `bFrost` | Frost channel |
| `bIris` | Iris channel |
| `bColor` | RGB color channels |
| `bColorWheel` | Color wheel channel |
| `bColorTemperature` | Color temperature channel |
| `bCoolWarmMix` | Cool-warm mix channel |
| `bGobo1` / `bGobo2` | Gobo wheel 1/2 channels |
| `bGobo_Rot` | Gobo rotation channel |
| `bPrism1` / `bPrism2` / `bPrism3` | Prism 1/2/3 channels |
| `bPrism_Rot` | Prism rotation channel |
| `bEffect` | Effect channel |
| `bCuttingChannel` | Cutting channel |
| `bPositionZ` | Lift channel |
| `bYRotation` | Rotation channel |

---

### 2.5 Initialization Functions

Call in the **first frame** of the `SuperDMXTick` event to set component defaults.

#### SetLightingDefaultValue

```cpp
UFUNCTION(BlueprintCallable, Category="A.Yunsio|1.Lighting|1.Init",
    meta = (DisplayName = "SetLightingDefaultValue"))
void SetLightingDefaultValue(USuperLightingComponent* NewSuperLighting);
```

Initialize default parameters for a single main light source component (max intensity, distance, volumetric fog, shadows, etc.).

**Parameter**: `NewSuperLighting` — Main light source component reference (drag component reference in Blueprint)

---

#### SetLightingMatrixDefaultValue

```cpp
UFUNCTION(BlueprintCallable, Category="A.Yunsio|1.Lighting|1.Init",
    meta = (DisplayName = "SetLightingMatrixDefaultValue"))
void SetLightingMatrixDefaultValue(TArray<USuperLightingComponent*> NewSuperLightingArray);
```

Initialize matrix mode light source component array.

---

#### SetLightingDefaultTexture / SetLightingDefaultTextureMatrix

```cpp
void SetLightingDefaultTexture(UTexture2D* NewTexture);
void SetLightingDefaultTextureMatrix(UTexture2D* NewTexture);
```

Set default Gobo texture (default pattern when no gobo wheel selection).

---

### 2.6 Intensity Control

#### SetLightingIntensity

```cpp
UFUNCTION(BlueprintCallable, Category="A.Yunsio|1.Lighting|2.Dimmer",
    meta = (DisplayName = "SetLightingIntensity"))
void SetLightingIntensity(FSuperDMXAttribute DmxAttribute);
```

Set main light source intensity via DMX.

**Behavior**:
1. Read DMX normalized value → `Dimmer`
2. Apply intensity curve: `pow(Dimmer, DimmerCurveExponent)`
3. Call `SuperLighting->SetLightingIntensity()`

---

#### SetLightingIntensityMatrix

```cpp
UFUNCTION(BlueprintCallable, Category="A.Yunsio|1.Lighting|2.Dimmer",
    meta = (DisplayName = "SetLightingIntensityMatrix"))
void SetLightingIntensityMatrix(FSuperDMXAttribute DmxAttribute);
```

Matrix mode intensity control, each pixel independent.

---

### 2.7 Strobe Control

#### SetLightingStrobe / SetLightingStrobeMatrix

```cpp
void SetLightingStrobe(FSuperDMXAttribute DmxAttribute);
void SetLightingStrobeMatrix(FSuperDMXAttribute DmxAttribute);
```

Strobe control. Supports strobe modes defined by the sub-attribute system:

| Strobe Mode | Description |
|---------|------|
| Open | Always on (no strobe) |
| Strobe | Standard strobe |
| Pulse | Pulse strobe |
| Random | Random strobe |
| Close | Always off |

---

### 2.8 Color Control

ASuperStageLight provides multiple color control schemes, covering the color systems of all mainstream fixtures.

#### RGB Direct Control

```cpp
// Single light
void SetLightingColorRGB(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB);
// Matrix
void SetLightingColorRGBMatrix(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB);
```

Three-channel independent RGB color control. Each channel `[0, 1]` normalized.

---

#### RGBW Mixing

```cpp
// Single light
void SetLightingColorRGBW(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB, FSuperDMXAttribute DMXAttW);
// Matrix
void SetLightingColorRGBWMatrix(...);
// RGBW + CTO
void SetLightingColorRGBWWithCTO(..., FSuperDMXAttribute DmxCTO,
    float CoolTemperature = 6500.f, float WarmTemperature = 3200.f);
void SetLightingColorRGBWMatrixWithCTO(...);
```

RGBW four-channel color mixing. White channel added to RGB. Optional CTO (Color Temperature Orange) channel adjustment.

---

#### HSV Color

```cpp
// Hue/Saturation/Value
void SetLightingColorHSV(FSuperDMXAttribute DMXAttH, FSuperDMXAttribute DMXAttS,
    FSuperDMXAttribute DMXAttV);
void SetLightingColorHSVMatrix(...);

// Hue/Saturation (value controlled by Dimmer channel)
void SetLightingColorHS(FSuperDMXAttribute DMXAttH, FSuperDMXAttribute DMXAttS);
void SetLightingColorHSMatrix(...);

// HS + CTO
void SetLightingColorHSAndCTO(FSuperDMXAttribute DMXAttH, FSuperDMXAttribute DMXAttS,
    FSuperDMXAttribute DMXAttCTO, float WarmTemperature = 2700.f, float CoolTemperature = 6500.f);
```

HSV color space control. In `SetLightingColorHS` variant, V (value) is independently controlled by the `Dimmer` channel.

---

#### Color Wheel

```cpp
// Single color wheel
void SetLightingColorWheel(FSuperDMXAttribute DmxAttribute, UTexture2D* ColorAtlas = nullptr);

// Color wheel + CMY
void SetLightingColorWheelAndCMY(FSuperDMXAttribute DmxAttribute,
    FSuperDMXAttribute DMXAttC, FSuperDMXAttribute DMXAttM, FSuperDMXAttribute DMXAttY,
    UTexture2D* ColorAtlas = nullptr);

// Dual color wheel
void SetLightingColorWheel2(FSuperDMXAttribute DmxAttribute1, FSuperDMXAttribute DmxAttribute2,
    UTexture2D* ColorAtlas1 = nullptr, UTexture2D* ColorAtlas2 = nullptr);

// Triple color wheel
void SetLightingColorWheel3(...);

// Dual color wheel + CMY
void SetLightingColorWheel2AndCMY(...);

// Dual color wheel + CMY + CTO
void SetLightingColorWheel2AndCMYAndCTO(..., FSuperDMXAttribute DMXAttCTO,
    float WarmTemperature = 2700.f, float CoolTemperature = 6500.f, ...);

// Triple color wheel + CMY
void SetLightingColorWheel3AndCMY(...);
```

Color wheel uses `ColorWheelModel` lookup table defined in fixture library, supporting:
- **Static color**: DMX value in `StaticColorRange` range → select preset color
- **Flow**: DMX value in `FlowRange` range → color auto-cycle
- **White**: DMX value in `WhiteRange` range → white

`ColorAtlas` is an optional color atlas texture that, when passed, synchronously updates the color texture parameter in the beam material.

---

#### Color Temperature Control

```cpp
// Single channel color temperature
void SetLightingColorTemperature(FSuperDMXAttribute DmxAttribute,
    float WarmTemperature = 1700.0f, float CoolTemperature = 12000.0f);

// Cool-Warm dual channel mixing
void SetLightingCoolWarmMix(FSuperDMXAttribute DmxCool, FSuperDMXAttribute DmxWarm,
    float CoolTemperature = 6500.0f, float WarmTemperature = 3200.0f);
```

**Single channel color temperature**: DMX `[0, 1]` → `Lerp(WarmTemperature, CoolTemperature)` → Kelvin color temperature → color

**Cool-Warm mixing**: Two independent channels respectively control cool and warm light intensity, mixed output.

---

#### Dynamic Multi-channel Color Mixing

```cpp
// Arbitrary channel combination mixing
void SetLightingColorMix(const TArray<FSuperColorChannel>& ColorChannels);
void SetLightingColorMixMatrix(const TArray<FSuperColorChannel>& ColorChannels);

// Multi-channel mixing + CTO
void SetLightingColorMixWithCTO(const TArray<FSuperColorChannel>& ColorChannels,
    FSuperDMXAttribute DmxCTO, float CoolTemperature = 6500.f, float WarmTemperature = 3200.f);
void SetLightingColorMixMatrixWithCTO(...);
```

Supports **RGBW/RGBA/RGBL/RGBLAM** and any other color channel combinations.

**Color mixing algorithm**: `FinalColor = Σ(DMX_Value[i] × ChannelColor[i])`, final Clamp to `[0, 1]`.

**Parameter**: `ColorChannels` — `FSuperColorChannel` array, each element contains DMX attribute and corresponding color.

---

### 2.9 Optical Control

#### SetLightingZoom / SetLightingZoomMatrix

```cpp
void SetLightingZoom(FSuperDMXAttribute DmxAttribute);
void SetLightingZoomMatrix(FSuperDMXAttribute DmxAttribute);
```

Control beam angle/spot size. Also triggers beam occlusion distance update.

---

#### SetLightingFrost

```cpp
void SetLightingFrost(FSuperDMXAttribute DmxAttribute);
```

Control frost effect. `[0, 1]`, 0 = no frost, 1 = full frost.

---

#### SetLightingIris

```cpp
void SetLightingIris(FSuperDMXAttribute DmxAttribute);
```

Control iris size. `[0, 1]`, 0 = fully closed (minimum spot), 1 = fully open.

---

#### UpdateBeamOcclusion

```cpp
void UpdateBeamOcclusion();
```

Independently update beam occlusion distance (not dependent on Zoom channel). Internally throttled at **30fps** to avoid per-frame ray detection performance overhead.

---

### 2.10 Gobo Wheel Control

#### SetBeamGobo1 / SetBeamGobo2 / SetBeamGobo3

```cpp
// Single gobo wheel
void SetBeamGobo1(FSuperDMXAttribute DmxGobo1, FSuperDMXAttribute DmxGobo1Rot,
    UTexture2D* Gobo1Atlas = nullptr);

// Dual gobo wheel
void SetBeamGobo2(FSuperDMXAttribute DmxGobo1, FSuperDMXAttribute DmxGobo1Rot,
    FSuperDMXAttribute DmxGobo2, FSuperDMXAttribute DmxGobo2Rot,
    UTexture2D* Gobo1Atlas = nullptr, UTexture2D* Gobo2Atlas = nullptr);

// Triple gobo wheel
void SetBeamGobo3(..., FSuperDMXAttribute DmxGobo3, FSuperDMXAttribute DmxGobo3Rot,
    ..., UTexture2D* Gobo3Atlas = nullptr);
```

**Parameters**:

| Parameter | Description |
|------|------|
| `DmxGoboN` | Gobo wheel N selection channel |
| `DmxGoboNRot` | Gobo wheel N rotation channel |
| `GoboNAtlas` | Gobo atlas texture (optional) |

**Rotation modes**:
- DMX `[0, 127]` → Static angle
- DMX `[128, 255]` → Infinite rotation (speed mapped)

---

#### SetBeamFocus

```cpp
void SetBeamFocus(FSuperDMXAttribute DmxFocus);
```

Focus control. Affects the Focus parameter of the beam material.

---

### 2.11 Prism Control

#### SetBeamPrism

```cpp
void SetBeamPrism(
    FSuperDMXAttribute DmxPrism1,
    FSuperDMXAttribute DmxPrism2,
    FSuperDMXAttribute DmxPrismRot,
    USuperPrismPreset* InPrismPreset = nullptr);
```

Dual prism control (with rotation).

| Parameter | Description |
|------|------|
| `DmxPrism1` | First prism selection channel |
| `DmxPrism2` | Second prism selection channel (pass empty FSuperDMXAttribute if none) |
| `DmxPrismRot` | Prism rotation channel |
| `InPrismPreset` | Prism preset data asset (`USuperPrismPreset*`), optional, fixture library selects layer index from preset via `PrismSelection` |

**Priority**: Prism1 > Prism2, if corresponding `PrismSelection == None`, prism is turned off.

---

### 2.12 Cutting Control

#### SetBeamCutting

```cpp
void SetBeamCutting(
    FSuperDMXAttribute DmxA1, FSuperDMXAttribute DmxB1,
    FSuperDMXAttribute DmxA2, FSuperDMXAttribute DmxB2,
    FSuperDMXAttribute DmxA3, FSuperDMXAttribute DmxB3,
    FSuperDMXAttribute DmxA4, FSuperDMXAttribute DmxB4);
```

Four-blade cutting control (8 channels).

**Blade mapping** (distinguished by blade pair direction, A/B in same pair share same range):
- Blade pair 1 (A1,B1) and blade pair 3 (A3,B3): DMX `[0, 1]` → `[0, 0.4]`
- Blade pair 2 (A2,B2) and blade pair 4 (A4,B4): DMX `[0, 1]` → `[1, 0.6]`

---

#### SetBeamCuttingRotate

```cpp
void SetBeamCuttingRotate(FSuperDMXAttribute DmxShaperRot, FSuperDMXAttribute DmxGoboRot);
```

Cutting system overall rotation.

**Rotation overlay**: `ShaperRot[-45°, 45°] + GoboRot[0°, 360°]`

---

### 2.13 Effect Control

#### Initialization

```cpp
void SetEffectDefaultValue(USuperEffectComponent* NewSuperEffect);
void SetEffectMatrixDefaultValue(TArray<USuperEffectComponent*> NewSuperEffectArray);
```

Initialize effect light source components.

---

#### Effect Intensity / Strobe / Color

```cpp
void SetEffectIntensity(FSuperDMXAttribute DmxAttribute);
void SetEffectStrobe(FSuperDMXAttribute DmxAttribute);
void SetEffectColor(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB);
void SetEffectColorMatrix(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB);
```

Independent intensity, strobe, and color control for the effect layer.

---

#### Effect Selection (LUT)

```cpp
// Effect selection (single channel, effect+speed+width determined by LUT lookup)
void SetEffect(FSuperDMXAttribute DmxAttribute);
void SetEffectMatrix(FSuperDMXAttribute DmxAttribute);

// Effect + speed separate control
void SetEffectWithSpeed(FSuperDMXAttribute DmxEffect, FSuperDMXAttribute DmxSpeed);
void SetEffectMatrixWithSpeed(FSuperDMXAttribute DmxEffect, FSuperDMXAttribute DmxSpeed);
```

**LUT system**: DMX `[0, 255]` → lookup `GetEffectLUT()` → returns `{Effect, Speed, Width}` triplet. Built-in dirty value detection, no redundant setting when value unchanged.

---

### 2.14 Matrix Component Control

Matrix components (`USuperMatrixComponent`) are used for pixel-level control of LED matrix lights.

#### Initialization and Color

```cpp
// Initialize (RGB + Dimmer)
void SetMatrixComponent(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB, FSuperDMXAttribute DmxAttribute,
    USuperMatrixComponent* NewSuperMatrix);

// White control
void SetMatrixWhiteSingle(FSuperDMXAttribute DmxAttribute, int32 Index,
    USuperMatrixComponent* NewSuperMatrix);
void SetMatrixWhiteMultiple(FSuperDMXAttribute DmxAttribute,
    USuperMatrixComponent* NewSuperMatrix);

// Strobe
void SetMatrixStrobe(FSuperDMXAttribute DmxAttribute, USuperMatrixComponent* NewSuperMatrix);

// Intensity
void SetMatrixIntensity(FSuperDMXAttribute DmxAttribute, USuperMatrixComponent* NewSuperMatrix);

// RGB Color
void SetMatrixColorSingle(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB, int32 Index, USuperMatrixComponent* NewSuperMatrix);
void SetMatrixColorMultiple(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB, USuperMatrixComponent* NewSuperMatrix);
```

**Single vs Multiple**:
- `Single`: Control a single pixel using specified `Index`
- `Multiple`: Use matrix reading, automatically iterate through all pixels

---

#### Matrix Extended Color Control

```cpp
// Cool-Warm mix (single/multi point)
void SetMatrixCoolWarmMixSingle(FSuperDMXAttribute DmxCool, FSuperDMXAttribute DmxWarm,
    float CoolTemperature, float WarmTemperature, int32 Index, USuperMatrixComponent* NewSuperMatrix);
void SetMatrixCoolWarmMixMultiple(FSuperDMXAttribute DmxCool, FSuperDMXAttribute DmxWarm,
    float CoolTemperature, float WarmTemperature, USuperMatrixComponent* NewSuperMatrix);

// Multi-channel color mixing
void SetMatrixColorMix(const TArray<FSuperColorChannel>& ColorChannels, USuperMatrixComponent* NewSuperMatrix);

// RGB + CTO (single/multi point)
void SetMatrixColorRGBWithCTOSingle(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB, FSuperDMXAttribute DmxCTO,
    float CoolTemperature, float WarmTemperature, int32 Index, USuperMatrixComponent* NewSuperMatrix);
void SetMatrixColorRGBWithCTOMultiple(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB, FSuperDMXAttribute DmxCTO,
    float CoolTemperature, float WarmTemperature, USuperMatrixComponent* NewSuperMatrix);

// RGBW (single/multi point)
void SetMatrixColorRGBWSingle(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB, FSuperDMXAttribute DMXAttW,
    int32 Index, USuperMatrixComponent* NewSuperMatrix);
void SetMatrixColorRGBWMultiple(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB, FSuperDMXAttribute DMXAttW, USuperMatrixComponent* NewSuperMatrix);

// RGBW + CTO (single/multi point)
void SetMatrixColorRGBWWithCTOSingle(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB, FSuperDMXAttribute DMXAttW, FSuperDMXAttribute DmxCTO,
    float CoolTemperature, float WarmTemperature, int32 Index, USuperMatrixComponent* NewSuperMatrix);
void SetMatrixColorRGBWWithCTOMultiple(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB, FSuperDMXAttribute DMXAttW, FSuperDMXAttribute DmxCTO,
    float CoolTemperature, float WarmTemperature, USuperMatrixComponent* NewSuperMatrix);

// RGB + CoolWarm (single/multi point)
void SetMatrixColorRGBWithCoolWarmSingle(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB, FSuperDMXAttribute DmxCool, FSuperDMXAttribute DmxWarm,
    float CoolTemperature, float WarmTemperature, int32 Index, USuperMatrixComponent* NewSuperMatrix);
void SetMatrixColorRGBWithCoolWarmMultiple(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB, FSuperDMXAttribute DmxCool, FSuperDMXAttribute DmxWarm,
    float CoolTemperature, float WarmTemperature, USuperMatrixComponent* NewSuperMatrix);

// RGBW + CoolWarm (single/multi point)
void SetMatrixColorRGBWWithCoolWarmSingle(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB, FSuperDMXAttribute DMXAttW, FSuperDMXAttribute DmxCool,
    FSuperDMXAttribute DmxWarm, float CoolTemperature, float WarmTemperature,
    int32 Index, USuperMatrixComponent* NewSuperMatrix);
void SetMatrixColorRGBWWithCoolWarmMultiple(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB, FSuperDMXAttribute DMXAttW, FSuperDMXAttribute DmxCool,
    FSuperDMXAttribute DmxWarm, float CoolTemperature, float WarmTemperature,
    USuperMatrixComponent* NewSuperMatrix);
```

---

### 2.15 Spot Fill Light Control

Spot fill light (`USuperSpotComponent`) is a SpotLight component separate from the main light source, used for supplementary lighting effects.

#### Initialization

```cpp
void SetSpotDefaultValue(USuperSpotComponent* NewSuperSpot);
void SetSpotMatrixDefaultValue(TArray<USuperSpotComponent*> NewSuperSpotArray);
```

#### Intensity / Strobe

```cpp
void SetSpotIntensity(FSuperDMXAttribute DmxAttribute);
void SetSpotIntensityMatrix(FSuperDMXAttribute DmxAttribute);
void SetSpotStrobe(FSuperDMXAttribute DmxAttribute);
void SetSpotStrobeMatrix(FSuperDMXAttribute DmxAttribute);
```

#### Color

```cpp
// RGB
void SetSpotColorRGB(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB);
void SetSpotColorRGBMatrix(...);

// RGBW
void SetSpotColorRGBW(..., FSuperDMXAttribute DMXAttW);
void SetSpotColorRGBWMatrix(...);

// Cool-Warm mix
void SetSpotCoolWarmMix(FSuperDMXAttribute DmxCool, FSuperDMXAttribute DmxWarm,
    float CoolTemperature = 6500.f, float WarmTemperature = 3200.f);
void SetSpotCoolWarmMixMatrix(...);

// RGBW + Cool-Warm mix
void SetSpotColorRGBWWithCoolWarmMix(...);
void SetSpotColorRGBWWithCoolWarmMixMatrix(...);

// Color temperature matrix
void SetSpotColorTemperatureMatrix(FSuperDMXAttribute DmxAttribute,
    float WarmTemperature = 1700.0f, float CoolTemperature = 12000.0f);
```

#### Optics

```cpp
void SetSpotZoom(FSuperDMXAttribute DmxAttribute);
void SetSpotZoomMatrix(FSuperDMXAttribute DmxAttribute);
void SetSpotFrost(FSuperDMXAttribute DmxAttribute);
void SetSpotFrostMatrix(FSuperDMXAttribute DmxAttribute);
```

---

## 3. FSuperColorChannel — Dynamic Color Mixing Channel

**Header File**: `LightActor/SuperStageLight.h`

```cpp
USTRUCT(BlueprintType)
struct FSuperColorChannel
```

Used for color channel definition in `SetLightingColorMix` series functions.

| Property | Type | Default | Description |
|------|------|--------|------|
| `DmxAttribute` | `FSuperDMXAttribute` | — | DMX attribute (channel value 0-1) |
| `ChannelColor` | `FLinearColor` | `White` | Color corresponding to this channel |

**Usage example** (Blueprint pseudocode):

```
ColorChannels = [
    { DmxAttribute = Red_channel,    ChannelColor = (1,0,0) },  // Red
    { DmxAttribute = Green_channel,  ChannelColor = (0,1,0) },  // Green
    { DmxAttribute = Blue_channel,   ChannelColor = (0,0,1) },  // Blue
    { DmxAttribute = White_channel,  ChannelColor = (1,1,1) },  // White
    { DmxAttribute = Amber_channel,  ChannelColor = (1,0.75,0) }, // Amber
    { DmxAttribute = Lime_channel,   ChannelColor = (0.5,1,0) }, // Lime
]
SetLightingColorMix(ColorChannels)
```

---

## 4. FLightDefaultValue — Light Default Parameters

**Header File**: `LightActor/SuperLightTypes.h`

```cpp
USTRUCT(BlueprintType)
struct FLightDefaultValue
```

### Top-level Properties

| Property | Type | Default | Description |
|------|------|--------|------|
| `MaxLightIntensity` | `float` | `100.0` | Maximum intensity (%), ≥1 |
| `LensIntensity` | `float` | `1.0` | Lens intensity multiplier |
| `MaxLightDistance` | `float` | `2345.0` | Maximum light distance (cm), ≥100 |
| `LightSpotDefaultValue` | `FLightSpotDefaultValue` | — | Spot parameters |
| `BeamDefaultValue` | `FBeamDefaultValue` | — | Beam parameters |

### FLightSpotDefaultValue

| Property | Type | Default | Range | Description |
|------|------|--------|------|------|
| `LightSpotIntensity` | `float` | `100.0` | ≥0 | Spot intensity (%) |
| `VolumetricScattering` | `float` | `0.0` | 0-100 | Volumetric fog intensity (%) |
| `bLightShadow` | `bool` | `false` | — | Shadow toggle |
| `bLightingChannel0/1/2` | `bool` | `true/false/false` | — | Lighting channels |
| `bAffectTransmission` | `bool` | `true` | — | Affect transmission |
| `SpecularScale` | `float` | `1.0` | 0-1 | Specular scale |
| `SourceRadius` | `float` | `0.0` | ≥0 | Soft shadow radius |

### FBeamDefaultValue

| Property | Type | Default | Range | Description |
|------|------|--------|------|------|
| `BeamIntensity` | `float` | `1.0` | ≥0 | Beam intensity |
| `AtmosphericDensity` | `float` | `0.03` | 0-1 | Atmospheric attenuation density |
| `BeamFogIntensity` | `float` | `20.0` | 0-100 | Beam fog intensity (%) |
| `AtmosBeamFogSpeed` | `float` | `10.0` | 0-100 | Beam fog flow speed (%) |
| `LensRadius` | `float` | `10.0` | 1-100 | Lens size (%) |
| `BeamQuality` | `float` | `75.0` | 0-100 | Beam render quality (%) |
| `bBeamBlock` | `bool` | `false` | — | Beam occlusion toggle |

---

## 5. Pan/Tilt Motion System Details

### Motion Flow

```
DMX data → GetSuperDmxAttributeValue → Normalized [0,1]
    ↓
Lerp(Range.X, Range.Y, DMX value) → Target angle (degrees)
    ↓
FInterpTo(Current, Target, DeltaTime, PTSpeed) → Smooth angle
    ↓
SetRelativeRotation → Update component rotation
```

### Interpolation Behavior

- **PTSpeed = 0**: Extremely slow response (almost no movement)
- **PTSpeed = 5**: Default speed, reaches target in about 0.5-1 seconds
- **PTSpeed = 10+**: Fast response, near instantaneous

### Infinite Rotation Flow

```
DMX data → GetSuperDmxAttributeValueNoConversion → Raw value 0-255
    ↓
FindSubAttribute(RawValue) → RotationMode determination
    ↓
├─ Off       → Fallback to YPan/YTilt position control
├─ Stop      → Keep current angle
├─ Position  → Offset rotate from start angle (using FInterpTo)
└─ Infinite  → GetMappedPhysical → Speed × InfiniteRotationalSpeed
                ↓
            AddRelativeRotation → Accumulate rotation per frame
```

### Matrix vs Single Light Mode

| Feature | Single Light Mode | Matrix Mode |
|------|---------|----------|
| Function | `YPan`/`YTilt` | `YMatrixPan`/`YMatrixTilt` |
| Target component | `SceneRocker`/`SceneHard` | Passed `MatrixHard[]`/`MatrixRocker[]` |
| Interpolation state | `CurrentPan`/`CurrentTilt` | `CurrentMatrixPan[]`/`CurrentMatrixTilt[]` |
| DMX reading | Single value | `GET_SUPER_DMX_MATRIX_VALUE` batch read |

---

## 6. Color System Details

### Color Control Selection Guide

| Fixture Type | Recommended Function |
|----------|---------|
| Pure RGB LED | `SetLightingColorRGB` |
| RGBW LED | `SetLightingColorRGBW` |
| RGBW + CTO | `SetLightingColorRGBWWithCTO` |
| RGBALWM etc. multi-color | `SetLightingColorMix` |
| Traditional light with color wheel | `SetLightingColorWheel` |
| Color wheel + CMY | `SetLightingColorWheelAndCMY` |
| Dual color wheel + CMY | `SetLightingColorWheel2AndCMY` |
| Dual color wheel + CMY + CTO | `SetLightingColorWheel2AndCMYAndCTO` |
| Triple color wheel + CMY | `SetLightingColorWheel3AndCMY` |
| Pure color temperature light | `SetLightingColorTemperature` |
| Cool-Warm dual color temperature | `SetLightingCoolWarmMix` |
| HSV control | `SetLightingColorHSV` |
| HS (intensity follows Dimmer) | `SetLightingColorHS` |

### CMY Mixing Principle

CMY (Cyan/Magenta/Yellow) is a **subtractive color mixing** system, physically realized through filter transmission:

```
FinalColor = BaseColor × (1 - Cyan) × (1 - Magenta) × (1 - Yellow)
```

- `C = 1, M = 0, Y = 0` → Red absorbed → Cyan
- `C = 0, M = 1, Y = 0` → Green absorbed → Magenta
- `C = 1, M = 1, Y = 0` → Red+Green absorbed → Blue

### CTO (Color Temperature Orange)

CTO channel adjusts overall color temperature:

```
CTO = 0 → No filter (CoolTemperature, default 6500K or higher)
CTO = 1 → Full orange filter (WarmTemperature, default 2700K-3200K)
```

---

## 7. Usage Examples

### 7.1 Typical Blueprint Usage — Complete Moving Light Control

Connect the following nodes in the `SuperDMXTick` event:

```
Event SuperDMXTick
 │
 ├─ SetLightingDefaultValue(SuperLighting)          // First frame initialization
 │
 ├─ YPan(DmxPan)                                     // Pan control
 ├─ YTilt(DmxTilt)                                   // Tilt control
 ├─ YPTSpeed(DmxPTSpeed)                             // PT speed
 │
 ├─ SetLightingIntensity(DmxDimmer)                  // Intensity
 ├─ SetLightingStrobe(DmxStrobe)                     // Strobe
 │
 ├─ SetLightingColorRGB(DmxR, DmxG, DmxB)           // RGB color
 ├─ SetLightingZoom(DmxZoom)                         // Zoom
 ├─ SetLightingFrost(DmxFrost)                       // Frost
 │
 ├─ SetBeamGobo1(DmxGobo1, DmxGobo1Rot)             // Gobo wheel
 ├─ SetBeamPrism(DmxPrism1, DmxPrism2, DmxPrismRot, PrismPreset) // Prism
 │
 └─ SetBeamCutting(A1,B1, A2,B2, A3,B3, A4,B4)      // Cutting
```

### 7.2 Blueprint — Matrix LED Light

```
Event SuperDMXTick
 │
 ├─ SetLightingMatrixDefaultValue(LightingArray)
 ├─ SetLightingIntensityMatrix(DmxDimmer)
 ├─ SetLightingColorRGBMatrix(DmxR, DmxG, DmxB)
 ├─ SetLightingZoomMatrix(DmxZoom)
 └─ YMatrixTilt(DmxTilt, MatrixHardArray)
```

### 7.3 Blueprint — RGBW + CTO Fixture

```
Event SuperDMXTick
 │
 ├─ SetLightingDefaultValue(SuperLighting)
 ├─ SetLightingIntensity(DmxDimmer)
 └─ SetLightingColorRGBWWithCTO(DmxR, DmxG, DmxB, DmxW, DmxCTO,
                                 CoolTemp=6500, WarmTemp=3200)
```

### 7.4 Blueprint — Multi-color LED (RGBALWM)

```
// Build ColorChannels array
ColorChannels = MakeArray(
    FSuperColorChannel(DmxR,     Red),
    FSuperColorChannel(DmxG,     Green),
    FSuperColorChannel(DmxB,     Blue),
    FSuperColorChannel(DmxAmber, (1, 0.75, 0)),
    FSuperColorChannel(DmxLime,  (0.5, 1, 0)),
    FSuperColorChannel(DmxWhite, White),
    FSuperColorChannel(DmxMint,  (0.6, 1, 0.8))
)

Event SuperDMXTick
 │
 ├─ SetLightingDefaultValue(SuperLighting)
 ├─ SetLightingIntensity(DmxDimmer)
 └─ SetLightingColorMix(ColorChannels)
```

### 7.5 C++ Plugin — Creating Custom Fixtures

```cpp
#include "LightActor/SuperStageLight.h"

// In your custom Actor or Controller:
void AMyLightController::UpdateLight(ASuperStageLight* Light, float DeltaTime)
{
    if (!Light) return;

    // Can directly set control parameters (without DMX)
    Light->Dimmer = 0.8f;
    Light->Color = FLinearColor(1.0f, 0.5f, 0.2f);
    Light->Zoom = 0.3f;

    // Or read via DMX attributes
    FSuperDMXAttribute DimmerAttr;
    DimmerAttr.InstanceIndex = 0;
    DimmerAttr.AttribName = FName("Dimmer");
    DimmerAttr.DMXChannelType = EDMXChannelType::Conarse;
    Light->SetLightingIntensity(DimmerAttr);
}
```

---

## 8. API Quick Reference

### ASuperLightBase — Position Control

| Function | Category | Description |
|------|------|------|
| `YPan(DmxPan)` | Position | Pan horizontal rotation |
| `YTilt(DmxTilt)` | Position | Tilt vertical rotation |
| `YPTSpeed(DmxAttr)` | Position | PT speed |
| `YMatrixPan(DmxPan, Array)` | Matrix | Matrix Pan |
| `YMatrixTilt(DmxTilt, Array)` | Matrix | Matrix Tilt |
| `YPanRot(DmxInf, DmxPan)` | Infinite | Pan infinite rotation |
| `YTiltRot(DmxInf, DmxTilt)` | Infinite | Tilt infinite rotation |
| `YMatrixPanRot(DmxInf, DmxPan, Array)` | Infinite | Matrix Pan infinite |
| `YMatrixTiltRot(DmxInf, DmxTilt, Array)` | Infinite | Matrix Tilt infinite |
| `YLampAngle()` | Angle | Manual angle |
| `SetLiftZ(Lift, DmxZ)` | Lift | Lift control |

### ASuperStageLight — Initialization

| Function | Description |
|------|------|
| `SetLightingDefaultValue(Comp)` | Main light source initialization |
| `SetLightingMatrixDefaultValue(Array)` | Matrix light source initialization |
| `SetLightingDefaultTexture(Tex)` | Default Gobo |
| `SetEffectDefaultValue(Comp)` | Effect initialization |
| `SetEffectMatrixDefaultValue(Array)` | Effect matrix initialization |
| `SetSpotDefaultValue(Comp)` | Spot fill light initialization |
| `SetSpotMatrixDefaultValue(Array)` | Spot matrix initialization |

### ASuperStageLight — Intensity / Strobe

| Function | Single Light | Matrix |
|------|------|------|
| Intensity | `SetLightingIntensity` | `SetLightingIntensityMatrix` |
| Strobe | `SetLightingStrobe` | `SetLightingStrobeMatrix` |
| Effect Intensity | `SetEffectIntensity` | — |
| Effect Strobe | `SetEffectStrobe` | — |
| Spot Intensity | `SetSpotIntensity` | `SetSpotIntensityMatrix` |
| Spot Strobe | `SetSpotStrobe` | `SetSpotStrobeMatrix` |

### ASuperStageLight — Color

| Color Mode | Single Light Function | Matrix Function |
|---------|---------|---------|
| RGB | `SetLightingColorRGB` | `SetLightingColorRGBMatrix` |
| RGBW | `SetLightingColorRGBW` | `SetLightingColorRGBWMatrix` |
| RGBW+CTO | `SetLightingColorRGBWWithCTO` | `SetLightingColorRGBWMatrixWithCTO` |
| HSV | `SetLightingColorHSV` | `SetLightingColorHSVMatrix` |
| HS | `SetLightingColorHS` | `SetLightingColorHSMatrix` |
| HS+CTO | `SetLightingColorHSAndCTO` | — |
| Color Wheel | `SetLightingColorWheel` | — |
| Color Wheel+CMY | `SetLightingColorWheelAndCMY` | — |
| Color Temperature | `SetLightingColorTemperature` | — |
| Cool-Warm Mix | `SetLightingCoolWarmMix` | — |
| Multi-channel Mix | `SetLightingColorMix` | `SetLightingColorMixMatrix` |
| Multi-channel+CTO | `SetLightingColorMixWithCTO` | `SetLightingColorMixMatrixWithCTO` |

### ASuperStageLight — Optics / Gobo / Prism / Cutting

| Function | Description |
|------|------|
| `SetLightingZoom` / `Matrix` | Zoom |
| `SetLightingFrost` | Frost |
| `SetLightingIris` | Iris |
| `UpdateBeamOcclusion` | Beam occlusion (30fps throttled) |
| `SetBeamGobo1/2/3` | Gobo wheel (1-3 wheels) |
| `SetBeamFocus` | Focus |
| `SetBeamPrism` | Prism |
| `SetBeamCutting` | Four-blade cutting |
| `SetBeamCuttingRotate` | Cutting rotation |

### ASuperStageLight — Effect / Matrix

| Function | Description |
|------|------|
| `SetEffect` / `SetEffectMatrix` | Effect selection (LUT) |
| `SetEffectWithSpeed` / `Matrix` | Effect + independent speed |
| `SetEffectColor` / `Matrix` | Effect color |
| `SetMatrixComponent` | Matrix init + color + intensity |
| `SetMatrixWhiteSingle/Multiple` | Matrix white |
| `SetMatrixColorSingle/Multiple` | Matrix RGB |
| `SetMatrixCoolWarmMixSingle/Multiple` | Matrix cool-warm mix |
| `SetMatrixColorRGBWSingle/Multiple` | Matrix RGBW |
| `SetMatrixColorRGBWithCTOSingle/Multiple` | Matrix RGB + CTO |
| `SetMatrixColorRGBWWithCTOSingle/Multiple` | Matrix RGBW + CTO |
| `SetMatrixColorRGBWithCoolWarmSingle/Multiple` | Matrix RGB + CoolWarm |
| `SetMatrixColorRGBWWithCoolWarmSingle/Multiple` | Matrix RGBW + CoolWarm |
| `SetMatrixStrobe` | Matrix strobe |
| `SetMatrixIntensity` | Matrix intensity |
| `SetMatrixColorMix` | Matrix multi-channel color mixing |
