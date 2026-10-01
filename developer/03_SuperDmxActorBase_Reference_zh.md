# SuperDmxActorBase.h 逐项参考

[返回目录](README.md) · [事件](02_Super_Events_zh.md) · [通道库](04_SuperFixtureLibrary_zh.md)

核对范围为当前 `SuperCore/Public/SuperDmxActorBase.h` 的全部公开和受保护方法、属性、辅助类型及宏，并以对应实现校正历史注释。读取函数放在Super事件调用链中使用。下面的“蓝图Pure”表示可从相应对象引出纯函数节点，不表示独立事件或恒定值。

## 1. 类与实例属性

`ASuperDmxActorBase : ASuperBaseActor`，反射类名SuperDmxActorBase，模块SuperCore。读取函数的Target通常为自己的控制对象Self；若项目辅助函数接收来源引用，仍由Super事件调用，不建立额外轮询调度。

| 字段 | 默认值 | 配置与含义 |
| --- | --- | --- |
| SuperDMXFixture | FSuperDMXFixture | EditAnywhere、BlueprintReadWrite、Interp，包含以下三项 |
| FixtureID | 1 | 对象ID，不参与字节地址计算 |
| Universe | 1 | 内部Universe，界面约束1..512；须与接收配置映射一致 |
| StartAddress | 1 | 当前Universe内的1基起始通道，1..512 |
| ControlMode | ESuperDMXControlMode::DMX | DMX读取信号；Property使用各读取函数的回退行为，并改变Changed调度条件 |
| FixtureLibrary | nullptr | USuperFixtureLibrary引用；EditAnywhere、BlueprintReadOnly，可编辑器设置，不是运行时蓝图Set变量 |
| bIncludeFixtureIdInLabel | false | 可编辑、蓝图可读写的遗留标签选项；当前地址文字组件已移除，不能据此承诺显示地址标签 |

编辑器工具修改SuperDMXFixture部分字段时要保留其余字段。FixtureLibrary运行期C++可以赋值；就地修改共享库会影响所有引用者。控制模式不会替你创建项目属性驱动逻辑。

## 2. 地址、下标与数值

```text
InstanceIndex = Modules数组下标（0基）
ModuleBase = StartAddress + max(0, Modules[InstanceIndex].Patch - 1)
CoarseAbs = ModuleBase + Coarse - 1   （Coarse > 0）
FineAbs   = ModuleBase + Fine   - 1   （Fine > 0）
UltraAbs  = ModuleBase + Ultra  - 1   （Ultra > 0）
未启用偏移的绝对地址输出为0。
```

Patch按1基模块起点配置；Coarse/Fine/Ultra按1基模块局部位置配置，0表示未用字节。StartAddress=101、Patch=5、Coarse=2、Fine=3时，地址106/107。

源码历史注释中的`StartAddress + Patch`和“Patch为0基”与当前计算不同；新配置一律Patch≥1。当前Patch=0和1恰好落在相同起点，不使用这种别名行为。超出512不会自动进下一Universe。

```text
Raw8  = C
Raw16 = (C << 8) | F
Raw24 = (C << 16) | (F << 8) | U
归一化分母分别为255、65535、16777215。
```

所有读数代表当前可读快照，不附包时间戳。值不变不能说明断线，值为0也不能说明失败。

## 3. ResolveAttributeAddressesByIndex

```cpp
bool ResolveAttributeAddressesByIndex(int32 InstanceIndex, FName AttribName,
    int32& OutCoarseAbs, int32& OutFineAbs, int32& OutUltraAbs) const;
```

**可用性：** 蓝图Pure，Address分类；C++公开。

**作用：** 用模块下标和属性名查库，再计算三个绝对地址，不读取网络值。输入InstanceIndex必须有效，AttribName应在该模块内唯一。输出先全部置0；找不到返回false。找到后仅当Coarse绝对地址>0返回true。

**限制：** true不保证地址≤512，也不保证有数据。AI须另外检查所有启用字节是否处于1..512。示例中模块1的ZPos可以解析到106/107，但必须据实际定义核对。

**场景：** 配接报告、库验收、排查错一位、在读数前校验地址。不要把它当在线检测。

## 4. FindAttributeDef（两个重载）

```cpp
const FSuperDMXAttributeDef* FindAttributeDef(int32 InstanceIndex, FName AttribName) const;
const FSuperDMXAttributeDef* FindAttributeDef(const FSuperDMXAttribute& DmxAttribute) const;
```

**可用性：** 仅C++，无UFUNCTION。

返回库内属性定义指针，找不到为nullptr。第二个重载只取选择器的InstanceIndex、AttribName，忽略DMXChannelType。可读取SubAttributes和PhysicalRange等定义，函数本身不解析业务、不返回DMX值。

返回的是借用指针；不delete，不跨库替换、Modules/AttributeDefs修改长期保存。重复同名定义当前按首先匹配处理，项目应直接拒绝重复配置。FName不要靠大小写区分不同属性。

**场景：** Changed中读取模式属性原始值后，找到对应定义并查询分段。蓝图没有FindAttributeDef原生节点，可从GetModules输出遍历结构，或使用明确标为项目新增的桥接。

## 5. GetAttributeRaw8ByIndex

```cpp
int32 GetAttributeRaw8ByIndex(int32 InstanceIndex, FName AttribName,
    int32 DefaultValue = 0) const;
```

蓝图Pure，Read8分类。只读取Coarse字节，成功0..255，即使有Fine也不拼16位。Property、属性解析失败、Universe无快照返回DefaultValue。**有快照但地址越界时返回0，而非DefaultValue。**默认值本身不限制在0..255，因此可显式传-1识别部分失败，但仍必须先校验地址。

**场景：** Changed中的按钮/档位，SuperDMXTick中的8位目标参数。Raw=128归一化为约0.50196。不要把Raw直接用作0..1材质参数。

## 6. GetAttributeRaw16ByIndex

```cpp
int32 GetAttributeRaw16ByIndex(int32 InstanceIndex, FName AttribName,
    int32 DefaultValue = 0) const;
```

蓝图Pure，Read16分类。要求Coarse、Fine同时存在，输出高粗低细拼接整数0..65535。无Fine时返回DefaultValue，**不降为8位**。Property、解析失败、无快照也返回默认值。已有快照中某个字节越界，该字节以0拼接，不一定整值为0。

**场景：** 现有机械Fine属性的蓝图显式回退读取。C=128/F=0得到32768；C=0/F=255得到255；C=1/F=0得到256，用后两组验收字节顺序。

## 7. GetAttributeRaw24ByIndex

```cpp
int32 GetAttributeRaw24ByIndex(int32 InstanceIndex, FName AttribName,
    int32 DefaultValue = 0) const;
```

蓝图Pure，Read24分类。Coarse必须能解析；Fine或Ultra缺失时用0填相应字节，仍按24位权重组合，0..16777215。Property、解析失败、无快照返回DefaultValue。有快照中的越界字节也为0。

**场景：** 项目协议确实使用三字节的参数。仅Coarse=128而无低字节时返回8388608，不是128；不能以此函数给8位协议“提高精度”。协议要求完整24位时，先检查三字节定义齐全。

## 8. GetAttributeBitDepthByIndex

```cpp
int32 GetAttributeBitDepthByIndex(int32 InstanceIndex, FName AttribName) const;
```

蓝图Pure，Info分类。按能解析到的字节位置返回0/8/16/24：失败0，C+F+U为24，C+F为16，C为8。不检查数据是否到达，不验证上界。C+U但没有F会报告8；此异常布局应在库校验时拒绝。

**场景：** 验证ZPos确实配置为16位，选择正确读取函数；不是信号质量指标。

## 9. GetSuperDmxAttributeValue

```cpp
void GetSuperDmxAttributeValue(const FSuperDMXAttribute& DmxAttribute,
    float& InOutDefault) const;
```

蓝图Pure，Read分类。选择器包含InstanceIndex、AttribName、DMXChannelType。Coarse返回Raw8/255，Fine返回Raw16/65535（要求Fine存在），Ultra返回Raw24/16777215（可补零）。未识别枚举值的当前实现退回8位；项目不产生无效枚举。

**C++失败语义：** Property、解析失败、无快照、Fine模式缺少Fine时不修改引用。现有机械把PosX等缓存变量传入，此行为使读取失败时保留其已有值。例：`float Z=0.25f; GetSuperDmxAttributeValue(Query,Z);`失败后Z仍0.25。有快照而字节越界仍按0读取，因此配置校验不能省。

它只归一化，不应用DefaultValue百分比、不做SubAttribute查找、不将PhysicalRange映射到厘米或度，也不插值。之后由项目Clamp、Lerp、应用。

**蓝图引脚：** 非const引用没有UPARAM(ref)。不能把参数名InOutDefault当作“必有默认值输入引脚”的证据。AI必须查询实际节点引脚；通常这是输出引用。需要明确失败回退的蓝图案例用ByIndex的DefaultValue，结果成功后再更新缓存变量。两种写法都在Super事件中执行。

## 10. GetSuperDmxAttributeValueNoConversion

```cpp
void GetSuperDmxAttributeValueNoConversion(const FSuperDMXAttribute& DmxAttribute,
    float& InOutDefault) const;
```

蓝图Pure，Read分类。选择位深和失败规则同上，但返回float表示的原始整数：0..255、0..65535、0..16777215。NoConversion指不归一化，不代表物理量。

**场景：** 按原始范围解析业务命令；希望保留上次值的C++逻辑。蓝图同样需核实引用引脚，不虚构失败布尔输出。

## 11. GetSuperDmxAttributeRawValue

```cpp
void GetSuperDmxAttributeRawValue(const FSuperDMXAttribute& DmxAttribute,
    int32& OutRawValue) const;
```

蓝图Pure。先把输出置0，失败也是0。**始终只读Coarse，忽略DMXChannelType**。选择器写Fine也不会得到16位。适合只需要粗字节的业务；需要区分失败或多字节时用ByIndex。

不要把此函数的0判作“已收到命令关闭”。0既可能是真实值，也可能是配置/输入失败。

## 12. GetSuperDMXColorValue

```cpp
void GetSuperDMXColorValue(const FSuperDMXAttribute& DmxRed,
    const FSuperDMXAttribute& DmxGreen, const FSuperDMXAttribute& DmxBlue,
    FLinearColor& OutColor) const;
```

蓝图Pure。分别对三个分量调用归一化属性读取，最后A=1。三个选择器可各有模块及位深，必须自己明确配对。C++失败的分量保留传入颜色已有分量；必须先初始化OutColor。Property模式RGB不变但A仍置1。

不做gamma转换、发射亮度或颜色校准。现有材质案例把所得Color写入动态材质Vector参数。蓝图若要逐通道可控回退，可用三个ByIndex读数构造LinearColor。

## 13. GetChannelValue

```cpp
float GetChannelValue(int32 Address = 1, float DefaultValue = 1) const;
```

蓝图Pure，Read8分类。Address是相对StartAddress的1基位置，绝对地址=StartAddress+Address-1；**不使用库的Patch和属性定义**。返回float形式的0..255。无数据、Property或绝对地址无效返回DefaultValue，默认是1，不是0。

**场景：** Super事件中检查发送端的某个固定通道或临时诊断。正式命名控制更适合按库查询。调用端必须保证Address≥1；当前实现只验证最终索引，因此0/负数在某些StartAddress下可能读到前面的通道，不能当合法用法。

GetChannelValue本身不需要库，但不创建正常Changed检测范围。需要持续诊断时使用SuperDMXTick，不添加Event Tick。

## 14. GetMatrixAttributeRaw

```cpp
void GetMatrixAttributeRaw(FName AttribName, TArray<float>& OutValues,
    float DefaultValue = 0, bool bSortByPatch = true) const;
```

蓝图Pure，Matrix分类。遍历所有模块，读取同名属性的Coarse并除255。**返回0..1，虽然名字含Raw。**缺少属性或Coarse的模块跳过。

输出数组每次清空。Property模式或实现判定为无效的FName返回空数组。项目禁止NAME_None，不依赖空名字的特殊查找。无快照时每个符合定义的模块输出DefaultValue，且默认值也Clamp到0..1，所以-1不会保留为哨兵。

默认按Patch升序排序；bSortByPatch=false保持原模块顺序但仍过滤。输出数组下标不一定等于模块下标。已有快照中越界字节按0读取。同Patch的排序稳定性不作为业务保证。

**场景：** 多个定义一致的目标同时读取一个8位参数；项目需建立清楚的目标顺序。无需再次除255。

## 15. GetMatrixAttributeRaw16

```cpp
void GetMatrixAttributeRaw16(FName AttribName, TArray<float>& OutValues,
    float DefaultValue = 0, bool bSortByPatch = true) const;
```

蓝图Pure。与上节流程一致，但模块必须同时含Coarse/Fine，返回Raw16/65535。没有Fine的模块跳过，不用默认值占位。其他回退、排序、清空规则相同。

**场景：** 多平台Fine位置输入。若模块不一致，不用过滤后的数组直接索引目标；蓝图逐模块ByIndex更容易保持目标身份。

## 16. GetMatrixAttributeRawWithIndex / GetMatrixAttributeRaw16WithIndex

```cpp
void GetMatrixAttributeRawWithIndex(FName AttribName, TArray<float>& OutValues,
    TArray<int32>& OutModuleIndices, float DefaultValue = 0) const;
void GetMatrixAttributeRaw16WithIndex(FName AttribName, TArray<float>& OutValues,
    TArray<int32>& OutModuleIndices, float DefaultValue = 0) const;
```

仅C++，无UFUNCTION。各自为8/16位归一化矩阵读取，始终按Patch排序。两个数组每次清空且平行：Values[k]属于Modules[Indices[k]]，Indices存原模块下标。

例：Modules原顺序为Patch9含ZPos、Patch1含ZPos、Patch5无ZPos，输出模块下标[1,0]，不含第三模块。**场景：** C++多目标控制避免排序后身份错位。只返回值而无有效性标记，不能据默认0判断输入在线。

## 17. GetModules

```cpp
const TArray<FSuperDMXModuleInstance>& GetModules() const;
```

蓝图Pure，Info分类。有FixtureLibrary时返回其Modules，无库返回静态空数组。C++是只读引用，不删除、不const_cast修改，换库/数组变更后重新取得。

**场景：** AI配置读回、蓝图遍历属性及分段、枚举目标模块。它不会创建模块，不返回互斥模式的“当前激活项”；全部模块同时属于该库配置。

## 18. GetFixtureChannelSpan

```cpp
int32 GetFixtureChannelSpan() const;
```

蓝图Pure，Info分类。统计全部模块中>0的C/F/U绝对地址，返回Max-Min+1；没有通道返回0。包括中间空隙，既不是属性个数，也不总是最大相对通道号。

只有相对通道5和8时Span=4。不能用StartAddress+Span-1推断最后通道，因为最前面可能有空隙。**场景：** 辅助配接及Changed范围诊断；完整边界仍逐项解析。

## 19. ForceRefreshDMX

```cpp
void ForceRefreshDMX();
```

蓝图Callable，Control分类。主动进入一次NativeSuperDMXTick，Property直接Changed、DMX执行变化检测，再标记组件渲染状态；编辑器路径有PostEditChange。

不取新网络包、不保证DMX模式Changed、不主动强制全视口重绘。仅用于明确的编辑器刷新/诊断动作，**不在Super事件内部调用，不每帧手动调用**。编辑器可能再次构造，项目初始化须能重复执行。

## 20. 生命周期及六个事件函数

| 函数 | 当前实现与项目用法 |
| --- | --- |
| ASuperDmxActorBase() | 构造体当前为空，父类创建SceneBase并具备调度能力；不在构造器读取实时信号 |
| BeginPlay() | 父类BeginPlay后NativeLightInitialization；项目一般在Super初始化事件组织逻辑 |
| Tick(float DeltaTime) | 基类调度NativeSuperDMXTick，再按模式处理Changed；项目不重写它作为DMX入口 |
| ShouldTickIfViewportsOnly() const | 返回true，允许视口更新；不保证所有暂停/后台条件都持续执行 |
| OnConstruction(const FTransform&) | protected；父类后顺序调用初始化、SuperTick、Changed，均可反复发生 |
| LightInitialization() | protected蓝图实现事件；初始化组件/资源 |
| SuperDMXTick() | protected蓝图实现事件；连续读取与应用 |
| SuperDMXChanged() | protected蓝图实现事件；变化参数和业务状态 |
| NativeLightInitialization() | protected C++ virtual；默认调用LightInitialization |
| NativeSuperDMXTick() | protected C++ virtual；默认调用SuperDMXTick |
| NativeSuperDMXChanged() | protected C++ virtual；默认调用SuperDMXChanged |

三个事件均无参数。项目Native override按现有案例先调用Super，再进入自己的处理函数。所有详细调度条件及业务去重见[事件章节](02_Super_Events_zh.md)。

## 21. GetFixtureModelId与辅助结构

```cpp
virtual FSuperFixtureModelId GetFixtureModelId() const;
```

仅C++，基类返回当前UClass和空FGuid。用于产品对象身份分组，不参与DMX读取；本套对象控制案例不需重写它。

头文件FSuperFixtureModelId不是USTRUCT：ActorClass默认null，DefinitionGuid默认无效；有默认构造及(UClass*,const FGuid&)构造。IsValid只看类非空；IsDataDriven看Guid有效；==比较两个字段、!=取反；GetTypeHash组合类与Guid。ToKeyString返回类路径，有Guid时加`#`与Digits格式Guid；无类名用UnknownClass。

该身份不是场景实例ID，不用来映射TargetActor，也不存在蓝图Make FSuperFixtureModelId节点。说明此类型是为了覆盖公开头文件，不要求项目扩展产品型号系统。

## 22. 四个C++宏

### FOREACH_LOOP(ArrayVar, ElementVar, IndexVar, LoopBody)

以const引用保存数组，int32下标从0迭代，元素为const引用，执行LoopBody。不能在循环中改变数组结构。它只是C++宏，无蓝图节点，不提供DMX读取。

### GET_SUPER_DMX_MATRIX_VALUE(DMXAttribute, DefaultValue, LoopBody)

Fine选择GetMatrixAttributeRaw16，其他值选择GetMatrixAttributeRaw，**Ultra也走8位**。对每个返回值赋给DefaultValue，并在LoopBody提供LightIndex。值已归一化，LightIndex是过滤排序后的下标；查询的InstanceIndex不限制模块。DefaultValue在此是会被反复改写的变量，不是常量。

### GET_SUPER_DMX_MATRIX_VALUE_WITH_SUBATTR(DMXAttribute, DefaultValue, LoopBody)

使用WithIndex取得原模块，提供LightIndex、DmxRawValue、AttrDef、SubAttr。AttrDef由原模块查找，SubAttr可为空。**DmxRawValue始终是归一化值乘255再转int，Fine也压到8位**；不适合保留16位区间精度。分段查找不自动检查ModeMaster。需要精准项目命令时显式读Raw16再查段。

### GET_SUPER_DMX_MATRIX_RGB(DmxR,DmxG,DmxB,OutColor,LoopBody)

独立读取三个8位归一化矩阵数组，按三者最大长度循环，不足项补0，写OutColor=(r,g,b,1)。忽略选择器的高位深及模块限制。三个数组独立过滤，缺少属性时可能错位；只有模块属性完整一致并明确目标顺序时适用。

普通对象接入的示例采用显式循环，便于AI核对目标、位深和缺项。宏语义必须了解，不能仅凭名字当成“自动完成多对象映射”。

## 23. 私有成员与实现差异

DetectAndFireDMXChanged、EnsureAddressCache、RebuildAddressCache、FindResolvedAttribute、CollectMatrixAttribute以及缓存结构和PreviousChannelSnapshot均为private。外部不调用、不反射改写这些内部字段。

地址缓存按FixtureLibrary对象、库EditVersion和StartAddress变化重建。直接修改库后用公开BumpEditVersion通知；普通读者不用管理缓存。Universe不是缓存/快照身份键，换Universe且值相同不能期待Changed必触发。

本文特意区分声明注释与行为：Patch公式以实现为准；矩阵Raw实为归一化；Raw8越界不总返回DefaultValue；RawValue忽略位深；Changed含构造和Property路径；地址标签不再生成。AI执行时以这些明确规则及当前安装版本读回结果为准。
