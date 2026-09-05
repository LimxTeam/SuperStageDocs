# 08 - DMX Recording & Playback

> **Module**: SuperTools / SuperDMX Sequencer  
> **Target Users**: Lighting programmers, virtual production technicians  
> **Prerequisites**: [01 - DMX Network Configuration](./01_DMX_Network_Configuration_en.md)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

SuperStage can use UE Take Recorder to record DMX buffers received by the SuperDMX subsystem into a Level Sequence. During playback, the SuperDMX track writes curve values back to the SuperDMX subsystem; if DMX output is enabled, those values are also sent to the network through the currently selected protocol.

This feature is meant for capturing DMX data that an external console sends into SuperStage. It is not a console project importer or a packet-level network recording tool.

---

## 2. Recording Entry Point

1. Open **Window > Cinematics > Take Recorder**.
2. Click **+ Source** in Take Recorder.
3. Add **Super DMX Input**.

The source display name is **Super DMX (Universes)**.

---

## 3. Recording Parameters

| Parameter | Description | Default | Range |
|-----------|-------------|---------|-------|
| **UniverseMin** | Minimum internal Universe to record | 1 | 1-512 |
| **UniverseMax** | Maximum internal Universe to record | 16 | 1-512 |

Notes:

- If UniverseMin is greater than UniverseMax, recording uses the smaller value first and the larger value last.
- The range uses SuperStage's internal Universe numbers. Input protocol Start Universe offset has already been applied by the SuperDMX input path.
- With sACN, recording temporarily joins multicast groups for the selected Universe range; after recording stops, that temporary subscription is cleared and the DMX configuration's default receive range is used again.

---

## 4. Start Recording

1. Enable input in the DMX Config Panel and confirm protocol, port, local IP, and Start Universe settings.
2. Use the DMX Activity Monitor to confirm that the target Universes have data.
3. Add **Super DMX Input** in Take Recorder and set UniverseMin and UniverseMax.
4. Click **Record** in Take Recorder.
5. Run the console playback or live operation, then click **Stop**.

Recording requires a valid SuperStage entitlement. The source checks authorization before creating DMX tracks; without authorization, recording tracks may not be created.

---

## 5. Recording Result

Recording creates or reuses SuperDMX tracks in the current Root Sequence MovieScene:

```
Level Sequence
  ├── Universes_1
  │     └── Section (Universe = 1)
  ├── Universes_2
  │     └── Section (Universe = 2)
  └── ...
```

Source-backed behavior:

- Each recorded Universe uses a separate SuperDMX Track.
- Track display names are `Universes_{Universe}`, for example `Universes_1`.
- Each Section records one Universe.
- Section data stores one curve entries by channel number 1-512.
- Keyframes use constant interpolation so DMX values are not smoothed.
- The recorder compares against the previous buffer and writes keys only for changed channels; it does not write all 512 channels on every frame.

---

## 6. Playback Behavior

When Sequencer plays, the SuperDMX Section evaluates curve values at the current time and asks the SuperDMX subsystem to send the buffer.

Playback rules:

- If the Section is marked as recording, it does not send data.
- Playback first reads the current Universe buffer as the baseline.
- Only channels with curves in the Section are overwritten.
- Channels without curves keep their current buffer values.
- The output Universe then goes through the DMX output Start Universe offset.
- If output is disabled, playback still updates the SuperDMX internal buffer, so the Activity Monitor and virtual fixtures can see changes.
- If output is enabled, SuperDMX sends Art-Net or sACN data according to the current protocol.

---

## 7. Sequencer Usage

The SuperTools module registers a **DMX Track** menu item in Sequencer, so you can manually add a SuperDMX Track. A manually added empty track needs a Section with channel curves before it can produce playback output.

This user manual does not present manual per-channel DMX curve editing as the recommended workflow, because the current implementation does not provide a complete user-facing channel curve editor. The reliable workflow is to record real input data through Take Recorder, then use normal Sequencer operations to trim, move, or remove sections.

---

## 8. File Size and Performance

Recording data size depends on:

- Universe range size
- Recording duration
- Take Recorder / engine Tick frequency
- How often DMX values actually change

The recorder skips unchanged channels, so final asset size cannot be estimated as "Universe count x 512 channels x every frame." To keep assets smaller, record only the Universe range you need.

---

## 9. FAQ

### Q: I cannot find Super DMX Input in Take Recorder.
Confirm that the SuperTools module is loaded and that the project has Take Recorder editor functionality enabled.

### Q: Recording did not create `Universes_` tracks.
Check that you have a valid SuperStage entitlement and that recording is using the Root Sequence. The source creates tracks on the Root Sequence MovieScene.

### Q: Tracks were created, but there is no channel data.
Use the Activity Monitor first and confirm that the selected Universe range contains non-zero or changing data. The recorder skips empty buffers and unchanged channels.

### Q: Virtual fixtures respond during playback, but external devices do not.
Check whether output is enabled in the DMX Config Panel, and verify output protocol, Local IP, Remote IP, port, and Start Universe. If output is disabled, playback only updates the internal buffer.

### Q: Can I use the Sequencer context-menu item "Start DMX Recording"?
**No.** Both **Start DMX Recording** and **Stop DMX Recording** on the track context menu are empty implementations — clicking them does nothing and reports nothing. Use the **Super DMX Input** Take Recorder source instead.

**DMX Track** (add a track) in the same menu does work.

---

> **Next Steps**: Read [09 - MA Console Export](./09_Export_To_MA_en.md).
