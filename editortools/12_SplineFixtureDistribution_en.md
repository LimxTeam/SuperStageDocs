# SuperStage Spline Fixture Distribution Tool User Manual

## 1. Scope

The Spline Fixture Distribution Tool places fixtures along a Spline Component. It does not use a source fixture. Instead, **Fixture Sequence** defines the fixture classes to spawn, and the tool repeats that sequence along the spline.

After creation, the spline Actor remains in the scene. Generated fixtures are independent Actors and are not linked to the spline afterward.

## 2. Access

Switch the editor's left mode panel to **SuperStage Edit Mode**, then select **Light Array Tool** from the mode toolbar.

Before using the tool, select an Actor that contains a Spline Component. You can also select the Spline Component directly.

## 3. Basic Workflow

1. Prepare an Actor with a Spline Component in the scene.
2. Select that Actor or its Spline Component.
3. Open **Light Array Tool**.
4. Add at least one fixture class to **Fixture Sequence**.
5. Set Count, Spacing, Start Offset, End Offset, and transform parameters.
6. Check the preview.
7. Click **Create Fixtures** to create fixtures, or click **Done** to exit.

The create operation can be undone in the editor.

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
| Fixture Class | SuperStage DMX fixture class to generate | Empty |
| Repeat | Number of consecutive repeats for this fixture class | 1-100, default 1 |
| Rotation | Extra rotation for this sequence item | 0, 0, 0 |

For example, a sequence of Spot x 2 and Wash x 1 creates Spot, Spot, Wash, then repeats.

## 6. Distribution Parameters

| Parameter | Description | Range/default |
| --- | --- | --- |
| Count | Number of fixtures to create; when 0, count is calculated from Spacing | 0-500, default 10 |
| Spacing (cm) | Spacing used when Count is 0 | Minimum 10, default 100 |
| Start Offset % | Start position as a percentage of spline length | 0-100, default 0 |
| End Offset % | End position as a percentage of spline length | 0-100, default 100 |

When Count is greater than 0, the tool places that many fixtures evenly between Start Offset and End Offset. When Count is 0, the tool calculates the count from the effective length and Spacing.

Start Offset must be smaller than End Offset, otherwise there is no valid path length for fixture generation.

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

If the folder name already exists, the tool adds a numeric suffix. Generated Actors are named from their fixture type name and continue from existing numeric labels.

## 9. Created Result

After **Create Fixtures**, the tool:

- Converts preview fixtures into regular Actors.
- Uses fixture types by cycling through Fixture Sequence.
- Sets each fixture's location, rotation, and fixed 1:1:1 scale.
- Places fixtures in a World Outliner folder when enabled.
- Keeps the original spline Actor.
- Closes the tool.

Generated fixtures do not receive unique DMX addresses automatically. After creation, use the Batch Patch Tool to check and assign addresses as needed.

## 10. Examples

| Scenario | Suggested setup |
| --- | --- |
| Curved light row | Set Count to the required fixture count and keep Follow Spline Rotation on |
| Closed circular edge lights | Use a closed spline, Start Offset 0%, End Offset 100% |
| Alternating fixtures | Add multiple fixture types to Fixture Sequence and set Repeat |

## 11. Notes

- Fixture Sequence needs at least one valid fixture class, otherwise no preview is generated.
- A zero-length spline or Start Offset not smaller than End Offset will not generate fixtures.
- While previewing, spline shape changes are reflected by the tool; created fixtures do not keep following the spline.
- Editor responsiveness when creating many fixtures depends on fixture count, fixture complexity, and level size.
