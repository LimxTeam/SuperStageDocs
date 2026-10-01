# C++ Integration: Follow the Super Event Call Chains of Existing Objects

[Back to Contents](README_en.md) · [Complete Example Files](examples/README_en.md)

## 1. Project Structure

Place example headers in your module's Public directory and cpp files in Private. Build.cs depends on Core, CoreUObject, Engine, and SuperCore. `.generated.h` must be last in the header's include list. The examples have no cross-module export macro and are intended for one project module; add your module's API macro if other modules need to call them.

DMX examples include only exposed SuperCore headers and standard Unreal headers. **Do not inherit concrete SuperAssets implementation classes, include their headers, or override the engine Tick.** Follow their event organization only.

Project classes appear in the editor's class list only after compilation. When adding UCLASS/UFUNCTION changes reflection, use the project's normal compilation process to complete UHT and module loading; merely copying files does not mean usable nodes have appeared.

## 2. Continuous Motion: ProjectDmxMotionController

Complete files: [header](examples/ProjectDmxMotionController.h), [implementation](examples/ProjectDmxMotionController.cpp). They follow the existing machinery's call structure:

```cpp
void AProjectDmxMotionController::NativeSuperDMXTick()
{
    Super::NativeSuperDMXTick();
    ReadDMXAndApply();
}
```

ReadDMXAndApply is a project function, not a base-class API. It checks the project enable state, ControlMode, target, world, and addresses, then reads PositionInput with GetSuperDmxAttributeValue, performs Clamp, Lerp, and optional VInterpTo, and finally applies the result to the external TargetActor.

Configuration steps:

1. Compile the project and place ProjectDmxMotionController.
2. Assign the imported Motion6Axis_12CH library to FixtureLibrary; set Universe according to the input mapping and StartAddress=101.
3. PositionInput defaults to MakeDmx(ZPos,true): module 0, attribute ZPos, 16-bit, at absolute addresses 105/106. Do not set InstanceIndex to 2 for the “third axis” just because this is a six-axis library; the six axes are six attributes in the same module.
4. TargetActor references your movable platform that is not driven by physics. Enter world coordinates for StartPosition/EndPosition, such as the actual start and start+(0,0,300). The target cannot be the controller itself.
5. Once configured correctly, set bEnabled=true. bPreviewInEditor defaults to false; explicitly enable it only if editor preview is needed. In a game world, BeginPlay must already have occurred to prevent motion during construction.
6. Verify the endpoint values and coarse/fine rollover at 105/106. With interpolation enabled, hold the input; the platform should continue converging.

This example uses the same reference-preservation semantics as the existing machinery: without a snapshot, PositionAlpha retains its previous value, initially 0. Prepare the input before enabling; an initial lack of input may apply the configured starting position. If the application requires “move only after the first readable data,” use Raw16 with a default of -1 and a HasValidInput gate in the same SuperDMXTick handler. This changes only the failure policy, not the event entry point.

NativeLightInitialization does not recapture the starting point from the target position. Configuration changes use explicit world-space endpoints, without accumulating offsets on each construction. This example does not include physics simulation, collision sweeps, stream-loss detection, or new-packet timing.

## 3. Changing Parameters: ProjectDmxMaterialController

Complete files: [header](examples/ProjectDmxMaterialController.h), [implementation](examples/ProjectDmxMaterialController.cpp). Two call chains are used:

```text
NativeLightInitialization → Super → EnsureMaterial → ApplyCachedParameters
NativeSuperDMXChanged → Super → Validate target/mode/addresses → Read parameters → ApplyCachedParameters
```

EnsureMaterial and ApplyCachedParameters are both project functions. As in the existing material object, initialization prepares the MID and Changed reads and updates parameters. Check the MID's Outer and source-material changes to avoid incorrectly sharing an old instance after duplicating an object or changing its material.

Import MaterialControl_6CH.json and assign the library with StartAddress=201. Select your StaticMeshActor as TargetActor, your material as BaseMaterial, and the actual slot as MaterialIndex. The material needs ProjectColor (Vector) and ProjectEffect/ProjectSpeed/ProjectWidth (Scalar), used in its calculations. With bEnabled=true, reversible material preview may be applied during initialization/construction.

Read Red/Green/Blue to combine a linear color; map Effect to 0..10, Speed to -10..10, and Width to 0..10. These names and ranges are this example's material protocol; your own material must use the same semantics before adopting them directly.

The default cache is white, Effect=0, Speed=0.5, and Width=0. Reference-based reads preserve the cache when no readable input exists. Property mode does not read DMX; initialization may still apply cached default material parameters. This does not mean Property automatically becomes application property-driven control.

## 4. Three Responsibilities in One Class

When a project control object needs to control both motion and materials, combine the two approaches: NativeLightInitialization prepares resources; NativeSuperDMXTick reads position/velocity and applies it continuously; NativeSuperDMXChanged reads colors/modes and updates state. Do not put all logic in Changed and thereby stop continuous motion, or create duplicate application chains for the same parameter.

A project Native method calls Super once, and the parent forwards the Blueprint event. Following existing examples, this documentation calls Super before project logic, so Blueprint executes first. If a child Blueprint needs the result after C++ updates it, the project can declare a separate completion notification, but it must clearly be a new event. Do not manually call the base SuperDMXChanged again.

## 5. Reading Raw Segments

Inside the project handler called by NativeSuperDMXChanged:

```cpp
const int32 Raw = GetAttributeRaw8ByIndex(0, TEXT("Mode"), -1);
const FSuperDMXAttributeDef* Def = FindAttributeDef(0, TEXT("Mode"));
const FSubAttribute* Segment = (Raw >= 0 && Def)
    ? Def->FindSubAttribute(Raw) : nullptr;
if (Segment)
{
    const FName NewMode = Segment->Name;
    // Project: compare with the cached mode; call the target's application function only on change.
}
```

This is a segment-query example, not a complete state machine. Complete application logic must first validate addresses and runtime gating, then maintain its own CurrentMode/Trigger edge state; see [Blueprint Example Four](05_Blueprint_Integration_en.md). Handle nullptr, and do not cache Segment across library edits.

## 6. Reading Multiple Modules

In NativeSuperDMXTick, read Raw16 per module, or use GetMatrixAttributeRaw16WithIndex: Values[k] and Indices[k] form a pair, with the latter holding the original module index. Validate target array indices and references first, then apply each value to its module's target.

Matrix values are already normalized; the default 0 without a snapshot cannot identify failure. For projects that gate on the first valid input, per-module ByIndex with default -1 is more explicit. Recheck module-to-target mapping after structural library changes, and do not cache raw pointers into the old structure.

## 7. Compilation and Delivery Acceptance

The examples are project source code, not a compiled SDK. Verify that UHT, compilation, and linking pass, the editor can place the classes, default fields and library references survive saving/reloading, and real input triggers the corresponding Super events; then verify target results. Do not claim the integration is connected before all checks are complete.

No plugin functional code was modified, and no non-exposed module implementation was copied into the examples. Source-reference tables are maintained separately in internal review records; customers need only the officially exposed interfaces and a matching delivery package.
