# SuperStage DMX Config Panel User Manual

## Purpose

The DMX Config Panel edits SuperStage DMX input and output network settings and applies them immediately to the running DMX subsystem. The bottom of the panel embeds the DMX Activity Monitor so you can inspect Universe channel values currently in the buffer.

## Opening The Panel

Open the panel from the **SuperDMX** entry in the editor.

## Controls

| Control | Description |
| --- | --- |
| Protocol | Selects the protocol. Input and output share the same protocol setting. |
| Input Local IP | Local IP used by input. Choose `0.0.0.0` or one of the network adapter addresses found by the system. |
| Input Enable | Enables or disables DMX input. |
| Input Start Universe | Input start Universe. |
| Output Local IP | Local IP used by output. Choose `0.0.0.0` or one of the network adapter addresses found by the system. |
| Output Enable | Enables or disables DMX output. |
| Output Start Universe | Output start Universe. |

## Protocol Rules

| Protocol | Port | Start Universe Range | Remote Address Rule |
| --- | --- | --- | --- |
| Art-Net | 6454 | 0-32767 | Default broadcast `255.255.255.255`. |
| sACN (E1.31) | 5568 | 1-63999 | No fixed remote address by default. Send uses the Universe multicast address `239.255.{hi}.{lo}`. |

When the protocol changes, the panel resets port and remote address to the protocol defaults and clamps Start Universe to the valid range.

## Local IP

- `0.0.0.0` means any local address.
- Other options come from the system network adapter list.
- Art-Net input binds to the selected local IP.
- sACN input binds to `0.0.0.0` in the subsystem; the selected local IP is used as the multicast interface reference.

## sACN Universes

This value controls how many continuous sACN multicast Universe groups are joined by default for input. The source range is 1-512. The first Universe comes from Input Start Universe.

## The Channel Value Grid

Below the settings is a continuously refreshed grid showing the channel values arriving right now. It is the fastest way to answer "is anything coming in at all?" — confirm the signal before you start suspecting the fixtures.

| Control | Description |
| --- | --- |
| **All Universes** | Lists every universe that has been seen. Untick it and set a **Universe** number to watch just one |
| **Universe** | The universe to watch on its own; requires All Universes to be unticked |
| **Clear** | Forgets what has been received so far — useful for confirming that new data is still flowing rather than looking at a stale picture |

The same grid can also be opened on its own; see [DMX Activity Monitor](15_DMXActivityMonitor_en.md).

## Saving And Applying

Any change to protocol, IP, Enable, or Start Universe is saved and applied to the running DMX subsystem immediately — there is no separate save step and no editor restart. Settings persist with the project and are still in effect the next time it is opened.

## Notes

- Input and output cannot use different protocols from this panel.
- The panel does not provide a manual target IP text field; Art-Net and sACN remote addresses come from protocol defaults.
- Fixture response also depends on scene fixture Universe, Start Address, DMX mode, and the external console output settings.
