# 06 - Effect Plane Component (SuperEffectComponent)

> **Module**: SuperStage Runtime  
> **Target Audience**: Lighting designers, fixture Blueprint creators  
> **Prerequisite Reading**: [00 - Light Component Overview](00_LightComponent_Overview.md)

---

## 1. Overview

**SuperEffectComponent** is a visual effect component based on **Static Mesh + Material Driven** rendering, used to simulate non-projective light-emitting elements on fixtures such as LED effect panels, strobe panels, decorative light strips, etc. It does not produce real lighting (no SpotLight or RectLight) — instead, it renders visual effects through the emissive property of dynamic materials.

### Use Cases

- **LED Effect Panels** — LED decorative panels on the front or side of fixtures
- **Strobe Panels** — Light-emitting surfaces of high-power strobe lights
- **Light Strip Effects** — Dynamic effects such as flowing light and chasing on LED strips
- **Decorative Emission** — Auxiliary decorative emissive elements on fixtures

### Differences from Other Light Components

| Feature | SuperEffectComponent | SuperSpotComponent / SuperBeamComponent |
|------|---------------------|----------------------------------------|
| **Light Projection** | None (pure visual effect) | Yes (real lighting) |
| **Rendering Method** | Static mesh + emissive material | SpotLight + light function |
| **Effect Animation** | Supports effect patterns, speed, direction, etc. | Does not support effect animation |
| **Performance Overhead** | Lower (material updates only) | Higher (lighting + shadow calculation) |
| **Transparent Mode** | Supports opaque/transparent switching | Not supported |

---

## 2. Component Structure

```
SuperEffectComponent (inherits from USceneComponent)
  └── YStaticMeshEffect  — Effect panel model (static mesh, carrying emissive material)
```

### Material Systems

The effect component supports two material modes:

| Material | Description |
|------|------|
| **Opaque Material (EffectMaterial)** | Standard emissive material, opaque rendering, better performance |
| **Transparent Material (EffectMaterialTransparent)** | Semi-transparent emissive material, objects behind are visible, suitable for transparent effects |

Switch between the two modes via the `bTransparent` toggle (see below).

---

## 3. Default Parameters

### 3.1 Effect Model

| Parameter | Description | Type | Default |
|------|------|------|--------|
| **StaticMeshEffect** | Static mesh model used for the effect panel | Static Mesh Reference | None |
| **EffectTransform** | Position, rotation and scale offset of the effect panel relative to the component | Transform | No offset |

### 3.2 Material Configuration

| Parameter | Description | Type | Default |
|------|------|------|--------|
| **EffectMaterialNo** | Material slot index, specifies which material slot of the mesh the dynamic material is bound to | Integer | 0 |
| **bTransparent** | Whether to use transparent material | Boolean | false |

#### Transparent Mode

| Mode | Behavior | Use Cases |
|------|------|----------|
| **Opaque** (bTransparent = false) | Effect panel completely occludes objects behind; better rendering performance | Standard LED panels |
| **Transparent** (bTransparent = true) | Effect panel is semi-transparent; objects behind are visible | Transparent lamp covers, holographic effects |

### 3.3 Component-Level Brightness Sub-Control

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **MaxLightIntensity** | Component maximum brightness percentage (design-time config, 100 = full) | ≥ 1 | 100.0 |

Final brightness formula:
```
Final Brightness = Actor-level Dimmer × (MaxLightIntensity / 100) × ComponentDimmer × Strobe Multiplier
```

> **Auto-Initialization**: Component default parameters are automatically initialized by `OnRegister()`. During component registration, `SetEffectMaterial()` is automatically called to create material instances and set defaults. No need to manually call in the Actor.

---

## 4. DMX-Controlled Parameters

### 4.1 Intensity

| Operation | Description | Parameter Range |
|------|------|----------|
| **Set Brightness** | Controls the emissive brightness of the effect panel | 0.0 (off) ~ 1.0 (full) |

Brightness is controlled via the material parameter `Brightness`.

### 4.2 Color

| Operation | Description | Parameter Range |
|------|------|----------|
| **Set Color** | Controls the emissive color of the effect panel | RGB linear color |

Color is controlled via the material parameter `LightColor`.

### 4.3 Strobe

The effect component has an **independent strobe system**, identical to the light component's strobe:

| Parameter | Description | Range |
|------|------|------|
| **Strobe Speed** | Flashing speed | ≥ 0 |
| **Strobe Mode** | Flashing waveform (0~7) | See table below |
| **Random Seed** | Seed value for random mode | Any |

#### Strobe Modes

| Mode | Name | Effect |
|------|------|------|
| 0 | Closed | Completely off |
| 1 | Open | Always on (default) |
| 2 | Linear | Smooth fade in/out |
| 3 | Pulse | Square wave flash |
| 4 | Ramp Up | Gradual brightening |
| 5 | Ramp Down | Gradual dimming |
| 6 | Sine | Sine pulsation |
| 7 | Random | Random flash |

> **Performance Optimization**: Strobe mode 0/1 automatically disables component Tick; modes 2~7 automatically enable Tick for per-frame calculation.

### 4.4 Effect Control

The effect component supports material-driven dynamic pattern effects controlled by the following parameters:

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **Effect** | Effect pattern number/blend level | 0.0 ~ any | 0.0 |
| **Speed** | Effect animation playback speed | Any | 1.0 |
| **Width** | Effect pattern width/range | 0.0 ~ 1.0 | 0.5 |

### 4.5 Combined Control (EffectsControl)

The effect component provides a "one-stop" combined control interface that can set multiple parameters at once:

| Parameter | Description |
|------|------|
| **Brightness** | Effect panel emissive brightness |
| **Strobe** | Strobe speed |
| **Color** | Emissive color |
| **Direction** | Running direction of effect animation |
| **Effect** | Effect pattern number |
| **Speed** | Effect animation speed |
| **Width** | Effect pattern width |

### 4.6 Direction

| Parameter | Description | Range |
|------|------|------|
| **Direction** | Running direction of effect animation | Any float value |

---

## 5. Usage Guide

### 5.1 Basic Setup Workflow

1. Add SuperEffectComponent in the fixture Blueprint
2. Specify the effect panel's **static mesh model** (StaticMeshEffect)
3. Set the **effect panel position and size** (EffectTransform)
4. Choose whether to use **transparent material** (bTransparent)
5. Specify the **material slot index** (EffectMaterialNo, typically 0)
6. Map DMX channels to parameters in the fixture library

### 5.2 Multiple Effect Panels

A single fixture can have **multiple** effect components, for example:

```
SuperStageLight
  └── Head
        ├── SuperEffectComponent[0]  — Front LED panel
        ├── SuperEffectComponent[1]  — Left decorative light strip
        └── SuperEffectComponent[2]  — Right decorative light strip
```

Each effect component has independent brightness, color and effect controls.

### 5.3 Effect + Lighting Combination

Effect components are typically used **in combination with** light components:

```
SuperStageLight
  └── Head
        ├── SuperBeamComponent   — Main light source (produces lighting and beam)
        └── SuperEffectComponent — Effect panel (pure visual effect)
```

---

## 6. FAQ

### Q: Effect panel material looks blank/black?
**A**: Confirm the following:
1. StaticMeshEffect (effect mesh model) is correctly specified
2. EffectMaterialNo matches the mesh model's material slot index
3. Brightness (Intensity) is greater than 0
4. Effect panel visibility is not turned off

### Q: Effect is abnormal after switching transparent/opaque mode?
**A**: The system automatically rebuilds dynamic material instances when detecting transparency state changes. If issues persist, try re-calling SetEffectMaterial to force rebuild.

### Q: Does the effect component produce lighting?
**A**: No. The effect component is purely a visual effect (emissive material) and does not illuminate surrounding objects. Add separate light components if lighting is needed.

### Q: How to desynchronize strobe flashing across multiple fixtures' effect panels?
**A**: Set different **Random Seed** values for each fixture's effect component. This produces different flashing sequences in Random strobe mode.

### Q: What do effect animation speed, width, etc. parameters actually do?
**A**: The specific visual effects of these parameters depend on the shader logic in the effect material (MI_Effect_Inst). Common effects include flowing lights, chasing lights, breathing lights, gradients, etc. View and modify specific implementations through the Material Editor.

---

## 7. API Quick Reference

The following are all public function signatures of `USuperEffectComponent`:

| Function Signature | Description |
|----------|------|
| `void SetComponentDimmer(const float NewDimmer)` | Component-level brightness sub-control (0.0 ~ 1.0) |
| `void SetEffectMaterial()` | Create dynamic material instances (auto-called by OnRegister) |
| `void SetEffectIntensity(const float NewIntensity)` | Set brightness |
| `void SetEffectStrobe(const float NewStrobe = 0.0f)` | Set strobe speed |
| `void SetEffectStrobeMode(const float NewStrobeMode = 1.0f)` | Set strobe mode (0 ~ 7) |
| `void SetRandomSeed(const float NewSeed)` | Set random seed |
| `void SetEffectColor(const FLinearColor NewColor = FLinearColor(1,1,1))` | Set color |
| `void SetEffectControl(const float NewEffect = 0, const float NewSpeed = 1, const float NewWidth = 0.5) const` | Effect pattern control (Effect/Speed/Width) |
| `void SetEffectsControl(float NewIntensity = 1, float NewStrobe = 0, FLinearColor NewColor = ..., float NewDirection = 0, float NewEffect = 0, float NewSpeed = 1, float NewWidth = 0.5) const` | Combined one-stop control |
