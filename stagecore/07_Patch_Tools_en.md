# 07 - Patch Tools

> **Module**: SuperStageEditor (SSuperPatchTool / SSuperDMXPatchPreview)  
> **Target Users**: Lighting programmers, stage designers  
> **Prerequisites**: [03 - DMX Fixture Base](/docs/stage-core/dmx-actor-base)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

SuperStage provides two Patch-related editor tools:

| Tool | Function | Use Case |
|------|------|---------|
| **Patch Tool** | Batch auto-assign DMX addresses | When there are many fixtures in the scene needing quick patching |
| **Patch Preview** | View and edit Patch info for all fixtures | Check/fine-tune existing Patch, troubleshoot address conflicts |

---

# Part 1: Patch Tool

## 2. Opening the Patch Tool

Click the **Patch Tool** button in the SuperStage toolbar to open the Patch Tool panel.

---

## 3. Interface Layout

The Patch Tool panel is divided into the following areas from top to bottom:

```
┌─────────────────────────────────────────┐
│ Start Parameter Settings                  │
│  Universe: [1]    Start Address: [1]     │
├─────────────────────────────────────────┤
│ Preview List                             │
│  Name   │ FixtureID │ Universe │ Addr    │
│  ────── ┼───────────┼──────────┼─────   │
│  Spot_01 │ 101       │ 1        │ 1      │
│  Spot_02 │ 102       │ 1        │ 31     │
│  Spot_03 │ 103       │ 1        │ 61     │
│  Wash_01 │ 201       │ 1        │ 91     │
│  ...     │ ...       │ ...      │ ...    │
├─────────────────────────────────────────┤
│ [Apply Patch]     [Clear Selected]       │
└─────────────────────────────────────────┘
```

---

## 4. Usage Steps

### Step 1: Select Fixtures to Patch

In the scene viewport, **select** the fixtures you need to assign addresses to. You can use the following methods:

- **Single Select**: Click a single fixture
- **Multi Select**: Hold Ctrl and click multiple fixtures
- **Box Select**: Drag to box-select fixtures in an area
- **Select All**: Press Ctrl+A to select all fixtures in the scene

> **Tip**: Only Actors inheriting from SuperDmxActorBase appear in the Patch Tool.

### Step 2: Set Start Parameters

| Parameter | Description | Default |
|------|------|--------|
| **Universe** | Starting Universe number | 1 |
| **Start Address** | Starting address of the first fixture | 1 |

### Step 3: Preview Assignment Results

After setting the start parameters, the Patch Tool **automatically calculates** the address assignment scheme for all selected fixtures and displays it in the preview list:

| Column | Description |
|----|------|
| **Name** | The Actor's label name in the scene |
| **Fixture ID** | Auto-assigned fixture ID (avoiding conflicts with unselected fixtures) |
| **Universe** | Assigned Universe number |
| **Address** | Assigned start address |

### Step 4: Confirm and Apply

- Check if the assignment scheme in the preview list is reasonable
- Click the **Apply Patch** button to write the scheme to all selected fixtures

### (Optional) Clear Patch

Click the **Clear Selected** button to reset the DMX settings of selected fixtures (Universe=1, Address=1, FixtureID=0).

---

## 5. Auto-Assignment Rules

### 5.1 Sequential Address Assignment

The Patch Tool auto-assigns addresses according to the following rules:

1. The first fixture uses the Start Address you set
2. Next fixture's address = Previous fixture's start address + Previous fixture's Channel Span
3. If the current Universe has insufficient remaining space (address exceeds 512), automatically jumps to address 1 of the next Universe

**Example**:

| Fixture | Channel Span | Assigned Universe | Assigned Address |
|------|---------|----------------|-----------|
| Spot_01 | 30 | 1 | 1 |
| Spot_02 | 30 | 1 | 31 |
| Spot_03 | 30 | 1 | 61 |
| ... | ... | ... | ... |
| Spot_17 | 30 | 1 | 481 |
| Spot_18 | 30 | **2** (overflow to next Universe) | **1** |

### 5.2 Sorting Rules

The Patch Tool sorts selected fixtures in the following order before assigning addresses:

1. **Group by parent Actor** — Fixtures mounted under the same parent Actor are grouped together
2. **Sort by numeric suffix in label name** — For example, Spot_01, Spot_02, Spot_03 will be sorted as 1, 2, 3

This ensures fixtures in the same group receive consecutive addresses, following lighting programming conventions.

### 5.3 Auto Fixture ID Assignment

The Patch Tool auto-assigns unique Fixture IDs to each fixture:

- Scans **unselected** fixtures in the scene, collecting already-used Fixture IDs
- Assigns from 1 for selected fixtures, skipping already-used IDs
- Ensures no Fixture ID conflicts in the entire scene

---

## 6. Universe Overflow Handling

When there are many fixtures and total channel requirements exceed 512, the Patch Tool automatically handles Universe overflow:

- When current fixture's address + channel span > 512, that fixture is assigned to the next Universe
- New Universe starts at address 1
- The preview list clearly labels each fixture's Universe number

> **Tip**: If you want to manually control Universe boundaries, select fixtures in batches and use the Patch Tool multiple times.

---

# Part 2: Patch Preview

## 7. Opening Patch Preview

Click the **Patch Preview** button in the SuperStage toolbar to open the Patch Preview panel.

---

## 8. Interface Layout

The Patch Preview panel displays Patch information for **all DMX fixtures** in the scene in list format:

```
┌──────────────────────────────────────────────────┐
│ Name          │ FixtureID │ Universe │ Address   │
│ ──────────────┼───────────┼──────────┼────────── │
│ Spot_01       │ [101]     │ [1]      │ [1]       │
│ Spot_02       │ [102]     │ [1]      │ [31]      │
│ Wash_01       │ [201]     │ [2]      │ [1]       │
│ DMXCamera_01  │ [301]     │ [3]      │ [1]       │
└──────────────────────────────────────────────────┘
```

---

## 9. Feature Details

### 9.1 Viewing Patch Information

The list displays the following columns (all sortable):

| Column | Description | Editable |
|----|------|--------|
| **Name** | Actor's display name | Yes |
| **Fixture ID** | Fixture number | Yes |
| **Universe** | DMX Universe number | Yes |
| **Start Address** | Start address | Yes |

### 9.2 In-Place Editing

In the Patch Preview list, you can **directly click** any editable field to modify:

- Click the **Fixture ID** column value → Enter new ID → Press Enter to confirm
- Click the **Universe** column value → Enter new Universe → Press Enter to confirm
- Click the **Start Address** column value → Enter new address → Press Enter to confirm
- Modified values are **immediately synced** to the fixture Actor in the scene

> **Note**: In-place editing does not check for address conflicts. If you need automatic conflict avoidance, use the Patch Tool.

### 9.3 Selection Linkage

Clicking a row in the Patch Preview list causes the corresponding fixture to be **selected and highlighted in the scene viewport**, making it easy to quickly locate fixtures.

### 9.4 Sorting Rules

Fixtures in the list are sorted by the following rules:

1. **Group by class name** — Fixtures of the same type are grouped together
2. **Sort by numeric suffix in label name** — For example, Spot_01 comes before Spot_02

---

## 10. Patch Tool vs Patch Preview Comparison

| Feature | Patch Tool | Patch Preview |
|------|-----------|-----------|
| **Purpose** | Batch auto-assignment | View and fine-tune |
| **Display Scope** | Only selected fixtures | All fixtures in scene |
| **Address Assignment** | Auto-calculated | Manual editing |
| **Conflict Check** | Auto-avoids conflicts | No conflict check |
| **Fixture ID** | Auto-assigned | Manual editing |
| **Universe Overflow** | Auto-handled | Not handled |
| **Typical Scenario** | Initial Patch for new scenes | Fine-tuning individual fixture addresses |

---

## 11. Best Practices

### Quick Patch Workflow for New Scenes

1. Place all fixtures in the scene
2. Select fixtures in batches by type (e.g., select all Spots first, then all Washes)
3. Use the **Patch Tool** to assign addresses to each batch (set different starting Universe/Address)
4. Use **Patch Preview** to check the final address assignments for all fixtures
5. Export MA macro file to the console (see [09 - MA Console Export](/docs/stage-core/export-ma))

### Address Planning Suggestions

| Fixture Type | Suggested Universe | Description |
|---------|--------------|------|
| Moving Lights (Spot) | 1-5 | Many channels, dedicated Universes |
| Wash Lights | 6-10 | Many channels, dedicated Universes |
| LED Panels | 11-20 | Matrix lights have very many channels |
| Smoke/FX Machines | 21 | Few channels, can share Universe |
| DMX Cameras | 22 | Independent Universe for easy management |

---

## 12. FAQ

### Q: Patch Tool preview list is empty?
Confirm that you have selected SuperDmxActorBase type fixtures in the scene. Regular StaticMeshActors do not appear in the Patch Tool.

### Q: Fixtures don't respond after applying Patch?
1. Check if DMX network is configured and enabled (see [01 - Network Configuration](/docs/stage-core/dmx-network))
2. Use the Activity Monitor to confirm signals are arriving
3. Confirm console fixture addresses match the addresses assigned by the Patch Tool

### Q: How to undo a Patch?
Use UE's standard undo function **Ctrl+Z** to undo Patch operations.

### Q: Do I need to re-Patch after modifying the Fixture Library?
If the modification affects channel span (e.g., attributes added/removed), re-Patch is recommended to ensure addresses don't overlap.

---

> **Next Steps**: Read [08 - DMX Recording & Playback](/docs/stage-core/dmx-recording) to learn how to record and playback DMX data.
