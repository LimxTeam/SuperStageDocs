# 16 - Stage Fountain

> **Module**: SuperCore ｜ **Applies to**: SuperStage 26H2.6 and later
> **Read first**: [03 - DMX Fixture Basics](03_DMX_Actor_Base_en.md)

---

## 1. Overview

**SuperFountain** is the stage fountain system added in 26H2.6. It is controlled over DMX and shares the same optics as the lighting.

It consists of two independent objects:

| Object | Nature | Description |
| --- | --- | --- |
| **Super Fountain** | DMX device (14 channels) | Moving fountain; nozzles are components on it, with an underwater light |
| **Super Water Pool** | Scenic object | Water surface and ripples |

> **Neither depends on the other.** If you only want a pool, place a pool — there is no need to place a fountain first. Conversely, **the fountain device does not carry a water surface of its own**: with no pool object in the scene, no water surface is rendered.

Both are in the **SuperVFX** category of the asset browser.

---

## 2. Placing and Basic Setup

1. Drag **Super Fountain** from the asset browser into the scene;
2. If you want a water surface, place a **Super Water Pool** as well and adjust its position and size;
3. Set **Universe**, **Start Address** and **ControlMode** as for any DMX fixture, reserving 14 channels;
4. To preview by hand in the Details panel, set `ControlMode` to `Property` and use the Manual fields.

---

## 3. DMX Channels (14CH)

| Channel | Function | Notes |
| ---: | --- | --- |
| 1–2 | Pan | 16-bit |
| 3–4 | Tilt | 16-bit |
| 5 | Pan / Tilt speed | The slowest step is 8% of full speed, the same convention as fixtures |
| 6 | Jet height | |
| 7 | Spread angle | Mapped to 0–20° |
| 8 | Underwater light intensity | |
| 9 | Underwater light strobe | 0–15 constant; 16–255 mapped linearly to 1–25 Hz |
| 10 | Underwater light red | |
| 11 | Underwater light green | |
| 12 | Underwater light blue | |
| 13 | Underwater light white | |
| 14 | Underwater light zoom | Mapped to a 10–60° outer angle |

> The physical limit of the spread angle is 45°, but a fountain past 20° is already mist, so the DMX travel only goes to 20°.

---

## 4. Fountain Parameters

### 4.1 Axes

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| Axis Max Speed | 60 | 0–720 deg/s | Maximum angular speed. DMX gives a target; the actual angle approaches it under this limit |
| Manual Pan | 0.5 | 0–1 | Manual pan in property mode |
| Manual Tilt | 0.5 | 0–1 | Manual tilt in property mode |
| Manual PT Speed | 0.0 | 0–1 | Manual speed in property mode |
| Manual Strobe | 0.0 | 0–25 Hz | Manual strobe in property mode |

### 4.2 Nozzle

Nozzle parameters are calibrated against manufacturer samples; exit velocity is derived from pressure.

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| Nozzle Diameter | 20 mm | 1–200 | Nozzle bore |
| Pump Pressure | 0.2 MPa | 0.01–2.0 | Pump pressure. 0.2 MPa gives about 20 m/s exit velocity |
| Discharge Coeff | 0.95 | 0.5–1.0 | Discharge coefficient |
| Velocity Coeff | 0.97 | 0.5–1.0 | Velocity coefficient |
| Max Coherent Length | 12 m | 0.5–40 | Maximum coherent jet length; breakup starts past it |
| Dispersion | 4.5° | 0–45 | Dispersion half-angle |
| Lifetime | 6 s | 0.2–30 | Droplet lifetime |
| Flow | 1.0 | 0–1 | Flow |
| Parcel Volume | 1.0 cm³ | 0.02–100 | Parcel volume |

### 4.3 Underwater Light

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| Dimmer | 1.0 | 0–1 | Intensity |
| Light Color | White | — | Colour |
| Light Cone | 40° | 1–89 | Outer angle |
| Light Range | 25 m | 0.5–200 | Throw distance |
| Intensity | 1.0 | — | Intensity multiplier |

The underwater light is in candela, the same convention as the projectors.

### 4.4 Mist

| Parameter | Default | Range |
| --- | ---: | --- |
| Mist Amount | 0.02 | 0–0.3 |
| Mist Density | 0.02 | 0–1 |
| Mist Rise Fraction | 0.33 | 0–2 |
| Mist Entrain Time | 1.0 s | 0.05–10 |
| Mist Lifetime | 4.0 s | 0.1–30 |
| Mist Radius Start | 5 cm | 0.5–200 |
| Mist Radius End | 90 cm | 1–1000 |
| Mist Turbulence | 70 | 0–500 |
| Mist Decay | 5.0 | 0–12 |

Scattering from stage lights on the water mist uses the same phase function as the beams.

---

## 5. Water Pool Parameters

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| Surface Size | 3000×3000 cm | 50–100000 | Water surface size |
| Water Depth | 60 cm | 1–2000 | Depth; affects refraction and colour |
| Refract Strength | 40 | 0–200 | Refraction strength |
| Reflect Strength | 1.0 | 0–1 | Reflection strength |
| Ripple Normal Scale | 0.15 | 0–2 | Ripple normal strength |
| Base Chop | 0.15 | 0–1 | Base chop |
| Max Wave Height | 8 cm | 0.1–100 | Maximum wave height |
| Foam Threshold | 0.35 | 0–2 | Foam threshold |
| Foam Amount | 0.8 | 0–1 | Foam amount |
| Sim Resolution | 512 | 64–2048 | Ripple height field resolution |

---

## 6. Render Layers

The fountain image is built from five layers:

| Layer | Content |
| --- | --- |
| Coherent jet | Analytic ballistics plus breakup length |
| Breakup droplets | GPU particles |
| Aerated white water | The white section after the jet breaks up |
| Pool ripples | Height field |
| Water surface | Analytic intersection; no mesh generated |

The motion blur shutter follows frame duration by default, so a change in frame rate does not smear droplets into continuous streaks.

All parameters are adjustable from both the Details panel and console variables.

---

## 7. Common Questions

### No water surface

Is there a **Super Water Pool** in the scene? The fountain device does not carry a water surface of its own.

### The jet is too low / too high

Start with Pump Pressure and Nozzle Diameter — those two set the exit velocity. The DMX jet height channel maps on top of that physical basis.

### The underwater light has no colour

Confirm DMX channels 10–13 carry values and that ControlMode is `DMX`. In property mode the Manual fields cover only axes and strobe; colour needs DMX or the Light Color nozzle parameter.

### Frame rate drops noticeably

Sim Resolution (the ripple height field) and the mist parameters are the main costs. Try dropping Sim Resolution to 256 first.

---

## 8. Tuning and Diagnostic Switches

The fountain has more than fifty console variables; nearly all of them are internal calibration values and should be left alone. The ones below are the ones you would actually reach for. They take effect at runtime.

### 8.1 Performance

| Variable | Default | What it does |
| --- | ---: | --- |
| `r.SuperFountain` | 1 | Fountain master switch. 0 = off, 1 = on |
| `r.SuperFountain.HalfRes` | 0 | Half-resolution droplet accumulation. 1 = half res. Water is a full-screen bandwidth hog, so **this is the first switch to reach for when frame rate drops** |
| `r.SuperFountain.MaxDrawDistance` | 30000 | Upper bound on droplet draw distance (cm, 0 = unlimited) |
| `r.SuperFountain.MaxParticles` | 2000000 | Particle pool capacity. Each parcel is 32 bytes and double-buffered, so 2 million is about 128 MB of VRAM. **Changing it rebuilds the buffers** |
| `r.SuperFountain.MaxLights` | 256 | Maximum cone lights fed into fountain lighting per frame (underwater plus external). Beyond that, lights are dropped in registration order with a warning. Lights out of range cost only a 16-byte read, so this number can be larger than instinct suggests |

### 8.2 Look

| Variable | Default | What it does |
| --- | ---: | --- |
| `r.SuperFountain.Ambient` | 0.3 | Ambient multiplier, applied to the world sky light intensity |
| `r.SuperFountain.MaxRadiance` | 6.0 | Hue-preserving radiance ceiling (0 = unlimited) |
| `r.SuperFountain.ExtinctionScale` | 1.0 | Optical thickness calibration — this is the occlusion strength knob |

### 8.3 Diagnostics

| Variable | Default | What it does |
| --- | ---: | --- |
| `r.SuperFountain.Freeze` | 0 | Freezes the simulation but keeps drawing. 1 = frozen, for inspecting one frame's particle distribution |
| `r.SuperFountain.WaterDebug` | 0 | Water surface debug. 1 = height field heat map (blue = dip / red = bump / flat grey = the impact did not register) |

---

## 9. Safety Boundary

The fountain is a **visual simulation asset**. It provides no pump control, electrical control, water treatment, shock protection or regulatory-approval basis for a real fountain. Real water features must be executed by licensed professionals under local regulations.

---

## 10. Related Documents

- [03 - DMX Fixture Basics](03_DMX_Actor_Base_en.md)
- [07 - Patch Tools](07_Patch_Tools_en.md)
- [14 - Stage VFX](14_Stage_VFX_en.md)
