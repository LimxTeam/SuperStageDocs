# SuperWashComponent (Wash)

> Applies to SuperStage 26H2.6 and later

Wash is for **colour fixtures, strobes and audience blinders**. It draws no aerial column; instead it forms a bounded glow in front of the head.

---

## 1. Why No Column

A wide-angle beam's screen cost is set by the square of `length × tan(half angle)`, not by the angle itself. Drawn as a cone, one 70° fixture can smear across half the screen.

Wash **locks both quantities on the component**: neither length nor spread follows the fixture's maximum throw distance or its zoom, so a wide-angle fixture cannot push the cost up as it zooms.

Its angle is fixed on the component and **does not respond to zoom**.

---

## 2. Floor Pool

Wash draws no column but it **still lights normally**: the floor pool comes from the [cone light](10_SuperConeLightComponent_en.md). So a wash fixture still has a normal pool, colour and falloff on the floor.

---

## 3. When You Will Meet It

| Situation | Description |
| --- | --- |
| Building a fixture by hand | Choose `Wash (Color / Blinder / Strobe)` on the emitter in the fixture definition |
| GDTF import | Groups declared `BeamType=Glow/None` are assigned Wash automatically — these are aura rings and backplates, and giving them columns would be wrong (a fixture with 25 aura rings would emit 25 beams) |

---

## 4. Parameters

Wash's own four parameters sit under **B.BeamParameter** in the details panel. The same group also carries the beam parameters inherited from the ray beam — Wash **does** use colour, density and haze noise from those; it only overrides the **shape** (length and spread) with its own.

| Parameter | Default | Range | Description |
| --- | ---: | --- | --- |
| **Wash Length** | 1000 cm | 50 ~ 6000 (slider to 2000) | Beam length. **This is the type's performance pressure point** — screen cost is proportional to (length × tan half-angle)², so at 70° every extra metre grows the covered area quadratically. A wash fixture does not throw far anyway and ten metres is usually plenty; the longer it gets, the faster the screen area this one fixture covers grows |
| **Wash Spread** | 70° | 20 ~ 160 | Spread (full angle). Fixed on the component and **does not respond to zoom** |
| **Wash Far Fade** | 0.6 | 0 ~ 1 | Far-end fade fraction. Set far higher than a normal beam (0.12) — a ten-metre wide cone cut off hard at the end reads as fake instantly, and the "faint layer of haze" look comes mostly from here |
| **Use Ellipsoid Volume** | off | — | Switch to an ellipsoid volume. See below |

**The length is not taken from the fixture's maximum throw distance.** That value is routinely fifty metres, which for a 70° cone means one fixture smearing across the whole screen; locking the length locks the cost.

Zoom, frost and iris are no-ops on Wash — its spread is a property of the model, not a channel.

### 4.1 Cone or Ellipsoid

The default is a **cone**: its screen cost is proportional to (length × tan half-angle)², held down by the locked length.

**Use Ellipsoid Volume** switches to an ellipsoid, which is fully bounded and independent of the spread angle. The trade is the look: a glow around the head rather than something that opens out from the lens. That is why it is off by default — it is **the fallback for the day a rig has a thousand wash fixtures and locking the length is no longer enough**.

> **Beam Occlusion has no effect on Wash**, the same as for the ray beam.

### 4.2 Relationship to the Cone Light

Wash keeps its lens mesh and switches the in-house cone light on — the opposite of the ray beam (which strips both because a pixel matrix puts hundreds of emitters on one fixture; wash fixtures are few and large, and the glass at the lens is their most recognisable feature).

The cone light's **Outer Cone Angle** stays in sync with **Wash Spread** automatically. All illumination goes through the [cone light](10_SuperConeLightComponent_en.md) and **creates no UE light source**.

---

## 5. Related Documents

- [Light Components](00_LightComponent_Overview_en.md)
- [SuperRayBeamComponent](13_SuperRayBeamComponent_en.md)
- [SuperConeLightComponent](10_SuperConeLightComponent_en.md)
- [The Fixture Definition Asset](../fixture/01_FixtureDefinition_en.md)
