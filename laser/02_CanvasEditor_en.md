# Laser Canvas Editor

> Applies to SuperStage 26H2.6 and later ｜ Read first: [Laser Animation Asset](01_LaserAnimationAsset_en.md)

Double-click a laser animation asset to open the editor.

---

## 1. Interface

| Position | Tab | Content |
| --- | --- | --- |
| Left ~20% | **Layers** | Layer list: add, remove, reorder, show/hide, lock |
| Middle ~52% | **Canvas** | The canvas |
| Right ~30% (Layer in front) | **Layer** | Shape parameters for the current layer |
| | **Effects** | The effect stack |
| | **Details** | **Asset-level settings**: scan profile, tempo, preview devices, export settings |

Day-to-day shape work happens on the **Layer** page; **the scan profile and preview devices live on the Details page** — that is where to look when you cannot find point count or point rate.

The strip under the canvas is a **transport bar** (play / stop / clock readout / tools / device dropdown / frame rate / display toggles), not a timeline — it has no scrubber. See [Animation Asset](01_LaserAnimationAsset_en.md), section 6.

---

## 2. The Three Tools

Three tool buttons at the top of the canvas, each with a shortcut (matching Illustrator / Photoshop):

| Tool | Shortcut | Purpose |
| --- | --- | --- |
| **Select** | `V` | Pick and drag layers |
| **Nodes** | `A` | Drag path points and their handles |
| **Pen** | `P` | Click to lay down path points |

`F` also **resets the view**.

---

## 3. Canvas Operations

| Operation | Description |
| --- | --- |
| **Middle-mouse drag** | Pan the canvas |
| Wheel | Zoom |
| `F` | Reset the view (zoom back to 1, pan to zero, fitting the window) |

The device dropdown on the transport bar selects **All Devices** (ignore device routing and draw every layer) or one device, so you can see what that device actually projects. It **only appears when there is more than one preview device, or the content contains per-device chase effects**.

> The canvas draws the scan field itself — a constant `[-1, 1]` square — not one device's real-world projection area.

---

## 4. The Select Tool

| Operation | Description |
| --- | --- |
| Click | Select a layer |
| **Ctrl / Shift click** | Add to the selection, as in the Content Browser and outliner |
| Drag | Move the selected layers |
| Transform handles | Corners scale freely; **edge-midpoint handles scale one axis only** |
| **Shift + corner handle** | Uniform scale, so the shape does not distort |

> Clicking a layer that is **already selected** does not clear the rest of the selection — otherwise multi-selecting and then dragging would collapse to one layer the moment you pressed down.

---

## 5. The Nodes Tool

Layer content is cubic Bezier curves, with handles stored as offsets relative to the node, so handles follow when a node is dragged.

| Operation | Description |
| --- | --- |
| Drag a node | Change the shape |
| Drag a handle | Change curvature. Both handles move **symmetrically** by default |
| **Alt + drag a handle** | **Breaks the symmetry**, so each handle moves on its own |
| **Ctrl + click empty space** | **Inserts a node on the nearest segment** |
| Drag in empty space | Marquee select |
| **Ctrl + A** | Select every node in the current layer |
| **C** | **Toggle the corner marker** (section 7) |
| **Delete** / **Backspace** | Delete selected nodes |

**Deleting several nodes merges into one undo.** Switching layers clears edit state bound to indices, so it does not leak into the new layer.

> **Alt only breaks handle symmetry; it does not touch the corner marker.** Combining the two would mean Alt-dragging a handle also marks the point as a corner — and a corner suppresses handles, so you would drag for a while with nothing happening.

---

## 6. The Pen Tool

Click to lay down path points one at a time. **`Esc` finishes the stroke** — use it when you want to start a separate contour, otherwise every click keeps extending the same one.

---

## 7. Corner Markers

The corner marker on a node is **not decoration** — it genuinely affects the bake. A node marked as a corner is allocated dwell samples during baking, giving the galvo time to turn. Unmarked nodes are treated as smooth transitions and the corner comes out rounded.

---

## 8. Text Layers

Text takes **Bezier curves directly from font outlines**, with no bitmap in between. So:

- Scaling stays crisp;
- It can be converted to a path and edited per node;
- The point budget is allocated by the arc length of the glyph outlines, not by pixels.

Multi-line text layers copied and pasted across assets are not cut in half (the clipboard uses Base64 encoding).

---

## 9. Layer Panel and Toolbar

**The Layers page**: a layer list with add, remove, reorder, show/hide and lock. **Layers can be copied and pasted across assets** (the clipboard uses Base64, so multi-line text is not cut in half).

The **main toolbar** has just three buttons:

| Button | Purpose |
| --- | --- |
| **SVG** | Imports an SVG / Illustrator path as a new layer. **It reads the vector data straight out of the file — nothing is imported into the project** |
| **Import ILDA** | Reads a `.ild` file. **A single frame becomes an editable shape; an animation keeps every frame and loops at its own rate** |
| **Export ILDA** | Bakes to a `.ild` file. It asks how many frames first — **this asset has no length of its own**, so one frame gives a still and a sequence gives an animation |

**Add Layer** sits on the layer list's own toolbar; **To Path** (turn a primitive or text into a per-node editable path) and the device dropdown sit on the transport bar below the canvas.

**The five Add Layer groups**:

| Group | Contents |
| --- | --- |
| **Import** | SVG, ILDA, Trace 3D Mesh |
| **Basic** | Circle, Ellipse, Rectangle, Polygon, Star, Line, Arc, Cross |
| **Curves** | Spiral, Lissajous, Wave, Rose Curve, Heart, Grid |
| **Beams** | Single Beam, Beam Fan |
| **Text** | Glyph outlines from a font; edit the string and it regenerates immediately |

**Trace 3D Mesh** traces a static mesh's outline from **a view angle you can keep turning**. It uses the mesh selected in the Content Browser, if there is one.

> Two menu labels differ slightly from the primitive names: `Rose Curve` is Rose, and `Single Beam` is Beam Dot.

---

## 10. Curve Subdivision

A layer's Curve Detail (4–256) controls curve subdivision, and changes genuinely reach the evaluated result. Higher subdivision gives a more accurate shape and spends more of the point budget.

---

## 11. Transport Bar

The transport bar **wraps**: on a narrow canvas the controls fold onto the next row rather than being pushed out of view.

**Import and export** are on the asset editor's main toolbar.

---

## 12. Related Documents

- [Laser Animation Asset](01_LaserAnimationAsset_en.md)
- [Effect System](03_Effects_en.md)
- [Baking and ILDA](04_Baking_and_ILDA_en.md)
