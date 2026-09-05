# Show File and Undo

> Applies to SuperStage 26H2.6 and later ｜ Read first: [Console Overview](00_Console_Overview_en.md)

---

## 1. The Show File

The show file holds everything in the console: groups, presets, cues, effects, timelines, timecode, layouts and views.

Create, open and save it on the **Settings → Show** page (the ⚙ button at the right of the encoder bar opens settings). The console's record of the last opened show file lives in `SuperStage/Config/SuperConsolePro.ini`.

### 1.1 Versions

The current format version is **2.7**.

| Version | Change |
| --- | --- |
| From 2.3 | Slot appearance (name and colour) |
| From 2.4 | Effect pool |
| From 2.5 | Structs carry a block length prefix, so adding or removing fields no longer misaligns the whole file |
| From 2.6 | The timeline records its timecode frame rate |
| **From 2.7** | Six timeline fields now saved: timeline name, beat subdivision, audio track volume, clip-to-snapshot association, audio fade-in and fade-out durations |

### 1.2 Compatibility

> **This release reads every 2.x show file**, converting slot names and colours to the new appearance format automatically.
>
> **The other direction needs care.** An older build opening a 2.7 file will not report an error — it simply does not read the fields 2.7 added, and that content silently returns to its defaults. If a version rollback may be needed, **save a copy first**.

The show file is a proprietary format and is not interchangeable with third-party console show files.

---

## 2. Undo

Undo covers **every operation except settings**:

| Covers | What that means |
| --- | --- |
| Programmer and selection | Value changes, selecting fixtures, clearing the programmer |
| Groups / presets / cues | Store, update, rename, delete, copy, move |
| Effects | Steps, waveform, phase and every run parameter |
| View layouts | Adding and deleting windows, dragging and resizing, switching views |
| Fixture layout | Where fixtures sit on the layout |
| Timecode and timeline | Tracks, clips, timecode cues |
| Playback keys | Executor shortcut bindings |

Every entry on the undo stack carries a plain-language label (for example `Store Group 2`), so you can see which step you are about to undo before pressing Undo.

### 2.1 Merge Rules

| Situation | Behaviour |
| --- | --- |
| **A drag with a clear start and end** (encoder, fader) | The whole gesture records one entry. This is the most accurate kind — it *is* what the user thinks of as one action |
| **Discrete steps with no start or end** (wheel, repeated clicks) | Consecutive changes under the same merge key are combined within a **0.5 second** window. A pause that long counts as the end of an action |
| **Switching to another target** | The previous entry is closed off before a new one starts |
| **Nested operations** | Merged by depth: only the outermost scope pushes, and the label comes from it (`Store Group 2` rather than `Store Group`) |
| Deleting several objects | Merged into one undo |

**The undo stack holds at most 100 entries.**

**An operation whose before and after states are identical is not pushed** — pressing Full with nothing selected, for example. Pushing it would only light up the undo key without anything to undo.

> A pending merge is flushed before **any non-merging operation**. Otherwise "two wheel clicks, then delete a group" would record the delete's before-state as it was before the wheel clicks.

### 2.2 Not Covered by Undo

- **Settings** (the Show / Patch / DMX pages) do not enter the undo stack;
- **Playback itself does not enter the undo stack.** Running the timeline or hitting an executor pushes nothing — undo is reserved for edits you made yourself;
- **Opening a show file clears the undo stack**, so undo can never pull the previous show's content back in.

---

## 3. Related Documents

- [Console Overview](00_Console_Overview_en.md)
- [Cues and Playback](02_Cues_and_Playback_en.md)
- [Command Line](03_CommandLine_en.md)
