# 04 - Cutting Beam Component (SuperCuttingComponent)

> **Module**: SuperStage Runtime  
> **Target Audience**: Lighting designers, fixture Blueprint creators  
> **Prerequisite Reading**: [03 - Volumetric Beam Component](03_SuperBeamComponent.md)

---

## 1. Overview

**SuperCuttingComponent** is a component that adds **four-leaf shutter cutting** functionality on top of the volumetric beam component. It simulates the shutter mechanism of real Profile lights (imaging lights / cutting lights), capable of cropping a circular aperture into any quadrilateral shape — from simple rectangles to complex trapezoids and parallelograms.

On real stages, Profile lights are typically equipped with 4 independently adjustable metal shutter blades. Both ends of each blade can extend/retract independently to crop the aperture in four directions (top, bottom, left, right). SuperCuttingComponent perfectly simulates this mechanism.

### Use Cases

- **Profile Lights / Imaging Lights** — Fixtures requiring precise aperture shape control
- **Cutting Lights** — Fixtures needing aperture cropped to specific shapes (such as stage areas, building silhouettes)
- **Follow Spots** — Fixtures needing shutter-limited aperture range

---

## 2. Component Structure

```
SuperCuttingComponent (inherits from SuperBeamComponent)
  ├── YStaticMeshLens   — Lens model (inherited, uses cutting-specific lens material)
  ├── YArrowComponent   — Direction arrow (inherited)
  ├── YSpotLight        — Spotlight source (inherited, uses cutting-specific aperture material)
  └── YStaticMeshBeam   — Beam model (inherited, uses cutting-specific beam material)
```

### Cutting-Specific Materials

SuperCuttingComponent **replaces** the parent class's three material systems with cutting-specific versions during construction:

| Material | Path | Description |
|------|------|------|
| **Beam Material** | MI_Beam_Cutting_Inst | Volumetric beam material supporting cutting parameters |
| **Aperture Material** | MI_CuttingLampLight_Inst | Light function material supporting cutting parameters |
| **Lens Material** | MI_CuttingLens_Inst | Lens material supporting cutting parameters |

> **Key Difference**: Regular Beam component materials do not support cutting parameters; only cutting-specific materials respond to shutter blade adjustments.

---

## 3. Shutter System (Cutting / Framing Shutter)

### 3.1 Shutter Structure

The cutting component has **4 shutter blades**, respectively controlling the top, bottom, left, and right edges of the aperture. **Both ends** of each blade can be independently adjusted, for a total of **8 control points**:

```
        ┌─── TopLeftX ───┬─── TopRightX ───┐
        │                │                  │
   UpperLeftY            │             TopRightY
        │                │                  │
        │      ┌─────────┼────────┐         │
        │      │  Visible Aperture│         │
        │      │        Area      │         │
        │      └─────────┼────────┘         │
        │                │                  │
   LowerLeftY            │           BottomRightY
        │                │                  │
        └── BottomLeftX ─┴── BottomRightX ──┘
```

### 3.2 Eight-Point Parameters

| Parameter | Direction | Edge Controlled | Description |
|------|------|-----------|------|
| **UpperLeftY** | Vertical | Left blade - upper end | Controls extension of the upper portion of the left blade |
| **LowerLeftY** | Vertical | Left blade - lower end | Controls extension of the lower portion of the left blade |
| **TopRightY** | Vertical | Right blade - upper end | Controls extension of the upper portion of the right blade |
| **BottomRightY** | Vertical | Right blade - lower end | Controls extension of the lower portion of the right blade |
| **TopLeftX** | Horizontal | Top blade - left end | Controls extension of the left portion of the top blade |
| **BottomLeftX** | Horizontal | Bottom blade - left end | Controls extension of the left portion of the bottom blade |
| **TopRightX** | Horizontal | Top blade - right end | Controls extension of the right portion of the top blade |
| **BottomRightX** | Horizontal | Bottom blade - right end | Controls extension of the right portion of the bottom blade |

> **Parameter Value Meaning**:  
> - **0.0** = Blade fully open (no obstruction)  
> - Increase value = Blade gradually extends into the aperture, blocking the beam  
> - Maximum value = Blade fully closed (blocks all light in that direction)

### 3.3 Cutting Shape Examples

#### Rectangle Cut
Set all blades to the same extension amount:
```
UpperLeftY = LowerLeftY = 0.3      ← Left side evenly blocked
TopRightY = BottomRightY = 0.3     ← Right side evenly blocked
TopLeftX = TopRightX = 0.2         ← Top side evenly blocked
BottomLeftX = BottomRightX = 0.2   ← Bottom side evenly blocked
```
Effect: Produces a centered rectangular aperture.

#### Trapezoid Cut
Use different values for upper and lower ends of left/right blades:
```
UpperLeftY = 0.2, LowerLeftY = 0.4   ← Left lower end blocks more than upper
TopRightY = 0.2, BottomRightY = 0.4  ← Right lower end blocks more than upper
```
Effect: Produces a top-wide, bottom-narrow trapezoidal aperture.

#### Triangle Approximation
One side blade fully closed:
```
UpperLeftY = 0.0, LowerLeftY = 1.0   ← Left from fully open to fully closed
TopRightY = 1.0, BottomRightY = 0.0  ← Right from fully closed to fully open
```
Effect: Produces an approximately triangular aperture.

---

## 4. DMX-Controlled Parameters

### 4.1 Cutting Parameters

| Parameter Group | DMX Channels | Description |
|--------|----------|------|
| **Blade A (Top)** | TopLeftX, TopRightX | Controls left and right ends of the top blade |
| **Blade B (Bottom)** | BottomLeftX, BottomRightX | Controls left and right ends of the bottom blade |
| **Blade C (Left)** | UpperLeftY, LowerLeftY | Controls upper and lower ends of the left blade |
| **Blade D (Right)** | TopRightY, BottomRightY | Controls upper and lower ends of the right blade |

### 4.2 Cutting Rotation (Rotate)

| Parameter | Description | Range |
|------|------|------|
| **Rotation Angle** | Rotates the entire cutting shape by the specified angle | 0 ~ 360 (degrees) |
| **Infinite Rotation** | Cutting shape continuously rotates | Any float value |

Cutting rotation is **synchronously** applied to the beam, aperture, and lens three material systems.

> **Difference from Pattern Rotation**: Cutting rotation controls the overall rotation of the shutter blades; Pattern Rotation (Gobo Rotate) controls the rotation of the Gobo pattern. The two are independent.

### 4.3 All Inherited Parameters

SuperCuttingComponent fully inherits all functionality from SuperBeamComponent:

| Function | Description |
|------|------|
| **Dimmer** | Controls beam, aperture, and lens brightness |
| **Color** | Controls overall color |
| **Zoom** | Controls light cone angle |
| **Frost** | Softens aperture edges |
| **Iris** | Shrinks aperture |
| **Gobo Pattern** | Projects Gobo patterns within the cutting shape |
| **Prism** | Prism splitting effect |
| **Focus** | Gobo pattern sharpness |
| **Color Wheel** | Up to 3 sets of color wheels |
| **Strobe** | 8 strobe modes |
| **Visibility** | Beam show/hide control |

---

## 5. Usage Tips

### 5.1 Cutting + Gobo Combination

Cutting and Gobo patterns can be **stacked**:
- First project a pattern via Gobo (e.g., window shape)
- Then crop out unwanted portions via cutting blades
- Effect: Precisely controls the area of pattern projection

### 5.2 Cutting + Rotation Combination

1. First set up the cutting shape (e.g., a narrow strip)
2. Use the rotation parameter to rotate the cutting shape to the desired angle
3. Combine with infinite rotation for rotating light column effects

### 5.3 Precise Edge Control

- Both ends of each blade are **independently adjustable**, enabling angled edges
- Four blades combined can create any convex quadrilateral
- Combine with Frost parameter to soften cutting edges

---

## 6. FAQ

### Q: Cutting effect not visible in the beam?
**A**: Confirm the fixture Blueprint uses SuperCuttingComponent, not SuperBeamComponent. Only the cutting component uses cutting-specific materials; regular beam components don't support cutting parameters.

### Q: Cutting shape position is off after rotation?
**A**: Cutting rotation is centered around the beam's center axis. Confirm rotation angle parameters are applied to all three material systems (the component handles this automatically).

### Q: How to achieve soft cutting edges?
**A**: Increase the Frost parameter value. Frost softens all aperture edges, including edges created by cutting.

### Q: Cutting parameter precision insufficient?
**A**: In the fixture library, configure cutting channels as 16bit (fine + coarse dual channels) for more precise blade control.

---

## 7. API Quick Reference

The following are public function signatures of `USuperCuttingComponent`:

| Function Signature | Description |
|----------|------|
| `void SetCuttingValue(float UpperLeftY, float LowerLeftY, float TopRightY, float BottomRightY, float TopLeftX, float BottomLeftX, float TopRightX, float BottomRightX) const` | Set 8-point cutting parameters |
| `virtual void SetLightingRotate(const float NewRotate, const float NewInfiniteRotation = 0.f) override` | Cutting shape rotation (three materials sync) |

> **Inheritance**: All APIs inherited from SuperBeamComponent (brightness, color, zoom, frost, iris, Gobo, prism, focus, strobe, visibility, etc.). See [03 - Volumetric Beam Component API Reference](03_SuperBeamComponent.md#7-api-quick-reference).
