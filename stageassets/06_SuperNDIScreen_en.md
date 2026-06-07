# Super NDI Screen — User Manual

## 1. Overview

**Super NDI Screen** is an NDI video stream reception and display tool provided by the SuperStage plugin. It can receive **NDI®** (Network Device Interface) video signals on the network and display them in real-time on the surface of one or more Static Mesh Actors in the scene, enabling pre-visualization of LED screens, video walls, and other equipment.

### What is NDI?

NDI is a network video transmission protocol developed by NewTek. Through NDI, you can transmit real-time video feeds from media servers (such as Resolume, disguise, WATCHOUT), video switchers, and even software like OBS over a local network into Unreal Engine and display them on "screens" within your scene.

### How It Works

1. An NDI sender (media server, etc.) broadcasts a video stream on the local network
2. Super NDI Screen subscribes to a specified NDI source through the **SuperNDI subsystem**
3. Received video frames (BGRA format) are updated to a dynamic texture
4. The dynamic texture is applied to user-specified Static Mesh Actors via **dynamic material instances**
5. Post-processing parameters such as **keystone correction**, **color adjustment**, and **transparency** are supported

### Use Cases

- Content pre-visualization for LED screens/video walls
- Stage multimedia system design preview
- Live performance video screen layout planning
- Real-time preview of media server output
- Video source display in virtual production

---

## 2. Prerequisites

Before using Super NDI Screen, ensure:

1. **NDI source available** — At least one NDI sender is broadcasting a video stream on the local network
2. **Network connectivity** — The computer running UE is on the same LAN as the NDI sender
3. **SuperNDI subsystem configured** — At least one input source has been added in the plugin's NDI configuration panel

> **Common NDI Senders**:
> - Resolume Arena/Avenue
> - disguise (d3)
> - WATCHOUT
> - OBS Studio (requires NDI plugin)
> - NDI Test Patterns (test signal generator included with NDI Tools)

---

## 3. How to Add to Scene

### 3.1 Basic Setup Steps

1. **Prepare display carrier** — Place one or more **Static Mesh Actors** in the scene as "screens". You can use a Plane, a face of a cube, or any custom-shaped mesh
2. **Place NDI Screen Actor** — Search for **"Super NDI Screen"** in the "Place Actor" panel and drag into the scene
3. **Associate screens** — In the Details panel, add the Static Mesh Actors from step 1 to the **Target Static Mesh Actors** array
4. **Select NDI source** — Select the NDI video source to receive from the **Input Name** dropdown list
5. The video image will automatically display on the associated mesh surfaces

### 3.2 About Target Static Mesh Actors

Super NDI Screen itself **does not contain screen meshes**. It is a "video receiver" that requires you to manually specify one or more Static Mesh Actors as display carriers.

- **Supports multiple screens** — The same NDI video source can be displayed simultaneously on multiple meshes
- **Mesh shape is free** — Can be flat, curved, or even irregular meshes
- **Material auto-replacement** — The system automatically replaces the mesh's first material slot (Slot 0) with the NDI dynamic material

> **Tip**: It is recommended to use a simple Plane mesh as the LED screen carrier, adjusting its dimensions to match the actual LED screen aspect ratio.

---

## 4. Parameter Reference

### 4.1 NDI Source Parameters

#### NDIInputSelection

- **Meaning**: The **NDI source name** to receive. This is the identifier name of the NDI sender broadcasting on the local network
- **Type**: Dropdown selection list (configured input sources + network discovered sources)
- **Default**: Empty (None)

> **Selection Method**: Click the dropdown arrow and the system will list all **configured input source** names in the current SuperNDI subsystem. Select the source you want to receive.

> **Auto-selection**: If Input Name is left empty (None), the system automatically uses the first source in the configuration list.

> **Switching sources**: After changing Input Name, the system automatically disconnects from the old source subscription and reconnects to the new source; the image switches instantly.

#### ScreenMeshActors

- **Meaning**: **List of Static Mesh Actor** that receive and display NDI video content
- **Type**: Array (multiple can be added)
- **Default**: Empty

> **Setup Method**:
> 1. Click the **"+"** button next to the array to add a new element
> 2. Click the eyedropper icon next to the element, then click the target Static Mesh Actor in the viewport
> 3. Or select from the dropdown list of Static Mesh Actors in the scene
> 4. Repeat above steps to add multiple screens

---

### 4.2 Display Settings

#### Transparent

- **Meaning**: Toggles screen material between **transparent** and **opaque** modes
- **Type**: Toggle (boolean)
- **Default**: Disabled ❌ (opaque mode)

| Mode | Description | Use Cases |
|------|------|----------|
| **Disabled (Opaque)** | Video completely opaque; black areas appear black | Standard LED screens, video walls |
| **Enabled (Transparent)** | Dark areas of the video become transparent, allowing objects behind the screen to be seen | Transparent LED screens, holographic effects, overlay projection |

> **Behavior on toggle**: Changing transparent mode automatically switches the underlying material (opaque ↔ transparent) and recreates the dynamic material instance.

#### Transparency

- **Meaning**: Controls the overall **transparency** (opacity) of the screen
- **Range**: 0.0 ~ 1.0
- **Default**: 0.95
- **Visible when**: Only shown when **Transparent** is enabled (hidden in opaque mode)

| Value | Effect |
|----|------|
| **0.95** | Near fully opaque (default, slightly transparent) |
| **0.5** | Semi-transparent (objects behind the screen visible) |
| **0.0** | Completely transparent (video invisible) |

#### Brightness

- **Meaning**: **Brightness multiplier** of the video image
- **Range**: 0.0 ~ unlimited
- **Default**: 1.0

| Value | Effect |
|----|------|
| **0.0** | Completely black screen |
| **1.0** | Original brightness |
| **2.0** | Doubled brightness |

> **Use Case**: Simulating LED screen brightness adjustment under different ambient lighting. Outdoor screens typically need higher brightness values.

#### Color

- **Meaning**: **Color filter** superimposed on the video image
- **Type**: Linear color (RGBA)
- **Default**: White (1, 1, 1, 1)

> **Usage**:
> - **White** (1,1,1) = No color change (default)
> - **Red** (1,0,0) = Red channel only
> - **Gray** (0.5,0.5,0.5) = Image dimmed by 50%
> - **Custom color** = Color-tint/tone the image

#### Contrast

- **Meaning**: **Contrast adjustment** of the video image
- **Range**: 0.0 ~ 1.0 (clamped to max 1.0 in editor)
- **Default**: 1.5 (constructor value, limited to 1.0 in editor)

| Value | Effect |
|----|------|
| **0.0** | No contrast (image fully gray) |
| **0.5** | Reduced contrast (softer image) |
| **1.0** | Maximum contrast (editor cap) |

---

### 4.3 Deformation (Keystone Correction)

Same keystone correction functionality as Super Projector, compensating for image distortion by adjusting four corner points.

Each corner point has two adjustment axes (X and Y), for a total of **8 adjustable parameters**.

#### Upper Left / Lower Left / Upper Right / Lower Right Corner

- **X component**: Controls vertical offset (0 = original position, 1 = moved to center)
- **Y component**: Controls horizontal offset (0 = original position, 1 = moved to center)

> **Use Case**: When video content has alignment deviation from the LED screen's physical pixels, fine-tune with keystone correction to perfectly align the image with the screen borders.
> **All values at 0.0** = No correction (original image mapping)

---

## 5. Usage Workflow

### 5.1 Quick Start

1. Place a **Plane** Static Mesh Actor in the scene as a screen
2. Adjust the Plane to suitable dimensions and position (e.g., 16:9 aspect ratio)
3. Place a **Super NDI Screen** Actor (position is unimportant, can be placed anywhere)
4. Select Super NDI Screen and add the Plane from step 1 to **Target Static Mesh Actors**
5. Select an NDI source from the **Input Name** dropdown
6. If everything is correct, the Plane surface will display the NDI video content

### 5.2 Multi-Screen Configuration

To display the same video source on multiple screens:

1. Place multiple Static Mesh Actors (as different screens) in the scene
2. Create one Super NDI Screen
3. Add all screens to the Target Static Mesh Actors array
4. All screens will simultaneously display the same video content

To display different video sources on different screens:

1. Create multiple Super NDI Screen Actors
2. Each Super NDI Screen associates different Target Static Mesh Actors
3. Each Super NDI Screen selects a different Input Name

### 5.3 Transparent LED Effect

1. Create an NDI video source with transparent areas (e.g., white text on black background)
2. Place screen mesh and associate it with Super NDI Screen
3. Enable **Transparent** mode
4. Dark/black areas of the video become transparent; bright areas are visible
5. Further adjust overall transparency via the **Transparency** parameter

---

## 6. Common Usage Examples

### Example 1: Standard Stage LED Main Screen (16:9)

- Screen carrier: Plane, dimensions 1600×900cm (16m × 9m)
- Transparent: Disabled
- Brightness: 1.0
- Color: White
- Contrast: 1.0
- Keystone: All 0.0

### Example 2: Curved LED Screen

- Screen carrier: Semi-cylindrical custom mesh
- Transparent: Disabled
- Brightness: 1.5 (curved screens typically need higher brightness)

### Example 3: Transparent Holographic Effect

- Screen carrier: Plane, placed vertically
- Transparent: Enabled
- Transparency: 0.8
- Brightness: 2.0
- Contrast: 1.5

### Example 4: Multi-Screen Stitched Video Wall

- Screen carrier: 9 Planes (3×3 arrangement)
- Each Plane associated with an independent Super NDI Screen
- Each Super NDI Screen receives a different NDI source (corresponding to each output of the video wall processor)

---

## 7. Troubleshooting

### 7.1 Screen Shows Black/No Image

1. **Check NDI source** — Confirm the NDI sender is running and broadcasting a signal
2. **Check network** — Confirm sender and receiver are on the same LAN, firewall is not blocking NDI ports
3. **Check Input Name** — Confirm the correct NDI source name is selected
4. **Check Target** — Confirm the Target Static Mesh Actors array contains the correct mesh
5. **Check license** — Confirm plugin authorization is valid

### 7.2 Image Lag or Stuttering

- NDI defaults to 1920×1080 reception resolution. Latency may occur if network bandwidth is insufficient
- Ensure wired gigabit network is used (WiFi not recommended)
- Reduce the number of simultaneously received NDI sources

### 7.3 Abnormal Image Color

- Check if the **Color** parameter is white (1,1,1)
- Check if **Brightness** and **Contrast** are at default values of 1.0
- Confirm the NDI source's color space is correctly set (sRGB)

### 7.4 Image Not Updating After Switching NDI Source

- After changing Input Name, the system automatically rebinds. If issues persist, try:
  1. Deselect and re-select the NDI source
  2. Or re-enter the level

---

## 8. Notes

1. **NDI reception resolution is fixed** — Current version receives NDI signals at a fixed 1920×1080 resolution; cannot be manually modified
2. **Color space is sRGB** — Received textures are forced to use sRGB color space
3. **Material slot replacement** — The system replaces the target mesh's **first material slot** (Material Slot 0); if the mesh has multiple material slots, only the first is replaced
4. **Copy/paste safe** — When copying and pasting an NDI Screen Actor, the system automatically creates an independent dynamic material instance for the new Actor, without sharing materials with the original
5. **Lifecycle management** — NDI subscriptions are automatically cancelled when deleting the NDI Screen Actor or exiting the level; no dangling callbacks
6. **License verification** — NDI reception requires a valid plugin license. Video frame processing and material updates will not execute without authorization
7. **GPU texture updates** — Video frames are asynchronously uploaded via GPU render commands, with minimal impact on main thread performance
