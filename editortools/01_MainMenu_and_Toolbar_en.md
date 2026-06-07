# SuperStage Main Menu & Toolbar — User Manual

## 1. Overview

The SuperStage Toolbar is the primary navigation hub of the plugin, located in the main toolbar at the top of the Unreal Engine editor. Click the **SuperStage** button in the toolbar to expand a dropdown menu and access all functional modules.

---

## 2. Access

In the Unreal Engine editor's main toolbar, find the **SuperStage** button (with the plugin icon) and click to expand the function menu.

---

## 3. SuperStage Dropdown Menu Structure

After clicking the **SuperStage** button in the toolbar, the expanded dropdown menu contains the following items (from top to bottom):

### 3.1 Top-Level Menu Items

| Menu Item | Function | Description |
|-----------|----------|-------------|
| **PluginAuth** | Open the plugin authentication window | Login to your account, view subscription status, manage licenses |
| **SuperBrowser** | Open the Asset Browser | Browse, search, and drag-and-drop all SuperStage fixture assets into the scene |

### 3.2 Modules Submenu

| Menu Item | Function | Description |
|-----------|----------|-------------|
| **SuperCAD** | Open SuperCAD lighting plot window | Draw lighting construction diagrams in an orthographic viewport |
| **SuperConsolePro** | Open SuperConsolePro console window | DMX lighting console simulator |

### 3.3 SuperDMXTool Submenu

| Menu Item | Function | Description |
|-----------|----------|-------------|
| **PatchTool** | Open the Batch Patch Tool | Batch assign DMX addresses to selected fixtures |
| **PatchPreview** | Open the Patch Preview panel | View and edit DMX address information for all fixtures in the scene |
| **DMXToMa** | Open the MA Export Tool | Export fixture configurations as grandMA2/MA3 macro scripts and stage view XML |
| **MVRImport** | Open the MVR Import panel | Import fixture layouts from .mvr files |
| **SuperDataImport** | Open the SuperData Sync panel | Synchronize fixture data with other clients over the local network |

### 3.4 Documentation Center Submenu

| Menu Item | Level | Description |
|-----------|-------|-------------|
| **QuickStart** | Level 1 | Open the Quick Start guide webpage |
| **ProductDocs** | Level 1 | Open the full product documentation webpage |
| **ModuleManuals** | Submenu | Individual module manuals |
| ├ SuperConsolePro | — | DMX lighting console manual |
| ├ SuperLaser | — | Laser system manual |
| ├ SuperNDI | — | NDI video system manual |
| ├ SuperDroneLink | — | Drone swarm manual |
| └ SuperCAD | — | Lighting construction diagram manual |
| **SystemReference** | Submenu | System reference documentation |
| ├ EditorTools | — | 17 editor tool references |
| ├ LightComponents | — | 9 lighting component references |
| ├ StageAssets | — | 6 stage asset references |
| └ DMXCoreSystem | — | DMX core system reference |
| **SuperDataProtocol** | Submenu | SuperData protocol documentation |
| ├ QuickStart | — | Protocol quick start |
| └ ProtocolSpec | — | Protocol specification |
| **DevDocs** | Level 1 | Developer documentation (API & Architecture) |
| **Changelog** | Level 1 | Version changelog |

### 3.5 Bottom Menu Items

| Menu Item | Function | Description |
|-----------|----------|-------------|
| **OnlineTutorial** | Online tutorial submenu | Contains links to Bilibili, Douyin, YouTube, and Instagram |
| **Website** | Open browser to visit official website | Visit the SuperStage official website (yunsio.com) |
| **ContactUs** | Open contact page | Visit the contact page |
| **UserAgreement** | Open user agreement page | View user agreement terms |

---

## 4. Bottom Status Bar Buttons

The following shortcut buttons are also registered in the editor's bottom status bar:

| Button | Function | Description |
|--------|----------|-------------|
| **SuperStage: {Version}** | Open official website | Shows the current plugin version number; click to visit the official website |
| **SuperDMX** | Open DMX Config Panel | Configure DMX protocol, IP address, and Universe range |
| **SuperNDI** | Open NDI Config Panel | Manage NDI video input source mappings |
| **LDLink** | Open Drone Activity Monitor | View real-time drone connection status, positions, and LED colors |

---

## 5. UE Main Menu Tools Extension

The following tools are registered in Unreal Engine editor's top **Tools** main menu (under the SuperStageTools section):

| Menu Item | Function | Description |
|-----------|----------|-------------|
| **GOBO Atlas Builder** | Open GOBO Atlas Builder tool | Extract gobo textures from fixture library attributes and generate a GOBO atlas |
| **Color Atlas Builder** | Open Color Atlas Builder tool | Extract colors from fixture library attributes and generate a color atlas texture |

---

## 6. SuperStage Edit Mode Tools

The following tools are accessed through the mode toolbar after switching to **SuperStage Edit Mode**:

| Tool | Function | Description |
|------|----------|-------------|
| **Fixture Array Tool** | Batch arrange fixtures | Duplicate and arrange selected fixtures in linear, grid, or radial patterns |
| **Spline Distribution Tool** | Arrange fixtures along a path | Distribute fixtures evenly along a spline path |

---

## 7. Quick Tips

- All tool windows can be independently dragged, dropped, and docked anywhere in the editor
- Most panels support opening multiple instances simultaneously
- If toolbar buttons are not visible, check if the plugin is enabled (Edit → Plugins → Search "SuperStage")
