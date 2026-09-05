# Selecting and Programming

> Applies to SuperStage 26H2.6 and later ｜ Read first: [Console Overview](00_Console_Overview_en.md)

This document covers selecting fixtures, changing values, and storing the result into groups and presets.

---

## 1. Selecting Fixtures

Three routes:

| Route | Description |
| --- | --- |
| Fixture Sheet | Click in the fixture table |
| Layout panel | Click on the fixture layout |
| Command line | `Fixture 1 Thru 20`, `Group 3` |

Selection itself is undoable.

---

## 2. The Programmer

Values you change go into the **programmer** first. The programmer is a holding layer: it sits above every playback until you clear it or store it into an object.

Ways to change a value:

- **Turn an encoder wheel** — the row of wheels on the encoder bar; one gesture is one undo step;
- **Type an exact value** — click a wheel to open a numeric entry box;
- **The command line**;
- **Pick a colour in the Color Picker panel**.

> The Fixture Sheet is for **selecting** and **reading**, not for editing values: click, `Ctrl+click` to add, `Shift+click` for a range, select all, cancel.

Attributes are grouped by category, matching the preset categories: Dimmer, Position, Gobo, Color, Beam, Focus, Control, Shaper, Strobe, Prism, Frost, Effect, Other.

---

## 3. Groups

A group stores **a set of fixtures**, no values.

- Select fixtures → **Store** → **Group** → slot number;
- Command line: `Store Group 5`;
- Copy / move: `Copy Group 1 At 5`, `Move Group 1 At 5`.

Group slots appear as tiles in the Groups panel; their appearance is customisable (section 5).

---

## 4. Presets

A preset stores **a set of attribute values**. Each entry records the attribute name, its category and a value (0–100).

Presets are filed by category, matching the attribute groups:

| Category | Typical content |
| --- | --- |
| All | Every attribute |
| Dimmer | Intensity |
| Position | Pan / Tilt |
| Gobo | Gobo wheels and rotation |
| Color | Mixing, colour wheel, colour temperature |
| Beam | Zoom, iris |
| Focus | Focus |
| Control | Control channels |
| Shaper | Framing |
| Strobe | Strobe |
| Prism | Prism |
| Frost | Frost |
| Effect | Effects |

Operations:

- **Store** → **Preset** → category + slot number;
- Command line: `Store Preset 2`, `Copy Preset 1 At 5`, `Move Preset 1 At 5`, `Delete Preset 3`.

---

## 5. Slot Appearance

Group slots, preset slots and sidebar view slots share one **named appearance**, with four independently toggled layers:

| Layer | Description |
| --- | --- |
| Name | Slot text |
| Background | Background / text / accent colour |
| Image | One texture, either Cover (fill, crop overflow) or Contain (fit, leave margins) |
| Doodle | Hand-drawn marking |

Key points:

- The combination of the four toggles is the "name mode" — image only, name only, image behind name, or a plain colour block are all the same data presented differently;
- **Doodles are stored per stroke using normalised coordinates**, so the same doodle lands identically on a small sidebar tile and a large preview;
- **Images are stored by reference, not embedded.** One image used by several slots does not take up show file size repeatedly.

---

## 6. Clearing the Programmer

Clearing the programmer hands output back to the playbacks. It does not affect groups, presets or cues already stored.

---

## 7. Common Questions

### I changed a value and the fixture did not respond

1. Confirm the fixture exists in the console Patch panel with the same universe and address as the scene;
2. Confirm the attribute exists in the fixture definition bindings (see the Validation page in [Fixture Editor](../fixture/03_FixtureEditor_en.md));
3. Confirm the scene fixture's `ControlMode` is `DMX`, not `Property`.

### A preset only affects some of the fixtures

Presets record attribute names. When fixture models use different attribute names, only the ones whose names match respond.

### I stored a group in the wrong slot

`Move Group <old> At <new>`, or simply store over it again. The whole operation is in the undo stack.

---

## 8. Related Documents

- [Console Overview](00_Console_Overview_en.md)
- [Cues and Playback](02_Cues_and_Playback_en.md)
- [Command Line](03_CommandLine_en.md)
- [Effects](04_Effects_en.md)
