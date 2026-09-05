# Light Components

> Applies to SuperStage 26H2.6 and later

This folder describes the emissive, beam, effect and matrix components found inside SuperStage fixtures. It is aimed at users working with fixtures and inspecting parameters, not at C++ or Blueprint developers.

Users do not normally add these components by hand. For data-driven fixtures the components are decided by the **emitters in the fixture definition**: the beam kind chosen on an emitter decides which component is created at runtime. See section 4 of [The Fixture Definition Asset](../fixture/01_FixtureDefinition_en.md).

---

## 1. Inheritance

```text
UE scene component
├── SuperLightingComponent           Base: lens, intensity, colour, strobe
│   └── SuperConeLightComponent      In-house cone light (replaces UE SpotLight)
│       ├── SuperSpotComponent       Spot
│       │   ├── SuperVolumetricBeamComponent      Volumetric beam
│       │   │   └── SuperVolumetricShaperComponent  Volumetric framing
│       │   └── SuperBeamComponent   Material beam (Legacy)
│       │       └── SuperShaperComponent            Material framing (Legacy)
│       └── SuperRayBeamComponent    Ray beam
│           └── SuperWashComponent   Wash
│
├── SuperEffectComponent             Effect plane
├── SuperMatrixComponent             Pixel matrix
└── SuperLiftComponent               Z-axis lift
```

> The last three are not lighting-component derivatives: the effect plane, the pixel matrix and the lift each hang directly off a scene component, each doing one job, outside the lens-and-cone chain.

---

## 2. Visible Capabilities

| Component | Purpose | What the user sees | Document |
| --- | --- | --- | --- |
| SuperLightingComponent | Base lens and colour/intensity control | Intensity, colour, strobe, lens material changes | [01](01_SuperLightingComponent_en.md) |
| SuperConeLightComponent | Key light projection | Floor pool, gobos, colour wheels, framing, prism | [10](10_SuperConeLightComponent_en.md) |
| SuperSpotComponent | Spot control | The effect of Zoom, Frost and Iris on cone angle and softening | [02](02_SuperSpotComponent_en.md) |
| SuperVolumetricBeamComponent | Volumetric column | Aerial beam, haze, occlusion by real geometry | [11](11_SuperVolumetricBeamComponent_en.md) |
| SuperVolumetricShaperComponent | Volumetric framing | Volumetric beam plus four-blade framing | [12](12_SuperVolumetricShaperComponent_en.md) |
| SuperRayBeamComponent | Ray beam | Column with no optical elements, for matrix and effect fixtures | [13](13_SuperRayBeamComponent_en.md) |
| SuperWashComponent | Wash glow | A bounded glow in front of the head, no column | [14](14_SuperWashComponent_en.md) |
| SuperBeamComponent | Material beam (Legacy) | Aerial beam, gobo, colour wheel, prism | [03](03_SuperBeamComponent_en.md) |
| SuperShaperComponent | Material framing (Legacy) | Four-blade framing shape and rotated pool | [04](04_SuperShaperComponent_en.md) |
| SuperEffectComponent | Light strip / effect plane material | Effect number, direction, speed, width, colour and strobe | [06](06_SuperEffectComponent_en.md) |
| SuperMatrixComponent | Segmented matrix | Per-segment colour, matrix intensity, strobe | [07](07_SuperMatrixComponent_en.md) |
| SuperLiftComponent | Z-axis lift | Movement within the lift range by normalised input | [09](09_SuperLiftComponent_en.md) |

---

## 3. What Changed in This Release

| Change | Description |
| --- | --- |
| **In-house cone light added** | `SuperConeLightComponent` replaces the UE SpotLight, saving one engine light and one light function material instance per fixture |
| **Volumetric beams became primary** | The shipped library is fully migrated. Fixtures newly imported from GDTF still default to the material beam and must be switched by hand |
| **Ray beam and Wash added** | Ray beam for matrix / effect fixtures, Wash for colour / blinder / strobe |
| **Cutting renamed to Shaper** | `SuperCuttingComponent` → `SuperShaperComponent`, with redirectors so existing assets are unaffected |
| **Rect component removed** | The rectangular area light component is gone; surface lighting is handled by the cone light |
| **Bodies out of physics and navigation** | Fixture meshes no longer generate collision, overlap events or navigation data; editor-only primitives such as the orientation arrows and the address label were removed along with their scene proxies. The hook is still there — it is part of the body model |

> If you are reading documentation from before 26H2.6: `SuperCuttingComponent` and `SuperRectComponent` no longer exist in this release.

---

## 4. What Users Should Check

- Whether the corresponding emitter in the fixture definition carries this capability (Zoom, Frost, Gobo, Prism, Matrix, Lift and so on);
- Whether the channel definitions in the channel library match the console's output;
- Whether materials, atlases, prism presets and other resources exist and are bound to the definition;
- In large scenes, the final result of many simultaneous volumetric beams depends on UE render settings and hardware.

---

## 5. Not a Promise

These components do not guarantee that every fixture has the same functions. What a fixture can actually do depends on its fixture definition, channel library, bindings, material resources and project settings.
