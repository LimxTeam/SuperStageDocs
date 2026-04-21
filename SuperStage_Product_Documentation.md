# SuperStage 产品文档

> **数字舞台的中央神经系统 —— 工业级现场控制与虚拟制片统一生产环境**

**版本**: 26Q2.5  
**更新日期**: 2026年4月14日  
**版权所有**: 佛山市壹贰冉冉科技有限公司 (LimxTeam)

---

## 目录

1. [产品概述](#1-产品概述)
   - 1.1 定义：数字舞台的中央神经系统
   - 1.2 系统架构：唯一真理源
   - 1.3 核心定位
   - 1.4 我们解决的问题
   - 1.5 适用边界
2. [核心价值](#2-核心价值)
   - 2.1 零摩擦工作流
   - 2.2 数字孪生级保真度
   - 2.3 跨维度媒体融合
   - 2.4 主权与解耦
   - 2.5 确定性交付
   - 2.6 典型应用场景
3. [设计哲学](#3-设计哲学)
   - 3.1-3.4 四大设计原则
   - 3.5 技术深潜：隐形护城河
4. [核心子系统](#4-核心子系统core-subsystems)
   - 4.1 灯光控制系统
   - 4.2 SuperConsole
   - 4.3 SuperLaser
   - 4.4 SuperNdi
   - 4.5 SuperDrone
   - 4.6 SuperShader
   - 4.7 SuperStage 核心框架
   - 4.8 SuperTools
   - 4.9 SuperAI
5. [SuperData - 跨平台数据同步](#5-superdata---跨平台数据同步)
   - 5.1 产品概述
   - 5.2 工作原理
   - 5.3 支持的平台
   - 5.4 协议规范
   - 5.5 典型工作流
   - 5.6 安装与配置
6. [LimxDroneStudio - 无人机编队软件](#6-limxdronestudio---无人机编队软件)
   - 6.1 产品概述
   - 6.2 资产管理
   - 6.3 时间轴系统
   - 6.4 解算引擎
   - 6.5 特效系统
   - 6.6 无人机管理
   - 6.7 网络输出
   - 6.8 性能指标
7. [技术规格](#7-技术规格)
   - 7.1 系统要求
   - 7.2 核心性能指标
   - 7.3 灯库兼容性
   - 7.4 网络端口
   - 7.5 数据精度规格
8. [安装指南](#8-安装指南)
   - 8.1 系统要求
   - 8.2 安装步骤
   - 8.3 授权激活
   - 8.4 验证安装
   - 8.5 卸载与更新
9. [快速入门](#9-快速入门)
   - 9.1 5分钟：放置灯具并配置 DMX
   - 9.2 10分钟：连接物理控台
   - 9.3 15分钟：Arena NDI 连接与渲染
   - 9.4 15分钟：录制激光到 Sequencer
10. [常见问题与故障排除](#10-常见问题与故障排除)
    - 10.1 安装问题
    - 10.2 DMX 问题
    - 10.3 激光问题
    - 10.4 NDI 问题
    - 10.5 性能问题
11. [已知限制与版本兼容性](#11-已知限制与版本兼容性)
    - 11.1 已知限制
    - 11.2 版本兼容性
    - 11.3 升级注意事项
12. [授权与定价](#12-授权与定价)
    - 12.1 产品模块划分
    - 12.2 套餐体系（7天体验 / Pro / Ultra / Team）
    - 12.3 各版本说明
    - 12.4 定价方式（季度订阅 + 买赠）
    - 12.5 订阅规则
    - 12.6 支付方式
13. [技术支持](#13-技术支持)
    - 13.1 官方渠道
    - 13.2 联系方式
    - 13.3 学习资源

---

## 1. 产品概述

### 1.1 定义：数字舞台的中央神经系统

SuperStage 是一款专为 **Unreal Engine 5** 架构研发的工业级舞台灯光与演艺生态系统。它超越了传统"可视化插件"的定义范畴，重新确立了数字舞台设计的底层逻辑。

SuperStage 旨在为 **虚拟制片（Virtual Production）**、**大型现场演出（Live Events）** 及 **跨媒体装置艺术** 提供一个 **确定性（Deterministic）、实时性（Real-Time）、全链路（End-to-End）** 的统一生产环境。

在传统演艺工作流中，创意被割裂在 CAD 绘图、离线预演、控台编程与媒体服务器渲染等多个孤立的软件孤岛中。数据的流转伴随着信息的丢失与精度的耗损。

**SuperStage 通过在虚幻引擎内部构建一套拥有自主知识产权的硬件抽象层（HAL），从根本上消除了这些壁垒。** 它不仅是一个渲染引擎，更是一个具备毫秒级响应能力的逻辑运算核心，能够同时调度 DMX 灯光网络、激光振镜数据、NDI 视频流以及无人机编队系统。

### 1.2 系统架构：唯一真理源

SuperStage 的架构设计遵循 **"零信任（Zero Trust）"** 的数据处理原则——系统不依赖外部硬件的反馈来维持稳定性，而是作为整个舞台系统的 **"唯一真理源（Single Source of Truth）"**。

#### 1.2.1 硬件抽象层（HAL）与协议转译

系统内核并不直接操作具体的 DMX 通道值，而是操作设备的 **"功能属性（Attributes）"**。例如，当设计师发出"灯具复位"的指令时，HAL 会根据连接设备的 GDTF/MA2/MA3 灯库描述文件，自动将其转译为对应品牌灯具所需的特定通道脉冲或持续电平信号。

这种机制确保了：

| 特性 | 价值 |
|------|------|
| **资产复用性** | 场景中的灯具可随时替换品牌，无需重写 CUE 列表 |
| **固件级还原** | 系统模拟设备物理行为（摇头灯惯性加速、色盘切换机械延迟） |
| **制造商信任** | 对硬件特性的精准还原，赢得专业厂商的深度认可 |

#### 1.2.2 统一时序引擎（Unified Timeline Engine）

SuperStage 深度集成 UE Sequencer，将其改造为支持 **SMPTE 时间码同步** 的广播级时间线。灯光 CUE、激光波形、视频纹理和机械运动被锁定在同一帧内进行渲染和输出。

这一设计彻底解决了传统工作流中"音画不同步"或"激光与灯光延时"的顽疾，为高精度的 Show Control 提供了底层保障。

### 1.3 核心定位

> **SuperStage = 舞美现场的操作系统（Stage OS）**
>
> SuperStage 解放了灯光控台的算力限制，使控台、激光系统、视频服务器、无人机地面站成为通往虚拟世界的物理接口。
> 创意资产在 SuperStage 中沉淀，硬件来来去去，平台恒久不变。

### 1.4 我们解决的问题

| 行业痛点 | SuperStage 方案 |
|---------|----------------|
| **协议碎片化** | 统一指令层，屏蔽 Art-Net/sACN/NDI/Beyond/MAVLink 差异 |
| **工具链割裂** | 提案、编程、渲染在同一 .uproject 文件中完成 |
| **预演与现场落差** | 数字孪生级保真度，1:1 制造商通道对齐 |
| **现场不可控** | 毫秒级时序保障，< 1ms 抖动，消除"黑天鹅"风险 |
| **知识流失** | 工程文件即资产，团队经验可复用可传承 |

### 1.5 适用边界

**SuperStage 适合：**
- 已在 Unreal Engine 工作流中的团队
- 需要影视级渲染能力的灯光预演与虚拟制片
- 需要激光/视频/无人机与灯光在统一时间轴编排
- 追求"办公室编程，现场一键输出"的确定性交付
- 对灯具物理特性有精准还原要求的专业项目

**SuperStage 不适合：**
- 仅需轻量级、非虚幻引擎流程的简单可视化
- 不使用 Unreal Engine 的团队
- 只需简单 UE 灯光效果（官方免费灯具库够用）

---

## 2. 核心价值

本部分阐述 SuperStage 如何将技术特性转化为 **生产力、安全性和商业回报**。

### 2.1 零摩擦工作流（Zero-Friction Workflow）—— 效率的指数级跃升

**传统痛点：** 在当前行业标准中，舞台设计是一个线性的、高损耗的过程。设计师需要在 CAD 软件中绘图，导出到控台进行配接，再导入可视化软件进行预演，最后在媒体服务器中合成视频。任何一次创意修改（如移动一根桁架），都需要在多个软件中重复操作，极易引入数据不一致的风险。

**SuperStage 解决方案：** 我们引入了 **"统一项目拓扑（Unified Project Topology）"** 概念——在 SuperStage 中，**场景即配接，位置即数据**。

| 能力 | 描述 |
|------|------|
| **实时所见即所得** | 在 UE5 视口中拖动灯具，DMX 地址、XYZ 坐标和遮挡关系实时更新 |
| **全流程单文件交付** | 从概念提案到编程预演到现场执行，所有数据在一个 .uproject 中完成 |
| **量化收益** | 500+ 灯具项目可节省 30%-40% 跨软件迁移与排错时间 |

### 2.2 数字孪生级保真度（Digital Twin Fidelity）—— 建立信任的基石

**传统痛点：** 通用可视化软件使用"通用配置文件"代替具体灯具——所有光束灯看起来都一样，所有 LED 变色都完美无瑕。这种"虚假的美好"是制造商和资深灯光师最为反感的，因为在现场设备上完全无法复现，导致预演与实操的巨大落差（The Reality Gap）。

**SuperStage 解决方案：** 我们实施了 **"1:1 制造商通道对齐（1:1 Manufacturer Channel Alignment）"** 策略。

| 维度 | 实现 |
|------|------|
| **光度学精准** | 每一盏虚拟灯具的光通量、色温、光斑分布（IES 文件）均经过物理校验 |
| **固件逻辑模拟** | 系统模拟"机器"而非仅"光"——Prism 插入的 0.5s 机械延迟会被精准呈现 |
| **灯库标准兼容** | 原生支持 GDTF/MA2/MA3 灯库格式导入，子属性系统完整还原 |
| **8/16/24 位精度** | Pan/Tilt 使用 24-bit 精度，长焦镜头下微小移动平滑无锯齿 |

> **战略意义：** 这种近乎偏执的严谨性，使 SuperStage 成为制造商展示产品特性的"安全港"——在这个系统中，他们的产品优势（更快的电机、更纯的色彩）能被直观看见。

### 2.3 跨维度媒体融合（Cross-Dimensional Convergence）

**传统痛点：** 激光、灯光和视频通常由三个独立团队使用三套系统（Pangolin、GrandMA、Resolume）分别控制。它们在物理空间中共存，但在数字空间中割裂——激光只是画面上的一层贴图，并不照亮周围物体；视频屏幕只是一个发光板，没有正确的光线反射。

**SuperStage 解决方案：** SuperStage 是首个在引擎底层实现 **全光谱融合** 的系统。

| 模块 | 融合能力 |
|------|---------|
| **Beyond 激光原生化** | 通过 UDP 5568 直接摄取 Pangolin Beyond 点云数据，转化为 UE 原生几何体，激光束可被玻璃折射、照亮烟雾、被景深模糊 |
| **NDI 视频体素化** | 视频不仅是纹理，而是光源——LED 屏幕播放的火焰视频会真实照亮虚拟角色 |
| **无人机编队同步** | LDLink 协议实现无人机与舞台灯光在统一 Sequencer 时间轴编排 |

### 2.4 主权与解耦：屏蔽异构差异的通用协议

传统模式下，系统受制于供应商私有协议。SuperStage 通过自主研发的 **动态转译内核**，强制统一了下行指令标准。

| 协议层 | SuperStage 处理方式 |
|--------|-------------------|
| **DMX (Art-Net/sACN)** | 统一寻址模型，屏蔽 Universe 映射差异，支持 100+ Universe |
| **激光 (Beyond)** | 点云数据 71.4% 压缩存储（28→8 字节/点），扫描仪物理模拟 |
| **视频 (NDI)** | 多源帧缓冲录制，支持 Alpha 通道，最高 8K 分辨率 |
| **无人机 (LDLink/MAVLink)** | 标准化飞控指令，地面站可替换 |

### 2.5 确定性交付：工业级的实时响应保障

拒绝"尽力而为"的软件通病。SuperStage 采用类 RTOS 的确定性调度架构（Deterministic Scheduling Architecture），逻辑线程独立于渲染帧运行。

| 指标 | 保障 |
|------|------|
| **DMX 延迟** | < 16ms（一帧内响应） |
| **时序抖动** | < 1ms（确定性调度） |
| **激光同步延迟** | < 1 帧（16.6ms @ 60fps） |
| **高并发稳定性** | 500+ 灯具 60fps 稳定 |
| **故障隔离** | 单设备故障不影响全局 |

### 2.6 典型应用场景

| 场景 | SuperStage 价值 |
|------|----------------|
| **虚拟演唱会预演** | 完整灯光编程 + 影视级渲染，提案即交付 |
| **大型巡演预演** | 混合模式：物理推杆触发 SuperStage 程序化效果，落地即演出 |
| **LED 墙虚拟制作** | NDI 帧录制，脱离视频服务器离线渲染，支持 Path Tracing 8K 重渲染 |
| **激光秀制作** | Beyond 点云录制，Sequencer 精确编排，扫描仪物理模拟 |
| **无人机天地联动** | 编队与舞台灯光在统一时间轴协同 |
| **离线编程交付** | 办公室编程，现场 Art-Net 一键输出 |

---

## 3. 设计哲学

以下原则是 SuperStage 的设计底线，不可逾越。这些原则不仅指导技术实现，更定义了产品在行业中的战略定位。

### 3.1 Configuration over Customization（配置优于定制）

**我们不为任何单一厂家写死代码。**

所有硬件特性必须通用化、配置化。如果某厂家要求一个非标功能，我们要么将其抽象为通用功能纳入平台，要么依据此原则拒绝。

这确保了 SuperStage 的中立性——我们是标准的制定者，不是某个厂家的外包团队。

**技术实现：** 灯库数据资产（`USuperFixtureLibrary`）采用模块化设计，支持 GDTF/MA2/MA3 标准导入，通过子属性系统（`FSubAttribute`）描述任意品牌灯具的通道行为，无需硬编码。

### 3.2 Graceful Degradation（优雅降级）

**当底层硬件出现故障时，SuperStage 必须保证核心流程不崩溃。**

| 故障场景 | SuperStage 响应 |
|---------|----------------|
| 单台灯具离线 | 标记状态，其余灯具正常运行 |
| Art-Net 网络中断 | 保持最后有效状态，自动重连 |
| NDI 源断流 | Hold-Last 模式显示最后帧 |
| Beyond 未启动 | 激光层静默，不阻塞 Sequencer 时间线 |
| 无人机通讯丢失 | 编队数据缓存，恢复后续传 |

这体现了我们比硬件厂更懂系统的健壮性。

### 3.3 Single Source of Truth（单一数据源）

**所有的状态判定以 SuperStage 为准，硬件端的状态只是"影射"。**

- 灯具的"当前值"由 SuperStage 定义，而非从硬件反读
- 时间轴的"播放位置"由 Sequencer 驱动，外部设备跟随
- CUE 数据存储完整的 DMX 快照（512 通道/Universe）
- 冲突时，SuperStage 的指令优先级最高

这确立了数据的统治权——我们是大脑，硬件是四肢。

### 3.4 Zero Trust for External Systems（外部系统零信任）

**不假设任何外部系统是可靠的。**

所有外部输入（DMX、NDI、Beyond、MAVLink）都经过校验和容错处理。外部系统的异常不会污染 SuperStage 的内部状态。

**技术实现：**
- DMX 输入经过地址范围校验（1-512）
- NDI 帧缓冲带时间戳校验，拒绝乱序帧
- 激光点云数据经过坐标范围校验（[-1, 1]）
- 网络数据包采用 Magic Number + CRC 校验

---

## 3.5 技术深潜：支撑价值的隐形护城河

以下技术细节是说服技术总监（TD）和极客型用户的关键。

#### 3.5.1 DMX 属性系统的数学逻辑

SuperStage 的 DMX 系统不直接操作通道值，而是操作 **"功能属性"**。属性读取支持三种精度：

| 精度 | 范围 | 应用场景 |
|------|------|---------|
| **8-bit** | 0-255 | Dimmer、Gobo 选择 |
| **16-bit** | 0-65535 | Pan/Tilt 精确定位 |
| **24-bit** | 0-16777215 | 超高精度运动控制 |

**数据结构优化：** 使用 Coarse/Fine/Ultra 三字节组合，24-bit 精度确保即使在长焦镜头下，灯具的微小移动也平滑无锯齿。

#### 3.5.2 NDI 帧级录制技术

SuperStage 开发了 **"NDI 序列化容器"**（`USuperNDIMultiSourceFrameBuffer`），能够将即时的 NDI 视频流捕获并写入硬盘，转化为与 Sequencer 时间轴锁定的媒体资产。

| 参数 | 规格 |
|------|------|
| **目标帧率** | 30/60 FPS 可配置 |
| **降采样** | 1.0/0.5/0.25 倍（减少 75% 内存占用） |
| **最大时长** | 15 分钟（可配置） |
| **检索算法** | 二分查找 O(log N)，Hold-Last 模式 |

**应用场景：** 直播结束后，使用 Path Tracing 对包含外部视频流的演出进行 8K 级别离线重渲染。

#### 3.5.3 激光点云压缩算法

Beyond 原始点数据占用 28 字节/点，SuperStage 实现了 **71.4% 压缩率**：

| 字段 | 原始 | 压缩 | 精度损失 |
|------|------|------|---------|
| **X/Y 位置** | float32 (8B) | int16 (4B) | 0.003% |
| **RGB 颜色** | float32 (12B) | uint8 (3B) | 0.4% |
| **Focus+Z** | float32 (8B) | uint8 (1B) | 0.8% |
| **总计** | 28 字节 | 8 字节 | — |

**扫描仪物理模拟：** 支持边缘淡化、速度平滑、光束重复点检测，还原真实激光扫描仪的物理特性。

#### 3.5.4 混合控制模式（Hybrid Control）

系统设计了三种模式以适应不同用户的肌肉记忆：

| 模式 | 描述 |
|------|------|
| **接收模式（Listen）** | 纯粹作为可视化端，接收 MA3/Hog4 的 Art-Net 数据 |
| **发送模式（Master）** | SuperStage 作为主控台，直接输出 DMX 到物理世界 |
| **混合模式（Hybrid）** | 物理推杆触发 SuperStage 内部逻辑，结合手感与算法无限性 |

**混合模式** 是 SuperStage 的杀手锏——用户可以使用物理推杆（MIDI 控制器或 MA 控台）触发 SuperStage 内部的复杂效果逻辑，再由 SuperStage 将最终的 DMX 值计算出来发送给灯具。系统支持自定义优先级逻辑（HTP 最高优先 / LTP 最新优先），确保多源输入时行为可预期。

---

## 4. 核心子系统（Core Subsystems）

SuperStage 由以下核心子系统组成：

### 4.1 灯光控制系统

SuperStage 的灯光控制系统包含 DMX 通讯、灯具库、灯具 Actor 三大部分，实现从"放置灯具"到"控制输出"的完整工作流。

#### 双向 Art-Net / sACN 通讯

SuperDMX 模块同时支持 **Art-Net 4** 和 **sACN/E1.31** 双协议，与传统控台和实体灯具双向联动：

- **接收模式** - 用你熟悉的 MA/GrandMA/珍珠台编程，UE 实时预览灯光效果
- **发送模式** - 在 UE 中编程完成后，输出 Art-Net/sACN 控制实体灯具
- **混合模式** - 虚拟灯 + 实体灯同时控制
- **sACN 特性** - 自动多播组管理、E1.31 三层 PDU 完整实现、单播/多播双模式

**应用场景：**
- 办公室编程，现场直接播放
- 接几盏真灯验证虚拟与实际一致性
- MA 编程 + UE 预览渲染

#### 零配接灯具系统

**传统控台流程：** 导入灯库 → Patch 配接 → 验证通道 → 开始编程

**SuperStage 流程：**
1. 在场景中放置灯具 Actor（已带完整通道定义）
2. 填写 Universe + 起始地址
3. 点击"扫描灯具" → 自动识别 → 开始编程

**功能亮点：**
- 一键扫描场景中所有灯具
- 自动识别灯具类型和通道映射
- 支持 8/16/24 位精度通道
- 多模块灯具（如矩阵灯）统一管理

#### 专业灯具库

已收录 Acme、Chauvet、ClayPaky、EK、ETC、Robe 等国内外品牌 **38+ 灯具配置**，持续更新中。

**灯库编辑器功能：**
- 可视化属性编辑（亮度/位置/图案/颜色/光束/聚焦/控制/切割/频闪/棱镜/雾化/效果）
- Coarse/Fine/Ultra 多精度通道配置
- 多模块实例管理（矩阵灯、多头灯）
- 支持导入 MA2/GDTF 灯库格式

**厂商合规认证（Manufacturer Certification）：** 我们开放了硬件接入标准。制造商可申请加入 **SuperStage 认证合作伙伴计划**。经实验室级物理校验通过的设备（几何精度、光度学、机械延迟），将作为 **"实验室校验资产（Lab-Verified Assets）"** 内置于全球分发版本中。认证申请：yerrkj@outlook.com

#### 地址码可视化

每个灯具 Actor 自动显示地址码标签：
- 格式：`UA.{Universe}.{StartAddress} ID.{FixtureID}`
- 编辑器中清晰可见，Game 模式自动隐藏
- 支持自定义偏移位置

### 4.2 SuperConsole - 内置专业控台

SuperConsole 是完全集成在 UE5 编辑器中的专业级灯光控台，实现"零硬件"灯光编程。它是一个功能完备的灯光控制系统，核心概念与 grandMA 系列控台高度一致：Patch 配接、Selection 选择、Programmer 编程器、CUE 场景、Preset 预设、Frame Effect 帧效果、Timeline 时间线、Layout 布局、Show File 演出文件。

#### 界面布局

控台主界面由以下区域组成：

| 区域 | 位置 | 功能 |
|------|------|------|
| **控制栏** | 顶部 | 核心操作按钮（Clear、Store、Update、Delete、Copy、命令行） |
| **标题栏** | 标题区域 | 显示演出文件名称和当前状态 |
| **网格窗口管理器** | 中央 | 可拖拽的多窗口面板系统，自由排列各功能面板 |
| **View 视图栏** | 底部 | 视图快速切换（最多 10 个自定义视图） |
| **Command Line** | 控制栏 | 命令行输入（文本指令控制） |
| **Encoder Bar** | 底部 | 虚拟编码轮，实时调整属性值 |

#### 主要功能面板

**Patch 配接面板**
- 一键扫描场景中所有灯具 Actor
- 自动识别灯具类型、Universe、地址
- 批量导入/移除灯具到控台
- 检测丢失/需同步的灯具（场景与控台不一致时提示）
- 编辑 Patch 信息：可直接修改灯具的 Fixture ID、标签、Universe、地址
- 搜索与过滤：按名称/类型/地址快速查找灯具
- 统计显示：显示灯具总数、类型分布等统计信息

**Fixture Sheet 灯具表**
- 类似 MA 控台的灯具表视图，每行一台灯具，每列一个属性
- 按 Fixture ID / Universe / 地址排序
- 属性值显示规则：百分比格式，绿色背景 = 编程器值，黄色背景 = 预设值
- 支持直接编辑属性值
- 显示选择顺序编号

**Programmer 编程器**
- 灯光控台的核心概念——编程器是一个**临时值缓冲区**
- 设计师在编码器/颜色选择器/预设中调整的值首先写入编程器
- 编程器中的值拥有 **最高 DMX 输出优先级**
- **Clear** 操作清空编程器中的所有值
- **Clear 属性** 可选择性清除单个属性
- 编程器值通过 **Store** 保存为 CUE 或 Preset

**Selection 选择与选择工具**
- 基础选择：单选、Ctrl 多选、框选
- 子灯具选择：矩阵灯支持选择单个像素/灯头
- 灯组选择：点击灯组快速选中整组灯具
- **选择顺序**：选中灯具的先后顺序会影响帧效果的分布
- 7 种高级选择工具：

| 工具 | 功能 |
|------|------|
| **Block** | 按固定块大小分组 |
| **Group** | 按组数量均分 |
| **Wings** | 对称翅膀分组（从两端向中间） |
| **Shuffle** | 随机打乱选择顺序 |
| **Reverse** | 反转选择顺序 |
| **Shift** | 循环移位选择顺序 |
| **Interleave** | 交错重排选择顺序 |

**Groups 灯组管理**
- 从当前选择创建灯组，自动命名
- 快速选择整组灯具
- 支持拖拽从灯组加载到编程器
- 编辑/删除/重命名灯组

**Playback CUE 播放**
- CUE 执行器按钮网格（支持分页，每页最多 60 个按钮）
- 三种触发模式：**Toggle**（点按切换）、**Momentary**（按住激活）、**Flash**（闪烁）
- 执行器按钮状态：Idle（空闲）、Active（运行中）、Releasing（释放中）
- CUE List 管理：每个执行器绑定一个 CUE List，支持多个独立 CUE List
- **Go 按钮**：按顺序执行 CUE List 中的下一个 CUE
- **Release All**：一键释放所有正在运行的 CUE
- **Running Playbacks 面板**：实时显示所有正在运行的 CUE 列表及其状态
- 快捷键绑定（F1-F12 等自定义）

**Color Picker 颜色选择器**
- **HSV 色轮**：直观拾色
- **RGB 滑块**：精确通道控制
- **颜色预设库**：常用色快速调取
- **高级颜色通道**：支持扩展颜色参数

**Preset 预设管理**
- 按属性分类存储，8 大类别：

| 类别 | 说明 |
|------|------|
| **Dimmer** | 亮度预设 |
| **Position** | 位置预设（Pan/Tilt 组合） |
| **Gobo** | 图案预设 |
| **Color** | 颜色预设 |
| **Beam** | 光束预设（Zoom/Iris） |
| **Focus** | 聚焦预设 |
| **Control** | 控制预设（Reset/Lamp） |
| **Shapers** | 切割预设 |

- 从当前编程器选择创建预设
- 一键应用预设到选中灯具
- 支持更新、删除、重命名、复制预设

**Frame Editor 帧效果编辑器**
- **波形类型**：Sine / Saw / Rect / Cos / Triangle 等
- **全局参数**：
  - Speed 速度 / Width 占空比 / Attack 曲线 / Decay 衰减
  - Phase Range 相位范围 / Measure 量度 / Loop Mode 循环模式
- **分组参数**（控制效果在灯具间的分布）：
  - **Wings**：对称翅膀模式
  - **Block**：块分组
  - **Group**：均分组
- **步骤系统**：多步骤关键帧动画，每步可独立设置属性参数
- **主控参数**：全局速率/大小控制
- **帧预设库**：保存/加载/删除帧效果预设

**Timecode 时间线编辑器**
- 多时间码池管理（创建/切换/删除/重命名）
- CUE 轨道拖拽编排：添加/移动/调整长度/删除 CUE 片段
- 音频轨道支持：添加/分割/调整音量/拷贝粘贴音频片段
- BPM 节拍网格设置
- 循环配置
- 标记点管理
- 播放控制：播放/暂停/停止/跳转/变速/缩放平移
- **导出到 UE Sequencer**：将时间线转换为 Sequencer 序列

**Layout 布局视图**
- **2D 俯视布局**：从顶部鸟瞰的灯具位置分布图
- 灯具图标根据灯具类型/颜色/亮度实时更新显示
- 多 Layout 保存管理（创建/切换/添加灯具/从世界坐标重置/删除/重命名）
- 画布操作：平移/缩放/重置视图
- 灯具位置编辑：在 2D 视图中直接拖拽灯具位置
- 选择工具：单选、多选、框选（支持选择方向）

**Encoder Bar 编码器栏**
- 虚拟编码轮控制属性值
- 属性分组标签页切换（Dimmer/Position/Color/Gobo/Beam/Focus/Control/Shapers 等）
- 编码器操作：拖拽调值、鼠标滚轮微调、双击精确输入
- 5 种编辑模式：**Value**（值）/ **FadeIn** / **DelayIn** / **FadeOut** / **DelayOut**
- Coarse/Fine 精度切换
- **Spread 扩展模式**：`>` / `<` / `><` / `<>` 四种分布模式

**DMX 设置面板**
- DMX 全局输出开关
- 独立 Universe 启用/禁用控制
- 网络配置（协议/IP/端口）

**Settings 设置面板**
- **Show 标签页**：演出全局设置
- **Patch 标签页**：配接相关设置
- **DMX 标签页**：DMX 输出配置

**Show File 演出文件**
- 新建/打开/保存/另存为演出文件（.ssshow 格式）
- 自动加载上次打开的文件
- 完整状态持久化，包含：Patch、Groups、CUE、Preset、帧效果预设、Timeline、Layout、窗口布局、快捷键绑定、DMX 设置

#### CUE 场景系统

- **CUE 录制**：从当前编程器状态存储 CUE（支持同时存储帧效果）
- **CUE 编号**：支持整数和小数编号（如 1、1.5、2、2.5），便于插入
- **渐变控制**：FadeIn / FadeOut / DelayIn / DelayOut 四个独立时间参数
- **时间分布**：支持 `0s Thru 2s` 语法（第一个灯 0s，最后一个灯 2s，中间线性插值）
- **触发模式**：Toggle（切换）、Momentary（瞬时）、Flash（闪烁）
- **多 CueList**：支持多个独立 CUE 列表，每个绑定到一个执行器
- **帧效果 CUE**：CUE 可包含帧效果，运行时持续播放
- **CUE 编辑**：更新（Update）、合并（Merge）、加载到编程器、重命名、移动、复制、删除

#### DMX 输出优先级

SuperConsole 的 DMX 输出采用**多来源优先级混合机制**，从高到低依次为：

| 优先级 | 来源 | 说明 |
|--------|------|------|
| **1（最高）** | Programmer 编程器 | 设计师实时调整的值 |
| **2** | Frame Effect 帧效果 | 当前运行的帧效果输出 |
| **3** | CUE Playback | CUE 播放输出 |
| **4** | Timecode CUE | 时间码触发的 CUE |
| **5** | Timeline | 时间线播放输出 |
| **6（最低）** | Default Value | 灯具默认值 |

**混合规则**：
- **Dimmer（亮度）属性**：采用 **HTP（Highest Takes Precedence）** 规则——取所有来源中的最高值
- **其他属性**：采用 **LTP（Latest Takes Precedence）** 规则——取最高优先级来源的值

#### 快捷键系统

SuperConsole 提供完整的快捷键体系，覆盖 CUE 操作、通用操作、选择、编码器、时间线、布局视图等六大领域，并支持 CUE 快捷键自定义绑定

### 4.3 SuperLaser - Beyond 激光系统集成

SuperLaser 实现 Pangolin Beyond 激光软件与 UE5 的实时联动，支持录制到 Sequencer 进行离线渲染。

**系统要求**：UE 5.6 – 5.7、Windows 64-bit、DLL 依赖（linetD2_x64.dll + matrix64.dll）

#### 两种激光 Actor

SuperLaser 提供两种渲染模式的激光 Actor，适用于不同场景：

| 特性 | SuperLaserActor（纹理模式） | SuperLaserProActor（程序化网格模式） |
|------|--------------------------|----------------------------------|
| **渲染方式** | RenderTarget 纹理投射 | ProceduralMesh 实时生成 |
| **视觉效果** | 体积雾化效果，适合烟雾环境 | 精确激光线渲染，边缘锐利 |
| **碰撞检测** | 不支持 | 支持（光束遇障碍物自动截断） |
| **性能开销** | 较低 | 较高（逐点生成网格） |
| **适用场景** | 背景氛围、大面积扫描 | 精确投射、碰撞遮挡需求 |

#### Beyond 连接配置

**Beyond 端：**
1. 清理默认投影区域
2. 添加投影区域（每个区域代表一台激光灯）
3. 设置灯具号（Fixture Number）：1、2、3、4...
4. 菜单 → 查看 → 勾选"启用外部可视化输出"

**UE5 端：**
1. 放置 SuperLaserProActor 到场景
2. 设置 DeviceID（与 Beyond Fixture Number 一一对应）
   - UE DeviceID = 1 → Beyond Fixture = 1
   - UE DeviceID = 2 → Beyond Fixture = 2
3. 连接成功后，Beyond 播放节目，UE 实时显示激光效果

#### 网络协议

- **协议**：UDP 多播
- **端口**：5568（Beyond 默认）
- **多播地址**：239.255.{DeviceID}.{SubnetID}
- **设备数**：最多 4 台（DeviceID 1-4）
- **依赖 DLL**：linetD2_x64.dll + matrix64.dll（Beyond 协议解析）

#### SuperLaserProComponent 激光渲染

**程序化网格激光（ProceduralMesh）：**
- 每个激光点生成一条四边形网格
- 相邻点连线，跳过空白点
- 支持碰撞检测截断（LineTrace）

**几何与材质参数：**

| 参数 | 说明 |
|------|------|
| BeamLength | 光束长度（默认 5000cm） |
| ProjectionAngle | 投射角度（默认 30°） |
| LaserWidth | 激光线宽度 |
| CoreSharpness | 核心锐度 |
| DepthFade | 深度衰减（UV.Y 编码距离比例） |
| Dim | 自发光强度 |
| FogInfluence | 烟雾影响强度 |
| SpotIntensity | 落点光斑强度 |
| SpotRadius | 落点光斑半径 |

#### 扫描仪模拟与质量设置

**扫描仪物理模拟参数：**
- **SampleCount**：采样点数，控制模拟精度
- **EdgeFade**：边缘淡化，模拟真实扫描仪的边缘衰减
- **VelocitySmoothing**：速度平滑，模拟振镜惯性

**质量级别降采样：**

| 级别 | 说明 |
|------|------|
| **Low** | 最大降采样，性能最优 |
| **Medium** | 平衡模式 |
| **High** | 高质量渲染 |
| **Ultra** | 最高质量，无降采样 |

**光束检测**：当检测到连续重复点时，自动识别为高强度光束（非扫描线），渲染为更亮的光柱效果。

#### 数据处理管线

- 扫描仪物理模拟（速度平滑、边缘淡化）
- 光束检测（连续重复点 → 高强度光束）
- 质量级别降采样
- 点插值增加密度

#### Sequencer 录制

**录制流程（Take Recorder）：**
1. 窗口 → 过场动画 → 镜头试拍录制器
2. 添加"激光输入源"
3. 为每台设备添加轨道（ID 1、2、3、4）
4. 点击录制，同时在 Beyond 播放时间线
5. 停止录制，数据自动保存

**录制优化：**
- 使用原始点（未插值），文件小得多
- 可选移除空白点
- 可选降采样
- 压缩存储（28字节→8字节，压缩比 71.4%）

**播放机制：**
- 查找 ≤ 当前时间的最后一个关键帧
- 解压并设置到 LaserSubsystem
- 录制后可完全脱离 Beyond 进行离线渲染
- **录制与播放互斥**：不能同时录制和播放

#### 多设备管理

- 支持最多 4 台激光设备同时工作
- 每台设备通过 DeviceID 唯一标识
- 场景中可放置多个不同 DeviceID 的 Actor
- 关闭编辑器或切换关卡时自动释放网络资源

### 4.4 SuperNdi - NDI 视频系统

SuperNdi 实现 NDI 视频流的接收、录制和渲染，解决官方 NDI 插件无法离线渲染的痛点。

#### Arena (Resolume) 连接配置

**Arena 端：**
1. 开启 NDI 输出功能
2. 高级输出 → 添加屏幕（按 UE 屏幕数量添加）
3. 每个屏幕的 Device 选择 **NDI**
4. 分配图层（屏幕1→图层1，屏幕2→图层2...）

**UE5 端：**
1. 底部 NDI 设置 → 添加输入源（选择 Arena 的 Screen 1/2/3...）
2. 放置 **SuperScreen** 资产到场景
3. 添加屏幕网格 → 吸管工具选取 3D 屏幕模型
4. 设置输入源名称对应 Arena 输出

#### SuperScreen Actor

**材质与纹理：**
- 自动创建动态材质实例（MID）
- BGRA 格式纹理实时更新（RHI 异步上传）
- 支持不透明/透明材质切换

**梯形校正参数：**
| 参数 | 说明 |
|------|------|
| UpperLeftCorner | 左上角 UV 偏移 |
| UpperRightCorner | 右上角 UV 偏移 |
| LowerLeftCorner | 左下角 UV 偏移 |
| LowerRightCorner | 右下角 UV 偏移 |
| Color | 颜色叠加 |
| Brightness | 亮度 |
| Contrast | 对比度 |
| Transparency | 透明度 |

#### USuperNDISubsystem 核心功能

**NDI SDK 集成：**
- 显式加载 Processing.NDI.Lib.x64.dll（避免系统 DLL 冲突）
- 持久化 Finder 自动发现源（mDNS）
- 50Hz 轮询接收帧 + 2.5s 刷新发现缓存

**格式转换：**
- BGRA/BGRX：直接 Memcpy
- UYVY：BT.709 YUV→RGB CPU 转换

**源匹配逻辑：**
1. 检查 LogicalToExternal 映射
2. Canonicalize 规范化（去空格/括号）
3. 精确匹配（IgnoreCase）
4. 模糊匹配（Contains 双向）

#### Sequencer 录制与回放

**录制流程（Take Recorder）：**
1. 打开镜头试拍录制器
2. 添加 NDI Source → 添加各输入源轨道
3. **重要：录制时将 Arena 切换到前台**（后台运行会卡顿）
4. 点击录制，Arena 播放素材
5. 停止录制，数据自动保存

**录制优化：**
- 降采样存储（默认 0.5 → 960x540）
- 二分查找帧数据 O(logN)
- BGRA 直接引用无额外拷贝

**回放机制（环回模式）：**
1. Setup：BeginLoopback 屏蔽真实 NDI 帧
2. Evaluate：GetFrameAtTime → InjectFrameBGRA
3. TearDown：EndLoopback 恢复真实接收器

### 4.5 SuperDrone - 无人机编队模块

SuperDrone 将实时无人机编队表演引入 UE5，接收 **LimxDroneStudio** 的 LDLink 数据流，实现天地联动的舞美效果。

**系统要求**：UE 5.3+、Windows 64-bit

**核心能力：**
- 接收 LDLink 协议数据流（MAVLink 兼容）
- 万级无人机实时渲染
- 位置 + 姿态 + LED 颜色同步
- 地理锚点（GeoAnchor）坐标映射
- Sequencer 录制与离线渲染

#### UE 端核心组件

| 组件 | 功能 |
|------|------|
| **UDroneLinkSubsystem** | 引擎级子系统，全局单例，管理无人机状态与网络接收 |
| **FLDLinkReceiver** | UDP 接收器（独立线程），解析 LDLink 协议 |
| **ADroneSwarmManager** | HISM 批量渲染，10,000+ 无人机，适合大编队 |
| **ASuperDroneActor** | 单体无人机 Actor，可挂载子组件，适合小规模/精细控制 |

**DroneSwarmManager vs SuperDroneActor 选择：**

| 对比项 | DroneSwarmManager | SuperDroneActor |
|--------|-------------------|-----------------|
| **渲染方式** | HISM 批量渲染（单 DrawCall） | 独立 Actor（每架一个 DrawCall） |
| **性能** | 万级无人机无压力 | 适合数十到数百架 |
| **扩展性** | 固定外观 | 可挂载子组件（摄像机、灯光、特效） |
| **适用场景** | 大规模编队表演 | 特写镜头、需要独立交互 |

#### LDLink 协议

- **基于 MAVLink**：兼容 MAVLink 消息格式
- **传输方式**：UDP，默认端口 **14555**
- **OpCode 类别**：心跳、位姿数据、LED 控制、状态报告
- **寻址方式**：扩展寻址 DroneId = (ComponentId - 1) × 256 + SystemId
- **心跳超时**：2 秒（超时自动移除无人机，触发 OnDroneRemoved 事件）
- **丢包检测**：通过序列号判断

#### GeoAnchor 地理锚点系统

无人机数据使用 WGS84 经纬度坐标，需要映射到 UE 世界坐标系。GeoAnchor 提供三种锚定模式：

| 模式 | 说明 |
|------|------|
| **Auto** | 收到第一包数据时，自动以首架无人机位置为原点建立锚点 |
| **Manual** | 手动输入经纬度/高度作为锚点原点（适合固定演出场地） |
| **Reset** | 清除当前锚点，重新建立 |

**坐标转换流程**：WGS84 (经纬高) → NED (北东地) → UE 世界坐标（+X=北, +Y=东, +Z=上），精度为厘米级。

#### ADroneSwarmManager 渲染优化

- HISM（分层实例化静态网格）- 单 DrawCall 绘制万级无人机
- NumCustomDataFloats = 4（RGBA LED 颜色传递到 GPU）
- 禁用碰撞、阴影、距离剔除
- 30fps 更新频率
- 脏检测优化（位置变化阈值 0.5cm）
- 对象池：预分配实例，避免运行时分配

#### ASuperDroneActor 功能

- 单体 Actor，可挂载子组件（摄像机、灯光、特效）
- 动态材质实例
- 位置/旋转插值平滑
- LED 颜色同步到材质参数

#### Sequencer 支持

- **MovieSceneSuperDroneTrack**：无人机专用轨道
- **10 通道曲线**：Position(X/Y/Z)、Attitude(Roll/Pitch/Yaw)、Led(R/G/B/Brightness)
- 录制模式 + 离线渲染

### 4.6 SuperShader - 着色器系统

SuperShader 提供专业舞台灯光所需的各类材质和着色器效果。

**核心功能：**
- 光束材质：光柱、雾气、丁达尔效果
- 频闪算法：8 种模式（Closed/Open/Linear/Pulse/RampUp/RampDown/Sine/Random）
- 自发光材质：LED 屏幕、灯带效果
- 物理光照：与 UE Lumen 深度集成

#### 棱镜预设系统（Prism Builder）

SuperShader 内置棱镜渲染引擎，通过数据资产 + 可视化编辑器实现任意形状的棱镜效果。

**USuperPrismPreset 数据资产：**
- **三层独立配置**：每个棱镜预设支持 3 层（Layer），每层最多 48 个分光面（Facet）
- 每面定义 X/Y 偏移和缩放参数
- 内置 11 种预设模板：Circle / Line / Triangle / Square / Pentagon / Hexagon / Star / Cross / Diamond / Arc / Honeycomb
- 懒创建 48×1 RGBA16F 位置查找纹理（R=X 偏移, G=Y 偏移, B=缩放, A=预留）
- 在内容浏览器中显示棱镜图案缩略图

**SPrismPresetEditor 2D 画布编辑器：**
- 鼠标拖拽调整面位置
- 双击添加新面
- **多选操作**：框选/Ctrl 多选面，批量移动/删除
- 层切换（Layer 1/2/3），每层独立编辑
- 精确数值编辑面板（坐标、缩放）
- 预设模板一键生成
- 实时 2D 预览棱镜图案
- **3D 光束预览**：实时查看棱镜在光束中的分光效果

**USuperPrismDraw 材质表达式：**
- 8 个输入引脚（位置纹理、面数、旋转、缩放等）
- 内嵌 HLSL 循环读取查找纹理，动态面数循环优化

### 4.7 SuperStage - 核心框架

SuperCore 模块是整个插件的核心，提供 **真正还原物理灯具行为** 的虚拟舞台设备。每个灯具资产都经过精心设计，确保与真实灯具的控制方式完全一致。

#### Actor 继承层级

SuperStage 采用四层 Actor 继承架构，每层职责清晰：

| 层级 | Actor | 职责 |
|------|-------|------|
| **L1 基础层** | ASuperBaseActor | 资产元数据、通用基础功能 |
| **L2 DMX 层** | ASuperDmxActorBase | DMX 读取、灯库绑定、灯具配址、多模块管理 |
| **L3 运动层** | ASuperLightBase | Pan/Tilt 运动、速度控制、地址码标签 |
| **L4 灯光层** | ASuperStageLight | 完整灯光功能（颜色、图案、棱镜、切割、效果等） |

其他 Actor 类型（机械、屏幕、激光等）继承 L2 或 L1，根据需求选择合适的继承层级。

#### 光组件系统

SuperStageLight 内部的光渲染由**光组件（Light Component）** 层级承担，每个组件负责特定的光学行为：

| 组件 | 继承自 | 职责 |
|------|--------|------|
| **USuperLightingComponent** | USceneComponent | 光束基础（镜头材质、光斑材质、亮度/颜色/频闪/图案旋转） |
| **USuperSpotComponent** | USuperLightingComponent | 真实 SpotLight 光源（阴影、体积光、Zoom、Frost、Iris） |
| **USuperBeamComponent** | USuperSpotComponent | Beam 光束效果（Gobo 图案、棱镜、高亮度光柱） |
| **USuperCuttingComponent** | USuperBeamComponent | 四叶片切割系统（8 通道控制 + 旋转） |
| **USuperRectComponent** | USuperLightingComponent | RectLight 面光源（Wash 灯、LED 面板） |
| **USuperEffectComponent** | USuperLightingComponent | LED 效果层（15 种内置效果，独立 RGB/Dimmer） |
| **USuperMatrixComponent** | USuperLightingComponent | LED 矩阵像素（独立控制每个像素的颜色/亮度） |
| **USuperLiftComponent** | USceneComponent | 升降组件（灯头升降/矩阵阵列） |
| **USuperLaserProComponent** | USceneComponent | 激光渲染（ProceduralMesh 点云） |

蓝图灯具根据灯型选择挂载不同组件组合。典型配置：
- **Spot 灯**：SpotComponent
- **Beam 灯**：BeamComponent
- **Profile 灯**：CuttingComponent
- **Wash 灯**：RectComponent
- **效果灯**：EffectComponent（+ 可选 SpotComponent）
- **矩阵灯**：MatrixComponent（×N 个像素）

#### 专业电脑灯 (SuperStageLight)

**像真实灯光师一样控制虚拟灯具。** SuperStageLight 完整还原专业电脑灯的所有功能，支持 Martin、Robe、ClayPaky 等主流品牌的灯具模拟。提供 **90+ BlueprintCallable 函数**，覆盖所有灯具控制需求。

**运动控制：**
- **Pan/Tilt**：水平 ±270°、垂直 ±135°，范围可自定义
- **无极旋转**：持续旋转模式，适合效果灯
- **速度控制**：PT Speed 通道控制移动快慢
- **矩阵模式**：多灯头独立 Pan/Tilt（如 Robe BMFL WashBeam）

**亮度与频闪：**
- **Dimmer**：0-100% 平滑调光，支持 16-bit 精细控制
- **亮度公式**：`最终亮度 = ActorDimmer × (MaxLightIntensity / 100) × ComponentDimmer × StrobeMultiplier`
- **Dimmer Curve**：指数曲线（指数 1.0 ~ 3.0，默认 2.0），模拟不同灯具的调光曲线特性
- **频闪**：8 种模式（关闭/常亮/线性/方波/锯齿上升/锯齿下降/正弦/随机乱闪）
- **频闪速度**：0-25Hz 可调

**颜色系统（6 种混色方式）：**
- **RGB 直控**：红/绿/蓝三通道独立控制
- **RGBW 混色**：加白光通道，色彩更饱满
- **HSV 控制**：色相/饱和度/明度，符合设计师直觉
- **颜色轮**：最多 3 个颜色轮，固定色选择、流水跑马、半色效果
- **CMY 减色**：青/品红/黄滤片物理叠加
- **色温调节**：1700K-12000K 冷暖连续可调
- **动态混色通道**：支持灯库定义的任意扩展颜色通道（如 Amber、UV、Lime 等）

**图案系统：**
- **双图案轮**：Gobo1 + Gobo2 同时工作
- **图案选择**：固定/流水/抖动三种模式
- **图案旋转**：静态角度 或 无极旋转（正/反向）
- **图案叠加**：两个图案轮效果叠加

**棱镜系统：**
- **三棱镜位**：Prism1/Prism2/Prism3 独立控制
- **棱镜参数**：面数、半径、缩放可配置
- **棱镜旋转**：静态 或 无极旋转
- **优先级**：棱镜优先于图案显示

**切割系统（Profile 灯）：**
- **四叶片切割**：A1/B1 到 A4/B4 八通道控制
- **切割旋转**：整个切割系统可旋转 ±45°
- **与图案联动**：切割旋转叠加图案旋转

**效果层（独立于主光束）：**
- **15 种内置效果**：脉冲、波浪、追逐、扫描、呼吸等
- **效果参数**：速度（-4 到 +4）、宽度（0.1-4.0）可调
- **独立颜色/亮度**：效果层有自己的 RGB 和 Dimmer

**光圈与雾化：**
- **Iris**：光圈大小 0-100%，缩小光束直径
- **Frost**：雾化效果 0-100%，采用**双机制模拟**——材质模糊 + SpotLight 锥角扩展，同时柔化图案边缘和光束边缘
- **Focus**：焦距调节
- **Zoom**：变焦控制，采用**分段线性插值映射表**精确模拟真实灯具的非线性 Zoom 曲线

**射线检测（Ray Detection）：**
- 灯具光束自动检测场景碰撞，实现光斑投射到地面/墙面的效果
- 两级检测策略：粗检测（低频）确定目标面 + 精检测（高频）更新光斑位置
- 可设置检测距离和响应通道

#### 激光灯

**SuperLaserActor** - 接收 Beyond 软件的激光点云数据，在 UE5 中实时渲染激光效果。支持激光碰撞检测，光束遇到物体自动截断。

**SuperLaserProActor** - 增强版激光，支持更复杂的光束效果和多投影区域。

**SuperLaserPatternActor** - 独立激光图案 Actor。无需外部激光软件（Beyond 等），在 UE5 内直接通过 DMX 控制激光图案效果，适合简单激光演示或离线渲染。

#### LED 屏幕与投影

**SuperScreen**（原 SuperNDIScreen，继承 `ASuperMediaBase`） - 支持两种媒体源模式：NDI 视频流 / 静态纹理。接收视频并显示在 3D 模型上，支持四角梯形校正，适合异形屏幕。可绑定任意网格模型，让 LED 屏幕“贴”在任何形状的物体上。

**SuperProjector** - Projection Mapping 投影仪。使用 UE5 的 Light Function 实现纹理投影，支持四点透视校正，用于建筑投影或舞台背景。

#### 机械与特效

**SuperLiftingMachinery** - 舞台升降机械。通过 DMX 控制 6 轴运动（XYZ 位移 + 三轴旋转），可用于升降舞台、旋转平台、机械灯架等。支持绝对位置和无极旋转两种模式。

**SuperRailMachinery** - DMX 控制的导轨机械。7 个 DMX 控制轴（RailPos + PosX/Y/Z + RotX/Y/Z），适用于导轨灯具、滑轨摄像机等需要沿轨道运动的设备。

**SuperLiftMatrix** - 升降矩阵系统。多个升降组件组成矩阵阵列，四角有钢丝绳从起点上方延伸到最后一个组件，适合动态天花板、升降灯阵等效果。

**SuperDMXCamera** - DMX 控制的虚拟摄像机。6 轴运动 + FOV/光圈/对焦控制，可将画面渲染到 RenderTarget，用于 LED 大屏显示虚拟机位画面。

**SuperLightStripEffect** - LED 灯带/灯条效果。10 种内置特效（流水、追逐、呼吸等），可批量应用到多个网格模型，一个 Actor 控制整个场景的灯带。

**SuperStageVFXActor** - DMX 控制的 Niagara 粒子特效。烟雾机、CO2 喷射、彩带炮、火焰、雪花等舞台特效，通过 DMX 控制开关、颜色、生成量

#### 舞台搭建资产（StageAssets）

**一键搭建专业舞台。** StageAssets 提供 10 种程序化舞台搭建资产，所有参数实时参数化驱动，调整参数即刻重建。基于 ISM（实例化静态网格）批量渲染，性能优秀。

**桁架系列（5 种）：**

| Actor | 说明 | 特性 |
|-------|------|------|
| **SuperTruss** | 桁架龙门架 | 4 种造型（GoalPost/TShape/Portal/DoubleSpan），3 种截面（Box/Triangle/Flat），3 种规格（S290/S400/S520），DIN 4113 载荷计算 |
| **SuperCircularTruss** | 圆形/环形桁架 | 极坐标分段拼接，双环可选 + 辐射撑 + 连接法兰 |
| **SuperCurvedTruss** | 样条弧形桁架 | USplineComponent 驱动，截面始终垂直于切线 |
| **SuperTrussGrid** | 桁架网格 | 水平双层网格（灯光吊挂系统），主弦杆 + 次弦杆 + 斜撑 |
| **SuperTrussTower** | 桁架塔 | 垂直立柱结构，欧拉临界载荷计算 |

**脚手架系列（2 种）：**

| Actor | 说明 | 特性 |
|-------|------|------|
| **SuperScaffold** | 直线脚手架 | XY 网格 + 层高参数化，EN 12811 标准重量/承载/配重计算 |
| **SuperCurvedScaffold** | 样条弧形脚手架 | USplineComponent 驱动，深度方向垂直于样条切线 |

**其他舞台资产（3 种）：**

| Actor | 说明 | 特性 |
|-------|------|------|
| **SuperDrape** | 程序化幕布 | 4 种褶皱（Flat/BoxPleat/Gathered/Austrian），4 种开合（Fixed/Fly/Traveler/Tab），5 种面料规格，ProceduralMesh 生成 |
| **SuperCrowd** | 程序化人群 | 泊松圆盘采样（Bridson），闭合样条区域边界，地形射线捕捉，权重角色分配 |
| **SuperStageFloor** | 舞台地板 | 参数化舞台地板生成 |

#### 灯具库系统

**让每一盏灯都像真的一样。** 灯具库定义了灯具的所有 DMX 通道配置，决定了虚拟灯具如何响应控台信号。

**灯库 = 灯具型号**
- 每个灯库对应一种真实灯具型号（如 Martin MAC Aura、Robe Spiider 等）
- 可从 GDTF/MA 灯库导入，也可手动创建
- 一个灯具 Actor 绑定一个灯库，切换灯库 = 换灯

**多模块支持（矩阵灯/多头灯）**
- 一个灯库可包含多个"模块"，每个模块独立配址
- 适用于：LED 矩阵灯（如 Robe Robin CycFX 4）、多头灯（如 Martin MAC 101）
- 每个模块可有不同的通道偏移

**13 种属性分类**
| 分类 | 说明 |
|------|------|
| 亮度 | Dimmer、Master 等 |
| 位置 | Pan、Tilt、XYZ 等 |
| 图案 | Gobo1、Gobo2、GoboRot 等 |
| 颜色 | Red、Green、Blue、ColorWheel 等 |
| 光束 | Zoom、Iris 等 |
| 聚焦 | Focus 等 |
| 控制 | Control、Reset 等 |
| 切割 | Shaper A1-B4 等 |
| 频闪 | Strobe、StrobeSpeed 等 |
| 棱镜 | Prism1-3、PrismRot 等 |
| 雾化 | Frost 等 |
| 效果 | Effect、EffectSpeed 等 |
| 其他 | 自定义属性 |

**通道精度**
- **8-bit**：标准精度（0-255）
- **16-bit**：高精度（Coarse + Fine，0-65535）
- **24-bit**：超高精度（+ Ultra，0-16777215）

**子属性系统**

每个 DMX 属性可定义多个"子属性"，对应 MA2/MA3 的 ChannelFunction 概念：

- **频闪模式**：闭光/常亮/脉冲/随机等
- **旋转模式**：关闭/停止/位置/无极旋转
- **颜色轮槽位**：预设颜色列表 + 对应 DMX 范围
- **图案轮槽位**：图案纹理 + 对应 DMX 范围
- **棱镜槽位**：面数/半径/缩放参数

#### 蓝图扩展

**SuperStageLight 完全支持蓝图扩展。** 灯具的控制逻辑写在蓝图的 EventGraph 中，每帧调用 SuperDMXTick 事件。

**常用蓝图函数：**
| 函数 | 说明 |
|------|------|
| SetLightingIntensity | 设置亮度 |
| SetLightingStrobe | 设置频闪 |
| SetLightingColorRGB | 设置 RGB 颜色 |
| SetLightingColorWheel | 设置颜色轮 |
| SetLightingZoom | 设置变焦 |
| SetLightingFrost | 设置雾化 |
| SetBeamGoboPrism | 设置图案/棱镜 |
| SetBeamCutting | 设置切割 |
| SetEffect | 设置效果 |

**矩阵灯函数（批量控制）：**
| 函数 | 说明 |
|------|------|
| SetLightingIntensityMatrix | 矩阵亮度 |
| SetLightingColorRGBMatrix | 矩阵颜色 |
| SetMatrixColorSingle | 单像素颜色 |
| SetMatrixColorMultiple | 多像素颜色 |

#### 渲染质量调节

**根据项目需求平衡效果与性能。** 每个灯具都可独立调整渲染参数：

| 参数 | 说明 | 默认值 |
|------|------|--------|
| MaxLightIntensity | 最大亮度倍数 | 100% |
| MaxLightDistance | 光照最大距离 | 23.45 m |
| BeamFogIntensity | 雾气浓度 | 20% |
| AtmosphericDensity | 大气衰减 | 3% |
| VolumetricScattering | 体积光强度 | 0% |
| LightShadow | 阴影开关 | 关 |

**性能优化建议：**
- 不需要体积光时关闭 VolumetricScattering
- 远景灯具降低 MaxLightDistance
- 光束采样质量由系统自动管理（距离 LOD）：近景高精度、远景自动节能，无需手动调整

### 4.8 SuperTools - 编辑器工具箱

SuperTools 提供一站式编辑器工具，大幅提升灯光设计工作效率。

#### Super Stage Mode 舞台模式

类似 UE 的建模模式，Super Stage Mode 是专门用于灯光布置的编辑器模式。

**样条挂灯工具 (LightArrayTool)：**
- 选中场景中的样条线（Spline）
- 自动沿样条分布灯具
- 设置数量、间距、起止偏移
- 跟随样条旋转
- 位置/旋转偏移微调
- 支持多种灯具混合序列

**灯具阵列工具 (FixtureArrayTool)：**
- **线性阵列**：指定数量、间距，一排灯光瞬间生成
- **网格阵列**：X×Y 网格，支持蜂窝偏移
- **环形阵列**：圆形/弧形排列，可设置起止角度
- 实时预览：参数调整时立即看到效果
- 支持朝向中心、自定义旋转

#### SuperBrowser 资产浏览器

**快速放置灯具：**
- 拖拽放置：从浏览器直接拖拽灯具到场景
- 分类浏览：按制造商、类型分组
- 缩略图预览：一眼识别灯具外观
- 支持项目自定义灯具

#### 灯具库编辑器

**三级层次结构编辑：**
- **灯库级**（FixtureLibrary）：灯具元信息（名称、制造商、来源）
- **模块级**（Module）：每个模块独立配址，适用于多灯头/矩阵灯
- **属性级**（Attribute）：Coarse/Fine/Ultra 通道配置，13 种属性分类
- 可视化通道编辑：拖拽调整通道偏移和精度
- 子属性编辑：为每个属性定义 ChannelFunction（频闪模式、旋转模式、颜色轮槽位、图案轮槽位、棱镜面参数等）
- 资产缩略图：内容浏览器显示灯具预览

#### DMX 配置面板

**DMX 通讯配置中心：**
- Art-Net / sACN (E1.31) 双协议切换
- 绑定 IP / 端口配置
- Universe 管理：启用/禁用/添加/移除
- DMX 活动监视器嵌入（详见下方）
- 信号收发状态实时显示

#### NDI 配置面板

**NDI 源管理中心：**
- 自动发现网络中的 NDI 源（mDNS）
- 添加/编辑/删除输入源映射
- 逻辑名称 → 外部源名称映射
- 源匹配状态实时显示

#### DMX Patch Tool

**批量配址神器：**
- 选中灯具 → 设置起始 Universe/Address → 一键应用
- **预览模式**：应用前预览配址结果，确认无误再提交
- 自动递增：Universe/Address 自动计算，跨 Universe 自动进位
- **自然排序**：灯具按场景空间位置自然排序
- 冲突检测：地址冲突高亮显示
- 支持撤销：Ctrl+Z 回退

#### MVR 导入

**从 MVR 标准导入舞台：**
- 支持 MVR 1.0 – 1.6 版本
- 支持 MVR/ZIP 格式
- 解析 GeneralSceneDescription XML
- 自动匹配灯具类型到 SuperStage 资产
- 批量生成 Actor 到场景（保留位置/旋转/编号）
- 无法匹配的灯具类型给出警告提示

#### SuperData 同步

**多端数据同步（v2.0 协议）：**
- 连接 SuperDataServer（TCP 5966）
- 从其他客户端导入灯具数据
- 坐标转换自动处理（不同软件坐标系自动适配）
- 实时显示在线客户端列表
- 支持双向同步：SuperStage ↔ Vectorworks / MA / Unity

#### DMX → MA 导出

**导出配置到物理控台：**
- 扫描场景所有灯具，收集 Patch 信息
- **grandMA2 格式**：生成 XML 导入文件
- **grandMA3 格式**：生成 Lua 宏脚本
- 支持批量导出整个场景
- 导出后可直接导入对应版本的 GrandMA 控台

#### 图集生成器

**自动生成纹理图集：**

| 图集类型 | 分辨率 | 格式 | 说明 |
|----------|--------|------|------|
| **GOBO 图集** | 4096×256 | BGRA8 | 从灯库提取图案纹理，横向拼接 |
| **Color 图集** | 256×16 | BGRA8 | 色轮颜色纹理图集 |

- 自动命名：LTA_{灯库名}_{属性名}
- 输出到灯库同级目录
- 放置灯具时自动生成，也可手动触发更新
- 支持多种源纹理格式（PNG/TGA/EXR 等自动转换）

#### DMX 活动监视器

**实时查看 DMX 信号：**
- 灰度柱状图显示 512 通道电平
- 监视模式：单 Universe / 全活跃 Universe 切换
- 实时刷新（**100ms 周期**，约 10Hz）
- 只读显示，不影响 DMX 数据流
- **Clear Buffer**：清空当前显示缓冲（验证信号是否持续活跃）
- 嵌入在 DMX 配置面板中，无需独立窗口
- 需要有效订阅才能使用

#### LDLink 活动监视器

**无人机数据监控：**
- **接收器控制**：启用/禁用、绑定 IP、端口设置、清空数据
- **实时统计**：在线无人机数量、每秒包/字节、错误计数
- **无人机列表**：显示每架无人机的 ID、状态、位置 (X/Y/Z)、偏航角、LED 颜色
- 数据刷新周期：**200ms**
- 适用于：演出前设备检查、实时飞行监控、连接问题排查

#### 用户认证与订阅面板

**账号与权限管理：**
- 邮箱验证码登录（无密码，安全便捷）
- 套餐类型：7 天免费体验 / Pro 版 / Ultra 版 / Team 版（详见 [§12 授权与定价](#12-授权与定价)）
- 功能权限按套餐自动判定：

| 套餐 | 功能范围 |
|------|----------|
| **7 天体验** | 全功能（Ultra 权益），不可商用 |
| **Pro** | SuperStage 灯光核心 + 免费模块 |
| **Ultra** | 全部付费模块 + 免费模块 |
| **Team** | Ultra 全部 + SuperCustom 定制开发 |

- 免费模块（SuperConsole / SuperShader / SuperData）始终可用，无需订阅
- 离线缓存：7 天内离线可用
- 多设备管理：同一账号多端登录

#### 棱镜预设编辑器

独立的可视化编辑器工具，用于创建和编辑棱镜分光预设（USuperPrismPreset 数据资产）。详见 [§4.6 SuperShader - 棱镜预设系统](#46-supershader---着色器系统)。

#### Take Recorder 集成

**一键录制所有信号：**
- DMX 录制源：录制 Art-Net/sACN 到 Sequencer
- NDI 录制源：录制视频帧到 Sequencer
- Laser 录制源：录制 Beyond 激光到 Sequencer
- DroneLink 录制源：录制无人机轨迹到 Sequencer

### 4.9 SuperAI - AI 智能助手

SuperAI 是编辑器内置的 AI 智能助手模块，通过自然语言对话驱动 **37 个专用工具**，覆盖场景搭建、属性配置、材质编辑、蓝图操作、资产管理、视口控制等完整工作流。

#### 系统架构

SuperAI 由 7 个核心组件协同运作：

| 组件 | 职责 |
|------|------|
| **SSuperAIChatPanel** | Slate 聊天面板 UI（Markdown 渲染、流式输出、工具卡片） |
| **USuperAISubsystem** | 引擎级子系统，管理会话生命周期与 LLM 通信 |
| **FSuperAIChatSession** | 单个会话上下文（消息历史、工具调用记录） |
| **FSuperAIToolRegistry** | 工具注册中心，自动发现并注册全部 37 个工具 |
| **FSuperAIMCPServer** | MCP 协议 HTTP 服务器，对外暴露工具能力 |
| **FSuperAIConfig** | 配置管理（LLM 预设、API Key、模型参数） |
| **FSuperAIToolBase** | 工具基类，所有工具继承此类并实现 Execute() |

**消息处理流水线**：用户输入 → 会话拼装上下文 → HTTP 请求 LLM → SSE 流式接收 → 解析工具调用 → GameThread 执行工具 → 结果回传 LLM → 最终回复渲染到 UI

#### 聊天面板

- **打开方式**：`Window → 工具 (Tools) → SuperAI 助手` / 编辑器工具栏 SuperAI 按钮 / 状态栏 SuperAI 按钮
- **面板功能**：Markdown 富文本渲染、流式 SSE 输出、工具调用卡片可视化、多会话管理
- **多会话管理**：创建/切换/删除会话，每个会话独立维护消息历史，会话持久化存储
- **Tab ID**：`SuperAIChat`，支持停靠持久化

#### 37 个专用工具

涵盖六大类别的编辑器自动化工具：

| 类别 | 工具示例 | 说明 |
|------|----------|------|
| **场景管理** | spawn_actor, delete_actor, list_actors | Actor 增删查，批量操作 |
| **属性配置** | get_property, set_property, batch_set_property | UPROPERTY 读写，支持嵌套路径 |
| **材质编辑** | create_material_graph, add_material_node, connect_nodes | 完整材质图编辑能力 |
| **蓝图操作** | get_blueprint_info, add_component | 蓝图信息查询与组件管理 |
| **资产管理** | list_super_assets, import_asset | 灯具库浏览与资产导入 |
| **视口控制** | set_viewport_camera, take_screenshot | 摄像机控制与截图 |

所有工具均自描述 JSON Schema，支持参数校验和错误提示。

#### 多 LLM 预设

配置文件存储在 `AppData/Local/SuperStage/SuperAI.sav`（二进制格式，v2 多预设架构），支持多个 LLM 服务预设自由切换：

| 预设字段 | 说明 |
|----------|------|
| PresetName | 预设名称（如 "OpenAI GPT-4o"、"DeepSeek"） |
| Endpoint | API 端点 URL（OpenAI 兼容格式） |
| ApiKey | API 密钥（磁盘 XOR 混淆存储） |
| Model | 模型名称 |
| Temperature | 采样温度（0.0 - 2.0，默认 0.7） |
| MaxTokens | 最大输出 Token 数（默认 4096） |
| bEnableStreaming | SSE 流式输出开关（默认开启） |

#### MCP 协议服务器

SuperAI 内置 MCP（Model Context Protocol）HTTP 服务器，允许外部 AI 客户端（Claude Desktop、Cursor 等）通过标准 MCP 协议调用全部 37 个工具。

- **启动方式**：聊天面板状态栏 MCP 开关按钮
- **通信方式**：Streamable HTTP（POST `/mcp` JSON-RPC 2.0，端口 **13090**）
- **协议端点**：
  - `POST /mcp` — JSON-RPC 2.0 主端点（initialize / tools/list / tools/call / ping）
  - `GET /mcp/tools` — REST 兼容：列出所有工具
  - `POST /mcp/tool/{name}` — REST 兼容：执行指定工具
  - `GET /mcp/status` — 服务器状态
- **安全控制**：手动启停，不使用时不占用端口和资源

---

## 5. SuperData - 跨平台数据同步

SuperData 是一个独立产品，但与 SuperStage 插件紧密集成。它实现了 **舞台灯具数据在不同软件之间的实时同步**，让灯光设计师可以在不同工具之间无缝协作。

### 5.1 产品概述

**打破软件壁垒。** 舞台设计往往需要多个软件协同工作：
- **Vectorworks Spotlight** - 灯光布局设计
- **GrandMA2/MA3** - 灯光控台编程
- **Unreal Engine** - 3D 可视化预演
- **Unity** - 实时渲染

传统工作流中，设计师需要在每个软件中重复输入灯具位置、DMX 地址等数据。SuperData 让这一切自动化——**在一处修改，处处同步**。

### 5.2 工作原理

#### 群聊架构

SuperData 采用 **"群聊"架构**：

1. **中央服务器 (SuperDataServer.exe)**
   - 运行在本地（127.0.0.1:5966）
   - 管理所有客户端连接
   - 转发灯具数据
   - 自动启动（首次连接时由客户端唤起）

2. **客户端**
   - 各软件的 SuperData 插件
   - 连接到中央服务器
   - 发送/接收灯具数据

3. **数据流**
   ```
   Vectorworks ←→ SuperDataServer ←→ Unreal Engine
                        ↑
                        ↓
                   GrandMA2
   ```

#### 同步数据内容

每个灯具包含以下同步字段：

| 字段 | 说明 |
|------|------|
| uuid | 唯一标识符（跨平台追踪同一盏灯） |
| name | 显示名称 |
| fixtureType | 灯具型号（如 "Martin MAC Aura"） |
| universe | DMX Universe (1-256) |
| startAddress | DMX 起始地址 (1-512) |
| fixtureID | 灯具编号 |
| position | 位置坐标 (X, Y, Z) - 厘米 |
| rotation | 旋转角度 (Pitch, Yaw, Roll) - 度 |
| scale | 缩放 |
| channelSpan | DMX 通道数 |

#### 坐标系转换

不同软件使用不同坐标系，SuperData 自动处理转换：

| 软件 | 坐标系 | 单位 |
|------|--------|------|
| Vectorworks | 右手 Z-up | mm |
| Unreal Engine | 左手 Z-up | cm |
| Unity | 左手 Y-up | m |
| Standard (传输) | 右手 Z-up | cm |

### 5.3 支持的平台

#### Unreal Engine (SuperStage 插件内置)

**SuperData Sync 面板：**
- 编辑器菜单 → SuperStage → SuperData Sync
- 一键连接到 SuperData 网络
- 显示所有在线客户端（Unity/VW/MA 等）
- 选择源客户端 → Fetch Data → 导入灯具
- 类型映射：将源灯具型号匹配到本地 SuperStage 资产
- 坐标自动转换

**导入功能：**
- 自动创建灯具 Actor
- 自动配置 DMX 地址
- 自动设置位置/旋转
- 支持增量同步（只更新变化的灯具）

#### Vectorworks Spotlight

**SuperStageForVw 插件：**
- Python 实现
- 读取 Vectorworks 中的 Lighting Device 数据
- 提取位置（3D）、旋转、DMX 地址、灯具型号
- 发送到 SuperData 网络
- 支持双向同步

**操作流程：**
1. 在 Vectorworks 中完成灯位图设计
2. 运行 SuperStageForVw
3. 插件自动扫描所有 Lighting Device
4. 发送灯具列表到 SuperData 服务器
5. Unreal Engine 端接收并生成灯具

#### GrandMA2

**SuperData.lua 插件：**
- Lua 实现
- 读取 Fixture Layer 中的灯具信息
- 提取 Channel、Patch Address、Fixture Type
- 发送到 SuperData 网络

**操作流程：**
1. 在 MA2 中完成 Patch
2. 运行 `Plugin 1`（SuperData）
3. 插件扫描 Fixture Layer
4. 发送灯具列表到 SuperData 服务器
5. 其他客户端接收灯具数据

**典型应用场景：**
- 从 MA2 导入灯具配置到 UE5
- 控台 Patch 变更自动同步到可视化

#### Unity (SuperStageForUnity 插件)

**SuperData 数据共享工具：**
- 菜单 SuperStage → 工具 → SuperData 数据共享
- 双模式界面：导入（Import）/ 导出（Export）
- 连接状态实时显示
- 在线客户端列表（显示平台图标）

**导入功能：**
- 选择源客户端（VW/UE/MA）
- Fetch Data 获取灯具列表
- 按灯具类型分组显示
- 类型映射：源型号 → 本地 Prefab
- 批量勾选/取消
- 一键导入生成 GameObject
- 自动配置 DMX 地址和位置

**导出功能：**
- 扫描场景中的 SuperDMX 灯具
- 自动提取位置/旋转/DMX 配置
- 发送到 SuperData 网络
- 其他客户端可接收

**技术实现：**
- `SuperDataService` - 单例服务管理器
- `SuperDataClient` - TCP 客户端（线程安全）
- `SuperDataTypes` - 协议数据类型
- 完整实现 SuperData Protocol v2.0

### 5.4 协议规范

**SuperData Protocol v2.0**

| 参数 | 值 |
|------|-----|
| 协议版本 | 2.0.0 |
| 端口 | TCP 5966 |
| 魔数 | "SPDT" |
| 包头大小 | 24 字节 |
| 负载格式 | JSON (UTF-8) |

**数据包类型：**

| 类型码 | 名称 | 说明 |
|--------|------|------|
| 0x0010 | Connect | 客户端连接请求 |
| 0x0011 | ConnectAck | 服务器响应（含其他客户端列表） |
| 0x0012 | Disconnect | 断开连接 |
| 0x0013 | Heartbeat | 心跳（3 秒间隔） |
| 0x0014 | ClientJoined | 新客户端加入通知 |
| 0x0015 | ClientLeft | 客户端离开通知 |
| 0x0020 | FixtureListRequest | 请求灯具列表 |
| 0x0021 | FixtureListResponse | 灯具列表响应 |
| 0x0022 | FixtureUpdate | 灯具增量更新 |
| 0x0023 | FixtureFullSync | 灯具全量同步 |
| 0x0024 | FixtureDelete | 删除灯具 |

### 5.5 典型工作流

#### 场景 1：从 Vectorworks 导入到 UE5

1. **VW 设计师**：在 Vectorworks Spotlight 中完成灯位图
2. **VW 设计师**：运行 SuperStageForVw 插件
3. **VW 设计师**：点击 "Sync" 发送灯具数据
4. **UE 可视化师**：打开 SuperData Sync 面板
5. **UE 可视化师**：看到 Vectorworks 客户端在线
6. **UE 可视化师**：点击 "Fetch Data" 获取灯具
7. **UE 可视化师**：配置类型映射（VW 型号 → SuperStage 资产）
8. **UE 可视化师**：点击 "Import" 生成灯具

#### 场景 2：从 MA2 导入 Patch

1. **灯光师**：在 GrandMA2 中完成 Patch 配置
2. **灯光师**：运行 SuperData 插件
3. **灯光师**：插件自动上传灯具数据
4. **UE 可视化师**：接收并导入灯具
5. **结果**：UE 中的灯具自动具有正确的 DMX 地址

#### 场景 3：实时协作

多个软件同时连接到 SuperData：
- VW 设计师修改灯位 → 自动同步到 UE5
- UE 可视化师预演效果 → 反馈给设计师
- MA 灯光师调整 Patch → 所有端口自动更新

### 5.6 安装与配置

#### 中央服务器

SuperDataServer.exe 通常由客户端自动启动，无需手动配置。

**手动启动（可选）：**
```
SuperDataServer.exe --port 5966 --verbose
```

**监控界面：**
服务器运行时显示实时日志：
- 客户端连接/断开
- 数据包收发统计
- 错误信息

#### Unreal Engine

已内置于 SuperStage 插件，无需额外安装。

#### Vectorworks

1. 将 `SuperStageForVw` 文件夹复制到 VW 插件目录
2. 重启 Vectorworks
3. 在菜单中找到 SuperStageForVw

#### GrandMA2

1. 将 `SuperData.lua` 复制到 MA2 插件目录
2. 重启 MA2 或重新加载插件
3. 使用 `Plugin 1` 命令运行

---

## 6. LimxDroneStudio - 无人机编队软件

> **授权说明**：LimxDroneStudio 对 SuperStage 付费用户免费开放使用。

LimxDroneStudio 是 LimxTeam 自研的 **专业无人机集群编排与仿真软件**，采用 Rust 语言开发，是整个无人机表演系统的"大脑"。

### 6.1 产品概述

**一站式无人机编队设计。** 从创意设计到实飞执行，LimxDroneStudio 覆盖全流程：

```
┌─────────────────┐     LDLink (UDP)    ┌─────────────────┐
│ LimxDroneStudio │ ──────────────────▶ │   UE5 渲染器    │
│   (编排大脑)     │                     │  (高画质预览)   │
└────────┬────────┘                     └─────────────────┘
         │
         │ MAVLink v2.0
         ▼
┌─────────────────┐
│   DSS 地面站    │
│   (实飞执行)    │
└─────────────────┘
```

**目标用户：**
- 专业无人机表演团队
- 编队设计师
- 大型活动技术总监

**平台支持：** Windows 10/11 (x64), Linux (Ubuntu 22.04+)

### 6.2 资产管理

**3D 模型导入：**
- 支持格式：OBJ / PLY / FBX
- 自动解析网格和纹理

**点云采样算法：**
| 算法 | 特点 | 适用场景 |
|------|------|----------|
| **Poisson Disk** | 均匀分布，保证最小间距 | 密集编队，视觉均匀 |
| **Vertex Snapping** | 精确顶点位置 | 几何边缘，精确轮廓 |
| **Face Center** | 面中心采样 | 均匀覆盖 |
| **Random** | 随机采样 | 快速预览 |

**矢量图导入：** SVG / AI (Illustrator) 路径

**形状生成器：**
- 基础 3D 形状：立方体、球体、圆柱、金字塔、圆环、螺旋
- 2D 形状：圆形、心形、星形
- 阵列生成：网格阵列、圆形阵列
- 自定义路径点

### 6.3 时间轴系统

**UE Sequencer 风格的多轨道编辑器：**

```
时间轴示例:
├─[00:00]─────────[00:10]─────────[00:20]─────────[00:30]─┤
│  立方体          球体            心形           文字    │
│    ↓              ↓               ↓              ↓     │
│  Shape A ───▶ Shape B ───▶ Shape C ───▶ Shape D        │
│         Linear    Ease-Out    Cubic                    │
└────────────────────────────────────────────────────────┘
```

**轨道类型：**
- **主轨道** - 无人机位置/形态
- **灯光轨道** - RGB 颜色动画
- **音频轨道** - 波形可视化 + 节拍标记
- **标记轨道** - 场次/段落注释

**关键帧动画：**
- 32 种缓动曲线（Linear/Ease-In/Ease-Out/Cubic Bezier 等）
- 特效片段：拖拽、调整时长、淡入淡出
- 框选批量操作
- 吸附网格

### 6.4 解算引擎

**这是 LimxDroneStudio 的核心竞争力。**

#### 智能配对 (Assignment Problem)

给定 Source Shape (N点) 和 Target Shape (N点)，求最优一一映射，使总飞行距离最小且无交叉。

| 算法 | 时间复杂度 | 适用规模 |
|------|-----------|----------|
| **Jonker-Volgenant** | O(n³) | n ≤ 5,000 (精确解) |
| **Auction Algorithm** | O(n²·log(nC)) | n ≤ 10,000 (近似解) |

> 默认 Jonker-Volgenant；超 5000 点自动切换 Auction Algorithm。

#### 轨迹生成

| 曲线类型 | 连续性 | 特点 |
|----------|--------|------|
| **Cubic Bezier** | C¹ | 控制点自动生成，计算快 |
| **B-Spline (Uniform)** | C² | 更平滑，适合复杂路径 |

#### 4D 避障系统

**检测阶段：**
1. 时间轴离散化 (Δt = 33ms @ 30FPS)
2. 空间八叉树 (Octree) 构建
3. 距离阈值检测（默认安全距离：**2.0m**）

**规避策略：**
| 策略 | 说明 | 优先级 |
|------|------|--------|
| **垂直偏移** | 冲突轨迹抬高 Z 轴 | 高 |
| **时间偏移** | 延后启动时间 | 中 |
| **路径弯曲** | 插入中间航点绕行 | 低 |

### 6.5 特效系统

**形态特效：**
- 旋转、缩放、波浪、螺旋

**LED 特效：**
- 跑马灯、RGB 渐变、呼吸灯、彩虹

**特效库管理：**
- 创建、编辑、保存预设
- 拖拽应用到时间轴

### 6.6 无人机管理

- 批量添加/删除
- 分组管理（颜色标识、锁定、可见性）
- 快速选择：每 N 个 / 范围 / 随机 / 反选
- 实时状态显示（在线/离线/飞行中/已解锁）

**控制命令：**
- 解锁 / 锁定
- 起飞 / 降落
- 返航
- 紧急停止
- LED 颜色/亮度控制

### 6.7 网络输出

**输出方式：**
| 输出类型 | 协议/格式 | 目标系统 |
|----------|----------|----------|
| **实时预览** | egui + three-d | 本地 3D 视口 |
| **UE5 推流** | LDLink (UDP) | SuperDrone 模块 |
| **实飞执行** | MAVLink v2.0 | QGC/DSS 地面站 |
| **离线导出** | .csv, .waypoints | 离线上传 |

**LDLink 协议 (v1.0)：**

| 字段 | 大小 | 说明 |
|------|------|------|
| Magic | 4字节 | "LDLK" |
| Version | 1字节 | 协议版本 |
| OpCode | 1字节 | 操作码 |
| Universe | 2字节 | Universe 编号 |
| Sequence | 1字节 | 序列号 |
| Length | 2字节 | 数据长度 |
| Payload | 可变 | 数据载荷 |

**操作码：**
| OpCode | 名称 | 说明 |
|--------|------|------|
| 0x10 | LedData | LED 颜色数据 |
| 0x20 | PositionData | 位置坐标 |
| 0x30 | StateData | 完整状态（位置+颜色） |
| 0x40 | Sync | 帧同步包 |
| 0x70 | Command | 控制命令 |

**寻址：** DroneId = Universe × 256 + Channel（支持 65,535 台无人机）

### 6.8 性能指标

| 无人机数量 | 目标帧率 | GPU 占用 |
|-----------|---------|---------|
| 1,000 | 60 FPS | < 10% |
| 10,000 | 60 FPS | < 40% |
| 50,000 | 30 FPS | < 80% |

---

## 7. 技术规格

为增强文档的"硬核"属性，以下数据表以工业级标准呈现核心指标。

### 7.1 系统要求

**软件要求：**
| 组件 | 要求 |
|------|------|
| **引擎版本** | Unreal Engine 5.6+ |
| **操作系统** | Windows 10/11 64-bit |

**硬件配置：**
| 规模 | CPU | GPU | 内存 | 说明 |
|------|-----|-----|------|------|
| **小型** (<50盏灯) | i5/R5 | GTX 1660 | 16GB | 基础预演 |
| **中型** (50-200盏) | i7/R7 | RTX 3060 | 32GB | 专业预演 |
| **大型** (200-500盏) | i9/R9 | RTX 4070+ | 64GB | 复杂场景 |
| **超大型** (激光+像素) | Xeon/TR | RTX 4090+ | 128GB | 顶级项目 |

### 7.2 核心性能指标

| 核心指标 | 参数规格 | 竞品对比优势 |
|---------|---------|-------------|
| **DMX 处理能力** | 100+ Universe (51,200+ 通道) @ 60fps | 远超一般 UE 插件的单线程瓶颈 |
| **DMX 精度** | 8/16/24-bit 可配置 | 24-bit 消除长焦镜头锯齿 |
| **DMX 延迟** | < 16ms（一帧内响应） | 确定性调度 |
| **时序抖动** | < 1ms | 工业级稳定 |
| **通讯协议** | Art-Net 4, sACN, OSC, UDP (Beyond), MIDI, Timecode (LTC/MTC) | 全协议栈，无须第三方转换器 |
| **激光同步延迟** | < 1 帧 (16.6ms @ 60fps) | 硬件级同步 |
| **激光点云压缩** | 71.4% (28→8 字节/点) | 独家压缩算法 |
| **NDI 录制规格** | 支持 Alpha 通道，最高 8K，同步写入 Sequencer | 独家功能，支持后期高画质重渲染 |
| **无人机渲染** | 50,000+ 架次 @ 30fps | HISM 批量优化 |

### 7.3 灯库兼容性

| 标准 | 支持程度 |
|------|---------|
| **GDTF** | 完整导入，子属性系统还原 |
| **MA2 Fixture Library** | 原生兼容 |
| **MA3 Fixture Library** | 原生兼容 |
| **自定义灯库** | 可视化编辑器创建 |

### 7.4 网络端口

| 协议 | 端口 | 类型 | 用途 |
|------|------|------|------|
| Art-Net | 6454 | UDP | DMX 收发 |
| sACN | 5568 | UDP | DMX 收发（多播 239.255.x.y） |
| Beyond | 5568 | UDP | 激光点云数据 |
| NDI | 动态 | TCP/UDP | 视频流 |
| LDLink | 14555 | UDP | 无人机数据（LimxDroneStudio） |
| SuperData | 5966 | TCP | 跨平台数据同步 |
| MCP | 13090 | HTTP | SuperAI MCP 服务器 |

### 7.5 数据精度规格

| 数据类型 | 精度 | 说明 |
|---------|------|------|
| **DMX 通道** | 8/16/24-bit | Coarse/Fine/Ultra 组合 |
| **坐标精度** | 32-bit Float | 世界坐标 |
| **激光位置** | int16 量化 | [-1, 1] 映射，0.003% 精度损失 |
| **激光颜色** | uint8 量化 | 0.4% 精度损失 |
| **NDI 帧时间** | FFrameNumber | Sequencer 帧级同步 |

---

## 8. 安装指南

### 8.1 系统要求

**软件要求：**

| 组件 | 要求 |
|------|------|
| **引擎版本** | Unreal Engine 5.6 / 5.7 |
| **操作系统** | Windows 10 / 11 (64-bit) |

**硬件配置：**

| 规模 | CPU | GPU | 内存 | 说明 |
|------|-----|-----|------|------|
| **小型** (<50盏灯) | i5/R5 | GTX 1660 | 16GB | 基础预演 |
| **中型** (50-200盏) | i7/R7 | RTX 3060 | 32GB | 专业预演 |
| **大型** (200-500盏) | i9/R9 | RTX 4070+ | 64GB | 复杂场景 |
| **超大型** (激光+像素) | Xeon/TR | RTX 4090+ | 128GB | 顶级项目 |

### 8.2 安装步骤

SuperStage 提供向导式安装程序，自动检测已安装的 Unreal Engine 版本。

**步骤 1：运行安装程序**

双击 `SuperStageInstaller.exe` 启动安装向导。

**步骤 2：查看更新历史**

安装向导会显示当前版本的更新内容，了解新功能和修复。

**步骤 3：选择目标版本**

安装程序自动检测系统中已安装的 UE 版本：
- 勾选需要安装的版本（支持多选）
- 灰色选项表示该版本未安装

> **检测路径**：安装程序从 Windows 注册表读取 UE 安装位置：
> `HKEY_LOCAL_MACHINE\SOFTWARE\EpicGames\Unreal Engine\{版本号}`

**步骤 4：阅读并同意用户协议**

仔细阅读《SuperStage 用户许可协议》，勾选同意后继续。

**步骤 5：等待安装完成**

安装程序将插件复制到以下目录：
```
{UE安装路径}\Engine\Plugins\Marketplace\SuperStage\
```

安装完成后，点击"完成"关闭向导。

### 8.3 授权激活

首次启动 Unreal Engine 并加载 SuperStage 时，通过工具栏 SuperStage 下拉菜单 → PluginAuth 打开用户认证窗口：

1. **输入邮箱**：在 Email 输入框填写您的邮箱地址
2. **获取验证码**：点击 "Get Code" 按钮，6位验证码将发送到您的邮箱
3. **登录**：输入验证码后点击 "Sign In" 登录（未注册用户将自动创建账号）
4. **查看订阅**：登录成功后可查看当前订阅状态和到期时间

> **注意**：验证码 60 秒内有效，超时需重新获取。如遇问题请联系 yerrkj@outlook.com。

### 8.4 验证安装

安装成功后，在 Unreal Engine 中确认以下内容：

| 检查项 | 预期结果 |
|-------|--------|
| **菜单栏** | 出现 "SuperStage" 顶级菜单 |
| **资产浏览器** | SuperStage → SuperBrowser 可打开资产管理器 |
| **插件管理器** | Edit → Plugins → 搜索 "SuperStage" 已启用 |
| **放置灯具** | 通过 SuperBrowser 拖拽灯具到场景，支持分组/厂商筛选 |
| **状态栏** | 底部状态栏出现 SuperDMX / SuperNDI / LDLink 按钮 |

### 8.5 卸载与更新

**卸载插件：**
1. 关闭 Unreal Engine
2. 删除目录：`{UE安装路径}\Engine\Plugins\Marketplace\SuperStage\`
3. 重启 UE 编辑器

**更新插件：**
1. 运行新版本安装程序
2. 安装程序会自动覆盖旧版本
3. 首次启动可能需要重新激活授权

---

## 9. 快速入门

### 9.1 5分钟：放置灯具并配置 DMX

**目标**：在 UE5 中放置灯具并配置 DMX 地址。

**步骤：**

1. **创建新项目**
   - 启动 UE5，创建空白项目
   - 确保 SuperStage 插件已启用（Edit → Plugins → 搜索 "SuperStage"）

2. **放置灯具**
   - 工具栏 SuperStage 下拉菜单 → **SuperBrowser** 打开资产浏览器
   - 在左侧分类树中选择灯具类别或厂商
   - 拖拽灯具到场景中，调整位置

3. **配置 DMX 地址**
   - 选中灯具，打开 Details 面板
   - 设置：`Universe = 1`，`StartAddress = 1`

4. **配置 DMX 输出**
   - 点击底部状态栏 **SuperDMX** 按钮打开 DMX 设置面板
   - 配置 Art-Net/sACN 输出参数

5. **使用配接工具**
   - SuperStage 下拉菜单 → SuperDMXTool → **PatchTool** 打开配接工具
   - 可批量管理场景中的灯具 DMX 地址

### 9.2 10分钟：连接物理控台

**目标**：使用 GrandMA2 等物理控台控制 SuperStage 中的虚拟灯具。

**步骤：**

1. **网络配置**
   - 确保 PC 和控台在同一网段
   - Art-Net 推荐使用 `2.x.x.x` 网段

2. **配置 DMX 接收**
   - 点击底部状态栏 **SuperDMX** 按钮打开设置面板
   - 配置 Art-Net 接收参数

3. **控台端配置**
   - 在 MA2 上 Patch 灯具
   - 配置 Art-Net 输出节点
   - 确保 Universe 映射与 SuperStage 灯具一致

4. **验证连接**
   - 在控台上推亮灯具
   - SuperStage 场景中的虚拟灯具同步亮起

### 9.3 15分钟：Arena NDI 连接与渲染

**目标**：将 Resolume Arena 的 NDI 输出投射到 UE5 场景中的屏幕模型，并录制为离线渲染。

**前置要求**：已安装 Resolume Arena 软件

**步骤：**

1. **创建屏幕模型**
   - 在 UE5 中使用建模模式 (Modeling Mode) 创建屏幕几何体
   - 根据需求划分屏幕区域（左屏、中屏、右屏等）

2. **配置 Arena NDI 输出**
   - 打开 Arena，启用 NDI 输出功能
   - 进入 Advanced Output，添加屏幕（例如 3 个屏幕）
   - 将每个屏幕的输出设备 (Device) 切换为 **NDI**
   - 分配图层：屏幕1 → 图层1，屏幕2 → 图层2，依此类推

3. **UE5 NDI 接收设置**
   - 点击底部状态栏 **SuperNDI** 按钮打开 NDI 设置面板
   - 添加输入源 (Inputs)，选择 Arena 输出的 Screen 1、Screen 2、Screen 3
   - 在 Content Browser 找到 **SuperScreen** 蓝图资产，拖入场景
   - 添加"屏幕网格" (Screen Grid)，用吸管工具绑定对应的 3D 屏幕模型

4. **录制 NDI 信号**
   - 打开 Take Recorder，添加 **NDI Source**
   - 添加需要录制的输入源轨道
   - 设置：目标帧率 30fps，最大时长 900秒
   - **重要**：录制时必须将 Arena 切换到前台，否则后台运行会导致卡顿
   - 点击录制 → Arena 播放 → 完成后停止

5. **离线渲染**
   - 关闭 Arena（已脱机）
   - 创建 Level Sequence，添加子序列，拖入录制的 NDI 数据
   - 添加摄像机，使用 Movie Render Queue 输出 MP4 (30fps)

---

### 9.4 15分钟：录制激光到 Sequencer

**目标**：将 Pangolin Beyond 的激光数据录制到 UE Sequencer，实现离线渲染。

**前置要求**：已安装 Pangolin Beyond 软件

**步骤：**

1. **配置 Beyond**
   - 打开 Beyond，进入“投影区域”设置，删除所有默认区域
   - 添加 4 个投影区域，分别设置 Fixture Number = 1, 2, 3, 4
   - 菜单 → 查看 (View) → 勾选“启用外部可视化输出”

2. **放置激光 Actor**
   - 通过 SuperBrowser 或 Content Browser 拖拽激光灯具到场景
   - 放置 4 个激光灯具，分别设置 DeviceID = 1, 2, 3, 4
   - DeviceID 与 Beyond Fixture Number 一一对应

3. **验证实时连接**
   - 关闭定向光源以便观察激光效果
   - 在 Beyond 中播放节目格 (Cue)，检查 UE 中对应激光灯是否亮起

4. **打开 Take Recorder**
   - 菜单 → Window → Cinematics → Take Recorder
   - 点击 "+ Source" → 添加“激光输入源”
   - 展开该输入源，添加 4 个轨道（ID 1, 2, 3, 4）

5. **开始录制**
   - 将 Beyond 时间线回到开头，设置为闭光状态
   - 在 UE 点击录制按钮，等待倒计时结束
   - 同时在 Beyond 中点击播放时间线
   - 播放结束后，在 UE 点击停止录制

6. **离线渲染**
   - 关闭实时连接（已拥有录制数据）
   - 新建 Level Sequence，添加子序列轨道，拖入录制的激光数据
   - 添加摄像机并调整视角
   - 打开影片渲染队列 (Movie Render Queue)，输出 MP4 (1080p, 30fps)

---

## 10. 常见问题与故障排除

### 10.1 安装问题

**Q：安装程序检测不到 UE 版本**

A：手动检查注册表路径是否存在：
```
HKEY_LOCAL_MACHINE\SOFTWARE\EpicGames\Unreal Engine\5.6
```
如使用 Epic Games Launcher 安装，路径应自动写入。源码编译版本需手动安装。

**Q：安装后 UE 中看不到 SuperStage 菜单**

A：
1. 检查 Edit → Plugins → 搜索 "SuperStage" 是否已启用
2. 重启 UE 编辑器
3. 检查 Output Log 是否有插件加载错误

### 10.2 DMX 问题

**Q：DMX 无法发送到实体灯具**

A：
1. 检查网络适配器 IP 是否在 `2.x.x.x` 网段（Art-Net 要求）
2. 确认防火墙未阻止 UDP 6454 端口
3. 点击状态栏 **SuperDMX** → 确认 DMX 输出模式已配置
4. 使用 DMX 活动监视器确认有数据输出

**Q：从控台接收 DMX 无反应**

A：
1. 点击状态栏 **SuperDMX** 确认接收模式已配置
2. 检查 Universe 映射是否一致
3. 确认灯具 DMX 地址与控台 Patch 一致

### 10.3 激光问题

**Q：Beyond 激光不显示**

A：
1. 确认 Beyond 已启用"外部可视化输出"
2. 检查 DeviceID 与 Beyond Fixture Number 是否一致
3. 确认 UDP 5568 端口未被防火墙阻止
4. 检查 Beyond 投影区域是否已配置 Fixture Number

**Q：激光录制后回放为空**

A：
1. 确认录制时 Beyond 正在播放内容
2. 检查 Take Recorder 是否添加了正确的 Device 轨道
3. 查看 Sequencer 中激光轨道是否有关键帧数据

### 10.4 NDI 问题

**Q：NDI 源找不到**

A：
1. 确认 NDI 源与 UE 在同一局域网
2. 等待 3-5 秒让 mDNS 发现生效
3. 检查 NDI 源名称是否包含特殊字符
4. 尝试重启 NDI 发送端

**Q：NDI 视频卡顿或丢帧**

A：
1. 降低视频分辨率（推荐 1080p）
2. 确保网络带宽充足（千兆网络）
3. 录制时将 Arena/Resolume 切换到前台

### 10.5 性能问题

**Q：帧率不稳定**

A：
1. 关闭不必要的 VolumetricScattering
2. 减少激光点云采样密度
3. 大场景使用 LOD 分组策略
4. 光束采样质量已由系统自动 LOD 管理，无需手动干预

---

## 11. 已知限制与版本兼容性

### 11.1 已知限制

| 模块 | 限制说明 |
|------|---------|
| **激光设备** | 单实例最多 4 台 Beyond 设备（DeviceID 1-4） |
| **NDI 分辨率** | 超过 4K 分辨率可能出现丢帧 |
| **无人机数量** | 超过 10,000 架建议降至 30fps |
| **DMX Universe** | 最多 100+ Universe（受网络带宽限制） |
| **操作系统** | 仅支持 Windows 10/11，暂不支持 macOS/Linux |
| **引擎版本** | 仅支持 UE 5.6 及以上版本 |

### 11.2 版本兼容性

| SuperStage 版本 | UE 版本 | 状态 |
|----------------|---------|------|
| **26Q2.5** | 5.6 / 5.7 | ✅ 当前稳定版 |

### 11.3 升级注意事项

- **从 25Q4 升级**：需重新导入灯库资产，Show 文件格式已更新
- **跨大版本升级**：首次打开旧版 `.ssshow` 文件会自动迁移格式
- **备份建议**：升级前备份项目目录和 Show 文件

### 11.4 外部依赖

| 功能模块 | 依赖项 | 获取方式 |
|---------|-------|---------|
| Beyond 激光 | linetD2_x64.dll, matrix64.dll | Pangolin Beyond 安装目录 |
| NDI 视频 | Processing.NDI.Lib.x64.dll | NDI Tools 安装后自动包含 |
| Art-Net/sACN | 无额外依赖 | 内置支持 |

---

## 12. 授权与定价

SuperStage 采用 **三档付费套餐 + 免费体验** 的季度订阅制定价模型。

### 12.1 产品模块划分

**付费模块（需订阅授权）：**

| 模块 | 功能描述 |
|------|----------|
| **SuperStage** | 舞台灯光核心——DMX 双向 Art-Net/sACN，100+ Universe，8/16/24bit 精度，专业电脑灯完整模拟 |
| **SuperLaser** | 激光编程系统——Beyond(Pangolin) UDP 接入，71.4% 点云压缩，扫描仪物理模拟 |
| **SuperNdi** | NDI 视频接入——视频流接收/录制/回放，Alpha 通道，Sequencer 集成 |
| **SuperDrone** | 无人机编队控制——LDLink 协议接入，HISM 万级渲染，GPS 坐标转换 |
| **SuperCAD** | 施工图绘制——UE 内直接绘图，DXF/PDF/PNG 导出，BOM 统计 |
| **SuperCustom** | 定制开发能力——开放集成接口，支持第三方系统对接（**Team 版专属**） |

**永久免费模块（所有用户始终可用）：**

| 模块 | 功能描述 |
|------|----------|
| **SuperConsole** | 内置专业灯光控台，CUE/Preset/Effects/Timeline，.ssshow 格式 |
| **SuperShader** | VFX 渲染——光束/雾气/丁达尔效果，频闪波形，棱镜预设 |
| **SuperData** | 跨平台数据同步——支持 UE/Unity/Vectorworks/GrandMA2 |

### 12.2 套餐体系

| 功能 | 7 天体验 | Pro 版 | Ultra 版 | Team 版 |
|------|:-------:|:------:|:--------:|:-------:|
| SuperConsole（DMX 控台） | 免费 | 免费 | 免费 | 免费 |
| SuperShader（VFX 视效） | 免费 | 免费 | 免费 | 免费 |
| SuperData（跨平台同步） | 免费 | 免费 | 免费 | 免费 |
| SuperStage（舞台灯光核心） | ✓ | ✓ | ✓ | ✓ |
| SuperLaser（激光系统） | ✓ | — | ✓ | ✓ |
| SuperNdi（NDI 视频） | ✓ | — | ✓ | ✓ |
| SuperDrone（无人机编队） | ✓ | — | ✓ | ✓ |
| SuperCAD（施工图绘制） | ✓ | — | ✓ | ✓ |
| SuperCustom（定制开发能力） | — | — | — | ✓ |
| **商业项目使用权** | 仅学习测试 | ✓ | ✓ | ✓ |
| **授权时长** | 7 天 | 季度自选 | 季度自选 | 自定义 |
| **使用席位** | 1 | 1 | 1 | 自定义 |
| **增值税发票 + 合同** | — | — | — | ✓ |
| **优先技术支持** | — | — | — | ✓ |
| **专属技术顾问** | — | — | — | ✓ |

### 12.3 各版本说明

**7 天免费体验**
- 注册即赠，与 Ultra 版功能完全相同
- 用于评估和测试，**不可用于商业项目**
- 时长固定 7 天，不可延长

**Pro 版**
- 专为只需核心灯光编程的独立创作者设计
- 包含 SuperStage 灯光核心模块，不含激光/NDI/无人机/施工图
- 季度订阅制（90 天/季度），支持多季度选购

**Ultra 版**
- 包含全部付费功能模块，适合专业灯光设计师、视效导演和演出公司
- 季度订阅制（90 天/季度），支持多季度选购

**Team 版**
- 包含 Ultra 全部功能 + **定制开发能力**（SuperCustom）
- 多席位授权（≥6 席位），根据团队规模定制
- 增值税专用发票 + 正式合同 + 专属技术顾问
- **联系报价**：yunsio@yunsio.com

### 12.4 定价方式

以季度（90 天）为基本订阅单位，3 季度起享「买 X 送 Y」优惠：

| 购买量 | 赠送量 | 实得天数 |
|:------:|:------:|--------:|
| 1 季度 | — | 90 天 |
| 2 季度 | — | 180 天 |
| 买 3 送 1 | +1 季度 | 360 天 |
| 买 4 送 3 | +3 季度 | 630 天 |
| 买 8 送 6 | +6 季度 | 1,260 天 |
| 买 12 送 8 | +8 季度 | 1,800 天 |

**具体价格请访问**：https://yunsio.com/pricing

### 12.5 订阅规则

| 场景 | 规则 |
|------|------|
| **续费（同档次）** | 到期时间在现有基础上累加延长 |
| **升级（Pro → Ultra）** | 剩余天数按比例折算，从当前时间重新计算到期日 |
| **到期前 7 天** | 系统提醒续费 |
| **到期后** | 软件进入只读模式，项目文件/灯光库/配置完全保留，续费后即刻恢复 |

### 12.6 支付方式

| 支付方式 | 适用范围 |
|----------|----------|
| 微信支付 | 国内个人/企业 |
| 支付宝 | 国内个人/企业 |
| 银行转账 / 对公转账 | Team 版 / 企业客户 |
| PayPal | 境外用户 |

---

## 13. 技术支持

### 13.1 官方渠道

| 渠道 | 链接/方式 |
|------|----------|
| **官方网站** | https://yunsio.com |
| **定价页面** | https://yunsio.com/pricing |
| **用户协议** | https://yunsio.com/terms |
| **Bilibili** | 搜索 "SuperStage2025" |
| **微信公众号** | 回复 "SuperStage2025" 加入社群 |

### 13.2 联系方式

| 类型 | 联系方式 |
|------|---------|
| **商务合作** | 微信 YERRKJ |
| **技术咨询** | 邮箱 yerrkj@outlook.com |
| **厂商认证申请** | yerrkj@outlook.com |

### 13.3 学习资源

**免费教程**
- Bilibili 搜索 "SuperStage2025" 观看零基础教程系列
- 完全免费，50+ 期完整系列

**付费培训**
- 《SuperStage 视效设计与全案实战特训营》
- 22节课完整工作流
- 购买插件用户专享

## 版权声明

**SuperStage** 是佛山市壹贰冉冉科技有限公司的注册商标。

本文档版权所有 © 2026 LimxTeam，保留所有权利。

未经书面许可，不得以任何形式复制、分发或传播本文档内容。

**商标声明**：Unreal Engine 是 Epic Games, Inc. 的商标或注册商标。GrandMA、MA2、MA3 是 MA Lighting International GmbH 的商标。Vectorworks 是 Vectorworks, Inc. 的商标。Pangolin、Beyond 是 Pangolin Laser Systems, Inc. 的商标。Martin、Robe、ClayPaky 等灯具品牌名称归各自所有者所有。本文档中提及的所有第三方商标仅作兼容性说明之用，不构成任何形式的授权或背书关系。

---

*文档最后更新：2026年4月14日*
