# 02 - 通道库 (Fixture Library)

> **适用对象**: 灯光设计师、灯具制作者
> **前置阅读**: [00 - DMX 系统总览](00_DMX_System_Overview_zh.md)、[灯具系统总览](../fixture/00_FixtureSystem_Overview_zh.md)
> **适用版本**: SuperStage 26H2.6 起

> **26H2.6 术语变化**：这份资产在文档中统一称作**通道库**（资产前缀 `CL_`）。它只描述"这支灯的某个 DMX 模式有哪些通道"；描述"这支灯是什么"（外形、机械、发射器、光学）的是**灯具定义**（资产名就是型号名，不带前缀）。
>
> 一份灯具定义可以有多个 DMX 模式，每个模式指向一份通道库；一份通道库可以被多份定义共用。**绑定**（哪条通道驱动哪个能力）住在定义里，不在通道库里。详见[灯具系统总览](../fixture/00_FixtureSystem_Overview_zh.md)。

---

## 一、什么是灯具库

**灯具库 (Fixture Library)** 是 SuperStage 中定义灯具 DMX 通道表的配置文件。它告诉系统：

- 这台灯具有哪些可控功能（亮度、Pan、Tilt、颜色、Gobo 等）
- 每个功能占用哪个通道（相对偏移）
- 每个功能的精度（8位/16位/24位）
- 每个功能中不同值范围对应的含义（例如：通道值 0-127 = 白光开，128-255 = 频闪）

简单来说，灯具库就是灯具的"说明书"——告诉 SuperStage 如何理解控台发来的 DMX 数据。

> **类比**：灯具库就像是 MA 控台中的 Fixture Profile，或 GDTF 文件中的通道定义。

---

## 二、创建灯具库资产

### 步骤

1. 在 Content Browser 中**右键**创建 Super Fixture Library
2. 为资产命名（随包灯库的约定是 `CL_<型号>_<模式名>`）
3. 双击打开资产进行编辑

> 大多数情况下不需要手工建：从 GDTF 导入一支灯会自动为它的每个 DMX 模式生成一份通道库。见 [GDTF 导入](../fixture/02_GdtfImport_zh.md)。

### 命名建议

| 前缀 | 示例 | 说明 |
|------|------|------|
| `CL_` | `CL_MegaPointe_Standard` | 随包灯库使用的约定：`CL_<型号>_<模式名>` |
| 无前缀 | `MegaPointe` | 灯具定义，不是通道库 |
| `LTC_` | `LTC_MegaPointe` | 该型号在内容浏览器里的缩略图 |
| `SP_` | `SP_MegaPointe_Prism` | 棱镜预设 |

---

## 三、灯具库属性详解

双击通道库资产打开专用编辑器。**它不是 UE 的细节面板**，是一个四层的列表编辑器。

### 3.1 头部字段

编辑器顶部一行：

| 字段 | 说明 | 示例 |
|------|------|------|
| **Fixture Name** | 灯具型号名称 | `MegaPointe` |
| **Manufacturer** | 制造商 | `Robe` |
| **Power (W)** | 额定功率（瓦特） | `1700` |
| **Weight (kg)** | 重量（千克） | `36.8` |
| **Channel Count** | 只读，当前模式的通道跨度 | `35` |
| **GDTF Library** | 绑定的 GDTF 文件路径 | `FixtureLibrary/GDTF/...` |

头部右侧还有一个帮助按钮。这些信息用于报表统计和外部交付资料，不影响 DMX 控制逻辑。

### 3.2 DMX 模式列表

灯具库最核心的部分是 **Modules（模块实例列表）**。

每个模块实例代表灯具的一个功能模组。对于大多数灯具，只需要 **一个模块实例**。对于矩阵灯（如 LED 面板灯），每个灯珠/像素点就是一个独立的模块实例。

点击 **Modules** 旁边的 **+** 号添加模块实例。

#### 模块实例属性

| 属性 | 说明 | 示例 |
|------|------|------|
| **Module Name** | 模块名称（可选，便于识别） | `Main` 或 `Pixel_1` |
| **Patch** | 模块相对 StartAddress 的起始偏移。运行时按 `StartAddress + max(0, Patch - 1)` 计算模块基址。单模块灯具通常设为 **1**，矩阵灯每个像素模块使用不同 Patch 值 | `1` |
| **Attribute Defs** | 该模块的属性定义列表（详见下文） | — |

---

## 四、属性定义 (Attribute Defs)

每个模块实例中可以添加多个**属性定义 (Attribute Def)**，每个属性对应灯具的一个可控功能。

新增靠底部工具栏的 **+ Add** 按钮：它按当前选中的层级新增一条——选中模式时加模式，选中属性时加属性，依此类推。

### 4.1 属性基本参数

| 参数 | 说明 | 详细解释 |
|------|------|---------|
| **Attrib Name** | 属性名称 | 必须唯一。绑定按这个名字把通道接到灯具的能力上。建议使用标准命名：`Dimmer`、`Pan`、`Tilt`、`Red`、`Green`、`Blue`、`White`、`Gobo`、`Prism`、`Focus`、`Zoom`、`Shutter`、`ColorWheel` 等 |
| **Coarse** | 粗调通道偏移（1 基） | **必填**。该属性的主通道在模块内的偏移。例如 Coarse=5 表示该属性从模块的第 5 个通道开始 |
| **Fine** | 精调通道偏移（1 基） | **可选**。设为 0 表示无精调通道。用于 16 位精度控制（如 Pan/Tilt） |
| **Ultra** | 超精调通道偏移（1 基） | **可选**。设为 0 表示无超精调通道。用于 24 位精度控制（极少使用） |
| **Category** | 属性分类 | 用于控台 UI 分组显示，不影响功能逻辑 |
| **DefaultValue** | 默认值，按百分比（0-100） | 静息值，即没有任何 DMX 输入时这条通道停在哪。GDTF 导入时从包里声明的初始值取 |
| **HighlightValue** | 高亮值，按百分比（0-100） | 用于控台 Highlight 功能 |

### 4.2 通道偏移详解

通道偏移是**相对于模块 Patch 的偏移**，从 1 开始计数。

**地址计算公式**：
```
绝对通道地址 = 灯具起始地址 (StartAddress) + 模块偏移 (Patch - 1) + 属性偏移 (Coarse - 1)
```

**示例**：一台灯具的 StartAddress = 101，模块 Patch = 1

| 属性 | Coarse | Fine | 实际占用通道 |
|------|--------|------|-------------|
| Dimmer | 1 | 0 | 101 |
| Pan | 2 | 3 | 102-103 |
| Tilt | 4 | 5 | 104-105 |
| Color Wheel | 6 | 0 | 106 |
| Gobo 1 | 7 | 8 | 107-108 |
| Prism | 9 | 0 | 109 |

### 4.3 位深

**位深没有单独的开关**，它由你填了几个偏移决定：

| 填了哪些偏移 | 位深 | 值范围 | 适用场景 |
|------|------|--------|---------|
| 只填 **Coarse** | 8 位 | 0 - 255 | 开关型功能（Gobo、棱镜、颜色轮等） |
| **Coarse + Fine** | 16 位 | 0 - 65,535 | 需要平滑运动的功能（Pan、Tilt、Zoom 等） |
| **Coarse + Fine + Ultra** | 24 位 | 0 - 16,777,215 | 极高精度需求（极少使用） |

Fine / Ultra 留 0 就是没有这一级。

> **重要**：读取灯具属性时，系统会根据是 8 位、16 位还是 24 位进行归一化：
> - Coarse → 原始值 ÷ 255 = 0.0 ~ 1.0
> - Fine → 原始值 ÷ 65,535 = 0.0 ~ 1.0
> - Ultra → 原始值 ÷ 16,777,215 = 0.0 ~ 1.0

#### 通道精度快速选择指南

| 属性类型 | 推荐精度 | 理由 |
|---------|---------|------|
| **Dimmer（亮度）** | Fine (16位) | 低亮度时 8 位精度会出现可见跳变 |
| **Pan / Tilt** | Fine (16位) | 旋转运动需要平滑过渡，8 位精度仅 256 步远不够 |
| **Zoom / Focus / Iris** | Fine (16位) | 光学参数调整需要精细控制 |
| **颜色轮 (Color Wheel)** | Coarse (8位) | 离散选择（6-12 个颜色），不需要连续过渡 |
| **Gobo 选择** | Coarse (8位) | 离散选择（6-12 个图案） |
| **Gobo 旋转** | Fine (16位) | 连续旋转速度需要平滑 |
| **棱镜 (Prism)** | Coarse (8位) | 离散选择（开/关/旋转方向） |
| **频闪 (Strobe)** | Coarse (8位) | 频闪速度通常 8 位足够 |
| **RGB / RGBW** | Coarse (8位) | 颜色混合通常 8 位（256 级）足够 |
| **CMY** | Coarse (8位) | 同 RGB |
| **CTO / CTB** | Coarse (8位) | 色温调整通常 8 位足够 |
| **控制通道 (Reset/Lamp)** | Coarse (8位) | 指令型通道，无需精调 |

> **经验法则**：如果灯具说明书中为某个属性分配了 2 个通道（Coarse + Fine），就填写 Fine 通道。如果只分配了 1 个通道，就只填写 Coarse。严格按照灯具说明书配置。

### 4.4 属性分类 (Category)

| 分类 | 说明 | 典型属性 |
|------|------|---------|
| **Dimmer** | 亮度 | Dimmer |
| **Position** | 位置 | Pan, Tilt, PanRot, TiltRot |
| **Gobo** | 图案 | Gobo1, Gobo2, GoboRot |
| **Color** | 颜色 | Red, Green, Blue, White, Amber, ColorWheel, CTO |
| **Beam** | 光束 | Zoom, Iris |
| **Focus** | 聚焦 | Focus |
| **Control** | 控制 | Reset, LampOn, LampOff |
| **Shapers** | 切割 | Blade A1-B4, ShaperRot |
| **Strobe** | 频闪 | Shutter, Strobe |
| **Prism** | 棱镜 | Prism1, Prism2, PrismRot |
| **Frost** | 雾化 | Frost |
| **Effects** | 效果 | EffectDimmer, EffectValue |
| **Other** | 其他 | 任何未分类功能 |

---

## 五、子属性 (Sub-Attributes)

每个属性定义中可以添加**子属性 (Sub-Attributes)**，用于定义通道值范围内的功能细分。这类似于 MA 控台中的"通道集 (Channel Set)"。

### 5.1 子属性参数

子属性这一层的列：

| 列 | 说明 | 示例 |
|------|------|------|
| **DMX Start** | DMX 值范围的最小值（0-255） | `0` |
| **DMX End** | DMX 值范围的最大值（0-255） | `255` |
| **Physical Range** | 物理值范围（真实单位，最小值 / 最大值） | (0.0, 540.0)（度/米/百分比等） |
| **Channel Sets** | 这一段下面挂了几个槽位（见第六节） | 一条子属性下可有多个槽位 |
| **Strobe Mode** | 频闪模式。**只在 Dimmer / Strobe 分类的属性下出现** | Closed / Open / Linear / Pulse / Ramp Up / Ramp Down / Sine / Random |
| **Rotation Mode** | 旋转模式。**只在 Position 分类的属性下出现** | Off / Position / Infinite。**Stop 已在 26H2.6 移除**，语义并入 Off；存量资产自动重定向 |

> 子属性这一层**没有名字列**——名字在下一层的槽位上。下面示例里的"子属性名称"是为了读起来方便，实际填在槽位的 Name 列。

### 5.2 子属性示例

**Gobo 通道的子属性定义**：

| 子属性名称 | DMX Min | DMX Max | 说明 |
|-----------|---------|---------|------|
| Open | 0 | 7 | 无 Gobo（白光） |
| Gobo 1 | 8 | 15 | 第一个 Gobo 图案 |
| Gobo 2 | 16 | 23 | 第二个 Gobo 图案 |
| Gobo 3 | 24 | 31 | 第三个 Gobo 图案 |
| Gobo 1 Spin CW | 32 | 95 | Gobo 1 顺时针旋转（慢→快） |
| Gobo 1 Spin CCW | 96 | 159 | Gobo 1 逆时针旋转（慢→快） |

**Pan 通道的子属性定义**：

| 子属性名称 | DMX Min | DMX Max | Physical Min | Physical Max | 说明 |
|-----------|---------|---------|-------------|-------------|------|
| Pan Range | 0 | 255 | -270.0 | 270.0 | Pan 角度范围 ±270° |

---

## 六、通道集 (Channel Sets)

**通道集 (Channel Set)** 是一种快捷方式，类似于 MA2 的 Channel Set 功能。它为属性定义预设的命名值。

### 6.1 槽位参数

槽位这一层的列（**分类不同，后面几列也不同**）：

| 列 | 默认 | 出现条件 | 说明 |
|------|---|---|------|
| **Name** | 空 | 总是 | 槽位名称，如 `Open`、`Gobo 1`、`Red` |
| **DMX Start** / **DMX End** | 0 / 0 | 总是 | 本段占的 DMX 值区间 |
| **Physical Range** | (0, 0) | 总是 | 物理值范围（最小 / 最大） |
| **Gobo Mode** | Static | Gobo 分类 | 图案模式：Static / Scroll / Shake |
| **Texture** | 空 | Gobo 分类 | 图案纹理 |
| **Color** | 白 | Color 分类 | 颜色值 |
| **Color Index** | 0.0 | Color 分类 | 颜色索引值，用于色轮定位 |
| **Prism Selection** | Off | Prism 分类 | 棱镜层选择：Off / Prism 1 / Prism 2 / Prism 3。**光在灯具定义里挂了棱镜预设还不够**——不在这里给某个 DMX 区间选层，就没有哪一段会真的选中棱镜 |

> **Physical Range 留空（两端都是 0）时，这一段沿用上一层子属性声明的范围**；填了就以填的为准。GDTF 导入进来的资产会保留原包里的写法，包括 `6000..6000` 这类两端相等的单点标定——那不是留空，别手工把它清成 0。

### 6.2 使用场景

通道集通常用于快速访问特定功能值，例如：

- Gobo 通道：Open=0, Gobo1=10, Gobo2=20 ...
- Shutter 通道：Open=255, Closed=0, Strobe=128

---

## 七、矩阵灯配置

对于 LED 矩阵灯（如 Robe Spiider、Ayrton MagicPanel 等），每个灯珠/像素点需要定义为独立的模块实例。

### 7.1 矩阵配置步骤

1. 确定灯具的像素数量（例如 7 个灯珠）
2. 确定每个像素的通道布局（例如每个像素 4 通道：RGBW）
3. 确定主控通道的数量（例如 Dimmer、Pan、Tilt 等占用前 16 通道）

**示例**：一台 7 珠 LED 矩阵灯，主控 16 通道，每珠 RGBW 4 通道

| 模块实例 | Module Name | Patch | 属性 |
|---------|-------------|-------|------|
| 0 | Main | 1 | Dimmer(1), Pan(2/3), Tilt(4/5), ... |
| 1 | Pixel 1 | 17 | Red(1), Green(2), Blue(3), White(4) |
| 2 | Pixel 2 | 21 | Red(1), Green(2), Blue(3), White(4) |
| 3 | Pixel 3 | 25 | Red(1), Green(2), Blue(3), White(4) |
| 4 | Pixel 4 | 29 | Red(1), Green(2), Blue(3), White(4) |
| 5 | Pixel 5 | 33 | Red(1), Green(2), Blue(3), White(4) |
| 6 | Pixel 6 | 37 | Red(1), Green(2), Blue(3), White(4) |
| 7 | Pixel 7 | 41 | Red(1), Green(2), Blue(3), White(4) |

> **注意**：每个像素模块的 Patch 值 = 主控通道数 + (像素索引 × 像素通道数) + 1

### 7.2 矩阵属性读取

配置完矩阵后，系统可以按模块遍历同名属性并返回数组。例如读取所有像素模块的 `Red` 值。实际响应取决于所使用灯具资产是否接入矩阵读取逻辑。

---

## 八、灯具库与灯具 Actor 的关联

创建好灯具库后，需要将其关联到场景中的灯具 Actor：

1. 选中场景中的灯具 Actor
2. 在细节面板中找到 **Fixture Library** 属性
3. 从下拉列表中选择对应的灯具库资产

> **提示**：如果使用自制灯具资产，建议在资产默认设置中预设好 Fixture Library，这样放置到场景中时就能自动关联。

---

## 九、内置灯具库

SuperStage 在 `Content/Library/Lighting/` 目录下按厂商组织预制灯库，26H2.6 覆盖 54 个厂商、807 个型号，全部由 GDTF 导入生成：

| 厂商目录 | 型号数 |
|------|------:|
| `ChauvetProfessional` | 71 |
| `Prolights` | 69 |
| `Robe_Lighting` | 68 |
| `CKC_Lighting` | 60 |
| `Elation` | 58 |
| `Ayrton` | 42 |
| `Terbly` | 38 |
| `Acme` | 38 |
| `MartinProfessional` | 35 |
| `Cameo` | 35 |
| 其余 44 个厂商 | 293 |

目录名即厂商名，与 GDTF 包内声明一致。

你可以直接使用这些预制灯具库，也可以复制一份作为自定义灯具库的起点。

---

## 十、使用建议

### 通道库命名
- 随包约定是 `CL_<型号>_<模式名>`
- 同一灯具不同 DMX 模式各用一份通道库

### 通道偏移
- 严格按照灯具说明书中的通道表填写
- 注意偏移从 **1** 开始（不是 0）
- Fine 通道通常紧跟在 Coarse 通道后面

### 属性命名
- 使用标准化名称（Dimmer、Pan、Tilt、Red、Green、Blue...）
- 同一项目中保持命名一致，便于蓝图复用
- 属性名称区分大小写

### 验证
- 配置完成后，在场景中放置灯具，连接控台
- 逐个通道推值，确认每个属性响应正确
- 使用 DMX 活动监视器辅助调试

---

## 十一、常见问题

### Q: 灯具不响应某个通道？
检查该通道的 Coarse 偏移值是否正确。注意偏移从 1 开始。

### Q: Pan/Tilt 运动不够平滑？
确保这条属性填了 Fine 偏移（填了才是 16 位）。

### Q: 矩阵灯只有第一个像素响应？
检查每个像素模块实例的 Patch 值是否正确计算。

### Q: 如何知道灯具占用了多少个通道？
编辑器头部的 **Channel Count** 显示的就是通道跨度：只统计大于 0 的 Coarse / Fine / Ultra 偏移，结果等于最大有效通道地址 − 最小有效通道地址 + 1。

---

> **下一步**：请阅读 [03 - DMX 灯具基础](03_DMX_Actor_Base_zh.md) 了解如何在场景中配置灯具的 DMX 地址。
