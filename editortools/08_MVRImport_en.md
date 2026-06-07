# SuperStage MVR Import — User Manual

## 1. Overview

The MVR Import Tool (MVR Import) is used to import fixture layout data from MVR (My Virtual Rig) files. MVR is an open standard file format (.mvr) defined by the GDTF organization, widely used for data exchange between lighting design software. With this tool, you can import stage views exported from software like Vectorworks, Capture, WYSIWYG, etc., directly into the SuperStage scene.

The tool displays parsed results grouped by **fixture type**, and you specify a local Actor Class for each type before importing with one click.

---

## 2. Access

**Main Menu Path**: Toolbar **SuperStage** dropdown menu → **SuperDMXTool** → **MVRImport**

The panel opens with a minimum size of 900 × 500 pixels.

---

## 3. Interface Description

```
┌──────────────────────────────────────────────────────┐
│  MVR File: [<Choose .mvr>                  ] [Browse]│
│                                                      │
│  [Select All] [Select None]              [Import]    │
│                                                      │
│  ┌──────────────────────────────────────────────┐    │
│  │ Import │ Fixture Type   │ Count │ Actor Class │    │
│  │   ☑   │ Spot380        │  24   │ BP_Spot  ▼  │    │
│  │   ☑   │ WashLED600     │  16   │ BP_Wash  ▼  │    │
│  │   ☑   │ BeamMoving     │   8   │ (None)   ▼  │    │
│  └──────────────────────────────────────────────┘    │
└──────────────────────────────────────────────────────┘
```

---

## 4. Workflow

### Step 1: Select MVR File

Click the **"Browse"** button and select a `.mvr` file in the file dialog.

Supported file formats:
- `.mvr` — Standard MVR file (ZIP archive, containing `GeneralSceneDescription.xml`)
- `.xml` — Directly select the MVR XML description file

> **Auto-Parse**: After selecting a file, the tool **automatically** reads and parses the file content; no additional button click is needed. Parsed results are displayed grouped by fixture type in the list.

### Step 2: Specify Actor Class for Each Type

After parsing, each row in the list represents a fixture type. You need to select the corresponding SuperStage fixture Blueprint class (subclass of `ASuperDmxActorBase`) in the **Actor Class** column's dropdown menu for each type.

### Step 3: Select Types to Import

Use the **Import** checkbox in each row to select which fixture types to import. You can also use the toolbar's **Select All** / **Select None** buttons for batch operations.

### Step 4: Import

Click the **"Import"** button. The tool will:
1. Check whether all checked types have an Actor Class specified (an alert will appear if any are missing)
2. Create all fixture Actors within a UE Transaction
3. Set each fixture's position, rotation, and DMX properties
4. Display the results via a notification popup after import

---

## 5. Fixture List Fields

| Column | Width | Description |
|--------|-------|-------------|
| **Import** | 70px | Checkbox; whether to import this fixture type |
| **Fixture Type** | Flexible | The fixture type name as defined in the MVR file |
| **Count** | 80px | Number of fixtures of this type |
| **Actor Class** | 240px | Dropdown to select the local corresponding SuperStage fixture Blueprint class |

The list is sorted in alphabetical ascending order by fixture type name.

---

## 6. MVR File Parsing

### 6.1 Parsing Flow

1. If the file is `.mvr` (ZIP format), the tool extracts `GeneralSceneDescription.xml` from the archive
2. If the file is `.xml`, the tool reads the XML content directly
3. Parse `<Fixture>` nodes in the XML, extracting type names, positions, rotations, and DMX addresses
4. Group and count by fixture type

### 6.2 ZIP Extraction Strategy

The tool supports multiple extraction methods to ensure compatibility:
- **Preferred**: In-memory direct decompression (supports Stored and Deflate compression)
- **Fallback**: Use system tools (calls PowerShell `Expand-Archive` or `tar.exe` on Windows)

### 6.3 Encoding Support

XML files support the following encodings: UTF-8, UTF-16 LE, UTF-16 BE.

---

## 7. DMX Address Parsing

DMX address information in MVR files supports multiple formats; the tool automatically recognizes them:

| Format | Example | Description |
|--------|---------|-------------|
| **Break Attribute** | `DMXBreakOverride` | Read from XML node attributes |
| **Absolute Address** | `1025` | Automatically calculated as Universe 3, Address 1 |
| **U.A Format** | `1.001` | Universe 1, Address 1 |

---

## 8. Post-Import Operations

After fixtures are imported into the scene:
- Fixtures are placed at the 3D positions recorded in the MVR file (with automatic coordinate conversion)
- Fixtures use the rotation information from the MVR file
- DMX properties (Universe, StartAddress, FixtureID) are automatically set
- Fixtures use the MVR label as the Actor Label

You can further adjust fixture configuration after import using SuperStage's other tools (Batch Patch, Patch Preview, etc.).

---

## 9. Supported MVR Versions

| Version | Support Status |
|---------|----------------|
| MVR 1.0 | Fully supported |
| MVR 1.4 | Fully supported |
| MVR 1.5 | Fully supported |
| MVR 1.6 | Fully supported |

---

## 10. Notes

- After selecting a file, parsing happens automatically; no manual trigger is needed
- Each fixture type **must** have an Actor Class specified to import; otherwise an error prompt will appear
- Both `.mvr` (ZIP) and `.xml` (direct XML) file formats are supported
- GDTF fixture description files in MVR are not automatically imported into the SuperStage fixture library
- Large MVR files (containing hundreds of fixtures) may take a few seconds to parse
- Import operations support **Ctrl+Z undo** (executed within a UE Transaction)

---

## 11. FAQ

| Issue | Solution |
|-------|----------|
| List is empty after parsing | Confirm that the MVR file actually contains fixture data |
| Actor Class dropdown is empty | Confirm the project has Blueprint classes inheriting from `ASuperDmxActorBase` |
| Clicking Import shows an alert | All checked types must have an Actor Class specified |
| All fixtures at origin position | The MVR file may not contain position information |
| Wrong fixture orientation | Different software may use different coordinate systems; manually adjust rotation after import |
| Cannot open .mvr file | Confirm the file is not corrupted, or try extracting and selecting the internal .xml file |
