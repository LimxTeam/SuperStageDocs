# 06 - DMX Activity Monitor

> **Module**: SuperStageEditor (SDmxActivityMonitor)  
> **Target Users**: Lighting programmers, technical staff  
> **Prerequisites**: [01 - DMX Network Configuration](/docs/stage-core/dmx-network)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

The **DMX Activity Monitor** is a debugging tool that displays DMX channel values in real-time. It visually shows the current values of 512 channels in each Universe, helping you:

- **Verify Network Connection** — Confirm DMX signals from the console are being received successfully
- **Troubleshoot Address Issues** — Check if specific channel values are correct
- **Monitor Signal Status** — Observe DMX data changes in real-time

---

## 2. Opening the Activity Monitor

The Activity Monitor is **embedded in the lower area of the DMX Configuration Panel**. Open the DMX Configuration Panel to see it.

Specific steps:
1. Click the **DMX** button in the SuperStage toolbar
2. After the DMX Configuration Panel opens, the upper section has Input/Output settings, and **the lower section is the Activity Monitor**

---

## 3. Interface Layout

The Activity Monitor interface is divided into two parts:

### 3.1 Control Bar

Located at the top of the monitor, containing the following controls:

| Control | Description |
|------|------|
| **Monitor All** checkbox | When checked, displays all active Universes (Universes with data appear automatically) |
| **Universe** number input | When Monitor All is unchecked, specifies a single Universe number to monitor |
| **Clear** button | Clears DMX buffer data for all Universes (resets all channel values to zero) |

### 3.2 Channel Display Area

Located below the control bar, occupying the main area of the panel:

```
Universe 1
┌─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┐
│▓│▓│ │ │▓│▓│▓│ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │
│█│█│ │ │▓│█│▓│ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │
│█│█│ │ │█│█│█│ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │
└─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┘
 1  2  3  4  5  6  7  8  ...                                      512
```

---

## 4. Channel Value Display

Each channel is represented by a **grayscale bar**:

| Channel Value | Bar Height | Color |
|--------|-----------|------|
| 0 | None (blank) | — |
| 1 - 127 | Low to medium | Dark gray to medium gray |
| 128 - 254 | Medium to high | Medium gray to light gray |
| 255 | Max (full) | White |

**Bar Color Rules**:
- Higher channel value → taller bar, brighter color
- Channel value = 0 → no bar displayed
- Channel value = 255 → full-height white bar

---

## 5. Monitoring Modes

### 5.1 Monitor All Universes (Monitor All = On)

- Automatically displays all Universes with **active data**
- Each Universe occupies one row with the Universe number labeled above
- New Universes are automatically added as they receive data
- Suitable for quickly overviewing all DMX signal statuses

### 5.2 Monitor Single Universe (Monitor All = Off)

- Only displays the single specified Universe number
- Enter the Universe number to monitor in the **Universe** number input box
- Suitable for focused debugging of a specific Universe's channels

---

## 6. Data Refresh

The Activity Monitor **automatically refreshes periodically**, reading the latest channel data from the SuperDMX subsystem.

| Feature | Description |
|------|------|
| **Refresh Rate** | Automatic (follows editor Tick) |
| **Data Source** | USuperDMXSubsystem's DMX buffers |
| **Latency** | Very low (typically < 1 frame) |

> **Note**: The monitor displays values from SuperStage's internal buffers. If configured correctly, these values should exactly match those sent by the console.

---

## 7. Clear Button

Clicking the **Clear** button:

1. Clears **all Universe** buffers in the SuperDMX subsystem
2. Resets all channel values to zero
3. Monitor display becomes blank
4. Scene fixtures also return to default states as channel values become 0

**Use Cases**:
- Clear residual data before testing
- Confirm fixture behavior when all channels are at 0
- Troubleshoot "ghost data" (values that remain despite no longer being received)

> **Note**: Clear only clears SuperStage's internal buffers. If the console is still sending signals, buffers will be immediately filled with new data.

---

## 8. Usage Tips

### Quick Connection Verification

1. On the console, push all channels of Universe 1 to max (Dimmer = 255)
2. Check if Universe 1 in the monitor shows all full-height white bars
3. If display is correct → network connection is normal

### Locate Channel Addresses

1. On the console, only push one fixture's Dimmer channel (value set to 255)
2. Find the lit channel position in the monitor
3. That position is the fixture's actual DMX address
4. Set the fixture Actor's Start Address to that position (minus the Dimmer offset)

### Check Channel Conflicts

1. Set Monitor All to On
2. Push different fixture channels on the console separately
3. Observe if multiple fixtures' channel ranges overlap
4. Overlap = address conflict, needs re-Patch

---

## 9. FAQ

### Q: Monitor is completely blank, nothing shows?
1. Check if "Input" is enabled in the DMX Configuration Panel
2. Confirm the console is sending Art-Net signals
3. Confirm local IP is correctly selected
4. Check firewall settings

### Q: Only seeing data for some Universes?
- In Monitor All mode, only Universes with **active data** are displayed
- If the console isn't sending data for a certain Universe, that Universe won't appear in the monitor

### Q: Data looks wrong (values don't match)?
- Check if the Start Universe offset is correctly set
- There may be a Universe number offset causing incorrect correspondence between displayed Universes and the console

### Q: Bars flickering?
- This is normal, indicating the console is sending data at a certain frequency
- DMX signals typically update at 30-44 Hz

---

> **Next Steps**: Read [07 - Patch Tools](/docs/stage-core/patch-tools) to learn how to batch assign DMX addresses.
