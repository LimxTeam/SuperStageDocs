# 03 - DMX Fixture Base (SuperDmxActorBase)

> **Module**: SuperStage Runtime (ASuperDmxActorBase)  
> **Target Users**: Lighting designers, blueprint developers  
> **Prerequisites**: [02 - Fixture Library](/docs/stage-core/fixture-library)  
> **Last Updated**: 2026-04-14

---

## 1. Overview

**SuperDmxActorBase** is the base class for all DMX fixtures in SuperStage. Every fixture in the scene that needs to respond to DMX signals — whether a moving light, LED panel, smoke machine, or custom device — inherits from this base class.

It provides the following core capabilities:
- **DMX Address Assignment** (Universe, Start Address, Fixture ID)
- **DMX Channel Reading** (extract channel values from network-received data, supporting 8/16/24-bit precision)
- **Fixture Library Association** (binding with Fixture Library asset, defining channel tables)
- **Batch Matrix Reading** (read same-named attribute values for all module instances at once)
- **Address Label Display** (visually display DMX information for fixtures in the scene)
- **Auto Per-Frame Update** (Tick-driven, real-time DMX response, also effective in editor)

---

## 2. Class Inheritance Chain

```
AActor (UE5 Engine Base Class)
  │
  └── ASuperBaseActor ·············· SuperStage Root Class (L1)
        │   ├ USceneComponent* SceneBase          ← Root component
        │   ├ UArrowComponent* ForwardArrow       ← +X direction preview arrow
        │   ├ UArrowComponent* UpArrow            ← +Z direction preview arrow
        │   ├ FAssetMetaData AssetData             ← Asset metadata (UUID/Manufacturer/Group)
        │   └ bool bPreviewAssetOrientation        ← Direction preview toggle
        │
        └── ASuperDmxActorBase ····· DMX Fixture Base Class (L2) ← Class described in this document
              │   ├ FSuperDMXFixture SuperDMXFixture     ← DMX address (Universe/Address/FixtureID)
              │   ├ USuperFixtureLibrary* FixtureLibrary ← Fixture library asset reference
              │   ├ UTextRenderComponent* AddressLabel   ← Address code text component
              │   ├ GetChannelValue / GetAttributeRaw8/16/24ByIndex   ← Channel value reading
              │   ├ GetMatrixAttributeRaw / Raw16        ← Batch matrix reading
              │   ├ GetSuperDmxAttributeValue            ← Normalized attribute value reading
              │   ├ GetSuperDMXColorValue                ← RGB color quick reading
              │   ├ SuperDMXTick(DeltaTime)              ← Blueprint per-frame event
              │   ├ ForceRefreshDMX()                    ← Editor force refresh
              │   └ OnSuperDMXInitialized                ← Initialization complete delegate
              │
              ├── ASuperLightBase ···── Moving Light Base Class (L3)    → Document 04
              ├── ASuperDMXCamera ····── DMX Camera                     → Document 05
              ├── ASuperLiftingMachinery ── Lifting Machinery (6-axis)  → Document 10
              ├── ASuperRailMachinery ···── Rail Machinery (7-axis)     → Document 10
              └── ASuperLightStripEffect ── LED Strip Effect            → Document 13
```

### Component Hierarchy Diagram

```
ASuperDmxActorBase
  └── SceneBase (USceneComponent)                    ← Root component (inherited from ASuperBaseActor)
        ├── ForwardArrow (UArrowComponent)            ← +X direction preview (red arrow)
        ├── UpArrow (UArrowComponent)                 ← +Z direction preview (blue arrow)
        └── AddressLabel (UTextRenderComponent)       ← DMX address code text
              │  Text color: Light blue (135, 206, 250)
              │  Text size: 16 world units
              │  Alignment: Horizontal center, vertical top
              └  Editor-only visibility (HiddenInGame = true)
```

> **Design Note**: `ASuperDmxActorBase` itself does not create any visible mesh components. 3D fixture models are added by subclasses (e.g., `ASuperStageLight`). This keeps the base class lightweight, suitable for any scenario requiring DMX data reading.

---

## 3. DMX Patch Configuration

Select a fixture Actor in the scene, find the **DMX** group in the Details panel, and you'll see the following configuration items:

### 3.1 Core Patch Parameters

| Parameter | Location | Description | Range | Default |
|------|------|------|------|--------|
| **Universe** | SuperDMXFixture > Universe | The DMX universe the fixture belongs to | 1 - 32,767 | 1 |
| **Start Address** | SuperDMXFixture > StartAddress | Starting channel address of the fixture in that universe | 1 - 512 | 1 |
| **Fixture ID** | SuperDMXFixture > FixtureID | Unique identifier for the fixture | 0 - any positive integer | 0 |

#### Universe

- Determines which DMX universe the fixture reads data from
- Must match the Universe assigned to that fixture in the console
- Multiple Universes can be used in the same scene

#### Start Address

- The fixture occupies consecutive channels starting from this address
- The number of channels occupied is determined by the Fixture Library
- Different fixtures in the same Universe should have **non-overlapping** address ranges

> **Example**: A fixture occupying 20 channels with Start Address set to 101 occupies channels 101-120. The next fixture's Start Address should start from at least 121.

#### Fixture ID

- Unique identification number for the fixture
- Corresponds to the Fixture ID in the MA console
- Used when exporting MA macro files
- Automatically assigned in the Patch tool (to avoid conflicts)

### 3.2 Control Mode

| Parameter | Description | Default |
|------|------|--------|
| **ControlMode** | `DMX` = driven by DMX signals, `Property` = directly controlled by Details panel properties (no console needed) | DMX |

> **Property Mode**: When switched to Property mode, control parameters (Dimmer/Color/Pan/Tilt, etc.) can be adjusted directly in the Details panel via drag without connecting a console or sending DMX signals. Suitable for standalone debugging and Sequencer keyframe animation.

### 3.3 Fixture Library

| Parameter | Description |
|------|------|
| **Fixture Library** | Associated fixture library asset (USuperFixtureLibrary) |

Select the fixture library corresponding to the fixture from the dropdown. The fixture library defines the channel table, telling the system which channel controls which function.

> **Important**: Without an associated fixture library, the fixture can still read DMX data directly via channel numbers, but cannot use attribute-name-based reading.

---

## 4. Address Label

SuperStage can display DMX address information labels next to each fixture in the scene viewport, helping you quickly identify fixtures.

### 4.1 Label Display Control

| Parameter | Description | Default |
|------|------|--------|
| **Show Address Label** | Whether to display the address label in the scene | Off |
| **Include Fixture ID In Label** | Whether to include Fixture ID in the label | Off |
| **Address Label Offset** | Offset position of the label relative to the fixture (local coordinates) | (30, 0, 0) |

### 4.2 Label Format

Label text format is as follows:

- **When including Fixture ID**: `UA.{Universe}.{StartAddress}  ID.{FixtureID}`
  - Example: `UA.1.101  ID.201`
- **When not including Fixture ID**: `UA.{Universe}.{StartAddress}`
  - Example: `UA.1.101`

### 4.3 Label Appearance

| Property | Value |
|------|-----|
| **Text Color** | Light blue (135, 206, 250) |
| **Text Size** | 16 world units |
| **Alignment** | Horizontal center, vertical top |
| **Visible in Game** | No (editor-only visibility) |

### 4.4 Label Position

The label is placed at the front direction of the fixture by default (determined by the ForwardArrow component) and can be fine-tuned via **Address Label Offset**.

- Label always stays horizontal (Pitch/Roll = 0), only Yaw follows the fixture's front direction
- This ensures the label is easy to read from any angle

---

## 5. Asset Metadata

Each fixture Actor inherits from SuperBaseActor and contains the following metadata information (in the **Asset** group of the Details panel):

| Parameter | Description | Use |
|------|------|------|
| **UUID** | Asset unique identifier | Internal use |
| **Group** | Asset group | Content Browser classification |
| **Manufacturer** | Manufacturer name | CAD construction drawings, reports |
| **DMX Mode** | DMX mode name | Identifies the channel mode currently used by the fixture |
| **Display Name** | Display name | UI display |
| **Thumbnail** | Thumbnail | Content Browser preview |

---

## 6. DMX Data Reading Methods

Fixture Actors provide multiple ways to read DMX data. The following is a guide for blueprint designers:

### 6.1 Read by Channel Number (Simple Mode)

**Applicable Scenario**: No fixture library needed, read directly by channel offset.

Call the **Get Channel Value** node in blueprints:
- **Input**: Channel number (offset relative to Start Address, starting from 1)
- **Output**: Float value 0.0 - 255.0

**Example**: Start Address = 101, call GetChannelValue(5) → reads the value of channel 105

### 6.2 Read by Attribute Name (Recommended Mode)

**Applicable Scenario**: Fixture library associated, read normalized values by attribute name.

Call the **Get Super DMX Attribute Value** node in blueprints:
- **Input**: DMX Attribute (containing attribute name and instance index)
- **Output**: Float value 0.0 - 1.0 (normalized value)

**Normalization Rules**:

| Channel Type | Raw Value Range | Normalization Formula | Output Range |
|---------|-----------|-----------|---------|
| Coarse (8-bit) | 0 - 255 | Value ÷ 255 | 0.0 - 1.0 |
| Fine (16-bit) | 0 - 65,535 | Value ÷ 65,535 | 0.0 - 1.0 |
| Ultra (24-bit) | 0 - 16,777,215 | Value ÷ 16,777,215 | 0.0 - 1.0 |

> **Benefit**: Regardless of whether the attribute has 8-bit or 16-bit precision, the output is always 0.0 ~ 1.0. Blueprint logic doesn't need to be concerned with bit depth differences.

### 6.3 Read Raw Value by Attribute Name (Not Normalized)

Call the **Get Super DMX Attribute Value No Conversion** node in blueprints:
- **Output**: Float value, the raw DMX value (8-bit: 0-255, 16-bit: 0-65535, 24-bit: 0-16777215)

### 6.4 Reading Color

Call the **Get Super DMX Color Value** node in blueprints:
- **Input**: Three DMX Attributes (Red, Green, Blue)
- **Output**: FLinearColor (RGBA, A always 1.0)

This is a shortcut for reading RGB colors, equivalent to reading the normalized values of the R, G, and B attributes separately.

### 6.5 Reading Raw DMX Value

Call the **Get Super DMX Attribute Raw Value** node in blueprints:
- **Input**: DMX Attribute
- **Output**: int32 raw DMX value (0-255)

### 6.6 Matrix Reading

For matrix lights (multiple module instances), read the same-named attribute for all instances at once:

**Get Matrix Attribute Raw** node:
- **Input**: Attribute name (e.g., `Red`)
- **Output**: Float array (each element corresponds to one module instance's value, 0.0 - 1.0)
- **Sort Option**: Option to sort by Patch order

**Get Matrix Attribute Raw 16** node:
- Same as above, but for 16-bit precision attributes

### 6.7 Address Resolution and Bit Depth Query

| Function | Description | Output |
|------|------|------|
| **ResolveAttributeAddressesByIndex** | Resolve attribute's absolute channel addresses in the Universe | Coarse/Fine/Ultra absolute addresses |
| **GetAttributeBitDepthByIndex** | Returns the effective bit depth of an attribute (0/8/16/24) | int32 |
| **FindAttributeDef** | Find attribute definition (for accessing sub-attribute system) | FSuperDMXAttributeDef* |

### 6.8 Blueprint Matrix Macros (C++ Blueprint Class Only)

The following macros simplify iteration logic in matrix fixture blueprint C++ code:

| Macro | Description |
|-----|------|
| **GET_SUPER_DMX_MATRIX_VALUE** | Iterate matrix attribute values (auto-differentiates 8/16-bit), provides LightIndex and DefaultValue |
| **GET_SUPER_DMX_MATRIX_VALUE_WITH_SUBATTR** | Iterate matrix attribute values, additionally provides AttrDef / SubAttr information |
| **GET_SUPER_DMX_MATRIX_RGB** | Iterate matrix RGB three channels, provides FLinearColor OutColor |

---

## 7. DMX Attribute Parameter Structure

When using attribute reading nodes in blueprints, you need to configure the **FSuperDMXAttribute** structure:

| Parameter | Description | Example |
|------|------|------|
| **Attrib Name** | Attribute name (must match the definition in the fixture library) | `Dimmer`, `Pan`, `Tilt` |
| **Instance Index** | Module instance index (starting from 0) | Main module = 0, matrix pixels start from 1 |
| **DMX Channel Type** | Reading precision | Coarse / Fine / Ultra |

> **Tip**: Instance Index and DMX Channel Type are typically set in the fixture blueprint defaults and don't need to be modified at runtime.

---

## 8. SuperDMXTick Blueprint Event

Each fixture Actor triggers the **SuperDMXTick** blueprint event every frame. This is the core entry point for the fixture to respond to DMX data.

### 8.1 How It Works

```
Per-frame Tick
  ↓
Trigger SuperDMXTick(DeltaTime) blueprint event
  ↓
Read DMX attribute values in blueprint
  ↓
Apply values to fixture parameters (light intensity, color, rotation, etc.)
  ↓
UE render update
```

### 8.2 Key Features

- **Also triggers in editor**: Even without pressing "Play", as long as the editor is open, fixtures respond to DMX in real-time
- **Auto Tick**: No manual enabling needed; works automatically when placed in the scene
- **Editor script safe**: Internally protected with FEditorScriptExecutionGuard

### 8.3 Force Refresh DMX

When you need to immediately refresh the fixture state (e.g., after switching a Cue on the console), call **Force Refresh DMX**:

- Immediately executes SuperDMXTick once
- Marks render state as needing update
- Triggers property change notifications (updates material parameters, etc.)

> **Note**: Typically not needed manually. The system automatically batch-refreshes when needed.

---

## 9. Channel Span

A fixture's **channel span** is the length from its first channel to its last channel.

**Calculation Method**: Maximum channel address - Minimum channel address + 1

**Example**:
- A fixture's attributes occupy channels 1, 2, 3, 4, 5 (Dimmer) and 6, 7 (Pan 16-bit)
- Channel span = 7 - 1 + 1 = **7**

Channel span is used in the following scenarios:
- **Patch Tool**: Automatically calculates the next fixture's start address
- **CAD Drawings**: Displays fixture information
- **MA Export**: Generates correct Patch information

---

## 10. Fixture Blueprint Development Guide

### 10.1 Creating a Custom Fixture Blueprint

1. In Content Browser, right-click → **Blueprint Class**
2. Select **SuperDmxActorBase** (or **SuperLightBase** for moving lights) as the parent class
3. Name the blueprint (e.g., `BP_MyCustomFixture`)
4. Set Fixture Library in the blueprint defaults
5. Implement the **SuperDMXTick** event in the Event Graph

### 10.2 Typical SuperDMXTick Implementation

```
Event SuperDMXTick(DeltaTime)
  │
  ├─ GetSuperDmxAttributeValue("Dimmer") → Set light intensity
  ├─ GetSuperDmxAttributeValue("Red")   ─┐
  ├─ GetSuperDmxAttributeValue("Green") ─┤→ Set light color
  ├─ GetSuperDmxAttributeValue("Blue")  ─┘
  ├─ GetSuperDmxAttributeValue("Pan")   → Set horizontal rotation
  └─ GetSuperDmxAttributeValue("Tilt")  → Set vertical rotation
```

### 10.3 Important Notes

- Attribute names must match **exactly** with those defined in the fixture library (case-sensitive)
- Normalized values (0.0-1.0) need to be mapped to physical ranges based on fixture parameters
  - E.g., Pan value 0.0-1.0 → -270° ~ +270°
  - E.g., Dimmer value 0.0-1.0 → brightness 0% ~ 100%

---

## 11. FAQ

### Q: Fixture not updating in editor?
Confirm the SuperDMXTick event is implemented in the fixture blueprint, and input is enabled in the DMX configuration panel.

### Q: Address label displays incorrectly?
After modifying the fixture's Universe / Start Address / Fixture ID, the label updates automatically on the next frame. If it doesn't update, try moving the fixture to trigger OnConstruction.

### Q: Can multiple fixtures use the same Fixture Library?
Yes. The fixture library is a shared configuration file. Fixtures of the same model should use the same fixture library, only needing different Universe and Start Address settings.

### Q: How do I know which channels my fixture occupies?
- Check the Coarse/Fine/Ultra offset values for each attribute in the fixture library
- Use the Patch Preview tool to view address occupancy for all fixtures
- Channel Span can be viewed in the property panel

---

> **Next Steps**: Read [04 - Moving Light](/docs/stage-core/computer-light) to learn about Pan/Tilt control and matrix light usage.
