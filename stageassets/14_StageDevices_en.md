# Other Stage Devices

> Applies to SuperStage 26H2.6 and later ｜ Read first: [Stage Assets Overview](00_StageAssets_Overview_en.md)

This document covers three objects in the asset browser's **SuperStage** category: the director camera, the Madrix matrix and white-model rendering.

The other objects in that category have their own documents: [screens](06_SuperScreen_en.md), [projectors](05_SuperProjector_en.md), [LED tape](../stagecore/13_Light_Strip_Effect_en.md), [lift and rail machinery](../stagecore/10_Stage_Machinery_en.md), [lasers](../laser/00_Laser_Overview_en.md).

---

## 1. Super Director Camera

A camera in the scene that captures a live picture for screens and projectors to reference. It is one of three media sources for [screens](06_SuperScreen_en.md) and [projectors](05_SuperProjector_en.md) (the other two being project static textures and NDI input).

### Parameters

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| Enable Capture | On | — | Whether to capture. Turning it off saves the cost |
| Capture Resolution | 1080 × 1080 | width and height ≥ 1 | Capture resolution (width × height) |
| Field Of View | 90° | 5–170 | Field of view |

### Usage

1. Place the director camera where you want the shot from and set its orientation and FOV;
2. Select this director camera as the media source on a screen or projector;
3. The picture is fed to the screen live.

### Notes

- **Resolution drives the cost directly.** Capturing at 4K for a screen far upstage is wasted; pick it from how large the screen actually is in frame;
- Several screens can reference the same director camera;
- Turn `Enable Capture` off when it is not needed — capture runs every frame.

---

## 2. Super Madrix

Puts **MADRIX's live main-output preview** onto meshes in the scene.

**It does not use DMX.** The data comes from the local MADRIX Remote HTTP service: SuperStage **pulls** the preview image from `http://127.0.0.1/RemoteCommands/GetPreviewOutput.bmp`, decodes it, and pushes it to a dynamic material as a texture. So what it receives is the picture MADRIX has already rendered, not DMX channel values.

### Parameters

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| **ScreenMeshActors** | Empty | — | The `StaticMeshActor` list that carries the picture. One MADRIX source can drive several screens at once |
| **MaterialIndex** | 0 | 0–255 | The material slot the dynamic material is applied to. **Targets without that slot are skipped** |
| **Brightness** | 1.0 | ≥ 0 | Brightness multiplier |
| **MADRIX Port** | 80 | 1–65535 | Port of the local MADRIX **HTTP service**. **Changing it takes effect on the next run** |

### Usage

1. Prepare the `StaticMeshActor`s that act as the LED panels in the scene;
2. Place Super Madrix and add those meshes to `ScreenMeshActors`;
3. Set `MaterialIndex` and confirm the target meshes have that slot;
4. **Enable the Remote HTTP service in MADRIX** on the same port as `MADRIX Port` here — SuperStage connects to it, not the other way round.

> The material on the target mesh needs a Texture2D parameter and a Brightness scalar parameter; the shipped LED screen material already has both.

### Notes

- How it differs from a [screen](06_SuperScreen_en.md): a screen plays a video source you pick in Unreal, while this object mirrors **whatever MADRIX is currently outputting**, so the content is programmed on the MADRIX side;
- MADRIX must be running on the same machine and its Remote HTTP service must be enabled — there is no remote-host option;
- Matrix data can be recorded into Sequencer tracks and replayed via Take Recorder;
- **It is not a real LED screen controller and not a substitute for Madrix software** — it does visual previs only.

---

## 3. Super White Model

A post-process object. Drop it into the scene and the entire world renders as a white model; **deleting it restores the original view automatically**.

### Parameters

| Parameter | Default | Description |
| --- | --- | --- |
| Enable White Model | On | Master switch. Turning it off restores normal rendering without deleting the object |
| Clay Color | Light grey | The colour of the white model itself |
| Background Color | Dark grey | The background colour |

### Where It Applies

| Context | Applies |
| --- | --- |
| Editor viewport | Yes |
| PIE (play in editor) | Yes |
| Movie Render Queue | Yes |

So it can be used directly for stills and video output, not just for looking at in the editor.

### Uses

- Massing and structure review — strip material and colour out and look only at form and spatial relationships;
- Lighting comparison — light distribution is easier to read against a white model;
- Storyboard frames for presentations.

### Notes

- It is a **post-process** and applies to the whole world; it cannot white-model a subset of objects;
- One per scene is enough.

---

## 4. Related Documents

- [Super Screen](06_SuperScreen_en.md)
- [Super Projector](05_SuperProjector_en.md)
- [NDI Inputs](../editortools/04_NDIConfigPanel_en.md)
- [Stage Assets Overview](00_StageAssets_Overview_en.md)
