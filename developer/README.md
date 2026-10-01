# SuperStage 开发文档：DMX 对象控制与 NDI 媒体应用

源码基线：当前仓库 `SuperStage.uplugin` 的版本标识 **26H2.7**。核对日期：2026-09-29。面向项目开发者，以及通过 UE5MCP 操作 Unreal Engine 的 AI。

本套文档从现有产品实现整理：DMX 参考现有升降机械、轨道机械及材质控制对象的调用方式；NDI 参考屏幕、投影对象的媒体钩子。内容用于控制自己的模型、平台、机关、材质和业务状态，或把 NDI 纹理用于自己的显示对象。

## 接入的主线

DMX 控制类以 `ASuperDmxActorBase` 为基类，配置 `SuperDMXFixture` 和 `FixtureLibrary`，在 **Super 自身事件**中读取和应用数据。可以直接控制自己的组件，也可以持有外部普通 Actor 引用并控制它。

| 工作 | 蓝图事件 | C++ 重写入口 |
| --- | --- | --- |
| 初始化引用、材质和默认状态 | Light Initialization | NativeLightInitialization |
| 连续运动、插值、速度积分 | Super DMX Tick | NativeSuperDMXTick |
| DMX 参数变化后更新颜色、档位、业务目标 | Super DMX Changed | NativeSuperDMXChanged |

**项目 DMX 逻辑不接 Event Tick，不重写引擎 Tick 进行轮询，不用定时器替代 Super 事件。** 底层如何调度由基类负责，项目保持基类正常运行。蓝图与 C++ 使用相同事件模型。

NDI 使用 `ASuperMediaBase`：基类管理订阅及纹理更新，C++ 消费端重写 `OnActiveTextureChanged()`，调用 `GetActiveTexture()` 并应用到自己的材质。媒体的蓝图可用性与 DMX 不同，见媒体章节，不假定存在未声明的节点。

## 文档顺序

1. [接入边界、项目准备与源码依据](01_Integration_Contract_zh.md)
2. [Super 事件：职责、时序与使用方式](02_Super_Events_zh.md)
3. [SuperDmxActorBase.h 逐项参考](03_SuperDmxActorBase_Reference_zh.md)
4. [SuperFixtureLibrary：字段、JSON 与资产创建](04_SuperFixtureLibrary_zh.md)
5. [蓝图接入：事件图与多种案例](05_Blueprint_Integration_zh.md)
6. [C++ 接入：完整项目示例](06_CPP_Integration_zh.md)
7. [NDI：媒体钩子、蓝图桥接与用途](07_NDI_Integration_zh.md)
8. [AI / UE5MCP 操作规程与验收](08_AI_Execution_zh.md)
9. [示例文件和通道协议](examples/README.md)

AI 开始操作前至少读完第1至5章及第8章。函数名、事件名、结构字段以本文和当前安装包反射结果共同核对；不要根据显示名称推测参数或创造工具名。

## 开放接口与交付边界

SuperStage 是商业闭源插件。开放的是 **SuperCore 核心模块头文件所提供的接入能力**；头文件开放不等于源码开源。其他模块不作为用户 C++ 开发接口。项目代码写在自己的模块中，只依赖需要的 Unreal 标准模块及 SuperCore。

本文中的项目示例类、辅助函数、事件和 JSON 文件都明确标为“项目示例”；它们不是插件安装后自带的 API。现有内置对象用于核对调用方式，不要求用户获取其他模块源码或将其加入编译依赖。

文档与示例已作源码静态核对、地址和协议校验。未声称完成客户交付包的 UHT/编译/链接、UE5MCP 实操、真实 DMX/NDI 或打包测试。各阶段验收方法在第8章，不能用“文档有代码”替代实际验证。
