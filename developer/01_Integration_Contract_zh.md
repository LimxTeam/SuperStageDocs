# 接入边界、项目准备与源码依据

[返回目录](README.md)

## 1. 本文解决什么

让用户自己的 Actor、组件、材质或业务系统接受 DMX 控制；让 NDI 画面成为用户自己的材质/UI等用途的输入。接收、协议、线程和纹理上传由 SuperStage 管理。项目实现“读取后的行为”。

DMX 控制对象继承 ASuperDmxActorBase。现有目标 Actor 已有其他父类时，另建一个以此类为父类的项目控制对象，给它保存 TargetActor/TargetComponent 引用；目标无需更改父类。目标通过控制对象的 Super 事件收到更新，不自行建立引擎 Tick 轮询链。

项目新增变量如 TargetActor、MoveStart、MoveEnd、LastTriggerHigh，属于项目数据，不是 ASuperDmxActorBase 预置字段。创建前必须区分继承属性与自己新增属性。

## 2. 以现有对象的真实调用为依据

以下记录调用事实，不要求用户包含对应模块头文件：

| 现有对象 | 已核对的调用 | 本文应用 |
| --- | --- | --- |
| SuperLiftingMachinery | NativeSuperDMXTick → ReadDMXAndApply → LiftingMachinery；以Fine读取XPos/YPos/ZPos/XRot/YRot/ZRot | 位置、旋转及持续插值放Super DMX Tick |
| SuperRailMachinery | NativeSuperDMXTick → ReadDMXAndApply | 轨道等连续控制沿用Super事件入口 |
| SuperLightStripEffect | NativeLightInitialization → 默认材质准备；NativeSuperDMXChanged → ReadDMXAndApply | 初始化可复用材质，Changed更新参数及外部目标 |
| 内置升降矩阵对象 | 初始化准备组件；Changed更新效果/颜色；SuperDMXTick执行升降 | 一个控制对象中按行为分配Super事件 |
| SuperScreen | OnActiveTextureChanged → GetActiveTexture → 动态材质；对目标网格数组应用材质 | 媒体通过纹理引用变化钩子应用到自己的对象 |
| SuperProjector | OnActiveTextureChanged将同一活动纹理应用到多个材质 | 一个媒体来源可服务多个消费材质 |

源码中有些旧注释写“每帧由Tick调用”，但函数体实际从NativeSuperDMXTick或NativeSuperDMXChanged进入。判断接入入口时以定义和调用点为准。个别内置类还有自己的非接收逻辑；不因此要求项目把DMX读取搬到引擎Tick。

## 3. 工程与许可

使用与引擎版本、目标平台匹配的插件二进制和正式开放头文件，启用SuperStage，确认当前账号具备项目扩展所需权益。父类生命周期包含授权处理；不得删除父类调用或重新启用被权限逻辑禁用的更新来规避限制。

公开C++类路径：`/Script/SuperCore.SuperDmxActorBase`、`/Script/SuperCore.SuperMediaBase`、`/Script/SuperCore.SuperFixtureLibrary`。媒体基类是Abstract，创建其具体派生类。蓝图创建前读取当前编辑器父类/反射信息，不使用旧模块名前缀猜路径。

项目模块Build.cs加入：

```csharp
PublicDependencyModuleNames.AddRange(new string[]
{
    "Core", "CoreUObject", "Engine", "SuperCore"
});
```

若已有这些条目就合并，不覆盖项目其他依赖。自己的公开头继承SuperCore类型时采用公共依赖。文档示例不添加SuperAssets、SuperDMX、SuperNdi或SuperTools为直接开发依赖，也不include这些模块的类。

当前SuperMediaBase.h存在跨模块包含依赖。能在全源码仓库编译，不代表只有开放头的交付包完整可编译。媒体示例必须用正式SDK验证头文件链和链接产物；缺少依赖由产品交付解决，不指导用户复制非开放源码。DMX和NDI可独立接入，不把媒体编译问题混入单纯DMX任务。

## 4. 先接通输入，再接业务

在产品的DMX输入配置中确认协议、网卡、网络Universe范围及实际输入。内部Universe映射为：

```text
内部Universe = 网络Universe - 输入起始Universe + 1
```

例如网络Universe=10、输入起始Universe=10，对象内部Universe应为1。该规则来自当前接收实现，不把控台编号直接复制给对象。

在对象上设置ControlMode=DMX、SuperDMXFixture.Universe、StartAddress，并赋FixtureLibrary。命名属性、矩阵读取和正常Changed检测都依赖库。GetChannelValue虽能独立读取，但正式案例仍使用库描述输入，使定义、地址与事件一致。

NDI先在输入面板配置并确认可用源，再选择SourceMode=NDI及InputName。InputName是配置输入的逻辑名称，不是任意填写的IP或流URL。

## 5. 数据单位

DMX字节为0..255；16位为0..65535；24位为0..16777215。归一化是0..1。世界/相对位置通常用厘米，角度用度；协议必须明确每个值的映射。0.5归一化不等于50厘米，也不等于DMX字节128的精确值。

所有案例先保存稳定起止位置/角度，再以绝对目标应用；只有“输入表示速度”的案例做DeltaSeconds积分。禁止把绝对角度每次作为增量叠加。

## 6. 本次文档所保证的范围

函数签名、反射标记、计算及回调条件按当前源码说明。未暴露逐网络包委托、断流时间戳或在线保证的地方，不自行补出这些能力。NDI纹理引用变化也不是逐帧CPU数据通知。

UE5MCP的工具清单和版本需运行时查询，文档给出精确的语义动作、类/函数标识和读回要求；不虚构通用工具名称或参数结构。
