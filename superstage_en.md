# SuperStage Product Documentation

## Overview

SuperStage is a next-generation stage lighting previz and virtual production system based on Unreal Engine 5. It integrates lighting console programming, laser control, NDI video streaming, drone formation, and construction drawing in a unified real-time 3D environment, achieving "one engine, full pipeline coverage."

## System Requirements

### Hardware Requirements

| Item | Minimum | Recommended |
|------|---------|-------------|
| CPU | Intel i7-10700 / AMD Ryzen 7 3700X | Intel i9-13900K / AMD Ryzen 9 7950X |
| GPU | NVIDIA RTX 3060 (12GB) | NVIDIA RTX 4080 (16GB) |
| RAM | 32 GB DDR4 | 64 GB DDR5 |
| Storage | 256 GB SSD | 1 TB NVMe SSD |
| OS | Windows 10 21H2 (64-bit) | Windows 11 23H2 (64-bit) |

### Software Dependencies

- Unreal Engine 5.6+
- Visual Studio 2022 (C++ Desktop Development Workload)
- .NET 6.0 Runtime

## Quick Start

### 1. Install Plugin

Extract the SuperStage plugin to your project's `Plugins/` directory:

```
YourProject/
├── Plugins/
│   └── SuperStage/
│       ├── SuperStage.uplugin
│       ├── Source/
│       └── Content/
```

### 2. Enable Plugin

In the UE5 Editor: **Edit → Plugins → Search "SuperStage" → Check to enable → Restart Editor**.

### 3. Create Your First Lighting Scene

1. Open the SuperBrowser asset browser (`Window → SuperStage → SuperBrowser`)
2. Drag fixtures from the fixture library into the scene
3. Open the SuperConsolePro console (`Window → SuperStage → Console`)
4. Select fixtures and adjust parameters

## Core Modules

### SuperConsolePro

Built-in professional lighting console supporting CUE programming, Preset presets, Effect engine, Timeline, and Timecode synchronization.

**Core Features:**

- **CUE System**: Supports Fade / Delay / Snap / Override
- **Preset System**: Eight preset categories — Dimmer, Position, Gobo, Color, Beam, Focus, Control, Shapers
- **Effect Engine**: Sine, Sawtooth, Random, Stepped, and other waveforms
- **Timeline**: Multi-CUE, multi-track visual orchestration
- **Timecode**: SMPTE LTC / Art-Net Timecode / Internal

### SuperLaser

Beyond laser control system that receives Pangolin Beyond software point cloud data via UDP multicast for real-time laser point cloud rendering within UE5.

**Core Features:**

- 71.4% ILDA point cloud compression ratio
- Zone → Actor automatic mapping
- Laser beams illuminating smoke particles

### SuperNdi

NDI video stream integration system enabling real-time video embedding in 3D scenes.

**Core Features:**

- NDI discovery and receiving
- Real-time video texture updates
- Alpha channel support

### SuperDroneLink

Drone formation module that receives LimxDroneStudio formation data and visualizes it in real time within UE5.

**Core Features:**

- UDP Limx Binary Protocol receiving
- 3D formation real-time rendering
- LED light synchronization

### SuperShader

Professional stage lighting shader system providing beam materials, strobe algorithms, prism rendering, and other optical effects.

**Core Features:**

- Beam materials: light column, fog, Tyndall effect
- Strobe algorithm: 8 modes (Closed/Open/Linear/Pulse/RampUp/RampDown/Sine/Random)
- Prism preset system: `USuperPrismPreset` data asset + 2D canvas visual editor
- Emissive materials: LED screen, light strip effects
- Physical lighting: deep integration with UE Lumen

### SuperDMX

DMX communication engine module providing Art-Net / sACN (E1.31) dual-protocol sending, receiving, caching, and query services.

**Core Features:**

- Dual protocol support: Art-Net 4 + sACN/E1.31
- 100+ Universe @ 60fps throughput
- Independent send/receive threads + thread-safe buffer
- Sequencer DMX recording and playback tracks
- sACN auto multicast group management

### SuperCAD

Professional construction drawing system that directly generates CAD-level lighting plots and elevation views within the UE5 Editor.

**Core Features:**

- 3D→2D real-time linkage
- Orthographic silhouette rendering
- DXF / SVG / PDF export
- Smart annotation system

## Network Ports

| Protocol | Port | Direction | Description |
|----------|------|-----------|-------------|
| Art-Net | UDP 6454 | Bidirectional | Lighting control signals |
| sACN | UDP 5568 | Bidirectional | Streaming ACN |
| SuperData | TCP 5966 | Bidirectional | Cross-platform data sync |
| Beyond | UDP 5568 | Receive | Laser point cloud |
| NDI | Dynamic | Receive | Video stream |
| OSC | UDP configurable | Bidirectional | Open Sound Control |
| LimxDroneStudio | UDP 14555 | Receive | Drone formation |

## Design Philosophy

### Configuration over Customization

No hardcoded code for any single manufacturer. All hardware features are generalized and configurable.

### Graceful Degradation

Individual fixture offline does not affect the global state. Art-Net interruption maintains the last valid state.

### Single Source of Truth

All state determination is based on SuperStage. Hardware-side states are merely "reflections."

### Zero Trust for External Systems

All external inputs undergo validation and fault-tolerant processing.

## Changelog

Please refer to the [Update History](/about) page for version change details.

## Contact Support

- **Email**: yerrkj@outlook.com
- **Bilibili**: SuperStage Official Channel
- **WeChat Group**: Add customer service WeChat for an invitation
