# SuperAI 工具 — 场景管理

## 概述

场景管理类工具覆盖了 Actor 的完整生命周期：生成、查询、选中、移动、复制、重命名、显隐、删除、撤销/重做，以及 Outliner 文件夹管理。

---

## spawn_actor — 生成 Actor

在场景中生成新 Actor，支持类路径、短类名、基础形状快捷名。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `class_path` | string | ✅ | Actor 类路径或短类名 |
| `actor_label` | string | — | 编辑器标签（默认自动命名） |
| `location` | object | — | 初始位置 `{"x":0,"y":0,"z":0}` |
| `rotation` | object | — | 初始旋转 `{"pitch":0,"yaw":0,"roll":0}` |
| `facing_direction` | object | — | 灯头出射方向（仅灯光 Actor 有效） |

### 支持的类路径格式

- **完整路径**：`/Script/Engine.PointLight`
- **短类名**：`PointLight`、`SpotLight`、`CameraActor`
- **蓝图路径**：`/SuperStage/Fixtures/BP_MyLight.BP_MyLight_C`
- **基础形状快捷名**：`Cube`、`Sphere`、`Cylinder`、`Cone`、`Plane`

### 行为标注

`Modifying` — 会修改场景内容，支持撤销。

### 重要约束

- **SuperStage 资产禁止使用 C++ 基类路径**（如 `/Script/SuperStage.SuperStageLight`），必须先通过 `list_super_assets` 获取蓝图 `class_path` 再传入
- `facing_direction` 仅对灯光 Actor（`ASuperLightBase` 子类）有效，系统自动根据 `UpArrow` 计算旋转
- 生成 SuperStage 资产后必须配置属性（默认值可能不亮或参数不对）

---

## get_level_actors — 查询场景 Actor 列表

查询当前关卡中的 Actor 列表，支持过滤和分页。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `class_filter` | string | — | 按类名过滤 |
| `name_filter` | string | — | 按名称/标签子串过滤 |
| `brief` | boolean | — | `true` 只返回基础信息，`false` 含 Transform（默认 `true`） |
| `limit` | number | — | 返回数量上限（1-500，默认 25） |
| `offset` | number | — | 跳过数量（分页用，默认 0） |

### 行为标注

`ReadOnly` — 只读操作，无副作用。

---

## find_actors_by_class — 按类名查找 Actor

精确按类名查找场景中的所有 Actor，比 `get_level_actors` 更高效。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `class_name` | string | ✅ | 类名（如 `PointLight`、`SuperStageLight`） |

### 行为标注

`ReadOnly`

### 常用类名

`PointLight`、`SpotLight`、`RectLight`、`DirectionalLight`、`SkyLight`、`StaticMeshActor`、`CameraActor`、`ExponentialHeightFog`、`PostProcessVolume`、`PlayerStart`、`SuperStageLight`

---

## get_selected_actors — 获取当前选中 Actor

获取编辑器中当前选中的 Actor 列表，返回名称、标签、类名和位置。

### 参数

无参数。

### 行为标注

`ReadOnly`

### 使用时机

当用户提及「选中的」「这些」「当前选择的」等词时调用。

---

## select_actors — 选中 Actor

在编辑器 Viewport 中选中指定的一个或多个 Actor，选中后高亮显示。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `actor_names` | array | ✅ | Actor 名称或标签数组 |
| `clear_first` | boolean | — | 是否先清空当前选中（默认 `true`） |

### 行为标注

`ReadOnly`

---

## delete_actor — 删除 Actor

从场景中删除指定 Actor，操作支持撤销（Ctrl+Z）。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `actor_name` | string | ✅ | Actor 名称或标签（字符串或字符串数组） |

### 行为标注

`Destructive` — 破坏性操作，但支持撤销。

---

## duplicate_actor — 复制 Actor

复制场景中的 Actor，保留全部属性设置。比 spawn + set 更高效。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `actor_name` | string | ✅ | 源 Actor 名称或标签 |
| `new_label` | string | — | 新 Actor 的编辑器标签 |
| `offset` | object | — | 相对源 Actor 的位置偏移 `{"x":0,"y":0,"z":0}` |

### 行为标注

`Modifying`

---

## rename_actor — 重命名 Actor

重命名 Actor 的编辑器标签，支持单个或批量。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `actor_name` | string | ✅ | 当前名称或标签（字符串或字符串数组） |
| `new_label` | string | ✅ | 新标签（批量时支持 `{i}` 占位符，如 `SpotLight_{i}`） |

### 行为标注

`Modifying`

---

## set_actor_visibility — 显示/隐藏 Actor

显示或隐藏场景中的 Actor，同时影响编辑器视口和运行时。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `actor_name` | string | ✅ | Actor 名称或标签（字符串或字符串数组） |
| `visible` | boolean | ✅ | `true` = 显示，`false` = 隐藏 |

### 行为标注

`Modifying`

---

## move_actor_to_folder — 移入 Outliner 文件夹

将 Actor 移入 Outliner 文件夹，文件夹不存在时自动创建。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `actor_name` | string | ✅ | Actor 名称或标签（字符串或字符串数组） |
| `folder_path` | string | ✅ | 目标文件夹路径（用 `/` 分隔层级，如 `Lighting/Key`） |

### 行为标注

`Modifying`

### 推荐文件夹结构

```
Lighting/
├── Key        — 主光
├── Fill       — 补光
└── Rim        — 轮廓光
Environment/   — 环境物体
```

---

## undo / redo — 撤销 / 重做

等同于 Ctrl+Z 和 Ctrl+Y，为所有 AI 操作提供安全网。

### 参数

无参数。

### 行为标注

`Modifying`

---

## open_level — 关卡管理

打开、新建、保存关卡。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `action` | string | ✅ | 操作类型：`open` / `new` / `save` / `save_as` / `get_current` |
| `level_path` | string | — | 关卡路径（`open` 和 `save_as` 必填） |

### 行为标注

`Modifying`
