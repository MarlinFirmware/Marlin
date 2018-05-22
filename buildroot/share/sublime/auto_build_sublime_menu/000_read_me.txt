Overview:
1) Install Sublime
2) Install Deviot  (?optional?)
3) Install WebDevShell (this will execute the auto-build script)
4) Copy the menu configuration to the proper Sublime directory
5) Add platformio to your path (usually not needed)


Sublime with autobuild
  Tools
    Install Package Control
  Tools
    Command Palette
      Package Control: Install Package
          type in deviot and click on it
  Tools
    Command Palette
      Package Control: Install Package
          type in WebDevShell and click on it

  in Sublime, open Marlin directory with "platformio.ini" in it

  starting in the top level directory, go to the folder "Buildroot/shared/Sublime"
    copy the folder "auto_build_sublime_menu" and contents to:
      Windows
       \Users\your_user_name\AppData\Roaming\Sublime Text 3\Packages
      Linux
        /home/your_user_name/.config/sublime-text-3/Packages/User
      macOS (Click on the Finder's 'Go' menu and hold down Option to open...)
        ~/Library/Application Support/Sublime Text 3/Packages/User

The menu should now be visible

If you get an error message that says "file not found" and  "subprocess.Popen(['platformio' ... "
then you'll need to add platformio to your path.
  macOS
      sudo nano /etc/paths
        add these to the bottom
            /Users/bob/.platformio
            /Users/bob/.platformio/penv/bin
