# Super Scaffold — User Manual

## 1. Overview

**Super Scaffold** is a procedural scaffold generation tool provided by the SuperStage plugin. It can generate scaffold systems with rectangular grid structures in the Unreal Engine editor, including components such as vertical posts, horizontal ledgers, diagonal braces, base plates, top deck panels, and counterweights.

After dragging this Actor into the scene, adjust parameters in the **Details Panel** to preview scaffold structures of different sizes and configurations. The statistics panel estimates self-weight, load, and required counterweight from built-in parameters. Results are for pre-visualization reference only.

### Use Cases

- Performance stage lift/audience platform construction preview
- Temporary grandstand planning for outdoor events
- Lighting/sound platform construction visualization
- Scaffold weight and counterweight estimate reference

---

## 2. How to Add to Scene

1. Search for **"Super Scaffold"** in the UE editor's "Place Actor" panel
2. Drag it into the viewport scene
3. Select the Actor and view/modify parameters in the "Details Panel" on the right

---

## 3. Parameter Reference

### 3.1 Structure Parameters

This group of parameters defines the overall frame layout of the scaffold.

#### 3.1.1 Span Count X

- **Meaning**: The number of **spans** in the X-axis direction (usually the stage front direction)
- **Range**: 1 ~ 500 (slider 1 ~ 60)
- **Default**: 3

> **Note**: "Span count" refers to the number of intervals between adjacent uprights. If set to 3, there will be 4 uprights (span count + 1 = node count) in the X direction.

#### 3.1.2 Span Count Y

- **Meaning**: The number of **spans** in the Y-axis direction (usually the stage depth direction)
- **Range**: 1 ~ 500 (slider 1 ~ 60)
- **Default**: 2

#### 3.1.3 Layer Count

- **Meaning**: The number of **layers** (vertical intervals of horizontal ledgers)
- **Range**: 1 ~ 100 (slider 1 ~ 25)
- **Default**: 3

> **Note**: Each "layer" consists of a set of horizontal ledgers. If set to 3 layers, starting from the bottom base height, there will be 4 ledger levels going upward (layer count + 1 = ledger level count).

#### 3.1.4 Bay Size X

- **Meaning**: The **distance** between two adjacent uprights in the X direction
- **Unit**: centimeters (cm)
- **Range**: 30 ~ 3000 cm (slider 50 ~ 600)
- **Default**: 207 cm (built-in reference bay)

> **Reference**: Common scaffold products use bay sizes such as 73/109/140/157/207/257/307cm. These values are modeling references only.

#### 3.1.5 Bay Size Y

- **Meaning**: The **distance** between two adjacent uprights in the Y direction
- **Unit**: centimeters (cm)
- **Range**: 50 ~ 500 cm
- **Default**: 207 cm (built-in reference bay)

#### 3.1.6 Layer Height

- **Meaning**: The **vertical spacing** between each layer of ledgers
- **Unit**: centimeters (cm)
- **Range**: 30 ~ 2000 cm (slider 50 ~ 500)
- **Default**: 200 cm (2m)

> **Note**: Total scaffold height = Base Height + Layer Count × Layer Height. E.g., base height 30cm, 3 layers, each 200cm → total height = 30 + 600 = 630cm.

---

### 3.2 Tube Dimensions

#### 3.2.1 Main Tube Diameter

- **Meaning**: The **outer diameter** of the steel tubes used for uprights and ledgers
- **Unit**: centimeters (cm)
- **Range**: 0.5 ~ 100 cm (slider 1 ~ 25)
- **Default**: 4.83 cm (48.3mm built-in reference tube diameter)

> **Reference**: 48.3mm (Ø48.3) is a common scaffold tube diameter. The plugin only uses this value for modeling and weight estimates.

#### 3.2.2 Brace Tube Diameter

- **Meaning**: The **outer diameter** of diagonal brace tubes
- **Unit**: centimeters (cm)
- **Range**: 0.5 ~ 100 cm (slider 1 ~ 25)
- **Default**: 4.83 cm (48.3mm built-in reference value)

> **Note**: The default brace tube diameter is the same as the main tube and can be adjusted as needed.

#### 3.2.3 Base Plate Size

- **Meaning**: The **diameter** of the circular base plate at the bottom of each upright
- **Unit**: centimeters (cm)
- **Range**: 5 ~ 300 cm (slider 5 ~ 80)
- **Default**: 15 cm

#### 3.2.4 Base Height

- **Meaning**: The **vertical distance** from the ground to the first layer of ledgers. Simulates scaffold base height or adjustable leg height
- **Unit**: centimeters (cm)
- **Range**: 0 ~ 2000 cm (slider 0 ~ 300)
- **Default**: 30 cm

> **Note**: This value corresponds to the height of a scaffold adjustable base (Screw Jack). In actual construction, it is typically set to 20~40cm to accommodate uneven ground.

#### 3.2.5 Deck Thickness

- **Meaning**: The **thickness** of the top deck panels
- **Unit**: centimeters (cm)
- **Range**: 0.5 ~ 50 cm (slider 1 ~ 15)
- **Default**: 3.2 cm (built-in reference thickness)

---

### 3.3 Visibility Toggles

#### 3.3.1 Diagonal Mode

Controls the display of diagonal brace tubes on the scaffold.

| Option | Description |
|------|------|
| **None** | No diagonal brace tubes displayed |
| **Exterior Only** | Diagonal braces (X-shaped cross braces) displayed only on the **outer four faces** of the scaffold, no braces in internal bays |
| **All Faces** | Diagonal braces displayed on **all faces**, including every face of every internal bay |

> **Usage Tip**:
> - For visual display purposes, choose **Exterior Only** for an appearance close to common scaffold layouts
> - Choosing **All Faces** significantly increases the component count and shows more brace members
> - Choosing **None** gives the cleanest appearance

#### 3.3.2 Show Base Plates

- **Default**: Enabled ✅
- **Description**: Displays a circular base plate at the bottom of each upright. The base plate is a 1cm thick flat cylinder with diameter controlled by the "Base Plate Size" parameter

#### 3.3.3 Show Top Deck

- **Default**: Disabled ❌
- **Description**: Lays deck panels on top of the uppermost ledger layer. Each bay receives one independent panel, with panel dimensions automatically matching the bay size

#### 3.3.4 Show Counterweights

- **Default**: Enabled ✅
- **Description**: Stacks concrete counterweight blocks next to each base plate. The block count is estimated by the built-in formula and offset in the X direction
- **Counterweight Specifications**: Built-in concrete block size 40×20×12cm, 25kg each
- **Stacking Method**: Vertical stacking, each block 12cm high

> **Important**: The number of counterweight blocks is estimated by the built-in formula and cannot be manually set. If the calculated result is 0 blocks, no counterweight blocks will be displayed even if this toggle is enabled. This result is for previz reference only.

#### 3.3.5 Centered

- **Default**: Enabled ✅
- **Description**:
  - **Enabled**: Actor origin is at the geometric center of the scaffold's XY plane, Z direction still starts from ground
  - **Disabled**: Actor origin is at the front-left bottom corner (X=0, Y=0, Z=0), with the scaffold extending in the positive X and positive Y directions

---

### 3.4 Load Parameters

#### 3.4.1 Load Class

Select the built-in scaffold load class. The values reference **EN 12811-1** uniformly distributed load classes and are used for statistics estimates; they are not proof of real structural capacity.

| Option | Uniform Surface Load | Use Cases |
|------|-----------|----------|
| **Class 1** | 0.75 kN/m² (≈76 kg/m²) | Inspection only |
| **Class 2** | 1.50 kN/m² (≈153 kg/m²) | Light work platform |
| **Class 3** | 2.00 kN/m² (≈204 kg/m²) | Built-in default option |
| **Class 4** | 3.00 kN/m² (≈306 kg/m²) | Heavy storage platform |
| **Class 5** | 4.50 kN/m² (≈459 kg/m²) | Masonry work platform |
| **Class 6** | 6.00 kN/m² (≈612 kg/m²) | Heavy masonry platform |

> **Usage Tip**: Use **Class 3** or **Class 4** to compare stage platform or audience platform previews. Real builds must follow manufacturer data and structural review.

#### 3.4.2 Additional Load

- **Meaning**: **Additional load** applied to the structure beyond the self-weight (e.g., equipment, personnel, etc.)
- **Unit**: kilograms (kg)
- **Range**: 0 ~ 5,000,000 kg
- **Default**: 0 kg

> **Note**: This value participates in counterweight calculation. Counterweight requirement = (Safety Factor - 1) × (SelfWeight + Additional Load). Increasing additional load directly increases the required counterweight.

---

### 3.5 Materials

Assign separate materials for different scaffold components. Leave empty to use engine default materials.

| Parameter | Affected Range |
|------|----------|
| **Tube Material** | All steel tubes (uprights, ledgers, diagonal braces, base plate columns) |
| **Deck Material** | Top deck panels |
| **Base Plate Material** | Base plates |
| **Counterweight Material** | Counterweight blocks |

> **Tip**: Gray/silver metal, wood or anti-slip deck, and concrete-style materials are useful visual matches for these components.

---

### 3.6 Statistics — Read-Only

#### 3.6.1 Component Count Statistics

| Field | Description |
|------|------|
| **Verticals** | Number of uprights = (X Span Count + 1) × (Y Span Count + 1) |
| **HorizontalsPrimary** | Number of X-direction ledgers = X Span Count × (Y Span Count + 1) × (Layer Count + 1) |
| **HorizontalsSecondary** | Number of Y-direction ledgers = (X Span Count + 1) × Y Span Count × (Layer Count + 1) |
| **Diagonals** | Number of diagonal braces (depends on diagonal mode) |
| **BasePlates** | Number of base plates = Number of uprights |
| **DeckPanels** | Number of deck panels = X Span Count × Y Span Count |
| **CounterweightBlocks** | Total number of counterweight blocks |
| **TotalInstances** | Total number of all components |

#### 3.6.2 Weight and Load Statistics

| Field | Description |
|------|------|
| **SelfWeight (kg)** | Total scaffold self-weight, including all steel tubes, base plates and deck panels |
| **MaxLoadCapacity (kg)** | Estimated load reference calculated from load class and platform area |
| **RequiredCounterweight (kg)** | Total counterweight estimated by the built-in formula (using safety factor parameter 1.5) |
| **CounterweightPerBase (kg)** | Counterweight allocated per base = Total counterweight ÷ Number of uprights |
| **BlocksPerBase** | Number of counterweight blocks needed per base (rounded up, 25kg each) |

---

## 4. Weight Calculation Notes

### 4.1 Self-Weight Calculation

The system estimates scaffold self-weight from built-in parameters:

- **Steel Tube Material**: Estimated with steel density 7850 kg/m³ (i.e., 0.00785 g/mm³)
- **Wall Thickness**: 3.2mm (fixed built-in value, referencing common hollow-section specs)
- **Tube Cross-Sectional Area**: π × (OuterDiameter²/4 - InnerDiameter²/4), where InnerDiameter = OuterDiameter - 2×WallThickness
- **Per Tube Weight**: Length × Cross-Sectional Area × Steel Density
- **Base Plate**: 2.5 kg each (built-in reference value)
- **Deck Panel**: 15 kg/m² (built-in reference value)

### 4.2 Counterweight Calculation

Counterweight calculation uses the built-in safety factor 1.5 and this simplified formula:

```
Required Counterweight = (Safety Factor - 1) × (SelfWeight + Additional Load)
                       = (1.5 - 1) × (SelfWeight + Additional Load)
                       = 0.5 × (SelfWeight + Additional Load)
```

Counterweight is evenly distributed to each base, then rounded up at 25kg per block to determine the number of counterweight blocks.

---

## 5. Common Usage Examples

### Example 1: Standard Stage Platform (6m × 4m × 2m)

- X Span Count: 3
- Y Span Count: 2
- Layer Count: 1
- X/Y Bay Size: 200cm
- Layer Height: 200cm
- Load Class: Class 3

### Example 2: Outdoor Audience Grandstand (10m × 6m × 4m)

- X Span Count: 4
- Y Span Count: 3
- Layer Count: 2
- X Bay Size: 250cm
- Y Bay Size: 200cm
- Layer Height: 200cm
- Load Class: Class 4
- Show Counterweights: Enabled

### Example 3: Lighting/Sound Rigging Tower (2m × 2m × 8m)

- X Span Count: 1
- Y Span Count: 1
- Layer Count: 4
- X/Y Bay Size: 200cm
- Layer Height: 200cm
- Diagonal Mode: All Faces

---

## 6. Notes

1. **Parameters take effect immediately** — The scaffold model is rebuilt and previewed immediately after any parameter change
2. **Watch performance with large structures** — With very large span counts and layer counts (e.g., exceeding 20×20×10), the total component count can reach tens of thousands; monitor editor performance
3. **Counterweight blocks are estimated by the built-in formula** — The number cannot be manually specified and is not a construction or safety review result
4. **Steel tube wall thickness is fixed** — Wall thickness is uniformly calculated at 3.2mm and cannot be manually modified
5. **Diagonal braces are X-shaped cross** — Diagonal braces on each face are always arranged as two crossing tubes
6. **Centering mode affects coordinates** — Enabling/disabling centered mode changes the position of the scaffold relative to the Actor origin

> ⚠️ **Disclaimer**: All weight and load calculation data is for pre-visualization reference only and cannot replace calculation and review by a professional structural engineer.
