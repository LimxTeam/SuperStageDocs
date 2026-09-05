# SuperConsole Lighting Console Overview

> Applies to SuperStage 26H2.6 and later

SuperConsole is an editor-resident lighting console delivered with the main plugin. It is used to select fixtures, adjust attributes, store groups and presets, store cues, and play back with executors and the timeline — without an external console.

**Open**: SuperStage toolbar → **SuperConsole** (a top-level menu, below `SuperDMXTool`).

> From 26H2.6 SuperConsole is an editor module of the main plugin, not a standalone plugin. If the SuperConsole plugin was previously installed separately, remove it from the project `Plugins` directory before upgrading to avoid colliding with the module of the same name.

---

## 1. Where It Sits in the Chain

```text
                    ┌──────────────────┐
External console ──>│                  │
(Art-Net / sACN     │  SuperDMX buffer │────> scene fixtures
 input)             │                  │────> DMX Activity Monitor
                    └──────────────────┘
                             ▲
                             │  writes
                    ┌──────────────────┐
                    │   SuperConsole   │
                    └──────────────────┘
                             │
                             └─> sent to the network over the current protocol
                                 when DMX output is enabled
```

Console output is written into the SuperDMX internal buffer, so:

- Scene fixtures and the DMX Activity Monitor see it immediately;
- With network output enabled it is also sent to external devices over the current protocol;
- The console and an external console input write the same buffer. When both push the same channel, the last write wins — during integration, keep only one path.

There are **two different switches** here; do not confuse them:

| Switch | Where | What stops when it is off |
| --- | --- | --- |
| Console output | DMX Settings panel | The console's whole aggregation chain stops, so **scene fixtures stop updating too** |
| Network output | SuperDMX configuration panel | Only sending on the wire stops; scene fixtures still follow the console |

To previs locally without disturbing devices on site, turn off **network output**.

---

## 2. Interface

The console window is a customisable grid layout.

- **Add a panel**: **left-click an empty cell**; an Add Window dialog appears to pick the type;
- **Resize / move**: drag the window's title bar and corners;
- **Settings or delete**: click the **SS ▼** button at the right of a window's title bar to open its settings, which hold that panel's own options and, at the top right, the red **Delete Window**.

Add Window offers eleven panels:

| Panel | Purpose |
| --- | --- |
| Fixture Sheet | Fixture table showing current values per attribute |
| Color Picker | Colour picker |
| Preset | Preset slots |
| Group | Group slots |
| Playback | Executor grid, which is also the cue list |
| Running | What is currently running, tabbed by Cues / Effects / Presets / All |
| Frame Editor | Effect editor |
| Frame Presets | Effect template slots |
| Layout View | Fixture layout |
| Timecode | Timecode and timeline |
| DMX Settings | DMX output view and the output switch |

**Patch, show file and DMX settings are not grid panels.** They live in the settings dialog: click the **⚙** button at the right of the encoder bar along the bottom, which opens the Show / Patch / DMX pages.

The bottom of the window always carries three fixed rows: the **DESK command line**, the **encoder bar** and the **command keypad**.

---

## 3. Shortest Way In

1. Place fixtures in the scene and patch them (see [Patch Tools](../stagecore/07_Patch_Tools_en.md));
2. Open SuperConsole and confirm on the ⚙ → Patch page that the console sees those fixtures;
3. Select some fixtures and set intensity and colour in the programmer;
4. Store a group (`Store Group 1 Please`) so they are easy to select next time;
5. Store a cue (`Store Cue 1 Please`);
6. Click that slot's button in the Playback panel to play it back.

---

## 4. Documents in This Folder

| Document | Content |
| --- | --- |
| [01 Selecting and Programming](01_Programming_en.md) | Selection, programmer, groups, presets, slot appearance |
| [02 Cues and Playback](02_Cues_and_Playback_en.md) | Cues, executors, timeline, timecode, output arbitration, highlight |
| [03 Command Line](03_CommandLine_en.md) | The mode × target matrix, and which keys have a backend |
| [04 Effects](04_Effects_en.md) | The Frame effect engine |
| [05 Show File and Undo](05_ShowFile_and_Undo_en.md) | File format, versioning, undo behaviour |

---

## 5. Boundaries

SuperConsole is a **previs and programming tool, not a certified show console**. The current boundaries:

- No console hardware surface, no on-site redundancy or backup-console failover, and no guarantee of show-grade uninterrupted operation;
- The show file is a proprietary format and is not interchangeable with third-party console show files;
- The command line only offers what is actually wired up; keys without a backend stay greyed out (see [03 Command Line](03_CommandLine_en.md));
- Executors have no physical or on-screen faders; playback is by key;
- Blind / Solo were removed in this release;
- It does not replace on-site commissioning with a real console.

---

## 6. Related Documents

- [DMX System Overview](../stagecore/00_DMX_System_Overview_en.md)
- [DMX Network Configuration](../stagecore/01_DMX_Network_Configuration_en.md)
- [Patch Tools](../stagecore/07_Patch_Tools_en.md)
- [Fixture System Overview](../fixture/00_FixtureSystem_Overview_en.md)
