# Fixture Builder: Making Each Type of Fixture by Hand

> Applies to SuperStage 26H2.6 and later ｜ Read first: [The Fixture Definition Asset](01_FixtureDefinition_en.md), [Fixture Editor](03_FixtureEditor_en.md)

If a GDTF exists, [import it](02_GdtfImport_en.md) — do not build by hand. This document is for the three cases where there is none: the manufacturer does not publish one, you are building an effect device that does not exist in the real world, or you are reworking an imported fixture into something else.

Three parts: **sections 1–3** cover every field (default, range, and what goes wrong if you get it wrong), **section 4** is seven complete worked examples, and **sections 5–6** are the full set of compiler diagnostics and what causes each.

---

## 1. The Four Layers of a Fixture

```text
Root
 ├ Base   static base mesh
 └ AxisA  first rotation axis (yoke), traditionally Pan
     └ AxisB  second rotation axis (head), traditionally Tilt
         └ AxisC  third rotation axis (optional, off by default)
             └ Emitters[]  emitters (single / array / grid / circle / segments / explicit)
```

The mechanical topology is a **fixed set of five role slots**, not a node tree you can wire freely. What that buys: fixed-size runtime data, imported models that can be validated automatically, and an editor that is five rows of form fields.

Fill them in the order **rig → emitters → optics → modes**: emitters mount onto axes and bindings reference emitters by index, so doing it backwards means redoing it.

---

## 2. Every Field

### 2.1 A. Identity

| Field | Default | Description |
| --- | --- | --- |
| **Manufacturer** | empty | e.g. `Robe` / `ClayPaky` |
| **Model** | empty | e.g. `MegaPointe` |
| **Group** | `StageLight` | **The top node in the asset browser's left tree.** Valid values: `StageLight` (manufacturer fixtures) / `SuperLight` (in-house) / `SuperVFX` (effects) / `StageModel` (scenic props) / `SuperStage` (tools) |
| **Fixture Type** | empty | Category, e.g. `Beam` / `Wash` / `Profile` / `Strobe` |
| **GUID** | automatic | Read-only. The authoritative basis for model identity, unchanged across rename / move / re-import |
| **Revision** | `1` | Asset revision, incremented on each release |
| **Thumbnail** | empty | Shown in the asset browser grid |

**Get Group wrong and the fixture lands under the wrong category node.** Leave it at `StageLight` for a manufacturer fixture; use `SuperLight` or `SuperVFX` for your own effect devices.

**Do not clear the GUID.** MVR export and the asset browser tell data-driven fixture models apart by the `(class, GUID)` pair — by class alone, every data-driven fixture would merge into one model.

### 2.2 B. Physical

| Field | Default | Range | Description |
| --- | --- | --- | --- |
| **Power (W)** | `0` | ≥ 0 | Rated power |
| **Weight (kg)** | `0` | ≥ 0 | Weight |
| **Luminous Flux (lm)** | `0` | ≥ 0 | Luminous flux |
| **Native Colour Temperature (K)** | `0` | ≥ 0 | Native white point. **0 = not declared, treated as pure white** |
| **Photometric Intensity** | off | — | Lumens → candela calibration. **Leave it off**, see below |
| **Body Size (m)** | `(0.5, 0.5, 0.5)` | — | Body size, in **metres** |
| **Max Light Distance (m)** | `50` | ≥ 1 | Maximum throw, in **metres** |
| **Max Intensity (%)** | `200` | ≥ 1 | Maximum brightness multiplier (percent) |

Power and weight feed power and load figures, and are carried into MVR / GDTF export.

**About native colour temperature**: this is what stops every fixture's white being the same white — on one wall, an 8000K beam and a 3000K tungsten blinder should look different. Colours are normalized against 6500K when composited, so 6500 and "not filled in" look the same. **It only takes effect when no colour temperature channel (CTC / CTO / CTB) is driving the fixture**, so the two never stack.

> Measured distribution, as a guide: 6000K is the most common (the discharge-lamp mainstream), then 6500K, 8000K, 7000K and 10000K; tungsten audience lights sit at 2500–3000K.

**About Photometric Intensity**: off by default, and **leave it off**. `Max Intensity` does not only feed the real light — it is also multiplied into the beam/lens material's MaxBrightness, and the material side is hand-tuned around a magnitude of 200. A 3-degree beam at 22000 lumens works out to the order of ten million candela; feeding that in directly blows beam brightness through five orders of magnitude. Turning it on presupposes the material side has been adapted to photometric magnitudes — until then, the lumen conversion is a calibration reference only.

### 2.3 C. Rig

At the top there is one **Base Mesh** (static, mounted on the root) plus three axes. Every axis has the same fields:

| Field | Default | Description |
| --- | --- | --- |
| **Enabled** | off | Whether this is a movable axis. **Off = rigid connection** — the mesh still mounts, but responds to no position channel |
| **Rotation Axis** | `Z / Yaw (Pan)` | Which local axis it turns about. Also `X / Roll (Tilt)` and `Y / Pitch` (a few unusual mechanisms) |
| **Pivot Offset (cm)** | `(0,0,0)` | Pivot offset relative to the parent slot, in **centimetres**. Convention uses only Z; all three components exist for the few asymmetric yokes |
| **Range (deg)** | `(-270, 270)` | Mechanical angle range, see below |
| **Invert** | off | Reverse the rotation direction |
| **Max Speed (deg/s)** | `0` | Maximum angular speed. **0 = no limit (instant)** |
| **Mesh** | empty | The model this level carries (axis A is usually the yoke, axis B the head) |

**Range is not "lower / upper bound"** — it is **X = the angle at the DMX minimum end, Y = the angle at the maximum end**. Evaluation is `Lerp(X, Y, normalized DMX)` with no clamping anywhere, so **X > Y is a valid value** expressing a mechanism that runs the other way (the Anolis Eminere's tilt really is `8 .. -8`).

> **The struct default of ±270 does not represent any real axis.** It is a placeholder. With no manufacturer data, the conventional fallbacks are **±270 for pan and ±135 for tilt**.
>
> **Never use ±270 for tilt** — 99.3% of packages do not declare it, and copying it doubles the tilt travel.

**An axis's role comes from its `Rotation Axis`, not from the slot name.** On a tilt-only fixture, put the tilt on axis A and leave axis B off.

### 2.4 D. Emitters

One emitter = one beam origin. **21 fields**, in five groups.

#### Basics

| Field | Default | Description |
| --- | --- | --- |
| **Name** | empty | Emitter name. The binding table references it by name, and the editor displays it. **For matrix fixtures this name must match the module name in the channel library** — see §4.5 |
| **Beam Kind** | `Material Beam (Legacy)` | Runtime component type, see §3.1 |
| **Layout** | `Single` | Layout, see §3.2 |
| **Attach Slot** | `Axis B (Head)` | Which slot to mount on: `Root` / `Base` / `Axis A (Yoke)` / `Axis B (Head)` / `Axis C (Spin)` |
| **Local Offset (cm)** | `(0,0,0)` | Offset from the mount point. On traditional fixtures that is the rig slot; **on multi-head fixtures with head axes enabled it is this instance's deepest head pivot**, so it turns with the head — used to push the lens out of the head mesh |
| **Local Rotation** | `(0,0,0)` | This emitter's orientation. Tilted lenses and ring LEDs need it; without it they all face the same way |

#### Layout parameters

| Field | Default | Range | Used by |
| --- | --- | --- | --- |
| **Count X** | `1` | ≥ 1 | Count along X / segment count / circle count (ignored by `Single`) |
| **Count Y** | `1` | ≥ 1 | Count along Y (**Grid 2D only**) |
| **Span X (cm)** | `0` | ≥ 0 | Total span along X, **centre of first to centre of last** |
| **Span Y (cm)** | `0` | ≥ 0 | Total span along Y |
| **Circle Radius (cm)** | `0` | ≥ 0 | Circle radius (**Circle only**; the panel hides it otherwise) |
| **Circle Angle Offset (deg)** | `0` | — | Circle start angle (**Circle only**) |
| **Explicit Placements** | empty | — | Per-instance transform array (**Explicit only**) |

#### Optics

| Field | Default | Range | Description |
| --- | --- | --- | --- |
| **Lens Mesh** | empty | — | Lens model (optional) |
| **Lens Mesh Scale (XY)** | `(1, 1)` | ≥ 0.01 | XY scale of the lens mesh. **Stretch one lens model to different apertures instead of modelling one per fixture.** XY only — the lens is a thin sheet on the exit face and Z is its thickness, so scaling Z sinks it into the head or floats it out |
| **Lens Radius (cm)** | `10` | ≥ 0.5 | **Beam start radius.** Iris scales down from this baseline |
| **Zoom Range (deg)** | `(1, 10)` | — | Zoom angle range. **X = narrowest, Y = widest** |
| **Default Angle (deg)** | `1` | ≥ 0 | Fixed beam angle used **when there is no Zoom channel** |
| **Default Lens Texture** | empty | — | Default projected texture (LED array patterns and the like; optional) |

> `Lens Radius` and `Zoom Range` are different things: the first is the **physical radius of the aperture** (how thick the beam is at its root), the second is the **spread**. A beam fixture is thin-rooted and narrow; a wash is thick-rooted and wide.

#### Head axes (multi-head fixtures)

| Field | Default | Description |
| --- | --- | --- |
| **Head Axis A (Pan)** | off | Per-instance pan axis; same fields as a rig axis |
| **Head Axis B (Tilt)** | off | Per-instance tilt axis |

**Head axes live on the emitter, rig axes on the whole fixture**: the rig's three axes are one set for the fixture, the head axes are one set per instance. See §4.6.

### 2.5 E. Optics

One group for the whole fixture, **not per emitter**.

| Field | Default | Description |
| --- | --- | --- |
| **Gobo Wheel 1 / 2 / 3** | empty | Three fixed named slots for gobo atlases |
| **Gobo Wheel 1 / 2 / 3 Slots** | `0` | Slot count of that atlas. **0 = unknown**, and the compiler falls back to estimating from the channel functions |
| **Color Wheel 1 / 2 / 3** | empty | Colour atlases |
| **Color Wheel 1 / 2 / 3 Slots** | `0` | Slot counts |
| **Prism Preset** | empty | Prism preset (up to 3 layer configs, matching the binding's `Wheel Index`) |
| **Dimmer Curve** | `2.0` | 1.0 ~ 3.0 | Dimmer response exponent. 1.0 = linear, 2.0 = recommended, 2.2 = Gamma |

Three named slots rather than an array: the binding's `Wheel Index` is always 1~3 and the component side has exactly three entries.

> **The slot count must come from the same source as the atlas — never derive it from the channel functions.**
>
> Colour wheels commonly put the scroll segment first, and that segment has no slot list — counting "the slot count of the first sub-attribute" gives 1, and the whole colour wheel is treated as a single cell (measured: 243 of 414 colour wheels are laid out this way). Some manufacturers also split colours across several functions, so no single one is the whole picture.
>
> Colour wheels especially can only be stored, not derived: a colour atlas is always 256×16, with N cells packed into 256 pixels, so the size tells you nothing (a gobo atlas is cell-width × N, which can be derived).

### 2.6 F. Modes and Bindings

One mode = **a name + one channel library + one binding table**. A definition can have several modes, each pointing at one channel library.

The 9 fields of a binding:

| Field | Default | Description |
| --- | --- | --- |
| **Attribute** | empty | The attribute name in the channel library. Matched character for character, but **not case-sensitive** |
| **Capability** | `None` | One of the 31 capability opcodes |
| **Emitter Index** | `-1` | Target emitter index. **−1 = applies to all emitters** (e.g. the main dimmer) |
| **Module Index** | `0` | Module instance index. **≥ 0 binds to that module; −1 iterates every module instance, driving the matching emitter per cell** |
| **Resolution** | `Coarse` (8-bit) | Read bit depth: 8 / 16 / 24 bit |
| **Response Curve** | `Linear` | `Linear` / `Power` / `S-Curve` |
| **Curve Exponent** (0.1 ~ 5.0) | `2.0` | Power curve exponent, **shown only when Curve = Power** |
| **Invert** | off | Reverse (rising DMX → falling physical quantity) |
| **Params** | — | Extra parameters, below |

The 5 `Params` fields, **each meaningful only to certain capabilities** — leave the rest at their defaults:

| Parameter | Default | Used by |
| --- | --- | --- |
| **Channel Color** | white | `Color Additive Component` / `Color Subtractive (CMY)` — the spectral component this channel represents |
| **Wheel Index** (1~3) | `1` | `Gobo Select` / `Gobo Rotate` / `Color Wheel` / `Prism Select` |
| **Blade Index** (1~4) | `1` | `Blade Insert` / `Blade Corner B` / `Blade Rotate` |
| **Temperature Range (K)** | `(2700, 8000)` | `Color Temperature` — X = channel-zero end, Y = channel-full end |
| **Physical Range Override** | `(0, 0)` | Overrides the range declared on the channel. **X == Y means no override**, falling back to the range this sub-attribute declares in the channel library |

---

## 3. Two Decisive Choices

### 3.1 Beam Kind

**The default is `Material Beam (Legacy)`. A new emitter must be changed.** The importer only overrides that default for the two kinds it can recognise — matrix/pixel goes to Ray Beam or Wash, framing goes to Shaper — and everything else lands on it.

| Goal | Choose | Runtime component |
| --- | --- | --- |
| Main light of a beam / moving head | **Volumetric Beam** | Volumetric beam. Analytic scattering with real occlusion from the shadow atlas, plus gobo / colour wheel / prism in gate space |
| Profile / framing fixture | **Volumetric Shaper (Profile)** | Volumetric plus four blades. The blades live in their own shader rather than in a permutation of the beam |
| Each cell of a matrix / pixel bar / FX fixture | **Ray Beam (Matrix / FX)** | Volumetric **minus the optical elements**. Always the zero-sample closed-form path, so hundreds of instances stay affordable and there are no sampling artifacts |
| Wash / blinder / strobe | **Wash (Color / Blinder / Strobe)** | No column — a bounded ellipsoidal glow in front of the head |
| Aura rings, backplates, emitting surfaces | **Wash** | Same |
| Floor pattern only, no column | **Spot Only (No Column)** | Cone light only. **The floor pattern is unaffected** — gobo / colour wheel / blades / prism are all projected by the cone light path |
| Pixel matrix (one component per block) | **Pixel Matrix** | See §4.5 |
| Effect plane running built-in patterns | **Effect Plane (LED FX / Magic)** | See the important note in §5.1 |
| — | **Material Beam / Shaper (Legacy)** | The old material pipeline, kept as a fallback. **Do not pick it for anything new** |

**Two ways to get this wrong, both of which the compiler warns about:**

- Binding gobo / prism / colour wheel to a **Ray Beam or Wash** — neither has optical elements, so the binding silently does nothing;
- Choosing Ray Beam for an **aura ring or backplate** — a fixture with 25 rings fires 25 columns, which is wrong and costs 25 columns.

### 3.2 Layout, and Its Exact Geometry

| Layout | Instances | How positions are computed |
| --- | ---: | --- |
| **Single** | 1 | Exactly at `Local Offset` |
| **Array 1D (X)** | `Count X` | Evenly along X; instance i sits at `Lerp(-SpanX/2, +SpanX/2, i/(CountX-1))`. With `Count X ≤ 1` it is always 0 |
| **Grid 2D (X/Y)** | `Count X × Count Y` | **Row-major**, index = `y × CountX + x`. X ascends from `-SpanX/2` to `+SpanX/2`; **Y descends from `+SpanY/2` to `-SpanY/2`** |
| **Circle (XY)** | `Count X` | Instance i is at `CircleAngleOffset + 360 × i / CountX`. **Divided by CountX, not CountX−1** (first and last do not coincide). 0° lands on **+X**, increasing **counter-clockwise**, matching UE's Yaw. The centre is `Local Offset` |
| **Segments** | `Count X` | All at `Local Offset`. **N instances share one matrix component**, each carrying its own segment index |
| **Explicit** | number of placements | Position = `Local Offset + that transform's translation`, orientation from that transform. **An empty list degrades to a single emitter** rather than producing none |

Key points:

- **Span is the total centre-to-centre distance from first to last**, not the pitch. A 12-cell bar at 10 cm pitch takes `Span X` = **110**, not 120;
- **Grid 2D's Y descends** — row 0 is at the top. Check this first when per-cell colours look flipped;
- **Circle's 0° is at +X.** A manufacturer's ring of lenses rarely starts exactly there, so use `Circle Angle Offset` to line it up rather than rotating the whole fixture's mount;
- **Curved bars and tilted panels need Explicit.** A regular layout straightens the line and flattens the orientation — 60 of the 316 packages in the full library carry a non-identity rotation on Position, and those are necessarily distorted by a regular layout.

---

## 4. Seven Complete Worked Examples

Each example gives every value needed to go from nothing to working. The numbers are illustrative magnitudes — use the manufacturer's data sheet for a real fixture.

### 4.1 Moving Head Beam

A 260 W beam fixture: 540° pan, 270° tilt, 2° beam, one fixed gobo wheel and one colour wheel.

**Identity / Physical**

| Field | Value |
| --- | --- |
| Manufacturer / Model | per the real fixture |
| Group | `StageLight` |
| Fixture Type | `Beam` |
| Power (W) | `260` |
| Weight (kg) | `21` |
| Native Colour Temperature (K) | `7000` (typical for a discharge lamp) |
| Body Size (m) | `(0.35, 0.25, 0.55)` |
| Max Light Distance (m) | `80` |
| Max Intensity (%) | `200` (leave at default) |

**Rig**

| | Base | Axis A | Axis B | Axis C |
| --- | --- | --- | --- | --- |
| Enabled | — | ✔ | ✔ | ✘ |
| Rotation Axis | — | `Z / Yaw (Pan)` | `X / Roll (Tilt)` | — |
| Range (deg) | — | `(-270, 270)` | `(-135, 135)` | — |
| Pivot Offset (cm) | — | `(0, 0, 18)` | `(0, 0, 22)` | — |
| Max Speed (deg/s) | — | `540` | `300` | — |
| Mesh | base | yoke | head | — |

> 540° pan is the **total travel**, which is Range `(-270, 270)`; 270° tilt is `(-135, 135)`.

**Emitter** (one)

| Field | Value |
| --- | --- |
| Name | `Beam` |
| Beam Kind | **Volumetric Beam** |
| Layout | `Single` |
| Attach Slot | `Axis B (Head)` |
| Local Offset (cm) | `(0, 0, 20)` (pushes the lens out of the head) |
| Lens Radius (cm) | `6` |
| Zoom Range (deg) | `(2, 2)` (a beam fixture is fixed-angle) |
| Default Angle (deg) | `2` |

**Optics**

| Field | Value |
| --- | --- |
| Gobo Wheel 1 | atlas from the [Gobo Atlas Builder](../editortools/14_GoboAtlasBuilder_en.md) |
| Gobo Wheel 1 Slots | the atlas's real slot count, e.g. `17` |
| Color Wheel 1 | atlas from the [Color Atlas Builder](../editortools/13_ColorAtlasBuilder_en.md) |
| Color Wheel 1 Slots | e.g. `14` |
| Dimmer Curve | `2.0` |

**Bindings**

| Attribute | Capability | Emitter | Resolution | Params |
| --- | --- | ---: | --- | --- |
| `Pan` | Axis A Position (Pan) | −1 | 16-bit | — |
| `Tilt` | Axis B Position (Tilt) | −1 | 16-bit | — |
| `PTSpeed` | Axis Speed (PT Speed) | −1 | 8-bit | — |
| `Dimmer` | Dimmer | −1 | 16-bit | — |
| `Shutter` | Shutter / Strobe | −1 | 8-bit | — |
| `Color` | Color Wheel | 0 | 8-bit | Wheel Index `1` |
| `Gobo` | Gobo Select | 0 | 8-bit | Wheel Index `1` |
| `GoboRot` | Gobo Rotate / Index | 0 | 16-bit | Wheel Index `1` |
| `Prism` | Prism Select | 0 | 8-bit | Wheel Index `1` |
| `PrismRot` | Prism Rotate | 0 | 16-bit | Wheel Index `1` |
| `Frost` | Frost | 0 | 8-bit | — |
| `Focus` | Focus | 0 | 16-bit | — |

> Pan / Tilt / Dimmer use **Emitter Index = −1** (all emitters); optics bind to **emitter index 0**. On a single-emitter fixture both read the same, but the difference matters as soon as there are several.
>
> **16-bit only works if the channel library actually has a Fine channel.** Without one, the compiler downgrades to 8-bit and warns — do not fill it in blind.

### 4.2 Moving Head Profile (with Framing)

Two changes on top of 4.1; everything else is the same.

**Emitter**: `Beam Kind` becomes **Volumetric Shaper (Profile)**, and `Zoom Range` becomes the fixture's real zoom, e.g. `(7, 48)`.

**Bindings** gain the framing section. Blade order: **1 = bottom, 2 = left, 3 = top, 4 = right**. A blade's A end is its own anticlockwise end.

| Attribute | Capability | Params |
| --- | --- | --- |
| `A1` | Blade Insert | Blade Index `1` |
| `B1` | Blade Corner B | Blade Index `1` |
| `A2` | Blade Insert | Blade Index `2` |
| `B2` | Blade Corner B | Blade Index `2` |
| `A3` | Blade Insert | Blade Index `3` |
| `B3` | Blade Corner B | Blade Index `3` |
| `A4` | Blade Insert | Blade Index `4` |
| `B4` | Blade Corner B | Blade Index `4` |
| `ShaperRot` | Shaper Rotate | — |

**Two channels per blade**: `Blade Insert` and `Blade Corner B` are the insertion depths of that blade's **two ends** — not "insert plus angle".

**"Insert + rotate" framing** (ClayPaky Arolla and similar) is written differently — one insert and one rotation:

| Attribute | Capability | Params |
| --- | --- | --- |
| `A1` | Blade Insert | Blade Index `1` |
| `Blade1Rot` | **Blade Rotate** | Blade Index `1` |

At apply time the two corners are derived as `corner = insert ± 0.5 × tan(θ)`.

> **Never map `Blade(n)Rot` to `Blade Corner B`** — that treats an angle as an insertion depth. Assembly rotation (`Shaper Rotate`) and gobo rotation (`Gobo Rotate`) are also two different capabilities; do not cross-wire them.

### 4.3 Moving Wash

Pan / tilt as in 4.1; the optics group goes unused.

**Emitter**

| Field | Value |
| --- | --- |
| Beam Kind | **Wash** |
| Layout | `Single` |
| Attach Slot | `Axis B (Head)` |
| Lens Radius (cm) | `12` (wash fixtures have a wide aperture) |
| Default Angle (deg) | ignored — a wash's spread lives on the component, below |

Wash's four parameters are on the **component** (`B.BeamParameter` in the details panel), not in the definition: `Wash Length` 1000 cm, `Wash Spread` 70°, `Wash Far Fade` 0.6, `Use Ellipsoid Volume` off. Its spread is **fixed and does not respond to zoom**; zoom, frost and iris are no-ops. See [SuperWashComponent](../lightcomponent/14_SuperWashComponent_en.md).

**Bindings (RGBW mixing)**

| Attribute | Capability | Params: Channel Color |
| --- | --- | --- |
| `Dimmer` | Dimmer | — |
| `Shutter` | Shutter / Strobe | — |
| `Red` | Color Additive Component | red `(1, 0, 0)` |
| `Green` | Color Additive Component | green `(0, 1, 0)` |
| `Blue` | Color Additive Component | blue `(0, 0, 1)` |
| `White` | Color Additive Component | white `(1, 1, 1)` |

**One binding per channel, with the colour set to the spectral component that channel really is.** Not limited to RGBW — amber, lime, UV and red-orange are the same capability with a different `Channel Color`.

Other mixing schemes:

| Scheme | Capability | Notes |
| --- | --- | --- |
| CMY | `Color Subtractive (CMY)` | One binding per channel, with the matching subtractive component |
| CTO / CTC | `Color Temperature (CTO/CTC)` | Put **this fixture's real range** in `Temperature Range (K)`; the default is `(2700, 8000)` |
| Cool / warm pair | One `Color Additive Component` each | Colour set to each one's own white point |
| Green / magenta correction | `Colour Tint (Green / Magenta)` | **Bipolar**: 0 = full magenta, 0.5 = neutral, 1 = full green. The additive and subtractive capabilities are unipolar, so a single channel cannot reach both ends — which is why this has its own capability |

### 4.4 Par / Strobe / Audience Blinder

The only difference from 4.3: **all three axes off**, and `Attach Slot` becomes `Root` or `Base`.

Drop `Pan` / `Tilt` / `PTSpeed` from the binding table; everything else is the same.

> For a tungsten blinder, remember `Native Colour Temperature (K) = 2700` — without it, it and an 8000K beam on the same wall are the same white.

### 4.5 Pixel Bar / Matrix Panel

**Two approaches**, depending on how fine the control has to be.

#### Option 1: Segments (lowest cost, plain pixel bar)

A 12-segment pixel bar at 10 cm pitch:

| Field | Value |
| --- | --- |
| Name | **`Pixel`** (must match the module name in the channel library — see below) |
| Beam Kind | **Pixel Matrix** |
| Layout | `Segments` |
| Count X | `12` |
| Attach Slot | `Root` (the body is fixed to the bar) |
| Local Offset (cm) | `(0, 0, 0)` |

Produces 12 instances that **share one matrix component**, each with its own segment index.

The trade: **the Segments layout cannot carry head axes** (cells are not independent scene components).

#### Option 2: Array 1D (one component per cell)

| Field | Value |
| --- | --- |
| Name | `Pixel` |
| Beam Kind | **Ray Beam (Matrix / FX)** |
| Layout | `Array 1D (X)` |
| Count X | `12` |
| **Span X (cm)** | **`110`** ← 12 cells at 10 cm pitch, first-to-last centre distance = 11 × 10 |
| Lens Radius (cm) | `2.5` |

Each cell is its own component, so each gets the full beam treatment and can carry head axes.

#### Per-cell binding (the same for both options)

| Attribute | Capability | Emitter | **Module Index** | Channel Color |
| --- | --- | ---: | ---: | --- |
| `Dimmer` | Dimmer | −1 | `0` | — |
| `Red` | Color Additive Component | 0 | **`-1`** | red |
| `Green` | Color Additive Component | 0 | **`-1`** | green |
| `Blue` | Color Additive Component | 0 | **`-1`** | blue |

`Module Index = -1` means **per cell**: iterate every module instance, with module i driving emitter instance i.

> **The emitter's Name must equal the module name in the channel library.** The compiler draws the group boundary by "module name == target emitter name". With no name match it falls back to scanning the whole library — and a fixture with several different light groups (one control module plus LED1×7, LED2×14, LED3×7) has the same colour channel names in all three groups, so a whole-library sweep would stuff 28 cells into a 7-instance emitter.
>
> **The module count and the instance count must match.** When they do not, the compiler warns: extra modules all clamp to the last instance (the last cell is driven several times) and extra instances receive nothing (permanently on or off). Changing `Count X` or deleting a module will hit this.

Matrix fixtures introduce **no dedicated capability opcode** — per-cell control is expressed entirely through `Module Index`, using the same general capabilities (`Dimmer`, `Color Additive Component`, …).

### 4.6 Multi-Head Fixture (Each Head Moves Independently)

Spiders and moving bars: one fixture, N heads, each with its own pan / tilt.

**The key: head axes go on the emitter, not on the rig.** The rig's three axes are one set for the whole fixture, so N heads driven through a main axis would overwrite each other and the last one would win.

An 8-head moving bar:

| Field | Value |
| --- | --- |
| Name | `Head` |
| Beam Kind | **Ray Beam** |
| Layout | **`Array 1D (X)`** |
| Count X | `8` |
| Span X (cm) | `70` (8 heads at 10 cm pitch) |
| Attach Slot | `Root` |
| **Head Axis B (Tilt)** | Enabled ✔, `X / Roll (Tilt)`, Range `(-180, 180)` |

Rig: **all three main axes off** (the body is fixed to the bar).

Binding:

| Attribute | Capability | Emitter | Module Index |
| --- | --- | ---: | ---: |
| `Tilt` | Axis B Position (Tilt) | 0 | **`-1`** |

Three conditions, all required, or the binding routes to the main axis instead:

1. **Per-module** (`Module Index = -1`) — a whole-fixture control channel (one pan for the fixture) still uses the main axis;
2. **The head axis is enabled** — with none configured there is nowhere to land;
3. **Not a Segments layout** — segments share one matrix component and cannot rotate.

Two common shapes:

| Fixture shape | Configuration |
| --- | --- |
| Shared pan, individual tilt | Main pan on **rig axis A**, only `Head Axis B` enabled on the emitter |
| Independent pan and tilt per head | **Both head axes** enabled on the emitter; the rig axes can all be off |

### 4.7 Aura Ring / Backplate

A ring of 25 LEDs:

| Field | Value |
| --- | --- |
| Name | `Aura` |
| Beam Kind | **Wash** ← **never Ray Beam** |
| Layout | **`Circle (XY)`** |
| Count X | `25` |
| Circle Radius (cm) | `14` |
| Circle Angle Offset (deg) | to match the real fixture, e.g. `7.2` |
| Attach Slot | `Axis B (Head)` |
| Lens Radius (cm) | `1.5` |

Pick Ray Beam and all 25 ring LEDs fire a beam column — wrong, and 25 columns' worth of wasted cost. On GDTF import, groups declared `BeamType = Glow / None` become Wash automatically.

When a backlight or aura needs a colour independent of the main light, use the `BgRed` / `BgGreen` / `BgBlue` / `BgWhite` / `BgCTO` attribute names, bound the same way with `Color Additive Component` plus the matching `Channel Color`.

---

## 5. Effect Fixtures, and What Cannot Be Built

### 5.1 Effect Fixtures Need a Lens Mesh on the Emitter

The `Effect Plane (LED FX / Magic)` beam kind drives **a flat plane carrying the effect material**, and that plane uses the emitter's **Lens Mesh** field. **An effect emitter with Lens Mesh left empty shows nothing** — the component is created, but with no mesh there is no dynamic material, so none of the effect parameters can be written.

How to bind it:

| Attribute | Capability | Notes |
| --- | --- | --- |
| `Effect` | `Effect` | Looks up a 256-entry table, yielding an (effect number 0–15, speed −4 ~ +4, width 0.1 ~ 4.0) triple in one lookup |
| `EffectSpeed` | `Effect Speed` | Bind it separately and speed comes from this channel instead, with only effect number and width taken from the table |

Direction needs no channel of its own: speed maps to −4 ~ +4, the midpoint is stop, and the two sides are the two directions — the sign *is* the direction.

> Binding `Effect` / `Effect Speed` still raises an "effect engine is not implemented" warning on Compile. That warning text is out of date; it does not invalidate the compiled product, and the effect channels do produce output.

### 5.2 Lifting Devices Cannot Be Built

**There is no lift-position capability among the 31 opcodes.** The `PosZ` attribute is handled only by the shipped C++ machinery fixtures, which are not definition assets and do not appear in the fixture editor.

Use what already ships: [Stage Machinery](../stagecore/10_Stage_Machinery_en.md) (lifting and rail machinery), the [Lift Matrix](../stagecore/12_Lift_Matrix_en.md), and the lifting effect ball — see [In-House Fixtures](../stagecore/17_InHouseFixtures_en.md).

---

## 6. Compiler Diagnostics and What Causes Them

After **Compile**, results land on the **Validation** page. **Any Error voids the compiled product** — the fixture does not update.

### 6.1 Errors (must be fixed)

| Diagnostic | Cause |
| --- | --- |
| Definition has no DMX modes | No mode has been created |
| Mode has no channel library assigned | The mode has no channel library |
| Channel library defines no modules | The channel library is empty |
| Mode has no bindings; nothing would be driven by DMX | The binding table is empty |
| Binding has no capability assigned | A binding's Capability is still `None` |
| Binding has no attribute name | A binding has no attribute name |
| Attribute '…' not found in the channel library | The name is not present in any module of this mode. Matching is case-insensitive, so check spelling and separators first |
| Emitter index N is out of range | The binding references an emitter index that does not exist |
| Module index N is out of range | The binding references a module index that does not exist |

### 6.2 Warnings (it compiles, but probably not what you wanted)

| Diagnostic | Cause and fix |
| --- | --- |
| Definition has no emitters; the fixture will render no light | No emitter has been created |
| Binding drives an axis that is disabled in the rig | Pan/tilt is bound but the matching rig axis has `Enabled` off |
| Head axes are not supported on a Segments-layout emitter | Segments cannot carry head axes. Switch to Array 1D / Grid 2D, or the binding falls back to the rig axis |
| Emitter uses the Ray Beam type, which has no optical elements | A gobo / prism / colour-wheel binding on a Ray Beam or Wash. If the fixture really projects patterns, change the Beam Kind to Volumetric |
| Wheel N has no atlas/preset assigned in Optics | The atlas or prism preset that `Wheel Index` points at is empty |
| Attribute '…' defines no slot list; the wheel will only ever show atlas cell 0 | That attribute has no slots authored in the channel library |
| Attribute '…' has continuous scroll/shake segments with no physical range | Scroll / shake segments have no speed data, so a fallback is used instead of vendor data |
| 16-bit requested but the library defines no Fine channel | Resolution was set to 16-bit but the library has no Fine channel; downgraded to 8-bit |
| 24-bit requested but the library defines neither Fine nor Ultra | As above; 24-bit needs Fine or Ultra |
| Per-module binding '…' resolved N module(s) but the emitter has M instance(s) | **Module count does not match instance count.** Extra modules clamp to the last instance; extra instances receive nothing |
| Duplicate binding: same attribute, capability, emitter and module | A duplicate binding |
| Channel '…' is defined in the library but not bound to any capability | In the library but not in the binding table. Control channels (Reset, Lamp On) here are normal; **a channel that clearly ought to do something visible is a missing binding** |
| The effect engine is not implemented yet | See §5.1 |

### 6.3 Four Steps After Building

1. **Compile** — without it, the fixtures on stage do not change;
2. **Validation page** — work through the tables above;
3. **Channel Map page** — catches the three usual channel-table mistakes: **conflicts** (two attributes on one channel), **gaps** (declared but nothing bound), **overflow** (bindings reaching past the declared span — that fixture is quietly reading the next fixture's channels);
4. **DMX Test page** — push every channel. **Default** makes the fixture do something sensible; the **Rig** aid confirms the pivots really sit on the mechanical joints.

---

## 7. Quick Reference: Ten Easy Mistakes

| Symptom | Cause |
| --- | --- |
| A new emitter draws no column, or looks like the old pipeline | `Beam Kind` is still the default `Material Beam (Legacy)` |
| Tilt travel is twice the real fixture's | The struct default ±270 was used for tilt. The tilt convention is **±135** |
| Cell pitch on a bar is wrong | `Span X` was filled in as the **pitch**. It is the **total centre-to-centre span** — 8 cells at 10 cm is 70 |
| A matrix panel is upside down | Grid 2D's **Y descends** — row 0 is at the top |
| An aura ring starts at the wrong angle | Circle's 0° is at **+X**; use `Circle Angle Offset`, not the fixture's mount rotation |
| Per-cell colours are offset, or the last cell flickers | Module count does not match instance count — check the compile warning |
| A per-cell binding also drives another group's cells | The emitter `Name` does not match the module name, so it fell back to a whole-library sweep |
| A whole gobo or colour wheel is misaligned | The **slot count** in the optics group does not match the atlas |
| Blade values do nothing | `Beam Kind` is not a Shaper kind |
| Heads on a multi-head fixture do not move | The Segments layout was used; or `Module Index` is not −1; or the head axis is not enabled |

---

## 8. Related Documents

- [The Fixture Definition Asset](01_FixtureDefinition_en.md) — field reference and the 31 capability opcodes
- [Fixture Editor](03_FixtureEditor_en.md) — how the four pages work
- [Fixture Motion](04_Motion_en.md) — travel, the speed channel, continuous rotation
- [Attribute Name Reference](05_AttributeNames_en.md) — the 119 DMX attribute names
- [GDTF Import](02_GdtfImport_en.md) — when a GDTF exists, do not build by hand
- [Channel Library Editor](../editortools/10_FixtureLibraryEditor_en.md)
- [Gobo Atlas Builder](../editortools/14_GoboAtlasBuilder_en.md), [Color Atlas Builder](../editortools/13_ColorAtlasBuilder_en.md)
- [Light Components](../lightcomponent/00_LightComponent_Overview_en.md)
