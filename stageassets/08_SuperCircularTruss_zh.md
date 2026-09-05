# Super Circular Truss 用户手册

## 功能定位

Super Circular Truss 用于生成完整的圆形或双环桁架。它不是任意角度弧形桁架；当前实现按 360 度整环生成，弧段数量由 `SegmentCount` 控制。

适合用于圆形灯光架、环形吊挂结构、舞台中央环架等预演场景。

## 主要参数

| 参数 | 说明 |
| --- | --- |
| `OuterRadius` | 外环半径，单位厘米。 |
| `InnerRadius` | 内环半径，单位厘米。设为 0 时只生成单环。 |
| `SegmentCount` | 圆环分段数量。数值越大，环形越细分，实例数量也越多。 |
| `SectionType` | 截面类型，可选 Box、Triangle、Flat。 |
| `SectionRotation` | 截面绕环向的自转角，−180 ~ 180 度，默认 0。**26H2.5 新增** |
| `TrussSize` | 桁架规格，可选 S290、S400、S520。 |
| `BracePattern` | 支撑样式，可选 Warren、Cross。 |
| `SuspendedLoad` | 悬挂载荷，用于统计面板的简化估算。 |
| `bShowInnerRing` | 显示内环。只有 `InnerRadius` 大于 0 时才有实际效果。 |
| `bShowRadialBraces` | 显示内外环之间的径向支撑。需要启用内环。 |
| `bShowConnectionFlanges` | 显示分段连接法兰。 |
| `ChordMaterial` | 主弦杆材质。 |
| `BraceMaterial` | 支撑杆材质。 |
| `FlangeMaterial` | 法兰材质。 |

## 统计信息

组件会根据当前参数更新数量和估算信息：

- `PartCounts`：主弦杆、支撑杆、法兰等实例数量。
- `WeightStats`：自重、悬挂载荷、最大点载荷、最大均布载荷、挠度、立柱反力等简化估算。
- `CurrentProfile`：当前截面和规格对应的桁架型材数据。
- `OuterCircumference`：外环周长。
- `SegmentArcLength`：每段外环弧长。

## 使用注意

- 需要半圆或任意弧线时，请使用 Super Curved Truss。
- `InnerRadius` 应小于 `OuterRadius`，否则内外环会重叠或生成异常。
- 载荷和挠度只用于快速预估，不可替代结构工程计算或现场验算。
