# SuperStage DMX Export MA Macro — User Manual

## 1. Overview

The DMX Export MA Macro Tool (Super DMX To MA) is used to export fixture configurations from the SuperStage scene into macro script files and stage view XML files that can be directly imported by grandMA2 or grandMA3 lighting consoles. This allows you to complete fixture layouts in a virtual environment and sync them to the actual performance console with one click, eliminating the need to manually enter large amounts of fixture parameters.

---

## 2. Access

**Main Menu Path**: Toolbar **SuperStage** dropdown menu → **SuperDMXTool** → **DMXToMa**

---

## 3. Interface Description

```
┌──────────────────────────────────────────┐
│  Export Settings                         │
│                                          │
│  Target Console:  [MA2 ▼] / [MA3 ▼]      │
│  Export Path:     [C:\Export\...] [Browse]│
│                                          │
│  Stage View Options:                     │
│  ☑ Export Stage View XML                │
│  ☑ Include Fixture Position             │
│  ☑ Include Fixture Rotation             │
│                                          │
│  Fixture List (Preview)                  │
│  ┌──────────────────────────────────┐   │
│  │ ☑ Spot_1  | FID:1 | U1.001     │   │
│  │ ☑ Spot_2  | FID:2 | U1.025     │   │
│  │ ☐ Wash_1  | FID:3 | U1.049     │   │
│  │ ...                              │   │
│  └──────────────────────────────────┘   │
│                                          │
│  [Select All] [Select None]  [Export]    │
└──────────────────────────────────────────┘
```

---

## 4. Parameter Description

### 4.1 Target Console

| Option | Description |
|--------|-------------|
| **grandMA2** | Generate command-line macro scripts for grandMA2 (.xml format) |
| **grandMA3** | Generate Lua macro scripts for grandMA3 (.lua format) |

> **Note**: MA2 and MA3 macro formats are completely different; ensure you select the option matching your console version.

### 4.2 Export Path

Click the **"Browse..."** button to select the save location for exported files. The default path is the `Saved/SuperStage/Export/` folder under the project directory.

### 4.3 Stage View Options

| Option | Description | Default |
|--------|-------------|---------|
| **Export Stage View XML** | Whether to also export a stage view file | ☑ Enabled |
| **Include Fixture Position** | Whether to include XYZ coordinates in the stage view | ☑ Enabled |
| **Include Fixture Rotation** | Whether to include rotation angles in the stage view | ☑ Enabled |

---

## 5. Fixture List

The list displays all SuperStage DMX fixtures in the scene. Each row includes:

| Content | Description |
|---------|-------------|
| **Checkbox** | Whether to export this fixture (default: all checked) |
| **Fixture Name** | The fixture's label in the scene |
| **Fixture ID** | Fixture ID number |
| **DMX Address** | Format: `U{Universe}.{StartAddress}` |

You can uncheck fixtures you don't want to export.

### 5.1 Quick Actions

| Button | Function |
|--------|----------|
| **Select All** | Check all fixtures in the list |
| **Select None** | Uncheck all fixtures |

---

## 6. Export Content Description

### 6.1 MA2 Macro Script Export

The exported MA2 macro file contains the following commands:

| Command Type | Description |
|-------------|-------------|
| **Fixture Type Creation** | Create the corresponding fixture type in the console's fixture library |
| **Fixture Patch** | Assign Universe and Start Address for each fixture |
| **Fixture ID** | Set the Fixture ID |
| **Fixture Name** | Set the fixture's label name |

### 6.2 MA3 Lua Script Export

The exported MA3 Lua file performs the same operations using MA3's command-line interface; syntax is adapted for the MA3 environment.

### 6.3 Stage View XML

The stage view file contains fixture position and rotation information in 3D space, which can be imported into MA software's 3D Stage View for accurate 1:1 virtual stage reproduction.

---

## 7. Workflow

### Step 1: Complete Fixture Layout

Place all fixtures and configure DMX Patch in the SuperStage scene.

### Step 2: Open Export Tool

Open the DMX Export MA Macro Tool via the main menu.

### Step 3: Select Target Console

Select your console version (MA2 or MA3).

### Step 4: Set Export Path

Choose an accessible folder (e.g., USB drive or network share) for easy transfer to the console.

### Step 5: Select Fixtures

By default, all fixtures are exported. Uncheck fixtures you don't want to export to the console.

### Step 6: Click Export

Click the **"Export"** button; files will be generated in the specified path.

### Step 7: Import to Console

- **MA2**: Import the macro file via Backup → Import on the console, then execute the macro
- **MA3**: Load and execute the Lua script via the Plugin feature on the console

---

## 8. Exported File List

After export completes, the following files will be generated in the export path:

| File | Description |
|------|-------------|
| `SuperStage_Patch_MA2.xml` or `SuperStage_Patch_MA3.lua` | Macro script file |
| `SuperStage_StageView.xml` | Stage view XML file (if stage view export was checked) |

---

## 9. Notes

- Ensure all fixtures have been correctly assigned DMX addresses and Fixture IDs before exporting
- Fixtures without assigned Fixture IDs will use auto-generated numbers
- MA macro scripts will overwrite existing fixture configurations with the same names on the console; back up the console first
- Coordinates in the stage view are in meters, automatically converted from SuperStage's Unreal coordinates
- If the console's fixture library lacks a corresponding fixture type, the macro script will attempt to create a basic fixture type configuration

---

## 10. FAQ

| Issue | Solution |
|-------|----------|
| Console reports errors executing macro | Check if target console version matches (MA2/MA3) |
| Stage view has large position offsets | Check if fixture world coordinates in the SuperStage scene are reasonable |
| Exported fixture count is incorrect | Check if some fixtures were unchecked |
| Console fixture types don't match | Manually select the correct fixture type to replace in the console's fixture library |
