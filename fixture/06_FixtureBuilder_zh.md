# 灯具构建器：手工做出各类灯具

> 适用版本：SuperStage 26H2.6 起 ｜ 前置阅读：[灯具定义资产](01_FixtureDefinition_zh.md)、[灯具编辑器](03_FixtureEditor_zh.md)

有 GDTF 就[直接导入](02_GdtfImport_zh.md)，不要手工建。本篇针对的是三种没有 GDTF 的场合：厂商不提供、要做现实里不存在的效果器件、或者把导进来的灯改造成别的东西。

全篇分三部分：**第 1–3 节**把每个字段讲清楚（含默认值、取值范围、填错的后果），**第 4 节**是七个从头到尾的完整案例，**第 5–6 节**是编译校验的全部诊断与成因。

---

## 1. 一支灯的四层结构

```text
Root
 ├ Base   底座静态模型
 └ AxisA  一级旋转轴（灯臂 / Yoke），传统为 Pan
     └ AxisB  二级旋转轴（灯头 / Head），传统为 Tilt
         └ AxisC  三级旋转轴（可选，默认关闭）
             └ Emitters[]  发射器（单颗 / 阵列 / 网格 / 圆周 / 分段 / 显式）
```

机械拓扑是**写死的五个角色槽位**，不是可以随便接的节点树。换来的是运行时定长数据、导入模型可自动校验、以及编辑器只有五行表单。

填写顺序必须是 **钻机 → 发射器 → 光学 → 模式**：发射器要挂到轴上，绑定要按下标引用发射器，反过来做会返工。

---

## 2. 逐字段详解

### 2.1 A. 身份（Identity）

| 字段 | 默认 | 说明 |
| --- | --- | --- |
| **Manufacturer** | 空 | 厂商，如 `Robe` / `ClayPaky` |
| **Model** | 空 | 型号，如 `MegaPointe` |
| **Group** | `StageLight` | **资产浏览器左侧树的顶层节点**。可用值：`StageLight`（厂商灯具）/ `SuperLight`（自研灯）/ `SuperVFX`（特效）/ `StageModel`（舞美道具）/ `SuperStage`（工具类）|
| **Fixture Type** | 空 | 灯具分类，如 `Beam` / `Wash` / `Profile` / `Strobe` |
| **GUID** | 自动 | 只读。型号身份的权威依据，改名 / 移动 / 重导都不变 |
| **Revision** | `1` | 资产修订号，每次发布递增 |
| **Thumbnail** | 空 | 资产浏览器网格里显示的缩略图 |

**Group 填错，灯会跑到错误的分类节点下。** 做厂商灯具就留 `StageLight`；做自己的效果器件填 `SuperLight` 或 `SuperVFX`。

**GUID 不要手动清空。** MVR 导出与资产浏览器靠 `(类, GUID)` 这一对区分数据驱动灯具的型号——只看类的话，所有数据驱动灯具会被合并成同一个型号。

### 2.2 B. 物理（Physical）

| 字段 | 默认 | 范围 | 说明 |
| --- | --- | --- | --- |
| **Power (W)** | `0` | ≥ 0 | 额定功率 |
| **Weight (kg)** | `0` | ≥ 0 | 重量 |
| **Luminous Flux (lm)** | `0` | ≥ 0 | 光通量 |
| **Native Colour Temperature (K)** | `0` | ≥ 0 | 灯具原生白点色温。**0 = 未声明，按纯白处理** |
| **Photometric Intensity** | 关 | — | 光通量→坎德拉的光度学标定。**保持关闭**，见下 |
| **Body Size (m)** | `(0.5, 0.5, 0.5)` | — | 灯体尺寸，单位**米** |
| **Max Light Distance (m)** | `50` | ≥ 1 | 最大照射距离，单位**米** |
| **Max Intensity (%)** | `200` | ≥ 1 | 最大亮度倍数（百分比） |

功率、重量用于配电与载荷统计，也参与 MVR / GDTF 导出。

**关于原生色温**：这个值让不同灯具的白不再是同一个白——同一面墙上，8000K 的光束灯与 3000K 的钨丝观众灯应该看得出区别。合成时以 6500K 为基准归一，所以填 6500 与不填在观感上一致。**只在灯具没有色温通道（CTC / CTO / CTB）驱动时生效**，避免两者叠加。

> 实测分布可作填写参考：6000K 最多（气体放电灯主流），其次 6500K、8000K、7000K、10000K；钨丝观众灯在 2500–3000K。

**关于 Photometric Intensity**：默认关闭，且**不要打开**。`Max Intensity` 不只喂真实光源，还会乘进光束 / 镜片材质的 MaxBrightness，而材质侧是按 200 量级手工调的；22000 流明的 3 度光束换算出来是千万坎德拉量级，直接喂进去会把光束亮度冲高五个数量级。打开它的前提是材质侧已经改成光度学量级——在那之前，流明字段的换算结果只是个标定参考。

### 2.3 C. 钻机（Rig）

顶层只有一个 **Base Mesh**（底座静态模型，挂在 Root 上），加三根轴。每根轴的字段相同：

| 字段 | 默认 | 说明 |
| --- | --- | --- |
| **Enabled** | 关 | 是否是可动轴。**关闭 = 刚性连接**——模型照常挂上，但不响应任何位置通道 |
| **Rotation Axis** | `Z / Yaw (Pan)` | 绕哪根局部轴转。另两个值：`X / Roll (Tilt)`、`Y / Pitch`（少数异形机构） |
| **Pivot Offset (cm)** | `(0,0,0)` | 相对上一级槽位的枢轴偏移，**单位厘米**。惯例只有 Z 分量，三个分量都保留是为了少数非对称灯臂 |
| **Range (deg)** | `(-270, 270)` | 机械角度范围，见下 |
| **Invert** | 关 | 反转旋转方向 |
| **Max Speed (deg/s)** | `0` | 最大角速度。**0 = 不限速（瞬间到位）** |
| **Mesh** | 空 | 这一级带的模型（轴 A 通常是灯臂，轴 B 通常是灯头） |

**Range 的两个分量不是"下限/上限"**，而是 **X = DMX 最小端的角度，Y = 最大端的角度**。求值是 `Lerp(X, Y, 归一化DMX)`，全程没有 Clamp，所以 **X > Y 是合法值**，用来表达机械方向相反的厂商实现（Anolis Eminere 的 Tilt 就是 `8 .. -8`）。

> **结构体默认的 ±270 不代表任何真实轴的行程。** 它只是占位值。没有厂商数据时的惯例兜底是：**Pan ±270、Tilt ±135**。
>
> **千万不要把 ±270 当 Tilt 用**——全库 99.3% 的包不声明 ±270，照搬会让俯仰行程翻一倍。

**轴的角色由 `Rotation Axis` 决定，不由槽位名决定。** tilt-only 的灯就把俯仰放在轴 A 上，轴 B 关掉。

### 2.4 D. 发射器（Emitters）

一条发射器 = 一个出光原点。**21 个字段**，分五组。

#### 基本

| 字段 | 默认 | 说明 |
| --- | --- | --- |
| **Name** | 空 | 发射器名。绑定表按名字引用它，编辑器也用它显示。**做矩阵灯时这个名字必须与通道库里的模块名一致**，见 §4.5 |
| **Beam Kind** | `Material Beam (Legacy)` | 运行时组件类型，见 §3.1 |
| **Layout** | `Single` | 排布，见 §3.2 |
| **Attach Slot** | `Axis B (Head)` | 挂到哪个槽位：`Root` / `Base` / `Axis A (Yoke)` / `Axis B (Head)` / `Axis C (Spin)` |
| **Local Offset (cm)** | `(0,0,0)` | 相对挂载点的偏移。传统灯的挂载点是钻机槽位；**多头灯启用头轴后，挂载点是该实例最深一级的头枢轴**，会跟着头转，用来把镜片推出灯头模型 |
| **Local Rotation** | `(0,0,0)` | 本发射器的朝向。斜置镜片、光环 LED 各自的朝向靠它，不填则全部朝同一方向 |

#### 排布参数

| 字段 | 默认 | 范围 | 用于 |
| --- | --- | --- | --- |
| **Count X** | `1` | ≥ 1 | X 方向数量 / 分段数 / 圆周数量（`Single` 忽略） |
| **Count Y** | `1` | ≥ 1 | Y 方向数量（**仅 Grid 2D**） |
| **Span X (cm)** | `0` | ≥ 0 | X 方向总跨度，**首尾中心到中心** |
| **Span Y (cm)** | `0` | ≥ 0 | Y 方向总跨度 |
| **Circle Radius (cm)** | `0` | ≥ 0 | 圆周半径（**仅 Circle**，其它排布下面板隐藏此项） |
| **Circle Angle Offset (deg)** | `0` | — | 圆周起始角（**仅 Circle**） |
| **Explicit Placements** | 空 | — | 逐实例变换数组（**仅 Explicit**） |

#### 光学

| 字段 | 默认 | 范围 | 说明 |
| --- | --- | --- | --- |
| **Lens Mesh** | 空 | — | 镜片模型（可选） |
| **Lens Mesh Scale (XY)** | `(1, 1)` | ≥ 0.01 | 镜片模型的 XY 缩放。**同一个镜片模型拉伸到不同口径，省掉逐型号做模型**。只有 XY——镜片是出光面上的薄片，Z 是厚度方向，缩放 Z 会让它陷进灯头或飘出来 |
| **Lens Radius (cm)** | `10` | ≥ 0.5 | **光束起端半径**。光圈（Iris）以此为基准向下缩 |
| **Zoom Range (deg)** | `(1, 10)` | — | 变焦角度范围。**X = 最窄，Y = 最宽** |
| **Default Angle (deg)** | `1` | ≥ 0 | **没有 Zoom 通道时**使用的固定光束角 |
| **Default Lens Texture** | 空 | — | 默认投射纹理（LED 阵列图案一类，可选） |

> `Lens Radius` 与 `Zoom Range` 是两件事：前者是**出光口的物理半径**（光束根部有多粗），后者是**张角**（光束张多开）。光束灯是"根部细、张角小"，染色灯是"根部粗、张角大"。

#### 头轴（多头灯专用）

| 字段 | 默认 | 说明 |
| --- | --- | --- |
| **Head Axis A (Pan)** | 关 | 逐实例的 Pan 轴，字段与钻机轴完全相同 |
| **Head Axis B (Tilt)** | 关 | 逐实例的 Tilt 轴 |

**头轴在发射器上，钻机轴在整支灯上**：钻机三根轴是整灯共用一套，头轴是每个实例各一套。详见 §4.6。

### 2.5 E. 光学（Optics）

整支灯共用一组，**不分发射器**。

| 字段 | 默认 | 说明 |
| --- | --- | --- |
| **Gobo Wheel 1 / 2 / 3** | 空 | 三个固定命名槽位的图案轮图集 |
| **Gobo Wheel 1 / 2 / 3 Slots** | `0` | 对应图集的格数。**0 = 未知**，编译期退回按通道函数估算 |
| **Color Wheel 1 / 2 / 3** | 空 | 三个色盘图集 |
| **Color Wheel 1 / 2 / 3 Slots** | `0` | 对应格数 |
| **Prism Preset** | 空 | 棱镜预设（内含最多 3 层配置，与绑定的 `Wheel Index` 对应） |
| **Dimmer Curve** | `2.0` | 1.0 ~ 3.0 | 调光响应曲线指数。1.0 = 线性，2.0 = 推荐，2.2 = Gamma |

轮位固定三个、不是数组：绑定里的 `Wheel Index` 取值域恒为 1~3，组件侧也只有三个入口。

> **格数必须与建图集时用的那个轮的槽位数同源，不能从通道函数反推。**
>
> 色轮常见写法是把流水段排在最前，那一段没有槽位列表——按"首个子属性的槽位数"去数会得到 1，整张色盘被当成一格（实测 414 个色轮里有 243 个是这种排法）。也有厂商把颜色拆进多个函数，数任何单独一个都不是全貌。
>
> 色盘尤其只能存不能推：色盘图集恒为 256×16，N 格挤在 256 像素里，从尺寸反推不出格数（图案图集是 格宽×N，倒是能反推）。

### 2.6 F. DMX 模式（Modes）与绑定

一个模式 = **模式名 + 一份通道库 + 一张绑定表**。一份定义可以有多个模式，每个模式指向一份通道库。

绑定的 9 个字段：

| 字段 | 默认 | 说明 |
| --- | --- | --- |
| **Attribute** | 空 | 通道库里的属性名。逐字匹配，但**不区分大小写** |
| **Capability** | `None` | 31 条能力操作码之一 |
| **Emitter Index** | `-1` | 目标发射器下标。**−1 = 作用于所有发射器**（例如主调光） |
| **Module Index** | `0` | 模块实例下标。**≥ 0 = 绑到该模块；−1 = 遍历所有模块实例，逐格驱动对应发射器** |
| **Resolution** | `Coarse`（8 位） | 读取位深：8 / 16 / 24 位 |
| **Response Curve** | `Linear` | `Linear` / `Power` / `S-Curve` |
| **Curve Exponent**（0.1 ~ 5.0） | `2.0` | 幂曲线指数，**仅 Curve = Power 时显示** |
| **Invert** | 关 | 反向（DMX 增大 → 物理量减小） |
| **Params** | — | 附加参数，见下 |

`Params` 的 5 个字段，**每个只对特定能力有意义**，无关的留默认：

| 参数 | 默认 | 用于哪些能力 |
| --- | --- | --- |
| **Channel Color** | 白 | `Color Additive Component` / `Color Subtractive (CMY)`——这一路代表的光谱分量 |
| **Wheel Index**（1~3） | `1` | `Gobo Select` / `Gobo Rotate` / `Color Wheel` / `Prism Select` |
| **Blade Index**（1~4） | `1` | `Blade Insert` / `Blade Corner B` / `Blade Rotate` |
| **Temperature Range (K)** | `(2700, 8000)` | `Color Temperature`——X = 通道 0 端，Y = 通道满端 |
| **Physical Range Override** | `(0, 0)` | 覆盖通道声明的物理范围。**X == Y 表示不覆盖**，退回通道库里这条子属性自己声明的范围 |

---

## 3. 两个决定性选择

### 3.1 光束类型（Beam Kind）

**默认值是 `Material Beam (Legacy)`。新建发射器必须改。** 导入器也只对它认得出的两类覆写这个默认值——矩阵 / 像素走 Ray Beam 或 Wash，带刀片的走 Shaper，其余全部落在这个默认上。

| 目标 | 选 | 运行时组件 |
| --- | --- | --- |
| 光束灯 / 摇头灯的主光 | **Volumetric Beam** | 体积光束。解析式散射 + 阴影图集实时遮挡，门空间里带图案 / 色轮 / 棱镜 |
| 成像灯 / 切割灯 | **Volumetric Shaper (Profile)** | 体积光束 + 四刀片。刀片在自己的着色器里，不做成光束的排列组合 |
| 矩阵 / 像素条 / 效果灯的每一格 | **Ray Beam (Matrix / FX)** | 体积光束**去掉光学元件**。恒走零采样闭式路径，几百个实例也扛得住，也不受采样率影响 |
| 染色 / 观众灯 / 频闪 | **Wash (Color / Blinder / Strobe)** | 不画光柱，灯头前一团有界椭球辉光 |
| 光环 / 背板一类的发光面 | **Wash** | 同上 |
| 只要地面光斑、不要空中光柱 | **Spot Only (No Column)** | 只有锥形光。**地面图案不受影响**——图案 / 色轮 / 刀片 / 棱镜都由锥形光投射 |
| 像素矩阵（整块一个组件） | **Pixel Matrix** | 见 §4.5 |
| 跑内置图案的效果平面 | **Effect Plane (LED FX / Magic)** | 见 §4.7 的重要提示 |
| — | **Material Beam / Shaper (Legacy)** | 旧材质管线，保留可退回，**新建不要选** |

**光束类型选错的两个后果，编译器都会报警告**：

- 给 **Ray Beam 或 Wash** 绑图案 / 棱镜 / 色轮 → 这两类没有光学元件，绑上去静默落空；
- 给**光环 / 背板**选 Ray Beam → 一支 25 圈光环的灯会射出 25 道光柱，既不对也白付 25 份开销。

### 3.2 排布（Layout）与它的精确几何

| 排布 | 生成实例数 | 位置怎么算 |
| --- | ---: | --- |
| **Single** | 1 | 就在 `Local Offset` |
| **Array 1D (X)** | `Count X` | 沿 X 均布，第 i 个在 `Lerp(-SpanX/2, +SpanX/2, i/(CountX-1))`。`Count X ≤ 1` 时恒为 0 |
| **Grid 2D (X/Y)** | `Count X × Count Y` | **行优先**，下标 = `y × CountX + x`。X 从 `-SpanX/2` 递增到 `+SpanX/2`，**Y 从 `+SpanY/2` 递减到 `-SpanY/2`** |
| **Circle (XY)** | `Count X` | 第 i 个的角度 = `CircleAngleOffset + 360 × i / CountX`。**除以 CountX 不是 CountX−1**（首尾不重合）。0° 落在 **+X**、**逆时针**递增，与 UE 的 Yaw 同向。圆心 = `Local Offset` |
| **Segments** | `Count X` | 全部在 `Local Offset`。**N 条实例共用一个矩阵组件**，各带自己的分段下标 |
| **Explicit** | `Placements` 数量 | 位置 = `Local Offset + 该变换的平移`，朝向取该变换的旋转。**列表为空时退化成单颗**，不会建出零个发射器 |

要点：

- **Span 是首尾中心到中心的总跨度**，不是间距。8 格、间距 10cm 的灯条，`Span X` 填 **70** 不是 80；
- **Grid 2D 的 Y 是递减的**——第 0 行在最上面。逐格颜色对不上时先查这个；
- **Circle 的 0° 在 +X**。厂商的一圈镜片很少正好从 +X 开始，用 `Circle Angle Offset` 对齐到实物，不要为了转它去改整支灯的挂载朝向；
- **弧形灯条、斜置面板用 Explicit**。规则排布会把它们拉直、把朝向抹平——全库 316 个包里有 60 个在 Position 上带非单位旋转，用规则排布必然失真。

---

## 4. 七个完整案例

每个案例给出**从零到能用**的全部取值。其中的数值是示例量级，实际做某支灯时按厂商说明书填。

### 4.1 摇头光束灯

一支 260W 光束灯，Pan 540°、Tilt 270°、光束角 2°、带 1 个固定图案轮和 1 个色轮。

**身份 / 物理**

| 字段 | 值 |
| --- | --- |
| Manufacturer / Model | 按实物填 |
| Group | `StageLight` |
| Fixture Type | `Beam` |
| Power (W) | `260` |
| Weight (kg) | `21` |
| Native Colour Temperature (K) | `7000`（气体放电灯常见值） |
| Body Size (m) | `(0.35, 0.25, 0.55)` |
| Max Light Distance (m) | `80` |
| Max Intensity (%) | `200`（保持默认） |

**钻机**

| | Base | Axis A | Axis B | Axis C |
| --- | --- | --- | --- | --- |
| Enabled | — | ✔ | ✔ | ✘ |
| Rotation Axis | — | `Z / Yaw (Pan)` | `X / Roll (Tilt)` | — |
| Range (deg) | — | `(-270, 270)` | `(-135, 135)` | — |
| Pivot Offset (cm) | — | `(0, 0, 18)` | `(0, 0, 22)` | — |
| Max Speed (deg/s) | — | `540` | `300` | — |
| Mesh | 底座 | 灯臂 | 灯头 | — |

> Pan 540° 是**总行程**，对应 Range `(-270, 270)`；Tilt 270° 对应 `(-135, 135)`。

**发射器**（1 条）

| 字段 | 值 |
| --- | --- |
| Name | `Beam` |
| Beam Kind | **Volumetric Beam** |
| Layout | `Single` |
| Attach Slot | `Axis B (Head)` |
| Local Offset (cm) | `(0, 0, 20)`（把镜片推出灯头） |
| Lens Radius (cm) | `6` |
| Zoom Range (deg) | `(2, 2)`（光束灯定角，不变焦） |
| Default Angle (deg) | `2` |

**光学**

| 字段 | 值 |
| --- | --- |
| Gobo Wheel 1 | 用 [GOBO 图集生成器](../editortools/14_GoboAtlasBuilder_zh.md)生成的图集 |
| Gobo Wheel 1 Slots | 图集实际格数，例如 `17` |
| Color Wheel 1 | 用[色盘图集生成器](../editortools/13_ColorAtlasBuilder_zh.md)生成的图集 |
| Color Wheel 1 Slots | 例如 `14` |
| Dimmer Curve | `2.0` |

**绑定表**

| Attribute | Capability | Emitter | Resolution | Params |
| --- | --- | ---: | --- | --- |
| `Pan` | Axis A Position (Pan) | −1 | 16 位 | — |
| `Tilt` | Axis B Position (Tilt) | −1 | 16 位 | — |
| `PTSpeed` | Axis Speed (PT Speed) | −1 | 8 位 | — |
| `Dimmer` | Dimmer | −1 | 16 位 | — |
| `Shutter` | Shutter / Strobe | −1 | 8 位 | — |
| `Color` | Color Wheel | 0 | 8 位 | Wheel Index `1` |
| `Gobo` | Gobo Select | 0 | 8 位 | Wheel Index `1` |
| `GoboRot` | Gobo Rotate / Index | 0 | 16 位 | Wheel Index `1` |
| `Prism` | Prism Select | 0 | 8 位 | Wheel Index `1` |
| `PrismRot` | Prism Rotate | 0 | 16 位 | Wheel Index `1` |
| `Frost` | Frost | 0 | 8 位 | — |
| `Focus` | Focus | 0 | 16 位 | — |

> Pan / Tilt / Dimmer 用 **Emitter Index = −1**（作用于所有发射器），光学类绑到**具体发射器下标 0**。单发射器的灯两种写法效果一样，多发射器时区别就出来了。
>
> **Resolution 填 16 位的前提是通道库里真有 Fine 通道**。灯库没有对应字节时编译器会降级到 8 位并报警告——不要盲填。

### 4.2 摇头成像灯（带刀片切割）

在 4.1 基础上改两处，其余相同。

**发射器**：`Beam Kind` 改为 **Volumetric Shaper (Profile)**，`Zoom Range` 改为成像灯的实际变焦范围，例如 `(7, 48)`。

**绑定表**追加刀片部分。刀序：**1 = 下、2 = 左、3 = 上、4 = 右**；每片刀的 A 端是它自己的逆时针端。

| Attribute | Capability | Params |
| --- | --- | --- |
| `A1` | Blade Insert | Blade Index `1` |
| `B1` | Blade Corner B | Blade Index `1` |
| `A2` | Blade Insert | Blade Index `2` |
| `B2` | Blade Corner B | Blade Index `2` |
| `A3` | Blade Insert | Blade Index `3` |
| `B3` | Blade Corner B | Blade Index `3` |
| `A4` | Blade Insert | Blade Index `4` |
| `B4` | Blade Corner B | Blade Index `4` |
| `ShaperRot` | Shaper Rotate | — |

**一片刀两条通道**：`Blade Insert` 与 `Blade Corner B` 是这片刀**两端各自的插入深度**，不是"插入 + 角度"。

**"插入 + 旋转"型刀架**（ClayPaky Arolla 一类）写法不同——只有一条插入和一条旋转：

| Attribute | Capability | Params |
| --- | --- | --- |
| `A1` | Blade Insert | Blade Index `1` |
| `Blade1Rot` | **Blade Rotate** | Blade Index `1` |

运行时按 `角点 = 插入 ± 0.5 × tan(θ)` 换算出两端值再送进去。

> **绝对不要把 `Blade(n)Rot` 接到 `Blade Corner B` 上**——那是把角度当插入深度用。刀架整体自转（`Shaper Rotate`）与图案轮自转（`Gobo Rotate`）也是两个能力，别接混。

### 4.3 摇头染色灯

Pan / Tilt 与 4.1 相同，光学部分全部不用。

**发射器**

| 字段 | 值 |
| --- | --- |
| Beam Kind | **Wash** |
| Layout | `Single` |
| Attach Slot | `Axis B (Head)` |
| Lens Radius (cm) | `12`（染色灯口径大） |
| Default Angle (deg) | 忽略——Wash 的张角在组件上，见下 |

Wash 的四个参数在**组件**上（细节面板 `B.BeamParameter`），不在定义里：`Wash Length` 1000cm、`Wash Spread` 70°、`Wash Far Fade` 0.6、`Use Ellipsoid Volume` 关。它的张角**写死、不响应变焦**，变焦 / 雾化 / 光圈三个入口是空实现。详见 [SuperWashComponent](../lightcomponent/14_SuperWashComponent_zh.md)。

**绑定表（RGBW 混色）**

| Attribute | Capability | Params: Channel Color |
| --- | --- | --- |
| `Dimmer` | Dimmer | — |
| `Shutter` | Shutter / Strobe | — |
| `Red` | Color Additive Component | 红 `(1, 0, 0)` |
| `Green` | Color Additive Component | 绿 `(0, 1, 0)` |
| `Blue` | Color Additive Component | 蓝 `(0, 0, 1)` |
| `White` | Color Additive Component | 白 `(1, 1, 1)` |

**一条通道一条绑定，颜色填这一路实际代表的光谱分量**。不限于 RGBW——琥珀、青柠、UV、红橙都是同一条能力配不同的 `Channel Color`。

其它混色方式：

| 混色方式 | 能力 | 要点 |
| --- | --- | --- |
| CMY | `Color Subtractive (CMY)` | 同样一路一条，填对应减色分量 |
| 色温 CTO / CTC | `Color Temperature (CTO/CTC)` | 在 `Temperature Range (K)` 填**这支灯的实际范围**，默认 `(2700, 8000)` |
| 冷暖双路 | 各一条 `Color Additive Component` | 颜色各填自己的白点 |
| 绿品修正 | `Colour Tint (Green / Magenta)` | **双极性**：0 = 满品红、0.5 = 中性、1 = 满绿。加色 / 减色两条能力都是单极性的，一条通道够不到两端，所以它单独一条能力 |

### 4.4 帕灯 / 频闪 / 观众灯

与 4.3 的唯一区别：**三根轴全部关闭**，`Attach Slot` 改为 `Root` 或 `Base`。

绑定表去掉 `Pan` / `Tilt` / `PTSpeed` 三条，其余相同。

> 钨丝观众灯记得填 `Native Colour Temperature (K) = 2700`——不填的话它和 8000K 的光束灯打在同一面墙上会是同一个白。

### 4.5 像素条 / 矩阵面板

**两种做法**，按需要多细的控制来选。

#### 做法一：Segments（开销最低，纯像素条）

一根 12 段的像素条，段间距 10cm：

| 字段 | 值 |
| --- | --- |
| Name | **`Pixel`**（必须与通道库里的模块名一致，见下） |
| Beam Kind | **Pixel Matrix** |
| Layout | `Segments` |
| Count X | `12` |
| Attach Slot | `Root`（灯体固定在杆上） |
| Local Offset (cm) | `(0, 0, 0)` |

生成 12 条实例，但**共用一个矩阵组件**，各带自己的分段下标。

代价：**Segments 排布不能带头轴**（格子不是独立的场景组件）。

#### 做法二：Array 1D（每格一个独立组件）

| 字段 | 值 |
| --- | --- |
| Name | `Pixel` |
| Beam Kind | **Ray Beam (Matrix / FX)** |
| Layout | `Array 1D (X)` |
| Count X | `12` |
| **Span X (cm)** | **`110`** ← 12 格间距 10cm，首尾中心距 = 11 × 10 |
| Lens Radius (cm) | `2.5` |

每格一个独立组件，可以逐格拿到完整的光束表现，也能带头轴。

#### 逐格绑定（两种做法通用）

| Attribute | Capability | Emitter | **Module Index** | Channel Color |
| --- | --- | ---: | ---: | --- |
| `Dimmer` | Dimmer | −1 | `0` | — |
| `Red` | Color Additive Component | 0 | **`-1`** | 红 |
| `Green` | Color Additive Component | 0 | **`-1`** | 绿 |
| `Blue` | Color Additive Component | 0 | **`-1`** | 蓝 |

`Module Index = -1` 表示**逐格**：遍历所有模块实例，第 i 个模块驱动第 i 个发射器实例。

> **发射器的 Name 必须等于通道库里的模块名。** 编译器按"模块名 == 目标发射器名"划定这一组的边界。没有同名模块时会退回遍历整个库——一支灯带多组不同类型光源时（主控 ×1 + LED1×7 + LED2×14 + LED3×7），三组的颜色通道同名，全库横扫会把 28 个格子塞进 7 实例的发射器。
>
> **模块数与实例数必须一致。** 不一致时编译器报警告：多出来的模块全被钳到最后一个实例（末格被驱动多次），多出来的实例永远收不到值（常亮或常灭）。改过 `Count X` 或删过模块都会踩到。

矩阵灯**没有专门的能力操作码**——逐格控制完全通过 `Module Index` 表达，用的还是 `Dimmer`、`Color Additive Component` 这些通用能力。

### 4.6 多头灯（每个头独立摇动）

蜘蛛灯、摆头灯条：一支灯上 N 个头，每个头有自己的 Pan / Tilt。

**关键：头轴配在发射器上，不在钻机上。** 钻机三根轴是整支灯共用一套，N 颗头的 Pan 若写整灯主轴会互相覆盖，最后一个说了算。

一支 8 头摆头条灯：

| 字段 | 值 |
| --- | --- |
| Name | `Head` |
| Beam Kind | **Ray Beam** |
| Layout | **`Array 1D (X)`** |
| Count X | `8` |
| Span X (cm) | `70`（8 头间距 10cm） |
| Attach Slot | `Root` |
| **Head Axis B (Tilt)** | Enabled ✔，`X / Roll (Tilt)`，Range `(-180, 180)` |

钻机：**三根主轴全关**（灯体固定在杆上）。

绑定：

| Attribute | Capability | Emitter | Module Index |
| --- | --- | ---: | ---: |
| `Tilt` | Axis B Position (Tilt) | 0 | **`-1`** |

三个条件缺一不可，否则路由不到头轴、退回主轴：

1. **逐模块**（`Module Index = -1`）——整体控制通道（单条 Pan 管整灯）仍走主轴；
2. **头轴已启用**——没配头轴就没有落点；
3. **不是 Segments 排布**——分段像素共用一个矩阵组件，转不动。

两种常见形态：

| 灯的形态 | 怎么配 |
| --- | --- |
| 多头共用一个 Pan、各自 Tilt | 主 Pan 放**钻机轴 A**，发射器上只开 `Head Axis B` |
| 每个头独立 Pan 与 Tilt | 发射器上**两个头轴都开**，钻机主轴可全关 |

### 4.7 光环 / 背板发光面

一圈 25 颗 LED 的光环：

| 字段 | 值 |
| --- | --- |
| Name | `Aura` |
| Beam Kind | **Wash** ← **绝对不要选 Ray Beam** |
| Layout | **`Circle (XY)`** |
| Count X | `25` |
| Circle Radius (cm) | `14` |
| Circle Angle Offset (deg) | 按实物对齐，例如 `7.2` |
| Attach Slot | `Axis B (Head)` |
| Lens Radius (cm) | `1.5` |

选 Ray Beam 的后果：25 圈光环各射出一道光柱，既不对，也白付 25 份光柱开销。从 GDTF 导入时，包内声明 `BeamType = Glow / None` 的组会自动判成 Wash。

背光 / 光环要独立于主光的颜色时，用 `BgRed` / `BgGreen` / `BgBlue` / `BgWhite` / `BgCTO` 这组属性名，绑定同样是 `Color Additive Component` + 对应 `Channel Color`。

---

## 5. 效果灯与做不了的器件

### 5.1 效果灯：必须给发射器配 Lens Mesh

`Effect Plane (LED FX / Magic)` 这个光束类型驱动的是**一块贴着效果材质的平面**，而这块平面用的就是发射器的 **Lens Mesh** 字段。**Lens Mesh 留空的效果灯不会有任何画面**——组件建出来了，但没有网格就不会生成动态材质，所有效果参数写不进去。

绑定方式：

| Attribute | Capability | 说明 |
| --- | --- | --- |
| `Effect` | `Effect` | 查一张 256 项的表，一次取出（效果编号 0–15、速度 −4 ~ +4、宽度 0.1 ~ 4.0）三元组 |
| `EffectSpeed` | `Effect Speed` | 单独绑了它，速度改由这条通道给，表里只取效果编号与宽度 |

方向不需要单独的通道：速度映射到 −4 ~ +4，中点为停、两侧即两个方向，符号就是方向。

> 按 Compile 时，绑定 `Effect` / `Effect Speed` 目前仍会收到一条"效果引擎尚未实现"的警告。这条警告文本已经过期，不影响编译产物，效果通道是能出画面的。

### 5.2 升降类器件做不了

**31 条能力里没有升降位置。** `PosZ` 属性只有随包的 C++ 机械灯具在处理，那些不是定义资产，也不出现在灯具编辑器里。

需要升降就用现成的：[舞台机械](../stagecore/10_Stage_Machinery_zh.md)（升降机械 / 轨道机械）、[升降矩阵](../stagecore/12_Lift_Matrix_zh.md)、升降效果球，见[自研灯具](../stagecore/17_InHouseFixtures_zh.md)。

---

## 6. 编译校验：全部诊断与成因

按 **Compile** 之后，结果落在 **Validation** 页。**任何一条 Error 都会让编译产物作废**——灯不会更新。

### 6.1 错误（必须修）

| 诊断 | 成因 |
| --- | --- |
| Definition has no DMX modes | 一个模式都没建 |
| Mode has no channel library assigned | 模式没指定通道库 |
| Channel library defines no modules | 通道库是空的 |
| Mode has no bindings; nothing would be driven by DMX | 绑定表是空的 |
| Binding has no capability assigned | 某条绑定的 Capability 还是 `None` |
| Binding has no attribute name | 某条绑定没填属性名 |
| Attribute '…' not found in the channel library | 属性名在这个模式的所有模块里都找不到。匹配不分大小写，先查拼写与分隔符 |
| Emitter index N is out of range | 绑定引用了不存在的发射器下标 |
| Module index N is out of range | 绑定引用了不存在的模块下标 |

### 6.2 警告（能编过，但多半不是你要的结果）

| 诊断 | 成因与处理 |
| --- | --- |
| Definition has no emitters; the fixture will render no light | 一条发射器都没建 |
| Binding drives an axis that is disabled in the rig | 绑了 Pan/Tilt，但对应的钻机轴 `Enabled` 是关的 |
| Head axes are not supported on a Segments-layout emitter | Segments 带不了头轴。改用 Array 1D / Grid 2D，否则退回主轴 |
| Emitter uses the Ray Beam type, which has no optical elements | 给 Ray Beam 或 Wash 绑了图案 / 棱镜 / 色轮。真要打图案就把 Beam Kind 改成 Volumetric |
| Wheel N has no atlas/preset assigned in Optics | 绑定的 `Wheel Index` 指向的图集 / 棱镜预设是空的 |
| Attribute '…' defines no slot list; the wheel will only ever show atlas cell 0 | 通道库里这条属性没编槽位，整轮只会显示图集第 0 格 |
| Attribute '…' has continuous scroll/shake segments with no physical range | 流水 / 抖动段没填速度，会用兜底值而不是厂商数据 |
| 16-bit requested but the library defines no Fine channel | Resolution 填了 16 位但灯库没有 Fine 通道，已降级为 8 位 |
| 24-bit requested but the library defines neither Fine nor Ultra | 同上，24 位需要 Fine 或 Ultra |
| Per-module binding '…' resolved N module(s) but the emitter has M instance(s) | **模块数与实例数不一致**。多出的模块钳到最后一个实例，多出的实例收不到值 |
| Duplicate binding: same attribute, capability, emitter and module | 重复绑定 |
| Channel '…' is defined in the library but not bound to any capability | 通道库里有、绑定表里没有。控制类通道（Reset、Lamp On）出现在这里正常；**明显该有画面的通道出现在这里就是漏了绑定** |
| The effect engine is not implemented yet | 见 §5.1 |

### 6.3 建完之后的四步

1. **Compile**——不编译，场上的灯不会变；
2. **Validation 页**——按上表逐条处理；
3. **Channel Map 页**——抓三类通道表错误：**冲突**（两个属性占同一条通道）、**空洞**（声明了但没绑定）、**越界**（绑定伸到声明跨度之外，这支灯在偷读下一支灯的通道）；
4. **DMX Test 页**——逐条推。按 **Default** 让灯做点正常的事；用 **Rig** 辅助确认枢轴真的落在机械关节上。

---

## 7. 速查：容易踩的十条

| 现象 | 原因 |
| --- | --- |
| 新建的发射器不出光柱或表现像旧管线 | `Beam Kind` 还是默认的 `Material Beam (Legacy)`，没改 |
| Tilt 行程比实物大一倍 | 用了结构体默认的 ±270 当 Tilt。Tilt 的惯例兜底是 **±135** |
| 灯条格间距不对 | `Span X` 填成了**间距**。它是**首尾中心到中心的总跨度**——8 格间距 10cm 应填 70 |
| 矩阵灯上下颠倒 | Grid 2D 的 **Y 是递减的**，第 0 行在最上面 |
| 光环起始位置转了一个角 | Circle 的 0° 在 **+X**，用 `Circle Angle Offset` 对齐，别去改整灯挂载朝向 |
| 逐格颜色错位 / 末格闪烁 | 模块数与实例数不一致，看编译警告 |
| 逐格绑定把别组的格子也带上了 | 发射器 `Name` 与通道库模块名不一致，退回了全库遍历 |
| 整轮图案或颜色错位 | 光学分组的**格数**与图集实际格数不符 |
| 刀片值不起作用 | `Beam Kind` 不是 Shaper 类 |
| 多头灯的头不动 | 用了 Segments 排布；或 `Module Index` 不是 −1；或头轴没启用 |

---

## 8. 相关文档

- [灯具定义资产](01_FixtureDefinition_zh.md)——字段参考与 31 条能力清单
- [灯具编辑器](03_FixtureEditor_zh.md)——四个页签怎么用
- [运动控制](04_Motion_zh.md)——行程、速度通道、无极旋转
- [属性名参考](05_AttributeNames_zh.md)——119 个 DMX 属性名
- [GDTF 导入](02_GdtfImport_zh.md)——有 GDTF 时别手工建
- [通道库编辑器](../editortools/10_FixtureLibraryEditor_zh.md)
- [GOBO 图集生成器](../editortools/14_GoboAtlasBuilder_zh.md)、[色盘图集生成器](../editortools/13_ColorAtlasBuilder_zh.md)
- [灯光组件](../lightcomponent/00_LightComponent_Overview_zh.md)
