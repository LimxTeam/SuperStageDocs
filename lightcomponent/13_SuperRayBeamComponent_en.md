# SuperRayBeamComponent (Ray Beam)

> Applies to SuperStage 26H2.6 and later

The ray beam is a **volumetric beam without optical elements**: no gobos, no colour wheel, no prism, no blades.

It exists for matrix and effect fixtures. Those carry dozens or hundreds of emitting units on one fixture, each a component — the ray beam always takes the zero-sample closed-form path, which costs far less than per-pixel stepping and is not affected by sampling rate artefacts. And this class of fixture does not project patterns in the first place.

---

## 1. When to Use It

| Situation | Choose |
| --- | --- |
| Columns on matrix fixtures, pixel bars, effect fixtures | **Ray Beam** |
| Patterns needed | [Volumetric beam](11_SuperVolumetricBeamComponent_en.md) |
| Framing needed | [Volumetric framing](12_SuperVolumetricShaperComponent_en.md) |
| Colour / blinder / strobe with no aerial column | [Wash](14_SuperWashComponent_en.md) |

On GDTF import, matrix / pixel groups are assigned Ray Beam **automatically**; groups declared `BeamType=Glow/None` in the package (aura rings, backplates) are assigned Wash and emit no column.

---

## 2. Parameters

It reuses the volumetric beam's default value group (density, haze, edge softness, fades and so on) with the same meanings. See section 1 of [SuperVolumetricBeamComponent](11_SuperVolumetricBeamComponent_en.md).

> **Beam Occlusion has no effect on the ray beam.** That option is implemented only on the volumetric beam pipeline; the ray beam reuses the same parameter struct for its defaults but has no occlusion implementation.

---

## 3. Performance Characteristics

- A constant zero-sample closed-form integral; samples do not grow with the quality tier;
- Produces no extra primitives;
- The hot path carries dirty checks, so nothing is written when nothing changed.

Those three together are why a matrix fixture can carry dozens or hundreds of emitting units.

---

## 4. Related Documents

- [Light Components](00_LightComponent_Overview_en.md)
- [SuperVolumetricBeamComponent](11_SuperVolumetricBeamComponent_en.md)
- [SuperWashComponent](14_SuperWashComponent_en.md)
- [SuperMatrixComponent](07_SuperMatrixComponent_en.md)
