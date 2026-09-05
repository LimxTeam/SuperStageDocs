# SuperVolumetricShaperComponent（体积切割）

> 适用版本：SuperStage 26H2.6 起

体积切割 = [体积光束](11_SuperVolumetricBeamComponent_zh.md) + 四刀片成像切割。它继承体积光束，所以体积光束那一组参数全部适用。

它与旧的 [SuperShaperComponent](04_SuperShaperComponent_zh.md) 的关系，正如体积光束与材质光束的关系：同一道光束，多一个把梯形遮罩乘进去的着色器。**新内容请用这个，不要用 Legacy 那个。**

---

## 1. 刀片参数

四片刀，每片两个角点，外加刀架整体旋转。

| 能力 | 说明 |
| --- | --- |
| Blade Insert | 刀片插入深度 |
| Blade Corner B | 刀片另一端的角点 |
| Blade Rotate | 单片刀的角度 |
| Shaper Rotate | 刀架整体自转 |

刀序：**1 = 下、2 = 左、3 = 上、4 = 右**。

**刀架自转与图案轮自转是分开的**，分别由 `Shaper Rotate` 与 `Gobo Rotate` 驱动。

---

## 2. 与光柱的关系

切割着色器与光柱本体是**真正拆开**的：不带刀片的体积光束编出来一条刀片指令都没有。所以给一支普通光束灯选体积光束、给成像灯选体积切割，成本不会互相拖累。

---

## 3. 使用注意

- 需要灯具定义里对应发射器的光束类型是 `Volumetric Shaper (Profile)`；
- 通道库里要有刀片通道，且绑定接到了上面四个能力——去灯具编辑器的 Validation 页确认；
- GDTF 导入时，**带刀片通道的灯默认落在 Legacy 的 `Shaper / Profile`**，需要手动改成 `Volumetric Shaper (Profile)`；
- 预演中的切割形状用于视觉沟通，不等同于真实灯具光闸的机械精度。

---

## 4. 相关文档

- [灯光组件总览](00_LightComponent_Overview_zh.md)
- [SuperVolumetricBeamComponent](11_SuperVolumetricBeamComponent_zh.md)
- [SuperShaperComponent（Legacy）](04_SuperShaperComponent_zh.md)
- [灯具定义资产](../fixture/01_FixtureDefinition_zh.md)
