# Super Circular Truss User Manual

## Purpose

Super Circular Truss generates a complete circular truss or dual-ring truss. It is not an arbitrary arc tool; the current implementation builds a full 360-degree ring and uses `SegmentCount` to control how many segments the ring is split into.

Use it for circular lighting rigs, central stage rings, and round suspended structures in visualization scenes.

## Main Parameters

| Parameter | Description |
| --- | --- |
| `OuterRadius` | Outer ring radius in centimeters. |
| `InnerRadius` | Inner ring radius in centimeters. Set it to 0 for a single ring. |
| `SegmentCount` | Number of ring segments. Higher values create more pieces. |
| `SectionType` | Section type: Box, Triangle, or Flat. |
| `SectionRotation` | Section rotation about the ring direction, −180 to 180 degrees, default 0. **Added in 26H2.5** |
| `TrussSize` | Truss size: S290, S400, or S520. |
| `BracePattern` | Brace pattern: Warren or Cross. |
| `SuspendedLoad` | Suspended load used by the simplified statistics. |
| `bShowInnerRing` | Shows the inner ring. This only has visible effect when `InnerRadius` is greater than 0. |
| `bShowRadialBraces` | Shows radial braces between the inner and outer rings. Requires the inner ring. |
| `bShowConnectionFlanges` | Shows segment connection flanges. |
| `ChordMaterial` | Material for chord members. |
| `BraceMaterial` | Material for brace members. |
| `FlangeMaterial` | Material for flanges. |

## Statistics

The Actor updates these values from the current parameters:

- `PartCounts`: instance counts for chords, braces, flanges, and related parts.
- `WeightStats`: simplified estimates for self weight, suspended load, maximum point load, maximum distributed load, deflection, and reaction per upright.
- `CurrentProfile`: profile data for the selected section and truss size.
- `OuterCircumference`: outer ring circumference.
- `SegmentArcLength`: outer arc length per segment.

## Notes

- Use Super Curved Truss when you need a half ring or a free-form arc.
- `InnerRadius` should be smaller than `OuterRadius`.
- Load and deflection values are quick estimates only. They do not replace structural engineering calculations or site approval.
