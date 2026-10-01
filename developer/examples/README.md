# 项目示例文件

[返回开发文档](../README.md)

这些是本文新编写的项目示例，用现有实现确认的公开入口组织，不是产品安装后已有的同名类/节点，也不是非开放模块源码。文件目录不参与当前插件编译。复制所需示例到自己的同一个模块，按[C++章节](../06_CPP_Integration_zh.md)设置依赖。

## C++文件

| 示例 | 文件 | 调用入口 |
| --- | --- | --- |
| 外部Actor运动 | [ProjectDmxMotionController.h](ProjectDmxMotionController.h)、[cpp](ProjectDmxMotionController.cpp) | NativeLightInitialization、NativeSuperDMXTick |
| 自有材质参数 | [ProjectDmxMaterialController.h](ProjectDmxMaterialController.h)、[cpp](ProjectDmxMaterialController.cpp) | NativeLightInitialization、NativeSuperDMXChanged |
| NDI纹理应用和蓝图桥接 | [ProjectMediaReceiver.h](ProjectMediaReceiver.h)、[cpp](ProjectMediaReceiver.cpp) | OnActiveTextureChanged |

没有项目Tick override，没有Event Tick轮询方案。DMX项目只使用SuperCore公开头。媒体项目先验证正式SDK头文件依赖完整，再编译对应两文件；不需要NDI时不用添加媒体示例。

运动例使用显式世界Start/End配置，默认关闭。材料例需要自己的材质参数ProjectColor/ProjectEffect/ProjectSpeed/ProjectWidth。媒体例需要ProjectMedia纹理参数。参数不存在不会自动创建材质图。

## 原生JSON库

| 文件 | 相对通道 | 说明 |
| --- | --- | --- |
| [Motion6Axis_12CH.json](libraries/Motion6Axis_12CH.json) | XPos1/2、YPos3/4、ZPos5/6、XRot7/8、YRot9/10、ZRot11/12 | 名称/位深与现有六轴机械读取一致 |
| [MaterialControl_6CH.json](libraries/MaterialControl_6CH.json) | Red1、Green2、Blue3、Effect4、Speed5、Width6 | 提取已核对的材质参数读取模式，项目自定义6CH布局 |
| [CommandControl_2CH.json](libraries/CommandControl_2CH.json) | Mode1、Trigger2 | 项目新增模式与边沿协议 |
| [MultiObject_6CH.json](libraries/MultiObject_6CH.json) | 三模块Patch1/3/5，各ZPos1/2 | 项目新增多目标位置协议 |

按[库创建步骤](../04_SuperFixtureLibrary_zh.md)导入到新数据资产。JSON键已对照现有解析器；JSON文件不是uasset，不可直接赋给FixtureLibrary。导入覆盖Modules，随后读回和校验地址。

## 验证状态

文件已经进行接口及协议静态检查。尚未完成客户SDK的UHT/编译/链接、UE内导入、真实网络输入或打包验收。实际操作按[AI执行规程](../08_AI_Execution_zh.md)记录，不能把静态核对说成运行测试通过。
