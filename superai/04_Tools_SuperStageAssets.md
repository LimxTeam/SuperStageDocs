# SuperAI 工具 — SuperStage 资产

## 概述

SuperStage 资产工具是 SuperAI 的核心能力之一——AI 可以浏览 SuperStage 资产库，帮助用户找到合适的灯具、桁架、幕布等舞台设备，并在场景中生成和配置。

---

## list_super_assets — 列出 SuperStage 资产

扫描 AssetRegistry 中所有 `ASuperBaseActor` 子类蓝图，返回可用于 `spawn_actor` 的资产列表。

### 参数

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `group` | string | — | 按分组过滤（如 `Moving Head`、`LED`） |
| `manufacturer` | string | — | 按制造商过滤 |
| `search` | string | — | 按显示名称搜索（子串匹配） |

### 行为标注

`ReadOnly`

### 返回字段

| 字段 | 说明 |
|------|------|
| `display_name` | 资产显示名称 |
| `group` | 分组分类 |
| `manufacturer` | 制造商 |
| `class_path` | 蓝图类路径（可直接传给 `spawn_actor`） |
| `forward_direction` | 正面朝向 |
| `up_direction` | 向上方向 |

### 重要约束（最高优先级）

```
⚠️ 禁止使用 /Script/SuperStage.XXX 形式的 C++ 类路径生成 SuperStage 资产！
```

- SuperStage 资产只能使用本工具返回的蓝图 `class_path`
- C++ 基类没有模型和预设参数，生成后是空白无模型的
- 所有灯光 Actor 的灯头沿 `up_direction` 出射

### SuperStage 资产完整工作流（必须严格遵守）

```
步骤 1: list_super_assets           → 选择资产，记录蓝图 class_path
步骤 2: spawn_actor(class_path=...)  → 生成资产
步骤 3: set_property(ControlMode='Property')  → 切换为属性直控模式
步骤 4: list_properties(actor_name=...)        → 查看全部可编辑属性
步骤 5: set_property(...)            → 根据 list_properties 结果逐项配置
```

> ⚠️ **禁止跳过步骤 3-5！** 默认值下灯具不亮或参数不对。不同资产的可用参数完全不同，禁止猜测属性名，必须以 `list_properties` 为准。

---

## SuperStage 方向约定

所有 SuperStage 资产统一使用以下方向约定：

| 方向 | 含义 |
|------|------|
| `ForwardArrow` | 正面朝向（资产的"正面"） |
| `UpArrow` | 向上方向 |

### 灯光特殊处理

- 灯光 Actor 的灯头沿 `UpArrow` 方向出射
- `spawn_actor` 的 `facing_direction` 参数仅对灯光 Actor 有效
- 指定 `facing_direction` 后，系统自动根据 `UpArrow` 计算旋转

### 非灯光资产

- 桁架、幕布、脚手架等资产的 `UpArrow` 始终朝上
- 这些资产请用 `rotation` 参数直接控制朝向

---

## 资产分类

SuperStage 资产库中包含以下类型的舞台设备：

### 灯具类

- **Moving Head**：电脑摇头灯（切割灯、染色灯、光束灯等）
- **LED**：LED 帕灯、LED 条灯等
- **Conventional**：传统灯具（成像灯、菲涅尔灯等）
- **Effect**：效果灯具（频闪、追光灯等）

### 舞台结构

- **Truss**：桁架（直线桁架、圆形桁架、弧形桁架、桁架网格、桁架塔）
- **Gantry**：龙门架（GoalPost / T形 / Portal / DoubleSpan）
- **Scaffold**：脚手架（直线脚手架、弧形脚手架）

### 舞台装饰

- **Drape**：幕布（多种褶皱和开合方式）
- **Crowd**：程序化人群

### 使用示例

```
用户: "帮我找一个 LED 帕灯"
AI:   list_super_assets(group="LED")
      → 返回所有 LED 分组的资产列表
      → 推荐最匹配的资产给用户

用户: "在舞台上方 6 米放一排 8 台染色灯"
AI:   list_super_assets(search="染色")
      → 找到染色灯蓝图路径
      spawn_actor × 8（计算等间距位置）
      set_property × 8（配置属性）
```
