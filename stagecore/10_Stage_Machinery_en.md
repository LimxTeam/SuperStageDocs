# 10 - Stage Machinery

> **Module**: SuperAssets  
> **Target Users**: Stage designers, machinery control technicians  
> **Prerequisites**: [03 - DMX Fixture Base](./03_DMX_Actor_Base_en.md)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

SuperStage currently provides two DMX-controlled stage machinery Actors:

| Type | Display Name | Default Library Tag | Purpose |
|------|--------------|---------------------|---------|
| Lifting Machinery | `SuperLiftingMachinery` | `12CH` | Controls a platform, bar, or rotating structure with XYZ translation and XYZ rotation |
| Rail Machinery | `SuperRailMachinery` | `14CH` | Moves along a spline rail and adds local offset/rotation at the mount point |

Both use SuperDMX fixture settings, so they need Universe, Start Address, Fixture Library, and ControlMode settings. Motion works only when the fixture library attributes match the expected names.

---

## 2. Lifting Machinery

Lifting Machinery moves and rotates the Actor itself. `BeginPlay` calls **BootRefresh** once at runtime, and you can also click it manually in the editor.

### Main Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| **Start** | False | When False, DMX motion is ignored |
| **MovingRange** | (0, 0, 0) | End-position offset from the start position, in centimeters |
| **InitialPosition** | Auto | Start position captured by BootRefresh |
| **EndPosition** | Auto | `InitialPosition + MovingRange` |
| **RotRange** | (360, 360, 360) | Total rotation range in absolute rotation mode |
| **InitialRotation** | Auto | Current rotation - `RotRange * 0.5` |
| **EndRotation** | Auto | Current rotation + `RotRange * 0.5` |
| **PositionInterpolation** | False | Enables interpolation for position |
| **RotationInterpolation** | False | Enables interpolation for rotation in absolute mode |
| **PosSpeed** | 1.0 | Visible only when PositionInterpolation is enabled, range 0-10 |
| **RotSpeed** | 1.0 | Visible only when RotationInterpolation is enabled and PolarRotation is off, range 0-10 |
| **PolarRotation** | False | Enables continuous rotation mode |
| **PolarRotationSpeed** | 1.0 | Visible when PolarRotation is enabled, range 0-10 |

### DMX Attributes

In DMX control mode, Lifting Machinery reads these fixture library attributes as 16-bit Fine values:

| Attribute | Controls |
|-----------|----------|
| `XPos` | X position |
| `YPos` | Y position |
| `ZPos` | Z position |
| `XRot` | Pitch |
| `YRot` | Yaw |
| `ZRot` | Roll |

Position maps to `Lerp(InitialPosition, EndPosition, DMX value)`. Absolute rotation maps to `Lerp(InitialRotation, EndRotation, DMX value)`.

In continuous rotation mode, `XRot`/`YRot`/`ZRot` map to rotation speeds from `-PolarRotationSpeed` to `+PolarRotationSpeed` and accumulate local rotation using DeltaTime.

### Property Mode

**Drive lifting machinery in DMX mode.** Setting **ControlMode** to Property exposes the six manual controls in the **C.ControlParameter** group (PosX / PosY / PosZ, RotX / RotY / RotZ — the position three run 0~100, the rotation three −100~100), but **the current implementation does not move the device in Property mode**: the read-and-apply step returns immediately.

---

## 3. Rail Machinery

Rail Machinery contains three components:

| Component | Purpose |
|-----------|---------|
| **RailSpline** | Spline path that defines the rail shape |
| **RailMountPoint** | Mount point that moves along the spline |
| **OffsetComponent** | Adds local offset and rotation under the mount point; child Actors should normally attach here |

Rail Machinery also ticks in editor viewports so RailPos changes can be previewed outside Play mode.

### Main Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| **Start** | False | When False, DMX motion is ignored |
| **LockOrientationToRail** | False | Makes RailMountPoint orientation follow the spline tangent |
| **ClosedLoop** | False | Closes the spline rail |
| **OffsetRange** | (0, 0, 0) | Local offset range for OffsetComponent, in centimeters |
| **InitialOffset** | Auto | Calculated from the current OffsetComponent location by BootRefresh |
| **EndOffset** | Auto | Calculated from OffsetRange by BootRefresh |
| **RotRange** | (360, 360, 360) | Total rotation range in absolute mode |
| **PositionInterpolation** | False | Enables interpolation for rail position and offset |
| **RotationInterpolation** | False | Enables interpolation for rotation in absolute mode |
| **RailSpeed** | 1.0 | Visible only when PositionInterpolation is enabled |
| **OffsetSpeed** | 1.0 | Visible only when PositionInterpolation is enabled |
| **RotSpeed** | 1.0 | Visible only when RotationInterpolation is enabled and PolarRotation is off |
| **PolarRotation** | False | Makes OffsetComponent use continuous rotation |
| **PolarRotationSpeed** | 1.0 | Visible when PolarRotation is enabled |

When the rail is closed and position interpolation is enabled, the rail uses the shortest wrapped path between 0 and 1. For example, 0.9 to 0.1 crosses 1.0/0.0 instead of traveling the long way around.

### DMX Attributes

In DMX control mode, Rail Machinery reads these fixture library attributes as 16-bit Fine values:

| Attribute | Controls |
|-----------|----------|
| `Dimmer` | Rail position, equivalent to RailPos in the UI |
| `XPos` | OffsetComponent local X offset |
| `YPos` | OffsetComponent local Y offset |
| `ZPos` | OffsetComponent local Z offset |
| `XRot` | OffsetComponent Pitch |
| `YRot` | OffsetComponent Yaw |
| `ZRot` | OffsetComponent Roll |

> **Note**: The current implementation uses `Dimmer` as the rail-position attribute name, not `RailPos`. Configure custom fixture libraries accordingly.

---

## 4. Workflow

### Lifting Machinery

1. Place `SuperLiftingMachinery` in the level.
2. Set MovingRange and RotRange.
3. Click **BootRefresh**, or let runtime BeginPlay initialize it.
4. Configure DMX address and fixture library.
5. Set **Start** to True.
6. Send console data for `XPos/YPos/ZPos/XRot/YRot/ZRot`.

### Rail Machinery

1. Place `SuperRailMachinery` in the level.
2. Edit RailSpline control points.
3. Set ClosedLoop and LockOrientationToRail as needed.
4. Attach child Actors that should follow the rail to **OffsetComponent**.
5. Set OffsetRange and RotRange, then click **BootRefresh**.
6. Configure DMX address and fixture library.
7. Set **Start** to True.
8. Send console data for `Dimmer/XPos/YPos/ZPos/XRot/YRot/ZRot`.

---

## 5. FAQ

### Q: Machinery does not move.
Check that **Start** is True, ControlMode allows DMX reading, the fixture library contains the expected attribute names, and DMX input is reaching the configured Universe/Address.

### Q: PosSpeed or RotSpeed has no effect.
These speeds only apply when the matching **PositionInterpolation** or **RotationInterpolation** option is enabled. With interpolation off, the value jumps directly to the target.

### Q: Should the rail-position channel be named RailPos?
The current implementation reads `Dimmer` for rail position. The fixture library needs a `Dimmer` attribute for rail position to update from DMX.

### Q: Child Actor does not follow the rail.
Attach it to **OffsetComponent**. `SuperRailMachinery` returns OffsetComponent as its default attachment component.

### Q: RailPos previews in the editor, but Lifting Machinery does not.
Rail Machinery implements editor viewport Tick and construction/property updates. Lifting Machinery does not currently have the same editor-preview path and mainly applies motion at runtime or during DMX Tick.

---

> **Next Steps**: Read [12 - Lift Matrix](./12_Lift_Matrix_en.md).
