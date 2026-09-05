# 11 - GrandMALink

> **Module**: SuperTools  
> **Target Users**: Lighting programmers, console technicians  
> **Prerequisites**: [07 - Patch Tools](./07_Patch_Tools_en.md), [09 - MA2 XML Export](./09_Export_To_MA_en.md)  
> **Last Verified**: 2026-06-28

---

## 1. Scope

GrandMALink sends fixture and patch data from the current UE level to a grandMA2 or grandMA3 environment. It is intended for console preparation and patch synchronization. It is not a real-time DMX transport tool, and it does not replace manual checking inside the console show file.

The panel has **MA2** and **MA3** tabs. Both tabs provide:

- Host selection.
- Connect and disconnect.
- Fixture model refresh for the current level.
- Model selection and **Import Patch**.
- Pending change refresh after an import cache exists.
- **Sync Selected** for selected pending changes.

Import and sync results must be checked in the target grandMA environment. Console version, network permissions, login settings, OSC/Telnet configuration, and fixture profile files can all affect the actual result.

---

## 2. Access

Open **GrandMALink** from the SuperStage toolbar.

The panel header contains MA2 / MA3 tabs and status text. **Refresh Adapters** rescans the local IPv4 network adapters and updates the Host dropdown.

---

## 3. Connection Settings

### MA2 Tab

The current implementation uses these defaults:

| Item | Current Value |
| --- | --- |
| Host | Selected from local IP options; `127.0.0.1` is also included |
| Port | `30000` |
| User | `administrator` |
| Password | `admin` |

MA2 commands are sent through TCP/Telnet. Before using it, confirm that grandMA2 / MA2 onPC allows the connection and that the firewall is not blocking the port.

### MA3 Tab

The current implementation uses these defaults:

| Item | Current Value |
| --- | --- |
| Host | Selected from local IP options; `127.0.0.1` is also included |
| Port | `8000` |
| OSC Prefix | `gma3` |

MA3 commands are sent through OSC. Before using it, confirm that the grandMA3 / MA3 onPC OSC input settings match these values.

---

## 4. Fixture Model List

After **Refresh Fixtures**, the panel scans SuperStage DMX fixtures in the current level and groups them by fixture model/type:

| Column | Description |
| --- | --- |
| Import | Whether this model participates in Import Patch |
| Model | Fixture model/type name |
| Count | Number of fixtures of this model in the current level |
| GDTF Library | GDTF library path bound to this model. The MA2 XML the MA2 tab needs is converted from it on demand |

Both tabs only need a **GDTF library** bound on the channel library asset: MA3 uses the GDTF directly, and the XML MA2 needs is converted from that GDTF on demand, so hand-maintained MA2 XML is no longer required (from 26H2.6). The MA3 tab requires a GDTF library bound in the fixture library asset. If a required library is unbound, the model is shown as unbound and import stops with a message.

---

## 5. Import Patch

**Import Patch** imports fixtures under the selected models into the target grandMA environment and saves a sync cache between the current level and the console show.

Suggested workflow:

1. Confirm FixtureID, Universe, Start Address, position, and rotation in UE.
2. Confirm that each model has the required GDTF library bound.
3. Open **GrandMALink** and choose the MA2 or MA3 tab.
4. Select the correct Host and click **Connect**.
5. Click **Refresh Fixtures**.
6. Select the models to import.
7. Click **Import Patch**.
8. Check fixture type, patch, groups, position, and orientation in the target grandMA environment.

Import Patch creates or updates the sync cache. Change refresh and sync workflows depend on that cache.

---

## 6. Pending Changes And Sync

After at least one Import Patch, later UE level changes can be synchronized:

1. Click **Refresh Changes**.
2. Review the **Pending Changes** list.
3. Select the changes to sync.
4. Click **Sync Selected**.
5. Spot-check the result in the console.

Every entry in the change list is labelled with its type:

| Label | Meaning | MA2 | MA3 |
| --- | --- | --- | --- |
| **Added** | New in the level, not yet on the console | Yes | Yes |
| **Modified** | Present on both, but patched differently | Yes | Yes |
| **Deleted** | On the console, no longer in the level | Not listed | Yes |

The current MA2 sync workflow handles Added and Modified only. Whether a sync succeeds still depends on the target console show and connection state.

---

## 7. When Something Does Not Go Through

**Status stays Offline**
The console is not reachable, or it is not accepting remote connections. Check the address, and that both machines are on the same network.

**A type is greyed out or refuses to send**
It is almost certainly showing **Unbound** — bind its profile and refresh.

**Pending Changes is empty but the scene has changed**
Press **Refresh Changes**. The list does not update by itself.

**Fixtures arrive with wrong channels**
The profile bound to that type is not the fixture you are actually using. Re-bind it and send again.

---

## 8. Boundaries

- GrandMALink sends console commands; it is not real-time DMX data.
- Run **Import Patch** before the first sync; otherwise there is no sync cache for the current level.
- If the level is unsaved or switched, the sync cache may not match the current scene and should be checked again.
- Import cannot be reliable when a fixture model is missing the required MA2 XML or GDTF library.
- Network connection does not mean the console accepts every command. Always verify the result in the console.

---

> **Return to Overview**: [00 - DMX System Overview](./00_DMX_System_Overview_en.md)
