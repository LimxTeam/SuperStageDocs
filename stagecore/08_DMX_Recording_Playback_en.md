# 08 - DMX Recording & Playback

> **Module**: SuperDMX Sequencer / SuperStageEditor Recording  
> **Target Users**: Lighting programmers, virtual production technicians  
> **Prerequisites**: [01 - DMX Network Configuration](/docs/stage-core/dmx-network)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

SuperStage supports **recording** external DMX input signals as UE Sequencer animations and later **playing back** these animations while re-outputting DMX signals to the network.

### Core Capabilities

| Function | Description |
|------|------|
| **DMX Recording** | Record console-sent real-time DMX signals frame-by-frame to Sequencer tracks |
| **DMX Playback** | Play recorded DMX animations in Sequencer to drive virtual fixtures |
| **DMX Output** | Send DMX data via Art-Net / sACN to external real fixtures during playback |

### Typical Workflow

```
Console live programming of light show
       ↓
Record DMX signals using Take Recorder
       ↓
View/edit recorded animation in Sequencer
       ↓
Play Sequencer → Virtual fixtures respond + Output to real fixtures
```

---

## 2. Recording DMX (Using Take Recorder)

SuperStage records DMX data through UE's built-in **Take Recorder** system.

### 2.1 Preparation

1. **Ensure DMX input is enabled** — See [01 - Network Configuration](/docs/stage-core/dmx-network)
2. **Confirm DMX signals received normally** — Verify in Activity Monitor
3. **Open Take Recorder** — Menu bar Window > Cinematics > Take Recorder

### 2.2 Adding DMX Recording Source

1. In the Take Recorder panel, click the **+ Source** button
2. In the list that appears, find **Super DMX Input**
3. Click to add

After adding, you'll see a new recording source with configurable parameters:

| Parameter | Description | Default | Range |
|------|------|--------|------|
| **Universe Min** | Minimum Universe number to record | 1 | 1 - any positive integer |
| **Universe Max** | Maximum Universe number to record | 16 | 1 - any positive integer |

**Universe Range Notes**:
- System records all Universes from Universe Min to Universe Max (inclusive)
- E.g., Min=1, Max=4 records Universes 1, 2, 3, 4
- If Min > Max, system auto-swaps (e.g., Min=4, Max=1 is equivalent to Min=1, Max=4)
- Only Universes with actual data produce recording content

> **Suggestion**: Only record the Universe range you need to avoid generating oversized recording files. If your fixtures only use Universes 1-3, set Min=1, Max=3.

### 2.3 Starting Recording

1. Prepare your light show on the console
2. Click the **Record** button in Take Recorder
3. Execute the light show on the console (push faders, switch cues, etc.)
4. After recording is complete, click the **Stop** button

### 2.4 What Happens During Recording

During recording, the system performs the following operations:

1. **Per-frame sampling** — Follows engine Tick frequency (typically 30-120 fps), no throttling
2. **Read all channels** — For each Universe in the specified range, reads all 512 channel data from the SuperDMX subsystem
3. **Write keyframes** — Adds each channel's value as a keyframe to Sequencer curves
4. **Constant interpolation** — Keyframes use constant interpolation mode (RCIM_Constant) to ensure DMX values are precise (no smooth interpolation)
5. **Auto extend** — Section time range automatically extends to the current recording frame

### 2.5 Recording Protection Mechanism

During recording, the system sets the **bIsRecording = true** flag to prevent Sequencer playback from sending DMX data. This avoids the "echo" problem — where recorded data is sent out again, causing a signal loop.

---

## 3. Recording Results

After recording is complete, Take Recorder generates a **Level Sequence** asset containing DMX tracks.

### 3.1 Track Structure

```
Level Sequence
  └── SuperDMX Track (one independent track per Universe)
        ├── Universes_1  ← Universe 1 track
        │     └── Section (contains curve data for 512 channels)
        ├── Universes_2  ← Universe 2 track
        │     └── Section
        └── Universes_3  ← Universe 3 track
              └── Section
```

### 3.2 Track Naming

Each Universe track is automatically named `Universes_{Number}`, for example:
- `Universes_1`
- `Universes_2`
- `Universes_3`

### 3.3 Data Format

Each Section contains:

| Data | Description |
|------|------|
| **Universe** | The Universe number corresponding to this section |
| **Channel Curves** | 512 FRichCurves, each curve storing one channel's keyframe data |

Each curve's keyframes:
- **Timeline**: Based on MovieScene's TickResolution
- **Value**: 0-255 (uint8 DMX raw value)
- **Interpolation**: Constant mode (snap, no smoothing)

---

## 4. DMX Playback (Sequencer Playback)

### 4.1 Basic Playback

1. Double-click to open the recorded Level Sequence
2. Click the **Play** button in the Sequencer editor
3. Sequencer evaluates DMX curve data frame by frame
4. Evaluation results are automatically sent to the SuperDMX subsystem
5. Scene virtual fixtures respond in real-time

### 4.2 Playback Data Flow

```
Sequencer Playback
  ↓
DMX Template Evaluation (per frame)
  ↓
Read channel values at current time (from RichCurve curves)
  ↓
Build 512-byte DMX buffer
  ↓
Call SendDMXBuffer to send
  ↓
Virtual fixtures respond + Art-Net / sACN output to network
```

### 4.3 Buffer Merge Strategy

DMX buffer during playback uses a **merge strategy** (not overwrite):

1. First read the current state of that Universe as the baseline ("seed")
2. Only overwrite channels defined in Sequencer curves
3. Preserve channel values from other sources (e.g., external console input)

**This means**: You can record/edit only some channels in Sequencer, while other channels remain real-time controllable via the external console.

### 4.4 Outputting to External Devices

If **output is enabled** in the DMX configuration panel, playback DMX data is simultaneously sent via Art-Net / sACN to the network to drive real fixtures.

Ensure:
- Output is enabled in the DMX configuration panel
- Output local IP and remote IP settings are correct
- Real fixture Universe and addresses match those during recording

---

## 5. Editing DMX Data in Sequencer

Recorded DMX data is stored as standard UE Sequencer curves and can be edited using the Sequencer editor.

### 5.1 Available Operations

| Operation | Description |
|------|------|
| **Move Keyframes** | Adjust a specific channel's value at a certain time point |
| **Delete Keyframes** | Remove unnecessary data points |
| **Copy/Paste** | Copy data from one time period to another |
| **Modify Time Range** | Trim or extend Section playback range |
| **Modify Universe** | Change target Universe in Section properties |

### 5.2 Manually Creating DMX Tracks

You can also manually create DMX tracks in Sequencer without recording:

1. Click the **+ Track** button in Sequencer
2. Select **SuperDMX Track**
3. System creates an empty DMX track
4. Use **Add New Section** to add a section for a specified Universe
5. Manually add keyframes to channel curves

---

## 6. Recording Parameter Details

### 6.1 Take Recorder Recording Source Parameters

| Parameter | Description | Default |
|------|------|--------|
| **Universe Min** | Minimum Universe to record | 1 |
| **Universe Max** | Maximum Universe to record | 16 |
| **Track Tint** | Track color marker in Sequencer | Orange (255, 160, 0) |

### 6.2 Recording Precision

| Feature | Value |
|------|-----|
| **Sampling Frequency** | Follows engine Tick (typically 30-120 fps) |
| **Channel Precision** | 8-bit (0-255) |
| **Channel Count** | Up to 512 channels per Universe |
| **Interpolation Mode** | Constant (RCIM_Constant) |
| **Time Precision** | Follows MovieScene's TickResolution |

### 6.3 Recording File Size Estimation

Recording file size depends on Universe count, recording duration, and frame rate:

```
Approximate Size = Universe Count × 512 Channels × Per-Frame Keyframe Size × FPS × Recording Seconds
```

**Reference Values** (rough estimates only):

| Universe Count | Recording Duration | FPS | Estimated Size |
|--------------|---------|------|---------|
| 1 | 1 minute | 30 fps | ~10 MB |
| 4 | 5 minutes | 30 fps | ~200 MB |
| 16 | 10 minutes | 60 fps | ~3 GB |

> **Suggestion**: Only record the actual needed Universe range to avoid wasting space on idle Universes.

---

## 7. Advanced Use Cases

### 7.1 Mixing Recording + Real-Time Control

You can mix Sequencer recording playback with console real-time control:

- Sequencer plays back Universes 1-4 (pre-recorded light show)
- Console controls Universes 5-8 in real-time (live follow spot, etc.)
- Both don't interfere with each other

### 7.2 Multi-Recording Overlay

1. First recording: Universes 1-2 (moving lights)
2. Second recording: Universes 3-4 (LED panels)
3. Tracks from both recordings appear in the same Level Sequence
4. During playback, all Universes are driven simultaneously

### 7.3 Offline Editing After Recording

After recording is complete, you can disconnect the console and edit DMX curve data offline in Sequencer. Reconnect fixtures for playback output after editing.

---

## 8. FAQ

### Q: After recording, playback doesn't move fixtures?
1. Confirm the DMX track in Sequencer has data (expand track to view keyframes)
2. Check if fixture Universe and address match those during recording
3. Confirm not in recording state (bIsRecording should be false)

### Q: Recorded data doesn't match the console?
1. Check Start Universe setting in the DMX configuration panel
2. Use the Activity Monitor to confirm received raw data is correct
3. Confirm recorded Universe range covers target fixtures

### Q: Recording file too large?
- Reduce recorded Universe range (only record Universes with actual data)
- After recording, delete keyframes for empty channels
- Shorten recording duration

### Q: Stuttering during playback?
- Many Universes + high frame rate recording may cause performance issues
- Try lowering Sequencer playback frame rate
- Reduce number of simultaneously playing Universes

### Q: Does playback affect the console's real-time control?
Sequencer playback uses a **merge strategy**: only overwrites channels with data in curves, leaving other channels unchanged. If you need complete isolation, use different Universes.

### Q: Can Sequencer DMX data be exported?
The current version doesn't support directly exporting Sequencer DMX data as files. However, you can output data in real-time via Art-Net / sACN to external recording devices by enabling output during playback.

---

> **Next Steps**: Read [09 - MA Console Export](/docs/stage-core/export-ma) to learn how to export fixture configurations to MA consoles.
