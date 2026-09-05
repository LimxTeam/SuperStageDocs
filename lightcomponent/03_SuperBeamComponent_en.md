# SuperBeamComponent (Material Beam · Legacy)

> Applies to SuperStage 26H2.6 and later
>
> **This component belongs to the old material pipeline.** The shipped fixture library is fully migrated to the [volumetric beam](11_SuperVolumetricBeamComponent_en.md); the material beam is kept for fallback and is not recommended for new content.
>
> Note: **a fixture newly imported from GDTF still gets the material beam on its main emitter** and has to be switched to a volumetric beam kind in the fixture editor.

SuperBeamComponent draws a **column painted onto a mesh** — a cone mesh carrying the beam material. It is a separate backend from the [volumetric beam](11_SuperVolumetricBeamComponent_en.md). On top of the spot component it adds the beam mesh, beam material, gobo/texture support, colour wheels, prism, focus, atmospheric density and beam disable control.

## Visible Results

- Dimmer, color, and strobe affect beam brightness.
- Zoom, Frost, and Iris affect beam and spot material parameters.
- Gobo texture, gobo count, index, rotation speed, and shake speed are written to the beam material.
- Prism uses a SuperPrismPreset and a selected prism layer.
- Focus uses 0 to 1 from sharp to blurred.
- Beam Disabled hides this column but does not remove the fixture.

## User Checks

| Issue | Check |
| --- | --- |
| No beam | Check Beam Disabled, intensity, color, material, and UE rendering settings. |
| Gobo not visible | Check atlas resource, gobo count, and fixture library channels. |
| Prism not visible | Check that a SuperPrismPreset is assigned and a valid layer is selected. |
| Block distance looks wrong | Check scene collision and visibility channels. |

## Boundary

The volumetric beam is a previz visual effect. It does not replace real haze, actual fixture output, or physical measurement.
