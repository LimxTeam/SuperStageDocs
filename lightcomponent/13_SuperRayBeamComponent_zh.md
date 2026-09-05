# SuperRayBeamComponent（纯光束）

> 适用版本：SuperStage 26H2.6 起

纯光束是**不带光学元件的体积光束**：没有图案、没有色轮、没有棱镜、没有刀片。

它为矩阵灯与效果灯而设。这类灯一支上面几十上百个发光单元，每个都是一个组件——纯光束恒定走零采样的闭式路径，开销远低于逐像素步进，画质也不受采样率影响。而且这类灯本来就不打图案。

---

## 1. 什么时候用它

| 情形 | 选什么 |
| --- | --- |
| 矩阵灯、像素条、效果灯的光柱 | **纯光束（Ray Beam）** |
| 需要打图案 | [体积光束](11_SuperVolumetricBeamComponent_zh.md) |
| 需要刀片切割 | [体积切割](12_SuperVolumetricShaperComponent_zh.md) |
| 不需要空中光柱的染色 / 观众灯 / 频闪 | [Wash](14_SuperWashComponent_zh.md) |

GDTF 导入时，矩阵 / 像素组会**自动**判为纯光束；包内声明 `BeamType=Glow/None` 的组（光环、背板一类发光面）判为 Wash，不会射出光柱。

---

## 2. 参数

复用体积光束那一组默认值（密度、雾气、边缘柔和度、淡入淡出等），含义相同，见 [SuperVolumetricBeamComponent](11_SuperVolumetricBeamComponent_zh.md) 第 1 节。

> **Beam Occlusion 对纯光束无效。** 这一项只有体积光束那条管线实现了；纯光束复用同一个参数结构体做默认值，但它没有遮挡实现。

---

## 3. 性能特性

- 恒定零采样的闭式积分，不随质量档增加采样；
- 不产生任何额外图元；
- 热路径带脏检查，值没变就不写。

这三条合起来，是矩阵灯能在一支灯上挂几十上百个发光单元的原因。

---

## 4. 相关文档

- [灯光组件总览](00_LightComponent_Overview_zh.md)
- [SuperVolumetricBeamComponent](11_SuperVolumetricBeamComponent_zh.md)
- [SuperWashComponent](14_SuperWashComponent_zh.md)
- [SuperMatrixComponent](07_SuperMatrixComponent_zh.md)
