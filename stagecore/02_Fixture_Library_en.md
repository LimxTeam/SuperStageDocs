# 02 - Fixture Library

> **Module**: SuperFixtureLibrary  
> **Target Users**: Lighting designers, fixture blueprint creators  
> **Prerequisites**: [00 - DMX System Overview](00_DMX_System_Overview_en.md)
> **Last Updated**: 2026-04-14


> **Terminology change in 26H2.6**: this asset is called the **channel library** throughout the documentation (asset prefix `CL_`). It describes only "which channels one DMX mode of this fixture has". What describes "what this fixture is" — body, mechanics, emitters, optics — is the **fixture definition** (its asset name is the model name, with no prefix).
>
> One fixture definition can have several DMX modes, each pointing at one channel library; one channel library can be shared by several definitions. **Bindings** (which channel drives which capability) live in the definition, not in the channel library. See [Fixture System Overview](../fixture/00_FixtureSystem_Overview_en.md).
---

## 1. What is a Fixture Library

The **Fixture Library** is the configuration file in SuperStage that defines a fixture's DMX channel table. It tells the system:

- What controllable functions this fixture has (Dimmer, Pan, Tilt, Color, Gobo, etc.)
- Which channel each function occupies (relative offset)
- The precision of each function (8-bit/16-bit/24-bit)
- The meaning of different value ranges within each function (e.g., channel value 0-127 = White on, 128-255 = Strobe)

In simple terms, the fixture library is the fixture's "manual" — it tells SuperStage how to interpret the DMX data sent by the console.

> **Analogy**: The fixture library is like the Fixture Profile in an MA console, or the channel definitions in a GDTF file.

---

## 2. Creating a Fixture Library Asset

### Steps

1. In the Content Browser, **right-click** and create a Super Fixture Library
2. Name the asset (the shipped library convention is `CL_<Model>_<ModeName>`)
3. Double-click to open the asset for editing

> Most of the time you do not need to create one by hand: importing a fixture from GDTF generates a channel library for each of its DMX modes. See [GDTF Import](../fixture/02_GdtfImport_en.md).

### Naming Conventions

| Prefix | Example | Description |
|------|------|------|
| `CL_` | `CL_MegaPointe_Standard` | The shipped library's convention: `CL_<Model>_<ModeName>` |
| no prefix | `MegaPointe` | The fixture definition, not a channel library |
| `LTC_` | `LTC_MegaPointe` | That model's thumbnail in the Content Browser |
| `SP_` | `SP_MegaPointe_Prism` | Prism preset |

---

## 3. Fixture Library Properties Detail

Double-click the channel library asset to open its dedicated editor. **It is not the UE Details panel** — it is a four-level list editor.

### 3.1 Header Fields

Across the top of the editor:

| Field | Description | Example |
|------|------|------|
| **Fixture Name** | Fixture model name | `MegaPointe` |
| **Manufacturer** | Manufacturer | `Robe` |
| **Power (W)** | Rated power (watts) | `1700` |
| **Weight (kg)** | Weight (kilograms) | `36.8` |
| **Channel Count** | Read-only channel span of the current mode | `35` |
| **GDTF Library** | Bound GDTF file path | `FixtureLibrary/GDTF/...` |

There is also a help button at the right of the header. This information is used for reports and external delivery material; it does not affect DMX control logic.

### 3.2 DMX Modes

The core part of the fixture library is the **Modules (module instance list)**.

Each module instance represents a functional module of the fixture. For most fixtures, only **one module instance** is needed. For matrix lights (e.g., LED panel lights), each bead/pixel is an independent module instance.

Click the **+** button next to **Modules** to add a module instance.

#### Module Instance Properties

| Property | Description | Example |
|------|------|------|
| **Module Name** | Module name (optional, for identification) | `Main` or `Pixel_1` |
| **Patch** | Module start offset relative to StartAddress. Runtime computes the module base as `StartAddress + max(0, Patch - 1)`. Single-module fixtures usually use **1**. Matrix fixtures use different Patch values for each pixel module | `1` |
| **Attribute Defs** | Attribute definition list for this module (see details below) | — |

---

## 4. Attribute Definitions (Attribute Defs)

Each module instance can contain multiple **attribute definitions (Attribute Def)**, each corresponding to a controllable function of the fixture.

Adding is done with the **+ Add** button on the bottom toolbar: it adds one entry at whatever level is currently selected — a mode when a mode is selected, an attribute when an attribute is selected, and so on.

### 4.1 Basic Attribute Parameters

| Parameter | Description | Detailed Explanation |
|------|------|---------|
| **Attrib Name** | Attribute name | Must be unique. Bindings use this name to connect the channel to one of the fixture's capabilities. Recommended standard names: `Dimmer`, `Pan`, `Tilt`, `Red`, `Green`, `Blue`, `White`, `Gobo`, `Prism`, `Focus`, `Zoom`, `Shutter`, `ColorWheel`, etc. |
| **Coarse** | Coarse channel offset (1-based) | **Required**. The main channel offset of this attribute within the module. For example, Coarse=5 means this attribute starts from the 5th channel of the module |
| **Fine** | Fine channel offset (1-based) | **Optional**. Set to 0 for no fine channel. Used for 16-bit precision control (e.g., Pan/Tilt) |
| **Ultra** | Ultra channel offset (1-based) | **Optional**. Set to 0 for no ultra channel. Used for 24-bit precision control (rarely used) |
| **Category** | Attribute category | Used for console UI grouping, does not affect functional logic |
| **DefaultValue** | Default value, as a percentage (0-100) | The idle value — where this channel sits with no DMX input. On GDTF import it is taken from the initial value declared in the package |
| **HighlightValue** | Highlight DMX value (0-100) | Used for console Highlight function |

### 4.2 Channel Offset Details

Channel offset is the **offset relative to the module Patch**, counting from 1.

**Address Calculation Formula**:
```
Absolute Channel Address = Fixture StartAddress + Module Offset (Patch - 1) + Attribute Offset (Coarse - 1)
```

**Example**: A fixture with StartAddress = 101, Module Patch = 1

| Attribute | Coarse | Fine | Actually Occupied Channels |
|------|--------|------|-------------|
| Dimmer | 1 | 0 | 101 |
| Pan | 2 | 3 | 102-103 |
| Tilt | 4 | 5 | 104-105 |
| Color Wheel | 6 | 0 | 106 |
| Gobo 1 | 7 | 8 | 107-108 |
| Prism | 9 | 0 | 109 |

### 4.3 Bit Depth

**There is no separate bit-depth setting** — it follows from how many offsets you fill in:

| Offsets filled | Bit depth | Value range | Applicable scenarios |
|------|------|--------|---------|
| **Coarse** only | 8-bit | 0 - 255 | On/off functions (Gobo, Prism, Color Wheel, etc.) |
| **Coarse + Fine** | 16-bit | 0 - 65,535 | Functions requiring smooth movement (Pan, Tilt, Zoom, etc.) |
| **Coarse + Fine + Ultra** | 24-bit | 0 - 16,777,215 | Extremely high precision needs (rarely used) |

Leaving Fine / Ultra at 0 means that level does not exist.

> **Important**: When fixture attributes are read, the system normalizes values according to the 8-bit, 16-bit, or 24-bit path:
> - Coarse → Raw value ÷ 255 = 0.0 ~ 1.0
> - Fine → Raw value ÷ 65,535 = 0.0 ~ 1.0
> - Ultra → Raw value ÷ 16,777,215 = 0.0 ~ 1.0

#### Quick Channel Precision Selection Guide

| Attribute Type | Recommended Precision | Reason |
|---------|---------|------|
| **Dimmer** | Fine (16-bit) | Visible stepping occurs with 8-bit precision at low brightness |
| **Pan / Tilt** | Fine (16-bit) | Rotational movement needs smooth transition; 8-bit only gives 256 steps — far from sufficient |
| **Zoom / Focus / Iris** | Fine (16-bit) | Optical parameter adjustment needs fine control |
| **ColorWheel** | Coarse (8-bit) | Discrete selection (6-12 colors), no continuous transition needed |
| **Gobo Selection** | Coarse (8-bit) | Discrete selection (6-12 gobos) |
| **Gobo Rotation** | Fine (16-bit) | Continuous rotation speed needs smoothness |
| **Prism** | Coarse (8-bit) | Discrete selection (on/off/rotation direction) |
| **Strobe** | Coarse (8-bit) | Strobe speed usually sufficient at 8-bit |
| **RGB / RGBW** | Coarse (8-bit) | Color mixing usually sufficient at 8-bit (256 levels) |
| **CMY** | Coarse (8-bit) | Same as RGB |
| **CTO / CTB** | Coarse (8-bit) | Color temperature adjustment usually sufficient at 8-bit |
| **Control Channels (Reset/Lamp)** | Coarse (8-bit) | Command-type channels, no fine adjustment needed |

> **Rule of Thumb**: If the fixture manual allocates 2 channels (Coarse + Fine) for an attribute, fill in the Fine channel. If only 1 channel is allocated, fill in Coarse only. Configure strictly according to the fixture manual.

### 4.4 Attribute Categories

| Category | Description | Typical Attributes |
|------|------|---------|
| **Dimmer** | Brightness | Dimmer |
| **Position** | Position | Pan, Tilt, PanRot, TiltRot |
| **Gobo** | Gobo | Gobo1, Gobo2, GoboRot |
| **Color** | Color | Red, Green, Blue, White, Amber, ColorWheel, CTO |
| **Beam** | Beam | Zoom, Iris |
| **Focus** | Focus | Focus |
| **Control** | Control | Reset, LampOn, LampOff |
| **Shapers** | Cutting | Blade A1-B4, ShaperRot |
| **Strobe** | Strobe | Shutter, Strobe |
| **Prism** | Prism | Prism1, Prism2, PrismRot |
| **Frost** | Frost | Frost |
| **Effects** | Effects | EffectDimmer, EffectValue |
| **Other** | Other | Any uncategorized functions |

---

## 5. Sub-Attributes

Each attribute definition can contain **sub-attributes (Sub-Attributes)** for defining functional subdivisions within channel value ranges. This is similar to "Channel Sets" in MA consoles.

### 5.1 Sub-Attribute Parameters

Columns at the sub-attribute level:

| Column | Description | Example |
|------|------|------|
| **DMX Start** | Minimum DMX value of the range (0-255) | `0` |
| **DMX End** | Maximum DMX value of the range (0-255) | `255` |
| **Physical Range** | Physical value range (real units, min / max) | (0.0, 540.0) (degrees/metres/percent etc.) |
| **Channel Sets** | How many slots hang under this segment (see Section 6) | One or more slots per sub-attribute |
| **Strobe Mode** | Strobe mode. **Only appears under Dimmer / Strobe category attributes** | Closed / Open / Linear / Pulse / Ramp Up / Ramp Down / Sine / Random |
| **Rotation Mode** | Rotation mode. **Only appears under Position category attributes** | Off / Position / Infinite. **Stop was removed in 26H2.6**; its meaning folded into Off, and existing assets are redirected automatically |

> There is **no name column at the sub-attribute level** — names live one level down, on the slots. The "sub-attribute name" in the examples below is for readability; in practice it goes in the slot's Name column.

### 5.2 Sub-Attribute Examples

**Gobo Channel Sub-Attribute Definitions**:

| Sub-Attribute Name | DMX Min | DMX Max | Description |
|-----------|---------|---------|------|
| Open | 0 | 7 | No Gobo (White) |
| Gobo 1 | 8 | 15 | First gobo pattern |
| Gobo 2 | 16 | 23 | Second gobo pattern |
| Gobo 3 | 24 | 31 | Third gobo pattern |
| Gobo 1 Spin CW | 32 | 95 | Gobo 1 clockwise rotation (slow→fast) |
| Gobo 1 Spin CCW | 96 | 159 | Gobo 1 counter-clockwise rotation (slow→fast) |

**Pan Channel Sub-Attribute Definitions**:

| Sub-Attribute Name | DMX Min | DMX Max | Physical Min | Physical Max | Description |
|-----------|---------|---------|-------------|-------------|------|
| Pan Range | 0 | 255 | -270.0 | 270.0 | Pan angle range ±270° |

---

## 6. Channel Sets

**Channel Sets** are a shortcut, similar to MA2's Channel Set function. They provide preset named values for attribute definitions.

### 6.1 Slot Parameters

Columns at the slot level (**the later columns change with the attribute's category**):

| Column | Default | Appears when | Description |
|------|---|---|------|
| **Name** | empty | Always | Slot name, e.g. `Open`, `Gobo 1`, `Red` |
| **DMX Start** / **DMX End** | 0 / 0 | Always | The DMX value range this segment occupies |
| **Physical Range** | (0, 0) | Always | Physical value range (min / max) |
| **Gobo Mode** | Static | Gobo category | Gobo mode: Static / Scroll / Shake |
| **Texture** | empty | Gobo category | Gobo texture |
| **Color** | white | Color category | Colour value |
| **Color Index** | 0.0 | Color category | Colour index, used to position the colour wheel |
| **Prism Selection** | Off | Prism category | Prism layer: Off / Prism 1 / Prism 2 / Prism 3. **Attaching a prism preset in the fixture definition is not enough** — without selecting a layer for some DMX range here, no segment ever actually selects the prism |

> **When Physical Range is left empty (both ends 0), the segment inherits the range declared on its parent sub-attribute**; fill it in and the filled value wins. Assets imported from GDTF preserve whatever the package wrote, including single-point calibrations like `6000..6000` — that is not "empty", so do not clear it to 0 by hand.

### 6.2 Use Cases

Channel Sets are typically used for quick access to specific function values, for example:

- Gobo channel: Open=0, Gobo1=10, Gobo2=20 ...
- Shutter channel: Open=255, Closed=0, Strobe=128

---

## 7. Matrix Light Configuration

For LED matrix lights (e.g., Robe Spiider, Ayrton MagicPanel, etc.), each bead/pixel needs to be defined as an independent module instance.

### 7.1 Matrix Configuration Steps

1. Determine the fixture's pixel count (e.g., 7 beads)
2. Determine each pixel's channel layout (e.g., each pixel 4 channels: RGBW)
3. Determine the master control channel count (e.g., Dimmer, Pan, Tilt, etc. occupy first 16 channels)

**Example**: A 7-bead LED matrix light, 16 master channels, each bead RGBW 4 channels

| Module Instance | Module Name | Patch | Attributes |
|---------|-------------|-------|------|
| 0 | Main | 1 | Dimmer(1), Pan(2/3), Tilt(4/5), ... |
| 1 | Pixel 1 | 17 | Red(1), Green(2), Blue(3), White(4) |
| 2 | Pixel 2 | 21 | Red(1), Green(2), Blue(3), White(4) |
| 3 | Pixel 3 | 25 | Red(1), Green(2), Blue(3), White(4) |
| 4 | Pixel 4 | 29 | Red(1), Green(2), Blue(3), White(4) |
| 5 | Pixel 5 | 33 | Red(1), Green(2), Blue(3), White(4) |
| 6 | Pixel 6 | 37 | Red(1), Green(2), Blue(3), White(4) |
| 7 | Pixel 7 | 41 | Red(1), Green(2), Blue(3), White(4) |

> **Note**: Each pixel module's Patch value = Master channel count + (Pixel index × Pixel channel count) + 1

### 7.2 Matrix Attribute Reading

After configuring the matrix, the system can iterate modules and return same-named attribute values as an array, such as all `Red` values across pixel modules. Actual response depends on whether the fixture actor or blueprint uses the matrix read path.

---

## 8. Associating Fixture Library with Fixture Actors

After creating a fixture library, you need to associate it with the fixture Actor in the scene:

1. Select the fixture Actor in the scene
2. In the Details panel, find the **Fixture Library** property
3. Select the corresponding fixture library asset from the dropdown

> **Tip**: For custom fixture blueprints, it's recommended to pre-set the Fixture Library in the blueprint defaults, so it's automatically associated when placed in the scene.

---

## 9. Built-in Fixture Libraries

SuperStage organises pre-made fixture libraries by manufacturer under `Content/Library/Lighting/`. In 26H2.6 that covers 54 manufacturers and 807 models, all generated by GDTF import:

| Manufacturer folder | Models |
|------|------:|
| `ChauvetProfessional` | 71 |
| `Prolights` | 69 |
| `Robe_Lighting` | 68 |
| `CKC_Lighting` | 60 |
| `Elation` | 58 |
| `Ayrton` | 42 |
| `Terbly` | 38 |
| `Acme` | 38 |
| `MartinProfessional` | 35 |
| `Cameo` | 35 |
| The other 44 manufacturers | See the shipped content directory |

Folder names are manufacturer names as declared in the GDTF packages.

You can use these pre-made fixture libraries directly, or duplicate one as a starting point for a custom fixture library.

---

## 10. Usage Notes

### Channel Library Naming
- Use the `CL_<Model>_<ModeName>` format, e.g. `CL_MegaPointe_Standard`
- Different DMX modes of the same fixture need different channel libraries

### Channel Offsets
- Fill in strictly according to the fixture manual's channel table
- Note that offsets start from **1** (not 0)
- Fine channels usually follow immediately after Coarse channels

### Attribute Naming
- Use standardized names (Dimmer, Pan, Tilt, Red, Green, Blue...)
- Keep naming consistent within the same project so fixture assets can reuse the same attribute names
- Attribute names are case-sensitive

### Verification
- After configuration, place a fixture in the scene and connect the console
- Push each channel value one by one to confirm each attribute responds correctly
- Use the DMX Activity Monitor to assist debugging

---

## 11. FAQ

### Q: Fixture doesn't respond to a certain channel?
Check if the Coarse offset value for that channel is correct. Note that offsets start from 1.

### Q: Pan/Tilt movement not smooth enough?
Make sure the attribute has a Fine offset filled in — that is what makes it 16-bit.

### Q: Only the first pixel of the matrix light responds?
Check if each pixel module instance's Patch value is correctly calculated.

### Q: How do I know how many channels a fixture occupies?
The **Channel Count** readout at the top of the editor is the channel span: only Coarse / Fine / Ultra offsets greater than 0 are counted, and the span is the highest valid channel address minus the lowest, plus one.

---

> **Next Steps**: Read [03 - DMX Fixture Base](03_DMX_Actor_Base_en.md) to learn how to configure fixture DMX addresses in the scene.
