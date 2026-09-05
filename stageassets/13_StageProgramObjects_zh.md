# 舞台程序化对象 — 用户手册

> 适用版本：SuperStage 26H2.5 起 ｜ 前置阅读：[舞台资产总览](00_StageAssets_Overview_zh.md)

本篇覆盖 26H2.5 新增的 7 个程序化对象。它们与桁架、脚手架一样是参数化生成的，改参数即重建。

放置方式：资产浏览器的 **StageModel** 分类，或在"放置 Actor"面板中按名字搜。

| 对象 | 一句话 | 驱动方式 |
| --- | --- | --- |
| [Super Barrier](#1-super-barrier护栏) | 防撞栏 / 隔离栏 / 场地围挡 | 样条 |
| [Super Stage Roof](#2-super-stage-roof舞台屋顶) | 舞台屋顶系统 | 参数 |
| [Super Grandstand](#3-super-grandstand看台) | 阶梯看台 | 参数 |
| [Super Stair Tower](#4-super-stair-tower楼梯塔) | 楼梯塔 | 参数 |
| [Super Trackway](#5-super-trackway铺板) | 地面铺板 / 过路板 | 样条 |
| [Super Cable Run](#6-super-cable-run线缆走线) | 线缆走线 | 样条 |
| [Super Ballast](#7-super-ballast配重) | 配重排布 | 参数 |

> ⚠️ 与其它结构类资产一致：这些对象的重量、载荷、配重与视线数据都是**基于简化模型的参考估算**，用于预演阶段的量级参考与统计报表。**它们不构成结构安全结论，不能替代结构设计、力学复核或施工安全审批。**

---

## 1. Super Barrier（护栏）

样条驱动。沿样条铺一排护栏面板。

| 分组 | 参数 |
| --- | --- |
| **Type** | Barrier Type（防撞栏 / 隔离栏 / 场地围挡）、Infill Mode（填充样式） |
| **Dimensions** | Panel Width、Panel Height、Foot Depth、Panel Gap、Post Diameter、Rail Diameter、Infill Bar Count、Mesh Wire Rows |
| **Layout** | Foot Side（脚朝哪边）、Lateral Offset、Start Offset、Gap Panel Indices（哪几块留空当通道口） |
| **Ground Snap** | Snap to Ground、Ground Collision Channel、Raycast Start Height、Raycast Depth、Default Ground Height、Ground Offset、Align To Ground Normal |
| **Visibility** | Show Posts / Rails / Infill / Feet / Braces |
| **Material** | Frame / Infill / Foot |

**要点**：

- `Gap Panel Indices` 用来开通道口——填第几块就空出第几块，不用把样条切断；
- `Foot Side` 决定支脚朝观众侧还是舞台侧，这在防撞栏上是有讲究的；
- 开 `Snap to Ground` 后护栏会贴合地形，配合 `Align To Ground Normal` 可以让面板跟着坡度倾斜。

---

## 2. Super Stage Roof（舞台屋顶）

参数驱动。生成带角塔的舞台屋顶系统。

| 分组 | 参数 |
| --- | --- |
| **Main** | Roof Shape（平顶 / 山形 / 拱形）、Span、Depth、Clear Height、Roof Rise |
| **Profile** | Tower Section、Truss Size、Brace Pattern、Rib Count、Rib Segments、Tower Bays |
| **Extras** | Show PA Wings、PA Wing Reach / Depth / Taper / Hang Points、Show Backdrop Frame、Backdrop Mullion Count |
| **Skin** | Show Roof Skin、Skin Thickness、Skin Overhang |
| **Load** | Suspended Load、Design Wind Speed |
| **Visibility** | Show Towers / Purlins / Base Plates |
| **Material** | Chord / Brace / Skin / Base Plate |

**要点**：

- `Clear Height` 是净高（地面到屋面下沿），`Roof Rise` 是屋面本身的起拱高度，两者分开；
- **PA 翼**（PA Wings）是挂音响的侧翼，`PA Wing Hang Points` 给出吊点数；
- `Design Wind Speed` 只参与统计估算，**不是抗风认证**。

---

## 3. Super Grandstand（看台）

参数驱动。生成阶梯看台，带通道自动切分与视线校核显示。

| 分组 | 参数 |
| --- | --- |
| **Seating** | Row Count、Seats Per Row、Seat Width、Row Depth、Row Rise |
| **Aisle** | Enable Aisles、Seats Per Block、Aisle Width |
| **Sightline** | Focus Distance、Eye Height |
| **Structure** | Deck Thickness、Post Diameter、Rail Diameter、Post Every N Rows、Post Spacing X |
| **Visibility** | Show Riser Boards、Show Railing、Railing Height、Show Front Guardrail、Show Seat Markers、Show Support Frame |
| **Material** | Deck / Tube / Seat Marker |

**要点**：

- 开 `Enable Aisles` 后，按 `Seats Per Block` 自动把每排切成若干块，块之间留 `Aisle Width` 的通道；
- **视线升起余量**由 `Focus Distance`（看向舞台上哪一点）与 `Eye Height`（坐姿眼高）算出并显示。它是**预演参考**，不是观演视线设计规范的合规判定；
- `Row Rise` 是每排的抬升高度，直接决定后排能不能看过前排。

---

## 4. Super Stair Tower（楼梯塔）

参数驱动。单跑直上或双跑折返。

| 分组 | 参数 |
| --- | --- |
| **Main** | Flight Mode（单跑 / 双跑折返）、Total Height、Target Riser、Going、Steps Per Flight |
| **Dimensions** | Flight Width、Lane Gap、Platform Depth、Tread Thickness、Post Diameter、Rail Diameter、Ledger Spacing |
| **Railing** | Show Railing、Railing Height、Mid Rail Count |
| **Visibility** | Show Diagonals、Show Base Plates |
| **Material** | Tube / Deck / Base Plate |

**要点**：

- `Target Riser` 是**目标**踏步高，不是最终值——总高按它取整成级数后，实际踏步高会略有出入。面板会显示实际的踏步与坡度参数；
- `Going` 是踏面进深。踏步高与踏面进深的组合决定坡度是否好走；
- 双跑折返模式下 `Lane Gap` 是两跑之间的间隙，`Platform Depth` 是折返平台的进深。

---

## 5. Super Trackway（铺板）

样条驱动。沿样条铺地面板，多车道并排。

| 分组 | 参数 |
| --- | --- |
| **Spec** | Panel Type |
| **Dimensions** | Panel Length、Panel Width、Panel Thickness、Lane Count、Panel Gap、Lane Gap |
| **Layout** | Stagger Joints、Lateral Offset、Start Offset |
| **Ground Snap** | 同 Barrier 的一组 |
| **Material** | Panel |

**要点**：

- `Stagger Joints` 开启后相邻车道错缝，接缝不会连成一条直线；
- `Lane Count` 是并排几条，`Lane Gap` 是车道之间的缝；
- 可贴合地形。

---

## 6. Super Cable Run（线缆走线）

样条驱动。两种敷设方式。

| 分组 | 参数 |
| --- | --- |
| **Route** | Route Mode（悬空垂坠 / 地面过线板）、Support Spacing、Sag Ratio、Segments Per Span |
| **Cable** | Cable Count、Cable Diameter、Cable Spacing、Weight Per Meter、Vary Sag Per Cable |
| **Support** | Show Supports、Support Size、Ramp Width、Ramp Height、Ramp Section Length |
| **Material** | Cable / Support |

**要点**：

- **悬空模式**下 `Sag Ratio` 控制垂度，`Support Spacing` 控制支撑点间距——两者一起决定线缆下垂的样子；
- 开 `Vary Sag Per Cable` 后每根线的垂度略有差异，比整齐划一自然；
- **地面模式**用过线板（Ramp）压线，`Ramp Width / Height / Section Length` 是板的规格；
- `Weight Per Meter` 只参与重量统计。

---

## 7. Super Ballast（配重）

参数驱动。摆配重块，并可按输入的倾覆力矩反算所需配重量。

| 分组 | 参数 |
| --- | --- |
| **Spec** | Block Type、Layout |
| **Layout** | Count X、Count Y、Layer Count、Block Gap、Centered |
| **Dimensions** | Block Length、Block Width、Block Height、Block Weight |
| **Requirement** | Compute Required Mass、Overturning Moment、Lever Arm、Safety Factor |
| **Material** | Block |

**要点**：

- 开 `Compute Required Mass` 后，填入 **Overturning Moment**（倾覆力矩，kN·m）、**Lever Arm**（力臂，m）与 **Safety Factor**，面板给出所需配重量；
- 这个数字是**简化公式的量级参考**。⚠️ **它不是抗倾覆校核结论，不能作为现场配重方案的依据。** 真实抗倾覆必须由有资质的结构工程师按实际荷载工况计算；
- `Layer Count` 是堆几层，`Centered` 决定配重阵列以 Actor 原点居中还是从原点向外延伸。

---

## 8. 通用说明

**样条编辑**：Barrier / Trackway / Cable Run 三个是样条驱动。选中 Actor 后在视口拖样条控制点即可改路径，右键控制点可增删。

**地形贴合**：Barrier 与 Trackway 支持 `Snap to Ground`。它靠碰撞通道向下打射线，所以地形必须有碰撞。

**统计信息**：各对象提供只读的构件数量与重量估算。数据仅供预可视化参考。

**场景复杂度**：这些对象都用实例化静态网格渲染。参数放得很大时（例如几百块铺板 × 多车道）构件总数会显著增长，建议先用较小规模确认布局。

---

## 9. 相关文档

- [舞台资产总览](00_StageAssets_Overview_zh.md)
- [Super Truss 桁架龙门架](01_SuperTruss_zh.md)
- [Super Scaffold 直线脚手架](02_SuperScaffold_zh.md)
- [Super Stage Floor 舞台地板](07_SuperStageFloor_zh.md)
- [Super Crowd 人群](12_SuperCrowd_zh.md)
