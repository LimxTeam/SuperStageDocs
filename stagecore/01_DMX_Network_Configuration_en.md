# 01 - DMX Network Configuration

> **Module**: SuperDMX Configuration Panel (SSuperDMXConfigPanel)  
> **Target Users**: Lighting programmers, technical staff  
> **Prerequisites**: [00 - DMX System Overview](/docs/stage-core/overview)  
> **Last Updated**: 2026-04-14

---

## 1. Opening the DMX Configuration Panel

Click the **DMX** button in the **SuperStage toolbar** at the bottom of the UE editor to open the DMX configuration panel. The panel docks into the editor and you can adjust its position at any time.

The configuration panel is divided into two main areas:
1. **Upper section** — Input/Output configuration controls (single-row layout)
2. **Lower section** — DMX Activity Monitor (real-time channel value display)

---

## 2. Network Topology Overview

```
┌──────────────────────┐          ┌──────────────────────┐
│   MA Console (MA3 onPC)│          │   Real Fixtures /      │
│   IP: 192.168.0.100    │          │   Art-Net Nodes /      │
│                        │          │   DMX Decoders         │
└──────────┬───────────┘          └──────────▲───────────┘
           │ Art-Net (UDP 6454)              │ Art-Net (UDP 6454)
           │ or sACN (UDP 5568)               │ or sACN (UDP 5568)
           │ Sending DMX data                 │ Receiving DMX data
           ▼                                 │
┌──────────────────────────────────────────────────────────┐
│                  Ethernet Switch (Gigabit Recommended)     │
└──────────────────────────┬───────────────────────────────┘
                           │
                           ▼
              ┌────────────────────────┐
              │   UE Editor (SuperStage) │
              │   IP: 192.168.0.50      │
              │                        │
              │   ┌─ Input ───────────┐│
              │   │ Receive console   ││
              │   │ DMX signals       ││
              │   └───────────────────┘│
              │   ┌─ Output ──────────┐│
              │   │ Send DMX to       ││
              │   │ fixtures           ││
              │   └───────────────────┘│
              └────────────────────────┘
```

> **Key Requirement**: The console, UE editor, and fixtures/nodes must be on the **same Ethernet subnet** (e.g., `192.168.0.x`, subnet mask `255.255.255.0`). Use wired Gigabit connections; **avoid WiFi**.

---

## 3. Panel Layout Details

The upper section of the configuration panel is a compact row of controls, divided from left to right into the **Input area** and **Output area**, separated by a vertical line:

```
┌─ Input Area ─────────────────────────────────┬─── Output Area ───────────────────────────────────────┐
│ Input │ [Protocol▼] │ [LocalIP▼] │ ☑ Enable │ Start Universe [0] │ │ Output │ [Protocol▼] │ [LocalIP▼] │ ☑ Enable │ Start Universe [0] │
└───────────────────────────────────────────────┴──────────────────────────────────────────────────────┘
```

---

## 4. Input Configuration (Receiving External DMX Signals)

"Input" is used for **receiving** DMX signals from external consoles (e.g., MA2, MA3, grandMA3 onPC).

### 4.1 Protocol Selection

| Parameter | Description |
|------|------|
| **Dropdown Options** | Art-Net / sACN (E1.31) |
| **Default Value** | Art-Net |

The current version supports two DMX over Ethernet protocols:

| Protocol | Port | Description |
|------|------|------|
| **Art-Net** | UDP 6454 | Most commonly used DMX network protocol, widely compatible with all brand consoles |
| **sACN (E1.31)** | UDP 5568 | ESTA standard protocol, multicast-based transmission, supports more Universes |

> **Selection Advice**: If both console and devices support it, **sACN** is recommended (more efficient multicast transmission, no need to configure IP subnet). For MA series consoles, both Art-Net and sACN work.

### 4.2 Local IP Address

| Parameter | Description |
|------|------|
| **Dropdown Options** | All available NIC IP addresses on the current computer |
| **Default Value** | Empty (means listen on all NICs, i.e., 0.0.0.0) |

**How to select the correct IP**:

- If your computer has only one NIC → leave blank or select the only IP
- If your computer has multiple NICs (e.g., one connected to console, one to internet) → **must select the NIC IP on the same subnet as the console**
- Selecting the wrong NIC IP will result in not receiving DMX signals

**Example**:
- Console IP: `192.168.0.100`
- Your computer NIC 1: `192.168.0.50` (same subnet as console ✅)
- Your computer NIC 2: `10.0.0.5` (different subnet ❌)
- → Should select `192.168.0.50`

### 4.3 Enable Switch

| Parameter | Description |
|------|------|
| **Type** | Checkbox |
| **Default Value** | Checked (Enabled) |

- **Checked** → SuperStage begins listening on network ports, receiving DMX packets (Art-Net UDP 6454 or sACN UDP 5568)
- **Unchecked** → Stops receiving, saves network resources

> **Tip**: If you temporarily don't need to receive external DMX signals (e.g., only using Sequencer playback), you can uncheck this to reduce unnecessary network overhead.

### 4.4 Start Universe

| Parameter | Description |
|------|------|
| **Type** | Integer input box (supports drag adjustment) |
| **Range** | 0 - 32,767 |
| **Default Value** | 0 |

This is an **offset** used to map Art-Net Universe numbers from the network to SuperStage's internal Universe numbers.

**Mapping Formula**:
```
SuperStage Internal Universe = Network Universe - Start Universe + 1
```

**Common Settings**:

| Scenario | Start Universe | Effect |
|------|---------------|------|
| Default (MA2/MA3 standard) | **0** | Network Universe 0 → SuperStage Universe 1 |
| MA2/MA3 (Universe starts from 1) | **1** | Network Universe 1 → SuperStage Universe 1 |
| Skip first 10 Universes | **10** | Network Universe 10 → SuperStage Universe 1 |

**In most cases, keep the default value of 0**. Only adjust when your console's Universe numbering doesn't align with SuperStage.

**How to determine if adjustment is needed**:
1. On the console, set Universe 1, Channel 1 to 255
2. In SuperStage Activity Monitor, check Universe 1, Channel 1
3. If it shows 255 → alignment correct, no adjustment needed
4. If it shows 0 → need to adjust Start Universe value

---

## 5. Output Configuration (Sending DMX Signals to External Devices)

"Output" is used to **send** DMX data from SuperStage (typically from Sequencer playback) to external devices (e.g., real fixtures, Art-Net nodes).

### 5.1 Protocol Selection

Same as Input configuration, supporting both **Art-Net** and **sACN (E1.31)** protocols.

### 5.2 Local IP Address

| Parameter | Description |
|------|------|
| **Dropdown Options** | All available NIC IP addresses on the current computer |
| **Default Value** | Empty (uses default NIC) |

Select the NIC for sending DMX packets. Should select the NIC on the same subnet as the target device.

### 5.3 Enable Switch

| Parameter | Description |
|------|------|
| **Type** | Checkbox |
| **Default Value** | Checked (Enabled) |

- **Checked** → SuperStage can send DMX data via Art-Net / sACN
- **Unchecked** → No DMX data sent to the network

> **Note**: Even with Output enabled, data is only actually sent during Sequencer playback or other active DMX transmission operations. Simply enabling the switch does not generate additional network traffic.

### 5.4 Start Universe

| Parameter | Description |
|------|------|
| **Type** | Integer input box (supports drag adjustment) |
| **Range** | 0 - 32,767 |
| **Default Value** | 0 |

**Mapping Formula** (opposite direction from Input):
```
Network Universe = SuperStage Internal Universe + Start Universe - 1
```

**Examples**:

| SuperStage Universe | Start Universe | Sent to Network Universe |
|---------------------|---------------|----------------------|
| 1 | 0 | 0 |
| 1 | 1 | 1 |
| 5 | 0 | 4 |
| 5 | 1 | 5 |

> **Tip**: Input and output Start Universe should usually be set to the same value to ensure consistency.

---

## 6. Remote IP Address (Send Target)

The output remote IP address (where DMX packets are sent) defaults to **255.255.255.255** (broadcast mode), meaning sent to all devices on the same subnet. sACN uses multicast transmission; no remote IP configuration needed.

| Mode | Remote IP | Description |
|------|---------|------|
| **Broadcast** (default) | `255.255.255.255` | All devices on the same subnet can receive |
| **Unicast** | e.g., `192.168.0.100` | Only sent to the specified device |

> **Note**: Remote IP can be modified in the current version through the editor settings panel (Project Settings > Plugins > SuperDMX). Default broadcast works for most scenarios.

---

## 7. Port Settings

| Protocol | Port | Modifiable |
|------|------|----------|
| Art-Net | **6454** | Fixed (Art-Net standard port) |
| sACN (E1.31) | **5568** | Fixed (ESTA standard port) |

The Art-Net protocol specifies UDP port 6454; input and output share this port. The sACN protocol specifies UDP port 5568.

---

## 8. Auto-Save Configuration

All settings in the DMX configuration panel are **automatically saved** to the project configuration file. You don't need to save manually.

- **Save Location**: Project's `EditorPerProjectUserSettings` configuration
- **Save Timing**: Immediately saved and applied whenever any parameter is modified
- **Load Timing**: Automatically loads the last configuration when reopening the UE editor

---

## 9. Configuration Checklist

Before starting, check your configuration against the following checklist:

- [ ] Computer and console are connected on the **same Ethernet network** (same switch or router)
- [ ] Computer and console IPs are on the **same subnet** (e.g., both 192.168.0.x, subnet mask 255.255.255.0)
- [ ] **Input is enabled** in the DMX configuration panel
- [ ] **Local IP** has the correct NIC selected (the one on the same subnet as the console)
- [ ] **Start Universe** is correctly set (default 0 usually works)
- [ ] Windows Firewall **allows UE editor** UDP 6454 (Art-Net) or 5568 (sACN) port communication
- [ ] Console's Art-Net output is enabled and the target address includes your computer's IP (or broadcast address)

---

## 10. Troubleshooting

### Issue: Activity Monitor shows no data

**Check in order**:

1. **Physical connection**: Confirm Ethernet cable is connected, NIC indicator light is on
2. **IP configuration**: Run `ipconfig` in command prompt, confirm your IP is on the same subnet as the console
3. **Firewall**: Temporarily disable Windows Firewall for testing. If it works after disabling, add UDP 6454 (Art-Net) or 5568 (sACN) firewall allow rules
4. **Console settings**: Confirm console's Art-Net output is enabled
5. **Protocol match**: Confirm console output protocol matches the protocol selected in SuperStage configuration panel (Art-Net or sACN)
6. **Local IP selection**: Try switching the local IP in the configuration panel, or leave it blank (listen on all NICs)

### Issue: Receiving signals but fixtures don't move

1. **Check Universe**: Does the fixture's Universe setting match the Universe receiving signals in the Activity Monitor
2. **Check Address**: Does the fixture's Start Address match the address in the console
3. **Check Fixture Library**: Is the fixture correctly associated with a Fixture Library

### Issue: Signal delays

1. Confirm using **wired Ethernet** (do not use WiFi)
2. Check if other software is heavily using network bandwidth
3. Confirm using Gigabit switch (especially for multi-Universe scenarios)

---

## 11. Advanced Settings (Project Settings)

In addition to the quick settings in the DMX configuration panel, you can find more detailed settings in UE editor's **Project Settings > Plugins > SuperDMX**:

| Setting | Description |
|--------|------|
| **Protocol** | Network protocol (Art-Net / sACN) |
| **Input > bEnabled** | Input enable switch |
| **Input > LocalIp** | Local IP for input listening |
| **Input > RemoteIp** | Remote IP for input (usually broadcast address) |
| **Input > Port** | Input port (Art-Net: 6454 / sACN: 5568) |
| **Input > StartUniverse** | Input Start Universe offset |
| **Output > bEnabled** | Output enable switch |
| **Output > LocalIp** | Local IP for output |
| **Output > RemoteIp** | Output target remote IP |
| **Output > Port** | Output port (Art-Net: 6454 / sACN: 5568) |
| **Output > StartUniverse** | Output Start Universe offset |

These settings are fully synchronized with the controls in the DMX configuration panel; modifying either location automatically updates the other.

---

> **Next Steps**: Read [02 - Fixture Library](/docs/stage-core/fixture-library) to learn how to create and configure fixture channel definitions.
