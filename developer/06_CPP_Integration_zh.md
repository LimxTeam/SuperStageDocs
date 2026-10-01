# C++接入：沿用现有对象的Super事件调用链

[返回目录](README.md) · [完整示例文件](examples/README.md)

## 1. 项目结构

将示例头文件放自己的模块Public，cpp放Private；Build.cs依赖Core、CoreUObject、Engine、SuperCore。`.generated.h`必须在头文件include列表最后。示例不带跨模块导出宏，适用于同一个项目模块；需要其他模块调用时添加自己模块的API宏。

DMX示例只include开放SuperCore头及Unreal标准头。**不继承SuperAssets的具体实现类，不include其头文件，不重写引擎Tick。** 参考其事件组织方式即可。

项目类必须编译后才会出现在编辑器类列表。新增UCLASS/UFUNCTION涉及反射变更时，按项目正常编译流程使UHT和模块加载完成；仅把文件复制进去不等于可用节点已经出现。

## 2. 连续运动：ProjectDmxMotionController

完整文件：[头文件](examples/ProjectDmxMotionController.h)、[实现](examples/ProjectDmxMotionController.cpp)。对应现有机械的调用结构：

```cpp
void AProjectDmxMotionController::NativeSuperDMXTick()
{
    Super::NativeSuperDMXTick();
    ReadDMXAndApply();
}
```

ReadDMXAndApply是项目函数，不是基类API。它验证项目启用、ControlMode、目标、世界和地址，然后用GetSuperDmxAttributeValue读取PositionInput，Clamp、Lerp、可选VInterpTo，最后应用到外部TargetActor。

配置步骤：

1. 编译项目并放置ProjectDmxMotionController。
2. 给FixtureLibrary赋导入的Motion6Axis_12CH库；Universe按输入映射设置，StartAddress=101。
3. PositionInput默认MakeDmx(ZPos,true)，即模块0、属性ZPos、16位；绝对105/106。不要因为是六轴库就将InstanceIndex写成“第三个轴”2，六个轴是同一个模块的六个属性。
4. TargetActor引用自己的可移动、非物理驱动平台；StartPosition/EndPosition填世界坐标，如实际起点及起点+(0,0,300)。目标不能是控制器自己。
5. 配置正确后bEnabled=true。bPreviewInEditor默认false；需要编辑器预览再明确开启。运行世界要求已经BeginPlay，避免构造过程执行运动。
6. 用105/106的端点与粗细交界值验收。开启插值后保持输入，平台应继续收敛。

本例使用与现有机械相同的引用保留语义：无快照时PositionAlpha保留上次值，初始为0。因此启用前应准备好输入；初次无输入时可能应用配置起点。若业务需要“首次收到可读数据才移动”，在同一个SuperDMXTick处理函数中改用Raw16默认-1和HasValidInput门控；这只是失败策略变化，不改变事件入口。

NativeLightInitialization不会重新从目标位置捕获起点。配置变化的作用是明确的世界起止值，不造成每次构造累计偏移。本例不含物理模拟、碰撞扫掠、断流检测或新包计时。

## 3. 变化参数：ProjectDmxMaterialController

完整文件：[头文件](examples/ProjectDmxMaterialController.h)、[实现](examples/ProjectDmxMaterialController.cpp)。采用两条调用链：

```text
NativeLightInitialization → Super → EnsureMaterial → ApplyCachedParameters
NativeSuperDMXChanged → Super → 确认目标/模式/地址 → 读取参数 → ApplyCachedParameters
```

EnsureMaterial、ApplyCachedParameters均是项目函数。与现有材质对象一致，初始化准备MID，Changed读取并更新参数。检查MID的Outer和来源材质变化，避免复制对象或改材质后错误共用旧实例。

导入MaterialControl_6CH.json并赋库，StartAddress=201；TargetActor选择自己的StaticMeshActor，BaseMaterial选择自己的材质，MaterialIndex填实际槽。材质需有ProjectColor（Vector）、ProjectEffect/ProjectSpeed/ProjectWidth（Scalar），并参与材质计算。bEnabled=true后允许在初始化/构造路径应用可逆材质预览。

读取Red/Green/Blue组合线性颜色，Effect映射0..10、Speed映射-10..10、Width映射0..10。这些参数名与范围是本例材质协议，用户自己的材质需要相同语义才可直接采用。

默认缓存是白色、Effect=0、Speed=0.5、Width=0。没有可读输入时引用式读取保持缓存。Property模式不读取DMX；初始化仍可能应用缓存的默认材质参数，不能据此认为Property自动变成业务属性驱动。

## 4. 同一个类的三种职责

一个项目控制对象需要同时控制运动和材质时，可组合上述两种方式：NativeLightInitialization准备资源；NativeSuperDMXTick读取位置/速度并持续应用；NativeSuperDMXChanged读取颜色/档位并更新状态。不要把所有逻辑塞进Changed而导致持续运动停止，也不要给同一参数同时建立重复应用链。

项目Native方法调用一次Super，父类会转发蓝图事件。本文按现有案例先Super再项目逻辑，蓝图先执行。若子蓝图需要使用C++更新后的结果，可由项目声明独立的完成通知，但必须明确它是新增事件，不手动再次调用基类SuperDMXChanged。

## 5. 原始分段读取

在NativeSuperDMXChanged的项目处理函数中：

```cpp
const int32 Raw = GetAttributeRaw8ByIndex(0, TEXT("Mode"), -1);
const FSuperDMXAttributeDef* Def = FindAttributeDef(0, TEXT("Mode"));
const FSubAttribute* Segment = (Raw >= 0 && Def)
    ? Def->FindSubAttribute(Raw) : nullptr;
if (Segment)
{
    const FName NewMode = Segment->Name;
    // 项目：与缓存模式比较，仅变化时调用目标业务函数。
}
```

此段是分段查询示例，不是完整状态机；完整业务需先验地址和运行门控，再维护自己的CurrentMode/Trigger边沿状态，见[蓝图案例四](05_Blueprint_Integration_zh.md)。nullptr必须处理，不缓存Segment跨库修改。

## 6. 多模块读取

在NativeSuperDMXTick中逐模块读Raw16，或用GetMatrixAttributeRaw16WithIndex：Values[k]与Indices[k]成对，后者是原模块下标。先验证目标数组索引及引用，按模块对应目标应用。

矩阵值已归一化；无快照的默认0不能识别失败。需要首次有效数据门控的项目，逐模块ByIndex默认-1更明确。库发生结构变化时重新核对模块与目标映射，不缓存旧结构内裸指针。

## 7. 编译与交付验收

示例是项目源码，不是已编译SDK。需确认UHT、编译、链接通过，编辑器能放置类，默认字段和库引用可保存重载，真实输入触发对应Super事件，再验证目标结果。所有检查完成前不宣称已经接通。

本次未修改插件功能代码，也没有复制非开放模块实现到示例。源码参考表单独维护在内部核对记录，客户只需正式开放接口和匹配交付包。
