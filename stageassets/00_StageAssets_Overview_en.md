# SuperStage — Stage Assets User Manual Overview

## 1. Introduction

**Stage Assets** is one of the core modules of the SuperStage plugin, providing a set of **procedural stage equipment generation tools**. These tools allow you to **instantly generate** 3D models of various stage structures and equipment in the Unreal Engine editor through simple parameter adjustments, for pre-visualization (Pre-Visualization) of performance design.

All Stage Assets are standard UE Actors that can be dragged directly into the scene, moved, rotated, scaled, and configured through parameters in the Details panel. Models are **rebuilt and previewed instantly** after parameter changes, without waiting for compilation or manual refresh.

> **Compatibility**: The Stage Assets module supports **Unreal Engine 5.7 ~ 5.8**.

---

## 2. Asset Overview

The Stage Assets module includes the following 12 available Actors:

### 2.1 Structural Assets

| Actor | Name | Brief Description |
|-------|------|----------|
| **Super Truss Gantry** | [Truss Gantry](01_SuperTruss.md) | Procedural truss gantry, supporting multiple sections (Box/Triangle/Flat), sizes (290/400/520mm) and shapes (Goal Post/T-Shape/Portal/Double Span), with built-in DIN 4113 load calculation |
| **Super Circular Truss** | [Circular Truss](08_SuperCircularTruss.md) | Polar-coordinate circular/ring truss, supporting dual-ring structure + radial braces + connection flanges |
| **Super Curved Truss** | [Curved Truss](09_SuperCurvedTruss.md) | Spline-driven free-form curved truss, with sections always perpendicular to the tangent direction |
| **Super Truss Grid** | [Truss Grid](10_SuperTrussGrid.md) | Horizontal double-layer grid truss for large-area lighting rigging systems |
| **Super Truss Tower** | [Truss Tower](11_SuperTrussTower.md) | Vertical truss column, with Euler critical load calculation |
| **Super Scaffold** | [Straight Scaffold](02_SuperScaffold.md) | Procedural rectangular scaffold system, including vertical posts, horizontal ledgers, diagonal braces, base plates, deck panels and counterweights, with built-in EN 12811 load and counterweight calculation |
| **Super Curved Scaffold** | [Curved Scaffold](03_SuperCurvedScaffold.md) | Spline-driven curved scaffold system, achieving any curvature through editable spline curves, with all other features identical to straight scaffold |
| **Super Stage Floor** | [Stage Floor](07_SuperStageFloor.md) | Procedural stage floor system, including panels/legs/cross braces/skirt/steps, with ANSI E1.21 load-bearing calculation |

### 2.2 Soft Furnishing Assets

| Actor | Name | Brief Description |
|-------|------|----------|
| **Super Drape** | [Stage Curtain](04_SuperDrape.md) | Procedural stage drape/curtain, supporting 6 drape types, 5 fabric types, 4 pleat patterns and 4 opening methods, with pipe and tie lines, built-in fabric weight calculation |
| **Super Crowd** | [Procedural Crowd](12_SuperCrowd.md) | Poisson disk sampling crowd generation, spline-defined area, multi-character weight distribution, terrain snapping |

### 2.3 Video/Projection Assets

| Actor | Name | Brief Description |
|-------|------|----------|
| **Super Projector** | [Projector](05_SuperProjector.md) | Projection mapping simulation tool, single white spotlight light function projection, supporting NDI video stream and static texture, with keystone correction |
| **Super Screen** | [Media Screen](06_SuperScreen.md) | Media source display tool (NDI video stream/static texture), supporting multi-screen sync, transparency mode and color adjustment |

---

## 3. General Operation Guide

### 3.1 Placing Actors

All Stage Asset Actors can be placed in the scene through the following methods:

1. **Place Actor Panel** — Search for the Actor name in the "Place Actor" panel on the left side of the editor and drag into the viewport
2. **Content Browser** — Find the corresponding blueprint/class in the Content Browser and drag into the viewport
3. **Right-Click Menu** — Right-click in the viewport → Place Actor → Search for the corresponding name

### 3.2 Adjusting Parameters

1. **Select** the Actor in the viewport
2. View all adjustable parameters in the **Details Panel** on the right
3. The model in the scene **updates immediately** after modifying parameters
4. Parameters are grouped by function (Structure, Materials, Visualization, Statistics, etc.), and groups can be collapsed/expanded

### 3.3 Material Assignment

All Stage Assets support assigning separate materials for different components:

1. Find the "Materials" group in the Details panel
2. Click the dropdown arrow next to the material property
3. Select a material asset from the project
4. Leave empty to use the UE engine default material

### 3.4 Statistics

Structural assets (truss, scaffold) and drape assets all provide read-only statistics panels:

- **Component Count** — Instance counts and totals for each component type
- **Weight Calculation** — Self-weight, load, counterweight and other calculations based on industry standards
- **Physical Parameters** — Detailed physical parameters of the currently selected material/fabric

> ⚠️ **All statistics are for pre-visualization reference only and cannot replace professional engineering calculations.**

---

## 4. Industry Standards Reference

The Stage Assets module references the following industry standards in its design:

| Standard | Applicable Assets | Description |
|------|----------|------|
| **DIN 4113** | Super Truss Series | German aluminum alloy structural design standard, used for truss load and deflection calculation |
| **EN 12811-1** | Super Scaffold, Super Curved Scaffold | European scaffold standard, used for load class, counterweight and safety factor calculation |
| **EN 10210** | Super Scaffold, Super Curved Scaffold | Hot-finished structural hollow section standard, providing steel tube wall thickness parameters |
| **BS 1139** | Super Scaffold | British scaffold tube standard, defining Ø48.3mm standard tube diameter |
| **ANSI E1.21** | Super Stage Floor | American entertainment technology standard — temporary floor/stage load-bearing calculation |

---

## 5. Performance Considerations

### 5.1 Instanced Rendering

Structural assets (truss, scaffold) use **Instanced Static Mesh Component (ISM)** technology for rendering:

- **Advantage**: Components of the same type share a single Draw Call, maintaining good performance even with thousands of tubes
- **Note**: Total component count exceeding 10,000 may still affect editor frame rate

### 5.2 Procedural Mesh

Drape assets use **Procedural Mesh Component** for rendering:

- **Advantage**: Can generate arbitrarily shaped pleated surfaces
- **Note**: Too many pleats (>100) or enabling double-sided rendering will increase triangle count

### 5.3 Recommendations

- Scaffolds in the distance can appropriately reduce span count and layer count
- Turn off double-sided rendering for drape faces that are not visible
- Avoid placing too many high-complexity Stage Assets simultaneously in large scenes

---

## 6. Documentation Directory

| No. | Document Name | Content |
|------|----------|------|
| 00 | [Overview](00_StageAssets_Overview.md) | This document. Stage Assets module overview, general operation guide |
| 01 | [Truss Gantry](01_SuperTruss.md) | Complete user manual for Super Truss Gantry |
| 02 | [Straight Scaffold](02_SuperScaffold.md) | Complete user manual for Super Scaffold |
| 03 | [Curved Scaffold](03_SuperCurvedScaffold.md) | Complete user manual for Super Curved Scaffold |
| 04 | [Stage Curtain](04_SuperDrape.md) | Complete user manual for Super Drape |
| 05 | [Projector](05_SuperProjector.md) | Complete user manual for Super Projector |
| 06 | [Media Screen](06_SuperScreen.md) | Complete user manual for Super Screen |
| 07 | [Stage Floor](07_SuperStageFloor.md) | Complete user manual for Super Stage Floor |
| 08 | [Circular Truss](08_SuperCircularTruss.md) | Complete user manual for Super Circular Truss |
| 09 | [Curved Truss](09_SuperCurvedTruss.md) | Complete user manual for Super Curved Truss |
| 10 | [Truss Grid](10_SuperTrussGrid.md) | Complete user manual for Super Truss Grid |
| 11 | [Truss Tower](11_SuperTrussTower.md) | Complete user manual for Super Truss Tower |
| 12 | [Procedural Crowd](12_SuperCrowd.md) | Complete user manual for Super Crowd |

---

## 7. Version Information

- **Plugin Name**: SuperStage
- **Module**: Stage Assets
- **Compatible Engine**: Unreal Engine 5.7 ~ 5.8
- **Document Version**: 2.0
- **Last Updated**: 2026-04
- **Development Team**: LimxTeam
