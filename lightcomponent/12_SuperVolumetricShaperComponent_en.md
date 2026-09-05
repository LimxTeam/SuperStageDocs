# SuperVolumetricShaperComponent (Volumetric Framing)

> Applies to SuperStage 26H2.6 and later

Volumetric framing = [volumetric beam](11_SuperVolumetricBeamComponent_en.md) + four-blade framing. It inherits the volumetric beam, so that component's whole parameter group applies.

Its relationship to the older [SuperShaperComponent](04_SuperShaperComponent_en.md) is the same as the volumetric beam's relationship to the material beam: the same beam, plus one extra shader that multiplies in the trapezoid mask. **Use this one for new content, not the Legacy one.**

---

## 1. Blade Parameters

Four blades, two corners each, plus overall assembly rotation.

| Capability | Description |
| --- | --- |
| Blade Insert | Blade insertion depth |
| Blade Corner B | The blade's other corner |
| Blade Rotate | Angle of an individual blade |
| Shaper Rotate | Rotation of the whole blade assembly |

Blade order: **1 = bottom, 2 = left, 3 = top, 4 = right**.

**Blade assembly rotation and gobo rotation are separate**, driven by `Shaper Rotate` and `Gobo Rotate` respectively.

---

## 2. Relationship to the Column

The framing shader and the beam column itself are **genuinely separated**: a volumetric beam without blades compiles with no blade instructions at all. So choosing a volumetric beam for an ordinary beam fixture and volumetric framing for a profile fixture does not make either pay for the other.

---

## 3. Notes

- Requires the corresponding emitter in the fixture definition to use the `Volumetric Shaper (Profile)` beam kind;
- The channel library needs blade channels, with bindings connected to the four capabilities above — check the Validation page in the fixture editor;
- On GDTF import, **fixtures with blade channels default to the Legacy `Shaper / Profile`** and must be changed to `Volumetric Shaper (Profile)` by hand;
- The framing shape in previs is for visual communication; it does not match the mechanical precision of a real fixture's shutters.

---

## 4. Related Documents

- [Light Components](00_LightComponent_Overview_en.md)
- [SuperVolumetricBeamComponent](11_SuperVolumetricBeamComponent_en.md)
- [SuperShaperComponent (Legacy)](04_SuperShaperComponent_en.md)
- [The Fixture Definition Asset](../fixture/01_FixtureDefinition_en.md)
