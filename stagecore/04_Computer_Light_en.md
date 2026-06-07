# 04 - Moving Light Base Class (SuperLightBase)

> **Module**: SuperStage Runtime (ASuperLightBase)  
> **Target Users**: Lighting designers, blueprint developers  
> **Prerequisites**: [03 - DMX Fixture Base](/docs/stage-core/dmx-actor-base)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

**SuperLightBase** is the base class for all moving lights (Moving Light / Moving Head), inheriting from SuperDmxActorBase. On top of basic DMX capabilities, it adds features specific to stage moving lights:

- **Pan / Tilt Motion Control** — Horizontal/vertical rotation with customizable angle ranges
- **Infinite Rotation** — Continuous rotation mode simulating infinite rotation motors
- **Pan/Tilt Speed Control** — Motion transition speed adjustment
- **Matrix Control** — Independent Pan/Tilt for multiple heads (matrix lights, multi-bead fixtures)
- **Lift Control** — Z-axis displacement (simulating fixture lifts), configurable lift range and speed
- **Lamp Angle Control** — Manual fixed beam divergence angle

---

## 2. Class Inheritance Chain

```
AActor (UE5 Engine Base Class)
  │
  └── ASuperBaseActor ·············· SuperStage Root Class (L1)
        │
        └── ASuperDmxActorBase ····· DMX Fixture Base Class (L2)
              │                      ├ DMX Address / Fixture Library / Channel Reading
              │                      └ SuperDMXTick / Address Label
              │
              └── ASuperLightBase ·· Moving Light Base (L3) ← Class described in this document
                    │                 ├ SceneRocker → SceneHard (Pan/Tilt rotation axes)
                    │                 ├ Pan/Tilt range mapping + infinite rotation
                    │                 ├ PTSpeed speed control
                    │                 ├ Matrix Pan/Tilt
                    │                 ├ Z-axis lift
                    │                 └ Lamp angle
                    │
                    ├── ASuperStageLight ···· Full-featured Moving Light  → Document 15
                    ├── ASuperStageVFXActor · Stage VFX                   → Document 14
                    └── ASuperLiftMatrix ···· Lift Matrix Light Array     → Document 12
```

> **Design Note**: `ASuperLightBase` only manages Pan/Tilt motion control and is completely unconcerned with color, gobo, prism, and other lighting effects. These functions are implemented by the subclass `ASuperStageLight`. If you only need a rotatable DMX device (e.g., a rotatable smoke machine nozzle), directly inherit `ASuperLightBase`.

---

## 3. Component Structure

Each moving light Actor contains the following key components:

```
ASuperLightBase
  └── SceneBase (USceneComponent)                    ← Root component (inherited from ASuperBaseActor)
        ├── ForwardArrow (UArrowComponent)            ← +X direction preview (inherited)
        ├── UpArrow (UArrowComponent)                 ← +Z direction preview (inherited)
        ├── AddressLabel (UTextRenderComponent)       ← DMX address label (inherited)
        │
        └── SceneRocker (USceneComponent)             ← Arm / Pan rotation axis (around Z-axis)
              │   The rocker arm portion of the fixture 3D model attaches here
              │
              └── SceneHard (USceneComponent)          ← Head / Tilt rotation axis (around Y-axis)
                    │   The head portion of the fixture 3D model attaches here
                    │   Subclass beam/color/effect components are also mounted here
                    │
                    └── [Subclass component mount points]
                          ├ ASuperStageLight: Beam components, SpotLight, materials...
                          ├ ASuperStageVFXActor: UNiagaraComponent
                          └ ASuperLiftMatrix: LiftComponents[], EffectComponents[]
```

- **SceneRocker** (Arm): The horizontal rotation part of the entire fixture, Pan rotation acts on this component
- **SceneHard** (Head): The vertical rotation part of the fixture, Tilt rotation acts on this component. All lighting effect components mount under SceneHard, ensuring they rotate with the head

---

## 4. Pan / Tilt Control

### 4.1 Range Settings

| Parameter | Property Name | Description | Default | Unit |
|------|--------|------|--------|------|
| **Pan Range** | `PanRange` | Pan (horizontal rotation) angle range | (-270, +270) | degrees |
| **Tilt Range** | `TiltRange` | Tilt (vertical rotation) angle range | (-135, +135) | degrees |
| **Pan Angle** | `PanAngle` | Pan initial offset angle | 0 | degrees |
| **Tilt Angle** | `TiltAngle` | Tilt initial offset angle | 0 | degrees |

**Range Meaning**:
- PanRange = (-270, +270) means the fixture can rotate from -270° to +270° (total span 540°)
- TiltRange = (-135, +135) means the fixture can rotate from -135° to +135° (total span 270°)

> **Tip**: Refer to the actual fixture's specification sheet for correct ranges. Common moving light Pan ranges are typically 540° (±270°) or 630° (±315°), and Tilt ranges are typically 270° (±135°) or 306° (±153°).

**Parameter Tuning Suggestions**:

| Fixture Type | Recommended PanRange | Recommended TiltRange | Description |
|---------|--------------|---------------|------|
| Standard Moving Light | (-270, +270) | (-135, +135) | Standard spec for most Moving Heads |
| High-speed Scanner | (-315, +315) | (-153, +153) | High-end fixtures like Robe MegaPointe |
| Compact Wash Light | (-180, +180) | (-90, +90) | Simple wash lights with smaller rotation |
| Rotating LED Panel | (-180, +180) | (0, 0) | Pan rotation only, Tilt fixed |

### 4.2 Angle Calculation

Mapping from DMX value (normalized to 0.0-1.0) to rotation angle:

```
Pan Angle = (DMX Value - 0.5) × PanRange
Tilt Angle = (DMX Value - 0.5) × TiltRange
```

**Example** (Pan Range = 540°):

| DMX Value (Normalized) | Pan Angle |
|-----------------|---------|
| 0.0 | -270° |
| 0.25 | -135° |
| 0.5 | 0° (center) |
| 0.75 | +135° |
| 1.0 | +270° |

---

## 5. Feature Toggles

SuperLightBase provides a series of boolean toggles to enable/disable specific DMX control features. These toggles are set in the fixture blueprint defaults:

### 5.1 Toggle List

| Toggle | Description | Default |
|------|------|--------|
| **bChannelEdit** | Master channel edit toggle (controls visibility of the following toggles in the Details panel) | Off |
| **bPan** | Enable Pan (horizontal rotation) control | Off |
| **bTilt** | Enable Tilt (vertical rotation) control | Off |
| **bPTSpeed** | Enable Pan/Tilt speed control channel | Off |
| **bPanRot** | Enable Pan infinite rotation mode | Off |
| **bTiltRot** | Enable Tilt infinite rotation mode | Off |
| **bYPolarRotation** | Infinite rotation mode toggle (controls InfiniteRotationalSpeed visibility) | Off |
| **bInPosZ** | Enable Z-axis lift control (interpolation mode) | Off |
| **bPositionZ** | Enable Z-axis lift control (position mode, controls LiftRange visibility) | Off |
| **bLampAngle** | Enable lamp beam angle control | Off |

### 5.2 Toggle Usage Notes

- **Turning off unnecessary toggles** avoids unnecessary DMX reading and computation
- Each toggle corresponds to one or more DMX attributes
- If a fixture doesn't support a feature (e.g., no infinite rotation), turn off the corresponding toggle

---

## 6. Infinite Rotation

Infinite rotation allows the fixture's Pan or Tilt to perform **continuous non-stop rotation**, rather than being positioned within a fixed range like in normal mode.

### 6.1 Enabling Infinite Rotation

1. In the fixture blueprint, set **bPanRot** (Pan infinite rotation) or **bTiltRot** (Tilt infinite rotation) to **True**
2. Ensure the fixture library defines the `PanRot` or `TiltRot` attribute

### 6.2 Infinite Rotation Speed

| Parameter | Property Name | Description | Default | Range | Unit |
|------|--------|------|--------|------|------|
| **Infinite Rotational Speed** | `InfiniteRotationalSpeed` | Maximum speed for infinite rotation | 1.0 | 0.1 - 10.0 | Speed multiplier |

**Parameter Tuning Suggestions**:
- Value 1.0 gives moderate speed (suitable for most fixtures)
- Larger values give faster rotation but may cause visual jitter
- Recommended to adjust based on real fixture infinite rotation speed (typically 0.3-2.0 rotations/sec)

### 6.3 Rotation Modes

| Mode | Description |
|------|------|
| **Standard** | Standard mode — infinite rotation uses default logic |
| **Custom** | Custom mode — custom rotation logic can be defined in blueprints |

### 6.4 How Infinite Rotation Works

Infinite rotation channel DMX value mapping:

| DMX Value Range (Normalized) | Behavior |
|---------------------|------|
| 0.0 - 0.49 | Counter-clockwise rotation, speed decreases from max to stop |
| 0.49 - 0.51 | Stop (dead zone) |
| 0.51 - 1.0 | Clockwise rotation, speed increases from stop to max |

---

## 7. Pan/Tilt Speed Control

When **bPTSpeed** is enabled, the fixture responds to a speed control channel for adjusting Pan/Tilt motion transition speed.

### 7.1 Speed Parameters

| Parameter | Property Name | Description | Default | Range |
|------|--------|------|--------|------|
| **PT Speed** | `PTSpeed` | Pan/Tilt movement interpolation speed | 1.0 | 0.1 - 20.0 |

**Parameter Tuning Suggestions**:
- Larger values → faster fixture response (closer to "snapping")
- Smaller values → slower fixture response (smoother transitions)
- Recommended range 3.0-8.0, simulating real fixture motion inertia
- Fast follow spot scenarios: 8.0-15.0
- Slow ambient scenarios: 1.0-3.0

### 7.2 How It Works

- Speed channel DMX value = 0 → fastest speed (direct jump to target position)
- Speed channel DMX value = 255 → slowest speed (slowly move to target position)

> **Note**: Specific behavior of speed control depends on the implementation in the fixture blueprint. The base class provides the YPTSpeed function as a framework.

---

## 8. Matrix Control

Matrix functionality is used for controlling **multi-head fixtures**, such as LED panel lights, multi-bead wash lights, etc.

### 8.1 Matrix Concept

A matrix fixture contains multiple "heads" (module instances), each independently controllable:

```
A 7×1 Matrix Light
┌───┬───┬───┬───┬───┬───┬───┐
│ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │  ← 7 module instances
└───┴───┴───┴───┴───┴───┴───┘
 │    │    │    │    │    │    │
 Independent RGBW / Pan / Tilt channels each
```

### 8.2 Matrix Pan/Tilt

When a fixture has multiple heads and each can rotate independently, SuperLightBase provides matrix Pan/Tilt functions:

- **YMatrixPan** — Independent Pan control for all heads
- **YMatrixTilt** — Independent Tilt control for all heads
- **YMatrixPanRot** — Independent Pan infinite rotation for all heads
- **YMatrixTiltRot** — Independent Tilt infinite rotation for all heads

### 8.3 Matrix Reading Method

In blueprints, use the **Get Matrix Attribute Raw** node to retrieve a specific attribute's value for all module instances at once:

- Input: Attribute name (e.g., `Red`)
- Output: Float array, each element is one module instance's value (0.0 - 1.0)

**Matrix Reading with Index** (Get Matrix Attribute Raw With Index):
- Additionally outputs module index array for knowing which value corresponds to which module instance
- Results sorted by Patch value

### 8.4 Matrix Configuration Requirements

1. Fixture library must define multiple module instances (one per head)
2. Each module instance's Patch value set correctly
3. Each module instance defines the same-named attributes (e.g., each pixel has Red, Green, Blue)

---

## 9. Lift Control (Lift Z)

When **bInPosZ** is enabled, the fixture can control its Z-axis position (up/down movement) via DMX, simulating the effect of a fixture lift.

### 9.1 Lift Parameters

| Parameter | Property Name | Description | Default | Range | Unit |
|------|--------|------|--------|------|------|
| **Lift Range** | `LiftRange` | Maximum lift distance | 500.0 | 0 - any positive | cm |
| **Lift Speed** | `LiftSpeed` | Lift interpolation speed | 1.0 | 0.1 - 10.0 | Speed multiplier |
| **InPosZ** | `InPosZ` | Current Z-axis lift control value | 0.0 | 0.0 - 1.0 | Normalized |

### 9.2 Lift Mapping

```
Z Displacement = DMX Value (0~1) × LiftRange
```

- DMX Value = 0.0 → Z offset = 0cm (original position)
- DMX Value = 0.5 → Z offset = 250cm (when LiftRange default is 500)
- DMX Value = 1.0 → Z offset = 500cm (maximum lift distance)

**Parameter Tuning Suggestions**:

| Scenario | Recommended LiftRange | Recommended LiftSpeed | Description |
|------|---------------|----------------|------|
| Small stage lift | 200-500 cm | 1.0-2.0 | Standard lift platform |
| Large show lift | 500-1500 cm | 0.5-1.5 | Long-travel lift |
| Quick pop-up effect | 50-200 cm | 5.0-10.0 | Sudden pop-up visual effect |

The **SetLiftZ** function receives normalized DMX values and maps them to the fixture's Z-axis displacement range.

---

## 10. Lamp Angle Control

When **bLampAngle** is enabled, the fixture can control its beam divergence angle via DMX (rather than using the Zoom zoom channel).

The **YLampAngle** function receives normalized DMX values and maps them to the fixture beam angle range.

> **Note**: This function is used for manually fixing the lamp angle and differs from the Zoom function in `ASuperStageLight`. Zoom is continuous zooming achieved through the optical system; LampAngle is manual adjustment of the physical lamp body angle.

---

## 11. Full Default Parameter Reference

The following is all configurable default parameters at the `ASuperLightBase` level:

| Parameter | Property Name | Category | Default | Range | Description |
|------|--------|------|--------|------|------|
| Pan Initial Angle | `PanAngle` | B.DefaultParameter | 0 | -360~360 | Pan axis initial offset |
| Tilt Initial Angle | `TiltAngle` | B.DefaultParameter | 0 | -360~360 | Tilt axis initial offset |
| Pan Range | `PanRange` | B.DefaultParameter | (-270, +270) | Custom | Horizontal rotation angle range |
| Tilt Range | `TiltRange` | B.DefaultParameter | (-135, +135) | Custom | Vertical rotation angle range |
| PT Speed | `PTSpeed` | B.DefaultParameter | 1.0 | 0~20 | Pan/Tilt interpolation speed |
| Infinite Rotation Speed | `InfiniteRotationalSpeed` | B.DefaultParameter | 1.0 | 0~10 | Infinite rotation speed multiplier |
| Lift Range | `LiftRange` | B.DefaultParameter | 500.0 | 0~any | Z-axis lift max distance (cm) |
| Lift Speed | `LiftSpeed` | B.DefaultParameter | 1.0 | 0~10 | Lift interpolation speed |

---

## 12. Built-in Control Function Reference

The following are the core control functions provided by SuperLightBase (called as BlueprintCallable in blueprints):

| Function | Description | Input |
|------|------|------|
| **YPan** | Set Pan rotation angle | DMX Attribute |
| **YTilt** | Set Tilt rotation angle | DMX Attribute |
| **YPTSpeed** | Set Pan/Tilt movement speed | DMX Attribute |
| **YPanRot** | Set Pan infinite rotation | DMX Attribute |
| **YTiltRot** | Set Tilt infinite rotation | DMX Attribute |
| **YMatrixPan** | Matrix light individual head Pan | Attribute name |
| **YMatrixTilt** | Matrix light individual head Tilt | Attribute name |
| **YMatrixPanRot** | Matrix light individual head Pan infinite rotation | Attribute name |
| **YMatrixTiltRot** | Matrix light individual head Tilt infinite rotation | Attribute name |
| **YLampAngle** | Set lamp beam angle | DMX Attribute |
| **SetLiftZ** | Set fixture Z-axis lift position | DMX Attribute |

---

## 13. Typical Moving Light Blueprint Structure

A typical SuperDMXTick event implementation flow in a moving light blueprint:

```
Event SuperDMXTick(DeltaTime)
  │
  ├─ YPan(PanAttribute)           ← Set horizontal rotation
  ├─ YTilt(TiltAttribute)         ← Set vertical rotation
  ├─ YPTSpeed(SpeedAttribute)     ← Set movement speed (optional)
  │
  ├─ GetSuperDmxAttributeValue("Dimmer") → Set light intensity
  ├─ GetSuperDMXColorValue(R, G, B)      → Set light color
  │
  ├─ GetSuperDmxAttributeValue("Zoom")   → Set beam angle
  ├─ GetSuperDmxAttributeValue("Focus")  → Set focus
  │
  └─ GetSuperDmxAttributeValue("Gobo")   → Switch gobo pattern
```

---

## 14. FAQ

### Q: Pan/Tilt direction reversed?
Check the rotation axis settings of SceneRocker and SceneHard components in the fixture blueprint. Pan should rotate around the Z-axis, Tilt around the Y-axis.

### Q: Pan/Tilt movement not smooth enough?
Ensure the Pan/Tilt attributes in the fixture library have Fine channels (16-bit precision) set and Channel Type is Fine.

### Q: Only one head of the matrix light responds?
1. Check if independent module instances were created for each head in the fixture library
2. Confirm each module instance's Patch value is correct
3. Confirm the blueprint uses matrix reading functions (GetMatrixAttributeRaw) rather than single-instance reading

### Q: Infinite rotation not working?
1. Confirm bPanRot or bTiltRot toggle is enabled
2. Confirm PanRot or TiltRot attribute is defined in the fixture library
3. Confirm the DMX channel has changing values (not within the dead zone)

### Q: Fixture angle range is wrong?
Adjust the Pan Range and Tilt Range values in the Details panel to match the actual fixture specifications.

---

> **Next Steps**: Read [05 - DMX Camera](/docs/stage-core/dmx-camera) to learn how to control virtual cameras via DMX.
