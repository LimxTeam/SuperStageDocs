# SuperStage Mirror Tool User Manual

## 1. Scope

The Mirror Tool copies the selected actors across a plane to the opposite side. Stage rigs are almost always left/right symmetric, and building the second half by hand is slow and error-prone.

**Sources are not restricted to fixtures.** Every actor in the selection is mirrored - when you mirror a symmetric position, the truss and scenery need to come along. Fixtures get fresh Fixture IDs and `ModelName_ID` labels; plain actors consume no Fixture ID and only get a unique label derived from the source name.

Source actors are neither moved nor deleted; the mirrored copies are new actors.

## 2. Access

Switch the editor's left mode panel to **SuperStage Edit Mode**, then select **Mirror** from the mode toolbar.

Select one or more actors in the viewport before using the tool.

## 3. Basic Workflow

1. Select the actors to mirror in the viewport.
2. Open **Mirror**.
3. Set **Plane Normal** (the mirror plane's normal axis) and **Plane Offset** (the plane's position along that axis). The plane is drawn in the viewport.
4. Optionally click **Set Plane To Selection Center** to move the plane to the center of the selection.
5. Check the preview.
6. Click **Create Fixtures** to create them, or **Cancel** to discard the preview and exit.

The create operation can be undone in the editor: Ctrl+Z removes the whole mirrored batch, and Ctrl+Y restores it.

## 4. Status Fields

| Field | Description |
| --- | --- |
| Status | Current operation prompt |
| Source Actors | How many actors are acting as mirror sources |
| Preview Count | Number of actors in the preview |

## 5. Mirror Plane

| Parameter | Description | Default |
| --- | --- | --- |
| Plane Normal | Normal axis of the mirror plane. Y mirrors left/right across the stage centerline | Y |
| Plane Offset (cm) | Position of the plane along its normal axis, in world centimeters | 0 |

The plane is drawn as a blue grid in the viewport, scaled to the size of the selection.

Actors sitting on the plane (within a 1 cm tolerance) are skipped - their mirror image is themselves, so you would only get an overlapping duplicate. The Status field reports how many were skipped.

## 6. How the Mirrored Orientation Is Computed

This section explains the tool's behavior so it is not mistaken for a bug.

**Position** is reflected across the plane. That part is uncontroversial.

**Orientation is not simply "the mirrored transform".** A reflection matrix has determinant -1; applying it to an actor is equivalent to a negative scale and turns the mesh inside out. More importantly, mirrored hardware does not exist - the fixture you hang on the other side is the same model, hung the same way, just turned around.

What the tool does instead: reflect the beam axis and the reference tangent, then rebuild a valid right-handed rotation from that pair. The result is a beam that aims **symmetrically**, with the fixture's own left and right sides swapped - exactly what happens when you hang the same fixture on the opposite side. **The yoke needs no special handling, and should not get any.**

## 7. DMX Assignment

| Parameter | Description | Default |
| --- | --- | --- |
| Assign DMX Addresses | Assigns sequential Universe and start addresses to the mirrored fixtures | On |

Fixture IDs are assigned to new fixtures regardless of this option, because the Actor label carries the ID.

**This tool does not handle pan inversion.** For a symmetric position to follow the same programming, pan has to be inverted at the **patch layer** - it cannot be produced geometrically, as no rotation turns "pan" into "inverted pan". In the MVR specification this is a per-fixture `DMXInvertPan` flag, and consoles (MA, for example) keep it in the patch as well. Handle it on the console or in the patch step.

Gobo and prism rotation direction are likewise not mirrored - those are per-cue programming choices (phase-mirrored in the console's effect engine), not fixture properties.

## 8. Created Result

After **Create Fixtures**, the tool:

- Creates the regular actors inside a single transaction, so the whole batch can be undone.
- Uses each source actor as its own template to keep matching settings.
- Assigns Fixture IDs to new fixtures, and DMX addresses when enabled; plain actors only get label de-duplication.
- Places everything in a World Outliner folder when enabled (`MirroredFixtures` when the name is left empty).
- Selects the newly created batch.
- Closes the tool.

## 9. Notes

- Any actor can be a mirror source, including plain static meshes. Only SuperStage DMX fixtures take part in Fixture ID assignment and fixture naming.
- While a preview is up, saving the level with Ctrl+S, starting PIE, or switching to another tool all **discard** the preview rather than committing it. **Create Fixtures** is the only way to commit.
- Panel parameters are remembered within the same editor session and reset to defaults after an editor restart.
- If no preview appears, confirm that actors are actually selected in the viewport and check the Status field - it may also be that every selected actor sits on the mirror plane.
