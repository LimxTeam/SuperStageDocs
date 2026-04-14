# SuperAI 工具 — 属性与变换

## 概述

属性与变换类工具通过 UE 反射系统实现对 Actor 属性的通用读写操作。所有 UPROPERTY 均可通过点分路径精确访问，包括嵌套结构体和组件属性。变换操作使用专用工具确保物理引擎和附着系统正确更新。

---

## list_properties — 列出可编辑属性

列出指定 Actor 上所有可编辑属性的路径、类型和当前值摘要。**使用 `set_property` 前必须先调用此工具获取正确的属性路径。**

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `actor_name` | string | ✅ | Actor 名称或编辑器标签 |
| `category` | string | — | 按 Category 子串过滤（如 `ControlParameter`） |
| `max_depth` | number | — | 结构体递归展开深度（1-4，默认 2） |
| `include_components` | boolean | — | 是否包含组件上的属性（默认 `false`） |

### 行为标注

`ReadOnly`

### 返回格式

每个属性返回：
- **path**：点分路径（如 `LightDefaults.MaxLightIntensity`），可直接传给 `set_property`
- **type**：属性类型（如 `float`、`FVector`、`FLinearColor`、`enum`）
- **value**：当前值摘要

### 使用提示

- SuperStageLight 的控制参数在 `Category='C.ControlParameter'` 下
- 灯光默认值在 `LightDefaults` 结构体中
- 属性路径必须从 `list_properties` 返回结果中获取，**禁止猜测或自行拼凑**

---

## get_property — 读取属性值

读取指定 Actor 的属性值，支持点分路径访问嵌套属性和组件属性。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `actor_name` | string | ✅ | Actor 名称或编辑器标签 |
| `property` | string | ✅ | 属性路径（点分） |

### 行为标注

`ReadOnly`

### 示例路径

- `bHidden` — Actor 隐藏状态
- `RootComponent.RelativeLocation` — 相对位置
- `LightComponent.Intensity` — 灯光强度

---

## set_property — 设置属性值

设置指定 Actor 的属性值。通过 UE 反射系统按点分路径导航到目标属性，从 JSON 值反序列化并写入。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `actor_name` | string | ✅ | Actor 名称或编辑器标签 |
| `property` | string | ✅ | 属性路径（点分） |
| `value` | any | ✅ | 要设置的值 |

### 行为标注

`Modifying`

### 支持的值类型

| 属性类型 | JSON 值格式 |
|----------|------------|
| `bool` | `true` / `false` |
| `int32` / `float` / `double` | 数字 |
| `FString` / `FName` / `FText` | 字符串 |
| `FVector` | `{"x":0,"y":0,"z":0}` |
| `FRotator` | `{"pitch":0,"yaw":0,"roll":0}` |
| `FLinearColor` | `{"r":1,"g":0,"b":0,"a":1}` |
| `enum` | 枚举项名称字符串 |
| `UObject*` 引用 | 资产路径字符串 |

### 重要约束

- **不能用于设置 Actor 变换属性**（位置/旋转/缩放），请使用 `set_actor_transform`
- 属性路径必须使用 `list_properties` 返回的 path
- 对多个 Actor 设置相同属性时，使用 `batch_set_property`

---

## batch_set_property — 批量设置属性

批量设置多个 Actor 的同一属性值，避免逐一调用 `set_property`。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `class_filter` | string | — | 按类名过滤（如 `SuperStageLight`） |
| `name_filter` | string | — | 按名称/标签子串过滤 |
| `actor_names` | array | — | 精确 Actor 名称/标签列表 |
| `property` | string | ✅ | 属性路径（点分） |
| `value` | any | ✅ | 要设置的值 |

### 行为标注

`Modifying`

### 匹配方式（二选一）

1. **过滤模式**：通过 `class_filter` 和/或 `name_filter` 匹配
2. **列表模式**：通过 `actor_names` 数组精确指定

---

## get_actor_transform — 读取 Actor 变换

读取 Actor 的世界变换（位置、旋转、缩放）。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `actor_name` | string | ✅ | Actor 名称或编辑器标签 |

### 行为标注

`ReadOnly`

### 返回值

- `location`：世界坐标位置（厘米）
- `rotation`：旋转角度（度）
- `scale`：缩放

### 使用时机

需要计算相对偏移时（如「往右移 200」），必须先用此工具读取当前变换，再用 `set_actor_transform` 设置。

---

## set_actor_transform — 设置 Actor 变换

设置 Actor 的世界变换（位置、旋转、缩放）。通过 UE 原生 API 设置，确保物理引擎、附着系统、组件层级等正确更新。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `actor_name` | string | ✅ | Actor 名称或编辑器标签 |
| `location` | object | — | 世界坐标 `{"x":0,"y":0,"z":0}`（厘米） |
| `rotation` | object | — | 旋转 `{"pitch":0,"yaw":0,"roll":0}`（度） |
| `scale` | object | — | 缩放 `{"x":1,"y":1,"z":1}` |

### 行为标注

`Modifying`

### 重要约束

- **所有 Actor 位置/旋转/缩放操作必须使用此工具**
- 禁止通过 `set_property` 或 `batch_set_property` 操作 `RelativeLocation` / `RelativeRotation` / `RelativeScale3D`
- 三个参数均为可选，只传需要修改的，未传的保持不变

---

## 属性操作典型工作流

### SuperStage 灯具配置

```
1. list_super_assets(search="LED")          → 找到灯具蓝图路径
2. spawn_actor(class_path=<蓝图路径>)        → 生成灯具
3. set_property(ControlMode="Property")      → 切换为属性直控模式
4. list_properties(actor_name=...,           → 查看可编辑属性
     category="ControlParameter")
5. set_property(property="Dimmer", value=1)  → 逐项配置属性
6. set_actor_transform(location=...)          → 设置位置
```

### 批量调整场景灯光

```
1. find_actors_by_class(class_name="SuperStageLight")  → 列出所有灯具
2. batch_set_property(class_filter="SuperStageLight",   → 批量修改
     property="Dimmer", value=0.8)
```
