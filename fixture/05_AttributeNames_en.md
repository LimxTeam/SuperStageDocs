# Appendix: DMX Attribute Name Reference

> Applies to SuperStage 26H2.6 and later ｜ Read first: [The Fixture Definition Asset](01_FixtureDefinition_en.md)

Every channel in a channel library carries an **attribute name**, and a binding uses that name to connect the channel to one of the fixture's capabilities. If the name does not match, the channel cannot be bound — which is the most common cause of "the console moves the fader and nothing happens".

This document lists all **119 attribute names** the current implementation recognises, taken from the constants table defined centrally in the source.

---

## 1. Case Variants Are Deliberate

`Pan` and `PAN`, `Zoom` and `ZOOM` are all in the table. Manufacturers write them differently in their GDTF packages, so both spellings are listed to match what is actually out there.

**Attribute name matching is case-insensitive.** `PAN` in the channel library and `Pan` in a binding still connect. When a name does not match, it is almost always **spelling or separators** (`ColorAdd_R` and `ColorAddR` are two different names), not case.

---

## 2. Movement

| Attribute | Description |
| --- | --- |
| `Pan` / `PAN` | Pan |
| `Tilt` / `TILT` | Tilt |
| `PTSpeed` / `PTSPEED` | Pan / tilt speed |
| `TiltRot` | Tilt continuous rotation |
| `PosZ` | Lift position |

Continuous rotation behaviour is decided by the sub-attribute's RotationMode — see [Fixture Motion](04_Motion_en.md).

---

## 3. Intensity and Strobe

| Attribute | Description |
| --- | --- |
| `Dimmer` / `DIM` | Main intensity |
| `Dimmer2` | Second dimmer |
| `Shutter` / `SHUTTER` | Shutter / strobe |
| `SHUTTER3` / `SHUTTER6` | Third / sixth shutter on multi-shutter fixtures |

---

## 4. Colour

### Additive

| Attribute | Description |
| --- | --- |
| `Red` / `Green` / `Blue` / `White` | RGBW |
| `Red1` / `Green1` / `Blue1` | Second RGB set |
| `Amber` / `Lime` / `Indigo` / `UV` / `RedOrange` | Extended emitters |
| `COLORRGB1` / `COLORRGB2` / `COLORRGB3` | Combined RGB |

### Subtractive (CMY)

| Attribute | Description |
| --- | --- |
| `Cyan` / `Magenta` / `Yellow` | CMY |
| `C` / `M` / `Y` | CMY shorthand |
| `COLORSUB_C` / `COLORSUB_M` / `COLORSUB_Y` | Another CMY spelling |

### Wheels and Colour Temperature

| Attribute | Description |
| --- | --- |
| `Color` / `Color1` / `ColorWheel` / `ColorWheel2` | Colour wheel |
| `COLOR1` / `COLOR2` | Uppercase variants |
| `CTO` / `CTC` / `ColorTemperature` | Colour temperature |
| `Cool` / `Warm` | Cool-warm two-channel mixing |
| `TINT` | Green / magenta correction |

### Backlight

| Attribute | Description |
| --- | --- |
| `BgRed` / `BgGreen` / `BgBlue` / `BgWhite` / `BgCTO` | Independent colour for backlights and aura rings |

---

## 5. Optics

| Attribute | Description |
| --- | --- |
| `Zoom` / `ZOOM` | Zoom |
| `Focus` / `FOCUS` | Focus |
| `Frost` / `FROST` / `Frost2` | Frost |
| `Iris` | Iris |

---

## 6. Gobos

| Attribute | Description |
| --- | --- |
| `Gobo` / `Gobo1` / `Gobo2` / `Gobo3` | Gobo wheels 1 / 2 / 3 |
| `GOBO1` / `GOBO2` / `GOBO3` | Uppercase variants |
| `GoboRot` / `Gobo2Rot` / `Gobo3Rot` / `GOBO3ROT` | Gobo rotation |
| `GOBO2_POS` | Gobo indexing |

Gobo wheel indices are always 1..3, matching the three fixed slots in the fixture definition's optics group.

---

## 7. Prism

| Attribute | Description |
| --- | --- |
| `Prism` / `Prism1` / `Prism2` / `Prism3` | Prisms 1 / 2 / 3 |
| `PRISMA1` / `PRISMA2` | Uppercase variants |
| `PrismRot` / `Prism1Rot` | Prism rotation |
| `PrismPos` / `PRISMA1_POS` | Prism indexing |

---

## 8. Framing (Shaper)

| Attribute | Description |
| --- | --- |
| `A1` `A2` `A3` `A4` / `B1` `B2` `B3` `B4` | Four blades, eight corners (the common library spelling) |
| `ShaperA1` `ShaperA2` / `ShaperB1` `ShaperB2` | The same, with a Shaper prefix |
| `ShaperC1` `ShaperC2` / `ShaperD1` `ShaperD2` | Third and fourth blades |
| `BLADE1A` `BLADE1B` / `BLADE2A` `BLADE2B` / `BLADE3A` `BLADE3B` / `BLADE4A` `BLADE4B` | Per-blade ends (uppercase variants) |
| `Blade1Rot` `Blade2Rot` `Blade3Rot` `Blade4Rot` | Individual blade rotation |
| `ShaperRot` / `SHAPERROT` | Blade assembly rotation |

Blade order: **1 = bottom, 2 = left, 3 = top, 4 = right**. A blade's two ends are its own anticlockwise and clockwise ends, not "left and right". Blade assembly rotation and gobo rotation are separate capabilities — do not cross-wire them.

---

## 9. Effects

| Attribute | Description |
| --- | --- |
| `Effect` | Effect number |
| `EffectSpeed` | Effect speed |
| `GenerationIndex` | Spawn amount on effect machines |

---

## 10. How to Use This Table

**When importing from GDTF**: attribute names come from the package declaration and bindings are derived automatically by name, so this table is usually not needed.

**When hand-writing a channel library**: use names from this table so bindings can be derived automatically. A name outside the table needs a binding added by hand in the fixture editor to connect it to a capability.

**When troubleshooting a dead channel**: check the fixture editor's **Validation** page for that attribute in the unbound list. If it is there, either the name does not match or a binding is missing.

> Control channels (Reset, Lamp On and the like) having no matching capability is normal — they still occupy addresses and the console can still push them, they just do not drive anything visible.

---

## 11. Related Documents

- [The Fixture Definition Asset](01_FixtureDefinition_en.md) — bindings and the 31 capability opcodes
- [Fixture Builder](06_FixtureBuilder_en.md) — how to fill in the binding table
- [Fixture Motion](04_Motion_en.md)
- [Fixture Editor](03_FixtureEditor_en.md) — the Validation page
- [Channel Library Editor](../editortools/10_FixtureLibraryEditor_en.md)
