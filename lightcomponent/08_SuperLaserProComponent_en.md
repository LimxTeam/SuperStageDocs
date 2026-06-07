# 08 - Laser Pro Component (SuperLaserProComponent)

> **Module**: SuperStage Runtime  
> **Target Audience**: Lighting designers, laser operators, fixture Blueprint creators  
> **Prerequisite Reading**: [00 - Light Component Overview](00_LightComponent_Overview.md)

---

## 1. Overview

**SuperLaserProComponent** is a dedicated component in SuperStage for rendering **laser line effects**. It uses procedural mesh technology to generate visible laser line geometry in real-time based on **point data** provided by external laser controllers (such as Pangolin Beyond).

Unlike traditional particle systems or Beam components, SuperLaserProComponent's laser lines are generated based on **precise mathematical descriptions** — each laser line is defined by a start and end point, supports rendering multiple lines simultaneously, and can perform collision occlusion detection to automatically truncate when hitting obstacles.

### Use Cases

- **Laser Lights** — Laser fixtures driven by Beyond and other laser controllers
- **Laser Shows** — Spatial patterns composed of multiple colored laser lines
- **Laser Safety Lines** — Simulating laser fences or safety boundaries

---

## 2. Component Structure

```
SuperLaserProComponent (inherits from USceneComponent)
  └── ProceduralMeshComp  — Procedural mesh component (generates laser line geometry in real-time)
```

### Rendering Method

Laser lines are rendered through the following pipeline:

```
Point Data Input → Change Detection → Collision Detection (optional) → Geometry Generation → Material Parameter Update → Rendering
```

Each laser line is constructed as a **quadrilateral strip** (a rectangular plane composed of two triangles), oriented toward the camera to ensure it appears as a line from any viewing angle.

---

## 3. Default Parameters

### 3.1 Laser Line Appearance Parameters

| Parameter | Description | Range | Default | Unit |
|------|------|------|--------|------|
| **BeamLength** | Maximum laser line length (default length when no collision occlusion) | 100 ~ 50000 | 5000.0 | cm |
| **ProjectionAngle** | Projection angle range, controls the cone angle mapped from point data X/Y [-1,1] | 1 ~ 90 | 30.0 | degrees |
| **LaserWidth** | Laser line width (thickness) | 0.1 ~ 20 | 1.0 | cm |
| **CoreSharpness** | Sharpness of the laser line core; higher values = more concentrated core | 1 ~ 10 | 2.0 | — |
| **DepthFade** | Depth fade, controls how much the laser line dims at the far end. 0=no fade, 1=full fade | 0 ~ 1 | 0.5 | — |

#### Parameter Descriptions

- **BeamLength**: When no collision occlusion, the laser line extends from its starting point to this distance. If collision detection is enabled and an obstacle is hit, the actual length will be shorter.

- **LaserWidth**: Controls the visual thickness of the laser line. Real lasers are very thin (~1mm), but in virtual scenes they're typically set wider for visibility.

- **CoreSharpness**: Controls the concentration of the laser line core (brightest part). Higher values make the core finer and sharper; lower values make it softer and more diffuse.

- **DepthFade**: When a laser line passes through or touches an object, this parameter controls the transition zone length. Larger values make the transition softer; smaller values make it harsher.

### 3.2 Brightness and Opacity Parameters

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **Dim** | Emissive intensity, exponential curve brightness (1=normal, 3=brighter, 10=brightest) | 1.0 ~ 10.0 | 10.0 |
| **OpacityScale** | Opacity scaling of laser lines; 0=fully transparent, 1=fully opaque | 0.0 ~ 1.0 | 0.1 |
| **SpotDimmer** | Laser spot brightness, the spot effect when hitting a surface | 0.0 ~ 5.0 | 1.0 |
| **ComponentDimmer** | Component-level brightness sub-control; FinalDim = Dim × ComponentDimmer | 0.0 ~ 1.0 | 1.0 |

### 3.3 Fog Effect Parameters

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **FogSpeed** | Speed of fog texture flowing along the laser line; 0=static, 2=fast flow | 0 ~ 2 | 0.3 |
| **FogInfluence** | Degree of fog texture influence on the laser line (0=no fog, 1=fully fog-controlled) | 0 ~ 1 | 0.5 |

### 3.4 Collision Detection

| Parameter | Description | Type | Default |
|------|------|------|--------|
| **bEnableCollision** | Whether to enable collision occlusion detection | Boolean | false |

#### Collision Detection Working Principle

When `bEnableCollision` is enabled:

1. Cast a ray from the start point of each laser line toward its end point
2. Detect collision intersections with objects in the scene
3. If an obstacle is hit, truncate the laser line at the collision point
4. Ignore collisions with the fixture's own Actor

```
Collision detection enabled:
 Fixture ━━━━━━━━━●━━━━× Wall
              Collision point (laser truncated here)

Collision detection disabled:
 Fixture ━━━━━━━━━━━━━━━━━━━━━━━●
                            (extends to BeamLength)
```

### 3.5 Laser Material

| Parameter | Description | Type |
|------|------|------|
| **BeamMaterial** | Material used for laser lines | Material Reference |

> Default uses the laser material provided by SuperStage (based on SuperLaserPro.usf custom shader), supporting core sharpness, depth fade, fog flow, and other effects.

---

## 4. DMX-Controlled Parameters

### 4.1 Laser Point Data

The core input of the laser component is a **point data array**. Each point contains:

| Property | Description |
|------|------|
| **Position** | Spatial position of the point |
| **Color** | RGB color of the point |

Point data is typically sent by external laser controllers (such as Pangolin Beyond) via network protocols. SuperStage's laser system receives and passes the data to the laser component.

#### Change Detection

The laser component includes built-in **change detection**:

- Compares the count and positions of new vs old point data
- Only rebuilds the mesh when point data **actually changes**
- Avoids unnecessary geometry rebuilds when point data is unchanged

### 4.2 Visibility Control

| Operation | Description |
|------|------|
| **Set Laser Visibility** | Controls show/hide of the entire laser component |

### 4.3 Component-Level Brightness Sub-Control

| Operation | Description | Parameter Range |
|------|------|----------|
| **Set ComponentDimmer** | Controls the overall brightness of this laser component | 0.0 ~ 1.0 |

---

## 5. Geometry Generation

### 5.1 Mesh Construction Process

After receiving point data, the laser component executes the following steps to generate visible laser lines:

1. **Parse point data** — Extract start point, end point and color for each laser line
2. **Collision detection** (if enabled) — Ray trace each line, truncate occluded segments
3. **Generate vertices** — 4 vertices per laser line (2 per end, forming a rectangular strip)
4. **Calculate normals** — Normals face the camera direction to ensure the strip always faces the viewer
5. **Generate UVs** — For material texture mapping
6. **Set vertex colors** — Each vertex carries the laser line's color information
7. **Create mesh segments** — Render using the procedural mesh component

### 5.2 Multiple Laser Lines

A single laser component can simultaneously render **multiple** laser lines. Each line independently calculates collision and color. All lines are merged into one procedural mesh for rendering, which is relatively efficient.

---

## 6. Material Parameters

The following parameters are pushed to the laser material at runtime:

| Material Parameter | Corresponding Component Parameter | Description |
|----------|---------------|------|
| **CoreSharpness** | CoreSharpness | Core sharpness |
| **DepthFade** | DepthFade | Depth fade distance |
| **Dim** | Dim | Laser line brightness |
| **OpacityScale** | OpacityScale | Opacity scaling |
| **FogSpeed** | FogSpeed | Fog flow speed |
| **FogInfluence** | FogInfluence | Fog influence degree |
| **SpotDimmer** | SpotDimmer | Spot brightness |

---

## 7. Usage Guide

### 7.1 Basic Setup Workflow

1. Add SuperLaserProComponent in the fixture Blueprint
2. Configure **laser line appearance** parameters (width, sharpness, depth fade, etc.)
3. Configure **laser line length** (BeamLength)
4. Decide whether to enable **collision detection** (bEnableCollision)
5. Configure **fog effects** (FogSpeed, FogInfluence)
6. Connect laser controller data source (e.g., Beyond)

### 7.2 Visual Effect Adjustment

#### Sharp vs Soft

| Effect | CoreSharpness | LaserWidth | Description |
|------|---------------|------------|------|
| Sharp laser | High (10+) | Small (0.3) | Fine and sharp, close to real laser |
| Soft laser | Low (1~3) | Large (1.0+) | Wide and soft, more visual impact |

#### Fog Density

| Effect | FogInfluence | FogSpeed | Description |
|------|-------------|----------|------|
| No fog | 0.0 | — | Clean laser line |
| Light fog | 0.3 | 0.5 | Subtle fog flow |
| Heavy fog | 0.8+ | 1.0+ | Visible fog scattering |

#### Brightness Levels

The laser component has multiple brightness control layers:

```
Final Brightness = Dim × ComponentDimmer
Final Spot = SpotDimmer × ComponentDimmer
Final Opacity = OpacityScale
```

### 7.3 Collision Detection Configuration

#### When to Enable Collision Detection

| Scenario | Recommendation |
|------|------|
| Laser hitting ground/walls | Enable — laser truncates at contact surface, more realistic |
| Laser sweeping in air | Disable — no collision needed, save performance |
| Many laser lines | Disable — avoid overhead of many ray traces |
| Interactive laser | Enable — laser interacts with moving objects |

---

## 8. FAQ

### Q: Laser lines not displaying?
**A**: Check: 1) Valid **point data** input (at least 2 points to define a line) 2) **BeamLength** is large enough 3) **Visibility** is enabled 4) **ComponentDimmer** and **Dim** > 0 5) **BeamMaterial** is valid

### Q: Laser lines noticeably flicker/jitter when moving?
**A**: The laser component has built-in change detection; mesh is only rebuilt when point data changes. If flicker is material-caused, try adjusting FogSpeed or DepthFade.

### Q: Collision detection inaccurate?
**A**: Collision detection uses standard linear ray tracing (Line Trace). Ensure target objects have correct collision body setup. Using Complex Collision provides more accurate results.

### Q: How to control individual laser line color?
**A**: Each laser line's color is determined by the color property in the **point data**. Set each line's color in the laser controller (e.g., Beyond). Color info is passed to the material via vertex colors.

### Q: Does the laser component produce real lighting?
**A**: No. Laser lines are rendered via emissive materials and do not produce real lighting. Additional logic is needed (e.g., placing light sources or using decals at collision points) for spot effects on surfaces.

### Q: Performance with many laser lines?
**A**: All laser lines are merged into one procedural mesh for rendering at 1 Draw Call. Geometry generation is CPU-side. Main bottlenecks: collision detection (1 ray trace per line when enabled), mesh rebuild (vertex/triangle/normal calculation when point data changes), material rendering (semi-transparent material GPU overhead).

---

## 9. API Quick Reference

The following are all public function signatures of `USuperLaserProComponent`:

| Function Signature | Description |
|----------|------|
| `void SetLaserPoints(const TArray<FLaserPoint>& InPoints)` | Set laser point data (triggers change detection and mesh rebuild) |
| `void RebuildMesh()` | Force rebuild procedural mesh |
| `void SetLaserVisibility(bool bNewVisibility)` | Laser visibility |
| `void SetComponentDimmer(const float NewDimmer)` | Component-level brightness sub-control (0.0 ~ 1.0, updates MaxLightIntensity and refreshes material parameters) |
