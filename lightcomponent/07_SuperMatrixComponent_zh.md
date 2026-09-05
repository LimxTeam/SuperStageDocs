# SuperMatrixComponent

SuperMatrixComponent 用于分段矩阵灯。它管理矩阵网格、矩阵材质、分段数量、分段方向、每段颜色、亮度、频闪，以及可选的**逐段真实光源**（固定是聚光，没有点光选项）。

## 参数

| 参数 | 作用 |
| --- | --- |
| SegCount | 分段数量，当前实现注释说明写入材质参数，最大按 200 处理。 |
| bUseUAxis | true 按 U 方向分段，false 按 V 方向分段。 |
| StaticMeshMatrix | 承载矩阵材质的网格。 |
| bTransparent | 是否使用透明矩阵材质。 |
| MatrixTransform | 矩阵网格相对变换。 |
| SegmentLightRadius | 分段光源衰减半径。 |
| LightSpotDefaults | 分段光源的整组默认参数，由灯具定义的灯光默认值映射进来。其中 **LightSpotIntensity** 是分段光源的强度倍率，**Angle** 是外锥角。分段光源固定为聚光。 |

> `LightSpotDefaults` 里的 **VolumetricScattering / LightShadow / AffectTransmission** 三项**只对分段矩阵的这条光源路径生效**：其它组件走的是自研锥形光，那条路不支持这三项。

## 使用注意

- 分段颜色通过材质参数写入；实际显示取决于矩阵材质。
- 启用分段真实光源会增加场景光源数量，最终性能取决于 UE 项目设置和硬件。
- 修改分段数量后，需要确保资产已重建对应分段光源。
