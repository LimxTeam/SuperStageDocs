# 14 - 舞台 VFX（特效机）

> **所属模块**: SuperAssets（Actor）+ SuperCore（基类）
> **适用版本**: SuperStage 26H2.6 起
> **前置阅读**: [03 - DMX 灯具基础](03_DMX_Actor_Base_zh.md)

---

> **26H2.6 结构变化**：此前那个通用的特效机对象已改为 **16 个具体的特效机**，各自绑定自己的 Niagara 系统与灯库。你不再需要自备 Niagara 资源、也不用手工配通道映射——放进场景、配好地址就能用。

---

## 一、概述

特效机是以 DMX 触发的舞台特效模拟对象，在资产浏览器的 **SuperVFX** 分类下。

7 类共 16 个变体，**全部为 1 通道**：

| 类别 | 变体 | 数量 |
| --- | --- | ---: |
| 烟花 Fireworks | Fireworks_A / B / C / D | 4 |
| 彩带 Confetti | Confetti_Low / Medium / High / Max / Ultra | 5 |
| 烟雾 Smoke | Smoke_Low / High / Max | 3 |
| 火焰 Fire | Fire | 1 |
| 烟火 Pyro | Pyro | 1 |
| 气泡 Bubble | Bubble | 1 |
| 雪 Snow | Snow | 1 |

同类的不同变体是**档位**（Low / Medium / High / Max / Ultra）或**样式**（Fireworks A–D），不是同一个 Actor 的参数——按需要的量级直接选对应的那一个。

同目录下还有[舞台喷泉](16_Fountain_zh.md)与水池，它们不是 1 通道触发型，单独成篇。

---

## 二、随包资源

每个特效机自带全套资源，不需要用户准备：

| 资源 | 说明 |
| --- | --- |
| Niagara 系统 | 各类特效的粒子系统，7 类共 16 个 |
| 材质与贴图 | 粒子与模型材质 |
| 设备模型 | 特效机本体的静态网格 |
| 灯库 | `SL_<类别>_1CH` 通道库（`LTC_<变体>` 是它在浏览器里的缩略图） |

因为灯库随包提供，特效机可以像普通灯具一样纳入配接与控台流程。

---

## 三、放置与配接

1. 从资产浏览器的 **SuperVFX** 分类拖一个特效机进场景；
2. 按普通 DMX 灯具设置 **Universe**、**Start Address**、**ControlMode**，预留 1 个通道；
3. 从控台推那一条通道即可触发。

配接方式与其它 DMX 灯具完全一致，见 [Patch 工具](07_Patch_Tools_zh.md)。

---

## 四、DMX 控制

**单通道触发**：通道值映射到特效强度 / 档位。

| 项 | 说明 |
| --- | --- |
| 通道数 | 1 |
| 通道含义 | 触发强度 / 生成量 |
| 颜色 | **不经 DMX 控制**，由特效资产本身决定 |
| 粒子细节 | 同上 |

要按时间轴触发，把特效机配好地址后用 Sequencer 的 DMX 轨道，见 [08 - DMX 录制与回放](08_DMX_Recording_Playback_zh.md)。

---

## 五、可调参数

细节面板的 **B.DefaultParameter** 分组：

| 参数 | 默认值 | 范围 | 说明 |
| --- | ---: | --- | --- |
| MaxGenerationIndex | 1.0 | 0–100 | DMX 满值对应的最大生成量 |
| Lifetime | 1.0 | 0–100 | 粒子寿命 |
| RangeIndex | 1.0 | 0–100 | 作用范围 |

**C.ControlParameter** 分组（仅 `ControlMode = Property` 时显示）：

| 参数 | 默认值 | 范围 | 说明 |
| --- | ---: | --- | --- |
| GenerationIndex | 0.0 | 0–1 | 手动触发量，用于不接控台时预览 |

特效机继承自带轴的灯具基类，因此也可以旋转朝向；具体哪几支灯启用了轴，以该 Actor 的实现为准。

---

## 六、使用检查

| 现象 | 检查 |
| --- | --- |
| 推通道没反应 | `ControlMode` 是不是 `DMX`；Universe / 起始地址与控台是否一致 |
| 只有一点点粒子 | `MaxGenerationIndex` 太低，DMX 推满也只到这个量 |
| 属性模式下不动 | 调 `GenerationIndex`，不是 `MaxGenerationIndex` |
| 想要更大的量级 | 换用同类的更高档变体（如 Smoke_Low → Smoke_Max），而不是把参数调到极限 |

---

## 七、安全边界

特效机是**视觉模拟资产**。它不提供真实特效设备的点火控制、安全距离计算、危险品管理或法规审批依据。

**真实烟火、明火与特效执行必须由持证专业人员按当地法规实施。**

---

## 八、相关文档

- [03 - DMX 灯具基础](03_DMX_Actor_Base_zh.md)
- [07 - Patch 工具](07_Patch_Tools_zh.md)
- [08 - DMX 录制与回放](08_DMX_Recording_Playback_zh.md)
- [16 - 舞台喷泉](16_Fountain_zh.md)
