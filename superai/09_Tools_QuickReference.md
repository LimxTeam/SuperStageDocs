# SuperAI 工具快速参考表

## 全部 35+ 工具一览

### 场景管理（11 个工具）

| 工具名 | 行为 | 说明 |
|--------|------|------|
| `spawn_actor` | Modifying | 生成新 Actor（类路径/短名/蓝图路径） |
| `delete_actor` | Destructive | 删除 Actor（支持批量） |
| `duplicate_actor` | Modifying | 复制 Actor（保留全部属性） |
| `rename_actor` | Modifying | 重命名 Actor 标签（支持批量+占位符） |
| `get_level_actors` | ReadOnly | 查询场景 Actor 列表（过滤+分页） |
| `find_actors_by_class` | ReadOnly | 按类名精确查找 Actor |
| `get_selected_actors` | ReadOnly | 获取当前选中 Actor 列表 |
| `select_actors` | ReadOnly | 在 Viewport 中选中 Actor |
| `set_actor_visibility` | Modifying | 显示/隐藏 Actor（支持批量） |
| `move_actor_to_folder` | Modifying | 移入 Outliner 文件夹（支持批量） |
| `open_level` | Modifying | 打开/新建/保存关卡 |

### 属性与变换（6 个工具）

| 工具名 | 行为 | 说明 |
|--------|------|------|
| `list_properties` | ReadOnly | 列出 Actor 可编辑属性（递归展开） |
| `get_property` | ReadOnly | 读取属性值（点分路径） |
| `set_property` | Modifying | 设置属性值（反射写入） |
| `batch_set_property` | Modifying | 批量设置多个 Actor 同一属性 |
| `get_actor_transform` | ReadOnly | 读取 Actor 位置/旋转/缩放 |
| `set_actor_transform` | Modifying | 设置 Actor 位置/旋转/缩放 |

### SuperStage 资产（1 个工具）

| 工具名 | 行为 | 说明 |
|--------|------|------|
| `list_super_assets` | ReadOnly | 列出 SuperStage 资产库 |

### 材质编辑（2 个工具，28+ 子操作）

| 工具名 | 行为 | 说明 |
|--------|------|------|
| `material_editor` | Modifying | 材质图完整编辑（28 种操作） |
| `material_operations` | Modifying | 材质实例创建/参数设置/赋材质 |

### 蓝图与动画（3 个工具）

| 工具名 | 行为 | 说明 |
|--------|------|------|
| `blueprint_query` | ReadOnly | 蓝图查询（变量/函数/节点/引脚） |
| `blueprint_modify` | Modifying | 蓝图修改（变量/函数/节点/连线） |
| `anim_blueprint_modify` | Modifying | 动画蓝图状态机编辑 |

### 资产管理（4 个工具）

| 工具名 | 行为 | 说明 |
|--------|------|------|
| `asset_search` | ReadOnly | 搜索项目资产（类型/路径/名称） |
| `asset_operations` | Modifying | 资产级属性读写/保存 |
| `asset_dependencies` | ReadOnly | 查询资产依赖链 |
| `asset_referencers` | ReadOnly | 查询资产被谁引用 |

### 视口控制（3 个工具）

| 工具名 | 行为 | 说明 |
|--------|------|------|
| `get_viewport_info` | ReadOnly | 获取 Viewport 摄像机信息 |
| `set_viewport_camera` | Modifying | 设置 Viewport 摄像机 |
| `capture_viewport` | ReadOnly | 截取视口截图（base64 JPEG） |

### 文档与日志（3 个工具）

| 工具名 | 行为 | 说明 |
|--------|------|------|
| `list_docs` | ReadOnly | 列出产品文档目录 |
| `read_doc` | ReadOnly | 读取产品文档内容 |
| `get_output_log` | ReadOnly | 读取引擎输出日志 |

### 撤销与安全（4 个工具）

| 工具名 | 行为 | 说明 |
|--------|------|------|
| `undo` | Modifying | 撤销（Ctrl+Z） |
| `redo` | Modifying | 重做（Ctrl+Y） |
| `execute_console_command` | Modifying | 白名单控制台命令 |
| `execute_script` | Destructive | Python/控制台脚本（万能后备） |

---

## 行为标注说明

| 标注 | 含义 |
|------|------|
| **ReadOnly** | 只读操作，零副作用 |
| **Modifying** | 会修改场景/资产内容，支持撤销 |
| **Destructive** | 破坏性操作，需要用户确认 |

---

## 常用工作流速查

### 放置 SuperStage 灯具

```
list_super_assets → spawn_actor → set_property(ControlMode) → list_properties → set_property × N
```

### 场景批量调整

```
find_actors_by_class → batch_set_property
```

### 创建完整材质

```
create_material → set_material_properties → batch_add_nodes → batch_connect → auto_layout → compile → save_material
```

### 回答 SuperStage 使用问题

```
list_docs → read_doc → 回答用户
```

### 理解用户视角

```
get_viewport_info → 计算相对位置 → spawn_actor / set_actor_transform
```
