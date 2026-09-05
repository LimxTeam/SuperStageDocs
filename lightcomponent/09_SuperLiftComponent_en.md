# SuperLiftComponent

SuperLiftComponent provides simple Z-axis lift movement. It takes a normalised position, a lift range and a speed, then interpolates the component's **relative Z** towards the target.

## Parameters

| Parameter | Purpose |
| --- | --- |
| InPosZ | Normalised position, clamped to 0–1. |
| LiftRange | Travel length. The target is `LiftRange × InPosZ`, measured **from the component's relative origin**, not from the height it was placed at. |
| LiftSpeed | Interpolation speed. |

## Notes

- The component does not update itself automatically by Tick. It is normally driven by a parent fixture or machinery asset.
- It only controls relative Z position. It does not handle rigging safety, cable physics, or real machinery feedback.
- If it does not move, check whether the parent asset continuously passes control values at runtime.
