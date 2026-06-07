# Super Truss Grid — User Manual

## 1. Overview

**Super Truss Grid** is a procedural horizontal double-layer grid truss generation tool provided by SuperStage. It simulates large-scale lighting rigging systems — upper and lower horizontal grids connected by vertical tie rods, used for large-area lighting suspension.

### Core Features

- **Double-layer grid structure** — Upper + lower horizontal grid, vertically connected
- **Main chords + secondary chords** — Perimeter frame (main chords) and internal grid (secondary chords)
- **Diagonal bracing system** — Bottom diagonals + top diagonals + vertical diagonals
- **Node plates** — Connection plates at grid intersections

### Use Cases

- Large performance venue ceiling lighting rigs
- Exhibition hall lighting suspension systems
- Large-area fixture installation platforms
- Sports arena/auditorium ceiling lighting grids

---

## 2. Main Parameters

### 2.1 Grid Dimensions

| Parameter | Description |
|------|------|
| **GridSizeX** | Number of spans in X direction |
| **GridSizeY** | Number of spans in Y direction |
| **GridSpacing** | Grid spacing (cm) |
| **GridDepth** | Distance between upper and lower layers (cm) |

### 2.2 Section Parameters

| Parameter | Description |
|------|------|
| **MainChordDiameter** | Main chord diameter (cm) |
| **SecondaryChordDiameter** | Secondary chord diameter (cm) |
| **DiagonalDiameter** | Diagonal brace diameter (cm) |

### 2.3 Component Visibility

| Parameter | Description |
|------|------|
| **bShowDiagonals** | Show diagonal braces |
| **bShowNodePlates** | Show node plates |

---

## 3. ISM Components

| Component | Elements |
|------|------|
| MainChordISM | Main chords (perimeter frame) |
| SecondaryChordISM | Secondary chords (internal grid + Z-direction connections) |
| DiagonalISM | Diagonal braces (bottom/top/vertical) |
| NodePlateISM | Node plates |

---

## 4. Usage Tips

- Grid spacing is typically set to 100~200cm, depending on fixture dimensions
- GridDepth affects structural rigidity — deeper is more stable
- Large venues can pair with Super Truss Tower as column supports
