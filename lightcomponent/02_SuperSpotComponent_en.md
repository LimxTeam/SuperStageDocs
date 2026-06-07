# 02 - Spotlight Component (SuperSpotComponent)

> **Module**: SuperStage Runtime  
> **Target Audience**: Lighting designers, fixture Blueprint creators  
> **Prerequisite Reading**: [01 - Base Light Component](01_SuperLightingComponent.md)

---

## 1. Overview

**SuperSpotComponent** is a component that adds a **real SpotLight spotlight source** on top of the base light component. It is one of the most commonly used light components in SuperStage, capable of producing real lighting effects (shadows, volumetric scattering, etc.) and supporting professional fixture control functions such as Zoom, Frost, and Iris.

### Use Cases

- **Spot Lights** — Fixtures that only need aperture projection without visible beam effects
- **Follow Spots** — Fixtures requiring precise control of light cone angle and brightness
- **Auxiliary Light Sources** — As a fixture's Spot auxiliary light component
- **As base class for Beam/Cutting components** — Providing SpotLight-related foundational capabilities

---

## 2. Component Structure

```
SuperSpotComponent (inherits from SuperLightingComponent)
  ├── YStaticMeshLens  — Lens model (inherited)
  ├── YArrowComponent  — Direction arrow (inherited)
  └── YSpotLight       — Spotlight source (SpotLight, produces real lighting)
```

### Material Systems

On top of the base class's two material systems, the aperture material is bound as the SpotLight's **Light Function Material**, enabling effects such as Gobo pattern projection and color wheel simulation.

| Material | Purpose |
|------|------|
| **DynamicMaterialLens** | Lens glow effect (inherited from base class) |
| **DynamicMaterialLightSpot** | SpotLight's light function material, controls aperture pattern and effects |

> **Special Case**: When the `bDisableLightFunction` switch is on, the aperture material is not bound to the SpotLight, and strobe directly controls light source intensity via Tick instead.

---

## 3. Default Parameters

### 3.1 Zoom Range

| Parameter | Description | Type | Default |
|------|------|------|--------|
| **ZoomRange** | Zoom angle range (degrees), X = min angle, Y = max angle | 2D Vector | (1.0, 10.0) |

The DMX Zoom channel value (0~1) is mapped to this range:
- Zoom = 0.0 → Beam angle = ZoomRange.X (narrowest)
- Zoom = 1.0 → Beam angle = ZoomRange.Y (widest)

> **Actual Fixture Examples**:  
> - Narrow-angle Spot: ZoomRange = (3, 15)  
> - Wide-angle Wash: ZoomRange = (10, 50)  
> - Profile: ZoomRange = (5, 35)

### 3.2 Light Source Disable Switch

| Parameter | Description | Type | Default |
|------|------|------|--------|
| **bDisableLights** | Whether to disable the SpotLight source (when disabled, only lens and aperture material effects remain; no real lighting produced) | Boolean | false (enabled) |

> **Use Case**: When a fixture only needs lens glow and beam visual effects without real lighting (e.g., pure beam lights), enable this option to save rendering overhead.

### 3.3 Light Function Switch

| Parameter | Description | Type | Default |
|------|------|------|--------|
| **bDisableLightFunction** | Whether to disable the light function | Boolean | false (enabled) |

| State | Behavior |
|------|------|
| **Off** (default) | SpotLight uses light function material, supports Gobo pattern projection. Strobe controlled via material parameters |
| **On** | SpotLight does not use light function material; strobe directly controls light source intensity via Tick. Suitable for simple fixtures not needing pattern projection |

### 3.4 Inherited Default Parameters

The following parameters are inherited from the base class SuperLightingComponent:

| Parameter | Description | Default |
|------|------|--------|
| **Angle** | Aperture angle reference value | 1.0 |
| **DimmerCurveExponent** | Dimmer response curve exponent | 2.0 |
| **MaxLightIntensity** | Component maximum brightness percentage (100 = full) | 100.0 |
| **StaticMeshLens** | Lens model | — |
| **LensTransform** | Lens transform | No offset |

> **Auto-Initialization**: Component default parameters are automatically initialized by `OnRegister()`. During component registration, `SetLightingMaterial()` is automatically called to create material instances, then `SetLightingDefaultValue()` sets the defaults. No need to manually call in the Actor.

---

## 4. Light Initialization Parameters

The following parameters are set by the fixture Actor during fixture initialization to define the SpotLight's physical characteristics:

| Parameter | Description | Default | Unit |
|------|------|--------|------|
| **Max Light Distance** | SpotLight's attenuation radius, determines the farthest distance light can reach | 2345.0 | cm |
| **Max Brightness** | Light source maximum brightness multiplier | 100.0 | — |
| **Volumetric Scattering Intensity** | SpotLight's volumetric fog scattering intensity | — | — |
| **Cast Shadows** | Whether SpotLight enables shadow casting | — | — |
| **Aperture Brightness (LightSpotIntensity)** | Aperture material brightness multiplier | 1.0 | — |
| **Lens Brightness** | Lens material brightness multiplier | 1.0 | — |

---

## 5. DMX-Controlled Parameters

### 5.1 Intensity / Dimmer

| Operation | Description | Parameter Range |
|------|------|----------|
| **Set Brightness** | Simultaneously controls SpotLight source and material brightness | 0.0 (off) ~ 1.0 (full) |

Brightness is simultaneously applied to:
- SpotLight intensity (actual lighting brightness)
- Lens material Brightness (lens glow effect)
- Aperture material Brightness (pattern projection brightness)

### 5.2 Color

| Operation | Description | Parameter Range |
|------|------|----------|
| **Set Color** | Simultaneously sets SpotLight color and material color | RGB linear color |

Color is simultaneously applied to:
- SpotLight source color (affects actual lighting)
- Lens material LightColor (lens display color)
- Aperture material LightColor (pattern projection color)

### 5.3 Zoom

| Parameter | Description | Range |
|------|------|------|
| **Zoom** | Zoom value, controls the light cone angle size | 0.0 (narrowest) ~ 1.0 (widest) |
| **Beam Block Mode** | When enabled, calculates attenuation radius based on angle to prevent light penetration | On/Off |

#### Zoom Mapping Mechanism

The Zoom value is mapped to the actual SpotLight cone angle via a **piecewise linear interpolation table**:

```
DMX Zoom (0~1) → ZoomRange angle (degrees) → Table lookup interpolation → SpotLight OuterConeAngle
```

The interpolation table is based on measured data calibration to ensure consistent zoom feel across different angle ranges:

| Input Angle (degrees) | Output Cone Angle (degrees) | Description |
|----------------|----------------|------|
| 1.0 | 2.2 | Narrowest beam |
| 5.0 | 6.2 | — |
| 10.0 | 11.0 | — |
| 20.0 | 20.0 | — |
| 30.0 | 29.0 | — |
| 40.0 | 36.0 | — |
| 50.0 | 46.0 | Widest beam |

> **Performance Optimization**: Zoom mapping uses a "neighborhood search" algorithm — remembers the segment index hit in the previous frame and starts searching from that position in the next frame, typically hitting in 0~2 comparisons with near-zero overhead.

#### Debounce Mechanism

SpotLight angle and attenuation radius updates have **debounce processing**:
- Angle change < 0.1° → no update (avoids frequent render thread refreshes)
- Attenuation radius change < 1cm → no update

### 5.4 Frost

| Parameter | Description | Range |
|------|------|------|
| **Frost** | Frost level, softens aperture edges | 0.0 (sharp) ~ 1.0 (fully frosted) |

The frost effect is achieved in two ways:

1. **Material level**: Modifies the aperture material's `LightFrost` parameter, softening pattern edges
2. **SpotLight level**:
   - **Increases outer cone angle**: Every 0.1 Frost increase adds ~0.6° to outer cone angle, widening the beam
   - **Decreases inner cone angle ratio**: Frost=0 → inner cone is 90% of outer (hard edge); Frost=1 → inner cone is 1% of outer (extremely soft edge)

> **Design Principle**: A real frost filter simultaneously widens the beam and softens the edges; this implementation simulates both physical effects.

### 5.5 Iris

| Parameter | Description | Range |
|------|------|------|
| **Iris** | Iris opening, can only shrink, not expand | 0.0 (fully closed, minimum aperture) ~ 1.0 (fully open, maximum aperture) |

Iris effects:
- **SpotLight angle**: `Final angle = Base angle × Iris value`
- Iris can only **shrink** the beam, cannot exceed the base angle determined by current Zoom
- Iris stacks with Frost effect

> **Example**:  
> - Zoom = 0.5 → Base angle = 20°  
> - Iris = 0.5 → Final angle = 10° (shrunk to half)  
> - Iris = 1.0 → Final angle = 20° (fully open, equals base angle)

### 5.6 Pattern Rotation (Rotate)

| Parameter | Description | Range |
|------|------|------|
| **Rotation Angle** | Static rotation angle of Gobo pattern | 0 ~ 360 (degrees) |
| **Infinite Rotation Speed** | Continuous rotation speed of Gobo pattern | Any float value |

Rotation parameters are simultaneously applied to the SpotLight aperture material and lens material.

### 5.7 Color Wheel

The spotlight component supports up to **3 sets of color wheels**, simulating the color wheel mechanism of real fixtures:

| Color Wheel | Description |
|--------|------|
| **Color Wheel 1** | Primary color wheel |
| **Color Wheel 2** | Secondary color wheel (high-end fixtures) |
| **Color Wheel 3** | Tertiary color wheel (special fixtures) |

Parameters for each color wheel:

| Parameter | Description |
|------|------|
| **Color Texture** | Texture map of the color wheel (contains all color slots) |
| **Color Count** | Number of color slots on the color wheel |
| **Color Index** | Currently selected color position |
| **Color Rotation Speed** | Rotation speed of the color wheel (for half-color/rainbow effects) |

> **Tip**: Color wheel effects are simultaneously applied to the SpotLight aperture material and lens material to ensure visual consistency.

### 5.8 Visibility Control

| Operation | Description |
|------|------|
| **Set Visibility** | Simultaneously controls SpotLight source show/hide |

When visibility is off, the SpotLight stops rendering and lighting calculation, consuming zero GPU resources.

### 5.9 Lighting Channels

| Parameter | Description | Default |
|------|------|--------|
| **Channel 0** | Whether to enable lighting channel 0 | On |
| **Channel 1** | Whether to enable lighting channel 1 | Off |
| **Channel 2** | Whether to enable lighting channel 2 | Off |

> **Purpose**: Lighting channels enable selective illumination — "certain lights only illuminate certain objects."

### 5.10 Lighting Transmission

| Parameter | Description | Default |
|------|------|--------|
| **Affect Translucent Materials** | Controls whether SpotLight illuminates translucent objects | On |

### 5.11 Advanced Parameters

| Parameter | Description | Range |
|------|------|------|
| **SpecularScale** | Specular highlight scaling, controls highlight intensity on glossy surfaces | 0.0 ~ 1.0 |
| **SourceRadius** | Light source radius, affects shadow softness; larger values produce softer shadows | ≥ 0.0 cm |

---

## 6. Light Function

When `bDisableLightFunction` is false (default), SpotLight configures a light function for projecting Gobo patterns:

| Parameter | Description | Value |
|------|------|-----|
| **Light Function Scale** | Controls light function resolution | 64 |
| **Light Function Fade Distance** | Maximum effective distance of the light function | 100000 cm |
| **Disabled Brightness** | Brightness fallback when light function is unavailable | 0.5 |
| **Shadow Resolution Scale** | Shadow map resolution multiplier | 0.5 |
| **Shadow Bias** | Offset to prevent shadow self-intersection | 0.1 |
| **Use Inverse Squared Falloff** | Whether to use physically accurate inverse square falloff | Off |

> **About Light Function Atlas**: SuperStage uses the engine's Light Function Atlas (size 32768), which at this resolution can support approximately 1000+ fixtures simultaneously using light functions while maintaining clear Gobo patterns.

---

## 7. FAQ

### Q: The light cone angle is non-linear when zooming; is this normal?
**A**: Yes, this is normal. Real fixture zoom mechanisms are also non-linear. SuperSpotComponent uses measured calibration data for piecewise linear interpolation, simulating real fixture zoom behavior.

### Q: Why does the beam widen after setting Frost?
**A**: This is correct behavior. A real frost filter scatters light rays, simultaneously widening and softening the beam. SuperSpotComponent simulates this physical effect by increasing the outer cone angle and decreasing the inner cone angle ratio.

### Q: What's the difference between Iris and Zoom?
**A**: 
- **Zoom** changes the fixture's optical focal length, affecting the beam's base angle range
- **Iris** is a mechanical aperture that can only **shrink** the beam from the current Zoom setting, not expand it

### Q: When should I enable bDisableLightFunction?
**A**: When the fixture doesn't need Gobo pattern projection (such as simple Wash lights or fill lights). Enabling this option can slightly improve performance. When enabled, strobe directly controls light source intensity via Tick.

---

## 8. API Quick Reference

The following are public function signatures of `USuperSpotComponent` (including inherited base class overrides):

| Function Signature | Description |
|----------|------|
| `virtual void SetLightingMaterial() override` | Create dynamic materials and bind as SpotLight light function |
| `virtual void SetLightingDefaultValue() override` | Push default parameters to SpotLight |
| `virtual void SetLightingIntensity(const float NewLightIntensity = 1.0f) override` | Simultaneously update SpotLight intensity and material brightness |
| `virtual void SetLightingColor(const FLinearColor NewColor = FLinearColor(1,1,1)) override` | Simultaneously set SpotLight color and material color |
| `virtual void SetLightingZoom(const float NewZoom = 0.0f) override` | Zoom mapping (DMX → piecewise interpolation → SpotLight cone angle) |
| `virtual void UpdateBeamBlockDistance(const float NewMaxLightDistance) override` | Update beam occlusion distance |
| `virtual void SetLightingFrost(const float NewFrost = 0.0f) override` | Frost (widen cone + decrease inner cone ratio) |
| `virtual void SetLightingIris(const float NewIris = 1.0f) override` | Iris (shrink cone angle) |
| `virtual void SetLightingRotate(const float NewRotate = 0, const float NewInfiniteRotation = 0) override` | Pattern rotation |
| `virtual void SetColorTexture(UTexture2D*, int32, float, float) override` | Color wheel 1 |
| `virtual void SetColorTexture2(UTexture2D*, int32, float, float) override` | Color wheel 2 |
| `virtual void SetColorTexture3(UTexture2D*, int32, float, float) override` | Color wheel 3 |
| `virtual void SetLightingVisibility(const bool bNewVisibility = false) override` | SpotLight visibility |
| `virtual void InitializeLightFunction()` | Initialize light function, shadow, channel configuration |
