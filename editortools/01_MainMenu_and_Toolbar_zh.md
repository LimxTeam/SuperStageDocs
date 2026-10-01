# SuperStage 主菜单与工具栏

> 适用版本：SuperStage 26H2.6 起

本文档说明当前 SuperStage 主插件在 Unreal Editor 中注册的用户入口。菜单内容以当前实现为准。

## 1. 打开方式

启用 SuperStage 后，Unreal Editor 顶部工具栏会出现 **SuperStage** 按钮。点击该按钮会展开 SuperStage 下拉菜单。

## 2. 顶级入口

| 菜单项 | 功能 |
| --- | --- |
| PluginAuth | 打开插件认证窗口，用于登录、查看授权和离线激活相关操作。 |
| OfflineActivation | 离线激活入口。 |
| SuperBrowser | 打开 SuperStage 资产浏览器，用于浏览并放置灯具和舞台资产。 |
| AssetDemo | 资产示例列表。 |

## 3. SuperDMXTool 子菜单

| 菜单项 | 功能 | 文档 |
| --- | --- | --- |
| PatchTool | 打开 DMX 批量配接工具。 | [07 - Patch 工具](../stagecore/07_Patch_Tools_zh.md) |
| DMXToMa | 打开 DMX 到 grandMA 的离线导出工具。 | [07 DMXExportMA](07_DMXExportMA_zh.md) |
| GrandMALink | 打开 grandMA2 / grandMA3 配接连接面板。 | [11 GrandMALink](../stagecore/11_GrandMA_Link_zh.md) |
| MVR | 打开 MVR 导入和导出工具面板。 | [08 MVR 导入导出](08_MVRImport_zh.md) |
| **GDTF Batch Import** | 一次导入一批 GDTF 包，批量生成灯具定义。**26H2.6 新增** | [GDTF 导入](../fixture/02_GdtfImport_zh.md) |

## 4. Documentation 子菜单

| 菜单项 | 功能 |
| --- | --- |
| QuickStart | 打开 SuperStage 快速入门网页。 |
| ProductDocs | 打开 SuperStage 产品文档网页。 |
| SuperLaser | 打开激光相关文档。 |
| ModuleManuals | 打开模块手册子菜单。 |
| SystemReference | 打开编辑器工具、灯光组件、舞台资产和 DMX 核心系统参考链接（EditorTools / LightComponents / StageAssets / DMXCoreSystem）。 |
| DevDocs | 打开开发/API 文档网页。普通用户通常不需要阅读。 |
| Changelog | 打开版本更新日志网页。 |

## 5. 其他入口

| 菜单项 | 功能 |
| --- | --- |
| Social Media | 教程与社媒子菜单：Bilibili、抖音、YouTube、Instagram、Facebook。 |
| Website | 打开官网。 |
| ContactUs | 打开联系页面。 |
| UserAgreement | 打开用户协议页面。 |

## 6. 底部状态栏

SuperStage 会在编辑器底部状态栏注册以下按钮：

| 按钮 | 功能 |
| --- | --- |
| SuperStage: 版本号 | 显示当前插件版本，点击打开官网。 |
| SuperDMX | 打开 SuperDMX 配置面板（含 DMX 活动监看）。 |

> DMX 配置面板的入口在**底部状态栏**，不在 SuperStage 下拉菜单里。

## 7. UE Tools 主菜单

SuperStage 在 Unreal Editor 顶部 **Tools** 菜单中注册以下工具：

| 菜单项 | 功能 |
| --- | --- |
| GOBO Atlas Builder | 从通道库属性中提取图案纹理并生成 GOBO 图集。 |
| Color Atlas Builder | 从通道库属性中提取颜色并生成颜色图集纹理。 |

## 8. 面板帮助按钮

**26H2.6 新增**：14 个工具面板右上角都有帮助按钮，弹出该面板的使用说明，内容已纳入本地化字典。

带帮助按钮的面板：资产浏览器、配接工具、DMX 配置、NDI 配置、灯具编辑器、通道库编辑器、棱镜预设编辑器、GDTF 批量导入、MVR、DMXToMa、GrandMALink、GOBO 图集生成器、颜色图集生成器、VAT 角色生成器。

## 9. 使用提示

- 如果工具栏没有出现，请确认 SuperStage 插件已启用并重启编辑器。
- 如果点击菜单没有反应，请查看 Output Log 中是否有模块加载或授权相关错误。
