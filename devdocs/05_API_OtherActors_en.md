# Other Actor Classes API Documentation

> **Document Version**: 1.0  
> **Applicable Version**: SuperStage Plugin 2025/2026  
> **Target Audience**: Blueprint Developers / C++ Plugin Integration Developers (without source code modification permissions)  
> **Last Updated**: 2026-03

---

## Table of Contents

1. [ASuperDMXCamera — DMX Camera](#1-asuperdmxcamera--dmx-camera)
2. [ASuperLiftingMachinery — Lifting Machinery](#2-asuperliftingmachinery--lifting-machinery)
3. [ASuperRailMachinery — Rail Machinery](#3-asuperrailmachinery--rail-machinery)
4. [ASuperStageVFXActor — Stage VFX](#4-asuperstagevfxactor--stage-vfx)
5. [ASuperLightStripEffect — LED Strip Effect](#5-asuperlightstripeffect--led-strip-effect)
6. [ASuperLiftMatrix — Lift Matrix](#6-asuperliftmatrix--lift-matrix)
7. [ASuperLaserActor — Laser Display (Texture Mode)](#7-asuperlaseractor--laser-display-texture-mode)
8. [ASuperLaserProActor — Laser Display (Point Data Mode)](#8-asuperlaserproactor--laser-display-point-data-mode)
9. [ASuperNDIScreen — NDI Video Screen](#9-asuperndiscreen--ndi-video-screen)
10. [ASuperProjector — Projector / Mapping](#10-asuperprojector--projector--mapping)
11. [API Quick Reference](#11-api-quick-reference)

---

## 1. ASuperDMXCamera — DMX Camera

**Header File**: `LightActor/SuperDMXCamera.h`  
**Base Class**: `ASuperDmxActorBase`  
**Export Macro**: `SUPERSTAGE_API`

Controls 6-axis motion (3-axis displacement + 3-axis rotation) and camera parameters (FOV/Aperture/Focus Distance) of a cine camera via DMX. Supports rendering to RenderTarget for real-time content output to LED screens.

### Inheritance Chain

```
AActor → ASuperBaseActor → ASuperDmxActorBase → ASuperDMXCamera
```

### Components

| Component | Type | Description |
|------|------|------|
| `CineCamera` | `UCineCameraComponent*` | Cine camera component |
| `SceneCapture` | `USceneCaptureComponent2D*` | Scene capture component (renders to RenderTarget) |

### Default Parameters

| Property | Type | Default | Description |
|------|------|--------|------|
| `Start` | `bool` | `false` | Start toggle |
| `MovingRange` | `FVector` | `(0,0,0)` | Movement range (cm) |
| `InitialPosition` | `FVector` | — | Initial position (computed by `BootRefresh`, read-only) |
| `EndPosition` | `FVector` | — | End position (computed by `BootRefresh`, read-only) |
| `RotRange` | `FRotator` | `(180,360,90)` | Rotation range (degrees) |
| `InitialRotation` | `FRotator` | — | Initial rotation (read-only) |
| `EndRotation` | `FRotator` | — | End rotation (read-only) |
| `FOVRange` | `FVector2D` | `(15, 120)` | FOV range (degrees) |
| `ApertureRange` | `FVector2D` | `(1.2, 22)` | Aperture range (f-stop) |
| `FocusDistanceRange` | `FVector2D` | `(50, 100000)` | Focus distance range (cm) |
| `CameraParamSpeed` | `float` | `3.0` | Camera parameter interpolation speed (0-10) |
| `RenderTarget` | `UTextureRenderTarget2D*` | `nullptr` | Render output target |
| `RenderResolution` | `FIntPoint` | `(1920,1080)` | Render resolution |
| `bEnableSceneCapture` | `bool` | `false` | Enable scene capture |

### Control Parameters

All control parameters are `[0, 1]` normalized values, supporting `Interp` (Sequencer animation).

| Property | Default | Description |
|------|--------|------|
| `PosX` / `PosY` / `PosZ` | `0.5` | XYZ position control |
| `RotX` / `RotY` / `RotZ` | `0.5` | XYZ rotation control |
| `FOV` | `0.5` | Field of view |
| `Aperture` | `0.5` | Aperture |
| `FocusDistance` | `0.5` | Focus distance |

### Cached Information (Read-only)

| Property | Description |
|------|------|
| `CurrentPosX/Y/Z` | Current interpolated position |
| `CurrentRotX/Y/Z` | Current interpolated rotation |
| `CurrentFOV` | Current FOV (degrees) |
| `CurrentAperture` | Current aperture (f-stop) |
| `CurrentFocusDistance` | Current focus distance (cm) |

### Functions

#### BootRefresh

```cpp
UFUNCTION(CallInEditor, Category = "B.DefaultParameter",
    meta = (DisplayName = "BootRefresh"))
void BootRefresh();
```

**Editor button**. Records current position/rotation as baseline, computes `InitialPosition`/`EndPosition` and `InitialRotation`/`EndRotation`.

**Calculation formula**:
```
InitialPosition = CurrentWorldPosition - MovingRange * 0.5
EndPosition     = CurrentWorldPosition + MovingRange * 0.5
InitialRotation = CurrentRotation - RotRange * 0.5
EndRotation     = CurrentRotation + RotRange * 0.5
```

> **Important**: Must be called after placing the Actor in the correct position.

---

#### DMXCameraControl

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Camera|Transform",
    meta = (DisplayName = "DMXCameraControl"))
void DMXCameraControl(
    FSuperDMXAttribute DmxXPos, FSuperDMXAttribute DmxYPos, FSuperDMXAttribute DmxZPos,
    FSuperDMXAttribute DmxXRot, FSuperDMXAttribute DmxYRot, FSuperDMXAttribute DmxZRot);
```

6-axis motion control (3-axis displacement + 3-axis rotation). Call in `SuperDMXTick`.

**DMX mapping**:
- Position: `DMX[0,1] → Lerp(InitialPosition, EndPosition)`
- Rotation: `DMX[0,1] → Lerp(InitialRotation, EndRotation)`

All axes use `FInterpTo` smooth interpolation.

---

#### DMXCameraParams

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Camera|Params",
    meta = (DisplayName = "DMXCameraParams"))
void DMXCameraParams(
    FSuperDMXAttribute DmxFOV,
    FSuperDMXAttribute DmxAperture,
    FSuperDMXAttribute DmxFocusDistance);
```

Camera parameter control.

**DMX mapping**:
- FOV: `DMX[0,1] → Lerp(FOVRange.X, FOVRange.Y)`
- Aperture: `DMX[0,1] → Lerp(ApertureRange.X, ApertureRange.Y)`
- Focus: `DMX[0,1] → Lerp(FocusDistanceRange.X, FocusDistanceRange.Y)`

---

#### GetCineCamera / GetRenderTarget

```cpp
UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCineCamera"))
UCineCameraComponent* GetCineCamera() const;

UFUNCTION(BlueprintPure, meta = (DisplayName = "GetRenderTarget"))
UTextureRenderTarget2D* GetRenderTarget() const;
```

Pure functions to get the camera component and render target reference.

---

### Usage Example

```
1. Place ASuperDMXCamera in the scene
2. Set MovingRange (e.g., (500, 500, 300)) and RotRange
3. Click BootRefresh to initialize ranges
4. Set Start = true
5. In SuperDMXTick, connect:
   DMXCameraControl(DmxX, DmxY, DmxZ, DmxRotX, DmxRotY, DmxRotZ)
   DMXCameraParams(DmxFOV, DmxAperture, DmxFocus)
6. (Optional) Enable bEnableSceneCapture and set RenderTarget for LED screen output
```

---

## 2. ASuperLiftingMachinery — Lifting Machinery

**Header File**: `LightActor/SuperLiftingMachinery.h`  
**Base Class**: `ASuperDmxActorBase`  
**Export Macro**: `SUPERSTAGE_API`

Controls 6-axis motion (3-axis displacement + 3-axis rotation) of stage devices via DMX, supporting both absolute rotation and infinite rotation modes. Suitable for lift platforms, rotating stages, robotic arms, and similar devices.

### Inheritance Chain

```
AActor → ASuperBaseActor → ASuperDmxActorBase → ASuperLiftingMachinery
```

### Default Parameters

| Property | Type | Default | Description |
|------|------|--------|------|
| `Start` | `bool` | `false` | Start toggle |
| `MovingRange` | `FVector` | `(0,0,0)` | Movement range (cm) |
| `InitialPosition` | `FVector` | — | Initial position (read-only) |
| `EndPosition` | `FVector` | — | End position (read-only) |
| `RotRange` | `FRotator` | `(360,360,360)` | Rotation range (degrees, absolute mode) |
| `InitialRotation` | `FRotator` | — | Initial rotation (read-only, absolute mode) |
| `EndRotation` | `FRotator` | — | End rotation (read-only, absolute mode) |
| `PosSpeed` | `float` | `1.0` | Movement interpolation speed (0-10) |
| `RotSpeed` | `float` | `1.0` | Rotation interpolation speed (0-10, absolute mode) |
| `PolarRotationSpeed` | `float` | `1.0` | Infinite rotation speed (0-10, infinite mode) |
| `PolarRotation` | `bool` | `false` | Infinite rotation mode toggle |

### Control Parameters

| Property | Default | Description |
|------|--------|------|
| `PosX` / `PosY` / `PosZ` | `0.5` | XYZ position control `[0, 1]` |
| `RotX` / `RotY` / `RotZ` | `0.5` | XYZ rotation control `[0, 1]` |

### Cached Information (Read-only)

`CurrentPosX/Y/Z`, `CurrentRotX/Y/Z` — Current interpolated actual values.

### Functions

#### BootRefresh

```cpp
UFUNCTION(CallInEditor, meta = (DisplayName = "BootRefresh"))
void BootRefresh();
```

Editor button. Records current pose as baseline, computes motion range.

---

#### LiftingMachinery

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Machinery|Transform",
    meta = (DisplayName = "LiftingMachinery"))
void LiftingMachinery(
    FSuperDMXAttribute DmxXPos, FSuperDMXAttribute DmxYPos, FSuperDMXAttribute DmxZPos,
    FSuperDMXAttribute DmxXRot, FSuperDMXAttribute DmxYRot, FSuperDMXAttribute DmxZRot);
```

6-axis DMX control function. Call in `SuperDMXTick`.

**Behavior**:
- **Displacement**: `DMX[0,1] → Lerp(InitialPosition, EndPosition)` + `FInterpTo(PosSpeed)`
- **Rotation (absolute)**: `DMX[0,1] → Lerp(InitialRotation, EndRotation)` + `FInterpTo(RotSpeed)`
- **Rotation (infinite)**: `DMX[0,1] → Speed[-PolarRotationSpeed, +PolarRotationSpeed]` → accumulate per frame

---

### Usage Example

```
1. Place ASuperLiftingMachinery in the scene
2. Set MovingRange (e.g., (0, 0, 500) for lift only)
3. Click BootRefresh
4. Start = true
5. In SuperDMXTick:
   LiftingMachinery(DmxX, DmxY, DmxZ, DmxRotX, DmxRotY, DmxRotZ)
```

**Pure lift scenario**: `MovingRange = (0, 0, 500)`, `RotRange = (0, 0, 0)`, only need to pass `DmxZPos`.

---

## 3. ASuperRailMachinery — Rail Machinery

**Header File**: `LightActor/SuperRailMachinery.h`  
**Base Class**: `ASuperDmxActorBase`  
**Export Macro**: `SUPERSTAGE_API`

Compound control moving along a `USplineComponent` spline curve path + 6-axis local offset/rotation. A total of **7 DMX control axes**: rail position (RailPos) + XYZ offset + XYZ rotation.

### Inheritance Chain

```
AActor → ASuperBaseActor → ASuperDmxActorBase → ASuperRailMachinery
```

### Component Structure

```
SceneBase
 ├─ RailSplineComponent (USplineComponent, rail path)
 └─ RailMountPoint (USceneComponent, moves along spline)
     └─ OffsetComponent (USceneComponent, 6-axis offset/rotation)
         └─ Child Actor/Component mounting point
```

| Component | Type | Description |
|------|------|------|
| `RailSplineComponent` | `USplineComponent*` | Spline curve (control points editable in editor) |
| `RailMountPoint` | `USceneComponent*` | Rail mounting point (moves along Spline) |
| `OffsetComponent` | `USceneComponent*` | Offset component (6-axis local transform) |

> **Attachment method**: Child Actors should be attached to `OffsetComponent` returned by `GetDefaultAttachComponent()`.

### Default Parameters

| Property | Type | Default | Description |
|------|------|--------|------|
| `Start` | `bool` | `false` | Start toggle |
| `bLockOrientationToRail` | `bool` | `false` | Lock orientation along rail tangent direction |
| `bClosedLoop` | `bool` | `false` | Closed loop rail (end-to-end cyclic) |
| `OffsetRange` | `FVector` | `(0,0,0)` | Offset range (cm) |
| `InitialOffset` / `EndOffset` | `FVector` | — | Offset range (computed by `BootRefresh`, read-only) |
| `RotRange` | `FRotator` | `(360,360,360)` | Rotation range (absolute mode) |
| `InitialRotation` / `EndRotation` | `FRotator` | — | Rotation range (read-only) |
| `RailSpeed` | `float` | `1.0` | Rail movement interpolation speed (0-10) |
| `OffsetSpeed` | `float` | `1.0` | Offset movement interpolation speed (0-10) |
| `RotSpeed` | `float` | `1.0` | Rotation interpolation speed (absolute mode, 0-10) |
| `PolarRotationSpeed` | `float` | `1.0` | Infinite rotation speed (0-10) |
| `PolarRotation` | `bool` | `false` | Infinite rotation mode toggle |

### Control Parameters

| Property | Default | Description |
|------|--------|------|
| `RailPos` | `0.0` | Rail position `[0,1]` (0=start, 1=end) |
| `PosX` / `PosY` / `PosZ` | `0.5` | XYZ offset control `[0, 1]` |
| `RotX` / `RotY` / `RotZ` | `0.5` | XYZ rotation control `[0, 1]` |

### Functions

#### BootRefresh

```cpp
UFUNCTION(CallInEditor, meta = (DisplayName = "BootRefresh"))
void BootRefresh();
```

Editor button. Records current offset/rotation as baseline.

---

#### RailMachinery

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Machinery|Rail",
    meta = (DisplayName = "RailMachinery"))
void RailMachinery(
    FSuperDMXAttribute DmxRailPos,
    FSuperDMXAttribute DmxXPos, FSuperDMXAttribute DmxYPos, FSuperDMXAttribute DmxZPos,
    FSuperDMXAttribute DmxXRot, FSuperDMXAttribute DmxYRot, FSuperDMXAttribute DmxZRot);
```

7-axis DMX control function. Call in `SuperDMXTick`.

**DMX mapping**:
- **Rail position**: `DMX[0,1] → SplineLength × t → position along spline`
- **Offset**: `DMX[0,1] → Lerp(InitialOffset, EndOffset)`
- **Rotation**: Same absolute/infinite mode as `ASuperLiftingMachinery`

**Closed loop interpolation**: When `bClosedLoop` is enabled, uses shortest arc path interpolation (`[-0.5, 0.5]` range) to avoid going the long way around.

---

#### RailPositionOnly

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|Machinery|Rail",
    meta = (DisplayName = "RailPositionOnly"))
void RailPositionOnly(FSuperDMXAttribute DmxRailPos);
```

Controls only rail position, without offset/rotation. Suitable for simple scenarios requiring only rail movement.

---

### Usage Example

```
1. Place ASuperRailMachinery in the scene
2. Edit RailSplineComponent control points to define the rail path
3. Set OffsetRange, RotRange
4. Click BootRefresh
5. Start = true
6. In SuperDMXTick:
   RailMachinery(DmxRail, DmxX, DmxY, DmxZ, DmxRotX, DmxRotY, DmxRotZ)
7. Attach child Actors such as fixtures/props to OffsetComponent
```

---

## 4. ASuperStageVFXActor — Stage VFX

**Header File**: `LightActor/SuperStageVFXActor.h`  
**Base Class**: `ASuperLightBase`  
**Export Macro**: `SUPERSTAGE_API`

Controls Niagara particle systems via DMX (smoke/fire/snow/CO2/confetti/fireworks, etc.). Inherits from `ASuperLightBase`, thus supports Pan/Tilt rotation control.

### Inheritance Chain

```
AActor → ASuperBaseActor → ASuperDmxActorBase → ASuperLightBase → ASuperStageVFXActor
```

### Components

| Component | Type | Description |
|------|------|------|
| `Niagara` | `UNiagaraComponent*` | Niagara particle emitter |

### Default Parameters

| Property | Type | Default | Range | Description |
|------|------|--------|------|------|
| `MaxSpawnCount` | `float` | `1.0` | 0-100 | Maximum particle spawn count |
| `MaxSpeed` | `float` | `100` | 0-100 | Maximum speed |
| `LoopTime` | `float` | `100` | 0-100 | Loop time (seconds) |
| `Minimum` | `float` | `100` | 0-100000 | Minimum range (cm) |
| `Maximum` | `float` | `100` | 0-100000 | Maximum range (cm) |

### Control Parameters

| Property | Type | Default | Description |
|------|------|--------|------|
| `SpawnCount` | `float` | `0.0` | Particle spawn count control `[0, 1]` |
| `Color` | `FLinearColor` | `White` | Particle color |

### Functions

#### SetNiagaraDefaultValue

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|VFX|Init",
    meta = (DisplayName = "SetNiagaraDefaultValue"))
void SetNiagaraDefaultValue();
```

Initialize Niagara system parameters. Sets `Speed`, `LoopTime`, `Minimum`, `Maximum` variables and activates the system.

---

#### SetNiagaraSpawnCount

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|VFX|Control",
    meta = (DisplayName = "SetNiagaraSpawnCount"))
void SetNiagaraSpawnCount(const FSuperDMXAttribute DmxAttribute);
```

DMX-controlled particle spawn count.

**Mapping**: `DMX[0,1] → Lerp(0, MaxSpawnCount) → RoundToInt → SetVariableInt("SpawnCount")`

---

#### SetNiagaraColor

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|VFX|Color",
    meta = (DisplayName = "SetNiagaraColor"))
void SetNiagaraColor(const FSuperDMXAttribute DMXAttR,
    const FSuperDMXAttribute DMXAttG, const FSuperDMXAttribute DMXAttB);
```

DMX-controlled particle color (RGB three channels).

**Mapping**: `GetSuperDMXColorValue(R,G,B) → SetVariableLinearColor("Color")`

---

### Niagara Variable Mapping Table

| Niagara Variable Name | Type | Source |
|---------------|------|------|
| `Speed` | `float` | `MaxSpeed` |
| `LoopTime` | `float` | `LoopTime` |
| `Minimum` | `float` | `Minimum` |
| `Maximum` | `float` | `Maximum` |
| `SpawnCount` | `int32` | DMX dynamic control |
| `Color` | `FLinearColor` | DMX dynamic control |

---

### Usage Example

```
1. Create Niagara System asset (smoke/fire, etc.)
2. Create ASuperStageVFXActor subclass Blueprint
3. Assign Niagara System to Niagara component
4. Set MaxSpawnCount, MaxSpeed, and other default parameters
5. In SuperDMXTick:
   SetNiagaraDefaultValue()       // First frame initialization
   SetNiagaraSpawnCount(DmxSpawn) // Spawn count
   SetNiagaraColor(DmxR, DmxG, DmxB) // Color
   YPan(DmxPan)                   // Pan (inherited)
   YTilt(DmxTilt)                 // Tilt (inherited)
```

---

## 5. ASuperLightStripEffect — LED Strip Effect

**Header File**: `LightActor/SuperLightStripEffect.h`  
**Base Class**: `ASuperDmxActorBase`  
**Export Macro**: `SUPERSTAGE_API`

Achieves programmable LED strip effects (flow, chase, etc.) via materials. Supports batch application of effect materials to multiple StaticMeshActors in the scene.

### Inheritance Chain

```
AActor → ASuperBaseActor → ASuperDmxActorBase → ASuperLightStripEffect
```

### Default Parameters

| Property | Type | Default | Range | Description |
|------|------|--------|------|------|
| `MaxLightIntensity` | `float` | `1.0` | 0-1000 | Maximum intensity |
| `MaterialIndex` | `int` | `0` | 0-255 | Material slot index for target meshes |
| `TargetMeshActors` | `TArray<AStaticMeshActor*>` | — | — | Target meshes for batch application |
| `StaticMeshEffect` | `UStaticMesh*` | — | — | Self effect mesh |

### Control Parameters

| Property | Type | Default | Range | Description |
|------|------|--------|------|------|
| `Dimmer` | `float` | `0.0` | 0-1 | Intensity |
| `Strobe` | `float` | `0.0` | 0-1 | Strobe |
| `Effect` | `float` | `0.0` | 0-1 | Effect selection |
| `Speed` | `float` | `0.5` | 0-1 | Effect speed |
| `Width` | `float` | `0.0` | 0-1 | Effect width |
| `Direction` | `float` | `0.0` | 0-1 | Effect direction |
| `Color` | `FLinearColor` | `White` | — | RGB color |

### Material Parameter Mapping

| Material Parameter Name | DMX Mapping |
|-----------|---------|
| `MaxBrightness` | `MaxLightIntensity` |
| `Brightness` | `Dimmer` |
| `Strobe` | `Strobe × 255` |
| `StrobeMode` | Sub-attribute definition |
| `LightColor` | `Color` |
| `Effect` | `Effect → Lerp(0, 10)` |
| `Speed` | `Speed → Lerp(-10, 10)` |
| `Width` | `Width → Lerp(0, 10)` |
| `EffectDirection` | `Direction` |

### Functions

#### SetLightStripDefault

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|LightStrip|Init")
void SetLightStripDefault();
```

Initialize strip material. Creates dynamic material instance and applies to self mesh and all `TargetMeshActors`.

---

#### SetLightStripEffect

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|LightStrip|Control")
void SetLightStripEffect(
    FSuperDMXAttribute DmxDimmer, FSuperDMXAttribute DmxStrobe,
    FSuperDMXAttribute DmxEffect, FSuperDMXAttribute DmxSpeed,
    FSuperDMXAttribute DmxWidth,  FSuperDMXAttribute DmxDirection,
    FSuperDMXAttribute DmxRed,    FSuperDMXAttribute DmxGreen,
    FSuperDMXAttribute DmxBlue);
```

Full-channel DMX control function (9 channels). Call in `SuperDMXTick`.

---

### Strobe Modes (StrobeMode Values)

| Value | Mode | Description |
|----|------|------|
| 0 | Closed | Off (always dark) |
| 1 | Open | Always on |
| 2 | Linear | Linear strobe |
| 3 | Pulse | Pulse strobe |
| 4 | RampUp | Fade in |
| 5 | RampDown | Fade out |
| 6 | Sine | Sine strobe |
| 7 | Random | Random strobe |

---

## 6. ASuperLiftMatrix — Lift Matrix

**Header File**: `LightActor/SuperLiftMatrix.h`  
**Base Class**: `ASuperLightBase`  
**Export Macro**: `SUPERSTAGE_API`

Lift matrix Actor, containing 5 vertically arranged scene components (5cm spacing), with two `USuperEffectComponent` under each component. Wire ropes simulated at four corners.

### Inheritance Chain

```
AActor → ASuperBaseActor → ASuperDmxActorBase → ASuperLightBase → ASuperLiftMatrix
```

### Components

| Component | Type | Count | Description |
|------|------|------|------|
| `LiftComponents` | `TArray<USceneComponent*>` | 5 | Lift scene components |
| `EffectComponents` | `TArray<USuperEffectComponent*>` | 10 | Effect components (2 per segment) |
| `CableMeshes` | `TArray<UStaticMeshComponent*>` | 4 | Wire ropes (four corners) |
| `CableOrigin` | `USceneComponent*` | 1 | Wire rope starting point |

### Constants

| Constant | Value | Description |
|------|-----|------|
| `ComponentSpacing` | `5.0` cm | Component spacing |
| `ComponentCount` | `5` | Number of lift components |
| `DivisionCount` | `6` | Distribution equal parts |
| `CableCornerOffset` | `22.0` cm | Cable corner offset |
| `CableOriginHeight` | `15.0` cm | Cable origin height |
| `CableRadius` | `0.5` cm | Cable radius |

### Default Parameters

| Property | Type | Default | Description |
|------|------|--------|------|
| `MaxLightIntensity` | `float` | `1.0` | Maximum effect intensity |

### Functions

#### LiftMatrix

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|LiftMatrix|Lift",
    meta = (DisplayName = "LiftMatrix"))
void LiftMatrix(FSuperDMXAttribute DmxLift);
```

Lift control. 5 components distributed in 6 equal parts, DMX controls overall lift height. Automatically updates wire rope lengths.

---

#### SetEffectMatrixDefaultValue

```cpp
UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetEffectMatrixDefaultValue"))
void SetEffectMatrixDefaultValue();
```

Initialize materials for all effect components.

---

#### SetEffectMatrix / SetEffectColorMatrix

```cpp
void SetEffectMatrix(FSuperDMXAttribute DmxAttribute);
void SetEffectColorMatrix(FSuperDMXAttribute DMXAttR, FSuperDMXAttribute DMXAttG,
    FSuperDMXAttribute DMXAttB);
```

Effect selection and color control.

---

## 7. ASuperLaserActor — Laser Display (Texture Mode)

**Header File**: `LightActor/SuperLaserActor.h`  
**Base Class**: `ASuperBaseActor`  
**Export Macro**: `SUPERSTAGE_API`

Displays laser projection textures in real-time within the UE scene. Retrieves the specified Beyond device's RenderTarget from `SuperLaserSubsystem` and applies it to the beam material.

### Inheritance Chain

```
AActor → ASuperBaseActor → ASuperLaserActor
```

> **Note**: This class does not inherit from `ASuperDmxActorBase` and **does not use DMX control**. Laser content is generated by external Beyond software.

### Default Parameters

| Property | Type | Default | Range | Description |
|------|------|--------|------|------|
| `DeviceID` | `int32` | `1` | 1-12 | Beyond device number |
| `LaserIntensity` | `float` | `100.0` | 1-5000 | Laser intensity |
| `MaxLaserDistance` | `float` | `2345.0` | 100-10000 | Maximum projection distance (cm) |
| `AtmosphericDensity` | `float` | `0.03` | 0-1 | Atmospheric attenuation |
| `FogIntensity` | `float` | `20.0` | 0-100 | Fog intensity (%) |
| `AtmosFogSpeed` | `float` | `10.0` | 0-100 | Fog flow speed (%) |

### Render Flow

```
Tick → GetLaserTexture(query Subsystem)
     → SetLaserTexture(update material LightTexture parameter)
     → SetLaserVisibility(auto show/hide based on intensity)
     → MarkRenderStateDirty(notify render thread)
```

---

## 8. ASuperLaserProActor — Laser Display (Point Data Mode)

**Header File**: `LightActor/SuperLaserProActor.h`  
**Base Class**: `ASuperBaseActor`  
**Export Macro**: `SUPERSTAGE_API`

Point data-driven laser display. Retrieves laser point data from `SuperLaserSubsystem` and uses `USuperLaserProComponent` to procedurally generate mesh for rendering laser lines.

### Inheritance Chain

```
AActor → ASuperBaseActor → ASuperLaserProActor
```

> **Note**: This class does not inherit from `ASuperDmxActorBase` and **does not use DMX control**.

### Default Parameters

| Property | Type | Default | Range | Description |
|------|------|--------|------|------|
| `DeviceID` | `int32` | `1` | 1-12 | Beyond device number |
| `bEnableCollision` | `bool` | `false` | — | Enable collision occlusion detection |

### Components

| Component | Type | Description |
|------|------|------|
| `LaserComponent` | `USuperLaserProComponent*` | Laser Pro component |

### Functions

#### SetLaserEnabled

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|LaserPro|Control")
void SetLaserEnabled(bool bEnabled);
```

Control laser visibility.

### Point Data Format

| Field | Range | Description |
|------|------|------|
| `X`, `Y` | `[-1, 1]` | Normalized position (mapped to projection range) |
| `Z` | `>0` = beam point | Beam marker |
| `R`, `G`, `B` | `[0, 1]` | Color |

### Render Flow

```
Tick → UpdateLaserPoints()
     → GetLaserPoints(query Subsystem, DeviceID)
     → LaserComponent->SetLaserPoints(point data)
     → Procedural mesh draws laser lines
```

---

## 9. ASuperNDIScreen — NDI Video Screen

**Header File**: `LightActor/SuperNDIScreen.h`  
**Base Class**: `ASuperBaseActor`  
**Export Macro**: `SUPERSTAGE_API`

Receives real-time NDI video streams from `SuperNDISubsystem` and displays them on a screen mesh. Supports multi-screen output, keystone correction, and transparent material switching.

### Inheritance Chain

```
AActor → ASuperBaseActor → ASuperNDIScreen
```

> **Note**: This class does not inherit from `ASuperDmxActorBase` and **does not use DMX control**. Video content comes from NDI network streams.

### Properties

| Property | Type | Default | Description |
|------|------|--------|------|
| `InputName` | `FName` | `NAME_None` | NDI input name (with dropdown selection) |
| `OutputTexture` | `UTexture2D*` | — | Output texture (created at runtime, read-only) |
| `TargetStaticMeshActors` | `TArray<AStaticMeshActor*>` | — | Target screen meshes |
| `bTransparent` | `bool` | `false` | Transparent material toggle |
| `Transparency` | `float` | `0.95` | Transparency (0-1, transparent mode only) |
| `Brightness` | `float` | `1.0` | Brightness |
| `Color` | `FLinearColor` | `White` | Color filter |
| `Contrast` | `float` | `1.5` | Contrast |
| `Deformation` | `FDeformation` | — | Keystone correction parameters |

### FDeformation — Keystone Correction

| Property | Type | Default | Description |
|------|------|--------|------|
| `UpperLeftCorner` | `FVector2D` | `(0,0)` | Upper-left UV offset `[0,1]` |
| `LowerLeftCorner` | `FVector2D` | `(0,0)` | Lower-left UV offset |
| `UpperRightCorner` | `FVector2D` | `(0,0)` | Upper-right UV offset |
| `LowerRightCorner` | `FVector2D` | `(0,0)` | Lower-right UV offset |

### Data Flow

```
NDI network stream → SuperNDISubsystem → OnFrame callback
           → HandleNDIFrame(BGRA, Width, Height)
           → EnsureTexture(create/recreate texture)
           → UpdateTextureGPU(RHI upload to GPU)
           → DynamicMaterial.SetTextureParameterValue
           → Apply to all TargetStaticMeshActors
```

### Texture Management

- **Format**: `PF_B8G8R8A8` (BGRA8)
- **Creation**: `UTexture2D::CreateTransient`
- **Size change**: Release old texture → GC collection → create new texture
- **Update**: `RHIUpdateTexture2D` (render thread async command)

### Material System

- `MaterialOpaque` — Opaque material (default)
- `TransparentMaterial` — Semi-transparent material
- Toggle via `bTransparent`, `PostEditChangeProperty` triggers material rebuild

---

## 10. ASuperProjector — Projector / Mapping

**Header File**: `LightActor/SuperProjector.h`  
**Base Class**: `ASuperBaseActor`  
**Export Macro**: `SUPERSTAGE_API`

Implements Projection Mapping using light functions of three SpotLights. RGB three channels project separately and overlay to synthesize full-color effects.

### Inheritance Chain

```
AActor → ASuperBaseActor → ASuperProjector
```

> **Note**: This class does not inherit from `ASuperDmxActorBase` and **does not use DMX control**. Projection content is defined by `MappingTexture`.

### Components

| Component | Type | Description |
|------|------|------|
| `MappingModel` | `UStaticMeshComponent*` | Projection model |
| `SpotLightR` | `USpotLightComponent*` | Red channel projection light |
| `SpotLightG` | `USpotLightComponent*` | Green channel projection light |
| `SpotLightB` | `USpotLightComponent*` | Blue channel projection light |

### Properties

| Property | Type | Default | Range | Description |
|------|------|--------|------|------|
| `MappingScale` | `FVector2D` | `(1920,1080)` | 128-4096 | Projection resolution |
| `MappingTexture` | `UTexture*` | `nullptr` | — | Projection texture content |
| `Dimmer` | `float` | `100.0` | ≥0 | Intensity (%) |
| `MaxLightDistance` | `float` | `3000.0` | ≥100 | Projection distance (cm) |
| `Zoom` | `float` | `30.0` | 10-100 | Projection angle (degrees) |
| `DilutionFactor` | `float` | `0.0` | 0-1 | Edge softening |
| `MappingDeformation` | `FMappingDeformation` | — | — | Keystone correction |

### FMappingDeformation — Keystone Correction

Same structure as `FDeformation`, four-corner UV offsets `[0, 1]`.

### Projection Principle

```
SpotLightR/G/B each set LightFunctionMaterial
    ↓
Light function material contains MappingTexture
    ↓
Keystone correction implemented as UV transform in material Shader
    ↓
Three-channel RGB overlay → Full-color projection
```

**SpotLight parameter mapping**:
- `Intensity` = `Dimmer` (Candelas units)
- `AttenuationRadius` = `MaxLightDistance`
- `OuterConeAngle` = `Zoom`
- `InnerConeAngle` = `Zoom × (1 - DilutionFactor)`
- `LightFunctionScale` = `(MappingScale.X, MappingScale.Y, Diagonal)`

---

## 11. API Quick Reference

### By Category

#### DMX-Controlled Actors

| Actor | Base Class | DMX Axes | Primary Use |
|-------|------|---------|---------|
| `ASuperDMXCamera` | `ASuperDmxActorBase` | 9 (6-axis+FOV+Aperture+Focus) | DMX Camera |
| `ASuperLiftingMachinery` | `ASuperDmxActorBase` | 6 (3-axis displacement+3-axis rotation) | Lift/Rotate Machinery |
| `ASuperRailMachinery` | `ASuperDmxActorBase` | 7 (Rail+3-axis offset+3-axis rotation) | Rail Machinery |
| `ASuperStageVFXActor` | `ASuperLightBase` | 3+2 (Pan/Tilt+SpawnCount+RGB) | Niagara VFX |
| `ASuperLightStripEffect` | `ASuperDmxActorBase` | 9 (Intensity+Strobe+Effect+Speed+Width+Direction+RGB) | LED Strip |
| `ASuperLiftMatrix` | `ASuperLightBase` | 1+3 (Lift+Effect+RGB) | Lift Matrix |

#### Non-DMX Actors

| Actor | Base Class | Data Source | Primary Use |
|-------|------|---------|---------|
| `ASuperLaserActor` | `ASuperBaseActor` | SuperLaserSubsystem (texture) | Beyond Laser (texture mode) |
| `ASuperLaserProActor` | `ASuperBaseActor` | SuperLaserSubsystem (point data) | Beyond Laser (point data mode) |
| `ASuperNDIScreen` | `ASuperBaseActor` | SuperNDISubsystem (video stream) | NDI Video Screen |
| `ASuperProjector` | `ASuperBaseActor` | UTexture (static texture) | Projector Mapping |

### Main Control Function Quick Reference

| Function | Actor | Description |
|------|-------|------|
| `DMXCameraControl` | `ASuperDMXCamera` | 6-axis camera motion |
| `DMXCameraParams` | `ASuperDMXCamera` | FOV/Aperture/Focus |
| `LiftingMachinery` | `ASuperLiftingMachinery` | 6-axis lifting machinery |
| `RailMachinery` | `ASuperRailMachinery` | 7-axis rail machinery |
| `RailPositionOnly` | `ASuperRailMachinery` | Rail position only |
| `SetNiagaraSpawnCount` | `ASuperStageVFXActor` | Particle spawn count |
| `SetNiagaraColor` | `ASuperStageVFXActor` | Particle color |
| `SetLightStripEffect` | `ASuperLightStripEffect` | Strip full-channel control |
| `LiftMatrix` | `ASuperLiftMatrix` | Lift matrix |
| `SetLaserEnabled` | `ASuperLaserProActor` | Laser toggle |

### Initialization Function Quick Reference

| Function | Actor | Call Timing |
|------|-------|---------|
| `BootRefresh` | Camera/Machinery/Rail | Editor button, call after placement |
| `SetNiagaraDefaultValue` | VFXActor | SuperDMXTick first frame |
| `SetLightStripDefault` | LightStrip | SuperDMXTick first frame |
| `SetEffectMatrixDefaultValue` | LiftMatrix | SuperDMXTick first frame |
