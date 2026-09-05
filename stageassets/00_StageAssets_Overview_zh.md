# SuperStage — Stage Assets 舞台资产用户手册总览

## 1. 简介

**Stage Assets** 是 SuperStage 插件的舞台资产模块，提供一组程序化舞台设备生成工具。用户可以在 Unreal Engine 编辑器中通过参数调整生成舞台结构和设备的 3D 模型，用于演出设计预可视化。

Stage Asset 是可放入场景的 UE Actor，可以移动、旋转、缩放，并在 Details 面板中调整参数。多数参数修改会触发编辑器中的模型重建或预览更新；复杂资产和大场景的更新时间取决于项目规模和机器性能。

> **兼容性**：请以当前分支和插件描述文件标注的 UE 版本为准。

---

## 2. 资产一览

Stage Assets 模块包含以下 19 个可用 Actor（含 26H2.5 新增的 7 个程序化对象）。

> 内容库已不再随包提供 Band、AudioSystem、AircraftCase 与控制台模型。**观众角色与 StageVFX 特效资源仍随包提供**（13 个 VAT 角色、7 类特效的 Niagara 系统与配套灯库），见 [Super Crowd](12_SuperCrowd_zh.md) 与 [舞台 VFX](../stagecore/14_Stage_VFX_zh.md)。下表只列出程序化生成的结构类 Actor。

### 2.1 结构类资产

| Actor | 名称 | 功能简述 |
|-------|------|----------|
| **Super Truss Gantry** | [桁架龙门架](01_SuperTruss_zh.md) | 程序化桁架龙门架，支持多种截面（方/三角/平面）、规格（290/400/520mm）和造型（门形/T形/Portal/双跨），提供简化载荷与挠度估算 |
| **Super Circular Truss** | [圆形桁架](08_SuperCircularTruss_zh.md) | 极坐标定位的圆形/环形桁架，支持双环结构 + 辐射撑 + 连接法兰 |
| **Super Curved Truss** | [弧形桁架](09_SuperCurvedTruss_zh.md) | 样条驱动的自由曲线桁架，截面始终垂直于切线方向 |
| **Super Truss Grid** | [桁架网格](10_SuperTrussGrid_zh.md) | 水平双层网格桁架，用于大面积灯光吊挂系统 |
| **Super Truss Tower** | [桁架塔](11_SuperTrussTower_zh.md) | 垂直桁架立柱，提供简化载荷统计 |
| **Super Scaffold** | [直线脚手架](02_SuperScaffold_zh.md) | 程序化矩形脚手架系统，包含立杆、横杆、斜撑、底座板、平台板和配重块，提供载荷与配重估算 |
| **Super Curved Scaffold** | [弧度脚手架](03_SuperCurvedScaffold_zh.md) | 样条驱动的弧形脚手架系统，通过编辑样条曲线生成弯曲脚手架结构，其余功能与直线脚手架接近 |
| **Super Stage Floor** | [舞台地板](07_SuperStageFloor_zh.md) | 程序化舞台地板系统，含面板/支腿/横撑/裙边/台阶，提供重量与载荷估算 |

### 2.2 软装类资产

| Actor | 名称 | 功能简述 |
|-------|------|----------|
| **Super Drape** | [舞台幕布](04_SuperDrape_zh.md) | 程序化舞台幕布/帘幕，支持 6 种幕布类型、5 种面料、4 种褶皱样式和 4 种开合方式，带吊杆和系绳，内置面料重量计算 |
| **Super Crowd** | [程序化人群](12_SuperCrowd_zh.md) | 泊松圆盘采样的人群生成，样条定义区域，多角色权重分配，地形贴合；需要用户自行指定角色 Static Mesh |

### 2.3 视频/投影类资产

| Actor | 名称 | 功能简述 |
|-------|------|----------|
| **Super Projector** | [投影仪](05_SuperProjector_zh.md) | 投影映射模拟工具，单白色聚光灯光照函数投影，支持静态纹理和导播相机，支持梯形校正 |
| **Super Screen** | [媒体屏幕](06_SuperScreen_zh.md) | 媒体源显示工具（静态纹理/导播相机），支持多屏同步、透明模式和颜色调整 |

### 2.4 舞台程序化对象（26H2.5 新增）

| Actor | 名称 | 功能简述 |
|-------|------|----------|
| **Super Barrier** | [护栏](13_StageProgramObjects_zh.md#1-super-barrier护栏) | 样条驱动护栏，防撞栏 / 隔离栏 / 场地围挡三种形式，可贴合地形 |
| **Super Stage Roof** | [舞台屋顶](13_StageProgramObjects_zh.md#2-super-stage-roof舞台屋顶) | 平顶 / 山形 / 拱形三种屋面，含角塔、屋面桁架、顶膜、PA 翼与背景框 |
| **Super Grandstand** | [看台](13_StageProgramObjects_zh.md#3-super-grandstand看台) | 阶梯看台，含通道自动切分与视线升起余量显示 |
| **Super Stair Tower** | [楼梯塔](13_StageProgramObjects_zh.md#4-super-stair-tower楼梯塔) | 单跑直上或双跑折返，显示踏步与坡度参数 |
| **Super Trackway** | [铺板](13_StageProgramObjects_zh.md#5-super-trackway铺板) | 样条铺板，多车道并排可错缝，可贴合地形 |
| **Super Cable Run** | [线缆走线](13_StageProgramObjects_zh.md#6-super-cable-run线缆走线) | 悬空垂坠与地面过线板两种敷设 |
| **Super Ballast** | [配重](13_StageProgramObjects_zh.md#7-super-ballast配重) | 配重排布，可按输入的倾覆力矩显示所需配重量 |


---

## 3. 通用操作指南

### 3.1 放置 Actor

所有 Stage Asset Actor 均可通过以下方式放置到场景中：

1. **放置面板** — 在编辑器左侧"放置 Actor"面板中搜索 Actor 名称，拖入视口
2. **内容浏览器** — 在内容浏览器中找到对应的资产类型，拖入视口
3. **右键菜单** — 在视口中右键 → 放置 Actor → 搜索对应名称

### 3.2 调整参数

1. 在视口中**选中** Actor
2. 在右侧**细节面板**（Details Panel）中查看所有可调参数
3. 修改参数后，场景中的模型通常会在编辑器中更新
4. 参数按功能分组（结构、材质、可视化、统计等），可折叠/展开各组

### 3.3 材质指定

所有 Stage Asset 都支持为不同构件分别指定材质：

1. 在细节面板中找到"Materials"分组
2. 点击材质属性旁的下拉箭头
3. 从项目中选择材质资源
4. 留空则使用 UE 引擎默认材质

### 3.4 统计信息

部分结构类资产和幕布资产提供只读的统计面板：

- **构件数量** — 各类构件的实例数量和总数
- **重量/载荷统计** — 基于内置参数的自重、载荷、配重等估算结果
- **物理参数** — 当前选用的材料/面料的详细物理参数

> ⚠️ **所有统计数据仅供预可视化参考，不可替代专业工程计算。**

---

## 4. 参考参数来源

Stage Assets 模块的参数和默认值参考了以下行业资料。它们用于预可视化统计，不代表插件输出工程认证结果。

| 标准 | 适用资产 | 说明 |
|------|----------|------|
| **DIN 4113** | Super Truss 系列 | 作为桁架材料和挠度估算的参考来源 |
| **EN 12811-1** | Super Scaffold, Super Curved Scaffold | 作为脚手架荷载等级参考来源 |
| **EN 10210** | Super Scaffold, Super Curved Scaffold | 热轧空心截面标准，作为钢管壁厚参考来源 |
| **BS 1139** | Super Scaffold | 英国脚手架管件标准，定义 Ø48.3mm 标准管径 |
| **ANSI E1.21** | Super Stage Floor | 作为临时地板/舞台载荷估算的参考来源 |

---

## 5. 场景复杂度注意事项

### 5.1 实例化渲染

结构类资产（桁架、脚手架）使用 **Instanced Static Mesh Component (ISM)** 技术渲染：

- **作用**：同类型构件可以共享实例化渲染，减少重复网格带来的开销
- **注意**：构件总数超过 10,000 时仍可能影响编辑器帧率

### 5.2 程序化网格

幕布资产使用 **Procedural Mesh Component** 渲染：

- **作用**：根据当前幕布尺寸、褶皱类型和细分参数生成褶皱表面
- **注意**：褶皱数量过多（>100）或开启双面渲染会增加三角面数量

### 5.3 建议

- 远景中的脚手架可适当减少跨数和层数
- 不可见面的幕布可关闭双面渲染
- 大场景中避免同时放置过多高复杂度的 Stage Asset

---

## 6. 文档目录

| 序号 | 文档名称 | 内容 |
|------|----------|------|
| 00 | [总览](00_StageAssets_Overview_zh.md) | 本文档。Stage Assets 模块概述、通用操作指南 |
| 01 | [桁架龙门架](01_SuperTruss_zh.md) | Super Truss Gantry 用户说明 |
| 02 | [直线脚手架](02_SuperScaffold_zh.md) | Super Scaffold 用户说明 |
| 03 | [弧度脚手架](03_SuperCurvedScaffold_zh.md) | Super Curved Scaffold 用户说明 |
| 04 | [舞台幕布](04_SuperDrape_zh.md) | Super Drape 用户说明 |
| 05 | [投影仪](05_SuperProjector_zh.md) | Super Projector 用户说明 |
| 06 | [媒体屏幕](06_SuperScreen_zh.md) | Super Screen 用户说明 |
| 07 | [舞台地板](07_SuperStageFloor_zh.md) | Super Stage Floor 用户说明 |
| 08 | [圆形桁架](08_SuperCircularTruss_zh.md) | Super Circular Truss 用户说明 |
| 09 | [弧形桁架](09_SuperCurvedTruss_zh.md) | Super Curved Truss 用户说明 |
| 10 | [桁架网格](10_SuperTrussGrid_zh.md) | Super Truss Grid 用户说明 |
| 11 | [桁架塔](11_SuperTrussTower_zh.md) | Super Truss Tower 用户说明 |
| 12 | [程序化人群](12_SuperCrowd_zh.md) | Super Crowd 用户说明 |
| 13 | [舞台程序化对象](13_StageProgramObjects_zh.md) | 护栏 / 屋顶 / 看台 / 楼梯塔 / 铺板 / 线缆走线 / 配重（26H2.5 新增） |

---

## 7. 版本信息

- **插件名称**：SuperStage
- **模块**：Stage Assets
- **兼容引擎**：以当前分支和插件描述文件为准
- **文档版本**：2.0
- **最后更新**：2026-04
- **开发团队**：LimxTeam
