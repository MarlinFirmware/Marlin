# Development on Windows
On Windows, the building and flashing processes have been automatized on a batch file.

1. Open a new Command Prompt. Press "WinKey + R" and type "cmd" on the recently open window.

2. Browse to your root project folder.

  ```
  cd C:\your_folder_path\witbox-fw
  ```

3. Connect your computer to the printer and identify the COM port assigned.

3. Launch the script.

  ```
  make.cmd
  ```

4. Follow the wizard's instructions to select your device configuration and the COM port used by the printer.

5. Wait until the script finish.