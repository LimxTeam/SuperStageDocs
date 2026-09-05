# Cues and Playback

> Applies to SuperStage 26H2.6 and later ｜ Read first: [Selecting and Programming](01_Programming_en.md)

This document covers storing and playing cues, how the result is decided when several playbacks push the same fixture, and the timeline and timecode.

---

## 1. Cues

A cue stores a snapshot of a lighting state.

- **Store** → **Cue** → number; command line `Store Cue 2`;
- Update: `Update Cue 2`;
- Copy / move: `Copy Cue 2 At 6`, `Move Cue 2 At 6`;
- Delete: `Delete Cue 2`.

**The Playback panel is the cue list.** One cue per slot, slots paged, with the page bar at the top of the panel. There is no separate Cues panel.

---

## 2. Executors

Every slot in the Playback panel is an executor: **press with the left button to trigger, release to end**. There are no faders — physical or on-screen — in this release.

**Right-click a slot** for its menu:

| Section | Item | What it does |
| --- | --- | --- |
| Trigger Mode | Toggle | One click on, another click off |
| | Momentary | Active while held, released on key up (with fade) |
| | Flash | Active while held, cut on key up (no fade) |
| Edit | Edit CUE | Load this cue back into the programmer to keep editing |
| | Update CUE (Replace) | Replace the whole cue with the current programmer |
| | Merge into CUE | Merge the programmer in: new fixtures and attributes are added, existing values updated |
| | Rename / Delete | Rename / delete |
| Copy / Move | Copy → Paste | Copy to the clipboard, paste on a target slot for a duplicate |
| | Move… → Move Here | Mark for move, then pick Move Here on the target slot; **timeline clips and shortcuts follow the cue automatically** |
| Shortcut | Set Shortcut… | Bind a keyboard shortcut to this slot |
| (empty slot) | Save CUE Here | Store the current programmer straight into this slot |

**Releasing one executor does not clear another executor's output on the same fixture.**

---

## 3. Output Arbitration

When several sources push the same attribute of the same fixture, they merge in three stages, in a fixed order:

```text
1. Default layer   The fixture definition's default values. Fallback only —
                   any playback or programmer value overrides it
       ↓
2. Playback layers timeline / timecode cue / cue / effect
       ↓              · Intensity attributes use HTP: highest wins,
                        regardless of source priority
                      · Everything else uses LTP: source priority first,
                        then timestamp within the same source
       ↓
3. Programmer      Overrides every playback above — the value you are
       ↓           holding must be visible
       ↓
4. Highlight       Overrides the programmer — "just show me which ones"
                   outranks everything
```

Key points:

- **The default layer does not take part in HTP.** An intensity channel whose definition default is non-zero would pin the output at that value forever if it competed for the maximum;
- **Intensity uses HTP deliberately.** When a base wash is covered by a darker effect, a real console takes the higher of the two. Flat overwriting would make the lower-priority value disappear entirely, and the base wash would get darker instead;
- **Layered by cue**: the same cue lands on the same layer whether driven by the Playback panel or a timeline clip; different cues occupy different layers;
- **Only real dimmer channels use HTP.** On an LED fixture with no dedicated Dimmer channel, the additive channels count as a **virtual dimmer**: they scale with the playback level, but they still merge by LTP (last write wins) rather than by maximum.

### 3.1 Highlight

The **Highlight** button on the encoder bar pushes the current selection to its highlight values (full intensity, beam open, no gobo) so you can pick those fixtures out on stage at a glance. It sits above the programmer, so a value you are editing cannot hide it. Press again to leave highlight.

The Running panel shows what is currently running, on four tabs: **Cues / Effects / Presets / All**.

---

## 4. Timeline

The timeline lives **inside the Timecode panel**; it is not a panel of its own.

- Two track kinds, **CUE** and **Audio**, added with `+ CUE` and `+ Audio` above the track area;
- Content is arranged by dragging clips;
- Overlapping clips on the same track release independently and do not clear each other's activation records;
- **Export Sequence** bakes the timeline's DMX and audio into a level sequence.

> Effects cannot go on the timeline by themselves. To run an effect from the timeline, wrap it in a cue and put that cue on a CUE track.

---

## 5. Timecode

**The frame rate is a property of the timeline itself**, and display and export read the same source.

Supported rates:

| Rate | Notes |
| --- | --- |
| 24 fps | Film |
| 25 fps | PAL / EBU |
| 29.97 fps | NTSC drop-frame |
| 30 fps | SMPTE |
| 50 fps | |
| 59.94 fps | Drop-frame |
| 60 fps | |

NTSC rates run on exact rationals (29.97 is 30000/1001, not a 29.97 float) with standard drop-frame numbering.

The display format is **HH:MM:SS:FF**, so it does not overflow past one hour. Drop-frame rates (29.97 / 59.94) follow the SMPTE convention of a semicolon before the frame field — **HH:MM:SS;FF** — so you can tell at a glance whether drop-frame is in play.

---

## 6. Common Questions

### Two playbacks push the same fixture and the result is not what I want

First identify the attribute type: intensity uses HTP (highest wins), everything else uses LTP (later write wins / higher priority wins). To make one always win, put it on a higher-priority source, or hold it in the programmer.

### A fixture is stuck on stage

Check the Running panel for an unreleased playback. Clearing the programmer does not release playbacks.

### I cannot tell which fixtures are selected

Press **Highlight** on the encoder bar and the selection jumps to its highlight values. Press again to leave.

### Timecode does not line up

Check the timeline's own frame rate and whether the external timecode source matches. 29.97 and 30 are not the same thing.

---

## 7. Related Documents

- [Console Overview](00_Console_Overview_en.md)
- [Selecting and Programming](01_Programming_en.md)
- [Effects](04_Effects_en.md)
- [Show File and Undo](05_ShowFile_and_Undo_en.md)
