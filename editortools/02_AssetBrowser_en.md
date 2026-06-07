# SuperStage Asset Browser — User Manual

## 1. Overview

The Asset Browser (Super Asset Browser) is SuperStage's fixture asset management panel. It automatically collects all available SuperStage fixture Blueprints in the project, organized by category and manufacturer, with support for searching, filtering, and drag-and-drop operations, making it easy to quickly place fixtures into the scene.

---

## 2. Access

**Main Menu Path**: Toolbar **SuperStage** dropdown menu → **SuperBrowser**

---

## 3. Interface Layout

The Asset Browser is divided into three main areas:

```
┌─────────────────────────────────┐
│  Search bar + filter buttons    │
├──────────┬──────────────────────┤
│          │                      │
│ Category │   Fixture Asset      │
│ Tree     │   Tile View          │
│          │                      │
│          │                      │
├──────────┴──────────────────────┤
│  Status bar (asset count)       │
└─────────────────────────────────┘
```

---

## 4. Feature Details

### 4.1 Search Bar

Located at the top of the panel, provides real-time text search functionality.

| Action | Description |
|--------|-------------|
| Enter keywords | Real-time filtering of fixture names, supports fuzzy matching |
| Clear search box | Show all fixtures |

**Search Scope**: Matches fixture asset names.

### 4.2 Category Tree (Left Panel)

Displays all fixture asset categories in a tree structure.

**Category Hierarchy**:

| Level | Description | Example |
|-------|-------------|---------|
| First Level | Fixture group | Moving Light, Conventional, Effect Light, Special Effect Device |
| Second Level | Manufacturer | Acme, Chauvet, ClayPaky |
| Third Level | Specific fixture model | XP-380Beam, Intimidator Spot |

**Operations**:
- **Click a category node**: The right asset view only shows fixtures under that category
- **Click root node "All"**: Show all fixtures
- The number next to a category indicates the fixture count in that category

### 4.3 Asset Tile View (Right Main Area)

Displays filtered fixture assets in tile form.

**Each tile shows**:
- Fixture thumbnail (automatically obtained from the Blueprint asset)
- Fixture name

**Operations**:

| Action | Description |
|--------|-------------|
| **Click** | Select that fixture asset |
| **Drag to viewport** | Drag the fixture to a specific location in the 3D scene; a fixture Actor is automatically created |
| **Double-click** | Locate the asset in the Content Browser |

### 4.4 Status Bar

Located at the bottom of the panel, displays statistics for the current filter results:
- Number of currently displayed assets
- Total number of assets

---

## 5. Workflow

### 5.1 Placing a Fixture into the Scene

1. Open the Asset Browser
2. (Optional) Use the left category tree to narrow down the scope, or enter keywords in the search bar
3. Find the target fixture in the right tile view
4. **Hold down the left mouse button and drag** the fixture tile into the 3D viewport
5. **Release the mouse** at the target location; the fixture will be placed there

### 5.2 Quickly Finding a Fixture

1. Enter part of the fixture name in the top search bar (e.g., "Beam", "Wash")
2. The view filters in real time, only showing fixtures whose names contain the keyword
3. Clear the search bar to restore the full display

---

## 6. Notes

- The Asset Browser only shows Blueprint assets that inherit from SuperStage fixture base classes
- Newly added fixture Blueprints need correct category metadata to appear under the correct category
- If the fixture list is empty, check whether the project's Content directory contains SuperStage fixture assets
- When dragging to the scene, fixtures use default DMX configuration, which can be modified later via the Patch Tool
