# 07 - Matrix Light Component (SuperMatrixComponent)

> **Module**: SuperStage Runtime  
> **Target Audience**: Lighting designers, fixture Blueprint creators  
> **Prerequisite Reading**: [00 - Light Component Overview](00_LightComponent_Overview.md)

---

## 1. Overview

**SuperMatrixComponent** is a light source component in SuperStage for implementing **matrix pixel-level control**. It divides a fixture panel into multiple independent "Segments", each with individually controllable color, enabling effects such as pixel arrays, LED strip lights, and matrix lights.

Beyond material-level segmented coloring, SuperMatrixComponent also supports creating **real light sources** (PointLight or SpotLight) for each segment, allowing each pixel to not only visually glow but also truly illuminate the surrounding environment.

### Use Cases

- **Matrix Lights** — Fixtures with matrix control, such as Robe Robin 600 LEDWash
- **LED Strip Lights** — Addressable LED strips, each pixel independently controlled
- **Pixel Panels** — LED pixel screens or decorative panels
- **Multi-Segment Wash Lights** — Wash fixtures with multi-zone independent color control

---

## 2. Component Structure

```
SuperMatrixComponent (inherits from USceneComponent)
  ├── YStaticMeshMatrix  — Matrix panel model (static mesh, carrying segmented material)
  └── Segment Light Sources[] — Real light sources for each segment (optional)
        ├── SegmentPointLights[]  — PointLight array (spherical emission)
        └── SegmentSpotLights[]   — SpotLight array (directional emission)
```

### Material Systems

| Material | Description |
|------|------|
| **Opaque Material (MatrixMaterial)** | Standard matrix segmented material, opaque rendering |
| **Transparent Material (MatrixMaterialTransparent)** | Semi-transparent matrix segmented material |

Switch between the two modes via the `bTransparent` toggle. Pixel-level coloring is achieved within the material through segmentation parameters (SegCount, color arrays).

---

## 3. Default Parameters

### 3.1 Segment Configuration

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **SegCount** | Number of segments (pixels) | 1 ~ 200 | 5 |
| **bUseUAxis** | Segment direction axis | Boolean | true |

#### Segment Direction

| bUseUAxis | Segment Direction | Description |
|-----------|----------|------|
| **true** (default) | Along U axis (X axis) | Pixels arranged left to right |
| **false** | Along V axis (Y axis) | Pixels arranged top to bottom |

```
bUseUAxis = true (segmented along X axis):
┌───┬───┬───┬───┬───┐
│ 0 │ 1 │ 2 │ 3 │ 4 │  ← 5 segments, left to right
└───┴───┴───┴───┴───┘

bUseUAxis = false (segmented along Y axis):
┌─────────┐
│    0    │
├─────────┤
│    1    │
├─────────┤  ← 5 segments, top to bottom
│    2    │
├─────────┤
│    3    │
├─────────┤
│    4    │
└─────────┘
```

### 3.2 Panel Model

| Parameter | Description | Type | Default |
|------|------|------|--------|
| **StaticMeshMatrix** | Static mesh model used for the matrix panel | Static Mesh Reference | None |
| **MatrixTransform** | Position, rotation and scale offset of the panel relative to the component | Transform | No offset |

### 3.3 Material Configuration

| Parameter | Description | Type | Default |
|------|------|------|--------|
| **bTransparent** | Whether to use transparent material | Boolean | false |

### 3.4 Component-Level Brightness Sub-Control

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **MaxLightIntensity** | Component maximum brightness percentage (design-time config, 100 = full) | ≥ 1 | 100.0 |

> **Auto-Initialization**: Component default parameters are automatically initialized by `OnRegister()`. During component registration, `SetMatrixMaterial()` is automatically called to create material instances and set defaults. No need to manually call in the Actor.

---

## 4. Segment Light Source System

### 4.1 Light Source Type Selection

| Parameter | Description | Type |
|------|------|------|
| **SegmentLightType** | Type of real light source created for each segment | Enum |

| Enum Value | Description | Use Cases |
|--------|------|----------|
| **None** | No real light sources created, material emission only | Pure visual effects, best performance |
| **PointLight** | One point light created per segment | Matrix lights needing omnidirectional illumination |
| **SpotLight** | One spotlight created per segment | Matrix lights needing directional illumination |

> **Performance Impact**: Each segment creates one independent light source component. 200 segments = 200 light sources, which can severely impact performance. Choose the number of light sources based on actual needs.

### 4.2 Light Source Parameters

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **SegmentLightIntensity** | Base brightness of segment light sources | ≥ 0 | 1000.0 |
| **SegmentLightRadius** | Attenuation radius of segment light sources | ≥ 1 | 2400.0 cm |
| **SpotOuterConeAngle** | Outer cone angle of segment SpotLights (only effective for SpotLight type) | 1 ~ 90 | 90.0 degrees |

#### Segment Light Source Brightness Calculation

The final brightness of each segment light source is determined by:

```
Final Brightness = SegmentLightIntensity × Color Luminance × Color Alpha × Overall Brightness × ComponentDimmer × Strobe Multiplier
```

Where:
- **Color Luminance** = Perceived brightness of pixel color (RGB weighted average)
- **Color Alpha** = Color alpha channel (0 = not enabled, 1 = enabled)

### 4.3 Light Source Position

Segment light source positions are automatically calculated:

1. Get bounding box of the matrix panel mesh
2. Calculate each segment's center position based on segment direction (U or V axis)
3. Transform positions to component local coordinate system

```
Segment Center Position = Mesh Bounding Box Min + Bounding Box Size × ((Segment Index + 0.5) / Total Segments)
```

### 4.4 Light Source Properties

All segment light sources share the following properties:

| Property | Value | Description |
|------|-----|------|
| Mobility | Movable | Supports real-time changes |
| Cast Shadows | Off | Avoids shadow overhead from many light sources |
| Initial Brightness | 0 | Driven by DMX data |
| Initial Color | Black | Driven by DMX data |

> **SpotLight Special Settings**:  
> - Inner cone angle fixed at 1° (hard core)  
> - Outer cone angle controlled by SpotOuterConeAngle  
> - Shines downward (rotated 90° downward)

---

## 5. DMX-Controlled Parameters

### 5.1 Segment Color

| Operation | Description |
|------|------|
| **Set Segment Color** | Set RGB color for a segment at the specified index |

| Parameter | Description | Range |
|------|------|------|
| **Index** | Segment index (0-based) | 0 ~ (SegCount - 1) |
| **RGB** | Segment color | RGB linear color |

Setting segment color automatically:
1. Sets color Alpha to 1.0 (marks segment as enabled)
2. Updates material parameters (segmented coloring)
3. Synchronously updates corresponding segment light source color and brightness

### 5.2 Overall Brightness (Intensity)

| Operation | Description | Parameter Range |
|------|------|----------|
| **Set Brightness** | Controls overall brightness of all segments and light sources | 0.0 (off) ~ 1.0 (full) |

### 5.3 Strobe

The matrix component has an **independent strobe system**:

| Parameter | Description | Range |
|------|------|------|
| **Strobe Speed** | Flashing speed | ≥ 0 |
| **Strobe Mode** | Flashing waveform (0~7) | Same as base class |
| **Random Seed** | Seed value for random mode | Any |

Strobe modes: Closed (0), Open (1), Linear (2), Pulse (3), Ramp Up (4), Ramp Down (5), Sine (6), Random (7).

> **Note**: Strobe **simultaneously affects** material brightness and segment light source brightness, ensuring consistent visual effects.

### 5.4 Component-Level Brightness Sub-Control

| Operation | Description | Parameter Range |
|------|------|----------|
| **Set ComponentDimmer** | Independently controls this matrix component's brightness | 0.0 ~ 1.0 |

---

## 6. Usage Guide

### 6.1 Basic Setup Workflow

1. Add SuperMatrixComponent in the fixture Blueprint
2. Set **static mesh model** (StaticMeshMatrix) — typically a plane
3. Set **panel position and size** (MatrixTransform)
4. Configure **segment count** (SegCount) — matches the fixture's actual pixel count
5. Choose **segment direction** (bUseUAxis) — matches the fixture's pixel arrangement direction
6. Select **segment light source type** (SegmentLightType)
7. Adjust **light source parameters** (brightness, radius, cone angle, etc.)
8. Map DMX channels in the fixture library

### 6.2 Segment Count vs Performance Trade-off

| Segment Count | Material Overhead | Light Source Overhead (when enabled) | Use Cases |
|--------|----------|-------------------|----------|
| 1~10 | Very low | Low | Wash lights with few zones |
| 10~50 | Low | Medium | Standard matrix lights |
| 50~100 | Medium | Higher | High-density pixel lights |
| 100~200 | Higher | High (use with caution) | Ultra-high-density LED strips |

> **Recommendation**: Set SegmentLightType to None if segment light sources don't need to illuminate the environment, for significant performance savings.

### 6.3 Initialization Behavior

The matrix component performs the following during initialization:

1. Creates static mesh component and sets model
2. Creates dynamic material instances (selecting material based on transparent mode)
3. Pushes core parameters (segment count, segment direction) to material
4. Initializes all segment colors to **black** (prevents stale state)
5. Creates segment light sources (if SegmentLightType is not None)
6. All light sources initially set brightness to 0 (driven by DMX data)

### 6.4 Light Source Rebuild

The following conditions trigger segment light source **rebuild** (destroy old + create new):

- Modify SegmentLightType (light source type change)
- Modify SegCount (segment count change)
- Component re-registration

---

## 7. FAQ

### Q: Segment colors don't change after setting?
**A**: Check: 1) SegCount is correctly set 2) Material is correctly created 3) Overall brightness (Intensity) > 0 4) ComponentDimmer > 0

### Q: Segment light source creation failed or count is wrong?
**A**: Segment light sources are created during: 1) Component OnRegister 2) SetMatrixMaterial initialization (only if not yet created). Confirm SegmentLightType is not None and SegCount is correct.

### Q: How to achieve a 2D matrix (e.g., 5×5 grid)?
**A**: SuperMatrixComponent currently only supports **1D segmentation** (along U or V axis). For 2D matrices, use multiple SuperMatrixComponents (one per row/column), or unfold 25 pixels into 1 row (SegCount=25) and achieve 2D layout via UV mapping in the material.

### Q: Can segment light source shadows be enabled?
**A**: Segment light sources default to shadow off for performance. Enabling shadows for many segments (200 light sources' shadow calculations severely impact frame rate) is not recommended.

### Q: Does modifying light source radius or cone angle require a rebuild?
**A**: No. After modifying SegmentLightRadius and SpotOuterConeAngle, call UpdateLightParameters to directly update all existing light source parameters without a rebuild.

---

## 8. API Quick Reference

The following are all public function signatures of `USuperMatrixComponent`:

| Function Signature | Description |
|----------|------|
| `void SetMatrixMaterial()` | Create dynamic material and initialize segments (auto-called by OnRegister) |
| `void SetSegmentColor(int32 Index = 0, FLinearColor RGB = FLinearColor::White)` | Set a single segment color |
| `void SetMatrixStrobe(float NewStrobe = 255.0f)` | Set strobe speed |
| `void SetMatrixStrobeMode(float NewStrobeMode = 1.0f)` | Set strobe mode (0 ~ 7) |
| `void SetMatrixIntensity(float NewIntensity)` | Set overall brightness |
| `void SetRandomSeed(float NewSeed)` | Set random seed |
| `void SetComponentDimmer(float NewDimmer)` | Component-level brightness sub-control (0.0 ~ 1.0) |
| `void RebuildSegmentLights()` | Rebuild segment light sources (call after modifying SegCount/SegmentLightType) |
| `void DestroyAllSegmentLights()` | Destroy all segment light sources |
| `void UpdateLightParameters()` | Update radius/cone angle, etc. for all segment light sources |
