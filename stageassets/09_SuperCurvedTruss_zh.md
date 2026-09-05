# Super Curved Truss 用户手册

## 功能定位

Super Curved Truss 使用样条线生成弯曲桁架。选择 Actor 后可以在视口编辑样条点，桁架会沿样条路径生成。

适合用于自由曲线灯架、波浪形吊挂结构、非标准路径桁架等预演场景。

## 主要参数

| 参数 | 说明 |
| --- | --- |
| `SectionType` | 截面类型，可选 Box、Triangle、Flat。 |
| `TrussSize` | 桁架规格，可选 S290、S400、S520。 |
| `BracePattern` | 支撑样式，可选 Warren、Cross。 |
| `SpanCountAlongSpline` | 沿样条划分的跨数。数值越大，路径分段越细。 |
| `SectionRotation` | 截面绕样条方向的旋转角度。 |
| `SuspendedLoad` | 悬挂载荷，用于统计面板的简化估算。 |
| `DistributedLoad` | 均布载荷，用于统计面板的简化估算。 |
| `bShowHorizontalBraces` | 显示水平支撑。 |
| `bShowEndPlates` | 显示两端端板。 |
| `ChordMaterial` | 主弦杆材质。 |
| `BraceMaterial` | 支撑杆材质。 |
| `EndPlateMaterial` | 端板材质。 |

## 样条编辑

- 选中 Actor 后，在视口移动样条控制点可改变桁架路径。
- 增加控制点可以让路径更贴合复杂曲线。
- `SectionRotation` 用于调整截面朝向，例如三角截面朝上或朝侧面。

## 统计信息

组件会根据当前样条和参数更新：

- `PartCounts`：主弦杆、支撑杆、端板等实例数量。
- `WeightStats`：自重、悬挂载荷、最大点载荷、最大均布载荷、挠度、立柱反力等简化估算。
- `CurrentProfile`：当前截面和规格对应的桁架型材数据。
- `BaySizeAlongSpline`：沿样条每跨长度。
- `SplineTotalLength`：样条总长度。

## 使用注意

- 样条至少需要两个有效点。
- 分段越多，形状越细，但实例数量也会增加。
- 载荷和挠度只用于快速预估，不可替代结构工程计算或现场验算。
