# Super Truss Gantry — User Manual

## 1. Overview

**Super Truss Gantry** is a procedural truss generation tool provided by the SuperStage plugin. It generates truss gantry structures in the Unreal Engine editor for stage lighting and audio equipment rigging previews.

After dragging this Actor into the scene, you can adjust parameters in the Details panel. Most structural parameter changes trigger an editor rebuild or preview update.

### Use Cases

- Performance stage lighting truss rigging preview
- Exhibition/conference gantry layout planning
- Pre-visualization of truss structures for large outdoor events
- Self-weight, load, and deflection estimates for pre-visualization reference

---

## 2. How to Add to Scene

1. Search for **"Super Truss Gantry"** in the UE editor's "Place Actor" panel
2. Drag it into the viewport scene
3. Select the Actor and view/modify parameters in the "Details Panel" on the right

---

## 3. Parameter Reference

### 3.1 Truss Profile Parameters

This group of parameters determines the cross-section shape, size specifications and brace arrangement of the truss itself.

#### 3.1.1 Section Type

Controls the cross-sectional shape of the truss, affecting appearance and the built-in load estimates.

| Option | Description | Use Cases |
|------|------|----------|
| **Box (4-Chord)** | Square section, 4 main chords + 4 faces of bracing. Often used to represent larger spans or heavier fixture layouts in previz | Large performance or heavy lighting rig previz |
| **Triangle (3-Chord)** | Triangular section, 3 main chords + 3 faces of bracing. Visually lighter than Box | Small to medium performance or exhibition previz |
| **Flat / Ladder (2-Chord)** | Flat section, only 2 chords + single-face bracing. Lightest, ladder-like appearance | Light installations, small exhibitions, backdrop decoration |

> **Usage Note**: Section type affects appearance and the built-in estimate values. Real rigging decisions must use actual product data and structural review.

#### 3.1.2 Truss Size

Select the size series for the truss. The built-in values reference common truss product specifications for previz modeling and estimates.

| Option | Section Dimensions | Self-Weight | Use Cases |
|------|----------|------|----------|
| **290mm (Light)** | 29cm | Approx. 6 kg/m | Previz reference for exhibitions, small performances, and indoor events |
| **400mm (Medium)** | 40cm | Approx. 9 kg/m | Previz reference for regular performances and medium events. **Default option** |
| **520mm (Heavy)** | 52cm | Approx. 16 kg/m | Previz reference for large outdoor performances and touring events |

Each specification includes the following physical parameters (read-only, auto-populated):

- **Main Chord Outer Diameter** — 290/400 is 50mm, 520 is 50mm
- **Main Chord Wall Thickness** — 290/400 is 2mm, 520 is 3mm
- **Brace Tube Outer Diameter** — 290 is 20mm, 400/520 is 25mm
- **Brace Tube Wall Thickness** — All are 2mm
- **Standard Bay Spacing** — 290/400 is 50cm, 520 is 60cm
- **Section Moment of Inertia** — Used for load calculation, 290=2500cm⁴, 400=5200cm⁴, 520=12800cm⁴

> **Usage Note**: Size affects visual dimensions and the estimate values in the statistics panel. Real project selection must follow actual product documentation and structural review.

#### 3.1.3 Brace Pattern

Controls the arrangement of diagonal brace tubes on each face of the truss.

| Option | Description | Visual Effect |
|------|------|----------|
| **Warren (V-Pattern)** | Alternating V-shaped arrangement, with brace directions alternating between adjacent bays. The most common performance truss brace pattern | Clean, symmetrical |
| **Cross (X-Pattern)** | Two braces crossing in an X shape within each bay | Dense, robust feel |

---

### 3.2 Gantry Shape Parameters

This group of parameters controls the overall structural form of the gantry — the number of uprights, beam arrangement, etc.

#### 3.2.1 Shape

| Option | Structural Description | Diagram |
|------|----------|------|
| **Goal Post** | Gate-shaped: 2 uprights + 1 beam. The most basic gantry shape | `┌──┐` |
| **T-Shape** | T-shaped: Beam extends beyond the uprights on both ends, forming overhangs. Suitable for scenarios requiring wider beam coverage | `├──┤` with overhangs |
| **Portal (with Outriggers)** | Reinforced portal: Adds bottom outrigger tubes and counterweight bases to the Goal Post | `┌──┐` + bottom supports |
| **Double Span** | Double span: 3 uprights + 2 beams, covering a wider span | `┌──┬──┐` |

#### 3.2.2 Span Length

- **Meaning**: The inner distance between two uprights (effective length of the horizontal beam)
- **Unit**: centimeters (cm)
- **Range**: 100 ~ 20000 cm (slider 100 ~ 6000; beyond that, type the value)
- **Default**: 800 cm (8m)

> **Usage Advice**: For standard stages, common spans are between 600~1200cm. For larger spans, check the deflection and maximum load estimates in the statistics panel and have the result reviewed by a qualified structural professional.

#### 3.2.3 Upright Height

- **Meaning**: The vertical height from the base plate to the top of the upright
- **Unit**: centimeters (cm)
- **Range**: 100 ~ 10000 cm (slider 100 ~ 4000; beyond that, type the value)
- **Default**: 600 cm (6m)

#### 3.2.4 Overhang — T-Shape Only

- **Meaning**: The distance the beam extends beyond the uprights on both ends. Only editable when the shape is **T-Shape**
- **Unit**: centimeters (cm)
- **Range**: 0 ~ 5000 cm (slider 0 ~ 1000; beyond that, type the value)
- **Default**: 100 cm (1m)

> **Note**: Total beam length = Span + Overhang on both sides × 2. E.g., span 800cm + overhang 100cm = total beam length 1000cm.

#### 3.2.5 Second Span — Double Span Only

- **Meaning**: The span length of the second span in a double span configuration. Only editable when the shape is **Double Span**
- **Unit**: centimeters (cm)
- **Range**: 100 ~ 20000 cm (slider 100 ~ 6000; beyond that, type the value)
- **Default**: 800 cm (8m)

> **Note**: Total double span width = First span + Second span. The middle upright serves as a shared support point.

---

### 3.3 Lift Parameters

#### 3.3.1 Lift Height

- **Meaning**: The distance the beam is lowered **downward** from the top of the uprights. Simulates the effect of a chain hoist lowering the beam
- **Unit**: centimeters (cm)
- **Range**: 0 ~ (Upright Height - Section Size)
- **Default**: 0 cm (beam at upright top position)

> **How It Works**: Actual beam height = Upright Height - Lift Height. E.g., upright 600cm, lift 100cm -> beam at 500cm height. The system clamps the lift height to the range allowed by the current parameters.

#### 3.3.2 Suspended Load

- **Meaning**: The total weight of equipment the user expects to hang on the beam
- **Unit**: kilograms (kg)
- **Range**: 0 or higher
- **Default**: 0 kg

> **Important**: This parameter does not affect the appearance of the truss, but affects the load and deflection estimates in the statistics panel. The system calculates beam deflection, maximum load and other reference values from it.

---

### 3.4 Visibility Toggles

These toggles control the display/hiding of gantry accessory components.

#### 3.4.1 Show Base Plates

- **Default**: Enabled ✅
- **Description**: Displays a 60×60cm aluminum alloy base plate at the bottom of each upright. Base plates are hidden when toggled off.

#### 3.4.2 Show Outriggers — Portal Shape Only

- **Default**: Enabled ✅
- **Description**: Extends support tubes (Ø48.3mm, length 120cm) in four directions (front/back/left/right) from the bottom of each upright. Only visible and editable when shape is **Portal**
- **Purpose**: Outriggers show bottom support structures in previz; they are not a real anti-toppling verification result.

#### 3.4.3 Show Chain Hoists

- **Default**: Enabled ✅
- **Description**: Displays chain hoist (electric hoist) models on the inner side of the uprights below the beam. Hoist dimensions are 50×20×30cm (standard 1-ton electric hoist)
- **Quantity Rules**:
  - Goal Post / Portal / T-Shape: 2 units (one on each side)
  - Double Span: 4 units (one on each side of each span)

#### 3.4.4 Centered

- **Default**: Enabled ✅
- **Description**:
  - **Enabled**: The Actor's origin is at the center of the beam, convenient for alignment and placement
  - **Disabled**: The Actor's origin is at the bottom of the left upright, and the gantry extends to the right

---

### 3.5 Materials

Assign separate materials for different gantry components. If not specified (left empty), engine default materials will be used.

| Parameter | Affected Range |
|------|----------|
| **Chord Material** | Main chords (thick tubes) |
| **Brace Material** | Diagonal braces, horizontal braces, outrigger tubes |
| **Base Plate Material** | Base plates |
| **Chain Hoist Material** | Chain hoists |

> **Tip**: Metallic materials such as aluminum silver or black powder coating usually make the truss appearance easier to read in previz.

---

### 3.6 Statistics — Read-Only

The statistics panel displays component, weight, and load estimates for the gantry under the current configuration. All data is automatically calculated and cannot be manually edited.

#### 3.6.1 Component Count Statistics

| Field | Description |
|------|------|
| **Chords** | Number of main chords |
| **HorizontalBraces** | Number of horizontal braces (short tubes connecting adjacent chords at each node) |
| **DiagonalBraces** | Number of diagonal braces |
| **BasePlates** | Number of base plates |
| **Outriggers** | Number of outriggers (Portal shape only) |
| **ChainHoists** | Number of chain hoists |
| **TotalInstances** | Total number of all components |

#### 3.6.2 Weight and Load Statistics

| Field | Description |
|------|------|
| **SelfWeight (kg)** | Truss self-weight, including the total weight of all components such as tubes, base plates, chain hoists |
| **MaxPointLoad (kg)** | Concentrated mid-span load reference from the simplified model, based on the current span, material parameters, and deflection limit |
| **MaxDistributedLoad (kg/m)** | Uniformly distributed load reference from the simplified model, calculated across the full beam length |
| **MaxDeflection (cm)** | Estimated mid-span deflection under the current suspended load |
| **SuspendedLoad (kg)** | The suspended load value you set (corresponding to "Suspended Load" in the lift parameters) |
| **ReactionPerUpright (kg)** | Reaction force at the bottom of each upright (weight that the ground must support), = (SelfWeight + SuspendedLoad) ÷ Number of Uprights |

#### 3.6.3 Current Specification Parameters

Displays detailed physical parameters of the currently selected truss size (e.g., S400), including section dimensions, tube diameters, wall thicknesses, bay spacing, self-weight, and moment of inertia. The built-in data references common product manuals and is only for previz and estimates.

---

## 4. Load Estimate Notes

Super Truss includes a simplified load estimate model for understanding the approximate effect of the current dimensions and loads during pre-visualization:

### Calculation Principles

- **Structural Model**: Simply supported beam (beam supported at both ends on uprights)
- **Material**: Aluminum alloy 6082-T6 (elastic modulus 70 GPa, yield strength 260 MPa)
- **Safety factor parameters**: Variable load 1.5, permanent load 1.35
- **Allowable Deflection**: Span/300 (performance industry convention)

### How to Use These Values

1. Check **MaxPointLoad** to estimate the mid-span concentrated load reference value for the current span
2. Check **MaxDeflection** to understand the deflection trend under the current suspended load
3. Check **ReactionPerUpright** to estimate the weight transferred to each upright

> ⚠️ **Disclaimer**: This calculation is for pre-visualization reference only and cannot replace formal structural engineering calculations. Actual construction must be professionally calculated and reviewed by a qualified structural engineer.

---

## 5. Common Usage Examples

### Example 1: Standard Performance Gantry

- Shape: Goal Post
- Size: S400 (400mm)
- Section: Box
- Span: 1000cm (10m)
- Upright Height: 700cm (7m)
- Suspended Load: 500kg

### Example 2: Exhibition Lightweight Gantry

- Shape: Goal Post
- Size: S290 (290mm)
- Section: Triangle
- Span: 600cm (6m)
- Upright Height: 400cm (4m)

### Example 3: Large Outdoor Supported Gantry

- Shape: Portal (with Outriggers)
- Size: S520 (520mm)
- Section: Box
- Span: 1500cm (15m)
- Upright Height: 1000cm (10m)
- Show Outriggers: Enabled
- Suspended Load: 1000kg

### Example 4: Double Span Stage

- Shape: Double Span
- Size: S400 (400mm)
- Span 1: 800cm
- Span 2: 800cm
- Upright Height: 600cm

---

## 6. Notes

1. **Parameters take effect immediately** — The truss model is rebuilt immediately after any parameter change, no additional action required
2. **Rebuild only when parameters actually change** — Simply dragging or rotating the Actor does not trigger a structural rebuild
3. **Lift height auto-limited** — The system ensures the beam does not go below the section size height to prevent clipping
4. **Conditional editing** — Some parameters are only editable under specific shapes (e.g., "Overhang" only for T-Shape, "Outriggers" only for Portal)
