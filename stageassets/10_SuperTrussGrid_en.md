# Super Truss Grid User Manual

## Purpose

Super Truss Grid generates a horizontal truss grid. The Actor calculates the X/Y grid counts from width, depth, and spacing, then creates main chords, secondary chords, diagonal braces, and node plates.

Use it for large lighting grids, exhibition hall overhead structures, and stage roof matrix layouts in visualization scenes.

## Main Parameters

| Parameter | Description |
| --- | --- |
| `GridWidthX` | Total grid width on X, in centimeters. |
| `GridDepthY` | Total grid depth on Y, in centimeters. |
| `GridSpacing` | Target grid spacing in centimeters. Actual spacing is recalculated from the final grid count. |
| `GridHeightZ` | Height between the upper and lower layers, in centimeters. |
| `TrussSize` | Truss size: S290, S400, or S520. |
| `BracePattern` | Brace pattern: Warren or Cross. |
| `DistributedLoad` | Distributed load used by the simplified statistics. |
| `PointLoadCount` | Number of point loads. |
| `WeightPerPoint` | Weight for each point load. |
| `bShowDiagonalBraces` | Shows diagonal braces. |
| `bShowNodePlates` | Shows node plates. |
| `bCentered` | Centers the grid around the Actor origin. |
| `ChordMaterial` | Material for chords. |
| `BraceMaterial` | Material for braces. |
| `PlateMaterial` | Material for node plates. |

## Statistics

The Actor updates these values from the current parameters:

- `GridCountX` and `GridCountY`: actual grid counts on X/Y.
- `GridStats.MainChords`: main chord count.
- `GridStats.SecondaryChords`: secondary chord count.
- `GridStats.DiagonalBraces`: diagonal brace count.
- `GridStats.NodePlates`: node plate count.
- `GridStats.TotalInstances`: total instance count.
- `GridStats.SelfWeight`, `TotalLoad`, and `MaxDeflection`: simplified estimate values.

## Notes

- The current component derives X/Y grid counts from dimensions and spacing instead of asking for direct span counts.
- Total point load is calculated from `PointLoadCount * WeightPerPoint`.
- Load and deflection values are quick estimates only. They do not replace structural engineering calculations or site approval.
