# 13 - LED Strip Effect (Light Strip Effect)

> **Module**: SuperStage Runtime (ASuperLightStripEffect)  
> **Target Users**: Lighting designers, LED programmers  
> **Prerequisites**: [03 - DMX Fixture Base](/docs/stage-core/dmx-actor-base)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

**SuperLightStripEffect** is a DMX-controlled LED strip effect Actor. It implements multiple programmable strip effects (chase, flow, breathing, etc.) via GPU material (MI_Effect_Inst) and can **batch-apply the same material effect to multiple static meshes in the scene**.

### Class Inheritance Chain

```
AActor (UE5 Engine Base Class)
  │
  └── ASuperBaseActor ·············· SuperStage Root Class (L1)
        │                           ├ SceneBase / ForwardArrow / UpArrow
        │                           └ AssetMetaData
        │
        └── ASuperDmxActorBase ····· DMX Fixture Base Class (L2)
              │                      ├ DMX Address / Fixture Library / Channel Reading
              │                      └ SuperDMXTick / Address Label
              │
              └── ASuperLightStripEffect · LED Strip Effect (L3) ← This document
                    ├ TargetMeshActors[] → Batch material application
                    ├ MI_Effect_Inst dynamic material instance
                    ├ 9-channel DMX control (Dimmer/Strobe/Effect/Speed/Width/Direction/RGB)
                    ├ 10 built-in GPU effects
                    └ MaxLightIntensity / MaterialIndex
```

> **Design Note**: `ASuperLightStripEffect` inherits directly from `ASuperDmxActorBase` (not `ASuperLightBase`) because LED strips **don't need Pan/Tilt rotation axes**. Its core function is driving GPU material parameters via DMX to produce visual effects, rather than controlling physical light head movement.

### Core Features

- **10 Built-in Effects** — Chase, flow, breathing, gradient and other GPU material effects
- **9-Channel DMX Control** — Dimmer, Strobe, Effect selection, Speed, Width, Direction, RGB color
- **Multi-Target Application** — One strip Actor can simultaneously drive materials on multiple static meshes in the scene
- **Material Sharing** — All target meshes share the same dynamic material instance (unified control, memory saving)
- **8 Strobe Modes** — Closed, On, Linear, Pulse, RampUp, RampDown, Sine, Random

### Use Cases

- LED bar/strip effect control
- Building contour lights / wall washer lights
- Stage background LED decoration
- Truss light strips / floor light strips
- Any linear fixtures requiring unified material effect control

---

## 2. Basic Principles

How SuperLightStripEffect works:

```
DMX Signal → Read 9 channel values → Update dynamic material parameters → GPU renders effects
                                    │
                                    ├─→ Own YStaticMeshEffect mesh
                                    └─→ TargetMeshActors[] meshes in scene
```

**Key**: All target meshes share **the same dynamic material instance**. This means:
- Modifying material parameters updates all targets simultaneously (unified effect)
- Only one set of DMX channels needed to control all targets
- For independent control of different strips, use multiple SuperLightStripEffect Actors

---

## 3. Property Details

### 3.1 Target Settings (A.ModelTargets)

| Parameter | Description | Default |
|------|------|--------|
| **TargetMeshActors** | Array of StaticMeshActors in the scene to apply effects to | Empty |
| **StaticMeshEffect** | Static mesh asset used by the actor's own mesh component | None |

#### TargetMeshActors

This is the **core configuration** of the strip effect — you need to select the static meshes in the scene to be controlled by this strip:

1. In the Details panel, expand **A.ModelTargets** → **TargetMeshActors**
2. Click the **+** button to add elements
3. Use the eyedropper tool or dropdown menu to select StaticMeshActors in the scene
4. Can add **multiple** targets; they share the same effect

> **Tip**: Target Actors must be **StaticMeshActor** type. Regular Actors or Blueprint Actors are not supported.

### 3.2 Default Parameters (B.DefaultParameter)

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **MaxLightIntensity** | Maximum light intensity (material brightness cap) | 0 - 1000 | 1.0 |
| **MaterialIndex** | Material slot index (which material slot on target meshes to apply to) | 0 - 255 | 0 |

#### MaxLightIntensity

Controls the maximum brightness value of the material. This value is set to the material's `MaxBrightness` parameter during initialization:
- Larger value → higher maximum strip brightness
- Typically set to 1.0 ~ 10.0 (depending on scene needs)
- Actual brightness = MaxLightIntensity × Dimmer

#### MaterialIndex

Specifies which **material slot** on the target mesh the effect material is applied to:
- **0** (default) → Apply to the first material slot
- If the target mesh has multiple material slots, a specific slot can be specified
- System automatically checks if the index is within valid range (prevents out-of-bounds)

#### Parameter Tuning Suggestions

| Scenario | MaxLightIntensity | Description |
|------|-------------------|------|
| Close decorative strip (< 3m) | 0.5 - 2.0 | Avoid overexposure and glare |
| Stage strip (3-10m) | 2.0 - 5.0 | Standard brightness |
| Large venue (10m+) | 5.0 - 15.0 | Long distance needs higher brightness |
| Building facade wall wash | 3.0 - 10.0 | Needs to penetrate ambient light |
| Ambient background | 0.3 - 1.0 | Low-key soft effect |

### 3.3 Control Parameters (C.ControlParameter)

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **Dimmer** | Total brightness (dimmer) | 0.0 - 1.0 | 0.0 |
| **Strobe** | Strobe frequency | 0.0 - 1.0 | 0.0 |
| **Effect** | Effect selection | 0.0 - 1.0 | 0.0 |
| **Speed** | Effect movement speed | 0.0 - 1.0 | 0.5 |
| **Width** | Effect width/size | 0.0 - 1.0 | 0.0 |
| **Direction** | Effect movement direction | 0.0 - 1.0 | 0.0 |
| **Color** | RGB color | Per channel 0.0 - 1.0 | White (1,1,1) |

---

## 4. Control Parameter Details

### 4.1 Dimmer

Total brightness control, multiplied by MaxLightIntensity for final brightness:

| DMX Value | Material Parameter | Effect |
|--------|---------|------|
| 0.0 | Brightness = 0 | Completely off |
| 0.5 | Brightness = 0.5 | Half bright |
| 1.0 | Brightness = 1.0 | Max brightness |

### 4.2 Strobe

DMX value mapped to strobe frequency (0~255):

```
Material Strobe Parameter = DMX Value × 255
```

Strobe mode controlled by the material's internal StrobeMode parameter:

| StrobeMode | Name | Description |
|-----------|------|------|
| 0 | Closed | Off (always dark) |
| 1 | Open | Always on (no strobe) |
| 2 | Linear | Linear flash |
| 3 | Pulse | Pulse flash |
| 4 | RampUp | Fade-in flash |
| 5 | RampDown | Fade-out flash |
| 6 | Sine | Sine wave flash |
| 7 | Random | Random flash |

### 4.3 Effect

DMX value mapped to 10 effects (0~10):

```
Material Effect Parameter = Lerp(0, 10, DMX Value)
```

| DMX Value Range | Effect Index | Typical Effect |
|-----------|---------|---------|
| 0.0 ~ 0.1 | 0 ~ 1 | Static/solid color |
| 0.1 ~ 0.2 | 1 ~ 2 | Flow |
| 0.2 ~ 0.3 | 2 ~ 3 | Chase |
| 0.3 ~ 0.4 | 3 ~ 4 | Gradient |
| ... | ... | ... |
| 0.9 ~ 1.0 | 9 ~ 10 | Complex combined effects |

> **Note**: Specific effect appearance depends on the MI_Effect_Inst material implementation. Visual performance of different effects is determined by the shader logic inside the material.

### 4.4 Speed

DMX value mapped to speed range (-10 ~ +10):

```
Material Speed Parameter = Lerp(-10, 10, DMX Value)
```

| DMX Value | Speed | Effect |
|--------|------|------|
| 0.0 | -10 | Fastest **reverse** movement |
| 0.25 | -5 | Medium reverse |
| 0.5 | 0 | **Stop** movement |
| 0.75 | +5 | Medium forward |
| 1.0 | +10 | Fastest forward movement |

> **Tip**: Speed supports negative values, which can reverse the effect's movement direction. DMX value = 0.5 means effect is stationary.

### 4.5 Width

DMX value mapped to width range (0 ~ 10):

```
Material Width Parameter = Lerp(0, 10, DMX Value)
```

- Smaller value → narrower/denser effect pattern
- Larger value → wider/sparser effect pattern

### 4.6 Direction

Directly mapped to material's EffectDirection parameter (0 ~ 1):

- Controls the movement direction or scanning direction of the effect
- Specific behavior depends on the selected effect

### 4.7 Color (RGB)

Three DMX channels independently control red, green, and blue components:

| Channel | DMX Value 0.0 | DMX Value 1.0 |
|------|-----------|-----------|
| Red | No red | Max red |
| Green | No green | Max green |
| Blue | No blue | Max blue |

Color values are directly set to the material's `LightColor` vector parameter.

---

## 5. Blueprint Control

### 5.1 Control Functions

#### SetLightStripDefault (Initialization)

Initializes strip material and applies to all target meshes. **Must be called once before starting control**.

Execution content:
1. Set StaticMeshEffect asset for own mesh
2. Create dynamic material instance (if not yet created)
3. Set MaxBrightness parameter
4. Apply material to own mesh's MaterialIndex slot
5. Iterate through TargetMeshActors, apply material to each target's MaterialIndex slot

#### SetLightStripEffect (9-Channel Control)

Main control function, called per frame in SuperDMXTick:

| Input Parameter | Control |
|---------|---------|
| **DmxDimmer** | Brightness |
| **DmxStrobe** | Strobe |
| **DmxEffect** | Effect selection |
| **DmxSpeed** | Speed |
| **DmxWidth** | Width |
| **DmxDirection** | Direction |
| **DmxRed** | Red |
| **DmxGreen** | Green |
| **DmxBlue** | Blue |

### 5.2 Typical Blueprint Implementation

```
Event BeginPlay
  │
  └─ SetLightStripDefault()    ← Initialize material

Event SuperDMXTick(DeltaTime)
  │
  └─ SetLightStripEffect(Dimmer, Strobe, Effect, Speed, Width, Direction, R, G, B)
```

---

## 6. DMX Channel Planning

| Channel | Attribute | Description |
|------|------|------|
| 1 | Dimmer | Brightness |
| 2 | Strobe | Strobe |
| 3 | Effect | Effect selection |
| 4 | Speed | Speed |
| 5 | Width | Width |
| 6 | Direction | Direction |
| 7 | Red | Red |
| 8 | Green | Green |
| 9 | Blue | Blue |

> **Total 9 channels**. If using 16-bit precision, assign Fine channels to key attributes as needed.

---

## 7. FAQ

### Q: Target mesh doesn't show effects?
1. Confirm **SetLightStripDefault** has been called to initialize material
2. Check if **MaterialIndex** is correct (starts from 0)
3. Confirm target Actor is **StaticMeshActor** type
4. Confirm **Dimmer** DMX value > 0

### Q: Can effects be seen in editor?
Yes. SetLightStripDefault is called both in OnConstruction (when placed/modified) and BeginPlay (at runtime). After configuring TargetMeshActors in the editor, the material is applied.

### Q: How to change the effect material?
Default uses MI_Effect_Inst material. For custom materials, create a new material instance ensuring it contains the same parameter names (MaxBrightness, Brightness, Strobe, LightColor, Effect, Speed, Width, EffectDirection).

### Q: Effect not applied after adding new target Actors?
After modifying TargetMeshActors, you need to **re-trigger initialization**. In the editor, moving the strip Actor triggers OnConstruction; at runtime, re-call SetLightStripDefault.

### Q: Effect speed is wrong?
Speed DMX value = 0.5 means speed is 0 (stopped), not half speed. < 0.5 is reverse, > 0.5 is forward.

---

> **Next Steps**: Read [14 - Stage VFX](/docs/stage-core/stage-vfx) to learn about Niagara particle effect control.
