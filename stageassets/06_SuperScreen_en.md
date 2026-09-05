# Super Screen User Notes

Super Screen displays the current media texture on one or more StaticMeshActors. In source, it creates a unique dynamic material instance and writes the active texture, color, brightness, contrast, opacity, and keystone parameters into that material.

## Basic Use

1. Place or prepare StaticMeshActors that will act as screen surfaces.
2. Place Super Screen.
3. Add target StaticMeshActors to `ScreenMeshActors`.
4. Set `MaterialIndex` and confirm the target mesh has that material slot.
5. Choose a media source and adjust visual parameters as needed.

## Parameters

| Parameter | Purpose |
| --- | --- |
| ScreenMeshActors | StaticMeshActors that receive the screen material. |
| MaterialIndex | Material slot index for applying the dynamic material; targets with out-of-range slots are skipped. |
| Transparent | Switches between opaque and transparent source materials. |
| Transparency | Writes the Opacity material parameter in transparent mode. |
| Brightness | Writes the Brightness material parameter. |
| Color | Writes the Color material parameter. |
| Contrast | Writes the Contrast material parameter. |
| Deformation | Four-corner keystone parameters, written to eight UV offset values in the material. |

## Notes

- Super Screen does not create an LED screen model by itself; it applies a material to the StaticMeshActors you assign.
- Each Screen Actor creates its own dynamic material instance to avoid sharing one MID after copy/paste.
- Image updates depend on the media source providing a valid active texture.
- Keystone correction is for visual alignment in previz and does not guarantee exact matching to real LED pixel boundaries.
