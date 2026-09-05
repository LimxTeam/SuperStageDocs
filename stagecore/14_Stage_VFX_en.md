# 14 - Stage VFX (Effect Machines)

> **Modules**: SuperAssets (actors) + SuperCore (base class)
> **Applies to**: SuperStage 26H2.6 and later
> **Read first**: [03 - DMX Fixture Basics](03_DMX_Actor_Base_en.md)

---

> **Structural change in 26H2.6**: the former single generic effect-machine object was replaced by **16 concrete effect machine actors**, each binding its own Niagara system and fixture library. You no longer need to supply Niagara assets or wire up channel mappings by hand — place one, patch it, and it works.

---

## 1. Overview

Effect machines are DMX-triggered stage effect simulation objects, found in the **SuperVFX** category of the asset browser.

7 categories in 16 variants, **all single-channel**:

| Category | Variants | Count |
| --- | --- | ---: |
| Fireworks | Fireworks_A / B / C / D | 4 |
| Confetti | Confetti_Low / Medium / High / Max / Ultra | 5 |
| Smoke | Smoke_Low / High / Max | 3 |
| Fire | Fire | 1 |
| Pyro | Pyro | 1 |
| Bubble | Bubble | 1 |
| Snow | Snow | 1 |

The variants within a category are **levels** (Low / Medium / High / Max / Ultra) or **styles** (Fireworks A–D), not parameters on one actor — pick the one whose scale you need.

The same folder also holds the [stage fountain](16_Fountain_en.md) and water pool, which are not single-channel trigger devices and are documented separately.

---

## 2. Shipped Resources

Every effect machine carries a full set of resources; nothing has to be supplied by the user:

| Resource | Description |
| --- | --- |
| Niagara systems | Particle systems for each effect, 16 across 7 categories |
| Materials and textures | Particle and model materials |
| Device models | Static meshes for the machine body |
| Fixture library | `SL_<Category>_1CH` channel library (`LTC_<Variant>` is its content-browser thumbnail) |

Because the fixture libraries ship with them, effect machines join patch and console workflows like any other fixture.

---

## 3. Placing and Patching

1. Drag an effect machine from the **SuperVFX** category of the asset browser into the scene;
2. Set **Universe**, **Start Address** and **ControlMode** as for any DMX fixture, reserving 1 channel;
3. Push that channel from the console to trigger it.

Patching works exactly as for other DMX fixtures — see [Patch Tools](07_Patch_Tools_en.md).

---

## 4. DMX Control

**Single-channel triggering**: the channel value maps to effect intensity / level.

| Item | Description |
| --- | --- |
| Channel count | 1 |
| Channel meaning | Trigger intensity / spawn amount |
| Colour | **Not DMX-controlled**; determined by the effect asset itself |
| Particle detail | As above |

For timeline-based triggering, patch the effect machine and use a Sequencer DMX track — see [08 - DMX Recording and Playback](08_DMX_Recording_Playback_en.md).

---

## 5. Adjustable Parameters

The **B.DefaultParameter** group in the Details panel:

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| MaxGenerationIndex | 1.0 | 0–100 | The maximum spawn amount full DMX maps to |
| Lifetime | 1.0 | 0–100 | Particle lifetime |
| RangeIndex | 1.0 | 0–100 | Effect range |

The **C.ControlParameter** group (shown only when `ControlMode = Property`):

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| GenerationIndex | 0.0 | 0–1 | Manual trigger amount, for previewing without a console |

Effect machines derive from the axis-carrying fixture base class, so they can also be oriented by rotation; which actors actually enable axes depends on the individual implementation.

---

## 6. Checks

| Symptom | What to check |
| --- | --- |
| Pushing the channel does nothing | Is `ControlMode` set to `DMX`? Do the universe and start address match the console? |
| Only a few particles | `MaxGenerationIndex` is too low — full DMX only reaches that amount |
| Nothing happens in property mode | Adjust `GenerationIndex`, not `MaxGenerationIndex` |
| You need a bigger effect | Use a higher-level variant of the same category (e.g. Smoke_Low → Smoke_Max) rather than pushing parameters to their limits |

---

## 7. Safety Boundary

Effect machines are **visual simulation assets**. They provide no firing control, safety-distance calculation, hazardous-material management or regulatory-approval basis for real effect devices.

**Real pyrotechnics, open flame and effects must be executed by licensed professionals under local regulations.**

---

## 8. Related Documents

- [03 - DMX Fixture Basics](03_DMX_Actor_Base_en.md)
- [07 - Patch Tools](07_Patch_Tools_en.md)
- [08 - DMX Recording and Playback](08_DMX_Recording_Playback_en.md)
- [16 - Stage Fountain](16_Fountain_en.md)
