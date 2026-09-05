# SuperStage Asset Browser

> Applies to SuperStage 26H2.6 and later (rebuilt in this release)

## Purpose

The asset browser finds a fixture, a prop or an effect by category and puts it into the scene.

**Open**: SuperStage toolbar → **SuperBrowser**. There is a help button in the top-right corner.

---

## 1. Interface

| Area | Description |
| --- | --- |
| Category tree (left) | Narrows what is shown on the right; every row carries its item count |
| Search box | Matches model, manufacturer and fixture type at once |
| Tile view (right) | The placeable assets |
| Slider (bottom right) | Tile size |
| Refresh | Rescan |

Tree structure: **All** sits at the top (that is how you get back to everything after drilling into a category), below it three shortcut rows (All / Favorites / Recent), then the broad categories, and under each the manufacturers that have something in it.

Assets fall into five categories:

| Category | Content |
| --- | --- |
| StageLight | Fixture library, imported from GDTF |
| StageModel | Scenic structures: truss, scaffolding, stage decks, seating stands, barriers, drapes, cable trays, audience |
| SuperVFX | Effect devices: smoke, fire, snow, bubbles, streamers, pyro, fountain, water pool |
| SuperLight | Seven in-house fixture series |
| SuperStage | Stage devices: screens, projectors, lasers, LED tape, broadcast cameras, lift and track machinery, Madrix, white-model object |

---

## 2. Search

The search box matches **name, manufacturer and fixture type at once**, with space-separated terms — all words have to match, in any order. So `robe beam` finds Robe's beam fixtures.

Search **works together with the tree**: pick a manufacturer first and search inside it.

---

## 3. Favourites and Recent

A show usually comes down to a handful of fixtures placed over and over. Two rows at the top of the tree exist for exactly that.

**Favourites** is the list you curate:

- Right-click a tile → **Add to favourites**; a star appears on its corner so you can spot it anywhere in the browser;
- Once it is in there, the same menu item reads **Remove from favourites**;
- **Favourites are kept per user**, so yours do not follow the project to anyone else.

**Recent** fills itself in: anything you drag out or place lands at the top, newest first. Nothing to maintain — the fixtures you are actually working with rise to the top on their own.

- Something you tried once and do not want cluttering the list: right-click it → **Remove from recent**;
- To empty the whole list: right-click the **Recent** row in the tree → **Clear recent list**. Favourites are left alone.

---

## 4. Placing Into the Scene

| Operation | Description |
| --- | --- |
| **Drag a tile into the viewport** | Drop it where you want it. The normal way to place something |
| **Double-click a tile** | Drops it straight into the level without aiming — useful when the position does not matter yet, or when you are about to move it with the array tools anyway |

> Fixtures arrive **unpatched**. Give one a universe, an address and a number in the Patch tool before expecting it to respond to a console.

---

## 5. Swapping Fixtures Already in the Scene

Select one or more items in the level, then **right-click a tile** here and choose **Replace selected actor with this asset**.

- The menu tells you how many actors are selected, so you can see the scale of what you are about to do before committing;
- Each selected item is swapped for the one you picked, **keeping its position, orientation and DMX mode**;
- This is how you change a whole rig from one fixture to another — for instance when the venue supplies a different model than the one the plot was drawn with — without placing anything again.

> **The patch does not carry over.** Re-address anything you swap.

---

## 6. Other Right-Click Actions

| Action | Description |
| --- | --- |
| Place | Put into the level |
| Replace | See section 5 |
| Add / remove favourite | See section 3 |
| Locate in Content Browser | Jump to the asset |
| Open fixture definition | Go straight to the fixture editor |

---

## 7. Tile Information

Each tile shows a thumbnail and a name. **Hovering** shows the full model, manufacturer, type and how many DMX modes it has.

---

## 8. When Something Is Missing

| Symptom | What to do |
| --- | --- |
| An item you just added is not listed | The list normally updates on its own; press **Refresh** if it has not |
| You cannot find a fixture you know exists | Click **All** first — a category or manufacturer may still be selected in the tree, and search only looks inside the current scope |
| Items appear under **Default** | That category is where anything without a category set ends up. Set one so the item is findable later |
| Thumbnails are blank at first | Previews are built in the background the first time the panel opens; they fill in as they finish |

---

## 9. Notes

- Dragging a fixture into the scene does **not** snap the page back to All or collapse the sidebar groups. Clicking empty space in the sidebar does return to All, which is intended behaviour;
- The list keeps itself up to date as assets are imported and deleted; Refresh is there for the rare case where something has changed outside the editor.

---

## 10. Related Documents

- [Fixture System Overview](../fixture/00_FixtureSystem_Overview_en.md)
- [GDTF Import](../fixture/02_GdtfImport_en.md)
- [Patch Tools](../stagecore/07_Patch_Tools_en.md)
- [Fixture Array Tool](11_FixtureArrayTool_en.md)
