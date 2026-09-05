# The Fixture Definition Asset

> Applies to SuperStage 26H2.6 and later ｜ Read first: [Fixture System Overview](00_FixtureSystem_Overview_en.md)

A fixture definition (Super Fixture Definition) describes a fixture's body, mechanics, light output and DMX modes. Its asset name is the model name with **no prefix** — the `LTC_` / `LTA_` / `CLA_` / `SM_` assets alongside it are the textures and meshes it references. This document goes through it group by group.

Open it by double-clicking the definition asset in the Content Browser, or by right-clicking a fixture in the scene and choosing to edit its definition. The editing interface is covered in [Fixture Editor](03_FixtureEditor_en.md).

---

## 1. A. Identity

| Field | Description |
| --- | --- |
| Manufacturer | e.g. `Robe`, `Clay Paky` |
| Model | e.g. `MegaPointe` |
| Group | Top-level category; decides which sidebar node it appears under in the asset browser |
| Fixture Type | Fixture category, e.g. `Beam` / `Wash` / `Profile` / `Strobe` |
| GUID | Unique asset identifier, read-only |
| Revision | Revision number, brought in from the GDTF package |
| Thumbnail | The image shown on the asset browser tile |

Manufacturer and Model feed patch naming, MA sync grouping and asset browser search. On import they are filled from the package declaration; if you renamed them yourself, untick "Identity" in the import scope on a re-import or the package values will overwrite yours.

---

## 2. B. Physical

This group is used for reporting, export and photometric calculation. It does not take part in channel resolution.

| Field | Unit | Description |
| --- | --- | --- |
| Power | W | Rated power |
| Weight | kg | Fixture weight |
| Luminous Flux | lm | Luminous flux |
| Native Colour Temperature | K | The fixture's native white point. It decides what "white" actually looks like on this fixture — an 8000 K beam fixture should not be the same white as a 3000 K tungsten unit |
| Photometric Intensity | — | Compute intensity photometrically when on |
| Body Size | m | Body envelope |
| Max Light Distance | m | Maximum lighting distance |
| Max Intensity | % | Intensity ceiling |

> Compile after changing Max Light Distance or Max Intensity. Those two used to be pushed back by model defaults on every rebuild; fixed in 26H2.6.

---

## 3. C. Rig

The rig describes the mechanics: one base plus up to three axes.

| Field | Description |
| --- | --- |
| Base Mesh | The part that does not turn with any axis |
| Axis A (Yoke / Pan) | First axis, usually the yoke / pan |
| Axis B (Head / Tilt) | Second axis, usually the head / tilt |
| Axis C (Spin, optional) | Third axis, for head-spin style mechanics |

Fields on each axis:

| Field | Default | Description |
| --- | --- | --- |
| Enabled | off | Whether this is a **movable** axis. Off does not mean removed — **the mesh still mounts**, it simply responds to no position channel. Used for tilt-only strobes, fully static bars and the like |
| Rotation Axis | `Z / Yaw (Pan)` | Which local axis it turns about. The other two values are `X / Roll (Tilt)` and `Y / Pitch` (a few unusual mechanisms) |
| Pivot Offset (cm) | (0,0,0) | Pivot offset relative to the parent slot, in centimetres. Convention uses only Z; all three exist for the few asymmetric yokes |
| Range (deg) | (−270, 270) | Mechanical travel, see below |
| Invert | off | Reverse |
| Max Speed (deg/s) | 0 | Maximum angular speed. **0 = no limit, snaps into place**; a positive value makes it turn frame by frame, which is also what gives the PT speed channel something to scale |
| Mesh | empty | The mesh this axis moves |

**Range's two components are not "lower / upper bound"** — they are **X = the angle at the DMX minimum end, Y = the angle at the maximum end**. Evaluation is `Lerp(X, Y, normalized DMX)` with no clamping, so **X > Y is a valid value**, expressing a mechanism that runs the other way on some manufacturers.

**The struct default of (−270, 270) does not represent any real axis** — it is a placeholder. The real value is imported from the GDTF declaration; when the package does not state it, the fallback goes by axis role — pan ±270°, tilt ±135°. **Do not use ±270 for tilt**: 99.3% of packages do not declare it, and copying it doubles the travel.

Before 26H2.6 this was never imported from GDTF at all, which made tilt travel wrong on the vast majority of fixtures. If a fixture's tilt travel looks wrong, check whether Range holds the real value from the package.

---

## 4. D. Emitters

An emitter answers "where does light come out, and in what form". A fixture can have several: one for the main beam, one for an aura ring, one for a backplate; a matrix fixture has one per pixel group.

### 4.1 Basics

| Field | Description |
| --- | --- |
| Name | Emitter name |
| Beam Kind | Beam type, see 4.2 |
| Layout | Arrangement, see 4.3 |
| Attach Slot | Which rig level it hangs from: Root / Base / Axis A / Axis B / Axis C |
| Local Offset (cm) | Offset relative to the attach point |
| Local Rotation | Rotation relative to the attach point |

### 4.2 Beam Kind

| Kind | Draws aerial column | Use |
| --- | --- | --- |
| Volumetric Beam | Yes | Volumetric beam. What the shipped library uses |
| Volumetric Shaper (Profile) | Yes | Volumetric beam plus four-blade framing, for profile fixtures |
| Ray Beam (Matrix / FX) | Yes | Pure beam with no optical elements. For matrix and effect fixtures — dozens or hundreds of instances on one fixture stay affordable |
| Wash (Color / Blinder / Strobe) | No | A bounded glow in front of the head. It exists for wide angles — a cone's screen cost grows with the square of the spread, and one 70° fixture can smear across half the screen |
| Spot Only (No Column) | No | Cone light only. The floor pattern is unaffected |
| Pixel Matrix | No | Pixel matrix |
| Effect Plane (LED FX / Magic) | No | Effect plane running built-in patterns |
| Material Beam (Legacy) | Yes | Old material pipeline, kept for fallback |
| Shaper / Profile (Legacy) | Yes | Old material framing, kept for fallback |

Key points:

- **Not drawing an aerial column does not mean not lighting.** Gobos, colour wheels, blades and prisms are all projected by the cone light, and the floor pool is normal. Beam Kind only decides whether and how the aerial column is drawn.
- **The shipped library is fully migrated to the volumetric pipeline** — there are no material-pipeline emitters left in it.
- **But a fixture newly imported from GDTF still gets Material Beam on its main emitter** (Shaper for profile fixtures with blade channels), and has to be switched to a volumetric kind here. Matrix / pixel groups are unaffected: they are assigned Ray Beam or Wash according to the BeamType declared in the package.
- The two Legacy entries sit last in the dropdown and are labelled Legacy. Enum values on existing assets are unchanged, so saved content is unaffected.

### 4.3 Layout

| Layout | Parameters | Description |
| --- | --- | --- |
| Single | — | One |
| Array 1D (X) | Count X, Span X | Evenly spaced along X |
| Grid 2D (X/Y) | Count X/Y, Span X/Y | Two-dimensional grid |
| Segments | Count X | Segmented |
| Circle (XY) | Count X, Circle Radius, Circle Angle Offset | Circular |
| Explicit (per-instance) | Explicit Placements | Per-instance transforms. Used when the GDTF carries layout data |

### 4.4 Lens and Cone

| Field | Description |
| --- | --- |
| Lens Mesh | Lens model. When the package has none, a generic default part is supplied and scaled automatically to the exit aperture radius |
| Lens Mesh Scale (XY) | XY scale of the lens model, for non-circular apertures |
| Lens Radius (cm) | Exit aperture radius. It sets both the beam root thickness and the aperture glow size |
| Zoom Range (deg) | Zoom range |
| Default Angle (deg) | Fixed spread used when there is no zoom channel |
| Default Lens Texture | Default lens texture |

### 4.5 Head Axes (multi-head fixtures)

| Field | Description |
| --- | --- |
| Head Axis A (Pan) | This head's own pan axis |
| Head Axis B (Tilt) | This head's own tilt axis |

For multi-head fixtures where each head pans and tilts independently. Field structure is the same as a rig axis.

---

## 5. E. Optics

Three fixed named slots per wheel type, not an array — the wheel index in a binding is always 1..3 and the component side has only three entries.

| Field | Description |
| --- | --- |
| Gobo Wheel 1 / 2 / 3 | Gobo wheel atlas (a horizontal strip texture) |
| Gobo Wheel 1 / 2 / 3 Slots | Slot count of that atlas; 0 means unknown |
| Color Wheel 1 / 2 / 3 | Colour wheel atlas |
| Color Wheel 1 / 2 / 3 Slots | Slot count of that atlas |
| Prism Preset | Prism preset asset |

**The slot count must come from the same source as the atlas.** The material slices the atlas by that number; get it wrong and the whole wheel is off. It cannot be derived from the channel functions: colour wheels commonly put a scroll segment first, and that segment carries no slot list, so counting "the slot count of the first sub-attribute" yields 1 and the whole wheel is treated as a single cell. Colour atlases are always 256×16, so the count cannot be recovered from the size either. The number is therefore written when the atlas is built; 0 means unknown and the compiler falls back to an estimate.

Atlases are produced by the [Gobo Atlas Builder](../editortools/14_GoboAtlasBuilder_en.md) and [Color Atlas Builder](../editortools/13_ColorAtlasBuilder_en.md), and generated automatically on GDTF import.

---

## 6. F. DMX Modes

| Field | Description |
| --- | --- |
| Mode Name | e.g. `Standard` / `Extended` |
| Channel Library | The channel library this mode uses |
| Bindings | Binding list, see below |

Which mode a fixture in the scene selects decides which channel library it reads.

### 6.1 Bindings

One binding answers: which capability of this fixture does a given attribute in the channel library drive.

| Field | Description |
| --- | --- |
| Attribute | Attribute name in the channel library, e.g. `Pan`, `Dimmer`, `Gobo1` |
| Capability | What this channel drives, see 6.2 |
| Emitter Index | Which emitter it acts on. This is how multi-emitter fixtures separate main beam from aura ring |
| Module Index | Which module of the channel library it acts on. This is how matrix fixtures separate pixels |
| Resolution | Read as 8 / 16 / 24-bit |
| Response Curve | Response curve |
| Curve Exponent | Curve exponent |
| Invert | Reverse |
| Params | Extra parameters, e.g. wheel index (1/2/3), colour component |

Bindings are derived automatically by attribute name on GDTF import, and can be added, removed and edited by hand in the fixture editor. Hand-written channel libraries can generate bindings too.

### 6.2 Capability List

The current implementation supports 31 capabilities:

| Group | Capabilities |
| --- | --- |
| Movement | Axis A Position (Pan), Axis B Position (Tilt), Axis C Position (Spin), Axis A Infinite Rotation, Axis B Infinite Rotation, Axis Speed (PT Speed) |
| Intensity | Dimmer, Shutter / Strobe |
| Colour | Color Additive Component, Color Subtractive (CMY), Color Wheel, Color Temperature (CTO/CTC), Colour Tint (Green / Magenta) |
| Optics | Zoom, Focus, Iris, Frost |
| Gobo | Gobo Select, Gobo Rotate / Index |
| Prism | Prism Select, Prism Rotate |
| Framing | Blade Insert, Blade Corner B, Shaper Rotate, Blade Rotate |
| Effect | Effect, Effect Speed, Effect Width, Effect Direction |
| Other | Control, None |

Channels with no matching capability are listed on the fixture editor's Validation page after import. They still occupy addresses and the console can still push them, they just drive nothing — which is normal for control channels such as Reset and Lamp On. But if a channel that clearly should do something appears in that list, a binding is missing.

---

## 7. Compiled Product

The definition is source data; the runtime reads the compiled product, which is stored alongside the definition asset.

- Saving in the fixture editor compiles automatically;
- Content Browser right-click → **Compile**, with multi-select;
- When the product is out of date, the fixture editor's debug information says it came from an earlier version.

Recompile after changing any of: bindings, emitters, rig, optics slots, physical endpoints (colour temperature range and the like).

---

## 8. Related Documents

- [Fixture System Overview](00_FixtureSystem_Overview_en.md)
- [Fixture Builder](06_FixtureBuilder_en.md) — how these fields go together, per fixture type
- [GDTF Import](02_GdtfImport_en.md)
- [Fixture Editor](03_FixtureEditor_en.md)
- [Channel Library Editor](../editortools/10_FixtureLibraryEditor_en.md)
- [Prism Preset Editor](../editortools/18_PrismPresetEditor_en.md)
- [Light Components](../lightcomponent/00_LightComponent_Overview_en.md)
