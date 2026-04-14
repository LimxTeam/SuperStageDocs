# SuperAI 工具 — 材质编辑

## 概述

`material_editor` 是 SuperAI 中功能最丰富的单个工具，提供 **28 种操作** 覆盖材质图编辑的完整工作流：创建材质、构建节点图、编写 Custom HLSL、材质函数、批量操作、自动布局、纹理管理等。

---

## material_editor — 材质图编辑工具

### 公共参数

所有操作共享以下参数（具体操作可能只用到其中一部分）：

| 参数 | 类型 | 说明 |
|------|------|------|
| `operation` | string | 操作类型（必填） |
| `material_path` | string | 材质资产路径（大多数操作必填） |

### 行为标注

`Modifying`

---

## 操作分类

### 基础操作

| 操作 | 说明 | 关键参数 |
|------|------|----------|
| `create_material` | 创建新 UMaterial 资产 | `package_path`、`material_name` |
| `add_node` | 添加材质表达式节点 | `node_type`、`node_pos` |
| `delete_node` | 删除节点 | `node_id` |
| `duplicate_node` | 复制已有节点 | `node_id` |

### 连接操作

| 操作 | 说明 | 关键参数 |
|------|------|----------|
| `connect_nodes` | 连接节点 A 的输出到节点 B 的输入 | `source_node_id`、`source_output_index`、`target_node_id`、`target_input_index`（或 `target_input_name`） |
| `connect_to_output` | 连接节点到材质输出引脚 | `node_id`、`output_pin`（如 `BaseColor`、`Metallic`、`EmissiveColor`） |
| `disconnect_nodes` | 断开两个节点之间的连接 | `source_node_id`、`target_node_id` |
| `disconnect_from_output` | 断开材质输出引脚的连接 | `output_pin` |

### 属性操作

| 操作 | 说明 | 关键参数 |
|------|------|----------|
| `set_node_values` | 设置节点属性值（反射写入） | `node_id`、`values` |
| `list_node_properties` | 列出节点全部可编辑属性 | `node_id` |
| `get_node_info` | 查看节点详情（属性值+连接） | `node_id` |
| `set_material_properties` | 设置材质级属性（BlendMode 等） | `properties` |
| `set_node_description` | 设置节点描述 | `node_id`、`description` |

### Custom HLSL 操作

| 操作 | 说明 | 关键参数 |
|------|------|----------|
| `add_node`（type=Custom） | 创建 Custom HLSL 节点 | `custom_hlsl`、`custom_output_type` |
| `add_custom_input` | 为 Custom 节点添加输入引脚 | `node_id`、`input_name`、`input_type` |
| `add_custom_output` | 为 Custom 节点添加额外输出 | `node_id`、`output_name` |
| `remove_custom_input` | 删除 Custom 节点的输入引脚 | `node_id`、`input_index` |
| `remove_custom_output` | 删除 Custom 节点的额外输出 | `node_id`、`output_index` |

### 材质函数操作

| 操作 | 说明 | 关键参数 |
|------|------|----------|
| `create_material_function` | 创建可复用的材质函数 | `package_path`、`material_name` |
| `call_material_function` | 添加 MaterialFunctionCall 节点 | `function_path` |

### 批量操作

| 操作 | 说明 | 关键参数 |
|------|------|----------|
| `batch_add_nodes` | 批量添加多个节点 | `nodes` 数组（每项：`{type, pos:{x,y}, values:{...}}`） |
| `batch_connect` | 批量建立多个连接 | `connections` 数组（每项：`{src_id, src_out, dst_id, dst_in}`） |

### 布局操作

| 操作 | 说明 | 关键参数 |
|------|------|----------|
| `set_node_position` | 设置节点在编辑器中的位置 | `node_id`、`node_pos` |
| `auto_layout` | 自动排布所有节点 | — |
| `add_comment` | 添加注释框 | `comment_text`、`comment_size`、`node_ids` |

### 纹理操作

| 操作 | 说明 | 关键参数 |
|------|------|----------|
| `find_textures` | 搜索项目内纹理资产 | `texture_folder`、`search` |
| `import_texture` | 从文件导入纹理 | `file_path`、`package_path` |

### 辅助操作

| 操作 | 说明 | 关键参数 |
|------|------|----------|
| `get_graph` | 获取材质图全部节点和连接信息 | — |
| `list_node_types` | 列出可用的材质表达式类型 | `search` |
| `compile` | 强制编译材质 | — |
| `save_material` | 保存材质到磁盘 | — |

---

## 典型工作流

### 创建基础材质

```
1. create_material(package_path="/Game/Materials", material_name="M_MyMaterial")
2. set_material_properties(properties={BlendMode:"Opaque", ShadingModel:"DefaultLit"})
3. batch_add_nodes(nodes=[
     {type:"Constant3Vector", pos:{x:-400,y:0}, values:{Constant:{r:0.8,g:0.2,b:0.1}}},
     {type:"Constant", pos:{x:-400,y:200}, values:{R:0.5}},
     {type:"Constant", pos:{x:-400,y:300}, values:{R:0.0}}
   ])
4. batch_connect(connections=[
     {src_id:0, output_pin:"BaseColor"},
     {src_id:1, output_pin:"Metallic"},
     {src_id:2, output_pin:"Roughness"}
   ])
5. auto_layout
6. compile
7. save_material
```

### Custom HLSL 节点工作流

```
1. add_node(node_type="Custom",
     custom_hlsl="return sin(Time * Speed) * 0.5 + 0.5;",
     custom_output_type="Float1")
2. add_custom_input(node_id=<id>, input_name="Speed", input_type="Float1")
3. connect_nodes(source → Custom 的 Speed 输入)
4. 如需多输出: add_custom_output(output_name="Extra")
```

### 材质函数工作流

```
1. create_material_function(function_path=...) — 创建函数
2. 在函数内 add_node / connect_nodes 构建子图
3. 在主材质中 call_material_function(function_path=...) — 引用
```

---

## 重要提示

- **复杂材质必须用 `batch_add_nodes` + `batch_connect`**，禁止逐个 `add_node`（减少 API 调用次数）
- 不确定节点类型时调用 `list_node_types(search='...')`
- 不确定节点属性时调用 `list_node_properties(node_id=...)`
- 需要纹理时先 `find_textures` 搜索项目资产，找不到则 `import_texture`
- 建图完成后调用 `auto_layout` 整理布局
- 修改后务必 `compile` 检查错误，然后 `save_material` 保存
