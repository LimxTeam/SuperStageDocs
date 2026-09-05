# 06 - DMX Activity Monitor

> **Module**: SuperTools — DMX Activity Monitor  
> **Target Users**: Lighting programmers, technical staff  
> **Prerequisites**: [01 - DMX Network Configuration](./01_DMX_Network_Configuration_en.md)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

The **DMX Activity Monitor** is a debugging tool for viewing SuperStage's internal DMX buffers. It shows channel value snapshots by Universe, helping you:

- **Verify Network Connection** — Confirm DMX signals from the console are being received successfully
- **Troubleshoot Address Issues** — Check if specific channel values are correct
- **Monitor Signal Status** — Observe whether DMX data is changing

---

## 2. Opening the Activity Monitor

The Activity Monitor is **embedded in the lower area of the DMX Configuration Panel**. Open the DMX Configuration Panel to see it.

Specific steps:
1. Click the **SuperDMX** button in the editor status bar at the bottom
2. After the DMX Configuration Panel opens, the upper section has Input/Output settings, and **the lower section is the Activity Monitor**

---

## 3. Interface Layout

The Activity Monitor interface is divided into two parts:

### 3.1 Control Bar

Located at the top of the monitor, containing the following controls:

| Control | Description |
|------|------|
| **All Universes** checkbox | When checked, displays all Universes that contain non-zero channel values |
| **Universe** number input | When All Universes is unchecked, specifies a single Universe number to monitor, range 1-512 |
| **Clear** button | Clears DMX buffer data for the current monitoring scope |

**The refresh rate is fixed at once every 0.1 s** and does not follow the engine frame rate — it shows a buffer snapshot, not a per-frame live value.

With nothing received at all, the panel shows `No DMX data yet. Check Input Enable and source.`

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

Each channel is represented by a fixed-height grayscale block and a numeric value:

| Channel Value | Grayscale Block | Number |
|--------|-----------|------|
| 0 | Black | 0 |
| 1 - 127 | Dark gray to medium gray | Current channel value |
| 128 - 254 | Medium gray to light gray | Current channel value |
| 255 | White | 255 |

**Display Rules**:
- Higher channel values appear brighter
- The block height is fixed and does not represent value
- The numeric value below each channel shows the 0-255 value directly

---

## 5. Monitoring Modes

### 5.1 Monitor All Universes (All Universes = On)

- Automatically displays all Universes with **non-zero channel values**
- Each Universe occupies one row with the Universe number labeled above
- New Universes are automatically added as they receive data
- Suitable for quickly overviewing all DMX signal statuses

### 5.2 Monitor Single Universe (All Universes = Off)

- Only queries the specified single Universe number
- Enter the Universe number to monitor in the **Universe** number input box
- If the selected Universe currently contains only zeros, the panel shows the no-data hint
- Suitable for focused debugging of a specific Universe's channels

---

## 6. Data Refresh

The Activity Monitor **automatically refreshes periodically**, reading the latest channel data snapshot from the SuperDMX subsystem.

| Feature | Description |
|------|------|
| **Refresh Rate** | About once every 0.1 seconds |
| **Data Source** | The DMX subsystem's receive buffers |
| **Displayed Range** | 1-512 channels by default |

> **Note**: The monitor displays values from SuperStage's internal buffers after protocol input and Universe offset handling. It is useful for confirming that SuperStage is receiving values, but it does not replace console-side or packet-level network diagnostics.

---

## 7. Clear Button

Clicking the **Clear** button:

1. With All Universes enabled, clears buffers for all Universes
2. With All Universes disabled, clears only the currently selected Universe buffer
3. Monitor display becomes blank
4. Scene fixtures reading those channels may change as the buffer values become 0

**Use Cases**:
- Clear residual data before testing
- Confirm fixture behavior when all channels are at 0
- Troubleshoot "ghost data" (values that remain despite no longer being received)

> **Note**: Clear only clears SuperStage's internal buffers. If the console is still sending signals, buffers will be immediately filled with new data.

---

## 8. Usage Tips

### Quick Connection Verification

1. On the console, raise a known channel
2. Check whether the matching Universe and channel value changes in the monitor
3. If it changes, SuperStage is receiving data for that channel

### Locate Channel Addresses

1. On the console, only push one fixture's Dimmer channel (value set to 255)
2. Find the lit channel position in the monitor
3. That position is the fixture's actual DMX address
4. Set the fixture Actor's Start Address to that position (minus the Dimmer offset)

### Check Channel Conflicts

1. Set All Universes to On
2. Push different fixture channels on the console separately
3. Observe if multiple fixtures' channel ranges overlap
4. If the same channel range is used by multiple fixtures, check whether that patch is intentional for your show

---

## 9. FAQ

### Q: Monitor is completely blank, nothing shows?
1. Check if "Input" is enabled in the DMX Configuration Panel
2. Confirm the console is sending Art-Net or sACN and that the protocol matches the input settings
3. Confirm local IP is correctly selected
4. Check firewall settings

### Q: Only seeing data for some Universes?
- In All Universes mode, only Universes with **non-zero channel values** are displayed
- If a Universe currently contains only zeros, it will not appear in the monitor

### Q: Data looks wrong (values don't match)?
- Check if the Start Universe offset is correctly set
- There may be a Universe number offset causing incorrect correspondence between displayed Universes and the console

### Q: Bars flickering?
- The display may change when channel values are changing or the sender is refreshing continuously
- The monitor itself refreshes about once every 0.1 seconds; the sender's actual refresh rate depends on the console, protocol, and network

---

> **Next Steps**: Read [07 - Patch Tools](./07_Patch_Tools_en.md) to learn how to batch assign DMX addresses.
