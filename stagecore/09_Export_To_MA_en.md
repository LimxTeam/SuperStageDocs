# 09 - MA2 XML Export

> **Module**: SuperTools — console export  
> **Target Users**: Lighting programmers, stage designers  
> **Prerequisites**: [07 - Patch Tools](./07_Patch_Tools_en.md)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

The **DMXToMa** tool exports SuperDMX fixtures from the current level as grandMA2 XML files. The export result contains:

- One fixture layer XML per selected fixture model
- One `SuperStageToMA.xml` macro file that imports layers, patches fixtures, and creates Groups by model

This is a one-time export snapshot. If you change the UE scene, FixtureID, Universe, StartAddress, position, or rotation, export again.

---

## 2. Access

Open the SuperDMX menu in the SuperStage toolbar and click **DMXToMa**. This opens the **DMXToMa** tab.

---

## 3. Controls

| Control | Description |
|---------|-------------|
| **Export Directory** | Current export root directory, defaulting to the desktop; read-only |
| **Browse** | Select the export root directory |
| **Refresh** | Rescan SuperDMX fixtures in the current level |
| **Select All** | Select all models |
| **Select None** | Deselect all models |
| **Export** | Generate fixture layer and macro files |

The model list contains:

| Column | Description |
|--------|-------------|
| **Export** | Whether this model will be exported |
| **Model** | Fixture model/type name |
| **Count** | Number of fixtures of that class in the current level |
| **FixtureType No** | FixtureType number in the MA2 console; must be entered manually and be greater than 0 |

> **Note**: The current implementation does not actually persist FixtureType No configuration. Check these numbers when reopening the tool.

---

## 4. Export Steps

1. Finish fixture placement and Patch in the level.
2. Open **DMXToMa**.
3. Click **Browse** and select the export root directory.
4. Click **Refresh** and confirm the model list is current.
5. Select the models to export.
6. Enter **FixtureType No** for every selected model.
7. Click **Export**.

If any selected model has FixtureType No less than or equal to 0, the tool shows a warning and stops the export.

---

## 5. Output Folder and Files

Each export creates a timestamped batch folder under the root directory:

```
SuperDMX_YYYYMMDD_HHmmss
  ├── fixture_layers
  │   ├── LAYER_ModelName-1.xml
  │   └── ...
  └── macros
      └── SuperStageToMA.xml
```

Files are written as UTF-8 without BOM.

### fixture layer XML

Each selected fixture model generates one layer XML. For each fixture of that model, the file includes:

| Content | Source |
|---------|--------|
| FixtureID | Fixture ID |
| FixtureType No | FixtureType No entered in the list |
| Position | Actor world location converted from UE to MA coordinates |
| Rotation | Actor world rotation converted from UE to MA rotation |

The Patch Address inside the layer XML is written as 0. Actual DMX Patch is performed by the macro file.

### Macro File

`SuperStageToMA.xml` contains two macros:

| Macro | Purpose |
|-------|---------|
| `SuperStageToMA import layers` | Opens MA2 EditSetup/Layers and imports the generated layer files |
| `SuperStageToMA patch+groups` | Runs `Assign Fixture ... At Dmx ...` by FixtureID and creates Groups by model |

Fixtures are processed in ascending FixtureID order.

---

## 6. Coordinate Conversion

The exporter uses the following verified conversion:

| MA Field | Source |
|----------|--------|
| X | UE Y / 100 |
| Y | -UE X / 100 |
| Z | UE Z / 100 |
| Rot X | UE Roll |
| Rot Y | UE Pitch + 270 |
| Rot Z | UE Yaw + 180 |

Position is converted from UE centimeters to meters. The rotation conversion reflects the current tool implementation; still spot-check fixture orientation after importing into the console.

---

## 7. Import into MA2

1. Confirm that the MA2 console already has the corresponding FixtureTypes and note their numbers.
2. Put the XML files from `fixture_layers` and `macros` where MA2 can import them.
3. Import and execute `SuperStageToMA.xml`.
4. The macro imports layers, patches fixtures, and creates Groups by model.

The export tool does not create or import Fixture Profiles. FixtureType No must match the existing fixture type numbers in the console.

---

## 8. FAQ

### Q: Export reports an invalid FixtureType No.
Enter a value greater than 0 for every selected model. Deselected models are not validated.

### Q: No files were exported.
Confirm that the current level contains SuperStage DMX fixtures and that at least one model is selected.

### Q: Patch is wrong after import.
Check each fixture's FixtureID, Universe, and StartAddress in UE, and confirm the macro FixtureType No matches the type number in the MA2 console.

### Q: Position or rotation does not match expectations.
First confirm the Actor world position and rotation in UE are final, then spot-check against the conversion rules above. Some console environments may still need manual correction.

### Q: Can this export CSV?
The current toolbar only registers DMXToMa. The repository contains a CSV tool header declaration, but no implementation or entry point, so this manual does not describe CSV export as an available feature.

---

> **Return to Overview**: [00 - DMX System Overview](./00_DMX_System_Overview_en.md)
