# 命令行

> 适用版本：SuperStage 26H2.6 起 ｜ 前置阅读：[控台总览](00_Console_Overview_zh.md)

命令行是「**模式 × 目标**」矩阵：先选做什么，再选对谁做。界面置灰、按键派发与实际能力来自**同一份真源**，所以不会出现按得下去但没有后端的键。

---

## 1. 模式与目标

**模式**（做什么）：Idle、Store、Update、Edit、Delete、Copy、Move、Assign、Select、On、Off、Stomp

**目标**（对谁做）：Fixture、Group、Preset、Sequence、Cue、执行器

> 执行器那个键在键盘上的**键面写的是 `DESK`**，不是 Executor。找不到 Executor 键是正常的。

---

## 2. 当前可用的组合

| 模式 | 可用目标 | 说明 |
| --- | --- | --- |
| **Idle** | Fixture / Group / Preset / Cue / 无对象 | 直接选择与调用；`At`、`On`、`Off` 这类不带对象的命令也走这里 |
| **Select** | Group / Fixture | 选择 |
| **Store** | Group / Preset / Cue | 存储 |
| **Update** | Group / Preset / Cue | 更新 |
| **Delete** | Group / Preset / Cue | 删除 |
| **Copy** | Group / Preset / Cue | 复制到另一个槽位 |
| **Move** | Group / Preset / Cue | 移动到另一个槽位 |
| **On / Off** | 作用于当前选择 | 不需要对象 |

表里没有的组合在界面上是**置灰**的：`Edit`、`Assign`、`Stomp` 三个模式，以及 `Sequence` 与执行器（`DESK`）两个目标。置灰而不是"按下去没反应"是有意的——台上按了没动静，人会以为是自己按错了。

> 执行器本身的回放功能可用，只是**指派要在 Playback 面板上右键操作**，不走命令行。

**Blind / Solo 已在 26H2.6 移除。**

---

## 3. 语法

### 复制与移动

```text
Copy Group 1 At 5
Move Cue 2 At 6
```

`At` 之前是源，之后是目标槽位。编组、预设、CUE 三类对象都支持。

### 选择

```text
Fixture 1 Thru 20
Group 3
```

### 存储

```text
Store Group 5
Store Preset 2
Store Cue 10
```

---

## 4. 键

| 键 | 说明 |
| --- | --- |
| 数字 / Thru | 输入对象编号与范围 |
| At | 分隔源与目标 |
| Please | 提交 |
| ESC | 取消当前正在录的这条命令 |
| Clear | **两层含义**：正在录入时清掉录入；录入已经是空的时候再按，才清编程器 |
| Undo / Redo | 撤销 / 重做，栈空时置灰 |

> Clear 分两层是为了防手滑：一次误触只会清掉刚敲的几个字符，不会把台上的值全清了。

模式键只要在任一目标上有实现就允许按下；按下之后，没有后端的目标会置灰。

---

## 5. 相关文档

- [控台总览](00_Console_Overview_zh.md)
- [选灯与编程](01_Programming_zh.md)
- [CUE 与回放](02_Cues_and_Playback_zh.md)
- [演出文件与撤销](05_ShowFile_and_Undo_zh.md)
