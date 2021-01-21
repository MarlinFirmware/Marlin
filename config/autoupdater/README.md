This is a total hacky script that may or may not work. It comes with no Guarantee so make sure to save backups of your configs in case things go wrong. 

Use:
Ensure old configs are present in Marlin/Marlin folder
run autoupdater.sh script
+ ./autoupdater.sh
+ bash autoupdater.sh

Your newly updated configs will now be present in the Marlin/Marlin folder. Your old config files have been overwritten so I hope you made your own backup. 
In case you didn't, read below:

Within the autoupdater folder, you will see two *_Config folders
One says Current and that is where your old configs in case you didn't make backups exists
One says Updated and is the same file that is located in the Marlin/Marlin folder

If the script doesn't work for any reason, try deleting everything inside of autoupdater folder except for autoupdater.sh. The script will regenerate any missing folders and files it needs.

Lastly AND MOST IMPORTANTLY,
Open the newly updated Configs in your favorite mergetool or simply open it within Visual Studio Code, which has a built in merge tool, and resolve all conflicts manually by clicking to either accept the new change or the user change.


Note to myself/future-devs:
Future ToDo:
+ Ability to dynamically select the best base_dir/config_base depending on the Machine Name and Motherboard listed in the user supplied config files.
+ include a virtualenv-ed python module that has a better merge tool that can be programmatically activated. 
+ ask for user preferences throughout to ensure tool can be used in more ways than a single size fits all method
+ make the script less hacky and more "professional" looking. Ensure it works the same in all environments. Possibly make it into a dockerfile. 
+ Programmaticly autoresolve conflicts that are known to be irrelevant (i.e. if the only difference is a space or some non essential character)
+ better config file visualizer for the command line impaired. 