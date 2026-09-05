# Stage Programmatic Objects

> Applies to SuperStage 26H2.5 and later ｜ Read first: [Stage Assets Overview](00_StageAssets_Overview_en.md)

This document covers the seven programmatic objects added in 26H2.5. Like the trusses and scaffolds, they are parametrically generated and rebuild when a parameter changes.

To place one: the **StageModel** category in the asset browser, or search by name in the Place Actors panel.

| Object | In one line | Driven by |
| --- | --- | --- |
| [Super Barrier](#1-super-barrier) | Crowd barrier / crowd-control fence / site fence | Spline |
| [Super Stage Roof](#2-super-stage-roof) | Stage roof system | Parameters |
| [Super Grandstand](#3-super-grandstand) | Tiered grandstand | Parameters |
| [Super Stair Tower](#4-super-stair-tower) | Stair tower | Parameters |
| [Super Trackway](#5-super-trackway) | Ground panels / trackway | Spline |
| [Super Cable Run](#6-super-cable-run) | Cable routing | Spline |
| [Super Ballast](#7-super-ballast) | Ballast layout | Parameters |

> ⚠️ As with the other structural assets: the weight, load, ballast and sightline figures these objects report are **reference estimates from simplified models**, for order-of-magnitude reference and statistics during previs. **They are not structural-safety conclusions and cannot replace structural design, mechanical verification or construction safety approval.**

---

## 1. Super Barrier

Spline-driven. Lays a run of barrier panels along the spline.

| Group | Parameters |
| --- | --- |
| **Type** | Barrier Type (front-of-stage / crowd-control / site fence), Infill Mode |
| **Dimensions** | Panel Width, Panel Height, Foot Depth, Panel Gap, Post Diameter, Rail Diameter, Infill Bar Count, Mesh Wire Rows |
| **Layout** | Foot Side, Lateral Offset, Start Offset, Gap Panel Indices |
| **Ground Snap** | Snap to Ground, Ground Collision Channel, Raycast Start Height, Raycast Depth, Default Ground Height, Ground Offset, Align To Ground Normal |
| **Visibility** | Show Posts / Rails / Infill / Feet / Braces |
| **Material** | Frame / Infill / Foot |

**Key points**:

- `Gap Panel Indices` opens gateways — name the panel indices to leave out, rather than cutting the spline;
- `Foot Side` decides whether the feet face the audience or the stage, which matters on a front-of-stage barrier;
- With `Snap to Ground` on, the barrier follows the terrain; add `Align To Ground Normal` to let the panels lean with the slope.

---

## 2. Super Stage Roof

Parameter-driven. Generates a stage roof system with corner towers.

| Group | Parameters |
| --- | --- |
| **Main** | Roof Shape (flat / gable / arch), Span, Depth, Clear Height, Roof Rise |
| **Profile** | Tower Section, Truss Size, Brace Pattern, Rib Count, Rib Segments, Tower Bays |
| **Extras** | Show PA Wings, PA Wing Reach / Depth / Taper / Hang Points, Show Backdrop Frame, Backdrop Mullion Count |
| **Skin** | Show Roof Skin, Skin Thickness, Skin Overhang |
| **Load** | Suspended Load, Design Wind Speed |
| **Visibility** | Show Towers / Purlins / Base Plates |
| **Material** | Chord / Brace / Skin / Base Plate |

**Key points**:

- `Clear Height` is the clear height from the ground to the underside of the roof; `Roof Rise` is the rise of the roof itself. They are separate;
- **PA Wings** are the side wings that carry the sound system; `PA Wing Hang Points` gives the number of hang points;
- `Design Wind Speed` feeds the statistics only. **It is not a wind-load certification.**

---

## 3. Super Grandstand

Parameter-driven. Tiered seating with automatic aisle splitting and a sightline readout.

| Group | Parameters |
| --- | --- |
| **Seating** | Row Count, Seats Per Row, Seat Width, Row Depth, Row Rise |
| **Aisle** | Enable Aisles, Seats Per Block, Aisle Width |
| **Sightline** | Focus Distance, Eye Height |
| **Structure** | Deck Thickness, Post Diameter, Rail Diameter, Post Every N Rows, Post Spacing X |
| **Visibility** | Show Riser Boards, Show Railing, Railing Height, Show Front Guardrail, Show Seat Markers, Show Support Frame |
| **Material** | Deck / Tube / Seat Marker |

**Key points**:

- With `Enable Aisles` on, each row is split into blocks of `Seats Per Block` with an `Aisle Width` gangway between them;
- The **sightline clearance** is computed from `Focus Distance` (the point on stage being watched) and `Eye Height` (seated eye height). It is a **previs reference**, not a compliance judgement against a sightline design code;
- `Row Rise` is the step-up per row and directly decides whether the back rows can see over the front.

---

## 4. Super Stair Tower

Parameter-driven. Single-flight or double-return.

| Group | Parameters |
| --- | --- |
| **Main** | Flight Mode (single / double-return), Total Height, Target Riser, Going, Steps Per Flight |
| **Dimensions** | Flight Width, Lane Gap, Platform Depth, Tread Thickness, Post Diameter, Rail Diameter, Ledger Spacing |
| **Railing** | Show Railing, Railing Height, Mid Rail Count |
| **Visibility** | Show Diagonals, Show Base Plates |
| **Material** | Tube / Deck / Base Plate |

**Key points**:

- `Target Riser` is a **target**, not the final value — the total height is rounded to a whole number of steps, so the actual riser differs slightly. The panel shows the resulting step and slope figures;
- `Going` is the tread depth. The riser-to-going ratio decides whether the stair is comfortable;
- In double-return mode `Lane Gap` is the gap between the two flights and `Platform Depth` is the depth of the turn landing.

---

## 5. Super Trackway

Spline-driven. Lays ground panels along the spline, several lanes side by side.

| Group | Parameters |
| --- | --- |
| **Spec** | Panel Type |
| **Dimensions** | Panel Length, Panel Width, Panel Thickness, Lane Count, Panel Gap, Lane Gap |
| **Layout** | Stagger Joints, Lateral Offset, Start Offset |
| **Ground Snap** | Same set as Barrier |
| **Material** | Panel |

**Key points**:

- With `Stagger Joints` on, adjacent lanes are offset so the joints do not line up in a straight run;
- `Lane Count` is how many lanes side by side; `Lane Gap` is the gap between them;
- Can conform to terrain.

---

## 6. Super Cable Run

Spline-driven. Two laying modes.

| Group | Parameters |
| --- | --- |
| **Route** | Route Mode (suspended sag / ground ramp), Support Spacing, Sag Ratio, Segments Per Span |
| **Cable** | Cable Count, Cable Diameter, Cable Spacing, Weight Per Meter, Vary Sag Per Cable |
| **Support** | Show Supports, Support Size, Ramp Width, Ramp Height, Ramp Section Length |
| **Material** | Cable / Support |

**Key points**:

- In **suspended mode**, `Sag Ratio` controls the droop and `Support Spacing` the distance between supports — together they decide how the cable hangs;
- With `Vary Sag Per Cable` on, each cable droops slightly differently, which reads more naturally than a uniform bundle;
- In **ground mode** the cables are covered by ramps; `Ramp Width / Height / Section Length` are the ramp dimensions;
- `Weight Per Meter` feeds the weight statistics only.

---

## 7. Super Ballast

Parameter-driven. Lays out ballast blocks, and can work back from an entered overturning moment to the mass required.

| Group | Parameters |
| --- | --- |
| **Spec** | Block Type, Layout |
| **Layout** | Count X, Count Y, Layer Count, Block Gap, Centered |
| **Dimensions** | Block Length, Block Width, Block Height, Block Weight |
| **Requirement** | Compute Required Mass, Overturning Moment, Lever Arm, Safety Factor |
| **Material** | Block |

**Key points**:

- With `Compute Required Mass` on, enter the **Overturning Moment** (kN·m), **Lever Arm** (m) and **Safety Factor**, and the panel reports the mass required;
- That figure is an **order-of-magnitude reference from a simplified formula**. ⚠️ **It is not an overturning-resistance verification and must not be used as the basis for an on-site ballast plan.** Real overturning resistance must be calculated by a qualified structural engineer for the actual load cases;
- `Layer Count` is how many layers to stack; `Centered` decides whether the array is centred on the Actor origin or extends from it.

---

## 8. General Notes

**Spline editing**: Barrier, Trackway and Cable Run are spline-driven. Select the Actor and drag the spline control points in the viewport; right-click a point to add or remove one.

**Ground conformity**: Barrier and Trackway support `Snap to Ground`. It traces downward on a collision channel, so the terrain must have collision.

**Statistics**: each object provides read-only part counts and weight estimates. The data is for previsualisation reference only.

**Scene complexity**: these objects all render with instanced static meshes. Pushed to large values (several hundred panels across several lanes, for example) the part count grows quickly — confirm the layout at a smaller scale first.

---

## 9. Related Documents

- [Stage Assets Overview](00_StageAssets_Overview_en.md)
- [Super Truss](01_SuperTruss_en.md)
- [Super Scaffold](02_SuperScaffold_en.md)
- [Super Stage Floor](07_SuperStageFloor_en.md)
- [Super Crowd](12_SuperCrowd_en.md)
