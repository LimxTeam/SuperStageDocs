# Super Screen — User Manual

## 1. Overview

**Super Screen** (formerly Super NDI Screen) is a media display tool provided by the SuperStage plugin. It inherits from the `ASuperMediaBase` media base class and supports two media source modes: **NDI real-time video streams** and **static textures**, displaying content on the surface of one or more Static Mesh Actors in the scene, enabling pre-visualization of LED screens, video walls, and other equipment.

### Inheritance

```
AActor → ASuperBaseActor → ASuperMediaBase → ASuperScreen
```

### Media Source Modes

| Mode | Description |
|------|------|
| **NDI** | Receives NDI® real-time video streams on the local network (from Resolume, disguise, OBS, etc.) |
| **Texture** | Uses static texture assets from the project, suitable for fixed images, standby screens, advertisement images, etc. |

### How It Works

1. The `ASuperMediaBase` base class selects the active texture (NDI video frame or static texture) based on **SourceMode**
2. Super Screen pushes the texture to dynamic materials by overriding `OnActiveTextureChanged()`
3. Dynamic materials are automatically applied to user-specified Static Mesh Actors
4. Post-processing parameters such as **keystone correction**, **color adjustment**, and **transparency** are supported

### Use Cases

- Content pre-visualization for LED screens/video walls
- Stage multimedia system design preview
- Live performance video screen layout planning
- Real-time preview of media server output
- Video source display in virtual production

---

## 3. How to Add to Scene

### 3.1 Basic Setup Steps

1. **Prepare display carrier** — Place one or more **Static Mesh Actors** in the scene as "screens". You can use a Plane, a face of a cube, or any custom-shaped mesh
2. **Place Screen Actor** — Search for **"Super Screen"** in the "Place Actor" panel and drag into the scene
3. **Associate screens** — In the Details panel, add the Static Mesh Actors from step 1 to the **ScreenMeshActors** array
4. **Select media source** — Set **SourceMode** (NDI or Texture), then select an NDI input name or specify a static texture
5. The image will automatically display on the associated mesh surfaces

### 3.2 About Target Static Mesh Actors

Super Screen itself **does not contain screen meshes**. It is a "media display" that requires you to manually specify one or more Static Mesh Actors as display carriers.

- **Supports multiple screens** — The same NDI video source can be displayed simultaneously on multiple meshes
- **Mesh shape is free** — Can be flat, curved, or even irregular meshes
- **Material auto-replacement** — The system automatically replaces the mesh's first material slot (Slot 0) with the NDI dynamic material

> **Tip**: It is recommended to use a simple Plane mesh as the LED screen carrier, adjusting its dimensions to match the actual LED screen aspect ratio.

---

## 4. Parameter Reference

### 4.1 Media Source Parameters (Inherited from SuperMediaBase)

| Parameter | Description | Default |
|------|------|--------|
| **SourceMode** | Media source mode: `NDI` (real-time video stream) / `Texture` (static texture) | NDI |
| **NDIInputSelection** | NDI input name to subscribe to (NDI mode only) | Empty |
| **StaticTexture** | Manually specified static texture (Texture mode only) | Empty |

> **NDI Mode**: Click the NDIInputSelection dropdown to select a configured NDI source. Automatically rebinds on change; image switches instantly.
>
> **Texture Mode**: Specify a project texture asset in StaticTexture. Suitable for fixed images, standby screens, advertisement images, etc.

### 4.2 Screen Parameters (SuperScreen)

#### ScreenMeshActors

- **Meaning**: **List of Static Mesh Actors** that receive and display media content
- **Type**: Array (multiple can be added)
- **Default**: Empty

> **Setup Method**:
> 1. Click the **"+"** button next to the array to add a new element
> 2. Click the eyedropper icon next to the element, then click the target Static Mesh Actor in the viewport
> 3. Or select a Static Mesh Actor from the scene via the dropdown list
> 4. Repeat the above steps to add multiple screens

---

### 4.3 Display Settings

#### Transparent

- **Meaning**: Toggles screen material between **transparent** and **opaque** modes
- **Type**: Toggle (boolean)
- **Default**: Disabled ❌ (opaque mode)

| Mode | Description | Use Cases |
|------|------|----------|
| **Disabled (Opaque)** | Video is completely opaque; black areas appear black | Standard LED screens, video walls |
| **Enabled (Transparent)** | Dark areas of the video become transparent, allowing objects behind the screen to be seen | Transparent LED screens, holographic effects, overlay projection |

#### Transparency

- **Meaning**: Controls the overall **transparency** (opacity) of the screen
- **Range**: 0.0 ~ 1.0
- **Default**: 0.95
- **Visible when**: Only shown when **Transparent** is enabled (hidden in opaque mode)

| Value | Effect |
|----|------|
| **0.95** | Near fully opaque (default, slightly transparent) |
| **0.5** | Semi-transparent (objects behind the screen are visible) |
| **0.0** | Completely transparent (video is invisible) |

#### Brightness

- **Meaning**: **Brightness multiplier** of the video image
- **Range**: 0.0 ~ unlimited
- **Default**: 1.0

| Value | Effect |
|----|------|
| **0.0** | Completely black screen |
| **1.0** | Original brightness |
| **2.0** | Doubled brightness |

#### Color

- **Meaning**: **Color filter** applied to the video image
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

### 4.4 Deformation (Keystone Correction)

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
3. Place a **Super Screen** Actor (position is unimportant, can be placed anywhere)
4. Select Super Screen and add the Plane from step 1 to **ScreenMeshActors**
5. Set **SourceMode** (NDI or Texture) and select the input source
6. The Plane surface will display the media content

### 5.2 Multi-Screen Configuration

**To display the same video source on multiple screens:**
1. Place multiple Static Mesh Actors (as different screens) in the scene
2. Create one Super Screen
3. Add all screens to the ScreenMeshActors array
4. All screens will simultaneously display the same content

**To display different media sources on different screens:**
1. Create multiple Super Screen Actors
2. Each Super Screen associates different ScreenMeshActors
3. Each Super Screen sets a different media source

### 5.3 Transparent LED Effect

1. Create a media source with transparent areas (e.g., white text on black background)
2. Place screen mesh and associate it with Super Screen
3. Enable **Transparent** mode
4. Dark/black areas of the video will become transparent; bright areas will be visible
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
- Each Plane associated with an independent Super Screen
- Each Super Screen receives a different NDI source

---

## 7. Troubleshooting

### 7.1 Screen Shows Black/No Image

1. **Check NDI source** — Confirm the NDI sender is running and broadcasting
2. **Check network** — Confirm sender and receiver are on the same LAN, firewall not blocking NDI ports
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
4. **Copy/paste safe** — When copying and pasting a Screen Actor, the system automatically creates an independent dynamic material instance for the new Actor, without sharing materials with the original
5. **Lifecycle management** — NDI subscriptions are automatically cancelled when deleting the Screen Actor or exiting the level; no dangling callbacks
6. **License verification** — Media reception requires a valid plugin license. Video frame processing and material updates will not execute without authorization
7. **GPU texture updates** — Video frames are asynchronously uploaded via GPU render commands, with minimal impact on main thread performance
