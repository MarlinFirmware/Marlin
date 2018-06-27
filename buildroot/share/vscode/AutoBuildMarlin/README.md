# Auto Build support for Visual Studio Code
This `Visual Studio Code` extension provides access to the `Auto Build` script.

## Installation

Get the MarlinFirmware repository from GitHub. Open the directory `buildroot/share/vscode` and copy the `AutoBuildMarlin` folder to the `Visual Studio Code` extension directory. Relaunch `Visual Studio Code` to complete the installation.

To find the `Visual Studio Code` extension directory:

- Windows - Use Windows Explorer's address bar to open `C:/Users/USERNAME/.vscode/extensions`.
- Mac - Use the Finder's `Go` menu to open `~/.vscode/extensions`.
- Linux - In the Terminal type `open ~/.vscode/extensions`.

### 3. Install the PlatformIO extension
Click on `View` > `Command Palette...`

![](./resources/view_command_palette.png)

Find and click on `Extensions: Install Extensions`

![](./resources/install_extensions.png)

Type `platformio` into the search box and click on `Install` under `PlatformIO IDE`.

![](./resources/platformio_install.png)

## Usage

This extension adds the Auto Build icon ![](./media/AB.svg) to the Activities bar.

### 1. Open the Marlin folder
Click on `File` > `Open Folder...`

![](./resources/Open_Folder.png)

This brings up the `Open Folder` dialog. Select the folder that has the `platformio.ini` file in it.

![](./resources/Open_Marlin.png)

You should see something like the following. If not, click on the Explorer icon in the Activities bar.

![](./resources/Activity_bar.png)

### 2. Click on the Auto Build Icon ![](./media/AB.svg)
This brings up the Auto Build menu icon bar.
![](./resources/AB_menu.png)

### 3. Click on one of the four icons
- ![](./resources/B_small.svg) - Clicking on it starts `PIO Build`
- ![](./resources/C_small.svg) - Clicking on it starts `PIO Clean`
- ![](./resources/U_small.svg) - Clicking on it starts `PIO Upload`
- ![](./resources/Ut_small.svg) - Clicking on it starts `PIO Upload (traceback)`
