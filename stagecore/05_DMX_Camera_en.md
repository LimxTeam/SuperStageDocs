# 05 - DMX Camera (SuperDMXCamera)

> **Module**: SuperStage Runtime (ASuperDMXCamera)  
> **Target Users**: Lighting designers, virtual production technicians  
> **Prerequisites**: [03 - DMX Fixture Base](/docs/stage-core/dmx-actor-base)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

**SuperDMXCamera** is a virtual cine camera controlled via DMX signals. It inherits from SuperDmxActorBase and can be controlled from an MA console just like a fixture, controlling the camera's:

- **6-Axis Motion** — X/Y/Z three-axis translation + Pitch/Yaw/Roll three-axis rotation
- **Lens Parameters** — FOV (Field of View), Aperture (f-stop), Focus Distance
- **Render Output** — Can render camera view to a RenderTarget for display on LED screens, etc.

### Use Cases

- Real-time camera preview on stage (console operator remotely controls virtual camera angles)
- Camera scheduling for virtual studios
- Real-time camera rendering for LED screen content
- Sync video content with lighting in light shows

---

## 2. Class Inheritance Chain

```
AActor (UE5 Engine Base Class)
  │
  └── ASuperBaseActor ·············· SuperStage Root Class (L1)
        │                           ├ SceneBase / ForwardArrow / UpArrow
        │                           └ AssetMetaData
        │
        └── ASuperDmxActorBase ····· DMX Fixture Base Class (L2)
              │                      ├ DMX Address (Universe/Address/FixtureID)
              │                      ├ Fixture Library / Channel Reading / Matrix Reading
              │                      └ SuperDMXTick / Address Label
              │
              └── ASuperDMXCamera ·· DMX Camera (L3) ← Class described in this document
                    ├ UCineCameraComponent  ← Cine camera (FOV/Aperture/Focus)
                    ├ USceneCaptureComponent2D ← Scene capture (render to RT)
                    ├ 6-axis motion control (translation + rotation)
                    ├ Lens parameter control (FOV/Aperture/FocusDistance)
                    ├ Boot Refresh initialization
                    └ CameraParamSpeed interpolation smoothing
```

> **Design Note**: `ASuperDMXCamera` inherits directly from `ASuperDmxActorBase` rather than `ASuperLightBase`, because cameras don't need the Pan/Tilt rotation axis structure (SceneRocker/SceneHard). It uses its own 6-axis free motion system, which is more suitable for camera control scenarios than the Pan/Tilt model.

---

## 3. Component Structure

```
ASuperDMXCamera
  └── SceneBase (USceneComponent)                    ← Root component (inherited)
        ├── ForwardArrow (UArrowComponent)            ← +X direction preview (inherited)
        ├── UpArrow (UArrowComponent)                 ← +Z direction preview (inherited)
        ├── AddressLabel (UTextRenderComponent)       ← DMX address label (inherited)
        │
        ├── CineCamera (UCineCameraComponent)         ← Cine camera component
        │     ├ FilmbackSettings   ← Film back size (Super 35mm etc.)
        │     ├ LensSettings       ← Lens parameters (focal length/aperture/focus)
        │     └ PostProcessSettings ← Post process settings
        │
        └── SceneCapture (USceneCaptureComponent2D)   ← Scene capture component
              ├ TextureTarget      ← Render target texture
              ├ CaptureSource      ← Capture source type
              └ bCaptureEveryFrame ← Whether to capture every frame
```

| Component | Type | Description |
|------|------|------|
| **CineCamera** | UCineCameraComponent | Cine camera component providing professional camera features (FOV/Aperture/Focus) |
| **SceneCapture** | USceneCaptureComponent2D | Scene capture component rendering view to RenderTarget for LED screen use |

---

## 4. Default Parameter Configuration

In the **B.DefaultParameter** group of the Details panel, you can set the camera's motion ranges and lens parameter ranges.

### 4.1 Start Switch

| Parameter | Description | Default |
|------|------|--------|
| **Start** | Camera start switch. When set to True, the camera begins responding to DMX signals | False |

> **Important**: After placing in the scene, you must set Start to **True** to begin accepting DMX control.

### 4.2 Initialization Function

| Operation | Description |
|------|------|
| **Boot Refresh** | Click this button (in editor) to automatically calculate InitialPosition/EndPosition and InitialRotation/EndRotation based on the current position and MovingRange/RotRange |

**Usage Steps**:
1. Place the camera at the **center position** you desire in the scene
2. Set MovingRange (translation range) and RotRange (rotation range)
3. Click the **Boot Refresh** button
4. The system automatically calculates InitialPosition and EndPosition (based on current position ± MovingRange/2)
5. The system automatically calculates InitialRotation and EndRotation (based on current rotation ± RotRange/2)

### 4.3 Translation Range

| Parameter | Description | Default | Unit |
|------|------|--------|------|
| **Moving Range** | Translation range for X/Y/Z three axes (total range) | (0, 0, 0) | cm |
| **Initial Position** | Start of translation range (system auto-calculated, read-only) | (0, 0, 0) | cm |
| **End Position** | End of translation range (system auto-calculated, read-only) | (0, 0, 0) | cm |

**Translation Mapping**:
```
Actual Position = Lerp(InitialPosition, EndPosition, DMX Value)
```

Where the DMX value is normalized 0.0 ~ 1.0:
- 0.0 → at InitialPosition (range start)
- 0.5 → at center position (initial placement position)
- 1.0 → at EndPosition (range end)

**Example**:
- Camera placed at (500, 0, 300)
- Moving Range set to (200, 100, 50)
- After Boot Refresh: InitialPosition = (400, -50, 275), EndPosition = (600, 50, 325)
- DMX PosX = 0.0 → X = 400, DMX PosX = 1.0 → X = 600

### 4.4 Rotation Range

| Parameter | Description | Default | Unit |
|------|------|--------|------|
| **Rot Range** | Pitch/Yaw/Roll three-axis rotation range (total range) | (180, 360, 90) | degrees |
| **Initial Rotation** | Start of rotation range (system auto-calculated, read-only) | (0, 0, 0) | degrees |
| **End Rotation** | End of rotation range (system auto-calculated, read-only) | (0, 0, 0) | degrees |

**Rotation Mapping**:
```
Actual Rotation = Lerp(InitialRotation, EndRotation, DMX Value)
```

**Default Rot Range = (180, 360, 90) Meaning**:
- **Pitch**: ±90°, total range 180°
- **Yaw**: ±180°, total range 360° (full rotation capability)
- **Roll**: ±45°, total range 90°

### 4.5 Lens Parameter Ranges

| Parameter | Description | Default | Unit |
|------|------|--------|------|
| **FOV Range** | FOV range (min, max) | (15.0, 120.0) | degrees |
| **Aperture Range** | Aperture range (min, max) | (1.2, 22.0) | f-stop |
| **Focus Distance Range** | Focus distance range (min, max) | (50.0, 100000.0) | cm |

**Mapping Method** (same as translation/rotation):
```
Actual FOV = Lerp(FOVRange.Min, FOVRange.Max, DMX Value)
Actual Aperture = Lerp(ApertureRange.Min, ApertureRange.Max, DMX Value)
Actual Focus Distance = Lerp(FocusDistanceRange.Min, FocusDistanceRange.Max, DMX Value)
```

### 4.6 Interpolation Speed

| Parameter | Description | Default | Range |
|------|------|--------|------|
| **Camera Param Speed** | Interpolation speed for camera parameter changes | 3.0 | 0.0 - 10.0 |

- Larger value → faster response (more direct)
- Smaller value → slower response (smoother but with delay)
- Value = 0 → no movement (locked)
- Value = 10 → almost no delay

> **Tip**: Recommended 3.0 ~ 5.0 for a balance between responsiveness and smoothness.

---

## 5. Render Output Configuration

SuperDMXCamera can render camera view to a RenderTarget for display on LED screens, monitors, etc.

| Parameter | Description | Default |
|------|------|--------|
| **Render Target** | Render target texture (UTextureRenderTarget2D) | None |
| **Render Resolution** | Render resolution | 1920 × 1080 |
| **Enable Scene Capture** | Whether to enable scene capture rendering | False |

### 5.1 Setup Steps

1. Create a **Render Target** asset in the Content Browser
2. Assign it to the camera's **Render Target** property
3. Set the desired **Render Resolution**
4. Set **Enable Scene Capture** to **True**
5. Apply the Render Target to a material in the scene (e.g., LED screen panel material)

> **Performance Tip**: Scene capture consumes additional GPU resources. If render output is not needed, keep Enable Scene Capture at False.

---

## 6. Control Parameters (Runtime Values)

In the **C.ControlParameter** group of the Details panel, you can see the current control parameter values. These values are typically driven by DMX signals but can also be manually adjusted in the editor for testing:

### 6.1 Translation Control

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **PosX** | X-axis position (normalized) | 0.0 - 1.0 | 0.5 (center) |
| **PosY** | Y-axis position (normalized) | 0.0 - 1.0 | 0.5 (center) |
| **PosZ** | Z-axis position (normalized) | 0.0 - 1.0 | 0.5 (center) |

### 6.2 Rotation Control

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **RotX** | Pitch rotation (normalized) | 0.0 - 1.0 | 0.5 (straight forward) |
| **RotY** | Yaw rotation (normalized) | 0.0 - 1.0 | 0.5 (straight forward) |
| **RotZ** | Roll rotation (normalized) | 0.0 - 1.0 | 0.5 (level) |

### 6.3 Lens Control

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **FOV** | Field of view (normalized) | 0.0 - 1.0 | 0.5 |
| **Aperture** | Aperture (normalized) | 0.0 - 1.0 | 0.5 |
| **Focus Distance** | Focus distance (normalized) | 0.0 - 1.0 | 0.5 |

> **All control parameters are normalized 0.0 ~ 1.0 values**. The system automatically maps them to actual physical values based on the previously set ranges.

---

## 7. Cache Information (Read-Only)

In the **A.Cache Info** group of the Details panel, you can view the current actual physical values (after interpolation smoothing):

| Parameter | Description | Unit |
|------|------|------|
| **CurrentPosX/Y/Z** | Current actual position | cm |
| **CurrentRotX/Y/Z** | Current actual rotation | degrees |
| **CurrentFOV** | Current actual FOV | degrees |
| **CurrentAperture** | Current actual aperture | f-stop |
| **CurrentFocusDistance** | Current actual focus distance | cm |

These values are read-only, primarily used for debugging and monitoring.

---

## 8. Blueprint Control Functions

SuperDMXCamera provides two core blueprint functions for DMX control:

### 8.1 DMXCameraControl (Motion Control)

Controls the camera's 6-axis motion:

| Input Parameter | Description |
|---------|------|
| **DmxXPos** | DMX attribute for X-axis translation |
| **DmxYPos** | DMX attribute for Y-axis translation |
| **DmxZPos** | DMX attribute for Z-axis translation |
| **DmxXRot** | DMX attribute for Pitch rotation |
| **DmxYRot** | DMX attribute for Yaw rotation |
| **DmxZRot** | DMX attribute for Roll rotation |

### 8.2 DMXCameraParams (Lens Parameter Control)

Controls the camera's lens parameters:

| Input Parameter | Description |
|---------|------|
| **DmxFOV** | DMX attribute for FOV |
| **DmxAperture** | DMX attribute for Aperture |
| **DmxFocusDistance** | DMX attribute for Focus Distance |

### 8.3 Typical Blueprint Implementation

```
Event SuperDMXTick(DeltaTime)
  │
  ├─ DMXCameraControl(PosX, PosY, PosZ, RotX, RotY, RotZ)
  └─ DMXCameraParams(FOV, Aperture, FocusDistance)
```

---

## 9. DMX Channel Planning Recommendations

One SuperDMXCamera requires 9 basic DMX attributes. Using 16-bit precision (recommended), it requires 18 channels:

| Channel | Attribute | Precision | Description |
|------|------|------|------|
| 1-2 | PosX | 16-bit | X-axis translation |
| 3-4 | PosY | 16-bit | Y-axis translation |
| 5-6 | PosZ | 16-bit | Z-axis translation |
| 7-8 | RotX | 16-bit | Pitch rotation |
| 9-10 | RotY | 16-bit | Yaw rotation |
| 11-12 | RotZ | 16-bit | Roll rotation |
| 13-14 | FOV | 16-bit | Field of view |
| 15-16 | Aperture | 16-bit | Aperture |
| 17-18 | FocusDistance | 16-bit | Focus distance |

> **Tip**: If fine control isn't needed, 8-bit precision (9 channels) can be used, but motion may not be smooth enough.

---

## 10. Usage Steps Summary

1. **Place Camera** — Drag SuperDMXCamera into the scene at your desired center position
2. **Set Ranges** — Configure MovingRange, RotRange, FOVRange, ApertureRange, FocusDistanceRange
3. **Initialize** — Click **Boot Refresh** to auto-calculate start/end ranges
4. **Configure Fixture Library** — Create or select a fixture library containing 9 attributes
5. **Set Patch** — Assign Universe and Start Address
6. **Start** — Set Start to True
7. **(Optional) Configure Render Output** — Set RenderTarget and Enable Scene Capture
8. **Send Signals from Console** — Push corresponding channel faders; the camera should begin responding

---

## 11. FAQ

### Q: Camera not responding to DMX signals?
1. Check if **Start** is set to True
2. Check if **Boot Refresh** was clicked
3. Confirm DMX network is correctly configured (see [01 - Network Configuration](/docs/stage-core/dmx-network))
4. Confirm fixture library and Patch configuration are correct

### Q: Camera movement jittery?
- Increase **Camera Param Speed** value (makes movement smoother)
- Or use 16-bit precision DMX attributes (reduces quantization error)

### Q: Camera movement range wrong?
- Re-adjust MovingRange / RotRange
- Re-click **Boot Refresh** to update calculations

### Q: Render output shows black?
- Confirm **Enable Scene Capture** is True
- Confirm **Render Target** is correctly assigned
- Confirm Render Target resolution is set correctly

---

> **Next Steps**: Read [06 - DMX Activity Monitor](/docs/stage-core/dmx-monitor) to learn how to monitor DMX channel data in real-time.
