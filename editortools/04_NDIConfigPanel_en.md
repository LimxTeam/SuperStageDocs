# SuperStage NDI Inputs

> Applies to SuperStage 26H2.6 and later ｜ Module: SuperNdi

## Purpose

Give each video feed a name the scene can rely on, then point that name at whichever NDI source is on the network today.

**Open**: the NDI configuration panel from the SuperStage toolbar. There is a help button in the top-right corner.

---

## 1. Why There Are Two Names

Screens and surfaces in the scene refer to an **input name** you choose — something like `MainScreen` or `Camera1`. Which actual NDI source that name points at is set here, in one place.

That indirection is the whole point: the machine sending the feed will have a different name at every venue, and rebuilding the scene each time is not workable. Repoint the name here and everything using it follows.

```text
Screen in the scene  ──refers to──>  input name "MainScreen"  ──maps to──>  NDI source on the network
(never changes)                      (changed in this panel)                (different at every venue)
```

---

## 2. Setting Up an Input

1. Type a name into the box at the top and click **Add**. Pick something that describes the **role**, not the machine;
2. Click **Refresh Sources** to look for what is broadcasting on the network. The count on the right tells you how many were found;
3. In the **Configured Inputs** list, open the drop-down next to your input and pick a source.

Changes **take effect at once and are remembered** for next time.

**Remove** deletes an input; anything in the scene still pointing at that name will have nothing to show.

---

## 3. About the Source List

**The list is a snapshot, not a live view.** A source that was switched on after your last refresh will not be there until you click **Refresh Sources** again.

Discovery runs in the background so the editor keeps responding; a progress indicator is shown while it scans.

Choosing **&lt;None&gt;** unhooks an input without deleting it — useful when a feed is temporarily offline and you do not want to lose the setup.

---

## 4. When a Feed Does Not Appear

| Symptom | What to do |
| --- | --- |
| **No sources found** | NDI only finds machines on the same network. Check that the sending machine is on the same subnet and that a firewall is not blocking it, then refresh again |
| **The source is listed but nothing shows in the scene** | Confirm the input name is actually the one the screen in your scene is set to — a typo in either place gives exactly this result |
| **The feed was working and stopped** | The sending machine most likely changed its source name. Refresh and pick it again |

---

## 5. Supported Pixel Formats

| Format | Handling |
| --- | --- |
| BGRA / BGRX | Received directly |
| UYVY / UYVA | Converted automatically |
| Others | Not supported |

---

## 6. Notes

- The NDI runtime ships with the plugin; no separate NDI tools installation is needed;
- NDI depends on LAN discovery and network bandwidth; sources are produced by third-party software whose behaviour is outside this product's control;
- The authorisation scope of NDI capability is subject to the official pricing page and your order.

---

## 7. Related Documents

- [Super Screen](../stageassets/06_SuperScreen_en.md)
- [Super Projector](../stageassets/05_SuperProjector_en.md)
- [DMX Recording and Playback](../stagecore/08_DMX_Recording_Playback_en.md) (NDI recording uses the same Sequencer flow)
