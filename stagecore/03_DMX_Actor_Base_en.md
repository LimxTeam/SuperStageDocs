# 03 - DMX Fixture Basics

This document explains the basic DMX settings for fixtures in a SuperStage scene. It does not cover C++ APIs, Blueprint macros, or class inheritance. It only covers what users need to configure in the Details panel.

## 1. Applies To

These actor types usually share the same basic DMX settings:

- Moving lights, wash lights, matrix lights, and other fixture actors.
- Lifting machinery and rail machinery.
- LED strip effects and other DMX-driven stage objects.

The functions a fixture can respond to depend on that actor's implementation and the assigned Fixture Library.

## 2. DMX Patch Parameters

Select a fixture actor in the scene and find the DMX-related group in the Details panel.

| Parameter | Description | Common range |
| --- | --- | --- |
| Universe | The SuperStage internal universe read by this fixture. It must match the result of the DMX panel's Start Universe alignment. | Starts at 1 |
| Start Address | The first channel read by the fixture in that universe. | 1 - 512 |
| Fixture ID | Fixture number used for organization and export. | Project-defined |
| Fixture Library | Channel map asset used by this fixture. | Select matching library |
| ControlMode | `DMX` uses external / playback DMX data; `Property` uses Details panel values. | DMX / Property |

In the same universe, fixture channel ranges should not overlap. Use Patch Tool and Activity Monitor to check assignments.

## 3. Universe And Start Address

The absolute channel address the fixture reads (1-based) is:

```text
module base    = Start Address + (module Patch − 1)
absolute addr  = module base + attribute channel offset − 1
```

An ordinary single-module fixture has Patch 1, so the two lines collapse to `Start Address + offset − 1`.

**Every module of a matrix fixture has its own Patch**, so the same attribute name resolves to a different address per module — that is how per-cell control works.

If the console shows Universe 1 but SuperStage receives the data in another universe, check Start Universe in the DMX configuration panel first.

Example:

| Setting | Result |
| --- | --- |
| Universe = 1, Start Address = 101 | The fixture reads from channel 101 in SuperStage Universe 1. |
| Fixture Library span is 20 channels | The fixture usually occupies 101 - 120. |

## 4. Fixture Library

The Fixture Library defines the fixture channel map. Once the correct library is assigned, the fixture can read functions such as Dimmer, Pan, Tilt, Color, and Gobo by attribute name.

If no Fixture Library is assigned:

- The actor may still read raw channel numbers.
- Functions that depend on attribute names usually will not work correctly.
- Patch Tool may not be able to calculate the correct channel span.

Multiple fixtures of the same model and DMX mode can share one Fixture Library. Set different Universe, Start Address, and Fixture ID values for each placed fixture.

## 5. ControlMode

| Mode | Description |
| --- | --- |
| DMX | The fixture reads from the DMX cache. Use this when connected to a console, receiving network DMX, or playing Sequencer data. |
| Property | The fixture uses its own property values and does not read the DMX cache. Use this for static setup or keyframing without a console. |

If the fixture is in `Property` mode, external DMX faders will not drive it. Check this first when data is present but the fixture does not move.

## 6. Channel Span

The channel span comes from the assigned Fixture Library: every Coarse / Fine / Ultra offset **greater than 0**, across every module and attribute, is converted to an absolute address, and the span is **highest address − lowest address + 1**.

**It is a span, not a channel count.** When the channel table leaves gaps, the span is larger than the number of channels actually used — patching against the span is what keeps a fixture from colliding with the next one.

It is used to:

- Let Patch Tool calculate the next fixture start address.
- Check whether address ranges may overlap in one universe.
- Provide channel occupancy information for export or reports.

If channel span looks wrong, check module Patch values and attribute channel offsets in the Fixture Library.

## 7. Troubleshooting

### Activity Monitor Shows Data, But The Fixture Does Not Respond

1. Confirm the fixture ControlMode is `DMX`.
2. Confirm the fixture Universe matches the universe with data in Activity Monitor.
3. Confirm Start Address matches the console patch.
4. Confirm the Fixture Library is assigned and its channel map is correct.
5. Confirm the console is changing channels actually occupied by this fixture.

### Multiple Fixtures Overlap

Use Patch Tool to inspect address usage in the same universe and reassign Start Address values. Fixture ID does not determine DMX channel occupancy.

## 8. Next Steps

- [Fixture Motion](../fixture/04_Motion_en.md)
- [07 - Patch Tools](07_Patch_Tools_en.md)
- [06 - DMX Activity Monitor](06_DMX_Activity_Monitor_en.md)
