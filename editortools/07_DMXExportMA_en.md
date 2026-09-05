# DMXToMa User Manual

## 1. Overview

**DMXToMa** exports SuperStage DMX fixtures from the current level into grandMA2-compatible XML layer files and a macro file, grouped by model. The exported files must still be imported, executed, and checked in a grandMA2 environment.

The current implementation does not provide a MA3 / Lua export option, and it does not create Fixture Profiles. For each selected model, you must manually enter the existing **FixtureType No** from the MA2 console.

---

## 2. Access

Toolbar **SuperStage** dropdown → **SuperDMXTool** → **DMXToMa**

---

## 3. Interface

The current panel contains:

| Control | Description |
| --- | --- |
| **Export Directory** | Export root folder. The desktop is preferred as the default location when available. |
| **Browse** | Choose the export folder. |
| **Refresh** | Rescan SuperStage DMX fixtures in the current level. |
| **Select All** | Select all model groups. |
| **Select None** | Clear all model groups. |
| **Export** | Generate export files. |
| **Model list** | Shows Model, Count, and FixtureType No grouped by fixture model/type. |

**FixtureType No** must be greater than 0 for every selected model. The current load/save functions for these numbers are placeholders, so check the numbers again after reopening the tool.

---

## 4. Exported Files

When you click **Export**, the tool creates a timestamped folder under the export directory:

```text
SuperDMX_YYYYMMDD_HHmmss/
  fixture_layers/
    LAYER_ModelName-1.xml
    ...
  macros/
    SuperStageToMA.xml
```

Exported data comes from selected SuperStage DMX fixture model groups in the current level:

| Data | Source |
| --- | --- |
| Universe | Fixture Universe |
| Start Address | Fixture Start Address |
| Fixture ID | Fixture ID |
| Position / Rotation | Actor world location and world rotation |
| Model name | Fixture model/type name |

Layer XML writes Patch Address as 0. Actual patching is done by `Assign Fixture ... At Dmx ...` commands in the macro file.

---

## 5. Coordinate Conversion

The source writes MA layer XML using this conversion:

| MA Position / Rotation | SuperStage / UE Source |
| --- | --- |
| X | UE Y / 100 |
| Y | -UE X / 100 |
| Z | UE Z / 100 |
| Rot X | UE Roll |
| Rot Y | UE Pitch + 270 |
| Rot Z | UE Yaw + 180 |

After import, check stage direction, origin, and fixture orientation in the console.

---

## 6. Workflow

1. Confirm fixture Universe, Start Address, and Fixture ID in the scene.
2. Open **DMXToMa**.
3. Click **Refresh**.
4. Select the model groups to export.
5. Enter the matching MA2 **FixtureType No** for each selected model.
6. Choose an export directory.
7. Click **Export**.
8. In grandMA2, import `fixture_layers` and `SuperStageToMA.xml`, then run the macros and check patch, groups, and positions.

---

## 7. When Export Stops

**"Please choose an export directory"**
No destination is set yet.

**"Model requires a valid FixtureType No"**
One of the ticked types still has no console number. The message names which one.

**"No actors found for export"**
There are no fixtures in the current level, or none of the ticked types are actually present. Press **Refresh** and check the counts in the list.

---

## 8. Notes

- Selection is by model group, not by individual Actor row.
- FixtureType No must match an existing fixture type in the console.
- The tool does not generate MA3 Lua files.
- The tool does not create or import Fixture Profiles.
- Exported files are handoff aids, not a guarantee of the final show-console patch.
