# SuperConsolePro Lighting Console — User Manual (1)

## Overview · Quick Start · UI Layout · Window Management

---

# Chapter 1 Overview

## 1.1 What Is SuperConsolePro

SuperConsolePro is a **professional-grade lighting console system** built into the Unreal Engine editor. It enables lighting designers to perform the following operations within Unreal Engine's 3D scenes, just like operating a real physical console (such as MA3, ETC Eos):

- Fixture Patching
- Fixture Selection and Grouping
- Attribute Programming (Dimmer, Color, Position, Beam, etc.)
- CUE Recording and Playback
- Frame Effects (dynamic effects such as Chase, Rainbow Chaser, etc.) Editing
- Timeline Arrangement and Music Synchronization
- Real-time DMX Output
- Show File Saving and Management

All operations are performed within the editor window using a mouse and keyboard, without the need for external hardware consoles.

## 1.2 Applicable Scenarios

| Scenario | Description |
|----------|-------------|
| Virtual Production | Preview lighting effects during the VP stage, verifying fixture positions and programming in advance |
| Show Pre-visualization | Simulate real performance lighting in a 3D environment, linked with stage design plans |
| Lighting Education | Learn professional console operation workflows at zero cost |
| Film/TV Lighting Preview | Preview lighting effects using Unreal Engine's physical lighting system |
| Live Performances | Control real fixtures via DMX output for live show control |

## 1.3 Core Concepts at a Glance

Before getting started, please understand the following core concepts:

### Patch
Importing SuperDMX fixtures from the scene into the console, assigning each fixture a **Fixture ID**. Only patched fixtures can be selected and controlled by the console.

### Selection
The currently selected set of fixtures. Encoders and the Programmer only affect selected fixtures.

### Programmer
A "temporary staging area." Values adjusted via encoders first enter the Programmer and are displayed with red highlighting. Values in the Programmer are not yet "saved" and will be lost when the console is closed.

### CUE
Saving a "snapshot" of the Programmer contents creates a CUE. A CUE contains fixture attribute values along with fade/delay time parameters. Multiple CUEs can be organized in a **CUE List** for sequential playback.

### Preset
Similar to a CUE but lighter. Presets typically save only one category of attributes (such as Color, Position) for quick recall of commonly used settings.

### Frame Effect
A waveform-based dynamic effect engine (comparable to MA3 Phaser). Capable of generating periodic effects such as Chase, Rainbow, and Strobe effects.

### DMX
The standard communication protocol for the lighting industry. The console sends control signals to fixtures via DMX. Each DMX **Universe** contains 512 channels.

### Show File
A file that saves all patching, CUE, Preset, Timeline, window layout, and other data (in .ssshow format), equivalent to the console's "project file."

---

# Chapter 2 Quick Start

## 2.1 Opening the Console

1. In Unreal Editor, click the **SuperConsolePro** button on the top toolbar (or via menu **Window → SuperConsolePro**)
2. The console main window will open as an independent panel

## 2.2 Five-Minute Getting Started Walkthrough

The following is the shortest path to completing a full lighting programming session from scratch:

### Step 1: Patch Fixtures

1. Open the **Settings** panel in the console window
2. Switch to the **Patch** tab
3. Click the **Scan Scene** button
4. All SuperDMX fixtures in the scene will appear in the list
5. Check the fixtures you want to control, click **Import Selected**
6. The fixtures appear in the console and are now selectable and controllable

### Step 2: Select Fixtures

1. Open the **Fixture Sheet** panel
2. Click a fixture row → selects a single fixture
3. Hold Ctrl + click → multi-select
4. Hold Shift + click → range selection
5. Selected fixtures are highlighted in blue

### Step 3: Adjust Attributes

1. Check the **Encoder Bar** at the bottom
2. The Encoder Bar automatically displays the available attributes of the selected fixtures
3. Drag the encoder wheels up/down → adjust attribute values
4. Click different categories in the attribute tab bar (Dimmer, Color, Position, etc.) to switch the displayed attribute groups

### Step 4: Store a CUE

1. Open the **Playback** panel
2. Right-click an empty executor slot
3. Select **Store**
4. The values in the Programmer are saved as a CUE

### Step 5: Playback a CUE

1. In the Playback panel, click the stored CUE button
2. The CUE is activated and the fixtures output the set effect
3. Click again to release (in Toggle mode)

### Step 6: Save the Show

1. Open the Settings panel → **Show** tab
2. Click **Save**
3. All data is saved to the .ssshow file
4. The console will automatically load the last show file the next time it is opened

---

# Chapter 3 UI Layout

## 3.1 Main Interface Structure

The console main interface consists of the following areas (from outside to inside):

```
┌────────────────────────────────────────────────────────────────┐
│  Control Bar                                       Title Bar   │
│  ┌──────┐                                              ┌────┐ │
│  │      │                                              │View│ │
│  │Control│         Grid Window Manager                  │ Bar│ │
│  │  Bar  │      (Grid Window Manager)                  │    │ │
│  │      │        16 cols × 8 rows grid                 │View│ │
│  │      │      Various panels displayed here            │Bar │ │
│  │      │                                              │    │ │
│  └──────┘                                              └────┘ │
│  ┌──────────────────────────────────────────────────────────┐ │
│  │  Command Line                                             │ │
│  ├──────────────────────────────────────────────────────────┤ │
│  │  Encoder Bar                                              │ │
│  └──────────────────────────────────────────────────────────┘ │
└────────────────────────────────────────────────────────────────┘
```

### 3.1.1 Control Bar — Left Side

Located on the far left of the main interface, collapsible/expandable. Provides the following quick action buttons:

| Button | Function |
|--------|----------|
| **Clear** | Clears all Programmer values for selected fixtures. Does not affect running CUE output |
| **Highlight** | Temporarily sets selected fixtures to highlight values (Dimmer=100%, other attributes at highlight preset values), for quickly locating fixtures in the scene. Click again to cancel highlight |
| **Full** | Sets the Dimmer of selected fixtures to 100% (writes to Programmer) |
| **Zero** | Sets the Dimmer of selected fixtures to 0% (writes to Programmer) |
| **Default** | Restores all attributes of selected fixtures to factory default values (writes to Programmer) |
| **Activate** | Reads all current DMX output values of selected fixtures into the Programmer (equivalent to "capturing the current state") |

**Usage Tips**:
- **Clear** is the most frequently used operation. After programming a CUE, remember to Clear the Programmer before selecting other fixtures for the next CUE's programming
- **Highlight** does not affect the Programmer and is a non-destructive operation. After releasing, the fixtures return to their original state

### 3.1.2 Title Bar — Top

Displays the current show file name and system status information.

### 3.1.3 Grid Window Manager — Center

The core area of the console interface. Uses a **16 column × 8 row** grid layout, with various function panels placed in the grid as windows.

For detailed operations, see **3.2 Window Management** in this chapter.

### 3.1.4 View Bar — Right Side

Located on the far right of the main interface, collapsible/expandable. Used for managing **view layout presets**, i.e., remembering the current window arrangement to switch between different work interfaces with a single click.

For detailed operations, see **3.3 View Management** in this chapter.

### 3.1.5 Command Line — Bottom

Located above the Encoder Bar. Used for entering text commands (an advanced feature, not commonly used in daily operation).

### 3.1.6 Encoder Bar — Very Bottom

The core interaction area of the console, detailed in the **User Manual (3) Programmer and Encoders** chapter.

---

## 3.2 Window Management

### 3.2.1 Adding Windows

In the Grid Window Manager, clicking any **empty cell** will bring up the "Add Window" dialog. The dialog displays all available panel types in a 3-column grid:

| Panel Type | Description |
|------------|-------------|
| **Fixture Sheet** | Displays all patched fixtures and their attribute values in a spreadsheet format |
| **Playback** | A grid of CUE executor buttons for triggering and managing CUEs |
| **Preset** | Preset pool, displaying and managing presets by category |
| **Groups** | Fixture group management panel |
| **Color Picker** | HSV color wheel for quickly setting fixture colors |
| **Layout** | 2D fixture layout editor |
| **Frame Editor** | Waveform effect editor |
| **Frame Presets** | Frame effect preset library |
| **Timecode** | Multi-track timeline editor |
| **Running Playbacks** | Displays all currently running CUEs/effects |
| **DMX** | DMX Universe management and output control |
| **Settings** | System settings (Show File, Patch, DMX Configuration) |
| **Show File** | Show file management (Open, Save, New) |
| **Patch** | Fixture patching management panel |

After selecting a panel type, a window will be created at the cell position you clicked.

### 3.2.2 Moving Windows

- Hover the mouse over the window's **title bar**
- Hold the left button and drag; the window follows the mouse
- Release the mouse; the window snaps to the new grid position

### 3.2.3 Resizing Windows

- Hover the mouse over the window's **bottom-right edge**
- The cursor changes to a resize shape
- Hold the left button and drag to change how many grid cells the window occupies
- The minimum window size is 1×1 cells; the maximum can fill the entire grid (16×8)

### 3.2.4 Closing Windows

- Click the **× button** on the right side of the window title bar
- The window is closed, and the corresponding grid area returns to empty

### 3.2.5 Window Layering

- When multiple windows overlap, clicking a window brings it to the front
- It is recommended to plan window sizes and positions reasonably to avoid overlap

---

## 3.3 View Management (View Bar)

The View Bar is located on the right side of the main interface, providing quick view layout management.

### 3.3.1 Built-in Preset Views

The system includes the following preset view layouts:

| Preset Name | Included Panels | Applicable Scenario |
|-------------|-----------------|---------------------|
| **Default** | Fixture Sheet + Playback + Encoder Bar | General programming |
| **Programming** | Fixture Sheet + Color Picker + Presets + Playback | Deep programming |
| **Playback** | Playback (large area) + Running Playbacks | Show playback |
| **Timeline** | Timeline (large area) + Playback | Timeline arrangement |

### 3.3.2 Loading a View

- **Single-click** a view button in the View Bar
- The window manager immediately switches to that view's window layout

### 3.3.3 Saving Custom Views

1. First arrange the windows into the desired layout
2. **Right-click** a view slot in the View Bar
3. Select **Save**
4. The current window layout is saved to that slot
5. You can optionally rename the view

### 3.3.4 Deleting / Renaming Views

- **Right-click** a saved view button
- Select **Rename** to enter a new name
- Select **Delete** to clear that slot

### 3.3.5 Clearing the Screen

- Click the **Clear Screen** button at the bottom of the View Bar
- All windows are closed, returning to the empty grid

---

## 3.4 Detailed Description of Available Panel Types

The following is a functional summary of all available panels in the console. Detailed operation methods for each panel will be explained in the subsequent manual chapters.

### Fixture Sheet Panel
Displays all patched fixtures in a spreadsheet format. Each row represents one fixture (matrix fixtures can be expanded into sub-fixture rows), and columns show the fixture's attribute values. Supports click selection of fixtures, sorting, and search filtering.

### Playback Panel
Displays CUE executors in a button grid format. Each button corresponds to a CUE. Left-click triggers a CUE; right-click opens a menu (Edit, Delete, Bind Key, etc.).

### Preset Panel
A preset pool. Top category tabs (All / Position / Color / Beam / General) switch the display to show the corresponding type of presets. Left-click to apply a preset; right-click an empty slot to save.

### Groups Panel
Displays fixture groups in a button grid. Click a group button → selects all fixtures in that group. Right-click to create a new group or manage existing groups.

### Color Picker Panel
An HSV color wheel + color bar + numeric input. Drag the color wheel to pick hue and saturation; drag the color bar to adjust brightness. The selected color is applied in real time to the selected fixtures' RGB attributes. The bottom shows color preset swatches.

### Layout View Panel
A 2D top-down view showing the physical positions of fixtures on the stage. Supports dragging fixtures to adjust position, box-selecting fixtures, and zooming/panning the canvas.

### Frame Editor Panel
A visual editor for waveform effects. The left toolbar selects waveform types and adjusts parameters; the center displays real-time waveform preview; the right side has a quick parameter panel.

### Frame Presets Panel
A frame effect preset library. Contains system built-in presets and user-defined presets. Click a preset to apply it to the currently selected fixtures.

### Timecode Panel
A multi-track timeline editor. Supports CUE tracks and audio tracks. CUEs can be arranged on the timeline for automatic playback by time.

### Running Playbacks Panel
Displays information about all currently running CUEs and effects in real time, including name, type, fade progress, intensity, and more.

### DMX Panel
DMX output management. Contains a global DMX output switch and independent enable/disable control for each Universe.

### Settings Panel
System settings, containing three tabs:
- **Show**: Create / Save / Save As show file
- **Patch**: Fixture patching management
- **DMX**: DMX output configuration

### Show File Panel
Displays the show file list, supporting creating, opening, saving, saving as, deleting, and renaming show files.

### Patch Panel
A dedicated fixture patching panel. Displays lists of imported and non-imported fixtures, supporting scene scanning, import, removal, filtering, and searching.

---

# Chapter 4 General UI Control Descriptions

## 4.1 Console Button

Buttons in the console have multiple style variants:

| Variant | Appearance | Purpose |
|---------|------------|---------|
| **Default** | Dark gray background | Normal operation button |
| **Primary** | Blue background | Primary action (such as Confirm, Apply) |
| **Secondary** | Dark gray background, gray text | Secondary action |
| **Success** | Green background | Currently active/running state |
| **Warning** | Yellow background, black text | Operation requiring attention |
| **Danger** | Red background | Dangerous operations like Delete, Clear |
| **Ghost** | Transparent background | Small toolbar buttons |

Buttons come in three sizes:
- **Small**: For compact toolbar buttons
- **Medium**: Default size
- **Large**: For primary action buttons

## 4.2 Numeric Input Popup

Double-clicking the value area of an encoder brings up a numeric input popup. Supports:

- Direct numeric input (e.g., `50`)
- Percentage input (e.g., `50%`)
- Time value input (e.g., `2s` for 2 seconds)
- Time expression input (e.g., `0 Thru 2` for linear distribution from 0 to 2 seconds)
- Press **Enter** to confirm, press **Esc** to cancel

## 4.3 Right-Click Menus

Many elements in the console support right-click menu operations. Common right-click menu scenarios:

- **Empty slot in Playback panel** → Store (store a CUE)
- **Existing CUE in Playback panel** → Edit / Delete / Rename / Set Trigger Mode / Bind Key
- **Empty slot in Preset panel** → Store (store a preset)
- **Existing Preset in Preset panel** → Update / Delete / Rename
- **Empty slot in Groups panel** → Store Group (save current selection as a group)
- **Existing Group in Groups panel** → Update / Delete / Rename / Set Color
- **Slot in View Bar** → Save / Rename / Delete

## 4.4 Confirmation Dialogs

When performing dangerous operations (such as deleting a CUE, creating a new show, overwriting a file), the system displays a confirmation dialog:

- **Title** describes the operation
- **Message** explains the consequences of the operation
- **Confirm** button executes the operation
- **Cancel** button aborts the operation

---

> **Next**: [User Manual (2) Fixture Patching, Fixture Selection and Fixture Groups](SuperConsolePro_UserManual_Part2.md)
