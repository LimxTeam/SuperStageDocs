# SuperAI 工具 — 资产管理

## 概述

资产管理类工具操作 Content Browser 中的资产（UObject），与操作场景 Actor 的 `set_property` / `get_property` 不同。涵盖通用资产搜索、依赖分析、引用查询、资产级属性读写、材质实例管理等。

---

## asset_search — 搜索项目资产

在项目中搜索资产，支持按类型、路径、名称过滤。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `class_filter` | string | — | 资产类型（如 `Blueprint`、`StaticMesh`、`Texture2D`、`Material`） |
| `path_filter` | string | — | 路径前缀过滤（如 `/Game/`、`/Engine/`） |
| `name_pattern` | string | — | 名称子串匹配（不区分大小写） |
| `limit` | number | — | 最大结果数（默认 25，最大 500） |
| `offset` | number | — | 分页跳过数量 |

### 行为标注

`ReadOnly`

### 常见资产类型

`Blueprint`、`StaticMesh`、`SkeletalMesh`、`Texture2D`、`Material`、`MaterialInstanceConstant`、`AnimSequence`、`SoundWave`、`NiagaraSystem`

---

## asset_operations — 资产级操作

对 Content Browser 中的资产执行属性读写、保存等操作。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `operation` | string | ✅ | 操作类型：`get_info` / `set_property` / `save` |
| `asset_path` | string | ✅ | 资产路径（如 `/Game/Materials/M_Ground`） |
| `property_name` | string | — | 属性名（`set_property` 用） |
| `property_value` | any | — | 属性值（`set_property` 用） |

### 行为标注

`Modifying`

### 操作说明

| 操作 | 说明 |
|------|------|
| `get_info` | 获取资产类型、路径、大小等基础信息 |
| `set_property` | 通过反射设置资产的 UPROPERTY 属性 |
| `save` | 保存资产到磁盘 |

---

## asset_dependencies — 资产依赖查询

查询指定资产依赖的所有其他资产（**它用了谁**）。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `asset_path` | string | ✅ | 资产路径 |
| `include_soft` | boolean | — | 是否包含软引用（默认 `true`） |
| `limit` | number | — | 最大结果数（默认 50） |

### 行为标注

`ReadOnly`

---

## asset_referencers — 反向引用查询

查询引用了指定资产的所有其他资产（**谁用了它**）。用于修改或删除资产前的影响分析。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `asset_path` | string | ✅ | 资产路径 |
| `limit` | number | — | 最大结果数（默认 50） |

### 行为标注

`ReadOnly`

---

## material_operations — 材质操作

材质实例的创建、参数设置、赋材质到 Actor 等操作。与 `material_editor` 的区别：`material_editor` 操作材质图节点，`material_operations` 操作材质实例参数。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `operation` | string | ✅ | 操作类型 |
| `material_path` | string | — | 材质/材质实例路径 |
| `parent_material` | string | — | 父材质路径（`create_instance` 用） |
| `package_path` | string | — | 新资产包路径 |
| `instance_name` | string | — | 材质实例名称 |
| `parameters` | object | — | 参数对象 |
| `actor_name` | string | — | 目标 Actor（`set_actor_material` 用） |
| `slot_index` | number | — | 材质槽索引（默认 0） |

### 行为标注

`Modifying`

### 操作说明

| 操作 | 说明 |
|------|------|
| `create_instance` | 从父材质创建材质实例 |
| `set_parameters` | 设置材质参数（标量、向量、纹理） |
| `get_info` | 获取材质信息 |
| `set_actor_material` | 将材质赋给场景中的 Actor |

### 参数格式

```json
{
  "scalar": { "参数名": 数值 },
  "vector": { "参数名": {"r":1, "g":0, "b":0, "a":1} },
  "texture": { "参数名": "/Game/Textures/T_MyTexture" }
}
```

---

## 典型工作流

### 资产依赖分析

```
1. asset_dependencies(asset_path="/Game/Maps/MainLevel")
   → 查看关卡依赖了哪些资产

2. asset_referencers(asset_path="/Game/Materials/M_Old")
   → 删除旧材质前检查哪些资产在使用它
```

### 材质实例工作流

```
1. material_operations(operation="create_instance",
     parent_material="/Game/Materials/M_Base",
     package_path="/Game/Materials",
     instance_name="MI_Red")

2. material_operations(operation="set_parameters",
     material_path="/Game/Materials/MI_Red",
     parameters={scalar:{Roughness:0.3}, vector:{BaseColor:{r:1,g:0,b:0,a:1}}})

3. material_operations(operation="set_actor_material",
     material_path="/Game/Materials/MI_Red",
     actor_name="Cube_01")
```
