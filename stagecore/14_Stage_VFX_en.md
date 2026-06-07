# 14 - Stage VFX

> **Module**: SuperStage Runtime (ASuperStageVFXActor)  
> **Target Users**: Lighting designers, stage VFX artists  
> **Prerequisites**: [04 - Moving Light](/docs/stage-core/computer-light)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

**SuperStageVFXActor** is a stage VFX Actor that controls **Niagara particle systems** via DMX. It maps DMX signals to particle system parameters, enabling real-time control of stage effects such as smoke, flame, confetti, snow, CO2 jets, and fireworks.

### Class Inheritance Chain

```
AActor (UE5 Engine Base Class)
  │
  └── ASuperBaseActor ·············· SuperStage Root Class (L1)
        │
        └── ASuperDmxActorBase ····· DMX Fixture Base Class (L2)
              │                      ├ DMX Address / Fixture Library / Channel Reading
              │                      └ SuperDMXTick / Address Label
              │
              └── ASuperLightBase ·· Moving Light Base (L3)
                    │                 ├ Pan/Tilt Control → Projection direction
                    │                 └ SceneRocker / SceneHard rotation axes
                    │
                    └── ASuperStageVFXActor · Stage VFX (L4) ← This document
                          ├ UNiagaraComponent (mounted to SceneHard)
                          ├ SpawnCount / Color → Niagara user variables
                          ├ MaxSpawnCount / MaxSpeed / LoopTime
                          └ Minimum / Maximum range parameters
```

> **Design Note**: `ASuperStageVFXActor` inherits from `ASuperLightBase`, so the Niagara particle component is mounted to `SceneHard` (the head rotation axis). This means the particle emission direction can be remotely controlled via Pan/Tilt — for example, a smoke machine can adjust the emission direction through the console.

### Core Features

- **Niagara-Driven** — Uses UE's Niagara particle system engine, providing high-quality real-time particle effects
- **DMX Control** — Controls particle count (SpawnCount) and color (RGB) via DMX channels
- **Inherits Pan/Tilt** — Inherits from SuperLightBase, Niagara component mounted on SceneHard, supports Pan/Tilt rotation for controlling projection direction
- **Configurable Parameters** — Speed, loop time, range and other parameters customizable in the editor
- **Auto-Initialize** — Automatically activates particle system and sets default parameters at BeginPlay

### Use Cases

| Effect Type | Description |
|---------|------|
| **Smoke Machine** | DMX controls smoke emission amount and color |
| **Flame Effect** | DMX controls flame intensity and color |
| **Confetti Cannon** | DMX triggers confetti launch |
| **Snow Machine** | DMX controls snow density |
| **CO2 Jet** | DMX controls CO2 emission amount |
| **Fireworks** | DMX triggers firework launch |
| **Water Mist / Bubbles** | DMX controls spray density and color |

---

## 2. Component Structure

```
Actor Root (SceneBase)
  └── SceneRocker (Pan rotation)
        └── SceneHard (Tilt rotation)
              └── Niagara (particle system component)
```

**Niagara component mounted on SceneHard**, meaning particle emission direction follows Pan/Tilt rotation. For example:
- Smoke machine: Pan/Tilt controls smoke emission direction
- CO2 jet: Emission direction can be remotely adjusted
- Flame effect: Projection angle can change in real-time

---

## 3. Property Details

### 3.1 Default Parameters (B.DefaultParameter)

| Parameter | Description | Range | Default | Unit |
|------|------|------|--------|------|
| **MaxSpawnCount** | Maximum particle spawn count | 0 - 100 | 1.0 | per frame |
| **MaxSpeed** | Maximum particle movement speed | 0 - 100 | 100.0 | — |
| **LoopTime** | Particle loop time | 0 - 100 | 100.0 | seconds |
| **Minimum** | Minimum range/size | 0 - 100,000 | 100.0 | cm |
| **Maximum** | Maximum range/size | 0 - 100,000 | 100.0 | cm |

#### MaxSpawnCount

Controls the maximum number of particles spawned per frame when DMX value = 1.0:
- DMX SpawnCount = 0.0 → No particles spawned (effect off)
- DMX SpawnCount = 0.5 → Spawn MaxSpawnCount × 0.5 particles
- DMX SpawnCount = 1.0 → Spawn MaxSpawnCount particles

#### MaxSpeed

Sets the `Speed` variable value in the Niagara system, controlling particle movement speed. Specific impact depends on the Niagara system's module configuration.

#### LoopTime

Sets the `LoopTime` variable value in the Niagara system, controlling the particle effect's loop period.

#### Minimum / Maximum

Sets the `Minimum` and `Maximum` variable values in the Niagara system. Typically used to control particle emission range or spread range, in centimeters.

> **Note**: These parameters are passed as Niagara system **user variables**. Specific effects depend on how these variables are referenced in the Niagara system asset you're using. Ensure the Niagara system defines `Speed`, `LoopTime`, `Minimum`, `Maximum` as Float-type user variables.

### 3.2 Parameter Tuning Suggestions

| Effect Type | Recommended MaxSpawnCount | Recommended MaxSpeed | Recommended LoopTime | Recommended Min/Max | Description |
|---------|-------------------|--------------|--------------|-------------|------|
| **Smoke** | 30 - 100 | 50 - 150 | 10 - 30 | 50 / 300 | Continuous high-density particles, low-mid speed diffusion |
| **CO2 Jet** | 50 - 200 | 200 - 500 | 1 - 5 | 10 / 100 | Short burst high-speed emission, small range |
| **Flame** | 10 - 50 | 100 - 300 | 5 - 15 | 20 / 150 | Medium density, upward movement |
| **Confetti Cannon** | 1 - 10 | 300 - 800 | 0.5 - 2 | 50 / 500 | Low count high-speed launch, large spread |
| **Snow** | 50 - 200 | 10 - 50 | 30 - 100 | 200 / 1000 | High count low-speed particles, large coverage |
| **Fireworks** | 1 - 5 | 500 - 1000 | 1 - 3 | 100 / 800 | Very low count explosive launch |
| **Bubbles** | 5 - 30 | 20 - 80 | 10 - 30 | 50 / 400 | Low density, slow float |

> **Tip**: Above are suggested starting values; actual results depend on the design of the Niagara system asset you're using. Try recommended values first, then preview and fine-tune in the editor.

### 3.3 Control Parameters (C.ControlParameter)

| Parameter | Description | Range | Default |
|------|------|------|--------|
| **SpawnCount** | Particle spawn amount (normalized) | 0.0 - 1.0 | 0.0 |
| **Color** | Particle color (RGB) | Per channel 0.0 - 1.0 | White (1,1,1,1) |

---

## 4. DMX Control Mapping

### 4.1 SpawnCount

```
DMX Attribute Value [0.0 ~ 1.0]
  ↓
Lerp(0, MaxSpawnCount, DMX Value) → RoundToInt → Integer particle count
  ↓
Niagara->SetVariableInt("SpawnCount", Integer Value)
```

| DMX Value | When MaxSpawnCount = 50 | Effect |
|--------|---------------------|------|
| 0.0 | 0 | Effect off |
| 0.2 | 10 | Low particles |
| 0.5 | 25 | Medium density |
| 1.0 | 50 | Max density |

### 4.2 Color

```
DMX R/G/B Three Channels [each 0.0 ~ 1.0]
  ↓
Combined as FLinearColor(R, G, B)
  ↓
Niagara->SetVariableLinearColor("Color", Color Value)
```

> **Note**: The Niagara system needs to define a LinearColor-type user variable named `Color`.

---

## 5. Blueprint Control

### 5.1 Control Functions

#### SetNiagaraDefaultValue (Initialization)

Initializes and activates the Niagara particle system, sets default parameters. **Automatically called at runtime in BeginPlay**.

#### SetNiagaraSpawnCount(DmxAttribute)

Reads one DMX attribute, maps to particle spawn count:

| Input Parameter | Description |
|---------|------|
| **DmxAttribute** | DMX attribute controlling spawn count |

#### SetNiagaraColor(DmxR, DmxG, DmxB)

Reads three DMX attributes (RGB), maps to particle color:

| Input Parameter | Description |
|---------|------|
| **DMXAttR** | DMX attribute for red channel |
| **DMXAttG** | DMX attribute for green channel |
| **DMXAttB** | DMX attribute for blue channel |

### 5.2 Typical Blueprint Implementation

```
Event SuperDMXTick(DeltaTime)
  │
  ├─ SetNiagaraSpawnCount(SpawnCount)     ← Control particle count
  └─ SetNiagaraColor(Red, Green, Blue)    ← Control particle color
```

With Pan/Tilt control for projection direction (inherited from SuperLightBase):

```
Event SuperDMXTick(DeltaTime)
  │
  ├─ SuperLightControl(Pan, Tilt)         ← Control projection direction
  ├─ SetNiagaraSpawnCount(SpawnCount)     ← Control particle count
  └─ SetNiagaraColor(Red, Green, Blue)    ← Control particle color
```

---

## 6. DMX Channel Planning

### Basic Configuration (4 Channels)

| Channel | Attribute | Description |
|------|------|------|
| 1 | SpawnCount | Particle count |
| 2 | Red | Red |
| 3 | Green | Green |
| 4 | Blue | Blue |

### With Pan/Tilt Control (8 Channels)

| Channel | Attribute | Precision | Description |
|------|------|------|------|
| 1-2 | Pan | 16-bit | Horizontal rotation |
| 3-4 | Tilt | 16-bit | Vertical rotation |
| 5 | SpawnCount | 8-bit | Particle count |
| 6 | Red | 8-bit | Red |
| 7 | Green | 8-bit | Green |
| 8 | Blue | 8-bit | Blue |

---

## 7. Creating Custom Niagara Effects

SuperStageVFXActor can work with **any Niagara system**, requiring only that the Niagara system defines the following **user variables**:

### Required User Variables

| Variable Name | Type | Description |
|--------|------|------|
| **SpawnCount** | Int32 | Particle spawn count |
| **Color** | LinearColor | Particle color |

### Optional User Variables (Set at Initialization)

| Variable Name | Type | Description |
|--------|------|------|
| **Speed** | Float | Movement speed |
| **LoopTime** | Float | Loop time |
| **Minimum** | Float | Minimum range |
| **Maximum** | Float | Maximum range |

### Creation Steps

1. Create a new **Niagara System** in Content Browser
2. Add the above user variables to the system
3. Reference these variables in modules to control particle behavior
4. Assign the Niagara System asset to SuperStageVFXActor's Niagara component
5. Adjust MaxSpawnCount, MaxSpeed, and other default parameters
6. Configure DMX and test

---

## 8. FAQ

### Q: Particles not showing?
1. Confirm a **Niagara System asset** is assigned to the Niagara component
2. Confirm DMX SpawnCount value > 0
3. Confirm MaxSpawnCount is not 0
4. Check if the Niagara system correctly references the `SpawnCount` user variable

### Q: Color not changing?
Confirm the Niagara system defines a **LinearColor** type user variable named `Color`, and that the variable is used in particle render modules.

### Q: How to create a "one-touch trigger" effect (e.g., fireworks)?
- Set MaxSpawnCount to a small value (e.g., 1~3)
- Quickly push SpawnCount to max on the console, then pull back to 0
- The Niagara system should be configured as "play on emission" mode (non-looping)

### Q: Pan/Tilt direction control not working?
Confirm SuperLightControl (Pan/Tilt control functions) is called in the blueprint, and relevant DMX attributes are correctly configured. Pan/Tilt functionality is inherited from SuperLightBase.

### Q: How to use different Niagara systems for different effects?
Each SuperStageVFXActor's Niagara component can be **independently assigned different Niagara System assets**. Create multiple Actors, assigning different Niagara systems for smoke, flame, snow, etc.

---

> **Return to Overview**: [00 - DMX System Overview](/docs/stage-core/overview)
