# SuperStage Developer Documentation: DMX Object Control and NDI Media Applications

Source baseline: version identifier **26H2.7** in the current repository's `SuperStage.uplugin`. Reviewed on: 2026-09-29. Intended for project developers and AI operating Unreal Engine through UE5MCP.

This documentation is based on the existing product implementation: the DMX sections follow the calling patterns of the existing lifting machinery, rail machinery, and material control objects; the NDI sections follow the media hooks used by screen and projector objects. It covers controlling your own models, platforms, mechanisms, materials, and application state, or using NDI textures on your own display objects.

## Main Integration Path

DMX control classes derive from `ASuperDmxActorBase`, configure `SuperDMXFixture` and `FixtureLibrary`, and read and apply data in **Super's own events**. They can control their own components directly, or hold a reference to an external ordinary Actor and control it.

| Task | Blueprint event | C++ override entry point |
| --- | --- | --- |
| Initialize references, materials, and default state | Light Initialization | NativeLightInitialization |
| Continuous motion, interpolation, and velocity integration | Super DMX Tick | NativeSuperDMXTick |
| Update color, modes, and application targets after DMX parameters change | Super DMX Changed | NativeSuperDMXChanged |

**Project DMX logic does not connect to Event Tick, override the engine Tick for polling, or replace Super events with timers.** The base class handles the underlying scheduling; the project keeps the base class operating normally. Blueprint and C++ use the same event model.

NDI uses `ASuperMediaBase`: the base class manages subscriptions and texture updates. C++ consumers override `OnActiveTextureChanged()`, call `GetActiveTexture()`, and apply the result to their own materials. Media Blueprint availability differs from DMX; see the media chapter, and do not assume undeclared nodes exist.

## Reading Order

1. [Integration Boundaries, Project Preparation, and Source Basis](01_Integration_Contract_en.md)
2. [Super Events: Responsibilities, Timing, and Usage](02_Super_Events_en.md)
3. [SuperDmxActorBase.h Item-by-Item Reference](03_SuperDmxActorBase_Reference_en.md)
4. [SuperFixtureLibrary: Fields, JSON, and Asset Creation](04_SuperFixtureLibrary_en.md)
5. [Blueprint Integration: Event Graphs and Multiple Examples](05_Blueprint_Integration_en.md)
6. [C++ Integration: Complete Project Examples](06_CPP_Integration_en.md)
7. [NDI: Media Hooks, Blueprint Bridge, and Applications](07_NDI_Integration_en.md)
8. [AI / UE5MCP Operating Procedure and Acceptance Checks](08_AI_Execution_en.md)
9. [Example Files and Channel Protocols](examples/README_en.md)

Before taking action, AI should read at least Chapters 1–5 and Chapter 8. Verify function names, event names, and structure fields against both this documentation and the reflection results from the installed package; do not infer parameters from display names or invent tool names.

## Exposed Interfaces and Delivery Boundaries

SuperStage is a commercial, closed-source plugin. What is exposed is **the integration capability provided by the SuperCore core module's header files**; exposing headers does not make the source code open source. Other modules are not user-facing C++ development interfaces. Write project code in your own module, depending only on the required standard Unreal modules and SuperCore.

Project example classes, helper functions, events, and JSON files in this documentation are explicitly identified as “project examples”; they are not APIs supplied by the plugin installation. Existing built-in objects are used to verify calling patterns; users are not required to obtain other modules' source code or add them as build dependencies.

The documentation and examples have undergone static source review and address and protocol validation. No claim is made that UHT/compilation/linking against a customer delivery package, UE5MCP operation, real DMX/NDI testing, or packaged-build testing has been completed. Chapter 8 describes validation for each stage; “the documentation contains code” is not a substitute for actual verification.
