# 13 - LED 灯带特效

> **所属模块**: SuperAssets  
> **适用对象**: 灯光设计师、舞美技术人员  
> **前置阅读**: [03 - DMX 灯具 Actor 基础](03_DMX_Actor_Base_zh.md)  
> **最后核对**: 2026-06-28

---

## 一、概述

**SuperLightStripEffect** 是一个 9 通道 DMX 灯带/发光材质控制 Actor，位于 SuperStage 插件的 **SuperAssets** 模块。

它会创建一个自身网格组件，也可以把同一个动态材质实例应用到多个场景中的 `StaticMeshActor`。这样一组 DMX 通道可以统一控制一批灯带模型的亮度、频闪、颜色和效果材质参数。

---

## 二、放置和绑定模型

1. 在 SuperStage 资产列表中放置 **SuperLightStripEffect**。
2. 如果要让 Actor 自身显示灯带效果，给 **StaticMeshEffect** 指定一个静态网格。
3. 如果要控制场景中已有模型，在 **TargetMeshActors** 数组里添加对应的 `StaticMeshActor`。
4. 设置 **MaterialIndex**，决定替换目标模型的第几个材质槽。
5. 设置 **Universe** 和 **Start Address**，给它预留 9 个通道。

编辑器里也有右键绑定入口：选中静态网格 Actor 后，可通过 SuperStage 的右键菜单把模型追加到某个 LightStripEffect 的目标列表；工具会去重，并在绑定后刷新材质。

---

## 三、可调参数

| 参数 | 分类 | 默认值 | 范围 | 说明 |
| --- | --- | ---: | --- | --- |
| **TargetMeshActors** | A.ModelTargets | 空 | — | 要批量应用灯带材质的 `StaticMeshActor` 列表。**只在场景实例上可编辑**，不能在类默认值里配 |
| **StaticMeshEffect** | A.ModelTargets | 空 | — | Actor 自身网格组件使用的静态网格 |
| **MaxLightIntensity** | B.DefaultParameter | 1.0 | 0–1000 | 写入材质参数 `MaxBrightness` |
| **MaterialIndex** | B.DefaultParameter | 0 | 0–255 | 材质槽索引。工具会检查索引是否在有效材质槽范围内 |
| **Dimmer** | C.ControlParameter | 0.0 | 0–1 | 亮度归一化值 |
| **Strobe** | C.ControlParameter | 0.0 | 0–1 | 频闪速度归一化值 |
| **Effect** | C.ControlParameter | 0.0 | 0–1 | 写入材质参数 `Effect`，映射到 0–10 |
| **Speed** | C.ControlParameter | **0.5** | 0–1 | 写入材质参数 `Speed`，映射到 −10–10。**0.5 即静止** |
| **Width** | C.ControlParameter | 0.0 | 0–1 | 写入材质参数 `Width`，映射到 0–10 |
| **Direction** | C.ControlParameter | 0.0 | 0–1 | 写入材质参数 `EffectDirection` |
| **Color** | C.ControlParameter | 白色 | — | 写入材质参数 `LightColor` |

> **常规使用请用 DMX 模式。** `C.ControlParameter` 这一组只在 ControlMode 为 `Property` 时显示，但**当前实现在属性模式下不写材质**——读取与应用这一步在属性模式会直接跳过。（唯一的例外是频闪：`Strobe > 0` 时每帧重算亮度那条路不看控制模式，但它算的仍是最近一次由 DMX 写入的 `Dimmer`。）

---

## 四、DMX 通道

该 Actor 的资产信息标记为 **9CH**，并按以下属性名读取粗通道：

| 通道 | 属性 | 材质/行为 |
| ---: | --- | --- |
| 1 | `Dimmer` | 参与写入 `Brightness`。 |
| 2 | `Strobe` | 非 0 时启用频闪倍率，影响 `Brightness`。 |
| 3 | `Effect` | `Lerp(0, 10, Effect)` 后写入 `Effect`。 |
| 4 | `Speed` | `Lerp(-10, 10, Speed)` 后写入 `Speed`。 |
| 5 | `Width` | `Lerp(0, 10, Width)` 后写入 `Width`。 |
| 6 | `Direction` | 写入 `EffectDirection`。 |
| 7 | `Red` | RGB 颜色红色分量。 |
| 8 | `Green` | RGB 颜色绿色分量。 |
| 9 | `Blue` | RGB 颜色蓝色分量。 |

实际材质外观由 `/SuperStage/SuperCore/LightMaterial/MainMaterial/M_Effect` 决定。当前实现只写入上述参数，不在文档中承诺固定的效果名称或数量。

---

## 五、频闪行为

当前实现没有继续把 `Strobe` 和 `StrobeMode` 写入材质。频闪由灯具逻辑计算：

- `Strobe > 0` 时启用频闪。
- 频闪速度缓存为 `Strobe * 255`。
- 每帧计算一个线性三角波加平滑过渡的倍率。
- 最终写入材质 `Brightness = Dimmer * StrobeMultiplier`。

因此本文档不再描述多种频闪模式。实际可见效果取决于 `Dimmer`、`Strobe`、帧率和材质响应。

---

## 六、常见问题

### 目标模型没有显示灯带材质？

检查以下几项：

- 目标是否是 `StaticMeshActor`。
- `MaterialIndex` 是否落在目标模型实际材质槽范围内。
- `MaxLightIntensity` 是否大于 0。
- DMX 是否写入了正确的 Universe 和 9 个通道。
- 灯带材质是否已经刷新；右键绑定会调用刷新，其他情况下可重新触发灯具初始化。

### 多个模型能不能独立控制？

同一个 SuperLightStripEffect 会把同一个动态材质实例应用到所有目标模型，所以这些目标会统一变化。需要独立控制时，放置多个 SuperLightStripEffect，并分配不同地址。

### 效果名称在哪里设置？

当前实现只把 `Effect` 数值写入材质参数。具体 0-10 对应什么视觉效果由 `M_Effect` 材质决定，不提供固定名称表。

---

## 七、相关文档

- [03 - DMX 灯具 Actor 基础](03_DMX_Actor_Base_zh.md)
- [07 - Patch 工具](07_Patch_Tools_zh.md)
- [10 - 舞台机械](10_Stage_Machinery_zh.md)
