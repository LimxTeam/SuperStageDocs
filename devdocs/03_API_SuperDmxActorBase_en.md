# ASuperBaseActor & ASuperDmxActorBase API Documentation

> **Document Version**: 1.0  
> **Applicable Version**: SuperStage Plugin 2025/2026  
> **Target Audience**: Blueprint Developers / C++ Plugin Integration Developers (without source code modification permissions)  
> **Last Updated**: 2026-03

---

## Table of Contents

1. [ASuperBaseActor — Base Asset Actor](#1-asuperbaseactor--base-asset-actor)
2. [FAssetMetaData — Asset Metadata](#2-fassetmetadata--asset-metadata)
3. [FSuperDMXFixture — DMX Address Configuration](#3-fsuperdmxfixture--dmx-address-configuration)
4. [ASuperDmxActorBase — DMX Data Reading Base Class](#4-asuperdmxactorbase--dmx-data-reading-base-class)
   - [Properties](#41-properties)
   - [Blueprint Events](#42-blueprint-events)
   - [Control Functions](#43-control-functions)
   - [Attribute Value Reading (Normalized)](#44-attribute-value-reading-normalized)
   - [Raw Value Reading (by Instance Index)](#45-raw-value-reading-by-instance-index)
   - [Matrix Batch Reading](#46-matrix-batch-reading)
   - [Address Resolution and Info Query](#47-address-resolution-and-info-query)
5. [DMX Address Calculation Details](#5-dmx-address-calculation-details)
6. [Blueprint Macro Utilities](#6-blueprint-macro-utilities)
7. [Usage Examples](#7-usage-examples)
8. [API Quick Reference](#8-api-quick-reference)

---

## 1. ASuperBaseActor — Base Asset Actor

**Header File**: `SuperBaseActor.h`  
**Base Class**: `AActor`  
**Export Macro**: `SUPERSTAGE_API`

The **root base class** for all SuperStage Actors, providing asset metadata management and editor direction preview functionality.

### Properties

| Property | Type | Category | Description |
|------|------|------|------|
| `AssetData` | `FAssetMetaData` | `A.Asset` | Asset metadata (UUID, manufacturer, DMX mode, etc.) |
| `SceneBase` | `USceneComponent*` | `Z.Components` | Root scene component, mounting point for all child components |
| `bPreviewAssetOrientation` | `bool` | `F.Preview` | Direction preview toggle (shows forward/up arrows) |
| `ForwardArrow` | `UArrowComponent*` | — | Forward arrow (green, +X direction), `Transient` |
| `UpArrow` | `UArrowComponent*` | — | Up arrow (blue, +Z direction), `Transient` |

### Component Structure

```
ASuperBaseActor
 └─ SceneBase (USceneComponent, root component)
     ├─ ForwardArrow (UArrowComponent, +X, green)
     └─ UpArrow (UArrowComponent, +Z, blue)
```

> **Note**: `ForwardArrow` and `UpArrow` are marked as `Transient` and not serialized to disk. Only visible when `bPreviewAssetOrientation = true`.

---

## 2. FAssetMetaData — Asset Metadata

**Header File**: `SuperBaseActor.h`

Stores descriptive information for SuperStage assets, used for asset management and identification.

```cpp
USTRUCT(BlueprintType)
struct FAssetMetaData
```

### Properties

| Property | Type | Default | Description |
|------|------|--------|------|
| `UUID` | `FGuid` | `FGuid()` (zero GUID) | Asset unique identifier |
| `Group` | `FString` | `""` | Group/category (e.g., "Beam", "Wash", "Spot") |
| `Manufacturer` | `FString` | `""` | Manufacturer name (e.g., "Acme", "Robe") |
| `DMXMode` | `FString` | `""` | DMX mode name (e.g., "17CH", "Extended") |
| `DisplayName` | `FString` | `""` | Display name |
| `Thumbnail` | `UTexture2D*` | `nullptr` | Thumbnail texture |

> **Design Note**: `UUID` defaults to zero GUID (rather than `NewGuid()`) to avoid unnecessary changes (serialization jitter) on each serialize/deserialize cycle.

---

## 3. FSuperDMXFixture — DMX Address Configuration

**Header File**: `LightActor/SuperLightTypes.h`

Defines the address information of a fixture in the DMX network. Every DMX-controlled Actor holds an instance of this struct.

```cpp
USTRUCT(BlueprintType)
struct FSuperDMXFixture
```

### Properties

| Property | Type | Default | Range | Description |
|------|------|--------|------|------|
| `FixtureID` | `int32` | `1` | ≥1 | Fixture number (for identification and address label display) |
| `Universe` | `int32` | `1` | 1-256 | DMX Universe number |
| `StartAddress` | `int32` | `1` | 1-512 | Starting DMX channel address |

### Usage Notes

- `Universe` corresponds to the **internal Universe number** of `USuperDMXSubsystem` (affected by `StartUniverse` offset)
- `StartAddress` is the first DMX channel occupied by this fixture
- Fixture channel range = `[StartAddress, StartAddress + ChannelSpan - 1]`
- `FixtureID` does not affect DMX communication, only used for identification and display

---

## 4. ASuperDmxActorBase — DMX Data Reading Base Class

**Header File**: `SuperDmxActorBase.h`  
**Base Class**: `ASuperBaseActor`  
**Export Macro**: `SUPERSTAGE_API`

The **core base class** for all DMX-controlled Actors, providing a complete interface for reading DMX data from `USuperDMXSubsystem` and converting it to usable values.

### Inheritance Hierarchy

```
AActor
 └─ ASuperBaseActor           (Metadata / Direction Preview)
     └─ ASuperDmxActorBase    (DMX Read Core) ← This class
         ├─ ASuperLightBase   (Pan/Tilt Motion Control)
         │   ├─ ASuperStageLight     (Full-function Moving Light)
         │   ├─ ASuperStageVFXActor  (Niagara VFX)
         │   └─ ASuperLiftMatrix     (Lift Matrix)
         ├─ ASuperLiftingMachinery   (Lifting Machinery)
         ├─ ASuperRailMachinery      (Rail Machinery)
         ├─ ASuperLightStripEffect   (LED Strip)
         └─ ASuperDMXCamera          (DMX Camera)
```

---

### 4.1 Properties

| Property | Type | Category | Access | Description |
|------|------|------|------|------|
| `SuperDMXFixture` | `FSuperDMXFixture` | `A.SuperDMX` | ReadWrite + Interp | DMX address configuration (Universe + StartAddress + FixtureID) |
| `FixtureLibrary` | `USuperFixtureLibrary*` | `A.SuperDMX` | ReadOnly | Fixture library data asset reference (defines channel layout) |
| `bShowAddressLabel` | `bool` | `F.Preview` | ReadWrite | Whether to display address text label |
| `bIncludeFixtureIdInLabel` | `bool` | `F.Preview` | ReadWrite | Whether label includes FixtureID |
| `AddressLabelOffset` | `FVector` | `F.Preview` | ReadWrite | Label offset (default `(30,0,0)` cm) |

#### SuperDMXFixture

The fixture's DMX address configuration. Supports `Interp` meta tag, allowing animation in Sequencer (dynamic switching of Universe/address).

**Blueprint setup**: Configure Universe and StartAddress in Actor's Details panel → `A.SuperDMX` → `SuperDMXFixture`.

#### FixtureLibrary

References a `USuperFixtureLibrary` data asset that defines all DMX channel mappings for this fixture. **Must be set**, otherwise attribute reading functions will not work.

**Blueprint setup**: Select data asset in Actor's Details panel → `A.SuperDMX` → `FixtureLibrary`.

---

### 4.2 Blueprint Events

#### SuperDMXTick

```cpp
UFUNCTION(BlueprintImplementableEvent, Category="A.Event", meta=(DisplayName="Super DMX Tick"))
void SuperDMXTick(float DeltaSeconds = 0.f);
```

Blueprint-implementable event triggered **every frame**. This is the **primary entry point** for writing DMX control logic.

**Parameters**:

| Parameter | Type | Description |
|------|------|------|
| `DeltaSeconds` | `float` | Frame interval time (seconds). `0.0` when called by `ForceRefreshDMX()` |

**Trigger timing**:
- Normal `Tick()` every frame
- `ForceRefreshDMX()` manual trigger
- `OnConstruction()` during construction (`DeltaSeconds = 0`)
- **Still triggered** when editor viewport is paused (`ShouldTickIfViewportsOnly() = true`)

**Typical usage** (Blueprint):

```
Event SuperDMXTick(DeltaSeconds)
 ├─ SetLightingDefaultValue(...)     // Initialize (optional, first frame only)
 ├─ SetLightingIntensity(Dimmer)     // Intensity
 ├─ SetLightingStrobe(Strobe)        // Strobe
 ├─ SetLightingColorRGB(R, G, B)     // Color
 ├─ YPan(Pan)                        // Pan rotation
 ├─ YTilt(Tilt)                      // Tilt rotation
 ├─ SetLightingZoom(Zoom)            // Zoom
 └─ ...
```

> **Important**: Blueprint execution is safely guarded in the editor via `FEditorScriptExecutionGuard`.

#### LightInitialization

```cpp
UPROPERTY(BlueprintAssignable, Category = "A.Event", meta = (DisplayName = "LightInitialization"))
FLightInitialization LightInitialization;
```

Fixture initialization delegate (`DECLARE_DYNAMIC_MULTICAST_DELEGATE`), can bind callbacks in Blueprint.

---

### 4.3 Control Functions

#### ForceRefreshDMX

```cpp
UFUNCTION(BlueprintCallable, Category = "A.Yunsio|SuperDMX|Control")
void ForceRefreshDMX();
```

Force refresh DMX state.

**Behavior**:
1. Call `SuperDMXTick(0.0f)` — trigger blueprint control logic
2. Call `MarkComponentsRenderStateDirty()` — mark render state needs update
3. Call `PostEditChange()` — trigger property change notifications such as material parameters

**Use cases**:
- Real-time preview of DMX control effects in the editor
- `SuperConsoleSubsystem` batch refresh of fixtures
- Manual single DMX update trigger (without waiting for next frame)

> **Performance note**: This function does **not actively redraw the viewport**. `SuperConsoleSubsystem` performs unified batch redraws to avoid N fixtures × N redraws.

---

### 4.4 Attribute Value Reading (Normalized)

This set of functions uses the `FSuperDMXAttribute` struct to query attributes and **automatically normalizes** to the `[0, 1]` range based on channel precision.

#### FSuperDMXAttribute Lookup Key

```cpp
USTRUCT(BlueprintType)
struct FSuperDMXAttribute
{
    int32 InstanceIndex;         // Module instance index (0 = main module)
    FName AttribName;            // Attribute name (must match fixture library definition)
    EDMXChannelType DMXChannelType; // Channel precision
};
```

| EDMXChannelType | Precision | Value Range | Normalization Divisor |
|-----------------|------|--------|-----------|
| `Conarse` | 8-bit | 0-255 | ÷255 |
| `Fine` | 16-bit | 0-65535 | ÷65535 |
| `Ultra` | 24-bit | 0-16777215 | ÷16777215 |

---

#### GetSuperDmxAttributeValue

```cpp
UFUNCTION(BlueprintPure, Category = "A.Yunsio|SuperDMX|Read",
    meta = (DisplayName = "GetSuperDmxAttributeValue"))
void GetSuperDmxAttributeValue(
    const FSuperDMXAttribute& DmxAttribute,
    float& InOutDefault) const;
```

Read DMX attribute value and **normalize** to `[0, 1]`.

**Parameters**:

| Parameter | Type | Direction | Description |
|------|------|------|------|
| `DmxAttribute` | `FSuperDMXAttribute` | Input | Attribute lookup key |
| `InOutDefault` | `float&` | Input/Output | Input as default value; output as normalized value |

**Normalization rules**:

| DMXChannelType | Formula |
|----------------|------|
| `Conarse` | `Coarse / 255.0` |
| `Fine` | `(Coarse × 256 + Fine) / 65535.0` |
| `Ultra` | `(Coarse × 65536 + Fine × 256 + Ultra) / 16777215.0` |

**Behavior**:
- If reading fails (attribute not found, Universe has no data), `InOutDefault` **remains unchanged**
- Internally uses Universe snapshot (`SnapshotUniverse`), single batch read of entire frame data
- Includes permission check (`SUPER_REQUIRE_PERMISSION_VOID`)

**Example** (Blueprint pseudocode):

```
DmxAttr.InstanceIndex = 0
DmxAttr.AttribName = "Dimmer"
DmxAttr.DMXChannelType = Conarse

DefaultValue = 0.0
GetSuperDmxAttributeValue(DmxAttr, DefaultValue)
// DefaultValue is now 0.0 ~ 1.0 brightness value
```

---

#### GetSuperDmxAttributeValueNoConversion

```cpp
UFUNCTION(BlueprintPure, Category = "A.Yunsio|SuperDMX|Read",
    meta = (DisplayName = "GetSuperDmxAttributeValueNoConversion"))
void GetSuperDmxAttributeValueNoConversion(
    const FSuperDMXAttribute& DmxAttribute,
    float& InOutDefault) const;
```

Read the **raw integer value** of a DMX attribute (without normalization).

**Return value range**:

| DMXChannelType | Range |
|----------------|------|
| `Conarse` | 0 - 255 |
| `Fine` | 0 - 65535 |
| `Ultra` | 0 - 16777215 |

**Use cases**:
- Scenarios requiring raw DMX values (e.g., sub-attribute lookup, ChannelSet matching)
- Custom normalization logic

---

#### GetSuperDMXColorValue

```cpp
UFUNCTION(BlueprintPure, Category = "A.Yunsio|SuperDMX|Read",
    meta = (DisplayName = "GetSuperDMXColorValue"))
void GetSuperDMXColorValue(
    const FSuperDMXAttribute& DmxRed,
    const FSuperDMXAttribute& DmxGreen,
    const FSuperDMXAttribute& DmxBlue,
    FLinearColor& OutColor) const;
```

Read three RGB DMX attributes and combine into `FLinearColor`.

**Parameters**:

| Parameter | Type | Direction | Description |
|------|------|------|------|
| `DmxRed` | `FSuperDMXAttribute` | Input | Red channel attribute |
| `DmxGreen` | `FSuperDMXAttribute` | Input | Green channel attribute |
| `DmxBlue` | `FSuperDMXAttribute` | Input | Blue channel attribute |
| `OutColor` | `FLinearColor&` | Output | Combined color (Alpha fixed to 1.0) |

**Behavior**:
- Calls `GetSuperDmxAttributeValue()` separately for R/G/B
- Fills results into `OutColor.R/G/B`, `OutColor.A = 1.0`
- Each channel read independently, can use different `DMXChannelType`

---

#### GetSuperDmxAttributeRawValue

```cpp
UFUNCTION(BlueprintPure, Category = "A.Yunsio|SuperDMX|Read",
    meta = (DisplayName = "GetSuperDmxAttributeRawValue"))
void GetSuperDmxAttributeRawValue(
    const FSuperDMXAttribute& DmxAttribute,
    int32& OutRawValue) const;
```

Get the **raw 8-bit value** of the attribute (only reads Coarse byte).

**Parameters**:

| Parameter | Type | Direction | Description |
|------|------|------|------|
| `DmxAttribute` | `FSuperDMXAttribute` | Input | Attribute lookup key (`DMXChannelType` is ignored) |
| `OutRawValue` | `int32&` | Output | Raw value 0-255, 0 when read fails |

> **Note**: This function always reads only the Coarse byte (8-bit), unaffected by `DMXChannelType`. For 16/24-bit raw values, use `GetSuperDmxAttributeValueNoConversion()`.

---

### 4.5 Raw Value Reading (by Instance Index)

This set of functions locates attributes directly using `InstanceIndex` + `AttribName`, returning **raw integer values**.

#### GetAttributeRaw8ByIndex

```cpp
UFUNCTION(BlueprintPure, Category="A.Yunsio|SuperDMX|Read8")
int32 GetAttributeRaw8ByIndex(
    int32 InstanceIndex,
    FName AttribName,
    int32 DefaultValue = 0) const;
```

Read **8-bit** raw DMX value (Coarse byte only).

**Parameters**:

| Parameter | Type | Default | Description |
|------|------|--------|------|
| `InstanceIndex` | `int32` | — | Module instance index (0 = main module) |
| `AttribName` | `FName` | — | Attribute name (e.g., `"Dimmer"`, `"Pan"`, `"Tilt"`) |
| `DefaultValue` | `int32` | `0` | Return value when read fails |

**Return value**: `int32`, range `[0, 255]`

---

#### GetAttributeRaw16ByIndex

```cpp
UFUNCTION(BlueprintPure, Category="A.Yunsio|SuperDMX|Read16")
int32 GetAttributeRaw16ByIndex(
    int32 InstanceIndex,
    FName AttribName,
    int32 DefaultValue = 0) const;
```

Read **16-bit** raw DMX value (Coarse as high 8 bits, Fine as low 8 bits).

**Return value**: `int32`, range `[0, 65535]`

**Calculation formula**: `(Coarse << 8) | Fine`

> **Note**: Requires both Coarse and Fine channel offsets defined in the fixture library for this attribute, otherwise returns `DefaultValue`.

---

#### GetAttributeRaw24ByIndex

```cpp
UFUNCTION(BlueprintPure, Category="A.Yunsio|SuperDMX|Read24")
int32 GetAttributeRaw24ByIndex(
    int32 InstanceIndex,
    FName AttribName,
    int32 DefaultValue = 0) const;
```

Read **24-bit** raw DMX value.

**Return value**: `int32`, range `[0, 16777215]`

**Calculation formula**: `(Coarse << 16) | (Fine << 8) | Ultra`

**Fallback rules**:
- Only Coarse needs to exist for it to work
- Missing Fine → Fine treated as 0
- Missing Ultra → Ultra treated as 0

---

#### GetAttributeBitDepthByIndex

```cpp
UFUNCTION(BlueprintPure, Category="A.Yunsio|SuperDMX|Info")
int32 GetAttributeBitDepthByIndex(
    int32 InstanceIndex,
    FName AttribName) const;
```

Query the effective bit depth of an attribute.

**Return value**:

| Value | Description |
|----|------|
| `0` | Attribute not found |
| `8` | Only Coarse |
| `16` | Coarse + Fine |
| `24` | Coarse + Fine + Ultra |

---

### 4.6 Matrix Batch Reading

Matrix reading functions iterate through **all module instances** in the fixture library, reading same-named attribute values, returning sorted arrays. Suitable for LED matrix lights, pixel bars, and other multi-module devices.

#### GetMatrixAttributeRaw

```cpp
UFUNCTION(BlueprintPure, Category="A.Yunsio|SuperDMX|Matrix")
void GetMatrixAttributeRaw(
    FName AttribName,
    TArray<float>& OutValues,
    float DefaultValue = 0,
    bool bSortByPatch = true) const;
```

Read same-named attribute from all module instances with **8-bit** precision.

**Parameters**:

| Parameter | Type | Default | Description |
|------|------|--------|------|
| `AttribName` | `FName` | — | Attribute name |
| `OutValues` | `TArray<float>&` | — | Output array, each element is **[0, 1]** normalized value |
| `DefaultValue` | `float` | `0` | Default value when no data |
| `bSortByPatch` | `bool` | `true` | Whether to sort by Patch offset |

**Behavior**:
1. Iterate through all module instances in `FixtureLibrary->Modules`
2. Find modules containing `AttribName`
3. Read Coarse byte, normalize to `[0, 1]` (`coarse / 255.0`)
4. Sort by `Patch` ascending (if `bSortByPatch = true`)

**Return value notes**:
- `OutValues[0]` corresponds to the module with smallest Patch (usually pixel 1)
- Array length = number of modules containing this attribute
- Modules without this attribute are skipped

---

#### GetMatrixAttributeRaw16

```cpp
UFUNCTION(BlueprintPure, Category="A.Yunsio|SuperDMX|Matrix")
void GetMatrixAttributeRaw16(
    FName AttribName,
    TArray<float>& OutValues,
    float DefaultValue = 0,
    bool bSortByPatch = true) const;
```

Read same-named attribute from all module instances with **16-bit** precision.

**Normalization formula**: `((Coarse << 8) | Fine) / 65535.0`

> **Note**: Each module must have both Coarse and Fine defined for this attribute; modules missing Fine will be skipped.

---

#### GetMatrixAttributeRawWithIndex / GetMatrixAttributeRaw16WithIndex

```cpp
void GetMatrixAttributeRawWithIndex(
    FName AttribName,
    TArray<float>& OutValues,
    TArray<int32>& OutModuleIndices,
    float DefaultValue = 0) const;

void GetMatrixAttributeRaw16WithIndex(
    FName AttribName,
    TArray<float>& OutValues,
    TArray<int32>& OutModuleIndices,
    float DefaultValue = 0) const;
```

Same as the above matrix functions, but additionally outputs **module index arrays**.

**Additional output**:

| Parameter | Type | Description |
|------|------|------|
| `OutModuleIndices` | `TArray<int32>&` | Original module indices corresponding to each element in `OutValues` |

**Use cases**:
- Need to further query sub-attributes via module index (`FindAttributeDef`)
- Used internally by `GET_SUPER_DMX_MATRIX_VALUE_WITH_SUBATTR` macro

> **Note**: These two functions are **not BlueprintCallable** (no UFUNCTION macro), C++ only.

---

### 4.7 Address Resolution and Info Query

#### GetChannelValue

```cpp
UFUNCTION(BlueprintPure, Category="A.Yunsio|SuperDMX|Read8")
float GetChannelValue(int32 Address = 1, float DefaultValue = 1) const;
```

Read a single DMX byte value by **relative channel address**.

**Parameters**:

| Parameter | Type | Default | Description |
|------|------|--------|------|
| `Address` | `int32` | `1` | Channel offset relative to `StartAddress` (**1-based**) |
| `DefaultValue` | `float` | `1` | Return value when read fails |

**Return value**: `float`, range `[0, 255]` (**Note: not normalized**)

**Absolute address calculation**: `StartAddress + Address - 1`

**Example**:

```
// StartAddress = 100
GetChannelValue(1)  → Read channel 100 (Coarse value 0-255)
GetChannelValue(2)  → Read channel 101
GetChannelValue(5)  → Read channel 104
```

---

#### ResolveAttributeAddressesByIndex

```cpp
UFUNCTION(BlueprintPure, Category="A.Yunsio|SuperDMX|Address")
bool ResolveAttributeAddressesByIndex(
    int32 InstanceIndex,
    FName AttribName,
    int32& OutCoarseAbs,
    int32& OutFineAbs,
    int32& OutUltraAbs) const;
```

Resolve the **absolute channel addresses** of an attribute in the Universe.

**Parameters**:

| Parameter | Type | Direction | Description |
|------|------|------|------|
| `InstanceIndex` | `int32` | Input | Module instance index |
| `AttribName` | `FName` | Input | Attribute name |
| `OutCoarseAbs` | `int32&` | Output | Coarse absolute address (1-based), 0 if invalid |
| `OutFineAbs` | `int32&` | Output | Fine absolute address, 0 if not exists |
| `OutUltraAbs` | `int32&` | Output | Ultra absolute address, 0 if not exists |

**Return value**: `true` means attribute found and at least Coarse address exists

**Address calculation formula**:

```
Base = StartAddress + Max(0, Patch - 1)
CoarseAbs = Base + Coarse - 1    (if Coarse > 0)
FineAbs   = Base + Fine - 1      (if Fine > 0)
UltraAbs  = Base + Ultra - 1     (if Ultra > 0)
```

---

#### FindAttributeDef

```cpp
// Find by instance index
const FSuperDMXAttributeDef* FindAttributeDef(int32 InstanceIndex, FName AttribName) const;

// Find by FSuperDMXAttribute
const FSuperDMXAttributeDef* FindAttributeDef(const FSuperDMXAttribute& DmxAttribute) const;
```

Find attribute definition, returns pointer to `FSuperDMXAttributeDef`.

**Return value**: Pointer when found, `nullptr` when not found

**Use cases**:
- Access sub-attribute system (`SubAttributes`)
- Get ChannelSet information
- Custom attribute resolution logic

> **Note**: These two overloads are **not BlueprintCallable**, C++ only.

---

#### GetModules

```cpp
UFUNCTION(BlueprintPure, Category="A.Yunsio|SuperDMX|Info",
    meta=(DisplayName="GetModules"))
const TArray<FSuperDMXModuleInstance>& GetModules() const;
```

Get the module instance array in the fixture library.

**Return value**: Reference to `FixtureLibrary->Modules`; returns static empty array when `FixtureLibrary` is null

---

#### GetFixtureChannelSpan

```cpp
UFUNCTION(BlueprintPure, Category="A.Yunsio|SuperDMX|Info",
    meta=(DisplayName="GetFixtureChannelSpan"))
int32 GetFixtureChannelSpan() const;
```

Calculate the total channel span occupied by the fixture.

**Return value**: `Max absolute address - Min absolute address + 1`; returns `0` when no valid channels

**Calculation method**:
- Iterate through all attribute definitions of all module instances
- Collect all >0 Coarse/Fine/Ultra absolute addresses
- Span = Max - Min + 1

---

## 5. DMX Address Calculation Details

### Basic Concepts

```
Universe  ──────────────────────────────── 512 channels
  │
  └─ StartAddress = 100 ─────── Fixture start position
       │
       ├─ Module[0] (Main), Patch = 0
       │   ├─ Dimmer:  Coarse=1  → Absolute address 100
       │   ├─ Pan:     Coarse=3, Fine=4  → Absolute addresses 102, 103
       │   ├─ Tilt:    Coarse=5, Fine=6  → Absolute addresses 104, 105
       │   └─ ...
       │
       ├─ Module[1] (Pixel1), Patch = 17
       │   ├─ Dimmer:  Coarse=1  → Absolute address 116
       │   ├─ Red:     Coarse=2  → Absolute address 117
       │   └─ ...
       │
       └─ Module[2] (Pixel2), Patch = 20
           ├─ Dimmer:  Coarse=1  → Absolute address 119
           └─ ...
```

### Address Formula

```
Base = StartAddress + Max(0, Patch - 1)
AbsoluteAddress = Base + RelativeOffset - 1
```

Where `RelativeOffset` is the `Coarse`/`Fine`/`Ultra` value in `FSuperDMXAttributeDef`.

### Example Calculation

| Parameter | Value | Description |
|------|-----|------|
| `StartAddress` | 100 | Fixture start |
| `Patch` | 17 | Pixel1 module offset |
| `Coarse` | 2 | Red attribute Coarse offset |
| **Base** | 100 + 16 = 116 | |
| **Absolute Address** | 116 + 2 - 1 = **117** | Red channel address in Universe |

---

## 6. Blueprint Macro Utilities

The following C++ macros simplify batch reading of matrix attributes, for C++ plugin developers (Blueprint users should use the corresponding BlueprintCallable functions).

### GET_SUPER_DMX_MATRIX_VALUE

```cpp
GET_SUPER_DMX_MATRIX_VALUE(DMXAttribute, DefaultValue, LoopBody)
```

Iterate through matrix attribute values, automatically selecting 8-bit or 16-bit reading based on `DMXChannelType`.

**Loop variables**:
- `LightIndex` (`int32`): Current pixel index
- `DefaultValue`: Current pixel's normalized value (updated in-place)

### GET_SUPER_DMX_MATRIX_VALUE_WITH_SUBATTR

```cpp
GET_SUPER_DMX_MATRIX_VALUE_WITH_SUBATTR(DMXAttribute, DefaultValue, LoopBody)
```

Enhanced version, additionally provides sub-attribute information.

**Loop variables**:
- `LightIndex` (`int32`): Current pixel index
- `DefaultValue`: Current pixel's normalized value
- `DmxRawValue` (`int32`): Raw DMX value (0-255)
- `AttrDef` (`const FSuperDMXAttributeDef*`): Attribute definition pointer
- `SubAttr` (`const FSubAttribute*`): Sub-attribute corresponding to current value (may be `nullptr`)

### GET_SUPER_DMX_MATRIX_RGB

```cpp
GET_SUPER_DMX_MATRIX_RGB(DmxR, DmxG, DmxB, OutColor, LoopBody)
```

Iterate through matrix RGB colors.

**Loop variables**:
- `LightIndex` (`int32`): Current pixel index
- `OutColor` (`FLinearColor`): Current pixel's color

---

## 7. Usage Examples

### 7.1 C++ Plugin: Read Fixture Dimmer

```cpp
#include "SuperDmxActorBase.h"

void AMyController::ReadFixtureDimmer(ASuperDmxActorBase* Light)
{
    if (!Light) return;

    // Method 1: Normalized reading via FSuperDMXAttribute
    FSuperDMXAttribute DimmerAttr;
    DimmerAttr.InstanceIndex = 0;
    DimmerAttr.AttribName = FName("Dimmer");
    DimmerAttr.DMXChannelType = EDMXChannelType::Conarse;

    float Dimmer = 0.0f;
    Light->GetSuperDmxAttributeValue(DimmerAttr, Dimmer);
    // Dimmer = 0.0 ~ 1.0

    // Method 2: Raw value reading by instance index
    int32 RawDimmer = Light->GetAttributeRaw8ByIndex(0, FName("Dimmer"));
    // RawDimmer = 0 ~ 255

    // Method 3: Reading by relative channel
    float ChannelValue = Light->GetChannelValue(1);
    // ChannelValue = 0 ~ 255 (channel StartAddress + 0)
}
```

### 7.2 C++ Plugin: Read Matrix Pixel Colors

```cpp
void AMyController::ReadMatrixPixels(ASuperDmxActorBase* Light)
{
    if (!Light) return;

    TArray<float> RedValues, GreenValues, BlueValues;
    Light->GetMatrixAttributeRaw(FName("Red"), RedValues);
    Light->GetMatrixAttributeRaw(FName("Green"), GreenValues);
    Light->GetMatrixAttributeRaw(FName("Blue"), BlueValues);

    int32 PixelCount = FMath::Min3(RedValues.Num(), GreenValues.Num(), BlueValues.Num());
    for (int32 i = 0; i < PixelCount; ++i)
    {
        FLinearColor PixelColor(RedValues[i], GreenValues[i], BlueValues[i], 1.0f);
        // Use PixelColor...
    }
}
```

### 7.3 C++ Plugin: Inspect Fixture Channel Info

```cpp
void AMyController::InspectFixture(ASuperDmxActorBase* Light)
{
    if (!Light) return;

    // Get channel span
    int32 Span = Light->GetFixtureChannelSpan();
    UE_LOG(LogTemp, Log, TEXT("Fixture uses %d channels"), Span);

    // Get module list
    const TArray<FSuperDMXModuleInstance>& Modules = Light->GetModules();
    UE_LOG(LogTemp, Log, TEXT("Fixture has %d modules"), Modules.Num());

    // Query attribute bit depth
    int32 PanBitDepth = Light->GetAttributeBitDepthByIndex(0, FName("Pan"));
    UE_LOG(LogTemp, Log, TEXT("Pan bit depth: %d"), PanBitDepth); // 8 or 16

    // Resolve absolute addresses
    int32 CoarseAddr, FineAddr, UltraAddr;
    if (Light->ResolveAttributeAddressesByIndex(0, FName("Pan"), CoarseAddr, FineAddr, UltraAddr))
    {
        UE_LOG(LogTemp, Log, TEXT("Pan Coarse=Ch%d, Fine=Ch%d"), CoarseAddr, FineAddr);
    }
}
```

---

## 8. API Quick Reference

### ASuperBaseActor

| Property | Type | Description |
|------|------|------|
| `AssetData` | `FAssetMetaData` | Asset metadata |
| `SceneBase` | `USceneComponent*` | Root component |
| `bPreviewAssetOrientation` | `bool` | Direction preview toggle |

### ASuperDmxActorBase — Properties

| Property | Type | Description |
|------|------|------|
| `SuperDMXFixture` | `FSuperDMXFixture` | DMX address (Universe + StartAddress) |
| `FixtureLibrary` | `USuperFixtureLibrary*` | Fixture library reference |
| `bShowAddressLabel` | `bool` | Show address label |

### ASuperDmxActorBase — Normalized Reading

| Function | Returns | Description |
|------|------|------|
| `GetSuperDmxAttributeValue(Attr, &Val)` | `void` | Normalized to [0,1] |
| `GetSuperDmxAttributeValueNoConversion(Attr, &Val)` | `void` | Raw integer value |
| `GetSuperDMXColorValue(R, G, B, &Color)` | `void` | RGB → FLinearColor |
| `GetSuperDmxAttributeRawValue(Attr, &Raw)` | `void` | Coarse only (0-255) |

### ASuperDmxActorBase — Raw Reading by Index

| Function | Return Range | Description |
|------|----------|------|
| `GetAttributeRaw8ByIndex(Idx, Name, Def)` | 0-255 | 8-bit |
| `GetAttributeRaw16ByIndex(Idx, Name, Def)` | 0-65535 | 16-bit |
| `GetAttributeRaw24ByIndex(Idx, Name, Def)` | 0-16777215 | 24-bit |
| `GetAttributeBitDepthByIndex(Idx, Name)` | 0/8/16/24 | Bit depth query |

### ASuperDmxActorBase — Matrix Reading

| Function | Precision | Output |
|------|------|------|
| `GetMatrixAttributeRaw(Name, &Vals, Def, Sort)` | 8-bit | `TArray<float>` [0,1] |
| `GetMatrixAttributeRaw16(Name, &Vals, Def, Sort)` | 16-bit | `TArray<float>` [0,1] |

### ASuperDmxActorBase — Utilities

| Function | Returns | Description |
|------|------|------|
| `ForceRefreshDMX()` | `void` | Force refresh DMX |
| `GetChannelValue(Addr, Def)` | `float` 0-255 | Relative channel read |
| `ResolveAttributeAddressesByIndex(...)` | `bool` | Address resolution |
| `GetModules()` | `TArray<FSuperDMXModuleInstance>&` | Module list |
| `GetFixtureChannelSpan()` | `int32` | Channel span |
