# SuperStage Full Suite Installation Tutorial

## Preparation: Download the Full Suite

1. Visit the SuperStage official website download page ([yunsio.com/download](https://yunsio.com/download)).
2. Click the **"Free Download"** button and select **Windows Download**.
3. The page will redirect to Baidu Cloud Drive. Please download the **entire folder** from the cloud drive to your local machine.
   * *Note: The folder contains essential modules including the UE main plugin, data transfer tools, drone control software, MA2 console software and crack patches, sample projects, and plugins.*

---

## Part 1: Install SuperStage Core Components

### Step 1: Install UE Main Plugin (SuperStageForUE)
1. Find the UE plugin installer in the downloaded folder (e.g., `SuperStageSetup-26Q2.2.exe`) and double-click to run.
2. Click "Next" repeatedly.
3. **Select Version**: Check the Unreal Engine version installed on your computer (this tutorial uses UE 5.7 as an example).
4. **Check Template**: Be sure to check **"Install Project Template"** at the bottom for conveniently creating projects later.
> **Critical Note**: Before clicking "Install," you **must close all running Unreal Engine (UE) editors!** Otherwise, files will be locked, causing installation failure or errors.
5. Accept the license agreement, click "Next" until installation is complete.

### Step 2: Install Data Transfer Hub (SuperData)
1. Find the installer in the `SuperData` folder (e.g., `SuperDataSetup-2.1.0.exe`) and double-click to run.
2. Select **"Install for all users"**.
3. Choose an installation path (custom folder allowed).
4. Click "Install" and exit when done.
*Note: This is a background system service that runs automatically after installation; no manual opening is needed for daily use.*

### Step 3: Install Drone Control Software (LimxDroneStudio)
1. Find the `LimxDroneStudio` folder and double-click the installer.
2. Similarly select **"Install for all users"**.
3. Choose an installation path and check "Create desktop shortcut"; click "Next" until installation completes.

---

## Part 2: Install MA2 Console Software (Recommended)

Follow these steps to install the software and configure the communication environment if you use MA2 for lighting control. Skip this part if you don't need MA2.

### Step 1: Install MA2 onPC
1. Find the MA2 installer `grandMA2_onPC_3.3.4.3.exe` in the download package and double-click to run.
2. It is recommended to **customize the installation path** (try not to install in the default Program Files directory on C drive to avoid permission issues).
3. Keep clicking "Next" until installation completes. **After installation, do NOT run the software immediately!**

### Step 2: Replace Crack Patch
1. Find the MA2 crack patch file `gma2onpc.exe` in the download package.
2. Go to the desktop, right-click the newly created MA2 shortcut, and select **"Open file location"**.
3. Drag the crack patch file into that directory and select **"Replace the file in the destination"** in the popup.
4. After replacement, double-click to open the MA2 software.
5. On first launch, a user agreement will appear; scroll to the bottom, click **"I AGREE"** (and check "Do not show again"), then close the software.

### Step 3: Import SuperStage Preset Project (Show File)
To help beginners get started quickly, the full suite includes a pre-configured MA2 template project with all fixtures and plugins set up.
1. Open Windows File Explorer and navigate to the C drive.
2. **Enable hidden file display**: Click "View" > "Show" > Check "Hidden items" in the top menu.
3. Navigate to this path: `C:\ProgramData\MA Lighting Technologies\grandma\gma2_V_3.3.4\shows`.
4. Copy the preset project file from the download package (e.g., `SuperStage 26Q2.1.show.gz`) into this `shows` folder.
5. Reopen the MA2 software and click the **Backup** button on the right.
6. Click **Load Show**, select the SuperStage project file you just placed, and load it.
   * *At this point, all fixtures and communication plugins are configured and ready for you.*

*(Alternative: Manual MA2 Plugin Installation)*
If you are an advanced user who wants to use SuperStage with your own existing MA2 project, copy the files in the download package's `plugins` folder (such as `SuperData.xml`, etc.) to `C:\ProgramData\...\gma2_V_3.3.4\plugins`. Then edit an empty plugin slot in MA2's Plugin window, click Import, and import `SuperData`.

---

## Part 3: Cleanup & Verification

### Step 1: Clean Up Installers
> **Strongly recommended**: After installation, please **delete all downloaded installer source files**.
This effectively avoids confusing old and new installers on your computer when SuperStage releases new versions, preventing accidental installation of old versions.

### Step 2: Create a SuperStage Project in UE for Verification
1. Open the Epic Games Launcher, or directly navigate to the UE installation directory (e.g., `Engine\Binaries\Win64\UnrealEditor.exe`) to launch the engine manually.
2. In the "New Project" panel on the left, find and select the **"SuperStage"** template.
3. Configure the project name and save path in the lower right corner.
> **Fatal Error Warning**: The project name and save path **must be entirely in English or pinyin**! Absolutely no Chinese characters are allowed, as this will cause frequent project errors or prevent packaging later.
4. Click "Create."
5. The first time loading this project requires compiling a large number of shaders, which takes considerable time (a percentage indicator appears in the lower right corner). Please be patient.
6. After compilation and loading, the main interface opens. You can now open the Content Browser and drag various fixtures and stage equipment from the SuperStage folder directly into the scene!
