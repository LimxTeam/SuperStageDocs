# SuperShaperComponent（材质切割 · Legacy）

> 适用版本：SuperStage 26H2.6 起
>
> **本组件在 26H2.6 之前叫 `SuperCuttingComponent`。** 全线改名为 Shaper，并提供了重定向，既有资产按名字存着的组件类、材质节点、枚举值与蓝图函数都不受影响。

SuperShaperComponent 在材质光束基础上加四刀片成像切割。它属于**旧材质管线**，保留供需要退回时使用；新内容请用 [SuperVolumetricShaperComponent](12_SuperVolumetricShaperComponent_zh.md)。

---

## 1. 参数含义

四片刀，每片一条刀口。一片刀由**两端的插入量**决定它切进来多深、以及刀口是不是斜的；四片刀各自还有一个单片旋转角，刀架整体另有一个自转角。

在灯具定义里，这几件事对应四个能力：

| 能力 | 作用 |
| --- | --- |
| Blade Insert | 这片刀的一端插进来多少 |
| Blade Corner B | 这片刀的另一端插进来多少（两端不等就是斜切） |
| Blade Rotate | 这片刀自己的角度 |
| Shaper Rotate | 四片刀连同刀架整体自转 |

前三个都要在绑定的 Params 里填 **Blade Index**（1–4），指明改的是哪一片。

---

## 2. 刀序

```text
        ┌───────┐
      2 │       │ 4
        └───────┘
   1 = 下   2 = 左   3 = 上   4 = 右
```

| 刀号 | 位置 |
| ---: | --- |
| 1 | 下 |
| 2 | 左 |
| 3 | 上 |
| 4 | 右 |

一片刀的两端不是"左右"也不是"上下"，而是**它自己的逆时针端（A）与顺时针端（B）**。所以相对的两片刀，A/B 两端在画面上是镜像的：刀 3（上）的 A 在左，刀 1（下）的 A 在右。绑定时把 `Blade Insert` 接到 A 端那条通道、`Blade Corner B` 接到 B 端那条，斜切方向就是对的。

**刀架自转与图案轮自转是两回事**，分别由 `Shaper Rotate` 与 `Gobo Rotate` 能力驱动。

---

## 3. 使用注意

- 切割效果需要灯具定义里对应发射器的光束类型是切割类（本组件对应 `Shaper / Profile (Legacy)`）；
- 如果通道库里没有切割通道、或绑定没接上，控台值不会驱动这些参数——去灯具编辑器的 Validation 页确认；
- 预演中的切割形状用于视觉沟通，不等同于真实灯具光闸的机械精度。

---

## 4. 相关文档

- [灯光组件总览](00_LightComponent_Overview_zh.md)
- [SuperVolumetricShaperComponent](12_SuperVolumetricShaperComponent_zh.md)（推荐用这个）
- [SuperBeamComponent](03_SuperBeamComponent_zh.md)
- [灯具定义资产](../fixture/01_FixtureDefinition_zh.md)
