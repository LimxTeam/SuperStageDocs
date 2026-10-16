# Asset List

> Applies to SuperStage 26H2.6 | 869 placeable assets in total

All objects available to place in a scene through the Asset Browser in this version. The list contains two distinct types of content:

- **Fixture Library — 806 models from 54 manufacturers.** Manufacturer fixtures built from GDTF data. Each is a fixture definition asset that drives the same fixture Actor, so it can be opened in the Fixture Editor and reimported or upgraded using an updated GDTF package.
- **Built-in — 63 items.** Objects included with the plugin: effects machines, procedural stage structures, in-house fixtures, and system objects. **Most are not lighting fixtures**, and those that are fixtures do not correspond to any manufacturer model.

Channel counts are determined by the channel libraries shipped with the plugin. Before patching, use the [Patch tools](https://www.yunsio.com/docs/stagecore/patch-tools) to confirm the actual channel footprint.

---

## Built-in Objects

### SuperStage — System Objects (11)

These belong to the tool itself rather than a manufacturer's fixture catalog: the fixture Actor used by the entire fixture library, media sources, laser devices, machinery, and rendering utilities. Some use DMX; others do not.

| Model | C++ Class Name | DMX | Description |
| --- | --- | ---: | --- |
| Data-Driven Fixture | `ASuperFixtureActor` | — | The fixture Actor used by the entire fixture library and every GDTF import |
| DirectorCamera | `ASuperDirectorCamera` | — | An in-scene camera that supplies live images to screens and projectors |
| LaserPattern | `ASuperLaserPatternActor` | 16CH | A laser fixture with 30 built-in vector patterns, controllable from a console |
| LaserPro | `ASuperLaserProActor` | — | Receives point clouds transmitted by external laser software over the network (UDP multicast) |
| LiftingMachinery | `ASuperLiftingMachinery` | 12CH | DMX-controlled lifting machinery |
| LightStrip | `ASuperLightStripEffect` | 9CH | An LED strip effect; a single patch can drive multiple meshes in the level |
| Madrix | `ASuperMadrixActor` | — | Maps the live MADRIX preview output onto meshes, retrieved over HTTP rather than DMX |
| Projector | `ASuperProjector` | — | Projection-mapping previs that projects a media source as light onto scene geometry |
| RailMachinery | `ASuperRailMachinery` | 14CH | DMX-controlled rail machinery |
| Screen | `ASuperScreen` | — | Displays a media source on one or more meshes |
| WhiteModel | `ASuperWhiteModelActor` | — | A post-process object that renders the entire scene as a white model when placed; deleting it restores normal rendering |

[Full documentation →](https://www.yunsio.com/docs/stageassets/stage-devices)

### SuperLight — In-house Fixtures (16)

Hand-coded C++ fixtures across seven series. These are actual fixtures and are patched in the same way as other fixtures, but represent generic stage equipment rather than replicas of specific manufacturer models. They therefore do not appear in the Fixture Editor and cannot be reimported from GDTF.

| Model | C++ Class Name | DMX | Description |
| --- | --- | ---: | --- |
| Aurora_Fan200 | `AAurora_Fan200` | 8CH | Fan effect (small) |
| Aurora_Fan400 | `AAurora_Fan400` | 8CH | Fan effect (large) |
| BlazeAtomic3K | `ABlazeAtomic3K` | 5CH | Strobe |
| BlazeMatrixStrip20 | `ABlazeMatrixStrip20` | 63CH | Matrix strobe bar (20 segments) |
| BlazeStrip1000 | `ABlazeStrip1000` | 5CH | Strobe bar |
| Flare_RGBW600 | `AFlare_RGBW600` | 7CH | RGBW wash PAR |
| Flare_WashMH600 | `AFlare_WashMH600` | 11CH | Moving-head wash |
| Hyperion550Beam | `AHyperion550Beam` | 15CH | Beam fixture |
| Hyperion700Profile | `AHyperion700Profile` | 25CH | Profile fixture |
| LiftBall | `AMachinery_LiftBall` | 9CH | Kinetic lifting ball |
| LiftMatrix | `AMachinery_LiftMatrix` | 42CH | Kinetic lifting light matrix |
| Spark_P1Evo | `ASpark_P1Evo` | 3CH | Film and television light |
| Spark_Profile600 | `ASpark_Profile600` | 15CH | Film and television profile light |
| Spark_V1 | `ASpark_V1` | 5CH | Film and television light |
| Spark_Vintage300 | `ASpark_Vintage300` | 4CH | Vintage film and television light |
| Thunder_Sidewinder10 | `AThunder_Sidewinder10` | 53CH | Beam bar (10 heads) |

[Full documentation →](https://www.yunsio.com/docs/stagecore/in-house-fixtures)

### SuperVFX — Effects Machines and Fountains (18)

DMX-triggered stage effects simulations. All 16 effects machines use a single trigger channel. Multiple variants within a type represent different levels or styles, rather than parameters on the same Actor. Fountains and water pools share this category but are independent of each other.

| Model | C++ Class Name | DMX | Description |
| --- | --- | ---: | --- |
| Bubble | `ASuperVFX_Bubble` | 1CH | Bubble machine |
| Confetti_High | `ASuperVFX_Confetti_High` | 1CH | Confetti, level 3 of 5 |
| Confetti_Low | `ASuperVFX_Confetti_Low` | 1CH | Confetti, level 1 of 5 |
| Confetti_Max | `ASuperVFX_Confetti_Max` | 1CH | Confetti, level 4 of 5 |
| Confetti_Medium | `ASuperVFX_Confetti_Medium` | 1CH | Confetti, level 2 of 5 |
| Confetti_Ultra | `ASuperVFX_Confetti_Ultra` | 1CH | Confetti, level 5 of 5 |
| Fire | `ASuperVFX_Fire` | 1CH | Flame effect |
| Fireworks_A | `ASuperVFX_Fireworks_A` | 1CH | Fireworks, style A |
| Fireworks_B | `ASuperVFX_Fireworks_B` | 1CH | Fireworks, style B |
| Fireworks_C | `ASuperVFX_Fireworks_C` | 1CH | Fireworks, style C |
| Fireworks_D | `ASuperVFX_Fireworks_D` | 1CH | Fireworks, style D |
| Pyro | `ASuperVFX_Pyro` | 1CH | Pyrotechnic / cold-spark effect |
| Smoke_High | `ASuperVFX_Smoke_High` | 1CH | Smoke, level 2 of 3 |
| Smoke_Low | `ASuperVFX_Smoke_Low` | 1CH | Smoke, level 1 of 3 |
| Smoke_Max | `ASuperVFX_Smoke_Max` | 1CH | Smoke, level 3 of 3 |
| Snow | `ASuperVFX_Snow` | 1CH | Falling snow effect |
| Fountain | `ASuperFountain` | 14CH | A movable fountain with nozzles attached as components and underwater lights |
| WaterPool | `ASuperWaterPool` | — | Water surface and ripples. A scenery object with no DMX; it does not require a fountain to be placed first |

[Full documentation →](https://www.yunsio.com/docs/stagecore/stage-vfx)

### StageModel — Procedural Stage Structures (18)

Stage structures generated from parameters or splines, rebuilt whenever their parameters change. They do not use DMX and are not fixtures. Their weight, load, and sightline values are reference estimates based on simplified models, not structural safety assessments.

| Model | C++ Class Name | DMX | Description |
| --- | --- | ---: | --- |
| Ballast | `ASuperBallast` | — | Ballast placement |
| Barrier | `ASuperBarrier` | — | Crowd barriers / site fencing drawn along a spline |
| CableRun | `ASuperCableRun` | — | Cable routing drawn along a spline |
| CircularTruss | `ASuperCircularTruss` | — | Circular / double-ring truss |
| Crowd V1 | `ASuperCrowdV1` | — | Static crowds inside a closed spline area |
| Crowd V2 | `ASuperCrowdV2` | — | Static crowds, second generation |
| CurvedScaffold | `ASuperCurvedScaffold` | — | Spline-driven curved scaffolding |
| CurvedTruss | `ASuperCurvedTruss` | — | Curved truss generated along a spline path |
| Drape | `ASuperDrape` | — | Stage drapes |
| Grandstand | `ASuperGrandstand` | — | Tiered grandstand |
| Scaffold | `ASuperScaffold` | — | Scaffolding |
| StageFloor | `ASuperStageFloor` | — | Stage floor: deck, legs, diagonal braces, skirting, and steps |
| StageRoof | `ASuperStageRoof` | — | Stage roof system |
| StairTower | `ASuperStairTower` | — | Stair tower |
| Trackway | `ASuperTrackway` | — | Ground panels / walkways drawn along a spline |
| TrussGantry | `ASuperTruss` | — | Trusses and gantries |
| TrussGrid | `ASuperTrussGrid` | — | Horizontal truss grid |
| TrussTower | `ASuperTrussTower` | — | Vertical truss tower |

[Full documentation →](https://www.yunsio.com/docs/stageassets)

---

## Fixture Library

806 models from 54 manufacturers, under the **StageLight** category in the [Asset Browser](https://www.yunsio.com/docs/editortools/asset-browser), all running on the same data-driven fixture Actor.

If the fixture you need is not included, you can [import from GDTF](https://www.yunsio.com/docs/fixture/gdtf-import), either as a single package or an entire folder. If no GDTF file is available, [build it manually](https://www.yunsio.com/docs/fixture/fixture-builder).

### Acme(38)

| # | Model |
| ---: | --- |
| 39 | ACME_AECO_5_AECO_5 |
| 40 | ACME_EYEFLASH_2_BLINDER_2_RGB |
| 41 | ACME_GEIST_BSWF_XA-500_BSWF |
| 42 | ACME_LYRA_XA_1000_BSWF_IP |
| 43 | ACME_SANDANE_FROST_PAR_54_IP_EF |
| 44 | ARES_IP_XP_500_BEAM_IP |
| 45 | ARES_XP_500_BEAM |
| 46 | AUTO_600 |
| 47 | Aeco15 |
| 48 | Aeco20 |
| 49 | Aeco30IP |
| 50 | BL_200_IP |
| 51 | CM-800Z |
| 52 | COOLIE_186_BW_186 |
| 53 | COOLIE_546_BW_546 |
| 54 | HUE_6_IP_HUE_6_IP |
| 55 | LP-2304 |
| 56 | PANEL_3612_EF |
| 57 | PAR54IP |
| 58 | SANA_BEAM_XA_300_BEAM_IP |
| 59 | SANDANE_FROST_PAR_54_IP |
| 60 | SOLAR_WIND_XA_500_BSW |
| 61 | STAGEPAR_400_ZOOM_IP_MP-400Z_IP |
| 62 | STROBE_3_IP |
| 63 | SUPERNOVA_LT_XA_2000_BSWF_IP_LT |
| 64 | TB_1060 |
| 65 | TB_5_IP |
| 66 | THUNDER_BL-200_CP |
| 67 | TV_LIGHT_PANEL_3000_LP_F3000 |
| 68 | XA_300_BSWF_IP |
| 69 | XA_500_BEAM |
| 70 | XA_600_BSWF_IP |
| 71 | XA_600_BSW_IP |
| 72 | XP_380_BEAM_IP |
| 73 | XP_400_BEAM_II |
| 74 | XP_400_BEAM_IP |
| 75 | XP_500_BSW_IP |
| 76 | XP_550_BSW_IP_II |

### ADJ(32)

| # | Model |
| ---: | --- |
| 1 | 12P_Hex |
| 2 | 7PZ_IP |
| 3 | 7P_Hex_IP |
| 4 | ADJ_Encore_LP7_IP |
| 5 | ADJ_Hydro_Beam_X1 |
| 6 | COB_Cannon_LP200X |
| 7 | Dotz_Par_RGBL |
| 8 | Element_Hex_IP |
| 9 | Eliminator_Stryker_Beam |
| 10 | Eliminator_Stryker_Spot |
| 11 | Encore_FR_Pro_Color |
| 12 | Encore_LB15IP |
| 13 | Encore_LP12IP |
| 14 | Encore_LP12Z_IP |
| 15 | Encore_Z7LP |
| 16 | Focus_CMY_Compact |
| 17 | Focus_Hybrid |
| 18 | Focus_Profile |
| 19 | Focus_Spot_4Z |
| 20 | Focus_Spot_6Z |
| 21 | Focus_Spot_7Z |
| 22 | Hydro_Beam_CMY |
| 23 | Hydro_Hybrid |
| 24 | Hydro_Profile |
| 25 | Hydro_Spot_1 |
| 26 | Hydro_Spot_2 |
| 27 | Protege_XL |
| 28 | Protege_XM |
| 29 | Protege_XS |
| 30 | SABER_SPOT_RGBL |
| 31 | Vizi_Beam_CMY |
| 32 | Vizi_Beam_RX2 |

### ARRI(6)

| # | Model |
| ---: | --- |
| 33 | ARRI_L5C_Plus |
| 34 | ARRI_L7C |
| 35 | ARRI_L7C_Plus |
| 36 | ARRI_Orbiter |
| 37 | L10-C |
| 38 | L5-C |

### Ayrton(42)

| # | Model |
| ---: | --- |
| 77 | Ayrton_Arcaline_2_3G_1M |
| 78 | Ayrton_Arcaline_2_3G_50CM |
| 79 | Ayrton_Argo_6_WASH |
| 80 | Ayrton_Bora |
| 81 | Ayrton_Cobra |
| 82 | Ayrton_Cobra_2 |
| 83 | Ayrton_ColorSun_200s |
| 84 | Ayrton_Diablo |
| 85 | Ayrton_Domino_LT |
| 86 | Ayrton_Domino_Profile |
| 87 | Ayrton_Domino_Wash |
| 88 | Ayrton_EagleStrike |
| 89 | Ayrton_Eurus |
| 90 | Ayrton_Ghibli |
| 91 | Ayrton_Huracan_LT |
| 92 | Ayrton_Huracan_Profile |
| 93 | Ayrton_Huracan_Wash |
| 94 | Ayrton_Intellipix_XT |
| 95 | Ayrton_Karif |
| 96 | Ayrton_Khamsin |
| 97 | Ayrton_Kyalami |
| 98 | Ayrton_Levante |
| 99 | Ayrton_MagicBlade_FX |
| 100 | Ayrton_MagicBlade_R |
| 101 | Ayrton_MagicPanel_FX |
| 102 | Ayrton_MagicRing_R1 |
| 103 | Ayrton_Mamba |
| 104 | Ayrton_Merak |
| 105 | Ayrton_MiniPanel_FX |
| 106 | Ayrton_Mistral |
| 107 | Ayrton_Naja |
| 108 | Ayrton_NandoBeam_S3 |
| 109 | Ayrton_Perseo_BEAM |
| 110 | Ayrton_Perseo_Profile |
| 111 | Ayrton_RawBeam_350 |
| 112 | Ayrton_Rivale_Profile |
| 113 | Ayrton_Rivale_Wash |
| 114 | Ayrton_Rollapix_100 |
| 115 | Ayrton_Stradale_Profile |
| 116 | Ayrton_Veloce_Profile |
| 117 | Ayrton_Veloce_Wash |
| 118 | Domino_LT_GroundControl_Followspot |

### BeamZ(2)

| # | Model |
| ---: | --- |
| 124 | BeamZ_Ignite180Spot |
| 125 | IGNITE180B_LED_Beam_Moving_Head |

### BOS Lighting(5)

| # | Model |
| ---: | --- |
| 119 | Amadeus_H300 |
| 120 | Amadeus_H600 |
| 121 | Amadeus_H800_Profile_IP |
| 122 | Fresnel_200_Bi_Color |
| 123 | HP7_QUAD_350 |

### Briteq(2)

| # | Model |
| ---: | --- |
| 126 | BTI-BLINDER_1FX |
| 127 | BTX-Saturn |

### Cameo(35)

| # | Model |
| ---: | --- |
| 202 | AZOR_S2_CLAZORS2 |
| 203 | AZOR_SP2IP_CLAZORSP2IP |
| 204 | AZOR_SP2_CLAZORSP2 |
| 205 | Azor_B1_CLAB1 |
| 206 | CLF1FC |
| 207 | CLF2FC |
| 208 | CLF4FC |
| 209 | CLH2FC |
| 210 | DROP_B1_CLDROPB1 |
| 211 | DROP_B4_CLDROPB4 |
| 212 | Dura_Spot_100_CLDS100 |
| 213 | Evos_S3_Spot_Moving_Head |
| 214 | F2FC+_with_Barndoor_CLF2FC+ |
| 215 | Flat_Pro_12_G2_CLFLATPRO12G2 |
| 216 | LED_Flat_PAR_RGBW_Utra-Compact_7X4_W_RGBW_Flat_PAR_Light |
| 217 | LUXIS_FC_CLLUXISFC |
| 218 | OPUS_S5_Spot_Moving_Head |
| 219 | OPUS_SP5+_CLOSP5PLUS |
| 220 | ORON_H2 |
| 221 | OTOS_H5_CLOTOSH5 |
| 222 | Opus_H5_CLOH5 |
| 223 | Opus_H6_IP |
| 224 | Opus_SP6_FC_CLOSP6FC |
| 225 | Opus_SP6_IP_CLOSP6IP |
| 226 | Opus_X4_CLOX4 |
| 227 | Opus_X4_Profile_IP_CLOX4PIP |
| 228 | Opus_X_Profile_CLOXP |
| 229 | Otos_B5_CLOTOSB5 |
| 230 | Otos_SP6_CLOTOSSP6 |
| 231 | Q-Spot_40 |
| 232 | S2_IP65_Softlight_CLS2IP |
| 233 | ZENIT_Z120_G2 |
| 234 | ZIYA_200_CLZIYA200 |
| 235 | ZIYA_200_COB_CLZIYA200COB |
| 236 | ZIYA_400_CLZIYA400 |

### Chauvet DJ(4)

| # | Model |
| ---: | --- |
| 308 | Chauvet_DJ_CorePar_Q120 |
| 309 | Intimidator_Hybrid_251_SR |
| 310 | Intimidator_Spot_475ZX |
| 311 | Shocker_2 |

### ChauvetProfessional(71)

| # | Model |
| ---: | --- |
| 237 | COLORado_1-Quad_Zoom |
| 238 | COLORado_1-Tri_Tour |
| 239 | COLORado_1QS |
| 240 | COLORado_1_Quad |
| 241 | COLORado_1_Solo |
| 242 | COLORado_2-Quad_Zoom |
| 243 | COLORado_2_Solo |
| 244 | COLORado_3_Quad_Zoom |
| 245 | COLORado_3_Solo |
| 246 | COLORado_M_Solo |
| 247 | COLORado_PXL_Curve_12 |
| 248 | COLORado_Panel_Q40 |
| 249 | COLORdash_Accent_3 |
| 250 | COLORdash_Accent_5XIP |
| 251 | COLORdash_Accent_Quad |
| 252 | COLORdash_PAR_H12X_IP |
| 253 | COLORdash_PAR_H18X |
| 254 | COLORdash_PAR_H7X |
| 255 | COLORdash_PAR_H7X_IP |
| 256 | COLORdash_Par_Quad_18 |
| 257 | COLORdash_Par_Quad_7 |
| 258 | Maverick_Force_X_Profile |
| 259 | Maverick_Force_X_Spot |
| 260 | Maverick_Silens_1X_Profile |
| 261 | Maverick_Silens_2X_Profile |
| 262 | Maverick_Storm_1_Beam |
| 263 | Maverick_Storm_1_Flex |
| 264 | Maverick_Storm_1_Hybrid |
| 265 | Maverick_Storm_1_Spot |
| 266 | Maverick_Storm_2_Profile |
| 267 | Maverick_Storm_3_Profile |
| 268 | Maverick_Storm_4_Profile |
| 269 | Maverick_Storm_4_SoloWash |
| 270 | Ovation_E-2_FC |
| 271 | Ovation_E-910FC |
| 272 | Ovation_E-930VW |
| 273 | Ovation_F-415VW |
| 274 | Ovation_F-415_FC |
| 275 | Ovation_F915VW |
| 276 | Ovation_H-605FC |
| 277 | Ovation_P-56FC |
| 278 | Ovation_P-56VW |
| 279 | Ovation_Reve_F-3_IP |
| 280 | Ovation_Reve_H2 |
| 281 | Ovation_Reve_P-3_IP |
| 282 | Ovation_Reve_P2_IP |
| 283 | Rogue_Outcast_1L_Beam |
| 284 | Rogue_Outcast_1M_Beam |
| 285 | Rogue_Outcast_1_Beam |
| 286 | Rogue_Outcast_1_Hybrid |
| 287 | Rogue_Outcast_2X_Wash |
| 288 | Rogue_Outcast_2_Beam |
| 289 | Rogue_Outcast_2_Hybrid |
| 290 | Rogue_outcast_3X_Wash |
| 291 | Strike_1 |
| 292 | Strike_Array_2 |
| 293 | Strike_Array_2C |
| 294 | Strike_Array_4 |
| 295 | Strike_Array_4C |
| 296 | Strike_Array_Ultra |
| 297 | Strike_P38 |
| 298 | Strike_Saber_10 |
| 299 | Strke_Array_1 |
| 300 | WELL_PANEL_X |
| 301 | Well_Fit_X |
| 302 | Well_Panel |
| 303 | Well_Pod_2 |
| 304 | Well_Pod_3 |
| 305 | onAir_Panel_1_IP |
| 306 | onAir_Panel_2_IP |
| 307 | onAir_Panel_Min_IP |

### CINDY(12)

| # | Model |
| ---: | --- |
| 128 | CINDY_CT4_HYBRID |
| 129 | CINDY_CT5_SPOT |
| 130 | CINDY_CT8_BSWF |
| 131 | CINDY_OMNI_1400 |
| 132 | CL-1000_BSWF |
| 133 | CL-1800_BSWF |
| 134 | CL-400LS_BEAM |
| 135 | CL-650D_HYBRID |
| 136 | CL-750_HYBRID |
| 137 | CL-800_BSWF |
| 138 | TOUGH_350_BEAM |
| 139 | TOUGH_500_HYBRID |

### CKC Lighting(60)

| # | Model |
| ---: | --- |
| 140 | FC_Par_14 |
| 141 | FC_Par_7 |
| 142 | FlatPar_1212 |
| 143 | FlatPar_712 |
| 144 | Fresnel_150TW |
| 145 | Fresnel_180TW |
| 146 | Fresnel_350TW |
| 147 | Fresnel_80TW |
| 148 | IP_Blinder_DTW2 |
| 149 | L_Par_18 |
| 150 | MP5 |
| 151 | MP5T |
| 152 | MP5X |
| 153 | P10 |
| 154 | P10_Plus |
| 155 | P200_FC_IP |
| 156 | P500_FC |
| 157 | P500_FC_IP |
| 158 | PAR_Z180 |
| 159 | PAR_Z180_IP |
| 160 | PAR_Z60_IP |
| 161 | Par_Z19_IP |
| 162 | Pinspot_40 |
| 163 | Profile_500FC_IP |
| 164 | Proxima_Hybrid |
| 165 | Proxima_IP |
| 166 | Proxima_Tiny |
| 167 | Proxima_X |
| 168 | Qdot_16_IP_DTW |
| 169 | SP4 |
| 170 | Soft_Panel_300DW |
| 171 | Soft_Panel_300TW |
| 172 | TS6 |
| 173 | Tourist_M3 |
| 174 | Tourist_M5 |
| 175 | Tourist_M6 |
| 176 | Tourist_M7 |
| 177 | Tourist_M9 |
| 178 | V10 |
| 179 | V15 |
| 180 | V15Pro |
| 181 | V15Pro_IP |
| 182 | V15_IP |
| 183 | V20 |
| 184 | V20_IP |
| 185 | V2sIP |
| 186 | V4 |
| 187 | V4s_IP |
| 188 | V6IP |
| 189 | V8IP |
| 190 | V8Pro |
| 191 | V9 |
| 192 | VH20 |
| 193 | VH5 |
| 194 | VH5BIP |
| 195 | VH5IP |
| 196 | Wash_Z120 |
| 197 | Wash_Z350 |
| 198 | Y1640 |
| 199 | Y840 |

### ClayPaky(26)

| # | Model |
| ---: | --- |
| 312 | Actoris_Fresnel |
| 313 | Arolla_Aqua |
| 314 | Arolla_Aqua_HP |
| 315 | Arolla_Aqua_LT |
| 316 | Arolla_Aqua_M-LT |
| 317 | Arolla_Aqua_S-LT |
| 318 | Arolla_Profile_HP |
| 319 | Arolla_Profile_MP |
| 320 | Arolla_Spot_MP |
| 321 | Axcor_Spot_400 |
| 322 | ClayPaky_SharpyPlusAqua |
| 323 | HY_B-EYE_K15 |
| 324 | HY_B-EYE_K25 |
| 325 | HY_B-EYE_K25_Teatro |
| 326 | Rhapsodya |
| 327 | Scenius_Unico |
| 328 | Sharpy |
| 329 | Sharpy_Plus |
| 330 | Sharpy_X_Frame |
| 331 | Sharpy_X_Spot |
| 332 | Skylos |
| 333 | Tambora_Stormy |
| 334 | Ultimo_Hybrid |
| 335 | Ultimo_Sharpy |
| 336 | Xtylos_Aqua |
| 337 | minixtylosHPE |

### CLF Lighting(2)

| # | Model |
| ---: | --- |
| 200 | CLF_Aorun |
| 201 | CLF_Lighting_Orion |

### DLW(15)

| # | Model |
| ---: | --- |
| 338 | AL_L800_BSWF |
| 339 | DLW_120B_IP |
| 340 | DLW_400_BEAM |
| 341 | DLW_400_BSW |
| 342 | DLW_420BV_IP |
| 343 | DLW_420B_IP |
| 344 | DLW_480BSW_IP |
| 345 | DLW_480B_IP |
| 346 | DLW_550BSW_IP |
| 347 | DLW_DREAM_L1000_SPOT |
| 348 | DLW_DREAM_L1600P_FLAG |
| 349 | DLW_J300B_IP |
| 350 | DLW_L1200PRO_IP |
| 351 | DLW_L600BSWF_IP |
| 352 | DLW_L600C6P_IP |

### DTS Lighting(5)

| # | Model |
| ---: | --- |
| 353 | DTS_Lighting_Core |
| 354 | DTS_Lighting_Synergy_5_Profile |
| 355 | DTS_Lighting_Synergy_5_Spot |
| 356 | DTS_Lighting_Synergy_6_Profile |
| 357 | DTS_Lighting_Synergy_7_Profile |

### EK Lights(15)

| # | Model |
| ---: | --- |
| 358 | BILLYTHEKID-CMY |
| 359 | Blinder_II_C |
| 360 | COLLIDER_FC |
| 361 | CYBER_BEAM |
| 362 | EC7_Par_IP |
| 363 | H1 |
| 364 | H1-IP |
| 365 | KX-100+ |
| 366 | LT15 |
| 367 | LT6 |
| 368 | MC10 |
| 369 | MC15 |
| 370 | MC6 |
| 371 | ORCA |
| 372 | TERMINATOR |

### Elation(58)

| # | Model |
| ---: | --- |
| 374 | ACL_360_Bar |
| 375 | ACL_360i |
| 376 | Arena_Q7_Zoom |
| 377 | Artiste_DaVinci |
| 378 | Artiste_Mondrian |
| 379 | Artiste_Monet |
| 380 | CUEPIX_16IP |
| 381 | DARTZ_360 |
| 382 | DTW_BLINDER_350_IP |
| 383 | Elation_ProTron_LED_Strobe |
| 384 | FUZE_PAR_Z120_IP |
| 385 | FUZE_PAR_Z175 |
| 386 | FUZE_PAR_Z60_IP |
| 387 | Fuze_Max_Profile |
| 388 | Fuze_Max_Spot |
| 389 | Fuze_PFX |
| 390 | Fuze_Profile |
| 391 | Fuze_SFX |
| 392 | Fuze_Spot |
| 393 | Fuze_Teatro |
| 394 | Fuze_Wash_500 |
| 395 | KL_Core_IP |
| 396 | KL_Fresnel_4_FC |
| 397 | KL_Fresnel_6_CW |
| 398 | KL_Fresnel_6_FC |
| 399 | KL_Fresnel_6_IP |
| 400 | KL_Fresnel_8_FC |
| 401 | KL_Fresnel_8_IP |
| 402 | KL_Panel |
| 403 | LIMELIGHT_PAR_L |
| 404 | LIMELIGHT_PAR_S |
| 405 | Paragon_LT |
| 406 | Paragon_M |
| 407 | Paragon_S |
| 408 | Proteus_Atlas |
| 409 | Proteus_Excalibur |
| 410 | Proteus_Hybrid_Max |
| 411 | Proteus_Lucius |
| 412 | Proteus_Lucius_OPS |
| 413 | Proteus_Maximus |
| 414 | Proteus_Odeon |
| 415 | Proteus_Radius |
| 416 | Protron_3K_LED_Strobe |
| 417 | REBEL_Dartz |
| 418 | REBEL_Profile |
| 419 | SEVEN_PAR_19IP |
| 420 | SEVEN_PAR_7IP |
| 421 | SIX+_PAR_S |
| 422 | SIXPAR_100 |
| 423 | SIXPAR_100_IP |
| 424 | SIXPAR_200_IP |
| 425 | SIXPAR_300 |
| 426 | SOL_II_Blinder |
| 427 | SOL_IV_Blinder |
| 428 | SOL_I_Blinder |
| 429 | Smarty_Max |
| 430 | VOLT+_PAR_L |
| 431 | VOLT+_PAR_S |

### ETC(1)

| # | Model |
| ---: | --- |
| 373 | SolaFrame_Theatre |

### Eurolite(3)

| # | Model |
| ---: | --- |
| 432 | EUROLITE_Audience_Blinder_2x100W_LED_COB_CW_WW |
| 433 | SUNSET_IP_EYE_1_Blinder_dim2warm |
| 434 | XB-200 |

### Event Lighting(2)

| # | Model |
| ---: | --- |
| 435 | HAVOCH330 |
| 436 | HAVOCS150 |

### Expolite(1)

| # | Model |
| ---: | --- |
| 437 | Expolite_SL700 |

### Fine Art(1)

| # | Model |
| ---: | --- |
| 441 | FINE_2000_PERF |

### flash(2)

| # | Model |
| ---: | --- |
| 805 | BEAM_7R_v1909 |
| 806 | EVENT_Moving_Head_17R_350W_3w1 |

### FOS Technologies(3)

| # | Model |
| ---: | --- |
| 438 | FOS_ARES |
| 439 | FOS_BEAM_7R_Hybrid_Plus |
| 440 | FOS_Triton |

### GLP(17)

| # | Model |
| ---: | --- |
| 442 | ArenaLED1_Touring_Color |
| 443 | FUSION_EXO_Hybrid_40 |
| 444 | FUSION_PAR_FP7 |
| 445 | FUSION_X-PAR_12Z_with_floor_stand |
| 446 | GLP_X4_atom_single_Head |
| 447 | GLP_X5_atom_single_Head |
| 448 | GLP_impression_S500_Profil_HC |
| 449 | GLP_impression_S500_Profil_HO |
| 450 | Impression_E350 |
| 451 | Impression_FR1 |
| 452 | Impression_GT-1 |
| 453 | Impression_S350 |
| 454 | Impression_X5 |
| 455 | Impression_X5_Compact |
| 456 | Impression_X5_IP |
| 457 | Impression_X5_IP_Maxx |
| 458 | JDC_1 |

### High End Systems(7)

| # | Model |
| ---: | --- |
| 459 | Halcyon_Gold_High_Fidelity |
| 460 | Halcyon_Gold_Ultra-Bright |
| 461 | Halcyon_Platinum_High_Fidelity |
| 462 | Halcyon_Platinum_Ultra-Bright |
| 463 | Halcyon_Silent |
| 464 | Halcyon_Titanium_High_Fidelity |
| 465 | Halcyon_Titanium_Ultra-Bright |

### Infinity(10)

| # | Model |
| ---: | --- |
| 466 | Furion_B401_Beam |
| 467 | Furion_B402_Beam |
| 468 | Furion_H402_Hybrid |
| 469 | Furion_P402_Profile |
| 470 | Furion_P602_Profile |
| 471 | Furion_S201_Spot |
| 472 | Furion_S401_Spot |
| 473 | Furion_S402_Spot |
| 474 | Furion_S601_Profile |
| 475 | Furion_S602_Spot |

### JB Lighting(32)

| # | Model |
| ---: | --- |
| 476 | M18_Profile |
| 477 | M18_Profile_MA |
| 478 | P10_Profile_HC |
| 479 | P10_Profile_HP |
| 480 | P10_Profile_MA_HC |
| 481 | P10_Profile_MA_HP |
| 482 | P10_Profile_MA_WW |
| 483 | P10_Profile_WW |
| 484 | P12_Profile_HC |
| 485 | P12_Profile_HP |
| 486 | P12_Profile_MA_HC |
| 487 | P12_Profile_MA_HP |
| 488 | P12_Profile_MA_WW |
| 489 | P12_Profile_WW |
| 490 | P12_Spot_HC |
| 491 | P12_Spot_HP |
| 492 | P12_Spot_WW |
| 493 | P15_Profile_HC |
| 494 | P15_Profile_HP |
| 495 | P15_Profile_WW |
| 496 | P18_MK2_Profile_HC |
| 497 | P18_MK2_Profile_HP |
| 498 | P18_MK2_Profile_MA_HC |
| 499 | P18_MK2_Profile_MA_HP |
| 500 | P18_MK2_Profile_MA_WW |
| 501 | P18_MK2_Profile_WW |
| 502 | P18_Profile_HC |
| 503 | P18_Profile_HP |
| 504 | P18_Profile_WW |
| 505 | P5_Beamspot_HP |
| 506 | P7 |
| 507 | P9_Beamspot_HPv4 |

### LIGHT4ME(1)

| # | Model |
| ---: | --- |
| 508 | VENOM_HYBRID_450_CMY |

### Lightmaxx(1)

| # | Model |
| ---: | --- |
| 520 | VECTOR_Shaft_7R |

### LightSky(11)

| # | Model |
| ---: | --- |
| 509 | 8800-C42 |
| 510 | AURORA |
| 511 | AURORA_AQUA |
| 512 | MAMBA |
| 513 | SHARK400BSW |
| 514 | SHARK450BSW |
| 515 | SUPER_SCOPE_COLOR |
| 516 | SUPER_SCOPE_HYBRID |
| 517 | SUPER_SHARK_BEAM |
| 518 | Super_Scope_Max |
| 519 | Super_Scope_Pro |

### MARK(6)

| # | Model |
| ---: | --- |
| 521 | BEAM_198_MKII |
| 522 | BEAM_WS_440_CMY |
| 523 | BSW_CMY_LED_400 |
| 524 | BSW_CMY_PROFILE_500 |
| 525 | MOVILED_2854 |
| 526 | MP_500_L |

### MartinProfessional(35)

| # | Model |
| ---: | --- |
| 527 | Atomic_3000_DMX |
| 528 | Atomic_3K_LED |
| 529 | ELP_Manet_8F |
| 530 | ELP_PAR |
| 531 | ELP_PAR_IP |
| 532 | ERA_300_Profile |
| 533 | ERA_400_Performance_CLD |
| 534 | ERA_400_Performance_WRM |
| 535 | ERA_600_Performance |
| 536 | ERA_600_Profile |
| 537 | ERA_700_Performance_IP |
| 538 | ERA_800_Performance |
| 539 | ERA_800_Profile |
| 540 | MAC_Allure_Profile |
| 541 | MAC_Allure_Wash_PC |
| 542 | MAC_Aura |
| 543 | MAC_Aura_PXL |
| 544 | MAC_Aura_XB |
| 545 | MAC_Axiom_Hybrid |
| 546 | MAC_Encore_Performance_CLD |
| 547 | MAC_Encore_Performance_WRM |
| 548 | MAC_Encore_Two |
| 549 | MAC_Quantum_Profile |
| 550 | MAC_Ultra_Performance |
| 551 | MAC_Ultra_Wash |
| 552 | MAC_Viper_AirFX |
| 553 | MAC_Viper_Performance |
| 554 | MAC_Viper_Profile |
| 555 | MAC_Viper_Wash_DX |
| 556 | MAC_Viper_XIP |
| 557 | RUSH_PAR_3 |
| 558 | Rush_MH3_Beam |
| 559 | VDO_Atomic_Bold |
| 560 | VDO_Atomic_Dot_CLD |
| 561 | VDO_Atomic_Dot_WRM |

### Meanreal(4)

| # | Model |
| ---: | --- |
| 562 | Blinder400IP |
| 563 | Hybrid_BSW_380W |
| 564 | Meanreal_FT-1200 |
| 565 | Meanreal_MP600 |

### MegaLite(6)

| # | Model |
| ---: | --- |
| 566 | EB1 |
| 567 | Framebot_600 |
| 568 | GB1 |
| 569 | GF-1 |
| 570 | Spotbot_LED_CYM_300 |
| 571 | Toad_400_Hybrid |

### Nicols(1)

| # | Model |
| ---: | --- |
| 572 | RACER_CMY_350 |

### PR Lighting(2)

| # | Model |
| ---: | --- |
| 573 | LSE_580BWS |
| 574 | LSE_580Beam |

### Pro Lux(1)

| # | Model |
| ---: | --- |
| 575 | LUX_HOTBEAM_420_IP |

### Prolights(69)

| # | Model |
| ---: | --- |
| 576 | ArcSpotDOTFC |
| 577 | ArcSpotDOTVW |
| 578 | ArcSpotLFC |
| 579 | ArcSpotLVW |
| 580 | ArcSpotMFC |
| 581 | ArcSpotMVW |
| 582 | ArcSpotSFC |
| 583 | ArcSpotSVW |
| 584 | ArcSpotXLFC |
| 585 | ArcSpotXLVW |
| 586 | ArcSpotXSFC |
| 587 | ArcSpotXSVW |
| 588 | ArenaCob4FC |
| 589 | AstraBeam120IP |
| 590 | AstraBeam260IP |
| 591 | AstraHybrid260IP |
| 592 | AstraHybrid330 |
| 593 | AstraHybrid330IP |
| 594 | AstraHybrid420 |
| 595 | AstraHybrid420IP |
| 596 | AstraProfile1200IP |
| 597 | AstraProfile300LTIP |
| 598 | AstraProfile400 |
| 599 | AstraProfile500LTIP |
| 600 | AstraProfile600IP |
| 601 | AstraProfile900 |
| 602 | DisplayCobTRWDFC_60 |
| 603 | EclFresnel2KTW |
| 604 | EclFresnelCTPL |
| 605 | EclFresnelCTPLIP |
| 606 | EclFresnelCTPM |
| 607 | EclFresnelCTPMIP |
| 608 | EclFresnelCTPS |
| 609 | EclFresnelCTPSIP |
| 610 | EclFresnelCTPXLIP |
| 611 | EclFresnelCTPXS |
| 612 | EclFresnelFWXSVW |
| 613 | EclFresnelJTW |
| 614 | EclFresnelTW |
| 615 | EclGalleryProfile_FC |
| 616 | EclMiniFresnelFC |
| 617 | EclParDOTFC |
| 618 | EclParDOTVW |
| 619 | EclParFC_24 |
| 620 | JetBeam1 |
| 621 | JetBeam120IP |
| 622 | JetHybrid200 |
| 623 | JetProfile300LT |
| 624 | JetSpot120 |
| 625 | JetSpot3 |
| 626 | Luma1500SH |
| 627 | LumiPar12UTRI |
| 628 | LumiPar_12UQPro |
| 629 | MuseFresnel70CTPlus |
| 630 | MuseProfile70CTPlus |
| 631 | RA2000Profile |
| 632 | RA3000Profile |
| 633 | SmartParNEO |
| 634 | StudioCobFC-30 |
| 635 | StudioCobPlusFC2_18 |
| 636 | StudioCobPlusTW_20 |
| 637 | SunRise2FLXFC |
| 638 | SunRise2FLXVW |
| 639 | SunRise4FLXFC |
| 640 | SunRise4FLXVW |
| 641 | Sunrise2 |
| 642 | Sunrise2IP |
| 643 | Sunrise2L |
| 644 | Versapar |

### Robe Lighting(68)

| # | Model |
| ---: | --- |
| 647 | Robin_600_LEDWash |
| 648 | Robin_Actor_12 |
| 649 | Robin_Actor_6 |
| 650 | Robin_BMFL_Blade |
| 651 | Robin_BMFL_Spot |
| 652 | Robin_BMFL_WashBeam |
| 653 | Robin_Cuete |
| 654 | Robin_Esprite |
| 655 | Robin_Esprite_FS |
| 656 | Robin_Esprite_Fresnel |
| 657 | Robin_Esprite_PC |
| 658 | Robin_Forte |
| 659 | Robin_Forte_FS |
| 660 | Robin_Forte_Fresnel |
| 661 | Robin_Forte_HCF |
| 662 | Robin_Forte_PC |
| 663 | Robin_GigaPointe |
| 664 | Robin_LEDBeam_150_RGBA |
| 665 | Robin_LEDBeam_350_RGBA |
| 666 | Robin_LedPOINTE |
| 667 | Robin_MegaPointe |
| 668 | Robin_MiniPointe |
| 669 | Robin_Painte |
| 670 | Robin_Painte_Fresnel |
| 671 | Robin_Painte_Spot |
| 672 | Robin_ParFect_150_FW_RGBW |
| 673 | Robin_ParFect_150_RGBA |
| 674 | Robin_ParFect_150_RGBW |
| 675 | Robin_Pointe |
| 676 | Robin_ProMotion |
| 677 | Robin_Spikie |
| 678 | Robin_SpikiePlus |
| 679 | Robin_SuperSpikie |
| 680 | Robin_T15_Fresnel |
| 681 | Robin_T1_Fresnel |
| 682 | Robin_T1_PC |
| 683 | Robin_T1_Profile |
| 684 | Robin_T1_Profile_FS |
| 685 | Robin_T2_Fresnel |
| 686 | Robin_T2_PC |
| 687 | Robin_T2_Profile |
| 688 | Robin_T2_Profile_FS |
| 689 | Robin_T3_Fresnel |
| 690 | Robin_T3_PC |
| 691 | Robin_T3_Profile |
| 692 | Robin_T3_Profile_FS |
| 693 | Robin_TX1_PosiProfile |
| 694 | Robin_T_5_Profile |
| 695 | Robin_Tetra1 |
| 696 | Robin_iBeam_350 |
| 697 | Robin_iBolt |
| 698 | Robin_iESPRITE_Fresnel |
| 699 | Robin_iESPRITE_LTL_FS |
| 700 | Robin_iESPRITE_LTL_WB |
| 701 | Robin_iEsprite |
| 702 | Robin_iFORTE_FS |
| 703 | Robin_iForte |
| 704 | Robin_iForte_Fresnel |
| 705 | Robin_iForte_LTX_FS |
| 706 | Robin_iForte_LTX_WB |
| 707 | Robin_iPAINTE_LTM_WB |
| 708 | Robin_iPainte |
| 709 | Robin_iParFect_150_FW_RGBA |
| 710 | Robin_iParFect_150_FW_RGBW |
| 711 | Robin_iProMotion |
| 712 | Robin_iProMotion_HDP |
| 713 | Robin_iSpiider |
| 714 | Robin_iTetra2 |

### Robert Juliat(2)

| # | Model |
| ---: | --- |
| 715 | Sully_650SX_4C |
| 716 | Sully_654SX_4C |

### ROXX(2)

| # | Model |
| ---: | --- |
| 645 | CLUSTER_B2-FC |
| 646 | CLUSTER_B2-W |

### SHEHDS(2)

| # | Model |
| ---: | --- |
| 717 | 230W_7R_Beam_Moving_Head_Button_Version |
| 718 | 230W_Beam_Moving_Head_Lighting |

### Showtec(7)

| # | Model |
| ---: | --- |
| 719 | FLEX_Blaze_Pod |
| 720 | Stage_Blinder_100_Blaze |
| 721 | Stage_Blinder_200_Blaze |
| 722 | The_Meg_Beam_One |
| 723 | Titan_Strobe_1000_FX |
| 724 | Xpression_2000B |
| 725 | Xpression_2000S |

### Spotlight(17)

| # | Model |
| ---: | --- |
| 726 | FRESNEL_HYPERION_LED_100W_RGBACL_6_COLOR_Mark-II |
| 727 | FRESNEL_HYPERION_LED_100W_RGBW_4_COLOR_Mark-II |
| 728 | FRESNEL_HYPERION_LED_200W_RGBACL_6_COLOR_Mark-II |
| 729 | FRESNEL_HYPERION_LED_200W_RGBW_4_COLOR_Mark-II |
| 730 | FRESNEL_HYPERION_LED_300W_RGBACL_6_COLOR_Mark-II |
| 731 | FRESNEL_HYPERION_LED_300W_RGBW_4_COLOR_Mark-II |
| 732 | FRESNEL_HYPERION_LED_700W_RGBACL+WW+CW_8_COLOR_Mark-II |
| 733 | FRESNEL_LED_50W_RGBW_4_COLOR_Mark-II |
| 734 | HYPERION_PC_6C |
| 735 | HYPERION_PC_TW |
| 736 | PLANO_CONVEX_HYPERION_LED_100W_RGBACL_6_COLOR_Mark-II |
| 737 | PLANO_CONVEX_HYPERION_LED_100W_RGBW_4_COLOR_Mark-II |
| 738 | PLANO_CONVEX_HYPERION_LED_200W_RGBACL_6_COLOR_Mark-II |
| 739 | PLANO_CONVEX_HYPERION_LED_200W_RGBW_4_COLOR_Mark-II |
| 740 | PLANO_CONVEX_HYPERION_LED_300W_RGBACL_6_COLOR_Mark-II |
| 741 | PLANO_CONVEX_HYPERION_LED_300W_RGBW_4_COLOR_Mark-II |
| 742 | PLANO_CONVEX_HYPERION_LED_700W_RGBACL+WW+CW_8_COLOR_Mark-II |

### Starway(7)

| # | Model |
| ---: | --- |
| 743 | BARACCA_360 |
| 744 | DAYTONA |
| 745 | ENZO |
| 746 | ENZO_WET |
| 747 | Lusso |
| 748 | Monza_Wet |
| 749 | Superfast_Wet |

### Terbly(38)

| # | Model |
| ---: | --- |
| 750 | G22H_NOVA |
| 751 | G22H_NOVA_IP |
| 752 | G25B_IP |
| 753 | G25_Hybrid |
| 754 | G25_Hybrid_IP |
| 755 | G7B_NOVA_IP |
| 756 | GL6_IP |
| 757 | GL8P_IP |
| 758 | GL9P_IP |
| 759 | S380B |
| 760 | S380B_IP |
| 761 | S380H_IP |
| 762 | S550B_IP |
| 763 | S550H |
| 764 | S550H_IP |
| 765 | S620B |
| 766 | S620B_IP |
| 767 | S620H |
| 768 | S620H_IP |
| 769 | SL10P_IP |
| 770 | SL10P_IP_Ultra |
| 771 | SL1200P |
| 772 | SL1200PH_IP |
| 773 | SL18P |
| 774 | SL3000P_IP |
| 775 | SL5000P_IP |
| 776 | SL880P |
| 777 | SL880P_IP |
| 778 | SL880P_IP_ULTRA |
| 779 | SLC_IP |
| 780 | Terbly_A51-E |
| 781 | Terbly_A51C |
| 782 | Terbly_LPAR-7 |
| 783 | Terbly_LPAR7QZ |
| 784 | Terbly_LPAR7_IP |
| 785 | Terbly_S-1000 |
| 786 | Terbly_TP300 |
| 787 | Terbly_TP300_FX |

### Triton Blue(2)

| # | Model |
| ---: | --- |
| 788 | Halley_H450 |
| 789 | Python_100 |

### UKing(1)

| # | Model |
| ---: | --- |
| 790 | ZQ02360_120W_LED_Moving_Head_Beam_with_Halo |

### Uplus Lighting(3)

| # | Model |
| ---: | --- |
| 791 | IP-S350B |
| 792 | IP_BLINDER_600_3in1 |
| 793 | Uplus_Lighting_PZ1940 |

### Vari-Lite(9)

| # | Model |
| ---: | --- |
| 794 | Fresnel_FC |
| 795 | VL10_BEAM_WASH |
| 796 | VL1600_Profile |
| 797 | VL2600_Profile |
| 798 | VL2600_Spot |
| 799 | VL2600_Wash |
| 800 | VL3600_Profile_IP |
| 801 | VL800_EventProfile |
| 802 | VLZ_Profile_E |

### Varytec(1)

| # | Model |
| ---: | --- |
| 803 | Varytec_Trident_R10 |

### Vello Light(1)

| # | Model |
| ---: | --- |
| 804 | BSW450 |
