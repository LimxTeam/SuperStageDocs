# 02 - Fixture Library

> **Module**: SuperFixtureLibrary (USuperFixtureLibrary)  
> **Target Users**: Lighting designers, fixture blueprint creators  
> **Prerequisites**: [00 - DMX System Overview](/docs/stage-core/overview)  
> **Last Updated**: 2026-04-14

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

1. In Content Browser, **right-click** → **Miscellaneous** → **Data Asset**
2. In the class selection window that appears, select **SuperFixtureLibrary**
3. Name the asset (recommend using the fixture model name, e.g., `FL_ClayPaky_Sharpy`)
4. Double-click to open the asset for editing

### Naming Conventions

| Prefix | Example | Description |
|------|------|------|
| `FL_` | `FL_Robe_MegaPointe` | Abbreviation of Fixture Library |
| `FL_Manufacturer_Model` | `FL_Chauvet_MaverickMK3` | Easy identification |

---

## 3. Fixture Library Properties Detail

After opening the fixture library asset, you can see the following properties in the Details panel:

### 3.1 Basic Information

| Property | Description | Example |
|------|------|------|
| **Fixture Name** | Fixture model name | `MegaPointe` |
| **Manufacturer** | Manufacturer | `Robe` |
| **Source** | Data source | `Custom`, `GDTF`, `MA2`, `MA3` |
| **Power** | Rated power (watts) | `1700` |
| **Weight** | Weight (kilograms) | `36.8` |

This information is mainly used for CAD construction drawings and report statistics, and does not affect DMX control logic.

### 3.2 Module Instance List (Modules)

The core part of the fixture library is the **Modules (module instance list)**.

Each module instance represents a functional module of the fixture. For most fixtures, only **one module instance** is needed. For matrix lights (e.g., LED panel lights), each bead/pixel is an independent module instance.

Click the **+** button next to **Modules** to add a module instance.

#### Module Instance Properties

| Property | Description | Example |
|------|------|------|
| **Module Name** | Module name (optional, for identification) | `Main` or `Pixel_1` |
| **Patch** | Channel offset relative to StartAddress (0-based). Absolute base = StartAddress + Patch. For single-module fixtures, typically set to **0** or **1**. Matrix lights have different Patch values for each pixel module | `1` |
| **Attribute Defs** | Attribute definition list for this module (see details below) | — |

---

## 4. Attribute Definitions (Attribute Defs)

Each module instance can contain multiple **attribute definitions (Attribute Def)**, each corresponding to a controllable function of the fixture.

Click the **+** button next to **Attribute Defs** to add an attribute.

### 4.1 Basic Attribute Parameters

| Parameter | Description | Detailed Explanation |
|------|------|---------|
| **Attrib Name** | Attribute name (FName) | Must be unique. This is the identifier used to reference this attribute in blueprints. Recommended standard names: `Dimmer`, `Pan`, `Tilt`, `Red`, `Green`, `Blue`, `White`, `Gobo`, `Prism`, `Focus`, `Zoom`, `Shutter`, `ColorWheel`, etc. |
| **Coarse** | Coarse channel offset (1-based) | **Required**. The main channel offset of this attribute within the module. For example, Coarse=5 means this attribute starts from the 5th channel of the module |
| **Fine** | Fine channel offset (1-based) | **Optional**. Set to 0 for no fine channel. Used for 16-bit precision control (e.g., Pan/Tilt) |
| **Ultra** | Ultra channel offset (1-based) | **Optional**. Set to 0 for no ultra channel. Used for 24-bit precision control (rarely used) |
| **Category** | Attribute category (EDMXAttributeCategory) | Used for console UI grouping, does not affect functional logic |
| **DefaultValue** | Default DMX value (0-100) | Used for console Default preset |
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

### 4.3 Channel Precision Types

| Type | Bit Depth | Channels Used | Value Range | Applicable Scenarios |
|------|------|-----------|--------|---------|
| **Coarse** | 8-bit | Coarse channel only | 0 - 255 | On/off functions (Gobo, Prism, Color Wheel, etc.) |
| **Fine** | 16-bit | Coarse + Fine channels | 0 - 65,535 | Functions requiring smooth movement (Pan, Tilt, Zoom, etc.) |
| **Ultra** | 24-bit | Coarse + Fine + Ultra channels | 0 - 16,777,215 | Extremely high precision needs (rarely used) |

> **Important**: Channel Type determines the normalization method when reading this attribute in blueprints:
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

> **Rule of Thumb**: If the fixture manual allocates 2 channels (Coarse + Fine) for an attribute, use Fine. If only 1 channel is allocated, use Coarse. Configure strictly according to the fixture manual.

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

| Parameter | Description | Example |
|------|------|------|
| **WheelName** | Sub-attribute name (Wheel identifier) | `Open White`, `Gobo 1`, `Strobe Slow-Fast` |
| **StrobeMode** | Strobe mode (Dimmer/Strobe type attributes only) | Closed / Open / Linear / Pulse / RampUp / RampDown / Sine / Random |
| **RotationMode** | Rotation mode (Position type attributes only) | Off / Stop / Position / Infinite |
| **DMX Range Min** | Minimum DMX value range (0-255) | `0` |
| **DMX Range Max** | Maximum DMX value range (0-255) | `255` |
| **Physical Range** | Physical value range (real units, FVector2D) | (0.0, 540.0) (degrees/meters/percent etc.) |
| **Channel Sets** | Embedded slot list (see Section 6) | Array of FChannelSet |

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

### 6.1 Channel Set Parameters (FChannelSet)

| Parameter | Description | Example |
|------|------|------|
| **Name** | Slot name | `Open`, `Gobo 1`, `Red` |
| **GoboMode** | Gobo mode | Static / Scrolling / Shake |
| **DmxMin** | DMX value range minimum | `0` |
| **DmxMax** | DMX value range maximum | `15` |
| **PhysicalRange** | Physical value range (FVector2D) | (0.0, 360.0) |
| **Texture** | Gobo texture (UTexture2D, Gobo-specific) | Pattern texture asset |
| **Color** | Color value (FLinearColor, ColorWheel-specific) | (1,0,0,1) |
| **ColorIndex** | Color index value (for ColorWheel positioning) | `0.5` |
| **PrismSelection** | Prism selection (EPrismLayerSelect) | None / Prism1 / Prism2 / Prism3 |

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

After configuring the matrix, blueprints can use matrix reading functions to retrieve same-named attribute values for all pixels at once (e.g., get Red values for all pixels), returning an array. See the matrix control section in [04 - Moving Light](/docs/stage-core/computer-light) for details.

---

## 8. Associating Fixture Library with Fixture Actors

After creating a fixture library, you need to associate it with the fixture Actor in the scene:

1. Select the fixture Actor in the scene
2. In the Details panel, find the **Fixture Library** property
3. Select the corresponding fixture library asset from the dropdown

> **Tip**: For custom fixture blueprints, it's recommended to pre-set the Fixture Library in the blueprint defaults, so it's automatically associated when placed in the scene.

---

## 9. Built-in Fixture Libraries

SuperStage provides pre-made fixture libraries for multiple brands in the `Content/LightingLibrary/` directory:

| Brand | Directory | Description |
|------|------|------|
| **Acme** | `LightingLibrary/Acme/` | Acme fixture series |
| **Chauvet** | `LightingLibrary/Chauvet/` | Chauvet fixture series |
| **ClayPaky** | `LightingLibrary/ClayPaky/` | Clay Paky fixture series |
| Others | `LightingLibrary/...` | More brands |

You can use these pre-made fixture libraries directly, or duplicate one as a starting point for a custom fixture library.

---

## 10. Best Practices

### Fixture Library Naming
- Use `FL_Brand_Model_Mode` format, e.g., `FL_Robe_MegaPointe_Standard`
- Different DMX modes of the same fixture should have different fixture libraries

### Channel Offsets
- Fill in strictly according to the fixture manual's channel table
- Note that offsets start from **1** (not 0)
- Fine channels usually follow immediately after Coarse channels

### Attribute Naming
- Use standardized names (Dimmer, Pan, Tilt, Red, Green, Blue...)
- Keep naming consistent within the same project for blueprint reusability
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
Ensure Fine channels are correctly set and Channel Type is set to Fine (16-bit).

### Q: Only the first pixel of the matrix light responds?
Check if each pixel module instance's Patch value is correctly calculated.

### Q: How do I know how many channels a fixture occupies?
The fixture's channel span is automatically calculated and displayed in the Patch tool and CAD view. It equals the maximum channel address across all modules - minimum channel address + 1.

---

> **Next Steps**: Read [03 - DMX Fixture Base](/docs/stage-core/dmx-actor-base) to learn how to configure fixture DMX addresses in the scene.
