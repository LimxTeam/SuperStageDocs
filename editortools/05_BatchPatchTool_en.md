# SuperStage Batch Patch Tool — User Manual

## 1. Overview

The Batch Patch Tool (Super Patch Tool) is used to batch assign DMX addresses to multiple selected fixtures in the scene. It can automatically calculate each fixture's Universe and starting channel address, supports preview, apply, and undo.

---

## 2. Access

**Main Menu Path**: Toolbar **SuperStage** dropdown menu → **SuperDMXTool** → **PatchTool**

---

## 3. Interface Description

```
┌──────────────────────────────────────┐
│  Starting Parameter Settings          │
│  ┌────────────┬──────────┬──────┐   │
│  │ Start Univ  │ Start Addr│Start FID│   │
│  └────────────┴──────────┴──────┘   │
│                                      │
│  Fixture List (Preview)              │
│  ┌──────────────────────────────┐   │
│  │ Fixture Name | Univ | Addr   │   │
│  │ Light_1      | 1    | 1      │   │
│  │ Light_2      | 1    | 25     │   │
│  │ Light_3      | 1    | 49     │   │
│  │ ...                          │   │
│  └──────────────────────────────┘   │
│                                      │
│  [Refresh Selection] [Preview] [Apply]│
└──────────────────────────────────────┘
```

---

## 4. Parameter Description

### 4.1 Starting Parameters

| Parameter | Description | Range | Default |
|-----------|-------------|-------|---------|
| **Start Universe** | Universe number assigned to the first fixture | 1 - 512 | 1 |
| **Start Address** | Starting channel address for the first fixture in that Universe | 1 - 512 | 1 |
| **Start Fixture ID** | Fixture ID number for the first fixture; subsequent fixtures auto-increment | 1 - 9999 | 1 |

### 4.2 Address Assignment Rules

The tool automatically calculates subsequent fixture addresses using the following rules:

1. Start from the specified Start Universe and Start Address
2. The number of channels each fixture occupies is determined by its fixture library's Channel Span
3. After a fixture is assigned, the next fixture's address = current address + channel span
4. If the address exceeds 512 (the maximum channels in a Universe), it automatically wraps to channel 1 of the next Universe
5. FixtureID auto-increments from **Start Fixture ID**, skipping IDs already occupied by other fixtures in the scene (ID=1 is an exception as a default value)

---

## 5. Fixture List

### 5.1 Column Description

| Column | Description |
|--------|-------------|
| **Fixture Name** | The label name of the fixture in the scene |
| **Model** | The fixture model/type name |
| **Universe** | The assigned Universe number |
| **Start Address** | The assigned starting channel address |
| **Channels** | The number of channels this fixture occupies |
| **FixtureID** | The assigned fixture ID number |

### 5.2 Sorting Rules

The fixture list is displayed in natural sort order. For example:
- Light_1, Light_2, Light_3, ..., Light_10, Light_11
- Not: Light_1, Light_10, Light_11, ..., Light_2, Light_3

---

## 6. Panel Controls

The panel always acts on the fixtures **currently selected in the level**, and follows that selection — pick a different batch in the viewport or the outliner and the plan is recalculated for them. There is **no separate Preview button**: the list always shows a plan that has not been written yet, and **Apply** is what commits it.

| Control | Type | What it does |
|---------|------|--------------|
| **Start Universe** | Field | Starting universe |
| **Start Address** | Field | Starting address |
| **Start Fixture ID** | Field | Starting fixture number |
| **SelectAllDevices** | Button | Selects every fixture in the level — useful when starting from scratch |
| **RenameID** | Checkbox | Also renames the selected fixtures so the outliner name matches the number they were just given. Untick it if you have named fixtures deliberately and want those names kept |
| **Apply** | Button | Writes the plan to the fixtures |
| **ClearPatch** | Button | Resets the selected fixtures to universe 1, address 1. **Fixture numbers are left alone** — the address is about to be rewritten anyway, while the number is how a console identifies the fixture and is not worth throwing away |
| **Refresh** | Button | Re-reads both the current selection and the level, for when fixtures were added or changed elsewhere while the panel was open |

After each Apply the three start values move on to the next free slot, so patching a whole rig is just **select, Apply, select, Apply** — you never have to touch the numbers in between.

**Ctrl+Z undoes the whole batch.**

---

## 7. Workflow

### Step 1: Select Fixtures

In the 3D viewport, box-select or individually select the fixtures to be batch patched.

### Step 2: Open the Batch Patch Tool

Open the tool panel via the main menu.

### Step 3: Set Starting Parameters

- Set **Start Universe** (e.g., Universe 1)
- Set **Start Address** (e.g., starting from channel 1)

### Step 4: Check the Plan

- The list already shows the plan that has not been written yet, and it updates live with the selection — nothing needs to be pressed to generate it;
- If fixtures were added or changed elsewhere while the panel was open, press **Refresh** to re-read them;
- Read the summary line under the toolbar — it says how many fixtures are about to be patched, and **hovering it** shows the exact range of universes, addresses and fixture numbers they would land on.

> An **orange** summary line means some fixtures cannot be patched. Apply skips them and patches everything else. Hover it to see which ones and why:
> **Span>512** — the fixture needs more channels than a single universe holds, so it cannot be patched anywhere; check that it is on the right DMX mode.
> **Universe overflow** — the run went past the last universe; start lower, or patch fewer fixtures in one pass.
> Check this line before pressing Apply, rather than discovering afterwards that part of the rig was left out.

### Step 5: Apply

- Press **Apply** to write the addresses to the fixtures;
- The three start values then move on to the next free slot, so you can select the next batch and press Apply again straight away.

---

## 8. The Channel Chart

The lower half of the panel lays out each universe's 512 slots as a grid (32 columns × 16 rows), with each patched fixture drawn as a coloured block over the addresses it occupies. It is the quickest way to spot a gap worth filling or a clash worth fixing.

**Every universe in use is stacked vertically**, each block under a `Universe N` header row, and you scroll through them — there is no paging control.

### 8.1 Patching Directly on the Chart

The chart is not read-only — you can patch on it:

- **Drag a fixture block** to a new slot to change its address, or drag it down onto another universe to move it there;
- When several fixtures are selected, **dragging any one of them moves the whole selection together**, each keeping its own offset — fixtures spread across two universes stay one universe apart;
- **A drop that would land on top of another fixture turns red and is refused.** Nothing is overwritten by accident; move to a free run and the colour comes back.

### 8.2 Dragging Fixtures In from the Outliner

Fixtures dragged in from the World Outliner drop onto the **first free run of channels from where you let go**, stepping over anything already patched.

- Fixtures that already have a number **keep it** — only ones that have never been numbered are given one;
- If some do not fit, a message tells you how many were left out.

---

## 9. Undo

Apply operations support **Ctrl+Z undo**. If you find errors in the assignment, you can undo to restore the previous state.

---

## 10. Notes

- Only SuperStage DMX fixtures (subclasses of SuperDmxActorBase) appear in the list
- Regular UE lights or other types of Actors are not recognized
- If no fixtures are selected in the scene, the list will be empty
- Fixtures with a channel span of 0 may cause address calculation anomalies; ensure the fixture library is correctly configured
- With a large number of fixtures, addresses may span multiple Universes; ensure your DMX network supports the required number of Universes

---

## 11. FAQ

| Issue | Solution |
|-------|----------|
| Fixture list is empty | Select fixtures in the scene first, then click "Refresh Selection" |
| Preview shows incorrect addresses | Check if Start Universe and Start Address settings are correct |
| Fixtures don't respond to console after applying | Check if input settings in DMX Config Panel match |
| Some fixture addresses overlap | Check if channel span configuration in the fixture library is correct |
