# Super Truss Tower — User Manual

## 1. Overview

**Super Truss Tower** is a procedural vertical truss column generation tool provided by SuperStage. It simulates stage lighting towers, speaker rigging towers and other vertical structures, and supports load calculation.

### Core Features

- **Vertical column structure** — Chords + diagonals + horizontal braces
- **Base plate** — Ground contact plate, increasing contact area
- **Top flange** — For connecting to truss grids or other structures
- **Euler critical load** — Simplified stability calculation

### Use Cases

- Stage lighting towers
- Speaker rigging towers
- Vertical support for truss grids
- Standalone signal/flag towers

---

## 2. Main Parameters

### 2.1 Dimension Parameters

| Parameter | Description |
|------|------|
| **TowerHeight** | Tower height (cm) |
| **SectionType** | Section type: Box / Triangle / Flat |
| **TrussSize** | Size: S290 / S400 / S520 |

### 2.2 Component Visibility

| Parameter | Description |
|------|------|
| **bShowBasePlates** | Show base plates |
| **bShowTopFlange** | Show top flange |
| **bShowDiagonals** | Show diagonal braces |
| **bShowHorizontalBraces** | Show horizontal braces |

### 2.3 Load Parameters

| Parameter | Description |
|------|------|
| **TopLoad** | Top load (kg) |

---

## 3. ISM Components

| Component | Elements |
|------|------|
| ChordISM | Chords (vertical main tubes) |
| DiagonalISM | Diagonal braces |
| HorizontalBraceISM | Horizontal braces |
| BasePlateISM | Base plates |
| TopFlangeISM | Top flange |

---

## 4. Statistics

- **Self-weight** — Total tower weight
- **Euler critical load** — Simplified buckling stability calculation

> ⚠️ Euler critical load is a theoretical simplified value, for reference only; cannot replace professional structural calculation.

---

## 5. Usage Tips

- When paired with Super Truss Grid, the tower top flange should align with the grid height
- Box section provides the best stability, suitable for tall towers
- Triangle section is suitable for lightweight short towers
- Base plates increase ground contact area in outdoor scenarios
