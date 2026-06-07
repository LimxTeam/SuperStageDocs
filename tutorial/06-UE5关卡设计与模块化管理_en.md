# UE5 Level Design & Modular Management

In stage design, we treat each level as an independent "scene module." By separating stage structure, lighting, decorative elements, and other content into different levels, we can achieve more efficient team collaboration and flexibly combine different stage proposals.

## 1. Basic Level Concepts
In SuperStage, each "Level" represents an independent world or scene component.
* **Main Level**: Acts as a container for all components, responsible for integrating the final result.
* **Sub-Level**: Stores specific resources (such as lighting groups, stage scaffolding, screen structures, etc.).

---

## 2. Preparation: Create a Level Directory
Good file management is the beginning of efficient development.
1.  In the root directory of the project Content Browser, right-click to create a new folder and name it **`Maps`**.
2.  All level files will be saved in this folder.

---

## 3. Create & Configure the Main Level
1.  **New Level**: Click "File" -> "New Level" and select a basic template.
2.  **Clean Up Environment**:
    * Delete the default "PlayerStart."
    * Delete unnecessary sky spheres or other preset objects.
3.  **Adjust Basic Lighting**: Select the directional light (DirectionalLight) and adjust its position or brightness to a state suitable for stage preview.
4.  **Save**: Name this level **`Main`** and store it in the `Maps` folder.

---

## 4. Modular Level Design Steps

### A. Stage Structure (Scaffold) Design
1.  In the main level, drag a stage model (such as the `Scaffold` rigging frame) from the SuperStage asset library.
2.  **Adjust Parameters**: Modify parameters in the details panel:
    * **X/Y Axis Count**: Set the width and depth of the frame (e.g., 2 layers thick, 10 layers wide).
    * **Height**: Set the number of layers (e.g., 5 layers high).
3.  **Convert to Sub-Level**:
    * Select all the constructed stage structures.
    * Open the **"Levels"** window (Window -> Levels).
    * Click "Create New Level" in the "Levels" window and choose "Create New Level with Selected Actors."
    * Name it **`StageStructure`**.
    * **Important Setting**: Right-click this sub-level and change the "Streaming Method" to **"Always Loaded"** (otherwise it may not display during rendering).

### B. Lighting Level Design
1.  Switch the current editing level to the **`Lighting`** level (or create a new one).
2.  Select fixture models from the library and use SuperStage's **Array Mode** to quickly arrange lighting:
    * Set the number of fixtures (e.g., 12 units).
    * Set spacing (e.g., 1 meter).
    * Adjust Z-axis rotation (e.g., rotate 90) to ensure the lamp heads face the correct direction.
3.  Similarly, save the lighting group as an independent level.

---

## 5. Level Reuse & Multi-Proposal Comparison

### Scenario 1: Reusing Structures Across Multiple Scenes
If you create a new project level (such as `NewMap`), you don't need to rebuild the scaffolding. Simply select "Add Existing" in the "Levels" window and add the previous `StageStructure` level.

### Scenario 2: Proposal Iteration (A/B Test)
1.  **Copy Proposal**: In the Content Browser, select the `StageStructure` level and press **`Ctrl + D`** to duplicate it as `StageStructure_ProposalB`.
2.  **Modify Proposal**: In `ProposalB`, delete or add some Actors (e.g., remove the portal frame).
3.  **Quick Switching**: In the main level, you can quickly show the client two visual effects — "with frame" and "without frame" — by toggling the visibility (eye icon) of different sub-levels.

---

## 6. Key Notes (Pitfall Avoidance Guide)
* **Naming Conventions**: Although Chinese naming is supported, it is recommended to use English whenever possible to prevent crashes or path errors in packaged or complex environments.
* **Streaming Settings**: Always check the streaming method of all sub-levels. In stage previews, they typically need to be set to **"Always Loaded"**.
* **Layer Operations**: Before adding or dragging in new models, ensure you have double-clicked the correct level in the "Levels Window"; otherwise, models will be saved haphazardly in other levels.

---
