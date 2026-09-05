# Laser Effect System

> Applies to SuperStage 26H2.6 and later ｜ Read first: [Laser Animation Asset](01_LaserAnimationAsset_en.md)

Effects attach to a layer and form an **effect stack**, evaluated in order. The categories follow the conventions of professional laser software, so anyone arriving from another tool does not have to relearn a new scheme.

---

## 1. Six Categories

| Category | Description |
| --- | --- |
| **Oscillator** | Continuous oscillation: the target moves between a start and a finish value along a waveform |
| **Keyframes** | Each effect carries its own local timeline, with a draggable curve editor |
| **Color** | Colour stop line × application scope × blend mode |
| **Points** | Point-level operations. Laser-only; video software has no equivalent |
| **Filter** | Cleanup and optimisation |
| **Device Chase** | Steps the content across projection zones, with fades between steps |

> Keyframes do not contradict "no fixed duration": keyframes live inside a single effect's own period, and the asset as a whole still has no total duration.

---

## 2. Time Base

| Time base | Description |
| --- | --- |
| **Seconds** | For effects that should not drift with tempo |
| **Beats** | Driven by the asset tempo. Use this when the show follows music |

Offering only hertz would force the user to work out the BPM by hand, so both are provided.

---

## 3. Targets

Oscillator and Keyframes can drive 17 targets:

| Target | Description |
| --- | --- |
| **Position** | Position as **one two-axis value**. Use it to key a motion path — driving X and Y as two separate effects means two key lists you have to line up by hand, and they drift apart the moment you move one key |
| **Position X** / **Position Y** | Single-axis position |
| **Rotation** | Rotation |
| **Scale** | Both axes at once |
| **Scale (X/Y)** | Scale as one two-axis value, so **a single set of keys can squash and stretch** |
| **Scale X** / **Scale Y** | Single-axis scale |
| **Skew X** / **Skew Y** | Shear |
| **Wave** | Pushes points **sideways off the path**. With `Along Path` distribution this is the classic travelling wave |
| **Ripple** | Pushes points **along the radius** from the centre. Ripple and breathing |
| **Twist** | Rotates each point by an amount that grows along the distribution — twists the shape |
| **Brightness** | Brightness |
| **Visibility** | Blanks points whose signal falls below the amount. **Draw-on, dashes and strobe are all this** |
| **Hue Shift** | Shifts hue on top of whatever colour the point already has |
| **Saturation** | Saturation |

---

## 4. Waveforms and Run Modes

### 4.1 The Thirteen Waveforms

| Waveform | Description |
| --- | --- |
| **Sine** | Sine |
| **Ping-Pong** | Triangle |
| **Square** | Hard on/off. **This is what makes a strobe** |
| **Linear** | A ramp that snaps back. **Continuous rotation and scrolling gradients want this** |
| **Linear (Reverse)** | A ramp that runs backwards |
| **Accelerate** | Starts slow, ends fast |
| **Decelerate** | Starts fast, ends slow |
| **Ping-Pong (Smooth)** | Runs to the finish then back rather than snapping |
| **Discrete Steps** | Jumps between a fixed number of levels instead of sweeping |
| **Random** | A new random level **once per period**, held until the next one. It does not jitter every frame |
| **Bounce** | Overshoots the finish and settles back in decreasing hops, like something dropped |
| **Random Positions** | Jumps to a random one of **a fixed number of positions** each cycle. Unlike Random, which can land anywhere — snapping to a handful of positions is what makes random beam movement read as deliberate rather than noisy |
| **Custom Waveform** | A shape you draw yourself on the effect's own curve editor |

The waveform dropdown lists **2 / 4 / 8 / 16 steps** and **2 / 3 / 4 / 8 / 16 random landing points** directly, with nothing to configure.

### 4.2 The Three Path Shapes

**These only mean anything for two-axis targets** (Position, Scale (X/Y)):

| Path | Description |
| --- | --- |
| **Linear** | Both axes move together along one direction; the angle aims it |
| **Circle** | The two axes run **a quarter cycle apart**, so the shape travels a circle |
| **Figure 8** | One axis runs at twice the rate of the other, tracing a figure eight |

> One waveform is not enough for a two-axis target: a circle needs the two axes a quarter cycle apart. Splitting that into two effects and aligning the phase by hand is the easiest thing to get wrong and the hardest to spot, so the path shape is offered directly.

---

## 5. Distribution

How an effect spreads across one frame of content:

| Distribution | Description |
| --- | --- |
| **Whole Frame** | Every point gets the same value; the whole frame moves together |
| **Along Path** | By position along the scan path, first point to last |
| **Radius** | By distance from the centre |
| **Angle** | By angle around the centre, a full turn from 0 to 1 |
| **By X** / **By Y** | By coordinate |
| **Symmetrical X** / **Symmetrical Y** | By distance from the centre along that axis, mirrored either side |
| **Random** | A **fixed** per-point noise value. It stays put frame to frame instead of boiling |

> **`Along Path` is the laser-native one**: a beam is physically a one-dimensional path, which is what makes a wave travel, a gradient stretch, and a draw-on sweep possible.

---

## 6. Colour Effects

**Colours are no longer rainbow across the board.**

Application scope (8): By Points, By X, By Y, By Radius, By Angle, Symmetrical X, Symmetrical Y, By Time

Blend modes (5):

| Mode | Description |
| --- | --- |
| Replace | Replace outright |
| Hue + Saturation | Change hue and saturation |
| Hue Shift | Offset from the original colour |
| **Brightness Only** | Leaves hue completely alone — **the only mode usable on a single-colour laser** |
| Multiply | Multiply |

Other parameters:

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| **Stops** | — | — | The colour stops making up the line. **Two stops with Discrete off is a simple two-colour blend** |
| **Discrete** | off | — | Snap to the nearest stop instead of blending, giving **flat bands of colour** rather than a gradient |
| **Repeats** | 1 | 1–32 | How many times the colour line repeats across the frame |
| **Speed** | **0** | −8–8 | Scroll speed of the colour line, in line-lengths per period. **0 holds it still** |

So colour is no longer always a full rainbow — for two or three colours, place two or three stops.

> Hue-to-RGB conversion is continuous rather than 8-bit quantised, so **scrolling does not produce stepped banding**.

---

## 7. Points (point-level operations)

These change **the composition of the point stream itself**, not the geometry and not the colour. **A beam show lives on them** — the mirrors park in one place and output the same point over and over, which is what makes the beam bright enough.

| Operation | Description |
| --- | --- |
| **Point Repeat** | Outputs every point N times. The beam dwells longer so it burns brighter — **at N times the points**. If the multiplied count exceeds the internal ceiling the operation is skipped entirely, with no warning |
| **Anchors** | Keeps one bright point every N and blanks the rest. **Turns a solid line into a row of beams** |
| **Anchor Count** | Reduces the whole frame to **exactly N** evenly spaced beams, whatever its length |
| **Beat Dots** | Takes N evenly spaced positions along the layer's whole point stream, dwells on each for a number of samples and drops the rest. **Turns graphics into a beam show** |

Two shared parameters:

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| **Count** | 8 | 1–256 | Repeat factor, spacing, or anchor count depending on the operation |
| **Brightness** | 1.0 | 0–1 | Applied to the points the operation keeps. **Beat Dots is the exception** — there it sets how many samples each beam dwells for (roughly 2–24) and colour is left untouched |

> Point-level operations spend the **point budget**. Repeating points makes the same arc consume more samples, and the frame rate drops accordingly — check the frame rate verdict on the scanner panel afterwards.

---

## 8. Filter (cleanup)

These **do not animate**; they change how the frame is drawn.

| Operation | Description |
| --- | --- |
| **Even Spacing** | Resamples to even spacing along the path. **Even spacing is even brightness** |
| **Reduce Points** | Thins the points out. Buys frame rate at the cost of a rougher path |
| **Soft Line Ends** | Fades the first and last points of every visible stroke so line ends stop flaring |
| **Trim Blanking** | Collapses long blanked jumps to a single move. **Recovers points wasted flying across the field** |
| **Smooth Path** | Averages each point against its neighbours to take jitter out of the path. Traced logos and old ILDA libraries are full of stair-stepping the mirrors have to chase; smoothing it **costs nothing in points** and takes load off the scanners |

One shared parameter, **Amount** (default 4.0, range 0–64), whose meaning depends on the operation:

| Operation | What Amount means |
| --- | --- |
| Even Spacing | **Point-count multiplier**. The default 4.0 resamples to four times the original point count |
| Reduce Points | Reduction factor |
| Soft Line Ends | Fade length |
| Smooth Path | Number of smoothing passes (1–8) |
| Trim Blanking | Blank-run length threshold (2–64 samples); anything longer collapses into one move |

---

## 9. Device Chase

Steps the content across several laser devices. When one animation drives a whole rig, this is what gives you a chase, a call-and-response between sides, or devices lighting one at a time.

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| **Steps** | empty | — | One step per position, each naming the devices lit at that moment (1-based). **An empty step goes dark** — that is how you get gaps in a chase |
| **Fade** | 0 | 0–0.5 | Share of each step spent fading in and out. 0 cuts hard |

Raise `Fade` and the content **hands over** between devices rather than jumping, which is what makes a sweep across a rig read as one movement.

> **Fading costs frame rate**: a layer that is fading out still takes its share of the point budget, so the frame rate dips a little while two devices overlap. A hard cut is free.

---

## 10. Parameter Exposure

**Scope and Strength appear only on the effects that actually use them**, rather than on all effects. Each effect has its own centre point, and the effect chain shows a summary so individual effects need not be expanded to see what they do.

---

## 11. Related Documents

- [Laser Animation Asset](01_LaserAnimationAsset_en.md)
- [Canvas Editor](02_CanvasEditor_en.md)
- [Baking and ILDA](04_Baking_and_ILDA_en.md)
