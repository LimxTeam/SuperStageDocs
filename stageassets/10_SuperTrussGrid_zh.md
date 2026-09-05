# Super Truss Grid 用户手册

## 功能定位

Super Truss Grid 用于生成水平网格桁架。组件会根据宽度、深度和间距计算 X/Y 方向的格数，并生成主弦杆、次弦杆、斜撑和节点板。

适合用于大型灯光吊挂网格、展厅顶部吊挂结构和舞台上方矩阵结构的预演。

## 主要参数

| 参数 | 说明 |
| --- | --- |
| `GridWidthX` | 网格 X 方向总宽度，单位厘米。 |
| `GridDepthY` | 网格 Y 方向总深度，单位厘米。 |
| `GridSpacing` | 目标网格间距，单位厘米。实际间距会按格数重新计算。 |
| `GridHeightZ` | 上下层之间的高度，单位厘米。 |
| `TrussSize` | 桁架规格，可选 S290、S400、S520。 |
| `BracePattern` | 支撑样式，可选 Warren、Cross。 |
| `DistributedLoad` | 均布载荷，用于统计面板的简化估算。 |
| `PointLoadCount` | 点载荷数量。 |
| `WeightPerPoint` | 每个点载荷的重量。 |
| `bShowDiagonalBraces` | 显示斜撑。 |
| `bShowNodePlates` | 显示节点板。 |
| `bCentered` | 将网格以 Actor 原点居中。 |
| `ChordMaterial` | 弦杆材质。 |
| `BraceMaterial` | 支撑杆材质。 |
| `PlateMaterial` | 节点板材质。 |

## 统计信息

组件会根据当前参数更新：

- `GridCountX`、`GridCountY`：X/Y 方向实际格数。
- `GridStats.MainChords`：主弦杆数量。
- `GridStats.SecondaryChords`：次弦杆数量。
- `GridStats.DiagonalBraces`：斜撑数量。
- `GridStats.NodePlates`：节点板数量。
- `GridStats.TotalInstances`：总实例数量。
- `GridStats.SelfWeight`、`TotalLoad`、`MaxDeflection`：简化估算数据。

## 使用注意

- 当前组件使用总尺寸和间距计算格数，不直接让用户填写 X/Y 格数。
- 点载荷总量由 `PointLoadCount * WeightPerPoint` 参与统计。
- 载荷和挠度只用于快速预估，不可替代结构工程计算或现场验算。
