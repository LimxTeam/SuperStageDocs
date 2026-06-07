# SuperStage SuperData Network Sync — User Manual

## 1. Overview

The SuperData Sync Panel (SuperData Sync v2.0) uses a **group chat architecture**, allowing multiple devices running different lighting software to exchange fixture data via SuperDataServer. You can obtain fixture configurations from other clients (Unity, MA3, Capture, etc.), map their types, and import them into the current UE scene with one click.

---

## 2. Access

**Main Menu Path**: Toolbar **SuperStage** dropdown menu → **SuperDMXTool** → **SuperDataImport**

The panel opens with a minimum size of 580 × 520 pixels.

---

## 3. Interface Description

The panel is divided into five areas from top to bottom:

```
┌──────────────────────────────────────────────────┐
│  SuperData Sync  v2.0                            │
├──────────────────────────────────────────────────┤
│  ● Connected           12 local fixtures [Connect]│
├──────────────────────────────────────────────────┤
│  Remote Clients (2)                   [Fetch Data]│
│  ┌──────────────────────────────────────────┐    │
│  │ Client         │ Platform  │ Fixtures    │    │
│  │ MA3-Console    │ grandMA3  │ 128         │    │
│  │ Capture-Laptop │ Capture   │ 64          │    │
│  └──────────────────────────────────────────┘    │
├──────────────────────────────────────────────────┤
│  Type Mapping  3/5 selected         [All] [None] │
│  ┌──────────────────────────────────────────┐    │
│  │ ☑│ Remote Type  │ Count │ Local Class    │    │
│  │ ☑│ Spot380      │ 24    │ BP_Spot380  ▼  │    │
│  │ ☑│ WashLED      │ 16    │ BP_WashLED  ▼  │    │
│  │ ☐│ Unknown      │ 8     │ (None)      ▼  │    │
│  └──────────────────────────────────────────┘    │
├──────────────────────────────────────────────────┤
│  40 of 48 fixtures ready to import    [Import to │
│                                         Scene]   │
└──────────────────────────────────────────────────┘
```

---

## 4. Connection Area

### 4.1 Connect Button

A **Connect / Disconnect** toggle button is located at the top-right of the panel:

| State | Button Text | Action |
|-------|-------------|--------|
| Disconnected | **Connect** | Connect to SuperDataServer (auto-starts if not running) |
| Connected | **Disconnect** | Disconnect |

> **Auto-Start**: When clicking Connect, if SuperDataServer is not running, the plugin will automatically start it.

### 4.2 Status Indicator

A circular status indicator and status text are located to the left of the button:

| Indicator Color | Status Text | Description |
|-----------------|-------------|-------------|
| Green | **Connected** | Successfully connected to the server |
| Red | **Disconnected** | Not connected |
| Orange | **Connecting...** | Connecting in progress |

### 4.3 Local Fixture Count

The status bar also displays the number of SuperStage DMX fixtures currently in the scene, in the format `{N} local fixtures`.

---

## 5. Remote Clients List

Displays **other clients** currently connected to the same SuperDataServer (excluding the local machine).

### 5.1 List Fields

| Column | Description |
|--------|-------------|
| **Client** | Client name (Tooltip shows Client ID) |
| **Platform** | The client's platform (e.g., grandMA3, Capture, Unity, etc.) |
| **Fixtures** | The number of fixtures this client has |

### 5.2 Operations

- **Select Client**: Single-click a client in the list to select it as the data source
- **Fetch Data**: Click the button at the top-right to request fixture data from the selected client

> When no clients are connected, the list area shows: "Click 'Connect' to join the SuperData network"  
> When connected but no other clients: "Waiting for other clients to connect..."

---

## 6. Type Mapping

When fixture data is received from a remote client, the Type Mapping area lists all received fixture types for you to perform local mapping.

### 6.1 List Fields

| Column | Description |
|--------|-------------|
| **☑** | Checkbox; whether to import this fixture type |
| **Remote Type** | The fixture type name from the remote client |
| **Count** | Number of fixtures of this type |
| **Local Actor Class** | Dropdown to select the local corresponding SuperStage fixture Blueprint class |

### 6.2 Batch Selection

| Button | Function |
|--------|----------|
| **All** | Select all fixture types |
| **None** | Deselect all |

The header row displays the count `{Selected}/{Total} selected`.

> When no data has been fetched yet, the area shows: "No fixture data received yet. Select a client above and click 'Fetch Data'."

---

## 7. Bottom Action Area

### 7.1 Import Count

The left side shows a live statistic: `{Importable} of {Total} fixtures ready to import`

Only fixtures whose type is checked **and** has a Local Actor Class specified are counted as importable.

### 7.2 Import to Scene Button

After clicking **"Import to Scene"**:

1. Iterate over all checked and mapped fixture types
2. Create corresponding Actors for each fixture in the scene
3. Set each fixture's position, rotation, DMX address, and other properties
4. The import operation executes within a UE Transaction, supporting **Ctrl+Z undo**
5. Display results via a UE notification popup upon completion

The button is only available when all of these conditions are met:
- Fixture data has been fetched (`PendingFixtures` is not empty)
- At least one type is checked and has a Local Actor Class specified

---

## 8. Workflow

### Scenario 1: Importing Fixtures from Other Software

1. Open the SuperData Sync Panel
2. Click **"Connect"** to connect to SuperDataServer
3. Wait for other clients (e.g., Unity, MA3) to join
4. Select the data source client in the Remote Clients list
5. Click **"Fetch Data"** to obtain fixture data
6. In the Type Mapping area, select a Local Actor Class for each fixture type
7. Check the fixture types to import
8. Click **"Import to Scene"** to import into the scene

### Scenario 2: Multi-User Collaboration

1. Lighting designer completes fixture layout in MA3/Capture
2. UE operator opens the SuperData panel and connects
3. Select the designer's client, Fetch Data
4. Map fixture types and import into the scene
5. After import, use SuperStage's Batch Patch Tool for further adjustments

---

## 9. Notes

- SuperDataServer is a standalone executable; it starts automatically when connecting
- All devices must be on the same local network
- Ensure the firewall allows SuperDataServer network communication
- Import operations create new Actors in the scene (they do not update existing Actors)
- Import operations support Ctrl+Z undo
- Closing the panel automatically disconnects and cleans up resources

---

## 10. Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| Cannot connect | SuperDataServer not started | Click Connect to auto-start; if it fails, start manually |
| Remote Clients is empty | Other clients not connected | Confirm other software has opened the SuperData panel and connected |
| Fetch Data has no response | No client selected | Select a client in the list first |
| Import button is disabled | Local Actor Class not specified | In Type Mapping, select a local Blueprint class for at least one type |
| Fixture positions are wrong | Coordinate system differences | Check the source client's coordinate units and axes |
| Fixture types cannot be mapped | Local fixture library lacks corresponding Blueprints | Create the corresponding fixture Blueprint class in the SuperStage fixture library first |
