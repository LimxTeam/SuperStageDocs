# 05 - Rectangular Area Light Component (SuperRectComponent)

> **Module**: SuperStage Runtime  
> **Target Audience**: Lighting designers, fixture Blueprint creators  
> **Prerequisite Reading**: [01 - Base Light Component](01_SuperLightingComponent.md)

---

## 1. Overview

**SuperRectComponent** is a light source component based on UE RectLight (rectangular area light source), specifically designed for simulating fixtures that produce uniform planar lighting such as **LED panel lights, Wash lights, and soft lights**. Unlike the cone-shaped lighting of SpotLight, RectLight uniformly emits light from a rectangular area, producing softer, more natural illumination effects.

### Use Cases

- **LED Panel Lights** — Flat LED array fixtures
- **Wash Lights** — Wash lights, flood lights
- **Soft Lights** — Fixtures requiring large-area uniform illumination
- **Cyc Lights** — Illuminating backdrops

### Differences from SpotComponent

| Feature | SuperSpotComponent | SuperRectComponent |
|------|-------------------|--------------------|
| **Light Source Type** | SpotLight (cone-shaped) | RectLight (rectangular planar) |
| **Light Form** | Conical, with defined aperture edges | Rectangular planar, soft and uniform lighting |
| **Zoom** | Supported | Not supported |
| **Frost** | Supported (material level) | Not supported |
| **Iris** | Supported | Not supported |
| **Barn Door** | Not supported | Supported |
| **Light Function** | Supported (Gobo projection) | Not supported |
| **Suitable Fixtures** | Spot/Beam/Profile lights | Wash/panel/soft lights |

---

## 2. Component Structure

```
SuperRectComponent (inherits from SuperLightingComponent)
  ├── YStaticMeshLens  — Lens model (inherited)
  ├── YArrowComponent  — Direction arrow (inherited)
  └── YRectLight       — Rectangular area light source (RectLight, produces planar lighting)
```

> **Note**: RectLight does not use light function materials, so Gobo pattern projection is not supported. Strobe effects are achieved by directly controlling light source intensity via Tick.

---

## 3. Default Parameters

### 3.1 Area Light Dimensions

| Parameter | Description | Range | Default | Unit |
|------|------|------|--------|------|
| **SourceWidth** | Width of the rectangular light source | 0 ~ 200 | 20.0 | cm |
| **SourceHeight** | Height of the rectangular light source | 0 ~ 200 | 20.0 | cm |

> **Tip**: Area light dimensions should match the fixture's actual light-emitting area. For example, a 30cm × 30cm LED panel light should be set to SourceWidth = 30, SourceHeight = 30.

### 3.2 Barn Door

| Parameter | Description | Range | Default | Unit |
|------|------|------|--------|------|
| **BarnDoorAngle** | Barn door opening angle, controls the spread range of light | 0 ~ 80 | 20.0 | degrees |
| **BarnDoorLength** | Barn door blade length, affects the sharpness of light edges | 0 ~ 1000 | 20.0 | cm |

#### Barn Door Working Principle

The Barn Door is a beam control mechanism unique to rectangular fixtures:

```
        ┌──────────────┐ ← Barn door blades
       ╱                ╲
      ╱    BarnDoorAngle  ╲  ← Opening angle
     ╱                      ╲
    ╱                        ╲
   ╱   RectLight Emitting Area  ╲
  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  │    SourceWidth × SourceHeight  │
  └────────────────────────────────┘
```

| BarnDoorAngle | Effect |
|---------------|------|
| 0° | Barn door fully closed, extremely narrow light range |
| 20° (default) | Moderate light spread |
| 80° | Barn door widely opened, very wide light range |

| BarnDoorLength | Effect |
|----------------|------|
| Small value | Short barn door, softer light edges |
| Large value | Long barn door, sharper edges, more pronounced shading |

### 3.3 Inherited Default Parameters

| Parameter | Description | Default |
|------|------|--------|
| **DimmerCurveExponent** | Dimmer response curve exponent | 2.0 |
| **MaxLightIntensity** | Component maximum brightness percentage (100 = full) | 100.0 |
| **StaticMeshLens** | Lens model | — |
| **LensTransform** | Lens transform | No offset |

> **Auto-Initialization**: Component default parameters are automatically initialized by `OnRegister()`. During component registration, `SetLightingMaterial()` is automatically called to create material instances, then `SetLightingDefaultValue()` sets the defaults. No need to manually call in the Actor.

---

## 4. Light Initialization Parameters

The following parameters are set by the fixture Actor during fixture initialization:

| Parameter | Description | Default | Unit |
|------|------|--------|------|
| **Max Light Distance** | RectLight attenuation radius | 2345.0 | cm |
| **Max Brightness** | Light source maximum brightness multiplier | 100.0 | — |
| **Volumetric Scattering Intensity** | RectLight volumetric fog scattering intensity | — | — |
| **Cast Shadows** | Whether RectLight enables shadow casting | — | — |
| **Aperture Brightness (LightSpotIntensity)** | Aperture material brightness multiplier | 1.0 | — |
| **Lens Brightness** | Lens material brightness multiplier | 1.0 | — |

### Shadow Configuration

RectLight automatically configures the following shadow parameters during initialization:

| Parameter | Value | Description |
|------|-----|------|
| Shadow resolution scale | 0.25 | Lower shadow resolution to save performance |
| Shadow bias | 0.2 | Prevent shadow self-intersection |
| Shadow slope bias | 0.2 | Prevent shadow banding on slopes |
| Ray-traced shadows | Off | Do not use RTX shadows |

---

## 5. DMX-Controlled Parameters

### 5.1 Intensity / Dimmer

| Operation | Description | Parameter Range |
|------|------|----------|
| **Set Brightness** | Simultaneously controls RectLight and material brightness | 0.0 (off) ~ 1.0 (full) |

Brightness is simultaneously applied to:
- RectLight intensity (actual lighting brightness)
- Lens material Brightness (lens glow effect)
- Aperture material Brightness

Final brightness calculation:
```
RectLight Intensity = Aperture Base Brightness × DMX Brightness × ComponentDimmer × Strobe Multiplier
```

### 5.2 Color

| Operation | Description | Parameter Range |
|------|------|----------|
| **Set Color** | Simultaneously sets RectLight color and material color | RGB linear color |

### 5.3 Strobe

RectLight strobe is implemented by **Tick directly controlling light source intensity** (does not use light functions). Supports the same 8 strobe modes as the base class:

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

### 5.4 Visibility Control

| Operation | Description |
|------|------|
| **Set Visibility** | Simultaneously controls RectLight and lens show/hide |

### 5.5 Lighting Channels

| Parameter | Description | Default |
|------|------|--------|
| **Channel 0** | Whether to enable lighting channel 0 | On |
| **Channel 1** | Whether to enable lighting channel 1 | Off |
| **Channel 2** | Whether to enable lighting channel 2 | Off |

### 5.6 Lighting Transmission

| Parameter | Description | Default |
|------|------|--------|
| **Affect Translucent Materials** | Whether RectLight illuminates translucent objects | On |

---

## 6. Unsupported Features

Due to RectLight feature limitations, the following functions are **unavailable** in the rectangular area light component:

| Feature | Reason |
|------|------|
| **Zoom** | RectLight uses barn doors to control light spread, not cone angle zoom |
| **Frost** | RectLight is inherently planar soft light, no additional frost needed |
| **Iris** | RectLight has no iris mechanism |
| **Gobo Pattern** | RectLight does not support light functions, cannot project patterns |
| **Pattern Rotation** | No pattern to rotate |
| **Prism** | RectLight does not support prism effects |
| **Focus** | No pattern to focus |
| **Color Wheel** | Color is directly controlled via RGB |

---

## 7. Performance Characteristics

| Feature | Description |
|------|------|
| **Rendering Overhead** | RectLight rendering overhead slightly higher than SpotLight (area light calculation is more complex) |
| **Shadow Overhead** | Uses lower shadow resolution scale (0.25), shadow overhead is manageable |
| **Strobe Overhead** | Strobe modes 0/1 have no additional overhead; modes 2~7 require Tick updates |
| **No Light Function** | Does not use light functions, saving function atlas overhead |

---

## 8. FAQ

### Q: How to control the illumination spread of area light?
**A**: Control via **BarnDoorAngle** (barn door angle). Smaller angles = narrower spread; larger angles = wider spread.

### Q: Area light edges are too hard/too soft, how to adjust?
**A**: Adjust **BarnDoorLength** (barn door length). Longer = harder edges (more pronounced shading); shorter = softer edges.

### Q: Why can't RectLight project Gobo patterns?
**A**: This is a UE RectLight limitation — rectangular area light sources do not support light functions. Use SuperSpotComponent or SuperBeamComponent if pattern projection is needed.

### Q: What unit is used for area light brightness?
**A**: RectLight uses Candelas as the luminous intensity unit and disables inverse squared falloff for more intuitive brightness control in stage design.

### Q: How to simulate pixel control of LED panel lights?
**A**: Area light components do not support pixel-level control. Use **SuperMatrixComponent** (matrix light component) in combination with area light components for pixel-level control.

---

## 9. API Quick Reference

The following are public function signatures of `USuperRectComponent`:

| Function Signature | Description |
|----------|------|
| `virtual void SetLightingMaterial() override` | Create dynamic material instances |
| `virtual void SetLightingDefaultValue() override` | Push default parameters to RectLight |
| `virtual void SetLightingIntensity(const float NewLightIntensity = 1.0f) override` | Simultaneously update RectLight intensity and material brightness |
| `virtual void SetLightingColor(const FLinearColor NewColor = FLinearColor(1,1,1)) override` | Simultaneously set RectLight color and material color |
| `virtual void SetLightingVisibility(const bool bNewVisibility = false) override` | RectLight + lens visibility |
| `void InitializeLightFunction() const` | Initialize RectLight shadow, channel configuration |
