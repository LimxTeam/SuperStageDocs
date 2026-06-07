# SuperStage Product Documentation

| Item | Content |
| --- | --- |
| Version | 26H2.0 |
| Release Date | May 21, 2026 |
| Document Revision Date | June 8, 2026 |
| Copyright | Foshan Yierranran Technology Co., Ltd. (LimxTeam) |

---

## Document Notes

This document introduces the product positioning, main capabilities, typical application scenarios, and delivery boundaries of SuperStage 26H2.0, helping clients evaluate whether the product suits their current projects.

The content primarily focuses on product capabilities and project value, and does not include specific operational steps, development interface descriptions, or on-site system configuration details.

Specific available modules, authorization scope, service content, and commercial terms are subject to the contract, order, authorization backend, or mutually confirmed delivery checklist.

---

## 1. Product Positioning

SuperStage is a stage lighting and performance visualization toolset designed for Unreal Engine. It is used for lighting previz, DMX integration, media content preview, special effects visualization, and project drawing and statistical data compilation within UE scenes.

In real projects, lighting, screens, projections, light strips, lasers, drone visualization, stage structures, and drawing documentation are often scattered across different teams, different software, and different files. What SuperStage does is consolidate this content into a single UE scene, allowing the team to review, align, and identify some issues before entering the venue.

It is suitable for these types of projects:

- Visual previz for concerts, music festivals, theaters, product launches, exhibitions, and large-scale events.
- Projects needing simultaneous presentation of lighting, screens, projections, light strips, lasers, and stage structures within UE.
- Projects requiring DMX integration, fixture status preview, and data recording via Art-Net or sACN.
- Projects needing collaboration around common performance production workflows such as MVR, GDTF, grandMA, NDI, and MADRIX.
- Projects requiring output of lighting plots, construction communication drawings, statistical data, and project archival files.

SuperStage does not replace real consoles, media servers, laser safety systems, drone flight control systems, rigging safety procedures, or on-site regulatory reviews. When it comes to actual performance execution, on-site hardware systems, professional engineering personnel, and local regulations remain the final authority.

### Team Value

| Role | What They Typically Worry About | How SuperStage Helps |
| --- | --- | --- |
| Production Company / Project Lead | The client can't understand the proposal; issues only surface after entering the venue. | Use UE scenes to review lighting, screens, stage design, and special effects together, reducing communication based on imagination. |
| Lighting Designer | Whether fixture positions, angles, beams, colors, and gobos truly meet design intent. | Check relationships between fixtures and stage design, screens, projections, and camera angles during the previz phase. |
| Console Engineer | Whether Fixture ID, Universe, Address, and fixture library data can be easily handed over. | Reduce redundant data entry and manual verification through Patch Tool, MVR / GDTF, and grandMA workflows. |
| Video / Media Team | Whether screens, projections, NDI, director feeds, and light strip content can be evaluated alongside lighting. | Place media inputs into the same stage scene and directly assess brightness, rhythm, and spatial relationships. |
| Laser / Drone Collaboration Team | Special effects tend to be reviewed separately from the ground stage. | Incorporate laser data and DroneLink / LDLink data into the overall previz view. |
| Delivery & Engineering Team | Drawings, statistics, and review materials still need to be recompiled after previz. | Let fixtures, Universe assignments, cables, and drawing elements from the scene continue to serve the delivery phase. |

---

## 2. What Problems Does SuperStage Solve

### 2.1 Proposals No Longer Rely Solely on Verbal Explanation

The hardest part of early-stage stage projects is often not the technology, but communication. Clients see renderings, lighting designers see fixture plots, video teams see content assets, and engineering teams see structures. Everyone is describing the same stage, but not necessarily seeing the same stage.

SuperStage places all this content into a single UE scene. Lighting coverage, screen content proportions, projection occlusion, light strip rhythm, and visual centerpiece relationships can all be discussed directly within the view.

This is especially valuable during pre-sales and proposal phases: clients more easily understand "what they will ultimately see," and the team more easily explains why fixtures are arranged this way, screens split this way, and visual segments arranged this way.

### 2.2 Patching Risks Exposed Earlier

In large projects, address overlaps, duplicate Fixture IDs, chaotic Universe planning, and unclear fixture naming all consume on-site debugging time. The venue is not a place to slowly organize spreadsheets — the later issues are discovered, the higher the cost of coordination and correction.

SuperStage moves these issues as far forward as possible to the previz phase through DMX, Patch Tool, 512-channel grid, conflict prompts, and activity monitoring. After fixtures are added to the scene, they are not just "a light sitting there" — you can also proceed to address planning, console handoff, and drawing statistics.

26H2.0 supports up to 512 DMX Universes for planning and integration, making it more suitable for matrix lights, pixel light strips, large light arrays, and multi-console integration projects. Actual project performance still depends on UE project scale, hardware configuration, network environment, and scene complexity.

### 2.3 Reduce Redundant Data Entry

One common occurrence in performance projects: the same batch of fixture data gets entered once in UE, once in design software, once in the console, and then once more for delivery documentation. The more times it happens, the more errors creep in.

SuperStage organizes fixture positions, orientations, Fixture IDs, Universes, Addresses, and fixture library resources from the UE scene through MVR, GDTF, and grandMA-related workflows, allowing previz data to continue into external collaboration workflows.

This is not about replacing all software, but about ensuring data that has already been organized is lost one fewer time, copied one fewer time, and errored one fewer time.

### 2.4 View Media, Light Strips, and Lighting Together

Many stage projects are no longer just about lighting previz. Screen content, director feeds, projection mapping, NDI inputs, MADRIX light strips, and matrix light walls all affect the final picture. Content that shows no obvious issues when checked individually may still need checking for brightness, rhythm, layering, and visual centerpiece relationships once combined.

SuperStage supports NDI, project textures, director camera feeds, and MADRIX main output preview. Screens, projections, light strips, and lighting can be evaluated in the same scene, allowing directors, lighting designers, video teams, and production teams to communicate around the same picture.

### 2.5 Dynamic Content Can Be Reviewed

Lighting Cues, NDI video, MADRIX light strip animations, laser data, and DroneLink data are all dynamic content. Viewing them only once through a real-time system is not ideal for client review or internal retrospective analysis.

SuperStage supports multiple types of data entering recording and playback workflows. Recorded content can be decoupled from some real-time input environments for proposal demonstrations, version comparison, internal review, and delivery explanation.

### 2.6 Extending from Previs to Delivery Materials

If a UE previz scene is only used for visual preview, its value is limited. SuperStage continues to use the scene's fixtures, Universes, cables, drawing elements, and statistical information for delivery material compilation.

SuperCAD supports DXF, PDF, and PNG output, and also provides statistical views including fixture lists, Universe assignments, power load, and hanging point load. Formal construction documents must still be reviewed by the engineering team, but previz data can serve as the foundation for drawing communication and archival materials.

---

## 3. 26H2.0 Release Highlights

The focus of 26H2.0 is not a single feature point, but the reinforcement of several practical workflows: large DMX projects, external data exchange, grandMA patching, media and light strip integration, beam performance, patching efficiency, and fixture resource coverage.

| Update Direction | 26H2.0 Capability | Project Significance |
| --- | --- | --- |
| MVR Export | Export selected fixtures' positions, orientations, DMX patch info, and associated GDTF fixture profiles as MVR files. | UE previz scenes more easily enter external design, previz, and console collaboration workflows. |
| 512 Universes | Art-Net and sACN workflows support up to 512 DMX Universes. | Large light arrays, matrix light walls, pixel light strips, and multi-console integration don't need to be prematurely split in planning. |
| grandMA Patching | Patch data compilation, import, and offline delivery workflows for grandMA2 / grandMA3. | Fixtures and addresses organized during previz can more smoothly enter the console preparation phase. |
| MADRIX Light Strips | Connect to local MADRIX main output preview, with support for recorded offline playback. | Light strips, matrix light walls, and LED lines can be directly reviewed within the overall stage scene. |
| NDI Configuration Experience | Source list refresh logic optimized to reduce stutter from repeated scanning. | Configuration and source switching feel lighter when there are many NDI sources or complex network environments. |
| Director Input | Screens and projections can use director camera feeds as input sources. | On-stage screens, virtual monitors, multi-camera preview, and director feed distribution become more convenient. |
| Beam & Shapers | Shaper range, surface hotspot, lighting color, and beam quality control updates. | Greater consistency among beam, hotspot, color, GOBO, prism, and shaper effects. |
| Patch Tool | Auto-increment start values, rename options, 512-channel grid, drag-to-change address, conflict prompts. | Multi-round patching and pre-venue checks are faster, and errors are easier to spot. |
| Model Binding | Quickly append selected static meshes to screen or light strip targets. | Reduces repetitive addition when configuring large numbers of screens, light strip carriers, or irregular structures. |
| Fixture Assets | 40 new stage lighting fixture assets added, with matching GDTF / MA2 fixture profile resources. | Resource coverage better suits real project selection and also serves MVR and console patching workflows. |

---

## 4. Product Capabilities

### 4.1 Stage Lighting Previs

SuperStage's foundational capability is stage lighting previz. Teams can place fixtures in UE scenes and observe common stage lighting performance including beams, color, gobos, shapers, prisms, strobe, zoom, frost, and pixel control.

It is best suited for answering these types of questions:

- Whether fixture positions and orientations can cover target areas.
- Whether beams, hotspots, GOBO, prism, and shaper effects are harmonious in the scene.
- Whether lighting and screens, projections, light strips, and stage structures affect the visual centerpiece relationship.
- Whether clients can understand the final visual relationships of the stage during the proposal presentation.

Externally communicable capabilities include:

- Fixture assets usable for UE scene construction, fixture placement, and visual previz.
- Support for common attribute expression including dimmer, color, color temperature, pan, tilt, zoom, strobe, GOBO, prism, shapers, frost, and pixel control.
- Program preview possible through DMX, built-in programming tools, Sequencer, or external consoles.
- Suitable for visual expression of Beam, Spot, Wash, Profile, Panel, Strobe, Pixel Bar, PAR, matrix lights, light strips, and effect-type fixtures.
- 26H2.0 includes updates to shaper range, surface hotspot, lighting color, and beam quality control.

In project delivery, it typically materializes as UE previz scenes, proposal screenshots, demo videos, lighting state playback, and foundational data that subsequent MVR, grandMA, SuperCAD, and other workflows can continue to use.

Boundaries must also be stated clearly: previz visuals are for communication and evaluation and do not replace final on-site verification of real fixtures. Fixture performance is influenced by fixture profiles, DMX patching, materials, rendering settings, and project reference materials.

### 4.2 DMX Networking & Patching

SuperStage supports Art-Net and sACN input and output for integration between external consoles, onPC environments, or other DMX transmission systems and UE scenes.

For large projects, the significance of DMX capability is not simply "can receive data," but rather making fixture addresses, Universes, Fixture IDs, channel occupancy, and recording/playback a reviewable process.

Main capabilities:

- Art-Net and sACN input and output.
- Support for up to 512 DMX Universes in planning and integration.
- Standard DMX 512-channel planning per Universe.
- Fixture Universe, Address, and Fixture ID management.
- DMX activity monitoring, address occupancy preview, and conflict prompts.
- DMX data recording and playback.

These capabilities are suitable for large stages, matrix lights, pixel light strips, multi-console integration, and projects requiring retrospective analysis. Teams can identify issues such as address overlaps, duplicate Fixture IDs, and unclear Universe planning before going on-site, and can also record DMX states for demonstration and verification.

Network stability, console output, switch configuration, and firewall settings remain the responsibility of on-site systems. This document does not promise fixed latency, fixed frame rate, or fixed hardware scale.

### 4.3 Patch Tool

The Patch Tool solves a very practical problem: the more fixtures there are, the easier it is to miscalculate addresses manually; the more project changes there are, the more tedious it becomes to reorganize.

It supports batch setting of Universe, Address, and Fixture ID. Starting Universe, starting Address, and starting Fixture ID can auto-increment based on scene state, suitable for adding fixtures in multiple batches. The 512-channel grid lets you view address occupancy of a Universe, with prompts for address overlap and duplicate Fixture IDs. Fixture start addresses can be adjusted by dragging, and naming can be organized according to project rules.

Its value is reflected in specific execution efficiency:

- After adding a batch of fixtures, no need to recalculate addresses from scratch.
- Whether a Universe is full can be seen directly in the channel grid.
- Conflicts don't need to wait until on-site integration to surface.
- Fixture naming, Fixture IDs, and console handoff materials are easier to align.

The patching data produced by the Patch Tool can continue to serve DMX integration, MVR export, grandMA collaboration, and SuperCAD statistics.

### 4.4 MVR / GDTF Data Exchange

MVR / GDTF capabilities allow SuperStage's UE scene to be more than just a previz image — it can also become part of the project data flow.

SuperStage supports importing fixture information from external MVR files, as well as organizing the spatial positions, orientations, Fixture IDs, Universes, start addresses, and associated GDTF fixture profiles of selected fixtures in the scene into MVR files.

This type of capability suits these scenarios:

- Design, previz, console, and delivery teams need to share the same batch of fixture data.
- The UE scene has already organized fixture positions and patching, and external workflows don't need to re-enter data.
- Related GDTF fixture profiles need to be organized alongside MVR delivery.
- The project needs to migrate fixture positions, types, and address information across different software.

Different external software may have varying levels of MVR / GDTF support and parsing rules. Import results must still be verified in the target software environment before formal delivery.

### 4.5 grandMA Patching Collaboration

26H2.0 updates the patch data compilation, import, and offline delivery workflows for grandMA2 / grandMA3. It addresses the problem of "handing over from UE previz projects to the console preparation phase."

SuperStage can read fixture patch information from UE scenes, organize import data by fixture type, and complete grandMA2 / grandMA3 patch import using project fixture profile resources. When fixtures are subsequently added or changed, updated patch data can continue to be organized. For scenarios that cannot be directly connected or require offline delivery, file-based DMX to MA data export workflows remain available.

In actual projects, it primarily reduces redundant organization on the console side:

- Fixture IDs, Universes, and Addresses already organized in UE don't all need to be rebuilt manually.
- Fixtures of the same type can be organized by type, reducing per-fixture processing workload.
- The previz team and console team can communicate around the same batch of fixture data.
- When large projects move from previz to programming preparation, foundational data is easier to align.

grandMA connection, import, and data organization results depend on the target MA environment, network permissions, fixture profile files, and on-site configuration. Formal projects should complete verification in the target console or onPC environment.

### 4.6 Media Screens, Projections & Director Input

SuperStage supports applying media content to stage screens, projection objects, and virtual monitors. If screens, projections, and lighting are viewed separately, it's easy for each system to look fine individually but feel off when combined.

Currently communicable input directions include:

- NDI video sources.
- Static texture content within the project.
- Director camera feeds within the UE scene.

Screen carriers can display media inputs, projection objects can be used for media casting and mapping preview, and common display controls such as brightness, opacity, color, contrast, and texture deformation are supported. Director input can distribute scene camera feeds to screens or projections, suitable for on-stage screens, virtual monitors, and multi-camera preview.

26H2.0 also adds model quick-binding capability. When large numbers of screens, light strip carriers, or irregular structures need configuration, selected static meshes can be appended to target objects, reducing repetitive manual additions.

### 4.7 NDI Media Input

NDI is suitable for bringing external video, director, or media server outputs into UE stage scenes. SuperStage supports discovering and selecting NDI sources, applying NDI video frames to screen or projection content, and NDI data recording and playback.

26H2.0 optimizes the refresh behavior of the NDI configuration panel. The source list uses caching and manual refresh logic, avoiding repeated scanning during selection, switching, and editing. For projects with many NDI sources or complex network environments, this change makes the configuration process lighter.

Actual NDI performance depends on the third-party NDI runtime environment, sender quality, network stability, and UE rendering load. Recording playback can reduce reliance on live NDI sources during review and demonstration, but cannot replace formal on-site signal chain testing.

### 4.8 MADRIX Light Strip Previs

SuperStage supports connecting to the local MADRIX main output preview and applying the output imagery to light strips, pixel light strips, matrix light walls, or LED line carriers.

The significance of this capability is that MADRIX content does not need to be viewed only in external software. Light strip animations can appear directly in the overall stage previz view, evaluated together with lighting, screens, projections, and stage structures.

Externally communicable capabilities:

- Preview MADRIX output in UE scenes.
- Apply MADRIX output to light strip or matrix carrier materials.
- Support brightness adjustment.
- Support recorded offline playback.

Offline playback is suitable for proposal demonstrations, client review, and internal retrospective analysis. MADRIX project content, output configuration, and on-site control remain the responsibility of the MADRIX software and its related systems.

### 4.9 Laser Visualization

SuperStage provides laser visualization capabilities oriented toward Pangolin Beyond-related workflows. Laser lines, patterns, and recorded content can enter the UE stage view and be evaluated alongside lighting, media, and stage structures.

Externally communicable capabilities include:

- Receive compatible laser point data for visual display within UE.
- Support texture-based and procedural mesh-based laser representation.
- Provide multiple built-in laser patterns for previz and demonstration.
- Support laser data recording and playback.

It is suitable for early discussion of relationships between lasers and audience perspective, screens, stage structures, and ground lighting. Laser capabilities are only for visual previz and do not replace real laser output devices, safety interlocks, on-site audits, or professional laser safety procedures.

### 4.10 DroneLink Drone Data Visualization

SuperStage provides UE-side DroneLink / LDLink data visualization capabilities for presenting drone positions, orientations, and lighting states in stage or event scenes.

DroneLink / LDLink is the currently supported drone data visualization workflow for SuperStage. Specific data sources and connection methods are subject to project delivery specifications.

This capability only describes visualization, not flight control. It is suitable for evaluating aerial formations and ground stages in the same view: when drones appear, whether they are co-directional with screens, whether they conflict with lasers and lighting, and whether the relationship from the client's main viewing angle holds true.

Externally communicable capabilities:

- Access drone position, orientation, and lighting state from LDLink data streams.
- Support conversion of latitude, longitude, and altitude to UE scene coordinates.
- Support visualization of single drones and multi-drone groups.
- Support integration with timeline recording/playback workflows for previz, review, and proposal presentation.

This version only commits to DroneLink / LDLink data visualization as a product capability and does not commit to other drone communication protocol connections. SuperStage is not responsible for real drone route planning, obstacle avoidance, takeoff/landing, flight control safety, or regulatory approvals. Actual flight operations should be managed by professional flight control systems, ground stations, and compliance teams.

### 4.11 Built-in Console & Show Programming

SuperStage provides lighting programming and playback tools for use within the UE Editor, suitable for organizing fixture states during previz, presentation, and review phases.

Externally communicable panels and workflows include fixture sheet, color picker, presets, groups, Playback, running playback, frame editor, frame presets, layout view, Timecode, and DMX settings. The system supports DMX value management from Programmer, Cue, TimecodeCue, Effect, and other sources, and also supports Cue, Playback, shortcut keys, and timeline-related workflows.

This capability is suitable for organizing basic show segments without a complete on-site console environment, for proposal presentations, internal review, or recorded retrospective analysis. It does not claim equivalence to any third-party hardware console. Real performance control authority should be planned according to on-site system design.

### 4.12 Stage Construction Drawings & Drawing Output

SuperCAD is oriented toward drawing output and material compilation for stage projects. It extends fixtures, stage objects, and statistical information from previz scenes into engineering communication materials.

Externally communicable capabilities:

- Drawing views with overlaid stage object display.
- Drawing elements including fixtures, dimensions, annotations, cables, and title blocks.
- Statistical panels for fixture BOM, Universe assignments, power load, and hanging point load.
- Support for DXF, PDF, and PNG export.
- Support for multi-page PDF and batch PNG output.

These outputs are suitable for proposal communication, construction communication, and project archival. PDF and PNG are more visual presentation-oriented; DXF is better suited for further processing in CAD software. Formal construction documents must still be reviewed by the project engineering team according to on-site standards.

---

## 5. Resources & Assets

### 5.1 Fixture Resources

SuperStage's fixture resources are not externally differentiated by internal implementation method. What clients truly care about is: whether common fixture types in projects are covered, whether there are fixture profile resources usable for patching and external collaboration, and whether they can serve previz, MVR, grandMA, and drawing statistics.

The current resource library covers these typical fixture types:

| Fixture Type | Coverage Direction | Project Value |
| --- | --- | --- |
| Beam / Beam Hybrid | Strong beams, aerial beams, prism beam splitting, and narrow-angle beam effects. | Suitable for strong visual impact moments in concerts, music festivals, and large events. |
| Spot / Profile / Framing | Gobo, shaper, imaging, zoom, frost, and stage surface hotspot expression. | Suitable for talent areas, stage sections, shaper boundaries, and GOBO pattern expression. |
| Wash / LED Wash | Color wash, flood light, RGB / RGBW / CMY color mixing, and stage atmosphere rendering. | Suitable for overall atmosphere, background wash, and large-area color change previz. |
| PAR / Fresnel / Fixed Wash | Basic front light, side light, ambient wash, and standard fixture positioning. | Suitable for theaters, product launches, exhibitions, and standard fixture position planning. |
| Linear / Pixel Bar / Tube | Light strips, linear lights, pixel bars, matrix arrays, and stage line effects. | Suitable for LED lines, pixel light strips, matrix effects, and stage contour expression. |
| Panel / Matrix | Panel lights, matrix light walls, pixel blocks, and large-area strobe/wash effects. | Suitable for matrix light walls, strobe arrays, and rhythmic visual segments. |
| Strobe / Blinder | Strobe, audience blinders, burst flash, and strong rhythmic visual effects. | Suitable for music festivals, concert climax segments, and audience interaction atmosphere. |
| Effect / SFX | Lasers, stage effects, and mechanical previz objects. | Suitable for incorporating non-traditional fixture objects into overall stage visual review. |

Brand and fixture profile coverage includes resource categories such as Robe, ClayPaky, ADJ, ETC, Martin, GLP, GTD, GTB, Chauvet, PR Lighting, JB-Lighting, Astera, CKC, Acme, EK, SuperLight, and SuperStage.

| Resource Category | Current Coverage | Notes |
| --- | --- | --- |
| 26H2.0 New Stage Fixture Assets | 40 models | For expanding common stage fixture coverage. |
| Acme Resource Library | 15 fixture projects, 24 GDTF / MA2 profile files/modes | Content library and FixtureLibrary work together to serve previz, patching, and profile collaboration. |
| EK Resource Library | 5 fixture projects, 8 GDTF / MA2 profile files/modes | Content library and FixtureLibrary work together to serve previz, patching, and profile collaboration. |
| GDTF Profile Files/Modes | 94 | For MVR, fixture binding, and external collaboration workflows. |
| MA2 Profile Files/Modes | 94 | For grandMA2 related patching collaboration and offline delivery workflows. |

### 5.2 Stage & Auxiliary Assets

In addition to fixture resources, SuperStage also provides screen and projection objects, light strip and matrix carriers, laser visualization objects, lift and track mechanical objects, as well as stage auxiliary objects such as trusses, scaffolding, stage decks, drapes, and crowd.

These resources are for faster previz scene construction and delivery visuals, and do not replace structural design, mechanical calculations, or construction safety reviews.

---

## 6. Typical Application Scenarios

| Scenario | Pain Point | SuperStage Role in Project | Common Deliverables |
| --- | --- | --- | --- |
| Performance Proposal Previs | Clients struggle to understand final results from drawings and text. | Build stage, fixtures, screens, projections, and stage structures in UE for proposal presentation, director communication, and client confirmation. | Real-time previz views, screenshots, screen recordings, render outputs, and UE scenes. |
| Lighting Show Verification | Addressing errors, fixture orientation, and beam effects often only surface on-site. | Check fixture states ahead of time through DMX, Patch Tool, built-in programming, and visual preview. | Verified patch data, DMX recordings, and show previz visuals. |
| Media & Light Strip Integration | Screens, projections, NDI, director feeds, and MADRIX content are disconnected from lighting. | Bring NDI, director camera feeds, or MADRIX output into UE scenes for overall visual evaluation. | Screen/projection/light strip previz scenes, media recording and playback content. |
| Console & Design Software Collaboration | Fixture positions, addresses, and profiles are repeatedly entered between previz, design, and console. | Use MVR, GDTF, and grandMA-related workflows to let the same batch of fixture data enter external production environments. | MVR files, GDTF resources, grandMA patch preparation data. |
| Laser & Drone Previs | Laser, drone, and ground stage visuals are reviewed separately. | Incorporate laser data and DroneLink / LDLink data into the overall stage view for cross-team communication. | Laser recordings, DroneLink visualization content, and comprehensive previz views. |
| Project Delivery Material Compilation | Lighting plots, statistics, and archival materials still need separate compilation after previz. | Use SuperCAD to output lighting plots, construction drawings, statistical information, and deliverable drawing files. | DXF, PDF, PNG, fixture BOM, Universe statistics, and auxiliary engineering materials. |

---

## 7. External System Collaboration

SuperStage can collaborate with the following types of external systems or formats:

| Type | Collaboration Scope | Product Value |
| --- | --- | --- |
| DMX Network | Art-Net, sACN input/output and integration. | Bring fixture states from external consoles or DMX transmitters into UE previz scenes. |
| Stage Data Exchange | MVR import and export, with GDTF fixture profile resources. | Reduce redundant data entry between design, previz, and console workflows. |
| Console Environment | Patch data compilation, import, and offline delivery for grandMA2 / grandMA3. | Make fixture patch information organized during previz easier to bring into the console preparation phase. |
| Media Input | NDI sources, project textures, and director camera feeds. | Let video, director feed, screen, and projection content participate in overall stage previz. |
| Light Strip Previs | MADRIX main output preview connection and offline playback. | Bring light strip, matrix light wall, and LED line content into UE stage scenes. |
| Laser Previs | Pangolin Beyond-related laser data visualization. | Evaluate laser effects alongside lighting, media, and stage structures. |
| Drone Previs | DroneLink / LDLink data visualization in UE scenes. | Incorporate drone positions, orientations, and lighting states into overall stage previz. |
| Drawing Delivery | DXF, PDF, PNG file output. | Extend previz scenes into project communication, construction, and archival materials. |

External system versions, licenses, network permissions, file compatibility, and on-site configuration can affect final use results. Verification should be completed in the target environment before formal delivery.

---

## 8. Licensing & Delivery Boundaries

SuperStage delivers different capabilities based on authorization scope. Specific available modules, license duration, machine binding method, offline authorization, commercial services, and technical support scope are subject to the contract, order, authorization backend, or mutually confirmed delivery checklist.

This document does not list prices, packages, discounts, gift periods, enterprise service commitments, or undisclosed commercial policies.

The following are not within SuperStage's product capability commitments:

- Functional commitments for real consoles, media servers, lasers, drones, rigging systems, or network devices themselves.
- Commitments regarding third-party software licensing, installation, compatibility, or interface changes.
- Commitments for fixed frame rate, fixed latency, fixed hardware scale, or fixed rendering quality.
- Safety reviews, regulatory compliance, flight approvals, laser safety audits, or structural safety audits for real performance venues.
- Features, customizations, or R&D content not delivered in official releases.

---

## 9. Known Boundaries

- NDI, MADRIX, grandMA, Pangolin Beyond, MVR / GDTF, and other workflows depend on third-party systems or format support.
- DroneLink / LDLink data visualization is not equivalent to a drone flight control system, nor does it include real flight choreography, obstacle avoidance, or takeoff/landing execution.
- Different external software may interpret MVR, GDTF, fixture profiles, and coordinate data differently.
- Rendering performance of large projects depends on Unreal Engine project settings, asset scale, GPU performance, network environment, and on-site configuration.
- Laser and drone-related capabilities are for previz and visual demonstration only, and do not replace real safety systems.
- Drawing output is for project communication and delivery compilation; formal construction must still be reviewed by the engineering team according to project standards.

---

## 10. Technical Support

It is recommended to provide the following information when reporting issues or requesting technical support:

- SuperStage version number.
- Unreal Engine version.
- Operating system and graphics card information.
- External systems or file formats in use.
- Issue screenshots, screen recordings, or logs.
- Minimum reproducible project steps.

Official information is based on in-product toolbar links, the official website, authorization backend, and actual delivery instructions.

---

## 11. Suitable and Unsuitable Uses

SuperStage is suitable for projects that require stage lighting previz, DMX integration, media content preview, special effects visualization, and delivery material compilation within Unreal Engine.

It is not suitable as the sole control system for real performance venues, nor does it replace consoles, media servers, laser safety systems, drone flight control systems, structural safety procedures, or regulatory reviews.

---

## 12. Access & Contact

To further evaluate SuperStage, access materials, trial versions, or contact the team through the following channels:

| Need | Channel |
| --- | --- |
| Website | https://yunsio.com |
| Download Trial | https://www.yunsio.com/download |
| Technical Support / Docs | https://yunsio.com/docs/superstage |
| Contact Email | yunsio@yunsio.com |
| Business Inquiries | yunsio@yunsio.com |
| YouTube Tutorials | https://www.youtube.com/channel/UCzr_PcLdN00gxABoTnCSZeA |
| Bilibili Tutorials | https://www.bilibili.com/video/BV1UcdVBsEXb |

---

## Copyright Notice

This document is copyright © 2026 LimxTeam. All rights reserved.
