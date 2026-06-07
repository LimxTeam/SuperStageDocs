# SuperCAD User Manual

> **SuperStage Professional Construction Drawing System — Complete User Guide**
>
> **Version**: 2.1
> **Applicable Product**: SuperStage (Unreal Engine 5.6–5.7 Plugin)
> **Copyright**: Foshan Yierranran Technology Co., Ltd. (LimxTeam)
> **Document Date**: March 2026

---

## Table of Contents

- [Chapter 1 Product Overview](#chapter-1-product-overview)
- [Chapter 2 Quick Start](#chapter-2-quick-start)
- [Chapter 3 Interface Details](#chapter-3-interface-details)
- [Chapter 4 Document Management](#chapter-4-document-management)
- [Chapter 5 Viewport Controls](#chapter-5-viewport-controls)
- [Chapter 6 Selection Tools](#chapter-6-selection-tools)
- [Chapter 7 Annotation Tools](#chapter-7-annotation-tools)
- [Chapter 8 Line Tools](#chapter-8-line-tools)
- [Chapter 9 Layer Management](#chapter-9-layer-management)
- [Chapter 10 Statistics & Reports](#chapter-10-statistics--reports)
- [Chapter 11 Export Functions](#chapter-11-export-functions)
- [Chapter 12 Page Management](#chapter-12-page-management)
- [Chapter 13 Auxiliary Functions](#chapter-13-auxiliary-functions)
- [Chapter 14 Shortcut Keys Overview](#chapter-14-shortcut-keys-overview)
- [Chapter 15 FAQ](#chapter-15-faq)
- [Appendix Glossary](#appendix-glossary)

---

## Chapter 1 Product Overview

### 1.1 What is SuperCAD

SuperCAD is the professional construction drawing module within the SuperStage plugin, designed specifically for the performance industry. It automatically converts three-dimensional lighting scenes from the Unreal Engine Editor into standard two-dimensional engineering drawings (lighting plots, elevation views, etc.), and provides complete CAD-level annotation, line drawing, statistical reporting, and export capabilities.

**Core Value**: From 3D scene to deliverable engineering drawings, all in one place without switching to external software such as AutoCAD.

### 1.2 What SuperCAD Can Do

| Function | Description |
|----------|-------------|
| **Auto Lighting Plot** | Automatically generate Plan/Front/Side/Rear orthographic views from 3D scenes |
| **Fixture Info Overlay** | Automatically display DMX address, number, type, power, and weight for each fixture |
| **Dimension Annotation** | Measure and annotate distances between fixtures; supports single and continuous dimensioning |
| **Text Annotation** | Add free text descriptions anywhere on the drawing |
| **Leader Annotation** | Text annotations with pointing arrows, directed at specific objects |
| **Area Annotation** | Draw polygonal areas with automatic area calculation |
| **Line Drawing** | Draw power lines, signal lines, and other cable paths |
| **Layer Management** | Assign different elements to different layers with independent display and lock control |
| **Statistical Reports** | Fixture BOM, Universe allocation table, power load table, hanging point load table |
| **Multi-Format Export** | Export as DXF (AutoCAD compatible), PDF, PNG formats |
| **Multi-Page Documents** | One document contains multiple pages (e.g., plan view and front view on separate pages) |

### 1.3 Applicable Scenarios

- Concert / music festival lighting design drawing output
- Theater / stage lighting construction drawing delivery
- Exhibition / event lighting proposal documentation
- Lighting project bidding and tendering drawing production
- Equipment statistics and BOM generation

### 1.4 System Requirements

- Unreal Engine 5.6–5.7 Editor environment
- SuperStage plugin installed
- Fixture Actors placed in the scene (ASuperDmxActorBase and its subclasses)

---

## Chapter 2 Quick Start

### 2.1 Opening SuperCAD

SuperCAD is launched via the Unreal Engine Editor menu:

1. In the UE Editor top menu bar, find the **SuperStage** menu
2. Click the **SuperCAD — Construction Drawing** menu item
3. SuperCAD will open in a standalone window

> **Tip**: You can also enter the `SuperCAD.Open` command in the console to open directly.

### 2.2 First Use

Upon opening SuperCAD, you will see the following:

1. **Viewport Area**: Displays the orthographic plan view of the current scene; fixtures appear in silhouette/outline form
2. **Toolbar**: At the top of the window, containing all operation tool buttons
3. **Status Bar**: At the bottom of the window, displaying current coordinates, zoom scale, fixture statistics, and other information
4. **Layer Panel**: On the left side of the window, managing drawing layers
5. **Page Tab Bar**: Above the status bar at the bottom, managing multiple pages

### 2.3 Five-Minute Quick Start

**Step 1: View the Lighting Plot**
- After opening SuperCAD, the default display is the Plan View
- All fixtures in the scene automatically appear in the view
- Each fixture automatically displays its DMX address (e.g., `U1.001`) and fixture number (e.g., `#001`)

**Step 2: Viewport Navigation**
- **Scroll Wheel**: Zoom the view
- **Middle-Click Drag**: Pan the view
- Press **F** to fit all fixtures to the viewport
- Press **1/2/3/4** to switch between Plan/Front/Side/Rear views

**Step 3: Add Dimension Annotation**
- Press **D** or click the "Annotation" button in the toolbar
- Click the first fixture (start point)
- Click the second fixture (end point)
- Move the mouse to adjust the annotation offset position; click to confirm placement
- Distance values are automatically calculated and displayed

**Step 4: Save Document**
- Press **Ctrl+S** or click the "Save" button in the toolbar
- The first save will pop up a "Save As" dialog; choose a save path and filename
- Documents are saved as UE assets (.uasset) stored in the project Content directory

**Step 5: Export Drawings**
- Click the "Export" button in the toolbar, or directly click the DXF/PDF/PNG quick export buttons
- Choose an export format and save location
- Exported DXF files can be directly opened in AutoCAD

---

## Chapter 3 Interface Details

### 3.1 Overall Layout

The SuperCAD window uses a classic CAD software layout, from top to bottom:

```
┌──────────────────────────────────────────────────────────┐
│  Toolbar                                            32px │
├────────┬─────────────────────────────────┬──────────────┤
│ Layer  │                                 │  Property    │
│ Panel  │      Viewport Area              │  Panel       │
│(collap-│      with Ruler                 │  (collaps-   │
│ sible) │                                 │  ible)       │
│        │                                 │              │
├────────┴─────────────────────────────────┴──────────────┤
│  Statistics Panel (expandable/collapsible)         280px │
├──────────────────────────────────────────────────────────┤
│  Page Tab Bar                                     26px   │
├──────────────────────────────────────────────────────────┤
│  Status Bar                                       22px   │
└──────────────────────────────────────────────────────────┘
```

### 3.2 Toolbar

The toolbar is the main operation entry point for SuperCAD, with the following functional areas arranged from left to right:

#### 3.2.1 File Menu

Click the "**File ▼**" dropdown menu for the following operations:

| Menu Item | Shortcut | Function |
|-----------|----------|----------|
| New | Ctrl+N | Create a new blank drawing document |
| Open | Ctrl+O | Open an existing drawing from Content Browser |
| Save | Ctrl+S | Save the current document |
| Save As | — | Save the document as a new asset |

#### 3.2.2 Drawing Tool Button Group

| Button | Shortcut | Function |
|--------|----------|----------|
| **Select** | S | Select and move drawing elements |
| **Dimension** | D | Add dimension annotations (measure distances) |
| **Text** | T | Add text annotations |
| **Line** | L | Draw lines (power/signal/leader) |
| **Area** | — | Draw area annotations (polygon + area calculation, toolbar button only) |

> **Tip**: The currently active tool button is highlighted. When switching tools, the previous tool automatically exits.

#### 3.2.3 View Control Area

| Button/Control | Function |
|---------------|----------|
| **View Dropdown** | Switch view direction: Plan / Front / Side / Rear |
| **Mode Dropdown** | Switch rendering mode: Line / Shaded / Lit |

#### 3.2.4 Zoom Control Area

| Button | Function |
|--------|----------|
| **Zoom Out (−)** | Zoom out |
| **Zoom In (+)** | Zoom in |
| **Fit** | Zoom to show all fixtures (F) |

#### 3.2.5 Auxiliary Toggle Area

| Button | Shortcut | Function | Default |
|--------|----------|----------|---------|
| **Grid** | G | Show/hide reference grid | On |
| **Snap** | — | Enable/disable smart snap | On |
| **Ortho** | — | Enable/disable ortho constraint | Off |
| **Border** | — | Show/hide drawing border | Off |
| **Axis** | — | Show/hide origin axes | On |

#### 3.2.6 Right-Side Function Area

| Button/Control | Function |
|---------------|----------|
| **Report** | Expand/collapse statistics panel |
| **Unit Dropdown** | Switch drawing units: cm / m / mm / ft / in |
| **Export** | Open export dialog |
| **DXF/PDF/PNG** | Quick export (directly opens file save dialog) |
| **Help** | Open official documentation (https://yunsio.com/docs) |

### 3.3 Viewport Area

The viewport is SuperCAD's core work area, displaying the orthographic projection view of the scene.

**Viewport Characteristics**:
- **Orthographic Projection**: No perspective distortion; all parallel lines remain parallel, suitable for engineering drafting
- **Ruler**: Scale rulers displayed at the top and left of the viewport, with adaptive precision based on zoom level
- **Grid**: Toggleable reference grid with major/minor grid line hierarchy
- **Scale Bar**: Current scale bar displayed within the viewport

**Content Displayed in Viewport**:
- Orthographic projection of all fixtures in the scene (wireframe or shaded mode)
- Fixture info labels (address code, number, type, etc.)
- User-added annotations (dimensions, text, leaders, areas)
- User-drawn lines (power lines, signal lines)
- Reference grid and coordinate axes

### 3.4 Status Bar

The status bar is at the very bottom of the window and continuously displays the following information (from left to right):

| Info | Description | Example |
|------|-------------|---------|
| **Document Name** | Current document name; shows "Untitled" if unsaved, shows * if modified | `MyDrawing*` |
| **View Direction** | Current view direction | `Plan View` |
| **View Mode** | Current rendering mode | `Wireframe` |
| **Current Tool** | Currently active tool | `Select` |
| **Mouse Coordinates** | Mouse coordinates in world space (unit varies with settings) | `X: 120.5 Y: 350.2` |
| **Fixture Count** | Total number of fixtures in the scene | `Fixtures: 48` |
| **Selected Count** | Number of currently selected objects | `Selected: 3` |
| **Total Power** | Sum of all fixture rated power | `Power: 28.5kW` |
| **Total Weight** | Sum of all fixture weights | `Weight: 450kg` |
| **Scale / Unit** | Current drawing scale and unit | `1:100 | m` |

### 3.5 Layer Panel

Located on the left side of the window, collapsible. See [Chapter 9 Layer Management](#chapter-9-layer-management) for details.

### 3.6 Property Panel

Located below the Layer Panel (or on the right side of the window), displays property information and project statistics for the currently selected object.

Statistics displayed in the Property Panel include:
- Total fixture count, fixture type count
- Total power (kW), total weight (kg)
- Universe count, total channel count
- Total line length (m)
- Total annotation count

### 3.7 Page Tab Bar

Located above the Status Bar, displays all page tabs in the current document. See [Chapter 12 Page Management](#chapter-12-page-management) for details.

---

## Chapter 4 Document Management

### 4.1 Document Type

SuperCAD documents are saved as Unreal Engine **UDataAsset** (`.uasset` files) in the project's Content directory. This means:

- Documents natively support version control (Git, Perforce, etc.)
- Documents can be browsed and managed in the UE Content Browser
- Double-clicking a CAD document asset in Content Browser directly opens SuperCAD

### 4.2 New Document

**Operation Methods** (choose one):
1. Toolbar → File Menu → **New**
2. Shortcut **Ctrl+N**
3. Right-click in Content Browser → Miscellaneous → **SuperCAD Document**

When creating a new document:
- If the current document has unsaved changes, the system will prompt "Current drawing has been modified. Save?"
- Choose "Yes" to save before creating new, "No" to discard changes and create new, "Cancel" to return to editing

A new document contains by default:
- One default page ("Page 1", Plan View)
- One default layer ("Default" layer)
- Default scale 1:100
- Default unit: mm

### 4.3 Open Document

**Operation Methods** (choose one):
1. Toolbar → File Menu → **Open**
2. Shortcut **Ctrl+O**
3. Double-click SuperCAD Document asset in Content Browser

The open operation opens an asset picker dialog, displaying only SuperCAD Document type assets. Select the document to open and confirm.

### 4.4 Save Document

**Operation Methods** (choose one):
1. Toolbar → File Menu → **Save**
2. Shortcut **Ctrl+S**

Save behavior:
- **First Save**: If the document has never been saved ("Untitled"), it automatically redirects to the "Save As" flow
- **Subsequent Saves**: Directly overwrite to the existing path

### 4.5 Save As

Toolbar → File Menu → **Save As**

Opens a save dialog where you can choose a new save path and filename. Save As creates a new document asset; the original document is unaffected.

### 4.6 Auto-Remember Last Document

SuperCAD automatically remembers the last opened document path. On the next launch, it will automatically load that document without manual opening.

### 4.7 Close Save Prompt

In the following situations, if the current document has unsaved changes, the system will pop up a save prompt:
- Closing the SuperCAD window
- Creating a new document
- Opening another document

Prompt options:
- **Yes**: Save the current document then proceed
- **No**: Discard changes and proceed
- **Cancel**: Cancel the operation and return to editing

### 4.8 Dirty Flag (Modification Indicator)

An asterisk `*` after the document name in the status bar indicates the document has unsaved modifications. The following operations trigger the dirty flag:
- Adding / deleting / modifying annotations
- Adding / deleting / modifying lines
- Modifying layer settings
- Adding / deleting pages
- Modifying document settings (scale, paper size, etc.)

---

## Chapter 5 Viewport Controls

### 5.1 View Direction

SuperCAD supports four standard orthographic view directions:

| View | Shortcut | Projection Direction | Measurement Plane | Typical Use |
|------|----------|---------------------|-------------------|-------------|
| **Plan View** | 1 | Top-down | XY Plane | Lighting plot (most common) |
| **Front Elevation** | 2 | Front-to-back | XZ Plane | Front elevation |
| **Side Elevation** | 3 | Side view | YZ Plane | Side elevation |
| **Rear Elevation** | 4 | Back-to-front | XZ Plane | Rear elevation |

**Switching Methods**:
- Press number keys **1/2/3/4**
- Select from the toolbar View dropdown

### 5.2 View Mode

SuperCAD provides three rendering modes:

| Mode | Shortcut | Description | Recommended Use |
|------|----------|-------------|-----------------|
| **Line** | O cycle | Only shows object outlines, closest to traditional CAD | Drafting, annotation |
| **Shaded** | O cycle | Shows base color and form of objects | Equipment identification |
| **Lit** | O cycle | Full lighting-rendered effect | Visual preview |

**Switching Methods**:
- Press **O** to cycle through the three modes (Line → Shaded → Lit → Line)
- Select from the toolbar Mode dropdown

> **Tip**: In Line mode, objects in different layers can display different wireframe colors for easy differentiation.

### 5.3 Viewport Navigation

#### 5.3.1 Pan

| Operation | Method |
|-----------|--------|
| **Middle-Click Drag** | Hold middle mouse button and drag to pan the view |
| **Right-Click Drag** | Hold right mouse button and drag to pan the view |

#### 5.3.2 Zoom

| Operation | Method |
|-----------|--------|
| **Scroll Up** | Zoom in (centered on mouse position) |
| **Scroll Down** | Zoom out (centered on mouse position) |
| **Toolbar +/−** | Click zoom in/out buttons |

Zoom range: From extremely close (centimeter level) to extremely far (kilometer level)

#### 5.3.3 Fit View

| Operation | Shortcut | Description |
|-----------|----------|-------------|
| **Fit All** | F | Zoom and center so all fixtures are visible in the viewport |
| **Fit Selection** | Shift+F | Zoom and center on currently selected fixtures |

Fit All can also be accessed via:
- Toolbar "Fit" button
- Right-click menu → Fit All

### 5.4 Grid Display

The grid is an important reference system for CAD drafting. SuperCAD's grid has the following characteristics:

- **Adaptive Density**: Automatically adjusts grid spacing based on zoom level, always remaining clearly legible
- **Major/Minor Grid Lines**: Major grid lines (thick) and minor grid lines (thin) alternate
- **Unit-Linked**: Grid ticks automatically adapt to the drawing unit

**Control Methods**:
- Press **G** to toggle grid display on/off
- Toolbar "Grid" button

### 5.5 Axis Display

Origin axes (red X-axis, green Y-axis) help confirm spatial orientation.

**Control Methods**:
- Toolbar "Axis" button

---

## Chapter 6 Selection Tools

### 6.1 Overview

The Selection tool is SuperCAD's default tool for selecting and manipulating objects in the drawing (fixtures, annotations, lines, etc.). Press **S** or click the "Select" button in the toolbar to switch to the selection tool.

### 6.2 Point Selection

**Operation**: Left-click an object in the viewport.

- Click a fixture: Selects that fixture
- Click an annotation: Selects that annotation (dimension, text, leader, area)
- Click a line: Selects that line
- Click empty space: Deselects all objects

### 6.3 Modifier Keys

| Modifier Key | Effect |
|-------------|--------|
| **Shift+Click** | Add the object to the current selection set (does not deselect already selected) |
| **Ctrl+Click** | Toggle the object's selected state (deselect if selected, add if not) |
| No modifier + Click | Clear previous selection, select only the current object |

### 6.4 Box Selection (CAD Dual Mode)

SuperCAD implements the dual-mode box selection found in professional CAD software:

#### 6.4.1 Window Selection (Left-to-Right Drag)

- **Operation**: Drag the mouse from left to right
- **Appearance**: Blue solid-line rectangle
- **Rule**: Only selects objects **completely inside** the box

Applicable Scenario: Precisely selecting all objects within a region, excluding objects only partially inside the box.

#### 6.4.2 Crossing Selection (Right-to-Left Drag)

- **Operation**: Drag the mouse from right to left
- **Appearance**: Green dashed-line rectangle
- **Rule**: Selects objects **inside or intersecting** the box

Applicable Scenario: Quickly selecting all objects within a region and on its boundaries.

> **Tip**: This is standard behavior in professional CAD software such as AutoCAD; users with existing CAD experience can seamlessly transition.

### 6.5 Select All

- Shortcut **Ctrl+A**
- Toolbar → File Menu → Select All

Select All selects all fixtures in the view.

### 6.6 Delete Selected

- Shortcut **Delete**
- Right-click menu → Delete

The Delete operation only deletes user-created elements such as annotations and lines, and does not delete fixture Actors in the scene.

### 6.7 Selection Feedback

When objects are selected:
- The status bar displays the selection count
- Selected annotations/lines are displayed in highlight color
- Selected fixtures are synchronized with the UE Editor's selection system (corresponding fixtures in the 3D viewport are also selected)

---

## Chapter 7 Annotation Tools

### 7.1 Dimension Annotation Tool

#### 7.1.1 Function Description

Dimension annotation is used to measure and annotate the distance between two points. Distance values are automatically calculated on the corresponding plane based on the current view direction:

| View Direction | Measurement Formula |
|---------------|---------------------|
| Plan View | √((x₂−x₁)² + (y₂−y₁)²) — XY plane distance |
| Front Elevation | √((x₂−x₁)² + (z₂−z₁)²) — XZ plane distance |
| Side Elevation | √((y₂−y₁)² + (z₂−z₁)²) — YZ plane distance |

Distance values are automatically converted and displayed according to the drawing unit (e.g., when switching to meters, 1500cm displays as 15.00m).

#### 7.1.2 Operation Steps (Three-Step Interaction)

1. **Switch to Dimension Tool**: Press **D** or click "Dimension" in the toolbar
2. **Click Start Point**: Click the first position in the viewport (the system automatically snaps to the nearest fixture anchor point)
3. **Click End Point**: Click the second position in the viewport
4. **Adjust Offset**: Move the mouse to adjust the distance between the dimension line and measurement line, click to confirm placement

After placing the dimension, the system automatically calculates and displays the distance value.

#### 7.1.3 Continuous Dimension Mode

Continuous dimensioning is the most commonly used annotation method on lighting plots — continuously measuring the spacing between a row of fixtures.

**Operation**:
1. Select "Continuous Dimension" mode in the toolbar (or choose from the dimension dropdown)
2. Click the first fixture
3. Click the second fixture → Automatically annotates the spacing between the two points
4. Continue clicking the next fixture → The second point automatically becomes the new start point
5. Repeat step 4 until annotation is complete
6. Press **ESC** or double-click to finish

**Features**:
- After committing, the end point automatically continues as the next start point
- The offset distance is locked after the first setting; subsequent annotations automatically use the same offset
- Highly efficient, suitable for annotating the spacing of a row of fixtures

#### 7.1.4 Annotation Auto-Update

When annotated fixtures (or objects) are moved in the 3D scene:
- If the annotation endpoints are bound to fixture anchor points, they automatically follow the movement
- Distance values are automatically recalculated and updated
- The dimension line geometry is automatically rebuilt

#### 7.1.5 Cancel / Step Back

- Press **ESC**: Step back one step (returns to start point state if end point is already set)
- Press **ESC** again: Completely cancel the dimension tool

### 7.2 Text Annotation Tool

#### 7.2.1 Function Description

Add free text descriptions anywhere on the drawing.

#### 7.2.2 Operation Steps

1. **Switch to Text Tool**: Press **T** or click "Text" in the toolbar
2. **Click Placement Position**: Click the position in the viewport where the text should be placed
3. **Enter Text**: Type text content directly from the keyboard
   - Blinking cursor indicates input mode is active
   - Supports letters, numbers, punctuation
   - **Shift+Enter**: Line break
   - **BackSpace**: Delete last character
4. **Confirm**: Press **Enter** to confirm the text content
5. The text annotation is written to the document

#### 7.2.3 Cancel

- Press **ESC**: Cancel the current text input and clear entered content

### 7.3 Area Annotation Tool

#### 7.3.1 Function Description

Draw polygonal areas; the system automatically calculates and annotates the area.

#### 7.3.2 Operation Steps

1. **Switch to Area Tool**: Press **A** or click "Area" in the toolbar
2. **Click Vertices Sequentially**: Click in the viewport to place the polygon's vertices (at least 3)
   - Each click adds a vertex
   - Real-time preview of polygon outline and edges formed with the current mouse position
3. **Close Polygon**: Double-click or press **Enter** to close the polygon
   - The system automatically calculates the area using the Shoelace formula
   - The area value is automatically filled into the text annotation (unit: square meters)
4. **Edit Area Text** (Optional): After closing, you can modify the auto-generated area text
5. **Confirm**: Press **Enter** to confirm

#### 7.3.3 Display Effect

- Semi-transparent fill color shows the area range
- Outline marks the boundaries
- Vertex positions show markers
- The area centroid position displays the area value

#### 7.3.4 Cancel

- **ESC**: In text editing phase, cancel and return to drawing phase
- Press **ESC** again in drawing phase: Completely cancel

---

## Chapter 8 Line Tools

### 8.1 Overview

The Line tool is used for drawing various electrical line paths, including power lines, signal lines, and leader annotations. SuperCAD unifies leader annotation and cable drawing into a single tool, switching between types via selection.

### 8.2 Line Types

| Type | Shortcut | Line Style | Color | Use |
|------|----------|-----------|-------|-----|
| **Leader Annotation** | L | Solid line + arrow | White | Text annotation with pointing arrow |
| **Three-Phase Power** | L (cycle) | Thick solid line | Red | Main distribution lines |
| **DMX Signal** | L (cycle) | Dashed line | Blue | DMX signal links |

**Type Switching**: When the line tool is already active, press **L** again to cycle through the types.

### 8.3 Leader Annotation Operation

Leader annotations are text annotations with pointing arrows, typically used for annotating detailed descriptions of specific fixtures or areas.

1. **Switch to Line Tool**: Press **L** (default leader mode)
2. **Place Leader Start Point**: Click the leader's start position in the viewport (usually near the object to annotate)
3. **Place Bend Points** (Optional): Continue clicking to add bend points for the leader
4. **End Leader**: Double-click or press **Enter** to finish leader drawing
5. **Enter Annotation Text**: Enter text input mode
   - Input the leader annotation text content
   - **Enter** to confirm
   - **ESC** to cancel text input

### 8.4 Cable Drawing Operation

Power and signal lines are used to represent electrical connection relationships between equipment.

1. **Select Cable Type**: Cycle to the target type using the **L** key
2. **Place Start Point**: Click the cable's start position
   - If snapped to a fixture anchor point, the cable is automatically associated with the fixture
3. **Place Path Nodes**: Continue clicking to add polyline bend points
4. **End Drawing**: Double-click or press **Enter** to finish

**Cable-Fixture Association**:
- When cable endpoints snap to fixtures, associations are automatically established
- When fixtures move in the 3D scene, associated cable endpoints follow the movement

### 8.5 Step Back & Cancel

- **ESC (once)**: Step back the last path node
- **ESC (continue pressing)**: Step back nodes one by one until all nodes are cleared, then cancel the tool
- Press **ESC** during text input in leader mode: Cancel text input and return to polyline state

---

## Chapter 9 Layer Management

### 9.1 Overview

The layer system allows you to organize different elements of the drawing into different levels, independently controlling their display, hiding, and locking states. This is a standard feature of professional CAD software.

### 9.2 Layer Panel

The Layer Panel is on the left side of the window, displaying all layers of the current document. Each layer entry shows:
- **Layer Name**: Customizable
- **Color**: The layer's wireframe color (effective in Line mode)
- **Visibility Icon** (): Click to toggle layer display/hide
- **Lock Icon** (): Click to toggle layer locked/unlocked

### 9.3 Default Layer

A "Default" layer is automatically created for new documents. All fixtures and annotations not assigned to a specific layer belong to this layer by default.

### 9.4 Add Layer

Click the "+" button at the bottom of the Layer Panel and enter a layer name to add a new layer.

### 9.5 Delete Layer

Select a layer and click the "−" button to delete it. The Default layer cannot be deleted.

### 9.6 Layer Properties

| Property | Description |
|----------|-------------|
| **Name** | The layer's display name |
| **Color** | The layer's display color in wireframe mode, click to modify |
| **Visibility** | Controls whether the layer is displayed in the viewport |
| **Locked** | Objects on locked layers cannot be edited |

### 9.7 Assign Objects to Layers

**Method 1: Via Right-Click Menu**
1. Select the fixtures or annotations to assign
2. Right-click → Choose "Assign Layer" from the context menu
3. Select the target layer from the submenu

**Auto Assignment**: When using annotation/line tools to create new elements, they are automatically assigned to the currently active layer.

### 9.8 Layer Color & Wireframe Mode

In Line view mode:
- Fixtures in different layers display wireframes in their respective layer colors
- When the layer color is modified, the wireframe color of all objects in that layer updates immediately
- In Shaded and Lit modes, layer colors do not affect display

### 9.9 Layer Visibility

- **Hide Layer**: All fixtures and annotations on that layer disappear from the viewport
- **Show Layer**: Restore display
- Hidden layers do not affect export (all layers are included during export, following their respective printable settings)

### 9.10 Layer Lock

- **Lock Layer**: Objects on that layer cannot be selected, edited, or deleted
- **Unlock Layer**: Restore editable state
- Attempting to use annotation/line tools on locked layers results in the operation being ignored

---

## Chapter 10 Statistics & Reports

### 10.1 Real-Time Statistics

The status bar and property panel continuously display the following statistical information:
- **Total Fixtures**: Number of all fixture Actors in the scene
- **Total Power**: Sum of all fixture rated power (kW)
- **Total Weight**: Sum of all fixture weights (kg)
- **Universe Count**: Number of DMX Universes used
- **Total Channels**: Total DMX channels occupied by all fixtures
- **Total Line Length**: Total length of all drawn line paths (m)
- **Total Annotations**: Total number of dimension and text annotations

This data is automatically collected from the 3D scene by the scene crawler in real time.

### 10.2 Statistics Report Panel

Click the "Report" button in the toolbar to expand/collapse the statistics report panel. The report panel contains four tabs:

#### 10.2.1 Fixture BOM (Bill of Materials)

Summary statistics grouped by fixture type:

| Column | Description |
|--------|-------------|
| # | Auto number |
| Fixture Type | Display name from fixture profile |
| Quantity | Total count of this fixture type |
| Power (W) | Rated power per unit |
| Subtotal (W) | Quantity × per-unit power |
| Weight (kg) | Weight per unit |
| Subtotal (kg) | Quantity × per-unit weight |
| Channels | DMX channel span |

The bottom summary row shows: Total quantity, total power, total weight.

#### 10.2.2 Universe Allocation Table

DMX address usage statistics grouped by Universe:

| Column | Description |
|--------|-------------|
| Universe | Universe number |
| Fixtures | Number of fixtures on this Universe |
| Used Channels | Used channel range |
| Free Channels | Number of unused channels |
| Utilization | Used channels / 512 |

#### 10.2.3 Power Load Table

Load statistics grouped by power circuit:

| Column | Description |
|--------|-------------|
| Circuit | Power line circuit number |
| Protection | Protection device type |
| Line Label | Line name for this circuit |
| Fixtures | Number of fixtures connected to this circuit |
| Load (W) | Total power of connected fixtures |
| Cable Spec | Cable specification used |

#### 10.2.4 Hanging Point Load Table

Per-fixture weight and height information:

| Column | Description |
|--------|-------------|
| # | Auto number |
| Fixture Type | Display name from fixture profile |
| Fixture No. | Fixture number (FixtureID) |
| Weight (kg) | Weight per unit |
| Position Z (m) | Fixture mounting height (world coordinate Z axis) |

> **Tip**: Statistics reports only compute when the panel is expanded; no performance impact when collapsed.

---

## Chapter 11 Export Functions

### 11.1 Supported Export Formats

| Format | Extension | Description | Applicable Scenarios |
|--------|-----------|-------------|----------------------|
| **DXF** | .dxf | AutoCAD Drawing Exchange Format (R12) | CAD software interoperability, construction delivery |
| **PDF** | .pdf | Portable Document Format | Viewing, printing, sharing |
| **PNG** | .png | Portable Network Graphics | Quick sharing, document embedding |

### 11.2 Export Operation

#### Method 1: Export Dialog

1. Click the "**Export**" button in the toolbar
2. Configure parameters in the export dialog:
   - Select export format (DXF / PDF / PNG)
   - Select paper size
   - Select scale
   - Select resolution multiplier (PDF/PNG)
   - Select whether to include grid, border, legend
   - Select whether to batch export
3. Click the "Export" button
4. Choose save location and filename

#### Method 2: Quick Export

Three quick export buttons (DXF / PDF / PNG) on the right side of the toolbar directly open the file save dialog and export with default settings.

### 11.3 DXF Export Details

SuperCAD exports DXF R12 format (ASCII text), compatible with all CAD software.

**Layer Mapping**:

| CAD Layer | DXF Layer Name | Content |
|-----------|---------------|---------|
| Fixture Outlines | FIXTURES | Fixture outline lines |
| DMX Address | DMX_ADDRESS | Address code text |
| Fixture Number | FIXTURE_ID | Number text |
| Dimensions | DIMENSIONS | Dimension lines and text |
| Annotations | ANNOTATIONS | Text and leaders |
| Power Cables | POWER_CABLES | Power lines |
| Signal Cables | SIGNAL_CABLES | Signal lines |
| Title Block | TITLEBLOCK | Border lines and title block |

**Entity Mapping**:
- Lines → LINE
- Polylines / Leaders / Cables / Area boundaries → LWPOLYLINE
- Text / Dimension values → TEXT

> **Note**: Advanced entities such as DIMENSION, HATCH, and MTEXT are not used to ensure the broadest CAD software compatibility.

### 11.4 PDF Export Details

PDF export is generated via viewport screenshot, capturing the current viewport image and embedding it into a PDF file. Output resolution is configurable.

### 11.5 PNG Export Details

PNG screenshot export directly captures the current viewport's rendered image:
- Supports 1x, 2x, 4x supersampling resolutions
- Suitable for quick sharing and document embedding

---

## Chapter 12 Page Management

### 12.1 Overview

SuperCAD documents support multiple pages, analogous to multiple sheets in a set of construction drawings. Each page can have an independent view direction, view mode, zoom level, and viewport position.

### 12.2 Page Tab Bar

Located above the status bar, displaying tabs for all pages.

- **Click a Tab**: Switch to that page
- **Current Page**: Highlighted
- **"+" Button**: Add a new page
- **"" Button**: Delete the page (at least one page must remain)

### 12.3 Add Page

Click the "+" button on the page tab bar. New pages default to the name "Page N" (N is the sequence number) with the Plan View direction.

### 12.4 Switch Pages

Click the tab of the desired page to switch. When switching:
- The current page's view state (direction, mode, position, zoom) is automatically saved
- The target page's view state is automatically loaded
- Toolbar and status bar information is synchronously updated

### 12.5 Delete Page

Click the "" button on the page tab. A page cannot be deleted if only one remains.

### 12.6 Page-Independent Settings

Each page independently saves the following settings:
- View direction (Plan / Front / Side / Rear)
- View mode (Line / Shaded / Lit)
- Viewport center position
- Zoom level
- Grid display toggle
- Border display toggle

### 12.7 Typical Multi-Page Usage

| Page | View Direction | Use |
|------|---------------|-----|
| Page 1 | Plan View | Lighting plot (main drawing) |
| Page 2 | Front Elevation | Front elevation view |
| Page 3 | Side Elevation | Side elevation view |
| Page 4 | Plan View | Power line diagram (lighting plot + power lines) |

---

## Chapter 13 Auxiliary Functions

### 13.1 Smart Snap System

The snap system helps you precisely place annotation endpoints and line nodes.

#### Snap Types & Priority (High to Low)

| Priority | Type | Icon | Description |
|----------|------|------|-------------|
| 1 | **Fixture Anchor** |  | Fixture geometric center, most commonly used |
| 2 | **Annotation Endpoint** |  | Start/end points of existing annotations |
| 3 | **Grid Intersection** | + | Intersection of grid lines |

#### Snap Behavior

- When the mouse is near a snappable object (within 10 pixels), the cursor automatically snaps to that point
- The snap icon indicating the current snap type is displayed in the viewport
- Snapped coordinates are used for precise placement of annotations/lines

#### Snap Toggle

The snap function can be toggled on/off via the "Snap" button in the toolbar. When off, the mouse position is unrestricted and can be placed freely.

### 13.2 Ortho Constraint

With ortho constraint enabled, line and annotation directions are restricted to horizontal or vertical (0/90), similar to AutoCAD's ORTHO mode.

**Control**: "Ortho" button in the toolbar

### 13.3 Drawing Border

The border is the standard frame and title block for engineering drawings.

**Control**: The "Border" button in the toolbar toggles the display of the drawing border.

The border includes:
- Outer frame line (thick solid line)
- Title block (bottom right corner, containing project name, designer, date, and other information)

### 13.4 Undo & Redo

SuperCAD integrates Unreal Engine's transaction system, supporting full undo/redo:

| Operation | Shortcut |
|-----------|----------|
| Undo | Ctrl+Z |
| Redo | Ctrl+Y |

The following operations can be undone:
- Adding annotations
- Deleting annotations
- Adding / deleting lines
- Adding / deleting layers
- Modifying layer properties
- Adding / deleting pages
- Assigning layers

### 13.5 Right-Click Context Menu

Right-click in the viewport to open a context menu. The menu content dynamically changes based on the current selection state:

#### Right-Click on Empty Area

| Menu Item | Function |
|-----------|----------|
| Fit All | Zoom to show all fixtures |
| View Settings | Open the left panel |
| Refresh Scene Data | Force rescan fixture data in the scene |

#### Right-Click with Fixtures Selected

| Menu Item | Function |
|-----------|----------|
| Locate in 3D Viewport | Focus on the selected fixture in UE main viewport |
| Assign Layer  | Assign selected fixtures to a specific layer |
| Delete | Delete selected annotations/lines |

#### Right-Click with Annotations/Lines Selected

| Menu Item | Function |
|-----------|----------|
| Assign Layer  | Assign selected annotations/lines to a specific layer |
| Set Line Style  Solid/Dashed | Modify the line style |
| Delete | Delete selected annotations/lines |

### 13.6 Unit Switching

SuperCAD supports the following drawing units:

| Unit | Abbreviation | Description |
|------|-------------|-------------|
| mm | mm | Default unit |
| cm | cm | Matches UE internal coordinates |
| m | m | Common engineering unit |
| ft | ft | Imperial unit |
| in | in | Imperial unit |

**Switching Method**: Unit dropdown on the right side of the toolbar

After switching units:
- Status bar coordinate display is automatically converted
- Dimension annotation distance values are automatically converted (e.g., 150cm  1.5m)
- Grid ticks adapt to the new unit

### 13.7 Fixture Information Display

SuperCAD automatically displays technical information next to each fixture. The types of information displayed can be controlled through document settings:

| Info | Format | Color | Example |
|------|--------|-------|---------|
| DMX Address | U{Universe}.{Address} | Cyan | U1.001 |
| Fixture Number | #{FixtureID} | White | #001 |
| Fixture Type | Profile name | Light Gray | Robe BMFL Spot |
| Power | {Wattage}W | Orange | 800W |
| Weight | {Weight}kg | Red | 25kg |

**Auto Update**: Fixture information is collected in real time by the scene crawler. When fixtures move in the 3D scene, DMX configuration changes, or fixture profile properties are modified, the information in the CAD view is automatically updated.

**LOD Control**: At farther zoom levels, fixture labels are automatically hidden to keep the view clear.

### 13.8 Online Help

Click the "Help" button in the toolbar to open the SuperStage official documentation website in the default browser:
https://yunsio.com/docs/supercad

---

## Chapter 14 Shortcut Keys Overview

### 14.1 Tool Switching

| Shortcut | Function |
|----------|----------|
| S | Select Tool |
| D | Dimension Annotation Tool |
| T | Text Annotation Tool |
| L | Line Tool (leader/power/signal; press L again to cycle types) |

### 14.2 View Controls

| Shortcut | Function |
|----------|----------|
| 1 | Plan View |
| 2 | Front Elevation |
| 3 | Side Elevation |
| 4 | Rear Elevation |
| F | Fit All |
| Shift+F | Fit Selection |
| G | Toggle grid display |
| O | Cycle view mode (LineShadedLit) |

### 14.3 File Operations

| Shortcut | Function |
|----------|----------|
| Ctrl+N | New Document |
| Ctrl+O | Open Document |
| Ctrl+S | Save Document |
| Ctrl+W | Close Window |

### 14.4 Editing Operations

| Shortcut | Function |
|----------|----------|
| Ctrl+Z | Undo |
| Ctrl+Y | Redo |
| Ctrl+A | Select All |
| Delete | Delete selected annotations/lines |
| ESC | Cancel current tool operation / deselect |

### 14.5 Mouse Operations

| Operation | Function |
|-----------|----------|
| Left Click | Select / Place annotation point (depends on current tool) |
| Left Drag | Box selection (under Select tool) |
| Middle-Click Drag | Pan view |
| Right Click | Context menu |
| Right-Click Drag | Pan view |
| Scroll Up | Zoom in (centered on mouse position) |
| Scroll Down | Zoom out (centered on mouse position) |
| Double-Click | End drawing (annotation/line tools) |

---

## Chapter 15 FAQ

### Q1: Viewport is blank after opening SuperCAD?

**Cause**: No fixture Actors in the scene.

**Solution**:
1. Confirm that SuperDMX fixture Actors (ASuperDmxActorBase and its subclasses) have been placed in the UE level
2. Press **F** to fit all, centering the viewport on the fixture area
3. If fixtures exist but are not visible, check if layers are hidden in the Layer Panel

### Q2: Address codes and numbers are not displayed next to fixtures?

**Cause**:
- Labels are automatically hidden at far zoom levels — try zooming in
- Fixtures are not configured with DMX addresses or profiles — check the fixture Actor's property settings
- Information display may be turned off — check document display settings

### Q3: Annotation distance values are incorrect?

**Cause**: Annotation distances are calculated on the plane corresponding to the current view direction (projected distance), not the true 3D distance.

**Solution**:
- Annotations in Plan View measure distances on the horizontal plane (XY)
- To measure height differences, switch to Front Elevation or Side Elevation views

### Q4: How to modify a placed annotation?

In the current version, annotation text content cannot be directly edited after placement. To modify:
1. Select the annotation using the Select tool
2. Press **Delete** to remove it
3. Recreate using the annotation tool

### Q5: Exported DXF file won't open in AutoCAD?

**Solution**:
- SuperCAD exports DXF R12 format, compatible with all CAD software
- Confirm the file extension is `.dxf`
- Try using "Open" rather than "Import" to load the DXF file
- Confirm file encoding is UTF-8

### Q6: Will data be lost after closing SuperCAD?

**Answer**:
- If the document has been saved (Ctrl+S), data will not be lost
- When closing the window, if there are unsaved changes, the system will pop up a save prompt
- The next time SuperCAD is opened, the last opened document will be automatically loaded

### Q7: How to manage CAD documents in Content Browser?

SuperCAD documents are standard UE assets:
- SuperCAD Document type assets can be seen in Content Browser
- Double-click to open
- Right-click  Miscellaneous  SuperCAD Document to create a new document
- Supports standard asset operations such as rename, move, and copy

### Q8: Extra yellow/red wireframes appear in the viewport?

**Cause**: UE Editor Builder Brush or BSP geometry auxiliary lines.

**Solution**: SuperCAD has built-in filtering mechanisms to automatically hide these auxiliary lines. If they still appear, try Right-Click Menu  Refresh Scene Data.

### Q9: How to refresh scene data?

When the 3D scene undergoes significant changes, you can manually force a refresh:
- Right-Click Menu  **Refresh Scene Data**

Under normal circumstances, the scene crawler automatically detects fixture additions, deletions, and position changes without manual refresh.

### Q10: How to control fixture colors in wireframe mode?

In wireframe mode, fixture colors are determined by their layer color:
1. Click the layer's color swatch in the Layer Panel
2. Select a new color in the color picker
3. The wireframe color of all fixtures in that layer updates immediately

---

## Appendix Glossary

| Term | Description |
|------|-------------|
| Lighting Plot / Light Plot | Top-down fixture layout drawing, the core of construction drawings |
| Orthographic Projection | A projection without perspective distortion, keeping parallel lines parallel |
| Silhouette / Outline | Object external contour lines (not wireframe mesh lines) |
| Title Block / Drawing Border | The border and title block of an engineering drawing |
| Scale | Ratio of drawing distance to actual distance, e.g., 1:100 |
| Dimensioning | Annotating geometric quantities such as distances and angles on drawings |
| Leader Annotation | Text annotation with a pointing arrow |
| Area Annotation | Area annotation for polygonal regions |
| Continuous Dimension | A dimensioning method for continuously measuring multiple intervals |
| Box Selection | Selecting objects by dragging a rectangular box |
| Window Selection | Left-to-right drag, selects only objects fully inside the box |
| Crossing Selection | Right-to-left drag, selects objects inside or intersecting the box |
| Snap | Mouse cursor automatically snapping to specific positions (fixture centers, endpoints, etc.) |
| Ortho Constraint | Restricting drawing direction to horizontal or vertical |
| Layer | A level used for grouping and managing drawing elements |
| Dirty Flag | Indicates the document has unsaved modifications |
| DXF | Drawing Exchange Format — AutoCAD graphic exchange format |
| BOM | Bill of Materials |
| DMX | Digital Multiplex — lighting control protocol |
| Universe | DMX Universe — a DMX bus (512 channels) |
| Daisy Chain | DMX series connection method |
| Anchor Point | The geometric center point of a fixture, used for snapping and annotation |
| Scene Crawler | Engine that automatically scans 3D scene fixture data |
| Content Browser | UE Editor's asset browser |
| UDataAsset | UE data asset type, used for storing structured data |

---

> ** 2026 Foshan Yierranran Technology Co., Ltd. (LimxTeam). All rights reserved.**
> **SuperStage and SuperCAD are trademarks of LimxTeam.**
