# Light Component Overview

> **Module**: SuperStage Runtime  
> **Target Audience**: Lighting designers, stage technicians, Blueprint developers  
> **Prerequisite Reading**: [15 - Full-Feature Moving Light (SuperStageLight)](../15_SuperStageLight.md)

---

## 1. Overview

SuperStage's light component system is the core module that actually handles "light rendering" inside fixture Actors (such as SuperStageLight). Each fixture Actor can combine multiple light components to simulate the various light output methods of real stage fixtures — spotlight projection, volumetric beams, framing cut shaping, rectangular area lighting, effect patterns, matrix pixels, laser projection, etc.

Lighting designers do not need to directly operate these components in daily work — they are pre-configured in fixture Blueprints and automatically driven by DMX channels. However, understanding the parameter meanings and working methods of these components helps with:

- **Correctly selecting and configuring components** when creating custom fixture Blueprints
- **Quickly locating issues** when debugging lighting effects
- **Understanding the overhead characteristics** of each component when optimizing scene performance

---

## 2. Component Inheritance

SuperStage's light components form a clear inheritance system:

```
USceneComponent (Engine base class)
  │
  ├── USuperLightingComponent ← Base light component (lens + aperture material)
  │     │
  │     ├── USuperSpotComponent ← Spotlight component (SpotLight + zoom/frost/iris)
  │     │     │
  │     │     └── USuperBeamComponent ← Volumetric beam component (beam mesh + atmospheric scattering)
  │     │           │
  │     │           └── USuperCuttingComponent ← Cutting beam component (4-leaf shutter cropping)
  │     │
  │     └── USuperRectComponent ← Rectangular area light component (RectLight + barn doors)
  │
  ├── USuperEffectComponent ← Effect plane component (LED panel/effect panel material-driven)
  │
  ├── USuperMatrixComponent ← Matrix light component (segmented pixel control + segmented light sources)
  │
  ├── USuperLaserProComponent ← Laser Pro component (point data-driven laser lines)
  │
  └── USuperLiftComponent ← Lift control component (Z-axis displacement)
```

---

## 3. Component Quick Reference

| Component | Purpose | Typical Fixtures | Documentation Link |
|------|------|----------|----------|
| **SuperLightingComponent** | Base light rendering (lens + aperture) | Light base for all fixtures | [01_SuperLightingComponent.md](01_SuperLightingComponent.md) |
| **SuperSpotComponent** | Spotlight projection (real SpotLight) | Spot lights, follow spots | [02_SuperSpotComponent.md](02_SuperSpotComponent.md) |
| **SuperBeamComponent** | Volumetric beam visualization | Beam lights | [03_SuperBeamComponent.md](03_SuperBeamComponent.md) |
| **SuperCuttingComponent** | Shutter cut shaping | Profile lights, imaging lights | [04_SuperCuttingComponent.md](04_SuperCuttingComponent.md) |
| **SuperRectComponent** | Rectangular area lighting | LED panel lights, soft lights | [05_SuperRectComponent.md](05_SuperRectComponent.md) |
| **SuperEffectComponent** | Effect panel/LED screen | LED effect lights, strobe lights | [06_SuperEffectComponent.md](06_SuperEffectComponent.md) |
| **SuperMatrixComponent** | Matrix pixel segmented control | Matrix lights, LED strip lights | [07_SuperMatrixComponent.md](07_SuperMatrixComponent.md) |
| **SuperLaserProComponent** | Laser line projection | Laser lights (Beyond-driven) | [08_SuperLaserProComponent.md](08_SuperLaserProComponent.md) |
| **SuperLiftComponent** | Z-axis lift motion | Fixtures with lift functionality | [09_SuperLiftComponent.md](09_SuperLiftComponent.md) |

---

## 4. Component Configuration for Typical Fixtures

Different types of fixtures use different component combinations. Below are reference configurations for common fixtures:

### 4.1 Spot Light (aperture projection only, no visible beam)

```
SuperStageLight
  └── Head
        └── SuperSpotComponent (main light source)
```

- Uses SpotLight to produce real lighting
- Projects Gobo patterns via light function material
- Suitable for scenes that don't need visible beam effects

### 4.2 Beam Light (volumetric beam + aperture projection)

```
SuperStageLight
  └── Head
        └── SuperBeamComponent (main light source)
              ├── SpotLight — Projection lighting
              ├── Lens mesh — Lens glow effect
              └── Beam mesh — Visible volumetric beam
```

- Adds visible volumetric beam mesh on top of Spot
- Beam affected by Zoom, Frost, Color, Gobo and other parameters
- Supports atmospheric scattering, fog flow effects

### 4.3 Profile/Cutting Light (shutter cropping + volumetric beam)

```
SuperStageLight
  └── Head
        └── SuperCuttingComponent (main light source)
              ├── SpotLight — Projection lighting
              ├── Lens mesh — Lens glow + cutting pattern
              └── Beam mesh — Visible volumetric beam + cutting shape
```

- Adds four cutting blade controls on top of Beam
- Can crop the aperture to any quadrilateral
- Supports cutting angle rotation

### 4.4 Wash Light / Panel Light (rectangular area light)

```
SuperStageLight
  └── Head
        └── SuperRectComponent (main light source)
              ├── RectLight — Rectangular area illumination
              └── Lens mesh — Lens glow effect
```

- Uses RectLight to produce soft area illumination
- Supports BarnDoor for beam shape control
- Suitable for Wash-type fixtures and LED panel lights

### 4.5 Effect Light (LED effect panel)

```
SuperStageLight
  └── Head
        ├── SuperBeamComponent (main light source, optional)
        └── SuperEffectComponent (effect panel)
```

- Uses plane mesh + material to achieve LED effect display
- Supports independent brightness, strobe, and color control
- Supports effect pattern animation (speed, width, direction)

### 4.6 Matrix Light (pixel-level control)

```
SuperStageLight
  └── Head
        ├── SuperBeamComponent (main light source, optional)
        └── SuperMatrixComponent (matrix pixels)
              └── Segmented light sources[] (PointLight or SpotLight)
```

- Divides the fixture panel into multiple independent pixel segments
- Each segment can independently control color
- Optional real light sources (PointLight/SpotLight) mapped to pixels

### 4.7 Laser Light (Beyond-driven)

```
SuperStageLight / LaserActor
  └── SuperLaserProComponent
        └── ProceduralMesh — Laser line geometry
```

- Point data provided by Beyond laser controller
- Real-time generation of laser line procedural meshes
- Supports collision occlusion detection

---

## 5. General Features

The following features are common across multiple light components and are explained here uniformly.

### 5.1 Dimmer Control

The final brightness of all light components is determined by the following factors:

```
Final Brightness = Actor-level Dimmer × (MaxLightIntensity / 100) × ComponentDimmer × Strobe Multiplier
```

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **Actor-level Dimmer** | Overall fixture brightness, controlled by DMX Dimmer channel | 0.0 ~ 1.0 | 1.0 |
| **MaxLightIntensity** | Component maximum brightness percentage (design-time config) | ≥ 1 | 100.0 |
| **ComponentDimmer** | Component-level runtime brightness sub-control (set by `SetComponentDimmer`) | 0.0 ~ 1.0 | 1.0 |

> **Use Case**: When a fixture Actor has both a main and auxiliary light source, you can set the auxiliary source's maximum brightness percentage via MaxLightIntensity, or independently reduce the auxiliary source's brightness at runtime via ComponentDimmer.

> **Initialization Note**: Component default parameters are automatically initialized by `OnRegister()`. During component registration, `SetLightingMaterial()` is automatically called to create material instances, then `SetLightingDefaultValue()` sets the defaults. No need to manually call initialization functions in the Actor.

### 5.2 Dimmer Curve

Light components support a "dimmer response curve" feature for converting linear DMX values to perceptually more uniform brightness output.

| Exponent | Curve Name | Description |
|--------|----------|------|
| **1.0** | Linear | DMX value directly maps to brightness (not recommended, rapid changes in low range, flat in high range) |
| **2.0** | Square curve | **Recommended**, simulates real console perception curve, more natural dimming transitions |
| **2.2** | Gamma correction | Monitor standard correction curve |
| **3.0** | Cubic curve | Stronger non-linear response, gentler in low range |

> **Tip**: Default is 2.0 (square curve), suitable for most stage fixtures. If low-brightness changes feel too fast, increase the exponent appropriately.

### 5.3 Strobe System

All light components share the same strobe calculation logic, supporting 8 strobe modes:

| Mode | Name | Effect Description |
|----------|------|----------|
| **0** | Closed | Light completely off (brightness = 0) |
| **1** | Open | Light always on, no strobe (default) |
| **2** | Linear | Triangle wave, smooth fade in/out with transitions |
| **3** | Pulse | Square wave flash, 50% on / 50% off, hard switch |
| **4** | Ramp Up | Sawtooth wave, linearly ramps up from dark to bright then instantly drops |
| **5** | Ramp Down | Sawtooth wave, linearly drops from bright to dark then instantly brightens |
| **6** | Sine | Sine wave smooth pulsation, the softest strobe effect |
| **7** | Random | Random flash, each time slot randomly decides on or off |

Strobe speed is controlled by the DMX Strobe channel; higher values mean faster flashing.

> **Performance Tip**: Strobe modes 0 (Closed) and 1 (Open) don't need real-time calculation; the component automatically disables Tick to save performance. Only modes 2~7 enable per-frame updates.

### 5.4 Collision Detection (Ray Detection)

Light-emitting components (Spot, Beam, Cutting, Rect) support ray collision detection for automatically truncating light distance:

- Casts a ray from the fixture lens position in the light emission direction
- Detects intersection with objects in the scene
- Limits light distance to the collision point (light won't penetrate walls/floors)
- Uses a two-level detection strategy: coarse ray first (sphere sweep), then fine ray (fallback)

### 5.5 Lens Model

The base light component and its subclasses all support lens model display:

| Parameter | Description |
|------|------|
| **StaticMeshLens** | Static mesh model used for the lens |
| **LensTransform** | Position/rotation/scale offset of the lens relative to the component |

The lens model updates its material appearance in real-time as light brightness, color, pattern and other parameters change, simulating the glow effect of real fixture lenses.

---

## 6. Relationship with Fixture Actors

Light components **cannot exist independently**; they must be used as sub-components of fixture Actors (such as SuperStageLight). The fixture Actor is responsible for:

1. **Receiving DMX data** — Getting channel values from the DMX subsystem
2. **Parsing fixture library definitions** — Mapping channel values to specific parameters
3. **Driving light components** — Calling component parameter interface functions to update visual effects
4. **Managing motion control** — Pan/Tilt rotation affects the light emission direction of components

Lighting designers typically only need to:
- **Add and configure** required light components in fixture Blueprints
- **Define DMX channel mappings** in the fixture library
- **Send DMX signals** from the console or Sequencer to control lighting effects

---

## 7. Performance Optimization Tips

| Recommendation | Description |
|------|------|
| **Choose component types wisely** | Use SuperSpotComponent instead of SuperBeamComponent when volumetric beams aren't needed |
| **Control matrix segment count** | SuperMatrixComponent segment count should not exceed necessary count (max 200) |
| **Disable unnecessary collision detection** | Laser component collision detection has some overhead; disable when not needed |
| **Leverage MaxLightIntensity** | Set unused component brightness sub-control to 0; the system automatically optimizes |
| **Watch strobe modes** | Strobe modes 0/1 don't consume Tick resources; modes 2~7 enable per-frame calculation |
| **Enable transparent materials as needed** | Effect and Matrix component transparent materials have slightly higher overhead than opaque |

---

## 8. Component Detailed Documentation

Click the links below to view detailed parameter descriptions and usage guides for each component:

1. [SuperLightingComponent — Base Light Component](01_SuperLightingComponent.md)
2. [SuperSpotComponent — Spotlight Component](02_SuperSpotComponent.md)
3. [SuperBeamComponent — Volumetric Beam Component](03_SuperBeamComponent.md)
4. [SuperCuttingComponent — Cutting Beam Component](04_SuperCuttingComponent.md)
5. [SuperRectComponent — Rectangular Area Light Component](05_SuperRectComponent.md)
6. [SuperEffectComponent — Effect Plane Component](06_SuperEffectComponent.md)
7. [SuperMatrixComponent — Matrix Light Component](07_SuperMatrixComponent.md)
8. [SuperLaserProComponent — Laser Pro Component](08_SuperLaserProComponent.md)
9. [SuperLiftComponent — Lift Control Component](09_SuperLiftComponent.md)
