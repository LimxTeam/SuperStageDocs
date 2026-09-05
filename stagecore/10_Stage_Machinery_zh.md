# 10 - 舞台机械

> **所属模块**: SuperAssets  
> **适用对象**: 舞美设计师、机械控制技术人员  
> **前置阅读**: [03 - DMX 灯具基础](./03_DMX_Actor_Base_zh.md)  
> **最后更新**: 2026-04-14

---

## 一、概述

SuperStage 当前提供两类 DMX 控制的舞台机械 Actor：

| 类型 | 显示名 | 默认灯库标签 | 用途 |
|------|--------|--------------|------|
| 升降机械 | `SuperLiftingMachinery` | `12CH` | 通过 XYZ 位移和 XYZ 旋转控制机械平台、吊杆或旋转结构 |
| 轨道机械 | `SuperRailMachinery` | `14CH` | 沿样条轨道移动，并在挂载点上叠加局部偏移和旋转 |

两者都使用 SuperDMX 灯具设置，因此需要设置 Universe、Start Address、Fixture Library 和 ControlMode。机械运动是否生效取决于灯库属性名是否匹配当前实现读取的属性。

---

## 二、升降机械

升降机械直接移动和旋转 Actor 本体。运行时 `BeginPlay` 会自动调用一次 **BootRefresh**，也可以在编辑器中手动点击。

### 主要参数

| 参数 | 默认值 | 说明 |
|------|--------|------|
| **Start** | False | 为 False 时不响应 DMX 运动 |
| **MovingRange** | (0, 0, 0) | 位置终点相对起点的偏移，单位厘米 |
| **InitialPosition** | 自动计算 | BootRefresh 时记录的起点 |
| **EndPosition** | 自动计算 | `InitialPosition + MovingRange` |
| **RotRange** | (360, 360, 360) | 绝对旋转模式下的总旋转范围 |
| **InitialRotation** | 自动计算 | 当前旋转 - `RotRange * 0.5` |
| **EndRotation** | 自动计算 | 当前旋转 + `RotRange * 0.5` |
| **PositionInterpolation** | False | 是否对位置使用插值 |
| **RotationInterpolation** | False | 绝对旋转模式下是否对旋转使用插值 |
| **PosSpeed** | 1.0 | 仅 PositionInterpolation 开启时显示，范围 0-10 |
| **RotSpeed** | 1.0 | 仅 RotationInterpolation 开启且 PolarRotation 关闭时显示，范围 0-10 |
| **PolarRotation** | False | 开启后使用连续旋转模式 |
| **PolarRotationSpeed** | 1.0 | PolarRotation 开启时显示，范围 0-10 |

### DMX 属性

升降机械在 DMX 控制模式下读取以下灯库属性，全部按 16 位 Fine 方式读取：

| 属性名 | 控制内容 |
|--------|----------|
| `XPos` | X 轴位置 |
| `YPos` | Y 轴位置 |
| `ZPos` | Z 轴位置 |
| `XRot` | Pitch |
| `YRot` | Yaw |
| `ZRot` | Roll |

位置映射为 `Lerp(InitialPosition, EndPosition, DMX值)`。绝对旋转映射为 `Lerp(InitialRotation, EndRotation, DMX值)`。

连续旋转模式下，`XRot`/`YRot`/`ZRot` 会被映射为 `-PolarRotationSpeed` 到 `+PolarRotationSpeed` 的旋转速度，并按 DeltaTime 累加本地旋转。

### 属性模式

**升降机械请用 DMX 模式驱动。** 把 **ControlMode** 切到 Property 后，细节面板会露出 **C.ControlParameter** 分组的六个手动控制项（PosX / PosY / PosZ、RotX / RotY / RotZ，位置三项 0~100、旋转三项 −100~100），但**当前实现在属性模式下不移动设备**——读取与应用这一步在属性模式会直接跳过。

---

## 三、轨道机械

轨道机械包含三个组件：

| 组件 | 作用 |
|------|------|
| **RailSpline** | 样条路径，决定轨道形状 |
| **RailMountPoint** | 沿样条移动的挂载点 |
| **OffsetComponent** | 在挂载点下叠加局部偏移和旋转；子 Actor 默认应挂到这里 |

轨道机械在编辑器视口中也会 Tick，用于预览 RailPos 对轨道位置的影响。

### 主要参数

| 参数 | 默认值 | 说明 |
|------|--------|------|
| **Start** | False | 为 False 时不响应 DMX 运动 |
| **LockOrientationToRail** | False | 开启后 RailMountPoint 的朝向跟随样条切线 |
| **ClosedLoop** | False | 开启后样条首尾闭合 |
| **OffsetRange** | (0, 0, 0) | OffsetComponent 的局部偏移范围，单位厘米 |
| **InitialOffset** | 自动计算 | BootRefresh 时基于当前 OffsetComponent 计算 |
| **EndOffset** | 自动计算 | BootRefresh 时基于 OffsetRange 计算 |
| **RotRange** | (360, 360, 360) | 绝对旋转模式下的总旋转范围 |
| **PositionInterpolation** | False | 是否对轨道位置和偏移使用插值 |
| **RotationInterpolation** | False | 绝对旋转模式下是否对旋转使用插值 |
| **RailSpeed** | 1.0 | 仅 PositionInterpolation 开启时显示 |
| **OffsetSpeed** | 1.0 | 仅 PositionInterpolation 开启时显示 |
| **RotSpeed** | 1.0 | 仅 RotationInterpolation 开启且 PolarRotation 关闭时显示 |
| **PolarRotation** | False | 开启后 OffsetComponent 使用连续旋转 |
| **PolarRotationSpeed** | 1.0 | PolarRotation 开启时显示 |

闭合轨道并启用位置插值时，当前实现会按最短路径在 0-1 之间环绕。例如从 0.9 到 0.1 会走跨越 1.0/0.0 的短路径。

### DMX 属性

轨道机械在 DMX 控制模式下读取以下灯库属性，全部按 16 位 Fine 方式读取：

| 属性名 | 控制内容 |
|--------|----------|
| `Dimmer` | 轨道位置，文档中可理解为 RailPos |
| `XPos` | OffsetComponent 局部 X 偏移 |
| `YPos` | OffsetComponent 局部 Y 偏移 |
| `ZPos` | OffsetComponent 局部 Z 偏移 |
| `XRot` | OffsetComponent Pitch |
| `YRot` | OffsetComponent Yaw |
| `ZRot` | OffsetComponent Roll |

> **注意**：当前实现使用 `Dimmer` 作为轨道位置属性名，不是 `RailPos`。如果自定义灯库，请按当前实现属性名配置。

---

## 四、使用流程

### 升降机械

1. 将 `SuperLiftingMachinery` 放到场景中。
2. 设置 MovingRange 和 RotRange。
3. 点击 **BootRefresh**，或让运行时 BeginPlay 自动初始化。
4. 配置 DMX 地址和灯库。
5. 将 **Start** 设为 True。
6. 从控台发送 `XPos/YPos/ZPos/XRot/YRot/ZRot` 对应通道。

### 轨道机械

1. 将 `SuperRailMachinery` 放到场景中。
2. 编辑 RailSpline 的控制点。
3. 根据需要设置 ClosedLoop 和 LockOrientationToRail。
4. 将需要跟随轨道的子 Actor 挂到 **OffsetComponent**。
5. 设置 OffsetRange、RotRange，并点击 **BootRefresh**。
6. 配置 DMX 地址和灯库。
7. 将 **Start** 设为 True。
8. 从控台发送 `Dimmer/XPos/YPos/ZPos/XRot/YRot/ZRot` 对应通道。

---

## 五、常见问题

### Q: 机械不动？
检查 **Start** 是否为 True，ControlMode 是否允许 DMX 读取，灯库中是否存在当前实现读取的属性名，以及 DMX 输入是否到达对应 Universe/Address。

### Q: 设置了 PosSpeed 或 RotSpeed 但没变化？
这些速度只在对应的 **PositionInterpolation** 或 **RotationInterpolation** 开启时生效。插值关闭时会直接跳到目标值。

### Q: 轨道位置通道应该叫 RailPos 吗？
当前实现读取 `Dimmer` 作为轨道位置。灯库里需要有 `Dimmer`，否则轨道位置不会按 DMX 更新。

### Q: 子 Actor 不跟随轨道？
确认子 Actor 挂载到 **OffsetComponent**。`SuperRailMachinery` 的默认挂载组件就是 OffsetComponent。

### Q: 编辑器里 RailPos 有预览，升降机械为什么没有同样预览？
轨道机械实现了编辑器视口 Tick 和构建/属性变化更新；升降机械当前没有同等编辑器预览路径，主要在运行时或 DMX Tick 中应用。

---

> **下一步**：请阅读 [12 - 升降矩阵](./12_Lift_Matrix_zh.md)。
