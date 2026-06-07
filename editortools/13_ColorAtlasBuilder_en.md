# SuperStage Color Atlas Builder — User Manual

## 1. Overview

The Color Atlas Builder is used to extract color data from specified attributes in the SuperStage fixture library (SuperFixtureLibrary) and automatically generate a horizontally arranged color atlas texture. The generated atlas texture is used by the fixture's material system to accurately simulate the actual color effect of the fixture's color wheel in the 3D viewport.

---

## 2. Access

**Main Menu Path**: Unreal Engine editor top main menu → **Tools** → **Color Atlas Builder** (under the SuperStageTools section)

---

## 3. Interface Description

```
┌──────────────────────────────────────────────┐
│  Fixture Library: [SuperFixtureLibrary Picker]│
│                                              │
│  Attribute Name: [ColorWheel1      ] [Search] │
│                                              │
│  Found 8 colors (Atlas Size: 256x16)         │
│                                              │
│  ┌──────────────────────────────────────┐    │
│  │ ■ ■ ■ ■ ■ ■ ■ ■  ← Horizontal scroll│    │
│  │ 0  1  2  3  4  5  6  7               │    │
│  └──────────────────────────────────────┘    │
│                                              │
│                              [Generate Atlas] │
└──────────────────────────────────────────────┘
```

---

## 4. Steps

### Step 1: Select Fixture Library

Select a `USuperFixtureLibrary` asset via the asset picker (SObjectPropertyEntryBox).

### Step 2: Enter Attribute Name

Enter the attribute name from which to extract colors in the **"Attribute Name"** input field, e.g., `ColorWheel1`.

### Step 3: Search Colors

Click the **"Search"** button (or press Enter in the input field). The tool traverses all Modules → AttributeDefs → SubAttributes → ChannelSets in the fixture library, extracting `FLinearColor` color values from matching attributes.

Search results are displayed in the status area:
- `Found N colors (Atlas Size: 256x16)` — Search successful
- `Attribute 'XXX' not found or has no color definitions` — No match found
- `Please select a fixture library first` / `Please enter an attribute name` — Incomplete parameters

### Step 4: Preview Colors

The color preview area displays all extracted colors in a **horizontal scroll** manner. Each color block is 32×32 pixels with the index number shown below.

### Step 5: Generate Atlas

Click the **"Generate Atlas"** button. The tool will:
1. Arrange all colors **horizontally** into a **256×16** pixel texture
2. Each color proportionally divides the atlas width to ensure complete fill
3. Automatically save as a UTexture2D asset

---

## 5. Generated Result

### 5.1 Output Path and Naming

| Item | Description |
|------|-------------|
| **Output Directory** | Same directory as the selected fixture library asset |
| **Naming Rule** | `CLA_{LibraryName}_{AttributeName}` |
| **Prefix Handling** | If the library name starts with `SL_`, the prefix is automatically removed |

Example: Library `SL_Spot380`, Attribute `ColorWheel1` → Output `CLA_Spot380_ColorWheel1.uasset`

### 5.2 Texture Properties

| Property | Value |
|----------|-------|
| **Size** | 256 × 16 pixels (fixed) |
| **Format** | BGRA8 |
| **sRGB** | Enabled |
| **Mipmap** | None |
| **Filtering** | Nearest — prevents color interpolation |
| **Compression** | VectorDisplacementmap (lossless) |
| **LOD Group** | UI |

---

## 6. Usage Scenarios

### Automatic Workflow

In most cases, when a fixture is placed in the scene, SuperStage automatically checks whether a color atlas already exists for the corresponding fixture library and generates one automatically if not.

### Manual Usage Scenarios

- After modifying color definitions in the fixture library, manually regenerate the atlas
- When you need to generate separate atlases for different color attributes (e.g., `ColorWheel2`)

---

## 7. Notes

- The attribute name must be entered manually; the tool does not automatically scan all color attributes
- Atlas size is fixed at 256×16 and cannot be adjusted
- Output path is automatically determined; no manual setting is needed
- Regenerating the atlas overwrites any existing texture asset with the same name
- If an existing asset with the same name exists, the old asset is deleted before the new one is created
- The generated atlas texture is a standard UE texture asset that can be viewed and managed in the Content Browser
