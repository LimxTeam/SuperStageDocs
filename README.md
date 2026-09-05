# SuperStage 用户文档

> 当前版本：SuperStage 26H2.6 ｜ [English](README_en.md)

本目录是 SuperStage 的用户手册入口。文档面向灯光设计、预演、视频、项目交付和编辑器操作人员，不要求读者阅读代码。

文档内容应遵守以下规则：

- 只描述当前实现、资源或插件配置中可以确认的功能。
- 不写固定性能、固定延迟、固定帧率或固定硬件规模承诺。
- 不把已删除模块写进当前功能说明。
- 不把内部实现细节、类名、宏和 C++ 用法放进普通用户手册，除非用户必须看到这个名称才能操作。

每篇文档都有中文（`_zh`）和英文（`_en`）两个版本。

## 主插件模块

SuperStage 是单一插件，当前包含以下模块，全部随主插件交付：

| 模块 | 类型 | 职责 |
| --- | --- | --- |
| SuperCore | 运行时 | 灯具、光源、光束、喷泉等舞台对象核心 |
| SuperDMX | 运行时 | Art-Net / sACN 收发、Universe 缓存、Sequencer DMX 轨道 |
| SuperNdi | 运行时 | NDI 输入与录制 |
| SuperMadrix | 运行时 | LED 矩阵可视化 |
| SuperLaser | 运行时 | 激光呈现、激光动画资产与运行时 |
| SuperShader | 运行时 | 舞台光束、锥形光、喷泉等渲染管线 |
| SuperAuth | 运行时 | 账户与授权 |
| SuperAssets | 运行时 | 舞台结构、程序化舞美与自研灯具 |
| SuperTools | 编辑器 | 资产浏览器、配接、灯具编辑器、GDTF / MVR / grandMA 工具面板 |
| SuperConsole | 编辑器 | 灯光控台 |

---

## 从这里开始

| 我要…… | 看这篇 |
| --- | --- |
| 了解产品能做什么 | [产品文档](SuperStageProductDoc_zh.md) |
| 知道这一版改了什么 | [更新日志](changelog_zh.md) |
| 搭场景、放灯 | [资产浏览器](editortools/02_AssetBrowser_zh.md) |
| 灯库里没有我要的灯 | [GDTF 导入](fixture/02_GdtfImport_zh.md) |
| 没有 GDTF，要从零建一支灯 | [灯具构建器](fixture/06_FixtureBuilder_zh.md) |
| 接控台、调 DMX | [DMX 系统总览](stagecore/00_DMX_System_Overview_zh.md) |
| 不接外部控台自己编程 | [控台总览](console/00_Console_Overview_zh.md) |
| 做激光内容 | [激光总览](laser/00_Laser_Overview_zh.md) |
| 接 NDI 视频信号 | [NDI 输入配置](editortools/04_NDIConfigPanel_zh.md) |
| 放特效机、喷泉 | [舞台 VFX](stagecore/14_Stage_VFX_zh.md)、[舞台喷泉](stagecore/16_Fountain_zh.md) |

---

## 文档分类

### 概览

- `SuperStageProductDoc_zh.md` — 产品定位、能力范围、边界与资源清单
- `changelog_zh.md` — 版本更新日志

### 灯具系统（26H2.6 重做）

一支灯不再是蓝图，而是一份可导入、可编辑、可升级的灯具定义。

- [`fixture/00_FixtureSystem_Overview_zh.md`](fixture/00_FixtureSystem_Overview_zh.md) — 三种资产、它们的分工、编译
- [`fixture/01_FixtureDefinition_zh.md`](fixture/01_FixtureDefinition_zh.md) — 定义资产逐字段说明
- [`fixture/02_GdtfImport_zh.md`](fixture/02_GdtfImport_zh.md) — 单个导入、批量导入、导入范围、原地升级
- [`fixture/03_FixtureEditor_zh.md`](fixture/03_FixtureEditor_zh.md) — 灯具编辑器四个页签
- [`fixture/04_Motion_zh.md`](fixture/04_Motion_zh.md) — Pan/Tilt 行程、速度通道、无极旋转、多头灯
- [`fixture/05_AttributeNames_zh.md`](fixture/05_AttributeNames_zh.md) — 119 个 DMX 属性名参考
- [`fixture/06_FixtureBuilder_zh.md`](fixture/06_FixtureBuilder_zh.md) — 手工建灯：各类灯具的完整配方

### 灯光控台（26H2.6 并入主插件）

- [`console/00_Console_Overview_zh.md`](console/00_Console_Overview_zh.md) — 入口、界面、上手流程、边界
- [`console/01_Programming_zh.md`](console/01_Programming_zh.md) — 选灯、编程器、编组、预设、槽位外观
- [`console/02_Cues_and_Playback_zh.md`](console/02_Cues_and_Playback_zh.md) — CUE、执行器、输出仲裁、时间线、时间码
- [`console/03_CommandLine_zh.md`](console/03_CommandLine_zh.md) — 模式 × 目标矩阵
- [`console/04_Effects_zh.md`](console/04_Effects_zh.md) — Frame 效果引擎
- [`console/05_ShowFile_and_Undo_zh.md`](console/05_ShowFile_and_Undo_zh.md) — 演出文件格式与撤销

### 激光

- [`laser/00_Laser_Overview_zh.md`](laser/00_Laser_Overview_zh.md) — 三层能力与安全边界
- [`laser/01_LaserAnimationAsset_zh.md`](laser/01_LaserAnimationAsset_zh.md) — 激光动画资产
- [`laser/02_CanvasEditor_zh.md`](laser/02_CanvasEditor_zh.md) — 画布编辑器
- [`laser/03_Effects_zh.md`](laser/03_Effects_zh.md) — 六类效果
- [`laser/04_Baking_and_ILDA_zh.md`](laser/04_Baking_and_ILDA_zh.md) — 烘焙与 ILDA

### DMX 与灯具

- [`stagecore/00_DMX_System_Overview_zh.md`](stagecore/00_DMX_System_Overview_zh.md)
- [`stagecore/01_DMX_Network_Configuration_zh.md`](stagecore/01_DMX_Network_Configuration_zh.md)
- [`stagecore/02_Fixture_Library_zh.md`](stagecore/02_Fixture_Library_zh.md) — 通道库
- [`stagecore/03_DMX_Actor_Base_zh.md`](stagecore/03_DMX_Actor_Base_zh.md)
- [`stagecore/06_DMX_Activity_Monitor_zh.md`](stagecore/06_DMX_Activity_Monitor_zh.md)
- [`stagecore/07_Patch_Tools_zh.md`](stagecore/07_Patch_Tools_zh.md)
- [`stagecore/08_DMX_Recording_Playback_zh.md`](stagecore/08_DMX_Recording_Playback_zh.md)
- [`stagecore/09_Export_To_MA_zh.md`](stagecore/09_Export_To_MA_zh.md)
- [`stagecore/10_Stage_Machinery_zh.md`](stagecore/10_Stage_Machinery_zh.md)
- [`stagecore/11_GrandMA_Link_zh.md`](stagecore/11_GrandMA_Link_zh.md)
- [`stagecore/12_Lift_Matrix_zh.md`](stagecore/12_Lift_Matrix_zh.md)
- [`stagecore/13_Light_Strip_Effect_zh.md`](stagecore/13_Light_Strip_Effect_zh.md)
- [`stagecore/14_Stage_VFX_zh.md`](stagecore/14_Stage_VFX_zh.md)
- [`stagecore/16_Fountain_zh.md`](stagecore/16_Fountain_zh.md) — 舞台喷泉（26H2.6 新增）
- [`stagecore/17_InHouseFixtures_zh.md`](stagecore/17_InHouseFixtures_zh.md) — 自研灯具（Aurora / Blaze / Flare / Hyperion / Spark / Thunder / Machinery）

### 灯光组件

- [`lightcomponent/00_LightComponent_Overview_zh.md`](lightcomponent/00_LightComponent_Overview_zh.md) — 继承关系与本版变化
- `lightcomponent/01_SuperLightingComponent_zh.md`
- `lightcomponent/02_SuperSpotComponent_zh.md`
- `lightcomponent/03_SuperBeamComponent_zh.md` — 材质光束（Legacy）
- `lightcomponent/04_SuperShaperComponent_zh.md` — 材质切割（Legacy，原 Cutting）
- `lightcomponent/06_SuperEffectComponent_zh.md`
- `lightcomponent/07_SuperMatrixComponent_zh.md`
- `lightcomponent/09_SuperLiftComponent_zh.md`
- `lightcomponent/10_SuperConeLightComponent_zh.md` — 自研锥形光（26H2.6 新增）
- `lightcomponent/11_SuperVolumetricBeamComponent_zh.md` — 体积光束
- `lightcomponent/12_SuperVolumetricShaperComponent_zh.md` — 体积切割
- `lightcomponent/13_SuperRayBeamComponent_zh.md` — 纯光束
- `lightcomponent/14_SuperWashComponent_zh.md` — Wash

### 编辑器工具

- [`editortools/01_MainMenu_and_Toolbar_zh.md`](editortools/01_MainMenu_and_Toolbar_zh.md)
- [`editortools/02_AssetBrowser_zh.md`](editortools/02_AssetBrowser_zh.md)
- [`editortools/03_DMXConfigPanel_zh.md`](editortools/03_DMXConfigPanel_zh.md)
- [`editortools/04_NDIConfigPanel_zh.md`](editortools/04_NDIConfigPanel_zh.md) — NDI 输入配置
- [`editortools/05_BatchPatchTool_zh.md`](editortools/05_BatchPatchTool_zh.md)
- [`editortools/06_DMXPatchPreview_zh.md`](editortools/06_DMXPatchPreview_zh.md)
- [`editortools/07_DMXExportMA_zh.md`](editortools/07_DMXExportMA_zh.md)
- [`editortools/08_MVRImport_zh.md`](editortools/08_MVRImport_zh.md)
- [`editortools/09_VATGenerator_zh.md`](editortools/09_VATGenerator_zh.md) — VAT 角色生成器
- [`editortools/10_FixtureLibraryEditor_zh.md`](editortools/10_FixtureLibraryEditor_zh.md) — 通道库编辑器
- [`editortools/11_FixtureArrayTool_zh.md`](editortools/11_FixtureArrayTool_zh.md)
- [`editortools/12_SplineFixtureDistribution_zh.md`](editortools/12_SplineFixtureDistribution_zh.md)
- [`editortools/13_ColorAtlasBuilder_zh.md`](editortools/13_ColorAtlasBuilder_zh.md)
- [`editortools/14_GoboAtlasBuilder_zh.md`](editortools/14_GoboAtlasBuilder_zh.md)
- [`editortools/15_DMXActivityMonitor_zh.md`](editortools/15_DMXActivityMonitor_zh.md)
- [`editortools/17_UserAuth_and_Subscription_zh.md`](editortools/17_UserAuth_and_Subscription_zh.md)
- [`editortools/18_PrismPresetEditor_zh.md`](editortools/18_PrismPresetEditor_zh.md)

> 14 个工具面板右上角都有帮助按钮，内容与本目录文档一致。

### 舞台资产

- [`stageassets/00_StageAssets_Overview_zh.md`](stageassets/00_StageAssets_Overview_zh.md)
- [`stageassets/01_SuperTruss_zh.md`](stageassets/01_SuperTruss_zh.md) — 桁架与龙门架
- [`stageassets/02_SuperScaffold_zh.md`](stageassets/02_SuperScaffold_zh.md) — 脚手架
- [`stageassets/03_SuperCurvedScaffold_zh.md`](stageassets/03_SuperCurvedScaffold_zh.md) — 曲面脚手架
- [`stageassets/04_SuperDrape_zh.md`](stageassets/04_SuperDrape_zh.md) — 幕布
- [`stageassets/05_SuperProjector_zh.md`](stageassets/05_SuperProjector_zh.md) — 投影仪
- [`stageassets/06_SuperScreen_zh.md`](stageassets/06_SuperScreen_zh.md) — 屏幕
- [`stageassets/07_SuperStageFloor_zh.md`](stageassets/07_SuperStageFloor_zh.md) — 舞台地板
- [`stageassets/08_SuperCircularTruss_zh.md`](stageassets/08_SuperCircularTruss_zh.md) — 环形桁架
- [`stageassets/09_SuperCurvedTruss_zh.md`](stageassets/09_SuperCurvedTruss_zh.md) — 曲线桁架
- [`stageassets/10_SuperTrussGrid_zh.md`](stageassets/10_SuperTrussGrid_zh.md) — 桁架网格
- [`stageassets/11_SuperTrussTower_zh.md`](stageassets/11_SuperTrussTower_zh.md) — 桁架塔
- [`stageassets/12_SuperCrowd_zh.md`](stageassets/12_SuperCrowd_zh.md) — 观众人群
- [`stageassets/13_StageProgramObjects_zh.md`](stageassets/13_StageProgramObjects_zh.md) — 护栏 / 屋顶 / 看台 / 楼梯塔 / 铺板 / 线缆走线 / 配重
- [`stageassets/14_StageDevices_zh.md`](stageassets/14_StageDevices_zh.md) — 导播相机 / Madrix / 白模渲染

舞台结构类文档只能说明 UE 场景建模和统计显示，不应承诺真实结构安全或法规合规。

### 渲染系统说明

- [`SuperVolumetricBeam.md`](SuperVolumetricBeam.md) — 体积光束的调优参考：运行时可调的控制台变量、诊断命令与已知边界

### 法务与合作政策

- `legal/terms_zh.md`、`legal/privacy_zh.md`
- `SuperStageTeam政策.md`、`SuperStage授权经销商合作政策.md`、`SuperStage教育机构合作政策.md`

> **注意**：蓝图灯具已在 26H2.6 废弃，其文档（原 `stagecore/04` 电脑灯、`stagecore/15` SuperStageLight）已删除。灯具相关内容一律以 `fixture/` 目录下的文档为准。

---

## 升级到 26H2.6 之前请先读

- **旧项目不做灯具类重定向**。26H2.6 起蓝图灯具已废弃，本目录不再提供其文档；仍在用旧版蓝图灯具的既有项目请继续使用旧版插件，不要就地升级。
- 若此前单独安装过 SuperConsole 插件，升级前请将其从项目 `Plugins` 目录移除。
- **演出文件格式升至 2.7，本版存出的文件旧版本读不了。**
- 详见[产品文档第 9 章](SuperStageProductDoc_zh.md)与[更新日志](changelog_zh.md)。
