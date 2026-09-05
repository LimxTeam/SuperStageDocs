# SuperEffectComponent

SuperEffectComponent is used by strip lights, effect planes, and other material-driven light effects. In source, it manages a static mesh, opaque/transparent materials, intensity, strobe, color, direction, effect index, speed, and width.

## Parameters

| Parameter | Purpose |
| --- | --- |
| MaxLightIntensity | Maximum component brightness percentage. |
| StaticMeshEffect | Mesh that carries the effect material. |
| EffectMaterialNo | Material slot index. |
| bTransparent | Whether to use the transparent effect material. |
| EffectTransform | Relative transform of the effect mesh. |
| Intensity / Strobe / Color | Controls brightness, strobe, and color. |
| Direction / Effect / Speed / Width | Written to material parameters for direction, effect index, speed, and width. |

## Notes

- Visible behavior depends on whether the assigned material reads these parameters.
- Strobe is calculated in source as a brightness multiplier by mode; speed 0 does not create flashing changes.
- If the effect plane is not visible, check mesh, material slot index, transparent material, and intensity first.
