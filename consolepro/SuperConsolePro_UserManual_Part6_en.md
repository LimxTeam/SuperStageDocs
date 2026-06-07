# SuperConsolePro Lighting Console — User Manual (6)

## DMX Output · Show File Management · Settings Panel · Shortcuts Quick Reference

---

# Chapter 1 DMX Output

## 1.1 DMX Output Architecture

SuperConsolePro sends control signals to fixtures via the underlying **SuperDMX module**. Internally, the console has multiple "value sources" that are merged by priority to produce the final DMX output.

### Value Sources and Priorities

```
  Priority from highest to lowest:

  ┌──────────────┐
  │  Programmer   │  ← Highest priority (values being adjusted by user)
  │              │
  └──────┬───────┘
         ↓
  ┌──────────────┐
  │  Frame Effect │  ← Running dynamic effects
  │              │
  └──────┬───────┘
         ↓
  ┌──────────────┐
  │  CUE Playback │  ← CUEs running in the Playback panel
  │              │
  └──────┬───────┘
         ↓
  ┌──────────────┐
  │  Timecode CUE │  ← CUEs triggered by external timecode
  │              │
  └──────┬───────┘
         ↓
  ┌──────────────┐
  │  Timeline     │  ← Timeline playback output
  │              │
  └──────┬───────┘
         ↓
  ┌──────────────┐
  │  Default      │  ← Factory default fixture values
  │              │
  └──────┬───────┘
         ↓
  ┌──────────────┐
  │  DMX Output   │  → Art-Net / sACN → Fixtures
  │  (SuperDMX)  │
  └──────────────┘
```

### Mixing Rules

| Channel Type | Mixing Method | Description |
|-------------|---------------|-------------|
| **Intensity Channels** (Dimmer) | HTP (Highest Takes Precedence) | Takes the highest value among multiple sources |
| **Other Channels** (Color, Position, etc.) | LTP (Latest Takes Precedence) | The last written value overrides earlier values |

## 1.2 DMX Panel

The DMX panel is used to manage global DMX output settings.

### Global DMX Output Switch

| Switch | Description |
|--------|-------------|
| **DMX Output** | Global DMX output master switch. **When off, all DMX output stops**, but programming and playback inside the console remain unaffected |

> **Use Case**: Turn off DMX output during the programming/preview phase to avoid affecting real fixtures that are in use.

### Universe Management

The panel lists all DMX Universes in use:

| Column | Description |
|--------|-------------|
| **Universe Number** | DMX universe number (1-64) |
| **Channels Used** | Number of channels occupied in this Universe |
| **Enable/Disable** | Individual control of whether this Universe outputs DMX |

### Disabling Individual Universes

- Each Universe has an independent enable/disable switch on the right
- Disabling a Universe → DMX output on that universe is paused
- Other Universes are not affected

## 1.3 DMX Value Display

### View in Fixture Sheet

The Fixture Sheet supports toggling the value display format:

| Format | Display | Description |
|--------|---------|-------------|
| **Percentage** | 0% - 100% | Default display format, intuitive and easy to read |
| **DMX Value** | 0 - 255 | Raw DMX channel values, suitable for debugging |

Toggle method: Click the **DMX Value Display** button in the Fixture Sheet toolbar.

### View in Encoders

Values displayed on encoder wheels default to percentages. For angular attributes (Pan, Tilt), they are displayed as angle values.

---

# Chapter 2 Show File Management

## 2.1 Show File Overview

A Show File is the SuperConsolePro project file, saving **all working data** of the console.

### File Format

| Item | Description |
|------|-------------|
| **Extension** | `.ssshow` |
| **Format** | Binary serialization |
| **Storage Location** | `%LOCALAPPDATA%\SuperStage\Shows\` |

### Data Contained in the File

| Data Category | Content |
|---------------|---------|
| **Patch Data** | All fixtures' Fixture ID, name, type, Universe/Channel mapping |
| **Fixture Groups** | All group definitions (name, members, color) |
| **CUE Data** | All CUE Lists and every CUE within them (attribute values, time parameters, trigger modes) |
| **Preset Data** | All presets (Dimmer / Position / Gobo / Color / Beam / Focus / Control / Shapers) |
| **Frame Effect Presets** | User-defined frame effect presets |
| **Timeline Data** | All timecodes and their tracks, clips, markers |
| **Layout Data** | All Layouts and fixture 2D positions |
| **Window Layout** | View presets (window arrangement) |
| **Key Bindings** | CUE keyboard shortcut mappings |
| **DMX Settings** | Universe enable status, etc. |

## 2.2 Show File Panel

### Panel Layout

```
┌──────────────────────────────────────────────────────────────┐
│ Current Show: SpringGala2026.ssshow               [Unsaved *] │
├──────────────────────────────────────────────────────────────┤
│ [New] [Save] [Save As] [Open]                                │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│ Show File List:                                              │
│ ┌──────────────────────────────────────────────────────────┐ │
│ │ ● SpringGala2026.ssshow      2026-01-15 14:30           │ │
│ │   LanternFestival.ssshow     2026-02-05 09:15           │ │
│ │   TestShow.ssshow            2026-01-10 11:00           │ │
│ └──────────────────────────────────────────────────────────┘ │
│                                                              │
│ [Delete] [Rename]              ← Planned feature, not yet implemented in current version │
└──────────────────────────────────────────────────────────────┘
```

## 2.3 Creating a New Show

### Operation Steps

1. Click the **New** button
2. A new show dialog pops up
3. Enter a show name
4. Click **Confirm**
5. The system creates a blank show file

> **Note**: Creating a new show clears all current data. If there are unsaved changes, the system will prompt to save first.

### New Show Dialog

| Field | Description |
|-------|-------------|
| **Show Name** | Enter the show name (do not include the extension) |

## 2.4 Saving a Show

### Quick Save

- Click the **Save** button
- If a file name already exists, it overwrites directly
- If it's a newly created show that hasn't been saved yet, it automatically proceeds to "Save As"

### Save As

1. Click the **Save As** button
2. A Save As dialog pops up
3. Enter a new file name
4. Click **Confirm**
5. The file is saved to the new path

### Unsaved Changes Indicator

- A **`*`** appears after the current show name in the title bar, indicating unsaved changes
- When closing the console or creating a new show, the system will prompt "Save current changes?"

## 2.5 Opening a Show

### Opening from File List

1. Click the **Open** button
2. An Open Show dialog pops up
3. The show file list displays all available `.ssshow` files
4. Each file shows its name and last modified time
5. Select a file and click **Confirm**

### Auto-Load Last Show

- The console automatically loads the last opened show file when it starts
- If the last file has been deleted, it starts with a blank state

## 2.6 Deleting a Show

> **Note**: The delete show feature is planned for a future version. In the current version, please manually delete `.ssshow` files from the `%LOCALAPPDATA%\SuperStage\Shows\` directory via File Explorer.

## 2.7 Renaming a Show

> **Note**: The rename show feature is planned for a future version. In the current version, please use the **Save As** function to save the show file with a new name.

---

# Chapter 3 Settings Panel

## 3.1 Panel Structure

The Settings panel contains three tabs:

| Tab | Content |
|-----|---------|
| **Show** | Show file management (New / Save / Save As / Open) |
| **Patch** | Fixture patching management |
| **DMX** | DMX output configuration |

## 3.2 Show Tab

Contains the same functions as the Show File panel (New, Save, Save As, Open). This is a convenient entry point without needing to open the Show File panel separately.

For detailed operations, refer to Chapter 2 of this manual, "Show File Management."

## 3.3 Patch Tab

Contains the same functions as the Patch panel (Scan Scene, Import/Remove Fixtures, Sync). This is a convenient entry point.

For detailed operations, refer to Chapter 1 of User Manual (2), "Fixture Patching."

## 3.4 DMX Tab

### DMX Output Master Switch

| Setting | Description |
|---------|-------------|
| **Enable DMX Output** | Global DMX output switch. When off, all DMX signals stop transmitting |

### Universe Configuration

Lists all DMX Universes in use:

| Setting | Description |
|---------|-------------|
| **Universe Number** | Universe number (read-only, determined by fixture patching) |
| **Enable** | Whether to output DMX signals for this universe |
| **Protocol** | DMX output protocol (determined by SuperDMX module configuration) |

---

# Chapter 4 Key Bindings

## 4.1 CUE Key Bindings

Keyboard shortcuts can be bound to CUEs for key-triggered CUE execution.

### Binding a Key

1. In the Playback panel, **right-click** a CUE executor button
2. Select **Bind Key**
3. In the popup dialog, press the desired key combination
4. The following modifier key combinations are supported:
   - Single key (e.g., `F1`, `1`, `A`)
   - **Ctrl** + key
   - **Shift** + key
   - **Alt** + key
   - Multiple modifier combinations (e.g., **Ctrl + Shift + F1**)
5. The system automatically detects conflicts — if the shortcut is already used by another CUE, it will prompt whether to overwrite

### Unbinding a Key

1. **Right-click** a CUE executor button
2. Select **Unbind Key**

### Key Binding Behavior

| Trigger Mode | Key Pressed | Key Released |
|--------------|-------------|--------------|
| **Toggle** | Activate/Release CUE (toggle) | No action |
| **Momentary** | Activate CUE | Release CUE |
| **Flash** | Instantly activate CUE (no fade) | Instantly release CUE |

### Viewing Bound Keys

- The CUE executor button displays a shortcut marker when a key is bound
- The bound shortcut can be viewed in the CUE's right-click menu

---

# Chapter 5 Shortcuts Quick Reference

## 5.1 General Shortcuts

| Shortcut | Function | Scope |
|----------|----------|-------|
| **Esc** | Clear selection / Close dialog | Global |
| **Ctrl + A** | Select all fixtures | Fixture Sheet / Layout View |
| **Ctrl + I** | Invert selection (planned feature, not yet available in current version) | Fixture Sheet |
| **Delete** | Delete selected item (CUE clip/audio clip/marker, etc.) | Timeline |

## 5.2 Selection Operations

| Shortcut | Function |
|----------|----------|
| **Click** | Select fixture (replace selection) |
| **Ctrl + Click** | Add to selection / Remove from selection |
| **Shift + Click** | Range selection |

## 5.3 Encoder Operations

| Shortcut | Function |
|----------|----------|
| **Drag encoder wheel** | Adjust attribute value |
| **Mouse wheel** (hover over encoder wheel) | Adjust attribute value (default step: 1% of range/notch) |
| **Shift + Mouse wheel** | Additional sensitivity reduction to 1/5 |
| **Double-click encoder value** | Open numeric input popup |

## 5.4 Timeline Operations

| Shortcut | Function |
|----------|----------|
| **Spacebar** | Play / Pause |
| **Mouse wheel** | Pan timeline horizontally |
| **Ctrl + Scroll** or **Shift + Scroll** | Zoom timeline |
| **Click time ruler** | Jump playhead |
| **Drag clip** | Move clip position |
| **Drag clip edge** | Resize clip |
| **Delete** | Delete selected clip |

## 5.5 Layout View Operations

| Shortcut | Function |
|----------|----------|
| **Middle-click drag** / **Right-click drag** | Pan canvas |
| **Mouse wheel** | Zoom canvas |
| **Left-click drag on empty area** | Box-select fixtures |
| **Left-click drag fixture** | Move fixture position (in Move mode) |
| **Double-click empty area** | Reset view |

## 5.6 Control Bar Operations

| Operation | Function |
|-----------|----------|
| **Clear** button | Clear Programmer values |
| **Full** button | Set Dimmer to 100% |
| **Zero** button | Set Dimmer to 0% |
| **Highlight** button | Highlight selected fixtures |

---

# Chapter 6 Common Workflows

## 6.1 Complete Show Programming Workflow from Scratch

```
Step 1: Preparation
├── Place SuperDMX fixtures in the scene
├── Open SuperConsolePro
└── Create a new show file

Step 2: Patching
├── Settings panel → Patch → Scan Scene
├── Import all fixtures
└── Confirm Fixture ID assignments

Step 3: Grouping
├── Select fixtures by area
├── Save as groups (e.g., "Front Light", "Side Light", "Wash", "Effects")
└── Set different colors for groups for easy identification

Step 4: Build Preset Library
├── Select fixtures group by group
├── Adjust common positions → Store as Position presets
├── Adjust common colors → Store as Color presets
└── Adjust common beams → Store as Beam presets

Step 5: Program CUEs
├── Select fixtures → Adjust attributes → Store CUE
├── Set appropriate fade times for each CUE
├── Test each CUE in the Playback panel
└── Repeat until all scenes are complete

Step 6: Arrange Timeline (Optional)
├── Create a timecode
├── Import background music
├── Place CUEs on the timeline
├── Add markers
└── Preview playback and fine-tune timing

Step 7: Save
└── Save the show file
```

## 6.2 Live Performance Workflow

```
Before the Show:
├── Open the show file
├── Confirm DMX output is enabled
├── Check that all fixtures are online
└── Test key CUEs

During the Show (Manual Mode):
├── Click Go sequentially to execute CUEs
├── Use shortcuts to quickly trigger effects
├── Manually adjust Programmer values when necessary
└── Use Release All for emergency blackout

During the Show (Timeline Mode):
├── Click Play to start the timeline
├── Timeline automatically triggers CUEs
├── Pause/jump when necessary
└── Manually overlay Programmer values for fine-tuning

After the Show:
└── Save the show file (if modified)
```

## 6.3 Frame Effect Quick Creation Workflow

```
1. Select fixtures (recommended: select a row or group of fixtures)
2. Open the Frame Editor
3. Select waveform type (Sine for chase, Rectangle for strobe)
4. Set speed (BPM)
5. Adjust phase range (360° = full chase)
6. Set Wings (mirror effect)
7. Click Play to preview
8. Store as CUE when satisfied (including the frame effect)
```

## 6.4 Color Chase Effect Creation

```
1. Select a row of fixtures (note selection order from left to right)
2. Open the Frame Editor
3. Add 3 steps:
   - Step 1: Red=100%, Green=0%, Blue=0%
   - Step 2: Red=0%, Green=100%, Blue=0%
   - Step 3: Red=0%, Green=0%, Blue=100%
4. Waveform type: Sine (smooth transition)
5. Speed: 30 BPM (slower, elegant color change)
6. Phase range: 360°
7. Play preview → fixtures display a rainbow chase effect
```

---

# Appendix A: Glossary

| Term | English | Description |
|------|---------|-------------|
| 配接 | Patch | The process of importing fixtures into the console and assigning IDs |
| 编程器 | Programmer | A buffer for temporarily storing edited values |
| 编码器 | Encoder | A rotary control used to adjust attribute values |
| 场景记忆 | CUE | A snapshot of fixture attribute values |
| 执行器 | Executor | A button used to trigger CUEs |
| 渐变 | Fade | Smooth transition of attribute values from one value to another |
| 延时 | Delay | The waiting time before a fade begins |
| 预设 | Preset | A reusable snapshot of attribute values |
| 帧效果 | Frame Effect | A waveform-based dynamic effect (corresponds to MA3 Phaser) |
| 发散 | Spread | The function of assigning different values to multiple fixtures (corresponds to MA2 Align) |
| 域 | Universe | A DMX transmission unit, each Universe contains 512 channels |
| 通道 | Channel | The smallest DMX control unit |
| 灯具组 | Group | A quick-selection collection of fixtures |
| 视图 | View | A preset scheme for window layouts |
| 演出文件 | Show File | A project file that saves all console data |
| 时间码 | Timecode | A timeline for automatically triggering CUEs by time |
| 轨道 | Track | A horizontal channel in the timeline |
| 片段 | Clip | An event placed on a track (CUE clip or audio clip) |
| 标记点 | Marker | A positioning mark on the timeline |
| 布局 | Layout | A 2D planar position map of fixtures |
| HTP | Highest Takes Precedence | Mixing rule for intensity channels: takes the highest value |
| LTP | Latest Takes Precedence | Mixing rule for other channels: takes the latest value |
| BPM | Beats Per Minute | The number of beats per minute |
| 占空比 | Width / Duty Cycle | The proportion of effective working time of a waveform |
| 突变 | Attack | The transition speed of a waveform from low to high |
| 衰减 | Decay | The transition speed of a waveform from high to low |
| 镜像 | Wings | Mirror segmentation of fixture phases |
| 块 | Block | Grouping where multiple fixtures share the same phase |

---

# Appendix B: Troubleshooting

| Problem | Possible Cause | Solution |
|---------|---------------|----------|
| Fixtures do not respond to console operations | DMX output is off | Check DMX panel → turn on DMX Output |
| Fixtures do not respond to console operations | Fixtures not patched | Settings panel → Patch → Scan and import fixtures |
| Fixtures do not respond to console operations | Fixtures not selected | Verify fixtures are selected in Fixture Sheet (blue highlight) |
| Encoder adjustments have no effect | Encoder is in time mode | Check encoder mode → switch back to Value |
| CUE not outputting | Programmer is overriding CUE values | Click Clear to clear the Programmer |
| CUE fade is very slow | Fade In time is too long | Edit CUE → modify Fade In time |
| Frame effect not running | Play not clicked | Frame Editor → click ▶ Play button |
| Frame effect causes all fixtures to sync | Phase Range is 0° | Set Phase Range to 360° |
| Timeline playback has no response | No CUE clips | Add CUE clips on CUE tracks |
| Audio does not play | Audio track is muted | Check the track's [M] button |
| Show file cannot be opened | File corrupted or version incompatible | Try restoring from backup |
| Window layout is messy | View preset was overwritten | Use Clear Screen to reset, rearrange |

---

> **This concludes the manual.**
>
> Full Manual Table of Contents:
> - [Manual (1) Overview, Quick Start, UI Layout and Window Management](SuperConsolePro_UserManual_Part1.md)
> - [Manual (2) Fixture Patching, Fixture Selection and Fixture Groups](SuperConsolePro_UserManual_Part2.md)
> - [Manual (3) Programmer, Encoders, Color Picker and Preset System](SuperConsolePro_UserManual_Part3.md)
> - [Manual (4) CUE Programming, Playback Panel and Frame Editor](SuperConsolePro_UserManual_Part4.md)
> - [Manual (5) Timeline Editor, Timecode and Layout View](SuperConsolePro_UserManual_Part5.md)
> - [Manual (6) DMX Output, Show File Management, Settings and Shortcuts Quick Reference](SuperConsolePro_UserManual_Part6.md)
