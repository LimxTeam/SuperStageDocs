# Super Crowd 用户手册

## 功能定位

Super Crowd 用于在封闭样条区域内生成静态人群实例。它按目标人数或目标密度生成点位，再根据角色类型权重分配 Static Mesh，并可选择向地面射线吸附。

适合用于观众区预演、活动人流密度预估和舞台前区占位效果。

插件随包提供 **13 个 VAT 观众角色**，每个角色带 Anim_V1 / Anim_V2 两套动画材质与基础 / 自发光两层表现，角色网格带 LOD。对应两个可放置对象：**Super Crowd V1** 与 **Super Crowd V2**，区别只在套用哪一套动画材质；两者都会在 `CharacterTypes` 为空时自动填入这 13 个角色。

也可以在 `CharacterTypes` 里改用项目自有或已获授权的 Static Mesh 和材质——填了就不会被自动填充覆盖。

## 基本使用

1. 在关卡中放置 Super Crowd。
2. 编辑 Actor 的样条点，让样条围成封闭区域。
3. 在 `CharacterTypes` 中添加至少一个启用的角色类型，并指定 Static Mesh。
4. 设置 `TargetCount`，或启用 `bUseDensityMode` 后设置 `TargetDensity`。
5. 根据需要启用地面吸附、偏航偏差和缩放。

如果没有有效角色 Mesh，组件不会生成可见人群。

## 角色类型

| 字段 | 说明 |
| --- | --- |
| `Mesh` | 角色 Static Mesh。 |
| `MaterialOverride` | 可选材质覆盖。 |
| `Weight` | 随机权重，数值越大越容易被选中。 |
| `Scale` | 基础缩放。 |
| `ScaleVariation` | 随机缩放浮动。 |
| `FootOffsetZ` | 脚底高度偏移。 |
| `BaseRotationOffset` | 基础旋转偏移。 |
| `DisplayName` | 编辑器内显示名称。 |
| `bEnabled` | 是否参与生成。 |
| `bCastShadow` | 是否投射阴影。 |

## 生成参数

| 参数 | 说明 |
| --- | --- |
| `TargetCount` | 目标人数。实际生成数量可能低于目标值。 |
| `SafetyRadius` | 人物之间的安全半径，单位厘米。 |
| `BoundaryPadding` | 距离样条边界的留空距离。 |
| `RandomSeed` | 随机种子，相同设置下可复现分布。 |
| `bUseDensityMode` | 使用密度模式。开启后根据区域面积和 `TargetDensity` 计算目标数量。 |
| `TargetDensity` | 目标密度，单位人/平方米。 |
| `PoissonAttemptsPerPoint` | 每个采样点的尝试次数。 |
| `GroundOversampleMultiplier` | 启用地面检测时的过采样倍率。 |
| `bRelaxSpacingWhenCrowded` | 目标过密时允许逐步放宽安全半径。 |
| `MinRelaxedSpacingScale` | 安全半径可放宽到的最小比例。 |
| `SpacingRelaxationPasses` | 放宽间距的尝试轮数。 |
| `SplineSampleSpacing` | 样条采样间距，单位厘米。 |

## 地面和外观参数

| 参数 | 说明 |
| --- | --- |
| `bSnapToGround` | 启用向地面射线吸附。 |
| `GroundChannel` | 地面检测使用的碰撞通道。 |
| `RaycastStartHeight` | 射线起点高度。 |
| `RaycastDepth` | 向下检测深度。 |
| `DefaultGroundHeight` | 未吸附时使用的默认高度。 |
| `GroundOffset` | 地面高度偏移。 |
| `bAlignToGroundNormal` | 按地面法线调整朝向。 |
| `bRejectSteepGround` | 拒绝过陡地面。 |
| `MaxGroundSlopeDegrees` | 允许的最大地面坡度。 |
| `bFlattenToSplinePlane` | 将点位压回样条平面。 |
| `bRandomYaw`（Enable Yaw Variation） | 启用偏航偏差，在基础朝向两侧做受控偏移。 |
| `GlobalMaxRotationVariation`（Yaw Variation Range） | 偏航偏差范围，0–180 度，默认 0。 |
| `bFaceSplineCenter`（Face Spline Center） | 朝向样条围合区域的中心。默认关。 |
| `bFaceAwayFromCenter`（Face Away From Center） | 背对中心。默认关。 |
| `GlobalScaleFactor`（Global Scale Factor） | 全局缩放，0.1–10，默认 1.0。 |
| `GlobalScaleVariation`（Global Scale Variation） | 全局缩放随机浮动，0–0.5，默认 0。 |
| `bCastShadows`（Cast Shadows） | 投射阴影。 |
| `bEnableInstanceCollision`（Enable Instance Collision） | 启用实例碰撞。 |
| `OverrideMaterial`（Override Material） | 覆盖材质。 |
| `bOverrideAllMaterialSlots`（Override All Material Slots） | 覆盖全部材质槽。 |
| `bShowSpline`（Show Spline） | 显示样条。 |
| `bDebugShowSafetyRadius`（Debug Show Safety Radius） | 调试显示安全半径。 |

## 统计信息

组件会更新：

- `ActualCount`：实际生成数量。
- `NormalizedWeights`：归一化后的角色权重。
- `CrowdStats.TotalCount`、`RequestedCount`、`CountPerType`：数量统计。
- `CrowdStats.ValidCharacterTypes`：有效角色类型数量。
- `CrowdStats.AreaM2`、`Density`：区域面积和实际密度。
- `CrowdStats.GroundSnappedCount`：成功地面吸附数量。
- `CrowdStats.FailedPlacements`、`PlacementAttempts`：失败和尝试次数。
- `CrowdStats.RejectedByBoundary`、`RejectedByGround`、`RejectedBySlope`：被边界、地面检测或坡度过滤的点位数量。
- `CrowdStats.EffectiveSafetyRadius`、`AverageSpacing`、`MinimumSpacing`：最终间距统计。

## 使用注意

- 样条需要围成有效区域，否则面积和分布结果会异常。
- 目标人数不保证一定达到；区域太小、安全半径太大或地面过滤太严格时会减少。
- 大量实例会影响编辑器性能，建议先用较小数量确认区域和角色设置。
