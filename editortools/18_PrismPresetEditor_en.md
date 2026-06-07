# Prism Preset Editor — User Manual

## 1. Overview

The **Prism Preset Editor** is a visual prism pattern editing tool provided by SuperStage. It allows you to directly drag the position and size of prism split points on a 2D canvas, preview the prism effect in real time, and save the results as a **prism preset data asset** (USuperPrismPreset) for use by fixtures.

### Core Capabilities

- **2D Visual Dragging** — Drag prism points directly on the canvas
- **Three Independent Layers** — Each prism preset supports 3 layers, with up to 48 split points per layer
- **Multi-Select Operations** — Box-select or Ctrl+click for multi-select; batch move/delete
- **Preset Templates** — 11 built-in templates: Circle, Line, Triangle, Square, Pentagon, Hexagon, Star, Cross, Diamond, Arc, Honeycomb
- **Precise Numeric Editing** — Position (X/Y) and scale of each point can be precisely entered
- **Preset Regeneration Generator** — Generate regular patterns quickly based on templates
- **3D Preview Viewport** — View the actual prism effect in lighting in real time
- **Thumbnail Rendering** — Preset thumbnails displayed in the Content Browser

---

## 2. Creating a Prism Preset

### Method 1: Content Browser Right-Click Creation

1. Right-click in the Content Browser → **Miscellaneous** → **Prism Preset**
2. Name the new asset
3. **Double-click** to open the Prism Preset Editor

### Method 2: Using Built-in Templates

1. After creating a blank preset, open the editor
2. Click a template button in the preset panel (Circle / Triangle / Square, etc.)
3. The template automatically fills the split points for the current layer

---

## 3. Editor Interface

The editor uses a **left-right layout**:

### Left — 2D Canvas

- **Reference Circle** — Circular boundary at the center of the canvas; prism points cannot exceed this range
- **Prism Points** — Circular markers, draggable to reposition
- **Point Size** — Reflects the Facet.Scale value

#### Canvas Operations

| Operation | Function |
|-----------|----------|
| Mouse drag prism point | Move position |
| Ctrl + Click | Multi-select / deselect |
| Box-select | Batch select |
| Delete | Delete selected points |

### Right — Properties Panel

- **Layer Selection** — Switch between Layer 0 / 1 / 2
- **Template Buttons** — One-click generation from 11 preset templates
- **Point List** — Precise numeric values for all split points in the current layer
- **Global Parameters** — Global scale, etc.

---

## 4. Coordinate System

The Prism Preset Editor uses **UV space** coordinates:

| Dimension | Range | Meaning |
|-----------|-------|---------|
| X | -0.5 ~ +0.5 | Horizontal position |
| Y | -0.5 ~ +0.5 | Vertical position |
| Scale | 0.0 ~ 1.0 | Split point size |

- The coordinate system is consistent with the shader UV coordinate system
- Circular constraint ensures points do not exceed the reference circle boundary

---

## 5. Preset Templates

| Template | Point Count | Description |
|----------|-------------|-------------|
| **Circle** | Adjustable | Circular arrangement |
| **Line** | Adjustable | Linear arrangement |
| **Triangle** | 3 | Triangle |
| **Square** | 4 | Square |
| **Pentagon** | 5 | Pentagon |
| **Hexagon** | 6 | Hexagon |
| **Star** | 5 | Star shape |
| **Cross** | 4 | Cross shape |
| **Diamond** | 4 | Diamond shape |
| **Arc** | Adjustable | Arc arrangement |
| **Honeycomb** | 7 | Honeycomb structure (1 center + 6 surrounding) |

---

## 6. Technical Architecture

### Position Lookup Texture

Each prism preset generates a **48×1 RGBA16F** position lookup texture:

| Channel | Meaning |
|---------|---------|
| R | X offset |
| G | Y offset |
| B | Scale |
| A | Reserved |

The `SuperPrismDraw` material expression in the material loops through this texture to render multi-split patterns.

### Data Asset

- `USuperPrismPreset` inherits from `UDataAsset`
- `GetPositionTexture()` lazy-creates/caches the texture
- The texture updates automatically when the preset is modified

---

## 7. Using in Fixtures

1. Create and edit a prism preset
2. Reference the preset in the fixture Blueprint's **Prism** property
3. Select which layer to use via PrismLayerIndex (0/1/2)
4. The corresponding prism split effect will appear in the light

---

## 8. Usage Tips

- Each layer supports up to 48 split points (limited by the position lookup texture resolution)
- Changes to presets update in fixtures in real time; no restart required
- Thumbnails are automatically generated from the preset's point distribution for easy identification in the Content Browser
- It's recommended to store commonly used prism presets in the project's `Content/PrismPresets/` folder
