# DMXToMa 用户手册

## 1. 概述

**DMXToMa** 工具用于把当前关卡中的 SuperStage DMX 灯具按模型分组导出为 grandMA2 兼容的 XML 图层文件和宏文件。导出的文件需要在 grandMA2 环境中导入、执行并人工检查。

当前实现没有 MA3 / Lua 导出选项，也不会创建 Fixture Profile。每个模型需要手动填写 MA2 控台中已有的 **FixtureType No**。

---

## 2. 打开方式

工具栏 **SuperStage** 下拉菜单 → **SuperDMXTool** → **DMXToMa**

---

## 3. 界面

当前界面包含：

| 控件 | 说明 |
| --- | --- |
| **Export Directory** | 导出根目录。默认优先使用桌面目录。 |
| **Browse** | 选择导出目录。 |
| **Refresh** | 重新扫描当前关卡中的 SuperStage DMX 灯具。 |
| **Select All** | 选中所有模型分组。 |
| **Select None** | 取消选中所有模型分组。 |
| **Export** | 生成导出文件。 |
| **模型列表** | 按灯具型号/类型分组显示 Model、Count、FixtureType No。 |

列表中的 **FixtureType No** 必须大于 0。当前实现中加载/保存该编号的函数目前是占位实现，重新打开工具后需要重新检查编号。

---

## 4. 导出内容

点击 **Export** 后，工具会在导出目录下创建一个时间戳文件夹：

```text
SuperDMX_YYYYMMDD_HHmmss/
  fixture_layers/
    LAYER_ModelName-1.xml
    ...
  macros/
    SuperStageToMA.xml
```

导出内容来自当前关卡中被选中型号分组下的 SuperStage DMX 灯具：

| 数据 | 来源 |
| --- | --- |
| Universe | 灯具 Universe |
| Start Address | 灯具 Start Address |
| Fixture ID | 灯具 Fixture ID |
| 位置/旋转 | Actor 世界坐标和世界旋转 |
| 模型名 | 灯具型号/类型名 |

图层 XML 中 Patch Address 写为 0，实际 Patch 由宏文件中的 `Assign Fixture ... At Dmx ...` 命令完成。

---

## 5. 坐标转换

当前实现使用以下转换写入 MA 图层 XML：

| MA 坐标/旋转 | SuperStage / UE 来源 |
| --- | --- |
| X | UE Y / 100 |
| Y | -UE X / 100 |
| Z | UE Z / 100 |
| Rot X | UE Roll |
| Rot Y | UE Pitch + 270 |
| Rot Z | UE Yaw + 180 |

导出后仍需要在控台中检查舞台方向、坐标原点和灯具朝向。

---

## 6. 使用流程

1. 在场景中确认灯具的 Universe、Start Address 和 Fixture ID。
2. 打开 **DMXToMa**。
3. 点击 **Refresh**。
4. 勾选需要导出的模型分组。
5. 为每个选中模型填写 MA2 中对应的 **FixtureType No**。
6. 选择导出目录。
7. 点击 **Export**。
8. 在 grandMA2 中导入 `fixture_layers` 和 `SuperStageToMA.xml`，执行宏后检查 Patch、分组和位置。

---

## 7. 导出停下来时

**提示 "Please choose an export directory"**
还没设导出目录。

**提示 "Model requires a valid FixtureType No"**
勾选的型号里有一个还没填控台编号。提示里会点名是哪一个。

**提示 "No actors found for export"**
当前关卡里没有灯具，或者勾选的这些型号在关卡里一支都没有。按 **Refresh**，看列表里各型号的数量。

---

## 8. 注意事项

- 只导出选中的模型分组，不是按单个 Actor 勾选。
- FixtureType No 必须对应控台中已有的灯具类型编号。
- 工具不会生成 MA3 Lua 文件。
- 工具不会创建或导入 Fixture Profile。
- 导出文件是交接辅助数据，不是现场控台 Patch 的最终保证。
