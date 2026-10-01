# SuperStage Changelog

This document records user-visible changes for the current documentation set. It only keeps information that can be confirmed from the current plugin structure, source tree, or user-facing tool entries. Unverified performance claims, internal development notes, and removed-module marketing text have been removed.

## 26H2.6

This release has three main threads:

- **Fixtures moved from Blueprints to data-driven definitions.** A fixture is no longer a Blueprint asset but a fixture definition that can be imported, edited, and upgraded. The GDTF import pipeline, the in-house light source system, and the fixture library content were rebuilt around it.
- **Beams moved to the volumetric pipeline.** The shipped library was migrated wholesale from material beams to volumetric beams. The beam column and the floor pool share one shadow atlas and are genuinely occluded.
- **Two new content systems:** stage fountains and laser animation assets. Lasers were extended this release from "can bake" to a complete authoring tool: canvas editing, six effect categories, keyframes, and ILDA read/write.

Automated regression tests pass on the release build.

### Plugin Structure Update

- The main SuperStage plugin currently contains: SuperCore, SuperDMX, SuperNdi, SuperMadrix, SuperLaser, SuperShader, SuperAuth, SuperAssets, SuperTools.

### Fixture System Rebuild: Data-Driven Fixture Definitions

- Three new asset types replace the previous "one Blueprint per fixture" approach:

  | Asset | Description |
  | --- | --- |
  | Super Fixture Definition | The complete definition of a fixture: identity, physical data, mechanical axes, emitters, optical elements, DMX modes. |
  | Super Fixture Library | Channel library (the fixture's DMX personality): modes, attributes, subattributes, channel sets. Can be shared by several fixtures. |
  | Super Prism Preset | Prism preset: three independent prism configurations, each describing how the beam is split. |

- All data-driven fixtures share one Actor class, and the model is determined by the definition asset. Everywhere that needs to distinguish "model" (Asset Browser categories, patch naming, MA sync grouping, channel grid color coding) now decides by channel library asset instead of by class name.
- Fixture Actors declare the definition assets they reference to the editor. Right-clicking a fixture in the scene edits or locates its definition directly.

#### Hardening in This Release

- **All fixtures dark after a plugin upgrade**: the compiled product is saved alongside the definition asset, so after a version bump the disk still held the old version number while the source data had not changed. The system decided no rebuild was needed, then failed silently at registration because the versions did not match. All three points are now closed, and the debug panel states that the product is from a previous version instead of reporting only `Registered: NO`.
- **Channel-set lookup off by one set on 16-bit channels**: the range table is stored in the 8-bit domain, while the evaluation side went through float normalization and then multiplied by 255 and truncated. The two are exactly equivalent at 8 bits and not equivalent at 16/24 bits, so single-value channel sets could never be selected.
- **Five additive LED channels had no binding rules**: the channels imported, held addresses in the patch table, and could be pushed from the console, but drove nothing. Rules were added, along with an audit case that runs the derivation over real assets and lists unbound attributes by frequency.
- Material instance writes on the hot path are now cached.

### GDTF Import

- Added a complete GDTF import pipeline. A single manufacturer package produces a usable fixture: channels and modes, geometry tree and mechanical axes, 3D models, gobo/color/prism wheels, thumbnails.
- **Selective import scope**: nine items grouped by channel / geometry / optics / identity. On re-import only the selected parts are replaced, so hand-adjusted content is not overwritten.
- **Batch import panel**: select a set of GDTF packages at once and build fixtures in bulk according to library conventions, handling overwriting existing assets, name collisions, and source paths.
- **Upgrade in place**: fixture definitions offer a right-click "Upgrade in place from source GDTF", picking up later importer improvements without a re-import. By default it only reports differences and does not modify assets.
- Models support `.3ds` and `.glb` as fallbacks, normalized to the dimensions declared in the package. Fixtures with no model get a placeholder body generated from the GDTF PrimitiveType, so they are no longer bodiless.
- ModeMaster conditional channels are supported; ranges are grouped by condition.
- Zip entry names are decoded as UTF-8, so models with non-ASCII names are no longer lost.
- GDTF export restores mode and function names, so round-trips no longer lose data.
- Added GDTF library download and categorization tools. The import smoke test was upgraded into a full-library auditor supporting mode-only parsing, exception aggregation, and collision statistics.

### Fixture Library Content

The Asset Browser now holds **869 placeable assets** in five categories:

| Category | Count | Description |
| --- | --- | --- |
| StageLight | 807 | Fixture library, imported from GDTF, 54 manufacturers |
| StageModel | 18 | Scenic structures: truss, scaffolding, stage decks, seating stands, barriers, drapes, cable trays, audience, and more |
| SuperVFX | 18 | Effect devices: smoke, fire, snow, bubbles, streamers, pyro, fountains, pools |
| SuperLight | 16 | In-house fixtures: Aurora / Blaze / Flare / Hyperion / Machinery / Spark / Thunder, seven series |
| SuperStage | 10 | Stage devices: screens, projectors, lasers, LED tape, broadcast cameras, lift and track machinery, Madrix, blockout models, and more |

- The fixture library portion covers **54 manufacturers and 807 models**, with 2884 channel libraries and 413 prism presets, backed by 904 retained GDTF source packages.
- Manufacturers with the most models: Chauvet Professional (71), Prolights (69), Robe (68), CKC Lighting (60), Elation (58), Ayrton (42), ACME (38), Terbly (38), Cameo (35), Martin Professional (35).
- Manufacturers added or migrated this release include: Ayrton, Robe, Martin Professional, Chauvet Professional, Elation, ACME, Clay Paky, JB-Lighting, Prolights, ADJ, CKC Lighting, Terbly, LightSky, High End Systems, Vari-Lite, EK Lights, Cameo, DTS Lighting, CINDY, GLP, Spotlight, DLW, Infinity, Starway, Showtec, MegaLite, MARK, ARRI, and others.
- PARs, blinders, and strobes were bulk-imported from a CSV list, with the import scripts and source-path conventions added to the toolchain.
- The fixture library directory was corrected from `GTDF` to `GDTF`. Source paths recorded on existing assets were corrected in bulk and normalized to plugin-relative form.

### In-House Light Source System

- Added **SuperConeLight**, an in-house cone light pipeline replacing the UE SpotLight. All three beam pipelines were moved onto it, saving one engine light and one light function material instance per fixture.
- Gobos, color wheels, framing shutters, and prisms are all projected by the cone light. The beam column and floor pool share one set of optics and one time base, so their color and angle no longer disagree.
- Exit aperture glow comes from the beam shader, with no extra primitive required. The aperture is treated as a disc, so it no longer turns with the camera or punches through the fixture body.
- Multi-head beams support independent Pan / Tilt per head. Emitters gained circular layout and XY scaling of the lens model; emitters with no lens model get a generic default part scaled automatically to the exit aperture radius.
- Fixture body models are out of physics and navigation. Arrows, address text, hooks, and other debug and decorative primitives were removed along the whole chain.

#### Volumetric Beams Become the Primary Pipeline

In the previous release the volumetric beam was still a test component. In this release it is the default backend for aerial beam columns, and the library has been fully migrated.

- **Full library migration**: the shipped library's emitters moved from material beams to volumetric beams, and profile fixtures with framing shutters moved to volumetric shaper beams. After migration there are no material-pipeline fixtures left in the library.
- The beam type dropdown was reordered by purpose: volumetric first, no-column types in the middle, the old material pipeline last and marked **Legacy**. All enum values on existing assets are unchanged, so saved content is unaffected.

  | Type | Use |
  | --- | --- |
  | Volumetric Beam | Volumetric beam plus aerial column. Current default. |
  | Volumetric Shaper (Profile) | Volumetric beam plus four-blade framing, for profile fixtures. |
  | Ray Beam (Matrix / FX) | Pure beam, no optical elements, for matrix and effect fixtures. |
  | Wash (Color / Blinder / Strobe) | Wash / blinder / strobe. |
  | Spot Only (No Column) | Cone light only, no aerial column drawn; the floor gobo is unaffected. |
  | Pixel Matrix | Pixel matrix fixtures. |
  | Effect Plane (LED FX / Magic) | Effect planes. |
  | Material Beam / Shaper (Legacy) | Old material pipeline, kept for fallback, not used for new content. |

- **Occlusion**: the cone light and volumetric beam share one shadow atlas and one sampling function. Previously only the beam column was occluded and the floor pool was not, which appeared as "the beam is blocked but the pool is not".
- **Fixtures visible on screen always have shadows**: the shadow slot allocation that queued by camera distance was removed, so "no shadow when far from the camera" no longer occurs.
- **Haze flow speed** is exposed as a parameter (HazeSpeed, cm/s), decoupled from wisp scale. Previously the flow speed was locked to wisp size and too slow to be visible.
- **Gobo edges**: per-segment footprints changed to anisotropic sampling, fixing soft-edged gobos turning into hard angular edges at side camera positions. Dithering is on by default to suppress concentric rings.
- The shaper shader and the beam column itself are genuinely separated: a volumetric beam without blades compiles with no blade instructions at all.

### Optical and Physical Correctness

- **Color**: additive channels use manufacturer-measured emitter chromaticity, subtractive channels use manufacturer-measured filter chromaticity. A ColorTint opcode was added for plus/minus green correction. Fixtures carry a native white point, so an 8000K beam fixture is no longer the same color as a 3000K tungsten unit.
- **Strobe**: the physical quantity is interpreted in Hz, with the range taken from declared values.
- **Color temperature**: endpoints now come from the Kelvin values declared in the library and are read per range, instead of one fixed range for everything.
- **Main axis mechanical ranges**: never imported from GDTF before, which made tilt travel incorrect on the vast majority of fixtures. Now imported from the declaration in the package, with fallback by axis role when missing (pan ±270°, tilt ±135°).
- **Continuous rotation**: default values no longer rotate continuously — a fixture placed with no DMX input has its wheels and prism at rest.
- **Framing shutters**: blade order follows the GDTF spec (1 top, 2 right, 3 bottom, 4 left). Blade edge ends, boundary ordering, and rotation ranges were corrected. Blade assembly rotation is separated from gobo rotation. Insert-and-rotate style framing fixtures are no longer cut into a crooked diamond by default.
- **Default values** are now taken from the initial value declared in the library, so fixtures no longer arrive in the scene with the shutter fully closed.
- Per-cell dimming works on matrix fixtures. GDTF BeamType is read, so rings and backplates no longer emit beam columns.
- **Prism layout** is now a live parameter (facet count / radius / facet size adjustable), with position normalized across the whole fixture. Added a **concentric ring layout** with any number of rings, each ring configurable for facet count, phase, size multiplier, and intensity, with per-ring position scaling. Previously there was only one row of fixed preset buttons; multiple rings required placing each facet by hand and could not be spaced evenly.
- Internal naming changed from Cutting to Shaper throughout, with redirectors provided to preserve existing assets.

### Fixture Editor

- Added the fixture editor workbench, using a single viewport plus the stock UE viewport toolbar, in four tabs: Fixture (definition), DMX Test (per-channel fixture test), Channel Map (address occupancy), Validation (check results).
- **Fixed beams and lasers not rendering in the panel**: the preview viewport uses a separate EditorPreview world, and both subsystems' world type whitelists omitted it, so the render chain could not proceed a single step.
- The channel library editor is wired into the undo system, with three-level lists storing index paths.
- Added "Compile" to the Content Browser right-click menu, with multi-select batch support.
- Asset naming normalization: non-ASCII characters, consecutive underscores, and leading/trailing underscores are stripped.

### MVR

- Rewritten to the MVR 1.6 spec as a complete read/write library.
- Fixed export: files previously generated were completely unreadable due to matrix syntax, DMX address, and uuid problems.
- Import retains GDTFSpec / GDTFMode and lists the model × DMX mode combinations in the library for selection, preselected from the GDTF information.

### grandMA

- MA3 sync sends the DMX mode (MA2 has no such capability). Fixture types and modes are mounted by index and read back for verification.
- MA3 sync groups in layers by model plus mode, instead of cramming several modes into one Grouping.
- Synced mode names use the same printable-ASCII cleaning rules as import.
- **Fixed MA2 not finding new fixtures**: when the MA2 panel scans the scene it groups by C++ class and takes the model from the class name. Under the old architecture one model was one class, so this was correct; data-driven fixtures are all the same Actor class, so the whole scene collapsed into one row, the library path was taken from the first fixture encountered, and other models were sent into the MA2 patch as that one. Now grouped by channel library asset.

### Tool Panels

- **Added the help system**: all 14 tool panels have a help button in the top-right corner that opens the instructions for that panel. The help text is included in the localization dictionary.
- **Asset Browser rebuilt**: search now matches model, manufacturer, and fixture type at once, with space-separated terms. Added the three entry points "All / Favorites / Recent" with per-node counts. Hovering a tile shows the full model, manufacturer, type, and DMX mode count. Added tile scaling and a result count. Right-click supports place, replace, favorite, locate in Content Browser, and open fixture definition; double-click places directly. The list updates automatically as assets are imported and deleted.
- **Asset Browser: dragging a fixture into the scene no longer loses the browse position**. The drag records a "recently used" entry, which rebuilds the category tree; the tree widget records both selection and expansion by node object, so a rebuild that swaps objects loses both, appearing as the page snapping back to All Fixtures with every sidebar group collapsed. Category nodes now reuse the same object across rebuilds and both are preserved. Clicking empty space in the sidebar still returns to All Fixtures, which is intended behavior.
- **Patch Tool**: the candidate list now follows the scene selection (previously patching a second batch back to back still acted on the first). Fixtures that Apply will skip are flagged before it is pressed. ClearPatch no longer resets the IDs of all selected fixtures. Inline table edits are all undoable and recompute conflicts immediately.
- **Channel grid**: cross-universe drags are no longer discarded; group drags keep their universe offsets instead of collapsing into one universe; conflicts at the drop target turn red during the drag and are refused on commit; dragging in from the Outliner automatically skips occupied ranges and no longer changes existing IDs; color blocks are coded by channel library and changed to a liquid glass look.
- **DMX configuration panel**: removed the sACN multicast count input; the protocol, network adapter, and start universe inputs were narrowed.
- Prism presets, channel libraries, and fixture definitions no longer share the product logo and each have their own icon.
- Fixed several panels that could not be translated due to a missing localization namespace.

### Fountains

Added the **SuperFountain** stage fountain system, controllable over DMX and sharing the same optics as the lighting.

- Nozzles are components on the fountain device. **The pool is a separate scenic Actor**, and neither depends on the other: if you only want a pool, place a pool, with no need to place a fountain first.
- Five-layer decomposition: coherent jet (analytic ballistics plus breakup length), breakup droplets (GPU particles), aerated white water, pool ripples (height field), water surface rendering (analytic intersection, no mesh).
- Physics calibrated against manufacturer samples: exit velocity derived from pressure (0.2 MPa gives 20 m/s), droplet size converging by Weber number over a wide distribution.
- Underwater lights are in candela, the same convention as the projectors. Scattering from stage lights on the water mist uses the same phase function as the beams.
- The motion blur shutter follows frame duration by default, so a change in frame rate does not smear droplets into continuous streaks.
- All parameters are adjustable from both the Details panel and console variables, with 36 automated tests.

### Lasers

Lasers were extended this release from "can bake" to a complete authoring tool.

**Assets and Runtime**

- **Laser animation assets**: created in the Content Browser and opened by double-click, with a baker, SVG importer, editor, and Sequencer track alongside.
- **Vectors are stored, not point clouds**. The asset holds Bezier outlines and modulation parameters, and the point stream is computed at playback, making it resolution independent, editable at any time, and free of the need to redo content when scanner parameters change.
- **There is no fixed duration**. Motion comes from the frequency of the effects, and the periods of different effects are unrelated. Any point in time can be evaluated directly, so scrubbing, jumping, and reverse playback need no warm-up.
- One animation can be sent to several devices. Previously, pressing "+" to add a device in the Details panel produced an entry that collided on default values, was treated as a duplicate, and was deleted on the spot, appearing as "only one can be added" with no error.
- Sequencer bakes per device.

**Canvas Editor**

- Node-level editing: edit Bezier control points directly, marquee and multi-select, grid snapping, transform handles (drag to rotate and scale), convert to path.
- Canvas pan and zoom, framing to a device, projection area display.
- Layers can be copied and pasted across assets. The clipboard moved to Base64 encoding, so multi-line text layers are no longer cut in half.
- Deleting several nodes merges into one undo. Switching layers clears edit state bound to indices.
- The layer panel exposes fields that were previously hidden; layer background colors were reduced to solid colors.
- **Text layers** take Bezier curves directly from font outlines, with no bitmap in between.
- Curve subdivision now genuinely takes effect.
- The corner marker changed from decoration to information that genuinely affects the bake.

**Effect System**

The effect categories were reworked to follow the conventions of professional laser software, in six kinds:

| Category | Description |
| --- | --- |
| Oscillator | Continuous oscillation, waveforms including accelerate / decelerate / ping-pong / step / random landing points |
| Keyframes | Keyframes, each effect carrying its own local timeline with a draggable curve editor |
| Color | Color stop line × application scope × blend mode |
| Points | Point-level operations: repeat points, anchor points, fixed-count beams, beam collapse. Laser only |
| Filter | Cleanup: even spacing, decimation, soft ending, removal of long blanking travel |
| Device Chase | Device chase, with fades between steps |

- Colors are no longer rainbow across the board: segmented mode gives two- and three-color splits, Brightness Only leaves hue completely alone (the only mode usable on a single-color laser), HueShift offsets from the original color, and speed defaults to 0, meaning static.
- Run modes filled in: one-shot, bounce, random landing points, custom waveforms. The waveform dropdown lists 2/4/8/16 steps and 2/3/4/8/16 random landing points directly.
- Each effect has its own center point; the effect chain shows a summary.
- Key gained position and dual-axis; Oscillator gained path shapes.
- Scope and Strength are exposed only on the effects that actually use them, rather than on all effects.

**Timeline**

- Added a Timeline tab spanning the full width at the bottom. Previously arranging content required reading numbers out of a list and computing the differences.
- Adaptive second ticks plus a beat grid drawn from Tempo, with a bar line every 4 beats.

**ILDA**

- Complete ILDA read/write, covering the various formats and malformed-file cases.
- A separate validator command line was written to the IDTF14 rev011 spec, trusting the spec rather than the in-house reader. A round-trip test is blind to the reader and writer making the same mistake, and byte order, color order, and Y-axis direction are exactly the things most likely to be wrong on both sides at once.
- The export device is now set explicitly. The frame count is shown before export is pressed. Empty frames no longer scramble the frame numbering.

**Baking and Previs**

- The baker respects galvo physics: the point budget is a hard ceiling, dwell points are allocated to corners by severity, dwell is added before and after blanking, and there is ordering optimization and color deskew. Frame rate is a derived result, and the interface states the verdict directly.
- Authored content and baked content both go through the scanner simulation, so the two sources no longer look different in previs.
- The transport bar now wraps: previously controls past the canvas width were clipped by Slate and not drawn, putting ILDA export, grid, snapping, blanking, and scan point controls completely out of reach with no indication. Import and export moved up to the asset editor's main toolbar.
- Added a pattern picker and a layer inspector panel.
- The laser multicast device limit was raised from 4 to 60, and the join operation moved to the receive thread.

### Interface Language

- Property names, enum dropdown entries, and **tooltips** in the Details panel moved to English, with Chinese supplied by the engine localization dictionary.
- Console variable help text moved to English, covering the cone light, beam, volumetric beam, and fountain pipelines.
- The localization gather configuration had the metadata gathering step added. Property names and tooltips had never entered the translation flow before.
- This release fills in the translations that were previously missing.

### Quality Assurance

- Automated regression tests pass, covering:

  | Area | Groups |
  | --- | --- |
  | Laser animation | LaserAnim (42) |
  | Fountains | Fountain (36) |
  | Fixture pipeline | Fixture (17), FixtureTools (17), FixtureActor (10), FixtureLibrary (2), Prism (7) |
  | Other | Mvr (11), AssetBrowser (4) |

- The GDTF import smoke test was upgraded into a full-library auditor supporting mode-only parsing, exception aggregation, and collision statistics.
- Added the separate ILDA validator command line.

### Rendering

- **All distance culling disabled**. Fixtures the camera could see previously disappeared once they passed a distance threshold; that behavior has been removed.
- Shader comments rewritten throughout.

### Migration Notes

- **No fixture class redirectors for old projects**. The fixture asset structure and naming changed substantially this release. Existing projects using the old Blueprint fixtures should stay on the old plugin and not upgrade in place. The only redirectors kept are the Cutting → Shaper set, covering component classes, material nodes, enum values, and Blueprint functions that current assets store by name.
- Fixture definitions already imported can pick up this release's importer improvements through the right-click "Upgrade in place from source GDTF". By default that operation only reports differences and modifies nothing until confirmed.
- The fixture library is fully migrated to the volumetric beam pipeline. Material Beam and Material Shaper remain in the dropdown marked Legacy, for fallback use only, and are not recommended for new content.
- The fountain water surface requires a pool Actor in the scene to render; the fountain device does not carry a water surface of its own.

## 26H2.5

### Plugin Structure Update

- SuperMadrix and SuperLaser have been merged from standalone plugins back into the main SuperStage plugin, and are now runtime modules of the main plugin. Enabling SuperStage is enough; those two plugins no longer need to be installed or enabled separately.
- The main SuperStage plugin currently contains: SuperCore, SuperDMX, SuperNdi, SuperMadrix, SuperLaser, SuperShader, SuperAuth, SuperAssets, SuperTools.

### New Rendering Features

- Added the white-model render actor `SuperWhiteModelActor`. Dropping it into a level renders the whole world as a white model; the details panel toggles and tints it, and deleting the actor restores the original view. It applies in editor viewports, PIE, and Movie Render Queue.
- Volumetric beams no longer add their additive pass in debug visualization views.

### Laser System (SuperLaser)

- Rewrote laser mesh generation around ILDA pen semantics: blank points lift the pen, same-position dwell points collapse into nodes, and cross-figure jumps are disconnected automatically. This fixes points previously being connected or separated incorrectly.
- HotBeam presentation: dwelling beam points now render as constant-width beam pillars with a gaussian hot core and dedicated gain, visible from any angle. Beam shots are noticeably brighter and more solid.
- Fixed flicker and brightness pulsing while playing animated laser content (several defects in the geometry update path and the scattering phase function).
- Fan sheets gained stroke-based dwell shading (slow segments and corners read brighter), world-space smoke, and scattering-phase modulation for a more realistic look.
- Reworked impact spots: hits render as camera-facing glow sprites (blazing core plus soft halo). Surface-hugging flat quads that clipped complex receivers such as trees and screens are gone.
- Surface hit patterns: connected line graphics project as crisp lines on walls and floors (requires collision to be enabled).
- Added MirrorX / MirrorY switches, implemented as render-mesh scaling without touching the laser point data. The default orientation now matches the preview in common laser software.
- Scanner simulation now defaults to off (its interpolation and inertia smoothing corrupt HotBeam dwell semantics). It can be re-enabled in settings.

### Laser Pattern System (SuperLaser sub-module)

- Added 7 global pattern motion effects: Sweep, Pulse, Orbit, Figure8, SpinPulse, Drift, and Bloom. Motion mode is decoded from Ch16 Bit2-4 and applied at the pattern-generation entry point — all 29 patterns benefit automatically.
- Added DMX color animation: Ch16 Bit5-6 supports Rainbow, Spectrum, Fire, and Cycle color animations, driven automatically in DMX mode.
- Extended Ch16 channel: previously only Bit0-1 controlled mirror; now Bit0-1 mirror + Bit2-4 motion + Bit5-6 color animation.
- Ch5/Ch6 position mapping now uses 128 as the centre dead zone with asymmetric slopes on each side, matching console feel.
- Ch8 rotation speed dead zone: changed from ±1.5 tolerance to exact ==128 check, eliminating spurious rotation triggers.
- MirrorX/MirrorY are now applied on the render mesh; generated point data no longer carries mirror transforms, so the same mirror is never applied twice.
- Dimmer default changed from 0.0 to 1.0 to prevent silent zero-output.
- Removed editor-only ArrowComponent from both SuperLaserPatternActor and SuperLaserProActor.

### Laser Recording Improvements

- MovieSceneSuperLaserSection playback accuracy: rewritten recording-frame to evaluation-time mapping for smoother recorded playback.
- LaserDataProcessor enhanced point-data processing pipeline.
- SuperLaserSubsystem simplified: removed redundant state-tracking code and streamlined recording interfaces.
- SuperLaserTypes refactored: consolidated laser data type definitions with consistent naming and structure.

### Laser Fog Speed Recalibration

- Three-layer fog noise speed constants recalibrated ×12: the old constants were copied directly from a screen-space version, producing ~2 cm/s drift in world space (effectively stationary). After recalibration, FogSpeed=1 yields approximately 267/99/93 cm/s across the three layers — consistent with real stage haze drift.
- Component default FogSpeed adjusted from 0.3 to 1.0.

### Procedural Stage Structures

The following procedural structure actors were added, under the `StageModel / StageProgram` category in the asset browser:

| Actor | Description |
| --- | --- |
| Super Barrier | Spline-driven barrier with front-of-stage, crowd-control, and site-fence forms; can conform to terrain. |
| Super Stage Roof | Stage roof system with flat, gable, and arch roof profiles, including corner towers, roof trusses, roof skin, PA wings, and a backdrop frame. |
| Super Grandstand | Tiered grandstand with automatic aisle splitting and sightline clearance readout. |
| Super Stair Tower | Stair tower with single-flight or double-return layout, showing step and slope parameters. |
| Super Trackway | Spline-driven ground panels with multi-lane layout and optional staggered joints; can conform to terrain. |
| Super Cable Run | Cable routing with either suspended sagging spans or ground cable ramps. |
| Super Ballast | Ballast layout that reports the required mass for an entered overturning moment. |

Other changes:

- Fixed circular truss section types having no effect. Box, Triangle, and Flat previously produced identical geometry; each section is now generated correctly, and a section rotation parameter was added.
- Size and count limits on stage structure actors were raised to match large-event scale. Sliders still cover the common range, and larger values can be typed in directly.
- Fixed an instanced-mesh navigation bounds ensure that could fire while editing parameters or dragging spline handles.

### Licensing

- When a session becomes invalid (signed out by support, expired, revoked, or otherwise invalid), the reason is now shown instead of silently clearing the entitlement cache.

### Documentation and License Files

- Added the software license agreements `LICENSE_zh.txt` / `LICENSE_en.txt` and `THIRD_PARTY_NOTICES.txt`.
- Added legal documents `docs/legal/terms_zh.md`, `terms_en.md`, `privacy_zh.md`, and `privacy_en.md`.
- Added partnership policy documents for SuperStageTeam, authorized resellers, and educational institutions.
- Removed the `docs/tutorial` getting-started documents, along with their entries in the documentation index.


## 26H2.2

### Plugin Structure Update

- The main SuperStage plugin currently contains: SuperCore, SuperDMX, SuperNdi, SuperShader, SuperAuth, SuperAssets, SuperTools.

## 26H2.1

### Plugin Structure Update

The main SuperStage plugin now contains only these modules:

- SuperCore
- SuperDMX
- SuperNdi
- SuperShader
- SuperAuth
- SuperAssets
- SuperTools

The following features are standalone plugins and must be enabled separately:

| Standalone plugin | Description |
| --- | --- |

### Main SuperStage User Features

- UE scene actors for fixtures, screens, projectors, light strips, truss, drapes, stage floors, crowd objects, and related stage elements.
- Art-Net / sACN related DMX workflows.
- Fixture Universe, Address, and Fixture ID management.
- Patch Tool batch patching, address-occupancy preview, and conflict hints.
- DMX Activity Monitor channel-value snapshots.
- MVR, GDTF, and grandMA2 / grandMA3 related data-preparation entries.
- Fixture Library Editor, Gobo Atlas, Color Atlas, Prism Preset, and related editor tools.
- Authorization, offline activation, and module-entitlement UI.

### Documentation Policy Changes

- SuperNdi is currently built into SuperStage.
- User documentation no longer states fixed latency, fixed frame rate, fixed hardware scale, or fixed performance improvements that cannot be directly verified from source.
- Stage structure actors are documented as UE scene-building and displayed-statistics tools only. They do not promise real-world structural safety or regulatory compliance.
- Removed the `devdocs` directory: its C++ / API reference material is for development and is not part of the product user documentation.

### Documentation Still Under Review

- Old changelog entries about removed modules and standalone plugins.
- Stage asset documents that mention load standards, safety decisions, or engineering conclusions.
- Editor tool documents whose menu paths must be checked against the current SuperStage toolbar menu.

## 26H2.0

The following areas still have current implementation or tool-entry evidence, but their real project behavior should be verified in the target environment:

- MVR import/export tools.
- grandMA2 / grandMA3 patch-related tools.
- Patch Tool start values, address handling, Fixture ID handling, and channel-occupancy preview.
- SuperScreen, SuperProjector, LightStripEffect, and related media/light-strip stage actors.
- Native fixture assets, fixture library resources, and Fixture Library editing tools.
- DMX recording and playback related Sequencer tracks.

Old documentation statements about performance improvements, fixed latency, or third-party software being ready to use without validation are not current user-facing commitments.

## Boundaries

- Previs output is for review and checking. It is not a guarantee of final on-site fixture or screen output.
- DMX, MVR, GDTF, and grandMA workflows depend on third-party systems, file formats, networking, and version compatibility.
- Fixture library data comes from GDTF packages published by the manufacturers; its accuracy is governed by what the manufacturer published.
- Module availability depends on the entitlements on your account.
- Real live events still require professional engineering review for hardware, networking, rigging, safety, and regulatory requirements.
