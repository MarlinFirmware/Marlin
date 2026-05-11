#pragma once

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif

#pragma region IGFD LICENSE

/*
MIT License

Copyright (c) 2018-2024 Stephane Cuillerdier (aka aiekick)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma endregion

#pragma region IGFD DOCUMENTATION

/*
// generated with "Text to ASCII Art Generator (TAAG)"
// https://patorjk.com/software/taag/#p=display&h=1&v=0&f=Big&t=ImGuiFileDialog%0Av0.6.7
  _____              _____         _  ______  _  _        _____   _         _
 |_   _|            / ____|       (_)|  ____|(_)| |      |  __ \ (_)       | |
   | |   _ __ ___  | |  __  _   _  _ | |__    _ | |  ___ | |  | | _   __ _ | |  ___    __ _
   | |  | '_ ` _ \ | | |_ || | | || ||  __|  | || | / _ \| |  | || | / _` || | / _ \  / _` |
  _| |_ | | | | | || |__| || |_| || || |     | || ||  __/| |__| || || (_| || || (_) || (_| |
 |_____||_| |_| |_| \_____| \__,_||_||_|     |_||_| \___||_____/ |_| \__,_||_| \___/  \__, |
                                                                                       __/ |
                                                                                      |___/
                                  ___      __    ______
                                 / _ \    / /   |____  |
                         __   __| | | |  / /_       / /
                         \ \ / /| | | | | '_ \     / /
                          \ V / | |_| |_| (_) |_  / /
                           \_/   \___/(_)\___/(_)/_/

github repo : https://github.com/aiekick/ImGuiFileDialog
this section is the content of the ReadMe.md file

# ImGuiFileDialog

## Purpose

ImGuiFileDialog is a file selection dialog built for (and using only) [Dear ImGui](https://github.com/ocornut/imgui).

My primary goal was to have a custom pane with widgets according to file extension. This was not possible using other
solutions.

## ImGui Supported Version

ImGuiFileDialog follow the master and docking branch of ImGui . currently ImGui 1.90.4 WIP

## Structure

* The library is in [Lib_Only branch](https://github.com/aiekick/ImGuiFileDialog/tree/Lib_Only)
* A demo app can be found the [master branch](https://github.com/aiekick/ImGuiFileDialog/tree/master)

This library is designed to be dropped into your source code rather than compiled separately.

From your project directory:

```
mkdir lib    <or 3rdparty, or externals, etc.>
cd lib
git clone https://github.com/aiekick/ImGuiFileDialog.git
git checkout Lib_Only
```

These commands create a `lib` directory where you can store any third-party dependencies used in your project, downloads
the ImGuiFileDialog git repository and checks out the Lib_Only branch where the actual library code is located.

Add `lib/ImGuiFileDialog/ImGuiFileDialog.cpp` to your build system and include
`lib/ImGuiFileDialog/ImGuiFileDialog.h` in your source code. ImGuiFileLib will compile with and be included directly in
your executable file.

If, for example, your project uses cmake, look for a line like `add_executable(my_project_name main.cpp)`
and change it to `add_executable(my_project_name lib/ImGuiFileDialog/ImGuiFileDialog.cpp main.cpp)`. This tells the
compiler where to find the source code declared in `ImGuiFileDialog.h` which you included in your own source code.

## Requirements:

You must also, of course, have added [Dear ImGui](https://github.com/ocornut/imgui) to your project for this to work at
all.

[dirent v1.23](https://github.com/tronkko/dirent/tree/v1.23) is required to use ImGuiFileDialog under Windows. It is
included in the Lib_Only branch for your convenience.

################################################################
## Features
################################################################

- C Api (succesfully tested with CimGui)
- Separate system for call and display
    - Can have many function calls with different parameters for one display function, for example
- Can create a custom pane with any widgets via function binding
    - This pane can block the validation of the dialog
    - Can also display different things according to current filter and UserDatas
- Advanced file style for file/dir/link coloring / icons / font
    - predefined form or user custom form by lambda function (the lambda mode is not available for the C API)
- Multi-selection (ctrl/shift + click) :
    - 0 => Infinite
    - 1 => One file (default)
    - n => n files
- Compatible with MacOs, Linux, Windows, Emscripten, Android
- Supports modal or standard dialog types
- Select files or directories
- Filter groups and custom filter names
- can ignore filter Case for file searching
- Keyboard navigation (arrows, backspace, enter)
- Exploring by entering characters (case insensitive)
- Custom places (bookmarks, system devices, whatever you want)
- Directory manual entry (right click on any path element)
- Optional 'Confirm to Overwrite" dialog if file exists
- Thumbnails Display (agnostic way for compatibility with any backend, sucessfully tested with OpenGl and Vulkan)
- The dialog can be embedded in another user frame than the standard or modal dialog
- Can tune validation buttons (placements, widths, inversion)
- Can quick select a parrallel directory of a path, in the path composer (when you clikc on a / you have a popup)
- regex support for filters, collection of filters and filestyle (the regex is recognized when between (( and )) in a filter)
- multi layer extentions like : .a.b.c .json.cpp .vcxproj.filters etc..
- advanced behavior regarding asterisk based filter. like : .* .*.* .vcx.* .*.filters .vcs*.filt.* etc.. (internally regex is used)
- result modes GetFilePathName, GetFileName and GetSelection (overwrite file ext, keep file, add ext if no user ext exist)
- you can use your own FileSystem Api
    - by default Api Dirent and std::filesystem are defined
    - you can override GetDrieveList for specify by ex on android other fs, like local and SDCards

################################################################
## Filter format
################################################################

A filter is recognized only if he respects theses rules :

0) a filter must have 2 chars mini and the first must be a .
1) a regex must be in (( and ))
2) a , will separate filters except if between a ( and )
3) name{filter1, filter2} is a special form for collection filters
   - the name can be composed of what you want except { and }
   - a filter can be a regex
4) the filters cannot integrate these chars '(' ')' '{' '}' ' ' except for a regex with respect to rule 1)
5) the filters cannot integrate a ','

################################################################
## Singleton Pattern vs. Multiple Instances
################################################################

### Single Dialog :

If you only need to display one file dialog at a time, use ImGuiFileDialog's singleton pattern to avoid explicitly
declaring an object:

```cpp
ImGuiFileDialog::Instance()->method_of_your_choice();
```

### Multiple Dialogs :

If you need to have multiple file dialogs open at once, declare each dialog explicity:

```cpp
ImGuiFileDialog instance_a;
instance_a.method_of_your_choice();
ImGuiFileDialog instance_b;
instance_b.method_of_your_choice();
```

################################################################
## Simple Dialog :
################################################################

```cpp
void drawGui()
{
  // open Dialog Simple
  if (ImGui::Button("Open File Dialog"))
    IGFD::FileDialogConfig config;
    config.path = ".";
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", config);

  // display
  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
  {
    // action if OK
    if (ImGuiFileDialog::Instance()->IsOk())
    {
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      // action
    }

    // close
    ImGuiFileDialog::Instance()->Close();
  }
}
```

![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/dlg_simple.gif)

################################################################
## Modal dialog :
################################################################

you have now a flag for open modal dialog :

```cpp
ImGuiFileDialogFlags_Modal
```

you can use it like that :

```cpp
IGFD::FileDialogConfig config;
config.path = ".";
config.countSelectionMax = 1;
config.flags = ImGuiFileDialogFlags_Modal;
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", config);
```

################################################################
## Directory Chooser :
################################################################

To have a directory chooser, set the file extension filter to nullptr:

```cpp
IGFD::FileDialogConfig config;
config.path = ".";
ImGuiFileDialog::Instance()->OpenDialog("ChooseDirDlgKey", "Choose a Directory", nullptr, config);
```

In this mode you can select any directory with one click and open a directory with a double-click.

![directoryChooser](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/directoryChooser.gif)

################################################################
## Dialog with Custom Pane :
################################################################

The signature of the custom pane callback is:

### for C++ :

```cpp
void(const char *vFilter, IGFDUserDatas vUserDatas, bool *vCantContinue)
```

### for C :

```c
void(const char *vFilter, void* vUserDatas, bool *vCantContinue)
```

### Example :

```cpp
static bool canValidateDialog = false;
inline void InfosPane(cosnt char *vFilter, IGFDUserDatas vUserDatas, bool *vCantContinue) // if vCantContinue is false,
the user cant validate the dialog
{
    ImGui::TextColored(ImVec4(0, 1, 1, 1), "Infos Pane");
    ImGui::Text("Selected Filter : %s", vFilter.c_str());
    if (vUserDatas)
        ImGui::Text("UserDatas : %s", vUserDatas);
    ImGui::Checkbox("if not checked you cant validate the dialog", &canValidateDialog);
    if (vCantContinue)
        *vCantContinue = canValidateDialog;
}

void drawGui()
{
  // open Dialog with Pane
  if (ImGui::Button("Open File Dialog with a custom pane")) {
	IGFD::FileDialogConfig config;
	config.path = ".";
	config.countSelectionMax = 1;
	config.sidePane = std::bind(&InfosPane, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	config.sidePaneWidth = 350.0f;
	config.useDatas = UserDatas("InfosPane");
	config.flags = ImGuiFileDialogFlags_Modal;
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", config);
  }

  // display and action if ok
  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
  {
    if (ImGuiFileDialog::Instance()->IsOk())
    {
        std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
        std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
        std::string filter = ImGuiFileDialog::Instance()->GetCurrentFilter();
        // here convert from string because a string was passed as a userDatas, but it can be what you want
        std::string userDatas;
        if (ImGuiFileDialog::Instance()->GetUserDatas())
            userDatas = std::string((const char*)ImGuiFileDialog::Instance()->GetUserDatas());
        auto selection = ImGuiFileDialog::Instance()->GetSelection(); // multiselection

        // action
    }
    // close
    ImGuiFileDialog::Instance()->Close();
  }
}
```

![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/doc/dlg_with_pane.gif)

################################################################
## File Style : Custom icons and colors by extension
################################################################

You can define style for files/dirs/links in many ways :

the style can be colors, icons and fonts

the general form is :
```cpp
ImGuiFileDialog::Instance()->SetFileStyle(styleType, criteria, color, icon, font);
```

### Predefined Form :

styleType can be thoses :

```cpp
IGFD_FileStyleByTypeFile				// define style for all files
IGFD_FileStyleByTypeDir					// define style for all dir
IGFD_FileStyleByTypeLink				// define style for all link
IGFD_FileStyleByExtention				// define style by extention, for files or links
IGFD_FileStyleByFullName				// define style for particular file/dir/link full name (filename + extention)
IGFD_FileStyleByContainedInFullName		// define style for file/dir/link when criteria is contained in full name
```

### Lambda Function Form :

You can define easily your own style include your own detection by using lambda function :

** To note, this mode is not available with the C API **

this lamba will treat a file and return a shared pointer of your files style

see in action a styling of all files and dir starting by a dot

this lambda input a FileInfos and output a FileStyle
return true if a FileStyle was defined

```cpp
ImGuiFileDialog::Instance()->SetFileStyle([](const IGFD::FileInfos& vFile, IGFD::FileStyle &vOutStyle) -> bool {
    if (!vFile.fileNameExt.empty() && vFile.fileNameExt[0] == '.') {
        vOutStyle = IGFD::FileStyle(ImVec4(0.0f, 0.9f, 0.9f, 1.0f), ICON_IGFD_REMOVE);
        return true;
    }
    return false;
});
```

see sample app for the code in action

### Samples :

ImGuiFileDialog accepts icon font macros as well as text tags for file types.

[ImGuIFontStudio](https://github.com/aiekick/ImGuiFontStudio) is useful here. I wrote it to make it easy to create
custom icon sets for use with Dear ImGui.

It is inspired by [IconFontCppHeaders](https://github.com/juliettef/IconFontCppHeaders), which can also be used with
ImGuiFileDialog.

samples :

```cpp
// define style by file extention and Add an icon for .png files
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".png", ImVec4(0.0f, 1.0f, 1.0f, 0.9f),,ICON_IGFD_FILE_PIC, font1); 
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".gif", ImVec4(0.0f, 1.0f, 0.5f, 0.9f), "[GIF]");

// define style for all directories
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir, "", ImVec4(0.5f, 1.0f, 0.9f, 0.9f), ICON_IGFD_FOLDER);
// can be for a specific directory
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir, ".git", ImVec4(0.5f, 1.0f, 0.9f, 0.9f), ICON_IGFD_FOLDER);

// define style for all files
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeFile, "", ImVec4(0.5f, 1.0f, 0.9f, 0.9f), ICON_IGFD_FILE);
// can be for a specific file
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeFile, ".git", ImVec4(0.5f, 1.0f, 0.9f, 0.9f),
ICON_IGFD_FILE);

// define style for all links
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeLink, "", ImVec4(0.5f, 1.0f, 0.9f, 0.9f));
// can be for a specific link
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeLink, "Readme.md", ImVec4(0.5f, 1.0f, 0.9f, 0.9f));

// define style for any files/dirs/links by fullname
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByFullName, "doc", ImVec4(0.9f, 0.2f, 0.0f, 0.9f),
ICON_IGFD_FILE_PIC);

// define style by file who are containing this string
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByContainedInFullName, ".git", ImVec4(0.9f, 0.2f, 0.0f, 0.9f),
ICON_IGFD_BOOKMARK);

all of theses can be miwed with IGFD_FileStyleByTypeDir / IGFD_FileStyleByTypeFile / IGFD_FileStyleByTypeLink
like theses by ex :
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir | IGFD_FileStyleByContainedInFullName, ".git",
ImVec4(0.9f, 0.2f, 0.0f, 0.9f), ICON_IGFD_BOOKMARK); ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeFile
| IGFD_FileStyleByFullName, "cmake", ImVec4(0.5f, 0.8f, 0.5f, 0.9f), ICON_IGFD_SAVE);

// for all these,s you can use a regex
// ex for color files like Custom*.h
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByFullName, "((Custom.+[.]h))", ImVec4(0.0f, 1.0f, 1.0f, 0.9f),
ICON_IGFD_FILE_PIC, font1);

// lambda function
// set file style with a lambda function
// return true is a file style was defined
ImGuiFileDialog::Instance()->SetFileStyle([](const IGFD::FileInfos& vFile, IGFD::FileStyle &vOutStyle) -> bool {
    if (!vFile.fileNameExt.empty() && vFile.fileNameExt[0] == '.') {
        vOutStyle = IGFD::FileStyle(ImVec4(0.0f, 0.9f, 0.9f, 1.0f), ICON_IGFD_REMOVE);
        return true;
    }
    return false;
});
```

this sample code of [master/main.cpp](https://github.com/aiekick/ImGuiFileDialog/blob/master/main.cpp) produce the
picture above :

```cpp
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".cpp", ImVec4(1.0f, 1.0f, 0.0f, 0.9f));
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".h", ImVec4(0.0f, 1.0f, 0.0f, 0.9f));
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".hpp", ImVec4(0.0f, 0.0f, 1.0f, 0.9f));
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".md", ImVec4(1.0f, 0.0f, 1.0f, 0.9f));
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".png", ImVec4(0.0f, 1.0f, 1.0f, 0.9f),
ICON_IGFD_FILE_PIC); // add an icon for the filter type
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".gif", ImVec4(0.0f, 1.0f, 0.5f, 0.9f), "[GIF]");
// add an text for a filter type ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir, nullptr,
ImVec4(0.5f, 1.0f, 0.9f, 0.9f), ICON_IGFD_FOLDER); // for all dirs
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeFile, "CMakeLists.txt", ImVec4(0.1f, 0.5f, 0.5f, 0.9f),
ICON_IGFD_ADD); ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByFullName, "doc", ImVec4(0.9f, 0.2f, 0.0f,
0.9f), ICON_IGFD_FILE_PIC); ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir |
IGFD_FileStyleByContainedInFullName, ".git", ImVec4(0.9f, 0.2f, 0.0f, 0.9f), ICON_IGFD_BOOKMARK);
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeFile | IGFD_FileStyleByContainedInFullName, ".git",
ImVec4(0.5f, 0.8f, 0.5f, 0.9f), ICON_IGFD_SAVE);
```

![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/color_filter.png)


################################################################
## Filter Collections
################################################################

You can define a custom filter name that corresponds to a group of filters using this syntax:

```custom_name1{filter1,filter2,filter3},custom_name2{filter1,filter2},filter1```

When you select custom_name1, filters 1 to 3 will be applied. The characters `{` and `}` are reserved. Don't use them
for filter names.

this code :

```cpp
const char *filters = "Source files (*.cpp *.h *.hpp){.cpp,.h,.hpp},Image files (*.png *.gif *.jpg *.jpeg){.png,.gif,.jpg,.jpeg},.md";
IGFD::FileDialogConfig config;
config.path = ".";
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", ICON_IMFDLG_FOLDER_OPEN " Choose a File", filters, config);
```


will produce :
![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/filters.gif)

################################################################
## Multi Selection
################################################################

You can define in OpenDialog call the count file you want to select :

- 0 => infinite
- 1 => one file only (default)
- n => n files only

See the define at the end of these funcs after path.

```cpp
IGFD::FileDialogConfig config; config.path = ".";

ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*,.cpp,.h,.hpp", config);

config.countSelectionMax = 1;
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose 1 File", ".*,.cpp,.h,.hpp", config);

config.countSelectionMax = 5;
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose 5 File", ".*,.cpp,.h,.hpp", config);

config.countSelectionMax = 0;
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose many File", ".*,.cpp,.h,.hpp", config);

config.countSelectionMax = 1;
config.sidePane = std::bind(&InfosPane, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
config.sidePaneWidth = 350.0f;
config.useDatas = UserDatas("SaveFile");
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png,.jpg", config); // 1 file
```

![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/multiSelection.gif)

################################################################
## File Dialog Constraints
################################################################

You can set the minimum and/or maximum size of the dialog:

```cpp
ImVec2 maxSize = ImVec2((float)display_w, (float)display_h);  // The full display area
ImVec2 minSize = maxSize * 0.5f;  // Half the display area
ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, minSize, maxSize);
```

![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/dialog_constraints.gif)

################################################################
## Exploring by keys
################################################################

You can activate this feature by uncommenting `#define USE_EXPLORATION_BY_KEYS`
in your custom config file (CustomImGuiFileDialogConfig.h)

You can also uncomment the next lines to define navigation keys:

* IGFD_KEY_UP => Up key for explore to the top
* IGFD_KEY_DOWN => Down key for explore to the bottom
* IGFD_KEY_ENTER => Enter key for open directory
* IGFD_KEY_BACKSPACE => BackSpace for comming back to the last directory

You can also jump to a point in the file list by pressing the corresponding key of the first filename character.

![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/explore_ny_keys.gif)

As you see the current item is flashed by default for 1 second. You can define the flashing lifetime with the function

```cpp
ImGuiFileDialog::Instance()->SetFlashingAttenuationInSeconds(1.0f);
```
################################################################
## Path Edition :
################################################################

Right clicking on any path element button allows the user to manually edit the path from that portion of the tree.
Pressing the completion key (GLFW uses `enter` by default) validates the new path. Pressing the cancel key (GLFW
uses`escape` by default) cancels the manual entry and restores the original path.

Here's the manual entry operation in action:
![inputPathEdition.gif](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/inputPathEdition.gif)

################################################################
## Confirm Overwrite Dialog :
################################################################

If you want avoid overwriting files after selection, ImGuiFileDialog can show a dialog to confirm or cancel the
operation.

To do so, define the flag ImGuiFileDialogFlags_ConfirmOverwrite in your call to OpenDialog.

By default this flag is not set since there is no pre-defined way to define if a dialog will be for Open or Save
behavior. (by design! :) )

Example code For Standard Dialog :

```cpp
IGFD::FileDialogConfig config;
config.path = ".";
config.flags = ImGuiFileDialogFlags_ConfirmOverwrite;
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey",
    ICON_IGFD_SAVE " Choose a File", filters, config);
```

Example code For Modal Dialog :

```cpp
IGFD::FileDialogConfig config;
config.path = ".";
config.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", ICON_IGFD_SAVE " Choose a File", filters, config);
```

This dialog will only verify the file in the file field, not with `GetSelection()`.

The confirmation dialog will be a non-movable modal (input blocking) dialog displayed in the middle of the current
ImGuiFileDialog window.

As usual, you can customize the dialog in your custom config file (CustomImGuiFileDialogConfig.h in this example)

Uncomment these line for customization options:

```cpp
//#define OverWriteDialogTitleString "The file Already Exist !"
//#define OverWriteDialogMessageString "Would you like to OverWrite it ?"
//#define OverWriteDialogConfirmButtonString "Confirm"
//#define OverWriteDialogCancelButtonString "Cancel"
```

See the result :

![ConfirmToOverWrite.gif](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/ConfirmToOverWrite.gif)

################################################################
## Open / Save dialog Behavior :
################################################################

ImGuiFileDialog uses the same code internally for Open and Save dialogs. To distinguish between them access the various
data return functions depending on what the dialog is doing.

When selecting an existing file (for example, a Load or Open dialog), use

```cpp
std::map<std::string, std::string> GetSelection(); // Returns selection via a map<FileName, FilePathName>
UserDatas GetUserDatas();                          // Get user data provided by the Open dialog
```

To selecting a new file (for example, a Save As... dialog), use:

```cpp
std::string GetFilePathName();                     // Returns the content of the selection field with current file
extension and current path std::string GetCurrentFileName();                  // Returns the content of the selection
field with current file extension but no path std::string GetCurrentPath();                      // Returns current path
only std::string GetCurrentFilter();                    // The file extension
```

################################################################
## Thumbnails Display
################################################################

You can now, display thumbnails of pictures.

![thumbnails.gif](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/thumbnails.gif)

The file resize use stb/image so the following files extentions are supported :
 * .png (tested sucessfully)
 * .bmp (tested sucessfully)
 * .tga (tested sucessfully)
 * .jpg (tested sucessfully)
 * .jpeg (tested sucessfully)
 * .gif (tested sucessfully_ but not animation just first frame)
 * .psd (not tested)
 * .pic (not tested)
 * .ppm (not tested)
 * .pgm (not tested)

Corresponding to your backend (ex : OpenGl) you need to define two callbacks :
* the first is a callback who will be called by ImGuiFileDialog for create the backend texture
* the second is a callback who will be called by ImGuiFileDialog for destroy the backend texture

After that you need to call the function who is responsible to create / destroy the textures.
this function must be called in your GPU Rendering zone for avoid destroying of used texture.
if you do that at the same place of your imgui code, some backend can crash your app, by ex with vulkan.

To Clarify :

This feature is spliited in two zones :
 - CPU Zone : for load/destroy picture file
 - GPU Zone : for load/destroy gpu textures.
This modern behavior for avoid destroying of used texture,
was needed for vulkan.

This feature was Successfully tested on my side with Opengl and Vulkan.
But im sure is perfectly compatible with other modern apis like DirectX and Metal

ex, for opengl :

```cpp
// Create thumbnails texture
ImGuiFileDialog::Instance()->SetCreateThumbnailCallback([](IGFD_Thumbnail_Info *vThumbnail_Info) -> void
{
    if (vThumbnail_Info &&
        vThumbnail_Info->isReadyToUpload &&
        vThumbnail_Info->textureFileDatas)
    {
        GLuint textureId = 0;
        glGenTextures(1, &textureId);
        vThumbnail_Info->textureID = (void*)textureId;

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
            (GLsizei)vThumbnail_Info->textureWidth, (GLsizei)vThumbnail_Info->textureHeight,
            0, GL_RGBA, GL_UNSIGNED_BYTE, vThumbnail_Info->textureFileDatas);
        glFinish();
        glBindTexture(GL_TEXTURE_2D, 0);

        delete[] vThumbnail_Info->textureFileDatas;
        vThumbnail_Info->textureFileDatas = nullptr;

        vThumbnail_Info->isReadyToUpload = false;
        vThumbnail_Info->isReadyToDisplay = true;
    }
});
```

```cpp
// Destroy thumbnails texture
ImGuiFileDialog::Instance()->SetDestroyThumbnailCallback([](IGFD_Thumbnail_Info* vThumbnail_Info)
{
    if (vThumbnail_Info)
    {
        GLuint texID = (GLuint)vThumbnail_Info->textureID;
        glDeleteTextures(1, &texID);
        glFinish();
    }
});
```

```cpp
// GPU Rendering Zone // To call for Create/ Destroy Textures
ImGuiFileDialog::Instance()->ManageGPUThumbnails();
```

################################################################
## Embedded in other frames :
################################################################

The dialog can be embedded in another user frame than the standard or modal dialog

You have to create a variable of type ImGuiFileDialog. (if you are using the singleton, you will not have the
possibility to open other dialog)

ex :

```cpp
ImGuiFileDialog fileDialog;

// open dialog; in this case, Place, directory creation are disabled with, and also the file input field is readonly.
// btw you can od what you want
IGFD::FileDialogConfig config;
config.path = ".";
config.countSelectionMax = -1;
config.flags = ImGuiFileDialogFlags_NoDialog |
    ImGuiFileDialogFlags_DisablePlaceMode |
    ImGuiFileDialogFlags_DisableCreateDirectoryButton |
    ImGuiFileDialogFlags_ReadOnlyFileNameField);
fileDialog.OpenDialog("embedded", "Select File", ".*", config);
// then display, here
// to note, when embedded the ImVec2(0,0) (MinSize) do nothing, only the ImVec2(0,350) (MaxSize) can size the dialog frame
fileDialog.Display("embedded", ImGuiWindowFlags_NoCollapse, ImVec2(0,0), ImVec2(0,350)))
```
the result :

![Embedded.gif](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/Embedded.gif)

################################################################
## Quick Parallel Path Selection in Path Composer
################################################################

you have a separator between two directories in the path composer
when you click on it you can explore a list of parrallels directories of this point

this feature is enabled by default
you can disable it with the flag : ImGuiFileDialogFlags_DisableQuickPathSelection

you can also customize the spacing between path button's with and without this mode
you can do that by define the compiler flag : #define CUSTOM_PATH_SPACING 2
if undefined the spacing is defined by the imgui theme

![quick_composer_path_select.gif](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/quick_composer_path_select.gif)

################################################################
## Case Insensitive Filtering
################################################################

you can use this flag 'ImGuiFileDialogFlags_CaseInsensitiveExtention' when you call the display function

```
by ex :
if the flag ImGuiFileDialogFlags_CaseInsensitiveExtention is used
with filters like .jpg or .Jpg or .JPG
all files with extentions by ex : .jpg and .JPG will be displayed
```

################################################################
## Tune the validations button group
################################################################

You can specify :
- the width of "ok" and "cancel" buttons, by the set the defines "okButtonWidth" and "cancelButtonWidth"
- the alignement of the button group (left, right, middle, etc..) by set the define "okCancelButtonAlignement"
- if you want to have the ok button on the left and cancel button on the right or inverted by set the define
"invertOkAndCancelButtons"

just see theses defines in the config file
```cpp
//Validation buttons
//#define okButtonString " OK"
//#define okButtonWidth 0.0f
//#define cancelButtonString " Cancel"
//#define cancelButtonWidth 0.0f
//alignement [0:1], 0.0 is left, 0.5 middle, 1.0 right, and other ratios
//#define okCancelButtonAlignement 0.0f
//#define invertOkAndCancelButtons false
```
with Alignement 0.0 => left

![alignement_0.0.gif](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/alignement_0.0.png)

with Alignement 1.0 => right

![alignement_1.0.gif](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/alignement_1.0.png)

with Alignement 0.5 => middle

![alignement_0.5.gif](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/alignement_0.5.png)

ok and cancel buttons inverted (cancel on the left and ok on the right)

![validation_buttons_inverted.gif](https://github.com/aiekick/ImGuiFileDialog/blob/master/doc/validation_buttons_inverted.png)

################################################################
## Regex support for Filtering and File Styling
################################################################

you can use a regex for filtering and file Styling

for have a filter recognized as a regex, you must have it between a (( and a ))

this one will filter files who start by the word "Common" and finish by ".h"
```cpp
ex : "((Custom.+[.]h))"
```

use cases :

* Simple filter :
```cpp
OpenDialog("toto", "Choose File", "((Custom.+[.]h))");
```

* Collections filter :
for this one the filter is between "{" and "}", so you can use the "(" and ")" outside

```cpp
OpenDialog("toto", "Choose File", "Source files (*.cpp *.h *.hpp){((Custom.+[.]h)),.h,.hpp}");
```

* file coloring :
this one will colorized all files who start by the word "Common" and finish by ".h"
```cpp
SetFileStyle(IGFD_FileStyleByFullName, "((Custom.+[.]h))", ImVec4(1.0f, 1.0f, 0.0f, 0.9f));
```

* with this feature you can by ex filter and colorize render frame pictures who have ext like .000, .001, .002, etc..
```cpp
OpenDialog("toto", "Choose File", "(([.][0-9]{3}))");
SetFileStyle(IGFD_FileStyleByFullName, "(([.][0-9]{3}))", ImVec4(1.0f, 1.0f, 0.0f, 0.9f));
```

################################################################
## Multi Layer / asterisk based filter
################################################################

you can add filter in the form : .a.b.c .json.cpp .vcxproj.filters

you can also add filter in the form : .* .*.* .vcx.* .*.filters .vcx*.filt.* etc..
all the * based filter are internally using regex's

################################################################
## Result Modes for GetFilePathName, getFileName and GetSelection
################################################################

you can add have various behavior when you get the file results at dialog end

you can specify a result mode to thoses function :

```cpp
GetFilePathName(IGFD_ResultMode = IGFD_ResultMode_AddIfNoFileExt)
GetFileName(IGFD_ResultMode = IGFD_ResultMode_AddIfNoFileExt)
GetFileSelection(IGFD_ResultMode = IGFD_ResultMode_KeepInputFile)
```
You can see these function who their default modes.
but you can modify them.

There is 3 Modes :
```cpp
IGFD_ResultMode_AddIfNoFileExt [DEFAULT]
This mode add the filter ext only if there is no file ext. (compatible multi layer)
ex :
   filter {.cpp,.h} with file :
     toto.h => toto.h
     toto.a.h => toto.a.h
     toto.a. => toto.a.cpp
     toto. => toto.cpp
     toto => toto.cpp
   filter {.z,.a.b} with file :
     toto.a.h => toto.a.h
     toto. => toto.z
     toto => toto.z
   filter {.g.z,.a} with file :
     toto.a.h => toto.a.h
     toto. => toto.g.z
     toto => toto.g.z

IGFD_ResultMode_OverwriteFileExt
This mode Overwrite the file extention by the current filter
This mode is the old behavior for imGuiFileDialog pre v0.6.6
ex :
   filter {.cpp,.h} with file :
     toto.h => toto.cpp
     toto.a.h => toto.a.cpp
     toto.a. => toto.a.cpp
     toto.a.h.t => toto.a.h.cpp
     toto. => toto.cpp
     toto => toto.cpp
   filter {.z,.a.b} with file :
     toto.a.h => toto.z
     toto.a.h.t => toto.a.z
     toto. => toto.z
     toto => toto.z
   filter {.g.z,.a} with file :
     toto.a.h => toto.g.z
     toto.a.h.y => toto.a.g.z
     toto.a. => toto.g.z
     toto. => toto.g.z
     toto => toto.g.z

IGFD_ResultMode_KeepInputFile
This mode keep the input file. no modification
es :
   filter {.cpp,.h} with file :
      toto.h => toto.h
      toto. => toto.
      toto => toto
   filter {.z,.a.b} with file :
      toto.a.h => toto.a.h
      toto. => toto.
      toto => toto
   filter {.g.z,.a} with file :
      toto.a.h => toto.a.h
      toto. => toto.
      toto => toto
```

to note :
  - in case of a collection of filter. the default filter will be the first.
    so in collection {.cpp,((.vcxproj.*)), .ft.*}, the default filter used for renaming will be .cpp
  - when you have multilayer filter in collection.
    we consider a filter to be replaced according to the max dot of filters for a whole collection
    a collection {.a, .b.z} is a two dots filter, so a file toto.g.z will be replaced by toto.a
    a collection {.z; .b} is a one dot filter, so a file toto.g.z will be replaced by toto.g.a

################################################################
## Custom FileSystem
################################################################

you can use your custom file system interface.

by default IGFD come with the File System Interfaces for Dirent or std::filesystem
but you have now a FileSystem interface called IFileSystem who can be overrided with your needs
by ex for android, emscripten, or boost

2 steps :

1) create a include file who must contain :
   - your override of IGFD::IFileSystem
   - a define of your class name in FILE_SYSTEM_OVERRIDE (ex : #define FILE_SYSTEM_OVERRIDE FileSystemBoost)

2) define your file system include file path in the preprocessor var "CUSTOM_FILESYSTEM_INCLUDE"
   ex : #define CUSTOM_FILESYSTEM_INCLUDE "src/FileSystemBoost.hpp"

you can check the DemoApp who is using an override for the Boost::filesystem

################################################################
## Modify file infos during scan by a callback
################################################################

in some case, it can be unsefull for modify file infos
so you can define your callback and attached it in the FileDialogConfig struct in the field userFileAttributes

the callback stamp is :
```cpp
bool (IGFD::FileInfos* vFileInfosPtr, IGFD::UserDatas vUserDatas)
```
if the callback is returning false, the file is ignored, so not displayed by the dailog

example in the gltf separated files : (see the branch DemoApp for example use)

A gltf file can have data description and datas files separated.
in this case only the file with description will be shown in the dialog, so with not the full size of all attached datas

With the file format .gltf who is containing datas in a separate .bin

syntax :
```cpp
config.userFileAttributes = [](IGFD::FileInfos* vFileInfosPtr, IGFD::UserDatas vUserDatas) -> bool {
    if (vFileInfosPtr != nullptr) {
        // this demo not take into account .gltf who have data insise. besauce keepd easy just for demo
        if (vFileInfosPtr->SearchForExt(".gltf", true)) {
            auto bin_file_path_name = vFileInfosPtr->filePath + IGFD::Utils::GetPathSeparator() + vFileInfosPtr->fileNameLevels[0] + ".bin";
            struct stat statInfos   = {};
            char timebuf[100];
            int result = stat(bin_file_path_name.c_str(), &statInfos);
            if (!result) {
                vFileInfosPtr->fileSize = (size_t)statInfos.st_size;
            } else {
                // no bin, so escaped.
                // normally we must parse the file and check the uri for get the buffer file
                // but here we keep the example as easy for demo.
                return false;
            }
        }
    }
    return true;
};
```

you can also display a tootlip for a file displayed when the mouse is over a dedicated column

you juste need to set your message for the FileDialogConfig.tooltipMessage
and specify the column in FileDialogConfig.tooltipColumn

ex code from the DemoApp branch for display the decomposition of gltf total size

syntax :
```cpp
vFileInfosPtr->tooltipMessage = toStr("%s : %s\n%s : %s",             //
    (vFileInfosPtr->fileNameLevels[0] + ".gltf").c_str(),             //
    IGFD::Utils::FormatFileSize(vFileInfosPtr->fileSize).c_str(),     //
    (vFileInfosPtr->fileNameLevels[0] + ".bin").c_str(),              //
    IGFD::Utils::FormatFileSize((size_t)statInfos.st_size).c_str());  //
vFileInfosPtr->tooltipColumn  = 1;
```

################################################################
## Places
################################################################

the Places system is a generic way for add custom links in the left side pane

you can organize them by groups

The bookmarks and devices are now groups in the left side pane.

for using it you need to
```cpp
#define USE_PLACES_FEATURE

// for have default bookmark editable groups
#define USE_PLACES_BOOKMARKS

// for have default groups for system devices (returned by the IFileSystem interface)
#define USE_PLACES_DEVICES
```

see the config file for more customization

you can also add your custom groups editable or not like what is done
the DemoApp branch with the "quick access" paths of win10

You must add a group first, then add a place to it :

```cpp
// you must add a group first, specifu display order, and say :
// if the user can add or remove palce like (bookmarks)
// if the group is opened by default
ImGuiFileDialog::Instance()->AddPlacesGroup(group_name, display_order, can_be_user_edited, opened_by_default);
// then you must get the group
auto places_ptr = ImGuiFileDialog::Instance()->GetPlacesGroupPtr(group_name);
if (places_ptr != nullptr) {
    // then add a place to the group
    // you msut specify the place name, the palce path, say if the palce can be serialized, and sepcify the style
    // for the moment the style support only the icon, can be extended if user needed in futur
    places_ptr->AddPlace(place_name, place_path, can_be_saved, style);
    // you can also add a separator
    places_ptr->AddPlaceSeparator(separator_thickness);
}
```

for editable group :
* You can select each place to edit the displayed name corresponding to a path
* Double-click on the label to apply the place

You can also serialize/deserialize groups and places (for example to load/save from/to a file):
```cpp
Load => ImGuiFileDialog::Instance()->DeserializePlaces(placesString);
Save => std::string placesString = ImGuiFileDialog::Instance()->SerializePlaces();
```

################################################################
## How to Integrate ImGuiFileDialog in your project
################################################################

### Customize ImGuiFileDialog :

You can customize many aspects of ImGuiFileDialog by overriding `ImGuiFileDialogConfig.h`.

To enable your customizations, define the preprocessor directive CUSTOM_IMGUIFILEDIALOG_CONFIG with the path of your
custom config file. This path must be relative to the directory where you put the ImGuiFileDialog module.

This operation is demonstrated in `CustomImGuiFileDialog.h` in the example project to:

* Have a custom icon font instead of labels for buttons or message titles
* Customize the button text (the button call signature must be the same, by the way! :)

The custom icon font used in the example code ([CustomFont.cpp](CustomFont.cpp) and [CustomFont.h](CustomFont.h)) was
made with [ImGuiFontStudio](https://github.com/aiekick/ImGuiFontStudio), which I wrote. :)

ImGuiFontStudio uses ImGuiFileDialog! Check it out.

################################################################
## Api's C/C++ :
################################################################

### the C Api

this api was sucessfully tested with CImGui

A C API is available let you include ImGuiFileDialog in your C project.
btw, ImGuiFileDialog depend of ImGui and dirent (for windows)

Sample code with cimgui :

```cpp
// create ImGuiFileDialog
ImGuiFileDialog *cfileDialog = IGFD_Create();

// open dialog
if (igButton("Open File", buttonSize)) {
    IGFD_FileDialog_Config config = IGFD_FileDialog_Config_Get();
    config.path = ".";
    config.flags = ImGuiFileDialogFlags_ConfirmOverwrite; // ImGuiFileDialogFlags
    IGFD_OpenDialog(cfiledialog,
        "filedlg",                              // dialog key (make it possible to have different treatment reagrding the dialog key
        "Open a File",                          // dialog title
        "c files(*.c *.h){.c,.h}",              // dialog filter syntax : simple => .h,.c,.pp, etc and collections : text1{filter0,filter1,filter2}, text2{filter0,filter1,filter2}, etc..
        config); 								// the file dialog config
}

ImGuiIO* ioptr = igGetIO();
ImVec2 maxSize;
maxSize.x = ioptr->DisplaySize.x * 0.8f;
maxSize.y = ioptr->DisplaySize.y * 0.8f;
ImVec2 minSize;
minSize.x = maxSize.x * 0.25f;
minSize.y = maxSize.y * 0.25f;

// display dialog
if (IGFD_DisplayDialog(cfiledialog, "filedlg", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
{
    if (IGFD_IsOk(cfiledialog)) // result ok
    {
        char* cfilePathName = IGFD_GetFilePathName(cfiledialog);
        printf("GetFilePathName : %s\n", cfilePathName);
        char* cfilePath = IGFD_GetCurrentPath(cfiledialog);
        printf("GetCurrentPath : %s\n", cfilePath);
        char* cfilter = IGFD_GetCurrentFilter(cfiledialog);
        printf("GetCurrentFilter : %s\n", cfilter);
        // here convert from string because a string was passed as a userDatas, but it can be what you want
        void* cdatas = IGFD_GetUserDatas(cfiledialog);
        if (cdatas)
            printf("GetUserDatas : %s\n", (const char*)cdatas);
        struct IGFD_Selection csel = IGFD_GetSelection(cfiledialog); // multi selection
        printf("Selection :\n");
        for (int i = 0; i < (int)csel.count; i++)
        {
            printf("(%i) FileName %s => path %s\n", i, csel.table[i].fileName, csel.table[i].filePathName);
        }
        // action

        // destroy
        if (cfilePathName) free(cfilePathName);
        if (cfilePath) free(cfilePath);
        if (cfilter) free(cfilter);

        IGFD_Selection_DestroyContent(&csel);
    }
    IGFD_CloseDialog(cfiledialog);
}

// destroy ImGuiFileDialog
IGFD_Destroy(cfiledialog);
```
################################################################
################################################################

Thats all.

You can check by example in this repo with the file CustomImGuiFileDialogConfig.h :
- this trick was used for have custom icon font instead of labels for buttons or messages titles
- you can also use your custom imgui button, the button call stamp must be same by the way :)

The Custom Icon Font (in CustomFont.cpp and CustomFont.h) was made with ImGuiFontStudio
(https://github.com/aiekick/ImGuiFontStudio) i wrote for that :) ImGuiFontStudio is using also ImGuiFileDialog.

################################################################
################################################################
*/

#pragma endregion

#pragma region IGFD VERSION

// compatible with 1.90.4 WIP
#define IMGUIFILEDIALOG_VERSION "v0.6.7"

#pragma endregion

#pragma region CONFIG FILE INCLUSION

#ifndef CUSTOM_IMGUIFILEDIALOG_CONFIG
#include "ImGuiFileDialogConfig.h"
#else  // CUSTOM_IMGUIFILEDIALOG_CONFIG
#include CUSTOM_IMGUIFILEDIALOG_CONFIG
#endif  // CUSTOM_IMGUIFILEDIALOG_CONFIG

#pragma endregion

#pragma region FLAGS : FileStyleFlags

// file style enum for file display (color, icon, font)
typedef int IGFD_FileStyleFlags;  // -> enum IGFD_FileStyleFlags_
enum IGFD_FileStyleFlags_         // by evaluation / priority order
{
    IGFD_FileStyle_None = 0,                         // define none style
    IGFD_FileStyleByTypeFile = (1 << 0),             // define style for all files
    IGFD_FileStyleByTypeDir = (1 << 1),              // define style for all dir
    IGFD_FileStyleByTypeLink = (1 << 2),             // define style for all link
    IGFD_FileStyleByExtention = (1 << 3),            // define style by extention, for files or links
    IGFD_FileStyleByFullName = (1 << 4),             // define style for particular file/dir/link full name (filename + extention)
    IGFD_FileStyleByContainedInFullName = (1 << 5),  // define style for file/dir/link when criteria is contained in full name
};

#pragma endregion

#pragma region FLAGS : ImGuiFileDialogFlags

typedef int ImGuiFileDialogFlags;  // -> enum ImGuiFileDialogFlags_
enum ImGuiFileDialogFlags_ {
    ImGuiFileDialogFlags_None = 0,                                 // define none default flag
    ImGuiFileDialogFlags_ConfirmOverwrite = (1 << 0),              // show confirm to overwrite dialog
    ImGuiFileDialogFlags_DontShowHiddenFiles = (1 << 1),           // dont show hidden file (file starting with a .)
    ImGuiFileDialogFlags_DisableCreateDirectoryButton = (1 << 2),  // disable the create directory button
    ImGuiFileDialogFlags_HideColumnType = (1 << 3),                // hide column file type
    ImGuiFileDialogFlags_HideColumnSize = (1 << 4),                // hide column file size
    ImGuiFileDialogFlags_HideColumnDate = (1 << 5),                // hide column file date
    ImGuiFileDialogFlags_NoDialog = (1 << 6),                      // let the dialog embedded in your own imgui begin / end scope
    ImGuiFileDialogFlags_ReadOnlyFileNameField = (1 << 7),         // don't let user type in filename field for file open style dialogs
    ImGuiFileDialogFlags_CaseInsensitiveExtention = (1 << 8),      // the file extentions treatments will not take into account the case
    ImGuiFileDialogFlags_Modal = (1 << 9),                         // modal
    ImGuiFileDialogFlags_DisableThumbnailMode = (1 << 10),         // disable the thumbnail mode
    ImGuiFileDialogFlags_DisablePlaceMode = (1 << 11),          // disable the place mode
    ImGuiFileDialogFlags_DisableQuickPathSelection = (1 << 12),    // disable the quick path selection

    // default behavior when no flags is defined. seems to be the more common cases
    ImGuiFileDialogFlags_Default = ImGuiFileDialogFlags_ConfirmOverwrite |  //
                                   ImGuiFileDialogFlags_Modal |             //
                                   ImGuiFileDialogFlags_HideColumnType
};

// flags used for GetFilePathName(flag) or GetSelection(flag)
typedef int IGFD_ResultMode;  // -> enum IGFD_ResultMode_
enum IGFD_ResultMode_ {
    // IGFD_ResultMode_AddIfNoFileExt
    // add the file ext only if there is no file ext
    //   filter {.cpp,.h} with file :
    //     toto.h => toto.h
    //     toto.a.h => toto.a.h
    //     toto.a. => toto.a.cpp
    //     toto. => toto.cpp
    //     toto => toto.cpp
    //   filter {.z,.a.b} with file :
    //     toto.a.h => toto.a.h
    //     toto. => toto.z
    //     toto => toto.z
    //   filter {.g.z,.a} with file :
    //     toto.a.h => toto.a.h
    //     toto. => toto.g.z
    //     toto => toto.g.z
    IGFD_ResultMode_AddIfNoFileExt = 0,

    // IGFD_ResultMode_OverwriteFileExt
    // Overwrite the file extention by the current filter :
    //   filter {.cpp,.h} with file :
    //     toto.h => toto.cpp
    //     toto.a.h => toto.a.cpp
    //     toto.a. => toto.a.cpp
    //     toto.a.h.t => toto.a.h.cpp
    //     toto. => toto.cpp
    //     toto => toto.cpp
    //   filter {.z,.a.b} with file :
    //     toto.a.h => toto.z
    //     toto.a.h.t => toto.a.z
    //     toto. => toto.z
    //     toto => toto.z
    //   filter {.g.z,.a} with file :
    //     toto.a.h => toto.g.z
    //     toto.a.h.y => toto.a.g.z
    //     toto.a. => toto.g.z
    //     toto. => toto.g.z
    //     toto => toto.g.z
    IGFD_ResultMode_OverwriteFileExt = 1,  // behavior pre IGFD v0.6.6

    // IGFD_ResultMode_KeepInputFile
    // keep the input file => no modification :
    //   filter {.cpp,.h} with file :
    //      toto.h => toto.h
    //      toto. => toto.
    //      toto => toto
    //   filter {.z,.a.b} with file :
    //      toto.a.h => toto.a.h
    //      toto. => toto.
    //      toto => toto
    //   filter {.g.z,.a} with file :
    //      toto.a.h => toto.a.h
    //      toto. => toto.
    //      toto => toto
    IGFD_ResultMode_KeepInputFile = 2
};

#pragma endregion

#pragma region Common Cpp& C Structures

#ifdef USE_THUMBNAILS
struct IGFD_Thumbnail_Info {
    int isReadyToDisplay = 0;             // ready to be rendered, so texture created
    int isReadyToUpload = 0;              // ready to upload to gpu
    int isLoadingOrLoaded = 0;            // was sent to laoding or loaded
    int textureWidth = 0;                 // width of the texture to upload
    int textureHeight = 0;                // height of the texture to upload
    int textureChannels = 0;              // count channels of the texture to upload
    unsigned char* textureFileDatas = 0;  // file texture datas, will be rested to null after gpu upload
    void* textureID = 0;                  // 2d texture id (void* is like ImtextureID type) (GL, DX, VK, Etc..)
    void* userDatas = 0;                  // user datas
};
#endif  // USE_THUMBNAILS

#pragma endregion

#ifdef __cplusplus

#pragma region Includes

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif  // IMGUI_DEFINE_MATH_OPERATORS

#ifdef IMGUI_INCLUDE
#include IMGUI_INCLUDE
#else  // IMGUI_INCLUDE
#include <imgui.h>
#endif  // IMGUI_INCLUDE

#include <set>
#include <map>
#include <list>
#include <regex>
#include <array>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <cfloat>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <functional>
#include <unordered_map>

#pragma endregion

#pragma region IGFD API

// Define attributes of all API symbols declarations (e.g. for DLL under Windows)
// Using ImGuiFileDialog via a shared library is not recommended, because we don't guarantee
// backward nor forward ABI compatibility and also function call overhead. If you
// do use ImGuiFileDialog as a DLL, be sure to call SetImGuiContext (see Miscellanous section).

#ifndef IGFD_API
#define IGFD_API
#endif  // IGFD_API

#pragma endregion

#pragma region Defines

#ifndef defaultSortField
#define defaultSortField FIELD_FILENAME
#endif  // defaultSortField

#ifndef defaultSortOrderFilename
#define defaultSortOrderFilename true
#endif  // defaultSortOrderFilename
#ifndef defaultSortOrderType
#define defaultSortOrderType true
#endif  // defaultSortOrderType
#ifndef defaultSortOrderSize
#define defaultSortOrderSize true
#endif  // defaultSortOrderSize
#ifndef defaultSortOrderDate
#define defaultSortOrderDate true
#endif  // defaultSortOrderDate
#ifndef defaultSortOrderThumbnails
#define defaultSortOrderThumbnails true
#endif  // defaultSortOrderThumbnails

#ifndef MAX_FILE_DIALOG_NAME_BUFFER
#define MAX_FILE_DIALOG_NAME_BUFFER 1024
#endif  // MAX_FILE_DIALOG_NAME_BUFFER

#ifndef MAX_PATH_BUFFER_SIZE
#define MAX_PATH_BUFFER_SIZE 1024
#endif  // MAX_PATH_BUFFER_SIZE

#ifndef EXT_MAX_LEVEL
#define EXT_MAX_LEVEL 10U
#endif  // EXT_MAX_LEVEL

#pragma endregion

#pragma region IGFD NAMESPACE

namespace IGFD {

#pragma region INTERNAL

#pragma region SEARCHABLE VECTOR

template <typename T>
class SearchableVector {
private:
    std::unordered_map<T, size_t> m_Dico;
    std::vector<T> m_Array;

public:
    void clear() {
        m_Dico.clear();
        m_Array.clear();
    }

    bool empty() const {
        return m_Array.empty();
    }
    size_t size() const {
        return m_Array.size();
    }
    T& operator[](const size_t& vIdx) {
        return m_Array[vIdx];
    }
    T& at(const size_t& vIdx) {
        return m_Array.at(vIdx);
    }
    typename std::vector<T>::iterator begin() {
        return m_Array.begin();
    }
    typename std::vector<T>::const_iterator begin() const {
        return m_Array.begin();
    }
    typename std::vector<T>::iterator end() {
        return m_Array.end();
    }
    typename std::vector<T>::const_iterator end() const {
        return m_Array.end();
    }

    bool try_add(T vKey) {
        if (!exist(vKey)) {
            m_Dico[vKey] = m_Array.size();
            m_Array.push_back(vKey);
            return true;
        }
        return false;
    }

    bool try_set_existing(T vKey) {
        if (exist(vKey)) {
            auto row = m_Dico.at(vKey);
            m_Array[row] = vKey;
            return true;
        }
        return false;
    }

    bool exist(const std::string& vKey) const {
        return (m_Dico.find(vKey) != m_Dico.end());
    }
};

#pragma endregion

#pragma region Utils

class IGFD_API Utils {
public:
    struct PathStruct {
        std::string path;
        std::string name;
        std::string ext;
        bool isOk = false;
    };

public:
    static bool ImSplitter(
        bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);
    static bool ReplaceString(std::string& str, const std::string& oldStr, const std::string& newStr, const size_t& vMaxRecursion = 10U);
    static void AppendToBuffer(char* vBuffer, size_t vBufferLen, const std::string& vStr);
    static void ResetBuffer(char* vBuffer);
    static void SetBuffer(char* vBuffer, size_t vBufferLen, const std::string& vStr);
    static std::string UTF8Encode(const std::wstring& wstr);
    static std::wstring UTF8Decode(const std::string& str);
    static std::vector<std::string> SplitStringToVector(const std::string& vText, const std::string& vDelimiterPattern, const bool& vPushEmpty);
    static std::vector<std::string> SplitStringToVector(const std::string& vText, const char& vDelimiter, const bool& vPushEmpty);
    static std::string LowerCaseString(const std::string& vString);  // turn all text in lower case for search facilitie
    static size_t GetCharCountInString(const std::string& vString, const char& vChar);
    static size_t GetLastCharPosWithMinCharCount(const std::string& vString, const char& vChar, const size_t& vMinCharCount);
    static std::string GetPathSeparator(); // return the slash for any OS ( \\ win, / unix)
    static std::string RoundNumber(double vvalue, int n);  // custom rounding number
    static std::string FormatFileSize(size_t vByteSize);   // format file size field
};

#pragma endregion

#pragma region FileStyle

class IGFD_API FileInfos;
class IGFD_API FileStyle {
public:
    typedef std::function<bool(const FileInfos&, FileStyle&)> FileStyleFunctor;

public:
    ImVec4 color = ImVec4(0, 0, 0, 0);
    std::string icon;
    ImFont* font = nullptr;
    IGFD_FileStyleFlags flags = 0;

public:
    FileStyle();
    FileStyle(const FileStyle& vStyle);
    FileStyle(const ImVec4& vColor, const std::string& vIcon = "", ImFont* vFont = nullptr);
};

#pragma endregion

#pragma region SearchManager

class IGFD_API FileDialogInternal;
class IGFD_API SearchManager {
public:
    std::string searchTag;
    char searchBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";
    bool searchInputIsActive = false;

public:
    void Clear();                                                 // clear datas
    void DrawSearchBar(FileDialogInternal& vFileDialogInternal);  // draw the search bar
};

#pragma endregion

#pragma region FilterInfos

class IGFD_API FilterInfos {
private:
    // just for return a default const std::string& in getFirstFilter.
    // cannot be const, because FilterInfos must be affected to an another FilterInfos
    // must stay empty alla time
    std::string empty_string;

public:
    std::string title;                                // displayed filter.can be different than rela filter
    SearchableVector<std::string> filters;            // filters
    SearchableVector<std::string> filters_optimized;  // optimized filters for case insensitive search
    std::vector<std::regex> filters_regex;            // collection of regex filter type
    size_t count_dots = 0U;                           // the max count dot the max per filter of all filters

public:
    void clear();                                                                 // clear the datas
    bool empty() const;                                                           // is filter empty
    const std::string& getFirstFilter() const;                                    // get the first filter
    bool regexExist(const std::string& vFilter) const;                            // is regex filter exist
    bool exist(const FileInfos& vFileInfos, bool vIsCaseInsensitive) const;       // is filter exist
    void setCollectionTitle(const std::string& vTitle);                           // set the collection title
    void addFilter(const std::string& vFilter, const bool& vIsRegex);             // add a filter
    void addCollectionFilter(const std::string& vFilter, const bool& vIsRegex);   // add a filter in collection
    std::string transformAsteriskBasedFilterToRegex(const std::string& vFilter);  // will transform a filter who contain * to a regex
};

#pragma endregion

#pragma region FilterManager

class IGFD_API FileInfos;
class IGFD_API FilterManager {
#ifdef NEED_TO_BE_PUBLIC_FOR_TESTS
public:
#else
private:
#endif
    std::vector<FilterInfos> m_ParsedFilters;
    std::unordered_map<IGFD_FileStyleFlags, std::unordered_map<std::string, std::shared_ptr<FileStyle>>> m_FilesStyle;  // file infos for file
                                                                                                                        // extention only
    std::vector<FileStyle::FileStyleFunctor> m_FilesStyleFunctors;  // file style via lambda function
    FilterInfos m_SelectedFilter;

public:
    std::string dLGFilters;
    std::string dLGdefaultExt;

public:
    const FilterInfos& GetSelectedFilter() const;
    void ParseFilters(const char* vFilters);                            // Parse filter syntax, detect and parse filter collection
    void SetSelectedFilterWithExt(const std::string& vFilter);          // Select filter
    bool m_FillFileStyle(std::shared_ptr<FileInfos> vFileInfos) const;  // fill with the good style
    void SetFileStyle(const IGFD_FileStyleFlags& vFlags, const char* vCriteria, const FileStyle& vInfos);  // Set FileStyle
    void SetFileStyle(const IGFD_FileStyleFlags& vFlags,
        const char* vCriteria,
        const ImVec4& vColor,
        const std::string& vIcon,
        ImFont* vFont);                                       // link file style to Color and Icon and Font
    void SetFileStyle(FileStyle::FileStyleFunctor vFunctor);  // lambda functor for set file style.
    bool GetFileStyle(const IGFD_FileStyleFlags& vFlags,
        const std::string& vCriteria,
        ImVec4* vOutColor,
        std::string* vOutIcon,
        ImFont** vOutFont);  // Get Color and Icon for Filter
    void ClearFilesStyle();  // clear m_FileStyle
    bool IsCoveredByFilters(const FileInfos& vFileInfos,
        bool vIsCaseInsensitive) const;    // check if current file extention (vExt) is covered by current filter, or by regex (vNameExt)
    float GetFilterComboBoxWidth() const;  // will return the current combo box widget width
    bool DrawFilterComboBox(FileDialogInternal& vFileDialogInternal);  // draw the filter combobox 	// get the current selected filter
    std::string ReplaceExtentionWithCurrentFilterIfNeeded(const std::string& vFileName,
        IGFD_ResultMode vFlag) const;     // replace the extention of the current file by the selected filter
    void SetDefaultFilterIfNotDefined();  // define the first filter if no filter is selected
};

#pragma endregion

#pragma region FileType

class IGFD_API FileType {
public:
    enum class ContentType {
        // The ordering will be used during sort.
        Invalid = -1,
        Directory = 0,
        File = 1,
        LinkToUnknown = 2,  // link to something that is not a regular file or directory.
    };

private:
    ContentType m_Content = ContentType::Invalid;
    bool m_Symlink = false;

public:
    FileType();
    FileType(const ContentType& vContentType, const bool& vIsSymlink);

    void SetContent(const ContentType& vContentType);
    void SetSymLink(const bool& vIsSymlink);

    bool isValid() const;
    bool isDir() const;
    bool isFile() const;
    bool isLinkToUnknown() const;
    bool isSymLink() const;

    // Comparisons only care about the content type, ignoring whether it's a symlink or not.
    bool operator==(const FileType& rhs) const;
    bool operator!=(const FileType& rhs) const;
    bool operator<(const FileType& rhs) const;
    bool operator>(const FileType& rhs) const;
};

#pragma endregion

#pragma region FileInfos

class IGFD_API FileInfos {
public:
    // extention of the file, the array is the levels of ext, by ex : .a.b.c, will be save in {.a.b.c, .b.c, .c}
    // 10 level max are sufficient i guess. the others levels will be checked if countExtDot > 1
    std::array<std::string, EXT_MAX_LEVEL> fileExtLevels;
    std::array<std::string, EXT_MAX_LEVEL> fileExtLevels_optimized;  // optimized for search => insensitivecase
    // same for file name, can be sued in userFileAttributesFun
    std::array<std::string, EXT_MAX_LEVEL> fileNameLevels;
    std::array<std::string, EXT_MAX_LEVEL> fileNameLevels_optimized;  // optimized for search => insensitivecase
    size_t countExtDot = 0U;                                         // count dots in file extention. this count will give the levels in fileExtLevels
    FileType fileType;                                               // fileType
    std::string filePath;                                            // path of the file
    std::string fileName;                                            // file name only
    std::string fileNameExt;                                         // filename of the file (file name + extention) (but no path)
    std::string fileNameExt_optimized;                               // optimized for search => insensitivecase
    std::string deviceInfos;                                         // quick infos to display after name for devices
    std::string tooltipMessage;                                      // message to display on the tooltip, is not empty
    int32_t tooltipColumn = -1;                                      // the tooltip will appears only when the mouse is over the tooltipColumn if > -1
    size_t fileSize = 0U;                                            // for sorting operations
    std::string formatedFileSize;                                    // file size formated (10 o, 10 ko, 10 mo, 10 go)
    std::string fileModifDate;                                       // file user defined format of the date (data + time by default)
    std::shared_ptr<FileStyle> fileStyle = nullptr;                  // style of the file
#ifdef USE_THUMBNAILS
    IGFD_Thumbnail_Info thumbnailInfo;  // structre for the display for image file tetxure
#endif                                  // USE_THUMBNAILS

public:
    bool SearchForTag(const std::string& vTag) const;  // will search a tag in fileNameExt and fileNameExt_optimized
    bool SearchForExt(const std::string& vExt,
        const bool& vIsCaseInsensitive,
        const size_t& vMaxLevel = EXT_MAX_LEVEL) const;  // will check the fileExtLevels levels for vExt, until vMaxLevel
    bool SearchForExts(const std::string& vComaSepExts,
        const bool& vIsCaseInsensitive,
        const size_t& vMaxLevel = EXT_MAX_LEVEL) const;  // will check the fileExtLevels levels for vExts (ext are coma separated), until vMaxLevel
    bool FinalizeFileTypeParsing(const size_t& vMaxDotToExtract);  // finalize the parsing the file (only a file or link to file. no dir)
};

#pragma endregion

#pragma region FILE SYSTEM INTERFACE

typedef std::pair<std::string, std::string> PathDisplayedName;

class IFileSystem {
public:
    virtual ~IFileSystem() = default;
    // say if a directory can be openened or for any reason locked
    virtual bool IsDirectoryCanBeOpened(const std::string& vName) = 0;
    // say if a directory exist
    virtual bool IsDirectoryExist(const std::string& vName) = 0;
    // say if a file exist
    virtual bool IsFileExist(const std::string& vName) = 0;
    // say if a directory was created, return false if vName is invalid or alreayd exist
    virtual bool CreateDirectoryIfNotExist(const std::string& vName) = 0;
    // extract the component of a file path name, like path, name, ext
    virtual IGFD::Utils::PathStruct ParsePathFileName(const std::string& vPathFileName) = 0;
    // will return a list of files inside a path
    virtual std::vector<IGFD::FileInfos> ScanDirectory(const std::string& vPath) = 0;
    // say if the path is well a directory
    virtual bool IsDirectory(const std::string& vFilePathName) = 0;
    // return a device list (<path, device name>) on windows, but can be used on other platforms for give to the user a list of devices paths.
    virtual std::vector<IGFD::PathDisplayedName> GetDevicesList() = 0;
};

#pragma endregion

#pragma region FileManager

class IGFD_API FileManager {
public:                            // types
    enum class SortingFieldEnum {  // sorting for filetering of the file lsit
        FIELD_NONE = 0,            // no sorting reference, result indetermined haha..
        FIELD_FILENAME,            // sorted by filename
        FIELD_TYPE,                // sorted by filetype
        FIELD_SIZE,                // sorted by filesize (formated file size)
        FIELD_DATE,                // sorted by filedate
        FIELD_THUMBNAILS,          // sorted by thumbnails (comparaison by width then by height)
    };

#ifdef NEED_TO_BE_PUBLIC_FOR_TESTS
public:
#else
private:
#endif
    std::string m_CurrentPath;                                   // current path (to be decomposed in m_CurrentPathDecomposition
    std::vector<std::string> m_CurrentPathDecomposition;         // part words
    std::vector<std::shared_ptr<FileInfos>> m_FileList;          // base container
    std::vector<std::shared_ptr<FileInfos>> m_FilteredFileList;  // filtered container (search, sorting, etc..)
    std::vector<std::shared_ptr<FileInfos>> m_PathList;          // base container for path selection
    std::vector<std::shared_ptr<FileInfos>> m_FilteredPathList;  // filtered container for path selection (search, sorting, etc..)
    std::vector<std::string>::iterator m_PopupComposedPath;      // iterator on m_CurrentPathDecomposition for Current Path popup
    std::string m_LastSelectedFileName;                          // for shift multi selection
    std::set<std::string> m_SelectedFileNames;                   // the user selection of FilePathNames
    bool m_CreateDirectoryMode = false;                          // for create directory widget
    std::string m_FileSystemName;
    std::unique_ptr<IFileSystem> m_FileSystemPtr = nullptr;

public:
    bool inputPathActivated = false;                             // show input for path edition
    bool drivesClicked = false;                                  // event when a drive button is clicked
    bool puPathClicked = false;                                  // event when a path button was clicked
    char inputPathBuffer[MAX_PATH_BUFFER_SIZE] = "";             // input path buffer for imgui widget input text (displayed in palce of composer)
    char variadicBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";       // called by m_SelectableItem
    char fileNameBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";       // file name buffer in footer for imgui widget input text
    char directoryNameBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";  // directory name buffer (when in directory mode)
    std::string headerFileName;                                  // detail view name of column file
    std::string headerFileType;                                  // detail view name of column type
    std::string headerFileSize;                                  // detail view name of column size
    std::string headerFileDate;  // detail view name of column date + time
#ifdef USE_THUMBNAILS
    std::string headerFileThumbnails;  // detail view name of column thumbnails
    bool sortingDirection[5] = {       // true => Ascending, false => Descending
        defaultSortOrderFilename, defaultSortOrderType, defaultSortOrderSize, defaultSortOrderDate, defaultSortOrderThumbnails};
#else
    bool sortingDirection[4] = {  // true => Ascending, false => Descending
        defaultSortOrderFilename, defaultSortOrderType, defaultSortOrderSize, defaultSortOrderDate};
#endif
    SortingFieldEnum sortingField = SortingFieldEnum::FIELD_FILENAME;  // detail view sorting column

    std::string dLGpath;               // base path set by user when OpenDialog was called
    std::string dLGDefaultFileName;    // base default file path name set by user when OpenDialog was called
    size_t dLGcountSelectionMax = 1U;  // 0 for infinite				// base max selection count set by user when OpenDialog was called
    bool dLGDirectoryMode = false;     // is directory mode (defiend like : dLGDirectoryMode = (filters.empty()))

    std::string fsRoot;

#ifdef NEED_TO_BE_PUBLIC_FOR_TESTS
public:
#else
private:
#endif
    static void m_CompleteFileInfos(const std::shared_ptr<FileInfos>& vInfos);     // set time and date infos of a file (detail view mode)
    void m_RemoveFileNameInSelection(const std::string& vFileName);                // selection : remove a file name
    void m_m_AddFileNameInSelection(const std::string& vFileName, bool vSetLastSelectionFileName);  // selection : add a file name
    void m_AddFile(const FileDialogInternal& vFileDialogInternal,
        const std::string& vPath,
        const std::string& vFileName,
        const FileType& vFileType);  // add file called by scandir
    void m_AddPath(const FileDialogInternal& vFileDialogInternal,
        const std::string& vPath,
        const std::string& vFileName,
        const FileType& vFileType);  // add file called by scandir
    void m_ScanDirForPathSelection(const FileDialogInternal& vFileDialogInternal,
        const std::string& vPath);  // scan the directory for retrieve the path list
    void m_OpenPathPopup(const FileDialogInternal& vFileDialogInternal,
        std::vector<std::string>::iterator vPathIter);  // open the popup list of paths
    void m_SetCurrentPath(std::vector<std::string>::iterator vPathIter);  // set the current path, update the path bar
    void m_ApplyFilteringOnFileList(const FileDialogInternal& vFileDialogInternal,
        std::vector<std::shared_ptr<FileInfos>>& vFileInfosList,
        std::vector<std::shared_ptr<FileInfos>>& vFileInfosFilteredList);
    void m_SortFields(const FileDialogInternal& vFileDialogInternal,
        std::vector<std::shared_ptr<FileInfos>>& vFileInfosList,
        std::vector<std::shared_ptr<FileInfos>>& vFileInfosFilteredList);  // will sort a column
    bool m_CompleteFileInfosWithUserFileAttirbutes(const FileDialogInternal& vFileDialogInternal, const std::shared_ptr<FileInfos>& vInfos);

public:
    FileManager();
    bool IsComposerEmpty();
    size_t GetComposerSize();
    bool IsFileListEmpty();
    bool IsPathListEmpty();
    bool IsFilteredListEmpty();
    bool IsPathFilteredListEmpty();
    size_t GetFullFileListSize();
    std::shared_ptr<FileInfos> GetFullFileAt(size_t vIdx);
    size_t GetFilteredListSize();
    size_t GetPathFilteredListSize();
    std::shared_ptr<FileInfos> GetFilteredFileAt(size_t vIdx);
    std::shared_ptr<FileInfos> GetFilteredPathAt(size_t vIdx);
    std::vector<std::string>::iterator GetCurrentPopupComposedPath();
    bool IsFileNameSelected(const std::string& vFileName);
    std::string GetBack();
    void ClearComposer();
    void ClearFileLists();  // clear file list, will destroy thumbnail textures
    void ClearPathLists();  // clear path list, will destroy thumbnail textures
    void ClearAll();
    void ApplyFilteringOnFileList(const FileDialogInternal& vFileDialogInternal);
    void SortFields(const FileDialogInternal& vFileDialogInternal);        // will sort a column
    void OpenCurrentPath(const FileDialogInternal& vFileDialogInternal);   // set the path of the dialog, will launch the
                                                                           // scandir for populate the file listview
    bool GetDrives();                                                      // list drives on windows platform
    bool CreateDir(const std::string& vPath);                              // create a directory on the file system
    std::string ComposeNewPath(std::vector<std::string>::iterator vIter);  // compose a path from the compose path widget
    bool SetPathOnParentDirectoryIfAny();                                  // compose paht on parent directory
    std::string GetCurrentPath();                                          // get the current path
    void SetCurrentPath(const std::string& vCurrentPath);                  // set the current path
    void SetDefaultFileName(const std::string& vFileName);
    bool SelectDirectory(const std::shared_ptr<FileInfos>& vInfos);  // enter directory
    void SelectFileName(const FileDialogInternal& vFileDialogInternal,
        const std::shared_ptr<FileInfos>& vInfos);  // select filename
    void SetCurrentDir(const std::string& vPath);   // define current directory for scan
    void ScanDir(const FileDialogInternal& vFileDialogInternal,
        const std::string& vPath);  // scan the directory for retrieve the file list

    std::string GetResultingPath();
    std::string GetResultingFileName(FileDialogInternal& vFileDialogInternal, IGFD_ResultMode vFlag);
    std::string GetResultingFilePathName(FileDialogInternal& vFileDialogInternal, IGFD_ResultMode vFlag);
    std::map<std::string, std::string> GetResultingSelection(FileDialogInternal& vFileDialogInternal, IGFD_ResultMode vFlag);

    void DrawDirectoryCreation(const FileDialogInternal& vFileDialogInternal);  // draw directory creation widget
    void DrawPathComposer(const FileDialogInternal& vFileDialogInternal); 

    IFileSystem* GetFileSystemInstance() {
        return m_FileSystemPtr.get();
    }
    const std::string& GetFileSystemName() {
        return m_FileSystemName;
    }
};

#pragma endregion

#pragma region FileDialogInternal

typedef void* UserDatas;
typedef std::function<void(const char*, UserDatas, bool*)> PaneFun;  // side pane function binding
typedef std::function<bool(FileInfos*, UserDatas)> UserFileAttributesFun;  // custom file Attributes call back, reject file if false

struct IGFD_API FileDialogConfig {
    std::string path;                                        // path
    std::string fileName;                                    // defaut file name
    std::string filePathName;                                // if not empty, the filename and the path will be obtained from filePathName
    int32_t countSelectionMax  = 1;                          // count selection max
    UserDatas userDatas        = nullptr;                    // user datas (can be retrieved in pane)
    ImGuiFileDialogFlags flags = ImGuiFileDialogFlags_None;  // ImGuiFileDialogFlags
    PaneFun sidePane;                                        // side pane callback
    float sidePaneWidth = 250.0f;                            // side pane width
    UserFileAttributesFun userFileAttributes;                // user file Attibutes callback
};

class IGFD_API FileDialogInternal {
public:
    FileManager fileManager;      // the file manager
    FilterManager filterManager;  // the filter manager
    SearchManager searchManager;  // the search manager

public:
    std::string name;                                           // the internal dialog name (title + ##word)
    bool showDialog = false;                                    // the dialog is shown
    ImVec2 dialogCenterPos = ImVec2(0, 0);                      // center pos for display the confirm overwrite dialog
    int lastImGuiFrameCount = 0;                                // to be sure than only one dialog displayed per frame
    float footerHeight = 0.0f;                                  // footer height
    bool canWeContinue = true;                                  // events
    bool okResultToConfirm = false;                             // to confim if ok for OverWrite
    bool isOk = false;                                          // is dialog ok button click
    bool fileInputIsActive = false;                             // when input text for file or directory is active
    bool fileListViewIsActive = false;                          // when list view is active
    std::string dLGkey;                                         // the dialog key
    std::string dLGtitle;                                       // the dialog title
    bool needToExitDialog = false;                              // we need to exit the dialog
    bool puUseCustomLocale = false;                             // custom user locale
    int localeCategory = LC_ALL;                                // locale category to use
    std::string localeBegin;                                    // the locale who will be applied at start of the display dialog
    std::string localeEnd;                                      // the locale who will be applaied at end of the display dialog

private:
    FileDialogConfig m_DialogConfig;

public:
    void NewFrame();           // new frame, so maybe neded to do somethings, like reset events
    void EndFrame();           // end frame, so maybe neded to do somethings fater all
    void ResetForNewDialog();  // reset what is needed to reset for the openging of a new dialog

    void configureDialog(                  // open simple dialog
        const std::string& vKey,           // key dialog
        const std::string& vTitle,         // title
        const char* vFilters,              // filters, if null, will display only directories
        const FileDialogConfig& vConfig);  // FileDialogConfig
    const FileDialogConfig& getDialogConfig() const;
    FileDialogConfig& getDialogConfigRef();
};

#pragma endregion

#pragma endregion

#pragma region Optional Features

#pragma region ThumbnailFeature

#ifdef USE_THUMBNAILS
typedef std::function<void(IGFD_Thumbnail_Info*)> CreateThumbnailFun;   // texture 2d creation function binding
typedef std::function<void(IGFD_Thumbnail_Info*)> DestroyThumbnailFun;  // texture 2d destroy function binding
#endif
class IGFD_API ThumbnailFeature {
protected:
    ThumbnailFeature();
    ~ThumbnailFeature();

    void m_NewThumbnailFrame(FileDialogInternal& vFileDialogInternal);
    void m_EndThumbnailFrame(FileDialogInternal& vFileDialogInternal);
    void m_QuitThumbnailFrame(FileDialogInternal& vFileDialogInternal);

#ifdef USE_THUMBNAILS
protected:
    enum class DisplayModeEnum { FILE_LIST = 0, THUMBNAILS_LIST, THUMBNAILS_GRID };

private:
    uint32_t m_CountFiles = 0U;
    bool m_IsWorking = false;
    std::shared_ptr<std::thread> m_ThumbnailGenerationThread = nullptr;
    std::list<std::shared_ptr<FileInfos>> m_ThumbnailFileDatasToGet;  // base container
    std::mutex m_ThumbnailFileDatasToGetMutex;
    std::condition_variable m_ThumbnailFileDatasToGetCv;
    std::list<std::shared_ptr<FileInfos>> m_ThumbnailToCreate;  // base container
    std::mutex m_ThumbnailToCreateMutex;
    std::list<IGFD_Thumbnail_Info> m_ThumbnailToDestroy;  // base container
    std::mutex m_ThumbnailToDestroyMutex;

    CreateThumbnailFun m_CreateThumbnailFun = nullptr;
    DestroyThumbnailFun m_DestroyThumbnailFun = nullptr;

protected:
    DisplayModeEnum m_DisplayMode = DisplayModeEnum::FILE_LIST;

private:
    void m_VariadicProgressBar(float fraction, const ImVec2& size_arg, const char* fmt, ...);

protected:
    // will be call in cpu zone (imgui computations, will call a texture file retrieval thread)
    void m_StartThumbnailFileDatasExtraction();                               // start the thread who will get byte buffer from image files
    bool m_StopThumbnailFileDatasExtraction();                                // stop the thread who will get byte buffer from image files
    void m_ThreadThumbnailFileDatasExtractionFunc();                          // the thread who will get byte buffer from image files
    void m_DrawThumbnailGenerationProgress();                                 // a little progressbar who will display the texture gen status
    void m_AddThumbnailToLoad(const std::shared_ptr<FileInfos>& vFileInfos);  // add texture to load in the thread
    void m_AddThumbnailToCreate(const std::shared_ptr<FileInfos>& vFileInfos);
    void m_AddThumbnailToDestroy(const IGFD_Thumbnail_Info& vIGFD_Thumbnail_Info);
    void m_DrawDisplayModeToolBar();  // draw display mode toolbar (file list, thumbnails list, small thumbnails grid, big thumbnails grid)
    void m_ClearThumbnails(FileDialogInternal& vFileDialogInternal);

public:
    void SetCreateThumbnailCallback(const CreateThumbnailFun& vCreateThumbnailFun);
    void SetDestroyThumbnailCallback(const DestroyThumbnailFun& vCreateThumbnailFun);

    // must be call in gpu zone (rendering, possibly one rendering thread)
    void ManageGPUThumbnails();  // in gpu rendering zone, whill create or destroy texture
#endif
};

#pragma endregion

#pragma region PlacesFeature

class IGFD_API PlacesFeature {
protected:
    PlacesFeature();

#ifdef USE_PLACES_FEATURE
private:
    struct PlaceStruct {
        std::string name;  // name of the place
        // todo: the path could be relative, better if the app is moved but place path can be outside of the app
        std::string path;  // absolute path of the place
        bool canBeSaved = true;  // defined by code, can be used for prevent serialization / deserialization
        FileStyle style;
        float thickness = 0.0f; // when more than 0.0f, is a separator
    };

    struct GroupStruct {
        bool canBeSaved                              = false;  // defined by code, can be used for prevent serialization / deserialization
        size_t displayOrder                          = 0U;     // the display order will be usedf first, then alphanumeric
        bool defaultOpened                           = false;  // the group is opened by default
        bool canBeEdited                             = false;  // will show +/- button for add/remove place in the group
        char editBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";     // temp buffer for name edition
        int32_t selectedPlaceForEdition              = -1;
        ImGuiTreeNodeFlags collapsingHeaderFlag      = ImGuiTreeNodeFlags_None;
        ImGuiListClipper clipper;           // the list clipper of the grou
        std::string name;                   // the group name, will be displayed
        std::vector<PlaceStruct> places;    // the places (name + path)
        bool AddPlace(                      // add a place by code
            const std::string& vPlaceName,  // place name
            const std::string& vPlacePath,  // place path
            const bool& vCanBeSaved,        // prevent serialization
            const FileStyle& vStyle = {});  // style
        void AddPlaceSeparator(const float& vThickness = 1.0f);
        bool RemovePlace(                    // remove a place by code, return true if succeed
            const std::string& vPlaceName);  // place name to remove
    };

private:
    std::unordered_map<std::string, std::shared_ptr<GroupStruct>> m_Groups;
    std::map<size_t, std::weak_ptr<GroupStruct> > m_OrderedGroups;

protected:
    float m_PlacesPaneWidth = 200.0f;
    bool m_PlacesPaneShown = false;

protected:
    void m_InitPlaces(FileDialogInternal& vFileDialogInternal);
    void m_DrawPlacesButton();                                                            // draw place button
    bool m_DrawPlacesPane(FileDialogInternal& vFileDialogInternal, const ImVec2& vSize);  // draw place Pane

public:
    std::string SerializePlaces(                                    // serialize place : return place buffer to save in a file
        const bool& vForceSerialisationForAll = true);              // for avoid serialization of places with flag 'canBeSaved to false'
    void DeserializePlaces(                                         // deserialize place : load place buffer to load in the dialog (saved from
        const std::string& vPlaces);                                // previous use with SerializePlaces()) place buffer to load
    bool AddPlacesGroup(                                            // add a group
        const std::string& vGroupName,                              // the group name
        const size_t& vDisplayOrder,                                // the display roder of the group
        const bool& vCanBeEdited     = false,                       // let the user add/remove place in the group
        const bool& vOpenedByDefault = true);                      // hte group is opened by default
    bool RemovePlacesGroup(const std::string& vGroupName);          // remove the group
    GroupStruct* GetPlacesGroupPtr(const std::string& vGroupName);  // get the group, if not existed, will be created
#endif  // USE_PLACES_FEATURE
};

#pragma endregion

#pragma region KeyExplorerFeature

// file localization by input chat // widget flashing
class IGFD_API KeyExplorerFeature {
protected:
    KeyExplorerFeature();

#ifdef USE_EXPLORATION_BY_KEYS
private:
    bool m_LocateFileByInputChar_lastFound = false;
    ImWchar m_LocateFileByInputChar_lastChar = 0;
    float m_FlashAlpha = 0.0f;             // flash when select by char
    float m_FlashAlphaAttenInSecs = 1.0f;  // fps display dependant
    int m_LocateFileByInputChar_InputQueueCharactersSize = 0;
    size_t m_FlashedItem = 0;  // flash when select by char
    size_t m_LocateFileByInputChar_lastFileIdx = 0;

protected:
    void m_LocateByInputKey(FileDialogInternal& vFileDialogInternal);  // select a file line in listview according to char key
    bool m_LocateItem_Loop(FileDialogInternal& vFileDialogInternal,
        ImWchar vC);  // restrat for start of list view if not found a corresponding file
    void m_ExploreWithkeys(FileDialogInternal& vFileDialogInternal,
        ImGuiID vListViewID);            // select file/directory line in listview accroding to up/down enter/backspace keys
    void m_StartFlashItem(size_t vIdx);  // define than an item must be flashed
    bool m_BeginFlashItem(size_t vIdx);  // start the flashing of a line in lsit view
    static void m_EndFlashItem();        // end the fleshing accrdoin to var m_FlashAlphaAttenInSecs
    static bool m_FlashableSelectable(const char* label,
        bool selected = false,
        ImGuiSelectableFlags flags = 0,
        bool vFlashing = false,
        const ImVec2& size = ImVec2(0, 0));  // custom flashing selectable widgets, for flash the selected line in a short time

public:
    void SetFlashingAttenuationInSeconds(  // set the flashing time of the line in file list when use exploration keys
        float vAttenValue);                // set the attenuation (from flashed to not flashed) in seconds
#endif                                     // USE_EXPLORATION_BY_KEYS
};

#pragma endregion

#pragma endregion

#pragma region FileDialog

class IGFD_API FileDialog : public PlacesFeature, public KeyExplorerFeature, public ThumbnailFeature {
protected:
    FileDialogInternal m_FileDialogInternal;
    ImGuiListClipper m_FileListClipper;
    ImGuiListClipper m_PathListClipper;
    float prOkCancelButtonWidth = 0.0f;

public:
    // Singleton for easier accces form anywhere but only one dialog at a time
    // vCopy or vForce can be used for share a memory pointer in a new memory space like a dll module
    static FileDialog* Instance(FileDialog* vCopy = nullptr, bool vForce = false) {
        static FileDialog _instance;
        static FileDialog* _instance_copy = nullptr;
        if (vCopy || vForce) {
            _instance_copy = vCopy;
        }
        if (_instance_copy) {
            return _instance_copy;
        }
        return &_instance;
    }

public:
    FileDialog();           // ImGuiFileDialog Constructor. can be used for have many dialog at same time (not possible with singleton)
    virtual ~FileDialog();  // ImGuiFileDialog Destructor

    //  standard dialog
    virtual void OpenDialog(                    // open simple dialog
        const std::string& vKey,                // key dialog
        const std::string& vTitle,              // title
        const char* vFilters,                   // filters, if null, will display only directories
        const FileDialogConfig& vConfig = {});  // FileDialogConfig

    // Display / Close dialog form
    bool Display(                                               // Display the dialog. return true if a result was obtained (Ok or not)
        const std::string& vKey,                                // key dialog to display (if not the same key as defined by OpenDialog => no opening)
        ImGuiWindowFlags vFlags = ImGuiWindowFlags_NoCollapse,  // ImGuiWindowFlags
        ImVec2 vMinSize = ImVec2(0, 0),                         // mininmal size contraint for the ImGuiWindow
        ImVec2 vMaxSize = ImVec2(FLT_MAX, FLT_MAX));            // maximal size contraint for the ImGuiWindow
    void Close();                                               // close dialog

    // queries
    bool WasOpenedThisFrame(const std::string& vKey) const;  // say if the dialog key was already opened this frame
    bool WasOpenedThisFrame() const;                         // say if the dialog was already opened this frame
    bool IsOpened(const std::string& vKey) const;            // say if the key is opened
    bool IsOpened() const;                                   // say if the dialog is opened somewhere
    std::string GetOpenedKey() const;                        // return the dialog key who is opened, return nothing if not opened

    // get result
    bool IsOk() const;  // true => Dialog Closed with Ok result / false : Dialog closed with cancel result
    std::map<std::string, std::string> GetSelection(
        IGFD_ResultMode vFlag = IGFD_ResultMode_KeepInputFile);  // Open File behavior : will return selection via a
                                                                 // map<FileName, FilePathName>
    std::string GetFilePathName(
        IGFD_ResultMode vFlag = IGFD_ResultMode_AddIfNoFileExt);  // Save File behavior : will return the current file path name
    std::string GetCurrentFileName(IGFD_ResultMode vFlag = IGFD_ResultMode_AddIfNoFileExt);  // Save File behavior : will return the content file name
    std::string GetCurrentPath();                                                            // will return current file path
    std::string GetCurrentFilter();                                                          // will return current filter
    UserDatas GetUserDatas() const;                                                          // will return user datas send with Open Dialog

    // file style by extentions
    void SetFileStyle(                                        // SetExtention datas for have custom display of particular file type
        const IGFD_FileStyleFlags& vFlags,                    // file style
        const char* vCriteria,                                // extention filter to tune
        const FileStyle& vInfos);                             // Filter Extention Struct who contain Color and Icon/Text for the display of the
                                                              // file with extention filter
    void SetFileStyle(                                        // SetExtention datas for have custom display of particular file type
        const IGFD_FileStyleFlags& vFlags,                    // file style
        const char* vCriteria,                                // extention filter to tune
        const ImVec4& vColor,                                 // wanted color for the display of the file with extention filter
        const std::string& vIcon = "",                        // wanted text or icon of the file with extention filter
        ImFont* vFont = nullptr);                             // wanted font
    void SetFileStyle(FileStyle::FileStyleFunctor vFunctor);  // set file style via lambda function
    bool GetFileStyle(                                        // GetExtention datas. return true is extention exist
        const IGFD_FileStyleFlags& vFlags,                    // file style
        const std::string& vCriteria,                         // extention filter (same as used in SetExtentionInfos)
        ImVec4* vOutColor,                                    // color to retrieve
        std::string* vOutIcon = nullptr,                      // icon or text to retrieve
        ImFont** vOutFont = nullptr);                         // font to retreive
    void ClearFilesStyle();                                   // clear extentions setttings

    void SetLocales(                      // set locales to use before and after the dialog display
        const int& vLocaleCategory,       // set local category
        const std::string& vLocaleBegin,  // locale to use at begining of the dialog display
        const std::string& vLocaleEnd);   // locale to use at the end of the dialog display

protected:
    void m_NewFrame();   // new frame just at begining of display
    void m_EndFrame();   // end frame just at end of display
    void m_QuitFrame();  // quit frame when qui quit the dialog

    // others
    bool m_Confirm_Or_OpenOverWriteFileDialog_IfNeeded(
        bool vLastAction, ImGuiWindowFlags vFlags);  // treatment of the result, start the confirm to overwrite dialog
                                                     // if needed (if defined with flag)

    // dialog parts
    virtual void m_DrawHeader();   // draw header part of the dialog (place btn, dir creation, path composer, search
                                   // bar)
    virtual void m_DrawContent();  // draw content part of the dialog (place pane, file list, side pane)
    virtual bool m_DrawFooter();   // draw footer part of the dialog (file field, fitler combobox, ok/cancel btn's)

    // widgets components
    virtual void m_DisplayPathPopup(ImVec2 vSize);  // draw path popup when click on a \ or /
    virtual bool m_DrawValidationButtons();         // draw validations btns, ok, cancel buttons
    virtual bool m_DrawOkButton();                  // draw ok button
    virtual bool m_DrawCancelButton();              // draw cancel button
    virtual void m_DrawSidePane(float vHeight);     // draw side pane
    virtual void m_SelectableItem(int vidx,
        std::shared_ptr<FileInfos> vInfos,
        bool vSelected,
        const char* vFmt,
        ...);                                       // draw a custom selectable behavior item
    virtual void m_DrawFileListView(ImVec2 vSize);  // draw file list view (default mode)

#ifdef USE_THUMBNAILS
    virtual void m_DrawThumbnailsListView(ImVec2 vSize);  // draw file list view with small thumbnails on the same line
    virtual void m_DrawThumbnailsGridView(ImVec2 vSize);  // draw a grid of small thumbnails
#endif

    // to be called only by these function and theirs overrides
    // - m_DrawFileListView
    // - m_DrawThumbnailsListView
    // - m_DrawThumbnailsGridView
    void m_BeginFileColorIconStyle(std::shared_ptr<FileInfos> vFileInfos,
        bool& vOutShowColor,
        std::string& vOutStr,
        ImFont** vOutFont);                                               // begin style apply of filter with color an icon if any
    void m_EndFileColorIconStyle(const bool& vShowColor, ImFont* vFont);  // end style apply of filter

    void m_DisplayFileInfosTooltip(const int32_t& vRowIdx, const int32_t& vColumnIdx, std::shared_ptr<FileInfos> vFileInfos);
};

#pragma endregion

}  // namespace IGFD

#pragma endregion

#endif  // __cplusplus

#pragma region IGFD_C_API

#include <stdint.h>

#if defined _WIN32 || defined __CYGWIN__
#ifdef IMGUIFILEDIALOG_NO_EXPORT
#define API
#else  // IMGUIFILEDIALOG_NO_EXPORT
#define API __declspec(dllexport)
#endif  // IMGUIFILEDIALOG_NO_EXPORT
#else   // defined _WIN32 || defined __CYGWIN__
#ifdef __GNUC__
#define API __attribute__((__visibility__("default")))
#else  // __GNUC__
#define API
#endif  // __GNUC__
#endif  // defined _WIN32 || defined __CYGWIN__

#ifdef __cplusplus
#define IGFD_C_API extern "C" API
typedef IGFD::UserDatas IGFDUserDatas;
typedef IGFD::PaneFun IGFDPaneFun;
typedef IGFD::FileDialog ImGuiFileDialog;
#else  // __cplusplus
#define IGFD_C_API
typedef struct ImGuiFileDialog ImGuiFileDialog;
typedef struct IGFD_Selection_Pair IGFD_Selection_Pair;
typedef struct IGFD_Selection IGFD_Selection;
#endif  // __cplusplus

typedef void (*IGFD_PaneFun)(const char*, void*, bool*);  // callback fucntion for display the pane

struct IGFD_FileDialog_Config {
    const char* path;              // path
    const char* fileName;          // defaut file name
    const char* filePathName;      // if not empty, the filename and the path will be obtained from filePathName
    int32_t countSelectionMax;     // count selection max
    void* userDatas;               // user datas (can be retrieved in pane)
    IGFD_PaneFun sidePane;         // side pane callback
    float sidePaneWidth;  // side pane width};
    ImGuiFileDialogFlags flags;    // ImGuiFileDialogFlags
};
IGFD_C_API IGFD_FileDialog_Config IGFD_FileDialog_Config_Get();  // return an initialized IGFD_FileDialog_Config

struct IGFD_Selection_Pair {
    char* fileName;
    char* filePathName;
};

IGFD_C_API IGFD_Selection_Pair IGFD_Selection_Pair_Get();                                  // return an initialized IGFD_Selection_Pair
IGFD_C_API void IGFD_Selection_Pair_DestroyContent(IGFD_Selection_Pair* vSelection_Pair);  // destroy the content of a IGFD_Selection_Pair

struct IGFD_Selection {
    IGFD_Selection_Pair* table;  // 0
    size_t count;                // 0U
};

IGFD_C_API IGFD_Selection IGFD_Selection_Get();                             // return an initialized IGFD_Selection
IGFD_C_API void IGFD_Selection_DestroyContent(IGFD_Selection* vSelection);  // destroy the content of a IGFD_Selection

// constructor / destructor
IGFD_C_API ImGuiFileDialog* IGFD_Create(void);               // create the filedialog context
IGFD_C_API void IGFD_Destroy(ImGuiFileDialog* vContextPtr);  // destroy the filedialog context

#ifdef USE_THUMBNAILS
typedef void (*IGFD_CreateThumbnailFun)(IGFD_Thumbnail_Info*);   // callback function for create thumbnail texture
typedef void (*IGFD_DestroyThumbnailFun)(IGFD_Thumbnail_Info*);  // callback fucntion for destroy thumbnail texture
#endif                                                           // USE_THUMBNAILS

IGFD_C_API void IGFD_OpenDialog(            // open a standard dialog
    ImGuiFileDialog* vContextPtr,           // ImGuiFileDialog context
    const char* vKey,                       // key dialog
    const char* vTitle,                     // title
    const char* vFilters,                   // filters/filter collections. set it to null for directory mode
    const IGFD_FileDialog_Config vConfig);  // config

IGFD_C_API bool IGFD_DisplayDialog(  // Display the dialog
    ImGuiFileDialog* vContextPtr,    // ImGuiFileDialog context
    const char* vKey,                // key dialog to display (if not the same key as defined by OpenDialog => no opening)
    ImGuiWindowFlags vFlags,         // ImGuiWindowFlags
    ImVec2 vMinSize,                 // mininmal size contraint for the ImGuiWindow
    ImVec2 vMaxSize);                // maximal size contraint for the ImGuiWindow

IGFD_C_API void IGFD_CloseDialog(   // Close the dialog
    ImGuiFileDialog* vContextPtr);  // ImGuiFileDialog context

IGFD_C_API bool IGFD_IsOk(          // true => Dialog Closed with Ok result / false : Dialog closed with cancel result
    ImGuiFileDialog* vContextPtr);  // ImGuiFileDialog context

IGFD_C_API bool IGFD_WasKeyOpenedThisFrame(  // say if the dialog key was already opened this frame
    ImGuiFileDialog* vContextPtr,            // ImGuiFileDialog context
    const char* vKey);

IGFD_C_API bool IGFD_WasOpenedThisFrame(  // say if the dialog was already opened this frame
    ImGuiFileDialog* vContextPtr);        // ImGuiFileDialog context

IGFD_C_API bool IGFD_IsKeyOpened(    // say if the dialog key is opened
    ImGuiFileDialog* vContextPtr,    // ImGuiFileDialog context
    const char* vCurrentOpenedKey);  // the dialog key

IGFD_C_API bool IGFD_IsOpened(      // say if the dialog is opened somewhere
    ImGuiFileDialog* vContextPtr);  // ImGuiFileDialog context

IGFD_C_API IGFD_Selection IGFD_GetSelection(  // Open File behavior : will return selection via a map<FileName, FilePathName>
    ImGuiFileDialog* vContextPtr,             // user datas (can be retrieved in pane)
    IGFD_ResultMode vMode);                   // Result Mode

IGFD_C_API char* IGFD_GetFilePathName(  // Save File behavior : will always return the content of the field with current
                                        // filter extention and current path, WARNINGS you are responsible to free it
    ImGuiFileDialog* vContextPtr,       // ImGuiFileDialog context
    IGFD_ResultMode vMode);             // Result Mode

IGFD_C_API char* IGFD_GetCurrentFileName(  // Save File behavior : will always return the content of the field with
                                           // current filter extention, WARNINGS you are responsible to free it
    ImGuiFileDialog* vContextPtr,          // ImGuiFileDialog context
    IGFD_ResultMode vMode);                // Result Mode

IGFD_C_API char* IGFD_GetCurrentPath(  // will return current path, WARNINGS you are responsible to free it
    ImGuiFileDialog* vContextPtr);     // ImGuiFileDialog context

IGFD_C_API char* IGFD_GetCurrentFilter(  // will return selected filter, WARNINGS you are responsible to free it
    ImGuiFileDialog* vContextPtr);       // ImGuiFileDialog context

IGFD_C_API void* IGFD_GetUserDatas(  // will return user datas send with Open Dialog
    ImGuiFileDialog* vContextPtr);   // ImGuiFileDialog context

IGFD_C_API void IGFD_SetFileStyle(        // SetExtention datas for have custom display of particular file type
    ImGuiFileDialog* vContextPtr,         // ImGuiFileDialog context
    IGFD_FileStyleFlags vFileStyleFlags,  // file style type
    const char* vFilter,                  // extention filter to tune
    ImVec4 vColor,                        // wanted color for the display of the file with extention filter
    const char* vIconText,                // wanted text or icon of the file with extention filter (can be sued with font icon)
    ImFont* vFont);                       // wanted font pointer

IGFD_C_API void IGFD_SetFileStyle2(       // SetExtention datas for have custom display of particular file type
    ImGuiFileDialog* vContextPtr,         // ImGuiFileDialog context
    IGFD_FileStyleFlags vFileStyleFlags,  // file style type
    const char* vFilter,                  // extention filter to tune
    float vR,
    float vG,
    float vB,
    float vA,               // wanted color channels RGBA for the display of the file with extention filter
    const char* vIconText,  // wanted text or icon of the file with extention filter (can be sued with font icon)
    ImFont* vFont);         // wanted font pointer

IGFD_C_API bool IGFD_GetFileStyle(ImGuiFileDialog* vContextPtr,  // ImGuiFileDialog context
    IGFD_FileStyleFlags vFileStyleFlags,                         // file style type
    const char* vFilter,                                         // extention filter (same as used in SetExtentionInfos)
    ImVec4* vOutColor,                                           // color to retrieve
    char** vOutIconText,                                         // icon or text to retrieve, WARNINGS you are responsible to free it
    ImFont** vOutFont);                                          // font pointer to retrived

IGFD_C_API void IGFD_ClearFilesStyle(  // clear extentions setttings
    ImGuiFileDialog* vContextPtr);     // ImGuiFileDialog context

IGFD_C_API void SetLocales(        // set locales to use before and after display
    ImGuiFileDialog* vContextPtr,  // ImGuiFileDialog context
    const int vCategory,           // set local category
    const char* vBeginLocale,      // locale to use at begining of the dialog display
    const char* vEndLocale);       // locale to set at end of the dialog display

#ifdef USE_EXPLORATION_BY_KEYS
IGFD_C_API void IGFD_SetFlashingAttenuationInSeconds(  // set the flashing time of the line in file list when use
                                                       // exploration keys
    ImGuiFileDialog* vContextPtr,                      // ImGuiFileDialog context
    float vAttenValue);                                // set the attenuation (from flashed to not flashed) in seconds
#endif

#ifdef USE_PLACES_FEATURE
IGFD_C_API char* IGFD_SerializePlaces(    // serialize place : return place buffer to save in a file, WARNINGS
                                             // you are responsible to free it
    ImGuiFileDialog* vContextPtr,            // ImGuiFileDialog context
    bool vDontSerializeCodeBasedPlaces);  // for avoid serialization of place added by code

IGFD_C_API void IGFD_DeserializePlaces(  // deserialize place : load bookmar buffer to load in the dialog (saved
                                            // from previous use with SerializePlaces())
    ImGuiFileDialog* vContextPtr,           // ImGuiFileDialog context
    const char* vPlaces);                // place buffer to load

IGFD_C_API bool IGFD_AddPlacesGroup(  // add a places group by code
    ImGuiFileDialog* vContextPtr,     // ImGuiFileDialog context
    const char* vGroupName,           // the group name
    size_t vDisplayOrder,             // the display roder of the group
    bool vCanBeEdited);               // let the user add/remove place in the group

IGFD_C_API bool IGFD_RemovePlacesGroup(  // remove a place group by code, return true if succeed
    ImGuiFileDialog* vContextPtr,        // ImGuiFileDialog context
    const char* vGroupName);             // place name to remove

IGFD_C_API bool IGFD_AddPlace(     // add a place by code
    ImGuiFileDialog* vContextPtr,  // ImGuiFileDialog context
    const char* vGroupName,        // the group name
    const char* vPlaceName,        // place name
    const char* vPlacePath,        // place path
    bool vCanBeSaved,              // place can be saved
    const char* vIconText);        // wanted text or icon of the file with extention filter (can be used with font icon)

IGFD_C_API bool IGFD_RemovePlace(  // remove a place by code, return true if succeed
    ImGuiFileDialog* vContextPtr,  // ImGuiFileDialog context
    const char* vGroupName,        // the group name
    const char* vPlaceName);       // place name to remove

#endif

#ifdef USE_THUMBNAILS
IGFD_C_API void SetCreateThumbnailCallback(        // define the callback for create the thumbnails texture
    ImGuiFileDialog* vContextPtr,                  // ImGuiFileDialog context
    IGFD_CreateThumbnailFun vCreateThumbnailFun);  // the callback for create the thumbnails texture

IGFD_C_API void SetDestroyThumbnailCallback(         // define the callback for destroy the thumbnails texture
    ImGuiFileDialog* vContextPtr,                    // ImGuiFileDialog context
    IGFD_DestroyThumbnailFun vDestroyThumbnailFun);  // the callback for destroy the thumbnails texture

IGFD_C_API void ManageGPUThumbnails(  // must be call in gpu zone, possibly a thread, will call the callback for create
                                      // / destroy the textures
    ImGuiFileDialog* vContextPtr);    // ImGuiFileDialog context
#endif                                // USE_THUMBNAILS

#pragma endregion
