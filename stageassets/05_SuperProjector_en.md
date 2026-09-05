# Super Projector User Notes

Super Projector is used for projection mapping previz in a UE scene. In source, it uses three RGB SpotLights with Light Function materials. The active texture comes from a static texture or a director camera RenderTarget.

## Basic Use

1. Place Super Projector in the scene.
2. Choose a static texture or director camera source in the media source settings.
3. Adjust projector location and rotation.
4. Tune projection parameters in the Details panel.

## Parameters

| Parameter | Purpose |
| --- | --- |
| Dimmer | Projection brightness. Written to the three SpotLight Intensity values, using Candelas. |
| MaxLightDistance | Projection distance. Source reads it as meters and multiplies by 100 for SpotLight attenuation radius. |
| Zoom | Projection angle, clamped to 10-100 degrees. |
| DilutionFactor | Edge softness factor used to calculate InnerConeAngle. |
| Deformation | Four-corner keystone parameters, written to eight UV offset values in the material. |

## Notes

- Projection depends on the built-in `M_Mapping` Light Function material.
- Image clarity and brightness depend on UE lighting, exposure, material, projection distance, and scene surfaces.
- Keystone correction is for previz alignment and does not guarantee exact optical geometry of a real projector.
- If the media source has no texture, the projection material will not show valid content.
