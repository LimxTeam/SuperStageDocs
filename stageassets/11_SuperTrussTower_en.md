# Super Truss Tower User Manual

## Purpose

Super Truss Tower generates a vertical truss tower. The Actor calculates the bay count from tower height and standard bay height, then creates vertical chords, diagonal braces, horizontal braces, a base plate, and a top flange.

Use it for lighting towers, speaker rigging towers, and support towers for truss grids in visualization scenes.

## Main Parameters

| Parameter | Description |
| --- | --- |
| `TowerHeight` | Total tower height in centimeters. |
| `SectionType` | Section type: Box, Triangle, or Flat. |
| `TrussSize` | Truss size: S290, S400, or S520. |
| `BracePattern` | Brace pattern: Warren or Cross. |
| `TopSuspendedLoad` | Top suspended load used by the simplified statistics. |
| `WindLoad` | Wind load input used by the simplified statistics. |
| `bShowBasePlate` | Shows the base plate. |
| `bShowTopFlange` | Shows the top flange. |
| `bShowDiagonalBraces` | Shows diagonal braces. |
| `bShowHorizontalBraces` | Shows horizontal braces. |
| `ChordMaterial` | Material for chord members. |
| `BraceMaterial` | Material for brace members. |
| `PlateMaterial` | Material for the base plate and top flange. |

## Statistics

The Actor updates these values from the current parameters:

- `PartCounts`: instance counts for chords, braces, base plate, top flange, and related parts.
- `WeightStats`: simplified estimates for self weight, suspended load, maximum point load, maximum distributed load, deflection, and reaction per upright.
- `CurrentProfile`: profile data for the selected section and truss size.
- `NumberOfBays`: bay count calculated from the tower height.
- `ActualBayHeight`: actual height per bay.

## Notes

- Tower height is rounded into bay count, so the actual bay height may differ slightly from the standard bay height.
- When used with Super Truss Grid, align the tower top and grid height manually.
- Load and deflection values are quick estimates only. They do not replace structural engineering calculations or site approval.
