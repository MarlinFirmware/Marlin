[![Win](https://github.com/aiekick/ImGuiFileDialog/actions/workflows/Win.yml/badge.svg?branch=DemoApp)](https://github.com/aiekick/ImGuiFileDialog/actions/workflows/Win.yml)
[![Linux](https://github.com/aiekick/ImGuiFileDialog/actions/workflows/Linux.yml/badge.svg?branch=DemoApp)](https://github.com/aiekick/ImGuiFileDialog/actions/workflows/Linux.yml)
[![Osx](https://github.com/aiekick/ImGuiFileDialog/actions/workflows/Osx.yml/badge.svg?branch=DemoApp)](https://github.com/aiekick/ImGuiFileDialog/actions/workflows/Osx.yml)
[![Wrapped Dear ImGui Version](https://img.shields.io/badge/Dear%20ImGui%20Version-1.90.4-blue.svg)](https://github.com/ocornut/imgui)

# ImGuiFileDialog

## Purpose

ImGuiFileDialog is a file selection dialog built for (and using only) [Dear ImGui](https://github.com/ocornut/imgui).

My primary goal was to have a custom pane with widgets according to file extension. This was not possible using other
solutions.

## Possible Dialog Customization

![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/demo_dialog.png)

## ImGui Supported Version

ImGuiFileDialog follow the master and docking branch of ImGui. Currently ImGui 1.90.4

## Structure

* The library is in [Master branch](https://github.com/aiekick/ImGuiFileDialog/tree/master)
* A demo app can be found in the [DemoApp branch](https://github.com/aiekick/ImGuiFileDialog/tree/DemoApp)

This library is designed to be dropped into your source code rather than compiled separately.

From your project directory:

```
mkdir lib    <or 3rdparty, or externals, etc.>
cd lib
git clone https://github.com/aiekick/ImGuiFileDialog.git
git checkout master
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

Android Requirements : Api 21 mini

## Features

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

### WARNINGS :
- the nav system keyboard behavior is not working as expected, so maybe full of bug for ImGuiFileDialog
 
### Filter format

A filter is recognized only if it respects theses rules :

0) a filter must have 2 chars mini and the first must be a .
1) a regex must be in (( and ))
2) a , will separate filters except if between a ( and )
3) name{filter1, filter2} is a special form for collection filters
   - the name can be composed of what you want except { and }
   - a filter can be a regex 
4) the filters cannot integrate these chars '(' ')' '{' '}' ' ' except for a regex with respect to rule 1)
5) the filters cannot integrate a ','

<details open><summary><h2>Singleton Pattern vs. Multiple Instances :</h2></summary><blockquote>

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

</blockquote></details>

<details open><summary><h2>Simple Dialog :</h2></summary><blockquote>

```cpp
void drawGui() { 
  // open Dialog Simple
  if (ImGui::Button("Open File Dialog")) {
    IGFD::FileDialogConfig config;config.path = ".";
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", config);
  }
  // display
  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
    if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      // action
    }
    
    // close
    ImGuiFileDialog::Instance()->Close();
  }
}
```

![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/dlg_simple.gif)

</blockquote></details>

<details open><summary><h2>Modal Dialog :</h2></summary><blockquote>

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

</blockquote></details>

<details open><summary><h2>Directory Chooser :</h2></summary><blockquote>

To have a directory chooser, set the file extension filter to nullptr:

```cpp
IGFD::FileDialogConfig config;
config.path = ".";
ImGuiFileDialog::Instance()->OpenDialog("ChooseDirDlgKey", "Choose a Directory", nullptr, config);
```

In this mode you can select any directory with one click and open a directory with a double-click.

![directoryChooser](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/directoryChooser.gif)

</blockquote></details>

<details open><summary><h2>Dialog with Custom Pane :</h2></summary><blockquote>

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
inline void InfosPane(cosnt char *vFilter, IGFDUserDatas vUserDatas, bool *vCantContinue) // if vCantContinue is false, the user cant validate the dialog
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

![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/dlg_with_pane.gif)

</blockquote></details>

<details open><summary><h2>File Style : Custom icons and colors by extension :</h2></summary><blockquote>

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
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".png", ImVec4(0.0f, 1.0f, 1.0f, 0.9f), ICON_IGFD_FILE_PIC, font1);
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".gif", ImVec4(0.0f, 1.0f, 0.5f, 0.9f), "[GIF]");

// define style for all directories
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir, "", ImVec4(0.5f, 1.0f, 0.9f, 0.9f), ICON_IGFD_FOLDER);
// can be for a specific directory
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir, ".git", ImVec4(0.5f, 1.0f, 0.9f, 0.9f), ICON_IGFD_FOLDER);

// define style for all files
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeFile, "", ImVec4(0.5f, 1.0f, 0.9f, 0.9f), ICON_IGFD_FILE);
// can be for a specific file
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeFile, ".git", ImVec4(0.5f, 1.0f, 0.9f, 0.9f), ICON_IGFD_FILE);

// define style for all links
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeLink, "", ImVec4(0.5f, 1.0f, 0.9f, 0.9f));
// can be for a specific link
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeLink, "Readme.md", ImVec4(0.5f, 1.0f, 0.9f, 0.9f));

// define style for any files/dirs/links by fullname
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByFullName, "doc", ImVec4(0.9f, 0.2f, 0.0f, 0.9f), ICON_IGFD_FILE_PIC);

// define style by file who are containing this string
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByContainedInFullName, ".git", ImVec4(0.9f, 0.2f, 0.0f, 0.9f), ICON_IGFD_BOOKMARK);

all of theses can be miwed with IGFD_FileStyleByTypeDir / IGFD_FileStyleByTypeFile / IGFD_FileStyleByTypeLink
like theses by ex :
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir | IGFD_FileStyleByContainedInFullName, ".git", ImVec4(0.9f, 0.2f, 0.0f, 0.9f), ICON_IGFD_BOOKMARK);
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeFile | IGFD_FileStyleByFullName, "cmake", ImVec4(0.5f, 0.8f, 0.5f, 0.9f), ICON_IGFD_SAVE);

// for all these,s you can use a regex
// ex for color files like Custom*.h
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByFullName, "((Custom.+[.]h))", ImVec4(0.0f, 1.0f, 1.0f, 0.9f), ICON_IGFD_FILE_PIC, font1);

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

this sample code of [master/main.cpp](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/main.cpp) produce the picture above :

```cpp
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".cpp", ImVec4(1.0f, 1.0f, 0.0f, 0.9f));
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".h", ImVec4(0.0f, 1.0f, 0.0f, 0.9f));
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".hpp", ImVec4(0.0f, 0.0f, 1.0f, 0.9f));
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".md", ImVec4(1.0f, 0.0f, 1.0f, 0.9f));
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".png", ImVec4(0.0f, 1.0f, 1.0f, 0.9f), ICON_IGFD_FILE_PIC); // add an icon for the filter type
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".gif", ImVec4(0.0f, 1.0f, 0.5f, 0.9f), "[GIF]"); // add an text for a filter type
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir, nullptr, ImVec4(0.5f, 1.0f, 0.9f, 0.9f), ICON_IGFD_FOLDER); // for all dirs
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeFile, "CMakeLists.txt", ImVec4(0.1f, 0.5f, 0.5f, 0.9f), ICON_IGFD_ADD);
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByFullName, "doc", ImVec4(0.9f, 0.2f, 0.0f, 0.9f), ICON_IGFD_FILE_PIC);
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir | IGFD_FileStyleByContainedInFullName, ".git", ImVec4(0.9f, 0.2f, 0.0f, 0.9f), ICON_IGFD_BOOKMARK);
ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeFile | IGFD_FileStyleByContainedInFullName, ".git", ImVec4(0.5f, 0.8f, 0.5f, 0.9f), ICON_IGFD_SAVE);
```

![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/color_filter.png)

</blockquote></details>

<details open><summary><h2>Filter Collections :</h2></summary><blockquote>

You can define a custom filter name that corresponds to a group of filters using this syntax:

You can also use a regex for the filtering. the regex must be betwwen the ( and ) for be recognized as a regex


```custom_name1{filter1,filter2,filter3,(regex0),(regex1)},custom_name2{filter1,filter2,(regex0)},filter1```

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
![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/collectionFilters.gif)

</blockquote></details>

<details open><summary><h2>Multi Selection :</h2></summary><blockquote>

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

![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/multiSelection.gif)

</blockquote></details>

<details open><summary><h2>File Dialog Constraints :</h2></summary><blockquote>

You can set the minimum and/or maximum size of the dialog:

```cpp
ImVec2 maxSize = ImVec2((float)display_w, (float)display_h);  // The full display area
ImVec2 minSize = maxSize * 0.5f;  // Half the display area
ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, minSize, maxSize);
```

![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/dialog_constraints.gif)

</blockquote></details>

<details open><summary><h2>Exploring by keys :</h2></summary><blockquote>

You can activate this feature by uncommenting `#define USE_EXPLORATION_BY_KEYS`
in your custom config file (CustomImGuiFileDialogConfig.h)

You can also uncomment the next lines to define navigation keys:

* IGFD_KEY_UP => Up key for explore to the top
* IGFD_KEY_DOWN => Down key for explore to the bottom
* IGFD_KEY_ENTER => Enter key for open directory
* IGFD_KEY_BACKSPACE => BackSpace for comming back to the last directory

You can also jump to a point in the file list by pressing the corresponding key of the first filename character.

![alt text](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/explore_ny_keys.gif)

As you see the current item is flashed by default for 1 second. You can define the flashing lifetime with the function

```cpp
ImGuiFileDialog::Instance()->SetFlashingAttenuationInSeconds(1.0f);
```

</blockquote></details>

<details open><summary><h2>Places :</h2></summary><blockquote>

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

![places.gif](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/places.gif)

You can also serialize/deserialize groups and places (for example to load/save from/to a file):
```cpp
Load => ImGuiFileDialog::Instance()->DeserializePlaces(placesString);
Save => std::string placesString = ImGuiFileDialog::Instance()->SerializePlaces();
```

(please see DemoApp branch for details)

</blockquote></details>

<details open><summary><h2>Path Edition :</h2></summary><blockquote>

Right clicking on any path element button allows the user to manually edit the path from that portion of the tree.
Pressing the completion key (GLFW uses `enter` by default) validates the new path. Pressing the cancel key (GLFW
uses`escape` by default) cancels the manual entry and restores the original path.

Here's the manual entry operation in action:
![inputPathEdition.gif](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/inputPathEdition.gif)

</blockquote></details>

<details open><summary><h2>Confirm Overwrite Dialog :</h2></summary><blockquote>

If you want avoid overwriting files after selection, ImGuiFileDialog can show a dialog to confirm or cancel the
operation.

To do so, define the flag ImGuiFileDialogFlags_ConfirmOverwrite in your call to OpenDialog/OpenModal.

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

![ConfirmToOverWrite.gif](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/ConfirmToOverWrite.gif)

</blockquote></details>

<details open><summary><h2>Open / Save dialog Behavior :</h2></summary><blockquote>

ImGuiFileDialog uses the same code internally for Open and Save dialogs. To distinguish between them access the various
data return functions depending on what the dialog is doing.

When selecting an existing file (for example, a Load or Open dialog), use

```cpp
std::map<std::string, std::string> GetSelection(); // Returns selection via a map<FileName, FilePathName>
UserDatas GetUserDatas();                          // Get user data provided by the Open dialog
```

To selecting a new file (for example, a Save As... dialog), use:

```cpp
std::string GetFilePathName();                     // Returns the content of the selection field with current file extension and current path
std::string GetCurrentFileName();                  // Returns the content of the selection field with current file extension but no path
std::string GetCurrentPath();                      // Returns current path only
std::string GetCurrentFilter();                    // The file extension
```

</blockquote></details>

<details open><summary><h2>Thumbnails Display :</h2></summary><blockquote>

You can now, display thumbnails of pictures. 

![thumbnails.gif](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/thumbnails.gif)

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

</blockquote></details>

<details open><summary><h2>Embedded in other frames :</h2></summary><blockquote>

The dialog can be embedded in another user frame than the standard or modal dialog

You have to create a variable of type ImGuiFileDialog. (if you are suing the singleton, you will not have the possibility to open other dialog)

ex :

```cpp
ImGuiFileDialog fileDialog;

// open dialog; in this case, Bookmark, directory creation are disabled with, and also the file input field is readonly.
// btw you can od what you want
IGFD::FileDialogConfig config; 
config.path = ".";
config.countSelectionMax = -1;
config.flags = ImGuiFileDialogFlags_NoDialog | 
	ImGuiFileDialogFlags_DisableBookmarkMode | 
	ImGuiFileDialogFlags_DisableCreateDirectoryButton | 
	ImGuiFileDialogFlags_ReadOnlyFileNameField);
fileDialog.OpenDialog("embedded", "Select File", ".*", config);
// then display, here 
// to note, when embedded the ImVec2(0,0) (MinSize) do nothing, only the ImVec2(0,350) (MaxSize) can size the dialog frame 
fileDialog.Display("embedded", ImGuiWindowFlags_NoCollapse, ImVec2(0,0), ImVec2(0,350)))
```
the result :

![Embedded.gif](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/Embedded.gif)

</blockquote></details>

<details open><summary><h2>Quick Parallel Path Selection in Path Composer :</h2></summary><blockquote>

you have a separator between two directories in the path composer
when you click on it you can explore a list of parrallels directories of this point

this feature is disabled by default
you can enable it with the compiler flag : #define USE_QUICK_PATH_SELECT 

you can also customize the spacing between path button's with and without this mode
you can do that by define the compiler flag : #define CUSTOM_PATH_SPACING 2
if undefined the spacing is defined by the imgui theme

![quick_composer_path_select.gif](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/quick_composer_path_select.gif)

</blockquote></details>

<details open><summary><h2>Case Insensitive Filtering :</h2></summary><blockquote>

you can use this flag 'ImGuiFileDialogFlags_CaseInsensitiveExtention' when you call the open functions

```
by ex :
if the flag ImGuiFileDialogFlags_CaseInsensitiveExtention is used
with filters like .jpg or .Jpg or .JPG
all files with extentions by ex : .jpg and .JPG will be displayed
```

</blockquote></details>

<details open><summary><h2>How to Integrate ImGuiFileDialog in your project :</h2></summary><blockquote>

### Customize ImGuiFileDialog :

You can customize many aspects of ImGuiFileDialog by overriding `ImGuiFileDialogConfig.h`.

To enable your customizations, define the preprocessor directive CUSTOM_IMGUIFILEDIALOG_CONFIG with the path of your
custom config file. This path must be relative to the directory where you put the ImGuiFileDialog module.

This operation is demonstrated in `CustomImGuiFileDialog.h` in the example project to:

* Have a custom icon font instead of labels for buttons or message titles
* Customize the button text (the button call signature must be the same, by the way! :)

The custom icon font used in the example code ([CustomFont.cpp](CustomFont.cpp) and [CustomFont.h](CustomFont.h)) was made
with [ImGuiFontStudio](https://github.com/aiekick/ImGuiFontStudio), which I wrote. :)

ImGuiFontStudio uses ImGuiFileDialog! Check it out.

### Overriding ImGuiFileDialog :

You can also override it to draw differently or to add/remove widgets.

Take a look at the example application where I show how to use it to display a read-only checkbox.

see the 'CustomDrawReadOnlyCheckBoxFileDialog' class.

</blockquote></details>

<details open><summary><h2>Tune the validations button group :</h2></summary><blockquote>

You can specify :
- the width of "ok" and "cancel" buttons, by the set the defines "okButtonWidth" and "cancelButtonWidth"
- the alignement of the button group (left, right, middle, etc..) by set the define "okCancelButtonAlignement"
- if you want to have the ok button on the left and cancel button on the right or inverted by set the define "invertOkAndCancelButtons"

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

![alignement_0.0.gif](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/alignement_0.0.png)

with Alignement 1.0 => right

![alignement_1.0.gif](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/alignement_1.0.png)

with Alignement 0.5 => middle

![alignement_0.5.gif](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/alignement_0.5.png)

ok and cancel buttons inverted (cancel on the left and ok on the right)

![validation_buttons_inverted.gif](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/validation_buttons_inverted.png)

</blockquote></details>

<details open><summary><h2>Filtering by a regex :</h2></summary><blockquote>

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

</blockquote></details>

<details open><summary><h2>Multi Layer / asterisk based filter :</h2></summary><blockquote>

you can add filter in the form : .a.b.c .json.cpp .vcxproj.filters 

you can also add filter in the form : .* .*.* .vcx.* .*.filters .vcx*.filt.* etc..
all the * based filter are internally using regex's

</blockquote></details>

<details open><summary><h2>Result Modes for GetFilePathName, getFileName and GetSelection :</h2></summary><blockquote>

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
IGFD_ResultMode_AddIfNoFileExt [DEFAULT for 
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

</blockquote></details>

<details open><summary><h2>Custom FileSystem</h2></summary><blockquote>

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

</blockquote></details>

<details open><summary><h2>Modify file infos during scan by a callback</h2></summary><blockquote>

In some case, it can be unsefull to modify file infos during scan
so you can define your callback and attached it in the FileDialogConfig struct in the field userFileAttributes

the callback stamp is :
```cpp
bool (IGFD::FileInfos* vFileInfosPtr, IGFD::UserDatas vUserDatas)
```
if the callback is returning false, the file is ignored, so not displayed by the dialog

example : with the gltf separated files : (see the branch DemoApp for example use)

A gltf file can have data description and datas files separated.
in this case only the file with description will be shown in the dialog, so with not the full size of all attached datas

with this function, you can compose the path of the bin file, get his size, sum it to the desciption file size and use it

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
                vFileInfosPtr->fileSize += (size_t)statInfos.st_size; // add the size of bin file to the size of the gltf file
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

Before the User of the userAttribute callback :

![user_files_attributes_before.png](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/user_files_attributes_before.png)

After :

![user_files_attributes_after.png](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/user_files_attributes_after.png)

You can also display a tootlip for a file displayed when the mouse is over a dedicated column

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
vFileInfosPtr->tooltipColumn  = 1; // column of file size
```
![file_tooltip_message.png](https://github.com/aiekick/ImGuiFileDialog/blob/DemoApp/doc/file_tooltip_message.png)

</blockquote></details>

<details open><summary><h2>C Api :</h2></summary><blockquote>

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

</blockquote></details>

<details open><summary><h2>How to build the sample app :</h2></summary><blockquote>

The sample app is [here in master branch]((https://github.com/aiekick/ImGuiFileDialog/tree/master)

You need to use cMake. For the 3 Os (Win, Linux, MacOs), the cMake usage is exactly the same,

    Choose a build directory. (called here my_build_directory for instance) and
    Choose a Build Mode : "Release" / "MinSizeRel" / "RelWithDebInfo" / "Debug" (called here BuildMode for instance)
    Run cMake in console : (the first for generate cmake build files, the second for build the binary)

cmake -B my_build_directory -DCMAKE_BUILD_TYPE=BuildMode
cmake --build my_build_directory --config BuildMode

Some cMake version need Build mode define via the directive CMAKE_BUILD_TYPE or via --Config when we launch the build. This is why i put the boths possibilities

By the way you need before, to make sure, you have needed dependencies.

### On Windows :

You need to have the opengl library installed

### On Linux :

You need many lib : (X11, xrandr, xinerama, xcursor, mesa)

If you are on debian you can run :

sudo apt-get update 
sudo apt-get install libgl1-mesa-dev libx11-dev libxi-dev libxrandr-dev libxinerama-dev libxcursor-dev

### On MacOs :

you need many lib : opengl and cocoa framework

</blockquote></details>

## Thats all folks :-)

You can check by example in this repo with the file CustomImGuiFileDialogConfig.h :
- this trick was used for have custom icon font instead of labels for buttons or messages titles
- you can also use your custom imgui button, the button call stamp must be same by the way :)

The Custom Icon Font (in CustomFont.cpp and CustomFont.h) was made with ImGuiFontStudio (https://github.com/aiekick/ImGuiFontStudio) i wrote for that :)
ImGuiFontStudio is using also ImGuiFileDialog.$

![Alt](https://repobeats.axiom.co/api/embed/22a6eef207d0bce7c03519d94f55100973b451ca.svg "Repobeats analytics image")
