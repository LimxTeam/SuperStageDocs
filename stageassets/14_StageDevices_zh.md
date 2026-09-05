# 其它舞台设备 — 用户手册

> 适用版本：SuperStage 26H2.6 起 ｜ 前置阅读：[舞台资产总览](00_StageAssets_Overview_zh.md)

本篇覆盖资产浏览器 **SuperStage** 分类下的三个对象：导播相机、Madrix 矩阵、白模渲染。

其余同分类对象另有专篇：[屏幕](06_SuperScreen_zh.md)、[投影仪](05_SuperProjector_zh.md)、[灯带](../stagecore/13_Light_Strip_Effect_zh.md)、[升降 / 轨道机械](../stagecore/10_Stage_Machinery_zh.md)、[激光](../laser/00_Laser_Overview_zh.md)。

---

## 1. Super Director Camera（导播相机）

场景内的一台相机，实时采集画面供屏幕与投影引用。它是[屏幕](06_SuperScreen_zh.md)与[投影仪](05_SuperProjector_zh.md)三类媒体源之一（另两类是项目静态纹理与 NDI 输入）。

### 参数

| 参数 | 默认值 | 范围 | 说明 |
| --- | ---: | --- | --- |
| Enable Capture | 开 | — | 是否采集。关掉可省开销 |
| Capture Resolution | 1080 × 1080 | 宽高各 ≥ 1 | 采集分辨率（宽 × 高） |
| Field Of View | 90° | 5–170 | 视场角 |

### 用法

1. 把导播相机放到要取景的位置，调好朝向与 FOV；
2. 在屏幕或投影对象的媒体源里选择这台导播相机；
3. 画面即实时送到屏幕上。

### 注意

- **分辨率直接决定开销**。一块远景屏用 4K 采集是浪费，按屏幕在画面里的实际大小选；
- 多块屏幕可以引用同一台导播相机；
- 不需要时把 `Enable Capture` 关掉——采集是每帧都在跑的。

---

## 2. Super Madrix（LED 矩阵）

把 **MADRIX 主输出的实时预览画面**贴到场景里的网格上。

**它不走 DMX。** 数据来自本机 MADRIX 的 Remote HTTP 服务：SuperStage 主动去 `http://127.0.0.1/RemoteCommands/GetPreviewOutput.bmp` **拉**预览图，解码后作为纹理推给动态材质。所以它拿到的是 MADRIX 已经渲染好的画面，不是 DMX 通道值。

### 参数

| 参数 | 默认值 | 范围 | 说明 |
| --- | ---: | --- | --- |
| **ScreenMeshActors** | 空 | — | 承载画面的 `StaticMeshActor` 列表。一个 MADRIX 源可以同时驱动多块屏 |
| **MaterialIndex** | 0 | 0–255 | 应用动态材质的材质槽索引。**越界的目标会被跳过** |
| **Brightness** | 1.0 | ≥ 0 | 亮度倍率 |
| **MADRIX Port** | 80 | 1–65535 | 本机 MADRIX **HTTP 服务**的端口。**改完要重新运行才生效** |

### 用法

1. 在场景里准备好作为 LED 屏的 `StaticMeshActor`；
2. 放置 Super Madrix，把这些网格加进 `ScreenMeshActors`；
3. 设好 `MaterialIndex`，确认目标网格有这个材质槽；
4. **在 MADRIX 里开启 Remote HTTP 服务**，端口与这里的 `MADRIX Port` 一致——是 SuperStage 去连它，不是它连过来。

> 目标网格用的材质需要提供一个 Texture2D 参数与一个 Brightness 标量参数；随包的 LED 屏材质已经带好。

### 注意

- 它与[屏幕](06_SuperScreen_zh.md)的区别：屏幕放的是你在 Unreal 里指定的一路视频；这个对象镜像的是 **MADRIX 此刻正在输出的画面**，内容在 MADRIX 那边编排；
- MADRIX 必须跑在本机、且开着 Remote HTTP 服务——没有连远程主机的选项；
- 支持把矩阵数据录制为 Sequencer 轨道并通过 Take Recorder 回放；
- **它不是真实 LED 屏的控制器，也不是 Madrix 软件的替代品**，只做画面预演。

---

## 3. Super White Model（白模渲染）

后处理对象。拖进场景即把整个世界渲染为白模，**删除后自动还原**。

### 参数

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| Enable White Model | 开 | 总开关。关掉即恢复正常渲染，不必删除对象 |
| Clay Color | 浅灰 | 白模本体的颜色 |
| Background Color | 深灰 | 背景颜色 |

### 生效范围

| 场合 | 是否生效 |
| --- | --- |
| 编辑器视口 | 是 |
| PIE（在编辑器中运行） | 是 |
| Movie Render Queue | 是 |

所以它可以直接用于出图与出片，不只是编辑器里看看。

### 用途

- 结构与体块评审——去掉材质与颜色干扰，只看形体与空间关系；
- 灯光效果对比——白模底下更容易看清光的分布；
- 方案汇报的分镜稿。

### 注意

- 它是**后处理**，作用于整个世界，不能只白模一部分对象；
- 一个场景里放一个就够了。

---

## 4. 相关文档

- [Super Screen 媒体屏幕](06_SuperScreen_zh.md)
- [Super Projector 投影仪](05_SuperProjector_zh.md)
- [NDI 输入配置](../editortools/04_NDIConfigPanel_zh.md)
- [舞台资产总览](00_StageAssets_Overview_zh.md)
