# Project Example Files

[Back to Developer Documentation](../README_en.md)

These project examples were newly written for this documentation using public entry points verified against the existing implementation. They are not same-named classes/nodes already present after product installation, nor are they source code from non-exposed modules. This directory does not participate in the current plugin build. Copy the required examples into the same module of your own project and configure dependencies according to the [C++ chapter](../06_CPP_Integration_en.md).

## C++ Files

| Example | Files | Entry points |
| --- | --- | --- |
| External Actor motion | [ProjectDmxMotionController.h](ProjectDmxMotionController.h), [cpp](ProjectDmxMotionController.cpp) | NativeLightInitialization, NativeSuperDMXTick |
| Custom material parameters | [ProjectDmxMaterialController.h](ProjectDmxMaterialController.h), [cpp](ProjectDmxMaterialController.cpp) | NativeLightInitialization, NativeSuperDMXChanged |
| NDI texture application and Blueprint bridge | [ProjectMediaReceiver.h](ProjectMediaReceiver.h), [cpp](ProjectMediaReceiver.cpp) | OnActiveTextureChanged |

There is no project Tick override or Event Tick polling approach. DMX projects use only public SuperCore headers. Media projects must verify that the official SDK's header dependencies are complete before compiling the corresponding two files; do not add the media example if NDI is not needed.

The motion example uses explicit world-space Start/End configuration and is disabled by default. The material example requires your own ProjectColor/ProjectEffect/ProjectSpeed/ProjectWidth material parameters. The media example requires a ProjectMedia texture parameter. Missing parameters do not cause a material graph to be created automatically.

## Native JSON Libraries

| File | Relative channels | Description |
| --- | --- | --- |
| [Motion6Axis_12CH.json](libraries/Motion6Axis_12CH.json) | XPos1/2, YPos3/4, ZPos5/6, XRot7/8, YRot9/10, ZRot11/12 | Names/bit depths match the existing six-axis machinery's reads |
| [MaterialControl_6CH.json](libraries/MaterialControl_6CH.json) | Red1, Green2, Blue3, Effect4, Speed5, Width6 | Uses the verified material-parameter read pattern with a project-defined 6CH layout |
| [CommandControl_2CH.json](libraries/CommandControl_2CH.json) | Mode1, Trigger2 | New project mode and edge-trigger protocol |
| [MultiObject_6CH.json](libraries/MultiObject_6CH.json) | Three modules with Patch1/3/5, each with ZPos1/2 | New project multi-target position protocol |

Import into new data assets using the [library creation steps](../04_SuperFixtureLibrary_en.md). JSON keys have been checked against the existing parser. A JSON file is not a uasset and cannot be assigned directly to FixtureLibrary. Import overwrites Modules; read back and validate addresses afterward.

## Validation Status

The files have undergone static interface and protocol checks. UHT/compilation/linking against a customer SDK, import inside UE, real network input, and packaged-build acceptance testing have not yet been completed. Record actual operations according to the [AI execution procedure](../08_AI_Execution_en.md); do not present static review as a passed runtime test.
