# Baking and ILDA

> Applies to SuperStage 26H2.6 and later ｜ Read first: [Laser Animation Asset](01_LaserAnimationAsset_en.md)

---

## 1. The Baker

Baking turns vector content into a point stream a scanner can consume. It respects galvo physics rather than simply spreading points along arc length.

| Stage | What it does |
| --- | --- |
| Point budget | All layers divide the Point Count between them by arc length. **Corner dwell, point repeats and beam dwell are added on top of it**, so the real point count runs a little above the budget |
| Corner dwell | Dwell samples allocated to corners by severity (scaled by angle/90) |
| Blanking dwell | Samples blanked before a jump (Blank Lead In) and settling samples after it (Blank Lead Out) |
| Ordering optimisation | Reduces blanking travel |
| Colour deskew | Compensates colour channel delay relative to position (Color Shift) |

**The frame rate is a derived result**: `frame rate = point rate ÷ point count`. The interface states the verdict directly:

- Below roughly 22 Hz, fast movement shows flicker;
- Below 15 Hz, it is obviously flickering.

To raise the frame rate: reduce content, reduce point-level operations (point repeats are expensive), or raise the point rate — though the ceiling there is set by the real scanner's specification.

---

## 2. Previs Consistency

**Authored content and baked content go through the same processing chain**, so the two sources look consistent in previs. The scanner simulation on that chain is off by default; turn it on in the scan profile settings when you want it.

---

## 3. Sequencer

Laser animations bake per device in Sequencer and can be arranged together with UE timeline content (cameras, animation, audio).

---

## 4. ILDA

ILDA (IDTF) is **the only frame exchange format the laser industry has in common**. Without it you could use none of the existing content libraries, and nothing you made could be delivered to anyone else's console.

The implementation follows the **IDTF14 rev011** specification.

### 4.1 Supported Format Codes

| Format | Meaning |
| ---: | --- |
| **0** | 3D indexed colour |
| **1** | 2D indexed colour |
| **2** | Palette |
| **4** | 3D true colour |
| **5** | 2D true colour |

**An unknown section format stops the import there.** With an unknown record size the section cannot be skipped, and reading on would misparse everything after it. Frames already read are kept, and the import report says how many frames were read and which format code stopped it.

### 4.2 Reading

An imported frame sequence can be used as a layer content source (`ILDA Sequence`): **a single frame becomes an editable shape; an animation keeps every frame and loops at its own rate**.

The **name and company** from the section header are carried through as-is for provenance.

**Indexed-colour palettes**: if the file carries a Format 2 section, that palette is used; only without one does it fall back to the default table. The default table covers only the saturated colours from the specification's appendix — **indices it does not cover are reported honestly rather than being quietly given white**.

The import report gives: frames read, points read, **points whose palette index could not be resolved**, and unknown-format sections skipped.

### 4.3 Writing

| Option | Default | Description |
| --- | --- | --- |
| **Frames** | 1 | How many frames to bake. **The asset has no length of its own, so this has to be stated** — one frame gives a still, a sequence gives an animation |
| **Frame Rate** | 30 fps | The time step between consecutive frames is derived from this. Unused when baking a single frame |
| **Start At** | wherever the transport is sitting | The moment to start baking from |
| **Device** | not per device | Which device's view to export |
| **Format** | 2D | 2D writes format 5, 3D writes format 4 |

The dialog also carries two things: a **Fit to Loop** button that sets the frame count from the content's own periods, and a loop verdict telling you whether the batch loops **exactly**, has **no common multiple** across its periods, or **does not repeat**.

The section header takes its name from the asset name (truncated to 8 bytes); the company field is always `SUPRSTGE`. The file ends with a **section header with a record count of zero**, as the specification requires.

Import and export sit on the asset editor's main toolbar.

### 4.4 Three Things Worth Checking When Interchanging

Exported files follow the IDTF14 rev011 specification. When interchange with third-party software goes wrong, check these three first — they are where implementations most often diverge:

| Item | What the spec says |
| --- | --- |
| **Byte order** | 32-byte section header, every multi-byte field **big-endian** |
| **Y axis** | X positive right, **Y positive up** |
| **True-colour byte order** | **B, G, R**, not R, G, B |

Status byte: bit 6 = blanking (1 = laser off), bit 7 = last point.

---

## 5. Safety Reminder

**Exported ILDA files must be reviewed by a licensed operator under on-site safety procedures before being fed to a real laser system.**

This product provides no audience-scanning approval, no maximum permissible exposure (MPE) calculation, no laser zoning certification, no physical projector calibration, no emergency-stop hardware and no scan-failure protection, and it constitutes no regulatory-approval basis.

---

## 6. Related Documents

- [Laser Overview](00_Laser_Overview_en.md)
- [Laser Animation Asset](01_LaserAnimationAsset_en.md)
- [Effect System](03_Effects_en.md)
