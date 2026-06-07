# SuperStage DMX Activity Monitor — User Manual

## 1. Overview

The DMX Activity Monitor is a real-time DMX data viewing tool that visually displays Universe channel levels using grayscale bar graphs. It supports monitoring a single Universe or auto-scanning all active Universes, helping you quickly verify DMX signal reception and transmission.

---

## 2. Access

The DMX Activity Monitor is embedded in the bottom area of the **DMX Config Panel**.

**Access**: Editor bottom status bar → Click the **SuperDMX** button; after opening the DMX Config Panel, the activity monitor is visible at the bottom of the panel.

---

## 3. Interface Description

```
┌──────────────────────────────────────────────────────┐
│  ☑ All Universes   Universe [1    ↕]   [Clear]       │
│  ─────────────────────────────────────────────────── │
│  Universe          1..512                            │
│                                                      │
│  U 1  │ 1   2   3   4   5   6   7   8  ...         │
│        │ ██  ▓▓  ▒▒  ░░  ·   ·   ·   ·  ...         │
│        │255 128  64  32   0   0   0   0  ...         │
│                                                      │
│  U 2  │ 1   2   3   4   5   6   7   8  ...         │
│        │ ·   ·   ██  ·   ·   ·   ·   ·  ...         │
│        │  0   0 200   0   0   0   0   0  ...         │
│                                                      │
│  (Horizontal + Vertical scrolling)                   │
└──────────────────────────────────────────────────────┘
```

---

## 4. Feature Description

### 4.1 Monitor Mode

| Control | Description |
|---------|-------------|
| **All Universes checkbox** | Checked: Auto-display all Universes with signals; Unchecked: Show only the specified Universe |
| **Universe input field** | Select the Universe number to monitor (range 1 – 32767), supports drag adjustment |

### 4.2 Channel Display

Each Universe is displayed as a row, containing:
- **Left label**: `U {Number}`
- **Channel columns**: Each channel is 28 pixels wide, showing from top to bottom:
  - **Channel address** (1–512)
  - **Grayscale bar** (20 pixels high): Brightness = channel value / 255; higher values appear brighter
  - **Numeric value** (0–255)

The number of displayable channels is configurable (default 512, maximum 512).

### 4.3 Signal Filtering

- **All Universes mode**: Retrieves all known Universes from `USuperDMXSubsystem`, only displaying Universes with non-zero signals
- **Single Universe mode**: Only queries the specified Universe's buffer

When no DMX data is available, the prompt text appears: *"No DMX data yet. Check Input Enable and source."*

### 4.4 Clear Button

Click the **"Clear"** button to clear the DMX buffer:
- **All Universes mode**: Clears buffers for all Universes
- **Single Universe mode**: Only clears the currently selected Universe

---

## 5. Usage Scenarios

### Scenario 1: Verify Console Signal Reception

1. Configure input in the DMX Config Panel
2. Open the DMX Activity Monitor
3. Check All Universes or select the corresponding Universe
4. Move a fader on the console
5. Observe whether the grayscale bars and values for the corresponding channels change

### Scenario 2: Check Fixture Patch Correctness

1. Select the Universe where the fixture is located
2. Observe the channels starting from the fixture's start address
3. Compare whether the channel count matches the channel span defined in the fixture library

### Scenario 3: Troubleshoot DMX Signal Issues

1. If a fixture is not responding, first check in the monitor whether the corresponding channels have data
2. If the monitor has no data → Check the network connection and DMX input settings
3. If the monitor has data but the fixture doesn't respond → Check the fixture's patch settings

---

## 6. Notes

- The activity monitor refreshes data every **100ms**
- The UI is only rebuilt when the Universe set changes (to avoid frequent redraws)
- The monitor is read-only; DMX values cannot be modified through this panel (except Clear)
- A valid subscription is required to refresh data (without permission, data stays frozen without flickering)
- Universe numbers range from 1–32767 (consistent with the SuperDMX subsystem)
- Supports both horizontal and vertical scrolling to accommodate viewing large numbers of channels
