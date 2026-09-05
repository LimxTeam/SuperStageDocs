# Super Crowd User Manual

## Purpose

Super Crowd generates static crowd instances inside a closed spline area. It creates points by target count or target density, assigns Static Mesh characters by type weight, and can optionally snap placements to the ground.

Use it for audience visualization, event crowd density previews, and front-of-stage placeholder crowds.

The plugin ships **13 VAT crowd characters**, each with two animation material sets (Anim_V1 / Anim_V2) and base plus emissive layers; the character meshes carry LODs. Two placeable objects correspond to them — **Super Crowd V1** and **Super Crowd V2** — differing only in which animation material set is applied. Both fill `CharacterTypes` with those 13 characters automatically when it is empty.

You can also assign your own licensed Static Meshes and materials in `CharacterTypes`; anything you set there is not overwritten by the automatic fill.

## Basic Usage

1. Place a Super Crowd Actor in the level.
2. Edit the Actor spline so it encloses an area.
3. Add at least one enabled entry in `CharacterTypes` and assign a Static Mesh.
4. Set `TargetCount`, or enable `bUseDensityMode` and set `TargetDensity`.
5. Enable ground snapping, random rotation, and scale options as needed.

If there is no valid character mesh, the Actor will not generate visible crowd instances.

## Character Types

| Field | Description |
| --- | --- |
| `Mesh` | Character Static Mesh. |
| `MaterialOverride` | Optional material override. |
| `Weight` | Random selection weight. Higher values are more likely to be selected. |
| `Scale` | Base scale. |
| `ScaleVariation` | Random scale variation. |
| `FootOffsetZ` | Foot height offset. |
| `BaseRotationOffset` | Base rotation offset. |
| `DisplayName` | Display name in the editor. |
| `bEnabled` | Whether this type participates in generation. |
| `bCastShadow` | Whether this type casts shadow. |

## Generation Parameters

| Parameter | Description |
| --- | --- |
| `TargetCount` | Target number of people. Actual count may be lower. |
| `SafetyRadius` | Safety radius between characters, in centimeters. |
| `BoundaryPadding` | Empty space kept from the spline boundary. |
| `RandomSeed` | Random seed. The same settings can reproduce the same distribution. |
| `bUseDensityMode` | Uses density mode. When enabled, target count is calculated from area and `TargetDensity`. |
| `TargetDensity` | Target density in people per square meter. |
| `PoissonAttemptsPerPoint` | Number of attempts per sample point. |
| `GroundOversampleMultiplier` | Oversampling multiplier when ground checks are active. |
| `bRelaxSpacingWhenCrowded` | Allows the safety radius to relax when the target is too dense. |
| `MinRelaxedSpacingScale` | Minimum scale that the safety radius may relax to. |
| `SpacingRelaxationPasses` | Number of spacing relaxation passes. |
| `SplineSampleSpacing` | Spline sampling spacing in centimeters. |

## Ground And Appearance Parameters

| Parameter | Description |
| --- | --- |
| `bSnapToGround` | Enables downward ground raycasts. |
| `GroundChannel` | Collision channel used for ground checks. |
| `RaycastStartHeight` | Raycast start height. |
| `RaycastDepth` | Downward raycast depth. |
| `DefaultGroundHeight` | Default height used when not snapped to ground. |
| `GroundOffset` | Height offset after ground placement. |
| `bAlignToGroundNormal` | Aligns placement to the ground normal. |
| `bRejectSteepGround` | Rejects ground hits that are too steep. |
| `MaxGroundSlopeDegrees` | Maximum allowed ground slope. |
| `bFlattenToSplinePlane` | Flattens placements back to the spline plane. |
| `bRandomYaw` (Enable Yaw Variation) | Enables yaw variation — a controlled offset either side of the base orientation. |
| `GlobalMaxRotationVariation` (Yaw Variation Range) | Yaw variation range, 0–180 degrees, default 0. |
| `bFaceSplineCenter` (Face Spline Center) | Face the centre of the spline area. Off by default. |
| `bFaceAwayFromCenter` (Face Away From Center) | Face away from the centre. Off by default. |
| `GlobalScaleFactor` (Global Scale Factor) | Global scale, 0.1–10, default 1.0. |
| `GlobalScaleVariation` (Global Scale Variation) | Random spread on the global scale, 0–0.5, default 0. |
| `bCastShadows` (Cast Shadows) | Casts shadows. |
| `bEnableInstanceCollision` (Enable Instance Collision) | Enables instance collision. |
| `OverrideMaterial` (Override Material) | Material override. |
| `bOverrideAllMaterialSlots` (Override All Material Slots) | Override every material slot. |
| `bShowSpline` (Show Spline) | Show the spline. |
| `bDebugShowSafetyRadius` (Debug Show Safety Radius) | Debug-draw the safety radius. |

## Statistics

The Actor updates:

- `ActualCount`: actual generated count.
- `NormalizedWeights`: normalized character weights.
- `CrowdStats.TotalCount`, `RequestedCount`, and `CountPerType`: count statistics.
- `CrowdStats.ValidCharacterTypes`: number of valid character types.
- `CrowdStats.AreaM2` and `Density`: area and actual density.
- `CrowdStats.GroundSnappedCount`: count successfully snapped to ground.
- `CrowdStats.FailedPlacements` and `PlacementAttempts`: failed placement and attempt counts.
- `CrowdStats.RejectedByBoundary`, `RejectedByGround`, and `RejectedBySlope`: rejected placement counts.
- `CrowdStats.EffectiveSafetyRadius`, `AverageSpacing`, and `MinimumSpacing`: final spacing statistics.

## Notes

- The spline must enclose a valid area. Otherwise area and distribution results may be invalid.
- The target count is not guaranteed. Small areas, large safety radius, or strict ground filtering can reduce the result.
- Large instance counts affect editor performance. Start with smaller values while checking the area and character setup.
