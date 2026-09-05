# Laser Overview

> Applies to SuperStage 26H2.6 and later

SuperStage's laser capabilities come in three layers, with different uses and different entry points:

| Layer | What it is | When to use it |
| --- | --- | --- |
| **Laser pattern object** | A 16-channel DMX laser fixture in the scene, running 30 built-in vector patterns | You want the console to control the laser like any other fixture |
| **Laser animation asset** | Vector laser content authored in the Content Browser: editable, bakeable, and usable in Sequencer | You are making specific laser show content |
| **External point-cloud input** | Receives network point clouds from professional laser control software | Content is authored in the external software; UE only presents it |

---

## 1. Laser Pattern Object

The scene object **LaserPattern**, in the SuperStage category of the asset browser.

- **30 procedural vector patterns**: geometry, waveforms, curves, digits and more, including Blackout;
- **16-channel DMX control**: pattern selection, size, position, rotation, colour, point density, plus the bit fields on Ch16 —

  | Bits | Function |
  | --- | --- |
  | Bit0–1 | Mirroring (X / Y) |
  | Bit2–4 | 7 global motion effects: Sweep / Pulse / Orbit / Figure-8 / Spin Pulse / Drift / Bloom |
  | Bit5–6 | Colour animation: Rainbow / Spectrum / Fire / Cycle |

- It can also run built-in animations in property mode without DMX.

Ch5/Ch6 position mapping centres on 128 with slightly different slopes either side (divided by 128 negative, by 127 positive); Ch8 stops rotation at exactly 128, with the two sides being the two directions.

---

## 2. Laser Animation Asset (new in 26H2.6)

Created in the Content Browser and opened by double-click, with a canvas editor, SVG importer, baker and Sequencer track alongside.

Two design premises worth knowing first:

**Vectors are stored, not point clouds.** The asset holds Bezier outlines and modulation parameters; the point stream is computed at playback. So it is resolution independent, editable at any time, and needs no rework when scanner parameters change.

**There is no fixed duration.** Motion comes from the frequency of the effects, and the periods of different effects are unrelated. Any point in time can be evaluated directly, so scrubbing, jumping and reverse playback need no warm-up, and "how long is this animation" is not a question that applies.

See:

- [01 Laser Animation Asset](01_LaserAnimationAsset_en.md)
- [02 Canvas Editor](02_CanvasEditor_en.md)
- [03 Effect System](03_Effects_en.md)
- [04 Baking and ILDA](04_Baking_and_ILDA_en.md)

---

## 3. External Point-Cloud Input

The scene object **LaserPro** receives network point-cloud data (UDP multicast) output by external professional laser control software.

| Item | Current value |
| --- | --- |
| Device slots | 60 by default |
| Multicast address | `239.255.{device}.{subnet}` |
| Group join | Performed on the receive thread |

Presentation capabilities:

- **HotBeam dwell enhancement**: dwell points render as constant-width beam columns with a Gaussian core and their own gain, visible from any angle;
- **ILDA pen-semantic path connection**: blank points lift the pen, dwell points at the same position collapse, and jumps between figures break the line automatically;
- **Surface hits**: line figures on walls and floors show as crisp surface lines (requires collision detection enabled); hit points render as camera-facing glow points;
- **Fans** carry pen brightness variation (slower segments and corners brighter), world-space haze and scattering phase modulation;
- **Scanner simulation is off by default** — its interpolation and inertia smoothing break HotBeam dwell semantics; it can be re-enabled in settings;
- **Mirror switches** MirrorX / MirrorY are implemented by scaling the render mesh and do not modify the laser point data;
- **Compressed point-cloud recording** and Sequencer playback (via Take Recorder).

The specific third-party laser software and versions supported are described on the official website and documentation centre.

---

## 4. Control Arbitration

One laser device can be written by three sources at once: an external network point cloud, the laser editor preview, and a Sequencer laser track. The subsystem arbitrates per device, not with one global switch.

| Priority | Source | When it writes |
| --- | --- | --- |
| High | **Sequencer** | A laser track exists on the timeline and is playing |
| Medium | **Editor Preview** | The laser editor window is open and previewing |
| Low | **Network** | Real-time point cloud from external laser software |

Rules:

- With no current owner, any source may write;
- Writing takes ownership of that device; only a source of **equal or higher priority** can take it away;
- Only the owner can release it — a lower-priority source cannot evict someone else's ownership;
- Arbitration is by priority alone, with no blending: whichever source wins takes the device's picture outright.

**Automatic release on timeout**: if the owner does not write for **one second**, the device is treated as released. This is a failure guard — a force-stopped Sequencer or a killed editor window never runs the normal release path, and without this the device would stay owned forever and the network stream would never come back.

> If the network point cloud is running but a device stays dark, check whether Sequencer or the preview owns it. Stop timeline playback, close the preview window, or wait out the one-second timeout, and the network stream comes back on its own.

---

## 5. Safety Boundary

**SuperLaser is for visual previsualisation, content authoring and data visualisation. It presents the picture of a laser show; it is not real laser output control.**

It provides no audience-scanning approval, no maximum permissible exposure (MPE) calculation, no laser zoning certification, no physical projector calibration, no emergency-stop hardware and no scan-failure protection, and it constitutes no regulatory-approval basis.

Any use with real laser systems remains subject to qualified laser operators, certified hardware, venue rules and local regulation. **Exported ILDA files must be reviewed by a licensed operator under on-site safety procedures before being fed to a real laser system.**

---

## 6. Related Documents

- [01 Laser Animation Asset](01_LaserAnimationAsset_en.md)
- [02 Canvas Editor](02_CanvasEditor_en.md)
- [03 Effect System](03_Effects_en.md)
- [04 Baking and ILDA](04_Baking_and_ILDA_en.md)
- [DMX Fixture Basics](../stagecore/03_DMX_Actor_Base_en.md)
