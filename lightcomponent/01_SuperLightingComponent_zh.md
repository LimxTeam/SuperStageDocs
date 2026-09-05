# SuperLightingComponent

SuperLightingComponent 是灯具资产中的基础发光组件。它负责所有出光组件共用的那部分：镜片网格与镜片材质、亮度、颜色、频闪、可见性。

**地面光斑不在这一层**——那是[锥形光](10_SuperConeLightComponent_zh.md)的事。

## 用户会看到什么

- 镜片模型和镜片材质显示。镜片网格由 **StaticMeshLens** 指定，其位置 / 旋转 / 缩放由 **LensTransform** 决定；两者都是逐灯具型号设置，不在场景实例上改。
- 镜片材质随亮度、颜色等参数变化。
- 频闪模式会影响最终亮度；频闪速度为 0 时保持常亮。
- 当颜色接近纯黑或亮度为 0 时，组件会关闭可见光效。

## 常见参数含义

| 参数 | 作用 |
| --- | --- |
| Dimmer / Intensity | 控制组件亮度。 |
| Color | 控制镜片和光斑颜色。 |
| Strobe | 控制频闪速度。 |
| StrobeMode | 控制频闪模式，当前实现中包含 Closed、Open、Linear、Pulse、RampUp、RampDown、Sine、Random 等分支。 |
| Zoom / Frost / Iris | 在这一层只是接口，实际生效在聚光等子类上，见 [02](02_SuperSpotComponent_zh.md)。 |
| Texture | 用于光斑或图案贴图。 |
| Visibility | 控制组件光效可见性。 |

## 使用注意

- 普通用户通常通过灯具资产、Fixture Library 和 DMX 通道间接控制它。
- 不是所有灯具都会暴露全部参数；未配置通道或资源时，对应效果可能没有可见变化。
- 光束遮挡距离依赖场景碰撞和可见性通道设置。
