# SuperStage MVR 导入 / 导出 用户手册

## 1. 功能范围

MVR（My Virtual Rig）是灯光设计软件、可视化软件与控台之间交换整份图纸的标准格式。本面板按 **MVR 1.6** 规范实现读写：

- **导入**：读取 `.mvr` 包，解析完整的场景描述（图层、分组、灯具、桁架、吊挂、道具、屏幕、投影机、挂位、类别、聚焦点、像素映射定义），在关卡中生成灯具与道具。
- **导出**：把当前关卡写回成一份符合规范的 `.mvr`，并把每个型号的 GDTF 一并打进包里。

包内的 `.gdtf` 不再被忽略：灯库里没有的型号可以直接用包里那份 GDTF 现场建出灯具定义资产。

打开方式：主菜单 **SuperStage** → **SuperDMXTool** → **MVR**，面板顶部有 **Import** / **Export** 两个页签。

---

## 2. 导入

### 2.1 面板

| 区域 | 用途 |
| --- | --- |
| MVR File / Browse | 选择 `.mvr`（也可直接选一份解出来的 `.xml`，排查互操作问题时很方便） |
| 选项区 | 见 2.2 |
| 摘要行 | 文件版本、导出方、图层数、各类对象数量 |
| Select All / None | 批量勾选灯具型号 |
| Re-match | 重新按灯库匹配一遍（新导入了灯具定义之后用） |
| Import | 生成 Actor |
| 列表 | 按「GDTF 型号 × DMX 模式」分组的灯具，每组在 **Target Fixture** 列指定目标 |

列表按 **(GDTFSpec, GDTFMode)** 分组，而不是按灯具实例名 —— 同一支灯的 16CH 与 32CH 是两套通道布局，必须分开选目标，否则一半灯具补进去通道全错。

### 2.2 选项

| 选项 | 说明 |
| --- | --- |
| Create missing fixtures from packaged GDTF | 灯库里没有的型号，用包内 GDTF 现场建出灯具定义资产（默认开）。规范要求 MVR 包自带每支灯的 GDTF，所以这条几乎总能成立 |
| Layers to outliner folders | 图层与分组还原成世界大纲文件夹（默认开）。导出时第一级文件夹会变回图层，其余变回分组 |
| Keep MVR metadata | 给每支灯挂一个 `Super MVR Metadata` 组件，保存挂位、类别、聚焦点、控台编号、颜色、协议、网络地址等 UE 侧没有对应概念的数据（默认开）。**关掉它，导回 MVR 时这些信息就没了** |
| Import trusses / supports / scenery | 从包内 `.glb` / `.3ds` 或 GDTF 里的模型生成静态网格并摆好位置（默认开） |
| DMX address | 地址解读方式，见 2.4 |

### 2.3 目标指定

每一组灯具都要在 **Target Fixture** 列指定一个目标，下拉菜单里有三类来源：

1. **From packaged GDTF** —— 用这份 MVR 自带的 GDTF 现场建一个灯具定义资产。
2. **Fixture library** —— 按 厂商 → 型号 → DMX 模式 三级选择既有灯库型号。模式名只在真正展开到第三级时才加载对应资产，几千个型号的灯库也不会卡。
3. **Legacy actor classes** —— SuperAssets 里的原生灯具类（16 支自研灯与机械），以及项目自己定义的蓝图灯具类（若有）。

打开文件时会**自动匹配**一遍：GDTF 文件名遵循 `Manufacturer@Model@Revision` 约定，而灯具定义的 Identity 用的正是同一对值，按 `@` 切开即可精确对上。厂商对不上但型号在全库唯一时也接受；型号重名（"Beam 200" 这种）时留空由人来挑，不猜。

新建的定义资产落在 `/Game/SuperStage/FixtureLibrary/<厂商>/<型号>/` 下，**导入后需要在内容浏览器里保存**（报告里会提醒）。

### 2.4 DMX 地址

MVR 的 `<Address>` 值是**跨宇宙的绝对地址**（宇宙 4 的 100 通道 = 3×512+100），`break` 属性是 GDTF 的 DMX break 编号，与宇宙无关。默认按规范读。

极少数老文件把 `break` 当宇宙下标、值写成宇宙内通道。此时把选项切到 **Legacy (break = universe - 1)** 重新导入。解析器发现「全部地址都在一个宇宙内、却出现了多个不同 break」这种可疑组合时会在报告里提示。

值写成 `"宇宙.通道"` 字符串的形式两种模式下都能正确识别。

多 break 灯具（灯体与像素分开补的那种）在 UE 侧只补得下第一组地址，其余各组保存在元数据组件里，导出时原样写回；报告会逐支点名。

### 2.5 导入结果

| 信息 | 来源 |
| --- | --- |
| 位置 / 旋转 / 缩放 | `<Matrix>`，已完成毫米→厘米、右手→左手、灯头朝向三步换算 |
| Universe / StartAddress | 主地址（break 最小的那条） |
| FixtureID | `<FixtureIDNumeric>` → `<FixtureID>` 文本 → `<UnitNumber>` 三级兜底 |
| Actor Label | `name` 属性；没有时用「型号 + 编号」 |
| 大纲文件夹 | 图层 / 分组名 |
| MVR 元数据组件 | 挂位、类别、聚焦点坐标、编号、颜色、Gobo、协议、网络地址、映射、接线、对齐、覆盖、自定义命令 |

整个导入在一个 UE 事务里完成，可以直接 Ctrl+Z 撤销。**新建的灯具定义资产不在事务内**（资产创建不属于关卡撤销栈），撤销只会撤掉灯，资产仍在。

导入结束会弹出报告：解析与导入两个阶段的全部错误、警告与提示都在里面，同时写进 `LogMvrImport`。

---

## 3. 导出

切到 **Export** 页签，勾好下面的选项，选好输出路径，点 **Export MVR**。

### 3.1 选项

| 选项 | 说明 |
| --- | --- |
| Pack GDTF files | 把每个型号的 `.gdtf` 打进包里（默认开）。规范要求如此 —— 缺了它对方即使认得型号名也没有通道表 |
| Outliner folders as layers | 第一级文件夹 → 图层，更深的层级 → 嵌套分组（默认开）。关掉则全部灯归入单一图层 |
| Selected actors only | 只导出关卡里当前选中的灯具 |
| Write MVR metadata back | 把导入时记下的挂位、类别、聚焦点、编号、颜色、协议等写回去（默认开） |
| Compress scene description | 场景描述走 deflate（默认开）。一份大图纸大约能压到十分之一；`.gdtf` 本身已是压缩包，一律直存 |

### 3.2 产出内容

```
GeneralSceneDescription.xml     完整场景，子节点顺序严格按 MVR 1.6 的 xs:sequence
Manufacturer@Model@Rev.gdtf     每个型号的灯库原包，文件名沿用源包原名
```

写出的内容包括：图层与分组、灯具（矩阵、GDTFSpec/GDTFMode、Focus、CastShadow、DMXInvertPan/Tilt、Position、Function、FixtureID/FixtureIDNumeric/UnitNumber、ChildPosition、Addresses + Network、Protocols、Alignments、CustomCommands、Overwrites、Connections、Color、CustomId、Mappings、Gobo）、AUXData 里的挂位 / 类别 / 像素映射定义、以及被引用的聚焦点。

`<GDTFMode>` 取的是数据驱动灯具的 **ActiveMode**，不是通道库的模块名 —— 后者是像素组名，写错了灯补得进去但通道全错位。

### 3.3 落盘前后各查一遍

导出不是「写完就算」：

- **落盘前**逐条校验规范硬性要求：uuid 是否规范且唯一、FixtureIDNumeric 是否为正且全场唯一、UnitNumber、同一支灯的 break 是否重复、GDTF 文件名是否合法、Position / Class / Focus / MappingDefinition 的引用是否都有定义、multipatch 父对象是否存在。任何一条不过就中止并给出原因。
- **落盘后**把刚写出的文件重新打开、重新解析，比对灯具数量。这一步逮的是「我们自己写出去、自己都读不回来」那类错误 —— 它们在下游只会表现为「控台打不开这个文件」。

两步的结果都在导出报告里。

---

## 4. 出问题时

**灯具进错了 Universe**
把 **DMX address** 切到 **Legacy**，重新导一次。

**某个型号显示 "No fixture / no GDTF"**
灯库里没有匹配的型号，包里也没带这支灯的 GDTF。手工指一个目标，或者先把 GDTF 导进灯库再来。

**导出报 "Missing GDTF"**
这支灯的通道库在磁盘上没有对应的 GDTF 文件。把这支灯从它的 GDTF 重新导入一次，路径才会被记下来。

**桁架没出来**
只有 `.glb` 与 `.3ds` 两种几何读得进来。GDTF 里只声明了占位基本体、没有真实模型文件的，不会生成网格——报告里会写明是哪些。

> 每次运行的完整报告都会写进输出日志，导入在 `LogMvrImport`、导出在 `LogMvrExport`。

---

## 5. 兼容性说明

- 包按 PKWARE 6.3.3 写，只用 STORE 与 DEFLATE 两种存储方式，无加密，全部文件在包根目录（规范要求扁平结构）。读取端额外容忍带子目录的第三方包，并在报告里提示。
- 读取端对不规范的写法尽量宽容：标签大小写不敏感、uuid 接受大写/花括号/无连字符写法、对象直接挂在 `<Layers>` 下时收进一个合成图层、`Geometry3D` 的 `fileName` 缺扩展名时按 `.3ds` 处理、`ScaleHandeling` 与 `ScaleHandling` 两种拼写都认。
- 写入端一律按规范：不写规范表里没有的节点（例如 `FixtureTypeId`，它读得进来但不写出去），必写节点即使值为 0 / 空也照写。
- 场景描述支持 UTF-8 / UTF-8 BOM / UTF-16 LE / UTF-16 BE，也支持压成一行的 XML。

## 6. 目前不做的事

- **不导出场景几何**：UE 侧没有把静态网格写成 `.3ds` / `.glb` 的通道，所以导出只包含灯具，桁架与道具不会出现在导出的包里。
- **不支持 MVR-xchange**：那是 MVR 生态里另一套东西（mDNS 发现 + TCP/WebSocket 的实时同步协议），与本面板读写的文件格式是两件事，本版本未实现。
- **不透传第三方 UserData**：导入时能读到其它软件写在根 `<UserData>` 下的私有数据块，但它不随关卡保存，因此导出时无法原样送回。导出会写入自己的一块出处信息（工程名、引擎版本、灯具数、导出时间）。
- GDTF 里只有 `PrimitiveType` 占位、没有实际模型文件的道具不会生成网格，报告里会说明。
