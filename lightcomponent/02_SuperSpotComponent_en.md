# SuperSpotComponent

SuperSpotComponent provides the spot used by fixtures. It is built on the [in-house cone light](10_SuperConeLightComponent_en.md) — from 26H2.6 the key light is projected by the cone light, not by a UE SpotLight. It synchronises intensity, colour, Zoom, Frost, Iris, rotation and visibility to the cone light and related materials.

## Visible Results

- Zoom affects the spotlight cone angle through a calibration table.
- Iris reduces the current cone angle and does not expand it beyond the current base angle.
- Frost widens the outer cone so the edge reads as more diffuse; it takes effect whenever the fixture has a Frost channel wired up.
- Color, intensity, and strobe affect the actual spotlight output.

## User Checks

| Issue | Check |
| --- | --- |
| Cone angle does not change | Confirm the fixture has a Zoom or related control channel. |
| Softness is not visible | Confirm the fixture's channel library has a Frost channel bound to the Frost capability. |
| Material changes but lighting is weak | Check scene exposure and the asset's intensity settings. |

## Boundary

This document does not promise that every moving-light asset has Zoom, Frost, or Iris. Actual capability depends on the selected fixture asset and fixture library.
