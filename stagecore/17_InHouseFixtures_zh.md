# 17 - 自研灯具（SuperLight）

> **所属模块**: SuperAssets ｜ **适用版本**: SuperStage 26H2.6 起
> **前置阅读**: [03 - DMX 灯具基础](03_DMX_Actor_Base_zh.md)

---

## 一、这是什么

除了从 GDTF 导入的 807 个厂商型号，SuperStage 还自带 **16 支自研灯具**，分七个系列。它们在资产浏览器的 **SuperLight** 分类下。

与灯库里的型号不同，自研灯具是**手写的 C++ 灯具**：外形、组件结构和通道映射都写死在代码里，不通过灯具定义资产描述。所以：

- 它们**不出现在灯具编辑器里**，也不能用 GDTF 重导；
- 它们各自绑定随包的通道库（`SL_*`），可以正常配接、正常被控台驱动；
- 它们不是某个真实厂商型号的复刻，而是通用的舞台效果器件。

需要某个具体厂商型号时，请用灯库里的型号，或[从 GDTF 导入](../fixture/02_GdtfImport_zh.md)。

---

## 二、清单

| 系列 | 型号 | 通道 | 用途 |
| --- | --- | ---: | --- |
| **Aurora** | Aurora_Fan200 | 8CH | 风扇灯（小） |
| | Aurora_Fan400 | 8CH | 风扇灯（大） |
| **Blaze** | BlazeAtomic3K | 5CH | 爆闪 |
| | BlazeStrip1000 | 5CH | 频闪条 |
| | BlazeMatrixStrip20 | 63CH | 矩阵频闪条（20 段） |
| **Flare** | Flare_RGBW600 | 7CH | RGBW 染色帕灯 |
| | Flare_WashMH600 | 11CH | 摇头染色灯 |
| **Hyperion** | Hyperion550Beam | 15CH | 光束灯 |
| | Hyperion700Profile | 25CH | 成像切割灯 |
| **Spark** | Spark_P1Evo | 3CH | 影视灯 |
| | Spark_V1 | 5CH | 影视灯 |
| | Spark_Vintage300 | 4CH | 复古影视灯 |
| | Spark_Profile600 | 15CH | 影视成像灯 |
| **Thunder** | Thunder_Sidewinder10 | 53CH | 光束条（10 头） |
| **Machinery** | Machinery_LiftBall | 9CH | 升降效果球 |
| | LiftMatrix | 42CH | 升降灯光矩阵 |

通道数以随包通道库为准；配接前用 [Patch 工具](07_Patch_Tools_zh.md)确认实际跨度。

---

## 三、放置与配接

与其它 DMX 灯具完全一致：

1. 从资产浏览器 **SuperLight** 分类拖进场景；
2. 设置 **Universe**、**Start Address**、**ControlMode**，按上表预留通道；
3. 从控台驱动。

它们出厂已绑定各自的通道库，不需要手工指定。

---

## 四、光束管线

自研灯具在 26H2.6 已按用途分配光束类型：

| 灯具类型 | 光束组件 |
| --- | --- |
| 光束灯（Hyperion550Beam 等） | 体积光束 |
| 成像切割灯（Hyperion700Profile、Spark_Profile600 等） | 体积切割 |
| 矩阵条（BlazeMatrixStrip20、Thunder_Sidewinder10） | 矩阵组件 |
| 效果类（Aurora 风扇灯、LiftBall / LiftMatrix 的发光单元） | 效果平面组件 |

各组件的参数说明见 [灯光组件](../lightcomponent/00_LightComponent_Overview_zh.md)。

> 部分自研灯具仍使用材质切割组件（Legacy）。它们是手写 C++ 灯具，不受"随包灯库已全量迁移体积管线"那条的覆盖——那条说的是灯具定义资产。

---

## 五、两支机械灯具

**LiftMatrix**（42CH）与 **LiftBall**（9CH）是带钢丝绳视觉的升降灯具，升降与效果颜色都由 DMX 驱动。

LiftMatrix 的结构与通道行为另有专篇：[12 - 升降矩阵](12_Lift_Matrix_zh.md)。

---

## 六、常见问题

### 在灯具编辑器里打不开

这是预期行为。自研灯具是 C++ 灯具，没有灯具定义资产，所以不进灯具编辑器。要查它的通道，在内容浏览器里打开它绑定的通道库（`SL_*`）。

### 想改它的外形 / 通道

自研灯具的结构写在代码里，面板上只有它自己暴露的参数。需要一支可自由编辑的灯，请建[数据驱动灯具定义](../fixture/01_FixtureDefinition_zh.md)。

### 控台推得动但没反应

按通用顺序排查：`ControlMode` 是否为 `DMX`、Universe 与起始地址是否对得上、通道是否落在这支灯的跨度内。见 [03 - DMX 灯具基础](03_DMX_Actor_Base_zh.md)。

---

## 七、相关文档

- [03 - DMX 灯具基础](03_DMX_Actor_Base_zh.md)
- [07 - Patch 工具](07_Patch_Tools_zh.md)
- [12 - 升降矩阵](12_Lift_Matrix_zh.md)
- [灯具系统总览](../fixture/00_FixtureSystem_Overview_zh.md)
- [灯光组件](../lightcomponent/00_LightComponent_Overview_zh.md)
