# SuperStage DMX Patch Preview — User Manual

## 1. Overview

The DMX Patch Preview panel (Super DMX Patch Preview) provides a visual list of DMX address information for all fixtures in the scene. You can view, sort, and directly edit each fixture's DMX parameters in this panel without needing to select fixtures individually.

---

## 2. Access

**Main Menu Path**: Toolbar **SuperStage** dropdown menu → **SuperDMXTool** → **PatchPreview**

---

## 3. Interface Description

```
┌────────────────────────────────────────────────────┐
│  Fixture Patch List                                 │
│  ┌──────────────────────────────────────────────┐  │
│  │ Model     | Name       | FID | Uni | Addr    │  │
│  │ Spot380   | Spot_1     | 1   | 1   | 1       │  │
│  │ Spot380   | Spot_2     | 2   | 1   | 25      │  │
│  │ Wash600   | Wash_1     | 3   | 1   | 49      │  │
│  │ Wash600   | Wash_2     | 4   | 1   | 73      │  │
│  │ ...                                          │  │
│  └──────────────────────────────────────────────┘  │
│                                                     │
│  [Refresh List]                                     │
└────────────────────────────────────────────────────┘
```

---

## 4. List Field Description

| Column | Description | Editable |
|--------|-------------|----------|
| **Model** | The Blueprint class name of the fixture | No (read-only) |
| **Name** | The fixture's label in the scene | No (read-only) |
| **Fixture ID (FID)** | Fixture ID number, used for console identification | Yes |
| **Universe (Uni)** | DMX Universe number | Yes |
| **Start Address (Addr)** | Starting channel in that Universe | Yes |

---

## 5. Operations

### 5.1 Viewing Fixture Information

After opening the panel, all SuperStage DMX fixtures currently in the scene and their patch information are automatically listed.

### 5.2 Editing DMX Parameters

Click on editable fields (Fixture ID, Universe, Start Address) directly in the list, enter the new value, and press **Enter** to confirm. Modifications are applied immediately to the corresponding fixture.

| Parameter | Valid Range |
|-----------|-------------|
| **Fixture ID** | 1 - 9999 |
| **Universe** | 1 - 256 |
| **Start Address** | 1 - 512 |

### 5.3 Sorting

The list is sorted by default according to these rules:
1. First by fixture model (class name) in alphabetical order
2. Within the same fixture type, by natural sort of numeric portions in the label names

### 5.4 Selection Linking

Clicking on a fixture row in the list automatically selects the corresponding fixture Actor in the 3D viewport, allowing you to quickly locate it.

### 5.5 Refreshing the List

If fixtures are added to or removed from the scene while using the panel, click the **"Refresh List"** button to update the display.

---

## 6. Usage Scenarios

### Scenario 1: Quickly Checking Patch Status of All Fixtures

Open the panel to see the DMX allocation for all fixtures at a glance, quickly identifying address conflicts or omissions.

### Scenario 2: Individually Adjusting a Fixture's Address

Find the target fixture in the list and directly modify its Universe or Start Address without needing to locate and select the fixture in the viewport.

### Scenario 3: Using Alongside the Batch Patch Tool

First use the Batch Patch Tool for initial allocation, then fine-tune individual fixture addresses in the Patch Preview panel.

---

## 7. Notes

- The panel only shows SuperStage DMX fixtures, not regular UE lights
- Modifications take effect immediately but support Ctrl+Z undo
- Address conflicts are not automatically detected; ensure manually that different fixtures do not occupy overlapping channel ranges
- If there are many fixtures (hundreds), refreshing may require a brief wait
