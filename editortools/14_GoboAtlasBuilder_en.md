# SuperStage GOBO Atlas Builder — User Manual

## 1. Overview

The GOBO Atlas Builder is used to extract gobo textures associated with specified attributes from the SuperStage fixture library (SuperFixtureLibrary), automatically packing them into a horizontally arranged GOBO atlas texture. The generated atlas is used by the fixture's material system to simulate gobo wheel projection effects in real time in the 3D viewport.

---

## 2. Access

**Main Menu Path**: Unreal Engine editor top main menu → **Tools** → **GOBO Atlas Builder** (under the SuperStageTools section)

---

## 3. Interface Description

```
┌──────────────────────────────────────────────┐
│  Fixture Library: [SuperFixtureLibrary Picker]│
│                                              │
│  Attribute Name: [Gobo1            ] [Search] │
│                                              │
│  Found 6 gobo textures                       │
│                                              │
│  ┌──────────────────────────────────────┐    │
│  │ [0] GoboStar (256x256)               │    │
│  │ [1] GoboCircle (256x256)             │    │
│  │ [2] GoboGear (128x128)              │    │
│  │ ...                                  │    │
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

Enter the attribute name from which to extract gobos in the **"Attribute Name"** input field, e.g., `Gobo1`.

### Step 3: Search Gobos

Click the **"Search"** button (or press Enter in the input field). The tool traverses all Modules → AttributeDefs → SubAttributes → ChannelSets in the fixture library, extracting `UTexture2D*` texture references associated with matching attributes.

Search results are displayed in the status area:
- `Found N gobo textures` — Search successful
- `Attribute 'XXX' not found or has no gobo textures` — No match found
- `Please select a fixture library first` / `Please enter an attribute name` — Incomplete parameters

### Step 4: Preview Gobo List

The preview area displays all extracted texture information as a **text list**, including index, texture name, and original dimensions, in the format `[0] GoboStar (256x256)`.

### Step 5: Generate Atlas

Click the **"Generate Atlas"** button. The tool will:
1. Read the source texture pixel data for each associated GOBO
2. Scale each texture to the assigned cell size using nearest-neighbor sampling
3. Arrange all textures **horizontally** into a **4096×256** pixel atlas
4. Skip GOBOs without textures (shown as black areas)
5. Automatically save as a UTexture2D asset

---

## 5. Generated Result

### 5.1 Output Path and Naming

| Item | Description |
|------|-------------|
| **Output Directory** | Same directory as the selected fixture library asset |
| **Naming Rule** | `LTA_{LibraryName}_{AttributeName}` |
| **Prefix Handling** | If the library name starts with `SL_`, the prefix is automatically removed |

Example: Library `SL_Spot380`, Attribute `Gobo1` → Output `LTA_Spot380_Gobo1.uasset`

### 5.2 Texture Properties

| Property | Value |
|----------|-------|
| **Size** | 4096 × 256 pixels (fixed) |
| **Format** | BGRA8 |
| **sRGB** | Disabled (linear space) |
| **Mipmap** | None |
| **Compression** | Default |
| **LOD Group** | UI |

---

## 6. Source Texture Format Support

The tool supports the following formats when reading source textures, automatically converting to BGRA8:

| Source Format | Processing Method |
|---------------|-------------------|
| **G8 (Grayscale)** | Gray → RGB (gray value copied to R/G/B), Alpha = 255 |
| **BGRA8** | Direct read |
| **Other ≥ 4-byte formats** | Read in BGRA byte order |
| **Unsupported formats** | Filled as white |

---

## 7. Usage Scenarios

### Automatic Workflow

Similar to the color atlas, when a fixture is placed in the scene, SuperStage automatically checks and generates a GOBO atlas.

### Manual Usage Scenarios

- After updating GOBO texture files, repack
- After adding new GOBO patterns to the fixture library
- When you need to generate separate atlases for different gobo attributes (e.g., `Gobo2`)

---

## 8. Notes

- The attribute name must be entered manually; the tool does not automatically scan all gobo attributes
- Atlas size is fixed at 4096×256 and cannot be adjusted
- Output path is automatically determined; no manual setting is needed
- Regenerating the atlas overwrites any existing texture asset with the same name
- If an existing asset with the same name exists, the old asset is deleted before the new one is created
- The GOBO arrangement order in the atlas matches the definition order in the fixture library
- Each GOBO proportionally divides the atlas width to ensure complete fill

---

## 9. FAQ

| Issue | Solution |
|-------|----------|
| GOBO list is empty | Confirm the fixture library has attributes matching the name, and ChannelSets contain texture references |
| Attribute name mismatch | Check the `AttribName` field in the Fixture Library Editor and confirm spelling matches |
| Generated atlas is all black | Check if source textures can be read normally and if the format is supported |
| Projected gobo pattern is blurry | Use higher-resolution source textures |
| Generation fails | Check if the fixture library asset path is valid and the disk has write permissions |
