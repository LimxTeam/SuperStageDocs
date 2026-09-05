# Effects (Frames)

> Applies to SuperStage 26H2.6 and later ｜ Read first: [Selecting and Programming](01_Programming_en.md)

SuperConsole's effect engine is called **Frame**. One Frame is made of a number of **Steps**, cycled between by a waveform, with phase distributed across the selected fixtures.

Put another way: **steps define which states to pass through, the waveform defines how to travel between them, and phase defines where in that travel each fixture starts.**

---

## 1. Steps

Each step records target values for one or more attributes:

| Field | Description |
| --- | --- |
| Attribute Name | Attribute name |
| Absolute Value | Absolute value 0–1; `-1` means not set |
| Relative Value | Relative value −1 to 1; `0` means no offset |
| Use Relative | Whether to use the relative or the absolute value |
| Width | The share of the cycle this step occupies |
| Enabled | Whether the step is active |

An absolute value means "go to this value"; a relative value means "offset the current value by this much". Use relative for position wobbles, absolute for a fixed colour sequence.

---

## 2. Per-Attribute Parameters

Within one Frame, each attribute can have its own run parameters:

| Parameter | Range | Default | Description |
| --- | --- | ---: | --- |
| Waveform | Rectangle / Sawtooth / Sine / Cosine | Sine | Rectangle steps instantly, sawtooth ramps linearly, cosine is sine offset by 90° |
| Speed | 0.1 – 600 BPM | 60 | This attribute's own speed |
| Width | 0 – 100 % | 100 | Duty cycle: how much of one cycle the waveform occupies. 10% is a short pulse, good for strobe looks |
| Attack | 0 – 100 % | 100 | The **share of time** the rise transition takes. 0 = instant, 100 = smooth throughout. **Only meaningful on the rectangle waveform** |
| Decay | 0 – 100 % | 100 | The share of time the fall transition takes, as above |
| Phase Offset | −180 – 180 ° | 0 | A phase offset for the whole attribute, on top of per-fixture phase |
| Phase Range | degrees | 360 | Total span the phase is spread over across a group |
| Wings | 1 – 32 | 1 | Number of wings; folds the selection into symmetrical groups |
| Block | 1 – 32 | 1 | Block size; one phase per block |
| Group | 1 – 32 | 1 | Number of groups |
| Random Phase | 0 – 100 % | 0 | Adds a per-fixture random offset on top of the regular phase. 0 = fully regular, 100 = fully scattered. Each fixture gets the same offset every frame, so the result is a scattered look rather than jitter |

---

## 3. Phase Distribution

How phase is spread across the selected fixtures is decided by the **selection tools** — the effect itself does not reorder anything, it hands out phase in selection order. Combined with Wings / Block / Group this produces the usual symmetrical, blocked and alternating arrangements.

---

## 4. Effect Editor Toolbar

The Frame Editor panel has a vertical strip of tool buttons down its left side, top to bottom:

| Button | Effect |
| --- | --- |
| **A+** | Creates a new step when no step is selected; merges the change into the selected steps when some are |
| **R-** | Remove steps |
| **P** | Adjust phase distribution |
| **W** | Select waveform |
| **Sp** | Speed. This page also carries **TAP** (set the speed master by tapping along to the music) and **SYNC** (make this effect follow the speed master instead of its own BPM) |
| **Wi** | Duty cycle |
| **At** | Rise transition. **Only appears on the rectangle waveform**; on every other waveform the button is hidden entirely |
| **Dc** | Fall transition. As above |
| **Wg** | Wings |
| **Bl** | Block size |
| **Gr** | Number of groups |
| **Di** | Playback direction, forward or backward |
| **Rd** | Random phase; scatters the per-fixture phase |
| **Fd** | Effect fade in and out, in seconds. The effect ramps up when it starts and settles back when released instead of cutting |
| **Ms** | Effect master: amplitude, speed multiplier and overall phase. Amplitude converges towards **what is already on stage**, so lowering it narrows the effect around the current look rather than fading to black |
| **?** | Help, listing every button and how the step bar works |

The **play / stop** button sits at the bottom of the same strip.

---

## 5. Where Effects Sit in Output Arbitration

Effects belong to the **playback layers**, at the same level as the timeline / timecode cue / cue layers:

- Intensity attributes use HTP (highest wins);
- Everything else uses LTP (source priority first, then timestamp within the same source);
- The programmer overrides effects, and highlight overrides the programmer.

See section 3 of [Cues and Playback](02_Cues_and_Playback_en.md).

---

## 6. Related Documents

- [Console Overview](00_Console_Overview_en.md)
- [Selecting and Programming](01_Programming_en.md)
- [Cues and Playback](02_Cues_and_Playback_en.md)
