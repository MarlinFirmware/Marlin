# Auto Build Marlin

"AutoBuildMarlin" is a *Visual Studio Code* extension that provides a one-button interface to build and upload Marlin Firmware to your selected `MOTHERBOARD`, removing the need to edit your `platformio.ini` file or scroll through a long list of Marlin environments.

## Get PlatformIO

Before you install AutoBuildMarlin you'll first need to [Install PlatformIO in VSCode](http://marlinfw.org/docs/basics/install_platformio_vscode.html). Once you have followed these instructions, continue below.

## Installing This Extension

- [Download Marlin Firmware](http://marlinfw.org/meta/download/) and unzip it to your documents folder.
- Open the directory `buildroot/share/vscode` and copy the "`AutoBuildMarlin`" folder to **the *Visual Studio Code* `extensions` directory**.
- Relaunch *Visual Studio Code* to complete the installation.

### To find your `extensions` directory:

- **Windows** - Use Windows Explorer's address bar to open `C:/Users/USERNAME/.vscode/extensions`.
- **Mac** - Use the Finder's `Go` menu to open `~/.vscode/extensions`.
- **Linux** - In the Terminal type `open ~/.vscode/extensions`.

## Usage

- Open up the downloaded *Marlin Firmware* project folder (***NOT the "Marlin" folder within***) in *Visual Studio Code*. (You may also use the **Import Project…** option from the "PlaformIO Home" page.)

- With Marlin open, the "File Explorer" should be firmly rooted in your Marlin Firmware folder:

  ![](https://github.com/MarlinFirmware/Marlin/raw/bugfix-2.0.x/buildroot/share/vscode/AutoBuildMarlin/img/Activity_bar.png)

- Click the **Marlin Auto Build** icon ![AutoBuild Icon](https://github.com/MarlinFirmware/Marlin/raw/bugfix-2.0.x/buildroot/share/vscode/AutoBuildMarlin/img/AB_icon.png) in the Activities Bar (on the left side of *Visual Studio Code* window) to bring up the **Marlin Auto Build** options bar.

  ![](https://github.com/MarlinFirmware/Marlin/raw/bugfix-2.0.x/buildroot/share/vscode/AutoBuildMarlin/img/AB_menu.png)

- Click one of the four icons

  Icon|Action
  ----|------
  ![](https://github.com/MarlinFirmware/Marlin/raw/bugfix-2.0.x/buildroot/share/vscode/AutoBuildMarlin/img/B_small.png)|Start **Marlin Build** to test your Marlin build
  ![](https://github.com/MarlinFirmware/Marlin/raw/bugfix-2.0.x/buildroot/share/vscode/AutoBuildMarlin/img/U_small.png)|Start **Marlin Upload** to install Marlin on your board
  ![](https://github.com/MarlinFirmware/Marlin/raw/bugfix-2.0.x/buildroot/share/vscode/AutoBuildMarlin/img/T_small.png)|Start **Marlin Upload (traceback)** to install Marlin with debugging
  ![](https://github.com/MarlinFirmware/Marlin/raw/bugfix-2.0.x/buildroot/share/vscode/AutoBuildMarlin/img/C_small.png)|Start **Marlin Clean** to delete old build files
