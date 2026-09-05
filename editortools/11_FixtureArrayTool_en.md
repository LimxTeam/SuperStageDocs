# SuperStage Fixture Array Tool User Manual

## 1. Scope

The Fixture Array Tool copies the currently selected SuperStage DMX fixture into a linear, grid, or radial arrangement. The tool creates a preview from the current parameters, then creates regular fixtures when confirmed.

After creation, the original source fixture is deleted. The first generated fixture occupies the source position or the position calculated by the selected array mode. Make sure the source fixture can be replaced before using the tool.

## 2. Access

Switch the editor's left mode panel to **SuperStage Edit Mode**, then select **Fixture Array Tool** from the mode toolbar.

Select a SuperStage DMX fixture in the viewport before using the tool. If multiple Actors are selected, the tool uses the first usable DMX fixture in the selection.

## 3. Basic Workflow

1. Select one SuperStage DMX fixture in the viewport.
2. Open **Fixture Array Tool**.
3. Choose **Array Type**: Linear, Grid, or Radial.
4. Adjust the mode parameters and check the preview.
5. Click **Create Fixtures** to create the regular fixtures.
6. Click **Done** or close the tool if you want to exit without creating fixtures.

The create operation can be undone in the editor.

## 4. Status Fields

| Field | Description |
| --- | --- |
| Status | Current operation prompt |
| Source Fixture | Current implementation fixture name |
| Preview Count | Number of new fixtures in the preview |

## 5. Common Parameters

| Parameter | Description | Default |
| --- | --- | --- |
| Array Rotation | Rotation applied to the whole array around the source fixture | 0, 0, 0 |
| Item Rotation | Extra rotation applied to each fixture | 0, 0, 0 |
| Create Folder | Places generated Actors into a World Outliner folder | On |
| Folder Name | Folder name; when empty, uses the fixture type name plus `_Array` | Empty |

If the folder name already exists, the tool adds a numeric suffix.

## 6. Linear Array

Linear array starts at the source fixture position and places fixtures by the **Spacing** vector.

| Parameter | Description | Range/default |
| --- | --- | --- |
| Count | Number of fixtures | 1-100, default 5 |
| Spacing | 3D distance between neighboring fixtures, in centimeters | Default 100, 0, 0 |

Spacing is affected by the source fixture rotation first, then by Array Rotation. The source fixture orientation therefore affects the linear array direction.

## 7. Grid Array

Grid array creates a two-dimensional fixture layout.

| Parameter | Description | Range/default |
| --- | --- | --- |
| Count X | Count in the first direction | 1-50, default 3 |
| Count Y | Count in the second direction | 1-50, default 3 |
| Spacing X | Distance in the first direction, in centimeters | Minimum 1, default 100 |
| Spacing Y | Distance in the second direction, in centimeters | Minimum 1, default 100 |
| Plane | Normal axis of the grid plane | X, Y, Z, default Z |
| Center At Source | Centers the grid on the source fixture position | On |
| Honeycomb Offset | Odd-row offset ratio along the first direction | 0-1, default 0 |

Plane Z uses the XY plane, Plane Y uses the XZ plane, and Plane X uses the YZ plane.

## 8. Radial Array

Radial array places fixtures around the source fixture position in a circle or arc.

| Parameter | Description | Range/default |
| --- | --- | --- |
| Count | Number of fixtures on the circle or arc | 2-100, default 8 |
| Radius | Radius in centimeters | Minimum 10, default 200 |
| Start Angle | Start angle | 0-360, default 0 |
| End Angle | End angle | 0-360, default 360 |
| Rotation Axis | Normal axis of the radial plane | X, Y, Z, default Z |
| Orient To Center | Rotates fixtures to face the center | On |
| Include Center | Adds one extra fixture at the center | Off |

When the Start Angle to End Angle range is nearly 360 degrees, the tool treats it as a full circle and does not duplicate the first point at the end. For partial arcs, both the start and end positions are included.

## 9. Created Result

After **Create Fixtures**, the tool:

- Converts the current preview fixtures into regular Actors.
- Keeps the same fixture type and copied source settings.
- Continues naming from existing Actor numbers of the same fixture type.
- Places generated fixtures in a World Outliner folder when enabled.
- Deletes the original source fixture.
- Closes the tool.

After creation, check DMX addresses. If the source address was copied to multiple fixtures, use the Batch Patch Tool to assign new addresses.

## 10. Notes

- Regular point lights, spotlights, and other non-SuperStage DMX Actors cannot be used as the source fixture.
- Editor responsiveness when creating many fixtures depends on fixture complexity and current level size.
- The preview is for position and rotation checks; final DMX addressing still needs project-level verification.
- If no preview appears, confirm that a SuperStage DMX fixture is selected and check the Status field.
