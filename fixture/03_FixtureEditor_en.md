# Fixture Editor

> Applies to SuperStage 26H2.6 and later ｜ Read first: [The Fixture Definition Asset](01_FixtureDefinition_en.md)

The fixture editor edits one fixture definition: its DMX modes, its moving parts, its beam and its wheels.

**Open**: double-click a fixture definition asset in the Content Browser — inside a model folder it is the one sharing the folder's name and carrying no prefix — or right-click a fixture in the scene and choose to edit its definition.

There is a help button in the top-right corner of the panel; its content matches this document.

---

## 1. Layout

The window is split into two columns.

**The left roughly two-thirds is a live preview** — the actual fixture with its actual beam, driven the same way it would be on stage. It carries the usual UE viewport controls, so you can orbit it, switch to an orthographic camera, or drop to wireframe to look at the body.

**The right-hand side holds four pages, tabbed in two pairs:**

| Position | Page | Content |
| --- | --- | --- |
| Upper (Fixture in front) | **Fixture** | Everything that defines the fixture, grouped into sections: who makes it, its physical data, its moving parts, where the light comes out, its wheels and prisms, and its DMX modes |
| | **DMX Test** | Drive the fixture by hand and watch the preview respond |
| Lower (Channel Map in front) | **Channel Map** | The whole address footprint at a glance |
| | **Validation** | Everything wrong with the fixture as it stands |

The pairing is deliberate: Fixture and DMX Test are the **change one thing, push it, look** loop, and tabbing them saves space and avoids scanning left and right; Channel Map and Validation are two views of the same thing — validation is telling you *which channel is bound wrong* — so one click puts them side by side in your head.

> Test, Channel Map and Validation all read the **compiled fixture**, so they stay blank until you press Compile at least once.

### 1.1 Toolbar and Summary Bar

Four toolbar buttons: **Compile**, **Import GDTF**, **Import JSON**, **Export JSON**.

To their right is a live summary of the fixture's size:

```text
Modes 2  ·  Emitter instances 25  ·  Ops 41  ·  Channels 39
```

| Item | Meaning |
| --- | --- |
| **Modes** | Number of DMX modes |
| **Emitter instances** | Total runtime components after every emitter is expanded by its layout. **This is the fixture's size figure** — a circle layout with Count X = 25 reads 25 here |
| **Ops** | Opcodes compiled for the default mode |
| **Channels** | Channel span of the default mode |

The last two come from the **default mode's compiled product**, and read 0 before it has been compiled.

---

## 2. Starting From a Manufacturer Package

You rarely build a fixture from nothing. **Import GDTF** reads the manufacturer's own package and fills the fixture in — channels, wheels, model.

The import dialog lets you choose **which parts to bring in**. That matters on a re-import: **anything left unticked is kept exactly as it is**. So if you have hand-tuned the moving parts but the manufacturer has released new gobo images, tick only the wheel atlases and your work survives.

> **Ticked sections are replaced wholesale, not merged.** Untick anything you want to keep.

To import a whole folder of packages at once, use the batch import tool rather than doing them one at a time here. See [GDTF Import](02_GdtfImport_en.md).

---

## 3. Compile and the Validation Page

**Compile** does two things, in order:

1. **Rebuilds every mode's bindings from the current channel library**;
2. Compiles the runtime product and runs validation.

Combining them into one button is deliberate: bindings are derived from the channel library's attribute table by rule, so editing the library without rebuilding means compiling the old bindings against a new library; and rebuilding without compiling leaves the old product in place. The two always belong together. They run **in one transaction**, so a single undo rolls back both and you never end up with "new bindings, old product".

**Press it after any change** — the fixture will not reflect your edits on stage until you do.

> **This is also the only way a hand-written channel library gets bindings.** Bindings used to come only from a GDTF import, so a hand-written library that never sees this button has none at all and the fixture does nothing.

If compilation fails, a dialog points you at the Validation page for the specific errors.

### 3.1 Reading the Validation Page

Results are sorted by severity, and each row reads:

```text
✕  Standard 39CH / binding 7    Attribute 'Zomm' not found in the channel library.
⚠  Standard 39CH                Channel 'Iris' is defined in the library but not bound to any capability.
```

| Level | Meaning |
| --- | --- |
| ✕ Error | The fixture will not work. It has to be fixed. **Any Error voids the compiled product** |
| ⚠ Warning | It will work, but something is probably not what you intended |
| ℹ Note | Worth knowing, nothing to do |

After the glyph comes a **location prefix**: the mode name, plus `/ binding N` where the issue can be pinned to one binding — count to that index in the Fixture page's binding table. With nothing wrong, the summary reads `Validation: no issues.`

Every diagnostic and its cause is listed in section 6 of the [Fixture Builder](06_FixtureBuilder_en.md).

### 3.2 How It Differs From the Content Browser's Compile

The Content Browser right-click menu also has **Compile** with multi-select batch support, but **it only compiles — it does not rebuild bindings**. After editing a channel library, use the button **in the editor**; the batch right-click is for sweeping the whole library after changing the importer or the compiler.

Both entry points leave the asset **dirty without saving it**. Unsaved, every load recompiles from scratch.

---

## 4. DMX Test Page

One slider per channel, values sent straight to the preview — the fastest way to find out whether pan really pans and whether the gobo you expect is the gobo you get.

| Button | Effect |
| --- | --- |
| **Default** | Opens the dimmer and shutter and centres pan, tilt and zoom, so you see the fixture doing something sensible in one click |
| **Blackout** | Zeroes everything |

A channel that has named slots — a gobo wheel, a colour wheel — shows a **slots** button. Pick a slot by name and it jumps there, rather than making you hunt for the right raw value.

Two view aids sit alongside:

| Aid | Effect |
| --- | --- |
| **Rig** | Draws each axis' rotation line, pivot and travel arc on the model. This is the only reliable way to see whether a pivot actually sits on the mechanical joint |
| **Floor** | Hides the floor when the light pool is in the way of the body |

---

## 5. Channel Map Page

Lays the mode's address footprint out as a grid, one cell per channel, and says in one line how many of them are actually in use. Hover a cell to see what claims it.

It exists to catch the three mistakes a channel table makes most often:

| Problem | Symptom |
| --- | --- |
| **Conflicts** | Two attributes claiming the same channel. Only one of them will ever be heard |
| **Gaps** | A channel the mode declares but nothing is bound to. Usually a channel that failed to import, or one you meant to fill in |
| **Overflow** | Bindings reaching past the end of the declared span. The fixture is quietly reading channels that belong to the next fixture along |

---

## 6. Sharing a Fixture

| Button | Effect |
| --- | --- |
| **Export JSON** | Writes the whole fixture to a single file you can send to someone else or keep as a backup before a risky edit |
| **Import JSON** | Reads one back. **It replaces the entire fixture**, not just part of it — export first if there is anything here you want to keep |

---

## 7. Troubleshooting

### No beam in the preview

Press **Compile** the first time you open it and after any change. The preview reads the compiled product.

### The fixture is dark on stage

1. Has the definition been compiled?
2. What beam kind is the emitter — `Spot Only` and `Wash` deliberately draw no aerial column;
3. Do the fixture's DMX mode, universe and start address in the scene match (see [DMX Fixture Basics](../stagecore/03_DMX_Actor_Base_en.md))?

### The console moves the fader and nothing happens

Check the Validation page for that attribute in the unbound list. Control channels (Reset, Lamp On) having no visual effect is normal.

### Pan / tilt travel is wrong

Look at that axis' **Range** on the Fixture page. It should hold the value declared in the GDTF package. Use the **Rig** aid on the DMX Test page to confirm the pivot position.

### A whole gobo or colour wheel is off by a slot

Check that the wheel's **slot count** in the optics group matches the actual number of cells in the atlas. Wrong count means the material slices the atlas at the wrong width and the whole wheel shifts.

---

## 8. Related Documents

- [Fixture System Overview](00_FixtureSystem_Overview_en.md)
- [Fixture Builder](06_FixtureBuilder_en.md) — a complete recipe per fixture type
- [The Fixture Definition Asset](01_FixtureDefinition_en.md)
- [GDTF Import](02_GdtfImport_en.md)
- [Channel Library Editor](../editortools/10_FixtureLibraryEditor_en.md)
