# Super Curved Scaffold — User Manual

## 1. Overview

**Super Curved Scaffold** is a **spline-driven curved scaffold** generation tool provided by the SuperStage plugin. It is the curved version of Super Scaffold — the scaffold is no longer arranged in straight lines, but bends and unfolds along an **editable spline curve**.

After dragging this Actor into the scene, you can drag spline control points in the viewport to adjust the curve shape, and adjust scaffold structural parameters in the Details panel. Most changes trigger an editor preview update.

### Differences from Straight Scaffold

| Feature | Super Scaffold (Straight) | Super Curved Scaffold |
|------|----------------------|----------------------------|
| Planar Shape | Rectangular grid | Curved along spline |
| Direction Definition | Two orthogonal X/Y directions | Along-spline direction + depth direction |
| Path Editing | Controlled by span count and bay size | Freely adjusted through spline control points |
| Bay Size | X/Y individually set fixed values | Auto-divided equally along spline, fixed value in depth direction |
| Panel Orientation | Always axis-aligned | Automatically follows curve tangent direction |

### Use Cases

- Curved stage/curved audience platform construction preview
- Circular/semi-circular lighting platform design
- Irregular booth/runway structure planning
- Curved grandstand weight and counterweight estimate reference

---

## 2. How to Add to Scene

1. Search for **"Super Curved Scaffold"** in the UE editor's "Place Actor" panel
2. Drag it into the viewport scene
3. Select the Actor and you will see a **green spline curve** running through the scaffold structure
4. Adjust parameters in the Details panel

### Editing the Spline Curve

1. After selecting the Actor, find the **SplinePath** component in the Details panel
2. In the viewport, spline control points are displayed as **white cubes**
3. **Drag control points** to change the curve shape
4. **Right-click** a control point to select "Add/Delete Spline Point"
5. Modify the **tangent handles** of control points to adjust curve curvature

> **Default Curve**: The Actor is created with a symmetrical curved spline (3 control points), with a span of approximately 6m and arc depth of approximately 1.5m.

---

## 3. Parameter Reference

### 3.1 Structure Parameters

#### 3.1.1 Span Count Along Spline

- **Meaning**: The number of **spans** (equal segments) along the spline curve direction
- **Range**: 1 ~ 1000 (slider 1 ~ 150)
- **Default**: 6

> **How It Works**: The system divides the total arc length of the spline curve into the specified number of segments. For example, if the total spline length is 600cm and span count is set to 6, each segment's arc length is approximately 100cm. In practice, the tubes are straight segments used to approximate the curve. More spans = smoother curve.

> **Usage Tip**: Curves with larger curvature usually need more spans to look smooth. Start with 100~150cm arc length per segment as a modeling reference.

#### 3.1.2 Span Count Depth

- **Meaning**: The number of **spans** in the direction perpendicular to the spline curve (i.e., the scaffold's "thickness" direction)
- **Range**: 1 ~ 500 (slider 1 ~ 60)
- **Default**: 2

> **Note**: The depth direction is always perpendicular to the tangent direction of the spline curve. If set to 1, the scaffold only has 2 rows of uprights (one front, one back). If set to 2, there are 3 rows of uprights.

#### 3.1.3 Layer Count

- **Meaning**: The number of **layers** in the vertical direction
- **Range**: 1 ~ 100 (slider 1 ~ 25)
- **Default**: 3

> **Note**: Identical to straight scaffold. Total scaffold height = Base Height + Layer Count × Layer Height.

#### 3.1.4 Bay Size Depth

- **Meaning**: The **distance** between two adjacent rows of uprights in the depth direction
- **Unit**: centimeters (cm)
- **Range**: 30 ~ 3000 cm (slider 50 ~ 600)
- **Default**: 207 cm (built-in reference bay)

#### 3.1.5 Layer Height

- **Meaning**: The **vertical spacing** between each layer of ledgers
- **Unit**: centimeters (cm)
- **Range**: 30 ~ 2000 cm (slider 50 ~ 500)
- **Default**: 200 cm (2m)

---

### 3.2 Tube Dimensions

#### 3.2.1 Main Tube Diameter

- **Meaning**: Outer diameter of steel tubes used for uprights and ledgers
- **Unit**: centimeters (cm)
- **Default**: 4.83 cm (48.3mm built-in reference tube diameter)

#### 3.2.2 Brace Tube Diameter

- **Meaning**: Outer diameter of diagonal brace tubes
- **Unit**: centimeters (cm)
- **Default**: 4.83 cm (48.3mm built-in reference value)

#### 3.2.3 Base Plate Size

- **Meaning**: Diameter of the circular base plate at the bottom of each upright
- **Unit**: centimeters (cm)
- **Default**: 15 cm

#### 3.2.4 Base Height

- **Meaning**: Vertical distance from the ground to the first layer of ledgers
- **Unit**: centimeters (cm)
- **Default**: 30 cm

#### 3.2.5 Deck Thickness

- **Meaning**: Thickness of the top deck panels
- **Unit**: centimeters (cm)
- **Default**: 3.2 cm (built-in reference thickness)

---

### 3.3 Visibility Toggles

#### 3.3.1 Diagonal Mode

| Option | Description |
|------|------|
| **None** | No diagonal braces displayed |
| **Exterior Only** | Diagonal braces displayed only on outer faces of the scaffold (front/back faces along the spline direction + both end faces) |
| **All Faces** | Diagonal braces displayed on all faces |

> **Note**: The "outer faces" of a curved scaffold are defined as: the inner face (depth index=0) and outer face (depth index=max) along the spline direction, plus the capping faces at both spline endpoints.

#### 3.3.2 Show Base Plates

- **Default**: Enabled ✅
- **Description**: Displays flat circular base plates at the bottom of each upright

#### 3.3.3 Show Top Deck

- **Default**: Disabled ❌
- **Description**: Lays deck panels on the top layer. Panel shapes **automatically adapt to the curve curvature** — each panel is trapezoidal/parallelogram, with long sides along the spline direction (chord length) and short sides along the depth direction

> **Curved Panel Characteristics**: Since panels are arranged along a curve, outer panels are slightly wider than inner panels. Panel orientation (rotation angle) automatically follows the spline tangent direction.

#### 3.3.4 Show Counterweights

- **Default**: Enabled ✅
- **Description**: Stacks concrete counterweight blocks next to each base, offset outward along the depth direction
- **Counterweight Specifications**: Built-in concrete block size 40×20×12cm, 25kg each

#### 3.3.5 Center Depth

- **Default**: Enabled ✅
- **Description**:
  - **Enabled**: The scaffold is symmetrically distributed **with the spline curve as the center** in the depth direction. For example, depth span count=2, bay size=200cm, extends 200cm to each side from the spline centerline
  - **Disabled**: The scaffold extends **in a single direction to the right** from the spline curve position

---

### 3.4 Load Parameters

#### 3.4.1 Load Class

Uses the same built-in load classes as straight scaffold. The values reference EN 12811-1 uniformly distributed load classes for statistics estimates; they are not proof of real structural capacity.

| Option | Uniform Surface Load | Use Cases |
|------|-----------|----------|
| **Class 1** | 0.75 kN/m² | Inspection only |
| **Class 2** | 1.50 kN/m² | Light work platform |
| **Class 3** | 2.00 kN/m² | Built-in default option |
| **Class 4** | 3.00 kN/m² | Heavy storage platform |
| **Class 5** | 4.50 kN/m² | Masonry work platform |
| **Class 6** | 6.00 kN/m² | Heavy masonry platform |

#### 3.4.2 Additional Load

- **Meaning**: Additional load applied
- **Unit**: kilograms (kg)
- **Default**: 0 kg

---

### 3.5 Materials

| Parameter | Affected Range |
|------|----------|
| **Tube Material** | All steel tubes (uprights, ledgers, diagonal braces) |
| **Deck Material** | Top deck panels |
| **Base Plate Material** | Base plates |
| **Counterweight Material** | Counterweight blocks |

---

### 3.6 Statistics — Read-Only

#### 3.6.1 Component Count Statistics

Same format as straight scaffold: Verticals, HorizontalsPrimary (along spline direction), HorizontalsSecondary (depth direction), Diagonals, BasePlates, DeckPanels, CounterweightBlocks, TotalInstances.

#### 3.6.2 Weight and Load Statistics

Same format as straight scaffold: SelfWeight, MaxLoadCapacity, RequiredCounterweight, CounterweightPerBase, BlocksPerBase. These statistics are pre-visualization references from built-in parameters and simplified formulas.

> **Estimate Note**: Weight estimates for curved scaffold account for tube length differences caused by curvature. Ledger lengths along the spline direction are calculated segment by segment from chord length, and panel area is estimated from trapezoid area.

#### 3.6.3 Spline Statistics

| Field | Description |
|------|------|
| **SplineTotalLength (cm)** | Total arc length of the spline curve |
| **BaySizeAlongSpline (cm)** | Average arc length per segment along the spline direction = Total Arc Length ÷ Span Count Along Spline |

---

## 4. Spline Editing Tips

### 4.1 Creating Common Curves

- **Semicircle**: Pull the middle control point to one side to form a semicircle
- **S-Curve**: At least 4 control points, alternating offsets to both sides
- **U-Curve**: 3 control points, large offset at the middle point
- **L-Shape Corner**: Add control points closely arranged at the corner

### 4.2 Controlling Curve Smoothness

- **Increase span count**: More spans along the spline mean shorter tube segments and a smoother curve
- **Adjust tangents**: Dragging tangent handles of control points changes local curvature
- **Equal spacing**: Evenly spaced control points produce more uniform curvature

### 4.3 Notes

- The spline curve only affects the planar shape of the scaffold on the XY plane; the Z direction is always vertical
- Extremely small curvature radii (tight bends) may cause tube overlap; maintain reasonable bend angles
- After modifying spline control point positions or tangents, the scaffold automatically rebuilds

---

## 5. Common Usage Examples

### Example 1: Curved Audience Grandstand

- Spline shape: Semicircular, span approx. 15m
- Span Count Along Spline: 12
- Span Count Depth: 3
- Layer Count: 2
- Bay Size Depth: 200cm
- Layer Height: 200cm
- Load Class: Class 4

### Example 2: Curved Lighting Platform

- Spline shape: Shallow curve, span approx. 10m
- Span Count Along Spline: 8
- Span Count Depth: 1
- Layer Count: 3
- Bay Size Depth: 150cm
- Layer Height: 200cm
- Diagonal Mode: Exterior Only

### Example 3: Circular Runway

- Spline shape: Large curvature curve or approximate arc
- Span Count Along Spline: 20
- Span Count Depth: 1
- Layer Count: 1
- Bay Size Depth: 200cm
- Layer Height: 100cm

---

## 6. Notes

1. **Spline modifications trigger rebuild** — After dragging spline control points or adding/deleting control points, the scaffold updates in the editor
2. **Tubes are linear approximations** — Tubes along the spline direction are straight segments, not truly curved tubes. More spans = better approximation
3. **Depth direction is always perpendicular** — The depth direction is always perpendicular to the spline tangent direction at the current position (projected onto the XY plane); uprights are always perpendicular to the ground
4. **Panels auto-adapt** — Top panels automatically calculate trapezoidal shapes and rotate to align with the curve direction
5. **Counterweight direction** — Counterweights are offset along the spline's right vector direction, following the curve orientation
6. **Performance note** — With large span counts along the spline (>30) and large depth span counts and layer counts, the total component count can become very large

> ⚠️ **Disclaimer**: All weight and load calculation data is for pre-visualization reference only and cannot replace calculation and review by a professional structural engineer.
