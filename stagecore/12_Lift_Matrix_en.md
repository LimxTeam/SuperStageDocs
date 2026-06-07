# 12 - Lift Matrix

> **Module**: SuperStage Runtime (ASuperLiftMatrix)  
> **Target Users**: Lighting designers, stage technical staff  
> **Prerequisites**: [04 - Moving Light](/docs/stage-core/computer-light)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

**SuperLiftMatrix** is a special stage fixture Actor that arranges multiple light-emitting components vertically on a steel cable suspension structure, controlled via DMX to achieve **lift deployment/retraction** and **independent color/effect control**. It simulates the lift matrix light arrays (Kinetic Light) commonly seen in performances.

### Class Inheritance Chain

```
AActor (UE5 Engine Base Class)
  │
  └── ASuperBaseActor ·············· SuperStage Root Class (L1)
        │
        └── ASuperDmxActorBase ····· DMX Fixture Base Class (L2)
              │                      ├ DMX Address / Fixture Library / Channel Reading
              │                      └ SuperDMXTick / Matrix Reading
              │
              └── ASuperLightBase ·· Moving Light Base (L3)
                    │                 ├ Pan/Tilt Control / Infinite Rotation
                    │                 ├ LiftRange lift control
                    │                 └ PTSpeed interpolation speed
                    │
                    └── ASuperLiftMatrix ·· Lift Matrix (L4) ← This document
                          ├ 5 LiftComponents (each with 2 EffectComponents)
                          ├ 4 CableComponents (steel cables)
                          ├ Lift deploy/retract control (InPosZ)
                          ├ Matrix effect + Matrix color independent control
                          └ MaxLightIntensity brightness cap
```

> **Design Note**: `ASuperLiftMatrix` inherits from `ASuperLightBase`, so it inherently has Pan/Tilt rotation capability — the entire lift matrix light array can rotate while performing lift/deploy actions. The lift function reuses the parent class's `InPosZ` channel and `LiftRange` parameter.

### Physical Structure

```
    ┌─ CableOrigin (steel cable origin point, fixed at top)
    │
    ║  ║                    ←── 4 steel cables (4-corner suspension)
    ║  ║
    ├──┤ LiftComponent_0   ←── Layer 1 (each with 2 EffectComponents)
    ║  ║
    ├──┤ LiftComponent_1   ←── Layer 2
    ║  ║
    ├──┤ LiftComponent_2   ←── Layer 3
    ║  ║
    ├──┤ LiftComponent_3   ←── Layer 4
    ║  ║
    └──┘ LiftComponent_4   ←── Layer 5
```

### Core Features

- **5 Layers of Light-Emitting Components** — Vertically arranged, each layer contains 2 SuperEffectComponents
- **4 Steel Cables** — Extend from top four corners to the bottom layer, length auto-adjusts with lift
- **DMX Lift Control** — Controls overall deploy/retract via one DMX channel
- **Matrix Effect Control** — Each layer independently controls effects and color (via matrix DMX reading)
- **Inherits Pan/Tilt** — Inherits from SuperLightBase, supports overall rotation

### Use Cases

- Lift matrix light arrays (Kinetic Light)
- Dynamic hanging fixture installations
- Interactive installation art
- Stage background dynamic lighting decoration

---

## 2. Component Hierarchy

```
Actor Root
  ├── CableOrigin (cable origin, height 15cm)
  │     ├── Cable_0 (front-left cable)
  │     ├── Cable_1 (front-right cable)
  │     ├── Cable_2 (back-left cable)
  │     └── Cable_3 (back-right cable)
  │
  ├── LiftComponent_0 (Layer 1)
  │     ├── EffectComponentA_0
  │     └── EffectComponentB_0
  ├── LiftComponent_1 (Layer 2)
  │     ├── EffectComponentA_1
  │     └── EffectComponentB_1
  ├── LiftComponent_2 (Layer 3)
  │     ├── EffectComponentA_2
  │     └── EffectComponentB_2
  ├── LiftComponent_3 (Layer 4)
  │     ├── EffectComponentA_3
  │     └── EffectComponentB_3
  └── LiftComponent_4 (Layer 5)
        ├── EffectComponentA_4
        └── EffectComponentB_4
```

**Total**: 5 lift layers × 2 effect components = **10 independently controllable light-emitting units**

---

## 3. Property Details

### 3.1 Default Parameters

| Parameter | Location | Description | Range | Default |
|------|------|------|------|--------|
| **MaxLightIntensity** | B.DefaultParameter | Maximum light intensity | 0 - unlimited | 1.0 |
| **LiftRange** | B.DefaultParameter (inherited) | Lift range (total travel) | 0 - unlimited | 500.0 cm |

**LiftRange** comes from the parent class SuperLightBase, defining the total height from fully retracted to fully deployed (unit: cm).

#### Parameter Tuning Suggestions

| Scenario | MaxLightIntensity | LiftRange | Description |
|------|-------------------|-----------|------|
| Small installation (< 3m) | 0.5 - 2.0 | 100 - 200 cm | Close viewing, moderate brightness |
| Medium stage (3-8m) | 2.0 - 5.0 | 300 - 600 cm | Standard performance scenario |
| Large concert (8m+) | 5.0 - 15.0 | 600 - 1200 cm | Long distance needs higher brightness and larger travel |
| Interactive installation art | 1.0 - 3.0 | 50 - 150 cm | Small movement, precision-focused |

### 3.2 Internal Constants

| Constant | Value | Description |
|------|-----|------|
| **ComponentCount** | 5 | Number of lift layers |
| **ComponentSpacing** | 5.0 cm | Layer spacing in retracted state |
| **DivisionCount** | 6 | Deployment equal division count (5 layers occupy positions 1/6~5/6 of 6 divisions) |
| **CableCornerOffset** | 22.0 cm | Cable offset distance from center |
| **CableOriginHeight** | 15.0 cm | Cable origin point height |
| **CableRadius** | 0.5 cm | Cable radius |

---

## 4. Lift Control

### 4.1 Lift Mapping

One DMX attribute (InPosZ, from parent class) controls the deployment degree of all layers:

| DMX Value | State | Description |
|--------|------|------|
| 0.0 | Fully retracted | All layers tightly packed, 5cm spacing |
| 0.5 | Half deployed | Layers evenly distributed within half of LiftRange |
| 1.0 | Fully deployed | Layers evenly distributed within entire LiftRange |

### 4.2 Deployment Distribution Algorithm

Each layer's Z position is calculated by **equal division distribution**:

```
Layer i (i=0~4):
  Retracted position = -ComponentSpacing × i        (tightly packed)
  Deployed position = -LiftRange × (i+1) / 6        (6 divisions, positions 1~5)
  Actual position = Lerp(Retracted, Deployed, DMX Value)
```

**Example** (LiftRange = 600cm, DMX = 1.0 fully deployed):

| Layer | Retracted Z | Deployed Z | Description |
|----|--------|--------|------|
| Layer 0 | 0 cm | -100 cm | 1/6 position |
| Layer 1 | -5 cm | -200 cm | 2/6 position |
| Layer 2 | -10 cm | -300 cm | 3/6 position |
| Layer 3 | -15 cm | -400 cm | 4/6 position |
| Layer 4 | -20 cm | -500 cm | 5/6 position |

> **Note**: The top 1/6 (0~-100cm) is empty, reserved for the cable origin and suspension mechanism.

### 4.3 Automatic Cable Update

During lifting, the 4 steel cables automatically adjust length:

- Cables extend from CableOrigin (height 15cm) to the lowest layer component
- Length = CableOriginHeight - bottom layer Z position
- Positioned at four corners (offset ±22cm)
- Achieved by scaling cylinder mesh

---

## 5. Effect Control

### 5.1 Initialization

| Function | Description |
|------|------|
| **SetEffectMatrixDefaultValue** | Initialize all 10 effect component materials, set MaxLightIntensity |

**Must be called before use** — typically in blueprint's BeginPlay or before SuperDMXTick's first execution.

### 5.2 Effect Selection

| Function | Description |
|------|------|
| **SetEffectMatrix(DmxAttribute)** | Control each layer's effect selection via matrix DMX channels |

Uses **matrix DMX reading** (GET_SUPER_DMX_MATRIX_VALUE macro), each effect component controlled by independent DMX channel:

- DMX value (0~255) mapped to specific effect parameters via **Effect LUT (Look-Up Table)**
- Effect parameters include: Effect (effect type), Speed, Width
- 10 effect components independently controllable with different effects

### 5.3 Color Control

| Function | Description |
|------|------|
| **SetEffectColorMatrix(DmxR, DmxG, DmxB)** | Control each layer's color via matrix DMX channels |

Also uses matrix DMX reading, each effect component controlled by 3 independent channels (R/G/B) for color.

---

## 6. Blueprint Control

### 6.1 Typical Blueprint Implementation

```
Event BeginPlay
  │
  └─ SetEffectMatrixDefaultValue()    ← Initialize materials

Event SuperDMXTick(DeltaTime)
  │
  ├─ LiftMatrix(DmxLift)              ← Lift control
  ├─ SetEffectMatrix(DmxEffect)       ← Effect selection (matrix)
  └─ SetEffectColorMatrix(R, G, B)    ← Color control (matrix)
```

### 6.2 DMX Channel Planning

| Channel | Attribute | Type | Description |
|------|------|------|------|
| 1-2 | Lift | 16-bit | Lift control (InPosZ) |
| 3+ | Effect × N | Matrix | Effect selection (each head independent) |
| N+ | R/G/B × N | Matrix | Color control (each head independent, 3 channels each) |

> **Channel count depends on matrix head count**. 10 effect components × (1 effect + 3 color) = 40 channels + 2 lift channels = 42 channels.

---

## 7. FAQ

### Q: Lift range not large enough / too large?
Adjust the **LiftRange** parameter. For example, setting 1000 means 10-meter travel.

### Q: Effect components not illuminated?
1. Confirm **SetEffectMatrixDefaultValue** has been called to initialize materials
2. Check that **MaxLightIntensity** is not 0
3. Confirm DMX signals are correctly arriving

### Q: Steel cables not visible?
Cables use the default engine cylinder mesh; if the resource is not available in the scene, they won't display. This does not affect functionality.

### Q: How to use only some layers?
Currently the component count is fixed at 5 layers (10 effect components). If fewer layers are needed, set the DMX effect channels for unused layers to 0 (off).

---

> **Next Steps**: Read [13 - LED Strip Effect](/docs/stage-core/light-strip) to learn about strip effect control.
