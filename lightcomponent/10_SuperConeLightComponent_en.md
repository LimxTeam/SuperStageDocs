# SuperConeLightComponent (In-House Cone Light)

> Applies to SuperStage 26H2.6 and later

SuperConeLightComponent is the in-house cone light pipeline added in 26H2.6. It **replaces the UE SpotLight** as the fixture key light, and all three beam pipelines (volumetric, ray beam, material) are built on top of it.

---

## 1. What It Handles

| Content | Description |
| --- | --- |
| Floor / surface pool | The patch of light the fixture casts on scene surfaces |
| Gobos | Gobo wheel projection |
| Colour wheels | Colour wheel projection, including scrolling |
| Framing | Four-blade framing |
| Prism | Prism splitting |

**The aerial column is not its job.** Whether and how the column is drawn is decided by the emitter's beam kind (see section 4.2 of [The Fixture Definition Asset](../fixture/01_FixtureDefinition_en.md)).

---

## 2. Why Replace the UE SpotLight

- One fewer engine light, one fewer component registration and one fewer light function material instance per fixture;
- The column and the floor pool **share one set of optics and one time base**, so their colour and angle cannot disagree;
- Gobos, colour wheels, framing and prisms are projected through the same shader path as the column.

---

## 3. What the User Sees

- The pool changes with intensity, colour, Zoom, Frost and Iris;
- Gobo and colour wheel scrolling, rotation and shake;
- Prism splitting consistent with the column;
> **Aperture glow** — the bright ring at the light exit — is not the cone light's job. It belongs to the [ray beam](13_SuperRayBeamComponent_en.md) pipeline, and only switches on when the lens mesh is stripped.

---

## 4. Occlusion

The cone light and the volumetric beam **share one shadow atlas and one sampling function**. Which means:

- Whatever blocks the column also blocks the floor pool;
- Previously only the column was occluded and the floor pool was not, which appeared as "the beam is blocked but the pool is not".

---

## 5. Notes

- Users control it indirectly through the fixture definition, channel library and DMX channels; it rarely needs direct adjustment;
- Whether the pool is visible also depends on UE exposure, render settings and the scene surface materials;
- The lighting cone angle follows the column and is linked to the zoom channel.

---

## 6. Tuning and Diagnostic Switches

These console variables take effect at runtime — change one and you see it immediately, no restart. You do not need them day to day; they are for troubleshooting and for trimming cost.

### 6.1 Tuning

| Variable | Default | What it does |
| --- | ---: | --- |
| `r.SuperConeLight` | 1 | Cone light master switch. 0 = off, 1 = on |
| `r.SuperConeLight.BrightnessScale` | 1.0 | Global brightness calibration multiplier |
| `r.SuperConeLight.SpecularScale` | 1.0 | Specular strength; 0 is pure diffuse. In previs, specular exists only to give metal parts a highlight |
| `r.SuperConeLight.MinScreenRadiusPx` | 2.0 | Lights whose projected radius falls below this pixel count are culled. Cost is proportional to the pixels the cone covers, so drawing a full cone for something two pixels wide is not worth it |
| `r.SuperConeLight.ZoomRefAngle` | 0.0 | Reference cone angle for zoom brightness (full angle, degrees). **0 means cone angle does not affect brightness** (default); a positive value enables luminous-flux conservation, so narrowing the beam makes it brighter |

### 6.2 Diagnostics

| Variable | Default | What it does |
| --- | ---: | --- |
| `r.SuperConeLight.DebugMode` | 0 | 1 = overdraw heat map. Each light adds a fixed amount, so the sum is the overdraw depth |
| `r.SuperConeLight.GateOrientation` | 0 | Symmetry transform applied to gate-space UV: 0 as-is, 1 flip U, 2 flip V, 3 swap UV. Use it to pin down a gobo or colour wheel that comes out the wrong way round on the floor |
| `r.SuperConeLight.FlipCull` | 0 | Flips face culling on the cone proxy. By default front faces are culled and back faces rasterized, so the light still renders with the camera inside the cone; use this to diagnose "only visible from inside the cone" on another platform |

### 6.3 Calibration Offsets

Both values are measured and calibrated. **Do not change them without a solid reason:**

| Variable | Default | Description |
| --- | ---: | --- |
| `r.SuperConeLight.PrismRotationOffset` | 90.0 | Angle offset for prism facet layout and spin. Only the prism needs it — gobo orientation is already correct |
| `r.SuperConeLight.BladeRotationOffset` | 0.0 | Gate-space UV angle offset for blade framing, matching console calibration |

---

## 7. Related Documents

- [Light Components](00_LightComponent_Overview_en.md)
- [SuperSpotComponent](02_SuperSpotComponent_en.md)
- [SuperVolumetricBeamComponent](11_SuperVolumetricBeamComponent_en.md)
- [The Fixture Definition Asset](../fixture/01_FixtureDefinition_en.md)
