# Super Drape — User Manual

## 1. Overview

**Super Drape** is a procedural drape/curtain generation tool provided by the SuperStage plugin. It generates editable stage drape meshes in the Unreal Engine editor, including main curtains, legs, borders, backdrops, scrims and cycloramas, with selectable pleat patterns and opening modes.

The drape surface is generated using Procedural Mesh to represent different pleat forms, with optional accessory components such as pipes and tie lines.

### Use Cases

- Stage drape system design and preview
- Theater/performance venue drape layout planning
- Drape fabric selection and visual effect preview
- Opening/closing animation effect pre-visualization
- Drape weight estimation

---

## 2. How to Add to Scene

1. Search for **"Super Drape"** in the UE editor's "Place Actor" panel
2. Drag it into the viewport scene
3. Select the Actor and view/modify parameters in the "Details Panel" on the right

---

## 3. Parameter Reference

### 3.1 Type & Fabric

#### 3.1.1 Drape Type

Select the functional role of the drape on stage. This parameter primarily serves as a classification label for organizational convenience and does not directly affect the drape's visual appearance.

| Option | Description |
|------|------|
| **Main Curtain** | The main drape at the front of the stage |
| **Legs** | Vertical drapes on both sides of the stage, used to mask the side stage area |
| **Border** | Horizontal short drapes above the stage, used to mask lighting truss |
| **Backdrop** | Large-area background drape at the very back of the stage |
| **Scrim** | Semi-transparent gauze drape, suitable for lighting effects and projection |
| **Cyclorama** | Curved drape surrounding the back of the stage, typically used for sky backgrounds |

#### 3.1.2 Fabric Type

Select the fabric material used for the drape. Different fabrics have different physical properties (weight, light transmittance, drape coefficient), which **directly affect the depth and form of pleats**.

| Option | GSM | Thickness | Light Transmittance | Drape Coefficient | Use Cases |
|------|------|------|------|------|----------|
| **Velour** | 600 g/m² | 5.0mm | 0% | 0.80 | Main curtain, legs. Heavy fabric preset with stronger pleat depth |
| **Commando** | 350 g/m² | 3.0mm | 0% | 0.50 | Backdrop. Medium-weight fabric preset with 0% transmittance in the preset |
| **Muslin** | 130 g/m² | 1.0mm | 60% | 0.30 | Scrim. High light transmittance, suitable for projection and scrim effects |
| **Silk** | 80 g/m² | 0.5mm | 80% | 0.90 | Decorative drape. Lightweight fabric preset with high drape coefficient and high transmittance |
| **IFR Polyester** | 300 g/m² | 2.5mm | 0% | 0.40 | General-purpose performance fabric preset with 0% transmittance in the preset |

> **Fabric affects pleat depth**: The higher the fabric's "drape coefficient", the larger the generated pleat displacement. Silk (0.90) and Velour (0.80) produce larger displacement values than Muslin (0.30).

> **Fabric affects weight estimates**: Fabric GSM affects the total weight estimate in the statistics panel. This data is for previz reference only and does not replace rigging, motor, or structural safety review.

---

### 3.2 Pleating Parameters

#### 3.2.1 Pleat Pattern

Controls the pleat form on the drape surface.

| Option | Visual Effect | Description |
|------|------|------|
| **Flat** | Flat surface, no generated pleat displacement | Fabric lies flat, suitable for projection scrims or backdrops |
| **Box Pleat** | Regular square pleats, like a folded accordion | Each pleat consists of three segments "rise→hold→fall", forming neat square wave shapes |
| **Gathered** | Soft wave-like pleats | Smooth ripples produced by evenly gathering the fabric, used to represent common performance drape appearance |
| **Austrian** | Three-dimensional pleats with horizontal ripples + vertical swags | Adds vertical scalloped draping on top of gathered pleats |

> **Design Advice**:
> - **Main curtains** typically use **Gathered** or **Box Pleat**
> - **Legs** typically use **Gathered** or **Flat**
> - **Austrian** is for decorative drape looks with vertical swags
> - **Flat** is for occasions requiring a flat surface (e.g., projection scrims, cycloramas)

#### 3.2.2 Fullness

- **Meaning**: The ratio of the fabric's **actual width** to the **display width**. Higher fullness means more fabric and richer pleats
- **Range**: 1.0 ~ 10.0 (slider 1.0 ~ 4.0)
- **Default**: 2.0 (i.e., fabric width is 2× the display width)

| Fullness Value | Effect | Use Cases |
|----------|------|----------|
| 1.0 | Flat, no generated pleat displacement | Projection scrim, backdrop |
| 1.5 | Light pleating | Legs, borders |
| **2.0** | Common pleat depth | **Reference value for regular main curtains and backdrops** |
| 2.5 ~ 3.0 | Larger pleat displacement | Main curtain, decorative drapes |
| 3.0+ | High pleat displacement | Special visual effects |

> **How It Works**: A fullness of 2.0 means 2× the display width of fabric was used in construction, with the excess fabric consumed by pleats. When fullness = 1.0 and pleat pattern is not Flat, pleat depth will be 0 since there is no excess fabric.

#### 3.2.3 Pleat Count

- **Meaning**: The number of **pleats** on the drape (one complete wave cycle counts as one pleat)
- **Range**: 1 ~ 1000 (slider 1 ~ 150)
- **Default**: 20

> **Design Tip**:
> - Too few pleats (<5) makes each pleat very wide
> - Too many pleats (>100) makes pleats very fine and dense
> - Regular main curtains typically use 15~30 pleats
> - The ratio of pleat count to fabric width determines each pleat's "pleat width"

#### 3.2.4 Swag Count — Austrian Only

- **Meaning**: The number of **scalloped swags** in the vertical direction for Austrian drapes
- **Range**: 1 ~ 200 (panel slider 1 ~ 30)
- **Default**: 4

> **This field is only editable when the pleat pattern is Austrian.**
>
> The scalloped bottom edge of the Tab opening mode is also driven by Swag Count, but under a non-Austrian pleat pattern the field cannot be changed and Tab uses the stored value (default 4). To adjust the Tab scallop count, set the pleat pattern to Austrian.

---

### 3.3 Dimensions

#### 3.3.1 Width

- **Meaning**: The **display width** of the drape (i.e., the horizontal visible width when hung)
- **Unit**: centimeters (cm)
- **Range**: 20 ~ 50000 cm (slider 50 ~ 10000)
- **Default**: 1200 cm (12m)

> **Note**: Actual fabric width = Display Width × Fullness. E.g., display width 1200cm, fullness 2.0 → actual fabric width 2400cm.

#### 3.3.2 Height

- **Meaning**: The **vertical height** of the drape (length from top to bottom)
- **Unit**: centimeters (cm)
- **Range**: 20 ~ 20000 cm (slider 50 ~ 4000)
- **Default**: 700 cm (7m)

#### 3.3.3 Hanging Height

- **Meaning**: The **height** of the drape's top edge (pipe position) above the ground
- **Unit**: centimeters (cm)
- **Range**: 0 ~ 20000 cm (slider 0 ~ 4000)
- **Default**: 800 cm (8m)

> **Note**: Drape top edge is at Z = Hanging Height, bottom edge is at Z = Hanging Height - Height. If hanging height(800) - height(700) = 100cm, the drape's bottom edge is 1m above the ground.

---

### 3.4 Opening Parameters

#### 3.4.1 Open Type

Controls the drape's opening/closing action mode.

| Option | Action Method | Description |
|------|------|------|
| **Fixed** | Drape stays fixed, always fully extended | Suitable for legs, backdrops and other drapes that don't need to open/close |
| **Fly** | Drape rises/falls from above | The entire drape lifts vertically, achieving open/close by changing visible height |
| **Traveler** | Drape parts from center to sides | Drape splits into left and right halves, gradually pulling apart from the center |
| **Tab** | Bottom edge is lifted in a scalloped shape | The bottom forms a curved scallop shape |

#### 3.4.2 Open Percent

- **Meaning**: The **degree of opening** of the drape
- **Range**: 0.0 ~ 1.0 (i.e., 0% ~ 100%)
- **Default**: 0.0 (fully closed)

| Value | Effect |
|----|------|
| **0.0** | Fully closed (drape fully covers) |
| **0.5** | Half open, half closed |
| **1.0** | Fully open |

**Effects under different opening types**:

- **Fly**: Open = 0.5 → Visible drape height becomes 50% of original (top half is "flown" away)
- **Traveler**: Open = 0.5 → Center gap is 50% of the width (25% drape remaining on each side)
- **Tab**: Open = 0.5 → Bottom edge raised to 50% of drape height in scallop shape
- **Fixed**: This parameter has no effect

---

### 3.5 Visibility Toggles

#### 3.5.1 Show Pipe

- **Default**: Enabled ✅
- **Description**: Displays a horizontal pipe at the top of the drape. Pipe length = Drape Width + 15cm extension on each end
- **Pipe Specifications**: Built-in pipe reference value, outer diameter 48.3mm (Ø48), estimated at 3.61 kg/m

#### 3.5.2 Show Tie Lines

- **Default**: Disabled ❌
- **Description**: Displays evenly spaced vertical tie lines below the pipe. Tie lines are used for tying the drape to the pipe
- **Tie Line Specifications**: 5mm diameter, 30cm length, approximately 30cm spacing
- **Quantity**: Automatically calculated from drape width (width ÷ 30cm spacing + 1, minimum 2)

#### 3.5.3 Double Sided

- **Default**: Enabled ✅
- **Description**:
  - **Enabled**: Both front and back faces of the drape are visible (drape visible from both front and back of the stage)
  - **Disabled**: Only renders the drape front face (the side facing the audience); the drape is transparent from the back

> **Performance Tip**: Enabling double-sided rendering doubles the triangle count. If the drape is only visible from the front, disable this option to save rendering performance.

#### 3.5.4 Centered

- **Default**: Enabled ✅
- **Description**:
  - **Enabled**: Actor origin is at the center of the drape width
  - **Disabled**: Actor origin is at the left edge of the drape

---

### 3.6 Materials

| Parameter | Affected Range |
|------|----------|
| **Fabric Material** | Drape fabric surface material |
| **Pipe Material** | Material for the pipe and tie lines |

> **Tip**:
> - Dark velvet-textured materials work well visually for velvet drapes
> - Semi-transparent materials work well visually for scrims
> - Black matte metal materials work well visually for pipes

---

### 3.7 Statistics — Read-Only

#### 3.7.1 Mesh Statistics

| Field | Description |
|------|------|
| **Vertices** | Total vertex count of the drape mesh. Depends on pleat pattern and pleat count |
| **Triangles** | Total triangle count (doubled in double-sided mode) |
| **PipeSegments** | Number of pipe segments (usually 1) |
| **TieLines** | Number of tie lines |
| **TotalInstances** | Total instance count of accessory components (pipe + tie lines) |

#### 3.7.2 Weight Statistics

| Field | Description |
|------|------|
| **FabricArea (m²)** | Total fabric area = Display Width × Fullness × Height ÷ 10000 |
| **FabricWeight (kg)** | Total fabric weight = Area × Fabric GSM ÷ 1000 |
| **PipeLength (m)** | Pipe length (including extensions on both ends) |
| **PipeWeight (kg)** | Pipe weight = Length × Tube linear density (3.61 kg/m, Sch40 Ø48.3mm steel pipe) |
| **TotalWeight (kg)** | Total weight = Fabric Weight + Pipe Weight |

#### 3.7.3 Current Fabric Parameters

Displays detailed physical parameters of the currently selected fabric: GSM, thickness, light transmittance, drape coefficient.

---

## 4. Pleat Effect Details

### 4.1 Flat

Fabric is completely flat, Y-direction displacement is 0. Suitable for:
- Projection scrims
- Cycloramas
- Drapes requiring flat display

### 4.2 Box Pleat

Fabric forms regular trapezoidal wave pleats:
- Each pleat cycle is divided into 4 segments: rise (25%), hold high (50%), fall (25%)
- Forms a clear "protrude→platform→descend" shape
- Pleat depth = (Fullness - 1) × Pleat Width × Drape Coefficient × 0.3

### 4.3 Gathered

Fabric forms smooth cosine wave pleats:
- Uses a cosine function to generate wave shapes
- Each pleat smoothly transitions between 0 (flat position) and maximum depth
- Used to represent the common form of gathered drapes

### 4.4 Austrian

Superimposes vertical swag effects on top of gathered pleats:
- **Horizontal**: Same gathered pleat effect as Gathered
- **Vertical**: Bottom produces scalloped draping, swag count controlled by Swag Count
- **Swag depth**: Deepens with distance from the top (V² relationship), more pronounced further down
- Visual effect commonly associated with opera-house style drapes

---

## 5. Opening Effect Details

### 5.1 Fixed

Drape always remains fully extended; Open Percent parameter has no effect.

### 5.2 Fly

- Drape "disappears" from above, simulating the pipe lifting the drape into the stage fly tower
- Visible height = Total Height × (1 - Open Percent)
- Open = 0.0 → fully visible; Open = 1.0 → fully raised (invisible)

### 5.3 Traveler

- Drape parts from center to sides
- Divided into left and right independent panels
- Center gap = Total Width × Open Percent
- Open = 0.0 → two panels tightly closed; Open = 1.0 → two panels fully pulled to sides

### 5.4 Tab

- Scalloped opening method
- Bottom edge lifted upward in a scalloped curve shape
- Lift height = Total Height × Open Percent
- Scallop shape controlled by Swag Count (more swags → more scallop arcs)
- Fabric bulges backward in the lifted area to simulate bunching
- Bottom edge forms a serrated curve

---

## 6. Common Usage Examples

### Example 1: Standard Theater Main Curtain

- Drape Type: Main Curtain
- Fabric Type: Velour
- Pleat Pattern: Gathered
- Width: 1200cm (12m)
- Height: 800cm (8m)
- Hanging Height: 900cm
- Fullness: 2.0
- Pleat Count: 24
- Open Type: Traveler

### Example 2: Opera House Austrian Curtain

- Drape Type: Main Curtain
- Fabric Type: Silk
- Pleat Pattern: Austrian
- Width: 1500cm (15m)
- Height: 1000cm (10m)
- Hanging Height: 1100cm
- Fullness: 2.5
- Pleat Count: 30
- Swag Count: 5
- Open Type: Tab

### Example 3: Stage Legs

- Drape Type: Legs
- Fabric Type: Commando
- Pleat Pattern: Gathered
- Width: 200cm (2m)
- Height: 700cm (7m)
- Fullness: 1.5
- Pleat Count: 8
- Open Type: Fixed

### Example 4: Projection Scrim

- Drape Type: Scrim
- Fabric Type: Muslin
- Pleat Pattern: Flat
- Width: 1000cm
- Height: 600cm
- Fullness: 1.0
- Double Sided: Enabled

### Example 5: Border

- Drape Type: Border
- Fabric Type: Velour
- Pleat Pattern: Box Pleat
- Width: 1200cm
- Height: 100cm
- Hanging Height: 800cm
- Fullness: 2.0
- Open Type: Fixed

---

## 7. Notes

1. **Pleat mesh resolution auto-adjusts** — Different pleat patterns use different mesh subdivision levels (Flat is coarsest, Austrian is finest); the system calculates automatically
2. **Mesh cap protection** — Maximum pleat column count is 400, reducing editor and rendering impact from overly large meshes
3. **Fullness = 1.0 means no pleats** — Even if a pleat pattern is selected, no pleats will be generated when fullness is 1.0 (no excess fabric)
4. **The Tab scallop count is driven by Swag Count**, but that field is only editable when the pleat pattern is Austrian; under other pleat patterns Tab uses the stored value (default 4)
5. **Traveler pleats remain continuous** — The pleats of the left and right panels use global coordinate calculation, ensuring continuous pleat patterns when closed
6. **Fabric affects pleats** — Changing fabric type not only changes weight statistics but also changes the actual depth of pleats

> **Disclaimer**: Weight calculation data is for reference only. Actual fabric weight may vary by brand, batch, and other factors. Please use data provided by the fabric supplier.
