# 蓝图接入：用Super事件控制自己的对象

[返回目录](README.md) · [通道库](04_SuperFixtureLibrary_zh.md)

以下图表按实际BlueprintImplementableEvent和BlueprintPure声明整理，是项目蓝图的搭建步骤；未声称提供已在UE中生成的uasset。AI必须在当前UE中检查节点引脚、连接、编译和运行结果。

## 1. 创建正确父类

创建项目蓝图BP_ProjectDmxController，父类选择SuperDmxActorBase（反射路径`/Script/SuperCore.SuperDmxActorBase`）。名称BP_ProjectDmxController是本文项目示例，不是产品自带资产。已有业务Actor保持自己的父类，由控制蓝图保存它的引用。

在事件选择/Overrides中定位：LightInitialization、SuperDMXTick、SuperDMXChanged；显示名为Light Initialization、Super DMX Tick、Super DMX Changed。三个事件都来自父类，**不要创建同名Custom Event冒充继承事件**。

不添加Event Tick，不用Set Timer驱动DMX读取，不在事件里手动调用ForceRefreshDMX。Super事件由父类负责调度。

先编译一次，放入关卡。配置FixtureLibrary、SuperDMXFixture及ControlMode=DMX。FixtureLibrary通过类默认值/实例详情或编辑器属性工具设置；它没有运行时蓝图Set属性节点。

## 2. 案例一：16位升降自己的平台

### 配置表

使用Motion6Axis_12CH.json导入的库。内部Universe=1、StartAddress=101，ZPos位于绝对105/106。只使用Z轴仍可保留完整库，其他属性无需连到业务。

新增项目变量：

| 变量 | 类型 / 示例值 | 用途 |
| --- | --- | --- |
| TargetActor | Actor引用，可实例编辑 | 用户自己的平台 |
| StartPosition | Vector，例如(0,0,0) | 世界起点，必须填写目标实际起点 |
| EndPosition | Vector，例如(0,0,300) | 世界终点 |
| Enabled | bool，初始false | 配置检查后开启 |
| PreviewInEditor | bool，初始false | 显式允许编辑器运动预览 |
| PositionAlpha | float，初始0 | 上次有效归一化位置 |
| Interpolate | bool，初始false | 是否持续插值 |
| InterpSpeed | float，例如5 | 插值速率 |

目标根组件需可移动；本例使用直接Transform，不同时开启物理模拟驱动。StartPosition/EndPosition是配置值，不每次初始化从已移动位置重新捕获。

### Light Initialization图

检查TargetActor引用和起止参数，初始化项目需要的资源。此例无需新增材质；不移动平台，不更改用户配置的起止点。可以记录配置问题，避免每次事件都重复刷屏。

### Super DMX Tick图

```text
Event Super DMX Tick
  → Enabled为true
  → ControlMode等于DMX
  → TargetActor IsValid
  → 运行状态允许，或显式启用编辑器预览
  → ResolveAttributeAddressesByIndex(Self, 0, "ZPos")
  → CoarseAbs和FineAbs均在1..512
  → GetAttributeRaw16ByIndex(Self, 0, "ZPos", -1)
  → 保存Raw到局部/变量，避免Pure重复求值
  → Raw>=0：PositionAlpha = Clamp(Raw / 65535.0, 0, 1)
  → DesiredPosition = Lerp(StartPosition, EndPosition, PositionAlpha)
  → Interpolate？
      是：VInterpTo(TargetActor位置, DesiredPosition,
                   Get World Delta Seconds, InterpSpeed)
      否：DesiredPosition
  → SetActorLocation(TargetActor, 结果)
```

地址配置无效时结束本次处理；无数据的Raw=-1保留已有PositionAlpha，这是本例选择的保持策略。若尚无有效值，项目可增加HasValidInput门控，不对初始位置做自动归零。所有读取、映射、插值及应用都由Super DMX Tick触发。

### 数值验收

| 105/106字节 | Raw16 | 从0到300厘米的目标Z |
| --- | --- | --- |
| 0/0 | 0 | 0 |
| 0/255 | 255 | 约1.1673 |
| 1/0 | 256 | 约1.1719 |
| 128/0 | 32768 | 约150.0023 |
| 255/255 | 65535 | 300 |

保持255/255，平台应稳定在终点；开插值后即使输入保持不变也应继续收敛。若仅Changed时运行插值，会停在中间，这是事件选择错误。重新构造控制蓝图不应把终点当新起点。

## 3. 案例二：自己的门板角度或持续旋转

沿用12通道库的XRot，绝对107/108，读取Fine。事件入口仍是Super DMX Tick。

**绝对角度：** 项目配置ClosedRotation和OpenRotation，例如围绕门板铰链Yaw从0到90度。归一化后Lerp角度，选择自己的组件SetRelativeRotation。门板必须有合适枢轴；没有则加项目SceneComponent作为旋转父组件。不能每次AddLocalRotation(TargetAngle)，否则目标角会累计。

**持续旋转：** 另一个业务模式把归一化输入映射为每秒角速度，例如Lerp(-90,90,Alpha)，再乘Get World Delta Seconds，以AddLocalRotation应用。保持相同速度输入时继续旋转，所以使用Super DMX Tick。16位中点并非精确0.5，可设置明确死区，比如Raw在32760..32775时速度0；这属于项目业务规则，不是插件自动规则。

验收绝对模式最大值停在90度；速度模式最大值约每秒90度且不依赖Changed次数。不能同时对同一目标运行两种模式。

## 4. 案例三：自己的材质颜色和效果参数

使用MaterialControl_6CH.json，StartAddress=201。模块0：Red/Green/Blue分别201/202/203，Effect=204、Speed=205、Width=206。显示参数名不自动产生材质参数，先建立自己的材质及对应参数。

项目变量：TargetMesh、MaterialIndex、BaseMaterial、MID、Color、EffectValue、SpeedValue、WidthValue。项目材质示例参数名为ProjectColor、ProjectEffect、ProjectSpeed、ProjectWidth；这些不是插件要求的固定名。

**Light Initialization：** 验证目标和材质槽；MID无效或来源材质变化时创建，保存引用；把MID应用到目标槽。重复初始化复用自己的MID；如果目标/来源变更，则重新应用，避免复制对象误共用实例。

**Super DMX Changed：** 先检查ControlMode=DMX和项目启用；按库读取Red/Green/Blue三个Raw8。成功后除255形成LinearColor(R,G,B,1)。Effect、Speed、Width各读取Raw8并归一化，映射到项目需要范围。参考现有材质控制方式：Effect映射0..10、Speed映射-10..10、Width映射0..10；只在项目材质确实按此范围设计时沿用。

把得到值写入MID。不要每次Changed重建MID，不用引擎Tick重复写相同参数。若自己的材质动画由材质Time驱动，同一参数保持不变时画面仍可持续动画，无需重复绑定。

测试201..203为255/0/0、0/255/0、0/0/255，应读出(1,0,0)、(0,1,0)、(0,0,1)。保持值时Changed正常路径不继续触发，但材质应继续显示。测试Effect=255→10、Speed=0→-10、Speed=255→10。MID参数正确但画面不变时检查材质连线、参数拼写和槽位。

## 5. 案例四：模式与一次性业务按钮

使用CommandControl_2CH.json，StartAddress=301：Mode=301、Trigger=302，均8位。读取与业务处理放Super DMX Changed。

Mode的库分段：0..63 Idle、64..191 Preview、192..255 Run。蓝图通过GetModules→模块0→AttributeDefs找到Mode→遍历SubAttributes，以闭区间找首个匹配段。不存在原生蓝图FindSubAttribute节点，不要虚构。项目也可用与协议完全一致的整数分支，但需同步协议变更。

段名与项目缓存CurrentMode不同才调用目标的项目模式切换函数。分段名字本身不会自动调用函数。

Trigger新增HasTriggerBaseline、LastTriggerHigh、CommandCount。先确认运行状态和有效地址/读数；首次记录高低，随后只在低→高时调用一次目标业务函数。不能直接“每次Changed就执行”。

输入0、127、128、255、255、0、128，预期CommandCount为0、0、1、1、1、1、2。初始高值只建基线。高电平时改变Mode应改变CurrentMode，不能增加CommandCount。编辑器构造不执行业务命令。

## 6. 案例五：三个自己的对象共用输入控制

使用MultiObject_6CH.json：三个模块ObjectA/B/C，Patch=1/3/5，每个模块ZPos粗细偏移1/2。StartAddress=401，地址为401/402、403/404、405/406。

控制蓝图保存Targets数组，与模块下标0/1/2一一对应；每个目标另有Start/End配置。Super DMX Tick中ForLoop模块下标，以GetAttributeRaw16ByIndex(Index,ZPos,-1)读取、归一化并应用该目标。

缺少某个目标或该模块属性时只跳过该项。不要用过滤后的矩阵值数组下标猜原模块身份。若明确使用GetMatrixAttributeRaw16，它返回的已经是0..1且默认按Patch排序；模块缺属性会被跳过。蓝图没有WithIndex原生节点，显式模块循环更清晰。

A输入0/0、B输入128/0、C输入255/255，预期各自为起点、中间约50%、终点。把B目标引用清空时A/C仍正确，不能错位。修改模块顺序需同步目标映射。

## 7. 蓝图节点可用性核对表

| 操作 | 是否原生可用 |
| --- | --- |
| 三个Super事件 | 正确父类派生蓝图可实现 |
| GetAttributeRaw8/16/24ByIndex | BlueprintPure |
| ResolveAttributeAddressesByIndex | BlueprintPure，多地址输出 |
| GetModules / GetFixtureChannelSpan | BlueprintPure |
| GetMatrixAttributeRaw / Raw16 | BlueprintPure，归一化输出 |
| FindAttributeDef / WithIndex系列 | C++，不能直接造节点 |
| MakeDmx C++快捷函数 | 非蓝图节点；蓝图用FSuperDMXAttribute结构体构造/拆分 |
| FixtureLibrary运行时Set | 不存在自动Set节点，通过编辑器配置 |
| Bind Event to SuperDMXChanged | 不存在这种委托；需要跨对象广播则声明项目Dispatcher |

纯函数没有执行引脚，把读取结果保存后再分支/应用。每个图完成后编译并查看错误，读回真正连接而不是仅确认节点已创建。
