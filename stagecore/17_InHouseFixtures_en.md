# 17 - In-House Fixtures (SuperLight)

> **Module**: SuperAssets ｜ **Applies to**: SuperStage 26H2.6 and later
> **Read first**: [03 - DMX Fixture Basics](03_DMX_Actor_Base_en.md)

---

## 1. What They Are

Besides the 807 manufacturer models imported from GDTF, SuperStage ships **16 in-house fixtures** in seven series. They are in the **SuperLight** category of the asset browser.

Unlike the library models, in-house fixtures are **hand-written C++ fixtures**: their body, component structure and channel mapping live in code rather than being described by a fixture definition asset. So:

- They **do not appear in the fixture editor** and cannot be re-imported from GDTF;
- Each binds its own shipped channel library (`SL_*`), so they patch and respond to a console normally;
- They are not reproductions of specific manufacturer models but general-purpose stage effect devices.

When you need a specific manufacturer model, use one from the library or [import it from GDTF](../fixture/02_GdtfImport_en.md).

---

## 2. The List

| Series | Model | Channels | Purpose |
| --- | --- | ---: | --- |
| **Aurora** | Aurora_Fan200 | 8CH | Fan effect (small) |
| | Aurora_Fan400 | 8CH | Fan effect (large) |
| **Blaze** | BlazeAtomic3K | 5CH | Strobe |
| | BlazeStrip1000 | 5CH | Strobe bar |
| | BlazeMatrixStrip20 | 63CH | Matrix strobe bar (20 segments) |
| **Flare** | Flare_RGBW600 | 7CH | RGBW colour par |
| | Flare_WashMH600 | 11CH | Moving-head wash |
| **Hyperion** | Hyperion550Beam | 15CH | Beam fixture |
| | Hyperion700Profile | 25CH | Profile / framing fixture |
| **Spark** | Spark_P1Evo | 3CH | Studio fixture |
| | Spark_V1 | 5CH | Studio fixture |
| | Spark_Vintage300 | 4CH | Vintage studio fixture |
| | Spark_Profile600 | 15CH | Studio profile fixture |
| **Thunder** | Thunder_Sidewinder10 | 53CH | Beam bar (10 heads) |
| **Machinery** | Machinery_LiftBall | 9CH | Lifting effect ball |
| | LiftMatrix | 42CH | Lifting light matrix |

Channel counts follow the shipped channel libraries; confirm the actual footprint with the [Patch Tool](07_Patch_Tools_en.md) before patching.

---

## 3. Placing and Patching

Exactly as for any other DMX fixture:

1. Drag from the **SuperLight** category of the asset browser into the scene;
2. Set **Universe**, **Start Address** and **ControlMode**, reserving the channels listed above;
3. Drive it from the console.

They ship already bound to their channel libraries; nothing has to be assigned by hand.

---

## 4. Beam Pipeline

In 26H2.6 the in-house fixtures are assigned beam types by purpose:

| Fixture type | Beam component |
| --- | --- |
| Beam fixtures (Hyperion550Beam and similar) | Volumetric beam |
| Profile / framing fixtures (Hyperion700Profile, Spark_Profile600 and similar) | Volumetric shaper |
| Matrix bars (BlazeMatrixStrip20, Thunder_Sidewinder10) | Matrix component |
| Effect types (Aurora fan effects, the emitting units on LiftBall / LiftMatrix) | Effect plane component |

Parameter details for each component are in [Light Components](../lightcomponent/00_LightComponent_Overview_en.md).

> Some in-house fixtures still use the material shaper component (Legacy). They are hand-written C++ fixtures and are not covered by "the shipped library is fully migrated to the volumetric pipeline" — that statement is about fixture definition assets.

---

## 5. The Two Mechanical Fixtures

**LiftMatrix** (42CH) and **LiftBall** (9CH) are lifting fixtures with cable visuals; both the lift and the effect colours are DMX-driven.

LiftMatrix's structure and channel behaviour have their own document: [12 - Lift Matrix](12_Lift_Matrix_en.md).

---

## 6. Common Questions

### It will not open in the fixture editor

That is expected. In-house fixtures are C++ fixtures with no fixture definition asset, so they do not go through the fixture editor. To inspect the channels, open the channel library (`SL_*`) they are bound to in the Content Browser.

### I want to change the body or the channels

An in-house fixture's structure is in code; the panel exposes only the parameters it chooses to. If you need a freely editable fixture, build a [data-driven fixture definition](../fixture/01_FixtureDefinition_en.md).

### The console moves the fader and nothing happens

Work through the usual order: is `ControlMode` set to `DMX`, do the universe and start address line up, is the channel inside this fixture's footprint. See [03 - DMX Fixture Basics](03_DMX_Actor_Base_en.md).

---

## 7. Related Documents

- [03 - DMX Fixture Basics](03_DMX_Actor_Base_en.md)
- [07 - Patch Tools](07_Patch_Tools_en.md)
- [12 - Lift Matrix](12_Lift_Matrix_en.md)
- [Fixture System Overview](../fixture/00_FixtureSystem_Overview_en.md)
- [Light Components](../lightcomponent/00_LightComponent_Overview_en.md)
