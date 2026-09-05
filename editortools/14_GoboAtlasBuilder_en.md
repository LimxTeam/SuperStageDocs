# Gobo Atlas Builder

To project a gobo wheel a fixture does not use the individual gobo image files — it uses a single **strip** with the images side by side. This tool builds that strip.

You need it when a gobo wheel was set up by hand, or when you have swapped an image and want the fixture to pick up the change.

The panel has a help button in its top-right corner; its content matches this document.

---

## 1. Opening the Tool

SuperStage toolbar / tools menu → **GOBO Atlas Builder**.

---

## 2. Building an Atlas

1. Pick the **Fixture Library** of the fixture you are working on;
2. Type the **Attribute Name** of the wheel, **exactly as it is spelled in that library** — spelling and capitalisation have to match. For example `Gobo1`;
3. Click **Search**. The status line reports how many images were found, and each one is listed below by slot;
4. Click **Generate Atlas**.

The finished texture is saved next to the fixture library, named after the library and the attribute so you can tell at a glance which wheel it came from:

```
Library SL_Acme_XP-380Beamll  +  attribute Gobo1
        ↓
Texture LTA_Acme_XP-380Beamll_Gobo1
```

Regenerating replaces the asset of the same name.

---

## 3. Reading the List

- The number in brackets before the name is the **slot number, counting from zero** — the same order the console steps through the wheel;
- The size in brackets after the name is the source image's **own resolution**.

The strip is a fixed width **shared evenly** between the slots, so a wheel with few gobos gives each one more pixels than a wheel with many. Source images are stretched to fit their share; **they do not all need to be the same size** to begin with.

Only the **first group** of images on the attribute is read. If a wheel is split into several groups in the library, the atlas is built from the first one.

---

## 4. When Something Looks Wrong

**"Attribute was not found or has no gobo textures"**
Either the name is spelled differently in this library, or the wheel's slots have no images assigned. Open the fixture library and check the attribute.

**A slot is missing from the list**
Slots without an image are skipped, so the open slot of a wheel will not appear. Only slots that actually hold an image are built into the strip.

**Generate Atlas is greyed out**
Run **Search** first — there is nothing to build until images have been found.

**A gobo looks stretched**
The slot's share of the strip is a fixed shape; a source image with a very different shape will be squeezed into it. Prepare gobo images **square** if you want them projected round.

---

## 5. Boundaries

- The tool does not scan for gobo attributes automatically; the attribute name has to be typed in;
- The atlas is for fixture previsualisation. It is not a promise of matching a real gobo, lens, focus, or on-site haze.

---

## 6. Related Documents

- [Channel Library Editor](10_FixtureLibraryEditor_en.md)
- [Color Atlas Builder](13_ColorAtlasBuilder_en.md)
- [The Fixture Definition Asset](../fixture/01_FixtureDefinition_en.md)
