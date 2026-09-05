# SuperShaperComponent (Material Framing · Legacy)

> Applies to SuperStage 26H2.6 and later
>
> **This component was called `SuperCuttingComponent` before 26H2.6.** The internal naming changed to Shaper throughout, with redirectors provided, so component classes, material nodes, enum values and Blueprint functions that existing assets store by name are unaffected.

SuperShaperComponent adds four-blade framing on top of the material beam. It belongs to the **old material pipeline** and is kept for fallback; new content should use [SuperVolumetricShaperComponent](12_SuperVolumetricShaperComponent_en.md).

---

## 1. Parameters

Four blades, one cutting edge each. A blade is defined by **how far each of its two ends is inserted** — that sets both how deep it cuts and whether the edge is angled. Each blade also has its own rotation angle, and the whole frame assembly has a rotation of its own.

In a fixture definition these are four capabilities:

| Capability | Effect |
| --- | --- |
| Blade Insert | How far one end of this blade is inserted |
| Blade Corner B | How far the other end is inserted (unequal ends give an angled cut) |
| Blade Rotate | This blade's own angle |
| Shaper Rotate | Rotation of all four blades together with the frame assembly |

The first three all need a **Blade Index** (1–4) in the binding's Params to say which blade is being driven.

---

## 2. Blade Order

```text
        ┌───────┐
      2 │       │ 4
        └───────┘
   1 = bottom   2 = left   3 = top   4 = right
```

| Blade | Position |
| ---: | --- |
| 1 | Bottom |
| 2 | Left |
| 3 | Top |
| 4 | Right |

A blade's two ends are not "left and right" or "top and bottom" — they are **its own anticlockwise end (A) and clockwise end (B)**. Opposite blades therefore have their A/B ends mirrored on screen: blade 3 (top) has A on the left, blade 1 (bottom) has A on the right. Bind `Blade Insert` to the A-end channel and `Blade Corner B` to the B-end channel and the angled cuts come out the right way round.

**Blade assembly rotation and gobo rotation are separate**, driven by the `Shaper Rotate` and `Gobo Rotate` capabilities respectively.

---

## 3. Notes

- Framing requires the corresponding emitter in the fixture definition to use a framing beam kind (this component corresponds to `Shaper / Profile (Legacy)`);
- If the channel library has no framing channels, or no binding connects them, console values will not drive these parameters — check the Validation page in the fixture editor;
- The framing shape in previs is for visual communication; it does not match the mechanical precision of a real fixture's shutters.

---

## 4. Related Documents

- [Light Components](00_LightComponent_Overview_en.md)
- [SuperVolumetricShaperComponent](12_SuperVolumetricShaperComponent_en.md) (use this instead)
- [SuperBeamComponent](03_SuperBeamComponent_en.md)
- [The Fixture Definition Asset](../fixture/01_FixtureDefinition_en.md)
