# 15 - Full-Featured Moving Light (SuperStageLight)

> **Module**: SuperStage Runtime (ASuperStageLight)  
> **Target Users**: Lighting designers, blueprint developers  
> **Prerequisites**: [04 - Moving Light Base Class](04_Computer_Light.md)

---

## 1. Overview

**SuperStageLight** is the **most core fixture Actor** in the SuperStage system, inheriting from `ASuperLightBase`. It is the unified implementation class for all full-featured moving lights (Spot, Beam, Wash, Profile, cutting lights, etc.). On top of the base class Pan/Tilt motion control, it provides all the functionality needed by professional stage fixtures:

- **Beam Control** — Dimmer, Strobe, Zoom, Focus, Frost, Iris
- **Color System** — RGB/RGBW/HSV direct control, ColorWheel, CMY subtractive mixing, color temperature (CTO), Cool/Warm mixing, multi-channel free color mixing (ColorMix)
- **Gobo System** — Up to 3 Gobo wheels + gobo rotation (static/infinite/shake/scrolling)
- **Prism System** — Up to 2 prisms + prism rotation
- **Cutting System** — 4 cutting blade pairs (A/B) + cutting rotation
- **Effect System** — Built-in 256-level Effect LUT lookup + effect dimmer/strobe/color
- **Matrix System** — Pixel-level independent control (brightness/color/strobe/effect)
- **Spot Fill Light System** — Spot fill light (independent dimmer/strobe/color/Zoom/Frost/color temperature control)
- **Lift Control** — Z-axis displacement (inherited from SuperLightBase)

---

## 2. Class Inheritance

```
AActor
  └── ASuperBaseActor          ← Asset metadata + direction preview
        └── ASuperDmxActorBase  ← DMX channel reading + Fixture Library + Address Label
              └── ASuperLightBase    ← Pan/Tilt + Infinite Rotation + Speed + Lift
                    └── ASuperStageLight  ← This document (full-featured moving light)
```

---

## 3. Component Structure

```
SceneBase (Root Component)
  ├── ForwardArrow     — Direction arrow (inherited from SuperBaseActor, editor visible)
  ├── UpArrow          — Up arrow (inherited from SuperBaseActor, editor visible)
  ├── AddressLabel     — DMX address label (inherited from SuperDmxActorBase)
  ├── SM_Hook          — Hook/mount model (UStaticMeshComponent)
  ├── SM_Base          — Fixture base model (UStaticMeshComponent)
  ├── SceneRocker      — Rocker rotation axis (inherited from SuperLightBase, Pan rotation)
  │     └── SM_Rocker  — Rocker model (UStaticMeshComponent)
  └── SceneHard        — Head rotation axis (inherited from SuperLightBase, Tilt rotation)
        ├── SM_Hard           — Head model (UStaticMeshComponent)
        ├── SuperLighting     — Main light source component (USuperLightingComponent / USuperBeamComponent / USuperCuttingComponent)
        ├── SuperLightingArray[] — Matrix main light source component array
        ├── SpotLighting      — Spot fill light component (USuperSpotComponent)
        ├── SuperSpotArray[]  — Matrix Spot fill light component array
        ├── SuperEffect       — Effect component (USuperEffectComponent)
        ├── SuperEffectArray[] — Matrix effect component array
        └── SuperMatrix       — Matrix pixel component (USuperMatrixComponent)
```

---

## 4. Default Parameters (B.DefaultParameter)

### 4.1 FLightingComponentDefaultValue — Unified Lighting Defaults

| Parameter | Description | Default | Unit |
|------|------|--------|------|
| **MaxLightIntensity** | Maximum light source brightness percentage | 100.0 | % |
| **LensIntensity** | Lens intensity multiplier | 1.0 | — |
| **MaxLightDistance** | Maximum lighting distance | 2345.0 | cm |
| **DimmerCurveExponent** | Dimmer response curve exponent (1.0=linear, 2.0=squared recommended, 3.0=cubic) | 2.0 | — |

### 4.2 FLightLightSpotDefaultValue — SpotLight Parameters

| Parameter | Description | Default |
|------|------|--------|
| **bDisableLights** | Disable light source | false |
| **bDisableLightFunction** | Disable light function (when disabled, strobe is controlled by Tick directly) | false |
| **LightSpotIntensity** | SpotLight brightness multiplier | 1.0 |
| **ZoomRange** | Spot angle range | (1.0, 10.0) |
| **VolumetricScattering** | Volumetric scattering intensity | 0.0 |
| **bLightShadow** | Enable shadows | false |
| **bAffectTransmission** | Affect transmission | true |
| **SpecularScale** | Specular intensity scale | 1.0 |
| **SourceRadius** | Light source radius (affects soft shadows) | 0.0 |

### 4.3 FBeamComponentDefaultValue — Beam Parameters

| Parameter | Description | Default |
|------|------|--------|
| **bDisableBeam** | Disable beam (when disabled, only light source and spot remain) | false |
| **bBeamBlock** | Enable beam occlusion detection | false |
| **BeamIntensity** | Beam brightness multiplier | 1.0 |
| **AtmosphericDensity** | Atmospheric scattering density | 0.03 |
| **BeamFogIntensity** | Beam fog intensity | 20.0 |
| **AtmosBeamFogSpeed** | Atmosphere/fog flow speed | 10.0 |
| **LensRadius** | Lens flare radius | 10.0 |
| **BeamQuality** | Beam rendering quality | 75.0 |

### 4.4 Parameter Tuning Suggestions

#### Light Brightness & Distance

| Scenario | MaxLightIntensity | MaxLightDistance | LensIntensity |
|------|-------------------|-----------------|---------------|
| Small studio (10m) | 50 - 100 | 1500 | 0.5 - 1.0 |
| Medium theater (20m) | 100 - 200 | 2500 | 1.0 |
| Large arena (50m+) | 200 - 500 | 5000 - 8000 | 1.5 - 3.0 |
| Outdoor festival | 300 - 800 | 8000 - 10000 | 2.0 - 5.0 |

#### SpotLight Parameters

| Scenario | VolumetricScattering | bLightShadow | SourceRadius |
|------|---------------------|-------------|-------------|
| Photorealistic | 0.5 - 1.0 | true | 5.0 - 20.0 |
| Live preview (perf first) | 0.0 | false | 0.0 |
| Atmosphere effect | 1.5 - 3.0 | false | 0.0 |

#### Beam Parameters

| Scenario | BeamIntensity | AtmosphericDensity | BeamFogIntensity | AtmosFogSpeed |
|------|--------------|-------------------|-----------------|--------------|
| No fog | 0.5 - 1.0 | 0.3 - 0.5 | 0.0 | 0.0 |
| Light fog | 1.0 - 2.0 | 0.8 - 1.0 | 0.5 - 1.0 | 5.0 |
| Heavy fog | 2.0 - 5.0 | 1.5 - 3.0 | 2.0 - 5.0 | 10.0 - 20.0 |

---

## 5. Control Parameters (C.ControlParameter)

### 5.1 Beam Control Parameters

| Parameter | Description | Default | Range | Toggle |
|------|------|--------|------|------|
| **Dimmer** | Light brightness | 1.0 | 0~1 | bDimmer |
| **Strobe** | Strobe speed | 1.0 | 0~1 | bStrobe |
| **Zoom** | Beam/spot zoom | 0.0 | 0~1 | bZoom |
| **Focus** | Beam focus | 0.0 | 0~1 | bFocus |
| **Frost** | Frost effect intensity | 0.0 | 0~1 | bFrost |
| **Iris** | Iris size (1=full open, 0=minimum) | 1.0 | 0~1 | bIris |

#### Strobe Modes (via SubAttributes)

| Strobe Mode | Value | Description |
|----------|-----|------|
| **Open** | 1 | Always on |
| **Linear** | 2 | Linear strobe |
| **Pulse** | 3 | Pulse strobe |
| **PulseOpen** | 4 | Pulse open |
| **PulseClose** | 5 | Pulse close |
| **RampUp** | 6 | Fade in |
| **Random** | 7 | Random strobe (per-fixture independent random seed) |

### 5.2 Color Control Parameters

| Parameter | Description | Default | Toggle |
|------|------|--------|------|
| **Color** | RGB color value | White(1,1,1) | bColor |
| **ColorWheel** | Color wheel position | 0.0 | bColorWheel |
| **ColorTemperature** | Color temperature | 0.0 | bColorTemperature |
| **Cool** | Cool light channel | 0.0 | bCoolWarmMix |
| **Warm** | Warm light channel | 0.0 | bCoolWarmMix |

### 5.3 Gobo/Prism Control Parameters

| Parameter | Description | Default | Toggle |
|------|------|--------|------|
| **Gobo1** | Gobo wheel 1 position | 0.0 | bGobo1 |
| **Gobo2** | Gobo wheel 2 position | 0.0 | bGobo2 |
| **Gobo_Rot** | Gobo rotation | 0.0 | bGobo_Rot |
| **Prism1** | Prism 1 | 0.0 | bPrism1 |
| **Prism2** | Prism 2 | 0.0 | bPrism2 |
| **Prism_Rot** | Prism rotation | 0.0 | bPrism_Rot |

### 5.4 Effect Control Parameters

| Parameter | Description | Default | Toggle |
|------|------|--------|------|
| **EffectDimmer** | Effect brightness | 0.0 | bEffect |
| **EffectStrobe** | Effect strobe | 0.0 | bEffect |
| **EffectValue** | Effect LUT index | 0.0 | bEffect |
| **EffectColor** | Effect RGB color | White | bEffect |

---

## 6. Channel Toggles (D.ChannelSwitch)

### SuperStageLight Own Toggles

| Toggle | Function | Default |
|------|------|--------|
| **bDimmer** | Brightness control | false |
| **bStrobe** | Strobe control | false |
| **bZoom** | Zoom control | false |
| **bFocus** | Focus control | false |
| **bFrost** | Frost control | false |
| **bIris** | Iris control | false |
| **bColor** | RGB color control | false |
| **bColorWheel** | Color wheel control | false |
| **bColorTemperature** | Color temperature control | false |
| **bCoolWarmMix** | Cool/Warm dual-channel mixing | false |
| **bGobo1** | Gobo wheel 1 | false |
| **bGobo2** | Gobo wheel 2 | false |
| **bGobo_Rot** | Gobo rotation | false |
| **bPrism1** | Prism 1 | false |
| **bPrism2** | Prism 2 | false |
| **bPrism3** | Prism 3 | false |
| **bPrism_Rot** | Prism rotation | false |
| **bYRotation** | Head self-spin | false |
| **bCuttingChannel** | Cutting control | false |
| **bEffect** | Effect control | false |

> **Design Principle**: Only enable channel functions the fixture actually has. Turning off unnecessary toggles avoids unnecessary DMX reading and computation overhead.

---

## 7. Color System Details

SuperStageLight provides the most complete color control system in the industry, supporting everything from simple RGB direct control to complex multi-channel free color mixing + CTO color temperature adjustment.

### 7.1 RGB / RGBW Direct Control

| Function | Description | Matrix Version |
|------|------|----------|
| **SetLightingColorRGB** | RGB 3-channel color | SetLightingColorRGBMatrix |
| **SetLightingColorRGBW** | RGBW 4-channel color | SetLightingColorRGBWMatrix |

### 7.2 HSV Color Space

| Function | Description |
|------|------|
| **SetLightingColorHSV** | H+S+V 3-channel |
| **SetLightingColorHS** | H+S 2-channel (V fixed at 1.0, brightness via Dimmer) |
| **SetLightingColorHSAndCTO** | H+S + CTO color temperature multiply |

### 7.3 ColorWheel

Supports up to **3 color wheels** simultaneously via color atlas texture:

| Function | Color Wheels | Additional Features |
|------|-----------|----------|
| **SetLightingColorWheel** | 1 | — |
| **SetLightingColorWheel2** | 2 | Priority selection |
| **SetLightingColorWheel3** | 3 | Priority selection |

### 7.4 CMY Subtractive Mixing

| Function | Description |
|------|------|
| **SetLightingColorWheelAndCMY** | 1 color wheel + CMY |
| **SetLightingColorWheel2AndCMY** | 2 color wheels + CMY |
| **SetLightingColorWheel3AndCMY** | 3 color wheels + CMY |

### 7.5 Color Temperature Control (CTO)

| Function | Description | Matrix Version |
|------|------|----------|
| **SetLightingColorTemperature** | Single-channel color temperature | — |
| **SetLightingColorRGBWWithCTO** | RGBW + CTO | — |

Uses **Tanner Helland algorithm** to convert Kelvin color temperature to RGB (input range: 1000K ~ 40000K).

### 7.6 CoolWarm Mixing

| Function | Description | Matrix Version |
|------|------|----------|
| **SetLightingCoolWarmMix** | Main light cool/warm mixing | — |
| **SetSpotCoolWarmMix** | Spot fill cool/warm mixing | SetSpotCoolWarmMixMatrix |

### 7.7 Multi-Channel Free ColorMix

Most flexible color control, supporting any number of color channels via `FSuperColorChannel`:

| Function | Description | Matrix Version |
|------|------|----------|
| **SetLightingColorMix** | Free color mixing | SetLightingColorMixMatrix |
| **SetLightingColorMixWithCTO** | Free mixing + CTO | SetLightingColorMixMatrixWithCTO |

---

## 8. Gobo, Prism, Cutting, and Effect Systems

### Gobo System
- Up to 3 Gobo wheels via GoboAtlas texture
- Modes: Static, Shake, Scrolling
- Priority: Non-zero DMX value wheels take priority; Gobo1 > Gobo2 > Gobo3

### Prism System
- Prism parameters defined in fixture library ChannelSet (PrismFacets, PrismRadius, PrismScale)
- Prism rotation reuses gobo rotation logic, supports static and infinite modes

### Cutting System
- 4 blade pairs (A1~B4), 8 DMX attributes for blade positions
- ShaperRot + GoboRot combined for cutting rotation

### Effect System
- Built-in 256-level Effect LUT look-up table
- DMX normalized value 0~1 → Lookup → FEffectParams { Effect, Speed, Width }
- Dirty value optimization with cached last parameters

---

## 9. Typical Blueprint Structures

### Typical Spot Light Blueprint

```
Event BeginPlay
  ├─ SetLightingDefaultValue(SuperLighting)        ← Init main light
  └─ SetEffectDefaultValue(SuperEffect)            ← Init effect component

Event SuperDMXTick(DeltaTime)
  ├─ Motion Control: YPan, YTilt, YPTSpeed
  ├─ Beam Control: SetLightingIntensity, SetLightingStrobe, SetLightingZoom
  ├─ Color Control: SetLightingColorRGB
  ├─ Gobo Control: SetBeamGobo1
  └─ Effect Control: SetEffectIntensity, SetEffect
```

### Typical Profile Cutting Light Blueprint

```
Event SuperDMXTick(DeltaTime)
  ├─ ... (motion/beam/color same as above)
  ├─ Cutting: SetBeamCutting(A1, B1, A2, B2, A3, B3, A4, B4)
  │            SetBeamCuttingRotate(ShaperRot, GoboRot)
  └─ Gobo+Prism: SetBeamGobo2
```

### Typical Matrix Light Blueprint

```
Event SuperDMXTick(DeltaTime)
  ├─ Matrix Motion: YMatrixPan, YMatrixTilt
  ├─ Matrix Beam: SetLightingIntensityMatrix, SetLightingStrobeMatrix
  ├─ Matrix Color: SetLightingColorRGBMatrix
  └─ Matrix Effect: SetEffectMatrix
```

---

## 10. DMX Channel Planning Examples

### Basic Spot Light (18 Channels)

| Channel | Attribute | Bit Depth | Description |
|------|------|------|------|
| 1-2 | Pan | 16bit | Horizontal rotation |
| 3-4 | Tilt | 16bit | Vertical rotation |
| 5 | PTSpeed | 8bit | Motion speed |
| 6 | Dimmer | 8bit | Brightness |
| 7 | Strobe | 8bit | Strobe |
| 8 | Red | 8bit | Red |
| 9 | Green | 8bit | Green |
| 10 | Blue | 8bit | Blue |
| 11 | White | 8bit | White |
| 12 | Zoom | 8bit | Zoom |
| 13 | Gobo1 | 8bit | Gobo wheel |
| 14 | Gobo1Rot | 8bit | Gobo rotation |
| 15 | Prism1 | 8bit | Prism |
| 16 | PrismRot | 8bit | Prism rotation |
| 17 | Focus | 8bit | Focus |
| 18 | Frost | 8bit | Frost |

### Profile Cutting Light (28 Channels)

| Channel | Attribute | Bit Depth | Description |
|------|------|------|------|
| 1-2 | Pan | 16bit | Horizontal rotation |
| 3-4 | Tilt | 16bit | Vertical rotation |
| 5 | PTSpeed | 8bit | Motion speed |
| 6 | Dimmer | 8bit | Brightness |
| 7 | Strobe | 8bit | Strobe |
| 8 | Cyan | 8bit | CMY-Cyan |
| 9 | Magenta | 8bit | CMY-Magenta |
| 10 | Yellow | 8bit | CMY-Yellow |
| 11 | CTO | 8bit | Color temperature |
| 12 | ColorWheel | 8bit | Color wheel |
| 13 | Zoom | 8bit | Zoom |
| 14 | Focus | 8bit | Focus |
| 15 | Iris | 8bit | Iris |
| 16 | Gobo1 | 8bit | Gobo wheel 1 |
| 17 | Gobo1Rot | 8bit | Gobo rotation 1 |
| 18 | Gobo2 | 8bit | Gobo wheel 2 |
| 19 | Gobo2Rot | 8bit | Gobo rotation 2 |
| 20 | Prism1 | 8bit | Prism |
| 21 | PrismRot | 8bit | Prism rotation |
| 22 | Frost | 8bit | Frost |
| 23 | A1 | 8bit | Cutting 1A |
| 24 | B1 | 8bit | Cutting 1B |
| 25 | A2 | 8bit | Cutting 2A |
| 26 | B2 | 8bit | Cutting 2B |
| 27 | ShaperRot | 8bit | Cutting rotation |
| 28 | Effect | 8bit | Effect |

---

## 11. FAQ

### Q: Light brightness on but no light visible?
1. Check if `MaxLightIntensity` and `MaxLightDistance` are reasonable
2. Confirm `bDimmer` toggle is enabled
3. Check if `bModelVisibility` is true
4. Confirm lighting channels are correctly set (LightingChannel0/1/2)

### Q: Color wheel not showing color?
1. Confirm valid `ColorAtlas` texture is passed
2. Confirm sub-attributes and ChannelSets are defined for the attribute in the fixture library
3. Check if `bColorWheel` toggle is enabled

### Q: Cutting not working?
1. Confirm main light component is `USuperCuttingComponent` type
2. Confirm `bCuttingChannel` toggle is enabled
3. A parameter from 0 to 0.4 increases cutting; B parameter from 1.0 to 0.6 increases cutting

### Q: Prism no effect?
1. Confirm main light component is `USuperBeamComponent` type
2. Confirm ChannelSet for prism attribute defines `PrismFacets > 0`
3. Check if `PrismRadius` and `PrismScale` are reasonable

### Q: Effect LUT not responding?
1. Confirm `SetEffectDefaultValue` has been called to initialize effect component
2. Confirm `bEffect` toggle is enabled
3. DMX value 0 typically corresponds to "no effect"

### Q: Matrix control only partially responds?
1. Check if module instance count in fixture library matches component array length
2. Confirm each module instance's Patch value is correct
3. Use Matrix-suffixed functions (e.g., `SetLightingIntensityMatrix`) instead of single light versions

### Q: How to configure RGBWAL fixture colors?
Use the `SetLightingColorMix` function passing a `FSuperColorChannel` array. Each channel defines one DMX attribute and corresponding color. Supports any number of color channel combinations.

---

> **Navigation**: [← 14 - Stage VFX](14_Stage_VFX.md) | [Return to Overview](00_DMX_System_Overview.md)
