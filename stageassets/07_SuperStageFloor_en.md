# Super Stage Floor — User Manual

## 1. Overview

**Super Stage Floor** is a procedural stage floor generation tool provided by the SuperStage plugin. It can **instantly generate** a complete stage floor system based on parameters such as panel count, specifications, and height, including panels, legs, cross braces, skirt trim, and steps.

### Core Features

- **Parametric layout** — Control stage size via X/Y direction panel count and gap
- **Multiple panel specifications** — Standard 4'×8', Large 4'×12', Compact StageDex three sizes
- **Multiple surface types** — Plywood, Hardwood, Sprung Dance Floor, Industrial Steel Deck
- **Leg types** — Fixed Height, Telescopic, Folding
- **Skirt trim** — None, Full, Front Only, Three Sides
- **Auto steps** — Code-compliant steps automatically graded at 18cm riser height
- **Load calculation** — Weight and load-bearing capacity calculation based on ANSI E1.21 standard

### Use Cases

- Performance stage construction pre-visualization
- Event stage layout planning
- Stage load-bearing scheme assessment
- Stage step position and dimension planning

---

## 2. How to Add to Scene

1. Find **Super Stage Floor** in the SuperBrowser asset browser and drag into the viewport
2. Or search for **"Super Stage Floor"** in the "Place Actor" panel
3. Select the Actor and adjust parameters in the Details panel

---

## 3. Parameter Reference

### 3.1 Layout

| Parameter | Description | Default | Range |
|------|------|--------|------|
| **PanelCountX** | Number of panels in X direction | 4 | 1 ~ 50 |
| **PanelCountY** | Number of panels in Y direction | 3 | 1 ~ 50 |
| **LayoutMode** | Layout mode | Grid | Grid / Staggered |
| **Centered** | Grid centered on Actor origin | true | — |
| **PanelGap (cm)** | Installation gap between panels | 0.2 | 0 ~ 5 |

#### Layout Mode

| Mode | Description |
|------|------|
| **Grid** | Panels aligned in rows, standard layout |
| **Staggered** | Every other row offset half a panel, more stable joints |

### 3.2 Dimensions

| Parameter | Description | Default | Range |
|------|------|--------|------|
| **PanelSize** | Panel specification | Standard | — |
| **SurfaceType** | Surface material type | Plywood | — |
| **PlatformHeight (cm)** | Platform height (ground to panel top surface) | 40.0 | 10 ~ 300 |
| **LegType** | Leg type | Telescopic | — |

#### Panel Specifications

| Spec | Dimensions | Reference Standard |
|------|------|----------|
| **Standard** | 122×244cm (4'×8') | Most commonly used size |
| **Large** | 122×366cm (4'×12') | Quick setup for large areas |
| **Compact** | 100×200cm (StageDex) | European standard |

#### Surface Material Types

| Type | Description |
|------|------|
| **Plywood** | 18mm birch plywood, most commonly used in the performance industry |
| **Hardwood** | 19mm maple/oak solid wood flooring, theaters/concert halls |
| **Sprung Dance Floor** | 25mm sprung composite layer, specifically for dance/musicals |
| **Steel Deck** | 3mm checkered steel plate, outdoor/heavy-duty |

#### Leg Types

| Type | Description |
|------|------|
| **Fixed Height** | Welded fixed, most stable |
| **Telescopic** | Manual/pneumatic adjustment, most commonly used |
| **Folding** | Convenient for transport/storage |

### 3.3 Visibility

| Parameter | Description | Default |
|------|------|--------|
| **ShowLegs** | Display legs | true |
| **ShowCrossBraces** | Display cross braces | true |
| **SkirtType** | Skirt type | None |
| **ShowStairs** | Display steps | false |

#### Skirt Types

| Type | Description |
|------|------|
| **None** | No skirt |
| **Full** | All four sides |
| **Front Only** | Front face only (audience side) |
| **Three Sides** | Front + two sides |

### 3.4 Stairs

Stair parameters only take effect when `ShowStairs = true`. Step riser height is fixed at **18cm** (building code standard), and the system automatically calculates the number of steps based on the platform height.

| Parameter | Description | Default | Range |
|------|------|--------|------|
| **StairWidth (cm)** | Step width | 120 | 60 ~ 500 |
| **StairOffsetY (cm)** | Step position offset (0 = front center) | 0 | — |

> **Example**: Platform height 72cm → auto-generate 4 steps (72 ÷ 18 = 4)

### 3.5 Materials

| Parameter | Description |
|------|------|
| **PanelMaterial** | Panel top surface material |
| **LegMaterial** | Leg/cross brace material |
| **SkirtMaterial** | Skirt panel material |
| **StairMaterial** | Step material |

> Leave empty to use UE engine default materials.

### 3.6 Load

| Parameter | Description | Default | Range |
|------|------|--------|------|
| **AdditionalLoad (kg)** | Additional load (equipment, personnel, etc.) | 0 | 0 ~ 100,000 |

---

## 4. Statistics (Read-Only)

### Component Count

| Field | Description |
|------|------|
| **Panels** | Number of panels |
| **Legs** | Number of legs |
| **CrossBraces** | Number of cross braces |
| **SkirtPanels** | Number of skirt panels |
| **StairTreads** | Number of stair treads |
| **StairLegs** | Number of stair legs |
| **TotalParts** | Total component count |

### Weight/Load

Weight and load data calculated based on **ANSI E1.21** standard; data is for reference only.

| Field | Description |
|------|------|
| **TotalWeight (kg)** | Total self-weight |
| **MaxLoadCapacity (kN/m²)** | Maximum uniformly distributed load |
| **MaxPointLoad (kN)** | Maximum concentrated load |

---

## 5. Industry Standards

| Standard | Description |
|------|------|
| **ANSI E1.21** | American entertainment technology standard — temporary floor/stage |
| **ISO 60** | International standard panel dimensions |
| **Wenger / StageRight / Prolyte StageDex** | Panel specification reference series |

---

## 6. Performance

Super Stage Floor uses 6 **Instanced Static Mesh Components (ISM)** to render all components:

| ISM Component | Components |
|----------|------|
| PanelISM | Panels |
| LegISM | Legs |
| CrossBraceISM | Cross braces |
| SkirtISM | Skirts |
| StairTreadISM | Stair treads |
| StairLegISM | Stair legs |

All components of the same type share a single Draw Call for efficient performance. Rebuild only triggers when the configuration hash changes.

---

## 7. Quick Configuration Examples

### Standard Performance Stage

- PanelCountX: 6, PanelCountY: 4
- PanelSize: Standard (122×244cm)
- PlatformHeight: 60cm
- SkirtType: Front Only
- ShowStairs: true, StairWidth: 150cm

### Large Music Festival Stage

- PanelCountX: 16, PanelCountY: 8
- PanelSize: Large (122×366cm)
- SurfaceType: Steel Deck
- PlatformHeight: 120cm
- SkirtType: Three Sides
- ShowStairs: true, StairWidth: 300cm

### Dance Rehearsal Floor (Low Platform)

- PanelCountX: 8, PanelCountY: 6
- SurfaceType: Sprung Dance Floor
- PlatformHeight: 15cm
- ShowLegs: false (no legs needed for low platform)
- SkirtType: None
