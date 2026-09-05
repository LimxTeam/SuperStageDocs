# 体积光束调优参考（SuperVolumetricBeam）

> 适用版本：SuperStage 26H2.6 起 ｜ 面向：需要在具体项目上调质量与开销的集成人员
>
> 组件本身怎么用见 [体积光束组件](lightcomponent/11_SuperVolumetricBeamComponent_zh.md)；这里只列运行时可调的控制台变量、命令与已知边界。

体积光束是舞台空中光柱的默认渲染方式：每像素解析求解散射积分，不做体积步进，也不依赖引擎的 Volumetric Fog。光柱与地面光斑共用同一张灯具视角阴影图，所以光柱会被场景里的真实几何切断。

---

## 1. 在场景里怎么验

1. 放一支带体积光束的灯，或直接放 **Super Volumetric Beam Actor**——它默认下倾 30°，细节面板 `B.BeamParameter` 下的参数实时生效；
2. **验遮挡**：在光柱路径上放一个**带碰撞**的静态网格，光柱应当被切断；移动它，切口实时跟随。遮挡体是靠物理查询发现的，所以**没有碰撞的网格不会遮挡**——给它开 QueryOnly 碰撞即可；
3. **不想让某个物体挡光**：给它挂 Super Beam Occluder 组件并设为 `Exclude`；
4. **骨骼网格**用胶囊代理投影，属预演级近似，不逐顶点。

---

## 2. 控制台命令

| 命令 | 作用 |
|---|---|
| `SuperBeam.Stats [0/1]` | 屏幕统计：总数 / GPU 可见数 / 阴影槽数 / 遮挡绘制数 / 是否半分辨率 |
| `stat SuperBeam` | CPU 计时 |
| `stat GPU` / `ProfileGPU` | GPU 计时行 `SuperBeam Render`、`SuperBeam Shadows` |

调之前先用 `stat unit` 判断当前顶在渲染线程还是 GPU——两边的对策不一样。

---

## 3. 质量与性能

| CVar | 默认 | 说明 |
|---|---|---|
| `r.SuperBeam` | 1 | 总开关 |
| `r.SuperBeam.Quality` | 2 | 0=Low（无阴影采样 + 半分辨率） 1=Medium（4 采样 + 半分辨率） 2=High（8 采样 + 全分辨率） 3=Ultra（16 采样） |
| `r.SuperBeam.HalfRes` | -1 | -1 按质量自动 / 0 强制全分辨率 / 1 强制半分辨率 |
| `r.SuperBeam.OcclusionTaps` | 0 | 覆盖遮挡采样数（0 = 按质量档） |
| `r.SuperBeam.MinScreenRadiusPx` | 0.8 | 屏幕尺寸剔除阈值 |
| `r.SuperBeam.DistanceCullScale` | 1.0 | 距离剔除缩放 |

**填充率是主要开销**：大锥角、近距离的光柱最贵。先试 `HalfRes 1`，再收 `Quality`。

---

## 4. 阴影

| CVar | 默认 | 说明 |
|---|---|---|
| `r.SuperBeam.ShadowSlotSize` | 256 | 单槽**最佳**分辨率（32..512）。这是质量上限不是固定值——装不下时槽尺寸逐档降低，**不丢灯** |
| `r.SuperBeam.ShadowAtlasMaxSize` | 8192 | 图集尺寸上限（512..8192），按需增长 |
| `r.SuperBeam.ShadowSoftness` | 2.0 | 半影柔和度（图集像素，0..8）。0 = 硬边 |
| `r.SuperBeam.MaxOccludersPerSlot` | 12 | 每槽遮挡体上限 |
| `r.SuperBeam.ShadowBias` | 1 | 距离比较偏差，**绝对厘米值**，与光束长度无关。斜面切口出现颗粒噪点时调大 |

阴影只对**屏幕里看得见、亮着、开了 Beam Occlusion** 的光束分配槽位，不按相机距离排队。`r.SuperBeam.Quality 0` 会整段跳过阴影。

---

## 5. 观感标定

| CVar | 默认 | 说明 |
|---|---|---|
| `r.SuperBeam.BrightnessScale` | 1 | 亮度标定 |
| `r.SuperBeam.PhaseG` / `PhaseBlend` | 0.35 / 0.55 | 相位各向异性 / 相位混合 |
| `r.SuperBeam.PhaseBackLobe` | 0.35 | 后向瓣权重：抬升背对灯口那一侧的光束亮度。0 = 纯前向瓣 |
| `r.SuperBeam.ZoomConserve` | 1 | 变焦能量守恒（0..1）。1 = 物理正确：窄束更亮、宽 wash 更暗，14° 参考角处观感不变 |
| `r.SuperBeam.CameraExtinction` | 0.004 | 相机侧雾体消光（1/米）：远处光柱变淡，产生空间纵深 |
| `r.SuperBeam.ContactFade` | 35 | 接触渐隐距离（cm，0 = 关）。光柱被场景深度切断时末段在这个距离内渐隐 |
| `r.SuperBeam.MaxRadiance` | 10 | 保色相的亮度上限（0 = 不限） |
| `r.SuperBeam.TapJitter` | 1.0 | 采样抖动强度（0..1）。0 = 确定性，无噪点但高频图案可能出条带；1 = 全幅抖动，条带化为噪点，依赖 TAA |

---

## 6. 图案与棱镜

| CVar | 默认 | 说明 |
|---|---|---|
| `r.SuperBeam.OpticsAnisotropy` | 16 | 图案 / Frost / 色轮采样的各向异性上限（0 = 三线性，否则钳到 4/8/16） |
| `r.SuperBeam.OpticsChordBlur` | 1.0 | 图案与棱镜的沿弦预滤宽度 |
| `r.SuperBeam.OpticsScreenAA` | 1 | 屏幕空间抗锯齿足迹开关 |
| `r.SuperBeam.BladeRotationOffset` | 0 | 把刀片切割的输入绕光束轴旋转，单位度 |

> **图案图集纹理必须带 mipmap**（Mip Gen Settings 不能是 NoMipmaps，尺寸取 2 的幂）。缺 mip 时细光束上的图案会出摩尔纹，系统会在日志里告警。

---

## 7. 诊断

| CVar | 默认 | 说明 |
|---|---|---|
| `r.SuperBeam.DebugMode` | 0 | 1 = LOD 着色 2 = 弦长热图 3 = 阴影槽着色 |
| `r.SuperBeam.DebugAtlas` | 0 | 左上角叠加阴影图集 |
| `r.SuperBeam.OpticsDebug` | 0 | 1 = 关图案调制 2 = 强制 mip0 3 = 强制高 mip |
| `r.SuperBeam.FlipCull` | 0 | 若光束只在相机进入锥内时可见，置 1 |

---

## 8. 已知边界

- 光束在**半透明物体之后**合成，所以半透明物体不遮挡光束；泛光与 TSR 正常作用于光束；
- 遮挡体的发现依赖物理场景查询（QueryOnly 碰撞即可）；骨骼网格以胶囊代理投影；
- 灯具视角的阴影投影半角上限 80°，超广角 wash 的极端外围可能欠遮挡；
- 正交视口（编辑器顶视图等）跳过屏幕尺寸剔除，全部按最高 LOD 渲染；
- 相机进入光柱内部是受支持的场景。

---

## 9. 相关文档

- [体积光束组件](lightcomponent/11_SuperVolumetricBeamComponent_zh.md)
- [体积切割光束](lightcomponent/12_SuperVolumetricShaperComponent_zh.md)
- [灯光组件总览](lightcomponent/00_LightComponent_Overview_zh.md)
