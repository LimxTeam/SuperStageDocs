# Blueprint Integration: Control Your Own Objects with Super Events

[Back to Contents](README_en.md) · [Channel Library](04_SuperFixtureLibrary_en.md)

The following graphs are organized according to the actual BlueprintImplementableEvent and BlueprintPure declarations and describe how to build project Blueprints. They do not claim to provide uassets already generated in UE. AI must inspect node pins, connections, compilation, and runtime results in the current UE instance.

## 1. Create with the Correct Parent Class

Create a project Blueprint named BP_ProjectDmxController and select SuperDmxActorBase as its parent (reflection path `/Script/SuperCore.SuperDmxActorBase`). BP_ProjectDmxController is the project example name used here, not an asset shipped with the product. Existing application Actors keep their own parent classes; the control Blueprint stores references to them.

Find LightInitialization, SuperDMXTick, and SuperDMXChanged in event selection/Overrides; their display names are Light Initialization, Super DMX Tick, and Super DMX Changed. All three events come from the parent. **Do not create same-named Custom Events in place of inherited events.**

Do not add Event Tick, use Set Timer to drive DMX reads, or manually call ForceRefreshDMX inside events. The parent class schedules Super events.

Compile once, then place the object in the level. Configure FixtureLibrary, SuperDMXFixture, and ControlMode=DMX. Set FixtureLibrary through class defaults, instance details, or editor property tools; it has no runtime Blueprint Set property node.

## 2. Example One: 16-Bit Lifting of Your Own Platform

### Configuration Table

Use the library imported from Motion6Axis_12CH.json. Set internal Universe=1 and StartAddress=101; ZPos occupies absolute addresses 105/106. You may keep the complete library even when using only the Z axis; the other attributes need not be connected to application behavior.

Add these project variables:

| Variable | Type / example value | Purpose |
| --- | --- | --- |
| TargetActor | Actor reference, instance-editable | Your own platform |
| StartPosition | Vector, e.g. (0,0,0) | World-space start; enter the target's actual starting position |
| EndPosition | Vector, e.g. (0,0,300) | World-space endpoint |
| Enabled | bool, initially false | Enable after configuration checks |
| PreviewInEditor | bool, initially false | Explicitly allow editor motion preview |
| PositionAlpha | float, initially 0 | Last valid normalized position |
| Interpolate | bool, initially false | Whether to interpolate continuously |
| InterpSpeed | float, e.g. 5 | Interpolation rate |

The target's root component must be movable. This example sets transforms directly; do not simultaneously drive it with physics simulation. StartPosition/EndPosition are configuration values, not recaptured from the already-moved position on each initialization.

### Light Initialization Graph

Check the TargetActor reference and start/end parameters, and initialize any resources the project needs. This example needs no new material; do not move the platform or change the user's configured endpoints here. Configuration problems may be logged, without repeatedly flooding the log on every event.

### Super DMX Tick Graph

```text
Event Super DMX Tick
  → Enabled is true
  → ControlMode equals DMX
  → TargetActor IsValid
  → Runtime state permits operation, or editor preview is explicitly enabled
  → ResolveAttributeAddressesByIndex(Self, 0, "ZPos")
  → CoarseAbs and FineAbs are both within 1..512
  → GetAttributeRaw16ByIndex(Self, 0, "ZPos", -1)
  → Save Raw to a local/variable to avoid repeated Pure evaluation
  → Raw>=0: PositionAlpha = Clamp(Raw / 65535.0, 0, 1)
  → DesiredPosition = Lerp(StartPosition, EndPosition, PositionAlpha)
  → Interpolate?
      Yes: VInterpTo(TargetActor position, DesiredPosition,
                    Get World Delta Seconds, InterpSpeed)
      No: DesiredPosition
  → SetActorLocation(TargetActor, result)
```

End this update if the address configuration is invalid. A no-data Raw=-1 preserves the existing PositionAlpha; this is the hold policy chosen for this example. If there has not yet been a valid value, the project may add a HasValidInput gate so the initial position is not automatically reset to zero. All reading, mapping, interpolation, and application are triggered by Super DMX Tick.

### Numeric Acceptance Checks

| Bytes at 105/106 | Raw16 | Target Z over a 0-to-300 cm range |
| --- | --- | --- |
| 0/0 | 0 | 0 |
| 0/255 | 255 | Approximately 1.1673 |
| 1/0 | 256 | Approximately 1.1719 |
| 128/0 | 32768 | Approximately 150.0023 |
| 255/255 | 65535 | 300 |

Hold 255/255; the platform should stabilize at the endpoint. With interpolation enabled, it should continue converging even while the input remains unchanged. If interpolation runs only on Changed, it will stop partway; that is an incorrect event choice. Reconstructing the control Blueprint must not turn the endpoint into a new starting point.

## 3. Example Two: Your Door Panel's Angle or Continuous Rotation

Use XRot from the 12-channel library at absolute addresses 107/108, reading Fine. The event entry point remains Super DMX Tick.

**Absolute angle:** Configure ClosedRotation and OpenRotation in the project, for example a door hinge's Yaw from 0 to 90 degrees. Normalize, Lerp the angle, and call SetRelativeRotation on your component. The door panel needs an appropriate pivot; otherwise, add a project SceneComponent as its rotational parent. Do not call AddLocalRotation(TargetAngle) each time, or the target angle will accumulate.

**Continuous rotation:** A different application mode maps normalized input to angular velocity per second, for example Lerp(-90,90,Alpha), then multiplies by Get World Delta Seconds and applies it with AddLocalRotation. The object keeps rotating while the same velocity input is held, so use Super DMX Tick. The 16-bit midpoint is not exactly 0.5; define an explicit dead zone if needed, such as a velocity of 0 when Raw is within 32760..32775. This is a project application rule, not an automatic plugin rule.

Verify that the maximum value in absolute mode stops at 90 degrees, while the maximum in velocity mode produces approximately 90 degrees per second independently of Changed counts. Do not run both modes on the same target simultaneously.

## 4. Example Three: Your Material's Color and Effect Parameters

Use MaterialControl_6CH.json with StartAddress=201. In module 0, Red/Green/Blue are 201/202/203, Effect=204, Speed=205, and Width=206. Displayed parameter names do not automatically create material parameters; create your own material and corresponding parameters first.

Project variables: TargetMesh, MaterialIndex, BaseMaterial, MID, Color, EffectValue, SpeedValue, and WidthValue. The example material's parameter names are ProjectColor, ProjectEffect, ProjectSpeed, and ProjectWidth; these are not fixed names required by the plugin.

**Light Initialization:** Validate the target and material slot. Create the MID if invalid or if the source material changes, store its reference, and apply it to the target slot. Repeated initialization reuses your MID; reapply it when the target/source changes to avoid accidentally sharing an instance after object duplication.

**Super DMX Changed:** First check ControlMode=DMX and that the project is enabled. Read Red/Green/Blue as three Raw8 values according to the library. On success, divide by 255 to form LinearColor(R,G,B,1). Read Effect, Speed, and Width as Raw8, normalize each, and map them to the ranges required by the project. Following the existing material-control pattern, Effect maps to 0..10, Speed to -10..10, and Width to 0..10; use these ranges only if the project material is actually designed for them.

Write the resulting values to the MID. Do not recreate the MID on every Changed or use the engine Tick to repeatedly write identical parameters. If your material animation is driven by material Time, it can continue animating while the same parameter values are held, without repeated binding.

Test 201..203 with 255/0/0, 0/255/0, and 0/0/255; readings should be (1,0,0), (0,1,0), and (0,0,1). When values are held, the normal Changed path stops triggering, but the material should remain displayed. Test Effect=255→10, Speed=0→-10, and Speed=255→10. If MID parameters are correct but the image does not change, check material connections, parameter spelling, and the slot.

## 5. Example Four: Modes and One-Shot Application Buttons

Use CommandControl_2CH.json with StartAddress=301: Mode=301 and Trigger=302, both 8-bit. Place reading and application processing in Super DMX Changed.

Mode library segments are 0..63 Idle, 64..191 Preview, and 192..255 Run. In Blueprint, use GetModules→module 0→find Mode in AttributeDefs→iterate SubAttributes, finding the first inclusive-range match. There is no native Blueprint FindSubAttribute node; do not invent one. The project may also use integer branches matching the protocol exactly, but must synchronize them with protocol changes.

Call the target's project mode-switch function only when the segment name differs from cached CurrentMode. Segment names do not automatically call functions.

For Trigger, add HasTriggerBaseline, LastTriggerHigh, and CommandCount. First confirm runtime state and valid addresses/readings; record the initial level, then call the target's application function once only on low→high transitions. Do not simply “execute on every Changed.”

For input 0,127,128,255,255,0,128, expected CommandCount is 0,0,1,1,1,1,2. An initially high value only establishes the baseline. Changing Mode while Trigger is high should change CurrentMode without increasing CommandCount. Editor construction does not execute application commands.

## 6. Example Five: Three of Your Own Objects Sharing Input Control

Use MultiObject_6CH.json: three modules ObjectA/B/C with Patch=1/3/5, each with ZPos coarse/fine offsets 1/2. With StartAddress=401, their addresses are 401/402, 403/404, and 405/406.

The control Blueprint stores a Targets array mapped one-to-one to module indices 0/1/2, with separate Start/End configuration for each target. In Super DMX Tick, ForLoop over module indices, read using GetAttributeRaw16ByIndex(Index,ZPos,-1), normalize, and apply to that target.

If a target or that module's attribute is missing, skip only that entry. Do not guess original module identity from filtered matrix-value array indices. If you explicitly use GetMatrixAttributeRaw16, its result is already 0..1 and sorted by Patch by default; modules missing the attribute are skipped. Blueprint has no native WithIndex node, so an explicit module loop is clearer.

With A=0/0, B=128/0, and C=255/255, expect the start, approximately 50% through the range, and the endpoint, respectively. Clearing B's target reference must leave A/C correct, without misalignment. If module order changes, synchronize the target mapping.

## 7. Blueprint Node Availability Checklist

| Operation | Native availability |
| --- | --- |
| Three Super events | Implementable in Blueprints derived from the correct parent |
| GetAttributeRaw8/16/24ByIndex | BlueprintPure |
| ResolveAttributeAddressesByIndex | BlueprintPure, multiple address outputs |
| GetModules / GetFixtureChannelSpan | BlueprintPure |
| GetMatrixAttributeRaw / Raw16 | BlueprintPure, normalized output |
| FindAttributeDef / WithIndex series | C++; cannot directly create these nodes |
| MakeDmx C++ helper | Not a Blueprint node; construct/break FSuperDMXAttribute in Blueprint |
| Runtime Set FixtureLibrary | No automatic Set node; configure through the editor |
| Bind Event to SuperDMXChanged | No such delegate; declare a project Dispatcher for cross-object broadcasts |

Pure functions have no execution pins. Save read results before branching/applying. Compile each completed graph and inspect errors; read back the actual connections rather than merely confirming that nodes were created.
