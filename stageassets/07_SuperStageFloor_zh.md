# Super Stage Floor 舞台地板 — 用户手册

## 1. 概述

**Super Stage Floor**（舞台地板）是 SuperStage 插件提供的程序化舞台地板生成工具。它可以根据面板数量、规格、高度等参数，**即时生成**完整的舞台地板系统，包括面板、支腿、横撑、裙边装饰和台阶。

### 核心功能

- **参数化布局** — 通过 X/Y 方向面板数和间隙即可控制舞台大小
- **多种面板规格** — 标准 4'×8'、大型 4'×12'、紧凑 StageDex 三种尺寸
- **多种表面材质** — 胶合板、硬木、弹性舞蹈地板、工业防滑钢板
- **支腿类型** — 固定高度、伸缩可调、折叠三种类型
- **裙边装饰** — 无裙边、全包、仅正面、三面四种模式
- **自动台阶** — 按 18cm 踏步高度自动分级的符合规范台阶
- **承载计算** — 基于 ANSI E1.21 标准的重量与承载能力计算

### 适用场景

- 演出舞台搭建预可视化
- 活动舞台布局规划
- 舞台承重方案评估
- 舞台台阶位置和尺寸规划

---

## 2. 如何添加到场景

1. 在 SuperBrowser 资产浏览器中找到 **Super Stage Floor**，拖入视口
2. 或在"放置 Actor"面板中搜索 **"Super Stage Floor"**
3. 选中该 Actor，在细节面板中调整参数

---

## 3. 参数详解

### 3.1 布局参数 (Layout)

| 参数 | 说明 | 默认值 | 范围 |
|------|------|--------|------|
| **PanelCountX** | X 方向面板数量 | 4 | 1 ~ 50 |
| **PanelCountY** | Y 方向面板数量 | 3 | 1 ~ 50 |
| **LayoutMode** | 布局模式 | Grid | Grid / Staggered |
| **Centered** | 网格中心对齐 Actor 原点 | true | — |
| **PanelGap (cm)** | 面板间安装间隙 | 0.2 | 0 ~ 5 |

#### 布局模式说明

| 模式 | 说明 |
|------|------|
| **Grid**（矩形网格） | 面板对齐排列，标准布局 |
| **Staggered**（交错砖砌） | 每隔一排偏移半个面板，接缝更稳定 |

### 3.2 尺寸参数 (Dimensions)

| 参数 | 说明 | 默认值 | 范围 |
|------|------|--------|------|
| **PanelSize** | 面板规格 | Standard | — |
| **SurfaceType** | 表面材质类型 | Plywood | — |
| **PlatformHeight (cm)** | 台面高度（地面到面板顶面） | 40.0 | 10 ~ 300 |
| **LegType** | 支腿类型 | Telescopic | — |

#### 面板规格

| 规格 | 尺寸 | 参考标准 |
|------|------|----------|
| **Standard** | 122×244cm (4'×8') | 最常用尺寸 |
| **Large** | 122×366cm (4'×12') | 快速搭建大面积 |
| **Compact** | 100×200cm (StageDex) | 欧洲标准 |

#### 表面材质类型

| 类型 | 说明 |
|------|------|
| **Plywood**（胶合板） | 18mm 桦木胶合板，演出行业最常用 |
| **Hardwood**（硬木） | 19mm 枫木/橡木实木地板，剧院/音乐厅 |
| **Sprung Dance Floor**（弹性舞蹈地板） | 25mm 弹性复合层，舞蹈/音乐剧专用 |
| **Steel Deck**（防滑钢板） | 3mm 花纹钢板，户外/重载 |

#### 支腿类型

| 类型 | 说明 |
|------|------|
| **Fixed Height**（固定高度） | 焊接固定，最稳定 |
| **Telescopic**（伸缩可调） | 手动/气动调节，最常用 |
| **Folding**（折叠） | 运输/存储方便 |

### 3.3 可见性控制 (Visibility)

| 参数 | 说明 | 默认值 |
|------|------|--------|
| **ShowLegs** | 显示支腿 | true |
| **ShowCrossBraces** | 显示横撑 | true |
| **SkirtType** | 裙边类型 | None |
| **ShowStairs** | 显示台阶 | false |

#### 裙边类型

| 类型 | 说明 |
|------|------|
| **None** | 无裙边 |
| **Full** | 四面全包 |
| **Front Only** | 仅正面（观众面） |
| **Three Sides** | 前面 + 两侧 |

### 3.4 台阶参数 (Stairs)

台阶参数仅在 `ShowStairs = true` 时生效。台阶踏步高度固定为 **18cm**（建筑规范标准），系统根据台面高度自动计算台阶级数。

| 参数 | 说明 | 默认值 | 范围 |
|------|------|--------|------|
| **StairWidth (cm)** | 台阶宽度 | 120 | 60 ~ 500 |
| **StairOffsetY (cm)** | 台阶位置偏移（正面中心为 0） | 0 | — |

> **示例**：台面高度 72cm → 自动生成 4 级台阶（72 ÷ 18 = 4）

### 3.5 材质 (Material)

| 参数 | 说明 |
|------|------|
| **PanelMaterial** | 面板顶面材质 |
| **LegMaterial** | 支腿/横撑材质 |
| **SkirtMaterial** | 裙边面板材质 |
| **StairMaterial** | 台阶材质 |

> 留空则使用 UE 引擎默认材质。

### 3.6 载荷参数 (Load)

| 参数 | 说明 | 默认值 | 范围 |
|------|------|--------|------|
| **AdditionalLoad (kg)** | 额外载荷（设备/人员等） | 0 | 0 ~ 100,000 |

---

## 4. 统计信息（只读）

### 构件数量

| 字段 | 说明 |
|------|------|
| **Panels** | 面板数量 |
| **Legs** | 支腿数量 |
| **CrossBraces** | 横撑数量 |
| **SkirtPanels** | 裙边面板数量 |
| **StairTreads** | 台阶踏板数量 |
| **StairLegs** | 台阶支腿数量 |
| **TotalParts** | 构件总数 |

### 重量/承载

重量和承载数据基于 **ANSI E1.21** 标准计算，数据仅供参考。

| 字段 | 说明 |
|------|------|
| **TotalWeight (kg)** | 自重总计 |
| **MaxLoadCapacity (kN/m²)** | 最大均布荷载 |
| **MaxPointLoad (kN)** | 最大集中荷载 |

---

## 5. 行业标准

| 标准 | 说明 |
|------|------|
| **ANSI E1.21** | 美国演艺技术标准——临时地板/舞台 |
| **ISO 60** | 国际标准面板尺寸 |
| **Wenger / StageRight / Prolyte StageDex** | 面板规格参考系列 |

---

## 6. 性能

Super Stage Floor 使用 6 个 **Instanced Static Mesh Component (ISM)** 渲染所有构件：

| ISM 组件 | 构件 |
|----------|------|
| PanelISM | 面板 |
| LegISM | 支腿 |
| CrossBraceISM | 横撑 |
| SkirtISM | 裙边 |
| StairTreadISM | 台阶踏板 |
| StairLegISM | 台阶支腿 |

所有同类构件共享一次 Draw Call，性能高效。参数修改仅在配置哈希变化时触发重建。

---

## 7. 快速配置示例

### 标准演出舞台

- PanelCountX: 6, PanelCountY: 4
- PanelSize: Standard (122×244cm)
- PlatformHeight: 60cm
- SkirtType: Front Only
- ShowStairs: true, StairWidth: 150cm

### 大型音乐节舞台

- PanelCountX: 16, PanelCountY: 8
- PanelSize: Large (122×366cm)
- SurfaceType: Steel Deck
- PlatformHeight: 120cm
- SkirtType: Three Sides
- ShowStairs: true, StairWidth: 300cm

### 舞蹈排练地板（低台面）

- PanelCountX: 8, PanelCountY: 6
- SurfaceType: Sprung Dance Floor
- PlatformHeight: 15cm
- ShowLegs: false（低台面无需支腿）
- SkirtType: None
