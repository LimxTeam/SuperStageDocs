# SuperNDI User Manual

> **Version**: SuperStage 26Q1
> **Last Updated**: 2026-03-05
> **Applicable Engine**: Unreal Engine 5.6 ~ 5.7
> **Target Audience**: Stage lighting designers, virtual production technicians, LED screen operators

---

## Table of Contents

1. [Overview & Core Concepts](#1-overview--core-concepts)
2. [System Requirements & Prerequisites](#2-system-requirements--prerequisites)
3. [NDI Configuration Panel (SuperNDI Config)](#3-ndi-configuration-panel-superndi-config)
4. [NDI Editor Global Settings (Project Settings)](#4-ndi-editor-global-settings-project-settings)
5. [NDI Screen Actor (SuperNDIScreen)](#5-ndi-screen-actor-superndiscreen)
6. [Take Recorder Recording NDI Video](#6-take-recorder-recording-ndi-video)
7. [Sequencer Playback NDI Recording](#7-sequencer-playback-ndi-recording)
8. [FAQ & Troubleshooting](#8-faq--troubleshooting)
9. [Performance Optimization Tips](#9-performance-optimization-tips)
10. [Glossary](#10-glossary)

---

## 1. Overview & Core Concepts

### 1.1 What is SuperNDI?

SuperNDI is the **NDI video stream input module** within the SuperStage plugin, allowing you to:

- **Receive in real time** video from any NDI source on the LAN (such as OBS, vMix, NDI cameras, Resolume, etc.) within the Unreal Engine Editor and runtime
- **Project video onto LED screens/projection screens** in 3D scenes for virtual production previz
- **Record** NDI video streams to Sequencer timelines as playable frame sequences
- **Play back** pre-recorded NDI video without needing a live NDI source

### 1.2 Core Workflow

```
External NDI Source (OBS/vMix/Camera etc.)
        │
        ▼
   LAN NDI Transport
        │
        ▼
  SuperNDI Config Panel (add logical inputs, map sources)
        │
        ▼
  SuperNDIScreen Actor (LED screen in scene)
        │
        ├──→ Live Preview (Editor/Runtime)
        ├──→ Take Recorder Recording (save frame sequences)
        └──→ Sequencer Playback (offline previz)
```

### 1.3 "Logical Input Name" & "External Source"

SuperNDI uses a **logical name mapping** mechanism, which is key to understanding the entire system:

| Concept | Description | Example |
|---------|-------------|---------|
| **Logical Input Name** (InputName) | A custom name you define to reference one NDI input in your project | `MainScreen`, `CameraA`, `SidePanel` |
| **External Source** (ExternalSource) | The display name of an actual NDI device/software on the network | `OBS (DESKTOP-ABC)`, `vMix - Output 1` |
| **Mapping** | Binding a logical name to an external source | `MainScreen` > `OBS (DESKTOP-ABC)` |

**Why this design?**

- External source names may change with network environment (different computer names, different devices)
- Logical names remain constant in your project; screen Actors in the scene only need to care about the logical name
- When switching to a different NDI source device, just re-map in the config panel without modifying any Actors in the scene

---

## 2. System Requirements & Prerequisites

### 2.1 Hardware Requirements

| Item | Minimum | Recommended |
|------|---------|-------------|
| **CPU** | SSE 4.2 instruction set support | Intel i7 / AMD Ryzen 7 or above |
| **Memory** | 16 GB | 32 GB or above (more needed for recording) |
| **Network** | Gigabit Ethernet | 10 Gigabit Ethernet (multi-channel 4K NDI) |
| **GPU** | DirectX 12 support | RTX 3060 or above |

### 2.2 Network Configuration

NDI uses mDNS (UDP port 5353) for source discovery and TCP for video data transmission. Ensure:

1. **All NDI devices are on the same LAN/subnet** or cross-subnet routing is configured
2. **Windows Firewall** allows the following ports:
   - **UDP 5353** (mDNS source discovery)
   - **TCP/UDP 5960-5969** (NDI data transfer)
3. **Bonjour service** is running (typically installed automatically with NDI Runtime)
4. If using VPN or virtual NICs, confirm mDNS broadcasts are not blocked

### 2.3 NDI Runtime

The SuperNDI plugin **already includes the NDI SDK DLL** (`Processing.NDI.Lib.x64.dll`); no additional NDI Runtime installation is required.

DLL file search order (highest priority first):
1. `Plugins/SuperStage/Binaries/Win64/Processing.NDI.Lib.x64.dll`
2. `Plugins/SuperStage/Source/SuperNdi/ThirdParty/NDI/Bin/x64/`
3. `Plugins/SuperStage/Source/SuperNdi/ThirdParty/NDI/Bin/` (fallback path)

> **Important**: SuperNDI **will not use** NDI DLLs installed in the system PATH (such as those bundled with NDI Tools 6) to avoid version conflicts.

### 2.4 Compatibility with NDI Tools 6

Possible conflicts if NDI Tools 6 (including NDI Studio Monitor) is installed:

- **NDI Studio Monitor source contention**: Close Studio Monitor before launching UE Editor
- **DLL version conflict**: SuperNDI has its own integrated DLL and ignores system-installed versions
- **Finder conflict**: Very rare cases where two NDI Finders running simultaneously interfere with discovery

**Recommendation**: Close standalone NDI tools like NDI Studio Monitor when using SuperNDI.

---

## 3. NDI Configuration Panel (SuperNDI Config)

The configuration panel is the **first step** in using SuperNDI — this is where logical inputs are managed and external sources are mapped.

### 3.1 Opening the Config Panel

Find the **SuperNDI** button in the UE Editor **main toolbar** (next to the SuperDMX config button) and click to open the configuration panel.

The panel opens as a standalone tab and can be docked anywhere in the editor.

### 3.2 Panel Interface

The configuration panel consists of:

```
+-------------------------------------------------+
|  NDI Inputs   [ Enter logical input name... ]   |
|               [Add]    [Refresh Sources]         |
+-------------------------------------------------+
|  Configured Inputs:                              |
|                                                  |
|  MainScreen    [v OBS (DESKTOP-ABC)]   [Remove]  |
|  CameraA       [v <None>            ]   [Remove]  |
|  SidePanel     [v vMix - Output 1   ]   [Remove]  |
+-------------------------------------------------+
```

### 3.3 Adding Logical Inputs

1. Enter a **logical input name** in the text input at the top (e.g., `MainScreen`)
2. Click the **Add** button
3. The new logical input appears in the "Configured Inputs" list

**Naming suggestions**:
- Use meaningful English names (e.g., `MainScreen`, `CameraA`, `BackDrop`)
- Names cannot be empty
- Names cannot be duplicated (duplicates are silently ignored)
- Names are globally unique within the project; all SuperNDIScreen Actors share the same set of logical inputs

### 3.4 Mapping External NDI Sources

Each logical input has a **dropdown menu** on the right for selecting the external NDI source to bind:

1. Click the dropdown arrow
2. The panel automatically refreshes the NDI source list from the network
3. Select the target source from the list (e.g., `OBS (DESKTOP-ABC)`)
4. Takes effect immediately; the NDI receiver auto-creates and begins receiving video

**Dropdown options**:
- **`<None>`**: Don't bind to any external source (clear mapping)
- **`DeviceName (HostName)`**: NDI sources discovered on the network

### 3.5 Refreshing Source List

Click the **Refresh Sources** button to force re-scan the network for NDI sources.

- Refresh is **asynchronous** and does not block the editor
- First scan may take 2-3 seconds to discover all sources (mDNS broadcast latency)
- An initial scan auto-executes when the panel opens
- Opening the dropdown also triggers a background refresh

> **Tip**: If a new NDI source comes online, wait about 2-5 seconds then refresh to see it.

### 3.6 Deleting Logical Inputs

Click the **Remove** button at the end of the corresponding row to delete that logical input.

- After deletion, all SuperNDIScreen Actors referencing that logical name will stop receiving video
- Deletion is immediately saved to project settings
- If that logical name was being recorded by Take Recorder, recorded data is not lost

### 3.7 Configuration Persistence

All configurations (logical input names, external source mappings) are saved in the **project's editor settings** (EditorPerProjectUserSettings), meaning:

- Configuration persists after closing and reopening the editor
- Configuration is **project-level**; different projects have different configurations
- Configuration is **user-level**; different users of the same project can have different configurations

---

## 4. NDI Editor Global Settings (Project Settings)

In addition to the config panel, SuperNDI's global parameters can also be managed through the UE Editor's **Project Settings**.

### 4.1 Opening the Settings Page

Menu bar > **Edit** > **Project Settings** > Find **Plugins** category in the left list > **SuperNDI Settings**

### 4.2 Setting Item Details

#### 4.2.1 Inputs

Same content as the config panel, displayed in table form:

| Field | Type | Description |
|-------|------|-------------|
| **InputName** | Name | Logical input name |
| **ExternalSource** | Name | Display name of mapped external NDI source (empty = unmapped) |

Logic inputs can be added/edited/deleted here directly, with the same effect as the config panel.

#### 4.2.2 DefaultTargetFPS

| Item | Description |
|------|-------------|
| **Type** | Integer |
| **Range** | 1 ~ 120 |
| **Default** | 30 |
| **Purpose** | Default frame rate when creating new Take Recorder recording sources |

> **Recommendation**: 30 fps suits most scenarios. Set to 60 if the NDI source itself is 60 fps and high-precision sync is needed.

#### 4.2.3 DefaultDownscale

| Item | Description |
|------|-------------|
| **Type** | Float |
| **Range** | 0.25 ~ 1.0 |
| **Default** | 1.0 |
| **Purpose** | Default downscale multiplier for new recording sources |

> **Important**: This value is **divided by 2** internally. That is:
> - Set to **1.0** (default): Actual recording resolution is **50% of original** (1920x1080 > 960x540)
> - Set to **0.5**: Actual recording resolution is **25% of original** (1920x1080 > 480x270)
> - Set to **0.25** (minimum): Actual recording resolution is **12.5% of original** (1920x1080 > 240x135)
>
> For **undownsampled** original resolution recording, set Downscale to **2.0** in the Take Recorder recording source.

#### 4.2.4 DefaultCompressionLevel

| Item | Description |
|------|-------------|
| **Type** | Integer |
| **Range** | 0 ~ 9 |
| **Default** | 0 |
| **Purpose** | Compression level for frame buffer data (0 = no compression) |

> **Note**: This is a **reserved parameter** in the current version; frame buffers are always stored in uncompressed BGRA raw pixel format. Compression will be implemented in a future version.

---

## 5. NDI Screen Actor (SuperNDIScreen)

SuperNDIScreen is an Actor placed in 3D scenes for displaying NDI video on specified screen meshes.

### 5.1 Placing SuperNDIScreen

In UE Editor:

1. Open **Place Actors** panel (shortcut Shift+1)
2. Search for `SuperNDIScreen`
3. Drag into the viewport

Or right-click in Content Browser > All Classes > search `SuperNDIScreen` > drag into scene.

> **Note**: SuperNDIScreen itself **does not contain a mesh**. It is a "controller" Actor that needs to output video **onto the static meshes you specify** (see section 5.3).

### 5.2 Property Panel — Complete Parameter Reference

#### 5.2.1 NDIInputSelection

| Item | Description |
|------|-------------|
| **Type** | Dropdown (FName) |
| **Default** | Empty (auto-select first configured source) |
| **Purpose** | Select which logical input name this screen receives |

The **dropdown** shows all logical input names added in the configuration panel.

**Auto-select behavior**: If empty (not manually selected), the Actor **auto-binds to the first configured logical input** on startup. Manual selection is recommended if you have multiple logical inputs.

#### 5.2.2 OutputTexture

| Item | Description |
|------|-------------|
| **Type** | UTexture2D (read-only) |
| **Purpose** | Auto-created texture object at runtime showing the currently received NDI frame |

Read-only property for debugging and confirming texture creation. Do not manually modify it.

#### 5.2.3 ScreenMeshActors

| Item | Description |
|------|-------------|
| **Type** | Array (AStaticMeshActor reference list) |
| **Default** | Empty |
| **Purpose** | Specifies one or more Static Mesh Actors onto which NDI video is rendered |

**Usage**:
1. Place one or more **Static Mesh Actors** in the scene (typically plane/curved meshes representing LED screens)
2. Return to the SuperNDIScreen Details panel
3. Click **+** in the ScreenMeshActors array to add an element
4. Use the eyedropper or dropdown to select the Static Mesh Actor in the scene

> **One-to-many support**: One SuperNDIScreen can simultaneously output the same NDI video to **multiple screens**, suitable for LED matrix splicing or video wall scenarios.

#### 5.2.4 Transparent (Transparency Mode)

| Item | Description |
|------|-------------|
| **Type** | Boolean (checkbox) |
| **Default** | Off (false) |
| **Purpose** | Toggle screen material to translucent mode |

- **Off**: Uses opaque material (`MI_Screen_Inst`), suitable for physical LED screens
- **On**: Uses translucent material (`MI_Screen_Transparent`), suitable for projection effects, holographic effects

#### 5.2.5 Transparency

| Item | Description |
|------|-------------|
| **Type** | Float |
| **Range** | 0.0 ~ 1.0 |
| **Default** | 0.95 |
| **Purpose** | Controls translucent material opacity |
| **Visibility Condition** | Only shown when Transparent is on |

- **0.0**: Fully transparent (invisible)
- **0.95** (default): Nearly opaque, slightly see-through to content behind
- **1.0**: Fully opaque

#### 5.2.6 Brightness

| Item | Description |
|------|-------------|
| **Type** | Float |
| **Minimum** | 0.0 |
| **Default** | 1.0 |
| **Purpose** | Adjusts overall brightness of the image |

- **0.0**: Black
- **1.0**: Original brightness
- **> 1.0**: Enhanced brightness (can exceed 1.0 for overexposure effect)

#### 5.2.7 Color

| Item | Description |
|------|-------------|
| **Type** | Linear color (FLinearColor) |
| **Default** | White (1, 1, 1, 1) |
| **Purpose** | Color overlay/tinting applied to the image |

White keeps the original colors; other colors "tint" the image (multiplicative blend).

**Use cases**:
- Set to red to simulate a red LED panel effect
- Set to (0.5, 0.5, 0.5) to overall darken the image

#### 5.2.8 Contrast

| Item | Description |
|------|-------------|
| **Type** | Float |
| **Minimum** | 0.0 |
| **Default** | 1.5 |
| **Purpose** | Adjusts image contrast |

> **Tip**: The default value of 1.5 slightly enhances contrast for more realistic LED screen effects. Set to 1.0 for original contrast.

#### 5.2.9 Deformation (Keystone Correction)

For simulating projector keystone correction or non-orthogonal LED screen deformation.

| Sub-Parameter | Type | Range | Default | Description |
|---------------|------|-------|---------|-------------|
| **UpperLeftCorner** | 2D Vector | (0,0) ~ (1,1) | (0, 0) | Upper-left corner UV offset |
| **LowerLeftCorner** | 2D Vector | (0,0) ~ (1,1) | (0, 0) | Lower-left corner UV offset |
| **UpperRightCorner** | 2D Vector | (0,0) ~ (1,1) | (0, 0) | Upper-right corner UV offset |
| **LowerRightCorner** | 2D Vector | (0,0) ~ (1,1) | (0, 0) | Lower-right corner UV offset |

**How it works**: Each corner's offset value controls that corner's movement in UV space. All values ultimately achieve four-point perspective transformation through the Material Shader.

**Typical use cases**:
- Correcting projector trapezoid distortion during projection blending
- Simulating image correction for tilted LED screen installation
- Edge blending adjustment for multi-screen splicing

### 5.3 Usage Steps (Quick Start)

**Step 1**: Add a logical input in the config panel (e.g., `MainScreen`) and map to an external NDI source.

**Step 2**: Place a Static Mesh Actor in the scene (e.g., a Plane), adjust size and position to represent the LED screen.

**Step 3**: Place a SuperNDIScreen Actor in the scene (position doesn't matter; it's invisible).

**Step 4**: Select the SuperNDIScreen Actor, in the Details panel:
- **NDIInputSelection**: Select `MainScreen`
- **ScreenMeshActors**: Add the Static Mesh Actor from Step 2

**Step 5**: Done! If the NDI source is sending video, you should see the image in the scene.

### 5.4 Multi-Screen Configuration

A scene can have **multiple SuperNDIScreen Actors**, each bound to a different logical input:

```
SuperNDIScreen_1 > InputName: MainScreen > Main LED screen
SuperNDIScreen_2 > InputName: CameraA    > Side monitor
SuperNDIScreen_3 > InputName: SidePanel  > Side LED panel
```

You can also have **multiple SuperNDIScreen Actors bound to the same logical input** to display the same video at different locations simultaneously.

### 5.5 Real-Time Preview in Editor

SuperNDIScreen supports **editor real-time preview** — NDI video is visible in edit mode without pressing Play.

If editor preview doesn't show the image:
1. Confirm the config panel correctly maps the external source
2. After selecting the Actor, re-select NDIInputSelection in the Details panel
3. Check if the OutputTexture property has been created (should not be None)

---

## 6. Take Recorder Recording NDI Video

SuperNDI deeply integrates with UE's **Take Recorder** system to record live NDI video streams as Sequencer-playable frame sequences.

### 6.1 Opening Take Recorder

Menu bar > **Window** > **Cinematics** > **Take Recorder**

### 6.2 Adding SuperNDI Recording Source

1. In the Take Recorder panel, click the **+ Source** button
2. Find **Super NDI Input** in the popup list
3. Click to add

After adding, a recording source named **"Super NDI (Logical Inputs)"** appears in the Sources list.

### 6.3 Configuring Recording Parameters

#### 6.3.1 InputsToRecord

| Item | Description |
|------|-------------|
| **Type** | Name array (dropdown multi-select) |
| **Default** | Empty (record all configured logical inputs) |
| **Purpose** | Specifies which logical inputs to record |

**Behavior rules**:
- If **empty**: Automatically records **all** configured logical inputs
- If **manually selected**: Only records selected logical inputs
- Supports **multi-select**: Multiple NDI inputs can be recorded simultaneously

#### 6.3.2 TargetFPS

| Item | Description |
|------|-------------|
| **Type** | Integer |
| **Range** | 1 ~ 120 |
| **Default** | 30 |
| **Purpose** | How many NDI video frames to record per second |

| Setting | Applicable Scenario |
|---------|---------------------|
| 15 | Low-precision preview, long-duration recording |
| 24 | Film frame rate |
| 30 | **Recommended**. Balance quality and memory |
| 60 | High-precision sync |

#### 6.3.3 Downscale

| Item | Description |
|------|-------------|
| **Type** | Float |
| **Range** | 0.5 ~ 2.0 |
| **Default** | 1.0 |
| **Purpose** | Controls recording frame resolution downsampling |

> **Important**: This value is **divided by 2** internally. Actual mapping:

| Panel Value | Actual Downscale | 1080p Actual Recording Resolution | Per-Frame Memory |
|-------------|-----------------|-----------------------------------|------------------|
| **2.0** | 1.0 (no downscale) | 1920 x 1080 | ~8 MB |
| **1.0** (default) | 0.5 | 960 x 540 | ~2 MB |
| **0.5** | 0.25 | 480 x 270 | ~0.5 MB |

#### 6.3.4 MaxDurationSeconds

| Item | Description |
|------|-------------|
| **Type** | Integer (seconds) |
| **Range** | 1 ~ 3600 |
| **Default** | 900 (15 minutes) |
| **Purpose** | Automatically stops recording after this duration |

**Memory estimation** (1080p@30fps, default Downscale=1.0 i.e. actual 0.5 downsample):

| Duration | Estimated Memory |
|----------|-----------------|
| 1 minute | ~3.5 GB |
| 5 minutes | ~17 GB |
| 15 minutes (default max) | ~52 GB |

> **Warning**: NDI frame buffers are stored in memory (not disk). Monitor memory usage closely when recording very long videos.

### 6.4 Start Recording

1. Confirm all parameters are set correctly
2. Confirm NDI source is sending video
3. Click the **Record** button in the Take Recorder panel
4. After recording starts, the status bar displays the recording duration

### 6.5 Stop Recording

- Click **Stop** to manually stop
- Or wait for MaxDurationSeconds to auto-stop

### 6.6 Recording Results

After recording completes:

1. The system creates **NDI tracks** in the current Level Sequence (one track per recorded logical input)
2. Tracks are named `NDI_<logical_input_name>` (e.g., `NDI_MainScreen`)
3. Each track contains one **Section** with associated frame buffer asset
4. Frame buffer assets are auto-saved with the Level Sequence

> **Tip**: Be sure to **Ctrl+S save** the current Level Sequence after recording, or frame buffer data may be lost.

---

## 7. Sequencer Playback NDI Recording

After recording, pre-recorded NDI video can be played back in Sequencer.

### 7.1 Opening Sequencer

Double-click the Level Sequence asset in Content Browser, or select the Level Sequence Actor in the scene > click **Open in Sequencer**.

### 7.2 NDI Track Structure

In the Sequencer timeline, you'll see the NDI tracks generated by recording:

```
 NDI_MainScreen  --------------------------
    [Section: MainScreen]
        InputName: MainScreen
        FrameBuffer: (frame buffer asset reference)
        HoldLastFrame: Yes
```

### 7.3 Section Properties

When selecting the NDI Section in Sequencer, the Details panel shows:

#### 7.3.1 InputName

| Item | Description |
|------|-------------|
| **Type** | Name (FName) |
| **Purpose** | Specifies which logical input to inject into during playback |

#### 7.3.2 FrameBuffer

| Item | Description |
|------|-------------|
| **Type** | Asset reference (USuperNDIMultiSourceFrameBuffer) |
| **Purpose** | Points to the buffer asset storing recorded frame data |

#### 7.3.3 HoldLastFrame

| Item | Description |
|------|-------------|
| **Type** | Boolean |
| **Default** | On (true) |
| **Purpose** | Behavior when playhead is outside the recorded frame range |

- **On** (recommended): Shows the nearest frame if no exact match (avoids black screen)
- **Off**: Injects no image if no exact frame (may cause black flickering)

### 7.4 Playback Mechanism Detailed

When Sequencer plays, NDI Section behavior is:

1. **Playhead enters Section range**:
   - Automatically enables **Loopback mode**
   - Blocks real NDI network frames for that logical input
   - Ensures scene SuperNDIScreens only show recorded content

2. **Per-frame update**:
   - Queries corresponding frame from frame buffer based on current time (binary search, efficient)
   - Injects queried frame into subsystem
   - Subsystem broadcasts frame data to all SuperNDIScreen Actors subscribed to that logical input

3. **Playhead leaves Section range**:
   - Disables Loopback mode
   - Restores real NDI network frame reception
   - SuperNDIScreen resumes displaying live NDI video

### 7.5 Multi-Track / Multi-Section

- **Multi-track**: Multiple logical inputs can be played back simultaneously
- **Multi-section**: Multiple Sections can exist in the same track for different time periods playing different frame buffers
- **Section arrangement**: Same-row Sections follow chronological order; different-row Sections can be parallel

### 7.6 Offline Rendering

NDI playback supports Sequencer's **offline rendering** mode. When rendering movies:

- No live NDI source needed
- Playback relies entirely on recorded frame buffer data
- Image quality depends on recording downscale settings

---

## 8. FAQ & Troubleshooting

### 8.1 Source Discovery

#### Q: No NDI sources visible in config panel dropdown

**Troubleshooting steps**:
1. Confirm NDI source device/software **is running and sending video**
2. Confirm NDI source is on the **same LAN/subnet** as the UE host
3. Check **Windows Firewall** — ensure UDP 5353 and TCP/UDP 5960-5969 are not blocked
4. Close **NDI Studio Monitor** (may exclusively occupy Finder resources)
5. Wait **2-5 seconds** then click **Refresh Sources** (mDNS discovery has latency)
6. Check if **Bonjour Service** is running
7. View UE's **Output Log**, search for `SuperNDI` keyword

#### Q: Source discovery takes too long

- Initial startup mDNS broadcast needs time to accumulate
- Persistent Finder starts discovering sources approximately 3-5 seconds after startup
- If over 10 seconds with no discovery, check network configuration

### 8.2 Video Display

#### Q: SuperNDIScreen shows black screen

1. Confirm the logical input is **mapped to a valid external source** in the config panel
2. Confirm SuperNDIScreen's **NDIInputSelection** has selected the correct logical input
3. Confirm at least one Static Mesh Actor is added to **ScreenMeshActors**
4. Check the **OutputTexture** property — should not be None
5. Try re-selecting NDIInputSelection (triggers rebinding)

#### Q: Choppy image / low frame rate

- NDI source itself has low frame rate > Check source device settings
- Insufficient network bandwidth > Use gigabit or higher wired network
- Low UE editor frame rate > Lower editor quality or close unnecessary windows
- UYVY/UYVA format conversion overhead > NDI source should output BGRA/BGRX format where possible

#### Q: Color deviation

SuperNDI defaults to **sRGB** color space. If the NDI source outputs linear color space, colors may appear too bright/tinted. Confirm the NDI source's output color space settings.

### 8.3 Recording

#### Q: Super NDI Input option not visible in Take Recorder

Confirm SuperStageEditor module is loaded. Search `SuperNDI` in Output Log to confirm initialization succeeded.

#### Q: Frame buffer data lost after recording

You must **save the Level Sequence asset** after recording (Ctrl+S or right-click > Save). Frame buffer data is embedded in the Level Sequence's Package and is lost if not saved before closing.

#### Q: Memory spikes during recording

This is normal — NDI frame buffers are stored as **uncompressed BGRA** in memory.

**Mitigation**:
- Lower **Downscale** (e.g., set to 0.5 for actual 0.25 downscale)
- Lower **TargetFPS** (e.g., 15 fps)
- Shorten **MaxDurationSeconds**
- Turn off unneeded NDI inputs

### 8.4 Playback

#### Q: Sequencer playback shows live NDI instead of recorded content

Check that the NDI Section's **FrameBuffer** property correctly references the frame buffer asset. If empty, playback cannot inject frames.

#### Q: Black screen at some time points during Sequencer playback

- Enable the Section's **HoldLastFrame** option
- Check that the frame buffer has frame data for the corresponding time range

### 8.5 DLL & Initialization

#### Q: Output Log shows "SuperNDI: CRITICAL - Integrated NDI DLL not found!"

Ensure `Processing.NDI.Lib.x64.dll` exists in one of the following paths:
- `Plugins/SuperStage/Binaries/Win64/`
- `Plugins/SuperStage/Source/SuperNdi/ThirdParty/NDI/Bin/x64/`
- `Plugins/SuperStage/Source/SuperNdi/ThirdParty/NDI/Bin/` (fallback)

#### Q: Output Log shows "NDIlib_initialize failed"

Possible causes:
- CPU doesn't support SSE 4.2 instruction set (very old CPU)
- DLL file corrupted
- Missing runtime dependencies (Visual C++ Redistributable)

#### Q: Output Log shows "Failed to create Persistent Finder"

Possible causes:
1. NDI Runtime not installed (download from [ndi.video](https://ndi.video))
2. NDI Studio Monitor occupies resources > Close and retry
3. Firewall blocking UDP 5353 (mDNS)
4. Bonjour service not running

---

## 9. Performance Optimization Tips

### 9.1 Network Optimization

| Recommendation | Description |
|---------------|-------------|
| Use **wired gigabit network** | WiFi has high packet loss, not recommended for NDI |
| NDI source and receiver use **separate network segment** | Avoid other traffic interference |
| Reduce NDI source count | Each 1080p NDI stream uses approx. 100-200 Mbps |

### 9.2 Resolution Optimization

| Scenario | Recommended Resolution |
|----------|------------------------|
| Previs/Rehearsal | 960x540 (Downscale=1.0) |
| Final Render | 1920x1080 (Downscale=2.0) |
| Long Recording | 480x270 (Downscale=0.5) |

### 9.3 Frame Rate Optimization

| Scenario | Recommended Frame Rate |
|----------|------------------------|
| Static images/slideshows | 10-15 fps |
| Normal video | 30 fps |
| Fast motion/music sync | 60 fps |

### 9.4 Memory Optimization

- **Save and close** unneeded Level Sequences immediately after recording
- Use the **minimum necessary downscale and frame rate**
- Split long recordings into multiple Take segments
- Monitor UE's memory usage in Windows Task Manager

### 9.5 NDI Source-Side Optimization

- OBS settings: Align NDI output resolution with actual needed resolution in UE
- vMix settings: Choose lower bandwidth output modes (e.g., SHQ instead of Full NDI)
- NDI source should output **BGRA/BGRX format** where possible to avoid UYVY/UYVA CPU conversion overhead

### 9.6 Reception Frame Rate Ceiling

The SuperNDI subsystem uses a **50Hz Ticker** to poll the NDI receiver (every 20ms), so even if the NDI source outputs 60fps, the actual reception frame rate is capped at approximately **50fps**. For most stage previz scenarios (30fps), this limit is not a concern.

---

## 10. Glossary

| Term | Description |
|------|-------------|
| NDI | Network Device Interface — low-latency IP video transport protocol developed by NewTek |
| Logical Input Name | User-defined name used to identify one NDI input within the project |
| External Source | The display name of an actual NDI device/software on the network |
| Frame Buffer | Memory/asset storing recorded NDI video frame sequences |
| Downscale | Reducing recorded frame resolution to save memory |
| Loopback Mode | Blocking real NDI frames during Sequencer playback, injecting only recorded frames |
| mDNS | Multicast DNS — network protocol used by NDI for source discovery (UDP port 5353) |
| Take Recorder | UE's built-in recording system for recording real-time data to Sequencer |
| Sequencer | UE's built-in film editor/timeline system |
| Section | A time segment/clip in a Sequencer track |
| MID | Material Instance Dynamic — dynamically created material instance at runtime |
| RHI | Render Hardware Interface — UE's rendering hardware abstraction layer |
| BGRA | Pixel format: Blue-Green-Red-Alpha, 8 bits per channel |
| sRGB | Standard RGB color space (gamma-corrected) |
| Keystone Correction / Deformation | Correcting geometric distortion of projection/display via UV offset |

---

> **Document Maintenance**: For any questions or suggestions, please contact the SuperStage technical support team.
