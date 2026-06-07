# 10 - Stage Machinery

> **Module**: SuperStage Runtime (ASuperLiftingMachinery / ASuperRailMachinery)  
> **Target Users**: Stage designers, machinery control technicians  
> **Prerequisites**: [03 - DMX Fixture Base](/docs/stage-core/dmx-actor-base)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

SuperStage provides two DMX-controlled stage machinery Actors for simulating the movement of stage lifts, rotating platforms, rail cars, and other mechanical equipment in virtual scenes:

| Type | Class Name | Axis Count | Description |
|------|------|---------|------|
| **Lifting Machinery** | SuperLiftingMachinery | **6 axes** | 3-axis translation + 3-axis rotation, suitable for lift platforms, rotating stages, boom bars |
| **Rail Machinery** | SuperRailMachinery | **7 axes** | Movement along spline curve rail + 6-axis local offset/rotation, suitable for rail cars, hanging trolley |

Both inherit from SuperDmxActorBase with full DMX receiving capability.

### Class Inheritance Chain

```
AActor (UE5 Engine Base Class)
  │
  └── ASuperBaseActor ·············· SuperStage Root Class (L1)
        │                           ├ SceneBase / ForwardArrow / UpArrow
        │                           └ AssetMetaData
        │
        └── ASuperDmxActorBase ····· DMX Fixture Base Class (L2)
              │                      ├ DMX Address / Fixture Library / Channel Reading
              │                      └ SuperDMXTick / Address Label
              │
              ├── ASuperLiftingMachinery ·· Lifting Machinery (L3)
              │     ├ 6 DOF (3 translation + 3 rotation)
              │     ├ Boot Refresh initialization
              │     ├ Absolute rotation / Infinite rotation mode
              │     └ PosSpeed / RotSpeed interpolation smoothing
              │
              └── ASuperRailMachinery ···· Rail Machinery (L3)
                    ├ 7 DOF (rail position + 3 offset + 3 rotation)
                    ├ USplineComponent spline curve rail
                    ├ RailMountPoint mount point → OffsetComponent
                    ├ Closed loop + shortest arc interpolation
                    └ Orientation lock to rail tangent
```

> **Design Note**: Both machineries directly inherit `ASuperDmxActorBase` rather than `ASuperLightBase`, because they don't need the Pan/Tilt rotation axis structure. They use their own multi-axis free motion system, more suitable for stage machinery control requirements.

### Use Cases

- **Lift Platform** — Stage floor up/down lifting (Z-axis translation)
- **Rotating Stage** — Entire stage area horizontal rotation (Yaw infinite rotation)
- **Boom Bar / Light Rack** — Up/down lift + forward/backward movement
- **Mechanical Arm** — Multi-axis coordinated motion
- **Rail Car** — Light/camera trolley moving along a preset path
- **Hanging Trolley** — Fixture mount point moving along Truss rails
- **Circular Rail** — Cyclic motion on a closed loop

---

# Part 1: Lifting Machinery (SuperLiftingMachinery)

## 2. Basic Principles

Lifting machinery provides **6 degrees of freedom**:

```
3-Axis Translation               3-Axis Rotation
┌─────────────┐               ┌─────────────┐
│ PosX (L/R)  │               │ RotX (Pitch) │
│ PosY (F/B)  │               │ RotY (Yaw)   │
│ PosZ (U/D)  │               │ RotZ (Roll)  │
└─────────────┘               └─────────────┘
```

Each axis is controlled by a DMX attribute (0.0 ~ 1.0 normalized value), mapped to a preset motion range.

---

## 3. Property Details

### 3.1 Start Switch

| Parameter | Location | Description | Default |
|------|------|------|--------|
| **Start** | B.DefaultParameter | Machinery start switch. When set to True, begins responding to DMX signals | **False** |

> **Important**: After placing in the scene, you must set Start to **True** to begin movement. This is a safety design — preventing sudden machinery movement from misoperation.

### 3.2 Initialization Button

| Operation | Description |
|------|------|
| **Boot Refresh** | Click this button to auto-calculate motion range start/end points based on the current Actor position and rotation |

**BootRefresh Operations**:
1. Records current world position as **InitialPosition**
2. Calculates **EndPosition** = InitialPosition + MovingRange
3. Calculates **InitialRotation** = Current Rotation - RotRange × 0.5
4. Calculates **EndRotation** = Current Rotation + RotRange × 0.5
5. Synchronizes all cached values

> **Usage Steps**: First place machinery at the correct starting position → Set MovingRange/RotRange → Click BootRefresh.

### 3.3 Translation Parameters

| Parameter | Editable | Description | Default | Unit |
|------|--------|------|--------|------|
| **MovingRange** | ✅ | X/Y/Z three-axis displacement amount (offset from InitialPosition to EndPosition) | (0, 0, 0) | cm |
| **InitialPosition** | ❌ Read-only | Start point of translation range (BootRefresh calculated) | — | cm |
| **EndPosition** | ❌ Read-only | End point of translation range (BootRefresh calculated) | — | cm |

**Translation Mapping**:
```
Target Position = Lerp(InitialPosition, EndPosition, DMX Value)
```
- DMX Value = 0.0 → Actor at InitialPosition (starting position)
- DMX Value = 0.5 → Actor at midpoint between InitialPosition and EndPosition
- DMX Value = 1.0 → Actor at EndPosition (end position)

**Example**: Lift platform only moves up/down
- Place lift at lowest position
- Set MovingRange = (0, 0, 300) (Z-axis rises 300cm = 3 meters)
- Click BootRefresh
- DMX PosZ = 0.0 → lowest position; DMX PosZ = 1.0 → raised 3 meters

### 3.4 Rotation Parameters (Absolute Mode)

The following parameters are only shown when **PolarRotation = False** (absolute rotation mode):

| Parameter | Editable | Description | Default | Unit |
|------|--------|------|--------|------|
| **RotRange** | ✅ | Pitch/Yaw/Roll three-axis total rotation range | (360, 360, 360) | degrees |
| **InitialRotation** | ❌ Read-only | Start angle of rotation range (BootRefresh calculated) | — | degrees |
| **EndRotation** | ❌ Read-only | End angle of rotation range (BootRefresh calculated) | — | degrees |

**Rotation Mapping**:
```
Target Rotation = Lerp(InitialRotation, EndRotation, DMX Value)
```

### 3.5 Speed Parameters

| Parameter | Condition | Description | Range | Default |
|------|------|------|------|--------|
| **PosSpeed** | Always visible | Translation interpolation speed | 0.0 - 10.0 | 1.0 |
| **RotSpeed** | PolarRotation = False | Rotation interpolation speed (absolute mode) | 0.0 - 10.0 | 1.0 |
| **PolarRotationSpeed** | PolarRotation = True | Infinite rotation max speed | 0.0 - 10.0 | 1.0 |

**Parameter Tuning Suggestions**:

| Scenario | Recommended PosSpeed | Recommended RotSpeed | Description |
|------|--------------|--------------|------|
| Large lift platform (dignified slow) | 0.5 - 1.5 | 0.5 - 1.5 | Simulating hydraulic lift's steady motion |
| Standard boom lift | 1.0 - 3.0 | — | Light rack up/down, medium speed |
| Quick pop-up effect | 5.0 - 10.0 | — | Sudden pop-up visual effect (pyro/personnel) |
| Rotating stage (slow) | — | 0.3 - 1.0 | Slow rotation display |
| Rotating stage (quick switch) | — | 3.0 - 8.0 | Fast scene transition |
| Mechanical arm multi-axis | 2.0 - 5.0 | 2.0 - 5.0 | Multi-axis coordinated motion |

### 3.6 Infinite Rotation Mode

| Parameter | Description | Default |
|------|------|--------|
| **PolarRotation** | Infinite rotation mode toggle | **False** |

- **False (Absolute Mode)**: DMX value maps to a fixed angle range (InitialRotation ~ EndRotation)
- **True (Infinite Mode)**: DMX value controls rotation speed; machinery can **rotate continuously without stopping**

**Infinite Rotation DMX Mapping**:

| DMX Value (Normalized) | Rotation Behavior |
|-----------------|---------|
| 0.0 | Counter-clockwise at **max speed** |
| 0.5 | **Stop** rotation |
| 1.0 | Clockwise at **max speed** |
| 0.0 ~ 0.5 | Counter-clockwise, decreasing speed |
| 0.5 ~ 1.0 | Clockwise, increasing speed |

### 3.7 Control Parameters (Runtime Values)

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **PosX** | X-axis translation control | 0.0 - 1.0 | 0.5 |
| **PosY** | Y-axis translation control | 0.0 - 1.0 | 0.5 |
| **PosZ** | Z-axis translation control | 0.0 - 1.0 | 0.5 |
| **RotX** | Pitch rotation control | 0.0 - 1.0 | 0.5 |
| **RotY** | Yaw rotation control | 0.0 - 1.0 | 0.5 |
| **RotZ** | Roll rotation control | 0.0 - 1.0 | 0.5 |

### 3.8 Cache Information (Read-Only)

| Parameter | Description | Unit |
|------|------|------|
| **CurrentPosX/Y/Z** | Current actual world position (after interpolation smoothing) | cm |
| **CurrentRotX/Y/Z** | Current actual rotation angle (after interpolation smoothing) | degrees |

---

## 4. Blueprint Control

### 4.1 LiftingMachinery Function

Main control function for lifting machinery, called in the blueprint's **SuperDMXTick** event:

| Input Parameter | Description |
|---------|------|
| **DmxXPos** | DMX attribute for X-axis translation |
| **DmxYPos** | DMX attribute for Y-axis translation |
| **DmxZPos** | DMX attribute for Z-axis translation |
| **DmxXRot** | DMX attribute for Pitch rotation |
| **DmxYRot** | DMX attribute for Yaw rotation |
| **DmxZRot** | DMX attribute for Roll rotation |

### 4.2 Typical Blueprint

```
Event SuperDMXTick(DeltaTime)
  │
  └─ LiftingMachinery(PosX, PosY, PosZ, RotX, RotY, RotZ)
```

---

## 5. Usage Steps Summary (Lifting Machinery)

1. **Place Machinery** — Drag SuperLiftingMachinery blueprint into the scene at the starting position
2. **Set Translation Range** — MovingRange, e.g., (0, 0, 300) for Z-axis lifting 3 meters
3. **Set Rotation Range** — RotRange (or enable PolarRotation for infinite rotation)
4. **Set Speed** — PosSpeed / RotSpeed / PolarRotationSpeed
5. **Click BootRefresh** — Auto-calculate start/end ranges
6. **Configure DMX** — Set Universe, Start Address, Fixture Library
7. **Enable** — Start = True
8. **Control from Console** — Push corresponding channel faders

---

# Part 2: Rail Machinery (SuperRailMachinery)

## 6. Basic Principles

Rail machinery adds a **rail movement axis** on top of lifting machinery, totaling **7 DOF**:

```
Rail Layer (Level 1)              Offset Layer (Level 2)
┌─────────────────┐               ┌─────────────────┐
│ RailPos          │               │ PosX / PosY / PosZ│
│ Move along spline │      +        │ RotX / RotY / RotZ│
│                  │               │ In mount point local│
└────────┬────────┘               └────────┬────────┘
         │                                 │
    Move along rail path               Apply local offset/rotation
    (global motion)                    on top of rail position
```

**Component Hierarchy**:
```
Actor Root
  └── RailSpline (spline curve — defines rail path)
        └── RailMountPoint (mount point — moves along spline)
              └── OffsetComponent (offset component — 6-axis local transform)
                    └── Child Actor/Light/Camera (mounted here)
```

## 7. Rail Components

### 7.1 RailSpline

| Parameter | Description |
|------|------|
| **Type** | USplineComponent |
| **Location** | A.RailComponents group |
| **Editing Method** | Select Actor in scene viewport → Select spline component → Drag control points |

### 7.2 RailMountPoint

| Parameter | Description |
|------|------|
| **Type** | USceneComponent |
| **Location** | A.RailComponents group |
| **Behavior** | Automatically moves along spline curve, position determined by RailPos DMX value |

### 7.3 OffsetComponent

| Parameter | Description |
|------|------|
| **Type** | USceneComponent |
| **Location** | A.RailComponents group |
| **Behavior** | Performs 6-axis offset/rotation in the mount point's local coordinate system |

> **Important**: Any child Actor (fixtures, cameras, etc.) that needs to follow rail motion should be **mounted to OffsetComponent**, not the Actor root component.

---

## 8. Property Details

### 8.1 Start & Initialization

| Parameter | Description | Default |
|------|------|--------|
| **Start** | Start switch | False |
| **Boot Refresh** | Initialize range calculation (button) | — |

### 8.2 Rail Parameters

| Parameter | Description | Default |
|------|------|--------|
| **LockOrientationToRail** | Lock orientation to rail tangent direction | False |
| **ClosedLoop** | Whether the rail is closed (ends connected) | False |

### 8.3 Offset Parameters

| Parameter | Editable | Description | Default | Unit |
|------|--------|------|--------|------|
| **OffsetRange** | ✅ | Offset range in mount point local coordinates | (0, 0, 0) | cm |
| **InitialOffset** | ❌ Read-only | Start value of offset range | — | cm |
| **EndOffset** | ❌ Read-only | End value of offset range | — | cm |

### 8.4 Speed Parameters

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **RailSpeed** | Rail movement interpolation speed | 0.0 - 10.0 | 1.0 |
| **OffsetSpeed** | Offset movement interpolation speed | 0.0 - 10.0 | 1.0 |
| **RotSpeed** | Rotation interpolation speed (absolute mode) | 0.0 - 10.0 | 1.0 |
| **PolarRotationSpeed** | Infinite rotation speed | 0.0 - 10.0 | 1.0 |

### 8.5 Control Parameters

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **RailPos** | Rail position (0=start, 1=end) | 0.0 - 1.0 | 0.0 |
| **PosX** | X-axis local offset | 0.0 - 1.0 | 0.5 |
| **PosY** | Y-axis local offset | 0.0 - 1.0 | 0.5 |
| **PosZ** | Z-axis local offset | 0.0 - 1.0 | 0.5 |
| **RotX** | Pitch rotation | 0.0 - 1.0 | 0.5 |
| **RotY** | Yaw rotation | 0.0 - 1.0 | 0.5 |
| **RotZ** | Roll rotation | 0.0 - 1.0 | 0.5 |

---

## 9. Blueprint Control

### 9.1 RailMachinery Function (7-axis full control)

| Input Parameter | Description |
|---------|------|
| **DmxRailPos** | DMX attribute for rail position |
| **DmxXPos** | DMX attribute for X-axis offset |
| **DmxYPos** | DMX attribute for Y-axis offset |
| **DmxZPos** | DMX attribute for Z-axis offset |
| **DmxXRot** | DMX attribute for Pitch rotation |
| **DmxYRot** | DMX attribute for Yaw rotation |
| **DmxZRot** | DMX attribute for Roll rotation |

### 9.2 RailPositionOnly Function (rail position only)

| Input Parameter | Description |
|---------|------|
| **DmxRailPos** | DMX attribute for rail position |

### 9.3 Typical Blueprint

```
Event SuperDMXTick(DeltaTime)
  │
  └─ RailMachinery(RailPos, PosX, PosY, PosZ, RotX, RotY, RotZ)
```

Or simplified:

```
Event SuperDMXTick(DeltaTime)
  │
  └─ RailPositionOnly(RailPos)
```

---

## 10. DMX Channel Planning Suggestions

### Lifting Machinery (6 axes)

| Channel | Attribute | Precision | Description |
|------|------|------|------|
| 1-2 | PosX | 16-bit | X-axis translation |
| 3-4 | PosY | 16-bit | Y-axis translation |
| 5-6 | PosZ | 16-bit | Z-axis translation |
| 7-8 | RotX | 16-bit | Pitch rotation |
| 9-10 | RotY | 16-bit | Yaw rotation |
| 11-12 | RotZ | 16-bit | Roll rotation |

### Rail Machinery (7 axes)

| Channel | Attribute | Precision | Description |
|------|------|------|------|
| 1-2 | RailPos | 16-bit | Rail position |
| 3-4 | PosX | 16-bit | X offset |
| 5-6 | PosY | 16-bit | Y offset |
| 7-8 | PosZ | 16-bit | Z offset |
| 9-10 | RotX | 16-bit | Pitch |
| 11-12 | RotY | 16-bit | Yaw |
| 13-14 | RotZ | 16-bit | Roll |

> **Tip**: If not all axes are needed, define only the required attributes in the fixture library to save channels.

---

## 11. FAQ

### Q: Machinery won't move?
1. Check if **Start** is True
2. Check if **BootRefresh** was clicked
3. Confirm DMX network configuration is correct
4. Confirm corresponding attributes are defined in the fixture library

### Q: Movement direction reversed?
Adjust the positive/negative sign of the corresponding axis in MovingRange / OffsetRange. For example, changing (0, 0, 300) to (0, 0, -300) reverses the Z-axis direction.

### Q: Movement jittery?
Lower PosSpeed / RotSpeed / RailSpeed values (makes interpolation smoother), or use 16-bit precision DMX attributes.

### Q: Rail machinery's child Actor not following?
Confirm the child Actor is mounted to **OffsetComponent** rather than the Actor root component. OffsetComponent is the default attachment point returned by GetDefaultAttachComponent.

---

> **Next Steps**: Read [11 - Laser System](/docs/stage-core/laser-system) to learn about Beyond laser visualization.
