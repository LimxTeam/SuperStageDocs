# SuperStage Main Menu and Toolbar

> Applies to SuperStage 26H2.6 and later

This document describes the user entry points the SuperStage main plugin registers in the Unreal Editor. Menu content reflects the current implementation.

## 1. Opening

With SuperStage enabled, a **SuperStage** button appears in the Unreal Editor's top toolbar. Clicking it opens the SuperStage dropdown menu.

## 2. Top-Level Entries

| Menu item | Function |
| --- | --- |
| PluginAuth | Opens the plugin authentication window for sign-in, license status and offline activation. |
| OfflineActivation | Offline activation entry. |
| SuperBrowser | Opens the SuperStage asset browser for browsing and placing fixtures and stage assets. |
| AssetDemo | Asset sample list. |

## 3. SuperDMXTool Submenu

| Menu item | Function | Document |
| --- | --- | --- |
| PatchTool | Opens the DMX batch patch tool. | [07 - Patch Tools](../stagecore/07_Patch_Tools_en.md) |
| DMXToMa | Opens the offline DMX-to-grandMA export tool. | [07 DMXExportMA](07_DMXExportMA_en.md) |
| GrandMALink | Opens the grandMA2 / grandMA3 patch connection panel. | [11 GrandMALink](../stagecore/11_GrandMA_Link_en.md) |
| MVR | Opens the MVR import and export panel. | [08 MVR Import / Export](08_MVRImport_en.md) |
| **GDTF Batch Import** | Imports a batch of GDTF packages and builds fixture definitions in bulk. **New in 26H2.6** | [GDTF Import](../fixture/02_GdtfImport_en.md) |

## 4. Documentation Submenu

| Menu item | Function |
| --- | --- |
| QuickStart | Opens the SuperStage quick start page. |
| ProductDocs | Opens the SuperStage product documentation page. |
| SuperLaser | Opens the laser documentation. |
| ModuleManuals | Opens the module manuals submenu. |
| SystemReference | Opens reference links for editor tools, light components, stage assets and the DMX core system (EditorTools / LightComponents / StageAssets / DMXCoreSystem). |
| DevDocs | Opens the developer / API documentation page. Normal users do not need it. |
| Changelog | Opens the changelog page. |

## 5. Other Entries

| Menu item | Function |
| --- | --- |
| Social Media | Tutorial and social submenu: Bilibili, Douyin, YouTube, Instagram, Facebook. |
| Website | Opens the official website. |
| ContactUs | Opens the contact page. |
| UserAgreement | Opens the user agreement page. |

## 6. Status Bar

SuperStage registers these buttons in the editor's bottom status bar:

| Button | Function |
| --- | --- |
| SuperStage: version | Shows the current plugin version; click to open the website. |
| SuperDMX | Opens the SuperDMX configuration panel (which contains the DMX Activity Monitor). |

> The DMX configuration panel's entry is in the **status bar at the bottom**, not in the SuperStage dropdown menu.

## 7. UE Tools Menu

SuperStage registers these tools in the Unreal Editor's top **Tools** menu:

| Menu item | Function |
| --- | --- |
| GOBO Atlas Builder | Extracts gobo textures from channel library attributes and builds a gobo atlas. |
| Color Atlas Builder | Extracts colours from channel library attributes and builds a colour atlas texture. |

## 8. Panel Help Buttons

**New in 26H2.6**: 14 tool panels have a help button in the top-right corner that opens the instructions for that panel. The help text is included in the localisation dictionary.

Panels with help buttons: asset browser, patch tool, DMX configuration, NDI configuration, fixture editor, channel library editor, prism preset editor, GDTF batch import, MVR, DMXToMa, GrandMALink, gobo atlas builder, colour atlas builder, VAT character generator.

## 9. Tips

- If the toolbar does not appear, confirm the SuperStage plugin is enabled and restart the editor.
- If a menu item does nothing, check the Output Log for module loading or authorisation errors.
