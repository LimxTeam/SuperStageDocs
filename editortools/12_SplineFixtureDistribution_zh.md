# SuperStage 样条线灯具分布工具 用户手册

## 1. 功能范围

样条线灯具分布工具用于沿一个 Spline Component 放置灯具。它不需要源灯具，而是通过 **Fixture Sequence** 指定要生成的灯具，并按顺序循环排列。

序列条目既可以填**灯具定义资产**（数据驱动灯具，推荐），也可以填**灯具类**（蓝图灯或手写 C++ 灯）。同一条目里两者都填时，以定义资产为准。

创建完成后，样条线 Actor 会保留在场景中；生成的灯具是独立 Actor，不会继续绑定到样条线。

## 2. 打开方式

在编辑器左侧模式栏切换到 **SuperStage Edit Mode**，然后在模式工具栏中选择 **Spline**。

使用前需要在场景中选中包含 Spline Component 的 Actor，也可以直接选中 Spline Component。

## 3. 基本流程

1. 在场景中准备一个带 Spline Component 的 Actor。
2. 选中该 Actor 或其中的 Spline Component。
3. 打开 **Spline**。
4. 在 **Fixture Sequence** 中添加至少一个条目，并填上灯具定义资产或灯具类。
5. 设置 Count、Spacing、Start Offset、End Offset 和变换参数。
6. 检查预览。调整参数或直接拖动样条控制点，预览都会实时跟随。
7. 点击 **Create Fixtures** 创建灯具，或点击 **Cancel** 丢弃预览并退出。

创建操作支持编辑器撤销：按 Ctrl+Z 会把这一批灯具整体撤回，Ctrl+Y 可以再恢复，标签、文件夹、灯具编号与 DMX 地址一并回来。

## 4. Spline 状态

| 字段 | 说明 |
| --- | --- |
| Status | 当前操作提示 |
| Target Spline | 当前选中样条线所属 Actor 的名称 |
| Preview Count | 当前预览灯具数量 |

## 5. Fixture Sequence

Fixture Sequence 决定沿样条线循环生成哪些灯具。

| 参数 | 说明 | 范围/默认值 |
| --- | --- | --- |
| Fixture Definition | 要生成的灯具定义资产（数据驱动灯具） | 空 |
| DMX Mode | 该定义使用的 DMX 模式；留空取定义中的首个有效模式 | 空 |
| Fixture Class | 要生成的灯具类；仅在 Fixture Definition 为空时生效 | 空 |
| Repeat | 该条目在序列中连续重复的次数 | 1-100，默认 1 |
| Rotation | 该条目的灯具额外叠加的旋转 | 0, 0, 0 |

例如序列为 Spot × 2、Wash × 1 时，生成顺序为 Spot、Spot、Wash，然后继续循环。

数据驱动灯具全部型号共用同一个 Actor 类，所以型号必须由定义资产指定。只填 Fixture Class 选到 SuperFixtureActor 会得到没有定义的空壳灯具。

## 6. 分布参数

| 参数 | 说明 | 范围/默认值 |
| --- | --- | --- |
| Count | 要生成的灯具数量；为 0 时按 Spacing 计算数量 | 0-500，默认 10 |
| Spacing (cm) | Count 为 0 时使用的间距 | 最小 10，默认 100 |
| Start Offset % | 分布起点占样条线总长度的百分比 | 0-100，默认 0 |
| End Offset % | 分布终点占样条线总长度的百分比 | 0-100，默认 100 |

当 Count 大于 0 时，工具会在 Start Offset 到 End Offset 之间等距放置 Count 个灯具。当 Count 为 0 时，工具根据有效长度和 Spacing 计算数量。

Start Offset 必须小于 End Offset，否则没有有效路径长度可用于生成灯具。

Count 为 0 时算出的数量同样受 500 上限约束（与 Count 手输时的上限一致）。长样条配小间距会触发这个上限，此时 Status 会写明 `clamped to 500 (spacing too small)`，请改大 Spacing 或分段生成。

## 7. 变换参数

| 参数 | 说明 | 默认值 |
| --- | --- | --- |
| Rotation Offset | 所有灯具叠加的旋转 | 0, 0, 0 |
| Position Offset | 在样条线切线坐标系中的位置偏移，单位为厘米 | 0, 0, 0 |
| Follow Spline Rotation | 是否使用样条线方向作为灯具基础朝向 | 开启 |

开启 Follow Spline Rotation 时，灯具会沿样条线方向旋转，再叠加 Rotation Offset 和序列条目里的 Rotation。关闭时，灯具使用 Rotation Offset 作为统一朝向，再叠加序列条目的 Rotation。

## 8. 文件夹和命名

| 参数 | 说明 | 默认值 |
| --- | --- | --- |
| Create Folder | 创建后是否放入 World Outliner 文件夹 | 开启 |
| Folder Name | 文件夹名称；留空时使用 `LightArray` | 空 |

如果文件夹名已存在，工具会自动加序号。生成的 Actor 命名为 `型号名_灯具编号`，编号从全场已有的最大灯具编号往后接。标签里的编号就是写进灯具 DMX 属性的灯具编号，两者始终一致。

## 9. DMX 分配

| 参数 | 说明 | 默认值 |
| --- | --- | --- |
| Assign DMX Addresses | 是否按通道跨度顺排 Universe 与起始地址 | 开启 |

灯具编号（Fixture ID）无论开关如何都会分配，因为 Actor 标签里写着它。

开启时，工具从全场已占用的最后一个槽位往后排，按每支灯的通道跨度推进，跨过 512 自动进位 —— 口径与批量 Patch 工具一致。域号排到上限后剩余灯具会保留默认地址，并在输出日志里给出警告。

关闭时只分配灯具编号，Universe 与起始地址保持灯具默认值，交给批量 Patch 工具统一处理。此时 Status 会提示 `DMX addresses left to Patch Tool`。

## 10. 创建结果

点击 **Create Fixtures** 后，工具会：

- 在一个事务内生成正式灯具（因此整批可撤销）。
- 按 Fixture Sequence 循环使用灯具定义或灯具类。
- 设置每个灯具的位置、旋转和固定 1:1:1 缩放。
- 分配灯具编号，并按设置分配 DMX 地址。
- 按型号名与编号命名，按设置放入 World Outliner 文件夹。
- 选中刚生成的这一批灯具。
- 保留原样条线 Actor。
- 关闭工具。

## 11. 示例

| 场景 | 推荐设置 |
| --- | --- |
| 弧形灯排 | Count 设为需要的灯具数，Follow Spline Rotation 开启 |
| 闭合圆形边灯 | 使用闭合样条线，Start Offset 0%，End Offset 100% |
| 交替灯具 | 在 Fixture Sequence 中添加多个条目，并设置 Repeat |

## 12. 注意事项

- Fixture Sequence 至少需要一个填了定义或类的条目，否则不会生成预览。
- 样条线长度为 0 或 Start Offset 不小于 End Offset 时不会生成灯具。
- 修改样条线形状时，预览阶段会跟随更新；正式创建后的灯具不会继续跟随样条线。
- 预览态下按 Ctrl+S 保存关卡、启动 PIE 或切换到别的工具，都会**丢弃**预览而不是提交。提交只有 **Create Fixtures** 一个入口。
- 面板参数在同一次编辑器会话内会被记住；重启编辑器后回到默认值。
- 创建大量灯具时，编辑器响应速度取决于灯具数量、灯具复杂度和关卡规模。调整参数时预览是就地移动而不是重建，因此拖动滑块比首次生成要轻。
