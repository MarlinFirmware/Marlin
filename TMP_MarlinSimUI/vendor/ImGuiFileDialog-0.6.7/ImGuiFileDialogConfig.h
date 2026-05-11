#pragma once

// uncomment and modify defines under for customize ImGuiFileDialog

// uncomment if you need to use your FileSystem Interface
// if commented, you have two defualt interface, std::filesystem or dirent
// #define USE_CUSTOM_FILESYSTEM

// this options need c++17
// #define USE_STD_FILESYSTEM

// #define MAX_FILE_DIALOG_NAME_BUFFER 1024
// #define MAX_PATH_BUFFER_SIZE 1024

// the slash's buttons in path cna be used for quick select parallles directories
// #define USE_QUICK_PATH_SELECT

// the spacing between button path's can be customized.
// if disabled the spacing is defined by the imgui theme
// define the space between path buttons
// #define CUSTOM_PATH_SPACING 2

// #define USE_THUMBNAILS
// the thumbnail generation use the stb_image and stb_resize lib who need to define the implementation
// btw if you already use them in your app, you can have compiler error due to "implemntation found in double"
// so uncomment these line for prevent the creation of implementation of these libs again
// #define DONT_DEFINE_AGAIN__STB_IMAGE_IMPLEMENTATION
// #define DONT_DEFINE_AGAIN__STB_IMAGE_RESIZE_IMPLEMENTATION
// #define IMGUI_RADIO_BUTTON RadioButton
// #define DisplayMode_ThumbailsList_ImageHeight 32.0f
// #define tableHeaderFileThumbnailsString "Thumbnails"
// #define DisplayMode_FilesList_ButtonString "FL"
// #define DisplayMode_FilesList_ButtonHelp "File List"
// #define DisplayMode_ThumbailsList_ButtonString "TL"
// #define DisplayMode_ThumbailsList_ButtonHelp "Thumbnails List"
// todo
// #define DisplayMode_ThumbailsGrid_ButtonString "TG"
// #define DisplayMode_ThumbailsGrid_ButtonHelp "Thumbnails Grid"

// #define USE_EXPLORATION_BY_KEYS
// this mapping by default is for GLFW but you can use another
// #include <GLFW/glfw3.h>
// Up key for explore to the top
// #define IGFD_KEY_UP ImGuiKey_UpArrow
// Down key for explore to the bottom
// #define IGFD_KEY_DOWN ImGuiKey_DownArrow
// Enter key for open directory
// #define IGFD_KEY_ENTER ImGuiKey_Enter
// BackSpace for comming back to the last directory
// #define IGFD_KEY_BACKSPACE ImGuiKey_Backspace

// by ex you can quit the dialog by pressing the key excape
// #define USE_DIALOG_EXIT_WITH_KEY
// #define IGFD_EXIT_KEY ImGuiKey_Escape

// widget
// begin combo widget
// #define IMGUI_BEGIN_COMBO ImGui::BeginCombo
// when auto resized, FILTER_COMBO_MIN_WIDTH will be considered has minimum width
// FILTER_COMBO_AUTO_SIZE is enabled by default now to 1
// uncomment if you want disable
// #define FILTER_COMBO_AUTO_SIZE 0
// filter combobox width
// #define FILTER_COMBO_MIN_WIDTH 120.0f
// button widget use for compose path
// #define IMGUI_PATH_BUTTON ImGui::Button
// standard button
// #define IMGUI_BUTTON ImGui::Button

// locales string
// #define createDirButtonString "+"
// #define resetButtonString "R"
// #define drivesButtonString "Drives"
// #define editPathButtonString "E"
// #define searchString "Search"
// #define dirEntryString "[DIR] "
// #define linkEntryString "[LINK] "
// #define fileEntryString "[FILE] "
// #define fileNameString "File Name : "
// #define dirNameString "Directory Path :"
// #define buttonResetSearchString "Reset search"
// #define buttonDriveString "Drives"
// #define buttonEditPathString "Edit path\nYou can also right click on path buttons"
// #define buttonResetPathString "Reset to current directory"
// #define buttonCreateDirString "Create Directory"
// #define OverWriteDialogTitleString "The file Already Exist !"
// #define OverWriteDialogMessageString "Would you like to OverWrite it ?"
// #define OverWriteDialogConfirmButtonString "Confirm"
// #define OverWriteDialogCancelButtonString "Cancel"

// Validation buttons
// #define okButtonString " OK"
// #define okButtonWidth 0.0f
// #define cancelButtonString " Cancel"
// #define cancelButtonWidth 0.0f
// alignement [0:1], 0.0 is left, 0.5 middle, 1.0 right, and other ratios
// #define okCancelButtonAlignement 0.0f
// #define invertOkAndCancelButtons 0

// DateTimeFormat
// see strftime functionin <ctime> for customize
// "%Y/%m/%d %H:%M" give 2021:01:22 11:47
// "%Y/%m/%d %i:%M%p" give 2021:01:22 11:45PM
// #define DateTimeFormat "%Y/%m/%d %i:%M%p"

// theses icons will appear in table headers
// #define USE_CUSTOM_SORTING_ICON
// #define tableHeaderAscendingIcon "A|"
// #define tableHeaderDescendingIcon "D|"
// #define tableHeaderFileNameString " File name"
// #define tableHeaderFileTypeString " Type"
// #define tableHeaderFileSizeString " Size"
// #define tableHeaderFileDateTimeString " Date"
// #define fileSizeBytes "o"
// #define fileSizeKiloBytes "Ko"
// #define fileSizeMegaBytes "Mo"
// #define fileSizeGigaBytes "Go"

// default table sort field (must be FIELD_FILENAME, FIELD_TYPE, FIELD_SIZE, FIELD_DATE or FIELD_THUMBNAILS)
// #define defaultSortField FIELD_FILENAME

// default table sort order for each field (true => Descending, false => Ascending)
// #define defaultSortOrderFilename true
// #define defaultSortOrderType true
// #define defaultSortOrderSize true
// #define defaultSortOrderDate true
// #define defaultSortOrderThumbnails true

// #define USE_PLACES_FEATURE
// #define PLACES_PANE_DEFAULT_SHOWN false
// #define placesPaneWith 150.0f
// #define IMGUI_TOGGLE_BUTTON ToggleButton
// #define placesButtonString "Place"
// #define placesButtonHelpString "Places"
// #define addPlaceButtonString "+"
// #define removePlaceButtonString "-"
// #define validatePlaceButtonString "ok"
// #define editPlaceButtonString "E"

// a group for bookmarks will be added by default, but you can also create it yourself and many more
// #define USE_PLACES_BOOKMARKS
// #define PLACES_BOOKMARK_DEFAULT_OPEPEND true
// #define placesBookmarksGroupName "Bookmarks"
// #define placesBookmarksDisplayOrder 0  // to the first

// a group for system devices (returned by IFileSystem), but you can also add yours
// by ex if you would like to display a specific icon for some devices
// #define USE_PLACES_DEVICES
// #define PLACES_DEVICES_DEFAULT_OPEPEND true
// #define placesDevicesGroupName "Devices"
// #define placesDevicesDisplayOrder 10  // to the end
