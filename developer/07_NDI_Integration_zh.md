# NDI媒体接入：沿用屏幕与投影的纹理变化钩子

[返回目录](README.md) · [项目媒体示例](examples/ProjectMediaReceiver.h)

## 1. 正确入口

开放核心类型为`ASuperMediaBase : ASuperBaseActor`，声明在SuperMediaBase.h，是Abstract类。现有屏幕和投影对象均重写`OnActiveTextureChanged()`，在其中取得`GetActiveTexture()`并更新材质纹理引用。项目媒体用途沿用此入口。

不自行建立NDI接收器，不include非开放SuperNdi模块，不把DMX的SuperDMXChanged当作视频帧事件。ASuperMediaBase并不继承ASuperDmxActorBase；同时需要DMX和NDI时用两个控制对象分别负责，再把数据应用到同一业务目标，不能对两个Actor基类做多继承。

## 2. 配置字段

| 字段 | 默认 / 用途 | 蓝图边界 |
| --- | --- | --- |
| SourceMode | Texture；枚举NDI/Texture/Director | EditAnywhere，未声明BlueprintReadWrite |
| InputName | NAME_None；已配置输入的逻辑名称 | 编辑器显示NDIInputSelection，下拉由GetInputNameOptions提供 |
| StaticTexture | nullptr；Texture模式的UTexture | 编辑器配置 |
| DirectorCamera | nullptr；Director模式来源 | 编辑器配置，项目NDI复用不必依赖具体相机类 |
| NDITexture | 运行时创建的UTexture2D | Transient、VisibleAnywhere，不是任意可写的蓝图纹理变量 |

InputName为空时，当前绑定逻辑尝试选择已配置输入中的第一个；正式项目显式选名称以免依赖配置顺序。GetInputNameOptions虽有UFUNCTION，但未标BlueprintCallable/Pure，是细节面板选项提供者，不据此创造可调用蓝图节点。

编辑器更改SourceMode/InputName会触发基类配置处理。运行时C++直接赋字段不等同于触发编辑器属性通知；本文示例预先在编辑器完成来源配置，不编造通用运行时切源节点。

## 3. GetActiveTexture

`UTexture* GetActiveTexture() const`是C++公开函数，无UFUNCTION。

NDI返回NDITexture；Texture返回StaticTexture；Director返回关联捕获纹理，无相机返回null。返回值统一为UTexture，消费端不要无条件强转UTexture2D，因为其他模式可能是RenderTarget。

首帧前可能null。有效纹理对象不代表发送端仍在线，断流可能保留最后画面。纹理不能证明“刚收到包”。项目要明确定义null时的备用纹理或空画面策略，不把保留帧当实时状态。

## 4. OnActiveTextureChanged的实际触发

这是protected C++ virtual，基类默认空实现，**不是原生BlueprintImplementableEvent，也不是可绑定委托**。

| 路径 | 行为 |
| --- | --- |
| BeginPlay、OnConstruction | 基类按配置绑定后调用钩子 |
| 编辑器属性改变 | 推送当前活动纹理 |
| 组件注册/初始化补偿 | 基类安排一次延后的初始推送 |
| NDI首帧导致创建纹理 | 上传后通知纹理引用 |
| NDI尺寸变化导致重建纹理 | 通知新的纹理引用 |
| NDI普通同尺寸帧 | 更新现有纹理内容，**不逐帧调用该钩子** |

头文件注释含“新帧到达”，但当前实现只有创建/重建时从帧路径通知。材质持有同一个UTexture时，GPU内容更新会呈现新画面，无需每个视频帧重新SetTextureParameter。

钩子可早于派生对象的材质准备，处理需可重复且能接受null。现有屏幕/投影在构造与初始化准备MID，基类还有延后补偿。项目示例在钩子内确保自己的MID有效，再应用纹理，避免依赖只有一次通知。

## 5. 头文件中的接收方法

| 方法 | 已确认行为 | 项目使用原则 |
| --- | --- | --- |
| BindToSubsystem() | 依据InputName绑定，改名时移除旧绑定，再去重订阅 | 基类生命周期负责；不每帧手动调用 |
| HandleNDIFrame(BGRA,Width,Height) | 只处理NDI模式，验证尺寸>0和数据长度=Width×Height×4，再建/传纹理 | 是基类接收处理入口，不是用户绑定的公开视频委托 |
| EnsureTexture(Width,Height) | 相同尺寸复用；不同尺寸创建Transient纹理，返回是否创建成功 | 不自行抢先创建NDITexture或手改其尺寸 |
| UpdateTextureGPU(SrcBGRA,Width,Height) | 将数据交给渲染上传路径 | 属于基类管理的上传流程，本文纹理消费者不直接调用 |
| GetInputNameOptions() | 返回已配置输入名 | 用于来源配置下拉，不作为蓝图Pure声明 |
| EndPlay等生命周期 | 解除订阅和正常资源管理 | 派生实现若覆盖须保留父类；示例无需覆盖 |

这些函数存在于开放头不意味着都应成为项目自己的接收流程。NDI消费端按现有屏幕的钩子模式即可，不进入协议/渲染内部管理。

## 6. 完整项目接收器与材质案例

文件：[ProjectMediaReceiver.h](examples/ProjectMediaReceiver.h)、[ProjectMediaReceiver.cpp](examples/ProjectMediaReceiver.cpp)。它继承ASuperMediaBase，仅重写OnActiveTextureChanged。项目新增TargetActor、BaseMaterial、MaterialIndex、TextureParameter、FallbackTexture；这些不是基类属性。

1. 在正式交付SDK验证SuperMediaBase.h包含链和模块链接可用，然后编译示例。
2. 创建自己的材质，加入Texture Sample Parameter2D，参数名ProjectMedia，连接到需要的材质输出。模型显示案例先用简单Unlit输出排除光照干扰。
3. 放置ProjectMediaReceiver，选择SourceMode=NDI、显式InputName；TargetActor引用自己的StaticMeshActor，BaseMaterial选自己的材质，MaterialIndex选有效槽，FallbackTexture选项目备用画面。
4. 钩子确保自己的MID存在，设置到目标槽，并写入活动纹理或备用纹理。复制对象或换BaseMaterial时会重建自己的MID。
5. 先Texture模式用静态图确认材质/UV，再切NDI验证动态画面；修改发送分辨率应正确换纹理引用。

此示例的FallbackTexture仅在GetActiveTexture无有效对象时生效；断流但纹理仍存在不会自动切备用。备用行为是项目逻辑，不是产品在线检测。

## 7. 蓝图怎么使用媒体

原生GetActiveTexture和OnActiveTextureChanged没有DMX式蓝图声明。不能照着DMX蓝图直接搜索并假设存在同名节点。本文提供的ProjectMediaReceiver新增：

| 项目新增接口 | 功能 |
| --- | --- |
| GetProjectTexture | BlueprintPure，返回活动纹理或项目FallbackTexture |
| ProjectTextureChanged | BlueprintImplementableEvent，运行世界中将纹理引用变化交给派生蓝图 |

必须先编译这两个示例文件，才存在这些项目节点。ProjectTextureChanged是派生蓝图实现事件，不是外部Dispatcher；需要跨对象广播时项目自己声明Dispatcher。

该示例事件仅在运行世界且Actor已开始运行时通知。编辑器材质预览由C++钩子完成；不承诺这个项目蓝图事件在非运行编辑器执行。UI等消费端初始化时主动读一次GetProjectTexture，之后通过项目转发接收引用更新，以免UI创建晚于首次通知。

## 8. UMG与Render Target用途

UMG：创建UI域材质，包含ProjectMedia纹理参数；建立自己的UI MID并作为Image Brush材质。UI就绪时从接收器取得UTexture写入参数；引用变化后再写。普通帧更新由持有的纹理内容更新体现，不反复创建Widget/MID。UTexture结果不要无条件接只接受UTexture2D的节点，使用材质可保留纹理通用性。

Render Target：若目标系统必须得到独立RenderTarget，可以由项目渲染流程把采样该纹理的材质绘入目标。但一次绘制只产生那次画面，普通NDI帧不会每帧触发OnActiveTextureChanged。此钩子仅适合重绑源引用，不足以作为连续拷贝时钟；需单独设计并验证项目渲染更新机制。本文不把一次拷贝描述成持续视频流。

多个材质消费同一纹理：在一次钩子中更新多个MID即可，现有投影案例采用此方式。消费目标之间独立保存MID及参数配置，避免为每个对象重复建立NDI输入。

## 9. 媒体验收

记录来源配置、首帧前null/备用行为、首帧引用建立、普通帧画面持续变化、分辨率变化引用重建、Texture/NDI模式切换、复制对象的MID独立性、目标无效处理及运行结束释放。断开发送端时记录实际行为，不承诺自动黑屏。

当前未在客户SDK完成编译或真实流测试。若开放核心头引用了未交付依赖，需先修复交付包完整性；不通过向用户开放其他模块源码来补本文。
