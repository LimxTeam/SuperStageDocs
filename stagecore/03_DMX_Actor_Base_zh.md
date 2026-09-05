# 03 - DMX 灯具基础

本文档说明场景中 DMX 灯具的基础设置。这里不介绍 C++ API、蓝图宏或继承结构，只说明普通用户需要在 Details 面板中配置什么。

## 1. 适用对象

以下类型的 Actor 通常都有相同的 DMX 基础设置：

- 电脑灯、染色灯、矩阵灯等灯具 Actor。
- 舞台机械、轨道机械。
- LED 灯带和其他通过 DMX 驱动的舞台对象。

具体灯具能响应哪些功能，取决于它自身实现和绑定的 Fixture Library。

## 2. DMX Patch 参数

选中场景中的灯具 Actor，在 Details 面板中找到 DMX 相关分组。

| 参数 | 说明 | 常见范围 |
| --- | --- | --- |
| Universe | 灯具读取的 SuperStage 内部 Universe。需要与 DMX 配置面板的 Start Universe 对齐结果一致。 | 1 起 |
| Start Address | 灯具在该 Universe 中的起始通道。 | 1 - 512 |
| Fixture ID | 灯具编号，用于整理和导出数据。 | 项目内自定 |
| Fixture Library | 该灯具使用的通道表资产。 | 选择对应灯具库 |
| ControlMode | `DMX` 使用外部/回放 DMX 数据；`Property` 使用细节面板属性。 | DMX / Property |

同一个 Universe 内，多个灯具占用的通道范围不应重叠。可以用 Patch 工具和 Activity Monitor 辅助检查。

## 3. Universe 与 Start Address

灯具实际读取的绝对通道地址（1 基）是：

```text
模块基址 = Start Address + (模块 Patch − 1)
绝对地址 = 模块基址 + 属性通道偏移 − 1
```

普通单模块灯具的 Patch 是 1，两式合起来就是 `Start Address + 偏移 − 1`。

**矩阵灯每个模块各有自己的 Patch**，所以同一个属性名在不同模块上解出的是不同地址——逐格控制就是这么实现的。

如果控台显示 Universe 1，但 SuperStage 收到的数据出现在另一个 Universe，先检查 DMX 配置面板中的 Start Universe 设置。

示例：

| 设置 | 结果 |
| --- | --- |
| Universe = 1, Start Address = 101 | 灯具从 SuperStage Universe 1 的 101 通道开始读取。 |
| 灯具库跨度为 20 通道 | 该灯具通常占用 101 - 120。 |

## 4. Fixture Library

Fixture Library 定义灯具通道表。绑定正确灯具库后，灯具才能按属性名读取 Dimmer、Pan、Tilt、Color、Gobo 等功能。

如果灯具没有绑定 Fixture Library：

- 仍可能按原始通道号读取 DMX。
- 依赖属性名的灯具功能通常不会正常工作。
- Patch 工具计算通道跨度时可能无法得到正确结果。

同型号、同 DMX 模式的多台灯具可以共用一个 Fixture Library，只需要分别设置 Universe、Start Address 和 Fixture ID。

## 5. ControlMode

| 模式 | 说明 |
| --- | --- |
| DMX | 灯具从 DMX 缓存读取数据。适合连接控台、接收网络 DMX 或回放 Sequencer 数据。 |
| Property | 灯具使用自身属性值，不读取 DMX 缓存。适合不接控台时做静态调试或关键帧设置。 |

如果灯具处于 `Property` 模式，外部 DMX 推杆不会驱动它。排查“收到信号但灯具不动”时要先确认该模式。

## 6. 通道跨度

通道跨度来自绑定的 Fixture Library：把全部模块、全部属性里**大于 0** 的 Coarse / Fine / Ultra 折算成绝对地址，取 **最大地址 − 最小地址 + 1**。

**它是跨度不是通道数。** 通道表中间留了空档时，跨度会大于实际用到的通道数量——配接时按跨度留位置才不会与下一台灯撞上。

用途：

- Patch 工具计算下一台灯具起始地址。
- 检查同一 Universe 内是否可能发生地址重叠。
- 导出或统计灯具信息时提供占用通道参考。

如果通道跨度显示不符合预期，优先检查 Fixture Library 的模块 Patch 和各属性通道偏移。

## 7. 排查

### Activity Monitor 有数据，但灯具不动

1. 确认灯具 ControlMode 是 `DMX`。
2. 确认灯具 Universe 与 Activity Monitor 中有数据的 Universe 一致。
3. 确认 Start Address 与控台 Patch 一致。
4. 确认 Fixture Library 已绑定且通道表正确。
5. 确认控台推的是该灯具真正占用的通道。

### 多台灯具地址冲突

使用 Patch 工具查看同一 Universe 内的地址占用，重新分配 Start Address。不要只看 Fixture ID，Fixture ID 不决定 DMX 通道占用。

## 8. 下一步

- [灯具运动控制](../fixture/04_Motion_zh.md)
- [07 - Patch 工具](07_Patch_Tools_zh.md)
- [06 - DMX 活动监视器](06_DMX_Activity_Monitor_zh.md)
