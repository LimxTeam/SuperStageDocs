# SuperConsolePro Lighting Console — User Manual (5)

## Timeline Editor · Timecode · Layout View

---

# Chapter 1 Timeline Editor (Timecode)

## 1.1 What Is the Timeline

The Timeline Editor is a tool used in the console to "arrange" lighting performances. It is similar to a timeline in video editing software: you can place CUEs and audio clips on the timeline so that they are automatically triggered at predetermined times, achieving precise synchronization between lighting and music.

### Core Concepts

| Concept | Description |
|---------|-------------|
| **Timecode** | An independent timeline. The timecode pool can contain multiple timelines |
| **Track** | A horizontal channel in the timeline. Divided into CUE tracks and audio tracks |
| **CUE Clip** | A CUE trigger event placed on a CUE track |
| **Audio Clip** | An audio playback event placed on an audio track |
| **Playhead** | The vertical line indicating the current playback position |
| **Marker** | A positioning mark on the timeline for quick navigation |

## 1.2 Panel Layout

```
┌──────────────────────────────────────────────────────────────────────────┐
│ [Timecode: Main Show ▼] [+New] │ [◀◀] [▶ Play] [■ Stop] [▶▶] │ 00:02:35  │
│ Speed: [1.0x ▼]  [Loop ☐]  [BPM: 120]  [Beat Grid ☑]                    │
├───────────┬──────────────────────────────────────────────────────────────┤
│           │  Time Ruler                                                   │
│           │  |0:00  |0:10  |0:20  |0:30  |0:40  |0:50  |1:00            │
│ Track List├──────────────────────────────────────────────────────────────┤
│           │  ▼ Playhead                                                    │
│ CUE Track1│  ┌────────┐    ┌──────┐         ┌──────────────┐           │
│ [M] [L]   │  │CUE 1.0 │    │CUE 2 │         │  CUE 3.0     │           │
│           │  └────────┘    └──────┘         └──────────────┘           │
│ CUE Track2│       ┌──────┐                                              │
│ [M] [L]   │       │CUE 4 │                                              │
│           │       └──────┘                                              │
│ Audio Track│ ┌──────────────────────────────────────────┐               │
│ [M] [L]   │ │  BackgroundMusic.wav                      │               │
│           │ └──────────────────────────────────────────┘               │
├───────────┴──────────────────────────────────────────────────────────────┤
│ Markers:  ▽ Intro    ▽ Verse 1    ▽ Chorus    ▽ Ending                   │
└──────────────────────────────────────────────────────────────────────────┘
```

## 1.3 Timecode Management

### Creating a Timecode

1. Click the **[+ New]** button at the top of the panel
2. Enter a timecode name (e.g., "Main Show", "Rehearsal Version")
3. A new empty timeline is created

### Switching Timecodes

- Click the timecode dropdown at the top
- Select the timecode to edit/play

### Deleting a Timecode

1. **Right-click** the timecode dropdown
2. Select **Delete**
3. Confirm deletion

### Renaming a Timecode

1. **Right-click** the timecode dropdown
2. Select **Rename**
3. Enter a new name

## 1.4 Timeline Configuration

### BPM (Beats Per Minute)

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **BPM** | 1 - 999 | 120 | Beats per minute, used for beat grid alignment |

- Directly enter the BPM value in the top toolbar
- BPM determines the spacing of the beat grid

### Beat Grid

| Parameter | Description |
|-----------|-------------|
| **Beat Grid** | Show/hide beat grid |
| **Subdivision** | Beat subdivision (1 = whole beat, 2 = half beat, 4 = quarter beat) |

- Check **Beat Grid** → beat grid lines appear on the timeline
- Clip start/end times can **snap to the beat grid**

### Loop Playback

| Parameter | Description |
|-----------|-------------|
| **Loop** | Enable/disable loop playback. When enabled, playback returns to the beginning after reaching the end |

### Total Duration

| Parameter | Description |
|-----------|-------------|
| **Duration** | Total timeline duration. Can be set manually and also extends automatically based on the last clip |

## 1.5 Track Management

### Adding Tracks

1. **Right-click** empty space in the track list area
2. Select:
   - **Add CUE Track** — adds a CUE track
   - **Add Audio Track** — adds an audio track
3. Enter a track name (optional)

### Deleting Tracks

1. **Right-click** the track name
2. Select **Delete Track**
3. Confirm deletion

> **Note**: Deleting a track also deletes all clips on that track.

### Renaming Tracks

1. **Right-click** the track name
2. Select **Rename**
3. Enter a new name

### Track Mute [M]

- Click the **[M]** (Mute) button to the left of the track
- When muted, clips on that track will not be triggered during playback
- Click again to unmute

### Track Lock [L]

- Click the **[L]** (Lock) button to the left of the track
- When locked, clips on that track cannot be edited (moved, resized, deleted)
- Click again to unlock

### Track Sorting

- Drag the track name area to reorder tracks vertically

## 1.6 CUE Clip Operations

### Adding CUE Clips

1. **Right-click** an empty area on a CUE track
2. Select **Add CUE Clip**
3. In the popup menu, select the CUE List and specific CUE
4. The clip appears at the clicked position

Alternatively:
- **Drag** a CUE button from the Playback panel onto a CUE track on the timeline

### Moving CUE Clips

- **Left-drag** the clip → moves it horizontally to a new time position
- If the beat grid is enabled, the clip snaps to the nearest beat

### Adjusting CUE Clip Length

- Hover the mouse over the **left or right edge** of the clip
- The cursor changes to a resize shape
- Hold left-drag → adjusts the clip duration

### Deleting CUE Clips

- Select the clip and press the **Delete** key
- Or **right-click** the clip → select **Delete**

### CUE Clip Behavior

When the playhead enters a CUE clip's range:
1. The corresponding CUE is automatically **Go**-ed (activated)
2. The CUE fades in according to the set Fade In time

When the playhead leaves a CUE clip's range:
1. The corresponding CUE is automatically **Released**
2. The CUE fades out according to the set Fade Out time

## 1.7 Audio Clip Operations

### Adding Audio Clips

1. **Right-click** an empty area on an audio track
2. Select **Add Audio Clip**
3. In the asset browser popup, select a Sound Wave asset
4. The audio clip appears at the clicked position

### Moving Audio Clips

- **Left-drag** the clip → moves horizontally

### Adjusting Audio Clip Length

- Drag the right edge of the clip → trims the end of the audio
- The clip cannot exceed the length of the original audio

### Splitting Audio Clips

1. **Right-click** the audio clip
2. Select **Split**
3. The clip is split into two independent clips at the clicked position
4. The two clips can be moved and edited independently

### Setting Audio Clip Volume

1. **Right-click** the audio clip
2. Select **Volume**
3. Drag the slider or enter a volume value (0% - 100%)

### Copy/Paste Audio Clips

- **Right-click** clip → **Copy**
- **Right-click** target position → **Paste**

### Deleting Audio Clips

- Select the clip and press the **Delete** key
- Or **right-click** → **Delete**

## 1.8 Playback Controls

### Playback Control Buttons

| Button | Shortcut | Function |
|--------|----------|----------|
| **▶ Play** | Spacebar | Start timeline playback |
| **❚❚ Pause** | Spacebar | Pause playback (press again to resume) |
| **■ Stop** | — | Stop playback and return to the beginning |
| **◀◀** | — | Jump to the previous marker |
| **▶▶** | — | Jump to the next marker |

### Playback Speed

| Speed | Description |
|-------|-------------|
| **0.25x** | Quarter speed (slow motion) |
| **0.5x** | Half speed |
| **1.0x** | Normal speed |
| **2.0x** | Double speed |
| **4.0x** | Quad speed (fast forward) |

- Adjust via the speed dropdown in the top toolbar
- Does not affect audio pitch (audio syncs with speed change)

### Seeking

- **Click** anywhere on the time ruler → playhead jumps to that position
- **Drag** the playhead → manually drag to a specific time
- Enter a precise time value → type in the time display box

### Timeline Zoom and Pan

| Operation | Method |
|-----------|--------|
| **Zoom** | Ctrl + Mouse Wheel / Shift + Mouse Wheel |
| **Horizontal Pan** | Mouse Wheel / Middle-click drag |

## 1.9 Markers

### Adding Markers

1. Move the playhead to the target time
2. **Right-click** the marker area (bottom of the panel)
3. Select **Add Marker**
4. Enter a marker name (e.g., "Intro", "Verse 1", "Chorus")

### Jumping to Markers

- **Click** a marker → playhead immediately jumps to the marker position
- Use the **◀◀ / ▶▶** buttons to jump between markers

### Moving Markers

- **Drag** the marker triangle → move to a new position

### Deleting Markers

- **Right-click** the marker → **Delete**

### Renaming Markers

- **Right-click** the marker → **Rename** → enter a new name

### Setting Marker Color

- **Right-click** the marker → **Set Color** → choose a color
- Different colors can visually distinguish different sections (e.g., green = section start, red = climax)

## 1.10 Exporting to Sequencer

The timeline can be exported as a Unreal Engine native **LevelSequence** asset for use in cutscene animation or final performance.

### Export Steps

1. Complete timeline editing
2. **Right-click** the timecode dropdown → **Export to Sequencer**
3. In the popup dialog, set:
   - **Save Path** — Location for the Sequencer asset
   - **Name** — Sequencer asset name
   - **Frame Rate** — Default 30 fps, options: 24/25/30/60
4. Click **Export**

### Export Contents

| Source Track Type | Exported As |
|-------------------|-------------|
| **CUE Track** | DMX keyframe track (DMX values baked per frame) |
| **Audio Track** | Sequencer audio track |
| **Markers** | Sequencer markers |

> **Tip**: Exporting is a one-time operation. You need to re-export after modifying the timeline.

---

# Chapter 2 Layout View

## 2.1 What Is the Layout View

The Layout View provides a **2D top-down view** of fixtures, allowing you to view and edit the spatial positions of fixtures on a flat plane. It is especially suitable for:

- Intuitively understanding the physical distribution of fixtures on stage
- Selecting fixtures in specific areas via drag selection
- Observing the real-time color and brightness status of fixtures
- Quickly selecting fixtures (more intuitive than the Fixture Sheet)

## 2.2 Panel Layout

```
┌──────────────────────────────────────────────────────────────┐
│ Layout: [Stage Top-Down ▼] [+New]  Tools: [Select] [Move] [Scale] │
│ Selection Direction: [Left→Right ▼]  [Reset Layout] [Add Fixtures]│
├──────────────────────────────────────────────────────────────┤
│                                                              │
│         ●1        ●2        ●3        ●4                    │
│                                                              │
│                   ●5                  ●6                    │
│                                                              │
│    ●7       ●8       ●9       ●10      ●11      ●12        │
│                                                              │
│                        Stage                                 │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

## 2.3 Layout Management

### Creating a Layout

1. Click the **[+ New]** button
2. Enter a Layout name (e.g., "Stage Top-Down", "Audience View")
3. A new empty Layout is created

### Switching Layouts

- Click the Layout dropdown to select a different Layout

### Adding Fixtures to a Layout

1. Select the fixtures you want to add in the Fixture Sheet
2. Click the **Add Fixtures** button in the Layout panel
3. Fixtures are automatically projected onto the 2D canvas based on their world coordinates in the scene

### Reset from World Coordinates

- Click the **Reset from World** button
- All fixture 2D positions are recalculated based on their 3D positions in the scene

### Deleting a Layout

1. **Right-click** the Layout dropdown
2. Select **Delete**

### Renaming a Layout

1. **Right-click** the Layout dropdown
2. Select **Rename**

## 2.4 Fixture Display

### Fixture Icons

Each fixture in the Layout is displayed as a **circular icon**:

- The circle's **color** reflects the fixture's current RGB output (updated in real time)
- The circle's **brightness** reflects the fixture's current Dimmer value
- The **Fixture ID** number is displayed next to the circle
- **Selected** fixture icons have a blue border

### Fixtures with Dimmer = 0

- When brightness is 0, the fixture icon is dimmed but still visible
- Can still be selected and edited normally

## 2.5 Canvas Operations

| Operation | Method |
|-----------|--------|
| **Pan Canvas** | Middle-click drag / Right-click drag |
| **Zoom Canvas** | Mouse wheel |
| **Reset View** | Double-click empty canvas area |

## 2.6 Fixture Selection

### Single Select

- **Left-click** a fixture icon → selects that fixture

### Multi-Select

- **Ctrl + Left-click** → add to selection

### Box Select

- **Hold left-click drag** on empty area → a rectangular selection box appears
- When released, all fixtures within the box are selected

### Selection Direction

Selection direction determines the **selection order** when fixtures are box-selected in the Layout. Selection order affects spread mode and frame effect phase distribution.

| Direction | Description |
|-----------|-------------|
| **Left to Right** | Sort from left to right (default) |
| **Right to Left** | Sort from right to left |
| **Top to Bottom** | Sort from top to bottom |
| **Bottom to Top** | Sort from bottom to top |
| **Center Out** | Spread outward from center |
| **Out to Center** | Contract inward from edges to center |

Switch via the **Selection Direction dropdown** at the top of the panel.

## 2.7 Fixture Position Editing

### Moving Fixtures

1. Switch to the **Move** tool
2. **Left-drag** a fixture icon → move to a new position

### Mapping Parameter Adjustment

Layout mapping parameters can be adjusted via the Encoder Bar:

| Parameter | Description |
|-----------|-------------|
| **Scale** | Overall scaling. Increase → fixture spacing increases |
| **Offset X** | Horizontal offset. Adjusts the horizontal position of all fixtures |
| **Offset Y** | Vertical offset. Adjusts the vertical position of all fixtures |
| **Rotation** | Rotation. Rotates the entire layout by an angle |

## 2.8 Removing Fixtures from Layout

1. Select fixtures in the Layout
2. Press the **Delete** key
3. The fixtures are removed from the current Layout (patch data remains unaffected)

---

# Chapter 3 Timecode CUE

## 3.1 Concept Description

A Timecode CUE is a mechanism for automatically triggering CUEs based on an **external timecode signal**. When the console receives a timecode signal from an external device (such as an audio workstation, video server, or SMPTE timecode generator), it can automatically trigger preset CUEs at the corresponding time points.

## 3.2 Use Cases

| Scenario | Description |
|----------|-------------|
| **Music Performances** | Lighting precisely synchronized with music, automatically triggered by music timecode |
| **Theater Performances** | Coordinated with the stage control system's timecode signal |
| **Multimedia Performances** | Synchronized with video servers, lighting follows video time changes |

## 3.3 Differences from Built-in Timeline

| Feature | Built-in Timeline | Timecode CUE |
|---------|-------------------|--------------|
| **Time Source** | Internal clock | External timecode signal |
| **Independence** | Fully autonomous playback | Follows external signal |
| **Precision** | Dependent on engine frame rate | Dependent on external timecode accuracy |
| **Applicable Scenario** | Independent performances, previews | Multi-device synchronized live performances |

---

> **Next**: [User Manual (6) DMX Output, Show File Management, Settings and Shortcuts Quick Reference](SuperConsolePro_UserManual_Part6.md)
