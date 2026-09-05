# Super Projector 用户说明

Super Projector 用于在 UE 场景中做投影映射预演。当前实现中它使用红、绿、蓝三个 SpotLight 叠加，并为每个光源设置 Light Function 材质；活动纹理来自静态纹理或导播相机 RenderTarget。

## 使用方式

1. 将 Super Projector 放入场景。
2. 在媒体源设置中选择静态纹理或导播相机来源。
3. 调整投影位置和朝向。
4. 在 Details 面板中调整投影参数。

## 参数

| 参数 | 作用 |
| --- | --- |
| Dimmer | 投影亮度，写入三个 SpotLight 的 Intensity，单位为 Candelas。 |
| MaxLightDistance | 投影距离，当前实现按米读取并乘以 100 写入 SpotLight 衰减半径。 |
| Zoom | 投影角度，限制范围 10-100 度。 |
| DilutionFactor | 边缘柔化系数，用于计算 InnerConeAngle。 |
| Deformation | 四角梯形校正参数，写入材质中的八个 UV 偏移值。 |

## 需要注意

- 投影依赖插件内置的 `M_Mapping` Light Function 材质。
- 画面是否清晰、亮度是否足够，取决于 UE 灯光、曝光、材质、投影距离和场景表面。
- 梯形校正用于预演画面对位，不保证和真实投影仪的光学几何完全一致。
- 如果媒体源没有纹理，投影材质不会显示有效画面。
