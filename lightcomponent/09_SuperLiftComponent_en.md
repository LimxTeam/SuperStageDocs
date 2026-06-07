# 09 - Lift Control Component (SuperLiftComponent)

> **Module**: SuperStage Runtime  
> **Target Audience**: Lighting designers, fixture Blueprint creators  
> **Prerequisite Reading**: [00 - Light Component Overview](00_LightComponent_Overview.md)

---

## 1. Overview

**SuperLiftComponent** is the simplest auxiliary light component in SuperStage, used to control the **Z-axis (vertical direction)** lift motion of fixtures. It simulates the electric lift mechanism of fixture rigging points in real stages, allowing real-time adjustment of fixture suspension height via DMX channels.

Unlike other light components, the lift component **involves no lighting or material rendering** — it is purely a motion control component that achieves lift effects by changing the component's relative position (Z-axis offset).

### Use Cases

- **Liftable Fixtures** — Fixtures suspended on motorized battens, needing real-time height adjustment
- **Lift Effects** — Rising/lowering actions of fixtures during performances
- **Dynamic Stages** — Applications requiring fixture height changes with scene variations

---

## 2. Component Structure

```
SuperLiftComponent (inherits from USceneComponent)
  └── (No sub-components)
```

> **Simplest Component**: The lift component creates no sub-components, loads no materials, and does not enable Tick — it only updates its own relative position when receiving lift commands.

---

## 3. Working Principle

### 3.1 Lift Motion Model

The lift component's motion is based on the following model:

```
Initial Position (Z=0)              Maximum Lift Position (Z=LiftRange)
     │                                          │
     ▼                                          ▼
     ┃══════════════════════════════════════════┃
     0.0              DMX Value                 1.0
```

The DMX channel value (0~1) is mapped to the fixture's lift range:
- **DMX = 0.0** → Fixture at lowest position (Z = 0)
- **DMX = 1.0** → Fixture at highest position (Z = LiftRange)

### 3.2 Smooth Interpolation

Lift motion uses **smooth interpolation** to avoid the fixture jumping instantaneously to the target position:

```
Current Position → Smooth Transition → Target Position
```

Interpolation calculation:
```
New Z Position = Current Z Position + (Target Z Position - Current Z Position) × Interpolation Speed × DeltaTime
```

Where:
- **Target Z Position** = DMX value × Lift Range (LiftRange)
- **Interpolation Speed** = Fixture-configured lift speed (LiftSpeed)
- **DeltaTime** = Frame interval time

> **Effect**: The fixture doesn't jump instantly to the target height but smoothly transitions at the set speed, simulating the motion characteristics of a real motor.

### 3.3 Value Clamping

DMX input values are automatically clamped to the [0, 1] range:
- Values < 0 are clamped to 0
- Values > 1 are clamped to 1

---

## 4. DMX-Controlled Parameters

### 4.1 Lift Position (Lift Z)

| Parameter | Description | Range |
|------|------|------|
| **LiftZ** | Normalized lift position | 0.0 (lowest) ~ 1.0 (highest) |

This is the **only control parameter** of the lift component.

| Input Value | Behavior |
|--------|------|
| 0.0 | Fixture descends to lowest position |
| 0.5 | Fixture at middle height of lift range |
| 1.0 | Fixture rises to highest position |

### 4.2 Associated Parameters (Provided by Fixture Actor)

The following parameters are not defined within the lift component but passed in by the fixture Actor when calling the lift component:

| Parameter | Description | Source |
|------|------|------|
| **LiftRange** | Lift range (maximum Z-axis displacement) | Fixture Actor's fixture library definition |
| **LiftSpeed** | Lift speed (interpolation speed coefficient) | Fixture Actor's fixture library definition |
| **DeltaTime** | Frame interval time | Automatically provided by engine |

---

## 5. Usage Guide

### 5.1 Basic Setup

1. Add SuperLiftComponent in the fixture Blueprint
2. Attach the lift component at the component hierarchy level that needs lifting
3. Configure the lift channel in the fixture library (mapped to LiftZ parameter)
4. Set lift range (LiftRange) and lift speed (LiftSpeed)

### 5.2 Component Hierarchy Position

The lift component should be placed **above the portion** of the fixture component hierarchy that needs lifting:

```
Fixture Actor
  └── Mounting Bracket (fixed, does not move)
        └── SuperLiftComponent ← Lift component
              └── Fixture Body (follows lift motion)
                    ├── Yoke
                    ├── Head
                    └── Light Source Components
```

This way, the lift component's Z-axis displacement is passed to all child components, achieving lift of the entire fixture body.

### 5.3 Performance Characteristics

| Feature | Description |
|------|------|
| **No Tick** | Component does not enable Tick; consumes no per-frame CPU resources |
| **Update on demand** | Only calculates position when receiving DMX lift commands |
| **Zero rendering overhead** | No materials or lighting involved; pure position transform |

> **Note**: While the component itself doesn't Tick, its position updates depend on external calls (SetLiftZ called by the fixture Actor in its Tick). This means lift smoothness depends on the fixture Actor's update frequency.

---

## 6. FAQ

### Q: Fixture noticeably jumps/is not smooth when lifting?
**A**: Check: 1) **LiftSpeed** is not set too high (larger values = faster interpolation, too large may cause jaggedness) 2) Fixture Actor's Tick frequency is sufficient (low frame rate = large interpolation steps, potentially less smooth) 3) DMX data update frequency is stable

### Q: Fixture lift range is wrong?
**A**: The LiftRange parameter is passed in by the fixture Actor. Check if the lift range configuration in the fixture library is correct. The lift component just multiplies the DMX value (0~1) by LiftRange to calculate the target position.

### Q: Fixture can't lift to the specified height?
**A**: Confirm the DMX value is correctly mapped to the 0~1 range. The lift component clamps input values to [0, 1]; values outside this range have no additional effect.

### Q: How to achieve an emergency stop for a fixture?
**A**: Set LiftSpeed to a very large value; the fixture will almost instantly reach the target position. But this is not recommended — real stage lift mechanisms have physical limits; an instant stop may not match realistic behavior.

### Q: Can the lift component control horizontal motion?
**A**: No. The lift component **only controls the Z axis** (vertical direction). For horizontal motion, use the fixture Actor's Pan/Tilt system or custom motion components.

### Q: Can multiple lift components be used in series?
**A**: Theoretically yes, but not recommended. Connecting multiple lift components in series causes displacement stacking, making control logic complex. It is recommended to use one lift component per fixture.

---

## 7. API Quick Reference

The following is the only public function signature of `USuperLiftComponent`:

| Function Signature | Description |
|----------|------|
| `void SetLiftZ(float InPosZ = 0.0f, float LiftRange = 100.0f, float LiftSpeed = 1.0f)` | Set lift position (InPosZ normalized 0~1, LiftRange max displacement cm, LiftSpeed interpolation speed) |
