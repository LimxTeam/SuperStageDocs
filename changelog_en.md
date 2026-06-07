# SuperStage Changelog

> Recording feature updates, bug fixes, and performance optimizations across SuperStage versions.

---

## Version 26H2.0

> Version: 26H2.0
> Release Date: June 4, 2026
> Based on: 26Q2.9

### MVR File Export Support

A new **MVR Export** feature has been added to the DMX tools. You can now export fixtures from the current SuperStage scene directly as standard MVR files (`.mvr`), readable and reusable by MVR-compatible software such as VectorWorks, grandMA, and Capture.

**Export content includes:**

- The **3D positions and orientations** of all selected fixtures in the scene (identical to the SuperStage viewport)
- **DMX patch information** for each light (Universe, start address, Fixture ID)
- All corresponding **GDTF fixture profile files** for all fixtures, packaged alongside the MVR without manual profile organization

**Usage:**

1. Open **DMX Tools** → switch to the **MVR Export** tab
2. Check the fixture types to export
3. Click **Export** and choose a save path

> The exported `.mvr` file can be directly imported into software supporting the MVR standard such as VectorWorks, Depence, and grandMA onPC. Fixture types are automatically grouped by model, and patch data is ready to use out of the box.

---

### SuperDMX Supports 512 Universes

SuperDMX's **Art-Net / sACN** input and output capabilities have been extended to **512 DMX Universes**, making it more suitable for large stages, matrix lights, pixel light strips, and multi-console integration projects.

**Benefits:**

- Both Art-Net and sACN can cover 512 Universes; complex projects no longer need to split configurations due to Universe count limits
- DMX patching, address preview, activity monitoring, and recording workflows simultaneously support 512 Universes
- Large light arrays, matrix light walls, and light strip projects can more easily maintain consistent Universe planning during on-site debugging

### grandMA Patch Import Fully Upgraded

The previous **SuperData** workflow has been deprecated and replaced by a new grandMA import method. You can now connect directly to grandMA2 / grandMA3 from the DMX Tools and quickly import fixture patch data from SuperStage scenes into the console.

**Benefits:**

- No longer need to install the **SuperData server**
- No longer need to install additional MA plugins in the MA console or onPC
- No longer need to manually prepare, replace, or maintain fixture profile files
- Imported patches automatically carry corresponding fixture profiles by fixture type; each fixture type is processed only once, making batch projects easier
- Supports subsequent synchronization of newly added or changed fixture data, reducing redundant imports and manual verification

**Fixture Profile Resource Updates:**

- **grandMA2 fixture profiles** added to the plugin directory
- **GDTF fixture profiles** added to the plugin directory
- The new import workflow prioritizes using profile resources bundled with the plugin and already bound, making project migration and delivery to client computers more convenient

### MADRIX Light Strip Connection & Recording Playback

New **MADRIX light strip connection** capability allows the local MADRIX main output to be brought into SuperStage in real time and directly drive light strip / matrix light strip materials for previewing effects such as stage LED lines, matrix light walls, and pixel light strips.

**Feature Highlights:**

- Supports displaying MADRIX main output in real time on light strip carriers in SuperStage scenes; light strip visuals change instantly during on-site debugging
- New dedicated MADRIX light strip material entry, keeping only brightness adjustment for cleaner configuration, making it easier to create custom materials according to project needs
- Supports recording MADRIX output to Sequence; recorded content can be played back directly in Sequencer
- Recorded MADRIX light strip signals can be played back offline without the MADRIX software, suitable for previz, delivery demos, and review in environments without external software
- Recording and playback optimized for continuous animation performance, reducing redundant refreshes while balancing real-time preview smoothness and visual quality

### NDI Configuration Panel Smoothness Optimization

The NDI configuration panel now auto-refreshes the source list once when opened; subsequent refreshes are manually triggered by the user as needed, avoiding repeated scanning during selection, switching, and editing that previously caused prolonged UI freezes.

**Experience Improvements:**

- Refreshing the NDI source list no longer freezes the entire configuration panel; a clear refresh status is displayed during the process
- The source list dropdown uses the last refresh result and does not trigger re-scanning during expand and selection
- Adding, deleting, and modifying input configurations feels lighter with many NDI sources or in complex network environments
- The manual refresh button can re-fetch the latest NDI sources at any time, making on-site troubleshooting and source switching more intuitive

### Framing Fixture Blade Range Recalibrated

The four-blade framing range for framing fixtures (Blade / Framing) has been recalibrated, correcting the previously insufficient framing depth issue.

**Experience Improvements:**

- When framing blades are pushed to 50%, the light spot is physically half-obstructed, matching real fixture behavior
- Full push achieves complete light cut-off, with no residual light from insufficient framing
- Applied uniformly to all fixtures using the framing system

### Beam Surface Hotspot Rendering Optimized

The hotspot effect when beams project onto stage surfaces has been comprehensively upgraded. For some fixtures, the surface hotspot is now **drawn by the beam itself**, no longer relying on the engine's built-in spotlight component, resulting in more accurate projection and better performance.

**Benefits:**

- Surface hotspot and beam volume are **fully synchronized** — GOBO patterns, color wheels, prism beam splitting, and blade framing appear identically on the hotspot as within the beam, eliminating beam-hotspot mismatch
- Reduced engine spotlight component overhead; **frame rate significantly improved in multi-light scenes**, especially dense fixture groups such as matrix light arrays
- Hotspot dynamically follows beam zoom, focus, rotation, and other parameters in real time without additional configuration

### Beam Quality Stability Optimization

New **Dynamic Quality** toggle and **Custom Quality** control added to beam default parameters. Auto quality strategy is still used by default; when projects have special camera positions, long-distance viewpoints, or high-precision capture needs, dynamic quality can be disabled and beam sampling quality manually fixed.

**Experience Improvements:**

- Custom quality range: **10%-100%**. When dynamic quality is off, adjustment is made directly in fixture defaults
- Manual quality bypasses automatic downsampling caused by camera distance, zoom, and viewing angle, avoiding beams being pulled to excessively low quality in some scenes
- Normal beams and framing beams use the same quality parameter set; framing, prism, GOBO, and color wheel effects remain consistent

### Lighting Color Fidelity Optimized

This version brings a unified optimization of color reproduction in lighting rendering. The color appearance of white light, color temperature, colored beams, GOBOs, framing lights, lasers, and effect lights is now closer to real stage fixtures.

**Experience Improvements:**

- White light reproduction is closer to the cool-white look of real fixtures, reducing problems of warm, gray, or "screen white" appearance
- Color temperature adjustment, CTO, warm/cool blending, and RGB / RGBW / CMY color mixing results are more natural, making it easier to align color adjustments with real fixture effects
- Colored beams have better saturation and clarity; red, green, blue, cyan, purple, and other colors no longer appear dull or artificial
- GOBO, prism, framing, matrix lights, lasers, and effect lights use more consistent color reproduction, with smaller color differences between different fixture modules

### New "Director Mode" for Media Input

Screens (SuperScreen) and projectors (SuperProjector) now support a third input source — **Director Mode**. Place a "Director Camera" in the scene to use the camera's live view as the content source for screens or projections, easily achieving on-stage screen linkage, virtual monitors, and other effects.

**Usage:**

1. Drag **DirectorCamera** from the asset browser and position it at the desired camera position
2. Select **Director** in the screen or projector's **SourceMode**
3. In the **DirectorCamera** property that appears, select the director camera instance in the scene

**Feature Highlights:**

- Director cameras support custom **resolution** and **field of view (FOV)**, allowing adjustment of image quality and field of view as needed
- Capture toggle can be turned on/off at any time; **zero performance overhead** when not in use
- Multiple screens/projections can reference the same director camera for **multi-screen same-source** display
- Multiple director cameras can be placed with different screens referencing different positions for **multi-camera switching**

### Patch Tool Experience Upgrade

Two practical improvements have been added to the DMX Patch Tool, greatly simplifying multi-round batch patching workflows.

**Auto-Increment Start Values:**

- When opening the Patch Tool, the start Universe, start Address, and start Fixture ID **automatically read existing fixtures in the scene**, starting from the next available value
- Each time "Apply" is clicked to complete a patch, all three start values **automatically update** to the latest available position, allowing the next round of patching without manual adjustment
- **Manual modification** is still supported; auto values serve only as intelligent defaults

**Auto-Rename Fixtures During Patching:**

- A new **RenameID** checkbox has been added to the toolbar (checked by default). When enabled, clicking "Apply" automatically sets the fixture name in the Outliner to the format **ClassName_FixtureID** (e.g., `SuperStageLight_1`, `SuperWashLight_5`)
- Blueprint fixtures are also supported; names are automatically stripped of engine suffixes for brevity
- Uncheck to keep the original names unchanged

### Patch Preview Panel Integration & Visual Channel Grid

The patch preview panel has been integrated into the DMX Patch Tool, with a new **visual channel grid** preview. No more switching windows — after patching, you can visually inspect the channel occupancy of all fixtures within the same panel.

**Feature Highlights:**

- The channel grid visually presents all 512 channels of a Universe in **32x16** format, with fixtures displayed as colored blocks occupying their corresponding address ranges
- Supports **multi-Universe display**; each Universe expands independently, scrollable to view all patches
- Fixture blocks support **mouse drag to change start address**; takes effect on release, with both table and grid synced in real time
- Built-in **conflict detection** — overlapping address channels in the same Universe are highlighted in red; duplicate Fixture IDs are also prominently flagged, making patch errors visible at a glance
- Grid and left-side patch table are **bidirectionally linked**; selecting a fixture on either side highlights it on the other

### One-Click Model Binding to Screens / Light Strips

A new **right-click menu one-click binding** entry has been added for screens (SuperScreen) and light strip effects (LightStripEffect), eliminating the tedious operation of adding StaticMeshActors one by one in the Details panel. Binding **100 models takes just 2 clicks**.

**Usage:**

1. **Select** one or more Static Mesh Actors in the level viewport or Outliner (as display carriers for screens / light strips)
2. **Right-click** to open the context menu → expand the **SuperStage** submenu
3. Choose **Bind To SuperScreen** or **Bind To LightStripEffect** → the secondary submenu lists all candidate target Actors in the scene in real time
4. Click any target Actor; the selected models are appended to that target's target array and **the material takes effect immediately**

**Feature Highlights:**

- Target lists are automatically sorted by **ActorLabel ascending** for easy navigation
- When there are multiple targets, an additional **Add To ALL** entry is provided for binding to all targets at once
- **Append + dedup** mode: previously manually added elements are preserved; repeated clicks do not create duplicates
- Full **Undo / Redo** support throughout; misoperations can be undone at any time
- Menu items **auto-hide** when the selection contains no static meshes or there are no candidate targets in the scene, causing zero disruption to other workflows

### 40 New Native C++ Stage Fixture Assets

This version completes **native C++ asset implementations** for fixtures from multiple major international manufacturers, all based on real GDTF channel mappings with full-pipeline rendering ready for DMX control → color / rotation / beam / gobo / prism / framing — **drag into scene, patch DMX, use immediately**.

Compared to Blueprint implementations, C++ fixtures deliver **significantly improved performance** in multi-light scenes, and incorporate the "Beam Surface Hotspot" and "Dual Color Wheel / Multi-Gobo Wheel" template function upgrades in this version for more accurate rendering details.

| Manufacturer | Models |
|------|------|
| **Robe** (7 models) | BMFL Blade, MegaPointe, LEDWash800, iSpiider Wash, DL4X Spot, iBar15 |
| **ClayPaky** (4 models) | Sharpy Plus, HY B-EYE K25, B-EYE K10, Alpha Beam 300 |
| **ADJ** (6 models) | 12P HEX, Encore Burst 200, 32HEX Panel IP, 40HEX IP Panel, Encore LB15IP |
| **ETC** (3 models) | ColorSource Par, ColorSource Spot, D60 Vivid |
| **GTD** (2 models) | 1500N Profile, 330II Beam |
| **Martin** (2 models) | ELP Par IP, MAC Aura |
| **Chauvet** (2 models) | Strike 1, Strike 4 |
| **CKC** (1 model) | P10+ |
| **PR-Lighting** (2 models) | AQUA 580 BWS, PR-2926 |
| **GLP** (5 models) | Impression GT-1, Impression S350, JDC1, Impression X5 IP Maxx, Impression X4 Bar 20 |
| **Astera LED** (1 model) | Titan Tube |
| **Spark** (1 model) | P1Evo |
| **JB-Lighting** (2 models) | P18 MK2 Profile MA HP, P18 MK2 Wash MA HP |
| **GTB** (2 models) | XS600IP, LED X7 |

> All fixtures come with built-in **fixture icons** (Asset Browser thumbnails), **3D housing models**, and **SuperFixtureLibrary profiles**, directly recognized by the Patch Tool with no additional configuration needed.

### Upgrade Instructions

Upgrading from 26Q2.9 to 26H2.0:

1. Direct overwrite installation is sufficient. This update may be incompatible with older project versions; please back up the old plugin before updating. If urgent compatibility handling is needed, contact the administrator.
2. **New**: MVR file export feature — export fixtures from DMX Tools to VW, MA, Capture, and other software
3. **New**: SuperDMX Art-Net and sACN protocols both support 512 Universes
4. **New**: grandMA2/MA3 patch direct import — no longer need SuperData server and MA plugins
5. **New**: MADRIX light strip connection, recording, and offline playback
6. **New**: Director Mode — SuperScreen and SuperProjector support using camera views as media sources
7. **New**: Patch Tool auto-increment start values + auto-rename fixtures during patching
8. **New**: Patch preview panel integrated into Patch Tool, new visual channel grid + conflict detection
9. **New**: Right-click menu one-click model binding to screens / light strips
10. **New**: 40 native C++ fixture assets (Robe, ClayPaky, ADJ, ETC, GTD, Martin, Chauvet, CKC, PR-Lighting, GLP, Astera LED, Spark, JB-Lighting, GTB)
11. **Optimized**: Framing fixture blade range recalibrated for more precise framing
12. **Optimized**: Beam surface hotspot rendering upgraded — better performance, GOBO/prism/framing effects fully synchronized
13. **Optimized**: Beam quality now has two modes: Dynamic / Custom
14. **Optimized**: Lighting color fidelity comprehensively improved
15. **Optimized**: NDI configuration panel source list refresh no longer causes stuttering

---

## Version 26Q2.9

> Version: 26Q2.9
> Release Date: April 25, 2026
> Based on: 26Q2.5

### Editor Experience Optimizations

#### Tool Window Docking Persistence

All SuperStage tool windows now support docking state persistence, automatically restoring to the last docked position when the editor restarts.

**Benefiting Windows:**

- SuperBrowser Asset Browser
- SuperDMX Patch Tool / Patch Preview / DMXToMa
- MVR Import / SuperData Import
- GOBO Atlas Builder / Color Atlas Builder
- SuperDMX Configuration / SuperNDI Configuration / LDLink Monitor

> Previously, these windows needed to be manually reopened and docked after each editor restart; they now behave consistently with native UE panels — set the layout once and it persists long-term.

#### Toolbar Icon Optimization

Toolbar and tab icons now load and display immediately on editor startup, eliminating the previous issue of blank icons during early startup.

#### Plugin Update Check

The editor now automatically checks for new SuperStage versions on startup:

- **New version available**: An update notification window pops up showing the latest version number and update content, with one-click access to the official download page or full update history
- **Already up to date**: A brief notification pops up in the bottom right confirming the current version
- A new **"Check for Updates"** button has been added to the bottom of the authentication panel for manual checks at any time

#### sACN Recording Optimization

When using Take Recorder to record DMX data via the sACN protocol, the system now automatically manages multicast groups:

- Automatically joins the multicast groups for the target Universe range when recording starts, ensuring complete data reception
- Automatically scales back to the default range after recording ends, releasing unneeded multicast subscriptions

> Previously, recording sACN required manually ensuring multicast groups were joined, or data might be lost. Now fully automatic; Art-Net users are unaffected.

#### CAD Perspective 3D View

The CAD Drawing Editor now includes a **Perspective 3D View**, allowing stereoscopic inspection of fixture layouts from any angle:

- The view direction dropdown in the toolbar now includes a **"Perspective 3D"** option, or press shortcut **5** for quick switching
- **Right-click drag** to rotate the view, **middle-click drag** to pan, **scroll wheel** to zoom in/out
- When switching back to orthogonal views (Top/Front/Side), previous zoom and position are automatically restored; switching back to perspective also restores the previous 3D view
- Fixture annotations, dimension lines, cable routes, and other CAD annotations display normally in perspective mode
- Ground grid automatically adapts to perspective projection; scale bar is automatically hidden in perspective mode (because the pixel-to-physical-dimension mapping varies with depth in perspective)

> Suitable for quickly previewing overall spatial layout after fixture arrangement, or showing stereoscopic effects to clients. All CAD tools (selection, annotation, measurement, line drawing, etc.) function normally in perspective mode.

#### Placement Panel Asset Categorization

SuperStage's fixture and stage assets are now automatically categorized in UE's built-in **Place Actors** panel. Assets can be quickly found and placed without opening SuperBrowser.

- Fixtures, stage equipment, procedural models, etc. are categorized and displayed in separate classification tabs
- Newly added fixtures and assets automatically appear in corresponding categories without additional configuration

### New Fixture Assets

This version significantly expands the fixture asset library with **18 new fixtures**, covering moving heads, framing fixtures, wash lights, strobes, matrix lights, profile lights, front lights, fan lights, vintage lights, and other mainstream fixture types.

#### Clay Paky

- **Sharpy Plus** — Classic beam moving head supporting CMY color mixing, dual color wheels, dual gobo wheels, effect wheel, dual prisms, frost, and zoom
- **HY B-EYE K25** — LED wash moving head with RGBW four-color mixing + CTO color temperature compensation, built-in 66-color macro color wheel, 4°-60° ultra-wide zoom range

#### Robe

- **BMFL Blade** — High-power moving head framing fixture with four-blade framing system
- **MegaPointe** — Multi-function moving head
- **LEDWash 800** — LED wash light, RGBW four-color mixing + CTO

#### Hyperion Series

- **Hyperion 700 Profile** — 700W RGB LED framing fixture

#### Flare Series

- **Flare RGBW600** — Wash light, RGBW four-color mixing + zoom
- **Flare WashMH600** — Moving head wash light

#### Spark Series

- **Spark Profile600** — Profile light, four-blade precision framing + zoom
- **Spark Vintage300** — Vintage light, warm/cool color mixing
- **Spark V1** — COB front light

#### Blaze Series

- **Blaze Atomic3K** — High-power strobe light
- **Blaze Strip1000** — Long bar LED strobe
- **Blaze MatrixStrip20** — Matrix bar strobe (20-pixel independent control)

#### Thunder Series

- **Thunder Sidewinder10** — Sidewinder 10-head matrix beam bar

#### Aurora Series

- **Aurora Fan200** — 200W LED fan light
- **Aurora Fan400** — 400W LED fan light

### Fixture Profile Management

#### JSON Profile Import & Export

Support for importing and exporting fixture configuration files (profiles) in JSON format, facilitating cross-project sharing and backup of fixture data.

#### MA2 Profile Support

New import and export capabilities for grandMA2 profile XML format, enabling direct data exchange with grandMA2 console fixture profiles.

### Programmer & Playback

#### CUE Copy & Paste

The CUE list now supports copy, paste, and slot move operations for quickly reusing and adjusting programmed lighting effects.

#### Show File Save Optimization

- A new **Save button** has been added to the encoder bar for one-click saving of the current show file
- Unsaved changes display a **badge indicator** on the button to prevent forgetting to save

### Bug Fixes

- Fixed issue where deleted fixture properties remained in the property panel
- Fixed issue where the interface did not properly refresh after show file overwrite save

### Lighting Distance Parameter Unit Standardization (Metric)

The maximum throw distance parameters for stage lights, lasers, and projectors have been unified **from centimeters to meters**, aligning with stage and lighting industry conventions (PAR light hanging height and laser throw distances are all measured in meters), making property panel input more intuitive.

**Three affected fields:**

- **Stage Light MaxLightDistance** (`FLightingComponentDefaultValue`) — default value `23.45 m` for all fixtures inheriting from `ASuperStageLight` (moving heads, PAR lights, Beam lights, Spot lights, rectangular lights, matrix lights, etc.)
- **Laser MaxLaserDistance** (`ASuperLaserActor`) — Beyond laser visualization Actor, default value `23.45 m`
- **Projector MaxLightDistance** (`ASuperProjector`) — NDI video projection / architectural mapping Actor, default value `30 m`

**User Experience:**

- Enter meters directly in the property panel (e.g., `23.45`), no more writing values like `2345` centimeters
- Internal light attenuation radius, Shader beam geometry, and scene ray detection still use centimeter precision; rendering is fully equivalent with no precision loss
- Reference ranges: small studio 10-15 m, medium theater 20-30 m, large stadium 50-80 m, outdoor festival 80-100 m

### Smart Beam Sampling Quality (Auto LOD)

Beam rendering quality is now **automatically managed** by the system based on camera-to-beam distance, with no manual configuration needed.

**Benefits:**

- When the camera is **close to the beam**, sampling precision automatically increases to maximum, with the strongest volumetric feel and clearest detail at close range
- When the camera is **far from the beam**, sampling overhead automatically decreases, maintaining consistent distant appearance with significantly reduced GPU load
- **Smooth transitions** as the viewpoint moves, with no quality jumps or performance jitter
- Fixed a visual issue where close-range beams showed **gaps** at certain angles

**Parameter Changes:**

- The original `BeamQuality` beam quality parameter has been removed (including beam default parameters, API function signatures, and DMX adjustment items)
- Previously users had to choose between "precision" and "performance"; now both are achieved simultaneously. If Blueprints reference the old field, delete it and re-save

### Volumetric Beam Rendering Performance Significantly Optimized

Deep optimization of stage beam rendering; **common wide-angle scene frame rates improved 2-4x**, eliminating the "frame drop when widening zoom" concern.

**Benefits:**

- Moving heads and beam lights with wide open zoom for stage flood effects can be used confidently without compromising frame rate
- Overall preview smoothness of dense light arrays significantly improved
- No more severe frame drops when the camera faces directly toward the beam direction during fixture placement
- **Visual quality remains consistent** — all colors, edge softness, GOBO patterns, and prism effects are identical to pre-optimization

### Engine Version Support Changes

Starting from version 26Q2.9, SuperStage **pauses UE 5.6 updates** while **adding UE 5.8 support**.

| Engine Version | Support Status |
|---------------|----------------|
| UE 5.6 | Paused (frozen at 26Q2.9) |
| UE 5.7 | Continued updates |
| UE 5.8 | Newly supported |

> From this version onward, SuperStage only updates for **UE 5.7 — UE 5.8**. Users still on UE 5.6 should upgrade their engine version as soon as possible.

### Upgrade Instructions

Upgrading from 26Q2.5 to 26Q2.9:

1. Direct overwrite installation is sufficient. This update may be incompatible with older project versions; please back up the old plugin before updating. If urgent compatibility handling is needed, contact the administrator.
2. **New**: All tool window docking layouts are automatically saved, no need to re-arrange after each restart
3. **Note**: Starting from this version, UE 5.6 builds are no longer provided; please use UE 5.7 or UE 5.8
4. **New**: 18 fixture assets (Clay Paky, Robe, Hyperion, Flare, Spark, Blaze, Thunder, Aurora series), covering moving heads, framing lights, wash lights, strobes, matrix lights, front lights, and other mainstream fixture types
5. **New**: JSON profile import/export + grandMA2 XML profile interoperability
6. **New**: CUE list copy & paste feature + encoder bar save button
7. **New**: Place Actors panel displays SuperStage assets by category; quick placement without opening SuperBrowser
8. **Note**: The maximum distance parameter unit for lights/lasers/projectors has changed from centimeters to meters. Existing saved values in Blueprints and levels will be read with the new unit (displayed distance will appear 100x larger). Please manually **divide** old centimeter values **by 100** before saving (e.g., `2345` → `23.45`, `3000` → `30`), or reset to new defaults in the Details panel

---

## Version 26Q2.5

> Version: 26Q2.5
> Release Date: April 8, 2026
> Based on: 26Q2.2

### Media Display Module

#### SuperScreen (Upgraded from SuperNDIScreen)

SuperNDIScreen has been upgraded to **SuperScreen**, no longer limited to NDI video streams and now supporting multiple media source inputs.

**New Features:**

- **Media Source Mode Switching**: New media source mode switching allows free switching between NDI live video streams and static textures
- **Static Texture Input**: After switching to static texture mode, any texture asset can be directly specified as screen content without NDI signals
- **Adaptive Property Panel**: Automatically shows or hides corresponding parameters based on the selected mode for more intuitive operation

#### SuperProjector Media Source Upgrade

SuperProjector has been synchronously upgraded to support the same media source switching capabilities as SuperScreen.

**New Features:**

- **NDI / Static Texture Switching**: Projectors now also support NDI and static texture input methods, freely switchable based on the scene
- **Unified Operation Experience**: Screens and projectors share the same media source configuration approach with zero learning curve

#### Keystone Correction Parameter Unification

Keystone correction parameters for screens and projectors have been unified into the same structure with fully consistent operation.

- **Four-Corner Correction**: Supports individual adjustment of top-left, bottom-left, top-right, and bottom-right corner points
- **Unified Operation Logic**: Screens and projectors use the same correction method; no need to re-learn when switching between devices

### Fixture Control Module

#### SuperStageLight Operation Optimization

The fixture property panel experience has been comprehensively upgraded for more intuitive and smoother operation.

**Improvements:**

- **Unified Default Parameter Panel**: Parameters such as light intensity, max distance, dimming curve, shadows, lighting channel, volumetric fog, beam density/fog/quality are now consolidated under the same group for immediate clarity
- **Smooth Details Panel Operation**: Completely resolved severe stuttering when dragging sliders; attribute adjustments respond instantly and smoothly

#### DMX Control Mode Switching

All DMX fixtures now include a **Control Mode** option, allowing free switching between DMX signal control and property panel manual control.

- **DMX Mode** (default): Fixture parameters are driven in real time by DMX signals, suitable for normal performance scenarios connected to lighting consoles
- **Property Mode**: Fixtures ignore DMX signals and are fully controlled by property values in the Details panel, suitable for offline debugging, preview positioning, teaching demonstrations, and other non-console scenarios

#### Prism Builder (New)

A completely new prism system is now available, supporting custom prism facet arrangement with a visual editor for real-time WYSIWYG adjustment.

**Core Features:**

- **Prism Preset Assets**: Supports creating "Super Prism Presets," each preset supporting 3 independent prism configurations
- **Visual 2D Editor**: Double-click a prism preset asset to open a dedicated editor; drag facets on the left canvas to adjust each facet's position; precisely edit parameters on the right panel
- **11 Built-in Preset Templates**: Circle, Line, Triangle, Square, Pentagon, Hexagon, Star, Cross, Diamond, Arc, Honeycomb — one-click apply then freely fine-tune
- **Multi-Select Editing**: Supports Ctrl+click multi-select facets for batch position and scale adjustment
- **Global Parameters**: Each prism config supports global scale, position scale, rotation, and offset — one parameter controls the overall layout
- **Asset Metadata**: Can fill in manufacturer, fixture model, identifier, and description for managing large numbers of prism presets
- **Position Texture Export**: Supports exporting prism configuration as a texture asset for reuse

**Fixture Integration:**

- **Auto Switch Prism Effects**: After binding prism preset assets in the fixture profile configuration, fixtures can automatically switch prism effects via DMX
- **Rotation & Speed Control**: Supports prism rotation and rotation speed control
- **Performance Friendly**: Zero performance overhead for fixtures without prisms; fixtures with prisms render dynamically based on actual facet count without wasting GPU resources

### Console & Programming Module

#### Console Property Bar & Patch Panel Fixes

**Fixes:**

- **Property Bar Default Value Display Fix**: Fixed an issue where default values for multiple fixture properties in the encoder, property bar, and patch table appeared smaller than expected; defaults now correctly reflect settings in the fixture profile
- **Patch Select All Fix**: Fixed an issue where the "Select All" button in the patch panel could not select lost fixtures; Select All now correctly covers all imported fixtures (including those lost in the scene)
- **Patch Statistics Count Fix**: Fixed an issue where the statistics count did not update after removing lost fixtures; fixture count now always matches the actual list

#### Patch Panel Category Filter Enhancement

The Patch Panel now includes finer-grained filter labels for quickly locating fixtures that need attention.

- **Pending Sync**: Yellow label — one-click filter for fixtures that have changed in the scene and need synchronization (auto-hides when no fixtures need syncing)
- **Lost**: Red label — one-click filter for fixtures that have been imported but cannot be found in the scene (auto-hides when no fixtures are lost)

#### Encoder Wheel Mouse Scroll Control

Attribute encoder wheels now support mouse scroll wheel operation; hover over a wheel and scroll to adjust attribute values without dragging.

- **Default Step**: Each scroll wheel notch adjusts 1% of the range
- **Coarse Mode**: 5% per notch for fast large-range adjustment
- **Fine Mode**: 0.2% per notch for precise micro-adjustment
- **Shift Fine**: Hold Shift for an additional 5x sensitivity reduction

#### Playback Panel Pagination

The Playback panel now supports paginated browsing, with 200 buttons per page and up to 60 pages (12,000 total button slots).

- **Page Navigation**: Previous/Next page buttons + current button range display
- **Page Jump Input**: Directly enter a page number and press Enter to jump to the specified page
- **Pagination Rules**: Page 1 = 1-200, Page 2 = 201-400, and so on

#### Color Picker Enhancement

The Color Picker has been comprehensively upgraded, supporting more color channels and fixture color wheel presets.

- **Extended Color Channel Support**: The color palette now correctly controls all color channels including White, Amber, Cyan, Indigo, Lime, and RedOrange; extended channels are automatically zeroed when selecting colors to avoid color contamination
- **CMY Subtractive Channel Linkage**: CMY subtractive channels are automatically linked when picking colors from the palette, reducing manual conversion and repeated fine-tuning
- **Color Wheel Preset Dynamic Loading**: Automatically reads color wheel slot colors from the currently selected fixture's profile and displays them as clickable color swatches; supports multi-color-wheel fixtures with each color wheel in a separate group

#### Frame Editor Attribute-Level Parameter Refactoring

All effect parameters in the Frame Effect Editor now support independent per-attribute control; each attribute can have completely different effect configurations.

**Core Upgrade:**

- **Independent Effect Parameter Adjustment**: 10 effect parameters including speed, width, attack, decay, waveform, phase, and grouping can all be set per attribute
- **Dynamic Phase Distribution**: Different attributes can have their own phase variations and distribution patterns for more flexible effect expression
- **Independent Speed Control**: Different attributes can be set to different speeds; e.g., dimmer and pan can use different rhythms

**UI Improvements:**

- **Toolbar Real-Time Reflection of Selected Attribute**: When switching the selected attribute, the right toolbar automatically shows that attribute's independent parameter values
- **Batch Editing**: When multiple attributes are selected, parameter modifications are applied to all selected attributes simultaneously
- **Waveform Display Synchronization**: Waveform view curves and fixture points update in real time with current attribute settings
- **Attribute Channel Toggle**: The checkbox next to each attribute can temporarily disable that attribute's effect output; when unchecked, the effect is no longer computed at runtime

**Step Bar Improvements:**

- **Step Merge Update**: After selecting an existing step, click A+ to merge and update the selected step with current programmer values, only overwriting changed attributes without affecting existing values of other attributes
- **Multi-Step Batch Update**: Supports multi-select steps for simultaneous updating
- **Default No Selection**: The step bar has no step activated by default; A+ creates a new step in this state

### DMX Network Module

#### Full sACN (E1.31) Protocol Support

DMX now supports the **sACN (E1.31)** protocol, running in parallel with Art-Net as dual protocols, freely switchable in the configuration panel.

- **One-Click Protocol Switching**: Selecting sACN in the configuration panel automatically switches port and network mode without manual configuration
- **Multicast Auto-Subscription**: Automatically joins multicast groups for corresponding Universes; receives signals out of the box
- **Multicast Auto-Send**: Automatically sends to corresponding multicast addresses per Universe during playback
- **Multi-NIC Compatible**: Works with specified NIC IP or system default routing, unaffected by virtual adapters such as Hyper-V

### Upgrade Instructions

Upgrading from 26Q2.2 to 26Q2.5:

1. Direct overwrite installation is sufficient; project files are compatible
2. **Note**: Existing SuperNDIScreen assets in scenes need to be replaced with the new SuperScreen
3. **Note**: SuperScreen still defaults to NDI as the input method, matching old behavior, no additional configuration needed
4. **New**: Both screens and projectors support switching between NDI / Static Texture modes in the Details panel

---

## Version 26Q2.2

> Version: 26Q2.2
> Release Date: March 26, 2026
> Based on: 26Q2.1

### Procedural Crowd Generation System (New)

New `SuperCrowd` procedural crowd Actor that automatically generates non-overlapping crowds within closed spline areas, supporting terrain snapping.

**Core Features:**
- **Poisson Disk Sampling**: Bridson algorithm ensures minimum spacing (safety radius) between characters
- **Terrain Ray Detection**: Rays cast from above to capture ground height; character feet automatically snap to terrain
- **Multi Character Types**: Supports any number of character templates, randomly assigned by weight
- **ISM Component Pool**: Pre-created component pool for efficient rendering; not lost during Blueprint compilation

**Parameter Configuration:**
| Parameter | Function | Default |
|-----------|----------|---------|
| SafetyRadius | Minimum distance between characters | 60cm |
| TargetCount | Target crowd size | 50 people |
| RandomSeed | Random seed (0 = random each time) | 0 |
| bSnapToGround | Terrain snapping toggle | On |
| bRandomYaw | Random Y-axis rotation | On |

**Character Template Properties:**
- `Mesh`: Character static mesh
- `Weight`: Selection weight (auto-normalized)
- `Scale`/`ScaleVariation`: Base scale and random variation
- `FootOffsetZ`: Foot Z offset (for non-standard origin models)
- `BaseRotationOffset`: Base rotation offset (default facing Y- direction)
- `MaxRotationVariation`: Random rotation range (default ±15°)

### Default Parameter Migration Optimization

Component default parameters have been fully migrated to component-internal `OnRegister()` auto-initialization, simplifying fixture development workflows.

**Core Changes:**
- Default parameters moved from Actor level to component level; components automatically call `SetLightingMaterial()` and `SetLightingDefaultValue()` on registration
- `ComponentDimmer` renamed to `MaxLightIntensity` for naming consistency
- Removed `VisibleAnywhere` markers from components to avoid redundant editor display
- New `BeamAttenuationMode` toggle for independent control of beam view-angle attenuation

**Fixture Development Simplification:**
- When creating custom fixtures, just set component default parameters; no need to manually call initialization functions in the Actor
- Material creation and default value application automatically complete after component registration

### Bug Fixes

- **Fixed PIE crash due to beam view-angle attenuation**: Editor viewport state is unstable during PIE startup; switched to using player camera during PIE

### Procedural Truss Asset System (New)

4 new procedural truss structure Actors added, supporting parametric modeling and DIN 4113 load calculation.

**New Assets:**

| Actor | Function | Features |
|-------|----------|----------|
| SuperCircularTruss | Circular/arc truss | Adjustable angle, radius, segment count |
| SuperCurvedTruss | Curved truss | Bezier curve control points |
| SuperTrussTower | Truss tower/column | Multi-level columns, top flange |
| SuperTrussGrid | Horizontal truss grid | Lighting hanging system, Cross/Warren bracing |

**Technical Features:**
- ISM component efficient batch rendering
- Hash dirty-check mechanism (rebuild only on property changes)
- DIN 4113 aluminum structure load calculation standards
- Customizable materials
- Real-time statistics on member count/weight/load

### Full Localization Translation Support

All UI interfaces and asset property parameters now have complete localization translation support, supporting automatic Chinese/English switching following the editor language setting.

**Core Changes:**

- **Asset Metadata Localization**: `FAssetMetaData` fields changed from `FString` to `FText`, supporting multi-language
- **Asset Browser Translation**: Group/Manufacturer/DMXMode/DisplayName display translated text
- **UI Panel Text Collection**: All editor panels use `LOCTEXT` macros to ensure text is collectible for localization

**Technical Implementation:**

- Asset category tree and filters support `FText` comparison
- MvrImport/SSuperDataImport panels have localization namespaces added
- All Blueprint asset metadata fields unified to `FText` type

---

## Version 26Q2.1

> Version: 26Q2.1
> Release Date: March 19, 2026
> Based on: 26Q2.0

### DMX Laser Pattern System v3.0 (New)

LaserPatternGenerator comprehensively upgraded, supporting 30 preset patterns + 6 major automated animation systems.

**New Patterns (+14):**
- Heart / Infinity / Rose / Helix / Polygon / Atom / Vortex / Lightning / Butterfly / Grid / DNA / Starburst / Pendulum / Galaxy

**6 Major Automated Animation Systems:**

| Animation Type | Function |
|---------------|----------|
| Color Animation | Rainbow / ColorPulse / ColorFade |
| Position Animation | Orbit / Bounce / Figure8 / Sway |
| Size Animation | Breathing pulse effect |
| Pattern Cycle | Auto cycle through patterns |
| Strobe Overlay | 0-30Hz adjustable |
| Global Speed | MasterSpeed multiplier control |

**Technical Optimizations:**
- AddPoint precomputed CosA/SinA optimization
- DeterministicHash deterministic random
- Time overflow protection (3600s cycle reset)
- Fixed material parameter cache bug

### Festival FX System (New)

New FestivalFX Niagara effect system supporting festival and celebration scenarios.

**Effect Types:**
- Confetti
- Pyrotechnics (×3)
- Smoke (CO2/Smoke)
- Bubble

### SuperCAD PDF Export Fixes

- Fixed PDF export clipping border overflow
- Fixed annotation leader line sub-pixel disappearance
- Added 3pt margin to PDF pages
- Leader line width adaptive (prevent thin line disappearance)

### Light Function Atlas Optimization

- Enabled MegaLights support
- Disabled Rect/Matrix light functions (performance optimization)
- Corrected Atlas CVar semantics

### Other Optimizations

- **Beam View-Angle Attenuation**: Beam density reduced when viewed from the back (light-facing-away view); beams normal when facing the light source (light-facing-toward view), simulating real light perception
- Optimized lighting component brightness control
- Updated fixture profile configuration (Acme / Chauvet / EK / ETC)
- Removed deprecated rule files and old assets
- Updated plugin icon

---

## Version 26Q2.0

> Version: 26Q2.0
> Release Date: March 6, 2026
> Based on: 26Q1.1

### SuperCAD — Construction Drawing System (New Module)

Draw professional construction drawings directly within the UE editor, eliminating the pain of going back to CAD software to redraw after 3D previz.

**How to Open**: Window Menu → "SuperCAD - Lighting Plot"

**Core Features:**

- **Orthographic Viewport**: Plan / Front Elevation / Side Elevation / Rear Elevation, three rendering modes (Line/Shaded/Lit)
- **Automatic Fixture Info Display**: DMX address, fixture number, type, power, weight — drawings auto-update when the scene changes
- **Dimension Annotation**: Linear dimension + continuous dimension, auto-calculated real distances, snap alignment support
- **Text Annotation**: Freely place text descriptions
- **Leader Annotation**: Polyline annotation with arrow, pointing to fixtures or positions
- **Power/Signal Lines**: Point-by-point power and signal line path drawing
- **Area Annotation**: Polygon area marking with automatic area calculation
- **Box & Point Selection**: Window selection (left→right) and Crossing selection (right→left), Shift/Ctrl multi-select support
- **Layer Management**: Layer by category (fixtures/annotations/power/signals, etc.), independent visibility and color control
- **Multi-Page**: One document contains multiple drawings (lighting plot + power diagram + signal diagram)
- **Title Block & Border**: A0-A4 standard sheet sizes + custom dimensions, editable project info/designer/version number
- **Right-Click Menu**: Three context-sensitive menus (empty/fixture/annotation)

**Export Formats:**

| Format | Description |
|--------|-------------|
| **DXF** | AutoCAD compatible, importable into VW / DraftSight, layer-based output |
| **PDF** | Directly printable for delivery |
| **PNG** | Quick screenshot for sharing |

**Statistical Reports:**

- **Fixture BOM (Bill of Materials)**: Summarized by type: quantity, power, weight
- **Universe Utilization**: Fixture count, channel occupancy, free channels per Universe
- **Power Load**: Load summary by circuit, estimated three-phase current
- **Hanging Point Load**: Per-fixture weight and mounting height listing

**Data Management:**

- Construction drawings saved as project assets (.uasset), can be double-clicked in Content Browser to open
- Ctrl+Z / Ctrl+Shift+Z undo/redo support
- Automatically remembers last opened document

### Track Mechanical System Enhancement (New Feature)

SuperStage track mechanical system comprehensively upgraded.

- **Editor Real-Time Preview**: Preview track motion paths in editor mode without entering Play mode
- **Closed Track Smart Interpolation**: Circular tracks automatically select the shortest path for motion
- **Boundary Safety Protection**: Auto limit at track endpoint boundaries for out-of-bounds prevention

### Lighting Component Individual Dimmer (New Feature)

All lighting components now have a **ComponentDimmer** parameter (0~1), supporting independent brightness adjustment of individual components for multi-module fixtures.

- Multiplicative overlay with Actor-level Dimmer
- Applicable to: Spot / Beam / Rect / Effect / Matrix / LaserPro — all components
- Use case example: A fixture with multiple light-emitting modules, each module's brightness can be independently controlled

### SuperTruss — Procedural Truss System (New)

Place parametric trusses directly in the scene with real-time dimension adjustment.

- **Section Types**: Square tube / Triangle / Flat plate
- **Form Modes**: Portal / T-type / Archway / Double-span
- **Engineering Calculation**: DIN 4113 standard load calculation
- All parameters adjustable in real time

### SuperDrape — Procedural Drape System (New)

Place parametric drapes directly in the scene.

- **6 Drape Types**: Standard / Austrian / Brail / Contour / Venetian / Kabuki
- **4 Pleat Styles**: Box Pleat / Pinch Pleat / Pencil Pleat / Flat
- **4 Opening Modes**: Fly / Traveller / Austrian / Static
- Fabric parameter support (density/drape/elasticity)

### Laser Pro Component (New)

High-performance laser system based on procedural meshes.

- Beyond protocol real-time receiving support
- Sequencer recording and playback support
- UV depth attenuation + HDR brightness control + multi-layer fog effects + hotspot effects
- High performance: memory reuse + change detection skipping unchanged frames

### SuperDroneLink Production-Ready Fixes

3 rounds of deep review, total **17 issues fixed**, including:

- Drones unable to receive signals and control in PIE mode
- Memory safety, null pointer protection, resource leaks
- Thread safety, lifecycle management
- Boundary conditions, exception recovery

### SuperLaser Production-Ready Hardening

Code audit total **12 issues fixed** (P0×3 / P1×4 / P2×5), including:

- AsyncTask lambda use-after-free fix (AliveFlag guard)
- FCanvas null resource pointer crash protection
- Recursive lock, float overflow, SEH protection
- Sequencer time unit mismatch fix
- Linear search optimized to binary search, division-by-zero protection

### Strobe System Refactored

Strobe computation migrated from material shaders to component Tick; unified strobe logic across all light types.

- Performance improvement: avoid per-pixel material computation every frame
- Auto-disable Tick when strobe is off
- 6 waveforms: Linear / Pulse / RampUp / RampDown / Sine / Random
- LightStripEffect strobe synchronized and migrated to C++ computation

### Spot Auxiliary Light Matrix Control (New)

Added full matrix control for SuperStageLight's Spot auxiliary light: RGB / RGBW / warm-cool blend / Zoom / Strobe / Frost.

### Beam Occlusion System Refactored

Beam occlusion detection and attenuation radius update fully refactored, resolving GPU Scene crashes and multiple accumulation bugs.

- **GPU Scene Safety**: New `SetAttenuationRadiusSafe()` bypassing `PushRadiusToRenderThread()` to avoid same-frame conflicts with GPU Scene
- **Occlusion & Zoom Decoupling**: Occlusion detection runs at independent 30fps rate-limited, no longer dependent on Zoom trigger
- **Frost Accumulation Fix**: BeamComponent `SetLightingFrost` changed to use cached base value overlay, eliminating infinite accumulation
- **Defensive Checks**: `UpdateBeamBlockDistance` comprehensive NaN/Inf validation, clamping, debouncing

### Beam Prism Effect Enhancement

Prism activation now automatically increases beam and hotspot angles, reproducing real fixture prism scattering effects.

- Angle increment linearly correlated with prism radius (PrismRadius × 20°)
- Instant application, synchronized with prism state (non-linear gradient)
- Correctly handles simultaneous Zoom / Frost / Iris overlay scenarios
- Anti-de-jitter accumulation protection: prism angle remains stable under any call order

### Infinite Rotation Optimization

New Pan / Tilt infinite rotation matrix control supporting clockwise / counterclockwise / stop modes.

### SuperNDI Stability Fixes

NDI video stream module deeply fixed for improved runtime stability.

- MoveTemp timing fix to prevent frame buffer double-free
- bNDIReady guard to prevent resource access when not ready
- Resource leak fix + frame buffer performance optimization

### SuperDMX Production-Ready Hardening

Code audit total **14 issues fixed** (P0×4 / P1×3 / P2×7), including:

- Thread safety and concurrency protection
- Network protocol parsing robustness
- HttpService switched to production environment

### Entitlement System Refactored

Upgraded from single subscription to module-level entitlement control.

- Each module independently licensed (SuperStage / SuperConsolePro / SuperDroneLink / SuperLaser / SuperNDI)
- AES-256-GCM encrypted token decryption, security upgrade

### Fixture Profile Editor (New)

MA2/GDTF standard-based fixture profile data asset editor.

- 13 DMX attribute categories
- Three-level navigation: Attribute → Sub-Attribute → Slot
- Multi-module / matrix fixture configuration support
- Multi-select operations: add, delete, copy, paste, sort

### DMX Start Universe Setting

Resolves MA2 to UE Universe numbering alignment issue.

- Default changed to 0: MA2's 1.1 directly maps to UE's 1.1, no manual offset needed
- Legacy project compatibility: to restore old behavior, set start Universe to 1

### SuperConsole Module Removed

SuperConsole basic console has been fully replaced by **SuperConsolePro**. All old module code removed in this version. No migration needed; SuperConsolePro inherits and extends all functionality.

### Full Module User Manuals

New comprehensive user manual documentation covering all modules:

- SuperStage — Lighting System & Scene Management
- SuperConsolePro — Console Operation Guide
- SuperDroneLink — Drone Formation Control
- SuperLaser — Laser System
- SuperNDI — NDI Video Streams
- SuperStageEditor — Editor Tools

### Developer API Documentation

7 new developer technical documents (Source/SuperStage/DevDocs/):

- Architecture Overview — Module layering and data flow
- SuperDMX API — Network communication and caching interfaces
- SuperDmxActorBase API — DMX data reading and channel mapping
- SuperLightBase / SuperStageLight API — Lighting Actor interfaces
- Other Actor APIs — Laser/NDI/Projection/Drone, etc.
- Component APIs — Spot/Beam/Rect/Effect/Matrix/Laser component interfaces
- Fixture Development Guide — Complete workflow for creating custom fixtures from scratch

### Toolbar Menu Optimization

- New Modules submenu for centralized SuperCAD / SuperConsolePro entry management
- Removed redundant SuperUI entries from Window menu

### Bug Fixes

- Fixed SuperSpot property name conflict
- Fixed Spot auxiliary light parameters inconsistent with main light source
- Fixed strobe algorithm inconsistency on matrix components
- Fixed fixture initialization functions not being called
- Fixed PatchTool fixture sorting: Select All now sorts naturally (C_1 < C_2 < ... < C_10 < C_100)
- Fixed SuperConsolePro attribute wheel and property bar incomplete display; removed hardcoded attribute names

### Upgrade Instructions

Upgrading from 26Q1.1 to 26Q2.0:

1. Direct overwrite installation is sufficient; project files are fully compatible
2. **Note**: Strobe parameters have been removed from materials; custom materials need to be updated synchronously
3. **Note**: SuperConsole module has been removed; please use SuperConsolePro
4. **Note**: Entitlement system has been refactored; old subscription tokens need to be re-obtained
5. **New**: SuperCAD Construction Drawings → Toolbar Modules menu → SuperCAD
6. **New**: SuperTruss / SuperDrape → Place directly in the scene
7. **New**: Track Mechanical Editor Preview → Preview motion paths directly in editor mode
8. **New**: Full module user manual documentation

---

---

---

## Version 26Q1.1

> Version: 26Q1.1
> Release Date: February 3, 2026
> Based on: 26Q1.0

### SuperDroneLink Drone Link Module (New)

Newly launched **SuperDroneLink** drone lighting control module, supporting drone formation lighting control via LDLink protocol.

**Core Features:**

- **LDLink Protocol Support**: Real-time drone position and status data reception
- **DroneSwarmManager**: High-performance drone swarm manager using HISM optimized rendering
- **SuperDroneActor**: Single drone Actor supporting independent control
- **Sequencer Recording**: Supports recording drone trajectories to LevelSequence
- **Activity Monitor**: Real-time display of drone connection status and data flow

**Technical Features:**

- Linear interpolation for smooth motion
- Automatic unit conversion (meters → centimeters)
- NIC selection and auto-discovery support
- Inherits ASuperBaseActor unified architecture

### Lighting Control Enhancements

#### Color Mixing System

**New Color Mixing Functions:**

- `SetLightingColorRGBW`: RGBW four-channel color mixing
- `SetLightingColorRGBWWithCTO`: RGBW + CTO color temperature mixing
- `SetLightingColorMixWithCTO`: Multi-channel mixing + CTO color temperature adjustment
- `SetLightingColorHSAndCTO`: Hue saturation + CTO control
- `SetLightingColorHS` / `SetLightingColorHSV`: HSV color control
- `SetLightingColorWheel2AndCMYAndCTO`: CMY + CTO color temperature filtering

**Matrix Component Extended Color Control (New):**

- RGB + CTO / RGBW / RGBW + CTO
- RGB + CoolWarm / RGBW + CoolWarm
- CoolWarm blend / Multi-color blend (`SetMatrixColorMix`)

#### Gobo Control Optimization

**Gobo Shake Function (New):**

- **EGoboMode Enum**: Static, Scroll, Shake
- Material-side implementation: shake amplitude 25 degrees, speed 0-10Hz
- Chinese keyword support: recognizes "shake", "swing" and other keywords

**Gobo / Prism Rotation Separation:**

- Separated gobo rotation and prism rotation
- `SetBeamCuttingRotate` only controls Shaper + Gobo
- All GoboModes calculate GoboIndex

#### Rotation Speed Control Optimization

- Physical range control: PT Speed, gobo / prism / shaper rotation use physical ranges
- Infinite rotation mode: New `EInfiniteRotationMode` enum

### Effect Component Enhancements

**Strobe System Fixes:**

- Fixed matrix component segment light source strobe algorithm matching materials
- Fixed matrix strobe mode index mapping issue
- Initialized Brightness and StrobeMode parameters

**Effect Speed Control:**

- `SetEffectWithSpeed`: Effect + independent speed channel
- `SetEffectMatrixWithSpeed`: Matrix effect + speed channel

### Tools & Optimizations

- Color / Gobo atlas generator naming rule optimization
- Color wheel scroll effect fix
- Fixture profiles changed from Actor inline to data assets
- AssetBrowser scans Blueprints by class, synchronous warmup at startup
- DroneSwarmManager HISM performance optimization

### Bug Fixes

- Fixed Iris aperture DMX value inversion and minimum value restriction
- Fixed FrameSubsystem using IPluginManager to find plugin path
- Fixed PatchSubsystem always updating Modules data when ScanScene
- Fixed TemperatureToRGB forward declaration order
- Fixed CalcGoboRotation call parameter in SetBeamPrism

### Commit Summary

| Category | Count | Description |
|:---------|:-----:|:------------|
| New Features | 25+ | Drone module, color mixing system, shake effects, etc. |
| Bug Fixes | 20+ | Strobe, color wheel, index mapping, etc. |
| Optimizations & Refactoring | 15+ | Performance optimization, code refactoring |
| Fixture Assets | 10+ | New / updated fixture configuration files |

### Upgrade Instructions

1. Direct overwrite installation is sufficient
2. Project files fully compatible; no migration needed
3. Recommended to recompile project to obtain all new features

---

## Version 26Q1.0

> **Mandatory Update**
> Release Date: January 20, 2026

### Important Notice

This is a **mandatory update** version. Due to a complete server-side refactoring, old versions will be unable to connect to the server.

- Users who do not update will be unable to use the plugin normally
- This update will not affect completed or in-progress project files

### Core Architecture & Version Changes

- **Domain Change**: Official website and plugin API domain migrated from `ue.yunsio.com` to `yunsio.com`
- **New Version Numbering**: Deprecated the original 1.x/2.x format, adopted Year/Quarter naming (e.g., 26Q1.0 = 2026 Q1 Release 0)
- **Update Schedule**: Continuing with quarterly major version iterations

### New Features & Optimizations

**Lighting & Effects:**

- High-Performance Laser (Beta): New laser system supporting higher-performance real-time rendering
- Lift Matrix Lights: Support for lift matrix lights and lift drone matrix lights
- Advanced Color Mixing: Support for RGBA / RGBL / RGBALM and other professional color modes
- Deep fixture performance optimization, smoother large-scale lighting scenes

**Tools & Systems:**

- DMX Camera: Camera parameters and motion controllable via DMX signals
- Complete server-side refactoring, stricter authentication mechanism, more stable login connection
- NDI compatibility improvements

---

## Version 2.1.3

> Release Date: December 16, 2025

### SuperConsolePro Professional Lighting Console (New)

Newly launched **SuperConsolePro** professional lighting console system, providing a complete lighting programming and playback solution for stage lighting, live performances, and virtual production.

**Core Features:**

- Modular subsystem architecture: 15 independent subsystems working together
- Professional programming workflow: Programmer, Presets, CUE, and other concepts
- Real-time DMX output: latency < 16ms
- Timeline editor: CUE tracks + audio tracks multi-track editing
- Frame effect engine: Sine, square, sawtooth, and other waveforms
- Flexible window layout: multi-window free layout, 10 preset slots
- Show file management: .ssshow format save/load

**Main Feature Modules:**

| Layer | Modules |
|-------|---------|
| Data Management | Show File, Patch, Attribute Subsystems |
| Programming Workflow | Selection, Programmer, Encoder, Preset Subsystems |
| Playback Control | CUE, Playback Runtime, Shortcut Key Subsystems |
| Advanced Features | Timeline, Frame Effect, Layout Subsystems |
| Output Control | Console DMX, Control Bar, Timecode CUE Subsystems |

**UI Panel System (12+ Panels):**

Fixture Sheet, Playback, Presets, Timeline, Frame Editor, Layout, Color Picker, Groups, Patch, Running Playback, DMX Monitor, Settings

---

## Version 2.0.0

> Release Date: December 6, 2025

### Important Changes

- **Deprecated launcher**, switched to cloud drive downloads (Baidu / Quark / Google)
- Changed to in-UE-editor authentication panel using email verification code login

### Major Updates

- Seamless UE 5.7 compatibility
- New perpetual license 3-year update, single-version perpetual subscription, source-code commercial version

### SuperData Protocol

Launched **SuperData Protocol**, supporting real-time lighting device data exchange across multiple software platforms:

- Supports UE, Unity, VW, MA2, Blender, C4D and other software
- Uses network communication technology, no file export needed

### SuperStage Multi-Version Plan

| Version | Status |
|:--------|:------:|
| SuperStageForUE | ✅ Updated 2.0 |
| SuperStageForLimx | Under development |
| SuperStageForUnity | Internal beta |
| SuperStageForVW | ✅ Launched (Free) |
| SuperStageForMA | ✅ Launched (Free) |
| SuperStageForBlender | Planned |
| SuperStageForC4D | Planned |

### SuperStageForUnity (Internal Beta)

- Plugin architecture, usage, and parameters largely identical to UE version
- Supports stage DMX devices, mechanics, light strips, SuperData protocol
- Not yet supported: laser lights, NDI, Super built-in console

### Super Built-In Console (Preview)

- Auto Layout for scanned fixtures
- Fixture group management / CUE list / built-in effect system
- Keyframe effect system (start-end frame programming)
- Timeline system (audio-light sync, convertible to Sequence rendering)

### Authentication & Authorization Optimization

- Login state retainable for up to 7 days
- Login state not lost on network connection failure
- Added reconnection mechanism on failure

---

## Version 1.2.1

> Release Date: November 2025

### Updates

- Deprecated CSV / XML export-import functionality
- Removed Blueprint encryption restrictions
- New Beyond laser, connection, and rendering modules
- Fixed authentication bugs, memory leaks, and background process bugs
- Fixed prism rotation issues, optimized all standards compliance
- New Windows platform packaging support (all users can package)

> Before updating, manually delete the old plugin directory

---

## Version 1.2.0

> **Official Release**

### Open Content

- All fixtures unlocked
- DMX one-click patch, patch preview, export to MA2, CSV export tools unlocked
- All stage assets and effects unlocked
- NDI screen connection and rendering module unlocked
- VWXml / Mvr import tools, audience assets unlocked
- Total **118 assets**

---

## Version 1.0.0

> Release Date: September 20, 2025

### First Release

- SuperStage plugin installer first release
- Supports Unreal Engine 5.6
- Automatic UE installation path detection from Windows Registry
- Multi-language support (English and Chinese)
- Wizard-style installation interface
- Plugin installed to `[UE Path]\Engine\Plugins\Marketplace\SuperStage`

---

**Thank you for using SuperStage! For questions, visit [yunsio.com](https://yunsio.com) or email yerrkj@outlook.com**
