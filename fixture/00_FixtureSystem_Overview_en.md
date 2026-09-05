# Fixture System Overview

> Applies to SuperStage 26H2.6 and later

From 26H2.6 on, a fixture is no longer a Blueprint asset but a **fixture definition**. A definition can be imported, edited, and upgraded; every data-driven fixture in the scene shares one Actor class, and the model is determined by the definition it references.

This document explains which assets make up the system, how they relate, and which one you open for what.

---

## 1. Three Assets

| Asset | The question it answers | Asset name |
| --- | --- | --- |
| **Fixture Definition** (Super Fixture Definition) | What this fixture looks like, how it moves, where light comes out | **the model name, with no prefix** |
| **Channel Library** (Super Fixture Library) | Which channels one DMX mode of this fixture has | `CL_<Model>_<ModeName>` |
| **Prism Preset** (Super Prism Preset) | How the prism splits the beam and how the facets are arranged | `SP_<Model>_Prism` |

> One model occupies one folder, and the asset inside it that **shares the folder's name and carries no prefix** is the fixture definition. Everything prefixed alongside it is material the definition references: `LTC_` is the content-browser thumbnail, `LTA_` gobo wheel atlases, `CLA_` colour wheel atlases, `SM_` meshes.

How they relate:

```text
Fixture definition MegaPointe
 ├── Identity   manufacturer / model / type / thumbnail
 ├── Physical   power / weight / luminous flux / native white point / body size
 ├── Rig        base mesh + axis A (yoke) / axis B (head) / axis C (spin)
 ├── Emitters   where light exits, beam kind, lens, zoom range
 ├── Optics     gobo atlases x3 / colour atlases x3 / prism preset
 └── DMX modes
      ├── Mode "Standard" ──> channel library CL_MegaPointe_Standard
      │                        └── bindings: which channel drives which capability
      └── Mode "Extended" ──> channel library CL_MegaPointe_Extended
                               └── bindings
```

- One definition can have several DMX modes; each mode points at one channel library;
- One channel library can be shared by several definitions (common across a product series);
- Prism presets can be shared too.

**Emitters are a list, not one.** This is easy to miss:

| | What it covers |
| --- | --- |
| An emitter's **Layout** | Duplicating **one** emitter N times — a bar's 1×N, a panel's X×Y, pixel segments |
| The emitter **list** | Several **genuinely different** emitting bodies that exist on this model |

The second is real: some fixtures carry both a BeamMatrix and a PlateMatrix, with different segment counts, grids, mount points and DMX attributes; a beam fixture with an aura ring is the same situation. One emitter plus a layout cannot express that.

**The default mode is "the first mode with a valid channel library"**, not simply the first one — a mode with no channel library drives nothing and is skipped.

> Every heavy asset in a definition (meshes, textures, prism presets) is a **soft reference**; the channel library is a hard reference. That is how the asset browser lists hundreds of models without loading any of them — manufacturer, model, category and the thumbnail path are exposed to the asset registry as tags.

---

## 2. Definition vs Channel Library

This is the easiest pair to confuse.

| | Fixture definition | Channel library |
| --- | --- | --- |
| Describes | What the fixture **is** — body, mechanics, optics | How the fixture is **controlled** — the channel chart |
| Switching DMX mode | Definition unchanged | A different channel library |
| Switching to another fixture in the same series | A different definition | The channel library may be reusable as is |
| Edited in | The fixture editor's Fixture page | The channel library editor |

**Bindings live in the definition's mode, not in the channel library.** A binding records a relationship such as "the attribute named `Pan` in the channel library drives axis A's position". So the same channel library used by two fixtures with different mechanics can have different bindings on each.

---

## 3. Where Fixtures Come From

| Route | When | Document |
| --- | --- | --- |
| The shipped library | 807 models are already built — drag from the asset browser | [Asset Browser](../editortools/02_AssetBrowser_en.md) |
| Import one GDTF | The model is not in the shipped library | [GDTF Import](02_GdtfImport_en.md) |
| Batch import GDTF | Dozens or hundreds at once | [GDTF Import](02_GdtfImport_en.md) |
| From an MVR package | Someone sent you an MVR plot | [MVR Import / Export](../editortools/08_MVRImport_en.md) |
| Build by hand | No GDTF available; describing a fixture from scratch | [Fixture Builder](06_FixtureBuilder_en.md) |

---

## 4. Compiling

The fixture definition is **source data**; the runtime reads its **compiled product**. After editing a definition it has to be compiled once before fixtures on stage follow the change.

Compiling does several things: compiles the three axes into the product, expands the emitters into instances according to their layout, expands the bindings into runtime opcodes, works out the channel span, and runs validation. Results land on the fixture editor's **Validation** page.

### 4.1 Two Compile Entry Points, Two Behaviours

| Entry point | What it does |
| --- | --- |
| The **fixture editor's** Compile button | **Rebuilds every binding from the current channel library first, then compiles.** Both happen in one transaction, so a single undo rolls back both |
| **Content Browser right-click** → Compile (multi-select batch) | **Compiles only; it does not rebuild bindings.** Use it to sweep the whole library after changing the importer or the compiler |

After changing a channel library, use the **editor's** Compile — the batch right-click does not re-derive bindings, so it would compile the old ones.

### 4.2 The Compile Result Has to Be Saved

At runtime, a missing or stale product is **recompiled in place**, so fixtures do not go dark. But that result **is not written back** — every load pays for it again and it never converges.

**After compiling from either entry point the asset is left dirty but not saved; save it yourself**, and loads stop recompiling.

> If every fixture on stage is dark after a plugin upgrade, batch-compile the definitions and then save. 26H2.6 closes the path where an out-of-date product after a version bump was judged as "no rebuild needed": an incompatible product header is now treated as stale outright, which triggers the rebuild.

### 4.3 Duplicating a Definition Assigns a New Identity

Duplicating a definition asset in the Content Browser **automatically assigns a new GUID** — a duplicate is a new model. Without that, MVR export and the asset browser would merge the original and the copy into one model.

---

## 5. From Data to Picture

```text
Console / DMX network
      │  universe + start address
      ▼
Fixture Actor ──> channel library of the current DMX mode ──> read by channel number
      │
      │  binding: attribute name → capability (Op) + emitter index + resolution
      ▼
Capability evaluation ──> axes (pan / tilt / spin)
                     ├─> cone light (pool, gobo, colour wheel, framing, prism)
                     ├─> aerial column (volumetric / ray beam / wash / none)
                     └─> pixel matrix, effect plane
```

Which functions a fixture responds to depends on every link in that chain being present: the channel exists in the channel library, a binding connects it to a capability, and the emitter's beam kind supports that capability. Break any link and the console moves the fader with nothing happening on screen. For the troubleshooting order see the DMX Test page in [Fixture Editor](03_FixtureEditor_en.md).

---

## 6. Relationship to Old Blueprint Fixtures

Before 26H2.6 a fixture was a Blueprint asset. **Blueprint fixtures are deprecated in this release** and their usage is no longer documented.

- **Do not upgrade existing projects in place.** The fixture asset structure and naming changed substantially, and this release provides no class redirectors from old Blueprint fixtures to definition assets. Projects using the old Blueprint fixtures should stay on the old plugin.
- The only redirectors kept are the Cutting → Shaper set, covering component classes, material nodes, enum values, and Blueprint functions that current assets store by name.
- Older documentation centred on "fixture Blueprints" and "channel switches" applies only to the old plugin and is no longer provided here.

---

## 7. Documents in This Folder

| Document | Content |
| --- | --- |
| [01 Fixture Definition](01_FixtureDefinition_en.md) | Every group of fields: identity, physical, rig, emitters, optics, modes and bindings |
| [02 GDTF Import](02_GdtfImport_en.md) | Single import, batch import, import scope, upgrade in place |
| [03 Fixture Editor](03_FixtureEditor_en.md) | The four pages, and how to verify a change |
| [04 Fixture Motion](04_Motion_en.md) | Pan/tilt travel, the speed channel, continuous rotation, multi-head |
| [05 Attribute Names](05_AttributeNames_en.md) | The 119 DMX attribute names the current implementation recognises |
| [06 Fixture Builder](06_FixtureBuilder_en.md) | Building by hand: complete recipes for moving heads, profiles, wash, matrix, aura, multi-head and effect fixtures |

Related:

- [Channel Library Editor](../editortools/10_FixtureLibraryEditor_en.md)
- [Prism Preset Editor](../editortools/18_PrismPresetEditor_en.md)
- [DMX Fixture Basics](../stagecore/03_DMX_Actor_Base_en.md)
- [Light Components](../lightcomponent/00_LightComponent_Overview_en.md)
