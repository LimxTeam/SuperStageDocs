# SuperStage 通道库编辑器 用户手册

> 适用版本：SuperStage 26H2.6 起
>
> 26H2.6 起，这份资产在文档中统一称作**通道库**（Super Fixture Library，前缀 `CL_`）。它描述的是一个 DMX 模式的通道表；描述"这支灯是什么"的是[灯具定义](../fixture/01_FixtureDefinition_zh.md)（资产名就是型号名，不带前缀）。两者的分工见[灯具系统总览](../fixture/00_FixtureSystem_Overview_zh.md)。

## 1. 功能范围

通道库编辑器用于查看和编辑通道库资产。资产保存灯具名称、制造商、物理信息、GDTF 外链路径，以及 DMX 模块、属性、子属性和槽位。

编辑器可以导入或导出 MA2 XML、GDTF 和 SuperStage JSON 文件。不同厂商文件内容可能存在差异，导入后请检查模块、通道和槽位是否符合实际灯具。

## 2. 打开方式

在内容浏览器中双击通道库资产（`CL_` 开头）。

如果需要创建新资产，可以在内容浏览器中右键创建 Super Fixture Library。

面板右上角有帮助按钮。

## 3. 顶部信息区

| 字段 | 说明 |
| --- | --- |
| Fixture Name | 灯具型号名称 |
| Manufacturer | 制造商 |
| Power (W) | 额定功率，最小值 0 |
| Weight (kg) | 灯具重量，最小值 0 |
| Channel Count | 根据当前模块和属性通道计算出的占用通道数 |
| GDTF Library | 绑定的 GDTF 外链路径 |
| MA2 Library | **已废弃，面板中只读**。26H2.6 起 MA2 XML 由绑定的 GDTF 现场转换生成，不再需要手工维护 |

GDTF Library 只能绑定到 SuperStage 插件 `FixtureLibrary/GDTF` 目录内的文件。手动输入路径时，工具会把路径规范化为插件内路径；如果路径不在允许目录内，会提示并拒绝保存。

> **灯库目录 26H2.6 由 `GTDF` 更正为 `GDTF`**，存量资产记录的源路径已批量修正为插件相对形式。

资产本身还包含 Beam Angle、Beam Intensity 和 Body Size 等字段，主要用于灯库导出，不一定全部显示在顶部编辑区。

## 4. 模块列表

左侧模块列表显示当前灯库的 DMX Modules。一个模块通常对应灯具的一种 DMX 模式。

可用操作：

| 操作 | 说明 |
| --- | --- |
| Add Module | 添加一个新模块 |
| 重命名 | 直接编辑模块名称 |
| 上移/下移 | 调整模块顺序 |
| Copy | 复制模块并插入到当前模块后面，复制项名称会追加 `_Copy` |
| Delete | 删除模块 |

选中模块后，右侧显示该模块的属性列表。

## 5. 属性层级

灯库数据分为三层：

| 层级 | 说明 |
| --- | --- |
| Attribute | DMX 属性，例如 Dimmer、Pan、Tilt、ColorWheel |
| Sub-Attribute | 属性下的功能区间 |
| Channel Set | 子属性下的具体槽位，例如颜色、图案、棱镜选择 |

双击 Attribute 进入 Sub-Attribute；双击 Sub-Attribute 进入 Channel Set。顶部面包屑可以返回上级。

## 6. Attribute 列表

| 列 | 说明 |
| --- | --- |
| No. | 顺序编号 |
| Attrib | 属性名称 |
| Category | 属性分类 |
| Coarse | 粗通道，范围 1-512 |
| Fine | 细通道，0 表示不使用，范围 0-512 |
| Ultra | 超细通道，0 表示不使用，范围 0-512 |
| Default | 默认值，按百分比，范围 0-100 |
| Highlight | 高亮值，按百分比，范围 0-100 |

同一模块内如果出现重复属性名，编辑器会用警告颜色提示。

## 7. Sub-Attribute 列表

| 列 | 说明 |
| --- | --- |
| No. | 顺序编号 |
| DMX Start | 起始 DMX 值，范围 0-255 |
| DMX End | 结束 DMX 值，范围 0-255 |
| Physical Range | 物理值范围 |
| Channel Sets | 当前子属性下的槽位数量 |
| Strobe Mode | Dimmer 或 Strobe 分类下显示 |
| Rotation Mode | Position 分类下显示 |

## 8. Channel Set 列表

| 列 | 说明 |
| --- | --- |
| No. | 顺序编号 |
| Name | 槽位名称 |
| DMX Start | 起始 DMX 值，范围 0-255 |
| DMX End | 结束 DMX 值，范围 0-255 |
| Physical Range | 物理值范围 |

根据当前属性分类，Channel Set 会显示额外列：

| 分类 | 额外列 |
| --- | --- |
| Gobo | Gobo Mode、Texture |
| Color | Color、Color Index |
| Prism | Prism Selection |

## 9. 工具栏

| 按钮 | 说明 |
| --- | --- |
| Add | 在当前层级添加新项 |
| Delete | 删除当前选中项 |
| Copy | 复制当前层级选中项 |
| Paste | 粘贴到当前层级列表末尾 |
| Move Up / Move Down | 移动当前层级选中项 |
| Export MA2 | 导出 MA2 Fixture XML |
| Import MA2 | 从 MA2 Fixture XML 导入到当前资产 |
| Export GDTF | 导出 GDTF 文件或 description XML |
| Import GDTF | 从 GDTF、GTDF 或 XML 文件导入到当前资产 |
| Import JSON | 从 SuperStage JSON 灯库导入 |
| Export JSON | 导出 SuperStage JSON 灯库 |

导入操作会修改当前灯库资产。导入外部文件后，应检查 Fixture Name、Manufacturer、Modules、通道号和槽位范围。

## 10. 保存和影响

编辑器会在修改后标记资产为已修改。使用编辑器保存功能保存资产。

修改通道库会影响引用它的全部灯具定义。修改 Coarse、Fine、Ultra 或模块结构后，需要**重新编译**受影响的灯具定义（内容浏览器右键 → Compile），再检查场景中相关灯具的 DMX 行为。

## 11. 注意事项

- 通道号从 1 开始；Fine 和 Ultra 为 0 时表示不使用。
- Channel Count 由当前模块属性中大于 0 的 Coarse、Fine、Ultra 通道计算。
- 导入 MA2、GDTF 或 JSON 不是对所有第三方文件的兼容承诺；导入后必须人工检查。
- 外链路径只是记录外部灯库文件位置，不等同于自动同步所有外部文件内容。
- 本编辑器已接入撤销体系，三级列表的增删改都可以 Ctrl+Z。
- 从整支灯的角度建库时，用[灯具编辑器](../fixture/03_FixtureEditor_zh.md)更合适——它同时管定义、绑定与预览。
