# SuperConsolePro Lighting Console — User Manual (2)

## Fixture Patching · Fixture Selection · Fixture Groups · Fixture Sheet

---

# Chapter 1 Fixture Patching (Patch)

## 1.1 What Is Patching

"Patching" is the process of **importing** SuperDMX fixtures placed in the scene **into the console**. Only patched fixtures can be selected, programmed, and controlled by the console.

The patching process assigns the following information to each fixture:

| Information Item | Description |
|------------------|-------------|
| **Fixture ID** | Fixture number, a unique identifier within the console. E.g., `1`, `2`, `101` |
| **Fixture Name** | The display name of the fixture, typically auto-read from the Actor name in the scene |
| **Fixture Type** | The fixture model (e.g., "Wash 600", "Spot 400 Pro", etc.) |
| **Universe** | DMX universe number (1-64), indicating which DMX bus the fixture is on |
| **Start Channel** | The starting DMX channel of the fixture within its Universe (1-512) |
| **Channel Span** | The number of DMX channels the fixture occupies |
| **Module Count** | The number of sub-fixtures for a matrix fixture. Single fixtures have a count of 1 |

## 1.2 Opening the Patch Panel

There are two ways to enter the Patch panel:

1. **Settings Panel** → switch to the **Patch** tab
2. Add a **Patch** panel directly in the Grid Window Manager

## 1.3 Scanning the Scene

### Operation Steps

1. Open the Patch panel
2. Click the **Scan Scene** button
3. The system will automatically search for all fixture Actors in the current level that inherit from `ASuperDmxActorBase`
4. After the scan completes, the **Unpatched Fixtures** list displays all discovered fixtures

### Scan Results List

The scan results are divided into two areas:

- **Unpatched Fixtures**: Fixtures present in the scene but not yet imported into the console
- **Patched Fixtures**: Fixtures already imported into the console

Each fixture entry displays:
- Fixture name (Actor name in the scene)
- Fixture type
- Universe / Start Channel
- Module count

## 1.4 Importing Fixtures

### Import Selected Fixtures

1. In the **Unpatched Fixtures** list, check the fixtures you want to import
2. Click the **Import Selected** button
3. The system automatically assigns Fixture IDs to the fixtures
4. The fixtures appear in the **Patched Fixtures** list

### Import All Fixtures

1. Click the **Import All** button
2. All unpatched fixtures are imported into the console at once

### Fixture ID Assignment Rules

- The system automatically assigns consecutive numbers starting from **1**
- If existing fixtures occupy certain numbers, new fixtures automatically skip the occupied numbers
- After import, Fixture IDs can be manually modified

## 1.5 Removing Fixtures

### Remove Selected Fixtures

1. In the **Patched Fixtures** list, check the fixtures you want to remove
2. Click the **Remove Selected** button
3. The system displays a confirmation dialog
4. After confirming, the fixtures are removed from the console

> **Note**: Removing fixtures does not delete the Actors from the scene; it only removes them from the console's patch list.

### Remove All Fixtures

1. Click the **Remove All** button
2. After confirming, all patch data is cleared

> **Warning**: Removing fixtures also clears all Programmer data, CUE data, and Preset data associated with those fixtures. This operation is irreversible.

## 1.6 Scene Synchronization

When fixtures in the scene change (added, deleted, attribute modified), the patch data needs to be updated:

### Automatic Synchronization

The system automatically detects scene changes at the following times:
- When the Patch panel is opened
- When the **Refresh** button is manually clicked

### Sync Status Indicators

| Status | Color | Description |
|--------|-------|-------------|
| **Synced** | Green | Fixture data matches the scene |
| **Changed** | Yellow | Fixture attributes have changed and need re-syncing |
| **Lost** | Red | The fixture cannot be found in the scene (Actor has been deleted) |

### Handling Changed Fixtures

1. In the patched fixtures list, fixtures marked in yellow indicate their scene attributes have changed
2. Click the **Sync Changes** button
3. The console automatically updates the patch data

### Handling Lost Fixtures

1. Fixtures marked in red indicate the corresponding Actor can no longer be found in the scene
2. You can choose to **Keep** (maintain patch data but no longer output DMX)
3. Or you can choose to **Remove** (delete from the patch list)

## 1.7 Editing Patch Information

Double-click a field of a patched fixture to manually edit:

| Field | Editable | Description |
|-------|----------|-------------|
| Fixture ID | Yes | Enter a new number. Must not conflict with existing numbers |
| Name | Yes | Modify the fixture display name |
| Universe | Read-only | Determined by the Actor settings in the scene |
| Start Channel | Read-only | Determined by the Actor settings in the scene |
| Fixture Type | Read-only | Determined by the fixture asset used in the scene |

## 1.8 Search and Filter

The Patch panel provides the following filtering features:

- **Search Box**: Enter keywords to filter fixture names or types
- **Status Filter Tags**: Multiple filter tags are provided at the top of the panel; click to quickly filter the fixture list
- **Sorting**: Click column headers to sort by Fixture ID, Name, Type, Universe, etc.

### Filter Tags

| Tag | Color | Description |
|-----|-------|-------------|
| **All** | White | Show all fixtures (default) |
| **Imported** | Green | Show only fixtures successfully imported into the console |
| **Available** | Gray | Show only scene fixtures not yet imported |
| **Pending Sync** | Yellow | Show only fixtures whose attributes have changed and need re-syncing. Only shown when there are fixtures pending sync |
| **Lost** | Red | Show only fixtures whose corresponding Actors can no longer be found in the scene. Only shown when there are lost fixtures |

### Fixture Status Icons

Each fixture entry has a status icon on the left that intuitively reflects its current state:

| Icon | Color | Meaning |
|------|-------|---------|
| **✓** | Green | Imported and normal |
| **⟳** | Yellow | Imported but needs syncing (scene attributes have changed) |
| **✕** | Red | Imported but Actor is lost |
| **○** | Gray | Not imported |

## 1.9 Patch Statistics

The bottom of the panel displays patch statistics:

- **Total patched fixture count**
- **Number of Universes occupied**
- **Total DMX channels occupied**

---

# Chapter 2 Fixture Selection

## 2.1 Basic Selection Operations

### Single Select

- In the Fixture Sheet, **click** a fixture row → selects that fixture
- All other fixtures are deselected

### Multi-Select (Add)

- Hold **Ctrl** + **click** a fixture → adds that fixture to the current selection
- Already selected fixtures remain selected

### Range Selection

- First select a fixture
- Hold **Shift** + **click** another fixture
- All fixtures between the two (inclusive) are selected

### Toggle Selection

- Hold **Ctrl** + **click** an already selected fixture → deselects that fixture

### Select All

- Press **Ctrl + A** → selects all patched fixtures

### Clear Selection

- Press **Esc** key → deselects all fixtures

### Invert Selection

- Press **Ctrl + I** → selects currently unselected fixtures and deselects currently selected ones

> **Note**: The invert selection feature is planned for a future version and is not yet available in the current version.

## 2.2 Sub-Fixture Selection (Matrix Fixtures)

For matrix fixtures (fixtures containing multiple sub-fixtures/module instances), the selection behavior is as follows:

- **Select master fixture** → simultaneously selects all sub-fixtures of that fixture
- **Expand master fixture row** → shows individual sub-fixture rows
- **Select individual sub-fixture** → selects only a specific sub-fixture instance

> **Tip**: When adjusting attributes in the encoder, if only some sub-fixtures are selected, only those selected sub-fixtures will be modified.

## 2.3 Selection via Fixture Groups

In the Groups panel:
- **Click a group button** → selects all fixtures in that group (replaces the current selection)
- **Ctrl + click a group button** → adds the group's fixtures to the current selection

## 2.4 Selection via Layout View

In the Layout View panel:
- **Click a fixture icon** → selects that fixture
- **Box select** → drag a rectangle to select multiple fixtures
- **Ctrl + click** → adds to the selection

## 2.5 Selection Order

The **order** in which fixtures are selected is very important! The selection order affects the following features:

- **Spread Mode**: Encoder spread values are distributed by selection order
- **Frame Effect Phase**: Phase angles are allocated by selection order
- **Time Distribution**: Fade/Delay times are calculated by selection order

For example, if you select fixture 1 first, then fixture 5, and finally fixture 3:
- In spread mode, fixture 1 has sequence number 0, fixture 5 has 1, and fixture 3 has 2

---

# Chapter 3 Selection Tools

Selection tools are used to adjust the **arrangement order** and **grouping method** of selected fixtures, thereby influencing the performance of spread and frame effects.

## 3.1 Selection Tool Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| **Block** | Integer | 1 | Block size. Treats consecutive fixtures as a group, sharing the same attributes |
| **Group** | Integer | 1 | Number of groups. Divides fixtures evenly into several groups |
| **Wings** | Integer | 1 | Number of mirrored segments. 1 = no mirroring, 2 = left-right mirror |

### Block

- **Block = 1** (default): Each fixture is independent
- **Block = 2**: Every 2 fixtures are treated as one group, and fixtures within a group receive the same value
- **Block = 3**: Every 3 fixtures form a group

**Example**: Fixtures 1-8 selected, Block = 2
```
Fixtures: 1  2  3  4  5  6  7  8
Groups:  [A  A] [B  B] [C  C] [D  D]
Spread:  0%    33%    66%    100%
```

### Group

- **Group = 1** (default): All fixtures in one group
- **Group = 2**: Fixtures are divided into 2 groups, with sequence numbers assigned independently within each group

**Example**: Fixtures 1-8 selected, Group = 2
```
Fixtures: 1  2  3  4  5  6  7  8
Groups:  [Group 1: 1 2 3 4] [Group 2: 5 6 7 8]
Spread:  0% 33% 66% 100%  0% 33% 66% 100%
```

### Wings (Mirror)

- **Wings = 1** (default): No mirroring
- **Wings = 2**: Fixtures are split into two halves, and the right half's order is reversed, creating a mirror effect

**Example**: Fixtures 1-8 selected, Wings = 2
```
Fixtures:1  2  3  4  5  6  7  8
Seq:     0  1  2  3  3  2  1  0  ← right half mirrored
Spread:  0% 33% 66% 100% 100% 66% 33% 0%
```

## 3.2 Advanced Arrangement Tools

In addition to Block/Group/Wings parameters, the following arrangement tools are also available:

### Shuffle

Randomly shuffles the order of selected fixtures. Suitable for creating "irregular" effects.

### Reverse

Reverses the order of selected fixtures. Fixture 1 becomes last, and the last becomes first.

### Shift

Shifts the order of selected fixtures by a specified number of steps.

- **Shift +1**: All fixtures move one position backward; the last moves to the front
- **Shift -1**: All fixtures move one position forward; the first moves to the back

### Interleave

Interleaves the selected fixtures by a specified step count.

- **Interleave 2**: Odd-position fixtures are placed first, even-position fixtures after

**Example**: Fixtures 1-8, Interleave 2
```
Original order: 1  2  3  4  5  6  7  8
After interleave: 1  3  5  7  2  4  6  8
```

---

# Chapter 4 Fixture Groups

## 4.1 What Are Fixture Groups

A fixture group is a "shortcut" to a set of fixtures. After saving a commonly used fixture combination as a group, you can select all those fixtures with a single button click, without manually selecting each one individually every time.

## 4.2 Groups Panel

The Groups panel displays all groups in a button grid format:

- Each button shows the group name and a color marker
- Empty slots are shown as gray blank buttons

## 4.3 Creating Fixture Groups

### Method 1: Create from Selection

1. First select the fixtures you want to group in the Fixture Sheet
2. Open the Groups panel
3. **Right-click** an empty slot
4. Select **Store Group**
5. Enter a group name
6. Click confirm

### Method 2: Quick Action

1. Select fixtures
2. In the Groups panel, **long-press** an empty slot
3. Directly save as a group

## 4.4 Using Fixture Groups

### Selecting a Group

- **Click** a group button → selects all fixtures in that group (replaces current selection)
- **Ctrl + click** → adds the group's fixtures to the current selection

### Quick Double-Click

- **Double-click** a group button → selects the group and automatically switches to Programmer mode

## 4.5 Managing Fixture Groups

### Updating Group Contents

1. Select a new fixture combination
2. **Right-click** the target group button
3. Select **Update**
4. The group contents are updated to the current selection

### Renaming a Group

1. **Right-click** the group button
2. Select **Rename**
3. Enter a new name
4. Press Enter to confirm

### Deleting a Group

1. **Right-click** the group button
2. Select **Delete**
3. Click confirm in the confirmation dialog

### Setting Group Color

1. **Right-click** the group button
2. Select **Set Color**
3. Pick a color in the color picker
4. The group button displays the new color for easy visual distinction

## 4.6 Uses of Fixture Groups

| Use | Description |
|-----|-------------|
| **Quick Selection** | Select a commonly used fixture combination with one click |
| **Programming Grouping** | Group by area/function (e.g., "Front Light", "Side Light", "Wash") |
| **Frame Effect Grouping** | Apply different frame effects to different groups |
| **CUE Programming** | Quickly switch between selecting different groups for programming |

---

# Chapter 5 Fixture Sheet

## 5.1 Panel Overview

The Fixture Sheet is the most essential panel for viewing and operating fixtures. It displays information and attribute values for all patched fixtures in a spreadsheet format.

## 5.2 Table Structure

### Column Definitions

| Column | Description |
|--------|-------------|
| **ID** | Fixture ID number |
| **Name** | Fixture name |
| **Type** | Fixture model |
| **Dimmer** | Brightness value (0-100%) |
| **Pan** | Horizontal rotation angle (if supported by the fixture) |
| **Tilt** | Vertical rotation angle (if supported by the fixture) |
| **Red / Green / Blue** | RGB color values (if supported by the fixture) |
| **Zoom** | Beam angle (if supported by the fixture) |
| **Other Attributes** | Dynamically displayed based on fixture type |

### Row Definitions

- Each patched fixture occupies one row
- Matrix fixtures can be expanded; each sub-fixture (module instance) occupies one row, displayed indented beneath the master fixture row

## 5.3 Value Display Rules

### Color Coding

| Color | Meaning |
|-------|---------|
| **White** | Current output value from CUE playback or default |
| **Red** | Value in the Programmer (modified but not yet stored to a CUE) |
| **Gray** | The fixture does not support this attribute |

### Mixed Values

When multiple fixtures are selected and their values differ for a given attribute, the encoder wheel displays a **"Mixed"** indicator.

## 5.4 Interactive Operations

### Selecting Fixtures

- **Click row** → selects that fixture
- **Ctrl + click** → multi-select
- **Shift + click** → range select
- Selected rows are highlighted with a blue background

### Sorting

- **Click column header** → sort ascending by that column
- **Click the same column header again** → toggle to descending
- Default sorting is by Fixture ID in ascending order

### Searching

- The search box at the top of the panel accepts keywords
- Real-time filtering of rows matching the fixture name or type

### Expanding Matrix Fixtures

- Click the **expand arrow** to the left of a matrix fixture row
- Displays all sub-fixture rows for that fixture
- Click again to collapse

## 5.5 Fixture Sheet Toolbar

The top of the panel provides the following function buttons:

| Button | Function |
|--------|----------|
| **Search Box** | Enter keywords to filter fixtures |
| **Expand/Collapse All** | Expand or collapse all matrix fixtures with one click |
| **DMX Value Display** | Toggle between displaying percentage values or raw DMX values (0-255) |

---

> **Next**: [User Manual (3) Programmer, Encoders, Color Picker and Preset System](SuperConsolePro_UserManual_Part3.md)
