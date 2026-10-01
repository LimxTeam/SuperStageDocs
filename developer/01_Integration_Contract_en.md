# Integration Boundaries, Project Preparation, and Source Basis

[Back to Contents](README_en.md)

## 1. What This Documentation Covers

Allow your own Actors, components, materials, or application systems to accept DMX control, and use NDI video as input for your own materials, UI, and other purposes. SuperStage manages reception, protocols, threads, and texture upload. The project implements “what happens after reading the data.”

DMX control objects inherit from ASuperDmxActorBase. If an existing target Actor already has another parent class, create a separate project control object derived from this class and store a TargetActor/TargetComponent reference in it; the target does not need to change its parent. The target receives updates through the control object's Super events and does not establish its own engine Tick polling chain.

New project variables such as TargetActor, MoveStart, MoveEnd, and LastTriggerHigh are project data, not predefined ASuperDmxActorBase fields. Distinguish inherited properties from newly added properties before creating them.

## 2. Based on Actual Calls in Existing Objects

The following records verified calling behavior; users are not required to include the corresponding module headers:

| Existing object | Verified calls | Application in this documentation |
| --- | --- | --- |
| SuperLiftingMachinery | NativeSuperDMXTick → ReadDMXAndApply → LiftingMachinery; reads XPos/YPos/ZPos/XRot/YRot/ZRot using Fine | Place position, rotation, and ongoing interpolation in Super DMX Tick |
| SuperRailMachinery | NativeSuperDMXTick → ReadDMXAndApply | Continuous control such as rail motion uses the Super event entry point |
| SuperLightStripEffect | NativeLightInitialization → default material preparation; NativeSuperDMXChanged → ReadDMXAndApply | Initialize reusable materials; Changed updates parameters and external targets |
| Built-in lifting matrix object | Initialization prepares components; Changed updates effects/colors; SuperDMXTick performs lifting | Assign Super events by behavior within one control object |
| SuperScreen | OnActiveTextureChanged → GetActiveTexture → dynamic material; applies materials to a target mesh array | Apply media to your own objects through the texture reference change hook |
| SuperProjector | OnActiveTextureChanged applies the same active texture to multiple materials | One media source can serve multiple consuming materials |

Some old source comments say “called by Tick every frame,” but the function bodies are actually entered from NativeSuperDMXTick or NativeSuperDMXChanged. Determine integration entry points from declarations and call sites. Some built-in classes also have their own non-reception logic; this does not require projects to move DMX reading into the engine Tick.

## 3. Project and Licensing

Use plugin binaries and officially exposed headers matching your engine version and target platform, enable SuperStage, and confirm that the current account has the entitlements required for project extensions. The parent lifecycle includes authorization handling; do not remove parent calls or re-enable updates disabled by permission logic to bypass restrictions.

Public C++ class paths: `/Script/SuperCore.SuperDmxActorBase`, `/Script/SuperCore.SuperMediaBase`, and `/Script/SuperCore.SuperFixtureLibrary`. The media base class is Abstract; create a concrete derived class. Before creating a Blueprint, read the current editor's parent-class/reflection information; do not guess paths using an old module-name prefix.

Add the following to the project module's Build.cs:

```csharp
PublicDependencyModuleNames.AddRange(new string[]
{
    "Core", "CoreUObject", "Engine", "SuperCore"
});
```

If these entries already exist, merge them without replacing other project dependencies. Use a public dependency when your public headers inherit SuperCore types. The documentation examples do not add SuperAssets, SuperDMX, SuperNdi, or SuperTools as direct development dependencies or include classes from those modules.

SuperMediaBase.h currently has a cross-module include dependency. Compiling in the full-source repository does not prove that a delivery package containing only exposed headers can compile completely. Media examples must verify the header chain and link artifacts using the official SDK; missing dependencies must be resolved in product delivery, not by instructing users to copy non-exposed source code. DMX and NDI can be integrated independently; do not mix media compilation issues into a DMX-only task.

## 4. Connect the Input Before Connecting Application Behavior

In the product's DMX input configuration, confirm the protocol, network adapter, network Universe range, and actual input. Internal Universe mapping is:

```text
InternalUniverse = NetworkUniverse - InputStartUniverse + 1
```

For example, with NetworkUniverse=10 and InputStartUniverse=10, the object's internal Universe should be 1. This rule comes from the current reception implementation; do not copy console numbering directly onto the object.

Set ControlMode=DMX, SuperDMXFixture.Universe, and StartAddress on the object, and assign FixtureLibrary. Named attributes, matrix reads, and normal Changed detection all depend on the library. Although GetChannelValue can read independently, the complete examples still describe input through a library so that definitions, addresses, and events remain consistent.

For NDI, first configure and confirm an available source in the input panel, then select SourceMode=NDI and InputName. InputName is the logical name of a configured input, not an arbitrary IP address or stream URL.

## 5. Data Units

DMX bytes range from 0..255; 16-bit values from 0..65535; 24-bit values from 0..16777215. Normalized values range from 0..1. World/relative positions normally use centimeters, and angles use degrees; the protocol must specify how each value maps. A normalized value of 0.5 is neither 50 centimeters nor the exact normalized value of DMX byte 128.

All examples first store stable start/end positions or angles, then apply absolute targets. Only examples where “the input represents velocity” integrate using DeltaSeconds. Never repeatedly add an absolute angle as an increment.

## 6. Scope of This Documentation's Guarantees

Function signatures, reflection specifiers, calculations, and callback conditions are described according to the current source. Where per-network-packet delegates, stream-loss timestamps, or online-status guarantees are not exposed, do not invent those capabilities. An NDI texture reference change is not a per-frame CPU data notification either.

The UE5MCP tool list and version must be queried at runtime. This documentation provides precise semantic actions, class/function identifiers, and readback requirements; it does not invent universal tool names or parameter schemas.
