# SuperStage Fixture Array Tool — User Manual

## 1. Overview

The Fixture Array Tool is SuperStage's interactive batch fixture arrangement tool. It can duplicate an existing fixture into an array using **linear**, **grid**, or **radial** patterns, with real-time preview. After confirming the arrangement, create all fixtures with one click, greatly increasing efficiency for large-scale fixture layouts.

---

## 2. Access

**Access**: In the editor's left mode selection bar, switch to **SuperStage Edit Mode**, then select **Fixture Array Tool** from the mode toolbar.

> **Prerequisite**: You must first select a SuperStage DMX fixture in the 3D viewport as the "source fixture".

---

## 3. Workflow

### Step 1: Select Source Fixture

Select a fixture Actor in the 3D viewport. This fixture serves as the template for the array — each new fixture in the array copies its type and configuration.

### Step 2: Open the Array Tool

Open the Fixture Array Tool via the main menu. The tool automatically detects your selected fixture.

### Step 3: Select Array Mode

Choose the arrangement mode in the tool panel (Linear / Grid / Radial).

### Step 4: Adjust Parameters

Adjust the corresponding parameters based on the selected mode. Semi-transparent **preview fixtures** appear in real time in the scene, letting you see the arrangement before creating.

### Step 5: Create the Array

Once satisfied with the preview, click the **"Create"** button. All preview fixtures become official scene fixtures.

> **Tip**: The source fixture is removed after array creation; its position is taken by the first fixture in the array.

---

## 4. Array Mode Details

### 4.1 Linear Array

Arrange fixtures at equal intervals along a straight line.

```
[F1] —spacing— [F2] —spacing— [F3] —spacing— [F4]
```

#### Parameters

| Parameter | Type | Description | Range | Default |
|-----------|------|-------------|-------|---------|
| **Count** | int32 | Number of fixtures in the array | 1 - 100 (UI limit 50) | 5 |
| **Spacing** | FVector | 3D spacing vector between adjacent fixtures (cm) | Unlimited | (100, 0, 0) |

> **Note**: Spacing is a 3D vector; you can simultaneously control X/Y/Z spacing to achieve diagonal arrangements.

#### Examples

- **A row of truss lights**: Count=8, Spacing=(150, 0, 0)
- **Diagonal arrangement**: Count=5, Spacing=(100, 0, 50)

---

### 4.2 Grid Array

Arrange fixtures in a 2D grid (row-column matrix).

```
[F1] [F2] [F3] [F4]
[F5] [F6] [F7] [F8]
[F9] [F10][F11][F12]
```

#### Parameters

| Parameter | Type | Description | Range | Default |
|-----------|------|-------------|-------|---------|
| **Count X** | int32 | Number of fixtures in the horizontal direction | 1 - 50 (UI limit 20) | 3 |
| **Count Y** | int32 | Number of fixtures in the vertical direction | 1 - 50 (UI limit 20) | 3 |
| **Spacing X** | float | Horizontal spacing between adjacent fixtures (cm) | ≥ 1.0 | 100.0 |
| **Spacing Y** | float | Vertical spacing between adjacent fixtures (cm) | ≥ 1.0 | 100.0 |
| **Plane** | EFixtureArrayAxis | Normal direction of the grid plane | X / Y / Z | Z |
| **Center At Source** | bool | Whether to center the grid on the source fixture position | On/Off | On |
| **Honeycomb Offset** | float | Horizontal offset for odd/even rows (0 = no offset, 0.5 = half-spacing honeycomb) | 0.0 - 1.0 (UI limit 0.5) | 0.0 |

#### Plane Normal Axis Description

| Normal Axis | Grid Plane | Use Case |
|-------------|------------|----------|
| **Z** | Horizontal plane (XY plane) | Floor light arrays, ceiling light arrays |
| **Y** | Front-back plane (XZ plane) | Backdrop wall light arrays |
| **X** | Left-right plane (YZ plane) | Side light arrays |

#### Honeycomb Offset Description

Honeycomb Offset is a continuous value (not a toggle), with the effect at 0.5:

```
Regular Grid (0.0):     Honeycomb Grid (0.5):
[1] [2] [3] [4]         [1] [2] [3] [4]
[5] [6] [7] [8]           [5] [6] [7] [8]
[9] [10][11][12]        [9] [10][11][12]
```

Odd rows (rows 2, 4, ...) are offset right by `SpacingX × HoneycombOffset`. A value of 0.5 forms a standard honeycomb/brick-wall arrangement.

---

### 4.3 Radial Array

Arrange fixtures in a circle or arc.

```
        [F2]
    [F1]     [F3]
              
  [F8]         [F4]
              
    [F7]     [F5]
        [F6]
```

#### Parameters

| Parameter | Type | Description | Range | Default |
|-----------|------|-------------|-------|---------|
| **Count** | int32 | Number of fixtures on the circumference | 2 - 100 (UI limit 36) | 8 |
| **Radius** | float | Radius of the circle (cm) | ≥ 10.0 (UI starts at 50.0) | 200.0 |
| **Start Angle** | float | Angular position of the first fixture (degrees) | 0° - 360° | 0° |
| **End Angle** | float | Angular position of the last fixture (degrees, 360 = full circle) | 0° - 360° | 360° |
| **Rotation Axis** | EFixtureArrayAxis | Rotation axis for the radial array | X / Y / Z | Z |
| **Orient To Center** | bool | Whether each fixture automatically rotates to face the center | On/Off | **On** |
| **Include Center** | bool | Whether to additionally place a fixture at the center | On/Off | Off |

#### Angle Description

- **Start Angle 0° / End Angle 360°**: Full circle (first and last do not overlap)
- **Start Angle 0° / End Angle 180°**: Half-circle arc
- **Start Angle 45° / End Angle 315°**: 270° arc

#### Orient To Center

| State | Description |
|-------|-------------|
| **Off** | All fixtures maintain the source fixture's original orientation |
| **On** | Each fixture automatically rotates so its front faces the center |

Suitable for ring light arrays where all lights need to shine toward the center stage.

#### Include Center

| State | Description |
|-------|-------------|
| **Off** | Only place fixtures on the circumference |
| **On** | Place fixtures on the circumference + 1 additional fixture at the center |

---

## 5. Common Parameters

The following parameters apply to all array modes, located under the **Common** category:

| Parameter | Type | Description | Default |
|-----------|------|-------------|---------|
| **Array Rotation** | FRotator | Rotation of the entire array around the source fixture position (Pitch/Yaw/Roll) | (0, 0, 0) |
| **Item Rotation** | FRotator | Additional rotation offset for each fixture (Pitch/Yaw/Roll) | (0, 0, 0) |

### 5.1 Outliner Folder (Organization Category)

| Parameter | Type | Description | Default |
|-----------|------|-------------|---------|
| **Create Folder** | bool | Whether to place all generated Actors into a World Outliner folder after creation | On |
| **Folder Name** | FString | Folder name (auto-generated from source fixture type if left empty); detects naming conflicts | Empty (auto-generated) |

---

## 6. Status Information

The **Status** category at the top of the tool panel displays the following read-only information:

| Field | Description |
|-------|-------------|
| **Status** | Current status prompt (e.g., "Select a Fixture Actor") |
| **Source Fixture** | Name of the source fixture |
| **Preview Count** | Current number of preview fixtures |

---

## 7. Real-Time Preview

When any parameter is adjusted, the position and rotation of preview fixtures in the scene update immediately. Preview fixture characteristics:
- Marked with `RF_Transient` flag; not saved to the scene
- Do not send/receive DMX signals
- Parameter changes mark `bPreviewDirty`, triggering automatic refresh on the next frame

---

## 8. Action Buttons and Undo

### Action Buttons

| Button | Description |
|--------|-------------|
| **Create Fixtures** | Destroy preview Actors, destroy source fixture, then close the tool (creation is done in a Transaction, supports undo) |
| **Done** | Finish and exit the tool |

### Undo

Creation operations support **Ctrl+Z undo**.

---

## 9. Folder Grouping and Unique Naming

### Folder Grouping

After creating the array, all generated fixtures are automatically placed into a **folder** named after the source fixture type, managed centrally in the World Outliner to avoid scene hierarchy clutter.

### Unique Naming

The tool scans existing numbering of same-type Actors in the current world and starts naming from **max number + 1** (e.g., if the scene already has `SuperStageLight_003`, naming starts from `SuperStageLight_004`), ensuring repeated use of the array tool does not produce duplicate names.

---

## 10. Notes

- You must first select a valid SuperStage DMX fixture to use this tool
- If multiple fixtures are selected, the tool uses only the first selected fixture as the source
- Non-SuperStage DMX fixtures (e.g., regular point lights, spotlights) cannot serve as source fixtures
- Creating a large number of fixtures (e.g., 100+) may affect editor performance
- All fixtures in the array use the same fixture type as the source fixture
- It's recommended to use the Batch Patch Tool to reassign DMX addresses after array creation

---

## 11. FAQ

| Issue | Solution |
|-------|----------|
| No preview after opening the tool | Confirm you have selected a SuperStage fixture in the viewport |
| Fixture arrangement direction is wrong | Adjust the "Rotation" or "Plane Normal Axis" parameters |
| Spacing looks too large/too small | Note that spacing is in centimeters; 1 meter = 100 centimeters |
| DMX address conflicts after creation | Use the Batch Patch Tool to reassign addresses |
| Radial array first and last overlap | Check if End Angle is set to 360°; if so, the last fixture will not overlap with the first |
