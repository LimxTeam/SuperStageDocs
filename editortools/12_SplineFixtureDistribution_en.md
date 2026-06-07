# SuperStage Spline Fixture Distribution Tool — User Manual

## 1. Overview

The Spline Fixture Distribution Tool (Light Array Tool) can distribute fixtures evenly along a Spline path. You only need to place a spline in the scene, then use this tool to specify the fixture type and quantity, and fixtures will automatically arrange along the path. This is very useful for curved trusses, circular stage edges, curved light arrays, and similar scenarios.

---

## 2. Access

**Access**: In the editor's left mode selection bar, switch to **SuperStage Edit Mode**, then select **Spline Distribution Tool** from the mode toolbar.

> **Prerequisite**: There must be an Actor with a Spline Component in the scene. You can use UE's built-in Spline Actor or any custom Actor containing a spline component.

---

## 3. Workflow

### Step 1: Create a Spline in the Scene

Place a Spline Actor in the 3D viewport and edit its control points to define the path shape for fixture arrangement.

### Step 2: Select the Spline

Select the Actor containing the spline in the viewport (or directly select the Spline Component).

### Step 3: Open the Spline Distribution Tool

Open the tool panel via the main menu. The tool automatically detects the selected spline.

### Step 4: Configure Fixture Sequence

Set the fixture type, quantity, spacing, and other parameters for distribution.

### Step 5: Preview and Adjust

Fixture previews appear in the scene in real time; adjust parameters until satisfied.

### Step 6: Create Fixtures

Click the **"Create"** button to convert preview fixtures into official fixtures.

---

## 4. Parameter Details

### 4.1 Fixture Sequence Settings (Device Category)

The Fixture Sequence defines the combination of fixture types distributed along the spline, as a `TArray<FLightArrayFixtureEntry>` array. Each entry contains:

| Parameter | Type | Description | Range | Default |
|-----------|------|-------------|-------|---------|
| **Fixture Class** | TSubclassOf\<ASuperDmxActorBase\> | Fixture Blueprint class (class picker, only shows ASuperDmxActorBase subclasses) | — | None |
| **Repeat** | int32 | Number of consecutive repeats of this fixture type | 1 - 100 (UI limit 10) | 1 |
| **Rotation** | FRotator | Additional rotation offset for this fixture type relative to the spline direction | Unlimited | (0, 0, 0) |

#### Sequence Example

Suppose the sequence is defined as:
```
[Spot380 × 2] → [Wash600 × 1] → [Spot380 × 2] → [Wash600 × 1] → ...
```

Fixtures will cycle in the order `Spot, Spot, Wash, Spot, Spot, Wash, ...` until the specified count is filled.

#### Adding Sequence Entries

Click the **"+"** button in the Fixture Sequence array in the property panel to add new entries.

### 4.2 Distribution Parameters (Array Category)

| Parameter | Type | Description | Range | Default |
|-----------|------|-------------|-------|---------|
| **Count** | int32 | Total number of fixtures along the spline (set to 0 to auto-calculate using Spacing) | 0 - 500 (UI limit 100) | 10 |
| **Spacing (cm)** | float | Distance between fixtures (only effective when Count = 0) | ≥ 10.0 | 100.0 |
| **Start Offset %** | float | Percentage offset from the spline start | 0% - 100% | 0% |
| **End Offset %** | float | Percentage where fixture distribution ends | 0% - 100% | 100% |

#### Count vs. Spacing Relationship

- **Count > 0**: Fixtures are evenly distributed within the StartOffset ~ EndOffset range; spacing is auto-calculated
- **Count = 0**: Uses the Spacing value; the number of fixtures is auto-determined by path length and spacing

#### Offset Percentage Description

- **Start Offset 10%**: The first fixture starts at 10% of the total spline length
- **End Offset 90%**: The last fixture ends at 90% of the path
- This avoids placing fixtures at the spline endpoints

### 4.3 Transform Settings (Transform Category)

| Parameter | Type | Description | Default |
|-----------|------|-------------|---------|
| **Rotation Offset** | FRotator | Global rotation offset for all fixtures (Pitch/Yaw/Roll) | (0, 0, 0) |
| **Position Offset** | FVector | Position offset for fixtures relative to the spline tangent-space coordinate system (cm) | (0, 0, 0) |
| **Follow Spline Rotation** | bool | Whether fixtures automatically rotate to follow the spline tangent direction | On |

### 4.4 Outliner Folder (Organization Category)

| Parameter | Type | Description | Default |
|-----------|------|-------------|---------|
| **Create Folder** | bool | Whether to place all generated Actors into a World Outliner folder after creation | On |
| **Folder Name** | FString | Folder name (auto-generated from fixture type if left empty); detects naming conflicts | Empty (auto-generated) |

### 4.5 Follow Spline Rotation

| State | Description |
|-------|-------------|
| **On** | Fixtures automatically rotate along the spline, always facing forward in the travel direction |
| **Off** | All fixtures maintain a unified world-space orientation |

#### Position Offset Coordinate System

Offset values are calculated in the spline's **local tangent space**.

---

## 5. Status Information

The **Spline** category at the top of the tool panel displays the following read-only information:

| Field | Description |
|-------|-------------|
| **Status** | Current status prompt (e.g., "Select an Actor with SplineComponent") |
| **Target Spline** | Name of the currently selected spline Actor |
| **Preview Count** | Current number of preview fixtures |

---

## 6. Real-Time Preview

When any parameter is modified, preview fixtures in the scene update in real time. Preview fixture characteristics:
- Marked with `RF_Transient` flag; not saved to the scene
- Do not send/receive DMX signals
- When the spline shape is modified, preview fixtures automatically follow the update

### Lightweight Updates

The tool internally distinguishes parameter change types intelligently:
- **Only Offset/Rotation edited**: Only updates fixture positions and rotations (`bTransformDirty`, faster)
- **Count/Sequence/Spacing edited**: Fully recalculates fixture distribution (`bPreviewDirty`, rebuilds preview Actors)

---

## 7. Action Buttons and Undo

### Action Buttons

| Button | Description |
|--------|-------------|
| **Create Fixtures** | Destroy preview Actors, create official fixtures (done in a Transaction, supports undo), then close the tool |
| **Done** | Finish and exit the tool |

### Post-Creation

1. The spline Actor remains in the scene (it is not deleted)
2. Creation operations support **Ctrl+Z undo**

---

## 8. Usage Examples

### Example 1: Curved Truss Light Row

1. Create a curved spline in the scene to simulate the truss's bent shape
2. Open the Spline Distribution Tool
3. Select fixture type `Spot380`
4. Set Count to 12
5. Enable "Follow Spline Rotation"
6. Adjust offset so fixtures are positioned below the truss

### Example 2: Circular Stage Edge Lights

1. Create a circular spline
2. Select fixture type `WashLED`
3. Set Count to 24
4. Start Offset 0%, End Offset 0% (full circle)
5. Enable "Follow Spline Rotation"

### Example 3: Alternating Fixture Sequence

1. Create a straight spline
2. Add two fixture types in the sequence:
   - Spot380 × 1
   - Wash600 × 1
3. Set Count to 20
4. Fixtures will alternate: `Spot, Wash, Spot, Wash, ...`

---

## 9. Notes

- You must first select an Actor containing a Spline Component
- If the spline has too few control points or the path is too short, it may not fit all fixtures
- After modifying the spline shape, the preview updates automatically
- After creation, fixtures are no longer linked to the spline (they are independent Actors)
- To modify the arrangement, delete the created fixtures and reuse the tool
- At least one fixture type is required in the sequence
- Creating a large number of fixtures (100+) may take a few seconds to process

---

## 10. FAQ

| Issue | Solution |
|-------|----------|
| No preview after opening the tool | Confirm you have selected an Actor containing a spline |
| Wrong fixture direction | Check if "Follow Spline Rotation" is enabled, or adjust global rotation offset |
| Fixtures are not evenly distributed | Check Start/End Offset percentage settings |
| Fixtures floating in the air | Adjust the Z offset parameter, or edit the spline height |
| Fixture types not selectable | Ensure SuperStage fixture Blueprint assets have been imported into the project |
| Fixtures not updating after spline modification | Created fixtures do not follow spline updates; only the preview phase follows in real time |
