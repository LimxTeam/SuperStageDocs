# Super Projector — User Manual

## 1. Overview

**Super Projector** is a projection mapping simulation tool provided by the SuperStage plugin. It inherits from the `ASuperMediaBase` media base class and supports two media source modes: **NDI real-time video stream projection** and **static texture projection**. It projects images onto scene surfaces using the **Light Function** technique of a single white spotlight, enabling pre-visualization of stage projection mapping effects.

### Inheritance

```
AActor → ASuperBaseActor → ASuperMediaBase → ASuperProjector
```

### How It Works

Super Projector simulates the operation of a real projector:

1. A single **white spotlight** (SpotLight) is set with a light function material
2. The light function material contains a texture, with NDI video frames or static textures selected based on SourceMode
3. The spotlight projects full-color images onto scene surfaces
4. **Keystone correction** (four-point perspective transform) is supported, simulating the corner adjustment of real projectors

### Use Cases

- Stage projection mapping effect preview
- Building facade projection scheme design
- Performance lighting projection texture pre-visualization
- Projection installation position and angle planning
- Keystone distortion correction preview

---

## 2. How to Add to Scene

1. Search for **"Super Projector"** in the UE editor's "Place Actor" panel
2. Drag it into the viewport scene
3. Select the Actor and configure the projection texture and various parameters in the "Details Panel"
4. Aim the projector at the target object surface

> **Direction Note**: Super Projector's default projection direction is the Actor's **negative X-axis direction** (opposite the Actor's front-facing direction). Adjust the Actor's rotation to aim at the projection target.

---

## 3. Parameter Reference

### 3.1 Media Source (Inherited from SuperMediaBase)

Super Projector inherits from `ASuperMediaBase` and supports two media source modes:

| Parameter | Description | Default |
|------|------|--------|
| **SourceMode** | Media source mode: `NDI` (real-time video stream) / `Texture` (static texture) | NDI |
| **NDIInputSelection** | NDI input name to subscribe to (NDI mode only) | None |
| **StaticTexture** | Manually specified static texture (Texture mode only) | None |

> **NDI Mode**: Receives real-time video frames from NDI senders (media servers, OBS, etc.) on the local network and projects them.
> **Texture Mode**: Uses static texture assets from the project as projection content, suitable for fixed-pattern projection.

### 3.2 Projection Parameters (Mapping)

#### Mapping Scale

- **Meaning**: Controls the **Light Function Scale** of the projected image; larger values mean a smaller projected image (units are render scale values, typically corresponding to resolution)
- **Type**: 2D vector (X, Y)
- **Range**: 128 ~ 4096
- **Default**: (1920, 1080)

| Parameter | Description |
|------|------|
| **X** | Horizontal scaling. Default 1920 (corresponds to 1080p width) |
| **Y** | Vertical scaling. Default 1080 (corresponds to 1080p height) |

> **How It Works**: Mapping Scale actually controls the spotlight's **Light Function Scale** property. The system also automatically calculates the diagonal length (√(X² + Y²)) as the Z-direction scale value to ensure correct projection aspect ratio.

> **Tip**: If the projected image is too large or too small, adjust the X and Y values of Mapping Scale. Keeping the X and Y ratio consistent with the texture's aspect ratio generally gives the best results.

---

### 3.2 Light Settings

#### 3.2.1 Dimmer

- **Meaning**: The projector's **light intensity**, directly setting the spotlight's Intensity value
- **Unit**: Candelas (cd)
- **Range**: 0 ~ unlimited
- **Default**: 100

> **Note**: The default value of 100 is relatively low brightness, suitable for indoor debugging. Increase as needed based on the scene.
>
> **Recommended Values**:
> - Small indoor projection: 5,000 ~ 20,000 cd
> - Stage projection: 50,000 ~ 200,000 cd
> - Large outdoor projection: 200,000+ cd

#### 3.2.2 Max Light Distance

- **Meaning**: The **farthest distance** projection light can reach. Surfaces beyond this distance will not receive projection
- **Unit**: centimeters (cm)
- **Range**: 100 ~ unlimited
- **Default**: 3000 cm (30m)

> **Note**: Corresponds to the spotlight's Attenuation Radius. Set this value slightly larger than the distance from the projector to the target surface. Setting it too large will add unnecessary lighting calculations.

#### 3.2.3 Zoom

- **Meaning**: The **outer angle** (half-angle) of the projection cone, controlling the spread angle of the projected image
- **Unit**: degrees (°)
- **Range**: 10 ~ 100°
- **Default**: 30°

> **How It Works**: Zoom corresponds to the spotlight's Outer Cone Angle. Larger angles cover a larger area at the same distance (but brightness decreases accordingly).
>
> **Real Projector Reference**:
> - Short-throw projector: approx. 50~70°
> - Standard projector: approx. 25~40°
> - Long-throw projector: approx. 10~20°

#### 3.2.4 Dilution Factor

- **Meaning**: Controls the **softness of edge-to-center transition** of the projected image
- **Range**: 0.0 ~ 1.0
- **Default**: 0.0

| Value | Effect |
|----|------|
| **0.0** | Sharp projection edge, no gradient (Inner Cone = Outer Cone) |
| **0.5** | Moderate soft transition at edges |
| **1.0** | Complete gradient from center to edge (Inner Cone = 0°) |

> **How It Works**: The system sets Inner Cone Angle = Lerp(Zoom, 0, DilutionFactor). DilutionFactor = 0 → inner and outer cone angles are the same (hard edge); DilutionFactor = 1 → inner cone angle is 0° (maximum softening).

---

### 3.3 Mapping Deformation (Keystone Correction)

Keystone correction is used to **compensate for image distortion** caused by angular deviation between the projector and the projection surface. By adjusting the positions of four corner points, the distorted trapezoidal image is corrected to a rectangle.

Each corner point has two adjustment axes (X and Y), for a total of **8 adjustable parameters**.

#### 3.3.1 Upper Left Corner

- **X component**: Controls the **vertical** offset of the top-left corner
  - 0.0 = corner at original position (topmost)
  - 1.0 = corner moved to image center height
- **Y component**: Controls the **horizontal** offset of the top-left corner
  - 0.0 = corner at original position (leftmost)
  - 1.0 = corner moved to image center width

#### 3.3.2 Lower Left Corner

- **X component**: Controls the vertical offset of the bottom-left corner (0=bottom, 1=center)
- **Y component**: Controls the horizontal offset of the bottom-left corner (0=leftmost, 1=center)

#### 3.3.3 Upper Right Corner

- **X component**: Controls the vertical offset of the top-right corner (0=top, 1=center)
- **Y component**: Controls the horizontal offset of the top-right corner (0=rightmost, 1=center)

#### 3.3.4 Lower Right Corner

- **X component**: Controls the vertical offset of the bottom-right corner (0=bottom, 1=center)
- **Y component**: Controls the horizontal offset of the bottom-right corner (0=rightmost, 1=center)

> **Usage**:
> 1. First aim the projector at the target surface without any correction (all corner values at 0.0)
> 2. If the image appears trapezoidally distorted (e.g., wider at top than bottom), adjust the corresponding corners to "pull" it back to a rectangle
> 3. Typically start fine-tuning from one corner and gradually correct
> 4. All values at 0.0 = no correction (original projection)

> **Common Correction Scenarios**:
> - Projector **upward projection** (projecting from below): need to shrink the Y values of the top two corners
> - Projector **side offset**: need to adjust the X values of both corners on the same side
> - **Pincushion/barrel distortion**: can be approximately corrected through comprehensive adjustment of all four corners

---

## 4. Usage Workflow

### 4.1 Basic Setup Steps

1. **Place Projector** — Drag a Super Projector into the scene
2. **Adjust Position and Orientation** — Move the projector to a suitable position, rotate to aim at the projection target surface
3. **Set Projection Texture** — Select the image to project in Mapping Texture
4. **Turn up Brightness** — Increase Dimmer from 0 (suggest starting testing at 50000)
5. **Adjust Projection Distance** — Set Max Light Distance to cover the target surface
6. **Adjust Projection Angle** — Control the projected image size via Zoom
7. **Fine-tune Correction** — If needed, use keystone correction to fix image distortion

### 4.2 Multi-Projector Stitching

If you need to cover a large area with multiple projectors:

1. Place multiple Super Projector Actors
2. Adjust their positions and angles so projection areas are adjacent
3. Appropriately increase Dilution Factor (edge softening) for natural transitions in overlapping areas
4. Set correspondingly mapped textures for each projector

---

## 5. Common Usage Examples

### Example 1: Stage Front Projection

- Position: Above the audience area, facing the stage backdrop
- Dimmer: 100,000 cd
- Max Light Distance: 2000cm (20m)
- Zoom: 30°
- Dilution Factor: 0.1
- Keystone: All values 0 (facing directly, no distortion)

### Example 2: Building Facade Projection

- Position: On the ground directly in front of the building
- Dimmer: 500,000 cd
- Max Light Distance: 5000cm (50m)
- Zoom: 45°
- Dilution Factor: 0.0
- Keystone: Due to upward projection, top two corners need adjustment

### Example 3: Small Decorative Projection

- Position: Close-range projection onto set pieces
- Dimmer: 10,000 cd
- Max Light Distance: 500cm (5m)
- Zoom: 60°
- Dilution Factor: 0.3
- Mapping Scale: (0.5, 0.5)

---

## 6. Notes

1. **Default brightness is 0** — A newly placed projector will not emit light; Dimmer must be manually increased
2. **Three-channel stacking** — Projection uses three independent RGB spotlights, theoretically 3× the lighting calculation of a regular spotlight
3. **Requires light function material** — Projection depends on the plugin's built-in light function material. If the material asset is missing, projection will not work
4. **Projection direction** — Projection direction is the Actor's negative X-axis (toward the Actor's "front")
5. **Shadows** — Shadow casting is enabled by default; objects in the scene will block projection light
6. **License verification** — Projection functionality requires a valid plugin license. Without authorization, Dimmer adjustment and keystone correction will not take effect
7. **Keystone correction range** — Each corner's adjustment range is 0~1, where 1 means pulling the corner to the image center. Fine-tuning values between 0~0.3 usually meet most correction needs
