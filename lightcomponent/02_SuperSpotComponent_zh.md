# SuperSpotComponent

SuperSpotComponent 用于灯具的聚光。它建在[自研锥形光](10_SuperConeLightComponent_zh.md)之上——26H2.6 起主光由锥形光投射，不再使用 UE SpotLight。当前实现中它会把亮度、颜色、Zoom、Frost、Iris、旋转和可见性同步到锥形光与相关材质。

## 可见效果

- Zoom 会根据标定表影响聚光角度。
- Iris 会缩小当前光锥角度，不能把光锥放大到超过当前基础角度。
- Frost 会把外锥角加宽，让边缘看起来更散；只要灯具接了 Frost 通道就有效果。
- 颜色、亮度和频闪会影响实际聚光灯输出。

## 用户检查项

| 问题 | 检查 |
| --- | --- |
| 光锥角度不变 | 确认灯具通道里有 Zoom 或相关控制。 |
| 柔化不明显 | 确认灯具的通道库里接了 Frost，且绑定的能力是 Frost。 |
| 只有材质变化但照明不明显 | 检查场景曝光与资产亮度设置。 |

## 边界

本文不承诺所有电脑灯都具备 Zoom、Frost、Iris。实际能力以具体灯具定义与通道库为准，见[灯具定义资产](../fixture/01_FixtureDefinition_zh.md)。
