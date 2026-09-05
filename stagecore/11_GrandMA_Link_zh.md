# 11 - GrandMALink

> **所属模块**: SuperTools  
> **适用对象**: 灯光编程师、控台技术人员  
> **前置阅读**: [07 - Patch 工具](./07_Patch_Tools_zh.md)、[09 - MA2 XML 导出](./09_Export_To_MA_zh.md)  
> **最后核对**: 2026-06-28

---

## 一、功能范围

GrandMALink 用于把当前 UE 关卡中的 SuperStage DMX 灯具整理后发送到 grandMA2 或 grandMA3 环境。它面向控台准备和 Patch 同步，不是实时 DMX 传输工具，也不替代控台工程文件的人工检查。

当前面板包含 **MA2** 和 **MA3** 两个页签。两个页签都提供：

- 选择 Host。
- 连接或断开控台。
- 刷新当前关卡中的灯具型号列表。
- 勾选要导入的型号并执行 **Import Patch**。
- 在已有同步缓存后刷新差异列表，并对选中的差异执行 **Sync Selected**。

导入或同步结果必须在目标 grandMA 环境中检查。控台版本、网络权限、登录设置、OSC/Telnet 配置和灯库文件都会影响实际结果。

---

## 二、打开方式

在 SuperStage 工具栏中打开 **GrandMALink**。

面板顶部有 MA2 / MA3 页签和状态栏。点击 **Refresh Adapters** 会重新扫描本机的 IPv4 网卡，并在 Host 下拉框中显示。

---

## 三、连接设置

### MA2 页签

当前实现使用以下默认连接参数：

| 项 | 当前值 |
| --- | --- |
| Host | 从本机 IP 列表选择，默认也包含 `127.0.0.1` |
| Port | `30000` |
| User | `administrator` |
| Password | `admin` |

MA2 使用 TCP/Telnet 方式发送命令。使用前请确认 grandMA2 / MA2 onPC 允许对应网络连接，并且防火墙没有阻止端口。

### MA3 页签

当前实现使用以下默认连接参数：

| 项 | 当前值 |
| --- | --- |
| Host | 从本机 IP 列表选择，默认也包含 `127.0.0.1` |
| Port | `8000` |
| OSC Prefix | `gma3` |

MA3 使用 OSC 方式发送命令。使用前请确认 grandMA3 / MA3 onPC 的 OSC 输入设置与这些参数一致。

---

## 四、型号列表

点击 **Refresh Fixtures** 后，面板会扫描当前关卡中的 SuperStage DMX 灯具，并按灯具型号/类型分组显示：

| 列 | 说明 |
| --- | --- |
| Import | 是否参与 Import Patch |
| Model | 灯具型号/类型名 |
| Count | 当前关卡中该型号数量 |
| GDTF Library | 该型号绑定的 GDTF 灯库路径。MA2 页签所需的 MA2 XML 由它现场转换生成 |

MA2 与 MA3 两个页签都只需要通道库资产绑定 **GDTF 灯库**：MA3 直接用 GDTF，MA2 所需的 XML 由 GDTF 现场转换生成，不再需要手工维护的 MA2 XML（26H2.6 起）。未绑定 GDTF 时，该型号会显示为未绑定，导入会停止并提示。

---

## 五、Import Patch

**Import Patch** 用于把选中型号下的灯具导入到目标 grandMA 环境，并保存当前关卡与控台工程之间的同步缓存。

建议流程：

1. 在 UE 中确认灯具的 FixtureID、Universe、Start Address、位置和旋转。
2. 确认每个型号已绑定对应的 GDTF 灯库。
3. 打开 **GrandMALink**，选择 MA2 或 MA3 页签。
4. 选择正确 Host，点击 **Connect**。
5. 点击 **Refresh Fixtures**。
6. 勾选要导入的型号。
7. 点击 **Import Patch**。
8. 到目标 grandMA 环境中检查灯具类型、Patch、分组、位置和方向。

Import Patch 会创建或更新同步缓存。之后才能使用差异刷新和同步流程。

---

## 六、差异刷新与同步

完成一次 Import Patch 后，后续如果 UE 关卡中的灯具发生变化，可以使用差异同步：

1. 点击 **Refresh Changes**。
2. 检查 **Pending Changes** 列表。
3. 勾选要同步的变化。
4. 点击 **Sync Selected**。
5. 在控台中抽查同步结果。

差异列表里每条变化标着类型：

| 标签 | 含义 | MA2 | MA3 |
| --- | --- | --- | --- |
| **Added** | 关卡里新增、控台上还没有的灯具 | 有 | 有 |
| **Modified** | 两边都有但配接不一致 | 有 | 有 |
| **Deleted** | 控台上有、关卡里已经没有的 | 不列出 | 有 |

MA2 当前的同步流程只处理 Added 与 Modified。实际能否执行成功仍取决于目标控台工程与连接状态。

---

## 七、发不过去时

**状态一直是 Offline**
控台不可达，或者它没有开放远程连接。检查地址，以及两台机器是不是在同一个网段上。

**某个型号是灰的，或者拒绝发送**
基本可以肯定它显示的是 **Unbound**——去给这个型号绑上灯库，然后刷新。

**场景明明改过了，Pending Changes 却是空的**
点一下 **Refresh Changes**。这个列表不会自己更新。

**灯具发过去以后通道对不上**
绑到这个型号上的灯库不是你实际在用的那支灯。重新绑一次再发。

---

## 八、使用边界

- GrandMALink 发送的是控台命令，不是实时 DMX 数据。
- 首次同步前需要先执行 **Import Patch**，否则没有当前关卡的同步缓存。
- 关卡未保存或切换关卡后，同步缓存可能与当前场景不匹配，需要重新检查。
- 灯具型号未绑定正确的 GDTF 灯库时，导入不能可靠完成。
- 网络连通不代表控台一定接受所有命令，导入后必须在控台端检查。

---

> **返回总览**：[00 - DMX 系统总览](./00_DMX_System_Overview_zh.md)
