# GDTF Import

> Applies to SuperStage 26H2.6 and later ｜ Read first: [Fixture System Overview](00_FixtureSystem_Overview_en.md)

GDTF (General Device Type Format) is the standard format manufacturers publish fixture data in. One `.gdtf` package imports into a usable fixture: channels and modes, geometry tree and mechanical axes, 3D models, gobo / colour / prism wheels, and a thumbnail.

---

## 1. What an Import Produces

One package produces a set of assets per model:

Everything lands in one folder, `<OutputPath>/<Model>/`:

| Asset | Naming | Count |
| --- | --- | --- |
| Fixture definition | `<Model>`, **no prefix** | 1 |
| Channel library | `CL_<Model>_<ModeName>` | One per DMX mode |
| Prism preset | `SP_<Model>_Prism` | 1 when the fixture has a prism |
| Thumbnail | `LTC_<Model>` | 1 when the package carries one |
| Gobo wheel atlas | `LTA_...` | One per gobo wheel |
| Colour wheel atlas | `CLA_...` | One per colour wheel |
| Static mesh | `SM_...` | One per model in the package |

How `<Model>` is derived: with **Brand and Model** selected in batch import it is `<Brand>_<Model>`, except that a model name already carrying the brand (`ACME SANA PROFILE`) is not prefixed again. Asset names are normalised: non-ASCII characters, consecutive underscores, and leading/trailing underscores are stripped.

> When two packages in one batch resolve to the same name, the importer **flags the clash but does not rename anything** — what to rename it to is a human decision. Left alone, the second import would overwrite the first, and it looks like "I imported 12 and only got 11".

---

## 2. Single Import

Import onto a fixture definition asset, or create a definition in the Content Browser and import into it. Set the **import scope** first.

### 2.1 Import Scope (nine items)

Nine items in four groups. Whatever you untick is kept exactly as it is.

| Group | Option | What it covers |
| --- | --- | --- |
| Identity | Identity | Manufacturer name, model name |
| Channels | Channels | DMX modes, the channel library per mode, channel→capability bindings |
| Geometry | Meshes | Mesh assets, base / rigging parts, axis hierarchy and pivot offsets |
| Geometry | Emitter Placement | Which rig level an emitter hangs from, local offset, lens model and radius |
| Geometry | Physical | Body envelope and luminous flux |
| Optics | Gobo Atlases | Gobo wheel atlases |
| Optics | Color Atlases | Colour wheel atlases |
| Optics | Slot Colors | Writes the package's slot filter colours back into the channel library |
| Optics | Prisms | Prism preset, and the prism-layer selection written back into the slots |

Key points:

- **Channels cannot be split further.** Modes, channel libraries and bindings are produced in the same pass over the mode declaration; splitting them would yield a binding pointing at the old channel table, which is worse than not importing at all.
- **Channels is where the other items land.** Filter colours are written back to slots, and prism-layer selections are written back to slots. With Channels off, those land on the existing channel table — provided it really is the same fixture as this GDTF.
- **ZoomRange is never touched**; the import report says so.
- Untick Identity if you renamed the manufacturer or model; untick Meshes if you use hand-built art; untick Gobo Atlases if you assembled the gobo images by hand.

### 2.2 Bindings Are Derived Automatically

GDTF attribute names are standardised (`Pan` / `Tilt` / `Dimmer` / `Shutter1` / `ColorSub_C` / `Gobo1` / `Gobo1Pos` / `Prism1` / `Zoom` / `Focus1` …), so the importer maps them to capability opcodes and fills in the wheel index, component colour and similar parameters along the way. "Gobo1 is gobo wheel one" is something a machine can work out; nobody should have to wire twenty bindings by hand.

**Attributes that do not map are reported honestly as "unrecognised" rather than silently dropped** — dropping them quietly would leave you believing the import was complete.

> **Bindings can be derived without a GDTF.** The fixture editor's **Compile** button rebuilds every mode's bindings from the current channel library before compiling. A hand-written channel library, an MA2 import, or an attribute table edited in the editor all take this path — otherwise a hand-written library would have no bindings at all and the fixture would do nothing.

### 2.3 Import Report

Every import ends with a report listing all errors, warnings and notes from both the parse and import phases. Alongside the counts (modes, bindings, atlases, prism layers, meshes) it summarises these classes of problem for the whole fixture:

| Report item | Meaning |
| --- | --- |
| **Unmapped attributes** | The attribute name has no matching capability opcode; a binding has to be added by hand |
| **Coarse-only position attributes** | Pan / tilt are 8-bit only, so movement will step |
| **Wheel overflow** | Only three wheel slots exist; further attributes have nowhere to go |
| **Wheel conflicts** | Two bindings claim the same wheel index |
| **Declared colour hue mismatch** | The manufacturer linked a channel to an emitter whose hue does not match; the hard-coded base colour was kept |
| **Normalized colour temperature** | The channel declares a 0..1 correction rather than Kelvin; the endpoints were anchored to the fixture's native white point — otherwise "no correction" would evaluate as 2700K and read visibly orange |

Plus specific notes such as:

- A pixel group with no layout positions in the package — all instances will sit at the origin and the emitter's Span X/Y has to be set by hand;
- A group declared `BeamType=Glow/None` in the package, created as a glow emitter with no beam column (aura ring or backplate);
- Parts declared with `PrimitiveType` alone and no model file, generated as a placeholder body.

---

## 3. Batch Import

**Open**: SuperStage toolbar → **SuperDMXTool** → **GDTF Batch Import**

### 3.1 Flow

1. **Add GDTF...** or **Browse...** to pick a set of `.gdtf` packages. The list shows number, file, manufacturer, model and status;
2. Set the **Output folder** — the library folder the new fixtures should go into. It is used exactly as typed; each fixture gets its own sub-folder inside it, so adding a few fixtures to a brand folder you already have just works;
3. Choose how assets are named: **Brand_Model** or **Model only**;
4. Press **Create fixtures**. Progress is shown while it runs, and a summary appears at the end.

**Clear** empties the list without touching anything already created. Individual rows can be removed from the list by right-click.

### 3.2 Three Options

| Option | Description |
| --- | --- |
| **Compile after import** | Compile each definition into its runtime program and keep the result in the asset. Without it, every load recompiles from scratch. Leave it on unless you have a reason not to |
| **Copy GDTF into library** | Copy each package into `FixtureLibrary/GDTF/<brand>/` first, then import from there. **Leave this on when importing from a downloads folder** |
| **Save each fixture** | Write each fixture to disk as it is built. One fixture is a lot of files, so a few hundred left unsaved will bog the editor down |

> **Why Copy GDTF into library matters**: the importer records where the source GDTF lives. Importing straight out of a downloads folder records that folder — once it is cleaned out, "Upgrade from source GDTF" can no longer find the source, and MVR export reports the fixture as missing its package. The file name is never changed.

### 3.3 The Status Column

The last column of the list is Status. **Before** a run it flags one problem only:

| Status | Meaning | What to do |
| --- | --- | --- |
| **duplicate target** | Two packages in the list would produce the same asset name, and the second would overwrite the first | Switch to **Brand_Model** naming, or import them separately into different folders |

**After** the run it reports the outcome per package:

| Status | Meaning |
| --- | --- |
| **Created** | Created successfully |
| **Updated** | A fixture of that name already existed and was refreshed |
| **compile errors** | The fixture was made, but something in the package did not translate cleanly. It may still work — check it before relying on it |
| **Package failed** / **Import failed** | The package could not be read. Usually a damaged download; fetch it again |
| **Bad target path** | The output folder is not a valid library path |
| **save failed** | The fixture was built but could not be written to disk. Check the folder is not read-only |

### 3.4 After Importing

New fixtures appear in the Asset Browser, ready to drag into the scene. Place them, then patch them.

Not everything in a package can be translated automatically — a fixture may come in with some channels unrecognised. If something does not behave, open the fixture in the fixture editor and check its channels on the Validation page.

---

## 4. Upgrade In Place

Right-click a fixture definition → **Upgrade from source GDTF**.

**Why**: after a few hundred fixtures have been built, the importer grows new capabilities (emitter mounting orientation, ModeMaster conditional channels, placeholder bodies). Existing fixtures have none of those, and a re-import either cannot reach them (placement rides along with the channel pass) or costs too much (ModeMaster means rebuilding the channel table, which discards every hand-adjusted binding).

**It only adds**: whatever the source file has and the asset lacks is filled in; existing channel tables, bindings, wheels and hand-made models are never touched. **This is not a re-import.**

### 4.1 The Three Options and Their Risk

**Nothing marks a value as hand-made** — the code cannot tell "the importer wrote this offset" from "a person placed this offset". So every option that could overwrite an existing value is off by default.

| Option | Default | Risk |
| --- | --- | --- |
| **ModeMaster conditional channels** | **on** | **Safe.** Writes only the four new fields on existing sub-attributes; not another byte of the channel table. Those four fields were added later, so they are empty in every existing asset and cannot have been filled in by hand either (the condition lives on the GDTF ChannelFunction) — there is nothing to overwrite |
| **Emitter placement** | off | **Overwrites hand-placed emitters.** Rewrites Layout / Count X / Count Y / Span X / Span Y / Local Offset / Local Rotation / explicit per-instance transforms as one block — exactly the fields you touch when laying out a matrix fixture by hand. Matched by emitter name, updating only existing ones; **it never creates an emitter** |
| **Generate a body for fixtures that have none** | off | **Overwrites pivots.** Only affects definitions with no body mesh at all. The mesh itself only fills a blank, but **pivot offsets are written along with it** — a mesh origin has to sit on its pivot or the fixture mounts crooked. And a fixture with no body is exactly the kind most likely to have had its rig nudged by hand |
| **Report only — change nothing** | **on** | Runs the whole match, writes nothing, dirties nothing, and reports how much would change |

**Leave Report only on the first time.** If the numbers come back zero there is nothing to decide, and a library that is already shipped deserves a zero-risk way to ask "should I touch this at all?". With it ticked the button reads **Check**; untick it and it reads **Upgrade**.

Turn the other two on **fixture by fixture, after checking** — redoing hand work costs far more than skipping one piece of data.

### 4.2 How It Finds the Source GDTF

The source path is recorded on the **channel library** asset (one per mode), not on the definition; the first non-empty one is used.

What was written at import time is an absolute path, so it breaks when the machine changes or the library folders are reorganised. When it does not resolve, the file is **searched for by name recursively under the fixture library root** (library file names always keep the original download name, so the same name is the same package). A fixture where neither route finds anything is skipped and named in the report.

### 4.3 The Report, and Saving

The report gives: how many fixtures would change / did change, emitter placements rebuilt, ModeMaster conditions filled in, body meshes generated, and how many already had everything. It separately lists fixtures that **record no source GDTF or whose file is gone**, and fixtures that **failed to parse**.

**Changed assets are left dirty and are not saved** — review them, then save.

---

## 5. What the Import Currently Covers

| Content | Status |
| --- | --- |
| DMX modes and channels | Imported |
| Channel functions and channel sets (slots) | Imported |
| ModeMaster conditional channels | Supported; ranges grouped by condition |
| Geometry tree and mechanical axes | Imported, including the declared axis travel |
| 3D models | Imported, with `.3ds` and `.glb` fallbacks, normalised to the size declared in the package `<Model>` |
| Gobo / colour wheels | Atlases generated |
| Prism | Prism preset asset generated |
| Thumbnail | Imported as the fixture icon |
| Physical parameters | Body envelope and luminous flux imported |

Other notes:

- Zip entry names are decoded as UTF-8, so models with non-ASCII names are not lost;
- A few GDTF archives packed with high compression depend on system extraction tools (Windows only);
- The accuracy of library data is governed by what the manufacturer published. Different manufacturers' versions of the same fixture vary a lot — check the result in the fixture editor after import.

---

## 6. Three Things to Confirm After Import

### 6.1 Beam Kind

**A newly imported fixture gets Material Beam (Legacy) on its main emitter**; profile fixtures with blade channels get Shaper (Legacy). To use volumetric beams, change the emitter's beam kind to `Volumetric Beam` or `Volumetric Shaper (Profile)` in the fixture editor.

Matrix / pixel groups are unaffected: they are assigned Ray Beam according to the package BeamType, and groups declared as non-projecting (aura rings, backplates) are assigned Wash.

### 6.2 Unbound Channels

Check the fixture editor's **Validation** page. Control channels (Reset, Lamp On) having no visual effect is normal; a channel that clearly should do something appearing in the unbound list means the attribute name and binding rules did not line up, and a binding has to be added by hand.

### 6.3 Test Every Channel

Use the fixture editor's **DMX Test** page to push each channel and confirm pan/tilt travel, colour, gobos, prism and framing.

---

## 7. GDTF Export

Both the fixture editor and the channel library editor can export GDTF. Export is organised according to the GDTF 1.2 structure and restores mode and function names, so round-trips do not lose data.

Exported content is focused on DMX data; the `Wheels` and `PhysicalDescriptions` sub-nodes are empty placeholders with no wheel-image or physical-description data. Verify compatibility in the target software.

---

## 8. Related Documents

- [The Fixture Definition Asset](01_FixtureDefinition_en.md)
- [Fixture Editor](03_FixtureEditor_en.md)
- [Channel Library Editor](../editortools/10_FixtureLibraryEditor_en.md)
- [MVR Import / Export](../editortools/08_MVRImport_en.md)
