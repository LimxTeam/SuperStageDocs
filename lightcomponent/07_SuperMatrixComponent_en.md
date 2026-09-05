# SuperMatrixComponent

SuperMatrixComponent is used by segmented matrix lights. It manages the matrix mesh, matrix material, segment count, segment direction, per-segment colour, intensity, strobe, and optional **real per-segment lights** (always spot lights — there is no point-light option).

## Parameters

| Parameter | Purpose |
| --- | --- |
| SegCount | Segment count, written into the material parameter; indexes are clamped to 200. |
| bUseUAxis | true segments along U, false along V. |
| StaticMeshMatrix | Mesh that carries the matrix material. |
| bTransparent | Whether to use the transparent matrix material. |
| MatrixTransform | Relative transform of the matrix mesh. |
| SegmentLightRadius | Attenuation radius for segment lights. |
| LightSpotDefaults | The whole default parameter group for segment lights, mapped in from the fixture definition's light defaults. **LightSpotIntensity** is the segment lights' intensity multiplier and **Angle** is the outer cone angle. Segment lights are always spot lights. |

> **VolumetricScattering / LightShadow / AffectTransmission** in `LightSpotDefaults` apply **only to this segmented-matrix light path**: every other component uses the in-house cone light, which does not support them.

## Notes

- Segment colors are written through material parameters; visible output depends on the matrix material.
- Enabling real segment lights increases the light count. Final performance depends on UE project settings and hardware.
- After changing the segment count, make sure the asset has rebuilt the corresponding segment lights.
