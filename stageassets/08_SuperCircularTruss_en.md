# Super Circular Truss — User Manual

## 1. Overview

**Super Circular Truss** is a procedural circular truss generation tool provided by SuperStage. It uses polar coordinate positioning and segment-based assembly to generate complete ring truss structures.

### Core Features

- **Outer/inner ring** — Supports single-ring or dual-ring structure
- **Radial braces** — Radial support between inner and outer rings
- **Connection flanges** — Connectors between segments
- **Section selection** — Box (square) / Triangle / Flat sections
- **Segment control** — Controls arc shape via arc angle and segment count
- **Multiple sizes** — S290 / S400 / S520

### Use Cases

- Circular lighting rigs (e.g., concert center lighting rig)
- Arc-shaped stage ceiling structures
- Semi-circle/quarter-circle arc truss structures

---

## 2. Differences from Super Truss Gantry

| Feature | Super Truss Gantry | Super Circular Truss |
|------|--------------------|-----------------------|
| Shape | Straight segment combinations (goal post/T-shape, etc.) | Arc/complete circle |
| Positioning | Cartesian coordinates | Polar coordinates (radius + angle) |
| Dual-layer | No | Supports outer + inner ring |
| Radial braces | No | Supported |

---

## 3. Main Parameters

### 3.1 Arc Parameters

| Parameter | Description |
|------|------|
| **Radius** | Outer ring radius (cm) |
| **ArcAngle** | Arc angle (360° = complete circle) |
| **SegmentCount** | Number of segments |
| **SectionType** | Section type: Box / Triangle / Flat |
| **TrussSize** | Size: S290 / S400 / S520 |

### 3.2 Dual-Ring Parameters

| Parameter | Description |
|------|------|
| **bInnerRing** | Enable inner ring |
| **InnerRadius** | Inner ring radius (cm) |
| **bRadialBraces** | Show radial braces |

### 3.3 Component Visibility

| Parameter | Description |
|------|------|
| **bShowFlanges** | Show connection flanges |
| **bShowDiagonals** | Show diagonal braces |

---

## 4. ISM Components

| Component | Elements |
|------|------|
| OuterChordISM | Outer ring chords |
| OuterDiagonalISM | Outer ring diagonals |
| InnerChordISM | Inner ring chords |
| InnerDiagonalISM | Inner ring diagonals |
| RadialBraceISM | Radial braces |
| FlangeISM | Connection flanges |

---

## 5. Usage Tips

- Set ArcAngle=360° for complete circle, 180° for semi-circle
- More segments = smoother but more components
- Inner radius must be smaller than outer radius
- Radial braces only available in dual-ring mode
