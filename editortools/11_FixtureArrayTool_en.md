# SuperStage Fixture Array Tool User Manual

## 1. Scope

The Array Tool copies the currently selected actors into a linear, grid, or radial arrangement. The tool creates a preview from the current parameters, then creates regular actors when confirmed.

**Sources are not restricted to fixtures.** Every actor in the selection is copied as a group - the real workflow is "a length of truss with its fixtures, arrayed together". Fixtures get fresh Fixture IDs and `ModelName_ID` labels; truss, scenery and other plain actors consume no Fixture ID and only get a unique label derived from the source name.

The source group is not deleted. On creation it claims, as a whole, the array slot nearest to the **group centroid**, and only the remaining slots get newly created actors. This preserves each source's DMX address, Fixture ID, name, layers, and any Sequencer or MA3 bindings that reference it. The total count matches slots x actors-in-group.

> In Linear arrays, and in Radial arrays with Include Center enabled, the nearest slot is usually the source group's original position, so it does not move. In Grid arrays with Center At Source enabled, the whole group moves to the nearest cell.

## 2. Access

Switch the editor's left mode panel to **SuperStage Edit Mode**, then select **Array** from the mode toolbar.

Select one or more actors in the viewport before using the tool. The **first** actor in the selection is the primary source: its orientation defines the array frame, so the Spacing vector and grid axes are interpreted in it. The array origin is the centroid of the selected actors' locations (for a single selection, that is simply its own location).

## 3. Basic Workflow

1. Select one or more actors in the viewport (fixtures, truss and scenery all work).
2. Open **Array**.
3. Choose **Array Type**: Linear, Grid, or Radial.
4. Adjust the mode parameters and check the preview.
5. Click **Create Fixtures** to create the regular fixtures.
6. Click **Cancel** or close the tool if you want to exit without creating fixtures.

The create operation can be undone in the editor: Ctrl+Z removes the newly created actors and returns the source group to its original position; Ctrl+Y restores them.

## 4. Status Fields

| Field | Description |
| --- | --- |
| Status | Current operation prompt |
| Source Group | The source group: its name when one actor is selected, otherwise "primary + N more" |
| Preview Count | Number of actors in the preview, including the slot claimed by the source group |

## 5. Common Parameters

| Parameter | Description | Default |
| --- | --- | --- |
| Array Rotation | Rotation applied to the whole array around the group centroid | 0, 0, 0 |
| Item Rotation | Extra rotation applied to each copy (with a multi-actor group it rotates that slot as a whole, not each actor about itself) | 0, 0, 0 |
| Create Folder | Places generated Actors into a World Outliner folder | On |
| Folder Name | Folder name; when empty, uses the model name plus `_Array` | Empty |
| Assign DMX Addresses | Assigns sequential Universe and start addresses to the new fixtures | On |

If the folder name already exists, the tool adds a numeric suffix. The source group is moved into the same folder.

The automatic folder name uses the **model name**, not the Actor class name: every data-driven fixture model shares one Actor class, so a class name would produce an unrecognizable `SuperFixtureActor_Array`.

## 6. Linear Array

Linear array starts at the group centroid and places one copy of the group per step along the **Spacing** vector.

| Parameter | Description | Range/default |
| --- | --- | --- |
| Count | Number of fixtures | 1-100, default 5 |
| Spacing | 3D distance between neighboring fixtures, in centimeters | Default 100, 0, 0 |

Spacing is affected by the **primary source** (first actor in the selection) rotation first, then by Array Rotation. The primary source orientation therefore affects the linear array direction.

## 7. Grid Array

Grid array creates a two-dimensional fixture layout.

| Parameter | Description | Range/default |
| --- | --- | --- |
| Count X | Count in the first direction | 1-50, default 3 |
| Count Y | Count in the second direction | 1-50, default 3 |
| Spacing X | Distance in the first direction, in centimeters | Minimum 1, default 100 |
| Spacing Y | Distance in the second direction, in centimeters | Minimum 1, default 100 |
| Plane | Normal axis of the grid plane | X, Y, Z, default Z |
| Center At Source | Centers the grid on the group centroid | On |
| Honeycomb Offset | Odd-row offset ratio along the first direction | 0-1, default 0 |

Plane Z uses the XY plane, Plane Y uses the XZ plane, and Plane X uses the YZ plane.

## 8. Radial Array

Radial array places copies of the group around the group centroid in a circle or arc.

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

With Orient To Center enabled, each fixture's orientation is derived from its direction to the ring center, using the ring plane normal as the reference up vector, so roll stays consistent all the way around. Array Rotation then affects position only — the orientation is already derived from the rotated positions and is not rotated a second time.

## 9. DMX Assignment

New fixtures are template clones of the source fixture, so they inherit its Universe and start address. With **Assign DMX Addresses** enabled (the default), the tool re-assigns sequential addresses to the new fixtures: it starts after the last occupied slot in the level, advances by each fixture's channel span, and rolls over to the next universe past 512 — the same rule the Batch Patch Tool uses. Once the universe limit is reached, the remaining fixtures keep their template address and a warning is written to the output log.

With the option disabled, only Fixture IDs are assigned and the addresses stay at their template values (that is, the whole array shares one address), leaving the addressing to the Batch Patch Tool. The Status field then reads `DMX addresses left to Patch Tool`.

Fixture IDs are always assigned to new fixtures regardless of this option, because the Actor label carries the ID and the two must agree. The source fixture keeps its own existing ID and address untouched.

## 10. Created Result

After **Create Fixtures**, the tool:

- Creates the regular actors inside a single transaction, so the whole batch can be undone.
- Uses each source actor as its own template to keep matching settings.
- Lets the source group claim the slot nearest the centroid and creates actors for the remaining slots.
- Assigns Fixture IDs to new **fixtures**, and DMX addresses when enabled; plain actors only get label de-duplication.
- Names new fixtures `ModelName_FixtureID`; plain actors reuse the source name with a numeric suffix. The source group keeps its own names.
- Places everything in a World Outliner folder when enabled (the source group too).
- Selects the whole batch.
- Closes the tool.

## 11. Notes

- Any actor can be a source, including plain static meshes (truss, scenery) and non-SuperStage lights. Only SuperStage DMX fixtures take part in Fixture ID assignment and fixture naming.
- Editor responsiveness when creating many actors depends on actor complexity and current level size. Parameter changes move the preview in place instead of rebuilding it, so dragging a slider is much lighter than the first generation. Note the total is slots x actors-in-group, so a large grid over a whole truss adds up fast.
- The source group is temporarily hidden while the preview is up, because previews already occupy its slot. Each source is restored to your own hidden state when the preview is cleared or the tool exits.
- While a preview is up, saving the level with Ctrl+S, starting PIE, or switching to another tool all **discard** the preview rather than committing it. **Create Fixtures** is the only way to commit.
- Panel parameters are remembered within the same editor session and reset to defaults after an editor restart.
- If no preview appears, confirm that actors are actually selected in the viewport and check the Status field.
