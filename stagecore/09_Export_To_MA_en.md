# 09 - MA Console Export & CSV Export

> **Module**: SuperStageEditor (SSuperDMXToMa / SSuperDMXToCsv)  
> **Target Users**: Lighting programmers, stage designers  
> **Prerequisites**: [07 - Patch Tools](/docs/stage-core/patch-tools)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

SuperStage provides two export tools for exporting fixture configuration information from the virtual scene to external systems:

| Tool | Output Format | Purpose |
|------|---------|------|
| **Export to MA** | XML macro file | Import into grandMA2/MA3 console, auto-complete fixture Patch and grouping |
| **Export to CSV** | CSV spreadsheet file | Export fixture information table for documentation or importing into other software |

### Core Value

After building your virtual scene and completing DMX Patch in UE, you can export the entire Patch table to the MA console. This ensures the **console and virtual scene fixture configurations are fully synchronized**, eliminating the need to manually repeat operations on both sides.

---

# Part 1: Export to MA (MA Macro Export)

## 2. Opening the MA Export Tool

Click the **Export to MA** button in the SuperStage toolbar to open the export panel.

---

## 3. Interface Layout

The MA export tool panel is divided into the following areas from top to bottom:

```
┌─────────────────────────────────────────────┐
│ Export Path                                  │
│  Path: [C:\Export\MyShow\]  [Browse...]       │
├─────────────────────────────────────────────┤
│ Fixture Model List                           │
│  ☑ │ Model Name         │ Count │ FixtureType│
│  ──┼────────────────── ┼───────┼────────────│
│  ☑ │ Robe_MegaPointe    │ 20    │ [1]        │
│  ☑ │ Chauvet_Maverick   │ 12    │ [2]        │
│  ☐ │ Generic_Dimmer     │ 48    │ [3]        │
├─────────────────────────────────────────────┤
│ [Export]                                      │
└─────────────────────────────────────────────┘
```

---

## 4. Usage Steps

### Step 1: Set Export Path

| Operation | Description |
|------|------|
| **Path** | Displays the current export directory path |
| **Browse...** | Click to open folder selection dialog, choose export target folder |

> **Suggestion**: Create a dedicated folder for export files, e.g., `C:\MyShow\MA_Export\`

### Step 2: Select Fixture Models to Export

The tool automatically scans all DMX fixtures in the scene and groups them by **fixture model**:

| Column | Description |
|----|------|
| **Checkbox** | Check to select fixture models for export (all checked by default) |
| **Model Name** | Fixture class name or display name |
| **Count** | Number of fixtures of that model in the scene |
| **Fixture Type** | FixtureType number in the MA console (can be manually modified) |

### Step 3: Set Fixture Type Numbers

**Fixture Type** is the fixture type number in the MA console. Each different fixture model should use a different number.

- Default starts incrementing from 1
- You can manually modify to match existing Fixture Type numbers in the console
- Numbers cannot be duplicated

### Step 4: Export

Click the **Export** button to generate the following files in the specified directory:

---

## 5. Export File Description

### 5.1 Generated Files

| File | Format | Description |
|------|------|------|
| **Fixture Layer XML** | `.xml` | Fixture layer XML file containing position, rotation, and group information for all fixtures |
| **Macro XML** | `.xml` | MA macro script file containing Patch and Group commands |

### 5.2 Fixture Layer XML Content

The fixture layer file contains the following information for each fixture:

| Field | Description | Source |
|------|------|------|
| **Fixture ID** | Unique fixture number | Actor's FixtureID |
| **Universe** | DMX Universe number | Actor's Universe |
| **Address** | Start address | Actor's StartAddress |
| **Name** | Fixture name | Actor's label name |
| **Position X/Y/Z** | 3D position (meters) | Actor's world position (after UE→MA coordinate conversion) |
| **Rotation X/Y/Z** | Rotation angle (degrees) | Actor's world rotation (after UE→MA coordinate conversion) |

### 5.3 Macro XML Content

The macro file contains MA command sequences. After execution, it will:

1. **Patch fixtures** — Assign FixtureType, Universe, and addresses for each fixture in the console
2. **Create groups** — Auto-create Groups by fixture model

### 5.4 Coordinate Conversion

UE and MA consoles use different coordinate systems. Export auto-converts:

| Axis | UE Coordinate System | MA Coordinate System | Conversion |
|----|----------|----------|------|
| **X** | Forward | Right | Y_UE → X_MA |
| **Y** | Right | Forward (stage depth) | X_UE → Y_MA |
| **Z** | Up | Up | Z_UE → Z_MA |
| **Unit** | cm | m | ÷ 100 |

Rotation angles also undergo corresponding axis mapping conversion.

---

## 6. Importing into MA Console

### 6.1 Import Fixture Layer

1. Copy the exported XML file to the MA console's USB or network share directory
2. In MA, open **Setup > Patch & Fixture Schedule**
3. Use the **Import** function to import the Fixture Layer XML

### 6.2 Execute Macro

1. Copy the Macro XML file to MA's macro directory
2. Execute the imported macro in the MA command line
3. The macro automatically completes Patch and grouping operations

> **Note**: Before executing the macro, ensure the console has already imported the corresponding Fixture Profile (fixture library file). The macro only performs Patch assignment and does not create Fixture Profiles.

---

# Part 2: Export to CSV

## 7. Opening the CSV Export Tool

Click the **Export to CSV** button in the SuperStage toolbar to open the export panel.

---

## 8. Interface Layout

The CSV export tool interface is similar to MA export:

```
┌─────────────────────────────────────────────┐
│ Export Path                                  │
│  Path: [C:\Export\]  [Browse...]              │
├─────────────────────────────────────────────┤
│ Fixture List                                 │
│  # │ ID │ Name    │ Uni │ Addr │ Pos...     │
│  ──┼────┼─────────┼─────┼──────┼────────── │
│  1  │101 │Spot_01  │ 1   │ 1    │ ...       │
│  2  │102 │Spot_02  │ 1   │ 31   │ ...       │
├─────────────────────────────────────────────┤
│ [Export]                                      │
└─────────────────────────────────────────────┘
```

---

## 9. CSV Export Content

The exported CSV file contains the following columns:

| Column Name | Description | Example |
|------|------|------|
| **Selection Index** | Sequential number | 1 |
| **Fixture ID** | Fixture number | 101 |
| **Name** | Fixture name | Spot_01 |
| **Display Name** | Display name | MegaPointe #1 |
| **Universe** | DMX Universe | 1 |
| **Address** | Start address | 1 |
| **Pos X (m)** | X position (meters, MA coordinate system) | 2.50 |
| **Pos Y (m)** | Y position (meters, MA coordinate system) | -1.20 |
| **Pos Z (m)** | Z position (meters, MA coordinate system) | 8.00 |
| **Rot X** | X rotation (degrees, MA coordinate system) | 0.0 |
| **Rot Y** | Y rotation (degrees, MA coordinate system) | 45.0 |
| **Rot Z** | Z rotation (degrees, MA coordinate system) | -90.0 |
| **Pos X (UE)** | X position (cm, UE coordinate system) | 250.0 |
| **Pos Y (UE)** | Y position (cm, UE coordinate system) | -120.0 |
| **Pos Z (UE)** | Z position (cm, UE coordinate system) | 800.0 |

### 9.1 Coordinate Conversion

The CSV file includes **both MA coordinate system** and **UE coordinate system** coordinates for use in different tools:

- **MA Coordinate System (Pos X/Y/Z (m))**: Units in meters, axes converted to MA standard
- **UE Coordinate System (Pos X/Y/Z (UE))**: Units in centimeters, original UE coordinates

---

## 10. CSV Use Cases

| Use | Description |
|------|------|
| **Lighting Documentation** | Import into Excel to create lighting equipment lists, fixture position charts |
| **Other Visualization Software** | Import into Vectorworks, Capture, and other lighting design software |
| **Data Archiving** | Save project fixture configuration records |
| **Custom Processing** | Use Python/Excel scripts for data analysis or format conversion |

---

## 11. Selection Linkage

In the fixture list of both export tools, **clicking a row** selects and highlights the corresponding fixture Actor in the scene viewport, making it easy to confirm the fixture's actual position.

---

## 12. Best Practices

### MA Export Workflow

1. Build the scene in UE, place all fixtures
2. Use the **Patch Tool** to batch assign DMX addresses
3. Confirm all fixture Fixture IDs are correct
4. Use **Export to MA** to export macro files
5. Import Fixture Profiles in the MA console (if not already imported)
6. Execute the exported macro script to complete Patch
7. Verify on the console → Console signals should drive virtual fixtures in UE

### Pre-Export Checklist

- [ ] All fixture **Universes** and **Start Addresses** are correctly assigned
- [ ] All fixture **Fixture IDs** are unique and conflict-free
- [ ] Fixture **positions and rotations** in the scene are in their final state (changes after export require re-export)
- [ ] Export path has **write permission**
- [ ] **Fixture Type** numbers correspond to fixture types in the MA console

---

## 13. FAQ

### Q: After executing MA macro, fixtures don't appear in the console?
1. Confirm the corresponding Fixture Profile is imported in the console
2. Check if Fixture Type numbers in the macro match Profile numbers
3. Manually check the Patch table in the console

### Q: Exported positions don't match reality?
1. Confirm fixture positions in UE are final (not temporary placements)
2. Note the differences between UE and MA coordinate systems (both axes and units differ)
3. Confirm the scene's world origin position is reasonable before exporting

### Q: CSV file opens with garbled characters in Excel?
CSV files use UTF-8 encoding. When opening in Excel, use the "Import Data" function and select UTF-8 encoding, or first confirm the encoding format with a text editor.

### Q: How to export only some fixtures?
- **MA Export**: In the fixture model list, uncheck models you don't want to export
- **CSV Export**: After exporting, filter/delete unwanted rows in Excel

### Q: Fixture rotation angles seem wrong?
UE and MA use different rotation axis conventions. The export tool already performs coordinate conversion, but due to slight differences in coordinate conventions across console versions, manual fine-tuning and verification on the console is recommended.

### Q: Do I need to re-export after each scene modification?
Yes. Export is a one-time snapshot operation and doesn't auto-sync. After modifying fixture positions, addresses, or adding/removing fixtures, you need to re-export.

---

> **Return to Overview**: [00 - DMX System Overview](/docs/stage-core/overview)
