# SuperAI 工具 — 视口控制与辅助工具

## 概述

视口控制工具让 AI 具备空间感知能力（获取/设置摄像机位置），辅助工具提供截图验证、日志读取、文档查询、脚本执行、控制台命令等扩展能力。

---

## get_viewport_info — 获取视口信息

获取编辑器活跃 Viewport 的摄像机位置、朝向和视野角度。

### 参数

无参数。

### 行为标注

`ReadOnly`

### 返回值

- `location`：摄像机世界坐标（厘米）
- `rotation`：摄像机朝向（角度）
- `fov`：视野角度

### 使用时机

当用户提及方位词（如「在我面前」「正前方」「左边」等）时，先调用此工具获取视角信息再计算位置。

---

## set_viewport_camera — 设置视口摄像机

设置编辑器活跃 Viewport 的摄像机位置和朝向。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `location` | object | — | 摄像机位置 `{"x":0,"y":0,"z":0}`（厘米） |
| `rotation` | object | — | 摄像机朝向 `{"pitch":0,"yaw":0,"roll":0}`（度） |

### 行为标注

`Modifying`

### 使用提示

- 配合 `get_viewport_info` 使用：先读取当前视角再做相对调整
- 配合 `get_actor_transform` 使用：将摄像机移到 Actor 附近观察

---

## capture_viewport — 截取视口截图

截取活动视口的截图，返回 base64 编码的 JPEG。用于 AI 验证场景变更效果。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `width` | number | — | 截图宽度（默认 1024） |
| `height` | number | — | 截图高度（默认 576） |
| `quality` | number | — | JPEG 质量 1-100（默认 70） |

### 行为标注

`ReadOnly`

### 使用场景

- 布局完灯光后截图验证效果
- 材质修改后截图确认外观
- 场景搭建的每个阶段截图记录

---

## get_output_log — 读取引擎日志

读取 UE 引擎输出日志的最近条目，可按关键词过滤。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `lines` | number | — | 返回的日志行数（默认 100，最大 1000） |
| `filter` | string | — | 过滤关键词（如 `Error`、`Warning`、`LogTemp`） |

### 行为标注

`ReadOnly`

### 使用场景

- 检查材质编译错误
- 监控蓝图编译警告
- 排查运行时问题

---

## list_docs — 列出产品文档

列出 SuperStage 产品文档目录中所有可用文件。递归扫描 `Plugins/SuperStage/docs/` 目录。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `subfolder` | string | — | 限定子目录名（如 `lightcomponent`、`stagecore`） |

### 行为标注

`ReadOnly`

### 使用流程

这是 **静默工具**——AI 内部使用，不对用户展示调用过程：

```
1. list_docs → 获取可用文档列表
2. read_doc  → 读取相关文档内容
3. 基于文档内容回答用户问题
```

---

## read_doc — 读取产品文档

按行范围读取 SuperStage 产品文档内容。严格限制只能读取 `docs/` 目录下的文件，防止目录穿越。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `path` | string | ✅ | 文档相对路径（从 `list_docs` 获取） |
| `start_line` | number | — | 起始行号（1 开始，默认 1） |
| `end_line` | number | — | 结束行号（包含，默认 200，单次上限 500 行） |

### 行为标注

`ReadOnly`

---

## execute_console_command — 控制台命令

执行白名单内的 Unreal Engine 控制台命令。仅允许安全的查询和显示类命令。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `command` | string | ✅ | 控制台命令 |

### 行为标注

`Modifying`

### 允许的命令前缀

| 命令 | 说明 |
|------|------|
| `stat fps` / `stat unit` / `stat memory` | 性能统计 |
| `show` | 显示标志切换 |
| `r.` | 渲染设置（如 `r.ScreenPercentage`） |
| `t.MaxFPS` | 帧率限制 |

> ⚠️ 不允许 `quit`、`exit`、`kill`、`delete`、`save` 等危险命令。

---

## execute_script — 执行脚本（万能后备）

执行 Python 脚本或控制台命令。仅在没有专用工具时使用。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `script_type` | string | ✅ | 脚本类型：`python` / `console` |
| `script_content` | string | ✅ | 脚本内容 |
| `description` | string | — | 脚本用途描述 |

### 行为标注

`Destructive`

### 重要提示

- 优先使用专用工具（`spawn_actor`、`set_property` 等）
- 脚本执行需要用户确认
- Python 脚本需要 Python 插件已启用
