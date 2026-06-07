# SuperStage Drone Activity Monitor — User Manual

## 1. Overview

The Drone Activity Monitor (LDLink Activity Monitor) is used to view real-time status information of drone lighting devices connected via the LDLink protocol. The panel provides receiver toggle, network configuration, real-time statistics, and a drone data list.

---

## 2. Access

**Access**: Editor bottom status bar → Click the **LDLink** button

---

## 3. Interface Description

The panel is divided into four areas from top to bottom:

```
┌──────────────────────────────────────────────────────┐
│  ☑ Enable Receiver                                    │
│  Bind IP: [0.0.0.0 ▼]  Port: [5568 ↕]  [Clear]      │
│  ─────────────────────────────────────────────────── │
│  Drones: 12 | Packets: 120/s | Bytes: 4800/s |       │
│  Errors: 0                                           │
│  ─────────────────────────────────────────────────── │
│  ID   │ Status │ Position              │ Yaw  │ LED  │
│  001  │ Flying │ X:120.5 Y:340.2 Z:50  │ 45°  │ ■    │
│  002  │ Flying │ X:210.0 Y:180.0 Z:50  │ 90°  │ ■    │
│  003  │ Idle   │ X:0.0   Y:0.0   Z:0   │ 0°   │ ·    │
│  ...                                                  │
└──────────────────────────────────────────────────────┘
```

---

## 4. Receiver Configuration

### 4.1 Enable Receiver Checkbox

| State | Action |
|-------|--------|
| Checked | Start the LDLink data receiver; begin receiving drone status data |
| Unchecked | Stop the receiver |

When starting, the current Bind IP and Port configuration are automatically applied to `UDroneLinkSubsystem`.

### 4.2 Network Parameters

| Parameter | Control | Description | Default |
|-----------|---------|-------------|---------|
| **Bind IP** | Dropdown list | Bound local network interface (auto-enumerates all available network adapters) | 0.0.0.0 (all interfaces) |
| **Port** | Numeric input field | Listening port number (range 1 – 65535) | Read from `UDroneLinkSubsystem` |

### 4.3 Clear Button

Click **"Clear"** to clear all current drone data.

---

## 5. Real-Time Statistics

The statistics bar displays the receiver's operational status as a single line of text:

`Drones: {N} | Packets: {N}/s | Bytes: {N}/s | Errors: {N}`

| Statistic | Description |
|-----------|-------------|
| **Drones** | Total number of currently connected drones (additional prompt `showing {M}` when exceeding display limit) |
| **Packets** | Number of packets received per second |
| **Bytes** | Number of bytes received per second |
| **Errors** | Cumulative number of data parse errors |

---

## 6. Drone List

### 6.1 List Fields

| Column | Width | Description |
|--------|-------|-------------|
| **ID** | 40px | Unique drone identifier |
| **Status** | 60px | Current drone status |
| **Position** | 180px | Drone 3D position coordinates (X, Y, Z) |
| **Yaw** | 60px | Drone yaw angle |
| **LED** | 40px | Current LED light color preview |

### 6.2 Performance Limits

To control Slate UI performance, when the drone count exceeds an internal limit (`MaxDronesToShow`), the list only displays the first N drones, and the statistics bar shows the actual total.

---

## 7. Data Refresh

- The panel refreshes a drone data snapshot every **200ms**
- The list UI is only rebuilt when the drone set changes (join/leave)
- Numeric fields (Position, Yaw, LED) update in real time via Lambda bindings without rebuilding rows

---

## 8. Usage Scenarios

### Scenario 1: Pre-Show Check

1. Open the Drone Activity Monitor
2. Check **Enable Receiver** to start receiving
3. Confirm all drones appear in the list
4. Check whether Status and Position are normal

### Scenario 2: Live Performance Monitoring

1. Monitor the status of drones in flight
2. Observe whether LED colors match the pre-programmed design
3. Watch for abnormal Errors count

### Scenario 3: Troubleshoot Connection Issues

1. Check whether the Drones count is 0
2. Confirm whether Packets/s has data
3. Try changing Bind IP to a specific network adapter address

---

## 9. Notes

- This panel is read-only monitoring; drones cannot be controlled through this panel
- You must check Enable Receiver to begin receiving data
- The Bind IP dropdown automatically enumerates all local network interfaces
- Closing the panel does not stop the `UDroneLinkSubsystem` receiver
- The panel refreshes at 200ms (5Hz), which is not equivalent to the protocol receive rate

---

## 10. FAQ

| Issue | Solution |
|-------|----------|
| No drones in the list | Confirm Enable Receiver is checked and the port is set correctly |
| Packets/s is 0 | Check if Bind IP and Port match the drone sender |
| Errors continuously increasing | Possible incompatible packet format; check drone firmware version |
| Only partial drones shown | Exceeds MaxDronesToShow limit; the statistics bar shows the actual total |
| Data updates slowly | The panel refreshes at 200ms intervals; this is normal behavior |
