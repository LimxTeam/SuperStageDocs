# SuperFixtureLibrary：完整字段、原生JSON与资产创建

[返回目录](README.md) · [现成JSON文件](examples/README.md)

## 1. 通道库是协议数据

`USuperFixtureLibrary`（`AssetTool/SuperFixtureLibrary.h`）是UPrimaryDataAsset。它规定一个控制对象有哪些模块、每个模块有哪些属性，以及属性字节相对位置。它不自动移动Actor、调用业务函数、应用PhysicalRange或创建显示对象。

最小结构：

```text
USuperFixtureLibrary
  Modules[0] : FSuperDMXModuleInstance
    ModuleName
    Patch
    AttributeDefs[] : FSuperDMXAttributeDef
      AttribName / Coarse / Fine / Ultra
      SubAttributes[] : FSubAttribute
        DmxMin / DmxMax / PhysicalRange
        ChannelSets[] : FChannelSet
```

Modules中的全部元素同时参与读数和跨度计算，**不是互斥模式列表**。新建工厂默认项虽然叫Mode 1，也只是数组中的一个模块。不同互斥协议应保存成不同库，选择对应资产。

## 2. 使用与真实机械代码一致的12通道定义

现有机械读取以下名称，全部Fine。本文提供[Motion6Axis_12CH.json](examples/libraries/Motion6Axis_12CH.json)，按实际名称和字节顺序编写；它是项目协议文件，不冒充从二进制uasset导出的原文件。

StartAddress=101，Modules[0].Patch=1：

| AttribName | Coarse | Fine | Ultra | 绝对地址 | 项目用途 |
| --- | --- | --- | --- | --- | --- |
| XPos | 1 | 2 | 0 | 101/102 | 归一化X位置 |
| YPos | 3 | 4 | 0 | 103/104 | 归一化Y位置 |
| ZPos | 5 | 6 | 0 | 105/106 | 归一化Z位置 |
| XRot | 7 | 8 | 0 | 107/108 | 归一化旋转控制 |
| YRot | 9 | 10 | 0 | 109/110 | 归一化旋转控制 |
| ZRot | 11 | 12 | 0 | 111/112 | 归一化旋转控制 |

项目自己决定归一化值映射到多少厘米/度。不要把XRot等名称擅自等同于某个UE旋转引脚；现有机械将RotX用于Pitch、RotY用于Yaw、RotZ用于Roll。改编控制自己对象时，明确填写实际轴的映射。

单模块Patch=1，属性偏移从1开始。第二个同结构模块若紧跟前一个，Patch=13，属性仍从1开始；不重复给属性加12。

## 3. 创建资产并导入JSON

1. 在自己的Content目录，例如`/Game/ProjectDMX/`，创建 **Super Fixture Library** 资产；原生工厂归于SuperStage资产分类。也可使用编辑器支持的数据资产创建能力指定类`/Script/SuperCore.SuperFixtureLibrary`。不要创建同名蓝图类冒充数据资产。
2. 命名`DA_Motion6Axis_12CH`并打开库编辑器。工厂会给FixtureName初值、Manufacturer=Unknown和一个Patch=1的默认模块。
3. 使用库编辑器的JSON导入操作，选择本文JSON文件。现有模块存在时，界面会提示导入覆盖；在本例新建资产上完成导入。已有用户库先确认是本次要替换的资产。
4. 导入会清空并重建Modules，不是追加。成功提示只证明解析器接受模块，不代表地址/名称/位深都合法。保存资产后读回全部模块、偏移及分段。
5. 在自己的DMX控制对象上赋`FixtureLibrary=DA_Motion6Axis_12CH`，设置内部Universe、StartAddress、ControlMode=DMX，保存蓝图默认值或场景实例及关卡。
6. 用ResolveAttributeAddressesByIndex检查表中每个地址；GetFixtureChannelSpan应为12。StartAddress最大可用值为501，502会让最后通道超出512。
7. 输入105/106=128/0，ZPos Raw16应为32768，归一化约0.5000076。再让Super DMX Tick应用到自己的目标。

UE5MCP可以使用其实际支持的资产编辑能力执行相同动作。需要先发现工具；若无JSON导入UI操作能力，可使用反射编辑库的嵌套结构数组并保存。不能虚构一个固定“import_superfixturelibrary”工具。项目C++不include或调用非开放SuperTools导入器。

## 4. 原生JSON语法与导入边界

字段名大小写按现有解析器，JSON不写注释。基本完整例子：

```json
{
  "FixtureName": "Project Z Position 2CH",
  "Manufacturer": "Project",
  "PowerConsumption": 0,
  "Weight": 0,
  "BeamAngle": 0,
  "BeamIntensity": 0,
  "BodySize": [0, 0, 0],
  "MA2FixtureLibraryPath": "",
  "GDTFFixtureLibraryPath": "",
  "Modules": [
    {
      "ModuleName": "Main",
      "Patch": 1,
      "AttributeDefs": [
        {
          "AttribName": "ZPos",
          "Category": "Position",
          "Coarse": 1,
          "Fine": 2,
          "Ultra": 0,
          "SubAttributes": [
            {"Name": "Travel", "DmxMin": 0, "DmxMax": 65535,
             "PhysicalRange": [0, 300]}
          ]
        }
      ]
    }
  ]
}
```

此例只有ZPos两通道，与12通道协议里的ZPos偏移5/6不同，不能混用地址表。

解析器对属性直接读取AttribName、Category、Coarse，生成文件必须给齐。模块建议明确ModuleName、Patch、AttributeDefs。根Modules必须至少一个元素才能返回成功；空属性数组仍可能通过，AI必须额外验收。

属性可选键包括Fine、Ultra、DmxBreak、**VirtualChannel**、Geometry、NativeResolutionBytes、DefaultValue、HighlightValue及四个Raw默认/高亮精度字段。C++字段是bVirtualChannel，JSON键不是bVirtualChannel。

子属性必须提供DmxMin/DmxMax；可选Name、NativeResolutionBytes、NativeDmxMin/Max、WheelName、StrobeMode、GoboMode、RotationMode、PhysicalRange和ChannelSets。ChannelSet提供Name、DmxMin、DmxMax，可带相应模式/精度、PhysicalRange、Color、ColorIndex、PrismSelection、WheelSlotIndex。

**不要把整个头文件当作JSON自动序列化schema。** 当前原生解析器未完整覆盖头文件中的GDTFAttribute、bPhysicalExplicit、DeclaredColor/bHasDeclaredColor、ModeMaster字段、Texture对象引用、GeometryReferences等；写进JSON不能证明已导入。需要这些字段时通过资产编辑并读回，或确认交付版本支持。本文JSON只用已经核对的字段。

原生导入器会清除属性构造时的默认子属性，再加入JSON数组；如果数组为空/缺失，重新补一个0..255默认段。**Fine属性省略分段仍会得到0..255段，不会自动扩成0..65535。** 需要16位分段时显式填写。

## 5. 库顶层字段

| 字段 | 意义 / 项目规则 |
| --- | --- |
| FixtureName | 协议显示名称，不作为对象地址 |
| Manufacturer | 实际项目/团队标识 |
| Modules | 所有同时生效的逻辑模块 |
| MA2FixtureLibraryPath | 已弃用兼容字段，资产面板只读；新项目协议留空 |
| GDTFFixtureLibraryPath | 绑定的真实外链文件路径；自定义对象协议不要求有GDTF，留空即可 |
| PowerConsumption | 瓦数元数据，默认540；与自有对象无关时本文设0 |
| Weight | 千克元数据，默认21.2；不用则设0 |
| BeamAngle、BeamIntensity | 度/流明元数据，不驱动自己模型；本例为0 |
| BodySize | 米制尺寸，默认(0.6,0.6,0.6)，不自动缩放目标Actor |
| GeometryReferences | 来源几何引用元数据；普通对象控制不需填写，不是TargetActor数组 |

GeometryReferences的结构字段为Name、TemplateGeometry、DMXBreak、DMXOffset、PositionX、ParentGeometry，分别表示引用名、模板名、Break、1基偏移、米制X位置、父几何。通过头文件解释其数据含义，不扩展为本案例要求。

顶层字段为EditAnywhere/BlueprintReadOnly居多；“蓝图只读”与“编辑器不能改”不同。运行时蓝图没有自动生成Set Modules或Set FixtureLibrary节点。

## 6. 模块与属性字段

FSuperDMXModuleInstance：ModuleName是FName；Patch默认1；AttributeDefs数组定义属性。实际地址公式详见[函数参考](03_SuperDmxActorBase_Reference_zh.md)。

FSuperDMXAttributeDef：

| 字段 | 默认 / 作用 |
| --- | --- |
| AttribName | NAME_None；必须改为模块内唯一名称，不用大小写区分 |
| Category | Other；分类不自动产生行为 |
| Coarse | 1；1基粗字节偏移 |
| Fine、Ultra | 0；未使用，高精度协议设置具体位置 |
| DmxBreak | 1；基础读API不据此跨Universe |
| bVirtualChannel | false；标记不自动跳过偏移或生成计算通道 |
| Geometry、GDTFAttribute | 来源信息，自定义协议无来源保持空 |
| NativeResolutionBytes | 0推断，1/2/3/4为来源字节精度；不会使基础读API支持32位 |
| DefaultValue、HighlightValue | UI范围0..100百分比；不是读取失败DefaultValue，不自动发到输入或写缓存 |
| DefaultValueRaw、HighlightValueRaw | double，默认-1表示从百分比推导；用于精度保留 |
| DefaultValueRawResolutionBytes、HighlightValueRawResolutionBytes | 0使用通道精度，否则记录对应原始值精度 |
| SubAttributes | 分段定义，构造时默认一个0..255段 |

Category可用枚举：Dimmer、Position、Gobo、Color、Beam、Focus、Control、Shapers、Strobe、Prism、Frost、Effects、Other。普通位置用Position，颜色用Color，业务命令用Control。

UI约束不是完整运行校验：重叠通道、Coarse/Fine重复、空名、负值、超512仍须项目检查。原始字节通常不应被两个独立参数同时占用；确需别名时必须协议明确，本文示例不使用。

## 7. SubAttributes与物理量

FSubAttribute的DmxMin/DmxMax为闭区间；Name是段标识；PhysicalRange为起止物理量，单位由项目协议定义。核心方法均为C++内联，未声明蓝图函数：

| 方法 | 行为 |
| --- | --- |
| Contains(Raw) | Min≤Raw≤Max |
| GetNormalizedPosition(Raw) | (Raw-Min)/(Max-Min)并Clamp0..1；Max≤Min返回0 |
| GetMappedPhysical(Raw) | 在PhysicalRange.X/Y之间Lerp |
| FindChannelSet(Raw) | 返回首个包含Raw的槽位，否则nullptr |
| HasModeMaster() | 主属性非空且ModeTo≥ModeFrom即true；不读取主控数据 |

FSuperDMXAttributeDef::FindSubAttribute返回首个匹配段；FindChannelSet先找段再找其槽位。**这两种查找不自动执行ModeMaster条件。** 项目命令段建议不配置条件主控，或自行明确读取、匹配和优先级。

例：ZPos段0..65535，PhysicalRange=[0,300]，Raw32768得到150.0023厘米。GetSuperDmxAttributeValue只给归一化值，必须另行Lerp或调用GetMappedPhysical才得到厘米。

C++创建分段时先`Def.SubAttributes.Reset()`，否则构造器0..255默认段可能抢先匹配。蓝图/编辑器手工编辑同理，替换默认项再建立自己的段。

子属性其余字段为GDTFAttribute、WheelName、GoboMode、StrobeMode、RotationMode、bPhysicalExplicit、NativeResolutionBytes、NativeDmxMin/Max、DeclaredColor、bHasDeclaredColor、ChannelSets及ModeMasterGeometry/ModeMasterAttribute/ModeFrom/ModeTo。它们存来源、专用模式、精度、颜色和条件元数据；基础读API不会自动把这些解释成目标行为。NativeDmx边界默认-1；ModeTo默认-1表示无有效区间。

## 8. ChannelSets与业务模式

FChannelSet有Name、DmxMin/Max、PhysicalRange及同样的Contains/归一化/物理映射。单点段Min=Max返回归一化0，物理值取X。其余GoboMode、StrobeMode、bPhysicalExplicit、NativeResolutionBytes、NativeDmxMin/Max、Texture、Color、ColorIndex、PrismSelection、WheelSlotIndex属于附加元数据。WheelSlotIndex为1基，0未指定。

自己的Mode属性可定义0..63 Idle、64..191 Preview、192..255 Run；作为三个SubAttributes，或在一个段中使用三个ChannelSets，消费逻辑须对应。不要同时维护两套不一致区间。63/64、191/192是必测边界。

数组顺序决定首次匹配，重叠不是自动优先级。缺口返回nullptr，项目声明保持或回退策略。`Mode`、`Trigger`是本文项目协议名称，只有实际创建库定义后才能读到。

## 9. 库方法与缓存更新

GetChannelSpan()是蓝图Pure，统计所有正偏移的Max-Min+1，含空隙，无通道返回0。GetPrimaryAssetId()仅C++，类型名SuperFixtureLibrary、名称为资产对象名；返回ID不保证自动Cook。

GetEditVersion()/BumpEditVersion()仅C++，分别读取/递增非序列化编辑版本。编辑器PostEditChangeProperty会递增；项目C++直接改数组后须BumpEditVersion。函数未限制在编辑器宏内，运行时就地修改也需要通知。不要每次Super事件重建或修改库。

FSuperDMXAttribute是查询选择器，不是定义：InstanceIndex=0、AttribName=NAME_None、DMXChannelType=Coarse。`MakeDmx(FName,bool bFine=false)`是C++辅助函数，设置名称和8/16位，模块下标仍0；不是蓝图节点。24位及其他模块显式写结构字段。

## 10. C++创建临时库的精确操作

```cpp
#include "AssetTool/SuperFixtureLibrary.h"

// 项目函数：调用一次后由控制对象的FixtureLibrary属性持有引用。
USuperFixtureLibrary* CreateProjectZLibrary(UObject* Owner)
{
    if (!IsValid(Owner)) return nullptr;
    auto* Library = NewObject<USuperFixtureLibrary>(Owner);
    Library->FixtureName = TEXT("Project Z Position 2CH");
    Library->Manufacturer = TEXT("Project");
    Library->PowerConsumption = 0;
    Library->Weight = 0;
    Library->BeamAngle = 0;
    Library->BeamIntensity = 0;
    Library->BodySize = FVector::ZeroVector;
    FSuperDMXModuleInstance Module;
    Module.ModuleName = TEXT("Main");
    Module.Patch = 1;
    FSuperDMXAttributeDef Z;
    Z.AttribName = TEXT("ZPos");
    Z.Category = EDMXAttributeCategory::Position;
    Z.Coarse = 1;
    Z.Fine = 2;
    Z.SubAttributes.Reset();
    FSubAttribute Travel;
    Travel.Name = TEXT("Travel");
    Travel.DmxMin = 0;
    Travel.DmxMax = 65535;
    Travel.PhysicalRange = FVector2D(0, 300);
    Z.SubAttributes.Add(Travel);
    Module.AttributeDefs.Add(Z);
    Library->Modules.Add(Module);
    Library->BumpEditVersion();
    return Library;
}
```

这是项目侧临时对象，不是已保存uasset，不自动出现在Content Browser。正式可重载配置优先使用编辑器资产加JSON；不要把临时对象路径当持久资产引用。共享库修改会影响全部引用对象，需要独立协议时使用独立库。

## 11. 协议验收与持久化

检查名称唯一、字节无意外重叠、Patch≥1、Fine/Ultra位深一致、全部绝对地址1..512、分段单位与读取位深一致。保存库与引用对象，重载后读回；软引用加载和Cook由项目安排。

跨版本升级重新检查导入字段、导出结果和地址。原生JSON导出可能对重复/空属性名做规范化，不能利用导出自动修补有歧义协议。先修正资产再交付发送端地址表。
