# SuperStage 样条线灯具分布工具 用户手册

## 1. 功能范围

样条线灯具分布工具用于沿一个 Spline Component 放置灯具。它不需要源灯具，而是通过 **Fixture Sequence** 指定要生成的灯具类，并按顺序循环排列。

创建完成后，样条线 Actor 会保留在场景中；生成的灯具是独立 Actor，不会继续绑定到样条线。

## 2. 打开方式

在编辑器左侧模式栏切换到 **SuperStage Edit Mode**，然后在模式工具栏中选择 **Light Array Tool**。

使用前需要在场景中选中包含 Spline Component 的 Actor，也可以直接选中 Spline Component。

## 3. 基本流程

1. 在场景中准备一个带 Spline Component 的 Actor。
2. 选中该 Actor 或其中的 Spline Component。
3. 打开 **Light Array Tool**。
4. 在 **Fixture Sequence** 中添加至少一种灯具类。
5. 设置 Count、Spacing、Start Offset、End Offset 和变换参数。
6. 检查预览。
7. 点击 **Create Fixtures** 创建灯具，或点击 **Done** 退出。

创建操作支持编辑器撤销。

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
| Fixture Class | 要生成的 SuperStage DMX 灯具类 | 空 |
| Repeat | 该灯具类在序列中连续重复的次数 | 1-100，默认 1 |
| Rotation | 该类灯具额外叠加的旋转 | 0, 0, 0 |

例如序列为 Spot × 2、Wash × 1 时，生成顺序为 Spot、Spot、Wash，然后继续循环。

## 6. 分布参数

| 参数 | 说明 | 范围/默认值 |
| --- | --- | --- |
| Count | 要生成的灯具数量；为 0 时按 Spacing 计算数量 | 0-500，默认 10 |
| Spacing (cm) | Count 为 0 时使用的间距 | 最小 10，默认 100 |
| Start Offset % | 分布起点占样条线总长度的百分比 | 0-100，默认 0 |
| End Offset % | 分布终点占样条线总长度的百分比 | 0-100，默认 100 |

当 Count 大于 0 时，工具会在 Start Offset 到 End Offset 之间等距放置 Count 个灯具。当 Count 为 0 时，工具根据有效长度和 Spacing 计算数量。

Start Offset 必须小于 End Offset，否则没有有效路径长度可用于生成灯具。

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

如果文件夹名已存在，工具会自动加序号。生成的 Actor 会按灯具类型名和现有编号继续命名。

## 9. 创建结果

点击 **Create Fixtures** 后，工具会：

- 将预览灯具转为正式 Actor。
- 按 Fixture Sequence 循环使用灯具类型。
- 设置每个灯具的位置、旋转和固定 1:1:1 缩放。
- 按设置放入 World Outliner 文件夹。
- 保留原样条线 Actor。
- 关闭工具。

生成的灯具不会自动分配唯一 DMX 地址。创建后请根据项目需要使用批量 Patch 工具检查和分配地址。

## 10. 示例

| 场景 | 推荐设置 |
| --- | --- |
| 弧形灯排 | Count 设为需要的灯具数，Follow Spline Rotation 开启 |
| 闭合圆形边灯 | 使用闭合样条线，Start Offset 0%，End Offset 100% |
| 交替灯具 | 在 Fixture Sequence 中添加多个灯具类型，并设置 Repeat |

## 11. 注意事项

- Fixture Sequence 至少需要一个有效灯具类，否则不会生成预览。
- 样条线长度为 0 或 Start Offset 不小于 End Offset 时不会生成灯具。
- 修改样条线形状时，预览阶段会跟随更新；正式创建后的灯具不会继续跟随样条线。
- 创建大量灯具时，编辑器响应速度取决于灯具数量、灯具复杂度和关卡规模。
