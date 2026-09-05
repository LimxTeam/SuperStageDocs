# SuperStage User Documentation

> Current version: SuperStage 26H2.6 ｜ [中文](README.md)

This folder is the entry point to the SuperStage user manual. It is written for lighting designers, previs, video, delivery and editor operators, and does not assume the reader looks at code.

These documents follow a few rules:

- Only describe behaviour that can be confirmed in the current implementation, shipped content, or plugin configuration.
- No fixed promises about performance, latency, frame rate, or hardware scale.
- No removed module is described as a current feature.
- No internal implementation detail, class name, macro, or C++ usage in the general user manual, unless the user has to see that name in order to operate the product.

Every document exists in Chinese (`_zh`) and English (`_en`).

## Plugin Modules

SuperStage is a single plugin. It currently contains the following modules, all shipped with the main plugin:

| Module | Type | Responsibility |
| --- | --- | --- |
| SuperCore | Runtime | Core stage objects: fixtures, lights, beams, fountain |
| SuperDMX | Runtime | Art-Net / sACN send and receive, universe buffers, Sequencer DMX tracks |
| SuperNdi | Runtime | NDI input and recording |
| SuperMadrix | Runtime | LED matrix visualisation |
| SuperLaser | Runtime | Laser rendering, laser animation assets and runtime |
| SuperShader | Runtime | Render pipelines for stage beams, cone light, fountain |
| SuperAuth | Runtime | Account and entitlement |
| SuperAssets | Runtime | Stage structures, procedural scenic objects, in-house fixtures |
| SuperTools | Editor | Asset browser, patching, fixture editor, GDTF / MVR / grandMA tool panels |
| SuperConsole | Editor | Lighting console |

---

## Start Here

| I want to… | Read this |
| --- | --- |
| Understand what the product does | [Product Documentation](SuperStageProductDoc_en.md) |
| See what changed in this release | [Changelog](changelog_en.md) |
| Build a scene and place fixtures | [Asset Browser](editortools/02_AssetBrowser_en.md) |
| Find a fixture that is not in the library | [GDTF Import](fixture/02_GdtfImport_en.md) |
| Build a fixture from scratch with no GDTF | [Fixture Builder](fixture/06_FixtureBuilder_en.md) |
| Connect a console and drive DMX | [DMX System Overview](stagecore/00_DMX_System_Overview_en.md) |
| Program without an external console | [Console Overview](console/00_Console_Overview_en.md) |
| Create laser content | [Laser Overview](laser/00_Laser_Overview_en.md) |
| Take in an NDI video feed | [NDI Input Configuration](editortools/04_NDIConfigPanel_en.md) |
| Place effect machines and fountains | [Stage VFX](stagecore/14_Stage_VFX_en.md), [Stage Fountain](stagecore/16_Fountain_en.md) |

---

## Documents by Area

### Overview

- `SuperStageProductDoc_en.md` — product positioning, capability scope, boundaries, resource inventory
- `changelog_en.md` — release changelog

### Fixture System (rebuilt in 26H2.6)

A fixture is no longer a Blueprint but a fixture definition that can be imported, edited, and upgraded.

- [`fixture/00_FixtureSystem_Overview_en.md`](fixture/00_FixtureSystem_Overview_en.md) — the three assets, how they divide the work, compiling
- [`fixture/01_FixtureDefinition_en.md`](fixture/01_FixtureDefinition_en.md) — the definition asset field by field
- [`fixture/02_GdtfImport_en.md`](fixture/02_GdtfImport_en.md) — single import, batch import, import scope, upgrade in place
- [`fixture/03_FixtureEditor_en.md`](fixture/03_FixtureEditor_en.md) — the four pages of the fixture editor
- [`fixture/04_Motion_en.md`](fixture/04_Motion_en.md) — pan/tilt travel, the speed channel, continuous rotation, multi-head
- [`fixture/05_AttributeNames_en.md`](fixture/05_AttributeNames_en.md) — reference for the 119 DMX attribute names
- [`fixture/06_FixtureBuilder_en.md`](fixture/06_FixtureBuilder_en.md) — building by hand: a complete recipe per fixture type

### Lighting Console (merged into the main plugin in 26H2.6)

- [`console/00_Console_Overview_en.md`](console/00_Console_Overview_en.md) — entry point, interface, first run, boundaries
- [`console/01_Programming_en.md`](console/01_Programming_en.md) — selection, programmer, groups, presets, slot appearance
- [`console/02_Cues_and_Playback_en.md`](console/02_Cues_and_Playback_en.md) — cues, executors, output arbitration, timeline, timecode
- [`console/03_CommandLine_en.md`](console/03_CommandLine_en.md) — the mode × target matrix
- [`console/04_Effects_en.md`](console/04_Effects_en.md) — the Frame effect engine
- [`console/05_ShowFile_and_Undo_en.md`](console/05_ShowFile_and_Undo_en.md) — show file format and undo

### Laser

- [`laser/00_Laser_Overview_en.md`](laser/00_Laser_Overview_en.md) — the three capability layers, arbitration, safety boundary
- [`laser/01_LaserAnimationAsset_en.md`](laser/01_LaserAnimationAsset_en.md) — the laser animation asset
- [`laser/02_CanvasEditor_en.md`](laser/02_CanvasEditor_en.md) — canvas editor
- [`laser/03_Effects_en.md`](laser/03_Effects_en.md) — the six effect categories
- [`laser/04_Baking_and_ILDA_en.md`](laser/04_Baking_and_ILDA_en.md) — baking and ILDA

### DMX and Fixtures

- [`stagecore/00_DMX_System_Overview_en.md`](stagecore/00_DMX_System_Overview_en.md)
- [`stagecore/01_DMX_Network_Configuration_en.md`](stagecore/01_DMX_Network_Configuration_en.md)
- [`stagecore/02_Fixture_Library_en.md`](stagecore/02_Fixture_Library_en.md) — channel libraries
- [`stagecore/03_DMX_Actor_Base_en.md`](stagecore/03_DMX_Actor_Base_en.md)
- [`stagecore/06_DMX_Activity_Monitor_en.md`](stagecore/06_DMX_Activity_Monitor_en.md)
- [`stagecore/07_Patch_Tools_en.md`](stagecore/07_Patch_Tools_en.md)
- [`stagecore/08_DMX_Recording_Playback_en.md`](stagecore/08_DMX_Recording_Playback_en.md)
- [`stagecore/09_Export_To_MA_en.md`](stagecore/09_Export_To_MA_en.md)
- [`stagecore/10_Stage_Machinery_en.md`](stagecore/10_Stage_Machinery_en.md)
- [`stagecore/11_GrandMA_Link_en.md`](stagecore/11_GrandMA_Link_en.md)
- [`stagecore/12_Lift_Matrix_en.md`](stagecore/12_Lift_Matrix_en.md)
- [`stagecore/13_Light_Strip_Effect_en.md`](stagecore/13_Light_Strip_Effect_en.md)
- [`stagecore/14_Stage_VFX_en.md`](stagecore/14_Stage_VFX_en.md)
- [`stagecore/16_Fountain_en.md`](stagecore/16_Fountain_en.md) — stage fountain (new in 26H2.6)
- [`stagecore/17_InHouseFixtures_en.md`](stagecore/17_InHouseFixtures_en.md) — in-house fixtures (Aurora / Blaze / Flare / Hyperion / Spark / Thunder / Machinery)

### Light Components

- [`lightcomponent/00_LightComponent_Overview_en.md`](lightcomponent/00_LightComponent_Overview_en.md) — inheritance and what changed in this release
- `lightcomponent/01_SuperLightingComponent_en.md`
- `lightcomponent/02_SuperSpotComponent_en.md`
- `lightcomponent/03_SuperBeamComponent_en.md` — material beam (legacy)
- `lightcomponent/04_SuperShaperComponent_en.md` — material framing (legacy, formerly Cutting)
- `lightcomponent/06_SuperEffectComponent_en.md`
- `lightcomponent/07_SuperMatrixComponent_en.md`
- `lightcomponent/09_SuperLiftComponent_en.md`
- `lightcomponent/10_SuperConeLightComponent_en.md` — in-house cone light (new in 26H2.6)
- `lightcomponent/11_SuperVolumetricBeamComponent_en.md` — volumetric beam
- `lightcomponent/12_SuperVolumetricShaperComponent_en.md` — volumetric framing
- `lightcomponent/13_SuperRayBeamComponent_en.md` — ray beam
- `lightcomponent/14_SuperWashComponent_en.md` — wash

### Editor Tools

- [`editortools/01_MainMenu_and_Toolbar_en.md`](editortools/01_MainMenu_and_Toolbar_en.md)
- [`editortools/02_AssetBrowser_en.md`](editortools/02_AssetBrowser_en.md)
- [`editortools/03_DMXConfigPanel_en.md`](editortools/03_DMXConfigPanel_en.md)
- [`editortools/04_NDIConfigPanel_en.md`](editortools/04_NDIConfigPanel_en.md) — NDI input configuration
- [`editortools/05_BatchPatchTool_en.md`](editortools/05_BatchPatchTool_en.md)
- [`editortools/06_DMXPatchPreview_en.md`](editortools/06_DMXPatchPreview_en.md)
- [`editortools/07_DMXExportMA_en.md`](editortools/07_DMXExportMA_en.md)
- [`editortools/08_MVRImport_en.md`](editortools/08_MVRImport_en.md)
- [`editortools/09_VATGenerator_en.md`](editortools/09_VATGenerator_en.md) — VAT character generator
- [`editortools/10_FixtureLibraryEditor_en.md`](editortools/10_FixtureLibraryEditor_en.md) — channel library editor
- [`editortools/11_FixtureArrayTool_en.md`](editortools/11_FixtureArrayTool_en.md)
- [`editortools/12_SplineFixtureDistribution_en.md`](editortools/12_SplineFixtureDistribution_en.md)
- [`editortools/13_ColorAtlasBuilder_en.md`](editortools/13_ColorAtlasBuilder_en.md)
- [`editortools/14_GoboAtlasBuilder_en.md`](editortools/14_GoboAtlasBuilder_en.md)
- [`editortools/15_DMXActivityMonitor_en.md`](editortools/15_DMXActivityMonitor_en.md)
- [`editortools/17_UserAuth_and_Subscription_en.md`](editortools/17_UserAuth_and_Subscription_en.md)
- [`editortools/18_PrismPresetEditor_en.md`](editortools/18_PrismPresetEditor_en.md)

> All 14 tool panels have a help button in their top-right corner; its content matches these documents.

### Stage Assets

- [`stageassets/00_StageAssets_Overview_en.md`](stageassets/00_StageAssets_Overview_en.md)
- [`stageassets/01_SuperTruss_en.md`](stageassets/01_SuperTruss_en.md) — truss and goal posts
- [`stageassets/02_SuperScaffold_en.md`](stageassets/02_SuperScaffold_en.md) — scaffolding
- [`stageassets/03_SuperCurvedScaffold_en.md`](stageassets/03_SuperCurvedScaffold_en.md) — curved scaffolding
- [`stageassets/04_SuperDrape_en.md`](stageassets/04_SuperDrape_en.md) — drapes
- [`stageassets/05_SuperProjector_en.md`](stageassets/05_SuperProjector_en.md) — projector
- [`stageassets/06_SuperScreen_en.md`](stageassets/06_SuperScreen_en.md) — screen
- [`stageassets/07_SuperStageFloor_en.md`](stageassets/07_SuperStageFloor_en.md) — stage floor
- [`stageassets/08_SuperCircularTruss_en.md`](stageassets/08_SuperCircularTruss_en.md) — circular truss
- [`stageassets/09_SuperCurvedTruss_en.md`](stageassets/09_SuperCurvedTruss_en.md) — curved truss
- [`stageassets/10_SuperTrussGrid_en.md`](stageassets/10_SuperTrussGrid_en.md) — truss grid
- [`stageassets/11_SuperTrussTower_en.md`](stageassets/11_SuperTrussTower_en.md) — truss tower
- [`stageassets/12_SuperCrowd_en.md`](stageassets/12_SuperCrowd_en.md) — audience crowd
- [`stageassets/13_StageProgramObjects_en.md`](stageassets/13_StageProgramObjects_en.md) — barrier / roof / grandstand / stair tower / trackway / cable run / ballast
- [`stageassets/14_StageDevices_en.md`](stageassets/14_StageDevices_en.md) — director camera / Madrix / white-model render

Stage structure documents describe UE scene modelling and on-screen figures only. They are not a promise of real structural safety or regulatory compliance.

### Render System Notes

- [`SuperVolumetricBeam.md`](SuperVolumetricBeam.md) — volumetric beam tuning reference: runtime console variables, diagnostic commands and known boundaries (Chinese only)

### Legal and Partner Policies

- `legal/terms_en.md`, `legal/privacy_en.md`
- `SuperStageTeam政策.md`, `SuperStage授权经销商合作政策.md`, `SuperStage教育机构合作政策.md` (Chinese only)

> **Note**: Blueprint fixtures are deprecated in 26H2.6. Their documents (formerly `stagecore/04` moving lights, `stagecore/15` SuperStageLight) have been removed. For anything fixture-related, the `fixture/` folder is authoritative.

---

## Read Before Upgrading to 26H2.6

- **No fixture class redirectors for old projects.** Blueprint fixtures are deprecated from 26H2.6 and are no longer documented here. Existing projects still using them should stay on the old plugin rather than upgrading in place.
- If you previously installed SuperConsole as a separate plugin, remove it from the project's `Plugins` folder before upgrading.
- **The show file format moves to 2.7; files saved by this release cannot be read by older versions.**
- See [chapter 9 of the Product Documentation](SuperStageProductDoc_en.md) and the [changelog](changelog_en.md).
