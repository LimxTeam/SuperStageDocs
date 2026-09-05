# SuperStage DMX Patch Preview 用户手册

## 功能定位

DMX Patch Preview 用于查看和编辑当前关卡里的 SuperStage DMX 灯具 Patch 信息。面板左侧是灯具列表，右侧是 DMX 通道网格。

面板只收集 SuperStage DMX 灯具，不显示普通 UE 灯光或其他非 DMX Actor。

## 打开方式

它是 **Super Patch Tool** 面板的下半部分：SuperStage 工具栏 → **SuperDMXTool** → **PatchTool**，打开后下方即是配接预览。

## 列表字段

| 列 | 说明 | 是否可编辑 |
| --- | --- | --- |
| Model | 灯具型号/类型名。 | 否 |
| Name | Actor 在关卡中的标签。 | 是 |
| FixtureID | 灯具编号。重复时会标红。 | 是 |
| Universe | DMX Universe。 | 是 |
| StartAddress | 起始地址。与同 Universe 其他灯具地址范围重叠时会标红。 | 是 |

## 可编辑范围

| 字段 | 当前实现限制 |
| --- | --- |
| FixtureID | 最小 1，最大为 `INT32_MAX`。 |
| Universe | 1-512。 |
| StartAddress | 1-512。 |

输入数值会被钳制到允许范围内。

## 排序规则

列表刷新时按以下顺序排序：

1. 灯具型号/类型名。
2. Actor 标签末尾的数字。
3. Actor 标签文本。

例如同类灯具中，名称末尾带数字的灯具会按数字顺序排列。

## 冲突提示

面板刷新列表时会计算：

- FixtureID 是否重复。
- 同一 Universe 内的地址范围是否重叠。

冲突项会用红色文本提示。地址范围按灯具当前通道占用计算。

## 选择联动

- 在左侧列表选中一行或多行，会同步选择关卡中的对应灯具，并高亮右侧通道网格。
- 在右侧通道网格选择灯具，也会同步选择关卡 Actor 和左侧列表行。
- 在通道网格拖拽提交地址后，列表会刷新。

## 使用注意

- 修改 Name 会更新关卡中的 Actor 显示名称。
- 修改 FixtureID、Universe、StartAddress 会写回对应灯具的 Patch 设置并触发编辑器更新。
- 冲突标记是在刷新列表时计算的；如果刚修改完地址，建议重新打开面板或通过通道网格刷新后再检查当前冲突状态。
