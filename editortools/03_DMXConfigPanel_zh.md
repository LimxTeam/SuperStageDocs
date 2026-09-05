# SuperStage DMX 配置面板用户手册

## 功能定位

DMX 配置面板用于快速修改 SuperStage 的 DMX 输入和输出网络设置，并把设置立即应用到运行中的 DMX 子系统。面板底部嵌入 DMX Activity Monitor，用于查看当前缓冲中的 Universe 通道值。

## 打开方式

点击编辑器中的 **SuperDMX** 入口打开配置面板。

## 面板控件

| 控件 | 说明 |
| --- | --- |
| Protocol | 选择协议。当前输入和输出共用同一个协议设置。 |
| Input Local IP | 输入端绑定的本地 IP。可选 `0.0.0.0` 或系统枚举到的网卡地址。 |
| Input Enable | 启用或关闭 DMX 输入。 |
| Input Start Universe | 输入起始 Universe。 |
| Output Local IP | 输出端使用的本地 IP。可选 `0.0.0.0` 或系统枚举到的网卡地址。 |
| Output Enable | 启用或关闭 DMX 输出。 |
| Output Start Universe | 输出起始 Universe。 |

## 协议规则

| 协议 | 端口 | Start Universe 范围 | 远端地址规则 |
| --- | --- | --- | --- |
| Art-Net | 6454 | 0-32767 | 默认广播 `255.255.255.255`。 |
| sACN (E1.31) | 5568 | 1-63999 | 默认不填写固定远端地址，发送时按 Universe 使用 `239.255.{hi}.{lo}` 组播地址。 |

选择协议后，面板会按协议重置端口和远端地址，并钳制 Start Universe 到对应范围。

## 本地 IP

- `0.0.0.0` 表示绑定任意地址。
- 其他选项来自系统网卡地址枚举。
- Art-Net 接收会按选择的本地 IP 绑定。
- sACN 接收在子系统中绑定 `0.0.0.0`，选择的本地 IP 用作组播接口参考。

## 底部的通道值表

设置区下面是一张实时刷新的表，显示当前正在收到的通道值。要回答"到底有没有数据进来"，看这里最快——先确认信号，再去怀疑灯具。

| 控件 | 说明 |
| --- | --- |
| **All Universes** | 列出所有收到过的 Universe。取消勾选后可在 **Universe** 里填一个编号，只看那一个 |
| **Universe** | 只看某一个 Universe 时填这里，需先取消 All Universes |
| **Clear** | 忘掉目前为止收到的内容。想确认数据是不是还在持续进来（而不是在看一张早就停了的旧画面）时用它 |

同样的表也可以单独打开，见 [DMX 活动监视器](15_DMXActivityMonitor_zh.md)。

## 配置保存

协议、IP、Enable 或 Start Universe 一改动，面板立即保存并应用到运行中的 DMX 子系统——不需要另外点保存，也不用重启编辑器。设置随项目保存，下次打开工程仍然有效。

## 使用注意

- 输入和输出不能在该面板里分别选择不同协议。
- 面板不提供手动填写目标 IP 的输入框；Art-Net 和 sACN 的远端地址由协议默认值决定。
- 灯具是否响应还取决于场景内灯具的 Universe、Start Address、DMX 模式和外部控台输出设置。
