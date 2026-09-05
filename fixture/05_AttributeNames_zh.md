# 附录：DMX 属性名参考

> 适用版本：SuperStage 26H2.6 起 ｜ 前置阅读：[灯具定义资产](01_FixtureDefinition_zh.md)

通道库里每条通道有一个**属性名**，绑定按这个名字把通道接到灯具的某个能力上。名字对不上，通道就绑不上——这是"控台推得动、画面没反应"最常见的原因。

本表列出当前实现认得的全部 **119 个属性名**，取自源码里集中定义的常量表。

---

## 1. 大小写变体是有意保留的

`Pan` 与 `PAN`、`Zoom` 与 `ZOOM` 都在表里。不同厂商的 GDTF 包写法不一样，两种都列出来是为了照顾各家的实际写法。

**属性名的匹配不分大小写。** 通道库里写 `PAN`、绑定里写 `Pan`，两边照样接得上。名字对不上通常是**拼写或分隔符**的问题（`ColorAdd_R` 与 `ColorAddR` 是两个名字），不是大小写。

---

## 2. 运动

| 属性名 | 说明 |
| --- | --- |
| `Pan` / `PAN` | 水平旋转 |
| `Tilt` / `TILT` | 垂直旋转 |
| `PTSpeed` / `PTSPEED` | Pan / Tilt 速度 |
| `TiltRot` | Tilt 无极旋转 |
| `PosZ` | 升降位置 |

无极旋转的行为由子属性的 RotationMode 决定，见[灯具运动控制](04_Motion_zh.md)。

---

## 3. 强度与频闪

| 属性名 | 说明 |
| --- | --- |
| `Dimmer` / `DIM` | 主亮度 |
| `Dimmer2` | 第二调光 |
| `Shutter` / `SHUTTER` | 快门 / 频闪 |
| `SHUTTER3` / `SHUTTER6` | 多快门灯的第 3 / 第 6 路 |

---

## 4. 颜色

### 加色

| 属性名 | 说明 |
| --- | --- |
| `Red` / `Green` / `Blue` / `White` | RGBW |
| `Red1` / `Green1` / `Blue1` | 第二组 RGB |
| `Amber` / `Lime` / `Indigo` / `UV` / `RedOrange` | 扩展色 |
| `COLORRGB1` / `COLORRGB2` / `COLORRGB3` | 整体 RGB |

### 减色（CMY）

| 属性名 | 说明 |
| --- | --- |
| `Cyan` / `Magenta` / `Yellow` | CMY |
| `C` / `M` / `Y` | CMY 简写 |
| `COLORSUB_C` / `COLORSUB_M` / `COLORSUB_Y` | CMY 另一种写法 |

### 色轮与色温

| 属性名 | 说明 |
| --- | --- |
| `Color` / `Color1` / `ColorWheel` / `ColorWheel2` | 颜色轮 |
| `COLOR1` / `COLOR2` | 颜色轮（大写变体） |
| `CTO` / `CTC` / `ColorTemperature` | 色温 |
| `Cool` / `Warm` | 冷暖双通道混色 |
| `TINT` | 绿品修正 |

### 背光

| 属性名 | 说明 |
| --- | --- |
| `BgRed` / `BgGreen` / `BgBlue` / `BgWhite` / `BgCTO` | 背光 / 光环的独立颜色 |

---

## 5. 光学

| 属性名 | 说明 |
| --- | --- |
| `Zoom` / `ZOOM` | 变焦 |
| `Focus` / `FOCUS` | 调焦 |
| `Frost` / `FROST` / `Frost2` | 雾化 |
| `Iris` | 光圈 |

---

## 6. 图案

| 属性名 | 说明 |
| --- | --- |
| `Gobo` / `Gobo1` / `Gobo2` / `Gobo3` | 图案轮 1 / 2 / 3 |
| `GOBO1` / `GOBO2` / `GOBO3` | 大写变体 |
| `GoboRot` / `Gobo2Rot` / `Gobo3Rot` / `GOBO3ROT` | 图案旋转 |
| `GOBO2_POS` | 图案定位 |

图案轮下标恒为 1..3，与灯具定义光学分组的三个固定槽位对应。

---

## 7. 棱镜

| 属性名 | 说明 |
| --- | --- |
| `Prism` / `Prism1` / `Prism2` / `Prism3` | 棱镜 1 / 2 / 3 |
| `PRISMA1` / `PRISMA2` | 大写变体 |
| `PrismRot` / `Prism1Rot` | 棱镜旋转 |
| `PrismPos` / `PRISMA1_POS` | 棱镜定位 |

---

## 8. 切割（Shaper）

| 属性名 | 说明 |
| --- | --- |
| `A1` `A2` `A3` `A4` / `B1` `B2` `B3` `B4` | 四刀八角点（灯库里的常见写法） |
| `ShaperA1` `ShaperA2` / `ShaperB1` `ShaperB2` | 同上，带 Shaper 前缀 |
| `ShaperC1` `ShaperC2` / `ShaperD1` `ShaperD2` | 第 3 / 第 4 片刀 |
| `BLADE1A` `BLADE1B` / `BLADE2A` `BLADE2B` / `BLADE3A` `BLADE3B` / `BLADE4A` `BLADE4B` | 逐刀两端（大写变体） |
| `Blade1Rot` `Blade2Rot` `Blade3Rot` `Blade4Rot` | 单片刀旋转 |
| `ShaperRot` / `SHAPERROT` | 刀架整体自转 |

刀序：**1 = 下、2 = 左、3 = 上、4 = 右**。每片刀的两端是它自己的逆时针端与顺时针端，不是"左右"。刀架自转与图案轮自转是两个能力，别接混。

---

## 9. 效果

| 属性名 | 说明 |
| --- | --- |
| `Effect` | 效果编号 |
| `EffectSpeed` | 效果速度 |
| `GenerationIndex` | 特效机的生成量 |

---

## 10. 怎么用这张表

**从 GDTF 导入时**：属性名由包内声明带入，绑定按名字自动推导，一般不用管这张表。

**手写通道库时**：按这张表取名，绑定才能自动推出来。用了表外的名字，需要在灯具编辑器里手工加一条绑定，把它接到某个能力上。

**排查通道不生效时**：去灯具编辑器的 **Validation** 页看这条属性在不在未绑定列表里。在，就是名字对不上或缺一条绑定。

> 控制类通道（Reset、Lamp On 等）没有对应能力是正常的——它们照常占地址、控台照常推得动，只是不驱动画面。

---

## 11. 相关文档

- [灯具定义资产](01_FixtureDefinition_zh.md)——绑定与 31 条能力操作码
- [灯具构建器](06_FixtureBuilder_zh.md)——绑定表怎么填
- [灯具运动控制](04_Motion_zh.md)
- [灯具编辑器](03_FixtureEditor_zh.md)——Validation 页
- [通道库编辑器](../editortools/10_FixtureLibraryEditor_zh.md)
