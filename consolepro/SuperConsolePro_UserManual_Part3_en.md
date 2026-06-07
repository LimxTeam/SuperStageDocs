# SuperConsolePro Lighting Console — User Manual (3)

## Programmer · Encoders · Color Picker · Preset System

---

# Chapter 1 Programmer

## 1.1 What Is the Programmer

The Programmer is one of the most core concepts in the lighting console. It is a **temporary value buffer** used to store the fixture attribute values that the user is currently editing.

### Characteristics of the Programmer

- Values adjusted via encoder wheels automatically enter the Programmer
- Values in the Programmer are displayed in **red** in the Fixture Sheet, indicating "modified but not stored"
- Values in the Programmer have the **highest priority** — they override CUE playback output
- After "Storing" the Programmer contents, values are saved to a CUE or Preset
- After executing a **Clear**, values in the Programmer are cleared, and fixtures return to the previous CUE playback state

### Programmer Workflow

```
Select Fixtures → Adjust Encoders → Values enter Programmer (red) → Store as CUE → Clear Programmer
                                    ↓
                         Real-time output to fixtures (highest priority)
```

## 1.2 Programmer Operations

### Clearing the Programmer (Clear)

| Operation Method | Scope |
|------------------|-------|
| Click the **Clear** button in the Control Bar | Clears all Programmer values for **selected fixtures** |
| Click **Clear** when no fixtures are selected | Clears all Programmer values for **all fixtures** |

After clearing, fixture attribute values return to:
1. Running CUE playback values (if any)
2. Default values (if no CUE is running)

### Activate

Click the **Activate** button in the Control Bar → "reads" the current DMX output values of selected fixtures into the Programmer.

**Use**: When you want to make small adjustments based on the current CUE playback state, first Activate to capture the current values, then fine-tune.

## 1.3 Programmer and DMX Output Priority

The console simultaneously has multiple "value sources" with the following priority from highest to lowest:

| Priority | Source | Description |
|----------|--------|-------------|
| **1 (Highest)** | Programmer | Values currently being edited by the user |
| **2** | Frame Effect | Running dynamic effects |
| **3** | CUE Playback | Running CUEs |
| **4 (Lowest)** | Default | Factory default values of fixtures |

> **Tip**: When the Programmer has values, CUE playback values will not take effect for that fixture's attributes. Remember to Clear the Programmer after programming is complete.

---

# Chapter 2 Encoder Bar

## 2.1 Encoder Bar Overview

The Encoder Bar is located at the very bottom of the console main interface and is the most frequently used area in daily programming.

### Layout Structure

```
┌────────────────────────────────────────────────────────────────────────────┐
│ Attribute Tab Bar                                                          │
│ [Dimmer] [Color] [Position] [Gobo] [Beam] [Focus] [Control] [...More]     │
├────────────────────────────────────────────────────────────────────────────┤
│ Function Area    Encoder Wheel Area                            Quick Area  │
│ ┌──────┐   ┌───────┐ ┌───────┐ ┌───────┐ ┌───────┐      ┌──────┐      │
│ │Mode  │   │Dimmer │ │  Pan  │ │ Tilt  │ │ Zoom  │      │ Full │      │
│ │Value │   │  50%  │ │ 180°  │ │  45°  │ │  30°  │      │ Zero │      │
│ ├──────┤   │  ◎   │ │  ◎   │ │  ◎   │ │  ◎   │      │Highlt│      │
│ │Prec. │   └───────┘ └───────┘ └───────┘ └───────┘      └──────┘      │
│ │Coarse│                                                                │
│ ├──────┤                                                                │
│ │Spread│                                                                │
│ │ Off  │                                                                │
│ └──────┘                                                                │
└────────────────────────────────────────────────────────────────────────────┘
```

## 2.2 Attribute Tab Bar

The Attribute Tab Bar is located above the encoder wheels and is used to switch the currently displayed attribute group.

### Attribute Categories

| Tab | Typical Attributes | Color |
|-----|--------------------|-------|
| **Dimmer** | Dimmer | Yellow |
| **Color** | Red, Green, Blue, White, Amber, UV, CTC, ColorWheel | Magenta |
| **Position** | Pan, Tilt, Pan Fine, Tilt Fine | Green |
| **Gobo** | Gobo1, Gobo2, Gobo1Rotation, Gobo2Rotation | Orange |
| **Beam** | Zoom, Focus, Iris | Blue |
| **Control** | Shutter, Strobe, Prism, Frost | White |

### Dynamic Attribute Discovery

- The Attribute Tab Bar **only shows attribute categories that the selected fixtures actually have**
- If the selected fixtures do not have Gobo attributes, the Gobo tab will not appear
- After switching selection to different types of fixtures, the tab bar automatically updates

### Switching Attribute Groups

- **Click** a tab → the encoder wheel area switches to show attributes under that category
- The currently active tab is displayed in a highlighted color

## 2.3 Encoder Wheels

Encoder wheels are the core controls in the console for adjusting fixture attribute values.

### Wheel Composition

Each encoder wheel contains the following from top to bottom:

| Area | Content |
|------|---------|
| **Attribute Name** | Displays the attribute name (e.g., "Dimmer", "Pan") |
| **Current Value** | Displays the current attribute value (e.g., "50%", "180°") |
| **Encoder Disc** | A draggable circular control; drag to change the value |
| **Unit** | The attribute's unit (%, °, etc.) |

### Value Display States

| Display Style | Meaning |
|---------------|---------|
| White numbers | Value is from CUE playback or default |
| **Red numbers** | Value is from the Programmer (modified) |
| **"Mixed"** | The selected fixtures have different values for this attribute |
| **Gray "--"** | The selected fixtures do not support this attribute |

### Drag Operation

- **Drag up/down** on the encoder disc → adjusts the attribute value
  - Drag up = increase value
  - Drag down = decrease value
- The drag speed determines the adjustment magnitude
- Coarse/Fine mode affects the step increment

### Mouse Wheel Operation

When hovering over an encoder wheel, you can use the **mouse wheel** to adjust attribute values:

| Operation | Step Size | Description |
|-----------|-----------|-------------|
| **Wheel scroll** | 1% of range/notch | Default step |
| **Coarse mode + wheel** | 5% of range/notch | Suitable for fast large adjustments |
| **Fine mode + wheel** | 0.2% of range/notch | Suitable for fine micro-adjustments |
| **Shift + wheel** | 1/5 of current step | Additional sensitivity reduction in any precision mode |

### Precise Input

- **Double-click** the encoder value area → brings up the numeric input popup
- Enter the exact numeric value directly, press Enter to confirm
- When the popup opens, the input field is auto-selected, and pressing a number key **directly replaces** the initial value (not appends)
- Supports **physical keyboard** input: digits 0-9, decimal point, Backspace, Delete, Enter to confirm, Esc to cancel, minus sign

## 2.4 Encoder Mode

The encoder mode determines which parameter of an attribute the encoder wheel adjusts.

### Mode List

| Mode | Abbreviation | What the Encoder Wheel Adjusts | Use |
|------|--------------|-------------------------------|-----|
| **Value** | Val | The current value of the attribute (e.g., brightness percentage) | Daily programming |
| **Fade In** | FIn | The fade-in time of the attribute (seconds) | Set the fade time when a CUE is activated |
| **Delay In** | DIn | The delay-in time of the attribute (seconds) | Set the wait time before a CUE activates |
| **Fade Out** | FOut | The fade-out time of the attribute (seconds) | Set the fade time when a CUE is released |
| **Delay Out** | DOut | The delay-out time of the attribute (seconds) | Set the wait time before a CUE is released |

### Switching Modes

- Click the **Mode** button in the left function button area
- Each click cycles through the 5 modes
- The current mode name is displayed on the button

### Time Mode Details

When the encoder is in any of the Fade In / Delay In / Fade Out / Delay Out modes:

- The encoder wheel no longer displays attribute values but **time values** (unit: seconds)
- Drag the encoder wheel → adjusts the time
- Double-click the value area → brings up a time input popup that supports expression input

### Time Expressions

In the time input popup, the following formats are supported:

| Expression | Meaning | Effect |
|------------|---------|--------|
| `2` or `2s` | 2 seconds | All selected fixtures use the same 2-second time |
| `0 Thru 2` or `0-2` | 0 to 2 seconds linear distribution | First fixture 0 seconds, last fixture 2 seconds, linearly distributed in between |
| `2 Thru 0` or `2-0` | 2 to 0 seconds linear distribution | First fixture 2 seconds, last fixture 0 seconds |
| `0 Thru 2 Thru 0` or `0-2-0` | Triangular distribution | Middle fixture 2 seconds, ends 0 seconds |
| `2 Thru 0 Thru 2` or `2-0-2` | Inverse triangular distribution | Middle fixture 0 seconds, ends 2 seconds |

**Example**: 5 fixtures selected, input `0 Thru 4`
```
Fixture 1 → 0.0 seconds
Fixture 2 → 1.0 seconds
Fixture 3 → 2.0 seconds
Fixture 4 → 3.0 seconds
Fixture 5 → 4.0 seconds
```

## 2.5 Encoder Precision

| Precision Mode | Abbreviation | Step Size | Use |
|----------------|--------------|-----------|-----|
| **Coarse** | C | Normal step | Quick adjustment to approximate value |
| **Fine** | F | Fine step (about 1/10) | Precise micro-adjustment |

### Switching Precision

- Click the **Precision** button in the left function button area
- Toggle between Coarse and Fine
- The current precision is displayed on the button

## 2.6 Spread Mode

Spread mode is used to set **different values** for multiple selected fixtures, rather than a uniform value. This corresponds to the Align function on MA consoles.

### Spread Mode List

| Mode | Icon | Effect |
|------|------|--------|
| **Off** | — | All fixtures set to the same value (default) |
| **Right** | > | Linear increase from first fixture to last |
| **Left** | < | Linear decrease from first fixture to last |
| **Center Out** | >< | Increase from center outward |
| **Out Center** | <> | Increase from edges toward center |

### Switching Spread Mode

- Click the **Spread** button in the left function button area
- Each click cycles through the 5 modes
- The current mode icon is displayed on the button

### Spread Mode Details

**Example**: Fixtures 1-5 selected, drag Dimmer encoder to 100%

**Off mode**:
```
Fixture 1: 100%    Fixture 2: 100%    Fixture 3: 100%    Fixture 4: 100%    Fixture 5: 100%
```

**Right (>) mode**:
```
Fixture 1: 0%      Fixture 2: 25%     Fixture 3: 50%     Fixture 4: 75%     Fixture 5: 100%
```

**Left (<) mode**:
```
Fixture 1: 100%    Fixture 2: 75%     Fixture 3: 50%     Fixture 4: 25%     Fixture 5: 0%
```

**Center Out (><) mode**:
```
Fixture 1: 0%      Fixture 2: 50%     Fixture 3: 100%    Fixture 4: 50%     Fixture 5: 0%
```

**Out Center (<>) mode**:
```
Fixture 1: 100%    Fixture 2: 50%     Fixture 3: 0%      Fixture 4: 50%     Fixture 5: 100%
```

> **Tip**: Spread mode combined with the Block/Group/Wings selection tool parameters can create more complex value distribution effects.

## 2.7 Quick Button Area (Right Side)

The right side of the Encoder Bar provides the following quick buttons:

| Button | Function |
|--------|----------|
| **Full** | Sets the Dimmer of selected fixtures to 100% |
| **Zero** | Sets the Dimmer of selected fixtures to 0% |
| **Highlight** | Highlights selected fixtures (temporary override, does not write to Programmer) |

---

# Chapter 3 Color Picker Panel

## 3.1 Panel Overview

The Color Picker panel provides an intuitive color selection interface for quickly setting the RGB color attributes of fixtures.

## 3.2 Color Selection Methods

### HSV Color Wheel

The center of the panel contains an HSV color wheel:

- **Drag the outer ring** → select Hue
- **Drag inside the triangle/square area** → adjust Saturation and Value
- The selected color is applied in real time to the selected fixtures' Red/Green/Blue attributes

### RGB Sliders

Below the color wheel are three independent R/G/B sliders:

| Slider | Range | Description |
|--------|-------|-------------|
| **R (Red)** | 0 - 255 | Drag or enter precise value |
| **G (Green)** | 0 - 255 | Drag or enter precise value |
| **B (Blue)** | 0 - 255 | Drag or enter precise value |

### Color Presets

The bottom of the panel shows color preset swatches:

- Contains commonly used colors (Red, Orange, Yellow, Green, Cyan, Blue, Purple, White, Warm White, etc.)
- **Click a swatch** → directly apply that color
- Custom color presets can be defined

## 3.3 Advanced Color Attributes

If fixtures support the following attributes, the Color Picker controls them synchronously:

| Attribute | Description |
|-----------|-------------|
| **Red / Green / Blue** | Basic RGB color mixing |
| **White** | White LED (some fixtures) |
| **Amber** | Amber LED |
| **UV** | Ultraviolet LED |
| **CTC** | Color temperature correction |
| **ColorWheel** | Color wheel position |

### Extended Color Channel Auto-Zeroing

When a fixture contains extended color channels (such as White, Amber, Indigo, Lime, RedOrange, CTO, etc.), after selecting a color via the Color Picker, the system will **automatically zero all non-RGB color category attributes** to prevent old values from extended color channels from contaminating the output.

For example: an RGBW fixture previously had White=100%, then using the Color Picker to select pure blue → Red=0%, Green=0%, Blue=100%, **White=0%** (auto-zeroed).

> **Tip**: Values modified by the Color Picker also enter the Programmer and require a Clear to restore the previous state.

---

# Chapter 4 Preset System

## 4.1 What Are Presets

A Preset is a snapshot of a set of attribute values that can be quickly applied to selected fixtures. Unlike CUEs, Presets typically save **only one category of attributes**, making them lighter and more flexible.

## 4.2 Preset Categories

| Category | Saved Attributes | Typical Use |
|----------|------------------|-------------|
| **Dimmer** | Dimmer | Save brightness values (e.g., "Half Bright", "Full Bright") |
| **Position** | Pan, Tilt | Save fixture pointing positions (e.g., "Center Stage", "Band Area") |
| **Gobo** | Gobo1, Gobo2, GoboRot | Save gobo wheel selections (e.g., "Starfield", "Flame") |
| **Color** | Red, Green, Blue, White, Amber, UV, CTC | Save color schemes (e.g., "Warm White", "Deep Blue", "Sunset Orange") |
| **Beam** | Zoom, Iris, Prism, Frost | Save beam parameters (e.g., "Narrow", "Wide", "Soft") |
| **Focus** | Focus | Save focus parameters |
| **Control** | Lamp, Reset, Speed | Save fixture control parameters |
| **Shapers** | Blade1, Blade2, Blade3, Blade4 | Save shaper blade positions |

## 4.3 Preset Panel

### Panel Layout

```
┌────────────────────────────────────────────────────────────┐
│ [All] [Dimmer] [Position] [Gobo] [Color] [Beam] [Focus] [Control] [Shapers] ← Category Tabs │
├────────────────────────────────────────────────────────────┤
│ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐  │
│ │WarmWh│ │DeepBl│ │Sunset│ │      │ │      │ │      │  │
│ └──────┘ └──────┘ └──────┘ └──────┘ └──────┘ └──────┘  │
│ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐  │
│ │      │ │      │ │      │ │      │ │      │ │      │  │
│ └──────┘ └──────┘ └──────┘ └──────┘ └──────┘ └──────┘  │
│                                             ← Preset Grid│
└────────────────────────────────────────────────────────────┘
```

### Category Tabs

- Click different category tabs to switch to showing the corresponding type of presets
- The **All** tab shows presets from all categories
- Category mapping: Dimmer (Brightness), Position, Gobo, Color, Beam, Focus, Control, Shapers

### Preset Buttons

- Saved presets are displayed as colored buttons with names
- Empty slots are displayed as dark gray blank buttons

## 4.4 Storing Presets

### Operation Steps

1. **Select fixtures** and adjust attributes to the desired state (values enter the Programmer)
2. Open the Preset panel
3. Switch to the target category tab (e.g., Color)
4. **Right-click** an empty slot
5. Select **Store**
6. Enter a preset name in the popup dialog
7. Click confirm

### Storage Rules

- Presets store only the attribute values corresponding to the current category
  - Example: Storing under the Color category will only save color attributes like Red/Green/Blue/White
- Presets store the attribute values for **all selected fixtures** currently in the Programmer

## 4.5 Applying Presets

### Basic Application

1. First select the target fixtures
2. **Click** the preset button
3. The attribute values saved in the preset are applied to the selected fixtures
4. Values enter the Programmer (displayed in red)

### Application Rules

- Presets only modify the attributes they contain
  - Example: Applying a Color preset will not affect the fixture's Position or Dimmer
- If the selected fixtures do not have a certain attribute in the preset, that attribute is automatically ignored

## 4.6 Updating Presets

### Operation Steps

1. Adjust fixture attributes to the new desired state
2. **Right-click** the preset button to update
3. Select **Update**
4. The preset contents are replaced with the current Programmer values

## 4.7 Deleting Presets

1. **Right-click** the preset button
2. Select **Delete**
3. Click confirm in the confirmation dialog
4. The preset is deleted and the slot returns to empty

## 4.8 Renaming Presets

1. **Right-click** the preset button
2. Select **Rename**
3. Enter a new name
4. Press Enter to confirm

## 4.9 Copying Presets

1. **Right-click** the preset button
2. Select **Copy**
3. **Right-click** the target empty slot
4. Select **Paste**
5. The preset is copied to the new slot

## 4.10 Preset Usage Tips

### Building a Color Preset Library

It is recommended to establish a standard color preset library at the beginning of a project:

| Preset Name | RGB Reference | Use |
|-------------|---------------|-----|
| Warm White | R255 G200 B150 | Warm white tone |
| Cool White | R200 G220 B255 | Cool white tone |
| Deep Blue | R0 G50 B255 | Deep blue night scene |
| Sunset Orange | R255 G100 B30 | Warm sunset tone |
| Forest Green | R30 G200 B50 | Forest green |
| Rose Pink | R255 G100 B150 | Rose pink |
| Congo Blue | R0 G0 B180 | Congo blue |

### Building a Position Preset Library

It is recommended to divide position presets by performance area:

| Preset Name | Description |
|-------------|-------------|
| Center Stage | Center of the stage |
| Down Stage Left | Front left of stage |
| Down Stage Right | Front right of stage |
| Up Stage Center | Center rear of stage |
| Band Area | Band area |
| Audience | Audience seating |

---

> **Next**: [User Manual (4) CUE Programming, Playback Panel and Frame Editor](SuperConsolePro_UserManual_Part4.md)
