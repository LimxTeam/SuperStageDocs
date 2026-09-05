# SuperStage DMX System Overview

This document is for users who use SuperStage for lighting previz and DMX troubleshooting. It only describes behavior that is supported by the current implementation. It does not promise fixed frame rate, fixed latency, a fixed fixture count, or guaranteed behavior on every show network.

## 1. What The DMX System Does

The SuperStage DMX system receives, caches, displays, and sends DMX data inside an Unreal Engine project.

Confirmed capabilities include:

| Capability | User-facing behavior |
| --- | --- |
| Art-Net input | Listens on UDP 6454 and parses Art-Net OpDmx packets. |
| sACN E1.31 input | Listens on UDP 5568, parses sACN packets, and can join consecutive sACN multicast universes. |
| DMX output | Sequencer playback or internal sending can output Art-Net or sACN depending on the current protocol. |
| Universe cache | The system stores the latest received or sent universe data for fixtures, monitoring, and playback. |
| Channel lookup | Fixture actors read channel values by Universe and Address, from 1 to 512. |
| Activity Monitor | The editor can show cached universe channel values. |
| Sequencer / Take Recorder | DMX data can be recorded into Sequencer and injected / output again during playback. |

## 2. Basic Concepts

| Term | Meaning |
| --- | --- |
| Universe | A set of DMX channels. In SuperStage user-facing fields, universes start at 1. |
| Channel | A value in a universe, from 0 to 255. |
| Start Address | The first channel read by a fixture, from 1 to 512. |
| Fixture ID | A fixture number used for organization and external console data. |
| Fixture Library | The channel map that defines which channels drive each fixture function. |

SuperStage has 8-bit, 16-bit, and 24-bit channel read paths. Whether a fixture uses those paths depends on how its Fixture Library defines coarse, fine, and ultra channels for each attribute.

## 3. Network Configuration

The DMX configuration panel in the SuperStage toolbar exposes:

| Setting | Description |
| --- | --- |
| Protocol | `Art-Net` or `sACN (E1.31)`. |
| Input Enable | Starts or stops the input receiver. |
| Input Local IP | Local adapter used for input. `0.0.0.0` listens on all available addresses. |
| Input Start Universe | Input universe offset used to align external universe numbering with SuperStage. |
| Output Enable | Allows DMX output to external devices. |
| Output Local IP | Local adapter used for output. |
| Output Start Universe | Output universe offset. |

Ports are fixed by protocol:

| Protocol | Default port | Notes |
| --- | --- | --- |
| Art-Net | UDP 6454 | Default remote target is broadcast. |
| sACN E1.31 | UDP 5568 | If no fixed remote target is configured, output uses `239.255.x.y` multicast addresses calculated from the universe. |

## 4. Universe Alignment

SuperStage shows internal universes as `Universe 1`, `Universe 2`, and so on. External network universe numbering can differ, so the panel provides Start Universe fields.

For input:

```text
SuperStage internal universe = network universe - Input Start Universe + 1
```

For output:

```text
Network universe = SuperStage internal universe + Output Start Universe - 1
```

Common settings:

| External numbering style | Suggested Start Universe |
| --- | --- |
| Art-Net network universe starts at 0 | 0 |
| Console or software UI starts at Universe 1 | 1 |

If channel values arrive but fixtures do not respond, check Start Universe alignment first.

## 5. Common Workflows

### Receive Console Data

1. Choose the protocol in the DMX configuration panel.
2. Select the correct local adapter.
3. Enable Input.
4. Open Activity Monitor and confirm that channel values change in the expected universe.
5. Confirm that each fixture's Universe, Start Address, and Fixture Library match the console patch.

### Output Sequencer Playback

1. Enable Output in the DMX configuration panel.
2. Select the protocol and output adapter.
3. Check Output Start Universe.
4. Play back recorded DMX Sequencer data.

Whether output drives real devices depends on the network, console/node configuration, firewall, and protocol settings.

## 6. Related Documents

| Document | Content |
| --- | --- |
| [01 - DMX Network Configuration](01_DMX_Network_Configuration_en.md) | Configure Art-Net / sACN input and output. |
| [02 - Fixture Library](02_Fixture_Library_en.md) | Create and maintain fixture channel maps. |
| [03 - DMX Actor Base](03_DMX_Actor_Base_en.md) | Fixture Universe, Address, Fixture ID, and Fixture Library reference. |
| [06 - DMX Activity Monitor](06_DMX_Activity_Monitor_en.md) | View universe channel values. |
| [07 - Patch Tools](07_Patch_Tools_en.md) | Batch edit fixture addresses. |
| [08 - DMX Recording And Playback](08_DMX_Recording_Playback_en.md) | Sequencer / Take Recorder workflow. |
| [09 - MA Console Export](09_Export_To_MA_en.md) | Export grandMA2 XML layer and macro files. |
| [11 - GrandMALink](11_GrandMA_Link_en.md) | Connect to grandMA2 / grandMA3 and run patch import or sync. |

## 7. Troubleshooting Order

If virtual fixtures do not respond:

1. Confirm that Input is enabled in the DMX configuration panel.
2. Confirm that the correct adapter is selected, or temporarily use `0.0.0.0`.
3. Confirm that Windows Firewall is not blocking UDP 6454 or UDP 5568.
4. Use Activity Monitor to check whether the expected universe has channel values.
5. Check Start Universe offset.
6. Check the fixture actor's Universe and Start Address.
7. Check that the fixture has the correct Fixture Library assigned.
8. Check that the console or sender is actually sending the selected protocol.

For sACN multicast, also confirm that the switch, adapter, and system routing are not joining multicast groups on the wrong network interface.
