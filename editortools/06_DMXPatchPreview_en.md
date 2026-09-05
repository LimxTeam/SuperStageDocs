# SuperStage DMX Patch Preview User Manual

## Purpose

DMX Patch Preview shows and edits patch information for SuperStage DMX fixtures in the current level. The left side is a fixture list and the right side is a DMX channel grid.

The panel only collects SuperStage DMX fixtures. It does not show regular Unreal lights or other non-DMX Actors.

## Opening The Panel

It is the lower half of the **Super Patch Tool** panel: SuperStage toolbar → **SuperDMXTool** → **PatchTool**; the patch preview is below the parameter bar.

## List Columns

| Column | Description | Editable |
| --- | --- | --- |
| Model | Fixture model/type name. | No |
| Name | Actor label in the level. | Yes |
| FixtureID | Fixture ID. Duplicate IDs are shown in red. | Yes |
| Universe | DMX Universe. | Yes |
| StartAddress | Start address. Overlapping address ranges in the same Universe are shown in red. | Yes |

## Editable Ranges

| Field | Source Limit |
| --- | --- |
| FixtureID | Minimum 1, maximum `INT32_MAX`. |
| Universe | 1-512. |
| StartAddress | 1-512. |

Numeric input is clamped into the allowed range.

## Sorting

When the list refreshes, rows are sorted by:

1. Fixture model/type name.
2. Trailing number in the Actor label.
3. Actor label text.

For fixtures of the same model/type, labels ending in numbers are ordered by those numbers.

## Conflict Indicators

When the list refreshes, the panel calculates:

- Whether FixtureID values are duplicated.
- Whether address ranges overlap inside the same Universe.

Conflicting values are shown in red. Address range uses the fixture's current channel span.

## Selection Linking

- Selecting one or more rows in the left list selects the corresponding level Actors and highlights them in the channel grid.
- Selecting fixtures in the channel grid also selects the level Actors and matching list rows.
- After an address drag operation is committed in the channel grid, the list refreshes.

## Notes

- Editing Name updates the Actor label in the level.
- Editing FixtureID, Universe, or StartAddress writes back to the fixture's Patch settings and triggers editor update.
- Conflict marks are calculated during list refresh. After manual address edits, reopen the panel or refresh through the channel grid before relying on the current conflict state.
