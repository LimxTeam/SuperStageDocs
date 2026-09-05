# SuperStage DMX 系统总览

本文档面向使用 SuperStage 进行灯光预演和 DMX 联调的用户。它只描述当前实现中可以确认的功能，不承诺固定帧率、固定延迟、固定灯具规模或现场网络效果。

## 1. DMX 系统能做什么

SuperStage 的 DMX 系统用于在 UE 场景中接收、缓存、查看和发送 DMX 数据。

当前实现可确认的能力包括：

| 能力 | 用户可见表现 |
| --- | --- |
| Art-Net 输入 | 监听 UDP 6454，解析 Art-Net OpDmx 数据。 |
| sACN E1.31 输入 | 监听 UDP 5568，解析 sACN 数据包，并可加入连续 sACN 组播 Universe。 |
| DMX 输出 | Sequencer 回放或内部发送时，可按当前协议发送 Art-Net 或 sACN 数据。 |
| Universe 缓存 | 系统保存最近收到或发送的 Universe 数据，供灯具、监视器和回放使用。 |
| DMX 通道查询 | 灯具 Actor 按 Universe 和 Address 读取 1-512 通道值。 |
| Activity Monitor | 在编辑器中查看已缓存 Universe 的通道值。 |
| Sequencer / Take Recorder | 可把 DMX 数据录制到 Sequencer，并在回放时重新注入/输出。 |

## 2. 基本概念

| 概念 | 说明 |
| --- | --- |
| Universe | 一组 DMX 通道。SuperStage 内部用户侧 Universe 从 1 开始显示和使用。 |
| Channel | Universe 中的通道值，范围 0-255。 |
| Start Address | 灯具在 Universe 中读取的起始地址，范围 1-512。 |
| Fixture ID | 灯具编号，用于整理和对接外部控台数据。 |
| Fixture Library | 灯具通道表，用于定义某个灯具的各个功能对应哪些 DMX 通道。 |

SuperStage 支持 8 位、16 位和 24 位通道读取接口。实际能否使用对应精度，取决于灯具库里是否为该属性配置了粗调、精调和超精调通道。

## 3. 网络配置

打开 SuperStage 工具栏中的 DMX 配置面板后，可以设置：

| 设置 | 说明 |
| --- | --- |
| Protocol | `Art-Net` 或 `sACN (E1.31)`。 |
| Input Enable | 是否启动输入接收。 |
| Input Local IP | 输入绑定的本机网卡。`0.0.0.0` 表示监听所有可用地址。 |
| Input Start Universe | 输入 Universe 偏移，用于让外部控台编号与 UE 内部编号对齐。 |
| Output Enable | 是否允许向外部设备发送 DMX。 |
| Output Local IP | 输出使用的本机网卡。 |
| Output Start Universe | 输出 Universe 偏移。 |

端口由协议固定：

| 协议 | 默认端口 | 说明 |
| --- | --- | --- |
| Art-Net | UDP 6454 | 默认远端为广播地址。 |
| sACN E1.31 | UDP 5568 | 未配置固定远端时，输出按 Universe 计算 `239.255.x.y` 组播地址。 |

## 4. Universe 对齐

SuperStage 内部按 `Universe 1`、`Universe 2` 这样的编号给用户显示。外部协议的网络 Universe 编号可能不同，所以配置面板提供 Start Universe。

输入时：

```text
SuperStage 内部 Universe = 网络 Universe - Input Start Universe + 1
```

输出时：

```text
网络 Universe = SuperStage 内部 Universe + Output Start Universe - 1
```

常见设置：

| 外部设备编号习惯 | Start Universe 建议 |
| --- | --- |
| Art-Net 网络 Universe 从 0 开始 | 0 |
| 控台或软件界面从 Universe 1 开始 | 1 |

如果发现通道值能收到但灯具不响应，优先检查 Start Universe 是否与控台一致。

## 5. 常用工作流

### 接收控台信号

1. 在 DMX 配置面板选择协议。
2. 选择正确的本机网卡。
3. 启用 Input。
4. 打开 Activity Monitor，确认对应 Universe 有通道值变化。
5. 确认灯具的 Universe、Start Address 和 Fixture Library 与控台一致。

### Sequencer 回放输出

1. 在 DMX 配置面板启用 Output。
2. 选择协议和输出网卡。
3. 检查 Output Start Universe。
4. 回放已录制的 DMX Sequencer 数据。

输出是否能驱动真实设备，取决于网络、控台/节点配置、防火墙和协议设置。

## 6. 相关文档

| 文档 | 内容 |
| --- | --- |
| [01 - DMX 网络配置](01_DMX_Network_Configuration_zh.md) | 配置 Art-Net / sACN 输入输出。 |
| [02 - 灯具库](02_Fixture_Library_zh.md) | 创建和维护灯具通道表。 |
| [03 - DMX 灯具基础](03_DMX_Actor_Base_zh.md) | 灯具的 Universe、Address、Fixture ID 和灯具库引用。 |
| [06 - DMX 活动监视器](06_DMX_Activity_Monitor_zh.md) | 查看 Universe 通道值。 |
| [07 - Patch 工具](07_Patch_Tools_zh.md) | 批量设置灯具地址。 |
| [08 - DMX 录制与回放](08_DMX_Recording_Playback_zh.md) | Sequencer / Take Recorder 相关工作流。 |
| [09 - MA 控台导出](09_Export_To_MA_zh.md) | 导出 grandMA2 XML 图层和宏文件。 |
| [11 - GrandMALink](11_GrandMA_Link_zh.md) | 连接 grandMA2 / grandMA3 并执行 Patch 导入或同步。 |

## 7. 排查顺序

如果虚拟灯具不响应：

1. 确认 DMX 配置面板中 Input 已启用。
2. 确认选择了正确网卡，或临时使用 `0.0.0.0`。
3. 确认 Windows 防火墙没有阻止 UDP 6454 或 UDP 5568。
4. 用 Activity Monitor 查看是否收到对应 Universe 的通道值。
5. 检查 Start Universe 偏移。
6. 检查灯具 Actor 的 Universe 和 Start Address。
7. 检查灯具是否绑定了正确的 Fixture Library。
8. 检查控台或发送端是否真的在发送当前协议的数据。

如果使用 sACN 组播，还需要确认交换机、网卡和系统路由没有把组播加入到错误的网络接口上。
