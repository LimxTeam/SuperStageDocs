# 灯光组件用户说明

> 适用版本：SuperStage 26H2.6 起

本目录说明 SuperStage 灯具内部常见的发光、光束、效果和矩阵组件。它面向使用灯具资产和检查参数的用户，不作为 C++ 或蓝图开发参考。

通常情况下，用户不需要手动添加这些组件。数据驱动灯具的组件由**灯具定义里的发射器**决定：发射器的光束类型选什么，运行时就创建哪个组件。详见 [灯具定义资产](../fixture/01_FixtureDefinition_zh.md) 第 4 节。

---

## 1. 继承关系

```text
UE 场景组件
├── SuperLightingComponent           基础：镜片、亮度、颜色、频闪
│   └── SuperConeLightComponent      自研锥形光（取代 UE SpotLight）
│       ├── SuperSpotComponent       聚光
│       │   ├── SuperVolumetricBeamComponent      体积光束
│       │   │   └── SuperVolumetricShaperComponent  体积切割
│       │   └── SuperBeamComponent   材质光束（Legacy）
│       │       └── SuperShaperComponent            材质切割（Legacy）
│       └── SuperRayBeamComponent    纯光束
│           └── SuperWashComponent   Wash
│
├── SuperEffectComponent             效果平面
├── SuperMatrixComponent             像素矩阵
└── SuperLiftComponent               Z 轴升降
```

> 下面三个不是发光组件的派生：效果平面、像素矩阵与升降各自直接挂在场景组件上，各管一件事，不参与镜片与光锥那条链。

---

## 2. 可见能力

| 组件 | 用途 | 用户能观察到的效果 | 文档 |
| --- | --- | --- | --- |
| SuperLightingComponent | 基础镜片与颜色/亮度控制 | 亮度、颜色、频闪、镜片材质变化 | [01](01_SuperLightingComponent_zh.md) |
| SuperConeLightComponent | 主光投射 | 地面光斑、图案、色轮、刀片切割、棱镜 | [10](10_SuperConeLightComponent_zh.md) |
| SuperSpotComponent | 聚光控制 | Zoom、Frost、Iris 对聚光角度和柔化的影响 | [02](02_SuperSpotComponent_zh.md) |
| SuperVolumetricBeamComponent | 体积光柱 | 空中光束、雾气、真实几何遮挡 | [11](11_SuperVolumetricBeamComponent_zh.md) |
| SuperVolumetricShaperComponent | 体积切割 | 体积光束 + 四刀片切割 | [12](12_SuperVolumetricShaperComponent_zh.md) |
| SuperRayBeamComponent | 纯光束 | 无光学元件的光柱，矩阵与效果灯用 | [13](13_SuperRayBeamComponent_zh.md) |
| SuperWashComponent | Wash 辉光 | 灯头前方一团有界的辉光，不画光柱 | [14](14_SuperWashComponent_zh.md) |
| SuperBeamComponent | 材质光束（Legacy） | 空中光束、Gobo、颜色轮、棱镜 | [03](03_SuperBeamComponent_zh.md) |
| SuperShaperComponent | 材质切割（Legacy） | 四边切割形状和旋转后的切割光斑 | [04](04_SuperShaperComponent_zh.md) |
| SuperEffectComponent | 灯带/效果面材质 | 效果编号、方向、速度、宽度、颜色和频闪 | [06](06_SuperEffectComponent_zh.md) |
| SuperMatrixComponent | 分段矩阵灯 | 分段颜色、矩阵亮度、频闪 | [07](07_SuperMatrixComponent_zh.md) |
| SuperLiftComponent | Z 轴升降 | 按归一化输入在升降范围内移动 | [09](09_SuperLiftComponent_zh.md) |

---

## 3. 本版变化

| 变化 | 说明 |
| --- | --- |
| **新增自研锥形光** | `SuperConeLightComponent` 取代 UE SpotLight。每支灯少一盏引擎光源与一份光照函数材质实例 |
| **体积光束成为主力** | 随包灯库已全量迁移。新从 GDTF 导入的灯默认仍是材质光束，需手动改 |
| **新增纯光束与 Wash** | 矩阵 / 效果灯用纯光束，染色 / 观众灯 / 频闪用 Wash |
| **Cutting 改名 Shaper** | `SuperCuttingComponent` → `SuperShaperComponent`。提供重定向，既有资产不受影响 |
| **移除 Rect 组件** | 矩形面光组件已删除。面光由锥形光统一承担 |
| **灯体退出物理与导航** | 灯体网格不再产生碰撞、重叠事件与导航数据；朝向箭头与地址码文本这类编辑期图元连同它们的场景代理一并移除。灯钩仍在，它是外壳模型的一部分 |

> 如果你在看 26H2.6 之前的文档：`SuperCuttingComponent` 与 `SuperRectComponent` 在本版已不存在。

---

## 4. 用户应关注的内容

- 灯具定义里对应的发射器是否配了这条能力（Zoom、Frost、Gobo、Prism、Matrix、Lift 等）；
- 通道库中的通道定义是否和控台输出一致；
- 材质、图集、棱镜预设等资源是否存在并已经绑定到定义；
- 大型场景中同时启用多支体积光束时，最终表现取决于 UE 渲染设置和硬件。

---

## 5. 不作为承诺的内容

这些组件不保证所有灯具都具备相同功能。具体功能取决于所使用的灯具定义、通道库、绑定、材质资源和项目设置。
