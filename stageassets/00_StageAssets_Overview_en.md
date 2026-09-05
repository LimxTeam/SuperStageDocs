# SuperStage — Stage Assets User Manual Overview

## 1. Introduction

**Stage Assets** is the stage asset module in the SuperStage plugin. It provides procedural stage equipment generation tools. Users can adjust parameters in the Unreal Engine editor to generate 3D models of stage structures and equipment for performance design pre-visualization.

Stage Assets are UE Actors that can be placed in a scene, moved, rotated, scaled, and configured through the Details panel. Most parameter changes trigger an editor rebuild or preview update; update time for complex assets and large scenes depends on project scale and machine performance.

> **Compatibility**: Use the UE version stated by the current branch and plugin descriptor.

---

## 2. Asset Overview

The Stage Assets module includes the following 19 available Actors (including the 7 programmatic objects added in 26H2.5).

> The content library no longer ships the former Band, AudioSystem, AircraftCase or console model. **Crowd characters and StageVFX effect assets are still shipped** (13 VAT characters, and Niagara systems plus matching fixture libraries for 7 effect categories) — see [Super Crowd](12_SuperCrowd_en.md) and [Stage VFX](../stagecore/14_Stage_VFX_en.md). The table below lists the procedurally generated structural Actors.

### 2.1 Structural Assets

| Actor | Name | Brief Description |
|-------|------|----------|
| **Super Truss Gantry** | [Truss Gantry](01_SuperTruss_en.md) | Procedural truss gantry, supporting multiple sections (Box/Triangle/Flat), sizes (290/400/520mm) and shapes (Goal Post/T-Shape/Portal/Double Span), with simplified load and deflection estimates |
| **Super Circular Truss** | [Circular Truss](08_SuperCircularTruss_en.md) | Polar-coordinate circular/ring truss, supporting dual-ring structure + radial braces + connection flanges |
| **Super Curved Truss** | [Curved Truss](09_SuperCurvedTruss_en.md) | Spline-driven free-form curved truss, with sections always perpendicular to the tangent direction |
| **Super Truss Grid** | [Truss Grid](10_SuperTrussGrid_en.md) | Horizontal double-layer grid truss for large-area lighting rigging systems |
| **Super Truss Tower** | [Truss Tower](11_SuperTrussTower_en.md) | Vertical truss column with simplified load statistics |
| **Super Scaffold** | [Straight Scaffold](02_SuperScaffold_en.md) | Procedural rectangular scaffold system, including vertical posts, horizontal ledgers, diagonal braces, base plates, deck panels and counterweights, with load and counterweight estimates |
| **Super Curved Scaffold** | [Curved Scaffold](03_SuperCurvedScaffold_en.md) | Spline-driven curved scaffold system that generates curved scaffold layouts from editable spline curves, with behavior close to the straight scaffold |
| **Super Stage Floor** | [Stage Floor](07_SuperStageFloor_en.md) | Procedural stage floor system, including panels/legs/cross braces/skirt/steps, with weight and load estimates |

### 2.2 Soft Furnishing Assets

| Actor | Name | Brief Description |
|-------|------|----------|
| **Super Drape** | [Stage Curtain](04_SuperDrape_en.md) | Procedural stage drape/curtain, supporting 6 drape types, 5 fabric types, 4 pleat patterns and 4 opening methods, with pipe and tie lines, built-in fabric weight calculation |
| **Super Crowd** | [Procedural Crowd](12_SuperCrowd_en.md) | Poisson disk sampling crowd generation, spline-defined area, multi-character weight distribution, terrain snapping; requires user-assigned character Static Meshes |

### 2.3 Video/Projection Assets

| Actor | Name | Brief Description |
|-------|------|----------|
| **Super Projector** | [Projector](05_SuperProjector_en.md) | Projection mapping simulation tool, single white spotlight light function projection, supporting static textures and director cameras, with keystone correction |
| **Super Screen** | [Media Screen](06_SuperScreen_en.md) | Media source display tool (static texture/director camera), supporting multi-screen sync, transparency mode and color adjustment |

### 2.4 Stage Programmatic Objects (added in 26H2.5)

| Actor | Name | Summary |
|-------|------|----------|
| **Super Barrier** | [Barrier](13_StageProgramObjects_en.md#1-super-barrier) | Spline-driven barrier with front-of-stage, crowd-control and site-fence forms; can conform to terrain |
| **Super Stage Roof** | [Stage Roof](13_StageProgramObjects_en.md#2-super-stage-roof) | Flat, gable and arch roof profiles, with corner towers, roof trusses, skin, PA wings and a backdrop frame |
| **Super Grandstand** | [Grandstand](13_StageProgramObjects_en.md#3-super-grandstand) | Tiered grandstand with automatic aisle splitting and sightline clearance readout |
| **Super Stair Tower** | [Stair Tower](13_StageProgramObjects_en.md#4-super-stair-tower) | Single-flight or double-return, showing step and slope figures |
| **Super Trackway** | [Trackway](13_StageProgramObjects_en.md#5-super-trackway) | Spline-driven ground panels, multi-lane with optional staggered joints; can conform to terrain |
| **Super Cable Run** | [Cable Run](13_StageProgramObjects_en.md#6-super-cable-run) | Suspended sagging spans or ground cable ramps |
| **Super Ballast** | [Ballast](13_StageProgramObjects_en.md#7-super-ballast) | Ballast layout that reports the mass required for an entered overturning moment |


---

## 3. General Operation Guide

### 3.1 Placing Actors

All Stage Asset Actors can be placed in the scene through the following methods:

1. **Place Actor Panel** — Search for the Actor name in the "Place Actor" panel on the left side of the editor and drag into the viewport
2. **Content Browser** — Find the corresponding asset type in the Content Browser and drag into the viewport
3. **Right-Click Menu** — Right-click in the viewport → Place Actor → Search for the corresponding name

### 3.2 Adjusting Parameters

1. **Select** the Actor in the viewport
2. View all adjustable parameters in the **Details Panel** on the right
3. The model in the scene usually updates in the editor after modifying parameters
4. Parameters are grouped by function (Structure, Materials, Visualization, Statistics, etc.), and groups can be collapsed/expanded

### 3.3 Material Assignment

All Stage Assets support assigning separate materials for different components:

1. Find the "Materials" group in the Details panel
2. Click the dropdown arrow next to the material property
3. Select a material asset from the project
4. Leave empty to use the UE engine default material

### 3.4 Statistics

Some structural assets and drape assets provide read-only statistics panels:

- **Component Count** — Instance counts and totals for each component type
- **Weight/load statistics** — Self-weight, load, counterweight and other estimates based on built-in parameters
- **Physical Parameters** — Detailed physical parameters of the currently selected material/fabric

> ⚠️ **All statistics are for pre-visualization reference only and cannot replace professional engineering calculations.**

---

## 4. Reference Parameters

The Stage Assets module uses the following industry references for parameters and default values. They support pre-visualization statistics and do not mean the plugin produces engineering certification.

| Standard | Applicable Assets | Description |
|------|----------|------|
| **DIN 4113** | Super Truss Series | Reference source for truss material and deflection estimates |
| **EN 12811-1** | Super Scaffold, Super Curved Scaffold | Reference source for scaffold load classes |
| **EN 10210** | Super Scaffold, Super Curved Scaffold | Hot-finished structural hollow section standard, used as a tube wall thickness reference |
| **BS 1139** | Super Scaffold | British scaffold tube standard, defining Ø48.3mm standard tube diameter |
| **ANSI E1.21** | Super Stage Floor | Reference source for temporary floor/stage load estimates |

---

## 5. Scene Complexity Considerations

### 5.1 Instanced Rendering

Structural assets (truss, scaffold) use **Instanced Static Mesh Component (ISM)** technology for rendering:

- **Purpose**: Components of the same type can share instanced rendering, reducing overhead from repeated meshes
- **Note**: Total component count exceeding 10,000 may still affect editor frame rate

### 5.2 Procedural Mesh

Drape assets use **Procedural Mesh Component** for rendering:

- **Purpose**: Generates pleated surfaces from the current drape size, pleat type, and subdivision parameters
- **Note**: Too many pleats (>100) or enabling double-sided rendering will increase triangle count

### 5.3 Recommendations

- Scaffolds in the distance can appropriately reduce span count and layer count
- Turn off double-sided rendering for drape faces that are not visible
- Avoid placing too many high-complexity Stage Assets simultaneously in large scenes

---

## 6. Documentation Directory

| No. | Document Name | Content |
|------|----------|------|
| 00 | [Overview](00_StageAssets_Overview_en.md) | This document. Stage Assets module overview, general operation guide |
| 01 | [Truss Gantry](01_SuperTruss_en.md) | Super Truss Gantry user notes |
| 02 | [Straight Scaffold](02_SuperScaffold_en.md) | Super Scaffold user notes |
| 03 | [Curved Scaffold](03_SuperCurvedScaffold_en.md) | Super Curved Scaffold user notes |
| 04 | [Stage Curtain](04_SuperDrape_en.md) | Super Drape user notes |
| 05 | [Projector](05_SuperProjector_en.md) | Super Projector user notes |
| 06 | [Media Screen](06_SuperScreen_en.md) | Super Screen user notes |
| 07 | [Stage Floor](07_SuperStageFloor_en.md) | Super Stage Floor user notes |
| 08 | [Circular Truss](08_SuperCircularTruss_en.md) | Super Circular Truss user notes |
| 09 | [Curved Truss](09_SuperCurvedTruss_en.md) | Super Curved Truss user notes |
| 10 | [Truss Grid](10_SuperTrussGrid_en.md) | Super Truss Grid user notes |
| 11 | [Truss Tower](11_SuperTrussTower_en.md) | Super Truss Tower user notes |
| 12 | [Procedural Crowd](12_SuperCrowd_en.md) | Super Crowd user notes |
| 13 | [Stage Programmatic Objects](13_StageProgramObjects_en.md) | Barrier / roof / grandstand / stair tower / trackway / cable run / ballast (added in 26H2.5) |

---

## 7. Version Information

- **Plugin Name**: SuperStage
- **Module**: Stage Assets
- **Compatible Engine**: Use the current branch and plugin descriptor as the source of truth
- **Document Version**: 2.0
- **Last Updated**: 2026-04
- **Development Team**: LimxTeam
