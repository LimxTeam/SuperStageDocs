# SuperStage Batch Patch Tool — User Manual

## 1. Overview

The Batch Patch Tool (Super Patch Tool) is used to batch assign DMX addresses to multiple selected fixtures in the scene. It can automatically calculate each fixture's Universe and starting channel address, supports preview and one-click application, and includes undo functionality.

---

## 2. Access

**Main Menu Path**: Toolbar **SuperStage** dropdown menu → **SuperDMXTool** → **PatchTool**

---

## 3. Interface Description

```
┌──────────────────────────────────────┐
│  Starting Parameter Settings          │
│  ┌────────────┬──────────┬──────┐   │
│  │ Start Univ  │ Start Addr│Start FID│   │
│  └────────────┴──────────┴──────┘   │
│                                      │
│  Fixture List (Preview)              │
│  ┌──────────────────────────────┐   │
│  │ Fixture Name | Univ | Addr   │   │
│  │ Light_1      | 1    | 1      │   │
│  │ Light_2      | 1    | 25     │   │
│  │ Light_3      | 1    | 49     │   │
│  │ ...                          │   │
│  └──────────────────────────────┘   │
│                                      │
│  [Refresh Selection] [Preview] [Apply]│
└──────────────────────────────────────┘
```

---

## 4. Parameter Description

### 4.1 Starting Parameters

| Parameter | Description | Range | Default |
|-----------|-------------|-------|---------|
| **Start Universe** | Universe number assigned to the first fixture | 1 - 256 | 1 |
| **Start Address** | Starting channel address for the first fixture in that Universe | 1 - 512 | 1 |
| **Start Fixture ID** | Fixture ID number for the first fixture; subsequent fixtures auto-increment | 1 - 9999 | 1 |

### 4.2 Address Assignment Rules

The tool automatically calculates subsequent fixture addresses using the following rules:

1. Start from the specified Start Universe and Start Address
2. The number of channels each fixture occupies is determined by its fixture library's Channel Span
3. After a fixture is assigned, the next fixture's address = current address + channel span
4. If the address exceeds 512 (the maximum channels in a Universe), it automatically wraps to channel 1 of the next Universe
5. FixtureID auto-increments from **Start Fixture ID**, skipping IDs already occupied by other fixtures in the scene (ID=1 is an exception as a default value)

---

## 5. Fixture List

### 5.1 Column Description

| Column | Description |
|--------|-------------|
| **Fixture Name** | The label name of the fixture in the scene |
| **Model** | The Blueprint class name of the fixture |
| **Universe** | The assigned Universe number |
| **Start Address** | The assigned starting channel address |
| **Channels** | The number of channels this fixture occupies |
| **FixtureID** | The assigned fixture ID number |

### 5.2 Sorting Rules

The fixture list is displayed in natural sort order. For example:
- Light_1, Light_2, Light_3, ..., Light_10, Light_11
- Not: Light_1, Light_10, Light_11, ..., Light_2, Light_3

---

## 6. Operation Buttons

| Button | Function | Description |
|--------|----------|-------------|
| **Refresh Selection** | Re-fetch the fixtures selected in the scene | Click this button to refresh the list after changing selections |
| **Preview** | Calculate and display address assignment preview | Does not actually modify fixtures; only shows the projected assignment results |
| **Apply** | Write the previewed address assignments to fixtures | Actually modifies the DMX address configuration of all listed fixtures |

---

## 7. Workflow

### Step 1: Select Fixtures

In the 3D viewport, box-select or individually select the fixtures to be batch patched.

### Step 2: Open the Batch Patch Tool

Open the tool panel via the main menu.

### Step 3: Set Starting Parameters

- Set **Start Universe** (e.g., Universe 1)
- Set **Start Address** (e.g., starting from channel 1)

### Step 4: Refresh and Preview

- Click **"Refresh Selection"** to ensure the list shows the fixtures you want to patch
- Click **"Preview"** to see the address assignment results

### Step 5: Confirm and Apply

- Check whether each fixture's Universe and address in the preview list match expectations
- Click **"Apply"** to write the addresses to the fixtures

---

## 8. Undo

Apply operations support **Ctrl+Z undo**. If you find errors in the assignment, you can undo to restore the previous state.

---

## 9. Notes

- Only SuperStage DMX fixtures (subclasses of SuperDmxActorBase) appear in the list
- Regular UE lights or other types of Actors are not recognized
- If no fixtures are selected in the scene, the list will be empty
- Fixtures with a channel span of 0 may cause address calculation anomalies; ensure the fixture library is correctly configured
- With a large number of fixtures, addresses may span multiple Universes; ensure your DMX network supports the required number of Universes

---

## 10. FAQ

| Issue | Solution |
|-------|----------|
| Fixture list is empty | Select fixtures in the scene first, then click "Refresh Selection" |
| Preview shows incorrect addresses | Check if Start Universe and Start Address settings are correct |
| Fixtures don't respond to console after applying | Check if input settings in DMX Config Panel match |
| Some fixture addresses overlap | Check if channel span configuration in the fixture library is correct |
