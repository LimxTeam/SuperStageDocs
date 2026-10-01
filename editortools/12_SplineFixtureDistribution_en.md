# SuperStage Spline Fixture Distribution Tool User Manual

## 1. Scope

The Spline Fixture Distribution Tool places fixtures along a Spline Component. It does not use a source fixture. Instead, **Fixture Sequence** defines the fixtures to spawn, and the tool repeats that sequence along the spline.

A sequence item can carry either a **fixture definition asset** (data-driven fixtures, recommended) or a **fixture class** (Blueprint or hand-written C++ fixtures). When both are set on one item, the definition wins.

After creation, the spline Actor remains in the scene. Generated fixtures are independent Actors and are not linked to the spline afterward.

## 2. Access

Switch the editor's left mode panel to **SuperStage Edit Mode**, then select **Spline** from the mode toolbar.

Before using the tool, select an Actor that contains a Spline Component. You can also select the Spline Component directly.

## 3. Basic Workflow

1. Prepare an Actor with a Spline Component in the scene.
2. Select that Actor or its Spline Component.
3. Open **Spline**.
4. Add at least one item to **Fixture Sequence** and set its fixture definition or fixture class.
5. Set Count, Spacing, Start Offset, End Offset, and transform parameters.
6. Check the preview. It follows parameter changes and spline control point edits live.
7. Click **Create Fixtures** to create fixtures, or click **Cancel** to discard the preview and exit.

The create operation can be undone in the editor: Ctrl+Z removes the whole batch, and Ctrl+Y restores it together with labels, folders, Fixture IDs, and DMX addresses.

## 4. Spline Status

| Field | Description |
| --- | --- |
| Status | Current operation prompt |
| Target Spline | Name of the Actor that owns the selected spline |
| Preview Count | Number of preview fixtures |

## 5. Fixture Sequence

Fixture Sequence controls which fixtures are generated along the spline.

| Parameter | Description | Range/default |
| --- | --- | --- |
| Fixture Definition | Fixture definition asset to generate (data-driven fixtures) | Empty |
| DMX Mode | DMX mode for that definition; empty picks the first valid mode | Empty |
| Fixture Class | Fixture class to generate; used only when Fixture Definition is empty | Empty |
| Repeat | Number of consecutive repeats for this sequence item | 1-100, default 1 |
| Rotation | Extra rotation for this sequence item | 0, 0, 0 |

For example, a sequence of Spot x 2 and Wash x 1 creates Spot, Spot, Wash, then repeats.

Every data-driven fixture model shares one Actor class, so the model has to come from the definition asset. Picking SuperFixtureActor through Fixture Class alone produces empty fixtures with no definition.

## 6. Distribution Parameters

| Parameter | Description | Range/default |
| --- | --- | --- |
| Count | Number of fixtures to create; when 0, count is calculated from Spacing | 0-500, default 10 |
| Spacing (cm) | Spacing used when Count is 0 | Minimum 10, default 100 |
| Start Offset % | Start position as a percentage of spline length | 0-100, default 0 |
| End Offset % | End position as a percentage of spline length | 0-100, default 100 |

When Count is greater than 0, the tool places that many fixtures evenly between Start Offset and End Offset. When Count is 0, the tool calculates the count from the effective length and Spacing.

Start Offset must be smaller than End Offset, otherwise there is no valid path length for fixture generation.

The count calculated when Count is 0 is subject to the same limit of 500 that applies to a manually entered Count. A long spline with a small spacing hits that limit, and the Status field then reads `clamped to 500 (spacing too small)` — increase Spacing or build the run in sections.

## 7. Transform Parameters

| Parameter | Description | Default |
| --- | --- | --- |
| Rotation Offset | Rotation applied to all fixtures | 0, 0, 0 |
| Position Offset | Position offset in spline tangent space, in centimeters | 0, 0, 0 |
| Follow Spline Rotation | Uses spline direction as the base fixture orientation | On |

When Follow Spline Rotation is on, each fixture uses the spline direction first, then Rotation Offset and the sequence item's Rotation. When it is off, fixtures use Rotation Offset as a shared orientation, then the sequence item's Rotation.

## 8. Folder and Naming

| Parameter | Description | Default |
| --- | --- | --- |
| Create Folder | Places created Actors into a World Outliner folder | On |
| Folder Name | Folder name; when empty, uses `LightArray` | Empty |

If the folder name already exists, the tool adds a numeric suffix. Generated Actors are named `ModelName_FixtureID`, continuing from the highest Fixture ID already in the level. The number in the label is exactly the Fixture ID written into the fixture's DMX properties, so the two always agree.

## 9. DMX Assignment

| Parameter | Description | Default |
| --- | --- | --- |
| Assign DMX Addresses | Assigns sequential Universe and start addresses by channel span | On |

Fixture IDs are assigned regardless of this option, because the Actor label carries the ID.

When enabled, the tool starts after the last occupied slot in the level, advances by each fixture's channel span, and rolls over to the next universe past 512 — the same rule the Batch Patch Tool uses. Once the universe limit is reached, the remaining fixtures keep their default address and a warning is written to the output log.

When disabled, only Fixture IDs are assigned; Universe and start address stay at the fixture defaults for the Batch Patch Tool to handle. The Status field then reads `DMX addresses left to Patch Tool`.

## 10. Created Result

After **Create Fixtures**, the tool:

- Creates the regular fixtures inside a single transaction, so the whole batch can be undone.
- Uses fixture definitions or classes by cycling through Fixture Sequence.
- Sets each fixture's location, rotation, and fixed 1:1:1 scale.
- Assigns Fixture IDs, and DMX addresses when enabled.
- Names fixtures from the model name and ID, and places them in a World Outliner folder when enabled.
- Selects the newly created batch.
- Keeps the original spline Actor.
- Closes the tool.

## 11. Examples

| Scenario | Suggested setup |
| --- | --- |
| Curved light row | Set Count to the required fixture count and keep Follow Spline Rotation on |
| Closed circular edge lights | Use a closed spline, Start Offset 0%, End Offset 100% |
| Alternating fixtures | Add multiple items to Fixture Sequence and set Repeat |

## 12. Notes

- Fixture Sequence needs at least one item with a definition or a class, otherwise no preview is generated.
- A zero-length spline or Start Offset not smaller than End Offset will not generate fixtures.
- While previewing, spline shape changes are reflected by the tool; created fixtures do not keep following the spline.
- While a preview is up, saving the level with Ctrl+S, starting PIE, or switching to another tool all **discard** the preview rather than committing it. **Create Fixtures** is the only way to commit.
- Panel parameters are remembered within the same editor session and reset to defaults after an editor restart.
- Editor responsiveness when creating many fixtures depends on fixture count, fixture complexity, and level size. Parameter changes move the preview in place instead of rebuilding it, so dragging a slider is much lighter than the first generation.
