# SuperLightingComponent

SuperLightingComponent is the base light component used by fixture assets. It handles what every light-emitting component shares: the lens mesh and lens material, intensity, colour, strobe and visibility.

**The floor pool is not at this level** — that belongs to the [cone light](10_SuperConeLightComponent_en.md).

## What Users See

- Lens mesh and lens material display. The lens mesh is set by **StaticMeshLens**, and its position / rotation / scale by **LensTransform**; both are set per fixture model, not on the scene instance.
- Lens material changes driven by intensity, colour and related parameters.
- Strobe mode affects final brightness; strobe speed 0 stays open.
- When colour is nearly black or intensity is 0, the visible light effect is turned off.

## Common Parameters

| Parameter | Purpose |
| --- | --- |
| Dimmer / Intensity | Controls component brightness. |
| Color | Controls lens and spot color. |
| Strobe | Controls strobe speed. |
| StrobeMode | Controls strobe behaviour: Closed, Open, Linear, Pulse, Ramp Up, Ramp Down, Sine and Random. |
| Zoom / Frost / Iris | At this level these are interfaces only; they take effect in subclasses such as the spot component — see [02](02_SuperSpotComponent_en.md). |
| Texture | Used for spot or pattern texture. |
| Visibility | Controls whether the component effect is visible. |

## Notes

- Normal users control this component through fixture assets, Fixture Library definitions, and DMX channels.
- Not every fixture exposes every parameter. If a channel or resource is not configured, the related effect may not visibly change.
- Beam block distance depends on scene collision and visibility channel settings.
