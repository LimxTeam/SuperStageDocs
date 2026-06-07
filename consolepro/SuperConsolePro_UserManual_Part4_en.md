# SuperConsolePro Lighting Console — User Manual (4)

## CUE Programming · Playback Panel · Frame Editor

---

# Chapter 1 CUE Programming

## 1.1 CUE Basic Concepts

**CUE** is the core function of the lighting console. A CUE is a "snapshot" of the Programmer contents — it fully records fixture attribute values and time parameters, and can be "played back" (Go) at any time to reproduce the lighting effect that was saved.

### Data Contained in a CUE

| Data Item | Description |
|-----------|-------------|
| **Fixture Attribute Values** | Each fixture's value for each attribute (e.g., Dimmer=80%, Pan=180°) |
| **Fade In Time** | The time required for attributes to change from the current value to the target value when the CUE is activated |
| **Delay In Time** | How long to wait after CUE activation before the fade begins |
| **Fade Out Time** | The time required for attributes to change from the current value back to zero/previous value when the CUE is released |
| **Delay Out Time** | How long to wait after CUE release before the fade-out begins |
| **Trigger Mode** | Toggle / Momentary / Flash |
| **CUE Number** | A number used for sorting and identification (e.g., 1.0, 2.0, 2.5) |
| **CUE Name** | A descriptive name (e.g., "Opening Full Bright", "Blue Night Scene") |
| **Frame Effect Data** | If the CUE contains frame effects, the effect parameters are also saved |

### CUE List

Multiple CUEs are organized in a **CUE List**. A CUE List is like a "playlist":

- A CUE List can contain any number of CUEs
- CUEs are sorted by number
- Multiple CUE Lists can be created (e.g., "Main Show", "Encore", "Rehearsal")
- A CUE List named "Main" is automatically created by default

## 1.2 Storing a CUE

### Method 1: Store from Programmer

This is the most commonly used method:

1. **Select fixtures**
2. **Adjust attribute values** using encoders (values enter the Programmer, displayed in red)
3. If needed, set fade and delay times (switch encoder mode to FadeIn/DelayIn, etc.)
4. Open the **Playback** panel
5. **Right-click** an empty executor slot
6. Select **Store**
7. In the popup dialog:
   - Enter a CUE name (optional; auto-named if left blank)
   - Enter a CUE number (optional; automatically assigned the next integer number if left blank)
8. Click **Confirm**
9. The CUE is saved, and the executor slot displays the CUE name

### Method 2: Store from Frame Effect

If you are running a Frame Effect, you can save it together with the current Programmer values as a CUE:

1. Create and start a frame effect (see Chapter 3 of this manual for details)
2. In the Playback panel, **right-click** an empty slot
3. Select **Store with Frame Effect**
4. The CUE will save both attribute values and frame effect parameters

> **Advantage**: When a CUE containing frame effects is played back, the frame effect runs automatically without manual activation.

### Method 3: Store to a Specific Slot

- **Right-click** a specific executor slot
- Select **Store** → the CUE is saved directly to that position
- If that position already has a CUE, the system will ask whether to overwrite

## 1.3 CUE Numbering System

### Numbering Rules

- A CUE number is a **floating-point number**, e.g., 1.0, 2.0, 3.5
- CUEs in a CUE List are sorted in **ascending order** by number
- Numbers can be decimals, making it convenient to insert new CUEs between existing ones

### Numbering Example

```
CUE 1.0  "Opening"
CUE 2.0  "First Section Blue"
CUE 2.5  "Blue Fine Tune"          ← inserted between 2.0 and 3.0
CUE 3.0  "Red Climax"
CUE 4.0  "Ending Blackout"
```

### Auto-Numbering

- When no number is specified, the system automatically assigns the next highest integer number
- For example, if the current highest number is 3.0, the new CUE is automatically numbered 4.0

## 1.4 CUE Time Parameters in Detail

### Fade Time

Fade time determines how quickly attribute values change when a CUE is activated/released.

```
  Value
100%│        ┌──────────────── Target value
    │       ╱
    │      ╱ ← Fade In process
    │     ╱
    │    ╱
  0%│───┘
    └──────────────────────── Time
        ↑                ↑
      Fade begins    Fade complete
      (Fade In time)
```

| Parameter | Default | Description |
|-----------|---------|-------------|
| **Fade In** | 0 sec | Fade time when the CUE is activated. 0 = instant switch |
| **Fade Out** | 0 sec | Fade time when the CUE is released. 0 = instant switch |

### Delay Time

Delay time is the waiting period before the fade begins.

```
  Value
100%│                ┌──────────── Target value
    │               ╱
    │              ╱ ← Fade In
    │             ╱
  0%│────────────┘
    └──────────────────────── Time
    ↑           ↑           ↑
  CUE triggered  Fade begins  Fade complete
    ← Delay In → ← Fade In →
```

| Parameter | Default | Description |
|-----------|---------|-------------|
| **Delay In** | 0 sec | How long to wait after CUE activation before fading in |
| **Delay Out** | 0 sec | How long to wait after CUE release before fading out |

### Per-Fixture / Per-Attribute Independent Times

Time parameters can be set **independently** for each fixture and each attribute:

1. Select specific fixtures
2. Switch encoder mode to Fade In (or another time mode)
3. Adjust the encoder → sets time only for the selected fixtures

### Time Distribution (Using Expressions)

Combined with time expressions, "sequential lighting" effects can be created:

**Example**: 10 fixtures, Fade In = `0 Thru 2`
```
Fixture  1 → Fade In = 0.0 sec (instant on)
Fixture  2 → Fade In = 0.22 sec
Fixture  3 → Fade In = 0.44 sec
Fixture  4 → Fade In = 0.67 sec
Fixture  5 → Fade In = 0.89 sec
...
Fixture 10 → Fade In = 2.0 sec (slowest on)
```

## 1.5 Trigger Modes

Each CUE can be set with a different trigger mode:

| Mode | Behavior | Applicable Scenario |
|------|----------|---------------------|
| **Toggle** | First click → activate CUE; click again → release CUE | Most regular CUEs |
| **Momentary** | Press → activate CUE; release → release CUE | Temporary effects (e.g., flash) |
| **Flash** | Press → instant activation (no fade); release → instant release | BOP (Blackout Push), etc. |

### Setting Trigger Mode

1. **Right-click** a CUE executor button
2. Select **Trigger Mode**
3. In the sub-menu, choose Toggle / Momentary / Flash

## 1.6 Editing Existing CUEs

### Updating a CUE

1. Play back the CUE to update (making it active)
2. Use Activate to read current values into the Programmer
3. Modify attribute values in the Programmer
4. **Right-click** the CUE's executor button
5. Select **Update**
6. The CUE contents are replaced with the current Programmer values

### Merging into a CUE

Unlike updating, "Merge" only updates attributes that have values in the Programmer, preserving other unmodified attributes in the CUE:

1. Select fixtures and only modify the attributes that need adjustment
2. **Right-click** the CUE executor button
3. Select **Merge**

### Loading a CUE into the Programmer

Load all values from a CUE into the Programmer for editing:

1. **Right-click** the CUE executor button
2. Select **Load to Programmer**
3. All CUE values enter the Programmer (displayed in red)
4. Freely modify and then Store/Update

### Renaming a CUE

1. **Right-click** the CUE executor button
2. Select **Rename**
3. Enter a new name

### Modifying a CUE Number

1. **Right-click** the CUE executor button
2. Select **Move**
3. Enter a new number
4. The CUE is automatically re-sorted

### Copying a CUE

1. **Right-click** the CUE executor button
2. Select **Copy**
3. Creates a new CUE with identical content (automatically assigned a new number)

### Deleting a CUE

1. **Right-click** the CUE executor button
2. Select **Delete**
3. Click confirm in the confirmation dialog

> **Warning**: Deleting a CUE is irreversible.

---

# Chapter 2 Playback Panel

## 2.1 Panel Overview

The Playback panel is the main interface for managing and triggering CUEs. It displays executors in a button grid format, with each executor able to be associated with a CUE.

## 2.2 Panel Layout

```
┌──────────────────────────────────────────────────────┐
│ CueList: [Main ▼]  [+ New]  [Go] [Release All]       │
├──────────────────────────────────────────────────────┤
│ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐       │
│ │ 1.0  │ │ 2.0  │ │ 3.0  │ │ 4.0  │ │      │       │
│ │Open  │ │Blue  │ │Red   │ │Black │ │      │       │
│ │AllOn │ │Night │ │Climax│ │Out   │ │      │       │
│ └──────┘ └──────┘ └──────┘ └──────┘ └──────┘       │
│ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐       │
│ │      │ │      │ │      │ │      │ │      │       │
│ └──────┘ └──────┘ └──────┘ └──────┘ └──────┘       │
├──────────────────────────────────────────────────────┤
│  [◀ Prev Page]   Page 1 / Total 1 (1-200)   [Next Page ▶] │
└──────────────────────────────────────────────────────┘
```

### Pagination System

The Playback panel uses a pagination mechanism to manage a large number of executor buttons:

| Parameter | Value | Description |
|-----------|-------|-------------|
| **Buttons Per Page** | 200 | Fixed value; Page 1 = buttons 1-200, Page 2 = 201-400, and so on |
| **Max Pages** | 60 | Supports up to 60 pages, totaling 12,000 executor positions |

- The bottom of the panel shows a **page navigation bar**: Previous/Next page buttons + current page number/total pages + button range
- Only pages containing stored CUEs are counted in the total page count
- Use the **[◀ Prev Page]** and **[Next Page ▶]** buttons to turn pages

## 2.3 CUE List Management

### Switching CUE Lists

- Click the CUE List dropdown at the top of the panel
- Select the CUE List to display

### Creating a CUE List

1. Click the **[+]** button next to the CUE List dropdown
2. Enter a name
3. A new empty CUE List is created

### Deleting a CUE List

1. **Right-click** the CUE List dropdown
2. Select **Delete CueList**
3. Confirm deletion

> **Note**: Deleting a CUE List also deletes all CUEs within it.

### Renaming a CUE List

1. **Right-click** the CUE List dropdown
2. Select **Rename CueList**
3. Enter a new name

## 2.4 Executor Button Operations

### Activating a CUE (Go)

- **Left-click** the executor button of a saved CUE
- The CUE begins activating, gradually taking effect according to the set delay and fade times
- The button turns green/bright indicating the CUE is running

### Releasing a CUE (Release)

- In **Toggle** mode, **click again** on the button to release the CUE
- In **Momentary** mode, **release the mouse** to release the CUE
- In **Flash** mode, **release the mouse** to instantly release

### Go Button (Sequential Execution)

The **Go** button at the top of the panel is used for sequential execution by CUE number:

- First click → executes CUE 1.0
- Second click → executes CUE 2.0 (while releasing CUE 1.0)
- And so on

### Release All

- Click the **Release All** button at the top of the panel
- All running CUEs are immediately released
- All fixtures return to default values

## 2.5 Executor Button States

| State | Appearance | Meaning |
|-------|------------|---------|
| **Empty** | Dark gray | No CUE saved |
| **Stored** | Button with name and number | CUE saved, not currently running |
| **Running** | Green/bright button | CUE is outputting |
| **Fading** | Fade progress indicator | CUE is fading in or out |

## 2.6 Running Playbacks Panel

This is an independent panel that displays all currently running CUEs and effects in real time.

### Panel Contents

Each running playback entry displays:

| Information | Description |
|-------------|-------------|
| **Name** | CUE name |
| **Type** | Cues / Sequences / Effects / Presets |
| **Status** | Running / Fading In / Fading Out / Paused |
| **Intensity** | Current output intensity (0-100%) |
| **Fade Progress** | Fade progress bar (0-100%) |

### Category Tabs

The top of the panel has category tabs for filtering display:
- **All** — Show all types
- **Cues** — Show only CUE playbacks
- **Sequences** — Show only sequences
- **Effects** — Show only frame effects
- **Presets** — Show only preset playbacks

### Quick Release

- Each running playback entry has a **Release** button on the right
- Click to quickly release that playback

---

# Chapter 3 Frame Editor

## 3.1 What Are Frame Effects

Frame Effects are a waveform-based **dynamic effect engine** corresponding to the "Phaser" function on MA3 consoles.

Frame effects enable fixture attributes to change continuously according to specific waveforms (sine, square, sawtooth, etc.), creating:

- Chase effects (multiple fixtures lighting/extinguishing sequentially)
- Rainbow effects (cycling color changes)
- Strobe effects (fast on/off)
- Pan/Tilt periodic swinging effects
- Beam breathing effects (Zoom gradient)
- And more combination effects

## 3.2 Frame Editor Panel Layout

```
┌──────────────────────────────────────────────────────────────────┐
│ Frame Name: [New Effect ▼]                   [▶ Play] [■ Stop]   │
├──────┬───────────────────────────────────────────┬──────────────┤
│      │                                           │              │
│ Left │          Waveform Display Area             │    Right     │
│ Tool │      (Real-time Waveform Preview)          │  Parameter   │
│ Bar  │                                           │    Panel     │
│      │                                           │              │
├──────┴───────────────────────────────────────────┴──────────────┤
│ Step Bar                                                        │
│ [Step 1: 100%] [Step 2: 0%] [A+ Add/Merge] [- Remove]           │
└──────────────────────────────────────────────────────────────────┘
```

## 3.3 Waveform Types

### Four Basic Waveforms

| Waveform | Shape | Attack | Decay | Typical Use |
|----------|-------|--------|-------|-------------|
| **Sine** | ∿ | 100% | 100% | Smooth breathing effects, gentle chase |
| **Rectangle** | ⊓ | 0% | 0% | Hard cut effects, strobe |
| **Sawtooth** | ⩘ | 100% | 100% | Linear gradient, one-way chase |
| **Cosine** | ∿(offset 90°) | 100% | 100% | Same as sine wave, different starting phase |

### Custom

When the Attack or Decay parameters are manually adjusted, the waveform automatically switches to Custom mode.

## 3.4 Global Parameters

> **Note**: The following global parameters serve as **inherited default values** when adding new attributes. All parameters support attribute-level independent control (see §3.7). Modifying global parameters also updates the corresponding values of all currently selected attributes.

### Speed

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **Speed** | 0.1 - 600 | 60 | Effect running speed |
| **Speed Unit** | BPM / Hz / Seconds | BPM | Speed unit |

**Unit Description**:

| Unit | Meaning | 60 = ? |
|------|---------|--------|
| **BPM** | Beats per minute | 1 cycle per second |
| **Hz** | Cycles per second | 60 cycles per second |
| **Seconds** | Seconds per cycle | 60 seconds per cycle |

**Right parameter panel quick buttons**: 30 / 60 / 120 / 240 BPM

### Width (Duty Cycle)

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **Width** | 0% - 100% | 100% | The proportion of effective working time for the waveform |

- **100%**: Waveform fills the entire cycle
- **50%**: Waveform is compressed to the first half of the cycle, with the second half held low
- **10%**: Very short pulse, suitable for strobe effects

**Right parameter panel quick buttons**: 10 / 25 / 50 / 100

### Attack (Rise Time)

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **Attack** | 0% - 100% | 100% | The proportion of transition time from low to high |

- **0%**: Instant jump (hard cut)
- **50%**: Medium speed rise
- **100%**: Full rise time (smoothest)

**Right parameter panel quick buttons**: 0 / 25 / 50 / 100

### Decay (Fall Time)

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **Decay** | 0% - 100% | 100% | The proportion of transition time from high to low |

- **0%**: Instant jump (hard cut)
- **50%**: Medium speed fall
- **100%**: Full fall time (smoothest)

**Right parameter panel quick buttons**: 0 / 25 / 50 / 100

### Phase Range

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **Phase Range** | 0° - 360° | 360° | The total phase distribution range among selected fixtures |

**Right parameter panel quick buttons**: 45° / 90° / 180° / 270° / 360°

- **360°**: Fixture phases are evenly distributed across a full cycle
- **180°**: Fixture phases occupy only half a cycle
- **0°**: All fixtures are synchronized (no chase effect)

### Measure

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **Measure** | 0.25 - 64 | 1 | The number of beats a full cycle contains |

- **1** (default): One beat completes one cycle
- **2**: Two beats complete one cycle (speed halved)
- **0.5**: Half a beat completes one cycle (speed doubled)

### Loop Mode

| Mode | Description |
|------|-------------|
| **Loop** | Continuous loop playback (default) |
| **Once** | Play once and stop |
| **PingPong** | Play back and forth (forward → reverse → forward...) |

## 3.5 Grouping Parameters (Matricks)

Grouping parameters control the phase relationship between fixtures, determining the "chase pattern" style.

### Wings (Mirrored Segments)

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **Wings** | 1 - 32 | 1 | Number of mirrored segments |

- **Wings = 1** (off): Phases are linearly distributed from 0° to PhaseRange by selection order
- **Wings = 2**: Fixtures are split in half; the right half's phases are mirror-flipped

**Example**: 8 fixtures, Phase Range = 360°

Wings = 1 (no mirroring):
```
Fixtures: 1    2    3    4    5    6    7    8
Phase:   0°  51°  103° 154° 206° 257° 309° 360°
Effect:  Sequential chase from left to right
```

Wings = 2 (left-right mirroring):
```
Fixtures: 1    2    3    4    5    6    7    8
Phase:   0°  90°  180° 270° 270° 180° 90°  0°
Effect:  Chase from edges toward center (or center toward edges)
```

### Block Size

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **Block** | 1 - 32 | 1 | Number of fixtures per block |

- **Block = 1** (default): Each fixture has an independent phase
- **Block = 2**: Every 2 fixtures share the same phase, treated as one group
- **Block = 4**: Every 4 fixtures form one group

**Example**: 8 fixtures, Block = 2
```
Fixtures:      1  2  3  4  5  6  7  8
Logical groups:[A  A] [B  B] [C  C] [D  D]
Phase:         0°     90°    180°   270°
Effect:        Every 2 fixtures move in sync
```

### Group Count

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **Group** | 1 - 32 | 1 | Split fixtures into multiple groups |

- **Group = 1** (default): All fixtures in one group
- **Group = 2**: Fixtures split into 2 groups, phases independently assigned within each group

**Example**: 8 fixtures, Group = 2
```
Fixtures: 1    2    3    4    5    6    7    8
Groups:  [------Group 1------]  [------Group 2------]
Phase:    0°  120° 240° 360°   0°  120° 240° 360°
Effect:    Two groups perform the same chase pattern
```

### Combined Usage

Block, Group, and Wings can be combined to create more complex effects:

**Example**: 12 fixtures, Wings=2, Block=2
```
Fixtures:  1   2   3   4   5   6   7   8   9   10  11  12
Block grp: [A  A] [B  B] [C  C] [C  C] [B  B] [A  A]
                                   ↑ Wings mirror point
Phase:     0°     120°   240°   240°   120°   0°
Effect:    Chase from edges to center, 2 fixtures in sync
```

## 3.6 Steps

### What Are Steps

Steps define the "keyframes" of the waveform. By default, a frame effect has 2 steps:

- **Step 1**: High value (e.g., 100%)
- **Step 2**: Low value (e.g., 0%)

The waveform transitions between steps (controlled by Attack/Decay for the transition curve).

### Adding Steps

- Ensure **no steps are selected** (default state)
- Click the **[A+]** button in the Step Bar
- A new step is added at the end, using the current Programmer values as the step's attribute values

### Merging/Updating Existing Steps

When a step is **selected**, the [A+] button behaves as a merge update:

1. Click to select an existing step in the Step Bar
2. Modify attribute values in the Programmer
3. Click the **[A+]** button
4. **Only overwrites** the currently active attributes of the currently selected fixtures in the Programmer, preserving other unmodified values in the step
5. New attributes are automatically appended to the target attribute list

> **Example**: A step already has Pan + Dimmer values for strobes and moving heads. Select only moving heads and modify Dimmer → click A+ → only updates the moving heads' Dimmer, preserving strobe values and moving heads' Pan.

### Deleting Steps

- Select the step to delete
- Click the **[- Remove]** button
- At least 1 step must remain

### Editing Step Values

- Click a step button in the Step Bar
- You can modify the step's attribute values (0-100%)

### Multi-Step Example

**3-step effect**: Red → Green → Blue cycle
```
Step 1: Red=100%, Green=0%,   Blue=0%     (Red)
Step 2: Red=0%,   Green=100%, Blue=0%     (Green)
Step 3: Red=0%,   Green=0%,   Blue=100%   (Blue)
→ Effect: Three-color cycling
```

### Step Width

Each step can have an independent width, controlling the percentage of a beat that step occupies:

| Width | Meaning |
|-------|---------|
| **100%** (default) | Occupies 1 full beat |
| **200%** | Occupies 2 beats (step duration is longer) |
| **50%** | Occupies half a beat (step duration is shorter) |

## 3.7 Attribute-Level Parameters

All effect parameters support **attribute-level independent control**. Each target attribute can have a completely independent set of parameters without sharing global settings.

### Waveform Parameters (Attribute-Level)

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **Width** | 0% - 100% | 100% | Independent duty cycle for this attribute |
| **Attack** | 0% - 100% | 100% | Independent rise time for this attribute |
| **Decay** | 0% - 100% | 100% | Independent fall time for this attribute |
| **Waveform Type** | Sine/Rectangle/Sawtooth/Cosine/Custom | Sine | Independent waveform type for this attribute |
| **Phase Offset** | -180° ~ +180° | 0° | Phase offset for this attribute, added on top of fixture phase distribution |

### Speed and Grouping Parameters (Attribute-Level)

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **Speed** | 0.1 - 600 BPM | 60 | Independent running speed for this attribute |
| **Phase Range** | 0° - 360° | 360° | Independent phase distribution range for this attribute |
| **Wings** | 1 - 32 | 1 | Independent mirror segment count for this attribute |
| **Block** | 1 - 32 | 1 | Independent block size for this attribute |
| **Group** | 1 - 32 | 1 | Independent group count for this attribute |

### Attribute Enable Switch

Each attribute also has an independent **Enabled** switch. When unchecked, effect calculation for that attribute is skipped during runtime, and the fixture's attribute remains static.

### Usage

After selecting one or more attribute tags in the Frame Editor panel, all parameters displayed in the right parameter panel are read and written per-attribute. Modifying any parameter only affects the currently selected attributes.

### Example

**Pan and Tilt using completely different effect parameters**:
- Pan: Speed=120 BPM, Attack=100%, Decay=100% (sine wave, fast smooth swing)
- Tilt: Speed=60 BPM, Attack=0%, Decay=0% (rectangle wave, slow hard-cut jump)
- Pan: Phase Range=360°, Wings=1 (full range chase)
- Tilt: Phase Range=180°, Wings=2 (half range mirrored chase)

## 3.8 Master Parameters

Global parameters that can be adjusted in real time while an effect is running:

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **Master Speed** | 0.1x - 10x | 1.0x | Speed multiplier. 2.0 = double speed |
| **Master Phase** | 0° - 360° | 0° | Global phase offset |
| **Master Intensity** | 0% - 100% | 100% | Effect intensity. Reducing to 0% softly fades out the effect |

## 3.9 Preview and Playback

### Starting Preview

- Click the **▶ Play** button at the top of the Frame Editor panel
- The effect is immediately applied to selected fixtures and starts running
- The waveform display area shows real-time waveform animation

### Stopping Preview

- Click the **■ Stop** button
- The effect stops, and fixtures return to their previous state

### Waveform Display Area

The large central area displays in real time:
- Waveform curve (white line)
- Step points (draggable nodes)
- Current phase positions of each fixture (colored dots)

## 3.10 Frame Presets Panel

The Frame Presets panel provides up to **255 slots** for storing system presets and user-defined presets.

### System Presets

The system includes common frame effect presets:

| Preset Category | Typical Presets |
|-----------------|-----------------|
| **Dimmer** | Chase, Breathe, Strobe |
| **Position** | Circle Scan, Swing H, Swing V |
| **Color** | Rainbow, Bi-Color, Tri-Color |
| **Beam** | Zoom Breathe, Iris Open/Close |
| **Combined** | Multi-attribute combination effects |

### Applying Presets

1. Select fixtures
2. Open the Frame Presets panel
3. **Click** a preset button
4. The effect is automatically applied and starts running

### Saving Custom Presets

1. Create a satisfying effect in the Frame Editor
2. Open the Frame Presets panel
3. **Right-click** an empty slot
4. Select **Store**
5. Enter a preset name
6. Select a preset category
7. Confirm to save

### Deleting Presets

1. **Right-click** a preset button
2. Select **Delete**
3. Confirm deletion

> **Note**: System presets cannot be deleted or modified.

---

> **Next**: [User Manual (5) Timeline Editor, Timecode and Layout View](SuperConsolePro_UserManual_Part5.md)
