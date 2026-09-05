# SuperConeLightComponent（自研锥形光）

> 适用版本：SuperStage 26H2.6 起

SuperConeLightComponent 是 26H2.6 新增的自研锥形光管线，**取代 UE SpotLight**作为灯具主光。三条光束管线（体积光束、纯光束、材质光束）全部建在它之上。

---

## 1. 它负责什么

| 内容 | 说明 |
| --- | --- |
| 地面 / 表面光斑 | 灯打在场景表面上的那一块 |
| 图案（GOBO） | 图案轮投射 |
| 色轮 | 颜色轮投射，含流水 |
| 刀片切割 | 四刀片成像 |
| 棱镜 | 棱镜分光 |

**空中光柱不归它管。** 光柱画不画、怎么画，由发射器的光束类型决定（见 [灯具定义资产](../fixture/01_FixtureDefinition_zh.md) 4.2 节）。

---

## 2. 为什么换掉 UE SpotLight

- 每支灯少一盏引擎光源、少一次组件注册、少一份光照函数材质实例；
- 光柱与地面光斑**共用同一套光学与同一个时基**，不会出现两者颜色或角度对不上；
- 图案、色轮、切割、棱镜由同一条着色器路径投射，与光柱同源。

---

## 3. 用户会看到什么

- 光斑随亮度、颜色、Zoom、Frost、Iris 变化；
- 图案与色轮的流水、旋转、抖动；
- 棱镜的分光效果与光柱一致。

> **出光口辉光**（灯口那一圈亮）不由锥形光负责，它属于[纯光束](13_SuperRayBeamComponent_zh.md)那条管线，且只在摘掉镜片网格时才开。

---

## 4. 遮挡

锥形光与体积光束**共用同一张阴影图集与同一个采样函数**。这意味着：

- 挡住光柱的东西，同样会挡住地面光斑；
- 此前只有光柱吃遮挡、地面光斑不吃，表现为"光束被挡了、光斑却没被挡"。

---

## 5. 使用注意

- 普通用户通过灯具定义、通道库与 DMX 通道间接控制它，一般不需要直接调；
- 光斑是否可见还取决于 UE 的曝光、渲染设置与场景表面材质；
- 照明锥角以光柱为准，与变焦通道联动。

---

## 6. 调优与诊断开关

以下控制台变量在运行时生效，改完立刻看得到，不需要重启。日常不用动，出问题或者要压性能时才用。

### 6.1 调优

| 变量 | 默认 | 作用 |
| --- | ---: | --- |
| `r.SuperConeLight` | 1 | 锥形光总开关。0 关、1 开 |
| `r.SuperConeLight.BrightnessScale` | 1.0 | 全局亮度校准倍数 |
| `r.SuperConeLight.SpecularScale` | 1.0 | 高光强度，0 为纯漫反射。预演里高光只是给金属件一点亮点 |
| `r.SuperConeLight.MinScreenRadiusPx` | 2.0 | 屏幕投影半径小于这个像素数的灯直接剔除。开销与锥体覆盖的像素数成正比，给一个只占两三个像素的灯画完整锥体不划算 |
| `r.SuperConeLight.ZoomRefAngle` | 0.0 | 变焦亮度的参考锥角（全角，度）。**0 表示锥角不影响亮度**（默认）；填正值则启用光通量守恒——收窄光斑会变亮 |

### 6.2 诊断

| 变量 | 默认 | 作用 |
| --- | ---: | --- |
| `r.SuperConeLight.DebugMode` | 0 | 1 = 过绘制热力图。每盏灯加一个固定量，叠加起来就是过绘制深度 |
| `r.SuperConeLight.GateOrientation` | 0 | 门空间 UV 的对称变换：0 原样、1 翻 U、2 翻 V、3 交换 UV。地面光斑上的图案或色轮方向不对时用它定位 |
| `r.SuperConeLight.FlipCull` | 0 | 翻转锥体代理的面剔除。默认剔正面、光栅化背面，所以相机进到锥体里灯仍然可见；换平台后出现"只在锥内可见"再用它诊断 |

### 6.3 校准偏移

这两个值是实测标定过的，**没有确凿理由不要改**：

| 变量 | 默认 | 说明 |
| --- | ---: | --- |
| `r.SuperConeLight.PrismRotationOffset` | 90.0 | 棱镜分束排布与自转的角度偏移。只有棱镜需要这个偏移，图案轮方向本来就是对的 |
| `r.SuperConeLight.BladeRotationOffset` | 0.0 | 刀片切割的门空间 UV 角度偏移，与控台标定一致 |

---

## 7. 相关文档

- [灯光组件总览](00_LightComponent_Overview_zh.md)
- [SuperSpotComponent](02_SuperSpotComponent_zh.md)
- [SuperVolumetricBeamComponent](11_SuperVolumetricBeamComponent_zh.md)
- [灯具定义资产](../fixture/01_FixtureDefinition_zh.md)
