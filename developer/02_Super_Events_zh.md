# Super 事件：DMX 驱动的唯一项目入口

[返回目录](README.md) · [函数参考](03_SuperDmxActorBase_Reference_zh.md)

## 1. 蓝图和C++一一对应

| 职责 | 蓝图真实函数名 / 显示名 | C++入口 |
| --- | --- | --- |
| 初始化 | LightInitialization / Light Initialization | NativeLightInitialization |
| 连续处理 | SuperDMXTick / Super DMX Tick | NativeSuperDMXTick |
| 变化处理 | SuperDMXChanged / Super DMX Changed | NativeSuperDMXChanged |

三个蓝图入口声明为protected BlueprintImplementableEvent，分类A.Event。它们出现在正确父类的派生蓝图事件/重写列表中。它们不是可在外部Actor上绑定的Dispatcher。C++重写Native前缀虚函数，不编写LightInitialization_Implementation之类不存在的BlueprintNativeEvent实现。

基类Native方法默认仅转发给对应蓝图事件。现有SuperAssets案例在Native override中先调用Super，再运行自身处理函数。本文C++示例遵从相同顺序。若C++和其派生蓝图同时实现同一任务，应明确由谁应用最终输出，避免移动两次、绑定两次或触发两次。

## 2. Light Initialization

适合配置组件默认状态、确保动态材质存在、整理目标引用和初始化可重复创建的资源。不要在这里每次都新建同一个MID或追加同一组件。现有材质案例会检查动态材质是否已存在，再创建和应用。

这个事件不是“一生仅一次”。OnConstruction与BeginPlay都会调用；编辑器修改和蓝图重建可重复发生。一次性业务命令不能放这里。

运动范围应是明确的配置值，或由项目显式初始化动作捕获一次。不要每次Light Initialization都把对象已经移动后的位置当作新起点。本文项目示例使用可编辑的起止位置，避免隐含的重复基准捕获。

## 3. Super DMX Tick

适合自己的平台运动、平滑插值、连续旋转或需要每次更新推进的逻辑。**在此事件中读取通道并应用连续行为**，这与现有升降机械一致。不要求先Changed缓存所有输入再运动。

典型调用链：

```text
Super DMX Tick
  → ControlMode / 项目启用开关 / 目标有效性检查
  → 读取属性
  → 限制0..1
  → 映射项目物理范围
  → 可选插值或按秒积分
  → Set目标位置/旋转/参数
```

事件没有DeltaTime引脚。C++按现有机械案例使用 `GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.0f`；蓝图使用当前世界的Get World Delta Seconds。**不要为了取DeltaSeconds重写Tick。** 编辑器构造时也可能进入该事件，项目自行决定是否允许预览；时间为0时不要除以DeltaSeconds。

保持输入不变时，连续旋转仍需继续旋转、插值仍需继续趋近目标，因此不能把这些过程只放Changed。

## 4. Super DMX Changed

适合读取并更新材质颜色、效果选择、离散档位和业务状态。可以一次读取多个属性，集中应用到多个目标。现有材质控制案例正是在NativeSuperDMXChanged内进入ReadDMXAndApply。

它没有NewValue、OldValue或Channel参数。事件到来后，项目使用读取函数取当前值。不要假定一次事件仅对应一个属性改变。

DMX模式正常调度中，基类根据FixtureLibrary计算最小到最大有效通道，比较整个连续范围的字节快照。规则如下：

| 条件 | 是否从变化检测触发 |
| --- | --- |
| 无库、没有有效跨度 | 否 |
| Universe无可读快照 | 否 |
| 任一范围端点越界，整段不能读 | 否 |
| 第一次有可读范围，或范围/长度变化 | 是 |
| 范围内字节与上次不同 | 是，先更新快照再回调 |
| 发送相同字节的新包 | 否 |
| 范围内未定义的空隙字节变化 | 也可能触发 |
| 换Universe但范围及字节相同 | 不保证触发 |

因此库中的一个错误远端偏移能阻断整个正常Changed路径；仅GetAttributeRaw8读到值，不能证明Changed范围正确。

## 5. 完整时序

```text
OnConstruction
  父类构造
  NativeLightInitialization → 蓝图Light Initialization
  NativeSuperDMXTick        → 蓝图Super DMX Tick
  NativeSuperDMXChanged     → 蓝图Super DMX Changed（无条件）

BeginPlay
  父类BeginPlay
  NativeLightInitialization

基类每次调度
  父类调度
  NativeSuperDMXTick
  DMX模式：变化检测 → 满足条件才NativeSuperDMXChanged
  Property模式：直接NativeSuperDMXChanged
```

上述底层生命周期用来解释什么时候收到Super事件，不是让项目另建Event Tick图。保持父类调度即可。

构造时Changed无条件调用；Property模式每次调度调用Changed。因此“收到Changed”不能直接执行一次性业务动作，也不能证明收到新网络包。基类SuperDMXTick先于Changed，若项目自行采用Changed缓存目标方案，要接受该先后顺序；连续运动案例直接在SuperDMXTick读取，避免不必要的双阶段状态。

## 6. Property模式要由项目选定行为

读取API在Property模式走默认/保留值路径。现有机械的ReadDMXAndApply会先检查Property并返回。本文运动示例也采用此规则：Property模式不执行DMX驱动。

如果项目要做“属性面板控制”，须自己定义控制变量及应用逻辑。C++引用式读取可以保留原变量值，但不代表所有API都如此，Raw系列和矩阵函数各有规则。不能把Property模式下的每次Changed误认为DMX不断变化。

## 7. 一次性命令使用业务边沿

自己的Trigger属性0..127为低、128..255为高。项目新增HasTriggerBaseline、LastTriggerHigh。Changed中先确认运行世界与项目启用、目标有效、ControlMode=DMX；读Raw8后按下列流程处理：

```text
无有效读数 → 不触发
首次有效读数 → 记录电平，建立基线，不触发
当前高 且 上次低 → 执行自己的动作一次
更新上次电平
```

这是本文案例明确采用的业务规则，不是插件内建边沿功能。若启动时高电平需要执行，项目需明确采用另一个初始规则。编辑器构造、其他属性变化或重复高值不能额外增加命令次数。

## 8. 转交外部Actor

控制对象可以直接调用目标的项目函数，也可以声明自己的BlueprintAssignable委托/蓝图Event Dispatcher。例如项目事件OnProjectModeChanged属于项目新增API，目标绑定它；不要创建Bind Event to SuperDMXChanged节点。

事件源负责读取、映射及去重，目标负责自身行为。目标销毁时检查引用；接收者生命周期结束时移除自己的绑定。一个对象的职责保持明确，不能控制器和目标同时对同一通道轮询。

## 9. ForceRefreshDMX与编辑器预览

ForceRefreshDMX主动执行一次SuperDMXTick，再按控制模式变化处理，并标记渲染状态；编辑器还调用PostEditChange，可能引发额外构造。DMX模式并非无条件Changed，也不会主动索取新网络包。

不要在任何Super事件里调用ForceRefreshDMX，会造成重入风险。常规输入由基类调度。允许编辑器预览时，用项目显式开关控制可逆视觉修改；不可逆业务命令仅在项目规定的运行条件下执行。

验收时分别计数Init、SuperTick、Changed和业务Command。不要只看对象动没动。保持值应允许运动继续，但一次性命令不再重复。
