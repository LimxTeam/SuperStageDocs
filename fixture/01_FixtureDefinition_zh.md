# 灯具定义资产

> 适用版本：SuperStage 26H2.6 起 ｜ 前置阅读：[灯具系统总览](00_FixtureSystem_Overview_zh.md)

灯具定义（Super Fixture Definition）描述一支灯的外形、机械、出光与 DMX 模式。它的资产名就是型号名，**不带前缀**——同目录下带 `LTC_` / `LTA_` / `CLA_` / `SM_` 前缀的是它引用的贴图与模型。本文按面板分组逐项说明。

打开方式：内容浏览器双击定义资产，或在场景中右键一支灯选择编辑其定义。编辑界面见 [灯具编辑器](03_FixtureEditor_zh.md)。

---

## 1. A.身份（Identity）

| 字段 | 说明 |
| --- | --- |
| Manufacturer | 制造商，例如 `Robe`、`Clay Paky` |
| Model | 型号，例如 `MegaPointe` |
| Group | 一级分类，决定它落在资产浏览器侧边栏的哪个节点下 |
| Fixture Type | 灯具类别，例如 `Beam` / `Wash` / `Profile` / `Strobe` |
| GUID | 资产唯一标识，只读 |
| Revision | 版本号，从 GDTF 包带入 |
| Thumbnail | 缩略图，资产浏览器格子里显示的那张图 |

Manufacturer 与 Model 参与配接命名、MA 同步归组和资产浏览器搜索。从 GDTF 导入时它们按包内声明填写；自己改过名之后重导，请关掉导入范围里的"身份"一项，否则会被包里的值覆盖回去。

---

## 2. B.物理（Physical）

这一组主要用于报表、导出和光度计算，不参与通道解析。

| 字段 | 单位 | 说明 |
| --- | --- | --- |
| Power | W | 额定功率 |
| Weight | kg | 灯具重量 |
| Luminous Flux | lm | 光通量 |
| Native Colour Temperature | K | 灯具原生白点。它决定这支灯"白光"到底是什么颜色——8000K 的光束灯和 3000K 的钨丝灯不该是同一个白 |
| Photometric Intensity | — | 打开后按光度量计算亮度 |
| Body Size | m | 灯体外廓尺寸 |
| Max Light Distance | m | 光照最远距离 |
| Max Intensity | % | 亮度上限 |

> Max Light Distance 与 Max Intensity 改完请编译一次。这两项此前会被模型默认值在每次重建时冲回去，26H2.6 已修。

---

## 3. C.钻机（Rig）

钻机描述灯体的机械结构：一个底座加最多三根轴。

| 字段 | 说明 |
| --- | --- |
| Base Mesh | 底座网格（不随轴转动的那部分） |
| Axis A (Yoke / Pan) | 一级轴，通常是灯臂 / 摇摆 |
| Axis B (Head / Tilt) | 二级轴，通常是灯头 / 俯仰 |
| Axis C (Spin, optional) | 三级轴，用于灯头自转一类结构 |

每根轴的字段：

| 字段 | 默认 | 说明 |
| --- | --- | --- |
| Enabled | 关 | 是否是**可动**轴。关掉不等于删掉——**网格照常挂上**，只是不响应任何位置通道，用于 tilt-only 频闪、全静态灯条一类 |
| Rotation Axis | `Z / Yaw (Pan)` | 绕哪个本地轴转。另两个值是 `X / Roll (Tilt)` 与 `Y / Pitch`（少数异形机构） |
| Pivot Offset (cm) | (0,0,0) | 转轴相对上一级的偏移，单位厘米。惯例只有 Z 分量，三个分量都保留是为了少数非对称灯臂 |
| Range (deg) | (−270, 270) | 机械行程，见下 |
| Invert | 关 | 反向 |
| Max Speed (deg/s) | 0 | 最大角速度。**0 = 不限速，直接到位**；填正值才会逐帧转过去，PT 速度通道也才有可缩放的对象 |
| Mesh | 空 | 这根轴带动的网格 |

**Range 的两个分量不是"下限 / 上限"**，而是 **X = DMX 最小端的角度，Y = 最大端的角度**。求值是 `Lerp(X, Y, 归一化DMX)`，全程不做 Clamp，所以 **X > Y 是合法值**，用来表达机械方向相反的厂商实现。

**结构体默认的 (−270, 270) 不代表任何真实轴的行程**，只是占位值。实际值从 GDTF 包内声明导入；包里没写时按轴的角色兜底——摇摆 ±270°、俯仰 ±135°。**不要拿 ±270 当俯仰用**，全库 99.3% 的包不声明这个值，照搬会让行程翻一倍。

26H2.6 之前这一项从来没有从 GDTF 导入过，绝大多数灯的俯仰行程是错的。如果某支灯的俯仰看起来行程不对，先看这里的 Range 是不是包里带来的真实值。

---

## 4. D.发射器（Emitters）

发射器回答"从哪里出光、以什么方式出光"。一支灯可以有多个发射器：主光一个、光环一个、背板一个，矩阵灯每个像素组一个。

### 4.1 基本

| 字段 | 说明 |
| --- | --- |
| Name | 发射器名称 |
| Beam Kind | 光束类型，见 4.2 |
| Layout | 排布方式，见 4.3 |
| Attach Slot | 挂在钻机的哪一级：Root / Base / Axis A / Axis B / Axis C |
| Local Offset (cm) | 相对挂点的偏移 |
| Local Rotation | 相对挂点的旋转 |

### 4.2 光束类型（Beam Kind）

| 类型 | 画空中光柱 | 用途 |
| --- | --- | --- |
| Volumetric Beam | 是 | 体积光束。随包灯库的默认选择 |
| Volumetric Shaper (Profile) | 是 | 体积光束 + 四刀片切割，成像灯用 |
| Ray Beam (Matrix / FX) | 是 | 纯光束，不带光学元件。矩阵与效果灯用，一支灯上几十上百个实例也扛得住 |
| Wash (Color / Blinder / Strobe) | 否 | 灯头前方一团有界的辉光。为大张角而设——锥体的屏幕开销随张角平方增长，70° 的一支灯能糊掉半个屏幕 |
| Spot Only (No Column) | 否 | 只有锥形光。地面图案不受影响 |
| Pixel Matrix | 否 | 像素矩阵 |
| Effect Plane (LED FX / Magic) | 否 | 效果平面，跑内置图案 |
| Material Beam (Legacy) | 是 | 旧材质管线，保留可退回 |
| Shaper / Profile (Legacy) | 是 | 旧材质切割，保留可退回 |

要点：

- **不画空中光柱不等于不打光**。图案、色轮、刀片、棱镜一律由锥形光投射，地面光斑照常。Beam Kind 只决定空中那道光柱画不画、怎么画。
- **随包灯库已全量迁到体积管线**，库里没有材质管线的发射器。
- **但新从 GDTF 导入的灯，主发射器默认仍是 Material Beam（带刀片通道的是 Shaper）**，需要在这里手动改成体积类型。矩阵 / 像素组按包内 BeamType 判定为 Ray Beam 或 Wash，不受这条影响。
- 两个 Legacy 项排在下拉框最后并标了 Legacy，既有资产的枚举数值没变，已存盘内容不受影响。

### 4.3 排布（Layout）

| 排布 | 参数 | 说明 |
| --- | --- | --- |
| Single | — | 单个 |
| Array 1D (X) | Count X、Span X | 沿 X 等距一排 |
| Grid 2D (X/Y) | Count X/Y、Span X/Y | 二维网格 |
| Segments | Count X | 分段 |
| Circle (XY) | Count X、Circle Radius、Circle Angle Offset | 圆周排布 |
| Explicit (per-instance) | Explicit Placements | 逐个指定变换。GDTF 带排布数据时用这个 |

### 4.4 镜片与光锥

| 字段 | 说明 |
| --- | --- |
| Lens Mesh | 镜片模型。包里没有时会补一个通用默认件，并按出光口半径自动缩放 |
| Lens Mesh Scale (XY) | 镜片模型的 XY 缩放，用于非圆形出光口 |
| Lens Radius (cm) | 出光口半径。它同时决定光柱根部粗细与灯口辉光大小 |
| Zoom Range (deg) | 变焦范围 |
| Default Angle (deg) | 无变焦通道时使用的固定张角 |
| Default Lens Texture | 镜片默认贴图 |

### 4.5 头轴（多头灯）

| 字段 | 说明 |
| --- | --- |
| Head Axis A (Pan) | 这颗头自己的摇摆轴 |
| Head Axis B (Tilt) | 这颗头自己的俯仰轴 |

用于每颗头能独立 Pan / Tilt 的多头灯。字段结构与钻机的轴相同。

---

## 5. E.光学（Optics）

三种轮各三个固定槽位，不是数组——绑定里的轮下标恒为 1..3，组件侧也只有三个入口。

| 字段 | 说明 |
| --- | --- |
| Gobo Wheel 1 / 2 / 3 | 图案轮图集（横向排列的条状纹理） |
| Gobo Wheel 1 / 2 / 3 Slots | 对应图集的格数，0 表示未知 |
| Color Wheel 1 / 2 / 3 | 色盘图集 |
| Color Wheel 1 / 2 / 3 Slots | 对应图集的格数 |
| Prism Preset | 棱镜预设资产 |

**格数必须与图集同源**。材质靠格数把图集切开，切错整轮的图案或颜色全对不上。格数不能从通道函数反推：色盘常见写法是把流水段排在最前，那一段没有槽位列表，按"首个子属性的槽位数"去数会得到 1，整张色盘被当成一格。色盘图集恒为 256×16，也无法从尺寸反推。所以这个数由建图集时的槽位数写入，填 0 表示未知、编译期退回估算。

图集由 [GOBO 图集生成器](../editortools/14_GoboAtlasBuilder_zh.md) 与 [颜色图集生成器](../editortools/13_ColorAtlasBuilder_zh.md) 生成，GDTF 导入时也会自动生成。

---

## 6. F.DMX 模式（Modes）

| 字段 | 说明 |
| --- | --- |
| Mode Name | 模式名，例如 `Standard` / `Extended` |
| Channel Library | 这个模式使用的通道库资产 |
| Bindings | 绑定列表，见下 |

场景里的灯选哪个模式，决定它读哪份通道库。

### 6.1 绑定

一条绑定回答：通道库里的某个属性，驱动这支灯的哪个能力。

| 字段 | 说明 |
| --- | --- |
| Attribute | 通道库里的属性名，例如 `Pan`、`Dimmer`、`Gobo1` |
| Capability | 这条通道驱动什么能力，见 6.2 |
| Emitter Index | 作用在第几个发射器上。多发射器灯具靠它区分主光与光环 |
| Module Index | 作用在通道库的第几个模块上。矩阵灯逐像素靠它区分 |
| Resolution | 按 8 / 16 / 24 位读取 |
| Response Curve | 响应曲线 |
| Curve Exponent | 曲线指数 |
| Invert | 反向 |
| Params | 附加参数，例如轮下标（1/2/3）、颜色分量 |

绑定由 GDTF 导入时按属性名自动推导，也可以在灯具编辑器里手工增删改。手写通道库同样能生成绑定。

### 6.2 能力清单

当前实现支持以下 31 种能力：

| 分组 | 能力 |
| --- | --- |
| 运动 | Axis A Position (Pan)、Axis B Position (Tilt)、Axis C Position (Spin)、Axis A Infinite Rotation、Axis B Infinite Rotation、Axis Speed (PT Speed) |
| 强度 | Dimmer、Shutter / Strobe |
| 颜色 | Color Additive Component、Color Subtractive (CMY)、Color Wheel、Color Temperature (CTO/CTC)、Colour Tint (Green / Magenta) |
| 光学 | Zoom、Focus、Iris、Frost |
| 图案 | Gobo Select、Gobo Rotate / Index |
| 棱镜 | Prism Select、Prism Rotate |
| 切割 | Blade Insert、Blade Corner B、Shaper Rotate、Blade Rotate |
| 效果 | Effect、Effect Speed、Effect Width、Effect Direction |
| 其它 | Control、None |

没有对应能力的通道，导入后会在灯具编辑器的 Validation 页列出来。它们照常占地址、控台照常推得动，只是不驱动任何东西——这是正常的（Reset、Lamp On 一类控制通道本来就没有画面表现），但如果一条明显该有画面的通道出现在这个列表里，就是绑定漏了。

---

## 7. 编译产物

定义是源数据，运行时读的是编译产物。产物随定义资产一起落盘。

- 灯具编辑器保存时自动编译；
- 内容浏览器右键 → **Compile**，可多选批量；
- 产物过期时，灯具编辑器的调试信息会说明它来自上一代版本。

改动以下任何一项都需要重新编译：绑定、发射器、钻机、光学槽位、物理端点（色温范围等）。

---

## 8. 相关文档

- [灯具系统总览](00_FixtureSystem_Overview_zh.md)
- [灯具构建器](06_FixtureBuilder_zh.md)——这些字段按类型怎么组合
- [GDTF 导入](02_GdtfImport_zh.md)
- [灯具编辑器](03_FixtureEditor_zh.md)
- [通道库编辑器](../editortools/10_FixtureLibraryEditor_zh.md)
- [棱镜预设编辑器](../editortools/18_PrismPresetEditor_zh.md)
- [灯光组件](../lightcomponent/00_LightComponent_Overview_zh.md)
