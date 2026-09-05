# 09 - MA2 XML 导出

> **所属模块**: SuperTools — 控台导出  
> **适用对象**: 灯光编程师、舞美设计师  
> **前置阅读**: [07 - Patch 工具](./07_Patch_Tools_zh.md)  
> **最后更新**: 2026-04-14

---

## 一、概述

**DMXToMa** 工具会把当前关卡中的 SuperDMX 灯具导出为 grandMA2 XML 文件。导出结果包含：

- 每个灯具型号一份 fixture layer XML
- 一份 `SuperStageToMA.xml` 宏文件，用于导入 layer、Patch 灯具并按型号创建 Group

这是一份一次性导出快照。修改 UE 场景、FixtureID、Universe、StartAddress、位置或旋转后，需要重新导出。

---

## 二、打开方式

在 SuperStage 工具栏的 SuperDMX 菜单中点击 **DMXToMa**，打开 **DMXToMa** 标签页。

---

## 三、界面控件

| 控件 | 说明 |
|------|------|
| **Export Directory** | 当前导出根目录，默认指向桌面；不可直接编辑 |
| **Browse** | 选择导出根目录 |
| **Refresh** | 重新扫描当前关卡中的 SuperDMX 灯具 |
| **Select All** | 勾选所有型号 |
| **Select None** | 取消勾选所有型号 |
| **Export** | 生成 fixture layer 和宏文件 |

型号列表包含以下列：

| 列 | 说明 |
|----|------|
| **Export** | 是否导出该型号 |
| **Model** | 灯具型号/类型名 |
| **Count** | 当前关卡中该类灯具数量 |
| **FixtureType No** | MA2 控台中对应的 FixtureType 编号，必须手动填写且大于 0 |

> **注意**：当前实现没有真正保存 FixtureType No 配置；重新打开工具后需要检查这些编号。

---

## 四、导出步骤

1. 在关卡中完成灯具摆放和 Patch。
2. 打开 **DMXToMa**。
3. 点击 **Browse** 选择导出根目录。
4. 点击 **Refresh** 确认型号列表是最新的。
5. 勾选需要导出的型号。
6. 为每个勾选型号填写 **FixtureType No**。
7. 点击 **Export**。

如果勾选型号的 FixtureType No 小于等于 0，工具会弹窗提示并停止导出。

---

## 五、导出目录和文件

每次导出会在根目录下创建一个带时间戳的批次文件夹：

```
SuperDMX_YYYYMMDD_HHmmss
  ├── fixture_layers
  │   ├── LAYER_ModelName-1.xml
  │   └── ...
  └── macros
      └── SuperStageToMA.xml
```

文件编码为 UTF-8 无 BOM。

### fixture layer XML

每个勾选的灯具型号会生成一份 layer XML。文件中包含该型号下每台灯具的：

| 内容 | 来源 |
|------|------|
| FixtureID | 灯具 Fixture ID |
| FixtureType No | 列表中填写的 FixtureType No |
| 位置 | Actor 世界位置，经 UE 到 MA 坐标转换 |
| 旋转 | Actor 世界旋转，经 UE 到 MA 旋转转换 |

layer XML 中的 Patch Address 写为 0；实际 DMX Patch 由宏文件执行。

### 宏文件

`SuperStageToMA.xml` 包含两个宏：

| 宏 | 作用 |
|----|------|
| `SuperStageToMA import layers` | 进入 MA2 的 EditSetup/Layers 并导入生成的 layer 文件 |
| `SuperStageToMA patch+groups` | 按 FixtureID 执行 `Assign Fixture ... At Dmx ...`，并按型号创建 Group |

宏里的灯具按 FixtureID 升序处理。

---

## 六、坐标转换

导出时使用以下当前实现转换：

| MA 字段 | 来源 |
|---------|------|
| X | UE Y / 100 |
| Y | -UE X / 100 |
| Z | UE Z / 100 |
| Rot X | UE Roll |
| Rot Y | UE Pitch + 270 |
| Rot Z | UE Yaw + 180 |

位置单位从 UE 厘米转换为米。旋转转换只是工具当前实现，导入控台后仍建议抽查灯位方向。

---

## 七、导入到 MA2

1. 确认 MA2 控台中已经有对应 FixtureType，并记下编号。
2. 将 `fixture_layers` 和 `macros` 中的 XML 文件放到 MA2 可导入的位置。
3. 导入并执行 `SuperStageToMA.xml`。
4. 宏会导入 layer、执行 Patch，并按型号创建 Group。

导出工具不会创建或导入 Fixture Profile。FixtureType No 必须与控台中已有的灯具类型编号一致。

---

## 八、常见问题

### Q: Export 提示 FixtureType No 无效？
给所有勾选型号填写大于 0 的 FixtureType No。未勾选型号不会参与校验。

### Q: 导出后没有文件？
确认当前关卡里有 SuperStage DMX 灯具，并且至少勾选了一个型号。

### Q: 导入后 Patch 不对？
检查 UE 中每台灯具的 FixtureID、Universe、StartAddress 是否正确，并确认宏中的 FixtureType No 与 MA2 控台里的类型编号一致。

### Q: 位置或旋转不符合预期？
先确认 UE 场景中的 Actor 世界位置和旋转是最终状态，再按上面的坐标转换规则抽查。不同控台环境可能仍需要人工校正。

### Q: 能导出 CSV 吗？
当前工具栏只注册了 DMXToMa。仓库里只有 CSV 工具头文件声明，没有对应实现和入口，本手册不把 CSV 导出作为可用功能描述。

---

> **返回总览**：[00 - DMX 系统总览](./00_DMX_System_Overview_zh.md)
