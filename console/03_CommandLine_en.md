# Command Line

> Applies to SuperStage 26H2.6 and later ｜ Read first: [Console Overview](00_Console_Overview_en.md)

The command line is a "**mode × target**" matrix: pick what to do, then pick what to do it to. UI greying, key dispatch and actual capability all come from **one source of truth**, so there are no longer keys that can be pressed with no backend behind them.

---

## 1. Modes and Targets

**Modes** (what to do): Idle, Store, Update, Edit, Delete, Copy, Move, Assign, Select, On, Off, Stomp

**Targets** (what to do it to): Fixture, Group, Preset, Sequence, Cue, executor

> The executor key is **labelled `DESK`** on the keypad, not Executor. There is no key marked Executor to look for.

---

## 2. Combinations Available Now

| Mode | Available targets | Notes |
| --- | --- | --- |
| **Idle** | Fixture / Group / Preset / Cue / no object | Direct selection and recall; `At`, `On`, `Off` and other object-less commands go through here |
| **Select** | Group / Fixture | Selection |
| **Store** | Group / Preset / Cue | Store |
| **Update** | Group / Preset / Cue | Update |
| **Delete** | Group / Preset / Cue | Delete |
| **Copy** | Group / Preset / Cue | Copy to another slot |
| **Move** | Group / Preset / Cue | Move to another slot |
| **On / Off** | Acts on the current selection | No object required |

Combinations not in the table are **greyed out** in the interface: the `Edit`, `Assign` and `Stomp` modes, and the `Sequence` and executor (`DESK`) targets. Greying out rather than pressable-with-no-effect is deliberate — a key that does nothing on stage reads as "I must have pressed the wrong thing".

> Executor playback itself works; **assignment is done by right-clicking a slot in the Playback panel**, not from the command line.

**Blind / Solo were removed in 26H2.6.**

---

## 3. Syntax

### Copy and Move

```text
Copy Group 1 At 5
Move Cue 2 At 6
```

Before `At` is the source, after it the target slot. Groups, presets and cues are all supported.

### Selection

```text
Fixture 1 Thru 20
Group 3
```

### Store

```text
Store Group 5
Store Preset 2
Store Cue 10
```

---

## 4. Keys

| Key | Description |
| --- | --- |
| Digits / Thru | Object numbers and ranges |
| At | Separates source from target |
| Please | Commit |
| ESC | Cancel the command currently being entered |
| Clear | **Two meanings**: with an entry in progress it clears the entry; pressed again on an empty entry it clears the programmer |
| Undo / Redo | Undo / redo; greyed out when the stack is empty |

> Clear works in two stages to guard against slips: one accidental press only drops the few characters just typed, it does not wipe the values on stage.

A mode key can be pressed as long as it is implemented on at least one target; once pressed, targets with no backend are greyed out.

---

## 5. Related Documents

- [Console Overview](00_Console_Overview_en.md)
- [Selecting and Programming](01_Programming_en.md)
- [Cues and Playback](02_Cues_and_Playback_en.md)
- [Show File and Undo](05_ShowFile_and_Undo_en.md)
