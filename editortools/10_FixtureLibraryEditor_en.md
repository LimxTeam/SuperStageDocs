# SuperStage Fixture Library Editor User Manual

## 1. Scope

The Fixture Library Editor is used to view and edit `SuperFixtureLibrary` assets. A fixture library stores the fixture name, manufacturer, physical information, external library file paths, and DMX modules, attributes, sub-attributes, and channel sets.

The editor can import or export MA2 XML, GDTF, and SuperStage JSON files. Files from different manufacturers may contain different structures, so check modules, channels, and channel sets after importing.

## 2. Access

Double-click a `SuperFixtureLibrary` asset in the Unreal Engine Content Browser.

To create a new asset, use the Content Browser context menu and create a `SuperFixtureLibrary` asset.

## 3. Header Area

| Field | Description |
| --- | --- |
| Fixture Name | Fixture model name |
| Manufacturer | Manufacturer |
| Power (W) | Rated power, minimum 0 |
| Weight (kg) | Fixture weight, minimum 0 |
| Channel Count | Channel span calculated from current modules and attribute channels |
| GDTF Library | Linked GDTF library path |
| MA2 Library | **Deprecated, read-only in the panel.** From 26H2.6 the MA2 XML is converted from the bound GDTF on demand and no longer needs hand maintenance |

GDTF Library can only bind to files inside the SuperStage plugin `FixtureLibrary/GDTF` directory. (The fixture library directory was corrected from `GTDF` to `GDTF` in 26H2.6, and source paths recorded on existing assets were normalised to plugin-relative form.) When entered manually, the path is normalized to a plugin path. Paths outside the allowed directory are rejected with a message.

The `SuperFixtureLibrary` asset also contains Beam Angle, Beam Intensity, and Body Size fields. These values are part of fixture data and export workflows, but they may not all appear in the header editor.

## 4. Module List

The left module list shows the current library's DMX Modules. A module usually represents one DMX mode of the fixture.

Available operations:

| Operation | Description |
| --- | --- |
| Add Module | Adds a new module |
| Rename | Edits the module name directly |
| Move Up / Move Down | Changes module order |
| Copy | Copies the module and inserts it after the current module; the copied name gets `_Copy` |
| Delete | Deletes the module |

Selecting a module shows its Attribute list on the right.

## 5. Data Levels

Fixture library data has three levels:

| Level | Description |
| --- | --- |
| Attribute | DMX attribute, such as Dimmer, Pan, Tilt, or ColorWheel |
| Sub-Attribute | Functional range under an attribute |
| Channel Set | Specific slot under a sub-attribute, such as color, gobo, or prism selection |

Double-click an Attribute to enter Sub-Attributes. Double-click a Sub-Attribute to enter Channel Sets. Use the breadcrumb to return to upper levels.

## 6. Attribute List

| Column | Description |
| --- | --- |
| No. | Row number |
| Attrib | Attribute name |
| Category | Attribute category |
| Coarse | Coarse channel, range 1-512 |
| Fine | Fine channel, 0 means unused, range 0-512 |
| Ultra | Ultra-fine channel, 0 means unused, range 0-512 |
| Default | Default value, as a percentage, range 0-100 |
| Highlight | Highlight value, as a percentage, range 0-100 |

If duplicate attribute names exist in the same module, the editor shows a warning color.

## 7. Sub-Attribute List

| Column | Description |
| --- | --- |
| No. | Row number |
| DMX Start | Start DMX value, range 0-255 |
| DMX End | End DMX value, range 0-255 |
| Physical Range | Physical value range |
| Channel Sets | Number of channel sets under this sub-attribute |
| Strobe Mode | Shown for Dimmer or Strobe categories |
| Rotation Mode | Shown for Position category |

## 8. Channel Set List

| Column | Description |
| --- | --- |
| No. | Row number |
| Name | Channel set name |
| DMX Start | Start DMX value, range 0-255 |
| DMX End | End DMX value, range 0-255 |
| Physical Range | Physical value range |

Additional columns appear based on the current attribute category:

| Category | Extra columns |
| --- | --- |
| Gobo | Gobo Mode, Texture |
| Color | Color, Color Index |
| Prism | Prism Selection |

## 9. Toolbar

| Button | Description |
| --- | --- |
| Add | Adds an item at the current level |
| Delete | Deletes the selected item |
| Copy | Copies selected items at the current level |
| Paste | Pastes items to the end of the current level list |
| Move Up / Move Down | Moves the selected item at the current level |
| Export MA2 | Exports MA2 Fixture XML |
| Import MA2 | Imports MA2 Fixture XML into the current asset |
| Export GDTF | Exports a GDTF file or description XML |
| Import GDTF | Imports from GDTF, GTDF, or XML into the current asset |
| Import JSON | Imports a SuperStage JSON fixture library |
| Export JSON | Exports a SuperStage JSON fixture library |

Import operations modify the current fixture library asset. After importing external files, check Fixture Name, Manufacturer, Modules, channel numbers, and channel set ranges.

## 10. Saving and Impact

The editor marks the asset as modified after changes. Use the editor save command to save the asset.

Changing a fixture library affects fixtures that reference it. After changing Coarse, Fine, Ultra, or module structure, check the DMX behavior of related fixtures in the scene.

## 11. Notes

- Channel numbers start at 1. Fine and Ultra set to 0 mean unused.
- Channel Count is calculated from positive Coarse, Fine, and Ultra channels in the current module data.
- MA2, GDTF, or JSON import is not a compatibility promise for every third-party file. Manual review is required after import.
- External library paths record the linked file location; they do not automatically synchronize all external file content.
