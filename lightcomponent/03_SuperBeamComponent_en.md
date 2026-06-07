# 03 - Volumetric Beam Component (SuperBeamComponent)

> **Module**: SuperStage Runtime  
> **Target Audience**: Lighting designers, fixture Blueprint creators  
> **Prerequisite Reading**: [02 - Spotlight Component](02_SuperSpotComponent.md)

---

## 1. Overview

**SuperBeamComponent** is a component that adds **visible volumetric beams** on top of the spotlight component. It renders a "visible light column" effect from the fixture lens to the ground/target in the scene using a dedicated beam static mesh and volumetric light material, making it the most visually impactful light component in SuperStage.

In real performance scenarios, light beams become visible when there is fog/haze on stage. SuperBeamComponent simulates exactly this effect — rendering clear volumetric beams regardless of whether real fog particle effects exist in the scene.

### Use Cases

- **Beam Lights** — Beam lights, light column lights needing clearly visible volumetric beams
- **Profile Lights** — Imaging lights combined with volumetric beams to display Gobo patterns
- **Spot + Beam** — Fixtures needing both light projection and visible beams
- **As base class for Cutting Component (CuttingComponent)**

---

## 2. Component Structure

```
SuperBeamComponent (inherits from SuperSpotComponent)
  ├── YStaticMeshLens   — Lens model (inherited)
  ├── YArrowComponent   — Direction arrow (inherited)
  ├── YSpotLight        — Spotlight source (inherited)
  └── YStaticMeshBeam   — Beam model (new, cone/cylinder volumetric mesh)
```

### Three Material Systems

SuperBeamComponent manages **three** independent sets of dynamic materials, ensuring visual consistency between lens, aperture, and beam:

| Material | Purpose | Affected Parameters |
|------|------|-------------|
| **DynamicMaterialLens** | Lens glow effect | Brightness, Color, Gobo, Rotation, Prism, Focus, Color Wheel |
| **DynamicMaterialLightSpot** | SpotLight light function | Brightness, Color, Gobo, Rotation, Prism, Focus, Color Wheel |
| **DynamicMaterialBeam** | Beam volumetric effect | Brightness, Color, Zoom, Frost, Atmospheric Density, Gobo, Rotation, Prism, Focus, Color Wheel |

> **Core Design**: When you set Color, Gobo patterns, Prism, etc., all three material systems are **synchronously updated**, ensuring that the pattern seen on the lens, the pattern displayed in the beam, and the aperture pattern projected onto the ground are completely consistent.

---

## 3. Default Parameters

### 3.1 Beam Quality Parameters

The following parameters are set during fixture initialization and determine the visual quality of the beam:

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **Max Brightness** | Beam material maximum brightness baseline (linked to fixture max brightness) | ≥ 1 | 100.0 |
| **Beam Intensity** | Beam material brightness multiplier, independent of SpotLight brightness | ≥ 0 | 1.0 |
| **Atmospheric Density** | Density of simulated atmospheric particles in the beam; higher values = "thicker" beam | 0 ~ 1 | 0.03 |
| **Lens Radius** | Initial radius of the beam at the lens position | 1 ~ 100 | 10.0 |
| **Beam Quality** | Controls sampling precision of beam volumetric effect; higher values = finer beam but more overhead | 0 ~ 100 | 75.0 |
| **Fog Intensity** | Degree of fog/haze effect on the beam; 0 = no fog texture, high values = visible fog flow (written to material ×0.02) | 0 ~ 100 | 20.0 |
| **Fog Speed** | Flow speed of fog texture in the beam (written to material ×0.02) | 0 ~ 100 | 10.0 |
| **Beam Block** | Enable beam collision occlusion detection, dynamically updates attenuation radius based on Zoom angle to prevent light penetration | On/Off | Off |
| **Disable Beam** | Hide volumetric beam mesh, keeping only light source and aperture | On/Off | Off |

> **Examples**:
> - High-power Beam light: high atmospheric density, small lens radius, high beam brightness → thin and bright light column
> - Wide-angle Wash light: low atmospheric density, large lens radius, low beam brightness → wide and soft light haze

### 3.2 Inherited Default Parameters

| Parameter | Source | Description |
|------|------|------|
| **ZoomRange** | SuperSpotComponent | Zoom angle range |
| **bDisableLightFunction** | SuperSpotComponent | Light function switch |
| **Angle** | SuperLightingComponent | Aperture angle |
| **DimmerCurveExponent** | SuperLightingComponent | Dimmer curve exponent |
| **MaxLightIntensity** | SuperLightingComponent | Component-level brightness sub-control |

> **Auto-Initialization**: Component default parameters are automatically initialized by `OnRegister()`. During component registration, `SetLightingMaterial()` and `SetBeamDefaultValue()` are automatically called. No need to manually call in the Actor.

---

## 4. DMX-Controlled Parameters

### 4.1 Intensity / Dimmer

| Operation | Description | Parameter Range |
|------|------|----------|
| **Set Brightness** | Simultaneously controls SpotLight, lens material and **beam material** brightness | 0.0 ~ 1.0 |

Final brightness calculation:
```
Beam Brightness = Actor-level Dimmer × ComponentDimmer × Strobe Multiplier
```

When brightness is 0, the beam material's Brightness parameter is zeroed and the beam completely disappears visually.

### 4.2 Strobe

The beam component overrides the base class's strobe interfaces so strobe simultaneously affects **SpotLight, lens, aperture, and beam** four material systems:

```cpp
virtual void SetLightingStrobe(const float NewStrobe = 0.0f) override;
virtual void SetLightingStrobeMode(const float NewStrobeMode = 1.0f) override;
virtual void SetRandomSeed(const float NewSeed) override;
```

| Parameter | Description | Range |
|------|------|------|
| **Strobe Speed (NewStrobe)** | Controls flashing speed; higher values = faster flashing | 0.0 ~ any positive value |
| **Strobe Mode (NewStrobeMode)** | Controls flashing waveform (0=closed, 1=always on, 2~7=dynamic waveforms) | 0 ~ 7 |
| **Random Seed (NewSeed)** | Used only in random mode (Mode 7), keeps each fixture's flashing desynchronized | Any float value |

> **Difference from Base Class**: The base class only updates lens + aperture materials; the beam component's override additionally updates the beam material's Brightness parameter, ensuring the beam flashes in sync with other visual elements.

### 4.3 Color

| Operation | Description |
|------|------|
| **Set Color** | Simultaneously sets SpotLight, lens and **beam** color |

Beam color is controlled via the material parameter `LightColor`, kept in sync with the SpotLight color.

### 4.4 Zoom

| Parameter | Description | Range |
|------|------|------|
| **Zoom** | Simultaneously controls SpotLight cone angle and beam material scaling | 0.0 ~ 1.0 |

Zoom effects on the beam:

| Effect | Description |
|------|------|
| **SpotLight Angle** | Zoom mapping inherited from SuperSpotComponent |
| **Beam Material LightZoom** | Beam expansion degree, linearly interpolated from ZoomRange |
| **Beam Material MaxLightDistance** | Maximum beam length |

### 4.5 Frost

| Parameter | Description | Range |
|------|------|------|
| **Frost** | Softens beam and aperture | 0.0 ~ 1.0 |

Special effects of frost on the beam:
- **Beam Material LightZoom**: Frost additionally increases beam Zoom value (Frost × 6.0), widening the beam
- **Beam Material LightFrost**: Directly controls the beam's frost parameter

### 4.6 Iris

| Parameter | Description | Range |
|------|------|------|
| **Iris** | Iris opening | 0.0 (fully closed) ~ 1.0 (fully open) |

Iris effects on the beam (**can only shrink, not expand**):

| Effect | Calculation |
|------|----------|
| **Beam LensRadius** | `Default Lens Radius × Iris`, aperture shrink makes beam root thinner |
| **Beam LightZoom** | `Current Zoom value × Iris`, aperture shrink makes beam overall narrower |
| **SpotLight Angle** | Iris effect inherited from SuperSpotComponent |
| **Lens/Aperture LensRadius** | Synchronously updated for consistency |

### 4.7 Gobo Pattern (Beam Texture)

| Parameter | Description |
|------|------|
| **BeamTexture** | Gobo pattern texture, projected into the beam volume |
| **NumGobos** | Number of Gobo patterns in the texture atlas |
| **GoboIndex** | Currently selected Gobo pattern number |
| **GoboSpeed** | Gobo pattern rotation speed |
| **ShakeSpeed** | Gobo pattern shake effect speed |

> **Core Feature**: Gobo patterns are **simultaneously** applied to the beam, aperture, and lens three material systems, ensuring consistent appearance from any viewing angle.

### 4.8 Color Wheel

Same as the spotlight component, supports up to **3 sets of color wheels**. Each set's parameters (texture, count, index, speed) are synchronously applied to the beam, aperture, and lens three material systems.

### 4.9 Prism

The prism system is driven by **prism preset data assets** (`USuperPrismPreset`). Each prism preset defines multiple split-face positions, offsets and scales, with prism effects implemented in materials via position lookup textures (48×1 RGBA16F).

| Parameter | Description | Range |
|------|------|------|
| **PrismPreset** | Prism preset data asset; `nullptr` = prism off | Data asset reference |
| **PrismLayerIndex** | Layer index within the prism preset (multi-layer prism support) | ≥ 0 |
| **PrismRotation** | Static rotation angle of the prism | Any |
| **PrismRotationSpeed** | Continuous rotation speed of the prism | Any |

Prism effects are synchronously applied to the beam, aperture, and lens three material systems.

> **Preset Editor**: In the UE editor, double-click a `USuperPrismPreset` asset to open the visual editor, where you can edit prism split-face positions and scales via drag-and-drop, or use built-in templates (Circle, Line, Triangle, Square) for quick generation.

### 4.10 Focus

| Parameter | Description | Range |
|------|------|------|
| **Focus** | Controls Gobo pattern sharpness | 0.0 (sharp) ~ 1.0 (blurry) |

Focus is synchronously applied to the `Focus` parameter of the beam, aperture, and lens three material systems.

### 4.11 Pattern Rotation (Rotate)

| Parameter | Description |
|------|------|
| **Rotation Angle** | Simultaneously rotates patterns on beam, aperture, and lens |
| **Infinite Rotation** | Patterns on beam, aperture, and lens continuously rotate |

### 4.12 Atmospheric Density Runtime Control

```cpp
void SetBeamAtmosphericDensity(const float Density) const;
```

| Parameter | Description | Range |
|------|------|------|
| **Density** | Beam atmospheric density; higher values = "thicker" beam | 0 ~ 1 |

### 4.13 Beam Disabled

| Parameter | Description | Default |
|------|------|--------|
| **bDisableBeam** | Whether to disable beam volumetric effect (only disables beam mesh, does not affect SpotLight lighting) | false (enabled) |

### 4.14 Visibility Control

| Operation | Description |
|------|------|
| **Set Visibility** | Controls beam mesh show/hide (**does not affect SpotLight lighting**) |

---

## 5. Visual Effect Adjustment Guide

### 5.1 Beam Density
- **Atmospheric Density** — Increase for overall "denser" beam
- **Fog Influence** — Increase for visible fog texture on beam

### 5.2 Beam Sharpness
- **Beam Quality** — Increase for clearer beam edges, but higher performance cost
- **Lens Radius** — Decrease for thinner beam root, appearing more focused
- **Focus** — Decrease for clearer Gobo patterns

### 5.3 Fog Effect
- **Fog Influence** — Controls visibility of fog texture
- **Fog Speed** — Controls how fast fog flows

### 5.4 Beam Brightness
- **Beam Intensity** — Independent of SpotLight, controls beam brightness separately
- **ComponentDimmer** — Simultaneously affects SpotLight and beam
- **DMX Dimmer** — Simultaneously affects all visual elements

---

## 6. FAQ

### Q: Beam and aperture colors/patterns don't match?
**A**: This shouldn't happen under normal use since all parameters are synchronously written to all three material systems. If inconsistency is found, check for custom code that only modifies partial materials.

### Q: Beam effect is very blurry / Gobo patterns hard to see?
**A**: Increase the Beam Quality parameter. Note this will increase GPU overhead.

### Q: How to show beam only without lighting?
**A**: Set SpotLight intensity to very low while keeping beam material brightness normal. Or don't bind light function material in the fixture Blueprint.

### Q: Beam noticeably jitters when moving/rotating?
**A**: This is caused by the zoom debounce mechanism with thresholds of 0.1° angle change and 1cm attenuation radius change. For smoother transitions, add interpolation logic at the fixture Actor level.

---

## 7. API Quick Reference

The following are public function signatures of `USuperBeamComponent` (including inherited and new):

| Function Signature | Description |
|----------|------|
| `void SetBeamDefaultValue() const` | Initialize beam material default parameters (auto-called by OnRegister) |
| `virtual void SetLightingMaterial() override` | Create three dynamic materials (lens + aperture + beam) |
| `virtual void SetLightingIntensity(const float NewLightIntensity = 1.0f) override` | Brightness (synchronously updates beam material) |
| `virtual void SetLightingStrobe(const float NewStrobe = 0.0f) override` | Strobe speed (synchronous beam material) |
| `virtual void SetLightingStrobeMode(const float NewStrobeMode = 1.0f) override` | Strobe mode |
| `virtual void SetRandomSeed(const float NewSeed) override` | Random seed |
| `virtual void SetLightingColor(const FLinearColor NewColor = FLinearColor(1,1,1)) override` | Color (three materials sync) |
| `virtual void SetLightingZoom(const float NewZoom = 0.0f) override` | Zoom (SpotLight + beam scaling) |
| `virtual void UpdateBeamBlockDistance(const float NewMaxLightDistance) override` | Update beam occlusion distance |
| `virtual void SetLightingFrost(const float NewFrost = 0.0f) override` | Frost (beam additional widening) |
| `virtual void SetLightingIris(const float NewIris = 1.0f) override` | Iris (beam LensRadius shrinkage) |
| `virtual void SetLightingRotate(const float NewRotate = 0, const float NewInfiniteRotation = 0) override` | Pattern rotation (three materials sync) |
| `void SetBeamTexture(UTexture2D* = nullptr, int32 NewNumGobos = 1, float GoboIndex = 0, float GoboSpeed = 0, float ShakeSpeed = 0) const` | Gobo pattern (three materials sync) |
| `virtual void SetColorTexture(UTexture2D*, int32, float, float) override` | Color wheel 1 |
| `virtual void SetColorTexture2(UTexture2D*, int32, float, float) override` | Color wheel 2 |
| `virtual void SetColorTexture3(UTexture2D*, int32, float, float) override` | Color wheel 3 |
| `void SetBeamPrism(USuperPrismPreset* = nullptr, int32 PrismLayerIndex = 0, float PrismRotation = 0, float PrismRotationSpeed = 0) const` | Prism |
| `void SetBeamFocus(const float Focus = 0.0f) const` | Focus |
| `void SetBeamAtmosphericDensity(const float Density) const` | Runtime atmospheric density |
| `void SetBeamDisabled(const bool bDisabled)` | Beam disable |
| `virtual void SetLightingVisibility(const bool bNewVisibility = false) override` | Beam visibility |
