# SuperStage Tutorial: UE Engine & Project Configuration Guide

Welcome to the documentation version of the SuperStage video tutorial. In this section, we will detail the two core configurations you must master when creating SuperStage projects with Unreal Engine (UE): **Editor Preferences** and **Project Settings**.

* **Editor Preferences**: Takes effect for the entire UE software, affecting your overall operating experience.
* **Project Settings**: Only takes effect for the currently opened SuperStage project, affecting final rendering and packaging results.

---

## Part 1: Editor Preferences

Select `Edit` -> `Editor Preferences` in the UE top menu bar to open the settings panel.

### 1. General - Performance
This is a key setting to ensure real-time rendering of stage lighting.
* **Disable Background Throttling**: Find `Use Less CPU when in Background` and **uncheck** it.
    * *Why turn this off?* If checked, when your mouse moves outside the UE window, lighting and visuals stop updating and rendering in real time. Disabling it ensures lighting continues to operate in real time even when UE is in the background.
* **Show Frame Rate**: Check `Show Frame Rate and Memory` to monitor the current project's runtime status (FPS and memory usage) in the upper-right corner of the viewport.

### 2. General - Loading & Saving
* **Disable Auto Save**: Scroll down to find the `Auto Save` option and **uncheck** it.
    * *Why turn this off?* UE's auto-save feature can sometimes trigger bugs. If an error occurs during auto-save, you might face a situation where even manual saving cannot be performed. It's recommended to develop the habit of pressing `Ctrl + S` manually.

### 3. Blueprint Editor
If you frequently use Blueprints, these settings can improve your efficiency.
* **Save on Compile**: Find `Save on Compile` and change it to **On Success Only**.
* **Error Navigation**: Check `Jump to Error Node`. When a compilation error occurs, the system will automatically locate the erroneous Blueprint node for you.
* **Execution Arrows**: Check `Draw Execution Arrows` to conveniently observe the execution flow of Blueprints during debugging.

### 4. General - Region & Language
For users unfamiliar with English Blueprint nodes, this feature is very practical.
* **Localize Blueprint Nodes**: Find and check `Use Localized Graph Node and Pin Naming`.
    * *Effect demonstration:* After checking, many English nodes in Blueprints (such as Set lighting parameters, etc.) will automatically be displayed in Chinese, significantly lowering the reading barrier.

---

## Part 2: Project Settings

Select `Edit` -> `Project Settings` in the UE top menu bar to open the panel. Since the SuperStage template has pre-configured most parameters, we only need to focus on checking the following items.

### 1. Project - Description
This is the project's basic identity card; you can fill it in as needed without affecting actual project operation.
* You can set a **project thumbnail**.
* You can fill in the project description, project ID, project name, and related copyright notices.

### 2. Project - Maps & Modes
* **Default Maps**: In `Editor Startup Map` and `Game Default Map`, select the level you need (e.g., the default level from the SuperStage template). This way, every time you open the project, it will directly enter that stage scene instead of UE's default empty open world.

### 3. Engine - Rendering
Since SuperStage involves many complex lighting effects, rendering settings are crucial.
* **Global Illumination**: Ensure `Dynamic Global Illumination Method` is set to **Lumen**.
* **Hardware Ray Tracing**:
    * Check `Support Hardware Ray Tracing`.
    * Check `Use Hardware Ray Tracing when available`.
* **Lighting Related**: Ensure project options related to "Super lighting/volumetric light" support are checked; these are essential for stage lighting effects.
* **Light Function Atlas Format**: (Very critical!) Find the format setting related to light functions and change it to **8-bit RGB**.
    * *Why change this?* By default, the light spots produced by Gobo (pattern lights) are only black, white, and gray (i.e., grayscale). After changing it to RGB format, your pattern light spots can display colorful effects.

### 4. Platforms - Windows
If your project needs to be packaged for the Windows platform, check the following settings:
* **Splash**: You can replace the default UE splash image with your own project poster or logo image.
* **Targeted RHIs**: In the `Default RHI` option, you **must select DirectX 12**. If DirectX 11 is selected, stage beam effects cannot be rendered.
* **D3D12 Targeted Shader Formats**: You must check **SM6**. Sometimes the system defaults to only checking SM5, which prevents Volumetric Light effects from rendering correctly.

---

> **Congratulations!** After completing the above configuration, your UE engine is now in the optimal state for running SuperStage projects. You can continue to the next section to learn about specific level operations and construction.
