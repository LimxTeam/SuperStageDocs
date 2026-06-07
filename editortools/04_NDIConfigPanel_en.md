# SuperStage NDI Config Panel — User Manual

## 1. Overview

The NDI Config Panel (Super NDI Config Panel) is used to manage SuperStage's NDI (Network Device Interface) video input sources. Through this panel, you can create logical NDI input channels, discover NDI video sources on the network, and map them to fixtures or LED screens in the scene.

---

## 2. Access

**Access**: Editor bottom status bar → Click the **SuperNDI** button

---

## 3. Interface Description

```
┌──────────────────────────────────────┐
│  NDI Input List                      │
│  ┌──────────────────────────────┐   │
│  │ Input 1: [NDI source dropdown]│   │
│  │ Input 2: [NDI source dropdown]│   │
│  │ Input 3: [NDI source dropdown]│   │
│  └──────────────────────────────┘   │
│                                      │
│  [+ Add Input]  [Apply to Selected]  │
└──────────────────────────────────────┘
```

---

## 4. Feature Details

### 4.1 NDI Input List

Each row represents a logical NDI input channel.

| Column | Description |
|--------|-------------|
| **Input Number** | Logical channel number (auto-assigned) |
| **NDI Source Selection** | Dropdown listing all NDI video sources discovered on the network |
| **Delete Button** | Remove this input channel |

### 4.2 NDI Source Dropdown

NDI source names displayed in the dropdown follow the format: `ComputerName (SourceName)`

For example:
- `LAPTOP-A1B2 (OBS Studio)`
- `WORKSTATION (Resolume Output 1)`

> **Note**: NDI source discovery refreshes asynchronously. If you don't see the expected source, wait a few seconds or check whether the NDI sender is running properly.

### 4.3 Add Input

Click the **"+ Add Input"** button to add a new empty NDI input channel at the bottom of the list.

### 4.4 Apply to Selected Actors

Select one or more Actors in the scene (e.g., LED screen, video panel), then click this button to apply the currently configured NDI input mapping to those Actors.

---

## 5. Workflow

### 5.1 Basic Configuration Flow

1. Open the NDI Config Panel
2. Click **"+ Add Input"** to create a new NDI input channel
3. Select an NDI video source from the dropdown
4. In the scene, select the Actor(s) that should receive NDI video
5. Click **"Apply to Selected"**
6. The selected Actors will begin displaying NDI video content

### 5.2 Multi-Input Configuration

If the scene has multiple LED screens that need to display different NDI sources:

1. Create multiple NDI input channels (e.g., Input 1, Input 2, Input 3)
2. Assign a different NDI source to each channel
3. Select different screen Actors separately and apply the corresponding input

---

## 6. Configuration Saving

- NDI input configuration is automatically saved to editor settings
- Automatically restored when the project is reopened

---

## 7. Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| NDI source dropdown is empty | No NDI senders on the network | Check if NDI sending software is running |
| NDI source dropdown is empty | Firewall blocking | Ensure the firewall allows NDI traffic (mDNS + NDI ports) |
| Video stuttering | Insufficient network bandwidth | Use gigabit wired network, reduce NDI video resolution |
| Cannot see a specific NDI source | Not on the same subnet | Ensure sender and receiver are on the same network segment |
| No video on Actor after applying | Actor type does not support NDI | Ensure the selected Actor is of a type that supports NDI input |
