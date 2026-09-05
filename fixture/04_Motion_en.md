# Fixture Motion

> Applies to SuperStage 26H2.6 and later ｜ Read first: [The Fixture Definition Asset](01_FixtureDefinition_en.md)

This document explains how a data-driven fixture moves: where pan/tilt travel comes from, how the speed channel is interpreted, what continuous rotation runs at, and how multi-head fixtures move each head independently.

Whether a fixture moves at all comes down to two things: **the axis exists and is enabled in the rig**, and **a binding connects a channel to that axis' capability**. Miss either and the console moves the fader with nothing happening.

---

## 1. Axes

A fixture definition's rig carries up to three axes:

| Axis | Usually | Capability |
| --- | --- | --- |
| Axis A | Yoke / pan | Axis A Position (Pan) |
| Axis B | Head / tilt | Axis B Position (Tilt) |
| Axis C | Head spin | Axis C Position (Spin) |

Motion-related fields on each axis:

| Field | Default | Description |
| --- | --- | --- |
| **Enabled** | — | Whether the axis exists. Off means it does not move at all |
| **Rotation Axis** | — | Which local axis it turns about |
| **Pivot Offset (cm)** | — | Pivot offset relative to the parent. It decides which point the head turns around |
| **Range (deg)** | (−270, 270) | Mechanical travel |
| **Invert** | Off | Reverse |
| **Max Speed (deg/s)** | 0 | Maximum angular speed when moving to a target angle |

### Range

**Imported from the GDTF package declaration.** When the package does not state it, it falls back by axis role: pan ±270°, tilt ±135°.

> Before 26H2.6 this was never imported from GDTF, which made tilt travel wrong on the vast majority of fixtures. If a fixture's tilt travel looks wrong, first check whether Range holds the real value from the package.

The normalised DMX value 0 to 1 interpolates within Range, so with Range at (−270, 270), 0.5 is the centre at 0°. Change it to an asymmetric range and the centre moves with it.

### Max Speed

This is the maximum angular speed for moving *to an angle*.

| Value | Behaviour |
| --- | --- |
| **0 (default)** | No rate limit; snaps straight to the target |
| **> 0** | Approaches the target angle at this angular speed, advanced per frame |

> A library that does not declare a speed limit leaves this at 0, which matches the old behaviour. To make a fixture look like it is *travelling* rather than teleporting, give the axis a real angular speed.

---

## 2. The PT Speed Channel

With **Axis Speed (PT Speed)** bound, that channel scales the movement speed of all three main axes and every head axis.

| Channel value | Effect |
| --- | --- |
| 0 | Fastest (full speed) |
| 1 | Slowest |

**The slowest step is 8% of full speed.** Any slower and it becomes "I pushed the speed fader and the fixture stopped moving", which on site reads as a broken unit.

**With no such channel bound, movement runs at full speed** — the same as pushing the channel to 0.

The channel only means anything when the axis has `Max Speed > 0` — an unlimited axis is already instantaneous and has no speed to scale. It also **has no effect on continuous rotation** (§3.2).

> A fixture only steps its axes forward while something has not arrived yet; once everything is in place it stops — a stationary fixture costs nothing per frame.

---

## 3. Continuous Rotation

With **Axis A / B Infinite Rotation** bound, that channel makes the axis rotate continuously instead of moving to an angle.

These channels are conventionally **split into segments**: a short indexing segment first, then one segment each for forward and reverse rotation. One fixture's Pan Rotation, for example, reads `000-003 indexing / 004-129 forward / 130-255 reverse`.

Each segment's behaviour is decided by the channel library sub-attribute's **RotationMode**:

| RotationMode | Behaviour |
| --- | --- |
| **Off** | Hands the axis back to normal pan / tilt position control |
| **Position** | An indexing segment — see below |
| **Infinite** | Rotates continuously at the mapped speed |

> **`Stop` was removed in 26H2.6** and its meaning folded into `Off`. Stop used to mean "pause rotating and freeze the current angle", but with the fader parked in the stop segment the whole axis stopped responding to pan/tilt — not the intended behaviour. The 38 places in existing assets that stored Stop are redirected to Off automatically.

### 3.1 Position (the indexing segment)

**An indexing segment gives a relative amount, not an absolute angle.** The sub-attribute's physical quantity means "turn this many more degrees **from where you are**", and it is written into a separate offset field. The final goal angle = **the absolute angle from the pan / tilt channel + this offset**.

Keeping them apart is necessary: written into the same slot, the two channels would overwrite each other depending on binding order, which shows up as "push one and the other stops working". Stored separately, binding order does not matter.

**When the manufacturer leaves the indexing segment's physical quantity blank — which is common — the segment is treated as `Off`**, meaning "hand the axis back to the independent pan / tilt channels". It must not be hard-written as 0°: that would fight the pan channel for the same axis, showing up as the fixture being dragged back to the origin whenever pan is pushed.

### 3.2 Where the speed comes from

**The physical quantity declared in the library is used first.** GDTF declares the PhysicalUnit of `PanRotate` / `TiltRotate` as AngularSpeed, which is a genuinely usable unit (measured: Ayrton Mamba is ±240 deg/s).

**With no physical quantity declared, the normalized value maps to ±360 deg/s** — it still rotates.

**Continuous rotation is not limited by Max Speed** — that is the maximum speed for reaching an angle, a separate parameter.

The angle is wrapped into [−180°, 180°), so rotating for hours does not lose floating-point precision.

**Default values no longer rotate continuously**: a fixture placed in the scene with no DMX input has its gobo wheels and prism at rest.

---

## 4. Multi-Head Fixtures

Each emitter on a fixture can carry its own pair of head axes:

| Field | Description |
| --- | --- |
| Head Axis A (Pan) | This head's own pan axis |
| Head Axis B (Tilt) | This head's own tilt axis |

The field structure is identical to a main axis (Enabled / Rotation Axis / Pivot Offset / Range / Invert / Max Speed).

Head axes **share the PT speed channel's scaling factor** with the main axes but have their own travel and speed limits.

A binding's **Emitter Index** decides which emitter the channel acts on — that is how a multi-head fixture moves each head independently.

---

## 5. Troubleshooting

### The console pushes Pan and the fixture does not move

In order:

1. Is **Axis A's Enabled** on in the rig?
2. Does the current DMX mode's binding list contain an **Axis A Position** entry, and is its Attribute a name that actually exists in the channel library?
3. Is the scene fixture's `ControlMode` set to `DMX`?
4. Do the universe and start address line up?

Steps 1 and 2 can be checked at once on the **Validation** page of the [fixture editor](03_FixtureEditor_en.md); steps 3 and 4 are in [DMX Fixture Basics](../stagecore/03_DMX_Actor_Base_en.md).

### The travel is wrong / it overshoots

Check that axis' **Range**. It should hold the value declared in the GDTF package, not the ±270 fallback.

### The head turns around the wrong point

Check that axis' **Pivot Offset**. Use the **Rig** aid on the fixture editor's DMX Test page — it draws each axis' rotation line, pivot and travel arc on the model, which is the only reliable way to see whether a pivot sits on the mechanical joint.

### It teleports instead of travelling

That axis' **Max Speed** is 0 (no rate limit). Give it a real angular speed.

### Pushing the speed fader changes nothing

The speed channel only scales the process of reaching a target angle. An axis with Max Speed at 0 is already instantaneous and has no speed to scale.

### Continuous rotation does not rotate

1. Is there an **Axis A / B Infinite Rotation** binding?
2. Which segment is the channel value in? The indexing segment at the start does not rotate — push into the forward or reverse segment;
3. Is that segment's **RotationMode** set to `Infinite`? `Position` or `Off` will not rotate continuously.

A missing physical quantity is not the cause: that falls back to ±360 deg/s and still rotates.

### Pushing pan drags the fixture back to a fixed angle

The indexing segment of a continuous-rotation channel has its physical quantity hard-set to 0. A blank indexing segment should be treated as `Off` and hand the axis back to pan / tilt; set to 0 it becomes a relative "turn 0 more degrees" and fights pan for the same axis. Check that segment's physical range in the channel library.

---

## 6. Related Documents

- [The Fixture Definition Asset](01_FixtureDefinition_en.md) — the full rig and binding fields
- [Fixture Editor](03_FixtureEditor_en.md) — the Validation and DMX Test pages
- [DMX Fixture Basics](../stagecore/03_DMX_Actor_Base_en.md)
- [In-House Fixtures](../stagecore/17_InHouseFixtures_en.md) — the 16 C++ fixtures move according to their own implementations, not the model described here
