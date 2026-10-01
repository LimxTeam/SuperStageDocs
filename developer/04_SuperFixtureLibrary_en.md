# SuperFixtureLibrary: Complete Fields, Native JSON, and Asset Creation

[Back to Contents](README_en.md) · [Ready-to-Use JSON Files](examples/README_en.md)

## 1. A Channel Library Is Protocol Data

`USuperFixtureLibrary` (`AssetTool/SuperFixtureLibrary.h`) is a UPrimaryDataAsset. It defines which modules a control object has, which attributes each module has, and the relative positions of attribute bytes. It does not automatically move Actors, call application functions, apply PhysicalRange, or create display objects.

Minimal structure:

```text
USuperFixtureLibrary
  Modules[0] : FSuperDMXModuleInstance
    ModuleName
    Patch
    AttributeDefs[] : FSuperDMXAttributeDef
      AttribName / Coarse / Fine / Ultra
      SubAttributes[] : FSubAttribute
        DmxMin / DmxMax / PhysicalRange
        ChannelSets[] : FChannelSet
```

All elements in Modules participate in reads and span calculations simultaneously; this is **not a list of mutually exclusive modes**. Although the creation factory's default entry is named Mode 1, it is simply one module in the array. Store different mutually exclusive protocols in separate libraries and select the appropriate asset.

## 2. Use a 12-Channel Definition Matching the Actual Machinery Code

The existing machinery reads the following names, all using Fine. The supplied [Motion6Axis_12CH.json](examples/libraries/Motion6Axis_12CH.json) follows the actual names and byte order. It is a project protocol file, not a purported original file exported from the binary uasset.

StartAddress=101, Modules[0].Patch=1:

| AttribName | Coarse | Fine | Ultra | Absolute addresses | Project use |
| --- | --- | --- | --- | --- | --- |
| XPos | 1 | 2 | 0 | 101/102 | Normalized X position |
| YPos | 3 | 4 | 0 | 103/104 | Normalized Y position |
| ZPos | 5 | 6 | 0 | 105/106 | Normalized Z position |
| XRot | 7 | 8 | 0 | 107/108 | Normalized rotation control |
| YRot | 9 | 10 | 0 | 109/110 | Normalized rotation control |
| ZRot | 11 | 12 | 0 | 111/112 | Normalized rotation control |

The project decides how many centimeters/degrees normalized values map to. Do not automatically equate names such as XRot with a particular UE rotation pin; the existing machinery uses RotX for Pitch, RotY for Yaw, and RotZ for Roll. When adapting control to your own object, specify the actual axis mapping explicitly.

For a single module, Patch=1 and attribute offsets start at 1. If a second module of the same structure immediately follows the first, its Patch=13 while its attributes still start at 1; do not also add 12 to the attributes.

## 3. Create an Asset and Import JSON

1. Create a **Super Fixture Library** asset in your own Content directory, for example `/Game/ProjectDMX/`; the native factory belongs to the SuperStage asset category. You can also use the editor's supported data-asset creation capability to specify `/Script/SuperCore.SuperFixtureLibrary`. Do not create a same-named Blueprint class in place of a data asset.
2. Name it `DA_Motion6Axis_12CH` and open the library editor. The factory supplies an initial FixtureName, Manufacturer=Unknown, and a default module with Patch=1.
3. Use the library editor's JSON import operation and select the supplied JSON file. If modules already exist, the UI prompts that the import will overwrite them; perform the import on the new asset created for this example. For an existing user library, first confirm that it is the asset intended for replacement.
4. Import clears and rebuilds Modules; it does not append. A success message only proves that the parser accepted the modules, not that every address/name/bit depth is valid. Save the asset, then read back all modules, offsets, and segments.
5. Assign `FixtureLibrary=DA_Motion6Axis_12CH` on your DMX control object, configure the internal Universe, StartAddress, and ControlMode=DMX, and save the Blueprint defaults or scene instance and level.
6. Check every address in the table using ResolveAttributeAddressesByIndex; GetFixtureChannelSpan should be 12. The highest valid StartAddress is 501; 502 puts the last channel beyond 512.
7. Send 105/106=128/0. ZPos Raw16 should be 32768, normalized to approximately 0.5000076. Then let Super DMX Tick apply it to your target.

UE5MCP can perform the same actions using its actually supported asset-editing capabilities. Discover the tools first. If JSON import UI operation is unavailable, use reflection to edit the library's nested structure arrays and save. Do not invent a fixed “import_superfixturelibrary” tool. Project C++ must not include or call the non-exposed SuperTools importer.

## 4. Native JSON Syntax and Import Boundaries

Field-name case follows the existing parser. Do not put comments in JSON. A complete basic example:

```json
{
  "FixtureName": "Project Z Position 2CH",
  "Manufacturer": "Project",
  "PowerConsumption": 0,
  "Weight": 0,
  "BeamAngle": 0,
  "BeamIntensity": 0,
  "BodySize": [0, 0, 0],
  "MA2FixtureLibraryPath": "",
  "GDTFFixtureLibraryPath": "",
  "Modules": [
    {
      "ModuleName": "Main",
      "Patch": 1,
      "AttributeDefs": [
        {
          "AttribName": "ZPos",
          "Category": "Position",
          "Coarse": 1,
          "Fine": 2,
          "Ultra": 0,
          "SubAttributes": [
            {"Name": "Travel", "DmxMin": 0, "DmxMax": 65535,
             "PhysicalRange": [0, 300]}
          ]
        }
      ]
    }
  ]
}
```

This example contains only two ZPos channels, unlike ZPos offsets 5/6 in the 12-channel protocol; do not mix their address tables.

The parser reads AttribName, Category, and Coarse directly for each attribute, so generated files must supply all three. Explicit ModuleName, Patch, and AttributeDefs are recommended for modules. The root Modules array must have at least one element for success; empty attribute arrays may still pass, so AI must perform additional acceptance checks.

Optional attribute keys include Fine, Ultra, DmxBreak, **VirtualChannel**, Geometry, NativeResolutionBytes, DefaultValue, HighlightValue, and the four raw default/highlight precision fields. The C++ field is bVirtualChannel; the JSON key is not bVirtualChannel.

Sub-attributes must supply DmxMin/DmxMax; optional fields are Name, NativeResolutionBytes, NativeDmxMin/Max, WheelName, StrobeMode, GoboMode, RotationMode, PhysicalRange, and ChannelSets. A ChannelSet supplies Name, DmxMin, and DmxMax, and may include the corresponding mode/precision fields, PhysicalRange, Color, ColorIndex, PrismSelection, and WheelSlotIndex.

**Do not treat the entire header as an automatic JSON serialization schema.** The current native parser does not fully cover header fields such as GDTFAttribute, bPhysicalExplicit, DeclaredColor/bHasDeclaredColor, ModeMaster fields, Texture object references, or GeometryReferences. Writing them into JSON does not prove they were imported. When needed, edit them through the asset and read them back, or confirm support in the delivered version. The JSON here uses only verified fields.

The native importer clears the default sub-attribute created by the attribute constructor before adding the JSON array. If the array is empty or missing, it adds a default 0..255 segment again. **Omitting segments for a Fine attribute still produces a 0..255 segment; it does not automatically expand to 0..65535.** Specify 16-bit segments explicitly when needed.

## 5. Top-Level Library Fields

| Field | Meaning / project rule |
| --- | --- |
| FixtureName | Protocol display name, not an object address |
| Manufacturer | Actual project/team identifier |
| Modules | All logical modules active simultaneously |
| MA2FixtureLibraryPath | Deprecated compatibility field, read-only in the asset panel; leave empty for new project protocols |
| GDTFFixtureLibraryPath | Path to an actual bound external file; custom object protocols do not require GDTF and may leave it empty |
| PowerConsumption | Wattage metadata, default 540; set to 0 here when unrelated to the custom object |
| Weight | Kilogram metadata, default 21.2; set to 0 when unused |
| BeamAngle, BeamIntensity | Degree/lumen metadata; does not drive your model; 0 in this example |
| BodySize | Dimensions in meters, default (0.6,0.6,0.6); does not automatically scale the target Actor |
| GeometryReferences | Source geometry-reference metadata; not required for ordinary object control and not a TargetActor array |

GeometryReferences contains Name, TemplateGeometry, DMXBreak, DMXOffset, PositionX, and ParentGeometry: reference name, template name, Break, 1-based offset, X position in meters, and parent geometry, respectively. Their data meanings are explained from the header; they are not additional requirements for this example.

Most top-level fields are EditAnywhere/BlueprintReadOnly. “Blueprint read-only” does not mean “cannot be edited in the editor.” Runtime Blueprints have no automatically generated Set Modules or Set FixtureLibrary nodes.

## 6. Module and Attribute Fields

FSuperDMXModuleInstance: ModuleName is an FName, Patch defaults to 1, and the AttributeDefs array defines attributes. See the [function reference](03_SuperDmxActorBase_Reference_en.md) for the actual address formula.

FSuperDMXAttributeDef:

| Field | Default / purpose |
| --- | --- |
| AttribName | NAME_None; replace with a name unique within the module, without relying on letter case |
| Category | Other; classification does not automatically create behavior |
| Coarse | 1; 1-based coarse-byte offset |
| Fine, Ultra | 0; unused; specify positions for higher-precision protocols |
| DmxBreak | 1; basic read APIs do not use this to cross Universes |
| bVirtualChannel | false; the flag does not automatically skip offsets or create a computed channel |
| Geometry, GDTFAttribute | Source information; leave empty for custom protocols without a source |
| NativeResolutionBytes | 0 infers resolution; 1/2/3/4 describe source byte precision; does not give basic read APIs 32-bit support |
| DefaultValue, HighlightValue | UI range 0..100 percent; not the read-failure DefaultValue; not automatically sent to input or written into the cache |
| DefaultValueRaw, HighlightValueRaw | double, default -1 means derive from the percentage; used to preserve precision |
| DefaultValueRawResolutionBytes, HighlightValueRawResolutionBytes | 0 uses channel precision; otherwise records the corresponding raw value's precision |
| SubAttributes | Segment definitions; the constructor adds one default 0..255 segment |

Available Category enum values: Dimmer, Position, Gobo, Color, Beam, Focus, Control, Shapers, Strobe, Prism, Frost, Effects, Other. Use Position for ordinary positions, Color for colors, and Control for application commands.

UI constraints are not complete runtime validation: the project must still check overlapping channels, duplicate Coarse/Fine positions, empty names, negative values, and values beyond 512. Two independent parameters should normally not occupy the same raw byte; any intentional alias must be explicit in the protocol. These examples do not use aliases.

## 7. SubAttributes and Physical Quantities

FSubAttribute's DmxMin/DmxMax form an inclusive range; Name identifies the segment; PhysicalRange supplies its physical endpoints, with units defined by the project protocol. The core methods are C++ inline methods, not declared Blueprint functions:

| Method | Behavior |
| --- | --- |
| Contains(Raw) | Min≤Raw≤Max |
| GetNormalizedPosition(Raw) | (Raw-Min)/(Max-Min), clamped to 0..1; returns 0 when Max≤Min |
| GetMappedPhysical(Raw) | Lerp between PhysicalRange.X/Y |
| FindChannelSet(Raw) | Returns the first slot containing Raw, otherwise nullptr |
| HasModeMaster() | true when the master attribute is non-empty and ModeTo≥ModeFrom; does not read master data |

FSuperDMXAttributeDef::FindSubAttribute returns the first matching segment; FindChannelSet finds a segment first, then its slot. **Neither lookup automatically evaluates ModeMaster conditions.** For project command segments, avoid conditional masters or explicitly implement reading, matching, and precedence yourself.

Example: with a ZPos segment of 0..65535 and PhysicalRange=[0,300], Raw32768 maps to 150.0023 centimeters. GetSuperDmxAttributeValue only returns the normalized value; use a separate Lerp or GetMappedPhysical to obtain centimeters.

When creating segments in C++, call `Def.SubAttributes.Reset()` first; otherwise, the constructor's default 0..255 segment may match first. The same applies to manual Blueprint/editor editing: replace the default entry before creating your own segments.

Other sub-attribute fields are GDTFAttribute, WheelName, GoboMode, StrobeMode, RotationMode, bPhysicalExplicit, NativeResolutionBytes, NativeDmxMin/Max, DeclaredColor, bHasDeclaredColor, ChannelSets, and ModeMasterGeometry/ModeMasterAttribute/ModeFrom/ModeTo. They store source, specialized mode, precision, color, and condition metadata; basic read APIs do not automatically interpret them as target behavior. NativeDmx bounds default to -1; ModeTo defaults to -1, indicating no valid range.

## 8. ChannelSets and Application Modes

FChannelSet has Name, DmxMin/Max, PhysicalRange, and the same Contains/normalization/physical mapping behavior. A single-point segment with Min=Max normalizes to 0 and uses X as its physical value. GoboMode, StrobeMode, bPhysicalExplicit, NativeResolutionBytes, NativeDmxMin/Max, Texture, Color, ColorIndex, PrismSelection, and WheelSlotIndex are additional metadata. WheelSlotIndex is 1-based; 0 means unspecified.

Your Mode attribute can define 0..63 Idle, 64..191 Preview, and 192..255 Run as three SubAttributes, or as three ChannelSets inside one segment; consuming logic must match the representation. Do not maintain two inconsistent range definitions. The 63/64 and 191/192 boundaries must be tested.

Array order determines the first match; overlap does not create automatic precedence. Gaps return nullptr, and the project must declare a hold or fallback policy. `Mode` and `Trigger` are project protocol names used here; they can be read only after their library definitions are actually created.

## 9. Library Methods and Cache Updates

GetChannelSpan() is Blueprint Pure. It calculates Max-Min+1 over all positive offsets, including gaps, and returns 0 without channels. GetPrimaryAssetId() is C++ only; its type name is SuperFixtureLibrary and its name is the asset object's name. Returning an ID does not guarantee automatic cooking.

GetEditVersion()/BumpEditVersion() are C++ only, reading/incrementing the non-serialized edit version respectively. The editor's PostEditChangeProperty increments it; project C++ must call BumpEditVersion after directly changing arrays. The function is not restricted by an editor macro, so in-place runtime edits also require notification. Do not rebuild or modify the library on every Super event.

FSuperDMXAttribute is a query selector, not a definition: InstanceIndex=0, AttribName=NAME_None, DMXChannelType=Coarse. `MakeDmx(FName,bool bFine=false)` is a C++ helper that sets the name and 8/16-bit mode, leaving the module index at 0; it is not a Blueprint node. Set structure fields explicitly for 24-bit values or other modules.

## 10. Exact C++ Steps for Creating a Temporary Library

```cpp
#include "AssetTool/SuperFixtureLibrary.h"

// Project function: call once, then retain the reference in the control object's FixtureLibrary property.
USuperFixtureLibrary* CreateProjectZLibrary(UObject* Owner)
{
    if (!IsValid(Owner)) return nullptr;
    auto* Library = NewObject<USuperFixtureLibrary>(Owner);
    Library->FixtureName = TEXT("Project Z Position 2CH");
    Library->Manufacturer = TEXT("Project");
    Library->PowerConsumption = 0;
    Library->Weight = 0;
    Library->BeamAngle = 0;
    Library->BeamIntensity = 0;
    Library->BodySize = FVector::ZeroVector;
    FSuperDMXModuleInstance Module;
    Module.ModuleName = TEXT("Main");
    Module.Patch = 1;
    FSuperDMXAttributeDef Z;
    Z.AttribName = TEXT("ZPos");
    Z.Category = EDMXAttributeCategory::Position;
    Z.Coarse = 1;
    Z.Fine = 2;
    Z.SubAttributes.Reset();
    FSubAttribute Travel;
    Travel.Name = TEXT("Travel");
    Travel.DmxMin = 0;
    Travel.DmxMax = 65535;
    Travel.PhysicalRange = FVector2D(0, 300);
    Z.SubAttributes.Add(Travel);
    Module.AttributeDefs.Add(Z);
    Library->Modules.Add(Module);
    Library->BumpEditVersion();
    return Library;
}
```

This is a temporary project-side object, not a saved uasset, and it does not automatically appear in the Content Browser. For a persistent, reloadable configuration, prefer editor assets plus JSON; do not treat a temporary object path as a persistent asset reference. Changes to a shared library affect all referencing objects; use separate libraries when independent protocols are needed.

## 11. Protocol Acceptance and Persistence

Check unique names, no unintended byte overlaps, Patch≥1, consistent Fine/Ultra bit depth, all absolute addresses in 1..512, and segment units matching the read bit depth. Save the library and referencing objects, then reload and read back. The project arranges soft-reference loading and cooking.

After a version upgrade, recheck imported fields, exported results, and addresses. Native JSON export may normalize duplicate/empty attribute names; do not rely on export to automatically repair an ambiguous protocol. Correct the asset first, then deliver the sender's address table.
