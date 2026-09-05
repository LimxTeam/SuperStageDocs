# 12 - 升降矩阵

> **所属模块**: SuperAssets  
> **适用对象**: 灯光设计师、舞美技术人员  
> **前置阅读**: [00 - DMX 系统概览](00_DMX_System_Overview_zh.md)、[03 - DMX 灯具 Actor 基础](03_DMX_Actor_Base_zh.md)  
> **最后核对**: 2026-06-28

---

## 一、概述

**LiftMatrix** 是一个 42 通道的轻机械灯具 Actor，显示名为 **LiftMatrix**，内置在 SuperStage 插件的 **SuperAssets** 模块中。

它由 5 个升降层组成，每层挂载 2 个效果组件，共 10 个发光单元。DMX 或属性控制会驱动整体升降、每个发光单元的效果和 RGB 颜色。

> 注意：本文不把 LiftMatrix 描述为可 Pan/Tilt 旋转的电脑灯。

---

## 二、放置和基本设置

1. 在 SuperStage 资产/灯具列表中找到 **LiftMatrix**。
2. 放入关卡后，按普通 DMX 灯具设置 **Universe**、**Start Address**、**ControlMode**。
3. 如果要用 DMX 控制，把 `ControlMode` 设为 DMX，并确保灯具库资产正常加载。
4. 如果要在属性面板手动预览，把 `ControlMode` 设为 Property，然后调节 **PosZ**。

当前实现中默认灯具库路径为：

`/SuperStage/SuperCore/SuperLight/Machinery/LiftMatrix/SL_Machinery_LiftMatrix`

---

## 三、可调参数

| 参数 | 分类 | 默认值 | 说明 |
| --- | --- | ---: | --- |
| **PosZ** | C.ControlParameter | 0.0 | 升降展开程度，当前实现按 0-1 归一化读取；在属性模式下可手动调节。 |
| **LiftRange** | B.DefaultParameter | 500.0 cm | 升降展开范围。数值越大，完全展开时各层之间拉开的距离越大。 |
| **MaxIntensity** | B.DefaultParameter | 5.0 | 传给效果组件的亮度上限参数。 |

`PosZ` 在 DMX 模式下读取 `PosZ` 属性，并以 16 位方式取值。属性模式下，面板中的 `PosZ` 会直接参与升降计算。

---

## 四、结构和运动

LiftMatrix 当前固定创建：

| 内容 | 数量 | 说明 |
| --- | ---: | --- |
| 升降层 | 5 | `LiftComponent_0` 到 `LiftComponent_4`。 |
| 效果组件 | 10 | 每个升降层 2 个效果组件，A 组使用 `SM_Effect`，B 组使用 `SM_Matrix`。 |
| 钢丝绳 | 4 | 使用 UE 内置圆柱体网格，位于四角。 |

升降位置由当前实现按 6 等分计算。5 个升降层占用第 1 到第 5 份，顶部留出 1 份空间：

```text
第 i 层，i = 0..4
收拢位置 = 初始偏移 - 5cm * i
展开位置 = 初始偏移 - LiftRange * (i + 1) / 6
最终位置 = Lerp(收拢位置, 展开位置, PosZ)
```

因此 `PosZ = 0` 时接近收拢状态，`PosZ = 1` 时按 `LiftRange` 完全展开。钢丝绳长度跟随最底层位置自动更新。

---

## 五、DMX 通道行为

该 Actor 的资产信息标记为 **42CH**。当前实现实际读取的主要属性如下：

| 属性 | 用途 | 说明 |
| --- | --- | --- |
| `PosZ` | 升降 | 16 位读取，控制 5 层整体展开/收拢。 |
| `Effect` | 效果 | 通过矩阵读取分配到 10 个效果组件，并查效果 LUT。 |
| `Red1` / `Green1` / `Blue1` | 颜色 | 通过矩阵 RGB 读取分配到 10 个效果组件。 |

42 通道的来源可以按当前实现理解为：`PosZ` 2 通道，加上 10 个发光单元的 `Effect`、`Red`、`Green`、`Blue` 共 40 通道。实际地址顺序以灯具库资产为准。

---

## 六、使用建议

- `Start Address` 要预留完整 42 通道，避免和后续灯具重叠。
- `LiftRange` 单位是厘米，设得过大会让模型展开距离明显变长。
- 如果发光面无颜色或效果，先确认 DMX 是否已经写入对应 Universe 和地址，再检查灯具库是否正常加载。
- 当前层数和每层发光单元数量是当前实现固定值，用户界面里没有层数设置。

---

## 七、相关文档

- [03 - DMX 灯具 Actor 基础](03_DMX_Actor_Base_zh.md)
- [07 - Patch 工具](07_Patch_Tools_zh.md)
- [10 - 舞台机械](10_Stage_Machinery_zh.md)
