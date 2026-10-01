# Super Events: The Sole Project Entry Point for DMX Control

[Back to Contents](README_en.md) · [Function Reference](03_SuperDmxActorBase_Reference_en.md)

## 1. One-to-One Blueprint and C++ Mapping

| Responsibility | Actual Blueprint function name / display name | C++ entry point |
| --- | --- | --- |
| Initialization | LightInitialization / Light Initialization | NativeLightInitialization |
| Continuous processing | SuperDMXTick / Super DMX Tick | NativeSuperDMXTick |
| Change processing | SuperDMXChanged / Super DMX Changed | NativeSuperDMXChanged |

The three Blueprint entry points are declared as protected BlueprintImplementableEvent functions in category A.Event. They appear in the event/override list of a Blueprint derived from the correct parent. They are not Dispatchers that can be bound on an external Actor. In C++, override the virtual functions with the Native prefix; do not write nonexistent BlueprintNativeEvent implementations such as LightInitialization_Implementation.

By default, the base Native methods only forward to their corresponding Blueprint events. Existing SuperAssets examples call Super first inside a Native override, then run their own handler. The C++ examples here follow the same order. If both C++ and its derived Blueprint implement the same task, define which one applies the final output to avoid moving, binding, or triggering twice.

## 2. Light Initialization

Use this to configure component defaults, ensure dynamic materials exist, organize target references, and initialize resources that can be created repeatedly. Do not recreate the same MID or append the same component on every call. Existing material examples check whether the dynamic material already exists before creating and applying it.

This event is not “once per lifetime.” Both OnConstruction and BeginPlay call it; editor changes and Blueprint reconstruction can occur repeatedly. Do not put one-shot application commands here.

Motion ranges should be explicit configuration values, or be captured once by an explicit project initialization action. Do not treat an object's already-moved position as a new starting point on every Light Initialization. The project examples use editable start/end positions to avoid implicit repeated baseline capture.

## 3. Super DMX Tick

Use this for your platform motion, smooth interpolation, continuous rotation, or logic that must advance on each update. **Read channels and apply continuous behavior in this event**, as the existing lifting machinery does. It is not necessary to cache every input in Changed before performing motion.

A typical call chain:

```text
Super DMX Tick
  → Check ControlMode / project enable switch / target validity
  → Read attributes
  → Clamp to 0..1
  → Map to the project's physical range
  → Optionally interpolate or integrate per second
  → Set target position/rotation/parameters
```

The event has no DeltaTime pin. In C++, follow the existing machinery example and use `GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.0f`; in Blueprint, use Get World Delta Seconds for the current world. **Do not override Tick merely to obtain DeltaSeconds.** Editor construction may also enter this event; the project decides whether preview is allowed. Do not divide by DeltaSeconds when it is 0.

When input remains unchanged, continuous rotation must keep rotating and interpolation must keep approaching its target, so these processes cannot live only in Changed.

## 4. Super DMX Changed

Use this to read and update material colors, effect selections, discrete modes, and application state. You can read several attributes at once and apply them to multiple targets together. The existing material control example enters ReadDMXAndApply from NativeSuperDMXChanged in exactly this way.

It has no NewValue, OldValue, or Channel parameters. When the event arrives, the project uses read functions to obtain current values. Do not assume that one event corresponds to only one changed attribute.

During normal scheduling in DMX mode, the base class calculates the minimum-to-maximum valid channel range from FixtureLibrary and compares the byte snapshot of that entire contiguous range. The rules are:

| Condition | Triggered by change detection? |
| --- | --- |
| No library or no valid span | No |
| Universe has no readable snapshot | No |
| Either range endpoint is out of bounds, making the full range unreadable | No |
| First readable range, or range/length changes | Yes |
| Bytes in the range differ from the previous snapshot | Yes; the snapshot is updated before the callback |
| A new packet contains identical bytes | No |
| An undefined gap byte within the range changes | May also trigger |
| Universe changes but the range and bytes remain the same | Not guaranteed to trigger |

Consequently, a single incorrect distant offset in the library can block the entire normal Changed path. Reading a value with GetAttributeRaw8 alone does not prove that the Changed range is correct.

## 5. Complete Execution Order

```text
OnConstruction
  Parent construction
  NativeLightInitialization → Blueprint Light Initialization
  NativeSuperDMXTick        → Blueprint Super DMX Tick
  NativeSuperDMXChanged     → Blueprint Super DMX Changed (unconditional)

BeginPlay
  Parent BeginPlay
  NativeLightInitialization

Each base-class scheduling cycle
  Parent scheduling
  NativeSuperDMXTick
  DMX mode: change detection → NativeSuperDMXChanged only when conditions are met
  Property mode: NativeSuperDMXChanged directly
```

This underlying lifecycle explains when Super events are received; it does not instruct projects to create a separate Event Tick graph. Keep the parent scheduling in place.

Changed is called unconditionally during construction, and on every scheduling cycle in Property mode. Therefore, “receiving Changed” must not directly execute a one-shot application action, nor does it prove that a new network packet was received. The base class runs SuperDMXTick before Changed. If a project chooses to cache targets in Changed, it must account for this order; the continuous-motion examples read directly in SuperDMXTick to avoid unnecessary two-stage state.

## 6. The Project Must Choose Its Property-Mode Behavior

Read APIs use their default/preserved-value paths in Property mode. The existing machinery's ReadDMXAndApply checks for Property first and returns. The motion example follows this rule: it does not perform DMX-driven control in Property mode.

If the project needs “Details panel control,” it must define its own control variables and application logic. C++ reference-based reads can preserve the original variable value, but not all APIs behave this way; Raw and matrix functions have their own rules. Do not mistake every Changed call in Property mode for continuously changing DMX.

## 7. Use Application-Level Edges for One-Shot Commands

For your Trigger attribute, 0..127 is low and 128..255 is high. Add HasTriggerBaseline and LastTriggerHigh to the project. In Changed, first confirm the runtime world, project enable state, valid target, and ControlMode=DMX; after reading Raw8, process it as follows:

```text
No valid reading → Do not trigger
First valid reading → Record the level, establish a baseline, do not trigger
Current level high AND previous level low → Execute your action once
Update the previous level
```

This is an application rule explicitly chosen for the example, not a built-in plugin edge-detection feature. If a high level at startup must execute an action, the project must explicitly choose a different initialization rule. Editor construction, changes in other attributes, or repeated high values must not add extra commands.

## 8. Forwarding to an External Actor

A control object can call the target's project functions directly or declare its own BlueprintAssignable delegate/Blueprint Event Dispatcher. For example, OnProjectModeChanged is a newly added project API that the target binds to; do not create a Bind Event to SuperDMXChanged node.

The event source handles reading, mapping, and deduplication; the target handles its own behavior. Check references when targets are destroyed, and remove the receiver's bindings when its lifecycle ends. Keep each object's responsibilities clear; the controller and target must not both poll the same channel.

## 9. ForceRefreshDMX and Editor Preview

ForceRefreshDMX actively executes SuperDMXTick once, then processes changes according to the control mode and marks render state dirty. In the editor, it also calls PostEditChange, which may cause additional construction. Changed is not unconditional in DMX mode, and this function does not actively request a new network packet.

Do not call ForceRefreshDMX from any Super event; doing so risks reentrancy. The base class schedules normal input processing. When editor preview is allowed, use an explicit project switch to control reversible visual changes; irreversible application commands must run only under the project's defined runtime conditions.

During acceptance testing, count Init, SuperTick, Changed, and application Command separately. Do not check only whether the object moves. Holding a value should allow motion to continue, while one-shot commands must not repeat.
