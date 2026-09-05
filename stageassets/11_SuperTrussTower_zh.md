# Super Truss Tower 用户手册

## 功能定位

Super Truss Tower 用于生成竖向桁架塔。组件会根据塔高和标准节高计算节数，并生成竖向主弦杆、斜撑、水平支撑、底板和顶部法兰。

适合用于灯光塔、音响吊挂塔、网架支撑塔等预演场景。

## 主要参数

| 参数 | 说明 |
| --- | --- |
| `TowerHeight` | 塔总高度，单位厘米。 |
| `SectionType` | 截面类型，可选 Box、Triangle、Flat。 |
| `TrussSize` | 桁架规格，可选 S290、S400、S520。 |
| `BracePattern` | 支撑样式，可选 Warren、Cross。 |
| `TopSuspendedLoad` | 顶部悬挂载荷，用于统计面板的简化估算。 |
| `WindLoad` | 风载输入，用于统计面板的简化估算。 |
| `bShowBasePlate` | 显示底板。 |
| `bShowTopFlange` | 显示顶部法兰。 |
| `bShowDiagonalBraces` | 显示斜撑。 |
| `bShowHorizontalBraces` | 显示水平支撑。 |
| `ChordMaterial` | 主弦杆材质。 |
| `BraceMaterial` | 支撑杆材质。 |
| `PlateMaterial` | 底板和顶部法兰材质。 |

## 统计信息

组件会根据当前参数更新：

- `PartCounts`：主弦杆、支撑杆、底板、顶部法兰等实例数量。
- `WeightStats`：自重、悬挂载荷、最大点载荷、最大均布载荷、挠度、立柱反力等简化估算。
- `CurrentProfile`：当前截面和规格对应的桁架型材数据。
- `NumberOfBays`：按塔高计算出的节数。
- `ActualBayHeight`：实际每节高度。

## 使用注意

- 塔高会按标准节高取整成节数，实际每节高度可能和标准节高略有差异。
- 与 Super Truss Grid 搭配时，需要手动对齐塔顶高度和网格高度。
- 载荷和挠度只用于快速预估，不可替代结构工程计算或现场验算。
