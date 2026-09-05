# 08 - DMX 录制与回放

> **所属模块**: SuperTools / SuperDMX Sequencer  
> **适用对象**: 灯光编程师、虚拟制作技术人员  
> **前置阅读**: [01 - DMX 网络配置](./01_DMX_Network_Configuration_zh.md)  
> **最后更新**: 2026-04-14

---

## 一、概述

SuperStage 可以通过 UE 的 Take Recorder 把 SuperDMX 子系统收到的 DMX 缓冲区录制到 Level Sequence 中。回放时，SuperDMX 轨道会把曲线值写回 SuperDMX 子系统；如果 DMX 输出已启用，也会通过当前协议输出到网络。

这项功能适合把外部控台发送到 SuperStage 的 DMX 数据保存成可回放的序列。它不是控台工程文件导入器，也不是完整的网络抓包工具。

---

## 二、录制入口

1. 打开 UE 菜单 **Window > Cinematics > Take Recorder**。
2. 在 Take Recorder 中点击 **+ Source**。
3. 添加 **Super DMX Input**。

录制源在列表中的显示名称为 **Super DMX (Universes)**。

---

## 三、录制参数

| 参数 | 说明 | 默认值 | 范围 |
|------|------|--------|------|
| **UniverseMin** | 要录制的最小内部 Universe | 1 | 1-512 |
| **UniverseMax** | 要录制的最大内部 Universe | 16 | 1-512 |

说明：

- 如果 UniverseMin 大于 UniverseMax，当前实现会自动按较小值到较大值处理。
- 录制范围使用 SuperStage 内部 Universe 编号。输入协议的 Start Universe 偏移已经在 SuperDMX 输入阶段处理完成。
- 使用 sACN 时，开始录制会临时加入所选 Universe 范围的多播组；停止录制后会清除这次临时订阅，回到 DMX 配置的默认接收范围。

---

## 四、开始录制

1. 在 DMX 配置面板中启用输入，并确认协议、端口、本地 IP、Start Universe 设置正确。
2. 用 DMX 活动监视器确认目标 Universe 有数据。
3. 在 Take Recorder 中添加 **Super DMX Input**，设置 UniverseMin 和 UniverseMax。
4. 点击 Take Recorder 的 **Record**。
5. 控台播放或操作完成后，点击 **Stop**。

录制需要有效的 SuperStage 授权。当前实现中创建 DMX 轨道前会检查授权；如果没有授权，可能不会创建录制轨道。

---

## 五、录制结果

录制会在当前 Root Sequence 的 MovieScene 中创建或复用 SuperDMX 轨道：

```
Level Sequence
  ├── Universes_1
  │     └── Section (Universe = 1)
  ├── Universes_2
  │     └── Section (Universe = 2)
  └── ...
```

当前实现行为：

- 每个录制的 Universe 使用一个独立的 SuperDMX Track。
- Track 显示名为 `Universes_{Universe}`，例如 `Universes_1`。
- 每个 Section 记录一个 Universe。
- Section 内部按通道号 1-512 各存一条曲线。
- 关键帧插值模式为常量模式，避免 DMX 值被平滑过渡。
- 录制时会比较上一帧数据，只给变化的通道写关键帧；不是每帧固定写入全部 512 个通道。

---

## 六、回放行为

播放 Sequencer 时，SuperDMX Section 会评估当前时间的曲线值，并调用 SuperDMX 子系统发送缓冲区。

回放规则：

- 如果 Section 标记为录制中，则不会发送，避免录制和回放互相影响。
- 回放前会先读取当前 Universe 缓冲区作为基础。
- 只覆盖该 Section 中已有曲线的通道。
- 没有曲线的通道保留当前缓冲区值。
- 输出 Universe 会再经过 DMX 输出配置中的 Start Universe 偏移。
- 如果输出未启用，回放仍会更新 SuperDMX 内部缓冲区，活动监视器和虚拟灯具可以看到变化。
- 如果输出已启用，SuperDMX 会按当前协议发送 Art-Net 或 sACN 数据。

---

## 七、在 Sequencer 中使用

SuperTools 模块会在 Sequencer 中注册 **DMX Track** 菜单项，可以手动添加 SuperDMX Track。手动添加的空轨道需要有包含通道曲线的 Section 才能产生回放输出。

当前用户手册不把“手动编辑每个 DMX 通道曲线”作为推荐工作流，因为当前实现没有提供完整的用户级通道曲线编辑面板。可靠流程仍然是通过 Take Recorder 录制实际输入数据，再进行常规 Sequencer 层面的剪辑、移动或删除区段。

---

## 八、文件大小和性能

录制数据量取决于：

- Universe 范围大小
- 录制时长
- Take Recorder/引擎 Tick 频率
- DMX 值实际变化频率

当前实现会跳过未变化的通道，因此不能用“Universe 数量 × 512 通道 × 每帧”直接估算最终资产大小。为了减少资产体积，建议只录制实际需要的 Universe 范围。

---

## 九、常见问题

### Q: Take Recorder 里找不到 Super DMX Input？
确认 SuperTools 模块已加载，并且项目启用了 Take Recorder 相关编辑器功能。

### Q: 录制后没有生成 `Universes_` 轨道？
检查是否有有效 SuperStage 授权，并确认录制使用的是 Root Sequence。当前实现会在 Root Sequence 的 MovieScene 上创建轨道。

### Q: 录制轨道有了，但没有通道数据？
先用活动监视器确认 Universe 范围内确实有非零或变化的数据。录制器会跳过空缓冲和未变化通道。

### Q: 回放时虚拟灯具有变化，但外部设备没动？
检查 DMX 配置面板的输出是否启用，输出协议、Local IP、Remote IP、端口和 Start Universe 是否正确。输出未启用时，回放只更新内部缓冲区。

### Q: Sequencer 右键菜单里的 “Start DMX Recording” 能用吗？
**不能。** 轨道右键菜单上的 **Start DMX Recording** 与 **Stop DMX Recording** 两项都是空实现——点了没有任何反应，也不报错。请使用 Take Recorder 的 **Super DMX Input** 录制源。

同一菜单里的 **DMX Track**（添加轨道）是可用的。

---

> **下一步**：请阅读 [09 - MA 控台导出](./09_Export_To_MA_zh.md)。
