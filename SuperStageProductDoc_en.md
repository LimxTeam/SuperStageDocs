# SuperStage Product Document

| Item | Content |
| --- | --- |
| Document Name | SuperStage Product Document (English) |
| Product Version | SuperStage 26H2.6 |
| Version Baseline | SuperStage 26H2.6 |
| Version Release Date | August 29, 2026 (26H2.6 release notes) |
| Document Revision Date | August 29, 2026 |
| Applicability | All officially delivered SuperStage packages; available modules are subject to the official pricing page, your order, and the authorization backend |
| Copyright | Foshan Yierranran Technology Co., Ltd. (LimxTeam) |

## About This Document

This document describes the product positioning, capability scope, typical workflows, dependencies, and delivery boundaries of the current SuperStage version, to help customers evaluate whether the product suits their projects.

The capabilities described in this document are based on the currently delivered SuperStage product and its active extension modules. Actual availability may vary according to the purchased package, authorization scope, Unreal Engine version, third-party software environment, and project configuration. Experimental, removed, internal-only, or unconfirmed capabilities are not presented as released features in this document.

This document does not contain operating tutorials, development API references, or on-site system configuration details. Available modules, authorization scope, service content, and commercial terms are governed by the SuperStage User Service and Software License Agreement, the official pricing page, your order, the authorization backend, or a mutually confirmed delivery list.

> **Upgrade notice**: 26H2.6 substantially changes the fixture asset structure. Before upgrading an existing project, read the migration notes in Chapter 9, "Known Boundaries and Dependencies."

---

## 1. Product Positioning

SuperStage is a professional Unreal Engine-based stage previsualization and entertainment-production data collaboration toolset. It brings the following into a shared real-time scene for visual review, patch inspection, system integration, dynamic-content playback, and project delivery preparation:

- Stage lighting and optical representation;
- DMX control data;
- Media screens and projection;
- NDI media input;
- Stage-effect and fountain visualization;
- Crowd visualization;
- Stage structures and mechanical visualization objects;
- MVR / GDTF data workflows;
- grandMA-related preparation and collaboration workflows;
- SuperLaser laser visualization and laser content authoring;
- SuperMadrix LED matrix visualization;

SuperStage is not:

- A simple UE asset pack or stage model package;
- A real lighting console or media server;
- A real laser safety controller, pyrotechnic firing controller, or stage-machinery controller;
- A structural engineering certification tool;
- A replacement for professional personnel, on-site hardware systems, or local regulation.

It serves the work done "before load-in" and "between handovers": see clearly, align, and find problems in the virtual scene first, then hand the organized data (patch sheets, MVR files, fixture-library resources, grandMA preparation data, recorded content) to the next stage. Real show sites remain governed by on-site hardware systems, qualified engineering personnel, and local regulations.

### Team Value

| Role | Common Pain Point | How SuperStage Helps |
| --- | --- | --- |
| Production company / project lead | Clients cannot read the plan; problems surface only after load-in | Review lighting, screens, scenery, and effects in one UE scene, reducing imagination-based communication |
| Lighting designer | Whether positions, angles, beams, colors, and gobos match the design intent | Check spatial relationships between fixtures and scenery, screens, projection, and camera views during previs |
| Console engineer | High handover cost of Fixture IDs, universes, addresses, and fixture libraries | Reduce duplicate entry and manual cross-checking via Patch Tool, MVR/GDTF, and grandMA workflows |
| Video / media team | Screens, projection, and camera feeds reviewed separately from lighting | Bring media inputs into the same stage scene to judge brightness, rhythm, and spatial relationships together |

---

## 2. Target Users

SuperStage is built for professionals in show production:

- Lighting designers and lighting programmers;
- Stage and show visual designers;
- Previsualization (previs) teams;
- Show production companies and technical directors;
- Console engineers (grandMA and others);
- Media / virtual production teams;
- Professional training and educational institutions.

Not primary target scenarios: general game-development lighting, architectural visualization, the ICVFX mainline of film virtual production, and scenarios requiring safety-certified on-site control systems.

---

## 3. What Problems SuperStage Solves

**Plans are no longer explained by words alone.** The hardest part of early stage projects is often communication: clients look at renderings, lighting designers at plots, video teams at content, engineering teams at structures. SuperStage puts lighting, screens, projection, light strips, scenery, and effects into one UE scene so everyone discusses the same picture.

**Fixture data does not have to be built by hand.** A manufacturer's GDTF package imports directly into a usable fixture: channels and modes, geometry tree and mechanical axes, 3D models, and gobo / color / prism wheels are all read from the package, so fixtures need not be assembled one at a time.

**Patch risks surface early.** Address overlaps, duplicate Fixture IDs, and unclear universe planning consume on-site time. SuperStage moves these checks forward into previs with the Patch Tool, channel grid, conflict indication, and activity monitoring.

**Less duplicate data entry.** The same fixture data re-entered in UE, design software, the console, and delivery documents accumulates errors. Through MVR, GDTF, and grandMA workflows, fixture positions, IDs, and patch data organized during previs flow into external collaboration steps.

**Media, strips, and lighting reviewed together.** Screen content, camera feeds, projection, and lighting may each look fine alone; combined, they still need checks on brightness, rhythm, and the main visual hierarchy. SuperStage brings media inputs into the same stage scene for a unified judgment.

**Dynamic content can be replayed.** Lighting states and video feeds are dynamic. SuperStage records DMX data and NDI frames into Sequencer tracks for client review, internal retrospectives, and design comparison.

**Deliverables have a source.** The previs scene settles into deliverable patch sheets, MVR / fixture-library files, grandMA preparation data, screenshots, videos, and packaged demo builds.

---

## 4. Core Workflows

Each step of the workflows below has a corresponding formal user entry point in the current version.

### 4.1 Build the Stage Scene

Open the asset browser from the SuperStage toolbar and drag fixtures, trusses, scaffolding, stage decks, drapes, screens, machinery, and effect machines into the scene.

The asset browser organizes content into five categories — StageLight / StageModel / SuperVFX / SuperLight / SuperStage — and offers three entry points: All, Favorites, and Recent. Search matches model, manufacturer, and fixture type at once with space-separated terms. Right-click supports place, replace, favorite, locate in Content Browser, and open fixture definition.

Stage-structure objects are procedurally generated: they rebuild automatically after span, layer, or curvature parameters change. External scene data can also be brought in through MVR import or Unreal Engine's own import workflows such as Datasmith and USD.

### 4.2 Build Fixtures from GDTF

A fixture is a **fixture definition asset** that can be imported, edited, and upgraded — not a Blueprint. Ways to create one:

- **Shipped library**: 807 models are already built and can be taken straight from the asset browser;
- **Single import**: one manufacturer GDTF package produces a usable fixture — channels and modes, geometry tree and mechanical axes, 3D models, gobo / color / prism wheels, and a thumbnail;
- **Batch import**: select a set of GDTF packages at once and build fixtures in bulk according to library directory and naming conventions, handling overwriting existing assets, name collisions, and source paths;
- **Upgrade in place**: a fixture definition offers a right-click "Upgrade in place from source GDTF" to pick up later importer improvements without a re-import. By default this only reports differences and modifies nothing until confirmed.

Import scope is selectable — nine items in four groups (identity / channels / geometry / optics): manufacturer and model name, DMX modes and channel libraries, meshes and axis hierarchy, emitter placement, physical parameters, gobo atlases, color atlases, slot filter colors, and prism presets. On re-import only the selected parts are replaced, so hand-adjusted content is left as it is.

### 4.3 Place and Configure Fixtures

- Place fixtures individually, or generate arrays with the fixture-array tools: linear / grid (with honeycomb offset) / ring arrays, and spline-based hanging with a repeating fixture sequence;
- Each fixture references a fixture definition asset; the DMX mode inside that definition determines its channel layout;
- Right-clicking a fixture in the scene edits or locates its definition asset directly;
- Static meshes in the scene can be quickly bound as screen or light-strip carriers via the context menu.

### 4.4 Assign Fixture IDs, Universes, and Addresses

The Patch Tool batch-patches selected fixtures: starting universe / address / Fixture ID auto-increment from the current scene state, addresses advance by each fixture's footprint and roll over across universes automatically; Fixture IDs already in use are skipped; fixtures can be batch-renamed by rule. The candidate list follows the scene selection, and fixtures that Apply will skip are flagged before it is pressed.

### 4.5 Inspect Channel Occupancy and Conflicts

- The patch preview table lists all DMX fixtures in the level; address overlaps and duplicate Fixture IDs are marked in red, table selection is synchronized both ways with the viewport, and inline edits are undoable and recompute conflicts immediately;
- The 512-channel grid shows per-universe address occupancy, with color blocks coded by channel library. Fixture blocks can be dragged to re-patch (across universes, and multi-selection moves as a group preserving relative offsets); conflicts at the drop target turn red during the drag and are refused on commit. Fixtures dragged in from the Outliner are auto-patched and skip occupied ranges.

### 4.6 Connect the DMX Network and Preview Lighting

In the SuperDMX configuration panel, choose the protocol (Art-Net or sACN), the local network adapter, and the starting universe; changes take effect immediately and save automatically. DMX output from an external console or onPC drives the fixtures in the scene in real time; the DMX Activity Monitor shows per-universe channel-value snapshots to confirm signal arrival.

### 4.7 MVR Import and Export

Read and written to the MVR 1.6 specification.

- **Import**: parses the scene description (layers, groups, fixtures, trusses, supports, scenery, positions, classes, focus points, and more) and places fixtures in the level with coordinate conversion. Fixtures are grouped by "GDTF model × DMX mode" so each group gets its own target, preselected from the GDTF information in the package; models missing from the library can be built on the spot from the packaged GDTF;
- **Export**: writes the level back into a specification-compliant `.mvr` with each model's GDTF packed in. Every hard requirement of the specification is validated before the file is written, and the written file is re-parsed and fixture counts compared afterwards.

Boundary: export contains fixtures only — UE has no channel for writing static meshes as `.3ds` / `.glb`, so trusses and scenery do not appear in the exported package. External applications interpret MVR / GDTF differently; verify results in the target software before formal delivery.

### 4.8 grandMA Patch Preparation

- **grandMA2**: connects to an MA2 onPC environment on the local machine or LAN, groups scene fixtures by channel library, copies fixture-library files, sends import and patch commands, and checks console responses; later scene changes can be previewed in a change list (showing "old → new" per item) before sending update commands for changed items only. The MA2 workflow does not delete fixtures already on the console.
- **grandMA3**: sends patch commands and GDTF libraries to an MA3 onPC environment, including the DMX mode with the command (on the MA2 channel the mode travels inside the imported library file instead); fixture types and modes are mounted by index and read back for verification, and grouping is layered by model plus mode. The MA3 channel is one-way command delivery; the software cannot read console state back. Changes likewise pass through a change-list preview, and a subset of fixtures can be selected for synchronization.
- **Offline delivery**: when a direct connection is not possible, patch data can be exported as MA layer XML plus macro files (DMXToMa) and imported on the console side.

> **grandMA3 Operational Notice (Important)**
>
> The grandMA3 workflow is one-way command delivery; the software cannot fully read back console state, and updates for removed fixtures send **delete commands** to the console. Before running an import or patch update:
>
> 1. Back up the current show file;
> 2. Rehearse in a copy show or a test / onPC environment first;
> 3. Review every pending change (including deletions) in the change list;
> 4. Confirm the results on the console side after execution;
> 5. Never apply patch updates directly to an irreplaceable production show without verification.

Actual results of these workflows depend on the target MA environment version, network permissions, fixture-library files, and on-site configuration. This document does not describe them as unconditional "automatic synchronization."

### 4.9 Add Media, Screens, and Video Inputs

Screen and projection objects support three input sources: project static textures, Director camera feeds, and NDI video input (see Section 5.4). One media source can drive multiple screen carriers; the projection object casts the picture onto arbitrary scene geometry as light.

### 4.10 Add Effect Machines, Fountains, and Crowds

- Effect machines (fireworks, confetti, flame, pyro, smoke, bubbles, snow) are placed as stage objects and triggered by a single DMX channel mapped to intensity / level;
- The fountain is a 14-channel DMX device; the water pool is a separate scenic object, and neither depends on the other;
- The crowd object outlines an area with a spline and auto-populates it by target count or density from the built-in character library.

### 4.11 Author Laser Content

Laser animation assets are created in the Content Browser and opened by double-click, with a canvas editor, SVG importer, baker, and Sequencer track alongside. The asset stores Bezier outlines and modulation parameters; the point stream is computed at playback. See Section 6.1.

### 4.12 Record and Replay Dynamic Content

- **DMX recording**: via Take Recorder, records live DMX data of a universe range into Sequencer tracks (one track per universe, only changed channels are keyed, stepped curves preserve DMX's discrete semantics); playback sends data back to the DMX system frame by frame, and channels not covered by the track keep their current values (merge semantics);
- **NDI recording**: records configured NDI inputs into a frame-buffer asset with Sequencer tracks; playback frames are distributed through the same channel as live input, so downstream screens do not distinguish live from replay;
- **Laser recording**: point clouds from external laser software can be recorded with compression and replayed through Sequencer; laser animation assets bake to Sequencer per device.

### 4.13 Delivery Preparation

The previs project can be packaged as a standalone Windows executable through the standard UE process for demos and presentations.

---

## 5. Main Capabilities

### 5.1 The Fixture System: Data-Driven Fixture Definitions

A fixture is described by three assets. All data-driven fixtures share one Actor class, and the model is determined by the definition asset:

| Asset | Description |
| --- | --- |
| **Fixture Definition** (Super Fixture Definition) | The complete definition of a fixture: identity, physical data, mechanical axes, emitters, optical elements, DMX modes |
| **Channel Library** (Super Fixture Library) | The fixture's DMX channel chart: modes, attributes, sub-attributes, channel sets. Can be shared by several fixtures |
| **Prism Preset** (Super Prism Preset) | Three independent prism configurations, each describing how the beam is split |

Everywhere that needs to distinguish "model" — asset browser categories, patch naming, MA sync grouping, channel grid color coding — decides by channel library asset rather than by class name.

The **fixture editor** is a single-viewport workbench with four tabs:

| Tab | Content |
| --- | --- |
| Fixture | The definition: identity, physical, mechanical axes, emitters, optics, DMX modes |
| DMX Test | Per-channel fixture test |
| Channel Map | Address occupancy |
| Validation | Check results |

The channel library editor provides three-level tabular editing (mode → attribute → sub-attribute → channel set) and is wired into the undo system. Fixture definitions can be compiled from the Content Browser right-click menu, with multi-select batch support.

### 5.2 Lighting and Optical Representation

Fixture attributes representable in the current version:

| Attribute Domain | Support |
| --- | --- |
| Dimmer / strobe | Dimmer; the strobe physical quantity is interpreted in Hz with the range taken from declared library values; strobe modes are resolved per library sub-attribute ranges |
| Color | RGB / RGBW / HSV / color temperature / cool-warm mix / CMY subtractive / CTO / CTC / single and multiple color wheels / multi-channel dynamic color mixing. Additive channels use manufacturer-measured emitter chromaticity, subtractive channels use manufacturer-measured filter chromaticity, with a ColorTint opcode for plus/minus green correction |
| Movement | Pan / Tilt, with main-axis mechanical travel imported from the GDTF package declaration (falling back by axis role when absent: pan ±270°, tilt ±135°); two drive modes with or without a PT-speed channel; continuous rotation (infinite / position / stop); independent pan/tilt per head on multi-head fixtures |
| Optics | Zoom, Focus, Iris, Frost |
| Gobos | Up to 3 gobo wheels; static / scrolling / shake modes; gobo rotation (indexed and continuous) |
| Prism | Prism representation based on prism-preset assets (3 layers, up to 48 facets per layer), with rotation |
| Framing | Four-blade framing shutters plus overall framing rotation; blade order follows the GDTF specification (1 top, 2 right, 3 bottom, 4 left) |
| Pixels | Per-pixel color / intensity / strobe on matrix fixtures, including per-cell dimming; light-strip effect material (16 built-in patterns with speed / width / direction control) |

**Important boundary**: not every fixture supports all attributes. The attribute set each fixture implements depends on the channels that actually exist in its definition and channel library — that is, on the DMX channel chart / GDTF data of the corresponding real fixture.

**Light source and beam pipelines**

Fixture key light is projected by an in-house cone light pipeline (SuperConeLight) that replaces the UE SpotLight. Gobos, color wheels, framing shutters, and prisms are all projected by the cone light. Whether and how an aerial beam column is drawn is decided by the emitter's beam type:

| Beam Type | Use |
| --- | --- |
| Volumetric Beam | Volumetric beam plus aerial column. The shipped library's choice |
| Volumetric Shaper (Profile) | Volumetric beam plus four-blade framing, for profile fixtures |
| Ray Beam (Matrix / FX) | Pure beam, no optical elements, for matrix and effect fixtures |
| Wash (Color / Blinder / Strobe) | Wash / blinder / strobe; no column drawn |
| Spot Only (No Column) | Cone light only, no aerial column; the floor gobo is unaffected |
| Pixel Matrix | Pixel matrix fixtures |
| Effect Plane (LED FX / Magic) | Effect planes |
| Material Beam / Shaper (Legacy) | Old material pipeline, kept for fallback, not recommended for new content |

The **volumetric beam** uses closed-form analytic scattering integration with GPU-instanced indirect drawing, and supports gobo projection, color wheels, prism splitting, and dynamic occlusion by real geometry, in four quality tiers (Low / Medium / High / Ultra). The cone light and the volumetric beam share one shadow atlas and one sampling function, so the aerial column and the floor pool are occluded consistently by the same occluders. Haze flow speed (HazeSpeed, cm/s) is an exposed parameter.

> **The shipped fixture library is fully migrated to the volumetric beam pipeline** (after migration the library contains zero material-beam and material-shaper emitters). However, **a fixture newly imported from GDTF still gets the material beam on its main emitter by default** (material shaper for profile fixtures with blade channels); switch it to a volumetric beam type in the fixture editor. Matrix / pixel groups are assigned Ray Beam or Wash on import according to the BeamType declared in the package.

**Distance culling is off by default** (DistanceFadeEnd defaults to 0, meaning no distance limit); fill it in per fixture when a limit is wanted.

**Surface lighting**: fixture key light casts light pools and gobo patterns onto scene surfaces through the cone light or rect lights; emissive surfaces such as LED screens, light strips, and matrix fixtures produce approximate illumination on nearby scenery through their light components.

**White-model rendering**: the `SuperWhiteModelActor` post-process object, when dropped into a level, renders the entire world as a white model; the details panel toggles and tints it, and deleting the actor restores the original view. It applies in editor viewports, PIE, and Movie Render Queue.

Actual beam counts, shadow quality, and frame rates depend on fixture types, beam quality tier, shadow configuration, scene complexity, and hardware. This document promises no fixed scale or frame rate.

### 5.3 DMX and Patching

- **Protocols**: Art-Net and sACN (E1.31) input and output, configured independently; unicast, broadcast (Art-Net), and standard multicast (sACN);
- **Universe scale**: the internal data cache has an architectural limit of 512 universes (512 channels each). This is a software data-structure limit and does not mean every project can run 512 active universes simultaneously — practical scale depends on project complexity, network, and hardware. Network-side universe numbering is aligned to internal numbering via the "start universe" setting to match console conventions (see Appendix B);
- **Channel library system**: defines modules (one per pixel for matrix fixtures), attributes (8 / 16 / 24-bit channel precision), sub-attributes (with strobe / rotation modes and physical ranges), and channel sets (with gobo textures, colors, prism-layer selection);
- **Library import / export**: GDTF (import described in 4.2; export organized according to the GDTF 1.2 structure, restoring mode and function names), MA2 XML (bidirectional), and JSON (plain-text format for manual or programmatic authoring). Verify exported-file compatibility in the target software;
- **Companion tools**: gobo-atlas and color-atlas builders (compose channel-set slots into lookup atlases for rendering), and the prism-preset editor (canvas-based facet editing with a live real-beam preview);
- **Patch tools**: see 4.4 / 4.5;
- **Diagnostics**: the DMX Activity Monitor shows per-universe channel snapshots and signal activity, with per-universe cache clearing.

### 5.4 Media, Screens, Projection, and NDI

| Input Source | Description |
| --- | --- |
| Project textures | Static / media texture assets in the project |
| Director camera | An in-scene director camera captures live picture (configurable resolution and field of view) for screens / projection |
| NDI input | Discovers NDI sources on the LAN via the bundled NDI runtime, connected through "logical input name → external source" mapping |

- **Screens**: apply the input picture to one or more static-mesh carriers (one media source can drive multiple screens), with opaque / transparent modes, brightness / contrast / color adjustments, and four-corner keystone correction; batch-bind meshes via the context menu;
- **Projection**: light-function-based projection mapping onto arbitrary scene geometry, with brightness, throw distance, throw angle, edge softening, and keystone correction — suited to previewing spatial and occlusion relationships between projected content and stage structures;
- **NDI recording**: NDI inputs can be recorded to a frame-buffer asset (configurable target frame rate, scaling, and maximum duration) and replayed through Sequencer; replay automatically mutes the live input of the same name. The NDI runtime ships with the plugin — no separate NDI tools installation is needed; supported pixel formats are listed in Appendix B. The authorization scope of NDI capability is subject to the official pricing page and your order.

### 5.5 Stage Effects and Fountains

**Effect machines**: 7 categories in 16 actor variants — fireworks (4), confetti (5 levels), flame (1), pyro (1), smoke (3 levels), bubbles (1), snow (1).

- Effect machines are placed as stage objects and ship with GDTF library files, so they join patch and console workflows;
- **DMX control is single-channel triggering**: the channel value maps to effect intensity / level; effect colors and particle details are determined by the effect assets themselves and are not DMX-controlled in the current version;
- Effects can be combined with Sequencer DMX tracks for timeline-based triggering.

**Stage fountain (SuperFountain)**: a 14-channel DMX moving fountain device that shares the same optics as the lighting.

| Channel | Content |
| --- | --- |
| 1–2 | Pan (16-bit) |
| 3–4 | Tilt (16-bit) |
| 5 | Pan / Tilt speed |
| 6 | Jet height |
| 7 | Spread angle |
| 8 | Underwater light intensity |
| 9 | Underwater light strobe |
| 10–13 | Underwater light R / G / B / W |
| 14 | Underwater light zoom |

- Nozzles are components on the fountain device; **the water pool is a separate scenic object**, and neither depends on the other — if you only want a pool, place a pool, with no need to place a fountain first;
- Decomposed into five layers: coherent jet, breakup droplets, aerated white water, pool ripples, and water surface rendering;
- Underwater lights are in candela, the same convention as the projectors; scattering from stage lights on the water mist uses the same phase function as the beams;
- All parameters are adjustable from both the Details panel and console variables.

**Safety boundary**: effect machines and fountains are visual simulation assets. They provide no firing control, safety-distance calculation, hazardous-material management, pump / electrical control, or regulatory-approval basis for real devices. Real pyrotechnics, open flame, effects, and water features must be executed by licensed professionals under local regulations.

### 5.6 Crowd and Characters

- **Built-in character library**: 13 preset characters (VAT vertex-animation meshes), each with two animation material variants (Anim_V1 / Anim_V2) and two material layers (base and emissive); character meshes include LODs. Two placeable objects correspond to them, Super Crowd V1 and Super Crowd V2, differing only in which animation material set is applied;
- **Crowd placement**: outline any area with a spline and auto-populate by target count or density; placement keeps a minimum inter-person spacing (relaxable when the area is too small), snaps to the ground, and can reject steep slopes; one crowd object is limited to 5,000 instances (software limit) — actual smoothness depends on hardware, character count, and LOD configuration;
- **Custom characters**: the VAT batch generator bakes skeletal meshes plus animation sequences into VAT static meshes and material instances (live progress, cancellable, LOD support); results can be added to the crowd character list;
- **Editor behavior**: crowds restore and rebuild automatically after level load.

The crowd is a visualization asset and placement system for audience-area atmosphere and composition review. It is not a crowd-simulation platform: no behavior simulation, evacuation analysis, or pedestrian dynamics.

### 5.7 Stage Structures and Mechanical Visualization

**Structural objects (procedurally generated)**: 5 truss types (gantry / spline-curved / circular / horizontal grid / vertical tower), 2 scaffold types (straight / curved), procedural drapes (6 drape types × 4 pleat styles × 4 opening modes), and modular stage decks (with auto-generated stairs). Structural objects display part counts, self-weight, and load **reference estimates** (simplified models based on public engineering formulas) for order-of-magnitude reference and statistics during previs. **These figures are not structural-safety conclusions and cannot replace structural design, mechanical verification, or construction safety approval.**

**Stage programmatic objects**:

| Object | Description |
| --- | --- |
| Super Barrier | Spline-driven barrier with front-of-stage, crowd-control, and site-fence forms; can conform to terrain |
| Super Stage Roof | Stage roof system with flat, gable, and arch roof profiles, including corner towers, roof trusses, roof skin, PA wings, and a backdrop frame |
| Super Grandstand | Tiered grandstand with automatic aisle splitting and sightline clearance readout |
| Super Stair Tower | Stair tower with single-flight or double-return layout, showing step and slope parameters |
| Super Trackway | Spline-driven ground panels with multi-lane layout and optional staggered joints; can conform to terrain |
| Super Cable Run | Cable routing with either suspended sagging spans or ground cable ramps |
| Super Ballast | Ballast layout that reports the required mass for an entered overturning moment |

**Mechanical objects (previs visualization)**:

| Object | Description |
| --- | --- |
| Lifting machinery | 12-channel DMX-driven previs motion (XYZ translation + XYZ rotation, 16-bit each), absolute positioning and continuous-rotation modes |
| Rail machinery | 14 channels, adding a spline rail axis on top of the 6 axes, with closed-loop nearest-path interpolation and orientation lock; attached objects follow |
| Lift matrix / lift ball | Lifting light matrix (42 channels) and effect ball (9 channels) with cable visuals; lifting and effect colors are DMX-driven |

**Drapes**: 6 drape types (Main Curtain / Legs / Border / Backdrop / Scrim / Cyclorama) × 4 pleat patterns (Flat / Box Pleat / Gathered / Austrian) × 4 opening modes (Fixed / Fly / Traveler / Tab), procedurally generated with pleated surfaces and pipe / tie-line visuals.

**Machinery safety boundary**: these mechanical objects are for motion previs and visual expression only. SuperStage provides no real machinery control, certified safety interlocks, emergency-stop systems, limit-switch protection, load monitoring, motion-control certification, SIL / functional-safety compliance, rigging approval, or operator authorization. Real stage machinery must be executed by professional machinery control systems and licensed personnel under applicable codes.

### 5.8 Sequencer Recording and Playback

See 4.12. DMX, NDI, and laser tracks are formal Sequencer track types and can be arranged together with UE timeline content (cameras, animation, audio); combined with Movie Render Pipeline, rendered videos can be produced.

---

## 6. Active Extension Modules

SuperStage currently consists of its core stage-previsualization toolset together with active extension modules. Module authorization scope is subject to the official pricing page and your order.

### 6.1 SuperLaser (Laser Visualization and Authoring Module)

SuperStage's laser capabilities come in three layers:

**Built-in laser pattern objects (provided by the core plugin)**

30 procedural vector patterns (geometry, waveforms, curves, digits, and more, including Blackout) with 16-channel DMX control: pattern selection, size, position, rotation, color, and point density, plus the bit fields on Ch16 — mirroring (Bit0-1), 7 global motion effects (Bit2-4: Sweep / Pulse / Orbit / Figure-8 / Spin Pulse / Drift / Bloom), and color animation (Bit5-6: none / Rainbow Cycle / Color Pulse / Color Fade). They can also run built-in animations in property mode without DMX.

**Laser animation assets (new in this release)**

Created in the Content Browser and opened by double-click, with a canvas editor, SVG importer, baker, and Sequencer track alongside.

- **Vectors are stored, not point clouds**: the asset holds Bezier outlines and modulation parameters, and the point stream is computed at playback, making it resolution independent, editable at any time, and free of the need to redo content when scanner parameters change;
- **There is no fixed duration**: motion comes from the frequency of the effects, so any point in time can be evaluated directly and scrubbing, jumping, and reverse playback need no warm-up;
- **Canvas editor**: node-level editing of Bezier control points, marquee and multi-select, grid snapping, transform handles, convert to path; layers can be copied and pasted across assets; text layers take Bezier curves directly from font outlines; a full-width timeline at the bottom carries adaptive second ticks and a beat grid drawn from Tempo;
- **Effect system** in six kinds (categories follow the conventions of professional laser software):

  | Category | Description |
  | --- | --- |
  | Oscillator | Continuous oscillation, waveforms including accelerate / decelerate / ping-pong / step / random landing points |
  | Keyframes | Keyframes, each effect carrying its own local timeline with a draggable curve editor |
  | Color | Color stop line × application scope × blend mode |
  | Points | Point-level operations: repeat points, anchor points, fixed-count beams, beam collapse |
  | Filter | Cleanup: even spacing, decimation, soft ending, removal of long blanking travel |
  | Device Chase | Device chase, with fades between steps |

  The time base is either seconds or beats, with beats driven by the asset tempo.
- **The baker respects galvo physics**: the point budget is a hard ceiling, dwell points are allocated to corners by severity, dwell is added before and after blanking, and there is ordering optimization and color deskew. Frame rate is a derived result, and the interface states the verdict directly. One animation can be sent to several devices, and Sequencer bakes per device;
- **ILDA read/write**: complete, covering the various formats and malformed-file cases, with a separate validator command line written independently to the IDTF14 rev011 specification.

**SuperLaser module (external point-cloud input)**

Receives network point-cloud data output by external professional laser-control software (UDP multicast) and visually renders the frame / point data as beams in the UE scene — including HotBeam dwell-point enhancement, a visual approximation of galvanometer scanning characteristics (disabled by default), multi-device input (60 device slots by default), compressed point-cloud recording with Sequencer playback (via Take Recorder), and ILDA pen-semantic path connection (blank-point lift, dwell-point collapse, cross-figure disconnection). The specific third-party laser software and versions supported are described on the official website and documentation center.

**Laser safety boundary**: SuperLaser is for visual previsualization, content authoring, and data visualization; it presents the picture of a laser show and is not real laser output control. It provides no audience-scanning approval, no maximum permissible exposure (MPE) calculation, no laser zoning certification, no physical projector calibration, no emergency-stop hardware, no scan-failure protection, and no regulatory-approval basis. Any use with real laser systems remains subject to qualified laser operators, certified hardware, venue rules, and local regulation. Exported ILDA files must be reviewed by a licensed operator under on-site safety procedures before being fed to a real laser system.

### 6.2 SuperMadrix (LED Matrix Visualization Module)

SuperMadrix ships as a built-in module of the main plugin. Main capabilities:

- **Madrix Actor**: a DMX-driven LED matrix pixel visualization object that receives pixel data from LED control software such as Madrix;
- **Sequencer recording and playback**: Madrix matrix data can be recorded into Sequencer tracks and replayed via Take Recorder.

SuperMadrix is for visual previsualization of LED matrix content. It is not a real LED screen controller or a substitute for Madrix software.

### 6.3 Main Plugin Module Composition

The SuperStage main plugin currently contains the following modules, all delivered with the main plugin:

| Module | Type | Responsibility |
| --- | --- | --- |
| SuperCore | Runtime | Core of stage objects: fixtures, light sources, beams, fountains |
| SuperDMX | Runtime | Art-Net / sACN transport, universe cache, Sequencer DMX tracks |
| SuperNdi | Runtime | NDI input and recording |
| SuperMadrix | Runtime | LED matrix visualization |
| SuperLaser | Runtime | Laser rendering, laser animation assets and runtime |
| SuperShader | Runtime | Rendering pipelines for stage beams, cone light, fountains |
| SuperAuth | Runtime | Account and authorization |
| SuperAssets | Runtime | Stage structures, procedural scenery, in-house fixtures |
| SuperTools | Editor | Asset browser, patching, fixture editor, GDTF / MVR / grandMA tool panels |

Among these, SuperShader and SuperAuth are internal foundation components and are not sold or described as separate commercial products.

### 6.4 Interface Language

The plugin interface is available in Simplified Chinese and English, following the UE language setting. Property names, enum entries, and tooltips in the Details panel, along with console variable help text, are all in the localization dictionary.

All 14 tool panels (asset browser, patch tool, DMX configuration, NDI configuration, fixture editor, channel library editor, prism preset editor, GDTF batch import, MVR, DMXToMa, GrandMALink, gobo atlas builder, color atlas builder, VAT character generator) have a help button in the top-right corner that opens the instructions for that panel.

---

## 7. Deliverables

With SuperStage you can produce and deliver:

- UE previs scenes / project files;
- Screenshots, screen recordings, and videos rendered via Movie Render Pipeline;
- Verified patch data (Fixture ID / universe / address sheets with conflict-check results);
- DMX recordings and Sequencer timelines (replayable, re-outputtable to the DMX network);
- NDI frame recordings (replayed with the project);
- MVR files (fixtures plus associated GDTF packaging);
- GDTF / JSON fixture-library files (MA2 XML is converted from the GDTF on demand when exporting);
- Fixture definition, channel library, and prism preset assets;
- grandMA patch preparation data (online import or offline XML + macro files);
- Laser animation assets and exported ILDA files;
- Packaged standalone Windows demo builds.

---

## 8. Compatibility and System Requirements

### 8.1 Unreal Engine Versions

| Engine Version | Product-Line Support | 26H2.6 Package | Notes |
| --- | --- | --- | --- |
| UE 5.6 | Supported | Subject to the official download page | Baseline engine version of 26H2.6 |
| UE 5.8 | Supported (in the product line since 26H2.1) | Subject to the official download page | — |
| UE 6.0 | Supported (in the product line since 26H2.1) | Subject to the official download page | — |
| Other versions | Not confirmed | — | Subject to builds offered on the official download page |

"Product-line support" means the engine version falls within SuperStage's supported range; whether a 26H2.6 installer package is available for a given engine version is subject to the builds actually offered on the official download page.

### 8.2 Platform and Environment

| Item | Requirement / Description |
| --- | --- |
| Operating system | Windows 10 / 11 (64-bit). Authorization sign-in, offline activation, and GDTF / MVR archive handling depend on the Windows environment; macOS / Linux editors are currently not supported |
| Graphics environment | Advanced rendering features (volumetric beams, fountains, Lumen lighting, etc.) depend on DirectX 12, Shader Model 6, and the deferred rendering pipeline |
| Packaging target | Standalone Windows 64-bit executables |
| Dependent plugins | Enabling SuperStage also enables these official Unreal Engine plugins: Datasmith Importer, Takes (Take Recorder), Movie Render Pipeline, USD Importer |
| Third-party runtime | The NDI® runtime ships with the plugin (NDI is a registered trademark of the Vizrt Group; see THIRD_PARTY_NOTICES in the plugin directory) |
| Network | DMX (Art-Net / sACN), NDI, laser point cloud, and grandMA connections use local-machine / LAN communication; online authorization requires access to SuperStage cloud services; for the offline authorization edition see Chapter 10 |

### 8.3 Project Configuration Behavior (Please Read)

Some advanced SuperStage rendering features depend on specific project settings (DirectX 12, Shader Model 6, deferred rendering, Lumen, virtual shadow maps, light-function atlas capacity, and others). On load, the current version supplements recommended values for related configuration entries that are **not yet explicitly set** in the project; entries with existing explicit settings are not intended to be overwritten.

Note: enabling the plugin may trigger shader recompilation, and rendering behavior may change with configuration. **Before adding SuperStage to a production project with heavily customized rendering settings, validate fully in a project copy first.** This document does not describe automatic configuration as risk-free or as a guarantee of final results.

### 8.4 Hardware

This document sets no minimum hardware specification and invents no recommended configuration. Actual performance depends on fixture types and counts, beam quality tier, shadow configuration, media inputs, crowd size, effect and fountain complexity, scene complexity, and CPU / GPU / memory / storage / network conditions. For large scenes, use a recent discrete GPU and validate at your project's actual scale; both the volumetric beam and crowd systems provide quality / scale controls.

### 8.5 Quality Assurance

This release ships automated regression tests with the plugin source (covering the fixture pipeline, laser animation, fountains, MVR, asset browser, and other areas), all passing on the release build. Automated tests serve regression checking and constitute no promise about behavior in any particular project scenario.

---

## 9. Known Boundaries and Dependencies

### 9.1 Upgrading From an Earlier Version (Important)

- **No fixture class redirectors for old projects.** The fixture asset structure and naming changed substantially in this release. Existing projects using the old Blueprint fixtures should stay on the old plugin and **must not be upgraded in place**. The only redirectors kept are the Cutting → Shaper set, covering component classes, material nodes, enum values, and Blueprint functions that current assets store by name.
- Fixture definitions already imported can pick up this release's importer improvements through the right-click "Upgrade in place from source GDTF"; by default that operation only reports differences and modifies nothing until confirmed.

### 9.2 Functional Boundaries

- **MVR**: import covers many scene-description object types; export contains fixtures only — trusses and scenery are not written out (UE has no channel for writing static meshes as `.3ds` / `.glb`). MVR-xchange live synchronization is not supported. Applications interpret coordinates and fixture matching differently — verify in the target software.
- **GDTF**: import covers channels and modes, geometry tree and mechanical axes, 3D models (`.3ds` / `.glb`), gobo / color / prism wheels, and thumbnails. Models are imported only when present in the package; parts declared with `PrimitiveType` alone and no model file get a placeholder body. Export is organized according to the GDTF 1.2 structure; verify compatibility in the target software. A few GDTF archives packed with high compression depend on system extraction tools (Windows only). The accuracy of library data is governed by the GDTF packages published by the manufacturers.
- **Beam pipeline**: the shipped library is fully migrated to volumetric beams; **a fixture newly imported from GDTF still gets the material beam (Legacy) on its main emitter by default** and must be switched in the fixture editor. The material pipeline is retained for fallback only.
- **grandMA**: the MA2 workflow depends on remote command permissions and a local / LAN onPC environment; the MA3 workflow is one-way command delivery, cannot read console state back, and includes delete commands (see the notice in 4.8); both are affected by console version, show-file state, and library environment.
- **DMX**: practical universe scale and refresh behavior depend on project, network, and hardware; sACN multicast depends on IGMP support in network equipment; firewalls must allow the relevant ports (see Appendix B).
- **NDI**: depends on LAN discovery and network bandwidth; only the pixel formats listed in Appendix B are supported; NDI sources are produced by third-party software whose behavior is outside this product's control.
- **Fountain**: the water surface requires a water pool object in the scene to render; the fountain device does not carry a water surface of its own.
- **Rendering**: high volumetric-beam quality tiers and large scenes are GPU-sensitive; previs images are not equivalent to final on-site results.
- **Structures, machinery, effects, fountains, and extension modules**: load estimates, mechanical motion, and effect / water simulation are previs references only and constitute no safety conclusion or control capability; SuperLaser provides laser visual previs and content authoring only, with no laser safety control or approval basis (see the boundaries in 5.5 / 5.7 / 6.1).
- **Crowd**: 5,000 instances per object maximum; no behavior simulation.
- **Authorization and backend**: online sign-in and session validation depend on the availability of SuperStage cloud services; service-side behavior is governed by the license agreement and privacy policy.
- **Platform**: Windows only; macOS / Linux editors and non-Windows packaging targets are not supported.

---

## 10. Account, Authorization, Privacy, and Support

### 10.1 Account and Authorization

- **Sign-in**: the account system is passwordless — sign in with a one-time email code or a Google account; clicking sign-in inside the plugin opens the browser to complete authorization confirmation;
- **Devices and sessions**: a standard online account is not bound to a device and can be used on any computer. Each account supports one device working online at a time. Signing in on a new device automatically signs out the old plugin session; changing computers requires no unbinding or device release. Organization members use their own seat accounts. Separately agreed offline-build authorization is not part of the standard account switching process;
- **Online validation**: after sign-in, the plugin keeps periodic session validation with the authorization service while the editor runs; during a temporary network interruption, a previously validated session may continue operating for a limited period — exact behavior depends on the current version and authorization-service policy;
- **Offline authorization**: the separately agreed offline edition uses a "machine code → activation code" flow, with activation information stored on the local machine;
- **Update check**: on startup the plugin queries the latest version number and prompts for updates;
- **Authorization scope**: authorization validation takes place in the UE editor environment; package differences, commercial-use rights, and packaged-delivery scope are governed by the User Service and Software License Agreement, your order, and the authorization backend.

### 10.2 Data and Privacy

- Stage scenes, DMX data, MVR / GDTF files, media content, and project files are normally processed on your device or within your local network;
- Account sign-in, authorization validation, session management, and version checking require communication with SuperStage cloud services;
- Data categories, processing purposes, retention, and user rights are governed by the current SuperStage Privacy Policy.

### 10.3 Technical Support

When reporting issues, please provide:

- SuperStage version (visible in the toolbar status bar);
- Unreal Engine version, operating-system version, GPU model;
- External systems involved and their versions (console model / onPC version, NDI source software, laser software, target software);
- Network protocol and configuration used (Art-Net / sACN, universe range, adapter selection);
- UE log files (project directory `Saved/Logs/`);
- Screenshots or recordings;
- Minimal reproduction steps (and a minimal reproduction project if possible).

Do not include account credentials, license information, full banking details, or unrelated confidential client content in support materials.

| Need | Entry |
| --- | --- |
| Website | https://yunsio.com |
| Trial download | https://www.yunsio.com/download |
| Documentation center | https://yunsio.com/docs/superstage |
| Support / business | yunsio@yunsio.com |
| YouTube tutorials | https://www.youtube.com/channel/UCzr_PcLdN00gxABoTnCSZeA |
| Bilibili tutorials | https://www.bilibili.com/video/BV1UcdVBsEXb |

---

## 11. Glossary

| Term | Usage in This Document | Description |
| --- | --- | --- |
| Fixture | Fixture | A DMX-controllable device object in the scene |
| Fixture Definition | Fixture definition | The asset describing a fixture's identity, mechanics, emitters, optics, and DMX modes |
| Fixture Library | Channel library | The fixture's DMX channel chart asset, shareable across fixtures |
| Fixture ID | Fixture ID | The fixture's number in the project and on the console |
| Universe | Universe | A DMX domain of 512 channels |
| Address | Address | A fixture's starting channel within a universe (1–512) |
| Footprint | Footprint | The number of consecutive channels a fixture occupies |
| Personality / Mode | DMX mode | One DMX channel-chart configuration of a fixture |
| Patch | Patch | Assigning universes, addresses, and IDs to fixtures |
| Art-Net | Art-Net | A UDP-based DMX-over-IP protocol |
| sACN | sACN | The ANSI E1.31 streaming ACN protocol |
| MVR | MVR | My Virtual Rig, a show-scene exchange format |
| GDTF | GDTF | General Device Type Format, a fixture description format |
| grandMA | grandMA | The MA Lighting console family (MA2 / MA3) |
| DMX recording | DMX recording | Recording live DMX data into Sequencer tracks |
| Director input | Director input | An in-scene director-camera feed used as a media source |
| Cone light | Cone light | The in-house key-light pipeline projecting pools, gobos, color wheels, framing, and prisms |
| Volumetric beam | Volumetric beam | Volume-rendered representation of the visible aerial beam column |
| Emitter | Emitter | The unit in a fixture definition describing where light exits and how |
| GOBO | GOBO | Patterns projected via gobo plates / wheels |
| Prism | Prism | An optical element splitting a beam into multiple beams |
| Framing / Shaper | Framing | The four-blade framing-shutter system |
| Effect machine | Effect machine | A DMX-triggered stage-effect simulation object |
| ILDA | ILDA | The International Laser Display Association's laser frame exchange format |

---

## Appendix A: Resource Inventory (26H2.6 Baseline)

The counts below are the actual quantities in this version's shipped content. Placeable assets, files, and models are three different measures; each table states which it uses, and they are never mixed. Resources change with version updates — the shipped content is authoritative.

### A.1 Placeable Assets Overview

| Category | Count | Description |
| --- | ---: | --- |
| StageLight | 807 | Fixture library, imported from GDTF, covering 54 manufacturers |
| StageModel | 18 | Scenic structures: truss, scaffolding, stage decks, seating stands, barriers, drapes, cable trays, audience, and more |
| SuperVFX | 18 | Effect devices: smoke, fire, snow, bubbles, streamers, pyro, fountain, water pool |
| SuperLight | 16 | In-house fixtures: Aurora / Blaze / Flare / Hyperion / Machinery / Spark / Thunder, seven series |
| SuperStage | 10 | Stage devices: screens, projectors, lasers, LED tape, broadcast cameras, lift and track machinery, Madrix, white-model object, and more |
| **Total** | **869** | |

### A.2 Fixture Library Coverage

| Item | Count |
| --- | ---: |
| Manufacturers | 54 |
| Fixture models | 807 |

Manufacturers with the most models: Chauvet Professional, Prolights, Robe, CKC Lighting, Elation, Ayrton, ACME, Terbly, Cameo, Martin Professional.

### A.3 Supporting Assets

Counting method: shipped `.uasset` file counts.

| Asset Type | Asset name | Count |
| --- | --- | ---: |
| Fixture definitions | model name, no prefix | 807 |
| Channel libraries (one per DMX mode) | `CL_` | 2,884 |
| Prism presets | `SP_` | 413 |
| Gobo wheel atlases | `LTA_` | 1,060 |
| Color wheel atlases | `CLA_` | 942 |
| Model thumbnails | `LTC_` | 820 |

### A.4 Effect Machines and Fountain

Counting method: actor variants. Effect machines: 7 categories, 16 variants (fireworks 4, confetti 5, flame 1, pyro 1, smoke 3, bubbles 1, snow 1), with matching GDTF library files; plus 1 fountain device and 1 water pool.

### A.5 Crowd Characters

13 VAT characters, each with two animation material sets (Anim_V1 / Anim_V2) and base plus emissive material layers; character meshes include LODs. Two placeable crowd objects correspond to them (Super Crowd V1 / V2).

### A.6 Stage Structure and Media Objects

By object type — the breakdown of StageModel 18 and SuperStage 10 in A.1:

- **Scenic structures (18)**: truss 5, scaffold 2, drape 1, stage deck 1, barrier 1, roof 1, grandstand 1, stair tower 1, trackway 1, cable run 1, ballast 1, crowd objects 2.
- **Stage devices (10)**: screen 1, projection 1, light-strip effect 1, lifting machinery 1, rail machinery 1, director camera 1, laser pattern object 1, laser device object 1, Madrix object 1, white-model render object 1.

The lift matrix and lift ball are in-house fixtures, counted under SuperLight in A.1, not here.

---

## Appendix B: Technical Reference

For system-integration and network-configuration personnel.

| Item | Parameters |
| --- | --- |
| Art-Net | UDP port 6454 (default); unicast or broadcast; network universe numbering 0–32767 |
| sACN (E1.31) | UDP port 5568 (default); standard multicast address computed per universe, unicast also supported; network universe numbering 1–63999; multicast depends on IGMP |
| DMX internal cache | Architectural limit of 512 universes × 512 channels; network numbering mapped to internal numbering via the "start universe" setting |
| DMX channel precision | 8 / 16 / 24-bit (Coarse / Fine / Ultra) |
| NDI pixel formats | BGRA / BGRX received directly; UYVY / UYVA converted automatically; other formats unsupported |
| NDI recording | Configurable target frame rate, scaling ratio, and maximum duration (stored at reduced resolution by default to control memory) |
| grandMA2 connection | Remote command channel (default port 30000); requires console / onPC sign-in permission |
| grandMA3 connection | OSC command channel (default port 8000); one-way delivery |
| SuperLaser input | Network point-cloud data from external laser-control software (UDP multicast), 60 device slots by default; supported software per official documentation |
| Interface languages | Simplified Chinese, English |

---

## Copyright Notice

This document is copyright © 2026 Foshan Yierranran Technology Co., Ltd. (LimxTeam). All rights reserved.

NDI® is a registered trademark of the Vizrt Group. grandMA is a trademark of MA Lighting. Unreal Engine is a trademark of Epic Games. GDTF and MVR are specifications and trademarks of the GDTF Group. Madrix is a trademark of inoage GmbH. Pangolin and Beyond are trademarks of Pangolin Laser Systems. Other trademarks belong to their respective owners.

References to third-party products and trademarks in this document are descriptive, for compatibility and interoperability purposes only, and do not imply any partnership, certification, endorsement, or affiliation between their owners and our company.
