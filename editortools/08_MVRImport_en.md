# SuperStage MVR Import / Export User Guide

## 1. Scope

MVR (My Virtual Rig) is the standard format that lighting design tools, visualisers and consoles use to exchange a complete plot. This panel implements reading and writing to the **MVR 1.6** specification:

- **Import** — read a `.mvr` package, parse the full scene description (layers, groups, fixtures, trusses, supports, scenery, video screens, projectors, positions, classes, focus points, mapping definitions) and place fixtures and scenery in the current level.
- **Export** — write the current level back out as a spec-conformant `.mvr`, packing the GDTF of every fixture type into the archive.

GDTF files inside the package are no longer ignored: a fixture type that is not in the library yet can be created directly from the GDTF that travelled with the MVR.

Open it from the main menu: **SuperStage** → **SuperDMXTool** → **MVR**. The panel has **Import** and **Export** tabs.

---

## 2. Import

### 2.1 Panel

| Area | Purpose |
| --- | --- |
| MVR File / Browse | Pick a `.mvr` (an already-extracted `.xml` also works, which is handy when chasing interop problems) |
| Options | See 2.2 |
| Summary line | File version, provider, layer count, per-type object counts |
| Select All / None | Bulk-select fixture types |
| Re-match | Match every type against the fixture library again (useful after importing new definitions) |
| Import | Spawn the actors |
| List | Fixtures grouped by GDTF type and DMX mode, each given a target in the **Target Fixture** column |

The list groups by **(GDTFSpec, GDTFMode)** rather than by fixture instance name. The 16CH and 32CH modes of the same fixture are two different channel layouts and need separate targets; grouping them together would patch half the rig with the wrong channels.

### 2.2 Options

| Option | Description |
| --- | --- |
| Create missing fixtures from packaged GDTF | Create a fixture definition asset from the packaged GDTF when the type is not in the library (on by default). MVR requires every fixture's GDTF to travel inside the package, so this almost always works |
| Layers to outliner folders | Recreate layers and groups as World Outliner folders (on by default). On export the first folder segment becomes a layer again and deeper segments become nested groups |
| Keep MVR metadata | Attach a `Super MVR Metadata` component holding position, class, focus point, console IDs, colour, protocols, network addresses and everything else Unreal has no concept for (on by default). **Turn it off and that information is gone when you export back to MVR** |
| Import trusses / supports / scenery | Build static meshes from the `.glb` / `.3ds` geometry in the package, or from the model inside a referenced GDTF, and place them (on by default) |
| DMX address | Address interpretation, see 2.4 |

### 2.3 Choosing a target

Every group needs a target in the **Target Fixture** column. The drop-down offers three sources:

1. **From packaged GDTF** — create a fixture definition asset from the GDTF this MVR carries.
2. **Fixture library** — pick an existing type through manufacturer → model → DMX mode. Mode names are only loaded when the third level is actually opened, so a library with thousands of types stays responsive.
3. **Legacy actor classes** — native fixture classes from SuperAssets (the 16 in-house fixtures and the machinery), plus any Blueprint fixture classes the project itself defines.

Opening a file runs an **automatic match**: GDTF file names follow the `Manufacturer@Model@Revision` convention and fixture definitions carry exactly the same pair in their Identity, so splitting on `@` gives an exact match. A model-only match is accepted when that model name is unique across the whole library; when several manufacturers share a model name ("Beam 200" and friends) the target is left empty for a human to pick rather than guessed.

New definition assets are created under `/Game/SuperStage/FixtureLibrary/<Manufacturer>/<Model>/` and **must be saved from the Content Browser afterwards** (the report reminds you).

### 2.4 DMX addresses

The MVR `<Address>` value is an **absolute, cross-universe address** (universe 4 channel 100 = 3×512+100) and the `break` attribute is the GDTF DMX break, unrelated to universes. This is the default reading.

A few older files instead treat `break` as a universe index and write the channel within that universe. Re-import with **Legacy (break = universe - 1)** in that case. The parser flags the suspicious combination — every address inside one universe while several different break values are in use — in the report.

The `"universe.channel"` string form is understood in both modes.

Multi-break fixtures (body and pixels patched separately) can only carry their first address in Unreal; the remaining breaks are preserved in the metadata component and written back out on export. The report names every affected fixture.

### 2.5 What import produces

| Data | Source |
| --- | --- |
| Location / rotation / scale | `<Matrix>`, converted through millimetres→centimetres, right→left handed and the fixture orientation flip |
| Universe / StartAddress | The primary address (lowest break) |
| FixtureID | `<FixtureIDNumeric>` → `<FixtureID>` text → `<UnitNumber>`, in that order |
| Actor Label | The `name` attribute; model plus ID when absent |
| Outliner folder | Layer and group names |
| MVR metadata component | Position, class, focus point location, IDs, colour, gobo, protocols, network addresses, mappings, connections, alignments, overwrites, custom commands |

The whole import runs inside one Unreal transaction, so Ctrl+Z undoes it. **Newly created definition assets are outside the transaction** (asset creation is not part of the level undo stack) — undo removes the fixtures, the assets stay.

A report opens when import finishes, listing every error, warning and note from both the parse and the spawn stage. The same lines go to `LogMvrImport`.

---

## 3. Export

Switch to the **Export** tab, set the options below, choose an output path, and click **Export MVR**.

### 3.1 Options

| Option | Description |
| --- | --- |
| Pack GDTF files | Put each type's `.gdtf` into the archive (on by default). The spec requires it — without it the receiving application knows the type name but has no channel layout |
| Outliner folders as layers | First folder segment → layer, deeper segments → nested groups (on by default). Turn off to put every fixture into a single layer |
| Selected actors only | Export only the fixtures currently selected in the level |
| Write MVR metadata back | Restore the position, class, focus point, IDs, colour and protocols captured on import (on by default) |
| Compress scene description | Deflate the scene description (on by default). A large plot compresses roughly ten to one; `.gdtf` files are already archives and are always stored uncompressed |

### 3.2 Archive contents

```
GeneralSceneDescription.xml     Full scene, child order following the MVR 1.6 xs:sequence exactly
Manufacturer@Model@Rev.gdtf     Each type's original GDTF, keeping its source file name
```

What gets written: layers and groups; fixtures (matrix, GDTFSpec/GDTFMode, Focus, CastShadow, DMXInvertPan/Tilt, Position, Function, FixtureID/FixtureIDNumeric/UnitNumber, ChildPosition, Addresses plus Network, Protocols, Alignments, CustomCommands, Overwrites, Connections, Color, CustomId, Mappings, Gobo); positions, classes and mapping definitions in AUXData; and the focus points that are referenced.

`<GDTFMode>` comes from the data-driven fixture's **ActiveMode**, not from the channel library's module name — the latter is a pixel-group name, and writing it would patch the fixture with a completely wrong channel layout.

### 3.3 Checked before and after writing

Export does not stop at "the file was written":

- **Before writing**, the hard requirements of the spec are checked one by one: canonical and unique uuids, positive and globally unique FixtureIDNumeric, UnitNumber, no duplicate breaks on one fixture, valid GDTF file names, every Position / Class / Focus / MappingDefinition reference actually defined, multipatch parents present. Any failure aborts the export with the reason.
- **After writing**, the file is reopened, re-parsed and the fixture count compared. This catches the class of error where we write something we cannot read back ourselves — downstream that only ever shows up as "the console will not open this file".

Both results appear in the export report.

---

## 4. When Something Looks Wrong

**Fixtures are in the wrong universe**
Switch **DMX address** to **Legacy** and import again.

**A type shows "No fixture / no GDTF"**
The library has no match and the package carried no profile for it. Pick a target by hand, or import the GDTF into the fixture library first.

**Export says "Missing GDTF"**
The fixture's channel library has no profile file on disk. Re-import that fixture from its GDTF so the path is recorded.

**Trusses did not appear**
Only `.glb` and `.3ds` geometry can be read. Profiles that declare a primitive placeholder instead of a real model produce no mesh - the report says which ones.

> Every run also writes its full report to the output log: `LogMvrImport` for import, `LogMvrExport` for export.

---

## 5. Interoperability notes

- The archive is written per PKWARE 6.3.3 using only STORE and DEFLATE, without encryption, with every file at the archive root (the spec requires a flat package). The reader additionally tolerates third-party packages with sub-directories and says so in the report.
- The reader is deliberately forgiving: tag names are case-insensitive, uuids are accepted in upper case, in braces or without hyphens, objects found directly under `<Layers>` are collected into a synthetic layer, a `Geometry3D` `fileName` without an extension is treated as `.3ds`, and both the `ScaleHandeling` and `ScaleHandling` spellings are read.
- The writer sticks to the spec: nodes that are not in the specification's child tables are never written (`FixtureTypeId`, for example, is read but not written back), and mandatory nodes are always written even when the value is 0 or empty.
- Scene descriptions in UTF-8, UTF-8 BOM, UTF-16 LE and UTF-16 BE are all read, as is XML minified onto a single line.

## 6. What this version does not do

- **Scenery is not exported.** Unreal has no path here for writing static meshes back out as `.3ds` / `.glb`, so an exported package contains fixtures only — trusses and props are not included.
- **MVR-xchange is not implemented.** That is a separate part of the MVR ecosystem (an mDNS discovery plus TCP/WebSocket live-sync protocol), distinct from the file format this panel reads and writes.
- **Third-party UserData is not passed through.** Private data blocks other applications write under the root `<UserData>` are read, but they are not stored with the level and therefore cannot be handed back on export. Export writes its own provenance block (project name, engine version, fixture count, export time).
- Scenery whose GDTF only declares a `PrimitiveType` placeholder and carries no model file produces no mesh; the report explains why.
