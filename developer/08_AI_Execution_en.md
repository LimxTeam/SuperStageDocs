# AI / UE5MCP Operating Procedure: Configuration, Event Graphs, and Acceptance

[Back to Contents](README_en.md)

## 1. Facts That Must Not Be Guessed

AI must first discover the current UE5MCP tools and schemas, then read the current project, level, PIE state, plugin version, target object paths, existing Blueprints, and data assets. Tool names come from the actual service; do not treat this documentation's step names as tool names.

Read parent-class reflection information and confirm SuperDmxActorBase, SuperFixtureLibrary, the three Super events, and the functions and pins to be used. Known class paths are `/Script/SuperCore.SuperDmxActorBase` and `/Script/SuperCore.SuperFixtureLibrary`. If the current version differs, report the actual reflection discrepancy first rather than forcing node creation.

All project DMX graphs enter only through Super events. **Do not add Event Tick polling, Tick override polling, timer-based DMX polling, same-named Custom Events impersonating Super events, or external Bind SuperDMXChanged.** Preserve base-class scheduling; do not disable its updates and create another loop.

Do not include SuperAssets/SuperDMX/SuperNdi/SuperTools headers. Existing implementations provide the basis for this documentation; code delivered to users uses only SuperCore and standard Unreal APIs. Project-defined functions, fields, and events must be identified as additions.

## 2. Stage A: Convert the User's Goal into a Protocol

Specify the target Actor/component, control axis or material parameter, absolute value versus velocity, bit depth, physical range and units, need for editor preview, initial no-input policy, and initial-level rule for one-shot commands. Read what is available from the existing project first; ask the user to clarify application meaning only where it remains uncertain.

Choose from the supplied protocols:

| Requirement | JSON | Super entry point |
| --- | --- | --- |
| Six-axis/single-axis position and rotation | Motion6Axis_12CH.json | Super DMX Tick |
| Color and material effect parameters | MaterialControl_6CH.json | Light Initialization prepares the MID; Super DMX Changed writes parameters |
| Modes and buttons | CommandControl_2CH.json | Super DMX Changed, with project-state deduplication |
| Independent positions for multiple objects | MultiObject_6CH.json | Super DMX Tick, with explicit module→target mapping |

If the target already exists, create a separate control Blueprint referencing it; do not arbitrarily change the application Actor's parent. Controlling the object's own components uses the same Super event chain.

## 3. Stage B: Generate/Import the Library and Read It Back

Create a project Super Fixture Library under `/Game/ProjectDMX/`. Actual asset-creation and import capabilities depend on the tools; prefer the existing JSON import flow, or use available structure-array editing if import is unsupported. Do not assign a disk JSON path to FixtureLibrary as though it were a uasset path.

Generated JSON must match the implemented fields in the [channel library chapter](04_SuperFixtureLibrary_en.md). Import overwrites Modules; verify the purpose of existing assets first. After saving, read back the class, object path, module count and order, ModuleName/Patch, each AttribName/Coarse/Fine/Ultra, and segment ranges.

Calculate every byte's relative and absolute position; check 1..512 bounds, bit depth, duplicate names, byte overlap, gaps in ranges, and units. An import success message does not replace these checks. Default percentages in the protocol are not runtime input initial values.

## 4. Stage C: Build Event Graphs or Compile Project Classes

Blueprint: create with the correct parent and add actual inherited events. Light Initialization prepares reusable resources; Super DMX Tick handles continuous motion; Super DMX Changed handles changing state. Store all Pure readings in variables before using them in multiple calculation branches.

Query the actual pins for reference outputs; do not invent an input pin based on the name InOutDefault. Blueprints needing explicit fallback use ByIndex's DefaultValue=-1 and check failure after validating addresses. Divide Raw16 by 65535; do not divide matrix outputs again.

C++: place the [project examples](examples/README_en.md) in the same project module and verify Build.cs, generated headers, and UHT. Ensure only the required Native Super methods are overridden; obtain DeltaSeconds from World. Preserve Super calls and do not call ForceRefreshDMX inside events.

Compile after each graph edit and read errors, then read back key nodes and connections. Successfully creating nodes does not mean the integration is connected. If the tool cannot create an actual inherited event, record the missing capability and specific remaining steps; do not substitute a Custom Event and claim completion.

## 5. Stage D: Bind Scene Objects

Set FixtureLibrary, SuperDMXFixture, and ControlMode on the control object, preserving other fields when updating structures. Reference targets by their actual object paths, not just potentially duplicate Labels. Verify target mobility, coordinate space, material slots, and parameter names individually.

Read back actual values after writing properties, and save the Blueprint, library, and level. Leave the project motion switch off initially; enable it only after confirming addresses and targets. Editor preview follows only the project's explicit switch; construction callbacks must not execute one-shot commands.

## 6. Stage E: Verify with Reproducible Inputs

The following test vectors must be produced through an actually supported sender/test tool, then read back from the object. Modifying private snapshots or directly changing the target Transform does not count as a DMX connection test.

| Example | Configuration and input | Expected result |
| --- | --- | --- |
| Fine position | 12CH library, Start101, 105/106=0/255→1/0 | Raw255→256, verifying coarse/fine order |
| Position midpoint | 105/106=128/0, start/end Z=0/300 | Raw32768, target Z approximately 150.0023 |
| Ongoing interpolation | Hold the same input after reaching the endpoint | SuperDMXTick keeps running; the target continues approaching the endpoint |
| Color | 6CH material library, Start201, 201..203=255/0/0 | MID ProjectColor=(1,0,0,1) |
| Mode boundaries | 2CH command library, Start301, 301=63/64/191/192 | Idle/Preview/Preview/Run |
| Application edge | 302=0/127/128/255/255/0/128 | Cumulative commands 0/0/1/1/1/1/2 |
| Multiple targets | 6CH multi-object library, Start401, three byte pairs=0/0,128/0,255/255 | A at start, B midway, C at endpoint, with no misalignment |

Changed counts may increase because of construction or changes in other channels, but Command counts must follow the application state machine. Identical network packets do not guarantee Changed. No input, an incorrect library, or out-of-bounds addresses may still read as 0; diagnose resolved addresses, default paths, and the actual signal separately.

## 7. Troubleshooting Order

1. **No Super events:** Check the parent/event nodes, preservation of base-class scheduling, authorization, and runtime conditions. Do not add Event Tick as an initial “fix.”
2. **SuperTick runs but Changed does not:** Check FixtureLibrary, a non-empty valid span, complete boundaries, the current internal Universe snapshot, and whether values actually changed.
3. **Addresses are correct but values are wrong:** Verify network Universe mapping, StartAddress, the one-based Patch rule, byte order, and sender protocol.
4. **Values are correct but the object does not move:** Check project Enabled, runtime/preview gates, target validity and mobility, axes and coordinate space, and physics conflicts.
5. **Moves only a little on each change:** Check whether continuous motion was mistakenly placed in Changed; move it to SuperDMXTick, not the engine Tick.
6. **Button actions repeat:** Check whether Changed is being treated directly as a button; add your own initial baseline and rising-edge logic.
7. **Multiple objects are misaligned:** Check whether sorted/filtered array indices are being treated as module indices; use explicit index mapping.
8. **NDI texture is not displayed:** Check the input name, SourceMode, first frame, MID, and parameters. GetActiveTexture is not a native Blueprint node and requires the correct project bridge.

## 8. Delivery Record

Mark the actual result of each item: asset created/saved; graph or C++ compiled; properties and addresses read back; real input verified; target results verified; reload verified; packaged build verified. Mark unperformed stages as unverified rather than treating “should work” as a result.

If only documentation/code/JSON was generated, clearly identify it as files awaiting import/compilation. Acceptance testing in an external project using the official delivery package must be independent of the internal full-source project.

## 9. Direct Task Instructions for AI

> First read this directory's integration contract, Super events, complete API, channel library, and Blueprint examples. Use the current UE5MCP's actual tool schemas to read back project state and objects. My DMX control must enter only through SuperStage's own Light Initialization, Super DMX Tick, and Super DMX Changed events, or their corresponding Native methods in C++. Configure the library, read, map, and control my own targets according to the existing SuperAssets calling patterns. Do not create Event Tick polling, guess tool names/nodes/pins, or include non-exposed modules. Create SuperFixtureLibrary using actually supported JSON import or property editing, then save and verify every address. Perform compilation, input testing, target-result checks, and reload verification individually, and report only stages actually completed.
