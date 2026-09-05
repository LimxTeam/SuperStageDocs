# 13 - LED Strip Effect

> **Module**: SuperAssets  
> **Target Users**: Lighting designers and stage technicians  
> **Prerequisites**: [03 - DMX Actor Basics](03_DMX_Actor_Base_en.md)  
> **Last Verified**: 2026-06-28

---

## 1. Overview

**SuperLightStripEffect** is a 9-channel DMX Actor for strip-style emissive material control in the **SuperAssets** module inside the SuperStage plugin.

It creates its own mesh component and can also apply one shared dynamic material instance to multiple `StaticMeshActor` targets in the level. One set of DMX channels can therefore drive brightness, strobe, color, and effect material parameters across a group of strip meshes.

---

## 2. Placement And Model Binding

1. Place **SuperLightStripEffect** from the SuperStage asset list.
2. To show the effect on the Actor itself, assign a static mesh to **StaticMeshEffect**.
3. To control existing meshes in the level, add `StaticMeshActor` targets to **TargetMeshActors**.
4. Set **MaterialIndex** to choose which material slot is replaced.
5. Set **Universe** and **Start Address**, reserving 9 channels.

The editor also provides a context-menu binding workflow. Select static mesh Actors, then use the SuperStage context menu to append them to a LightStripEffect target list. The tool removes duplicates and refreshes the material after binding.

---

## 3. Editable Parameters

| Parameter | Category | Default | Range | Description |
| --- | --- | ---: | --- | --- |
| **TargetMeshActors** | A.ModelTargets | Empty | — | `StaticMeshActor` targets that receive the strip material. **Editable on a placed instance only**, not on class defaults |
| **StaticMeshEffect** | A.ModelTargets | Empty | — | Static mesh used by the Actor's own mesh component |
| **MaxLightIntensity** | B.DefaultParameter | 1.0 | 0–1000 | Written to the material parameter `MaxBrightness` |
| **MaterialIndex** | B.DefaultParameter | 0 | 0–255 | Material slot index. The tool checks that the slot exists before assigning the material |
| **Dimmer** | C.ControlParameter | 0.0 | 0–1 | Normalized brightness value |
| **Strobe** | C.ControlParameter | 0.0 | 0–1 | Normalized strobe speed value |
| **Effect** | C.ControlParameter | 0.0 | 0–1 | Written to `Effect` after mapping to 0–10 |
| **Speed** | C.ControlParameter | **0.5** | 0–1 | Written to `Speed` after mapping to −10–10. **0.5 is stationary** |
| **Width** | C.ControlParameter | 0.0 | 0–1 | Written to `Width` after mapping to 0–10 |
| **Direction** | C.ControlParameter | 0.0 | 0–1 | Written to `EffectDirection` |
| **Color** | C.ControlParameter | White | — | Written to `LightColor` |

> **Use DMX mode for normal operation.** The `C.ControlParameter` group is only shown when ControlMode is `Property`, and **in Property mode the current implementation writes nothing to the material**: the read-and-apply step returns immediately. (The one exception is the strobe: the per-frame brightness recalculation does not check the control mode, but the `Dimmer` it multiplies is still whatever DMX last wrote.)

---

## 4. DMX Channels

The Actor asset data marks this fixture as **9CH**, and it reads these coarse attributes:

| Channel | Attribute | Material / Behavior |
| ---: | --- | --- |
| 1 | `Dimmer` | Used when writing `Brightness`. |
| 2 | `Strobe` | Enables the strobe multiplier when above 0, affecting `Brightness`. |
| 3 | `Effect` | Written to `Effect` as `Lerp(0, 10, Effect)`. |
| 4 | `Speed` | Written to `Speed` as `Lerp(-10, 10, Speed)`. |
| 5 | `Width` | Written to `Width` as `Lerp(0, 10, Width)`. |
| 6 | `Direction` | Written to `EffectDirection`. |
| 7 | `Red` | RGB red component. |
| 8 | `Green` | RGB green component. |
| 9 | `Blue` | RGB blue component. |

The actual visual result is defined by `/SuperStage/SuperCore/LightMaterial/MainMaterial/M_Effect`. The current implementation writes the parameters above; this manual does not promise fixed effect names or a fixed visual list.

---

## 5. Strobe Behavior

The current implementation no longer writes `Strobe` and `StrobeMode` into the material. Strobe is calculated by the fixture logic:

- `Strobe > 0` enables strobing.
- The cached strobe speed is `Strobe * 255`.
- Each frame calculates a linear triangle-wave multiplier with smoothing.
- The material receives `Brightness = Dimmer * StrobeMultiplier`.

For that reason, this manual no longer describes multiple strobe modes. The visible result depends on `Dimmer`, `Strobe`, frame rate, and material response.

---

## 6. FAQ

### Target meshes do not show the strip material

Check these items:

- The target is a `StaticMeshActor`.
- `MaterialIndex` is within the target mesh's actual material slot range.
- `MaxLightIntensity` is above 0.
- DMX reaches the configured Universe and 9-channel range.
- The strip material has been refreshed; context-menu binding refreshes it, while other workflows may require reinitializing the fixture.

### Can multiple meshes be controlled independently?

One SuperLightStripEffect applies one shared dynamic material instance to all targets, so those targets change together. For independent control, place multiple SuperLightStripEffect Actors and give them different addresses.

### Where are effect names configured?

The current implementation only writes the numeric `Effect` value to the material parameter. The visual meaning of 0-10 is defined by the `M_Effect` material, not by a fixed table in the Actor.

---

## 7. Related Documents

- [03 - DMX Actor Basics](03_DMX_Actor_Base_en.md)
- [07 - Patch Tools](07_Patch_Tools_en.md)
- [10 - Stage Machinery](10_Stage_Machinery_en.md)
