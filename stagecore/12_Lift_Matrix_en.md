# 12 - Lift Matrix

> **Module**: SuperAssets  
> **Target Users**: Lighting designers and stage technicians  
> **Prerequisites**: [00 - DMX System Overview](00_DMX_System_Overview_en.md), [03 - DMX Actor Basics](03_DMX_Actor_Base_en.md)  
> **Last Verified**: 2026-06-28

---

## 1. Overview

**LiftMatrix** is a 42-channel light machinery fixture Actor. Its display name is **LiftMatrix**, and it is included in the **SuperAssets** module inside the SuperStage plugin.

It creates 5 lift layers. Each layer contains 2 effect components, for a total of 10 light-emitting units. DMX or property control drives the lift position, per-unit effect selection, and per-unit RGB color.

> Note: This manual does not describe LiftMatrix as a moving light with Pan/Tilt control.

---

## 2. Placement And Basic Setup

1. Find **LiftMatrix** in the SuperStage asset or fixture list.
2. Place it in the level, then set **Universe**, **Start Address**, and **ControlMode** like any other DMX fixture.
3. To drive it from DMX, set `ControlMode` to DMX and make sure the fixture library asset is available.
4. To preview it from the Details panel, set `ControlMode` to Property and adjust **PosZ**.

The default fixture library path is:

`/SuperStage/SuperCore/SuperLight/Machinery/LiftMatrix/SL_Machinery_LiftMatrix`

---

## 3. Editable Parameters

| Parameter | Category | Default | Description |
| --- | --- | ---: | --- |
| **PosZ** | C.ControlParameter | 0.0 | Normalized lift amount. In Property mode, this value directly drives the lift movement. |
| **LiftRange** | B.DefaultParameter | 500.0 cm | Deployment range. Larger values spread the layers farther apart at full deployment. |
| **MaxIntensity** | B.DefaultParameter | 5.0 | Brightness limit passed to the effect components. |

In DMX mode, `PosZ` is read from the `PosZ` attribute as a 16-bit value. In Property mode, the Details panel value is used by the lift calculation.

---

## 4. Structure And Motion

LiftMatrix currently creates a fixed structure:

| Item | Count | Description |
| --- | ---: | --- |
| Lift layers | 5 | `LiftComponent_0` through `LiftComponent_4`. |
| Effect components | 10 | Two effect components per lift layer. Group A uses `SM_Effect`; group B uses `SM_Matrix`. |
| Cables | 4 | UE cylinder meshes placed at the four corners. |

The lift positions are calculated across 6 divisions. The 5 lift layers occupy divisions 1 through 5, leaving the top division empty:

```text
Layer i, i = 0..4
Retracted position = initial offset - 5 cm * i
Expanded position = initial offset - LiftRange * (i + 1) / 6
Final position = Lerp(retracted position, expanded position, PosZ)
```

So `PosZ = 0` is close to the retracted state, while `PosZ = 1` deploys the layers according to `LiftRange`. Cable length is updated from the lowest layer position.

---

## 5. DMX Channel Behavior

The Actor asset data marks this fixture as **42CH**. The source reads these main attributes:

| Attribute | Purpose | Notes |
| --- | --- | --- |
| `PosZ` | Lift | Read as 16-bit data and used to deploy or retract all 5 layers. |
| `Effect` | Effect selection | Matrix-read into the 10 effect components and mapped through the effect LUT. |
| `Red1` / `Green1` / `Blue1` | Color | Matrix RGB-read into the 10 effect components. |

The 42-channel count matches the current implementation as 2 channels for `PosZ`, plus 10 light-emitting units with `Effect`, `Red`, `Green`, and `Blue` channels. The exact address order is defined by the fixture library asset.

---

## 6. Usage Notes

- Reserve the full 42-channel range from the fixture `Start Address`.
- `LiftRange` is measured in centimeters; high values make the model deploy over a much longer distance.
- If the panels do not show color or effects, first confirm that DMX data reaches the configured Universe and address, then check that the fixture library asset is loaded.
- The number of layers and light-emitting units is fixed in the current implementation; there is no layer-count setting in the user interface.

---

## 7. Related Documents

- [03 - DMX Actor Basics](03_DMX_Actor_Base_en.md)
- [07 - Patch Tools](07_Patch_Tools_en.md)
- [10 - Stage Machinery](10_Stage_Machinery_en.md)
