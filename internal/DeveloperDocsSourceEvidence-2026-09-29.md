# 开发文档重写：源码依据与静态验收（内部）

日期：2026-09-29。当前插件版本标识26H2.7。本文不发布到客户开发文档站。

旧developer文档及七个旧C++文件已删除，重新建立文档与项目示例。用户要求DMX驱动使用Super自身事件；外部普通Actor由Super控制对象引用并驱动，不给目标建立引擎Tick轮询。

## 实际参考调用点

| 文件（路径相对插件根） | 证据 | 文档用途 |
| --- | --- | --- |
| Source/SuperAssets/Private/StageMisc/SuperLiftingMachinery.cpp:92 | NativeSuperDMXTick调用Super后ReadDMXAndApply；125行从World取DeltaSeconds | 连续读取、归一化、映射、插值和应用均在Super事件链；该类Tick仅调用父类，无DMX轮询 |
| Source/SuperAssets/Private/StageMisc/SuperRailMachinery.cpp:124 | NativeSuperDMXTick→ReadDMXAndApply | 相同连续控制入口 |
| Source/SuperAssets/Private/StageMisc/SuperLightStripEffect.cpp:79 | 初始化准备默认材质，85行Changed→ReadDMXAndApply | 材质初始化及变化参数路径；额外Tick中的频闪计算不作为项目DMX入口 |
| Source/SuperAssets/Private/SuperLight/SuperMachinery.cpp:157 | 初始化；168行Changed更新参数；183行SuperTick持续升降 | 事件职责分工，不发布该模块专用扩展教程 |
| Source/SuperAssets/Private/StageMisc/SuperScreen.cpp:119 | OnActiveTextureChanged→GetActiveTexture→MID | NDI应用方式 |
| Source/SuperAssets/Private/StageMisc/SuperProjector.cpp:96 | 一个活动纹理应用多个MID | 多材质消费 |
| Source/SuperTools/Private/FixtureLibrary/JsonFixtureImporter.cpp | 精确解析支持的键；清Modules；空子属性补0..255 | 提供真实可解析格式而不是自造schema |
| Source/SuperDMX/Private/SuperDMXSubsystem.cpp:627 | Universe-InputStartUniverse+1 | 内部Universe映射 |

## 与历史注释的差异

Patch实际StartAddress+max(0,Patch-1)；矩阵Raw输出已归一化；Raw8越界已有快照时返回0；RawValue仅粗字节；Changed包含构造及Property路径；媒体同尺寸帧不触发引用变化钩子。JSON解析并未覆盖整个结构头文件，尤其ModeMaster/GDTFAttribute/DeclaredColor等字段不可盲写后宣称导入成功。

## 交付包注意

SuperMediaBase.h直接包含非开放SuperNdi模块头，正式只开放SuperCore头的交付包必须验证完整包含链和链接。本文不复制该模块头给客户，不修改产品API，不宣称客户编译已通过。父类授权行为保留；示例不绕过授权。

## 静态检查

- 所有新文档相对链接及代码围栏检查通过（50个相对链接）。
- 两组DMX源码和媒体示例无引擎Tick重写或Super::Tick调用，无非开放模块include。
- 四份JSON通过语法、已实现字段名、唯一属性名、字节位置、非重叠和区间检查；跨度2、6、12、6。
- 公共/受保护方法从SuperDmxActorBase实现及三个蓝图事件提取，逐项在03章找到；重载由对应章节分别解释。
- 旧文档与旧类名无残留引用。
- 未运行UE内导入、UHT/编译/链接、UE5MCP实操、真实DMX/NDI或打包测试。

## 本次方法覆盖

`ASuperDmxActorBase`, `BeginPlay`, `FindAttributeDef`, `ForceRefreshDMX`, `GetAttributeBitDepthByIndex`, `GetAttributeRaw16ByIndex`, `GetAttributeRaw24ByIndex`, `GetAttributeRaw8ByIndex`, `GetChannelValue`, `GetFixtureChannelSpan`, `GetFixtureModelId`, `GetMatrixAttributeRaw`, `GetMatrixAttributeRaw16`, `GetMatrixAttributeRaw16WithIndex`, `GetMatrixAttributeRawWithIndex`, `GetModules`, `GetSuperDMXColorValue`, `GetSuperDmxAttributeRawValue`, `GetSuperDmxAttributeValue`, `GetSuperDmxAttributeValueNoConversion`, `NativeLightInitialization`, `NativeSuperDMXChanged`, `NativeSuperDMXTick`, `OnConstruction`, `ResolveAttributeAddressesByIndex`, `ShouldTickIfViewportsOnly`, `Tick`, `LightInitialization`, `SuperDMXTick`, `SuperDMXChanged`

## 核对源码指纹

| 文件 | SHA-256 |
| --- | --- |
| `Source/SuperAssets/Private/StageMisc/SuperLiftingMachinery.cpp` | `69f9883d7b6a50c317fb69b866cd07477b25181f0e354c99e9c87c98c4988ea1` |
| `Source/SuperAssets/Private/StageMisc/SuperRailMachinery.cpp` | `614c13810621b71d9d525724a241cf79df96eba5d48f273aefd0e633857f0fab` |
| `Source/SuperAssets/Private/StageMisc/SuperLightStripEffect.cpp` | `c2c91b0f9afe154f575855df121193075f696c876f6960aacc2decc59c5446a7` |
| `Source/SuperAssets/Private/SuperLight/SuperMachinery.cpp` | `4e8bad08d3289ca0c1efb9bc2741d49d56a3b1221687cd623395925bcc4adbef` |
| `Source/SuperAssets/Private/StageMisc/SuperScreen.cpp` | `92c1beb934884a03783ee2b31e7bbfca954062feac955e9fb868b3d4a7001782` |
| `Source/SuperAssets/Private/StageMisc/SuperProjector.cpp` | `d7938eac193d5fcd239f4fc397f158147cfbcfe45d98652c5b1999504a523128` |
| `Source/SuperCore/Public/SuperDmxActorBase.h` | `33ef5be8addf9381220d53f9cd3c67d5a9bc1f786275a73dd64830f6378d449b` |
| `Source/SuperCore/Private/SuperDmxActorBase.cpp` | `e4e439019de7739d785f250e1cf00b4e980da8f0c4ec195606ba4dc12c82b616` |
| `Source/SuperCore/Public/AssetTool/SuperFixtureLibrary.h` | `a9b606122aa5b73e0f348a1c686bf690f956fb7dc0f08783f06af86c988c7708` |
| `Source/SuperCore/Private/AssetTool/SuperFixtureLibrary.cpp` | `f0dfe4d0c7916f38866746be917b16bfdfa6d479a81dceb5c274c91d3fce56bd` |
| `Source/SuperCore/Public/SuperMediaBase.h` | `f2bcd1170d8c9d4a3f34eb5dcc09956b8a0abcd2b74c8febbe810a563a9342aa` |
| `Source/SuperCore/Private/SuperMediaBase.cpp` | `2779fc4a37493f8f0197581a46d06d433f0116bddc2797dc9aeaf93f8c8d928c` |
| `Source/SuperTools/Private/FixtureLibrary/FixtureLibraryFactory.cpp` | `f42c7aa8421612981ef625ed8a28d031dd2843bc08bbf129b8a8d5324c125147` |
| `Source/SuperTools/Private/FixtureLibrary/JsonFixtureImporter.cpp` | `9c43e704b2430caabf5a97b3c31c2eb50f9d9c94677ff12b4957e0430525cbbd` |
| `Source/SuperTools/Private/FixtureLibrary/SFixtureLibraryEditor.cpp` | `297d81411667d6499655e9c4fb2c0a03706289c34fd7d635b9210ba73834b803` |
| `Source/SuperDMX/Private/SuperDMXSubsystem.cpp` | `28e1651f956f30b2d9cc3583a0dc09e60ae9ced95b7b591da76a3f1339fa8e5e` |
