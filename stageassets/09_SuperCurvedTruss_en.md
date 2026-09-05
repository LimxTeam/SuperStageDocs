# Super Curved Truss User Manual

## Purpose

Super Curved Truss generates truss along a spline path. After selecting the Actor, edit the spline points in the viewport and the truss follows the updated path.

Use it for free-form lighting rigs, wave-shaped suspended structures, and non-standard truss paths in visualization scenes.

## Main Parameters

| Parameter | Description |
| --- | --- |
| `SectionType` | Section type: Box, Triangle, or Flat. |
| `TrussSize` | Truss size: S290, S400, or S520. |
| `BracePattern` | Brace pattern: Warren or Cross. |
| `SpanCountAlongSpline` | Number of spans along the spline. Higher values create finer path segmentation. |
| `SectionRotation` | Rotates the section around the spline direction. |
| `SuspendedLoad` | Suspended load used by the simplified statistics. |
| `DistributedLoad` | Distributed load used by the simplified statistics. |
| `bShowHorizontalBraces` | Shows horizontal braces. |
| `bShowEndPlates` | Shows end plates at both ends. |
| `ChordMaterial` | Material for chord members. |
| `BraceMaterial` | Material for brace members. |
| `EndPlateMaterial` | Material for end plates. |

## Spline Editing

- Select the Actor and move spline control points in the viewport to change the truss path.
- Add more control points for more complex curves.
- Use `SectionRotation` to adjust the section orientation, such as turning a triangular section upward or sideways.

## Statistics

The Actor updates these values from the current spline and parameters:

- `PartCounts`: instance counts for chords, braces, end plates, and related parts.
- `WeightStats`: simplified estimates for self weight, suspended load, maximum point load, maximum distributed load, deflection, and reaction per upright.
- `CurrentProfile`: profile data for the selected section and truss size.
- `BaySizeAlongSpline`: bay length along the spline.
- `SplineTotalLength`: total spline length.

## Notes

- The spline needs at least two valid points.
- More spans give a finer shape but increase instance count.
- Load and deflection values are quick estimates only. They do not replace structural engineering calculations or site approval.
