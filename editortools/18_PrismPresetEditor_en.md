# Prism Preset Editor User Manual

## 1. Scope

The Prism Preset Editor edits SuperStage Prism Preset assets. One preset contains 3 prism layers, mapped to Prism 1, Prism 2, and Prism 3. Each layer contains multiple split points and has global scale, position scale, rotation, and offset settings.

Whether a fixture displays these prism effects depends on whether the fixture asset references the preset and whether the fixture library Prism slot selects the matching Prism layer.

## 2. Create and Open

1. Create a **Prism Preset** asset in the Content Browser.
2. Double-click the asset to open the editor.
3. Select Prism 1, Prism 2, or Prism 3 in the editor.

## 3. Canvas Operations

The left canvas shows the split points for the active layer.

| Operation | Description |
| --- | --- |
| Drag a split point | Moves the point |
| Ctrl + click | Adds to or removes from selection |
| Box select | Selects multiple points |
| Delete | Deletes selected points |
| Add Facet | Adds one split point |
| Delete Selected | Deletes selected split points |

Each layer supports up to 48 split points. When the limit is reached, no more points can be added.

## 4. Layer Parameters

| Parameter | Description |
| --- | --- |
| Enabled | Enables the current prism layer |
| Layer Name | Name of the current layer |
| Facet Size Scale | Scales all split point sizes |
| Position Scale | Scales all split point distances from center |
| Global Rotation | Rotates the current layer, in degrees |
| Global Offset | Moves the current layer |

Each point also has Position and Scale. Position is the XY offset from the center. Scale controls that point's size.

## 5. Arrangement (a live parameter from 26H2.6)

Arrangement is no longer a one-shot template that fills in a batch of points. It is a **live parameter**: facet count, radius and facet size all recompute as you change them.

| Arrangement | Description |
| --- | --- |
| **Manual (hand-placed)** | Hand-placed facets; the facet array is the final result. The default on existing assets, which keeps hand-placed content as it is |
| **Ring** | N evenly spaced facets on a ring. The most common form |
| **Ring + Centre** | A ring plus one facet in the centre, usually larger and brighter |
| **Double Ring** | Inner and outer rings, each with its own facet count and radius |
| **Line** | A single row (linear prism) |
| **Concentric Rings** | **Any number of concentric rings**, with an optional centre facet |

**Concentric Rings** is new in this release and the recommended choice for new prisms: three rings, five rings, a different facet count per ring, a phase offset per ring — none of it has to be placed by hand. Per-ring parameters:

| Per-ring parameter | Range |
| --- | --- |
| Facet Count | 2–48 |
| Phase | 0–360° |
| Size Scale | 0.05–4.0 |
| Position Scale | 0–4.0 |
| Radius | 0–0.5 |
| Intensity | 0–4.0 |

Ring / Ring + Centre / Double Ring are all special cases of Concentric Rings; they are kept because existing assets serialise them by enum name.

> The facet count, radius and facet size are independent, so changing a 6-facet layout to 8 keeps everything else you set.


## 6. Preview and Save

The editor refreshes preset data after asset changes. Use the Unreal Editor save command to save the asset.

The Content Browser shows a thumbnail for Prism Preset assets to help identify different presets.

## 7. Export Position Texture

The editor provides position texture export:

| Button | Description |
| --- | --- |
| Export Current Layer | Exports the position texture for the current enabled layer |
| Export All Enabled Layers | Exports all enabled layers that contain split points |

Exported textures are for material workflows that need direct texture references. For ordinary fixture use, referencing the Prism Preset asset is usually enough.

## 8. Use in Fixtures

1. Reference the Prism Preset in the fixture asset or fixture configuration.
2. In the fixture library Prism slot, select Prism 1, Prism 2, or Prism 3.
3. When console or DMX data triggers that slot, the fixture uses the matching prism layer.

## 9. Notes

- Prism 2 and Prism 3 may be disabled by default. Enable them or apply a template before use.
- Empty layers do not provide usable split points.
- Templates replace existing split points in the current layer. Duplicate the asset first if you need to keep the old pattern.
- Final visual output also depends on fixture materials, beam components, and the fixture's prism control logic.
