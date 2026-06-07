# Super Crowd — User Manual

## 1. Overview

**Super Crowd** is a procedural audience/crowd generation tool provided by SuperStage. It generates non-overlapping randomly distributed crowds within spline-defined enclosed areas using Poisson disk sampling algorithms, and supports terrain snapping.

### Core Features

- **Spline boundary** — Closed spline defines the crowd distribution area
- **Poisson disk sampling** — Bridson algorithm ensures uniform, non-overlapping character spacing
- **Multiple character templates** — Supports multiple character meshes + weight-based random assignment
- **Terrain snapping** — Ray detection automatically snaps to ground
- **Density/Count dual mode** — Controls crowd scale by density or target count

### Use Cases

- Performance venue audience area pre-visualization
- Event venue crowd density planning
- Standing audience simulation in front of stage
- Crowding any arbitrary area

---

## 2. Usage

1. Place a **Super Crowd** Actor in the scene
2. After selecting, edit spline control points to **enclose a closed area**
3. Adjust density/count parameters
4. Crowd automatically generated within the enclosed area

> **Important**: The spline must form a closed polygon (end-to-end connected); otherwise the area cannot be calculated.

---

## 3. Main Parameters

### 3.1 Distribution Parameters

| Parameter | Description |
|------|------|
| **FillMode** | Fill mode: Density / TargetCount |
| **Density** | Density (people/m²), used in Density mode |
| **TargetCount** | Target number of people, used in TargetCount mode |
| **MinSpacing** | Minimum spacing between characters (cm) |
| **RandomSeed** | Random seed (same seed produces same distribution) |

### 3.2 Character Templates

Define available character models via the `Characters` array:

| Field | Description |
|------|------|
| **Mesh** | Character Static Mesh (StaticMesh) |
| **Weight** | Weight (higher = more likely to be selected) |
| **MinScale / MaxScale** | Random scale range |
| **bRandomYaw** | Random yaw rotation |

### 3.3 Terrain Parameters

| Parameter | Description |
|------|------|
| **bSnapToGround** | Snap to ground |
| **TraceDistance** | Ray detection distance (cm) |
| **GroundOffset** | Ground offset (cm) |

---

## 4. Algorithm Notes

### Poisson Disk Sampling (Bridson)

- Generates uniformly distributed random points within the spline-defined area
- Ensures distance between any two points is not less than MinSpacing
- More natural than pure random distribution, no clustering

### Area Determination

- Ray casting method to determine if a point is inside the polygon
- Shoelace formula for area calculation

### Terrain Snapping

- Cast rays from above each sample point downward
- Local coordinates → world coordinates → ray detection → world coordinates → local coordinates

---

## 5. ISM Components

Uses ISM component pool (up to `MAX_CHARACTER_ISM_COUNT` components), each character mesh type occupies one ISM component.

---

## 6. Usage Tips

- Recommended density: dense standing 2~4 people/m², spacious 0.5~1 person/m²
- Character Weight controls proportions, e.g., 70% regular audience + 20% hands up + 10% taking photos
- Crowd automatically regenerates after modifying spline control points
- Large crowds (>1000) may affect editor frame rate
