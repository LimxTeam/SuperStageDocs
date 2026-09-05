# 07 - Patch 工具

> **所属模块**: SuperTools  
> **适用对象**: 灯光编程师、舞美设计师  
> **前置阅读**: [03 - DMX 灯具基础](./03_DMX_Actor_Base_zh.md)  
> **最后更新**: 2026-04-14

---

## 一、概述

Patch 工具用于给场景中的 SuperDMX 灯具分配和检查 DMX 地址。当前面板包含两部分：

| 区域 | 用途 |
|------|------|
| 顶部参数栏 | 给当前选中的灯具批量写入 Universe、Start Address 和 FixtureID |
| 下方 Patch 预览 | 查看场景中所有 SuperDMX 灯具，并通过表格或通道网格微调 Patch |

只有 SuperStage DMX 灯具会被这些工具处理。普通 Actor 不会出现在 Patch 列表中。

---

## 二、打开方式

在 SuperStage 工具栏的 SuperDMX 菜单中点击 **PatchTool**，打开 **Super Patch Tool** 标签页。

---

## 三、顶部参数栏

| 控件 | 说明 |
|------|------|
| **Start Universe** | 批量 Patch 的起始 Universe，范围 1-512 |
| **Start Address** | 第一台选中灯具的起始地址，范围 1-512 |
| **Start Fixture ID** | 第一台选中灯具的起始 FixtureID，范围 1-9999 |
| **SelectAllDevices** | 选中关卡中的所有 SuperDMX 灯具 |
| **ClearPatch** | 将当前选中的灯具重置为 Universe=1、StartAddress=1。**灯具编号保持不动** |
| **RenameID** | 勾选时，Apply 后将灯具 Actor 名称改为“灯具类型名_FixtureID”的形式 |
| **Apply** | 按当前起始参数给选中的灯具写入 Patch |
| **Refresh** | 刷新下方 Patch 预览和通道网格 |

打开面板时，工具会扫描场景中已有灯具，并把起始值设到已使用 Patch 后面的下一个位置；如果场景里没有灯具，则从 1.1 和 FixtureID 1 开始。

---

## 四、批量 Patch

1. 在场景中选中需要分配地址的 SuperDMX 灯具。
2. 设置 **Start Universe**、**Start Address** 和 **Start Fixture ID**。
3. 根据需要勾选或取消 **RenameID**。
4. 点击 **Apply**。

Apply 会对当前选中的灯具执行以下操作：

- 第一台灯具使用设置的起始 Universe 和 Start Address。
- 下一台灯具从上一台灯具的通道跨度后继续分配。
- 如果当前 Universe 剩余地址放不下下一台灯具，会切到下一个 Universe 的地址 1。
- Universe 超过 512 或单台灯具通道跨度超过 512 时，该项会被跳过。
- FixtureID 从 **Start Fixture ID** 开始递增，并跳过场景中未选中灯具已经占用的 FixtureID。
- FixtureID 为 1 的未选中灯具按默认未分配值处理，不参与占用检测。

> **注意**：手动多选时，工具使用当前编辑器选择顺序。点击 **SelectAllDevices** 后，工具会按父 Actor 名称和自身 Actor 名称的自然排序分配。

---

## 五、Patch 预览表

下方左侧表格显示关卡中的所有 SuperDMX 灯具：

| 列 | 说明 | 可编辑 |
|----|------|--------|
| **Model** | 灯具型号/类型名 | 否 |
| **Name** | Actor 显示名称 | 是 |
| **FixtureID** | 灯具编号 | 是 |
| **Universe** | DMX Universe | 是，范围 1-512 |
| **StartAddress** | 起始地址 | 是，范围 1-512 |

编辑表格中的 Name、FixtureID、Universe 或 StartAddress 后，会直接写回对应灯具。Universe 和 StartAddress 会被限制在 1-512，FixtureID 最小为 1。

表格会按灯具型号/类型分组，再按名称末尾数字排序。FixtureID 重复或 DMX 地址重叠时，相关字段会以红色显示，提醒你检查 Patch。

---

## 六、通道网格

右侧通道网格按 Universe 显示 1-512 地址格。灯具占用的地址会显示为彩色块：

- 点击色块会选中对应灯具，并同步到表格和编辑器选择。
- Ctrl + 点击可以切换多选。
- 拖动色块可以调整灯具的 Universe 和 StartAddress。
- 将选中的灯具拖放到网格空位时，会从落点开始连续分配地址和 FixtureID。
- 地址重叠会以红色覆盖显示，FixtureID 重复会在灯具起始格左侧显示红色标记。

---

## 七、撤销

批量 Apply、ClearPatch、网格拖拽和拖放 Patch 都使用 UE 编辑器事务，可以用 **Ctrl+Z** 撤销。表格中的部分文本编辑也会调用 Actor 修改流程，但使用前仍建议先确认关卡可正常保存。

---

## 八、常见问题

### Q: Apply 后没有任何变化？
确认当前确实选中了 SuperStage DMX 灯具。普通 StaticMeshActor 不会被 Patch 工具处理。

### Q: 为什么 ClearPatch 之后灯具编号还在？
这是有意的。ClearPatch 只重置 Universe 与起始地址，**编号刻意不动**：清 50 支灯如果把编号一并设成 1，结果就是 50 支共用编号 1，控台再也分不开它们——「清除配接」不该顺手把灯具的身份也毁掉。地址反正下一次 Apply 会重写，编号留着还能对照。

### Q: 为什么有些灯具没有被 Apply？
如果灯具通道跨度超过 512，或分配过程导致 Universe 超过 512，工具会跳过该灯具。

### Q: 红色字段代表什么？
FixtureID 列变红表示编号重复；StartAddress 列或网格地址变红表示同一 Universe 内地址范围重叠。

---

> **下一步**：请阅读 [08 - DMX 录制与回放](./08_DMX_Recording_Playback_zh.md)。
