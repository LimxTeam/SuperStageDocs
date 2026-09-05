# SuperEffectComponent

SuperEffectComponent 用于灯带、效果面或其他材质驱动的发光效果。当前实现中它管理静态网格、普通/透明材质、亮度、频闪、颜色、方向、效果编号、速度和宽度。

## 参数

| 参数 | 作用 |
| --- | --- |
| MaxLightIntensity | 组件最大亮度百分比。 |
| StaticMeshEffect | 承载效果材质的网格。 |
| EffectMaterialNo | 使用的材质槽编号。 |
| bTransparent | 是否使用透明效果材质。 |
| EffectTransform | 效果网格相对变换。 |
| Intensity / Strobe / Color | 控制亮度、频闪和颜色。 |
| Direction / Effect / Speed / Width | 写入材质的方向、效果编号、速度和宽度参数。 |

## 使用注意

- 可见效果取决于绑定的材质是否读取这些参数。
- 频闪在当前实现中按模式计算亮度倍率；速度为 0 时不会产生闪烁变化。
- 如果效果面不可见，先检查网格、材质槽编号、透明材质和亮度。
