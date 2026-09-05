# 07 - Patch Tools

> **Module**: SuperTools  
> **Target Users**: Lighting programmers, stage designers  
> **Prerequisites**: [03 - DMX Fixture Base](./03_DMX_Actor_Base_en.md)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

Patch Tools assign and inspect DMX addresses for SuperDMX fixtures in the current level. The current panel has two main areas:

| Area | Purpose |
|------|---------|
| Top parameter bar | Batch-write Universe, Start Address, and FixtureID to the currently selected fixtures |
| Lower Patch Preview | View all SuperDMX fixtures in the level, then adjust Patch values through the table or channel grid |

Only SuperStage DMX fixtures are handled by these tools. Regular Actors do not appear in the Patch list.

---

## 2. Access

Open the SuperDMX menu in the SuperStage toolbar and click **PatchTool**. This opens the **Super Patch Tool** tab.

---

## 3. Top Parameter Bar

| Control | Description |
|---------|-------------|
| **Start Universe** | Starting Universe for batch Patch, range 1-512 |
| **Start Address** | Start address for the first selected fixture, range 1-512 |
| **Start Fixture ID** | Starting FixtureID for the first selected fixture, range 1-9999 |
| **SelectAllDevices** | Selects all SuperDMX fixtures in the level |
| **ClearPatch** | Resets selected fixtures to Universe=1, StartAddress=1. **Fixture numbers are left alone** |
| **RenameID** | When checked, Apply renames fixture Actors using the fixture type name and FixtureID |
| **Apply** | Writes Patch values to the selected fixtures using the current start values |
| **Refresh** | Refreshes the Patch Preview table and channel grid |

When the panel opens, it scans existing fixtures and places the start values after the latest used Patch. If no fixtures exist, it starts from 1.1 and FixtureID 1.

---

## 4. Batch Patch

1. Select the SuperDMX fixtures you want to patch in the level.
2. Set **Start Universe**, **Start Address**, and **Start Fixture ID**.
3. Check or uncheck **RenameID** as needed.
4. Click **Apply**.

Apply performs the following operations on the currently selected fixtures:

- The first fixture uses the configured starting Universe and Start Address.
- Each following fixture continues after the previous fixture's channel span.
- If the next fixture does not fit in the remaining addresses of the current Universe, it moves to address 1 of the next Universe.
- If the Universe would exceed 512, or a single fixture's channel span is greater than 512, that fixture is skipped.
- FixtureID starts from **Start Fixture ID**, increments, and skips IDs already used by unselected fixtures in the level.
- FixtureID 1 on unselected fixtures is treated as a default unassigned value and is not counted as occupied.

> **Note**: Manual multi-selection uses the current editor selection order. After clicking **SelectAllDevices**, the tool sorts by parent Actor name and then by the fixture Actor name using natural ordering.

---

## 5. Patch Preview Table

The lower-left table shows all SuperDMX fixtures in the level:

| Column | Description | Editable |
|--------|-------------|----------|
| **Model** | Fixture model/type name | No |
| **Name** | Actor display name | Yes |
| **FixtureID** | Fixture number | Yes |
| **Universe** | DMX Universe | Yes, range 1-512 |
| **StartAddress** | Start address | Yes, range 1-512 |

Editing Name, FixtureID, Universe, or StartAddress writes the change back to the fixture. Universe and StartAddress are clamped to 1-512, and FixtureID is clamped to 1 or above.

The table is sorted by fixture model/type, then by trailing number in the Actor name. Duplicate FixtureIDs and overlapping DMX address ranges are shown in red so you can review the Patch.

---

## 6. Channel Grid

The right-side channel grid shows addresses 1-512 by Universe. Fixture address ranges appear as colored blocks:

- Clicking a block selects the fixture and synchronizes the table and editor selection.
- Ctrl-click toggles multi-selection.
- Dragging a block changes the fixture's Universe and StartAddress.
- Dropping selected fixtures onto an empty grid position assigns consecutive addresses and FixtureIDs starting from the drop point.
- Overlapping addresses are shown with a red overlay, and duplicate FixtureIDs are marked with a red strip at the fixture's first address cell.

---

## 7. Undo

Batch Apply, ClearPatch, grid drag, and drag-drop Patch operations use UE editor transactions and can be undone with **Ctrl+Z**. Some table text edits also call the Actor modification flow, but you should still confirm the level can be saved normally before making large edits.

---

## 8. FAQ

### Q: Apply does not change anything.
Confirm that the current selection contains SuperStage DMX fixtures. Regular StaticMeshActors are ignored by the Patch Tool.

### Q: Why do fixtures still have their numbers after ClearPatch?
That is deliberate. ClearPatch resets only the universe and start address; **fixture numbers are left alone**. Clearing 50 fixtures and setting every number to 1 would leave 50 fixtures sharing number 1, and the console could no longer tell them apart — "clear the patch" should not destroy the fixtures' identity along the way. The address is going to be rewritten by the next Apply anyway, and keeping the number leaves something to check against.

### Q: Why were some fixtures skipped by Apply?
Fixtures are skipped if their channel span is greater than 512, or if the assignment would require a Universe greater than 512.

### Q: What do red fields mean?
A red FixtureID means the ID is duplicated. A red StartAddress field or red grid area means address ranges overlap within the same Universe.

---

> **Next Steps**: Read [08 - DMX Recording & Playback](./08_DMX_Recording_Playback_en.md).
