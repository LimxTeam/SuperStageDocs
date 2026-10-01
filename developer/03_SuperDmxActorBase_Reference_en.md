# SuperDmxActorBase.h Item-by-Item Reference

[Back to Contents](README_en.md) · [Events](02_Super_Events_en.md) · [Channel Library](04_SuperFixtureLibrary_en.md)

This review covers all public and protected methods, properties, helper types, and macros in the current `SuperCore/Public/SuperDmxActorBase.h`, using the corresponding implementation to correct historical comments. Use read functions within the Super event call chain. “Blueprint Pure” below means that a pure function node is available from the corresponding object; it does not mean a standalone event or a constant value.

## 1. Class and Instance Properties

`ASuperDmxActorBase : ASuperBaseActor`, reflected class name SuperDmxActorBase, module SuperCore. A read function's Target is normally Self, your control object. If a project helper function accepts a source reference, it is still called from a Super event, without additional polling scheduling.

| Field | Default | Configuration and meaning |
| --- | --- | --- |
| SuperDMXFixture | FSuperDMXFixture | EditAnywhere, BlueprintReadWrite, Interp; contains the following three fields |
| FixtureID | 1 | Object ID; does not participate in byte address calculation |
| Universe | 1 | Internal Universe, constrained to 1..512 in the UI; must match the reception configuration mapping |
| StartAddress | 1 | 1-based starting channel within the current Universe, 1..512 |
| ControlMode | ESuperDMXControlMode::DMX | DMX reads the signal; Property uses each read function's fallback behavior and changes Changed scheduling conditions |
| FixtureLibrary | nullptr | USuperFixtureLibrary reference; EditAnywhere, BlueprintReadOnly; editor-configurable, not a runtime Blueprint Set variable |
| bIncludeFixtureIdInLabel | false | Editable, Blueprint-readable/writable legacy label option; the address text component has been removed, so this does not guarantee an address label will be displayed |

When editor tools modify selected SuperDMXFixture fields, preserve the remaining fields. C++ can assign FixtureLibrary at runtime; modifying a shared library in place affects all references to it. The control mode does not create project property-driven logic for you.

## 2. Addresses, Indices, and Values

```text
InstanceIndex = Modules array index (0-based)
ModuleBase = StartAddress + max(0, Modules[InstanceIndex].Patch - 1)
CoarseAbs = ModuleBase + Coarse - 1   (Coarse > 0)
FineAbs   = ModuleBase + Fine   - 1   (Fine > 0)
UltraAbs  = ModuleBase + Ultra  - 1   (Ultra > 0)
The absolute address output for an unused offset is 0.
```

Configure Patch as a 1-based module starting position. Configure Coarse/Fine/Ultra as 1-based positions within the module, with 0 indicating an unused byte. With StartAddress=101, Patch=5, Coarse=2, and Fine=3, the addresses are 106/107.

The historical source comments `StartAddress + Patch` and “Patch is 0-based” differ from the current calculation; use Patch≥1 for all new configurations. Patch=0 and Patch=1 currently happen to resolve to the same starting point; do not use this aliasing behavior. Addresses beyond 512 do not automatically continue into the next Universe.

```text
Raw8  = C
Raw16 = (C << 8) | F
Raw24 = (C << 16) | (F << 8) | U
The normalization divisors are 255, 65535, and 16777215, respectively.
```

All readings represent the currently readable snapshot, without packet timestamps. An unchanged value does not indicate disconnection, and a value of 0 does not indicate failure either.

## 3. ResolveAttributeAddressesByIndex

```cpp
bool ResolveAttributeAddressesByIndex(int32 InstanceIndex, FName AttribName,
    int32& OutCoarseAbs, int32& OutFineAbs, int32& OutUltraAbs) const;
```

**Availability:** Blueprint Pure, Address category; public in C++.

**Purpose:** Looks up the library by module index and attribute name, then calculates three absolute addresses without reading network values. InstanceIndex must be valid, and AttribName should be unique within that module. All outputs are initially set to 0; returns false if no match is found. After a match, returns true only if the absolute Coarse address is >0.

**Limitations:** true does not guarantee addresses ≤512 or available data. AI must separately check that all enabled bytes lie within 1..512. In the example, module 1's ZPos can resolve to 106/107, but this must be checked against the actual definition.

**Use cases:** Patch reports, library acceptance checks, off-by-one diagnosis, and address validation before reading. Do not use it as an online-status check.

## 4. FindAttributeDef (Two Overloads)

```cpp
const FSuperDMXAttributeDef* FindAttributeDef(int32 InstanceIndex, FName AttribName) const;
const FSuperDMXAttributeDef* FindAttributeDef(const FSuperDMXAttribute& DmxAttribute) const;
```

**Availability:** C++ only; no UFUNCTION.

Returns a pointer to the attribute definition in the library, or nullptr if not found. The second overload uses only InstanceIndex and AttribName from the selector and ignores DMXChannelType. You can read definitions such as SubAttributes and PhysicalRange; the function itself does not interpret application behavior or return a DMX value.

The returned pointer is borrowed. Do not delete it or retain it across library replacement or Modules/AttributeDefs modifications. Duplicate definitions with the same name currently use the first match; the project should reject duplicates outright. Do not distinguish attributes solely by FName letter case.

**Use cases:** After reading a mode attribute's raw value in Changed, find its definition and query its segments. There is no native Blueprint FindAttributeDef node; traverse the structures returned by GetModules, or use a bridge explicitly identified as a project addition.

## 5. GetAttributeRaw8ByIndex

```cpp
int32 GetAttributeRaw8ByIndex(int32 InstanceIndex, FName AttribName,
    int32 DefaultValue = 0) const;
```

Blueprint Pure, Read8 category. Reads only the Coarse byte, returning 0..255 on success; even if Fine exists, it does not combine a 16-bit value. Property mode, attribute resolution failure, or a Universe without a snapshot returns DefaultValue. **If a snapshot exists but the address is out of bounds, it returns 0, not DefaultValue.** The default value itself is not constrained to 0..255, so you can explicitly pass -1 to identify some failures, but addresses must still be validated first.

**Use cases:** Buttons/modes in Changed, and 8-bit target parameters in SuperDMXTick. Raw=128 normalizes to approximately 0.50196. Do not use Raw directly as a 0..1 material parameter.

## 6. GetAttributeRaw16ByIndex

```cpp
int32 GetAttributeRaw16ByIndex(int32 InstanceIndex, FName AttribName,
    int32 DefaultValue = 0) const;
```

Blueprint Pure, Read16 category. Requires both Coarse and Fine, and returns an integer in 0..65535 with Coarse as the high byte and Fine as the low byte. Without Fine, it returns DefaultValue; it **does not fall back to 8-bit**. Property mode, resolution failure, and no snapshot also return the default. If a byte is out of bounds in an existing snapshot, that byte is combined as 0; the entire value is not necessarily 0.

**Use cases:** Blueprint reads with an explicit fallback for the existing machinery's Fine attributes. C=128/F=0 gives 32768; C=0/F=255 gives 255; C=1/F=0 gives 256. Use the last two cases to verify byte order.

## 7. GetAttributeRaw24ByIndex

```cpp
int32 GetAttributeRaw24ByIndex(int32 InstanceIndex, FName AttribName,
    int32 DefaultValue = 0) const;
```

Blueprint Pure, Read24 category. Coarse must resolve. If Fine or Ultra is absent, the corresponding byte is filled with 0, and the result is still combined with 24-bit weights, in 0..16777215. Property mode, resolution failure, and no snapshot return DefaultValue. Out-of-bounds bytes in an existing snapshot are also 0.

**Use cases:** Parameters that genuinely use three bytes in the project protocol. With only Coarse=128 and no lower bytes, the result is 8388608, not 128; this function cannot “increase the precision” of an 8-bit protocol. If the protocol requires a complete 24-bit value, first verify that all three bytes are defined.

## 8. GetAttributeBitDepthByIndex

```cpp
int32 GetAttributeBitDepthByIndex(int32 InstanceIndex, FName AttribName) const;
```

Blueprint Pure, Info category. Returns 0/8/16/24 based on the resolvable byte positions: 0 on failure, 24 for C+F+U, 16 for C+F, and 8 for C. It does not check whether data has arrived or validate the upper bound. C+U without F reports 8; reject this abnormal layout during library validation.

**Use cases:** Verify that ZPos is actually configured as 16-bit and select the correct read function. It is not a signal-quality indicator.

## 9. GetSuperDmxAttributeValue

```cpp
void GetSuperDmxAttributeValue(const FSuperDMXAttribute& DmxAttribute,
    float& InOutDefault) const;
```

Blueprint Pure, Read category. The selector contains InstanceIndex, AttribName, and DMXChannelType. Coarse returns Raw8/255; Fine returns Raw16/65535 (requires Fine); Ultra returns Raw24/16777215 (may zero-fill). The current implementation falls back to 8-bit for unrecognized enum values; projects should not produce invalid enum values.

**C++ failure semantics:** Property mode, resolution failure, no snapshot, or Fine mode without Fine leaves the reference unchanged. Existing machinery passes cached variables such as PosX, so a failed read preserves their existing values. For example, after `float Z=0.25f; GetSuperDmxAttributeValue(Query,Z);`, Z remains 0.25 if the read fails. Out-of-bounds bytes in an existing snapshot are still read as 0, so configuration validation cannot be omitted.

It only normalizes. It does not apply the DefaultValue percentage, look up SubAttributes, map PhysicalRange to centimeters or degrees, or interpolate. The project then performs Clamp, Lerp, and application.

**Blueprint pins:** The non-const reference has no UPARAM(ref). The parameter name InOutDefault is not evidence that a “default-value input pin must exist.” AI must query the actual node pins; this is usually an output reference. Blueprint examples needing explicit failure fallback use ByIndex's DefaultValue and update cached variables only after success. Both approaches execute within Super events.

## 10. GetSuperDmxAttributeValueNoConversion

```cpp
void GetSuperDmxAttributeValueNoConversion(const FSuperDMXAttribute& DmxAttribute,
    float& InOutDefault) const;
```

Blueprint Pure, Read category. Bit-depth selection and failure rules are the same as above, but it returns raw integers represented as float: 0..255, 0..65535, or 0..16777215. NoConversion means no normalization; it does not mean a physical quantity.

**Use cases:** Interpreting application commands by raw ranges, or C++ logic that should preserve the previous value. Blueprint must also verify the reference pins; do not invent a failure Boolean output.

## 11. GetSuperDmxAttributeRawValue

```cpp
void GetSuperDmxAttributeRawValue(const FSuperDMXAttribute& DmxAttribute,
    int32& OutRawValue) const;
```

Blueprint Pure. Sets the output to 0 first; failure also yields 0. **Always reads only Coarse and ignores DMXChannelType.** Setting Fine in the selector does not produce a 16-bit value. Suitable for behavior requiring only the coarse byte; use ByIndex when you need to distinguish failure or read multiple bytes.

Do not interpret 0 from this function as “a close command was received.” 0 may be a genuine value or a configuration/input failure.

## 12. GetSuperDMXColorValue

```cpp
void GetSuperDMXColorValue(const FSuperDMXAttribute& DmxRed,
    const FSuperDMXAttribute& DmxGreen, const FSuperDMXAttribute& DmxBlue,
    FLinearColor& OutColor) const;
```

Blueprint Pure. Performs a normalized attribute read for each of the three components, then sets A=1. Each selector can have its own module and bit depth; pair them explicitly. In C++, a failed component preserves that component's existing value in the supplied color; initialize OutColor first. In Property mode, RGB is unchanged, but A is still set to 1.

It does not perform gamma conversion, emission-intensity processing, or color calibration. The existing material example writes the resulting Color into a dynamic material Vector parameter. Blueprint can construct LinearColor from three ByIndex readings when per-channel fallback control is needed.

## 13. GetChannelValue

```cpp
float GetChannelValue(int32 Address = 1, float DefaultValue = 1) const;
```

Blueprint Pure, Read8 category. Address is a 1-based position relative to StartAddress; absolute address=StartAddress+Address-1. It **does not use the library's Patch or attribute definitions**. Returns 0..255 as a float. No data, Property mode, or an invalid absolute address returns DefaultValue, whose default is 1, not 0.

**Use cases:** Checking a fixed sender channel or performing temporary diagnostics inside a Super event. Library-based queries are better suited to named control. The caller must ensure Address≥1; the current implementation only validates the final index, so 0/negative values may read earlier channels for some StartAddress values and are not valid usage.

GetChannelValue itself does not require a library, but does not create the normal Changed detection range. Use SuperDMXTick for continuous diagnostics; do not add Event Tick.

## 14. GetMatrixAttributeRaw

```cpp
void GetMatrixAttributeRaw(FName AttribName, TArray<float>& OutValues,
    float DefaultValue = 0, bool bSortByPatch = true) const;
```

Blueprint Pure, Matrix category. Iterates all modules, reads Coarse for the same named attribute, and divides by 255. **Returns 0..1 even though its name contains Raw.** Modules missing the attribute or Coarse are skipped.

The output array is cleared on every call. Property mode or an FName deemed invalid by the implementation returns an empty array. Projects prohibit NAME_None and do not rely on special empty-name lookup behavior. Without a snapshot, each module with a qualifying definition outputs DefaultValue, which is also clamped to 0..1, so -1 is not preserved as a sentinel.

By default, results are sorted by Patch ascending; bSortByPatch=false preserves original module order but still filters. Output array indices do not necessarily equal module indices. Out-of-bounds bytes in an existing snapshot are read as 0. Stable ordering for equal Patch values is not an application guarantee.

**Use cases:** Reading an 8-bit parameter for multiple consistently defined targets at once; the project must establish a clear target order. Do not divide by 255 again.

## 15. GetMatrixAttributeRaw16

```cpp
void GetMatrixAttributeRaw16(FName AttribName, TArray<float>& OutValues,
    float DefaultValue = 0, bool bSortByPatch = true) const;
```

Blueprint Pure. Follows the preceding process, but each module must contain both Coarse and Fine; returns Raw16/65535. Modules without Fine are skipped, not represented by default-value placeholders. Other fallback, sorting, and clearing rules are the same.

**Use cases:** Fine position input for multiple platforms. If modules differ, do not index targets directly with the filtered array; per-module ByIndex reads in Blueprint make target identity easier to preserve.

## 16. GetMatrixAttributeRawWithIndex / GetMatrixAttributeRaw16WithIndex

```cpp
void GetMatrixAttributeRawWithIndex(FName AttribName, TArray<float>& OutValues,
    TArray<int32>& OutModuleIndices, float DefaultValue = 0) const;
void GetMatrixAttributeRaw16WithIndex(FName AttribName, TArray<float>& OutValues,
    TArray<int32>& OutModuleIndices, float DefaultValue = 0) const;
```

C++ only, no UFUNCTION. These are normalized 8/16-bit matrix reads, respectively, always sorted by Patch. Both arrays are cleared on each call and remain parallel: Values[k] belongs to Modules[Indices[k]], and Indices stores original module indices.

Example: if Modules originally contains Patch9 with ZPos, Patch1 with ZPos, and Patch5 without ZPos, the output module indices are [1,0], excluding the third module. **Use cases:** Avoiding identity mismatches after sorting in C++ multi-target control. Only values are returned, without a validity flag; a default of 0 cannot establish whether input is online.

## 17. GetModules

```cpp
const TArray<FSuperDMXModuleInstance>& GetModules() const;
```

Blueprint Pure, Info category. Returns FixtureLibrary's Modules if a library exists, otherwise a static empty array. In C++, it is a read-only reference; do not delete it or modify it through const_cast. Obtain it again after library replacement or array changes.

**Use cases:** AI configuration readback, Blueprint traversal of attributes and segments, and target-module enumeration. It does not create modules or return the “currently active entry” from mutually exclusive modes; all modules belong to the library configuration simultaneously.

## 18. GetFixtureChannelSpan

```cpp
int32 GetFixtureChannelSpan() const;
```

Blueprint Pure, Info category. Collects C/F/U absolute addresses with values >0 across all modules and returns Max-Min+1; returns 0 when there are no channels. Includes gaps between addresses, so it is neither the number of attributes nor always the highest relative channel number.

With only relative channels 5 and 8, Span=4. Do not infer the last channel from StartAddress+Span-1, because there may be a leading gap. **Use cases:** Patching assistance and Changed-range diagnostics; complete boundary validation still requires resolving each entry.

## 19. ForceRefreshDMX

```cpp
void ForceRefreshDMX();
```

Blueprint Callable, Control category. Enters NativeSuperDMXTick once, directly calls Changed in Property mode or performs change detection in DMX mode, then marks component render state dirty. The editor path includes PostEditChange.

Does not obtain a new network packet, guarantee Changed in DMX mode, or actively force a full viewport redraw. Use only for explicit editor refresh/diagnostic actions; **do not call it inside Super events or manually every frame**. The editor may construct again, so project initialization must be repeatable.

## 20. Lifecycle and the Six Event Functions

| Function | Current implementation and project usage |
| --- | --- |
| ASuperDmxActorBase() | Constructor body is currently empty; the parent creates SceneBase and provides scheduling capability. Do not read live signals in the constructor |
| BeginPlay() | Calls NativeLightInitialization after the parent BeginPlay; projects generally organize logic in the Super initialization event |
| Tick(float DeltaTime) | The base class schedules NativeSuperDMXTick, then processes Changed by mode; projects do not override this as the DMX entry point |
| ShouldTickIfViewportsOnly() const | Returns true to allow viewport updates; does not guarantee continued execution under all paused/background conditions |
| OnConstruction(const FTransform&) | protected; after the parent, calls initialization, SuperTick, and Changed in order, all of which can recur |
| LightInitialization() | protected Blueprint-implemented event; initializes components/resources |
| SuperDMXTick() | protected Blueprint-implemented event; continuous reading and application |
| SuperDMXChanged() | protected Blueprint-implemented event; changed parameters and application state |
| NativeLightInitialization() | protected C++ virtual; calls LightInitialization by default |
| NativeSuperDMXTick() | protected C++ virtual; calls SuperDMXTick by default |
| NativeSuperDMXChanged() | protected C++ virtual; calls SuperDMXChanged by default |

All three events have no parameters. Project Native overrides follow the existing examples: call Super first, then enter their own handler. See the [event chapter](02_Super_Events_en.md) for all detailed scheduling conditions and application deduplication.

## 21. GetFixtureModelId and Helper Structure

```cpp
virtual FSuperFixtureModelId GetFixtureModelId() const;
```

C++ only. The base class returns the current UClass and an empty FGuid. Used for product object identity grouping, not DMX reading; these object-control examples do not need to override it.

FSuperFixtureModelId in the header is not a USTRUCT: ActorClass defaults to null and DefinitionGuid to invalid. It has a default constructor and a (UClass*,const FGuid&) constructor. IsValid only checks that the class is non-null; IsDataDriven checks Guid validity; == compares both fields, != negates that result; GetTypeHash combines the class and Guid. ToKeyString returns the class path, appending `#` and a Digits-format Guid if one exists; it uses UnknownClass when there is no class name.

This identity is not a scene instance ID, is not used to map TargetActor, and has no Blueprint Make FSuperFixtureModelId node. The type is explained to cover the public header, not to require projects to extend the product's model identity system.

## 22. Four C++ Macros

### FOREACH_LOOP(ArrayVar, ElementVar, IndexVar, LoopBody)

Stores the array by const reference, iterates with an int32 index starting at 0, exposes each element by const reference, and executes LoopBody. Do not change the array structure during the loop. It is only a C++ macro, has no Blueprint node, and provides no DMX reading.

### GET_SUPER_DMX_MATRIX_VALUE(DMXAttribute, DefaultValue, LoopBody)

Fine selects GetMatrixAttributeRaw16; other values select GetMatrixAttributeRaw, so **Ultra also uses 8-bit**. Assigns each returned value to DefaultValue and provides LightIndex inside LoopBody. Values are already normalized; LightIndex is the index after filtering and sorting. The query's InstanceIndex does not restrict the modules. Here DefaultValue is a variable overwritten repeatedly, not a constant.

### GET_SUPER_DMX_MATRIX_VALUE_WITH_SUBATTR(DMXAttribute, DefaultValue, LoopBody)

Uses WithIndex to recover the original module and provides LightIndex, DmxRawValue, AttrDef, and SubAttr. AttrDef is looked up in the original module; SubAttr may be null. **DmxRawValue is always the normalized value multiplied by 255 and converted to int; Fine is also reduced to 8-bit.** This is unsuitable for preserving 16-bit range precision. Segment lookup does not automatically check ModeMaster. For precise project commands, explicitly read Raw16 and then look up the segment.

### GET_SUPER_DMX_MATRIX_RGB(DmxR,DmxG,DmxB,OutColor,LoopBody)

Independently reads three normalized 8-bit matrix arrays, loops to the greatest of their lengths, fills missing entries with 0, and writes OutColor=(r,g,b,1). Ignores the selectors' higher bit depths and module restrictions. The three arrays are filtered independently, so missing attributes can misalign them; use this only with complete, consistent module attributes and an explicit target order.

The ordinary-object integration examples use explicit loops so AI can verify targets, bit depths, and missing entries. Understand the macro semantics; do not infer from their names that they “automatically handle multi-object mapping.”

## 23. Private Members and Implementation Differences

DetectAndFireDMXChanged, EnsureAddressCache, RebuildAddressCache, FindResolvedAttribute, CollectMatrixAttribute, the cache structures, and PreviousChannelSnapshot are all private. External code must not call them or modify these internal fields through reflection.

The address cache is rebuilt when the FixtureLibrary object, library EditVersion, or StartAddress changes. After directly modifying a library, notify readers using the public BumpEditVersion; ordinary readers need not manage the cache. Universe is not a cache/snapshot identity key, so changing Universe with identical values does not guarantee Changed.

This documentation deliberately distinguishes declaration comments from behavior: the implementation governs the Patch formula; matrix Raw values are normalized; out-of-bounds Raw8 does not always return DefaultValue; RawValue ignores bit depth; Changed includes construction and Property paths; address labels are no longer generated. AI execution must follow these explicit rules and the readback results from the currently installed version.
