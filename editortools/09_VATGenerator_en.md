# VAT Generator

> Applies to SuperStage 26H2.6 and later

## Purpose

Bake character animation into textures for large crowds.

There is a help button in the top-right corner of the panel.

---

## 1. What It Produces

It turns an animated character into something a stage scene can show in **very large numbers**. Instead of playing the animation joint by joint, the movement is baked into textures and replayed by the material, so a crowd costs a fraction of what the same crowd would cost as ordinary animated characters.

Each item you generate produces three things:

| Output | Description |
| --- | --- |
| **VAT mesh** | The static mesh for the character |
| **Animation textures** | A pair of textures (position + normal) for that one clip |
| **Material instance** | Ready to use, with the textures already plugged in |

Drop the VAT mesh into the level, assign the material instance, and it plays. Generated characters can be added to the [Super Crowd](../stageassets/12_SuperCrowd_en.md) character list.

---

## 2. Building the List

| Button | Effect |
| --- | --- |
| **Add Selected** | Takes whatever animation sequences are selected in the Content Browser and adds one row each. The character mesh is filled in **automatically** from the animation's own preview mesh, so most of the time you only need to check it |
| **Add Item** | Gives you an empty row to fill in by hand |
| **Clear** | Empties the list |
| **Remove** | Drops just that one row |

Every row needs both an **animation** and a **character mesh**, and the two must belong to the **same skeleton**. If they do not, the row says so in place of its destination folder and is skipped when you generate.

---

## 3. Generating

**Generate VAT** first asks where the results should go. Pick a folder and everything is filed underneath it automatically: **one folder per character, and inside it one folder per clip**. Once you have chosen a folder, each row shows the exact destination it will write to, so you can check the layout before committing.

A progress dialog tracks the whole batch and can be **cancelled** at any point. Whatever finished before you cancelled is kept; the summary line at the bottom tells you how many were created, skipped and failed.

> Long clips and dense meshes take the longest, because the work is proportional to **frames multiplied by vertices**. The progress bar is measured in frames, so it moves at an honest pace rather than jumping per item.

---

## 4. Re-running and Reusing

Generating is **safe to repeat**. Anything that already exists at the destination is left alone and reported as "already exists" rather than being rebuilt, so you can add a few new clips to an existing list and run it again without waiting for the old ones.

**Several clips of the same character share a single VAT mesh** — it is built once and reused for every clip in the batch. To force a rebuild, delete the existing assets in the Content Browser first.

**A row that fails is shown in red with the reason on it.** The two common ones are a mismatched skeleton and a name that cannot be used as an asset name; fix the row and run again, and the rows that already succeeded will simply be skipped.

---

## 5. Notes

- A generated VAT character is a **static mesh with material-driven animation**. It does not take part in the skeletal animation system and cannot switch clips at runtime;
- Each clip of a character costs its own pair of textures, so more clips means more memory;
- 13 VAT characters already ship with the plugin (see [Super Crowd](../stageassets/12_SuperCrowd_en.md)); this tool is for adding your own.

---

## 6. Related Documents

- [Super Crowd](../stageassets/12_SuperCrowd_en.md)
- [Stage Assets Overview](../stageassets/00_StageAssets_Overview_en.md)
