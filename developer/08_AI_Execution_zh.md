# AI / UE5MCP执行规程：配置、事件图与验收

[返回目录](README.md)

## 1. 不允许推测的事实

AI先发现当前UE5MCP工具及schema，读取当前工程、关卡、PIE状态、插件版本、目标对象路径、已有蓝图和数据资产。工具名取自实际服务，不把本文步骤名当作工具名。

读取父类反射信息，确认SuperDmxActorBase、SuperFixtureLibrary、三个Super事件、要调用的函数和引脚。已知类路径为`/Script/SuperCore.SuperDmxActorBase`及`/Script/SuperCore.SuperFixtureLibrary`；若当前版本不匹配，先报告实际反射差异，不强造节点。

所有项目DMX图只从Super事件进入。**禁止新增Event Tick轮询、Tick override轮询、定时器DMX轮询、同名Custom Event冒充Super事件、外部Bind SuperDMXChanged。** 保留基类调度，不禁用其更新后再另建循环。

不得include SuperAssets/SuperDMX/SuperNdi/SuperTools头文件。现有实现是编写依据，交付给用户的代码只用SuperCore和Unreal标准API。项目自定义函数、字段、事件必须标记为新增。

## 2. 阶段A：把用户目标转换成协议

明确目标Actor/组件、控制轴或材质参数、绝对值还是速度、位深、物理范围和单位、是否需要编辑器预览、首次无输入策略、一次性命令的初始电平规则。能从现有工程读取的先读取，不能确定的业务意义再向用户确认。

从本文提供的协议选择：

| 需求 | JSON | Super入口 |
| --- | --- | --- |
| 六轴/单轴位置与旋转 | Motion6Axis_12CH.json | Super DMX Tick |
| 颜色及材质效果参数 | MaterialControl_6CH.json | Light Initialization准备MID，Super DMX Changed写参数 |
| 档位与按钮 | CommandControl_2CH.json | Super DMX Changed，项目状态去重 |
| 多对象独立位置 | MultiObject_6CH.json | Super DMX Tick，明确模块→目标映射 |

目标已存在时创建单独控制蓝图并引用目标，不擅自重设业务Actor父类。控制自身组件也是同一Super事件链。

## 3. 阶段B：生成/导入库并读回

在`/Game/ProjectDMX/`创建项目Super Fixture Library。实际资产创建与导入能力取决于工具，优先现有JSON导入流程；不支持时用可用的结构体数组编辑能力。不能把磁盘json当uasset路径赋给FixtureLibrary。

生成JSON必须符合[通道库章节](04_SuperFixtureLibrary_zh.md)的已实现字段。导入覆盖Modules，已有资产先核对用途。保存后读回：类、对象路径、模块数量和顺序、ModuleName/Patch、每条AttribName/Coarse/Fine/Ultra、分段范围。

计算每条字节相对位置与绝对位置，检查1..512、位深、同名重复、字节重叠、区间缺口和单位。导入成功提示不替代这些验证。协议中的默认百分比不等于运行时输入初值。

## 4. 阶段C：建立事件图或编译项目类

蓝图：创建正确父类，添加真实继承事件。Light Initialization准备可复用资源；Super DMX Tick处理连续运动；Super DMX Changed处理变化状态。所有Pure读取先保存到变量，再用于多条计算分支。

对引用式输出查询实际引脚；不能根据InOutDefault名字创造输入端口。需要显式回退的蓝图使用ByIndex的DefaultValue=-1，并在有效地址前提下检查失败。接Raw16后除65535，接矩阵输出后不再除。

C++：将[项目示例](examples/README.md)放同一项目模块，确认Build.cs、generated头和UHT。检查只override所需Native Super方法；DeltaSeconds从World获取。保留Super调用，不在事件中调用ForceRefreshDMX。

每次图编辑后编译并读取错误，最后读回关键节点与连线。创建节点成功不等于已接通。工具不能创建某个真实继承事件时，记录缺少能力和剩余具体步骤，不能用Custom Event替代后声称完成。

## 5. 阶段D：绑定场景对象

给控制对象设置FixtureLibrary、SuperDMXFixture和ControlMode；结构体更新保留其他字段。目标引用用真实对象路径，不仅靠可能重复的Label。目标移动性、坐标空间、材质槽和参数名逐项验证。

属性写入后读回实际值，保存蓝图、库和关卡。先不开启项目运动开关，确认地址和目标再启用。编辑器预览只按项目明确开关，不因构造回调就执行一次性命令。

## 6. 阶段E：用可复现输入验证

以下是测试向量，必须通过实际支持的发送端/测试工具产生，再从对象读回。修改私有快照或直接改目标Transform不算DMX接通测试。

| 案例 | 配置与输入 | 期望 |
| --- | --- | --- |
| Fine位置 | 12CH库，Start101，105/106=0/255→1/0 | Raw255→256，验证粗细顺序 |
| 位置中点 | 105/106=128/0，起终点Z=0/300 | Raw32768，目标Z约150.0023 |
| 持续插值 | 到终点后保持同样输入 | SuperDMXTick继续运行，目标继续趋近终点 |
| 颜色 | 6CH材质库，Start201，201..203=255/0/0 | MID ProjectColor=(1,0,0,1) |
| 模式边界 | 2CH命令库，Start301，301=63/64/191/192 | Idle/Preview/Preview/Run |
| 业务边沿 | 302=0/127/128/255/255/0/128 | 命令累计0/0/1/1/1/1/2 |
| 多目标 | 6CH多对象库，Start401，三对字节=0/0、128/0、255/255 | A起点、B中间、C终点，不错位 |

Changed计数可能因构造或其他通道变化增加，但Command计数必须符合业务状态机。相同网络包不会保证Changed触发。无输入、错误库或越界可能仍读到0，按解析地址、默认路径和真实信号分别定位。

## 7. 故障定位顺序

1. **没有Super事件：** 父类/事件节点是否正确；是否保持基类调度；授权和运行条件；不要先加Event Tick“补救”。
2. **SuperTick有、Changed没有：** 检查FixtureLibrary、非空有效跨度、整体边界、当前内部Universe快照，以及值是否真发生变化。
3. **地址正确、值不对：** 核对网络Universe映射、StartAddress、Patch一基规则、字节顺序和发送端协议。
4. **值正确、对象不动：** 检查项目Enabled、运行/预览门控、目标有效性、可移动性、轴与坐标空间、物理冲突。
5. **只有变化时动一点：** 连续运动是否误放Changed；改到SuperDMXTick处理，不用引擎Tick。
6. **按钮重复执行：** 是否直接把Changed当按钮；补自己的首次基线与上升沿。
7. **多对象错位：** 是否把排序过滤后的数组索引当模块索引；用明确索引映射。
8. **NDI纹理不显示：** 检查输入名、SourceMode、首帧、MID及参数；GetActiveTexture不是原生蓝图节点，需正确项目桥接。

## 8. 交付记录

每项标记实际结果：资产已创建/已保存；图表或C++已编译；属性和地址已读回；真实输入已验证；目标结果已验证；重载已验证；打包已验证。未执行的阶段写未验证，不用“应该可以”当结果。

若仅生成文档/代码/JSON，明确它们是待导入/待编译文件。正式交付包的外部工程验收必须独立于内部全源码工程。

## 9. 给AI的直接任务说明

> 先阅读本目录的接入约定、Super事件、完整API、通道库及蓝图案例。使用当前UE5MCP真实工具schema读回工程状态和对象。我的DMX控制只能从SuperStage自己的Light Initialization、Super DMX Tick、Super DMX Changed事件进入；C++对应Native方法。根据现有SuperAssets调用模式配置库、读取、映射并控制我自己的目标。不要创建Event Tick轮询，不要猜测工具名/节点/引脚，不要include非开放模块。使用实际支持的JSON导入或属性编辑创建SuperFixtureLibrary，保存后核对全部地址。逐项编译、输入测试、目标结果和重载验证，只报告实际完成的阶段。
