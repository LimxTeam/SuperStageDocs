# Color Atlas Builder

To show a colour wheel on stage a fixture does not use the list of slot colours — it uses a single **texture** with those colours in it. This tool reads the list and writes that texture.

You only need it when a fixture's colour wheel was set up by hand, or when you have changed the slot colours and want the fixture to pick up the change.

The panel has a help button in its top-right corner; its content matches this document.

---

## 1. Opening the Tool

SuperStage toolbar / tools menu → **Color Atlas Builder**.

---

## 2. Building an Atlas

1. Pick the **Fixture Library** of the fixture you are working on;
2. Type the **Attribute Name** of the wheel, **exactly as it is spelled in that library** — spelling and capitalisation have to match. For example `ColorWheel1`;
3. Click **Search**. The status line reports how many colours were found, and the swatches appear below in slot order;
4. **Check the swatches against the real fixture**, then click **Generate Atlas**.

The finished texture is saved next to the fixture library, named after the library and the attribute:

```
Library SL_Acme_XP-380Beamll  +  attribute ColorWheel1
        ↓
Texture CLA_Acme_XP-380Beamll_ColorWheel1
```

Regenerating replaces the asset of the same name.

---

## 3. Reading the Preview

The number under each swatch is that colour's **slot number, counting from zero**. It is the same order the console steps through the wheel — so **a colour that looks out of place here will be out of place on stage**.

Only the **first group** of colours on the attribute is read. If a wheel is split into several groups in the library, the atlas is built from the first one.

---

## 4. When Something Looks Wrong

**"Attribute was not found or has no color definitions"**
Either the name is spelled differently in this library, or the wheel's slots have no colours set. Open the fixture library and check the attribute.

**The swatches are all black or all white**
The slots exist but their colours were never filled in. Set them in the fixture library first, then search again.

**Generate Atlas is greyed out**
Run **Search** first — there is nothing to build until colours have been found.

**The fixture still shows the old colours**
The atlas is a new asset: **save it**, and make sure the fixture is pointed at it.

---

## 5. Boundaries

- The tool does not scan for colour attributes automatically; the attribute name has to be typed in;
- The atlas is a helper texture for the material. It is not a promise of matching a real fixture's colour wheel; what you see also depends on the library data, the material, and render settings.

---

## 6. Related Documents

- [Channel Library Editor](10_FixtureLibraryEditor_en.md)
- [Gobo Atlas Builder](14_GoboAtlasBuilder_en.md)
- [The Fixture Definition Asset](../fixture/01_FixtureDefinition_en.md)
