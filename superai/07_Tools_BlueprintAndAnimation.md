# SuperAI 工具 — 蓝图与动画

## 概述

蓝图与动画工具允许 AI 查询和修改 Blueprint 与 AnimBlueprint，包括变量管理、函数管理、节点操作、引脚连线、状态机编辑等完整蓝图编辑能力。

---

## blueprint_query — 蓝图查询（只读）

查询蓝图信息，理解蓝图结构。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `operation` | string | ✅ | 操作类型 |
| `blueprint_path` | string | — | 蓝图路径（`inspect` 等操作必填） |
| `graph_name` | string | — | 图名称（`get_nodes` 用，空则为 EventGraph） |
| `node_id` | string | — | 节点 ID（`get_node_pins` 用） |
| `path_filter` | string | — | 路径前缀过滤（`list` 用） |
| `name_filter` | string | — | 名称子串过滤（`list` 用） |
| `limit` | number | — | 最大结果数（默认 25） |

### 行为标注

`ReadOnly`

### 操作说明

| 操作 | 说明 |
|------|------|
| `list` | 列出项目中的蓝图 |
| `inspect` | 获取蓝图详情（变量、函数、父类） |
| `get_nodes` | 获取图中所有节点 |
| `get_variables` | 获取蓝图变量列表 |
| `get_functions` | 获取蓝图函数列表 |
| `get_node_pins` | 获取指定节点的引脚信息 |

---

## blueprint_modify — 蓝图修改

创建和修改蓝图，修改后自动编译。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `operation` | string | ✅ | 操作类型 |
| `blueprint_path` | string | — | 蓝图路径（`create` 以外必填） |
| `package_path` | string | — | 新蓝图包路径（`create` 用） |
| `blueprint_name` | string | — | 新蓝图名称（`create` 用） |
| `parent_class` | string | — | 父类（`create` 用，如 `Actor`、`Pawn`） |
| `variable_name` | string | — | 变量名 |
| `variable_type` | string | — | 变量类型（`bool`、`int32`、`float`、`FString`、`FVector` 等） |
| `function_name` | string | — | 函数名 |
| `graph_name` | string | — | 目标图名（空为默认 EventGraph） |
| `node_type` | string | — | 节点类型 |
| `node_params` | object | — | 节点参数 |
| `pos_x` / `pos_y` | number | — | 节点位置 |
| `node_id` | string | — | 节点 ID |
| `source_node_id` / `source_pin` | string | — | 源节点和引脚 |
| `target_node_id` / `target_pin` | string | — | 目标节点和引脚 |
| `pin_name` / `pin_value` | string | — | 引脚名和默认值 |

### 行为标注

`Modifying`

### 操作说明

| 操作 | 说明 |
|------|------|
| `create` | 创建新蓝图 |
| `add_variable` / `remove_variable` | 管理变量 |
| `add_function` / `remove_function` | 管理函数 |
| `add_node` / `delete_node` | 管理节点 |
| `connect_pins` / `disconnect_pins` | 管理连线 |
| `set_pin_value` | 设置引脚默认值 |

### 支持的节点类型

`CallFunction`、`Branch`、`Event`、`VariableGet`、`VariableSet`、`Sequence`、`PrintString`、`Add`、`Subtract`、`Multiply`、`Divide`

---

## anim_blueprint_modify — 动画蓝图操作

动画蓝图查询与修改，管理状态机结构。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `blueprint_path` | string | ✅ | 动画蓝图路径 |
| `operation` | string | ✅ | 操作类型 |
| `state_machine` | string | — | 状态机名称 |
| `state_name` | string | — | 状态名称 |
| `from_state` / `to_state` | string | — | 源/目标状态名（转换用） |
| `animation_path` | string | — | 动画资产路径 |
| `animation_type` | string | — | 动画类型：`sequence` / `blendspace` / `montage` |
| `search_pattern` | string | — | 动画搜索模式 |
| `position` | object | — | 节点位置 `{x, y}` |

### 行为标注

`Modifying`

### 操作说明

| 操作 | 说明 |
|------|------|
| `get_info` | 获取动画蓝图结构概览 |
| `get_state_machine` | 获取状态机详情 |
| `create_state_machine` | 创建状态机 |
| `add_state` / `remove_state` | 管理状态 |
| `add_transition` / `remove_transition` | 管理状态转换 |
| `set_state_animation` | 设置状态动画 |
| `find_animations` | 搜索兼容动画资产 |
| `validate` | 编译验证 |

---

## 典型工作流

### 创建新蓝图

```
1. blueprint_modify(operation="create",
     package_path="/Game/Blueprints",
     blueprint_name="BP_MyActor",
     parent_class="Actor")

2. blueprint_modify(operation="add_variable",
     blueprint_path="/Game/Blueprints/BP_MyActor",
     variable_name="Health",
     variable_type="float")

3. blueprint_modify(operation="add_function",
     blueprint_path="/Game/Blueprints/BP_MyActor",
     function_name="TakeDamage")
```

### 动画蓝图状态机

```
1. anim_blueprint_modify(operation="get_info",
     blueprint_path="/Game/AnimBP/ABP_Character")
   → 查看现有结构

2. anim_blueprint_modify(operation="create_state_machine",
     state_machine="Locomotion")

3. anim_blueprint_modify(operation="add_state",
     state_machine="Locomotion",
     state_name="Idle")

4. anim_blueprint_modify(operation="set_state_animation",
     state_name="Idle",
     animation_path="/Game/Animations/Idle_Anim")
```
