# 01 - Base Light Component (SuperLightingComponent)

> **Module**: SuperStage Runtime  
> **Target Audience**: Lighting designers, fixture Blueprint creators  
> **Prerequisite Reading**: [00 - Light Component Overview](00_LightComponent_Overview.md)

---

## 1. Overview

**SuperLightingComponent** is the **base class** (root component) of all light components, providing the core foundational capabilities for light rendering. It manages two material systems — **lens material** and **aperture material** — as well as common lighting parameters such as brightness, color, strobe, and pattern rotation.

Although lighting designers rarely use this base component directly (typically using its subclasses such as SuperSpotComponent or SuperBeamComponent), understanding its parameters is very important for comprehending the entire light component system, as **all subclass components inherit these foundational capabilities**.

### Use Cases

- Serves as the common parameter foundation for all light components
- Simple fixtures that only need "lens glow + aperture projection" without real lighting (SpotLight/RectLight)
- Fundamental building block for custom fixture Blueprints

---

## 2. Component Structure

```
SuperLightingComponent (Scene Component)
  ├── YStaticMeshLens  — Lens model (static mesh, visualizes the fixture lens)
  └── YArrowComponent  — Direction arrow (indicates light emission direction in editor, not rendered at runtime)
```

### Material Systems

This component maintains two independent sets of dynamic materials:

| Material | Purpose | Description |
|------|------|------|
| **DynamicMaterialLens** | Lens material | Controls the glow effect of the lens mesh, reflecting brightness/color/pattern |
| **DynamicMaterialLightSpot** | Aperture material | Controls the projection effect of the aperture (bound to actual light source by subclasses) |

> **Note**: Both material systems simultaneously receive brightness, color and other parameter updates, ensuring visual consistency between the lens glow and aperture projection.

---

## 3. Default Parameters

The following parameters are set in the fixture Blueprint's Details Panel, defining the fixture's basic characteristics.

### 3.1 Lens Model

| Parameter | Description | Type | Default |
|------|------|------|--------|
| **StaticMeshLens** | Static mesh model used for the lens. Different fixtures can specify differently shaped lenses | Static Mesh Reference | None |
| **LensTransform** | Position, rotation and scale offset of the lens relative to the component | Transform | No offset |

> **Tip**: The lens model's position and size should match the lens opening of the fixture head model; otherwise the lens may appear floating or embedded in the fixture body.

### 3.2 Beam Angle

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **Angle** | Aperture angle (Spot angle), controls the projection range value of the aperture material | — | 1.0 |

> **Note**: This parameter only affects the aperture material's angle reference value. Actual light cone angle control is implemented by subclasses (such as SuperSpotComponent) through the Zoom function.

### 3.3 Dimmer Curve

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **DimmerCurveExponent** | Dimmer response curve exponent, controls non-linear mapping from DMX value to brightness | 1.0 ~ 3.0 | 2.0 |

Brightness calculation formula: `Output Brightness = DMX value ^ Exponent`

| Exponent | Effect | Recommendation |
|------|------|----------|
| 1.0 | Linear mapping, rapid changes in low brightness range | Not recommended |
| **2.0** | **Square curve, natural and uniform dimming transitions** | **Recommended** |
| 2.2 | Gamma correction standard | Suitable |
| 3.0 | Cubic curve, extremely gentle in low range | Special needs |

### 3.4 Component-Level Brightness Sub-Control

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **MaxLightIntensity** | Component maximum brightness percentage (design-time config, 100 = full brightness) | ≥ 1 | 100.0 |

Additionally, each component provides a runtime brightness sub-control interface `SetComponentDimmer(float NewDimmer)`, with a range of 0.0 ~ 1.0.

Final brightness formula:
```
Final Brightness = Actor-level Dimmer × (MaxLightIntensity / 100) × ComponentDimmer × Strobe Multiplier
```

> **Use Case**: When a fixture has both a main light source (SuperBeamComponent) and auxiliary light source (SuperSpotComponent), you can set the auxiliary source's MaxLightIntensity to 50 so the auxiliary light's max brightness is always half of the main light.

> **Auto-Initialization**: All default parameters are automatically initialized by component `OnRegister()`. During component registration, `SetLightingMaterial()` is automatically called to create materials, then `SetLightingDefaultValue()` sets the default parameters. No need to manually call in the Actor.

---

## 4. DMX-Controlled Parameters

The following parameters are driven in real-time by DMX channels, controlling the fixture's dynamic behavior.

### 4.1 Intensity / Dimmer

| Operation | Description | Parameter Range |
|------|------|----------|
| **Set Brightness** | Controls overall light brightness | 0.0 (off) ~ 1.0 (full) |

The brightness value simultaneously updates the `Brightness` parameter of both the lens material and aperture material, keeping lens and aperture brightness consistent.

### 4.2 Color

| Operation | Description | Parameter Range |
|------|------|----------|
| **Set Color** | Controls the light emission color | RGB linear color, each channel 0.0 ~ 1.0 |

The color value is simultaneously applied to the `LightColor` parameter of both the lens material and aperture material.

> **Examples**:  
> - White: R=1.0, G=1.0, B=1.0  
> - Red: R=1.0, G=0.0, B=0.0  
> - Amber: R=1.0, G=0.5, B=0.0  

### 4.3 Strobe

The strobe system is jointly controlled by **strobe speed** and **strobe mode** parameters:

| Parameter | Description | Range |
|------|------|------|
| **Strobe Speed** | Controls flashing speed; higher values mean faster flashing | 0.0 (no flashing) ~ any positive value |
| **Strobe Mode** | Controls the flashing waveform | 0 ~ 7 (see table below) |
| **Random Seed** | Used only in random mode (Mode 7), keeps each fixture's flashing desynchronized | Any float value |

#### Strobe Mode Details

| Mode | Name | Waveform | Visual Effect |
|------|------|------|----------|
| **0** | Closed | — | Completely off, light extinguished |
| **1** | Open | — | Always on, no strobe (default) |
| **2** | Linear | Triangle wave (smooth) | Smooth fade in/out, soft and natural transitions |
| **3** | Pulse | Square wave | Hard-switch flashing, equal on/off |
| **4** | Ramp Up | Rising sawtooth | Slowly brightens from dark, instantly drops |
| **5** | Ramp Down | Falling sawtooth | Slowly dims from bright, instantly brightens |
| **6** | Sine | Sine wave | Softest pulsation effect |
| **7** | Random | Random | Irregular flashing, seed prevents sync |

> **Performance Notes**:  
> - Modes **0** (Closed) and **1** (Open): No additional performance cost; component automatically disables Tick  
> - Modes **2~7**: Requires per-frame strobe multiplier calculation; component automatically enables Tick

### 4.4 Frost

| Operation | Description | Parameter Range |
|------|------|----------|
| **Set Frost** | Softens aperture edges, making the aperture hazy | 0.0 (sharp) ~ 1.0 (fully frosted) |

The frost effect is achieved by modifying the aperture material's `LightFrost` parameter.

### 4.5 Texture

| Operation | Description | Parameter |
|------|------|------|
| **Set Texture** | Applies a Gobo pattern texture to the lens and aperture materials | UTexture2D* texture pointer (nullptr = clear pattern) |

```cpp
virtual void SetLightingTexture(UTexture2D* NewBeamTexture);
```

The texture is simultaneously updated to the `LightTexture` parameter of both the lens material and aperture material, ensuring the pattern displayed on the lens matches the projected aperture pattern.

> **Note**: The base class only accepts a texture pointer as a single parameter. The subclass SuperBeamComponent provides an enhanced `SetBeamTexture()` that supports additional parameters such as Gobo count, index, rotation speed, and shake speed.

### 4.6 Rotate

| Parameter | Description | Range |
|------|------|------|
| **Rotation Angle (Rotate)** | Static rotation angle of the pattern | 0.0 ~ 360.0 (degrees) |
| **Infinite Rotation Speed** | Continuous rotation speed of the pattern; positive = clockwise, negative = counter-clockwise | Any float value |

> **Tip**: Rotation angle and infinite rotation can be used simultaneously. Rotation angle sets the initial orientation; infinite rotation provides continuous rotation.

### 4.7 Zoom

```cpp
virtual void SetLightingZoom(const float NewZoom = 0.0f);
```

| Parameter | Description | Range |
|------|------|------|
| **NewZoom** | Normalized zoom value | 0.0 (narrowest) ~ 1.0 (widest) |

> **Base Class Note**: Base class provides empty implementation; actual SpotLight cone angle mapping and beam scaling is implemented by subclass SuperSpotComponent.

### 4.8 Iris

```cpp
virtual void SetLightingIris(const float NewIris = 1.0f);
```

| Parameter | Description | Range |
|------|------|------|
| **NewIris** | Iris opening (can only shrink) | 0.0 (fully closed) ~ 1.0 (fully open) |

> **Base Class Note**: Base class provides empty implementation; actual iris control is implemented by subclass SuperSpotComponent.

### 4.9 Color Wheel

```cpp
virtual void SetColorTexture (UTexture2D* NewColorTexture, const int32 NewNumColors, const float ColorIndex, const float ColorSpeed) {}
virtual void SetColorTexture2(UTexture2D* NewColorTexture, const int32 NewNumColors, const float ColorIndex, const float ColorSpeed) {}
virtual void SetColorTexture3(UTexture2D* NewColorTexture, const int32 NewNumColors, const float ColorIndex, const float ColorSpeed) {}
```

The base class declares **3 sets of color wheel** virtual functions (empty implementation), which are overridden by subclasses SuperSpotComponent / SuperBeamComponent to push actual material parameters.

| Parameter | Description |
|------|------|
| **NewColorTexture** | Color wheel texture (atlas containing all color slots) |
| **NewNumColors** | Number of color slots on the color wheel (int32) |
| **ColorIndex** | Currently selected color position (float) |
| **ColorSpeed** | Color wheel rotation speed (float) |

### 4.10 Visibility Control

| Operation | Description |
|------|------|
| **Set Light Visibility** | Controls overall show/hide of the light component (base class has no actual effect; implemented by subclasses) |
| **Set Lens Visibility** | Individually controls show/hide of the lens model |

---

## 5. Collision Detection (Ray Detection)

The base light component has built-in ray collision detection for calculating the distance from the light to the nearest obstacle, preventing light from penetrating walls or floors.

### Detection Principle

1. Cast a ray from the fixture's arrow component position (along the light emission direction)
2. **Level 1**: Sphere sweep detection (radius 2cm), checks visibility channel
3. **Level 2** (fallback): Finer sphere sweep (radius 0.5cm), prevents missed detections
4. Returns collision distance (minimum 100cm, prevents zero light distance)

### Collision Parameters

| Parameter | Description | Default |
|------|------|--------|
| Minimum return distance | Minimum value returned by collision detection | 100 cm |
| Sweep radius | Radius of sphere sweep | 2 cm |
| Fallback detection | Whether to perform finer secondary detection when primary misses | Enabled |
| Detection channel | Collision channel used | Visibility |

> **Note**: Collision detection ignores the fixture's own Actor and direction arrow component to prevent the fixture from detecting itself.

---

## 6. Lighting Channels and Transmission

| Operation | Description |
|------|------|
| **Set Lighting Channels** | Controls which rendering channels the light affects (channels 0/1/2) |
| **Set Lighting Transmission** | Controls whether the light affects translucent materials |

> **Base Class Note**: These two functions are empty implementations in the base class and are implemented by subclasses (SuperSpotComponent, SuperRectComponent, etc.) based on actual light source types.

---

## 7. Relationship with Other Components

| Subclass Component | Capabilities Added on Top of Base Class |
|----------|----------------------|
| **SuperSpotComponent** | Real SpotLight light source + Zoom + Iris |
| **SuperBeamComponent** | Visible volumetric beam mesh + atmospheric scattering |
| **SuperCuttingComponent** | Four-leaf shutter cut cropping |
| **SuperRectComponent** | Real RectLight area light source + barn doors |

> **Tip**: If your fixture only needs "lens glow effect" without real light projection, you can directly use SuperLightingComponent as the light component to save rendering overhead.

---

## 8. API Quick Reference

The following are all public function signatures of `USuperLightingComponent` (subclasses can `override`):

| Function Signature | Description |
|----------|------|
| `void SetComponentDimmer(const float NewDimmer)` | Component-level brightness sub-control (0.0 ~ 1.0) |
| `virtual void SetLightingMaterial()` | Create/bind dynamic material instances (auto-called by OnRegister) |
| `virtual void SetLightingDefaultValue()` | Push default parameters to materials (auto-called by OnRegister) |
| `virtual void SetLightingIntensity(const float NewLightIntensity = 1.0f)` | Set brightness (0.0 ~ 1.0) |
| `virtual void SetLightingStrobe(const float NewStrobe = 0.0f)` | Set strobe speed |
| `virtual void SetLightingStrobeMode(const float NewStrobeMode = 1.0f)` | Set strobe mode (0 ~ 7) |
| `virtual void SetRandomSeed(const float NewSeed)` | Set random seed |
| `virtual void SetLightingColor(const FLinearColor NewColor = FLinearColor(1,1,1))` | Set color |
| `virtual void SetLightingZoom(const float NewZoom = 0.0f)` | Set zoom (base class empty impl) |
| `virtual void SetLightingFrost(const float NewFrost = 0.0f)` | Set frost |
| `virtual void SetLightingIris(const float NewIris = 1.0f)` | Set iris (base class empty impl) |
| `virtual void SetLightingTexture(UTexture2D* NewBeamTexture)` | Set texture (texture pointer only, 1 parameter) |
| `virtual void SetColorTexture(UTexture2D*, int32, float, float)` | Color wheel 1 (base class empty impl) |
| `virtual void SetColorTexture2(UTexture2D*, int32, float, float)` | Color wheel 2 (base class empty impl) |
| `virtual void SetColorTexture3(UTexture2D*, int32, float, float)` | Color wheel 3 (base class empty impl) |
| `virtual void SetLightingRotate(const float NewRotate = 0, const float NewInfiniteRotation = 0)` | Pattern rotation |
| `virtual void SetLightingVisibility(const bool bNewVisibility = false)` | Light visibility |
| `virtual void SetLightingLensVisibility(const bool bLensVisibility = true)` | Lens visibility |
| `float GetRayDetectionDistance(const float NewMaxLightDistance) const` | Collision detection distance calculation |
| `virtual void UpdateBeamBlockDistance(const float NewMaxLightDistance)` | Update beam occlusion distance (internal) |
