# SuperVolumetricBeamComponent (Volumetric Beam)

> Applies to SuperStage 26H2.6 and later

The volumetric beam is the **default backend** for the aerial column. The shipped fixture library is fully migrated to it.

It shares one shadow atlas and one set of optics with the [cone light](10_SuperConeLightComponent_en.md), so the column and the floor pool are occluded consistently by the same occluders and their colour and angle cannot disagree.

---

## 1. Parameters

The **VolumetricBeamDefaults** group in the fixture definition. All native physical quantities, with no legacy conversion factors.

The same parameters can also be edited directly on the beam component in the level: select it and open the **B.BeamParameter** group in the details panel. Changes take effect immediately, which makes A/B comparison easy. Note that **the fixture's DMX channels override the matching fields** — once a console is attached, the values driven by channels (intensity, iris and so on) come from the channel.

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| **Disable Beam** | Off | — | Hides the volumetric beam, keeping the light source and pool. **Not the same as deleting the fixture** |
| **Beam Occlusion** | On | — | Dynamic occlusion by real geometry |
| **Beam Intensity** | 1.0 | 0–10 | Beam intensity multiplier |
| **Density** | 0.36 | 0.05–10 | Medium density (haze concentration multiplier). A measured calibration value — a density of 1.0 smears the beam into a solid rod and buries all detail |
| **Haze Amount** | 0.6 | 0–1 | Haze noise amount: how visible the drifting haze inside the beam is |
| **Haze Scale** | 1.3 | 0.01–10 | Haze noise scale (1/m). 1.3 ≈ sub-metre wisps, measured as the closest to real stage haze |
| **Haze Speed** | 25 | 0–200 cm/s | Haze drift speed, 0 = static. **New parameter in 26H2.6** — previously the flow speed was locked to wisp size and too slow to be visible |
| **Lens Radius** | 8 | 0.5–100 cm | Beam start radius. Iris scales down from this baseline |
| **Falloff** | 1.0 | 0–1 | Axial falloff, 0 = none, 1 = zero at the far end. Combined with the low 0.36 density, full falloff gives natural dissipation along the beam rather than a hard cut |
| **Edge Softness** | 1.0 | 0–1 | Edge softness, 1 = softest. Beams in real haze have no hard edge |
| **Near Fade** | 0 | 0–2000 cm | Near fade-in distance. 0 = full brightness at the aperture |
| **Far Fade Fraction** | 0.12 | 0–1 | Far fade-out as a fraction of beam length |
| **Distance Fade End** | 0 | 0–500000 cm | Camera distance fade end. **0 = no distance limit, and 0 must be the default** |

> **About Distance Fade End**: it used to default to 80000 (800 m), past which the whole beam was dropped during culling, which showed up as "the camera can clearly see it but the beam is gone". Stage previs is not an open world, the scene is small anyway, and distance culling only produces that unexplained disappearance. To limit distance, fill it in per fixture.

---

## 2. Occlusion

Beams with Beam Occlusion enabled are given a **shadow atlas slot**. Each slot rasterises the occluders inside the cone into a distance map from the fixture's viewpoint; the beam shader samples it per segment with trapezoidal weighting, so cuts are smooth rather than hard-edged. The atlas is rebuilt every frame when occluders or beams move, so occlusion is fully dynamic.

**Slots are not queued by camera distance.** The candidate pass does one thing: drop what is off screen. Every fixture that is visible, lit and asking to cast gets in, so "no shadow when far from the camera" does not occur.

Two cases still end up with no shadow: **global quality dropped to Low** skips the shadow pass entirely, and fixtures beyond the atlas capacity get no slot (this is logged).

Occluder discovery relies on physics scene queries (QueryOnly collision is enough). Skeletal meshes are projected with capsule proxies.

---

## 3. Optical Elements

Gobos, colour wheels and prisms are projected through the same shader path as the cone light, so the column and the pool come from one source.

**Gobo edges**: per-segment footprints use anisotropic sampling, fixing soft-edged gobos turning into hard angular edges at side camera positions. Dithering is on by default to suppress concentric rings.

> **Gobo atlas textures must carry mipmaps** (Mip Gen Settings ≠ NoMipmaps, power-of-two dimensions). A texture without mips defeats every anti-aliasing strategy; the subsystem warns about it.

---

## 4. Quality Tiers

Four tiers: Low / Medium / High / Ultra. They differ in the number of occlusion samples per beam and whether the half-resolution path is used.

Actual frame rate depends on beam count, cone angle, camera distance and hardware — wide-angle beams close to the camera are fill-rate bound, and half resolution plus screen-size culling are the main levers. This document promises no fixed frame rate.

---

## 5. Notes

- Users normally control it indirectly through the fixture definition and DMX rather than adjusting it directly;
- When there is no beam, check Disable Beam, intensity and colour first, then UE render settings;
- The volumetric beam is a previs visual effect. It **does not replace real haze, real fixture output or physical measurement**.

---

## 6. Related Documents

- [Light Components](00_LightComponent_Overview_en.md)
- [SuperConeLightComponent](10_SuperConeLightComponent_en.md)
- [SuperVolumetricShaperComponent](12_SuperVolumetricShaperComponent_en.md)
- [The Fixture Definition Asset](../fixture/01_FixtureDefinition_en.md)
