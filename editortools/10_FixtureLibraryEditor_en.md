# SuperStage Fixture Library Editor — User Manual

## 1. Overview

The Fixture Library Editor is SuperStage's fixture library management tool. Each SuperStage fixture depends on a fixture library asset (SuperFixtureLibrary) that defines its DMX channel layout, attribute types, color wheels, gobo wheels, and other detailed parameters. With the Fixture Library Editor, you can create and modify these library definitions without manually editing data files.

---

## 2. Access

In Unreal Engine's **Content Browser**, double-click a `SuperFixtureLibrary` asset to open the Fixture Library Editor.

> **Tip**: If you don't have a fixture library asset yet, right-click in the Content Browser → Miscellaneous → SuperFixtureLibrary to create a new one.

---

## 3. Interface Layout

```
┌──────────────────────────────────────────────────────┐
│  Header Info Bar                                     │
│  Fixture Name | Manufacturer | Source | Power(W) | Weight(kg) │
├──────────┬───────────────────────────────────────────┤
│          │  Breadcrumb Navigation                    │
│ Module   │  [ModuleName] > [AttrName] > [SubAttrName]│
│ List     │─────────────────────────────────────────── │
│          │  Toolbar                                  │
│ [Module1]│  [+Add] [Copy] [Paste] [▲Up] [▼Down] [✕Del]│
│ [Module2]│─────────────────────────────────────────── │
│ [Module3]│  Content Area (Attribute/SubAttr/Slot List)│
│          │  ┌──────────────────────────────────────┐ │
│          │  │ No. | Name   | Category | Coarse | ...│ │
│          │  │ 1   | Dimmer | Dimmer   | 1      | ...│ │
│          │  │ 2   | Pan    | Position | 3      | ...│ │
│          │  │ 3   | Tilt   | Position | 5      | ...│ │
│          │  │ ...                                  │ │
│          │  └──────────────────────────────────────┘ │
└──────────┴───────────────────────────────────────────┘
```

---

## 4. Header Info Bar

Displays and allows editing of the fixture's basic information. All fields can be edited by clicking directly.

| Field | Description | Example |
|-------|-------------|---------|
| **Fixture Name** | The fixture's model name | XP-380 Beam |
| **Manufacturer** | The fixture manufacturer name | Acme |
| **Source** | Source of the library data | Custom / GDTF / MA2 / MA3 |
| **Power (W)** | Fixture rated power, in watts | 380 |
| **Weight (kg)** | Fixture weight, in kilograms | 18.5 |
| **Channel Span** | Total number of DMX channels the fixture occupies (auto-calculated) | 24 |

---

## 5. Module List (Left Panel)

A fixture can contain one or more **DMX Modules**. Each module represents a working mode of the fixture (e.g., Standard mode, Extended mode).

### 5.1 Module Information

| Field | Description | Example |
|-------|-------------|---------|
| **Module Name** | The module's name, editable | Standard / Extended / Basic |
| **Patch Value** | Total channel count for this module (auto-calculated or manually set) | 24 |

### 5.2 Module Operations

| Operation | Method |
|-----------|--------|
| **Add Module** | Click the "+" button below the module list |
| **Delete Module** | Select a module and click the "✕" button |
| **Rename Module** | Click the module name directly to edit |
| **Reorder** | Use the "▲" "▼" buttons to move up/down |

> **Tip**: Most fixtures only need one module. Only create multiple modules when the fixture has multiple DMX channel modes.

---

## 6. Data Hierarchy

The Fixture Library Editor uses a three-level hierarchy:

```
Module
 └── Attribute           — e.g., Dimmer, Pan, Tilt, ColorWheel
      └── SubAttribute   — e.g., individual color ranges under ColorWheel
           └── ChannelSet — e.g., specific DMX value range for a color
```

Navigate between levels using the **breadcrumb navigation**. Click a level name in the breadcrumb to return to the previous level.

---

## 7. Attribute List (Level 1)

After selecting a module, the right content area displays all DMX attributes for that module.

### 7.1 Attribute List Fields

| Column | Description | Editable |
|--------|-------------|----------|
| **No.** | Attribute sequence number (auto-numbered) | No |
| **Attribute Name** | The name of the attribute | Yes |
| **Category** | The functional category this attribute belongs to | Yes (dropdown) |
| **Coarse** | Coarse DMX channel number | Yes |
| **Fine** | Fine DMX channel number (for 16-bit) | Yes |
| **Ultra** | Ultra-fine DMX channel number (for 24-bit) | Yes |
| **Default** | Default DMX value when the fixture starts | Yes |
| **Highlight** | DMX value in highlight mode | Yes |

### 7.2 Attribute Category Description

| Category | Description | Typical Attributes |
|----------|-------------|-------------------|
| **Dimmer** | Light output intensity control | Dimmer, DimmerFine |
| **Position** | Fixture movement control | Pan, Tilt, PanFine, TiltFine |
| **Color** | Color control | ColorWheel, Red, Green, Blue, White, CTO |
| **Beam** | Beam shape control | Zoom, Focus, Iris |
| **Gobo** | Gobo wheel control | GoboWheel1, GoboWheel1Rotation |
| **Prism** | Prism effect control | Prism, PrismRotation |
| **Strobe** | Strobe effect control | Shutter, Strobe |
| **Control** | Fixture system control | Reset, LampOn, LampOff |
| **Effect** | Special effects | Effect, Frost, Animation |
| **Maintenance** | Maintenance functions | DisplayControl, FanSpeed |
| **Media** | Media playback related | MediaFolder, MediaFile |
| **Video** | Video input related | VideoInput, Resolution |
| **Other** | Uncategorized | Custom attributes |

### 7.3 Entering SubAttributes

**Double-click** an attribute row to enter that attribute's SubAttribute list.

---

## 8. SubAttribute List (Level 2)

Each attribute can contain multiple SubAttributes, used to subdivide the attribute's behavior across different DMX value ranges.

### 8.1 SubAttribute List Fields

| Column | Description | Editable |
|--------|-------------|----------|
| **No.** | Sequence number | No |
| **Name** | SubAttribute name | Yes |
| **DMX Start** | Starting DMX value for this SubAttribute | Yes |
| **DMX End** | Ending DMX value for this SubAttribute | Yes |
| **Physical Start** | Corresponding physical value start (e.g., angle, percentage) | Yes |
| **Physical End** | Corresponding physical value end | Yes |
| **Slot Count** | Number of ChannelSets defined under this SubAttribute | No (read-only) |

### 8.2 Extra Columns for Special Categories

Depending on the current attribute's category, the SubAttribute list dynamically shows additional columns:

| Attribute Category | Extra Column | Description |
|--------------------|-------------|-------------|
| **Dimmer / Strobe** | Strobe Mode | Set the strobe effect mode type |
| **Position** | Rotation Mode | Set the rotation behavior mode |

### 8.3 Entering ChannelSets

**Double-click** a SubAttribute row to enter that SubAttribute's ChannelSet list.

---

## 9. ChannelSet List (Level 3)

ChannelSets define the mapping of more granular DMX values to functions within a SubAttribute. For example, each color in a color wheel attribute is a ChannelSet.

### 9.1 ChannelSet List Fields

| Column | Description | Editable |
|--------|-------------|----------|
| **No.** | Sequence number | No |
| **Name** | ChannelSet name (e.g., "Red", "Open") | Yes |
| **DMX Start** | Starting DMX value for this ChannelSet | Yes |
| **DMX End** | Ending DMX value for this ChannelSet | Yes |
| **Physical Range** | Corresponding physical quantity value | Yes |

### 9.2 Extra Columns for Special Categories

Depending on the current attribute's category, the ChannelSet list dynamically shows additional columns:

| Attribute Category | Extra Column | Description |
|--------------------|-------------|-------------|
| **Gobo** | Gobo Mode | Rotation or fixed mode selection for the gobo |
| **Gobo** | Texture | Related gobo texture asset (select an image file) |
| **Color** | Color | The color value this ChannelSet represents (color picker) |
| **Color** | Color Index | The color's index number in the color wheel |
| **Prism** | Prism Facets | Number of prism facets (e.g., 3-facet prism, 8-facet prism) |
| **Prism** | Prism Radius | Spread radius of the prism effect |
| **Prism** | Prism Scale | Scale factor of the prism effect |

---

## 10. General Toolbar Operations

The toolbar above the content area provides the following operations, applicable to any currently displayed level (Attribute / SubAttribute / ChannelSet):

| Button | Function | Quick Description |
|--------|----------|-------------------|
| **+ Add** | Add a new empty item at the end of the list | New item uses default values |
| **Copy** | Copy selected items to the clipboard | Supports multi-select |
| **Paste** | Paste items from the clipboard to the end of the list | Pasted items are independent copies |
| **▲ Move Up** | Move selected items up one row | Changes order |
| **▼ Move Down** | Move selected items down one row | Changes order |
| **✕ Delete** | Delete selected items | Supports multi-select batch deletion |

### 10.1 Multi-Select Operations

- **Ctrl + Click**: Add/remove individual selections
- **Shift + Click**: Select all items between two clicks

---

## 11. Breadcrumb Navigation

The breadcrumb bar shows the current level path, for example:

```
Standard > Pan > Pan Coarse
```

- Click **"Standard"** → Return to the Attribute list
- Click **"Pan"** → Return to Pan's SubAttribute list
- **"Pan Coarse"** → Currently viewing the ChannelSet list

---

## 12. Usage Examples

### Example 1: Create a Simple Dimmer Attribute

1. Select the target module in the module list
2. Click **"+ Add"** in the attribute list
3. Set the attribute name to `Dimmer`
4. Set Category to **"Dimmer"**
5. Set Coarse channel to `1`
6. Set Default value to `0`
7. Set Highlight value to `255`

### Example 2: Configure a Color Wheel

1. Create an attribute named `ColorWheel`, Category **"Color"**, Coarse channel `7`
2. Double-click to enter the SubAttribute level
3. Add a SubAttribute named `Colors`, DMX range `0-127`
4. Double-click to enter the ChannelSet level
5. Add color ChannelSets one by one:
   - `Open` — DMX 0-9 — Color white
   - `Red` — DMX 10-19 — Color red
   - `Blue` — DMX 20-29 — Color blue
   - `Green` — DMX 30-39 — Color green
   - ...

### Example 3: Configure a Gobo Wheel

1. Create attribute `GoboWheel1`, Category **"Gobo"**, Coarse `9`
2. Enter SubAttributes → Add `Fixed Gobos`, DMX 0-63
3. Enter ChannelSets → Add gobos one by one:
   - `Open` — DMX 0-7
   - `Star` — DMX 8-15, select star pattern texture file
   - `Circle` — DMX 16-23, select circle pattern texture file
   - ...

---

## 13. Duplicate Name Detection

The editor automatically detects duplicate attribute names within the same module. If duplicates are found, the attribute name is displayed in a warning color to alert you to make changes.

---

## 14. Saving

All modifications to the fixture library automatically mark the asset as "modified" (a * is displayed next to the asset name). Use **Ctrl+S** or click the editor's save button to save changes.

---

## 15. Notes

- Modifying the fixture library affects all fixture instances that use it
- Changing channel assignments may alter the DMX behavior of existing fixtures
- It's recommended to back up the fixture library asset before making changes
- Channel numbering starts at 1, maximum 512
- An attribute can have no fine or ultra-fine channels (leave as 0 to disable)
- Extra fields like color and texture only appear under attributes of the corresponding category
