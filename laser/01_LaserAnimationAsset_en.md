# Laser Animation Asset

> Applies to SuperStage 26H2.6 and later ｜ Read first: [Laser Overview](00_Laser_Overview_en.md)

The laser animation asset is the laser content authoring vehicle added in 26H2.6. Create it by right-clicking in the Content Browser and open it by double-click.

---

## 1. Two Design Premises

### Vectors are stored, not point clouds

The asset holds **Bezier outlines and modulation parameters**; the point stream is computed at playback. That gives three things:

- **Resolution independence** — the same content is correct on a 20 kpps and a 40 kpps scanner;
- **Editable at any time** — the points are not baked in; move one control point and the whole piece follows;
- **No rework when scanner parameters change.**

### There is no fixed duration

Motion comes from the **frequency** of the effects, and the periods of different effects are unrelated. Any point in time can be evaluated directly, so scrubbing, jumping and reverse playback need no warm-up, and "how long is this animation" is not a question that applies.

---

## 2. Asset Structure

```text
Laser animation asset
 ├── Tempo                BPM, default 120 (20~300). The basis when effects run on beats
 ├── Scan Profile         Scanner specification (section 4)
 ├── Preview Device IDs   Which devices the preview goes to (1-based, matching each device's ID)
 ├── Device Time Shift    Time offset between devices, seconds (−5~5)
 ├── Export Frame Count   Frames to bake, default 1 (1~9999)
 ├── Export Frame Rate    Export frame rate, default 30 (1~120)
 ├── Export Device ID     Which device's view to write, 0 = every layer ignoring routing
 └── Layers[]             Layer stack, **index 0 is the bottom layer**
      ├── Content source (one of six)
      ├── Transform / Color / ScanMode
      └── Effects[]       Effect stack, evaluated in order
```

**All coordinates are in scanner normalized space `[-1, 1]`, with Y positive downward** (matching UE's screen convention).

**Layers are a single level and cannot nest.** Two levels would mean two sets of transform, colour and effect stack, and two sets of add/remove buttons in the UI, with no way to tell which one to press.

> **Why the export settings live on the asset**: the asset has no total duration (every effect loops on its own), so "how long to export" cannot be derived from the content — anything derived would be wrong. It has to be stated by a person. The export dialog starts from these values and writes back what you confirm, so the setting carries over.

---

## 3. Layers

### 3.1 Content Source

Each layer's content comes from one of six sources:

| Source | Description |
| --- | --- |
| **Path** | Hand-drawn / edited Bezier path |
| **Primitive** | Built-in primitives, 16 of them: Circle, Ellipse, Rectangle, Polygon, Star, Line, Arc, Spiral, Lissajous, Wave, Grid, Cross, Heart, Rose, Beam Dot, Beam Fan |
| **Text** | Text. Bezier curves are taken directly from font outlines, with no bitmap in between |
| **Imported** | Imported vector art (SVG) |
| **Mesh** | Outline taken from a mesh |
| **ILDA Sequence** | An imported ILDA frame sequence |

### 3.2 Scan Mode

**This is the most easily missed field on a layer, and it decides how the layer reads.** It is a **content choice** — is this layer drawn as lines, or parked as a set of beams — not a scanner setting, which is why it lives on the layer rather than in the scan profile.

| Mode | Meaning |
| --- | --- |
| **Line** | Lines. The default |
| **Beam** | Beams. Takes a number of beams along the path at **equal arc length**, each dwelling for a number of samples |
| **Hybrid** | Both |

The two parameters below only apply in `Beam` and `Hybrid`:

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| **Beam Count** | 12 | 1–64 | How many beams to take along the path |
| **Beam Dwell** | 8 | 1–60 | Samples each beam dwells for. **The baker currently uses a fixed 8 samples per beam, so changing this value does not change the baked result** |

> This is **not the same thing** as the `Beam Dot` / `Beam Fan` primitives in the content source: those are shapes, this is a scanning method. Any shape can be switched to beam mode.

### 3.3 Other Layer Parameters

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| **Name** | `Layer` | — | Layer name |
| **Visible** | on | — | Whether it takes part in evaluation |
| **Locked** | off | — | The node tool stops editing its path and arrow-key nudging skips it. **The Select tool can still pick it up and drag it** — what is locked is the path, not the position |
| **Curve Detail** | 256 | 4–256 | How finely curves are broken into straight segments. **Only affects sources that actually contain curves** — text, rounded shapes, imported vector art. Lower spends fewer points; too low and curves visibly go faceted |
| **Transform** | — | — | See 3.4 |
| **Color** | — | — | The layer's solid base colour. Actual colour movement is the job of colour effects |
| **Effects** | empty | — | Effect stack, evaluated in order — see [Effect System](03_Effects_en.md) |

Layers can be copied and pasted across assets — the clipboard uses Base64 encoding, so multi-line text layers are not cut in half.

### 3.4 Transform

Applied in the same order as Illustrator: **scale → shear → rotate → translate**.

| Parameter | Default | Description |
| --- | --- | --- |
| **Location** | (0, 0) | Translation |
| **Rotation** | 0 | Degrees. **Positive is clockwise** (screen-space Y is down, so it reads the same as Illustrator) |
| **Scale** | (1, 1) | Scale |
| **Shear** | (0, 0) | Degrees. X shear / Y shear |
| **Pivot** | (0, 0) | Anchor for rotation and scale, in the shape's local space |

> With a zero scale, or a degenerate shear, the transform is not invertible and dragging nodes on the canvas stops working — that is the mathematics, not a bug.

### 3.5 Primitive Parameters

What `Param A` / `Param B` mean **depends on the primitive type**, and the UI relabels them accordingly:

| Primitive | Param A | Param B |
| --- | --- | --- |
| Polygon | Sides (3~64) | — |
| Star | Points (3~32) | Inner radius ratio (0~1) |
| Rectangle | Corner radius (0~1; at 1 it becomes a capsule) | — |
| Arc | Start angle (deg) | Sweep (deg) |
| Spiral | Turns | Inner radius ratio |
| Lissajous | X frequency | Y frequency |
| Wave | Cycles | Amplitude |
| Grid | Columns | Rows |
| Rose | k | unused |
| Beam Fan | Beam count (1~64) | Spread (deg) |

Two more: **Size** (default 0.5 × 0.5) and **Resolution** (sampling density for the generated outline, **0 = derived from point spacing**, max 2048).

> **Changing the primitive type re-applies sensible defaults to Param A / B.** The same number means completely different things per type — 5 on a star is "five-pointed", on a rectangle it is a corner-radius ratio clamped to 1.0, which gives you a capsule. Values you adjusted yourself are left alone as long as the type does not change.

### 3.6 Text Parameters

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| **Text** | `LASER` | — | The string |
| **Font** | empty | — | Font asset. Empty uses the project default font |
| **Size** | 0.4 | 0.01–2.0 | Height in normalized space |
| **Tracking** | 0 | −0.5–2.0 | Letter spacing, as a fraction of size |
| **Line Height** | 1.2 | 0.1–4.0 | Line spacing, as a fraction of size |
| **Align** | Center | — | Left / Center / Right |

---

## 4. Scan Profile

**The point budget is the core**: a frame gets this many samples, and all content divides them by arc length.

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| Point Count | 600 | 24–6000 | Samples per frame. This is the budget; point repeats and mirrors in the effect chain spend it too |
| Point Rate | 30 kpps | 5–80 | Sample clock, thousand points per second |
| Corner Dwell | 4 | 0–16 | Samples repeated at corners, scaled by angle/90. Without them corners are drawn round — galvos have mass and cannot turn instantly |
| Blank Lead In | 3 | 0–24 | Samples blanked before a jump |
| Blank Lead Out | 4 | 0–24 | Samples waited after a jump before lighting again, letting the galvo settle |
| Color Shift | 0 | −12–12 | RGB sample offset relative to XY, compensating the colour smear caused by modulation lag |
| Optimize Path Order | on | — | Reorders strokes to shorten blanked travel. **Blanked travel costs samples too**, and a bad order collapses the frame rate |

> **The frame rate is not a setting, it is derived**: `frame rate = point rate ÷ point count`. Below roughly 22 Hz fast movement shows flicker; below 15 Hz it is obviously flickering. The interface states the verdict directly — no need to work it out.

---

## 5. Multiple Devices

One animation can be sent to several devices.

- **Preview Device IDs**: which devices the preview goes to;
- **Device Time Shift**: time offset between devices, for staggered movement;
- **Sequencer bakes per device**: one track per device.

---

## 6. Transport

The strip under the canvas is a **transport bar**, not a timeline: play / pause, stop-and-rewind, a free-running clock readout, plus the tools, the device dropdown, the frame rate and the Grid / Snap / Blanking / Points display toggles.

**There is no scrubber and no timeline.** Content of this kind has no "total length" to begin with: every effect runs on its own period and the periods are unrelated, so a bar dragged from 0 to "the end" would mean nothing. To place content to the second, use Sequencer — see the next section.

---

## 7. Using It in a Scene

1. Place a laser device object in the scene;
2. Reference this animation asset on the device;
3. Or add a laser track in Sequencer and bake per device.

---

## 8. Related Documents

- [Laser Overview](00_Laser_Overview_en.md)
- [Canvas Editor](02_CanvasEditor_en.md)
- [Effect System](03_Effects_en.md)
- [Baking and ILDA](04_Baking_and_ILDA_en.md)
