# Super Curved Truss — User Manual

## 1. Overview

**Super Curved Truss** is a spline-driven curved truss generation tool provided by SuperStage. It uses the UE Spline Component to control the truss path, enabling truss structures of any free-form curve shape.

### Core Features

- **Spline-driven** — Define truss path by editing spline control points
- **Section perpendicular to tangent** — Truss sections are always automatically perpendicular to the spline tangent direction
- **Section rotation** — SectionRotation parameter controls section rotation around the tangent axis
- **End plates** — End caps/flanges at both ends
- **Multiple sections** — Box / Triangle / Flat
- **Multiple sizes** — S290 / S400 / S520

### Use Cases

- Free-form curved stage trusses
- S-shaped or wave-shaped lighting rigs
- Irregular ceiling structures
- Truss systems that need to follow arbitrary paths

### Differences from Circular Truss

| Feature | Circular Truss | Curved Truss |
|------|---------------|--------------|
| Path control | Fixed arc (radius + angle) | Free spline curve |
| Shape flexibility | Arc only | Any curve |
| Dual-layer | Supports inner + outer ring | No |
| Editing method | Parameter adjustment | Spline control point dragging |

---

## 2. Main Parameters

### 2.1 Truss Parameters

| Parameter | Description |
|------|------|
| **SectionType** | Section type: Box / Triangle / Flat |
| **TrussSize** | Size: S290 / S400 / S520 |
| **SectionRotation** | Section rotation angle around the tangent axis |

### 2.2 Spline Editing

After selecting the Actor, spline control points are visible in the viewport:

1. **Drag control points** — Modify the truss path
2. **Alt + click on spline** — Add a new control point
3. **Select control point + Delete** — Delete a control point
4. **Adjust tangent handles** — Control curve curvature

### 2.3 Component Visibility

| Parameter | Description |
|------|------|
| **bShowDiagonals** | Show diagonal braces |
| **bShowHorizontalBraces** | Show horizontal braces |
| **bShowEndPlates** | Show end plates |

---

## 3. ISM Components

| Component | Elements |
|------|------|
| ChordISM | Chords |
| DiagonalISM | Diagonal braces |
| HorizontalBraceISM | Horizontal braces |
| EndPlateISM | End plates |

---

## 4. Usage Tips

- A spline requires at least 2 control points
- More control points = finer truss, but more components
- SectionRotation adjusts section orientation, e.g., triangle section pointed up or down
- Curved truss does not support load calculation (due to irregular path)
