# SuperConsole 灯光控台总览

> 适用版本：SuperStage 26H2.6 起

SuperConsole 是随主插件交付的编辑器内置灯光控台。它用于在不接外部控台的情况下，对场景里的 SuperStage 灯具选灯、调参、存编组与预设、存 CUE，并用执行器和时间线回放。

**打开方式**：SuperStage 工具栏 → **SuperConsole**（一级菜单，排在 `SuperDMXTool` 下方）。

> 26H2.6 起 SuperConsole 是主插件的编辑器模块，不再是独立插件。如果此前单独装过 SuperConsole 插件，升级前请把它从项目的 `Plugins` 目录移除，避免与主插件里的同名模块冲突。

---

## 1. 它在整条链上的位置

```text
              ┌──────────────────┐
外部控台 ────>│                  │
（Art-Net /   │  SuperDMX 缓冲   │────> 场景灯具
  sACN 输入） │                  │────> DMX 活动监看
              └──────────────────┘
                       ▲
                       │  写入
              ┌──────────────────┐
              │   SuperConsole   │
              └──────────────────┘
                       │
                       └─> 启用 DMX 输出后，按当前协议发到网络
```

控台的输出写进 SuperDMX 内部缓冲，所以：

- 场景灯具与 DMX 活动监看会同步看到控台的输出；
- 启用了网络输出时，也会按当前协议发送到外部设备；
- 控台与外部控台输入写的是同一个缓冲。两边同时推同一条通道时，以最后写入者为准——联调时建议只留一路。

这里有**两个不同的开关**，别弄混：

| 开关 | 位置 | 关掉之后 |
| --- | --- | --- |
| 控台输出 | DMX Settings 面板 | 控台整条聚合链停下，**场景里的灯也一起停止更新** |
| 网络输出 | SuperDMX 配置面板 | 只停止往网线上发，场景灯具照常跟随控台 |

想「只在本机预演、不打扰现场设备」，关的是**网络输出**那一个。

---

## 2. 界面

控台窗口是可自定义的网格布局。

- **加面板**：用鼠标**左键点一块空格子**，会弹出 Add Window 对话框选类型；
- **改大小 / 换位置**：拖窗口的标题栏与边角；
- **改设置或删除**：点窗口标题栏右侧的 **SS ▼** 按钮打开窗口设置，里面有各面板自己的选项，以及右上角红色的 **Delete Window**。

Add Window 能加的面板一共十一种：

| 面板 | 用途 |
| --- | --- |
| Fixture Sheet | 灯具表，按属性看当前值 |
| Color Picker | 取色器 |
| Preset | 预设槽位 |
| Group | 编组槽位 |
| Playback | 执行器网格，也是 CUE 表 |
| Running | 当前正在跑的回放，按 Cues / Effects / Presets / All 分页 |
| Frame Editor | 效果编辑器 |
| Frame Presets | 效果模板槽位 |
| Layout View | 灯位图 |
| Timecode | 时间码与时间线 |
| DMX Settings | DMX 输出查看与输出开关 |

**配接、演出文件与 DMX 设置不是网格面板**，它们在设置对话框里：点底部编码器栏右侧的 **⚙** 按钮打开，分 Show / Patch / DMX 三页。

窗口底部固定三层，自上而下是 **DESK 命令行**、**编码器栏**、**命令键盘**。

---

## 3. 一次最短的上手流程

1. 场景里放好灯并完成配接（见 [Patch 工具](../stagecore/07_Patch_Tools_zh.md)）；
2. 打开 SuperConsole，在 ⚙ → Patch 页确认控台看到了这些灯；
3. 选中若干灯，在编程器里推调光、改颜色；
4. 存成编组（`Store Group 1 Please`）方便下次选；
5. 存成 CUE（`Store Cue 1 Please`）；
6. 在 Playback 面板点那个槽位的按钮回放。

---

## 4. 本目录文档

| 文档 | 内容 |
| --- | --- |
| [01 选灯与编程](01_Programming_zh.md) | 选择、编程器、编组、预设、槽位外观 |
| [02 CUE 与回放](02_Cues_and_Playback_zh.md) | CUE、执行器、时间线、时间码、输出仲裁、高亮 |
| [03 命令行](03_CommandLine_zh.md) | 「模式 × 目标」矩阵，哪些键有后端 |
| [04 效果](04_Effects_zh.md) | Frame 效果引擎 |
| [05 演出文件与撤销](05_ShowFile_and_Undo_zh.md) | 存盘格式、版本、撤销行为 |

---

## 5. 使用边界

SuperConsole 是**预演与编程工具，不是经认证的现场控台**。当前实现的边界：

- 不提供控台硬件面板、现场冗余 / 备份控台切换，也不承诺演出级不间断运行；
- 演出文件是本产品自有格式，与第三方控台工程文件不互通；
- 命令行只做已经接通的那部分，没接通的键在界面上保持置灰（详见 [03 命令行](03_CommandLine_zh.md)）；
- 执行器没有实体推杆，也没有屏幕推杆，回放靠按键；
- Blind / Solo 已在本版移除；
- 它不替代真实控台的现场调试流程。

---

## 6. 相关文档

- [DMX 系统总览](../stagecore/00_DMX_System_Overview_zh.md)
- [DMX 网络配置](../stagecore/01_DMX_Network_Configuration_zh.md)
- [Patch 工具](../stagecore/07_Patch_Tools_zh.md)
- [灯具系统总览](../fixture/00_FixtureSystem_Overview_zh.md)
