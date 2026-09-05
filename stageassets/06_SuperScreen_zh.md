# Super Screen 用户说明

Super Screen 用于把当前媒体纹理显示到一个或多个 StaticMeshActor 上。当前实现中它会创建独立动态材质实例，并把活动纹理、颜色、亮度、对比度、透明度和梯形校正参数写入材质。

## 使用方式

1. 在场景中放置或准备作为屏幕的 StaticMeshActor。
2. 放置 Super Screen。
3. 在 `ScreenMeshActors` 中添加目标 StaticMeshActor。
4. 设置 `MaterialIndex`，确认目标网格的材质槽存在。
5. 选择媒体源，并根据需要调整画面参数。

## 参数

| 参数 | 作用 |
| --- | --- |
| ScreenMeshActors | 接收屏幕材质的 StaticMeshActor 列表。 |
| MaterialIndex | 应用动态材质的材质槽索引；越界目标会被跳过。 |
| Transparent | 在不透明材质和透明材质之间切换。 |
| Transparency | 透明模式下写入材质的 Opacity 参数。 |
| Brightness | 写入材质的 Brightness 参数。 |
| Color | 写入材质的 Color 参数。 |
| Contrast | 写入材质的 Contrast 参数。 |
| Deformation | 四角梯形校正参数，写入材质中的八个 UV 偏移值。 |

## 需要注意

- Super Screen 本身不创建 LED 屏模型；它把材质应用到你指定的 StaticMeshActor。
- 每个 Screen Actor 会创建自己的动态材质实例，避免复制/粘贴后共享同一个 MID。
- 画面是否更新取决于媒体源是否提供有效活动纹理。
- 梯形校正用于画面对位预演，不保证和真实 LED 像素边界完全一致。
