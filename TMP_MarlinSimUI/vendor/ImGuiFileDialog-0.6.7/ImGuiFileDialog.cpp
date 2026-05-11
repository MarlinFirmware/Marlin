#pragma region PVS STUDIO

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma endregion

#pragma region IGFD LICENSE

/*
MIT License

Copyright (c) 2019-2024 Stephane Cuillerdier (aka aiekick)

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

#include "ImGuiFileDialog.h"

#ifdef __cplusplus

#pragma region Includes

#include <cfloat>
#include <cstring>  // stricmp / strcasecmp
#include <cstdarg>  // variadic
#include <sstream>
#include <iomanip>
#include <ctime>
#include <memory>
#include <sys/stat.h>
#include <cstdio>
#include <cerrno>

// this option need c++17
#ifdef USE_STD_FILESYSTEM
#include <filesystem>
#include <exception>
#endif  // USE_STD_FILESYSTEM

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif  // __EMSCRIPTEN__

#ifdef _MSC_VER

#define IGFD_DEBUG_BREAK \
    if (IsDebuggerPresent()) __debugbreak()
#else
#define IGFD_DEBUG_BREAK
#endif

#if defined(__WIN32__) || defined(WIN32) || defined(_WIN32) || defined(__WIN64__) || defined(WIN64) || defined(_WIN64) || defined(_MSC_VER)
#define _IGFD_WIN_
#define stat _stati64
#define stricmp _stricmp
#include <cctype>
// this option need c++17
#ifdef USE_STD_FILESYSTEM
#include <windows.h>
#else
#include "dirent/dirent.h"  // directly open the dirent file attached to this lib
#endif                      // USE_STD_FILESYSTEM
#define PATH_SEP '\\'
#ifndef PATH_MAX
#define PATH_MAX 260
#endif  // PATH_MAX
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__APPLE__) || defined(__EMSCRIPTEN__)
#define _IGFD_UNIX_
#define stricmp strcasecmp
#include <sys/types.h>
// this option need c++17
#ifndef USE_STD_FILESYSTEM
#include <dirent.h>
#endif  // USE_STD_FILESYSTEM
#define PATH_SEP '/'
#endif  // _IGFD_UNIX_

#include "imgui.h"
#include "imgui_internal.h"

// legacy compatibility 1.89
#ifndef IM_TRUNC
#define IM_TRUNC IM_FLOOR
#endif

#include <cstdlib>
#include <algorithm>
#include <iostream>

#pragma endregion

#pragma region Common defines

#ifdef USE_THUMBNAILS
#ifndef DONT_DEFINE_AGAIN__STB_IMAGE_IMPLEMENTATION
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif  // STB_IMAGE_IMPLEMENTATION
#endif  // DONT_DEFINE_AGAIN__STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#ifndef DONT_DEFINE_AGAIN__STB_IMAGE_RESIZE_IMPLEMENTATION
#ifndef STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#endif  // STB_IMAGE_RESIZE_IMPLEMENTATION
#endif  // DONT_DEFINE_AGAIN__STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize.h"
#endif  // USE_THUMBNAILS

// float comparisons
#ifndef IS_FLOAT_DIFFERENT
#define IS_FLOAT_DIFFERENT(a, b) (fabs((a) - (b)) > FLT_EPSILON)
#endif  // IS_FLOAT_DIFFERENT
#ifndef IS_FLOAT_EQUAL
#define IS_FLOAT_EQUAL(a, b) (fabs((a) - (b)) < FLT_EPSILON)
#endif  // IS_FLOAT_EQUAL

#pragma endregion

#pragma region IGFD NAMESPACE

#pragma region CUSTOMIZATION DEFINES

///////////////////////////////
// COMBOBOX
///////////////////////////////
#ifndef FILTER_COMBO_AUTO_SIZE
#define FILTER_COMBO_AUTO_SIZE 1
#endif  // FILTER_COMBO_AUTO_SIZE
#ifndef FILTER_COMBO_MIN_WIDTH
#define FILTER_COMBO_MIN_WIDTH 150.0f
#endif  // FILTER_COMBO_MIN_WIDTH
#ifndef IMGUI_BEGIN_COMBO
#define IMGUI_BEGIN_COMBO ImGui::BeginCombo
#endif  // IMGUI_BEGIN_COMBO
///////////////////////////////
// BUTTON
///////////////////////////////
// for lets you define your button widget
// if you have like me a special bi-color button
#ifndef IMGUI_PATH_BUTTON
#define IMGUI_PATH_BUTTON ImGui::Button
#endif  // IMGUI_PATH_BUTTON
#ifndef IMGUI_BUTTON
#define IMGUI_BUTTON ImGui::Button
#endif  // IMGUI_BUTTON
///////////////////////////////
// locales
///////////////////////////////
#ifndef createDirButtonString
#define createDirButtonString "+"
#endif  // createDirButtonString
#ifndef okButtonString
#define okButtonString "OK"
#endif  // okButtonString
#ifndef okButtonWidth
#define okButtonWidth 0.0f
#endif  // okButtonWidth
#ifndef cancelButtonString
#define cancelButtonString "Cancel"
#endif  // cancelButtonString
#ifndef cancelButtonWidth
#define cancelButtonWidth 0.0f
#endif  // cancelButtonWidth
#ifndef okCancelButtonAlignement
#define okCancelButtonAlignement 0.0f
#endif  // okCancelButtonAlignement
#ifndef invertOkAndCancelButtons
// 0 => disabled, 1 => enabled
#define invertOkAndCancelButtons 0
#endif  // invertOkAndCancelButtons
#ifndef resetButtonString
#define resetButtonString "R"
#endif  // resetButtonString
#ifndef editPathButtonString
#define editPathButtonString "E"
#endif  // editPathButtonString
#ifndef searchString
#define searchString "Search :"
#endif  // searchString
#ifndef dirEntryString
#define dirEntryString "[Dir]"
#endif  // dirEntryString
#ifndef linkEntryString
#define linkEntryString "[Link]"
#endif  // linkEntryString
#ifndef fileEntryString
#define fileEntryString "[File]"
#endif  // fileEntryString
#ifndef fileNameString
#define fileNameString "File Name:"
#endif  // fileNameString
#ifndef dirNameString
#define dirNameString "Directory Path:"
#endif  // dirNameString
#ifndef buttonResetSearchString
#define buttonResetSearchString "Reset search"
#endif  // buttonResetSearchString
#ifndef buttonEditPathString
#define buttonEditPathString "Edit path\nYou can also right click on path buttons"
#endif  // buttonEditPathString
#ifndef buttonResetPathString
#define buttonResetPathString "Reset to current directory"
#endif  // buttonResetPathString
#ifndef buttonCreateDirString
#define buttonCreateDirString "Create Directory"
#endif  // buttonCreateDirString
#ifndef tableHeaderAscendingIcon
#define tableHeaderAscendingIcon "A|"
#endif  // tableHeaderAscendingIcon
#ifndef tableHeaderDescendingIcon
#define tableHeaderDescendingIcon "D|"
#endif  // tableHeaderDescendingIcon
#ifndef tableHeaderFileNameString
#define tableHeaderFileNameString "File name"
#endif  // tableHeaderFileNameString
#ifndef tableHeaderFileTypeString
#define tableHeaderFileTypeString "Type"
#endif  // tableHeaderFileTypeString
#ifndef tableHeaderFileSizeString
#define tableHeaderFileSizeString "Size"
#endif  // tableHeaderFileSizeString
#ifndef tableHeaderFileDateString
#define tableHeaderFileDateString "Date"
#endif  // tableHeaderFileDateString
#ifndef fileSizeBytes
#define fileSizeBytes "o"
#endif  // fileSizeBytes
#ifndef fileSizeKiloBytes
#define fileSizeKiloBytes "Ko"
#endif  // fileSizeKiloBytes
#ifndef fileSizeMegaBytes
#define fileSizeMegaBytes "Mo"
#endif  // fileSizeMegaBytes
#ifndef fileSizeGigaBytes
#define fileSizeGigaBytes "Go"
#endif  // fileSizeGigaBytes
#ifndef OverWriteDialogTitleString
#define OverWriteDialogTitleString "The selected file already exists!"
#endif  // OverWriteDialogTitleString
#ifndef OverWriteDialogMessageString
#define OverWriteDialogMessageString "Are you sure you want to overwrite it?"
#endif  // OverWriteDialogMessageString
#ifndef OverWriteDialogConfirmButtonString
#define OverWriteDialogConfirmButtonString "Confirm"
#endif  // OverWriteDialogConfirmButtonString
#ifndef OverWriteDialogCancelButtonString
#define OverWriteDialogCancelButtonString "Cancel"
#endif  // OverWriteDialogCancelButtonString
#ifndef DateTimeFormat
// see strftime functionin <ctime> for customize
#define DateTimeFormat "%Y/%m/%d %H:%M"
#endif  // DateTimeFormat
///////////////////////////////
// THUMBNAILS
///////////////////////////////
#ifdef USE_THUMBNAILS
#ifndef tableHeaderFileThumbnailsString
#define tableHeaderFileThumbnailsString "Thumbnails"
#endif  // tableHeaderFileThumbnailsString
#ifndef DisplayMode_FilesList_ButtonString
#define DisplayMode_FilesList_ButtonString "FL"
#endif  // DisplayMode_FilesList_ButtonString
#ifndef DisplayMode_FilesList_ButtonHelp
#define DisplayMode_FilesList_ButtonHelp "File List"
#endif  // DisplayMode_FilesList_ButtonHelp
#ifndef DisplayMode_ThumbailsList_ButtonString
#define DisplayMode_ThumbailsList_ButtonString "TL"
#endif  // DisplayMode_ThumbailsList_ButtonString
#ifndef DisplayMode_ThumbailsList_ButtonHelp
#define DisplayMode_ThumbailsList_ButtonHelp "Thumbnails List"
#endif  // DisplayMode_ThumbailsList_ButtonHelp
#ifndef DisplayMode_ThumbailsGrid_ButtonString
#define DisplayMode_ThumbailsGrid_ButtonString "TG"
#endif  // DisplayMode_ThumbailsGrid_ButtonString
#ifndef DisplayMode_ThumbailsGrid_ButtonHelp
#define DisplayMode_ThumbailsGrid_ButtonHelp "Thumbnails Grid"
#endif  // DisplayMode_ThumbailsGrid_ButtonHelp
#ifndef DisplayMode_ThumbailsList_ImageHeight
#define DisplayMode_ThumbailsList_ImageHeight 32.0f
#endif  // DisplayMode_ThumbailsList_ImageHeight
#ifndef IMGUI_RADIO_BUTTON
inline bool inRadioButton(const char* vLabel, bool vToggled) {
    bool pressed = false;
    if (vToggled) {
        ImVec4 bua = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
        ImVec4 te  = ImGui::GetStyleColorVec4(ImGuiCol_Text);
        ImGui::PushStyleColor(ImGuiCol_Button, te);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, te);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, te);
        ImGui::PushStyleColor(ImGuiCol_Text, bua);
    }
    pressed = IMGUI_BUTTON(vLabel);
    if (vToggled) {
        ImGui::PopStyleColor(4);  //-V112
    }
    return pressed;
}
#define IMGUI_RADIO_BUTTON inRadioButton
#endif  // IMGUI_RADIO_BUTTON
#endif  // USE_THUMBNAILS
///////////////////////////////
// BOOKMARKS
///////////////////////////////
#ifdef USE_PLACES_FEATURE
#ifndef defaultPlacePaneWith
#define defaultPlacePaneWith 150.0f
#endif  // defaultPlacePaneWith
#ifndef placesButtonString
#define placesButtonString "Places"
#endif  // placesButtonString
#ifndef placesButtonHelpString
#define placesButtonHelpString "Places"
#endif  // placesButtonHelpString
#ifndef placesBookmarksGroupName
#define placesBookmarksGroupName "Bookmarks"
#endif  // placesBookmarksGroupName
#ifndef PLACES_BOOKMARK_DEFAULT_OPEPEND
#define PLACES_BOOKMARK_DEFAULT_OPEPEND true
#endif  // PLACES_BOOKMARK_DEFAULT_OPEPEND
#ifndef PLACES_DEVICES_DEFAULT_OPEPEND
#define PLACES_DEVICES_DEFAULT_OPEPEND true
#endif  // PLACES_DEVICES_DEFAULT_OPEPEND
#ifndef placesBookmarksDisplayOrder
#define placesBookmarksDisplayOrder 0
#endif  // placesBookmarksDisplayOrder
#ifndef placesDevicesGroupName
#define placesDevicesGroupName "Devices"
#endif  // placesDevicesGroupName
#ifndef placesDevicesDisplayOrder
#define placesDevicesDisplayOrder 10
#endif  // placesDevicesDisplayOrder
#ifndef addPlaceButtonString
#define addPlaceButtonString "+"
#endif  // addPlaceButtonString
#ifndef removePlaceButtonString
#define removePlaceButtonString "-"
#endif  // removePlaceButtonString
#ifndef validatePlaceButtonString
#define validatePlaceButtonString "ok"
#endif  // validatePlaceButtonString
#ifndef editPlaceButtonString
#define editPlaceButtonString "E"
#endif  // editPlaceButtonString
#ifndef PLACES_PANE_DEFAULT_SHOWN
#define PLACES_PANE_DEFAULT_SHOWN false
#endif  // PLACES_PANE_DEFAULT_SHOWN
#ifndef IMGUI_TOGGLE_BUTTON
inline bool inToggleButton(const char* vLabel, bool* vToggled) {
    bool pressed = false;

    if (vToggled && *vToggled) {
        ImVec4 bua = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
        // ImVec4 buh = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
        // ImVec4 bu = ImGui::GetStyleColorVec4(ImGuiCol_Button);
        ImVec4 te = ImGui::GetStyleColorVec4(ImGuiCol_Text);
        ImGui::PushStyleColor(ImGuiCol_Button, te);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, te);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, te);
        ImGui::PushStyleColor(ImGuiCol_Text, bua);
    }

    pressed = IMGUI_BUTTON(vLabel);

    if (vToggled && *vToggled) {
        ImGui::PopStyleColor(4);  //-V112
    }

    if (vToggled && pressed) *vToggled = !*vToggled;

    return pressed;
}
#define IMGUI_TOGGLE_BUTTON inToggleButton
#endif  // IMGUI_TOGGLE_BUTTON
#endif  // USE_PLACES_FEATURE

#pragma endregion

#pragma region INTERNAL

#pragma region EXCEPTION

class IGFDException : public std::exception {
private:
    std::string m_Message;

public:
    IGFDException(const std::string& vMessage) : m_Message(vMessage) {
    }
    const char* what() {
        return m_Message.c_str();
    }
};

#pragma endregion

#pragma region FILE SYSTEM INTERFACE

#ifndef CUSTOM_FILESYSTEM_INCLUDE
#ifdef USE_STD_FILESYSTEM
class FileSystemStd : public IGFD::IFileSystem {
public:
    bool IsDirectoryCanBeOpened(const std::string& vName) override {
        bool bExists = false;
        if (!vName.empty()) {
            namespace fs = std::filesystem;
#ifdef _IGFD_WIN_
            std::wstring wname = IGFD::Utils::UTF8Decode(vName.c_str());
            fs::path pathName  = fs::path(wname);
#else   // _IGFD_WIN_
            fs::path pathName = fs::path(vName);
#endif  // _IGFD_WIN_
            try {
                // interesting, in the case of a protected dir or for any reason the dir cant be opened
                // this func will work but will say nothing more . not like the dirent version
                bExists = fs::is_directory(pathName);
                // test if can be opened, this function can thrown an exception if there is an issue with this dir
                // here, the dir_iter is need else not exception is thrown..
                const auto dir_iter = std::filesystem::directory_iterator(pathName);
                (void)dir_iter;  // for avoid unused warnings
            } catch (const std::exception& /*ex*/) {
                // fail so this dir cant be opened
                bExists = false;
            }
        }
        return bExists;  // this is not a directory!
    }
    bool IsDirectoryExist(const std::string& vName) override {
        if (!vName.empty()) {
            namespace fs = std::filesystem;
#ifdef _IGFD_WIN_
            std::wstring wname = IGFD::Utils::UTF8Decode(vName.c_str());
            fs::path pathName  = fs::path(wname);
#else   // _IGFD_WIN_
            fs::path pathName = fs::path(vName);
#endif  // _IGFD_WIN_
            return fs::is_directory(pathName);
        }
        return false;  // this is not a directory!
    }
    bool IsFileExist(const std::string& vName) override {
        namespace fs = std::filesystem;
        return fs::is_regular_file(vName);
    }
    bool CreateDirectoryIfNotExist(const std::string& vName) override {
        bool res = false;
        if (!vName.empty()) {
            if (!IsDirectoryExist(vName)) {
#ifdef _IGFD_WIN_
                namespace fs       = std::filesystem;
                std::wstring wname = IGFD::Utils::UTF8Decode(vName.c_str());
                fs::path pathName  = fs::path(wname);
                res                = fs::create_directory(pathName);
#elif defined(__EMSCRIPTEN__)  // _IGFD_WIN_
                std::string str = std::string("FS.mkdir('") + vName + "');";
                emscripten_run_script(str.c_str());
                res = true;
#elif defined(_IGFD_UNIX_)
                char buffer[PATH_MAX] = {};
                snprintf(buffer, PATH_MAX, "mkdir -p \"%s\"", vName.c_str());
                const int dir_err = std::system(buffer);
                if (dir_err != -1) {
                    res = true;
                }
#endif  // _IGFD_WIN_
                if (!res) {
                    std::cout << "Error creating directory " << vName << std::endl;
                }
            }
        }
        return res;
    }

    std::vector<IGFD::PathDisplayedName> GetDevicesList() override {
        std::vector<IGFD::PathDisplayedName> res;
#ifdef _IGFD_WIN_
        const DWORD mydrives = 2048;
        char lpBuffer[2048];
#define mini(a, b) (((a) < (b)) ? (a) : (b))
        const DWORD countChars = mini(GetLogicalDriveStringsA(mydrives, lpBuffer), 2047);
#undef mini
        if (countChars > 0U && countChars < 2049U) {
            std::string var = std::string(lpBuffer, (size_t)countChars);
            IGFD::Utils::ReplaceString(var, "\\", "");
            auto arr = IGFD::Utils::SplitStringToVector(var, '\0', false);
            wchar_t szVolumeName[2048];
            IGFD::PathDisplayedName path_name;
            for (auto& a : arr) {
                path_name.first = a;
                path_name.second.clear();
                std::wstring wpath = IGFD::Utils::UTF8Decode(a);
                if (GetVolumeInformationW(wpath.c_str(), szVolumeName, 2048, NULL, NULL, NULL, NULL, 0)) {
                    path_name.second = IGFD::Utils::UTF8Encode(szVolumeName);
                }
                res.push_back(path_name);
            }
        }
#endif  // _IGFD_WIN_
        return res;
    }

    IGFD::Utils::PathStruct ParsePathFileName(const std::string& vPathFileName) override {
        // https://github.com/aiekick/ImGuiFileDialog/issues/54
        namespace fs = std::filesystem;
        IGFD::Utils::PathStruct res;
        if (vPathFileName.empty()) return res;
        auto fsPath = fs::path(vPathFileName);
        if (fs::is_directory(fsPath)) {
            res.name = "";
            res.path = fsPath.string();
            res.isOk = true;
        } else if (fs::is_regular_file(fsPath)) {
            res.name = fsPath.filename().string();
            res.path = fsPath.parent_path().string();
            res.isOk = true;
        }
        return res;
    }

    std::vector<IGFD::FileInfos> ScanDirectory(const std::string& vPath) override {
        std::vector<IGFD::FileInfos> res;
        try {
            const std::filesystem::path fspath(vPath);
            const auto dir_iter   = std::filesystem::directory_iterator(fspath);
            IGFD::FileType fstype = IGFD::FileType(IGFD::FileType::ContentType::Directory, std::filesystem::is_symlink(std::filesystem::status(fspath)));
            {
                IGFD::FileInfos file_two_dot;
                file_two_dot.filePath    = vPath;
                file_two_dot.fileNameExt = "..";
                file_two_dot.fileType    = fstype;
                res.push_back(file_two_dot);
            }
            for (const auto& file : dir_iter) {
                IGFD::FileType fileType;
                if (file.is_symlink()) {
                    fileType.SetSymLink(file.is_symlink());
                    fileType.SetContent(IGFD::FileType::ContentType::LinkToUnknown);
                }
                if (file.is_directory()) {
                    fileType.SetContent(IGFD::FileType::ContentType::Directory);
                }  // directory or symlink to directory
                else if (file.is_regular_file()) {
                    fileType.SetContent(IGFD::FileType::ContentType::File);
                }
                if (fileType.isValid()) {
                    auto fileNameExt = file.path().filename().string();
                    {
                        IGFD::FileInfos _file;
                        _file.filePath    = vPath;
                        _file.fileNameExt = fileNameExt;
                        _file.fileType    = fileType;
                        res.push_back(_file);
                    }
                }
            }
        } catch (const std::exception& ex) {
            printf("%s", ex.what());
        }
        return res;
    }
    bool IsDirectory(const std::string& vFilePathName) override {
        namespace fs = std::filesystem;
        return fs::is_directory(vFilePathName);
    }
};
#define FILE_SYSTEM_OVERRIDE FileSystemStd
#else
class FileSystemDirent : public IGFD::IFileSystem {
public:
    bool IsDirectoryCanBeOpened(const std::string& vName) override {
        if (!vName.empty()) {
            DIR* pDir = nullptr;
            // interesting, in the case of a protected dir or for any reason the dir cant be opened
            // this func will fail
            pDir = opendir(vName.c_str());
            if (pDir != nullptr) {
                (void)closedir(pDir);
                return true;
            }
        }
        return false;
    }
    bool IsDirectoryExist(const std::string& vName) override {
        bool bExists = false;
        if (!vName.empty()) {
            DIR* pDir = nullptr;
            pDir      = opendir(vName.c_str());
            if (pDir) {
                bExists = true;
                closedir(pDir);
            } else if (ENOENT == errno) {
                /* Directory does not exist. */
                // bExists = false;
            } else {
                /* opendir() failed for some other reason.
                   like if a dir is protected, or not accessable with user right
                */
                bExists = true;
            }
        }
        return bExists;
    }
    bool IsFileExist(const std::string& vName) override {
        std::ifstream docFile(vName, std::ios::in);
        if (docFile.is_open()) {
            docFile.close();
            return true;
        }
        return false;
    }
    bool CreateDirectoryIfNotExist(const std::string& vName) override {
        bool res = false;
        if (!vName.empty()) {
            if (!IsDirectoryExist(vName)) {
#ifdef _IGFD_WIN_
                std::wstring wname = IGFD::Utils::UTF8Decode(vName);
                if (CreateDirectoryW(wname.c_str(), nullptr)) {
                    res = true;
                }
#elif defined(__EMSCRIPTEN__)  // _IGFD_WIN_
                std::string str = std::string("FS.mkdir('") + vName + "');";
                emscripten_run_script(str.c_str());
                res = true;
#elif defined(_IGFD_UNIX_)
                char buffer[PATH_MAX] = {};
                snprintf(buffer, PATH_MAX, "mkdir -p \"%s\"", vName.c_str());
                const int dir_err = std::system(buffer);
                if (dir_err != -1) {
                    res = true;
                }
#endif  // _IGFD_WIN_
                if (!res) {
                    std::cout << "Error creating directory " << vName << std::endl;
                }
            }
        }

        return res;
    }

    std::vector<IGFD::PathDisplayedName> GetDevicesList() override {
        std::vector<IGFD::PathDisplayedName> res;
#ifdef _IGFD_WIN_
        const DWORD mydrives = 2048;
        char lpBuffer[2048];
#define mini(a, b) (((a) < (b)) ? (a) : (b))
        const DWORD countChars = mini(GetLogicalDriveStringsA(mydrives, lpBuffer), 2047);
#undef mini
        if (countChars > 0U && countChars < 2049U) {
            std::string var = std::string(lpBuffer, (size_t)countChars);
            IGFD::Utils::ReplaceString(var, "\\", "");
            auto arr = IGFD::Utils::SplitStringToVector(var, '\0', false);
            wchar_t szVolumeName[2048];
            IGFD::PathDisplayedName path_name;
            for (auto& a : arr) {
                path_name.first = a;
                path_name.second.clear();
                std::wstring wpath = IGFD::Utils::UTF8Decode(a);
                if (GetVolumeInformationW(wpath.c_str(), szVolumeName, 2048, NULL, NULL, NULL, NULL, 0)) {
                    path_name.second = IGFD::Utils::UTF8Encode(szVolumeName);
                }
                res.push_back(path_name);
            }
        }
#endif  // _IGFD_WIN_
        return res;
    }

    IGFD::Utils::PathStruct ParsePathFileName(const std::string& vPathFileName) override {
        IGFD::Utils::PathStruct res;
        if (!vPathFileName.empty()) {
            std::string pfn = vPathFileName;
            std::string separator(1u, PATH_SEP);
            IGFD::Utils::ReplaceString(pfn, "\\", separator);
            IGFD::Utils::ReplaceString(pfn, "/", separator);
            size_t lastSlash = pfn.find_last_of(separator);
            if (lastSlash != std::string::npos) {
                res.name = pfn.substr(lastSlash + 1);
                res.path = pfn.substr(0, lastSlash);
                res.isOk = true;
            }
            size_t lastPoint = pfn.find_last_of('.');
            if (lastPoint != std::string::npos) {
                if (!res.isOk) {
                    res.name = pfn;
                    res.isOk = true;
                }
                res.ext = pfn.substr(lastPoint + 1);
                IGFD::Utils::ReplaceString(res.name, "." + res.ext, "");
            }
            if (!res.isOk) {
                res.name = std::move(pfn);
                res.isOk = true;
            }
        }
        return res;
    }

    std::vector<IGFD::FileInfos> ScanDirectory(const std::string& vPath) override {
        std::vector<IGFD::FileInfos> res;
        struct dirent** files = nullptr;
        size_t n              = scandir(vPath.c_str(), &files, nullptr,                         //
                                        [](const struct dirent** a, const struct dirent** b) {  //
                               return strcoll((*a)->d_name, (*b)->d_name);
                           });
        if (n && files) {
            for (size_t i = 0; i < n; ++i) {
                struct dirent* ent = files[i];
                IGFD::FileType fileType;
                switch (ent->d_type) {
                    case DT_DIR: fileType.SetContent(IGFD::FileType::ContentType::Directory); break;
                    case DT_REG: fileType.SetContent(IGFD::FileType::ContentType::File); break;
#if defined(_IGFD_UNIX_) || (DT_LNK != DT_UNKNOWN)
                    case DT_LNK:
#endif
                    case DT_UNKNOWN: {
                        struct stat sb = {};
#ifdef _IGFD_WIN_
                        auto filePath  = vPath + ent->d_name;
#else
                        auto filePath = vPath + IGFD::Utils::GetPathSeparator() + ent->d_name;
#endif
                        if (!stat(filePath.c_str(), &sb)) {
                            if (sb.st_mode & S_IFLNK) {
                                fileType.SetSymLink(true);
                                // by default if we can't figure out the target type.
                                fileType.SetContent(IGFD::FileType::ContentType::LinkToUnknown);
                            }
                            if (sb.st_mode & S_IFREG) {
                                fileType.SetContent(IGFD::FileType::ContentType::File);
                                break;
                            } else if (sb.st_mode & S_IFDIR) {
                                fileType.SetContent(IGFD::FileType::ContentType::Directory);
                                break;
                            }
                        }
                        break;
                    }
                    default: break;  // leave it invalid (devices, etc.)
                }
                if (fileType.isValid()) {
                    IGFD::FileInfos _file;
                    _file.filePath    = vPath;
                    _file.fileNameExt = ent->d_name;
                    _file.fileType    = fileType;
                    res.push_back(_file);
                }
            }
            for (size_t i = 0; i < n; ++i) {
                free(files[i]);
            }
            free(files);
        }
        return res;
    }
    bool IsDirectory(const std::string& vFilePathName) override {
        DIR *pDir = opendir(vFilePathName.c_str());
        if (pDir) {
            (void)closedir(pDir);
            return true;
        }
        return false;
    }
};
#define FILE_SYSTEM_OVERRIDE FileSystemDirent
#endif  // USE_STD_FILESYSTEM
#else
#include CUSTOM_FILESYSTEM_INCLUDE
#endif  // USE_CUSTOM_FILESYSTEM

#pragma endregion

#pragma region Utils

// https://github.com/ocornut/imgui/issues/1720
bool IGFD::Utils::ImSplitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size) {
    using namespace ImGui;
    ImGuiContext& g     = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiID id          = window->GetID("##Splitter");
    ImRect bb;
    bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
    bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
    return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 1.0f, 0.0, ImGui::GetColorU32(ImGuiCol_FrameBg));
}

// Convert a wide Unicode string to an UTF8 string
std::string IGFD::Utils::UTF8Encode(const std::wstring& wstr) {
    std::string res;
#ifdef _IGFD_WIN_
    if (!wstr.empty()) {
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
        if (size_needed) {
            res = std::string(size_needed, 0);
            WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &res[0], size_needed, NULL, NULL);
        }
    }
#else
    // Suppress warnings from the compiler.
    (void)wstr;
#endif  // _IGFD_WIN_
    return res;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring IGFD::Utils::UTF8Decode(const std::string& str) {
    std::wstring res;
#ifdef _IGFD_WIN_
    if (!str.empty()) {
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
        if (size_needed) {
            res = std::wstring(size_needed, 0);
            MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &res[0], size_needed);
        }
    }
#else
    // Suppress warnings from the compiler.
    (void)str;
#endif  // _IGFD_WIN_
    return res;
}

bool IGFD::Utils::ReplaceString(std::string& str, const ::std::string& oldStr, const ::std::string& newStr, const size_t& vMaxRecursion) {
    if (!str.empty() && oldStr != newStr) {
        bool res             = false;
        size_t pos           = 0;
        bool found           = false;
        size_t max_recursion = vMaxRecursion;
        do {
            pos = str.find(oldStr, pos);
            if (pos != std::string::npos) {
                found = res = true;
                str.replace(pos, oldStr.length(), newStr);
                pos += newStr.length();
            } else if (found && max_recursion > 0) {  // recursion loop
                found = false;
                pos   = 0;
                --max_recursion;
            }
        } while (pos != std::string::npos);
        return res;
    }
    return false;
}

std::vector<std::string> IGFD::Utils::SplitStringToVector(const std::string& vText, const std::string& vDelimiterPattern, const bool& vPushEmpty) {
    std::vector<std::string> arr;
    if (!vText.empty()) {
        size_t start = 0;
        size_t end   = vText.find(vDelimiterPattern, start);
        while (end != std::string::npos) {
            auto token = vText.substr(start, end - start);
            if (!token.empty() || (token.empty() && vPushEmpty)) {  //-V728
                arr.push_back(token);
            }
            start = end + vDelimiterPattern.size();
            end   = vText.find(vDelimiterPattern, start);
        }
        auto token = vText.substr(start);
        if (!token.empty() || (token.empty() && vPushEmpty)) {  //-V728
            arr.push_back(token);
        }
    }
    return arr;
}

std::vector<std::string> IGFD::Utils::SplitStringToVector(const std::string& vText, const char& vDelimiter, const bool& vPushEmpty) {
    std::vector<std::string> arr;
    if (!vText.empty()) {
        size_t start = 0;
        size_t end   = vText.find(vDelimiter, start);
        while (end != std::string::npos) {
            auto token = vText.substr(start, end - start);
            if (!token.empty() || (token.empty() && vPushEmpty)) {  //-V728
                arr.push_back(token);
            }
            start = end + 1;
            end   = vText.find(vDelimiter, start);
        }
        auto token = vText.substr(start);
        if (!token.empty() || (token.empty() && vPushEmpty)) {  //-V728
            arr.push_back(token);
        }
    }
    return arr;
}

void IGFD::Utils::AppendToBuffer(char* vBuffer, size_t vBufferLen, const std::string& vStr) {
    std::string st = vStr;
    size_t len     = vBufferLen - 1u;
    size_t slen    = strlen(vBuffer);

    if (!st.empty() && st != "\n") {
        IGFD::Utils::ReplaceString(st, "\n", "");
        IGFD::Utils::ReplaceString(st, "\r", "");
    }
    vBuffer[slen]   = '\0';
    std::string str = std::string(vBuffer);
    // if (!str.empty()) str += "\n";
    str += vStr;
    if (len > str.size()) {
        len = str.size();
    }
#ifdef _MSC_VER
    strncpy_s(vBuffer, vBufferLen, str.c_str(), len);
#else   // _MSC_VER
    strncpy(vBuffer, str.c_str(), len);
#endif  // _MSC_VER
    vBuffer[len] = '\0';
}

void IGFD::Utils::ResetBuffer(char* vBuffer) {
    vBuffer[0] = '\0';
}

void IGFD::Utils::SetBuffer(char* vBuffer, size_t vBufferLen, const std::string& vStr) {
    ResetBuffer(vBuffer);
    AppendToBuffer(vBuffer, vBufferLen, vStr);
}

std::string IGFD::Utils::LowerCaseString(const std::string& vString) {
    auto str = vString;

    // convert to lower case
    for (char& c : str) {
        c = (char)std::tolower(c);
    }

    return str;
}

size_t IGFD::Utils::GetCharCountInString(const std::string& vString, const char& vChar) {
    size_t res = 0U;
    for (const auto& c : vString) {
        if (c == vChar) {
            ++res;
        }
    }
    return res;
}

size_t IGFD::Utils::GetLastCharPosWithMinCharCount(const std::string& vString, const char& vChar, const size_t& vMinCharCount) {
    if (vMinCharCount) {
        size_t last_dot_pos = vString.size() + 1U;
        size_t count_dots   = vMinCharCount;
        while (count_dots > 0U && last_dot_pos > 0U && last_dot_pos != std::string::npos) {
            auto new_dot = vString.rfind(vChar, last_dot_pos - 1U);
            if (new_dot != std::string::npos) {
                last_dot_pos = new_dot;
                --count_dots;
            } else {
                break;
            }
        }
        return last_dot_pos;
    }
    return std::string::npos;
}

std::string IGFD::Utils::GetPathSeparator() {
    return std::string(1U, PATH_SEP);
}

std::string IGFD::Utils::RoundNumber(double vvalue, int n) {
    std::stringstream tmp;
    tmp << std::setprecision(n) << std::fixed << vvalue;
    return tmp.str();
}

std::string IGFD::Utils::FormatFileSize(size_t vByteSize) {
    if (vByteSize != 0) {
        static double lo = 1024.0;
        static double ko = 1024.0 * 1024.0;
        static double mo = 1024.0 * 1024.0 * 1024.0;

        auto v = (double)vByteSize;

        if (v < lo)
            return RoundNumber(v, 0) + " " + fileSizeBytes;  // octet
        else if (v < ko)
            return RoundNumber(v / lo, 2) + " " + fileSizeKiloBytes;  // ko
        else if (v < mo)
            return RoundNumber(v / ko, 2) + " " + fileSizeMegaBytes;  // Mo
        else
            return RoundNumber(v / mo, 2) + " " + fileSizeGigaBytes;  // Go
    }

    return "0 " fileSizeBytes;
}

#pragma endregion

#pragma region FileStyle

IGFD::FileStyle::FileStyle() : color(0, 0, 0, 0) {
}

IGFD::FileStyle::FileStyle(const FileStyle& vStyle) {
    color = vStyle.color;
    icon  = vStyle.icon;
    font  = vStyle.font;
    flags = vStyle.flags;
}

IGFD::FileStyle::FileStyle(const ImVec4& vColor, const std::string& vIcon, ImFont* vFont) : color(vColor), icon(vIcon), font(vFont) {
}

#pragma endregion

#pragma region SearchManager

void IGFD::SearchManager::Clear() {
    searchTag.clear();
    IGFD::Utils::ResetBuffer(searchBuffer);
}

void IGFD::SearchManager::DrawSearchBar(FileDialogInternal& vFileDialogInternal) {
    // search field
    if (IMGUI_BUTTON(resetButtonString "##BtnImGuiFileDialogSearchField")) {
        Clear();
        vFileDialogInternal.fileManager.ApplyFilteringOnFileList(vFileDialogInternal);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip(buttonResetSearchString);
    ImGui::SameLine();
    ImGui::Text(searchString);
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    bool edited = ImGui::InputText("##InputImGuiFileDialogSearchField", searchBuffer, MAX_FILE_DIALOG_NAME_BUFFER);
    if (ImGui::GetItemID() == ImGui::GetActiveID()) searchInputIsActive = true;
    ImGui::PopItemWidth();
    if (edited) {
        searchTag = searchBuffer;
        vFileDialogInternal.fileManager.ApplyFilteringOnFileList(vFileDialogInternal);
    }
}

#pragma endregion

#pragma region FilterInfos

void IGFD::FilterInfos::setCollectionTitle(const std::string& vTitle) {
    title = vTitle;
}

void IGFD::FilterInfos::addFilter(const std::string& vFilter, const bool& vIsRegex) {
    setCollectionTitle(vFilter);
    addCollectionFilter(vFilter, vIsRegex);
}

void IGFD::FilterInfos::addCollectionFilter(const std::string& vFilter, const bool& vIsRegex) {
    if (!vIsRegex) {
        if (vFilter.find('*') != std::string::npos) {
            const auto& regex_string = transformAsteriskBasedFilterToRegex(vFilter);
            addCollectionFilter(regex_string, true);
            return;
        }
        filters.try_add(vFilter);
        filters_optimized.try_add(Utils::LowerCaseString(vFilter));
        auto _count_dots = Utils::GetCharCountInString(vFilter, '.');
        if (_count_dots > count_dots) {
            count_dots = _count_dots;
        }
    } else {
        try {
            auto rx = std::regex(vFilter);
            filters.try_add(vFilter);
            filters_regex.emplace_back(rx);
        } catch (std::exception&) {
            assert(0);  // YOUR REGEX FILTER IS INVALID
        }
    }
}

void IGFD::FilterInfos::clear() {
    title.clear();
    filters.clear();
    filters_optimized.clear();
    filters_regex.clear();
}

bool IGFD::FilterInfos::empty() const {
    return filters.empty() || filters.begin()->empty();
}

const std::string& IGFD::FilterInfos::getFirstFilter() const {
    if (!filters.empty()) {
        return *filters.begin();
    }
    return empty_string;
}

bool IGFD::FilterInfos::exist(const FileInfos& vFileInfos, bool vIsCaseInsensitive) const {
    for (const auto& filter : filters) {
        if (vFileInfos.SearchForExt(filter, vIsCaseInsensitive, count_dots)) {
            return true;
        }
    }
    return false;
}

bool IGFD::FilterInfos::regexExist(const std::string& vFilter) const {
    for (auto regex : filters_regex) {
        if (std::regex_search(vFilter, regex)) {
            return true;
        }
    }
    return false;
}

std::string IGFD::FilterInfos::transformAsteriskBasedFilterToRegex(const std::string& vFilter) {
    std::string res;
    if (!vFilter.empty() && vFilter.find('*') != std::string::npos) {
        res = "((";
        for (const auto& c : vFilter) {
            if (c == '.') {
                res += "[.]";  // [.] => a dot
            } else if (c == '*') {
                res += ".*";  // .* => any char zero or many
            } else {
                res += c;  // other chars
            }
        }
        res += "$))";  // $ => end fo the string
    }
    return res;
}

#pragma endregion

#pragma region FilterManager

const IGFD::FilterInfos& IGFD::FilterManager::GetSelectedFilter() const {
    return m_SelectedFilter;
}

void IGFD::FilterManager::ParseFilters(const char* vFilters) {
    m_ParsedFilters.clear();

    if (vFilters) {
        dLGFilters = vFilters;  // file mode
    } else {
        dLGFilters.clear();  // directory mode
    }

    if (!dLGFilters.empty()) {
        /* Rules
        0) a filter must have 2 chars mini and the first must be a .
        1) a regex must be in (( and ))
        2) a , will separate filters except if between a ( and )
        3) name{filter1, filter2} is a spetial form for collection filters
        3.1) the name can be composed of what you want except { and }
        3.2) the filter can be a regex
        4) the filters cannot integrate these chars '(' ')' '{' '}' ' ' except for a regex with respect to rule 1)
        5) the filters cannot integrate a ','
        */

        bool current_filter_found = false;
        bool started              = false;
        bool regex_started        = false;
        bool parenthesis_started  = false;

        std::string word;
        std::string filter_name;

        char last_split_char = 0;
        for (char c : dLGFilters) {
            if (c == '{') {
                if (regex_started) {
                    word += c;
                } else {
                    started = true;
                    m_ParsedFilters.emplace_back();
                    m_ParsedFilters.back().setCollectionTitle(filter_name);
                    filter_name.clear();
                    word.clear();
                }
                last_split_char = c;
            } else if (c == '}') {
                if (regex_started) {
                    word += c;
                } else {
                    if (started) {
                        if (word.size() > 1U && word[0] == '.') {
                            if (m_ParsedFilters.empty()) {
                                m_ParsedFilters.emplace_back();
                            }
                            m_ParsedFilters.back().addCollectionFilter(word, false);
                        }
                        word.clear();
                        filter_name.clear();
                        started = false;
                    }
                }
                last_split_char = c;
            } else if (c == '(') {
                word += c;
                if (last_split_char == '(') {
                    regex_started = true;
                }
                parenthesis_started = true;
                if (!started) {
                    filter_name += c;
                }
                last_split_char = c;
            } else if (c == ')') {
                word += c;
                if (last_split_char == ')') {
                    if (regex_started) {
                        if (started) {
                            m_ParsedFilters.back().addCollectionFilter(word, true);
                        } else {
                            m_ParsedFilters.emplace_back();
                            m_ParsedFilters.back().addFilter(word, true);
                        }
                        word.clear();
                        filter_name.clear();
                        regex_started = false;
                    } else {
                        if (!started) {
                            if (!m_ParsedFilters.empty()) {
                                m_ParsedFilters.erase(m_ParsedFilters.begin() + m_ParsedFilters.size() - 1U);
                            } else {
                                m_ParsedFilters.clear();
                            }
                        }
                        word.clear();
                        filter_name.clear();
                    }
                }
                parenthesis_started = false;
                if (!started) {
                    filter_name += c;
                }
                last_split_char = c;
            } else if (c == '.') {
                word += c;
                if (!started) {
                    filter_name += c;
                }
                last_split_char = c;
            } else if (c == ',') {
                if (regex_started) {
                    regex_started = false;
                    word.clear();
                    filter_name.clear();
                } else {
                    if (started) {
                        if (word.size() > 1U && word[0] == '.') {
                            m_ParsedFilters.back().addCollectionFilter(word, false);
                            word.clear();
                            filter_name.clear();
                        }
                    } else {
                        if (word.size() > 1U && word[0] == '.') {
                            m_ParsedFilters.emplace_back();
                            m_ParsedFilters.back().addFilter(word, false);
                            word.clear();
                            filter_name.clear();
                        }
                        if (parenthesis_started) {
                            filter_name += c;
                        }
                    }
                }
            } else {
                if (c != ' ') {
                    word += c;
                }
                if (!started) {
                    filter_name += c;
                }
            }
        }

        if (started) {
            if (!m_ParsedFilters.empty()) {
                m_ParsedFilters.erase(m_ParsedFilters.begin() + m_ParsedFilters.size() - 1U);
            } else {
                m_ParsedFilters.clear();
            }
        } else if (word.size() > 1U && word[0] == '.') {
            m_ParsedFilters.emplace_back();
            m_ParsedFilters.back().addFilter(word, false);
            word.clear();
        }

        for (const auto& it : m_ParsedFilters) {
            if (it.title == m_SelectedFilter.title) {
                m_SelectedFilter     = it;
                current_filter_found = true;
                break;
            }
        }

        if (!current_filter_found) {
            if (!m_ParsedFilters.empty()) {
                m_SelectedFilter = *m_ParsedFilters.begin();
            }
        }
    }
}

void IGFD::FilterManager::SetSelectedFilterWithExt(const std::string& vFilter) {
    if (!m_ParsedFilters.empty()) {
        if (!vFilter.empty()) {
            for (const auto& infos : m_ParsedFilters) {
                for (const auto& filter : infos.filters) {
                    if (vFilter == filter) {
                        m_SelectedFilter = infos;
                    }
                }
            }
        }

        if (m_SelectedFilter.empty()) {
            m_SelectedFilter = *m_ParsedFilters.begin();
        }
    }
}

void IGFD::FilterManager::SetFileStyle(const IGFD_FileStyleFlags& vFlags, const char* vCriteria, const FileStyle& vInfos) {
    std::string _criteria                  = (vCriteria != nullptr) ? std::string(vCriteria) : "";
    m_FilesStyle[vFlags][_criteria]        = std::make_shared<FileStyle>(vInfos);
    m_FilesStyle[vFlags][_criteria]->flags = vFlags;
}

// will be called internally
// will not been exposed to IGFD API
bool IGFD::FilterManager::m_FillFileStyle(std::shared_ptr<FileInfos> vFileInfos) const {
    // todo : better system to found regarding what style to priorize regarding other
    // maybe with a lambda fucntion for let the user use his style
    // according to his use case
    if (vFileInfos.use_count() && !m_FilesStyle.empty()) {
        for (const auto& _flag : m_FilesStyle) {
            for (const auto& _file : _flag.second) {
                if ((_flag.first & IGFD_FileStyleByTypeDir && _flag.first & IGFD_FileStyleByTypeLink && vFileInfos->fileType.isDir() && vFileInfos->fileType.isSymLink()) ||
                    (_flag.first & IGFD_FileStyleByTypeFile && _flag.first & IGFD_FileStyleByTypeLink && vFileInfos->fileType.isFile() && vFileInfos->fileType.isSymLink()) ||
                    (_flag.first & IGFD_FileStyleByTypeLink && vFileInfos->fileType.isSymLink()) || (_flag.first & IGFD_FileStyleByTypeDir && vFileInfos->fileType.isDir()) ||
                    (_flag.first & IGFD_FileStyleByTypeFile && vFileInfos->fileType.isFile())) {
                    if (_file.first.empty()) {  // for all links
                        vFileInfos->fileStyle = _file.second;
                    } else if (_file.first.find("((") != std::string::npos && std::regex_search(vFileInfos->fileNameExt,
                                                                                                std::regex(_file.first))) {  // for links who are equal to style criteria
                        vFileInfos->fileStyle = _file.second;
                    } else if (_file.first == vFileInfos->fileNameExt) {  // for links who are equal to style criteria
                        vFileInfos->fileStyle = _file.second;
                    }
                }

                if (_flag.first & IGFD_FileStyleByExtention) {
                    if (_file.first.find("((") != std::string::npos && std::regex_search(vFileInfos->fileExtLevels[0], std::regex(_file.first))) {
                        vFileInfos->fileStyle = _file.second;
                    } else if (vFileInfos->SearchForExt(_file.first, false)) {
                        vFileInfos->fileStyle = _file.second;
                    }
                }

                if (_flag.first & IGFD_FileStyleByFullName) {
                    if (_file.first.find("((") != std::string::npos && std::regex_search(vFileInfos->fileNameExt, std::regex(_file.first))) {
                        vFileInfos->fileStyle = _file.second;
                    } else if (_file.first == vFileInfos->fileNameExt) {
                        vFileInfos->fileStyle = _file.second;
                    }
                }

                if (_flag.first & IGFD_FileStyleByContainedInFullName) {
                    if (_file.first.find("((") != std::string::npos && std::regex_search(vFileInfos->fileNameExt, std::regex(_file.first))) {
                        vFileInfos->fileStyle = _file.second;
                    } else if (vFileInfos->fileNameExt.find(_file.first) != std::string::npos) {
                        vFileInfos->fileStyle = _file.second;
                    }
                }

                for (auto& functor : m_FilesStyleFunctors) {
                    if (functor) {
                        FileStyle result;
                        if (functor(*(vFileInfos.get()), result)) {
                            vFileInfos->fileStyle = std::make_shared<FileStyle>(std::move(result));
                        }
                    }
                }

                if (vFileInfos->fileStyle.use_count()) {
                    return true;
                }
            }
        }
    }

    return false;
}

void IGFD::FilterManager::SetFileStyle(const IGFD_FileStyleFlags& vFlags, const char* vCriteria, const ImVec4& vColor, const std::string& vIcon, ImFont* vFont) {
    std::string _criteria;
    if (vCriteria) _criteria = std::string(vCriteria);
    m_FilesStyle[vFlags][_criteria]        = std::make_shared<FileStyle>(vColor, vIcon, vFont);
    m_FilesStyle[vFlags][_criteria]->flags = vFlags;
}

void IGFD::FilterManager::SetFileStyle(FileStyle::FileStyleFunctor vFunctor) {
    if (vFunctor) {
        m_FilesStyleFunctors.push_back(vFunctor);
    }
}

// todo : refactor this fucking function
bool IGFD::FilterManager::GetFileStyle(const IGFD_FileStyleFlags& vFlags, const std::string& vCriteria, ImVec4* vOutColor, std::string* vOutIcon, ImFont** vOutFont) {
    if (vOutColor) {
        if (!m_FilesStyle.empty()) {
            if (m_FilesStyle.find(vFlags) != m_FilesStyle.end()) {  // found
                if (vFlags & IGFD_FileStyleByContainedInFullName) {
                    // search for vCriteria who are containing the criteria
                    for (const auto& _file : m_FilesStyle.at(vFlags)) {
                        if (vCriteria.find(_file.first) != std::string::npos) {
                            if (_file.second.use_count()) {
                                *vOutColor = _file.second->color;
                                if (vOutIcon) *vOutIcon = _file.second->icon;
                                if (vOutFont) *vOutFont = _file.second->font;
                                return true;
                            }
                        }
                    }
                } else {
                    if (m_FilesStyle.at(vFlags).find(vCriteria) != m_FilesStyle.at(vFlags).end()) {  // found
                        *vOutColor = m_FilesStyle[vFlags][vCriteria]->color;
                        if (vOutIcon) *vOutIcon = m_FilesStyle[vFlags][vCriteria]->icon;
                        if (vOutFont) *vOutFont = m_FilesStyle[vFlags][vCriteria]->font;
                        return true;
                    }
                }
            } else {
                // search for flag composition
                for (const auto& _flag : m_FilesStyle) {
                    if (_flag.first & vFlags) {
                        if (_flag.first & IGFD_FileStyleByContainedInFullName) {
                            // search for vCriteria who are containing the criteria
                            for (const auto& _file : m_FilesStyle.at(_flag.first)) {
                                if (vCriteria.find(_file.first) != std::string::npos) {
                                    if (_file.second.use_count()) {
                                        *vOutColor = _file.second->color;
                                        if (vOutIcon) *vOutIcon = _file.second->icon;
                                        if (vOutFont) *vOutFont = _file.second->font;
                                        return true;
                                    }
                                }
                            }
                        } else {
                            if (m_FilesStyle.at(_flag.first).find(vCriteria) != m_FilesStyle.at(_flag.first).end()) {  // found
                                *vOutColor = m_FilesStyle[_flag.first][vCriteria]->color;
                                if (vOutIcon) *vOutIcon = m_FilesStyle[_flag.first][vCriteria]->icon;
                                if (vOutFont) *vOutFont = m_FilesStyle[_flag.first][vCriteria]->font;
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

void IGFD::FilterManager::ClearFilesStyle() {
    m_FilesStyle.clear();
}

bool IGFD::FilterManager::IsCoveredByFilters(const FileInfos& vFileInfos, bool vIsCaseInsensitive) const {
    if (!dLGFilters.empty() && !m_SelectedFilter.empty()) {
        return (m_SelectedFilter.exist(vFileInfos, vIsCaseInsensitive) || m_SelectedFilter.regexExist(vFileInfos.fileNameExt));
    }

    return false;
}

float IGFD::FilterManager::GetFilterComboBoxWidth() const {
#if FILTER_COMBO_AUTO_SIZE
    const auto& combo_width = ImGui::CalcTextSize(m_SelectedFilter.title.c_str()).x + ImGui::GetFrameHeight() + ImGui::GetStyle().ItemInnerSpacing.x;
    return ImMax(combo_width, FILTER_COMBO_MIN_WIDTH);
#else
    return FILTER_COMBO_MIN_WIDTH;
#endif
}

bool IGFD::FilterManager::DrawFilterComboBox(FileDialogInternal& vFileDialogInternal) {
    if (!dLGFilters.empty()) {
        ImGui::SameLine();
        bool needToApllyNewFilter = false;
        ImGui::PushItemWidth(GetFilterComboBoxWidth());
        if (IMGUI_BEGIN_COMBO("##Filters", m_SelectedFilter.title.c_str(), ImGuiComboFlags_None)) {
            intptr_t i = 0;
            for (const auto& filter : m_ParsedFilters) {
                const bool item_selected = (filter.title == m_SelectedFilter.title);
                ImGui::PushID((void*)(intptr_t)i++);
                if (ImGui::Selectable(filter.title.c_str(), item_selected)) {
                    m_SelectedFilter     = filter;
                    needToApllyNewFilter = true;
                }
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();
        if (needToApllyNewFilter) {
            vFileDialogInternal.fileManager.OpenCurrentPath(vFileDialogInternal);
        }
        return needToApllyNewFilter;
    }
    return false;
}

std::string IGFD::FilterManager::ReplaceExtentionWithCurrentFilterIfNeeded(const std::string& vFileName, IGFD_ResultMode vFlag) const {
    auto result = vFileName;
    if (!result.empty()) {
        const auto& current_filter = m_SelectedFilter.getFirstFilter();
        if (!current_filter.empty()) {
            Utils::ReplaceString(result, "..", ".");

            // is a regex => no change
            if (current_filter.find("((") != std::string::npos) {
                return result;
            }

            // contain .* => no change
            if (current_filter.find(".*") != std::string::npos) {
                return result;
            }

            switch (vFlag) {
                case IGFD_ResultMode_KeepInputFile: {
                    return vFileName;
                }
                case IGFD_ResultMode_OverwriteFileExt: {
                    const auto& count_dots = Utils::GetCharCountInString(vFileName, '.');
                    const auto& min_dots   = ImMin<size_t>(count_dots, m_SelectedFilter.count_dots);
                    const auto& lp         = Utils::GetLastCharPosWithMinCharCount(vFileName, '.', min_dots);
                    if (lp != std::string::npos) {  // there is a user extention
                        const auto& file_name_without_user_ext = vFileName.substr(0, lp);
                        result                                 = file_name_without_user_ext + current_filter;
                    } else {  // add extention
                        result = vFileName + current_filter;
                    }
                    break;
                }
                case IGFD_ResultMode_AddIfNoFileExt: {
                    const auto& count_dots = Utils::GetCharCountInString(vFileName, '.');
                    const auto& min_dots   = ImMin<size_t>(count_dots, m_SelectedFilter.count_dots);
                    const auto& lp         = Utils::GetLastCharPosWithMinCharCount(vFileName, '.', min_dots);
                    if (lp == std::string::npos ||        // there is no user extention
                        lp == (vFileName.size() - 1U)) {  // or this pos is also the last char => considered like no user extention
                        const auto& file_name_without_user_ext = vFileName.substr(0, lp);
                        result                                 = file_name_without_user_ext + current_filter;
                    }
                    break;
                }
            }

            Utils::ReplaceString(result, "..", ".");
        }
    }
    return result;
}

void IGFD::FilterManager::SetDefaultFilterIfNotDefined() {
    if (m_SelectedFilter.empty() &&                   // no filter selected
        !m_ParsedFilters.empty()) {                   // filter exist
        m_SelectedFilter = *m_ParsedFilters.begin();  // we take the first filter
    }
}

#pragma endregion

#pragma region FileType

IGFD::FileType::FileType() = default;
IGFD::FileType::FileType(const ContentType& vContentType, const bool& vIsSymlink) : m_Content(vContentType), m_Symlink(vIsSymlink) {
}
void IGFD::FileType::SetContent(const ContentType& vContentType) {
    m_Content = vContentType;
}
void IGFD::FileType::SetSymLink(const bool& vIsSymlink) {
    m_Symlink = vIsSymlink;
}
bool IGFD::FileType::isValid() const {
    return m_Content != ContentType::Invalid;
}
bool IGFD::FileType::isDir() const {
    return m_Content == ContentType::Directory;
}
bool IGFD::FileType::isFile() const {
    return m_Content == ContentType::File;
}
bool IGFD::FileType::isLinkToUnknown() const {
    return m_Content == ContentType::LinkToUnknown;
}
bool IGFD::FileType::isSymLink() const {
    return m_Symlink;
}
// Comparisons only care about the content type, ignoring whether it's a symlink or not.
bool IGFD::FileType::operator==(const FileType& rhs) const {
    return m_Content == rhs.m_Content;
}
bool IGFD::FileType::operator!=(const FileType& rhs) const {
    return m_Content != rhs.m_Content;
}
bool IGFD::FileType::operator<(const FileType& rhs) const {
    return m_Content < rhs.m_Content;
}
bool IGFD::FileType::operator>(const FileType& rhs) const {
    return m_Content > rhs.m_Content;
}

#pragma endregion

#pragma region FileInfos

bool IGFD::FileInfos::SearchForTag(const std::string& vTag) const {
    if (!vTag.empty()) {
        if (fileNameExt_optimized == "..") return true;
        return fileNameExt_optimized.find(vTag) != std::string::npos ||  // first try without case and accents
               fileNameExt.find(vTag) != std::string::npos;              // second if searched with case and accents
    }

    // if tag is empty => its a special case but all is found
    return true;
}

bool IGFD::FileInfos::SearchForExt(const std::string& vExt, const bool& vIsCaseInsensitive, const size_t& vMaxLevel) const {
    if (!vExt.empty()) {
        const auto& ext_to_check = vIsCaseInsensitive ? Utils::LowerCaseString(vExt) : vExt;
        const auto& ext_levels   = vIsCaseInsensitive ? fileExtLevels_optimized : fileExtLevels;
        if (vMaxLevel >= 1 && countExtDot >= vMaxLevel) {
            for (const auto& ext : ext_levels) {
                if (!ext.empty() && ext == ext_to_check) {
                    return true;
                }
            }
        } else {
            return (fileExtLevels[0] == vExt);
        }
    }
    return false;
}

bool IGFD::FileInfos::SearchForExts(const std::string& vComaSepExts, const bool& vIsCaseInsensitive, const size_t& vMaxLevel) const {
    if (!vComaSepExts.empty()) {
        const auto& arr = Utils::SplitStringToVector(vComaSepExts, ',', false);
        for (const auto& a : arr) {
            if (SearchForExt(a, vIsCaseInsensitive, vMaxLevel)) {
                return true;
            }
        }
    }
    return false;
}

bool IGFD::FileInfos::FinalizeFileTypeParsing(const size_t& vMaxDotToExtract) {
    if (fileType.isFile() || fileType.isLinkToUnknown()) {  // link can have the same extention of a file
        countExtDot = Utils::GetCharCountInString(fileNameExt, '.');
        size_t lpt  = 0U;
        if (countExtDot > 1U) {  // multi layer ext
            size_t max_dot_to_extract = vMaxDotToExtract;
            if (max_dot_to_extract > countExtDot) {
                max_dot_to_extract = countExtDot;
            }
            lpt = Utils::GetLastCharPosWithMinCharCount(fileNameExt, '.', max_dot_to_extract);
        } else {
            lpt = fileNameExt.find_first_of('.');
        }
        if (lpt != std::string::npos) {
            size_t lvl                   = 0U;
            fileNameLevels[lvl]          = fileNameExt.substr(0, lpt);
            fileNameLevels[lvl]          = Utils::LowerCaseString(fileNameLevels[lvl]);
            fileExtLevels[lvl]           = fileNameExt.substr(lpt);
            fileExtLevels_optimized[lvl] = Utils::LowerCaseString(fileExtLevels[lvl]);
            if (countExtDot > 1U) {  // multi layer ext
                auto count = countExtDot;
                while (count > 0 && lpt != std::string::npos && lvl < fileExtLevels.size()) {
                    ++lpt, ++lvl;
                    if (fileNameExt.size() > lpt) {
                        lpt = fileNameExt.find_first_of('.', lpt);
                        if (lpt != std::string::npos) {
                            fileNameLevels[lvl]          = fileNameExt.substr(0, lpt);
                            fileNameLevels[lvl]          = Utils::LowerCaseString(fileNameLevels[lvl]);
                            fileExtLevels[lvl]           = fileNameExt.substr(lpt);
                            fileExtLevels_optimized[lvl] = Utils::LowerCaseString(fileExtLevels[lvl]);
                        }
                    }
                }
            }
        }
        return true;
    }
    return false;
}

#pragma endregion

#pragma region FileManager

IGFD::FileManager::FileManager() {
    fsRoot           = IGFD::Utils::GetPathSeparator();
#define STR(x)              #x
#define STR_AFTER_EXPAND(x) STR(x)
    m_FileSystemName = STR_AFTER_EXPAND(FILE_SYSTEM_OVERRIDE);
#undef STR_AFTER_EXPAND
#undef STR
    // std::make_unique is not available un cpp11
    m_FileSystemPtr = std::unique_ptr<FILE_SYSTEM_OVERRIDE>(new FILE_SYSTEM_OVERRIDE());
    // m_FileSystemPtr = std::make_unique<FILE_SYSTEM_OVERRIDE>();
}

void IGFD::FileManager::OpenCurrentPath(const FileDialogInternal& vFileDialogInternal) {
    ClearComposer();
    ClearFileLists();
    if (dLGDirectoryMode) {  // directory mode
        SetDefaultFileName(".");
    } else {
        SetDefaultFileName(dLGDefaultFileName);
    }
    ScanDir(vFileDialogInternal, GetCurrentPath());
}

void IGFD::FileManager::SortFields(const FileDialogInternal& vFileDialogInternal) {
    m_SortFields(vFileDialogInternal, m_FileList, m_FilteredFileList);
}

void IGFD::FileManager::m_SortFields(const FileDialogInternal& vFileDialogInternal, std::vector<std::shared_ptr<FileInfos> >& vFileInfosList, std::vector<std::shared_ptr<FileInfos> >& vFileInfosFilteredList) {
    if (sortingField != SortingFieldEnum::FIELD_NONE) {
        headerFileName = tableHeaderFileNameString;
        headerFileType = tableHeaderFileTypeString;
        headerFileSize = tableHeaderFileSizeString;
        headerFileDate = tableHeaderFileDateString;
#ifdef USE_THUMBNAILS
        headerFileThumbnails = tableHeaderFileThumbnailsString;
#endif  // #ifdef USE_THUMBNAILS
    }
    if (sortingField == SortingFieldEnum::FIELD_FILENAME) {
        if (sortingDirection[0]) {
#ifdef USE_CUSTOM_SORTING_ICON
            headerFileName = tableHeaderAscendingIcon + headerFileName;
#endif  // USE_CUSTOM_SORTING_ICON
            std::sort(vFileInfosList.begin(), vFileInfosList.end(), [](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool {
                if (!a.use_count() || !b.use_count()) return false;
                // tofix : this code fail in c:\\Users with the link "All users". got a invalid comparator
                /*
                // use code from
                https://github.com/jackm97/ImGuiFileDialog/commit/bf40515f5a1de3043e60562dc1a494ee7ecd3571
                // strict ordering for file/directory types beginning in '.'
                // common on _IGFD_WIN_ platforms
                if (a->fileNameExt[0] == '.' && b->fileNameExt[0] != '.')
                    return false;
                if (a->fileNameExt[0] != '.' && b->fileNameExt[0] == '.')
                    return true;
                if (a->fileNameExt[0] == '.' && b->fileNameExt[0] == '.')
                {
                    return (stricmp(a->fileNameExt.c_str(), b->fileNameExt.c_str()) < 0); // sort in insensitive
                case
                }
                */
                if (a->fileType != b->fileType) return (a->fileType < b->fileType);    // directories first
                return (stricmp(a->fileNameExt.c_str(), b->fileNameExt.c_str()) < 0);  // sort in insensitive case
            });
        } else {
#ifdef USE_CUSTOM_SORTING_ICON
            headerFileName = tableHeaderDescendingIcon + headerFileName;
#endif  // USE_CUSTOM_SORTING_ICON
            std::sort(vFileInfosList.begin(), vFileInfosList.end(), [](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool {
                if (!a.use_count() || !b.use_count()) return false;
                // tofix : this code fail in c:\\Users with the link "All users". got a invalid comparator
                /*
                // use code from
                https://github.com/jackm97/ImGuiFileDialog/commit/bf40515f5a1de3043e60562dc1a494ee7ecd3571
                // strict ordering for file/directory types beginning in '.'
                // common on _IGFD_WIN_ platforms
                if (a->fileNameExt[0] == '.' && b->fileNameExt[0] != '.')
                    return false;
                if (a->fileNameExt[0] != '.' && b->fileNameExt[0] == '.')
                    return true;
                if (a->fileNameExt[0] == '.' && b->fileNameExt[0] == '.')
                {
                    return (stricmp(a->fileNameExt.c_str(), b->fileNameExt.c_str()) > 0); // sort in insensitive
                case
                }
                */
                if (a->fileType != b->fileType) return (a->fileType > b->fileType);    // directories last
                return (stricmp(a->fileNameExt.c_str(), b->fileNameExt.c_str()) > 0);  // sort in insensitive case
            });
        }
    } else if (sortingField == SortingFieldEnum::FIELD_TYPE) {
        if (sortingDirection[1]) {
#ifdef USE_CUSTOM_SORTING_ICON
            headerFileType = tableHeaderAscendingIcon + headerFileType;
#endif  // USE_CUSTOM_SORTING_ICON
            std::sort(vFileInfosList.begin(), vFileInfosList.end(), [](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool {
                if (!a.use_count() || !b.use_count()) return false;
                if (a->fileType != b->fileType) return (a->fileType < b->fileType);  // directory in first
                return (a->fileExtLevels[0] < b->fileExtLevels[0]);                  // else
            });
        } else {
#ifdef USE_CUSTOM_SORTING_ICON
            headerFileType = tableHeaderDescendingIcon + headerFileType;
#endif  // USE_CUSTOM_SORTING_ICON
            std::sort(vFileInfosList.begin(), vFileInfosList.end(), [](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool {
                if (!a.use_count() || !b.use_count()) return false;
                if (a->fileType != b->fileType) return (a->fileType > b->fileType);  // directory in last
                return (a->fileExtLevels[0] > b->fileExtLevels[0]);                  // else
            });
        }
    } else if (sortingField == SortingFieldEnum::FIELD_SIZE) {
        if (sortingDirection[2]) {
#ifdef USE_CUSTOM_SORTING_ICON
            headerFileSize = tableHeaderAscendingIcon + headerFileSize;
#endif  // USE_CUSTOM_SORTING_ICON
            std::sort(vFileInfosList.begin(), vFileInfosList.end(), [](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool {
                if (!a.use_count() || !b.use_count()) return false;
                if (a->fileType != b->fileType) return (a->fileType < b->fileType);  // directory in first
                return (a->fileSize < b->fileSize);                                  // else
            });
        } else {
#ifdef USE_CUSTOM_SORTING_ICON
            headerFileSize = tableHeaderDescendingIcon + headerFileSize;
#endif  // USE_CUSTOM_SORTING_ICON
            std::sort(vFileInfosList.begin(), vFileInfosList.end(), [](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool {
                if (!a.use_count() || !b.use_count()) return false;
                if (a->fileType != b->fileType) return (a->fileType > b->fileType);  // directory in last
                return (a->fileSize > b->fileSize);                                  // else
            });
        }
    } else if (sortingField == SortingFieldEnum::FIELD_DATE) {
        if (sortingDirection[3]) {
#ifdef USE_CUSTOM_SORTING_ICON
            headerFileDate = tableHeaderAscendingIcon + headerFileDate;
#endif  // USE_CUSTOM_SORTING_ICON
            std::sort(vFileInfosList.begin(), vFileInfosList.end(), [](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool {
                if (!a.use_count() || !b.use_count()) return false;
                if (a->fileType != b->fileType) return (a->fileType < b->fileType);  // directory in first
                return (a->fileModifDate < b->fileModifDate);                        // else
            });
        } else {
#ifdef USE_CUSTOM_SORTING_ICON
            headerFileDate = tableHeaderDescendingIcon + headerFileDate;
#endif  // USE_CUSTOM_SORTING_ICON
            std::sort(vFileInfosList.begin(), vFileInfosList.end(), [](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool {
                if (!a.use_count() || !b.use_count()) return false;
                if (a->fileType != b->fileType) return (a->fileType > b->fileType);  // directory in last
                return (a->fileModifDate > b->fileModifDate);                        // else
            });
        }
    }
#ifdef USE_THUMBNAILS
    else if (sortingField == SortingFieldEnum::FIELD_THUMBNAILS) {
        // we will compare thumbnails by :
        // 1) width
        // 2) height

        if (sortingDirection[4]) {
#ifdef USE_CUSTOM_SORTING_ICON
            headerFileThumbnails = tableHeaderAscendingIcon + headerFileThumbnails;
#endif  // USE_CUSTOM_SORTING_ICON
            std::sort(vFileInfosList.begin(), vFileInfosList.end(), [](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool {
                if (!a.use_count() || !b.use_count()) return false;
                if (a->fileType != b->fileType) return (a->fileType.isDir());  // directory in first
                if (a->thumbnailInfo.textureWidth == b->thumbnailInfo.textureWidth) return (a->thumbnailInfo.textureHeight < b->thumbnailInfo.textureHeight);
                return (a->thumbnailInfo.textureWidth < b->thumbnailInfo.textureWidth);
            });
        }

        else {
#ifdef USE_CUSTOM_SORTING_ICON
            headerFileThumbnails = tableHeaderDescendingIcon + headerFileThumbnails;
#endif  // USE_CUSTOM_SORTING_ICON
            std::sort(vFileInfosList.begin(), vFileInfosList.end(), [](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool {
                if (!a.use_count() || !b.use_count()) return false;
                if (a->fileType != b->fileType) return (!a->fileType.isDir());  // directory in last
                if (a->thumbnailInfo.textureWidth == b->thumbnailInfo.textureWidth) return (a->thumbnailInfo.textureHeight > b->thumbnailInfo.textureHeight);
                return (a->thumbnailInfo.textureWidth > b->thumbnailInfo.textureWidth);
            });
        }
    }
#endif  // USE_THUMBNAILS

    m_ApplyFilteringOnFileList(vFileDialogInternal, vFileInfosList, vFileInfosFilteredList);
}

bool IGFD::FileManager::m_CompleteFileInfosWithUserFileAttirbutes(const FileDialogInternal& vFileDialogInternal, const std::shared_ptr<FileInfos>& vInfos) {
    if (vFileDialogInternal.getDialogConfig().userFileAttributes != nullptr) {
        if (!vFileDialogInternal.getDialogConfig().userFileAttributes(vInfos.get(), vFileDialogInternal.getDialogConfig().userDatas)) {
            return false;  // the file will be ignored, so not added to the file list, so not displayed
        } else {
            if (!vInfos->fileType.isDir()) {
                vInfos->formatedFileSize = IGFD::Utils::FormatFileSize(vInfos->fileSize);
            }
        }
    }
    return true; // file will be added to file list, so displayed
}

void IGFD::FileManager::ClearFileLists() {
    m_FilteredFileList.clear();
    m_FileList.clear();
}

void IGFD::FileManager::ClearPathLists() {
    m_FilteredPathList.clear();
    m_PathList.clear();
}

void IGFD::FileManager::m_AddFile(const FileDialogInternal& vFileDialogInternal, const std::string& vPath, const std::string& vFileName, const FileType& vFileType) {
    auto infos = std::make_shared<FileInfos>();

    infos->filePath              = vPath;
    infos->fileNameExt           = vFileName;
    infos->fileNameExt_optimized = Utils::LowerCaseString(infos->fileNameExt);
    infos->fileType              = vFileType;

    if (infos->fileNameExt.empty() || (infos->fileNameExt == "." && !vFileDialogInternal.filterManager.dLGFilters.empty())) {  // filename empty or filename is the current dir '.' //-V807
        return;
    }

    if (infos->fileNameExt != ".." && (vFileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_DontShowHiddenFiles) && infos->fileNameExt[0] == '.') {  // dont show hidden files
        if (!vFileDialogInternal.filterManager.dLGFilters.empty() || (vFileDialogInternal.filterManager.dLGFilters.empty() && infos->fileNameExt != ".")) {  // except "." if in directory mode //-V728
            return;
        }
    }

    if (infos->FinalizeFileTypeParsing(vFileDialogInternal.filterManager.GetSelectedFilter().count_dots)) {
        if (!vFileDialogInternal.filterManager.IsCoveredByFilters(*infos.get(), (vFileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_CaseInsensitiveExtention) != 0)) {
            return;
        }
    }

    vFileDialogInternal.filterManager.m_FillFileStyle(infos);

    m_CompleteFileInfos(infos);

    if (m_CompleteFileInfosWithUserFileAttirbutes(vFileDialogInternal, infos)) {
        m_FileList.push_back(infos);
    }
}

void IGFD::FileManager::m_AddPath(const FileDialogInternal& vFileDialogInternal, const std::string& vPath, const std::string& vFileName, const FileType& vFileType) {
    if (!vFileType.isDir()) return;

    auto infos = std::make_shared<FileInfos>();

    infos->filePath              = vPath;
    infos->fileNameExt           = vFileName;
    infos->fileNameExt_optimized = Utils::LowerCaseString(infos->fileNameExt);
    infos->fileType              = vFileType;

    if (infos->fileNameExt.empty() || (infos->fileNameExt == "." && !vFileDialogInternal.filterManager.dLGFilters.empty())) {  // filename empty or filename is the current dir '.' //-V807
        return;
    }

    if (infos->fileNameExt != ".." && (vFileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_DontShowHiddenFiles) && infos->fileNameExt[0] == '.') {           // dont show hidden files
        if (!vFileDialogInternal.filterManager.dLGFilters.empty() || (vFileDialogInternal.filterManager.dLGFilters.empty() && infos->fileNameExt != ".")) {  // except "." if in directory mode //-V728
            return;
        }
    }

    vFileDialogInternal.filterManager.m_FillFileStyle(infos);

    m_CompleteFileInfos(infos);

    if (m_CompleteFileInfosWithUserFileAttirbutes(vFileDialogInternal, infos)) {
        m_PathList.push_back(infos);
    }
}

void IGFD::FileManager::ScanDir(const FileDialogInternal& vFileDialogInternal, const std::string& vPath) {
    std::string path = vPath;

    if (m_CurrentPathDecomposition.empty()) {
        SetCurrentDir(path);
    }

    if (!m_CurrentPathDecomposition.empty()) {
#ifdef _IGFD_WIN_
        if (path == fsRoot) {
            path += IGFD::Utils::GetPathSeparator();
        }
#endif  // _IGFD_WIN_

        ClearFileLists();

        const auto& files = m_FileSystemPtr->ScanDirectory(path);
        for (const auto& file : files) {
            m_AddFile(vFileDialogInternal, path, file.fileNameExt, file.fileType);
        }

        m_SortFields(vFileDialogInternal, m_FileList, m_FilteredFileList);
    }
}

void IGFD::FileManager::m_ScanDirForPathSelection(const FileDialogInternal& vFileDialogInternal, const std::string& vPath) {
    std::string path = vPath;

    if (!path.empty()) {
#ifdef _IGFD_WIN_
        if (path == fsRoot) path += IGFD::Utils::GetPathSeparator();
#endif  // _IGFD_WIN_

        ClearPathLists();

        const auto& files = m_FileSystemPtr->ScanDirectory(path);
        for (const auto& file : files) {
            if (file.fileType.isDir()) {
                m_AddPath(vFileDialogInternal, path, file.fileNameExt, file.fileType);
            }
        }

        m_SortFields(vFileDialogInternal, m_PathList, m_FilteredPathList);
    }
}

void IGFD::FileManager::m_OpenPathPopup(const FileDialogInternal& vFileDialogInternal, std::vector<std::string>::iterator vPathIter) {
    const auto path = ComposeNewPath(vPathIter);
    m_ScanDirForPathSelection(vFileDialogInternal, path);
    m_PopupComposedPath = vPathIter;
    ImGui::OpenPopup("IGFD_Path_Popup");
}

bool IGFD::FileManager::GetDrives() {
    auto drives = m_FileSystemPtr->GetDevicesList();
    if (!drives.empty()) {
        m_CurrentPath.clear();
        m_CurrentPathDecomposition.clear();
        ClearFileLists();
        for (auto& drive : drives) {
            auto info                   = std::make_shared<FileInfos>();
            info->fileNameExt           = drive.first;
            info->fileNameExt_optimized = Utils::LowerCaseString(drive.first);
            info->deviceInfos           = drive.second;
            info->fileType.SetContent(FileType::ContentType::Directory);
            if (!info->fileNameExt.empty()) {
                m_FileList.push_back(info);
            }
        }
        return true;
    }
    return false;
}

bool IGFD::FileManager::IsComposerEmpty() {
    return m_CurrentPathDecomposition.empty();
}

size_t IGFD::FileManager::GetComposerSize() {
    return m_CurrentPathDecomposition.size();
}

bool IGFD::FileManager::IsFileListEmpty() {
    return m_FileList.empty();
}

bool IGFD::FileManager::IsPathListEmpty() {
    return m_PathList.empty();
}

size_t IGFD::FileManager::GetFullFileListSize() {
    return m_FileList.size();
}

std::shared_ptr<IGFD::FileInfos> IGFD::FileManager::GetFullFileAt(size_t vIdx) {
    if (vIdx < m_FileList.size()) return m_FileList[vIdx];
    return nullptr;
}

bool IGFD::FileManager::IsFilteredListEmpty() {
    return m_FilteredFileList.empty();
}

bool IGFD::FileManager::IsPathFilteredListEmpty() {
    return m_FilteredPathList.empty();
}

size_t IGFD::FileManager::GetFilteredListSize() {
    return m_FilteredFileList.size();
}

size_t IGFD::FileManager::GetPathFilteredListSize() {
    return m_FilteredPathList.size();
}

std::shared_ptr<IGFD::FileInfos> IGFD::FileManager::GetFilteredFileAt(size_t vIdx) {
    if (vIdx < m_FilteredFileList.size()) return m_FilteredFileList[vIdx];
    return nullptr;
}

std::shared_ptr<IGFD::FileInfos> IGFD::FileManager::GetFilteredPathAt(size_t vIdx) {
    if (vIdx < m_FilteredPathList.size()) return m_FilteredPathList[vIdx];
    return nullptr;
}

std::vector<std::string>::iterator IGFD::FileManager::GetCurrentPopupComposedPath() {
    return m_PopupComposedPath;
}

bool IGFD::FileManager::IsFileNameSelected(const std::string& vFileName) {
    return m_SelectedFileNames.find(vFileName) != m_SelectedFileNames.end();
}

std::string IGFD::FileManager::GetBack() {
    return m_CurrentPathDecomposition.back();
}

void IGFD::FileManager::ClearComposer() {
    m_CurrentPathDecomposition.clear();
}

void IGFD::FileManager::ClearAll() {
    ClearComposer();
    ClearFileLists();
    ClearPathLists();
}
void IGFD::FileManager::ApplyFilteringOnFileList(const FileDialogInternal& vFileDialogInternal) {
    m_ApplyFilteringOnFileList(vFileDialogInternal, m_FileList, m_FilteredFileList);
}

void IGFD::FileManager::m_ApplyFilteringOnFileList(const FileDialogInternal& vFileDialogInternal, std::vector<std::shared_ptr<FileInfos> >& vFileInfosList, std::vector<std::shared_ptr<FileInfos> >& vFileInfosFilteredList) {
    vFileInfosFilteredList.clear();
    for (const auto& file : vFileInfosList) {
        if (!file.use_count()) continue;
        bool show = true;
        if (!file->SearchForTag(vFileDialogInternal.searchManager.searchTag))  // if search tag
            show = false;
        if (dLGDirectoryMode && !file->fileType.isDir()) show = false;
        if (show) vFileInfosFilteredList.push_back(file);
    }
}

void IGFD::FileManager::m_CompleteFileInfos(const std::shared_ptr<FileInfos>& vInfos) {
    if (!vInfos.use_count()) return;

    if (vInfos->fileNameExt != "." && vInfos->fileNameExt != "..") {
        // _stat struct :
        // dev_t     st_dev;     /* ID of device containing file */
        // ino_t     st_ino;     /* inode number */
        // mode_t    st_mode;    /* protection */
        // nlink_t   st_nlink;   /* number of hard links */
        // uid_t     st_uid;     /* user ID of owner */
        // gid_t     st_gid;     /* group ID of owner */
        // dev_t     st_rdev;    /* device ID (if special file) */
        // off_t     st_size;    /* total size, in bytes */
        // blksize_t st_blksize; /* blocksize for file system I/O */
        // blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
        // time_t    st_atime;   /* time of last access - not sure out of ntfs */
        // time_t    st_mtime;   /* time of last modification - not sure out of ntfs */
        // time_t    st_ctime;   /* time of last status change - not sure out of ntfs */

        std::string fpn;

        // FIXME: so the condition is always true?
        if (vInfos->fileType.isFile() || vInfos->fileType.isLinkToUnknown() || vInfos->fileType.isDir()) {
            fpn = vInfos->filePath + IGFD::Utils::GetPathSeparator() + vInfos->fileNameExt;
        }

        struct stat statInfos = {};
        char timebuf[100];
        int result = stat(fpn.c_str(), &statInfos);
        if (!result) {
            if (!vInfos->fileType.isDir()) {
                vInfos->fileSize         = (size_t)statInfos.st_size;
                vInfos->formatedFileSize = IGFD::Utils::FormatFileSize(vInfos->fileSize);
            }

            size_t len = 0;
#ifdef _MSC_VER
            struct tm _tm;
            errno_t err = localtime_s(&_tm, &statInfos.st_mtime);
            if (!err) len = strftime(timebuf, 99, DateTimeFormat, &_tm);
#else   // _MSC_VER
            struct tm* _tm = localtime(&statInfos.st_mtime);
            if (_tm) len = strftime(timebuf, 99, DateTimeFormat, _tm);
#endif  // _MSC_VER
            if (len) {
                vInfos->fileModifDate = std::string(timebuf, len);
            }
        }
    }
}

void IGFD::FileManager::m_RemoveFileNameInSelection(const std::string& vFileName) {
    m_SelectedFileNames.erase(vFileName);

    if (m_SelectedFileNames.size() == 1) {
        snprintf(fileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, "%s", vFileName.c_str());
    } else {
        snprintf(fileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, "%zu files Selected", m_SelectedFileNames.size());
    }
}

void IGFD::FileManager::m_m_AddFileNameInSelection(const std::string& vFileName, bool vSetLastSelectionFileName) {
    m_SelectedFileNames.emplace(vFileName);

    if (m_SelectedFileNames.size() == 1) {
        snprintf(fileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, "%s", vFileName.c_str());
    } else {
        snprintf(fileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, "%zu files Selected", m_SelectedFileNames.size());
    }

    if (vSetLastSelectionFileName) m_LastSelectedFileName = vFileName;
}

void IGFD::FileManager::SetCurrentDir(const std::string& vPath) {
    std::string path = vPath;
#ifdef _IGFD_WIN_
    if (fsRoot == path) path += IGFD::Utils::GetPathSeparator();
#endif  // _IGFD_WIN_

    bool dir_opened = m_FileSystemPtr->IsDirectory(path);
    if (!dir_opened) {
        path       = ".";
        dir_opened = m_FileSystemPtr->IsDirectory(path);
    }
    if (dir_opened) {
#ifdef _IGFD_WIN_
        DWORD numchar      = 0;
        std::wstring wpath = IGFD::Utils::UTF8Decode(path);
        numchar            = GetFullPathNameW(wpath.c_str(), 0, nullptr, nullptr);
        std::wstring fpath(numchar, 0);
        GetFullPathNameW(wpath.c_str(), numchar, (wchar_t*)fpath.data(), nullptr);
        std::string real_path = IGFD::Utils::UTF8Encode(fpath);
        while (real_path.back() == '\0')  // for fix issue we can have with std::string concatenation.. if there is a \0 at end
            real_path = real_path.substr(0, real_path.size() - 1U);
        if (!real_path.empty())
#elif defined(_IGFD_UNIX_)  // _IGFD_UNIX_ is _IGFD_WIN_ or APPLE
        char real_path[PATH_MAX];
        char* numchar = realpath(path.c_str(), real_path);
        if (numchar != nullptr)
#endif                      // _IGFD_WIN_
        {
            m_CurrentPath = std::move(real_path);
            if (m_CurrentPath[m_CurrentPath.size() - 1] == PATH_SEP) {
                m_CurrentPath = m_CurrentPath.substr(0, m_CurrentPath.size() - 1);
            }
            IGFD::Utils::SetBuffer(inputPathBuffer, MAX_PATH_BUFFER_SIZE, m_CurrentPath);
            m_CurrentPathDecomposition = IGFD::Utils::SplitStringToVector(m_CurrentPath, PATH_SEP, false);
#ifdef _IGFD_UNIX_  // _IGFD_UNIX_ is _IGFD_WIN_ or APPLE
            m_CurrentPathDecomposition.insert(m_CurrentPathDecomposition.begin(), IGFD::Utils::GetPathSeparator());
#endif  // _IGFD_UNIX_
            if (!m_CurrentPathDecomposition.empty()) {
#ifdef _IGFD_WIN_
                fsRoot = m_CurrentPathDecomposition[0];
#endif  // _IGFD_WIN_
            }
        }
    }
}

bool IGFD::FileManager::CreateDir(const std::string& vPath) {
    if (!vPath.empty()) {
        std::string path = m_CurrentPath + IGFD::Utils::GetPathSeparator() + vPath;
        return m_FileSystemPtr->CreateDirectoryIfNotExist(path);
    }
    return false;
}

std::string IGFD::FileManager::ComposeNewPath(std::vector<std::string>::iterator vIter) {
    std::string res;

    while (true) {
        if (!res.empty()) {
#ifdef _IGFD_WIN_
            res = *vIter + IGFD::Utils::GetPathSeparator() + res;
#elif defined(_IGFD_UNIX_)  // _IGFD_UNIX_ is _IGFD_WIN_ or APPLE
            if (*vIter == fsRoot)
                res = *vIter + res;
            else
                res = *vIter + PATH_SEP + res;
#endif                      // _IGFD_WIN_
        } else
            res = *vIter;

        if (vIter == m_CurrentPathDecomposition.begin()) {
#ifdef _IGFD_UNIX_  // _IGFD_UNIX_ is _IGFD_WIN_ or APPLE
            if (res[0] != PATH_SEP) res = PATH_SEP + res;
#else
            if (res.back() != PATH_SEP) res.push_back(PATH_SEP);
#endif  // defined(_IGFD_UNIX_)
            break;
        }

        --vIter;
    }

    return res;
}

bool IGFD::FileManager::SetPathOnParentDirectoryIfAny() {
    if (m_CurrentPathDecomposition.size() > 1) {
        m_CurrentPath = ComposeNewPath(m_CurrentPathDecomposition.end() - 2);
        return true;
    }
    return false;
}

std::string IGFD::FileManager::GetCurrentPath() {
    if (m_CurrentPath.empty()) m_CurrentPath = ".";
    return m_CurrentPath;
}

void IGFD::FileManager::SetCurrentPath(const std::string& vCurrentPath) {
    if (vCurrentPath.empty())
        m_CurrentPath = ".";
    else
        m_CurrentPath = vCurrentPath;
}

void IGFD::FileManager::SetDefaultFileName(const std::string& vFileName) {
    dLGDefaultFileName = vFileName;
    IGFD::Utils::SetBuffer(fileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, vFileName);
}

bool IGFD::FileManager::SelectDirectory(const std::shared_ptr<FileInfos>& vInfos) {
    if (!vInfos.use_count()) return false;

    bool pathClick = false;

    if (vInfos->fileNameExt == "..") {
        pathClick = SetPathOnParentDirectoryIfAny();
    } else {
        std::string newPath;

#ifdef __linux__
            if (fsRoot == m_CurrentPath)
                newPath = m_CurrentPath + vInfos->fileNameExt;
            else
#endif  // __linux__
                newPath = m_CurrentPath + IGFD::Utils::GetPathSeparator() + vInfos->fileNameExt;

        if (m_FileSystemPtr->IsDirectoryCanBeOpened(newPath)) {
            m_CurrentPath = newPath;  //-V820
            pathClick = true;
        }
    }

    return pathClick;
}

void IGFD::FileManager::SelectFileName(const FileDialogInternal& vFileDialogInternal, const std::shared_ptr<FileInfos>& vInfos) {
    if (!vInfos.use_count()) return;

    if (ImGui::IsKeyDown(ImGuiMod_Ctrl)) {
        if (dLGcountSelectionMax == 0)  // infinite selection
        {
            if (m_SelectedFileNames.find(vInfos->fileNameExt) == m_SelectedFileNames.end())  // not found +> add
            {
                m_m_AddFileNameInSelection(vInfos->fileNameExt, true);
            } else {  // found +> remove
                m_RemoveFileNameInSelection(vInfos->fileNameExt);
            }
        } else  // selection limited by size
        {
            if (m_SelectedFileNames.size() < dLGcountSelectionMax) {
                if (m_SelectedFileNames.find(vInfos->fileNameExt) == m_SelectedFileNames.end())  // not found +> add
                {
                    m_m_AddFileNameInSelection(vInfos->fileNameExt, true);
                } else {  // found +> remove
                    m_RemoveFileNameInSelection(vInfos->fileNameExt);
                }
            }
        }
    } else if (ImGui::IsKeyDown(ImGuiMod_Shift)) {
        if (dLGcountSelectionMax != 1) {
            m_SelectedFileNames.clear();
            // we will iterate filelist and get the last selection after the start selection
            bool startMultiSelection     = false;
            std::string fileNameToSelect = vInfos->fileNameExt;
            std::string savedLastSelectedFileName;  // for invert selection mode
            for (const auto& file : m_FileList) {
                if (!file.use_count()) continue;

                bool canTake = true;
                if (!file->SearchForTag(vFileDialogInternal.searchManager.searchTag)) canTake = false;
                if (canTake)  // if not filtered, we will take files who are filtered by the dialog
                {
                    if (file->fileNameExt == m_LastSelectedFileName) {
                        startMultiSelection = true;
                        m_m_AddFileNameInSelection(m_LastSelectedFileName, false);
                    } else if (startMultiSelection) {
                        if (dLGcountSelectionMax == 0)  // infinite selection
                        {
                            m_m_AddFileNameInSelection(file->fileNameExt, false);
                        } else {  // selection limited by size
                            if (m_SelectedFileNames.size() < dLGcountSelectionMax) {
                                m_m_AddFileNameInSelection(file->fileNameExt, false);
                            } else {
                                startMultiSelection = false;
                                if (!savedLastSelectedFileName.empty()) m_LastSelectedFileName = savedLastSelectedFileName;
                                break;
                            }
                        }
                    }

                    if (file->fileNameExt == fileNameToSelect) {
                        if (!startMultiSelection)  // we are before the last Selected FileName, so we must inverse
                        {
                            savedLastSelectedFileName = m_LastSelectedFileName;
                            m_LastSelectedFileName    = fileNameToSelect;
                            fileNameToSelect          = savedLastSelectedFileName;
                            startMultiSelection       = true;
                            m_m_AddFileNameInSelection(m_LastSelectedFileName, false);
                        } else {
                            startMultiSelection = false;
                            if (!savedLastSelectedFileName.empty()) m_LastSelectedFileName = savedLastSelectedFileName;
                            break;
                        }
                    }
                }
            }
        }
    } else {
        m_SelectedFileNames.clear();
        IGFD::Utils::ResetBuffer(fileNameBuffer);
        m_m_AddFileNameInSelection(vInfos->fileNameExt, true);
    }
}

void IGFD::FileManager::DrawDirectoryCreation(const FileDialogInternal& vFileDialogInternal) {
    if (vFileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_DisableCreateDirectoryButton) return;

    if (IMGUI_BUTTON(createDirButtonString)) {
        if (!m_CreateDirectoryMode) {
            m_CreateDirectoryMode = true;
            IGFD::Utils::ResetBuffer(directoryNameBuffer);
        }
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip(buttonCreateDirString);

    if (m_CreateDirectoryMode) {
        ImGui::SameLine();

        ImGui::PushItemWidth(100.0f);
        ImGui::InputText("##DirectoryFileName", directoryNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER);
        ImGui::PopItemWidth();

        ImGui::SameLine();

        if (IMGUI_BUTTON(okButtonString)) {
            std::string newDir = std::string(directoryNameBuffer);
            if (CreateDir(newDir)) {
                SetCurrentPath(m_CurrentPath + IGFD::Utils::GetPathSeparator() + newDir);
                OpenCurrentPath(vFileDialogInternal);
            }

            m_CreateDirectoryMode = false;
        }

        ImGui::SameLine();

        if (IMGUI_BUTTON(cancelButtonString)) {
            m_CreateDirectoryMode = false;
        }
    }

    ImGui::SameLine();
}

void IGFD::FileManager::DrawPathComposer(const FileDialogInternal& vFileDialogInternal) {
    if (IMGUI_BUTTON(resetButtonString)) {
        SetCurrentPath(".");
        OpenCurrentPath(vFileDialogInternal);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip(buttonResetPathString);

#ifdef _IGFD_WIN_
    /*ImGui::SameLine();

    if (IMGUI_BUTTON(drivesButtonString)) {
        drivesClicked = true;
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip(buttonDriveString);*/
#endif  // _IGFD_WIN_

    ImGui::SameLine();

    if (IMGUI_BUTTON(editPathButtonString)) {
        inputPathActivated = !inputPathActivated;
        if (inputPathActivated) {
            if (!m_CurrentPathDecomposition.empty()) {
                auto endIt    = m_CurrentPathDecomposition.end();
                m_CurrentPath = ComposeNewPath(--endIt);
                IGFD::Utils::SetBuffer(inputPathBuffer, MAX_PATH_BUFFER_SIZE, m_CurrentPath);
            }
        }
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip(buttonEditPathString);

    ImGui::SameLine();

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

    // show current path
    if (!m_CurrentPathDecomposition.empty()) {
        ImGui::SameLine();

        if (inputPathActivated) {
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##pathedition", inputPathBuffer, MAX_PATH_BUFFER_SIZE);
            ImGui::PopItemWidth();
        } else {
            int _id = 0;
            for (auto itPathDecomp = m_CurrentPathDecomposition.begin(); itPathDecomp != m_CurrentPathDecomposition.end(); ++itPathDecomp) {
                if (itPathDecomp != m_CurrentPathDecomposition.begin()) {
#if defined(CUSTOM_PATH_SPACING)
                    ImGui::SameLine(0, CUSTOM_PATH_SPACING);
#else
                    ImGui::SameLine();
#endif  // USE_CUSTOM_PATH_SPACING
                    if (!(vFileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_DisableQuickPathSelection)) {
#if defined(_IGFD_WIN_)
                        const char* sep = "\\";
#elif defined(_IGFD_UNIX_)
                        const char* sep = "/";
                        if (itPathDecomp != m_CurrentPathDecomposition.begin() + 1)
#endif
                        {
                            ImGui::PushID(_id++);
                            bool click = IMGUI_PATH_BUTTON(sep);
                            ImGui::PopID();

#if defined(CUSTOM_PATH_SPACING)
                            ImGui::SameLine(0, CUSTOM_PATH_SPACING);
#else
                            ImGui::SameLine();
#endif  // USE_CUSTOM_PATH_SPACING

                            if (click) {
                                m_OpenPathPopup(vFileDialogInternal, itPathDecomp - 1);
                            } else if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                                m_SetCurrentPath(itPathDecomp - 1);
                                break;
                            }
                        }
                    }
                }

                ImGui::PushID(_id++);
                bool click = IMGUI_PATH_BUTTON((*itPathDecomp).c_str());
                ImGui::PopID();
                if (click) {
                    m_CurrentPath = ComposeNewPath(itPathDecomp);
                    puPathClicked = true;
                    break;
                } else if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {  // activate input for path
                    m_SetCurrentPath(itPathDecomp);
                    break;
                }
            }
        }
    }
}

void IGFD::FileManager::m_SetCurrentPath(std::vector<std::string>::iterator vPathIter) {
    m_CurrentPath = ComposeNewPath(vPathIter);
    IGFD::Utils::SetBuffer(inputPathBuffer, MAX_PATH_BUFFER_SIZE, m_CurrentPath);
    inputPathActivated = true;
}

std::string IGFD::FileManager::GetResultingPath() {
    if (dLGDirectoryMode && m_SelectedFileNames.size() == 1) {  // if directory mode with selection 1
        std::string selectedDirectory = fileNameBuffer;
        std::string path              = m_CurrentPath;
        if (!selectedDirectory.empty() && selectedDirectory != ".") {
            path += IGFD::Utils::GetPathSeparator() + selectedDirectory;
        }
        return path;
    }
    return m_CurrentPath;  // if file mode
}

std::string IGFD::FileManager::GetResultingFileName(FileDialogInternal& vFileDialogInternal, IGFD_ResultMode vFlag) {
    if (!dLGDirectoryMode) {  // if not directory mode
        const auto& filename = std::string(fileNameBuffer);
        return vFileDialogInternal.filterManager.ReplaceExtentionWithCurrentFilterIfNeeded(filename, vFlag);
    }
    return "";  // directory mode
}

std::string IGFD::FileManager::GetResultingFilePathName(FileDialogInternal& vFileDialogInternal, IGFD_ResultMode vFlag) {
    if (!dLGDirectoryMode) {  // if not directory mode
        auto result          = GetResultingPath();
        const auto& filename = GetResultingFileName(vFileDialogInternal, vFlag);
        if (!filename.empty()) {
            if (m_FileSystemPtr != nullptr && m_FileSystemPtr->IsFileExist(filename)) {
                result = filename; // #144, exist file, so absolute, so return it (maybe set by user in inputText)
            } else { // #144, else concate path with current filename
#ifdef _IGFD_UNIX_
                if (fsRoot != result)
#endif  // _IGFD_UNIX_
                {
                    result += IGFD::Utils::GetPathSeparator();
                }
                result += filename;
            }
        }

        return result;
    }
    return "";  // file mode
}

std::map<std::string, std::string> IGFD::FileManager::GetResultingSelection(FileDialogInternal& vFileDialogInternal, IGFD_ResultMode vFlag) {
    std::map<std::string, std::string> res;
    for (const auto& selectedFileName : m_SelectedFileNames) {
        auto result = GetResultingPath();
#ifdef _IGFD_UNIX_
        if (fsRoot != result)
#endif  // _IGFD_UNIX_
        {
            result += IGFD::Utils::GetPathSeparator();
        }
        result += vFileDialogInternal.filterManager.ReplaceExtentionWithCurrentFilterIfNeeded(selectedFileName, vFlag);
        res[selectedFileName] = result;
    }
    return res;
}

#pragma endregion

#pragma region FileDialogInternal

void IGFD::FileDialogInternal::NewFrame() {
    canWeContinue             = true;   // reset flag for possibily validate the dialog
    isOk                      = false;  // reset dialog result
    fileManager.drivesClicked = false;
    fileManager.puPathClicked = false;

    needToExitDialog = false;

#ifdef USE_DIALOG_EXIT_WITH_KEY
    if (ImGui::IsKeyPressed(IGFD_EXIT_KEY)) {
        // we do that here with the data's defined at the last frame
        // because escape key can quit input activation and at the end of the frame all flag will be false
        // so we will detect nothing
        if (!(fileManager.inputPathActivated || searchManager.searchInputIsActive || fileInputIsActive || fileListViewIsActive)) {
            needToExitDialog = true;  // need to quit dialog
        }
    } else
#endif
    {
        searchManager.searchInputIsActive = false;
        fileInputIsActive                 = false;
        fileListViewIsActive              = false;
    }
}

void IGFD::FileDialogInternal::EndFrame() {
    // directory change
    if (fileManager.puPathClicked) {
        fileManager.OpenCurrentPath(*this);
    }

    if (fileManager.inputPathActivated) {
        auto gio = ImGui::GetIO();
        if (ImGui::IsKeyReleased(ImGuiKey_Enter)) {
            fileManager.SetCurrentPath(std::string(fileManager.inputPathBuffer));
            fileManager.OpenCurrentPath(*this);
            fileManager.inputPathActivated = false;
        }
        if (ImGui::IsKeyReleased(ImGuiKey_Escape)) {
            fileManager.inputPathActivated = false;
        }
    }
}

void IGFD::FileDialogInternal::ResetForNewDialog() {
}

void IGFD::FileDialogInternal::configureDialog(const std::string& vKey, const std::string& vTitle, const char* vFilters, const FileDialogConfig& vConfig) {
    m_DialogConfig = vConfig;
    ResetForNewDialog();
    dLGkey   = vKey;
    dLGtitle = vTitle;

    // treatment
    if (m_DialogConfig.sidePane == nullptr) {
        m_DialogConfig.sidePaneWidth = 0.0f;
    }

    if (m_DialogConfig.filePathName.empty()) {
        if (m_DialogConfig.path.empty()) {
            fileManager.dLGpath = fileManager.GetCurrentPath();
        } else {
            fileManager.dLGpath = m_DialogConfig.path;
        }
        fileManager.SetCurrentPath(m_DialogConfig.path);
        fileManager.dLGcountSelectionMax = (size_t)m_DialogConfig.countSelectionMax;
        fileManager.SetDefaultFileName(m_DialogConfig.fileName);
    } else {
        auto ps = fileManager.GetFileSystemInstance()->ParsePathFileName(m_DialogConfig.filePathName);
        if (ps.isOk) {
            fileManager.dLGpath = ps.path;
            fileManager.SetDefaultFileName(ps.name);
            filterManager.dLGdefaultExt = "." + ps.ext;
        } else {
            fileManager.dLGpath = fileManager.GetCurrentPath();
            fileManager.SetDefaultFileName("");
            filterManager.dLGdefaultExt.clear();
        }
    }

    filterManager.dLGdefaultExt.clear();
    filterManager.ParseFilters(vFilters);
    filterManager.SetSelectedFilterWithExt(filterManager.dLGdefaultExt);
    fileManager.SetCurrentPath(fileManager.dLGpath);
    fileManager.dLGDirectoryMode     = (vFilters == nullptr);
    fileManager.dLGcountSelectionMax = m_DialogConfig.countSelectionMax;  //-V101
    fileManager.ClearAll();
    showDialog = true;
}

const IGFD::FileDialogConfig& IGFD::FileDialogInternal::getDialogConfig() const {
    return m_DialogConfig;
}

IGFD::FileDialogConfig& IGFD::FileDialogInternal::getDialogConfigRef() {
    return m_DialogConfig;
}

#pragma endregion

#pragma endregion

#pragma region Optional Features

#pragma region ThumbnailFeature

IGFD::ThumbnailFeature::ThumbnailFeature() {
#ifdef USE_THUMBNAILS
    m_DisplayMode = DisplayModeEnum::FILE_LIST;
#endif
}

IGFD::ThumbnailFeature::~ThumbnailFeature() = default;

void IGFD::ThumbnailFeature::m_NewThumbnailFrame(FileDialogInternal& /*vFileDialogInternal*/) {
#ifdef USE_THUMBNAILS
    m_StartThumbnailFileDatasExtraction();
#endif
}

void IGFD::ThumbnailFeature::m_EndThumbnailFrame(FileDialogInternal& vFileDialogInternal) {
#ifdef USE_THUMBNAILS
    m_ClearThumbnails(vFileDialogInternal);
#else
    (void)vFileDialogInternal;
#endif
}

void IGFD::ThumbnailFeature::m_QuitThumbnailFrame(FileDialogInternal& vFileDialogInternal) {
#ifdef USE_THUMBNAILS
    m_StopThumbnailFileDatasExtraction();
    m_ClearThumbnails(vFileDialogInternal);
#else
    (void)vFileDialogInternal;
#endif
}

#ifdef USE_THUMBNAILS
void IGFD::ThumbnailFeature::m_StartThumbnailFileDatasExtraction() {
    const bool res = m_ThumbnailGenerationThread.use_count() && m_ThumbnailGenerationThread->joinable();
    if (!res) {
        m_IsWorking                 = true;
        m_CountFiles                = 0U;
        m_ThumbnailGenerationThread = std::shared_ptr<std::thread>(new std::thread(&IGFD::ThumbnailFeature::m_ThreadThumbnailFileDatasExtractionFunc, this), [this](std::thread* obj_ptr) {
            m_IsWorking = false;
            if (obj_ptr != nullptr) {
                m_ThumbnailFileDatasToGetCv.notify_all();
                obj_ptr->join();
            }
        });
    }
}

bool IGFD::ThumbnailFeature::m_StopThumbnailFileDatasExtraction() {
    const bool res = m_ThumbnailGenerationThread.use_count() && m_ThumbnailGenerationThread->joinable();
    if (res) {
        m_ThumbnailGenerationThread.reset();
    }
    return res;
}

void IGFD::ThumbnailFeature::m_ThreadThumbnailFileDatasExtractionFunc() {
    m_CountFiles = 0U;
    m_IsWorking  = true;
    // infinite loop while is thread working
    while (m_IsWorking) {
        std::unique_lock<std::mutex> thumbnailFileDatasToGetLock(m_ThumbnailFileDatasToGetMutex);
        m_ThumbnailFileDatasToGetCv.wait(thumbnailFileDatasToGetLock);
        if (!m_ThumbnailFileDatasToGet.empty()) {
            std::shared_ptr<FileInfos> file = nullptr;
            // get the first file in the list
            file = (*m_ThumbnailFileDatasToGet.begin());
            m_ThumbnailFileDatasToGet.pop_front();
            thumbnailFileDatasToGetLock.unlock();
            // retrieve datas of the texture file if its an image file
            if (file.use_count()) {
                if (file->fileType.isFile()) {  //-V522
                    //|| file->fileExtLevels == ".hdr" => format float so in few times
                    if (file->SearchForExts(".png,.bmp,.tga,.jpg,.jpeg,.gif,.psd,.pic,.ppm,.pgm", true)) {
                        auto fpn       = file->filePath + IGFD::Utils::GetPathSeparator() + file->fileNameExt;
                        int w          = 0;
                        int h          = 0;
                        int chans      = 0;
                        uint8_t* datas = stbi_load(fpn.c_str(), &w, &h, &chans, STBI_rgb_alpha);
                        if (datas) {
                            if (w && h) {
                                // resize with respect to glyph ratio
                                const float ratioX = (float)w / (float)h;
                                const float newX   = DisplayMode_ThumbailsList_ImageHeight * ratioX;
                                float newY         = w / ratioX;
                                if (newX < w) newY = DisplayMode_ThumbailsList_ImageHeight;
                                const auto newWidth       = (int)newX;
                                const auto newHeight      = (int)newY;
                                const auto newBufSize     = (size_t)(newWidth * newHeight * 4U);  //-V112 //-V1028
                                auto resizedData          = new uint8_t[newBufSize];
                                const int resizeSucceeded = stbir_resize_uint8(datas, w, h, 0, resizedData, newWidth, newHeight, 0, 4);  //-V112
                                if (resizeSucceeded) {
                                    auto th              = &file->thumbnailInfo;
                                    th->textureFileDatas = resizedData;
                                    th->textureWidth     = newWidth;
                                    th->textureHeight    = newHeight;
                                    th->textureChannels  = 4;  //-V112
                                    // we set that at least, because will launch the gpu creation of the texture in the
                                    // main thread
                                    th->isReadyToUpload = true;
                                    // need gpu loading
                                    m_AddThumbnailToCreate(file);
                                }
                            } else {
                                printf("image loading fail : w:%i h:%i c:%i\n", w, h, 4);  //-V112
                            }
                            stbi_image_free(datas);
                        }
                    }
                }
            }
        } else {
            thumbnailFileDatasToGetLock.unlock();
        }
    }
}

void IGFD::ThumbnailFeature::m_VariadicProgressBar(float fraction, const ImVec2& size_arg, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char TempBuffer[512];
    const int w = vsnprintf(TempBuffer, 511, fmt, args);
    va_end(args);
    if (w) {
        ImGui::ProgressBar(fraction, size_arg, TempBuffer);
    }
}

void IGFD::ThumbnailFeature::m_DrawThumbnailGenerationProgress() {
    if (m_ThumbnailGenerationThread.use_count() && m_ThumbnailGenerationThread->joinable()) {
        if (!m_ThumbnailFileDatasToGet.empty()) {
            const auto p = (float)((double)m_CountFiles / (double)m_ThumbnailFileDatasToGet.size());                     // read => no thread concurency issues
            m_VariadicProgressBar(p, ImVec2(50, 0), "%u/%u", m_CountFiles, (uint32_t)m_ThumbnailFileDatasToGet.size());  // read => no thread concurency issues
            ImGui::SameLine();
        }
    }
}

void IGFD::ThumbnailFeature::m_AddThumbnailToLoad(const std::shared_ptr<FileInfos>& vFileInfos) {
    if (vFileInfos.use_count()) {
        if (vFileInfos->fileType.isFile()) {
            //|| file->fileExtLevels == ".hdr" => format float so in few times
            if (vFileInfos->SearchForExts(".png,.bmp,.tga,.jpg,.jpeg,.gif,.psd,.pic,.ppm,.pgm", true)) {
                // write => thread concurency issues
                m_ThumbnailFileDatasToGetMutex.lock();
                m_ThumbnailFileDatasToGet.push_back(vFileInfos);
                vFileInfos->thumbnailInfo.isLoadingOrLoaded = true;
                m_ThumbnailFileDatasToGetMutex.unlock();
                m_ThumbnailFileDatasToGetCv.notify_all();
            }
        }
    }
}

void IGFD::ThumbnailFeature::m_AddThumbnailToCreate(const std::shared_ptr<FileInfos>& vFileInfos) {
    if (vFileInfos.use_count()) {
        // write => thread concurency issues
        m_ThumbnailToCreateMutex.lock();
        m_ThumbnailToCreate.push_back(vFileInfos);
        m_ThumbnailToCreateMutex.unlock();
    }
}

void IGFD::ThumbnailFeature::m_AddThumbnailToDestroy(const IGFD_Thumbnail_Info& vIGFD_Thumbnail_Info) {
    // write => thread concurency issues
    m_ThumbnailToDestroyMutex.lock();
    m_ThumbnailToDestroy.push_back(vIGFD_Thumbnail_Info);
    m_ThumbnailToDestroyMutex.unlock();
}

void IGFD::ThumbnailFeature::m_DrawDisplayModeToolBar() {
    if (IMGUI_RADIO_BUTTON(DisplayMode_FilesList_ButtonString, m_DisplayMode == DisplayModeEnum::FILE_LIST)) m_DisplayMode = DisplayModeEnum::FILE_LIST;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip(DisplayMode_FilesList_ButtonHelp);
    ImGui::SameLine();
    if (IMGUI_RADIO_BUTTON(DisplayMode_ThumbailsList_ButtonString, m_DisplayMode == DisplayModeEnum::THUMBNAILS_LIST)) m_DisplayMode = DisplayModeEnum::THUMBNAILS_LIST;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip(DisplayMode_ThumbailsList_ButtonHelp);
    ImGui::SameLine();
    /* todo
    if (IMGUI_RADIO_BUTTON(DisplayMode_ThumbailsGrid_ButtonString,
        m_DisplayMode == DisplayModeEnum::THUMBNAILS_GRID))
        m_DisplayMode = DisplayModeEnum::THUMBNAILS_GRID;
    if (ImGui::IsItemHovered())	ImGui::SetTooltip(DisplayMode_ThumbailsGrid_ButtonHelp);
    ImGui::SameLine();
    */
    m_DrawThumbnailGenerationProgress();
}

void IGFD::ThumbnailFeature::m_ClearThumbnails(FileDialogInternal& vFileDialogInternal) {
    // directory wil be changed so the file list will be erased
    if (vFileDialogInternal.fileManager.puPathClicked) {
        size_t count = vFileDialogInternal.fileManager.GetFullFileListSize();
        for (size_t idx = 0U; idx < count; idx++) {
            auto file = vFileDialogInternal.fileManager.GetFullFileAt(idx);
            if (file.use_count()) {
                if (file->thumbnailInfo.isReadyToDisplay)  //-V522
                {
                    m_AddThumbnailToDestroy(file->thumbnailInfo);
                }
            }
        }
    }
}

void IGFD::ThumbnailFeature::SetCreateThumbnailCallback(const CreateThumbnailFun& vCreateThumbnailFun) {
    m_CreateThumbnailFun = vCreateThumbnailFun;
}

void IGFD::ThumbnailFeature::SetDestroyThumbnailCallback(const DestroyThumbnailFun& vCreateThumbnailFun) {
    m_DestroyThumbnailFun = vCreateThumbnailFun;
}

void IGFD::ThumbnailFeature::ManageGPUThumbnails() {
    if (m_CreateThumbnailFun) {
        m_ThumbnailToCreateMutex.lock();
        if (!m_ThumbnailToCreate.empty()) {
            for (const auto& file : m_ThumbnailToCreate) {
                if (file.use_count()) {
                    m_CreateThumbnailFun(&file->thumbnailInfo);
                }
            }
            m_ThumbnailToCreate.clear();
        }
        m_ThumbnailToCreateMutex.unlock();
    } else {
        printf(
            "No Callback found for create texture\nYou need to define the callback with a call to "
            "SetCreateThumbnailCallback\n");
    }

    if (m_DestroyThumbnailFun) {
        m_ThumbnailToDestroyMutex.lock();
        if (!m_ThumbnailToDestroy.empty()) {
            for (auto thumbnail : m_ThumbnailToDestroy) {
                m_DestroyThumbnailFun(&thumbnail);
            }
            m_ThumbnailToDestroy.clear();
        }
        m_ThumbnailToDestroyMutex.unlock();
    } else {
        printf(
            "No Callback found for destroy texture\nYou need to define the callback with a call to "
            "SetCreateThumbnailCallback\n");
    }
}

#endif  // USE_THUMBNAILS

#pragma endregion

#pragma region PlacesFeature

IGFD::PlacesFeature::PlacesFeature() {
#ifdef USE_PLACES_FEATURE
    m_PlacesPaneWidth = defaultPlacePaneWith;
    m_PlacesPaneShown = PLACES_PANE_DEFAULT_SHOWN;
#endif  // USE_PLACES_FEATURE
}

#ifdef USE_PLACES_FEATURE
void IGFD::PlacesFeature::m_InitPlaces(FileDialogInternal& vFileDialogInternal) {
#ifdef USE_PLACES_BOOKMARKS
    AddPlacesGroup(placesBookmarksGroupName, placesBookmarksDisplayOrder, true, PLACES_BOOKMARK_DEFAULT_OPEPEND);
#endif  // USE_PLACES_BOOKMARK
#ifdef USE_PLACES_DEVICES
    AddPlacesGroup(placesDevicesGroupName, placesDevicesDisplayOrder, false, PLACES_DEVICES_DEFAULT_OPEPEND);
    auto devices_ptr = GetPlacesGroupPtr(placesDevicesGroupName);
    if (devices_ptr != nullptr && vFileDialogInternal.fileManager.GetFileSystemInstance() != nullptr) {
        const auto& devices = vFileDialogInternal.fileManager.GetFileSystemInstance()->GetDevicesList();
        for (const auto& device : devices) {
            devices_ptr->AddPlace(device.first + " " + device.second, device.first + IGFD::Utils::GetPathSeparator(), false);
        }
        devices_ptr = nullptr;
    }
#endif  // USE_PLACES_DEVICES
}

void IGFD::PlacesFeature::m_DrawPlacesButton() {
    IMGUI_TOGGLE_BUTTON(placesButtonString, &m_PlacesPaneShown);
    if (ImGui::IsItemHovered()) ImGui::SetTooltip(placesButtonHelpString);
}

bool IGFD::PlacesFeature::m_DrawPlacesPane(FileDialogInternal& vFileDialogInternal, const ImVec2& vSize) {
    bool res = false;
    ImGui::BeginChild("##placespane", vSize);
    for (const auto& group : m_OrderedGroups) {
        auto group_ptr = group.second.lock();
        if (group_ptr != nullptr) {
            if (ImGui::CollapsingHeader(group_ptr->name.c_str(), group_ptr->collapsingHeaderFlag)) {
                ImGui::BeginChild(group_ptr->name.c_str(), ImVec2(0, 0), ImGuiChildFlags_AutoResizeY);
                if (group_ptr->canBeEdited) {
                    ImGui::PushID(group_ptr.get());
                    if (IMGUI_BUTTON(addPlaceButtonString "##ImGuiFileDialogAddPlace")) {
                        if (!vFileDialogInternal.fileManager.IsComposerEmpty()) {
                            group_ptr->AddPlace(vFileDialogInternal.fileManager.GetBack(), vFileDialogInternal.fileManager.GetCurrentPath(), true);
                        }
                    }
                    if (group_ptr->selectedPlaceForEdition >= 0 && group_ptr->selectedPlaceForEdition < (int)group_ptr->places.size()) {
                        ImGui::SameLine();
                        if (IMGUI_BUTTON(removePlaceButtonString "##ImGuiFileDialogRemovePlace")) {
                            group_ptr->places.erase(group_ptr->places.begin() + group_ptr->selectedPlaceForEdition);
                            if (group_ptr->selectedPlaceForEdition == (int)group_ptr->places.size()) {
                                --group_ptr->selectedPlaceForEdition;
                            }
                        }
                        if (group_ptr->selectedPlaceForEdition >= 0 && group_ptr->selectedPlaceForEdition < (int)group_ptr->places.size()) {
                            ImGui::SameLine();
                            if (IMGUI_BUTTON(validatePlaceButtonString "##ImGuiFileDialogOkPlace")) {
                                group_ptr->places[(size_t)group_ptr->selectedPlaceForEdition].name = std::string(group_ptr->editBuffer);
                                group_ptr->selectedPlaceForEdition                                 = -1;
                            }
                            ImGui::SameLine();
                            ImGui::PushItemWidth(vSize.x - ImGui::GetCursorPosX());
                            if (ImGui::InputText("##ImGuiFileDialogPlaceEdit", group_ptr->editBuffer, MAX_FILE_DIALOG_NAME_BUFFER)) {
                                group_ptr->places[(size_t)group_ptr->selectedPlaceForEdition].name = std::string(group_ptr->editBuffer);
                            }
                            ImGui::PopItemWidth();
                        }
                    }
                    ImGui::PopID();
                    ImGui::Separator();
                }
                if (!group_ptr->places.empty()) {
                    const auto& current_path = vFileDialogInternal.fileManager.GetCurrentPath();
                    group_ptr->clipper.Begin((int)group_ptr->places.size(), ImGui::GetTextLineHeightWithSpacing());
                    while (group_ptr->clipper.Step()) {
                        for (int i = group_ptr->clipper.DisplayStart; i < group_ptr->clipper.DisplayEnd; i++) {
                            if (i < 0) {
                                continue;
                            }
                            const PlaceStruct& place = group_ptr->places[(size_t)i];
                            if (place.thickness > 0.0f) {
                                ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, place.thickness);
                            } else {
                                ImGui::PushID(i);
                                std::string place_name = place.name;
                                if (!place.style.icon.empty()) {
                                    place_name = place.style.icon + " " + place_name;
                                }
                                if (group_ptr->canBeEdited) {
                                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                                    if (ImGui::SmallButton(editPlaceButtonString "##ImGuiFileDialogPlaceEditButton")) {
                                        group_ptr->selectedPlaceForEdition = i;
                                        IGFD::Utils::ResetBuffer(group_ptr->editBuffer);
                                        IGFD::Utils::AppendToBuffer(group_ptr->editBuffer, MAX_FILE_DIALOG_NAME_BUFFER, place.name);
                                    }
                                    ImGui::PopStyleVar();
                                    ImGui::PopStyleColor();
                                    ImGui::SameLine();
                                }
                                if (ImGui::Selectable(place_name.c_str(), current_path == place.path || group_ptr->selectedPlaceForEdition == i, ImGuiSelectableFlags_AllowDoubleClick)) {  // select if path is current
                                    if (ImGui::IsMouseDoubleClicked(0)) {
                                        group_ptr->selectedPlaceForEdition = -1; // stop edition
                                        // apply path
                                        vFileDialogInternal.fileManager.SetCurrentPath(place.path);
                                        vFileDialogInternal.fileManager.OpenCurrentPath(vFileDialogInternal);
                                        res = true;
                                    }
                                }
                                ImGui::PopID();
                                if (ImGui::IsItemHovered()) {
                                    ImGui::SetTooltip("%s", place.path.c_str());
                                }
                            }                            
                        }
                    }
                    group_ptr->clipper.End();
                }
                ImGui::EndChild();
            }
        }
    }
    ImGui::EndChild();
    return res;
}

std::string IGFD::PlacesFeature::SerializePlaces(const bool& vForceSerialisationForAll) {
    std::string res;
    size_t idx = 0;
    for (const auto& group : m_Groups) {
        if (group.second->canBeSaved) {
            // ## is used because reserved by imgui, so an input text cannot have ##
            res += "###" + group.first + "###";
            for (const auto& place : group.second->places) {
                if (place.canBeSaved) {
                    if (idx++ != 0) res += "##";
                    res += place.name + "##" + place.path;
                }
            }
        }
    }
    return res;
}

void IGFD::PlacesFeature::DeserializePlaces(const std::string& vPlaces) {
    if (!vPlaces.empty()) {
        const auto& groups = IGFD::Utils::SplitStringToVector(vPlaces, "###", false);
        if (groups.size() > 1) {
            for (size_t i = 0; i < groups.size(); i += 2) {
                auto group_ptr = GetPlacesGroupPtr(groups[i]);
                if (group_ptr != nullptr) {
                    const auto& places = IGFD::Utils::SplitStringToVector(groups[i + 1], "##", false);
                    if (places.size() > 1) {
                        for (size_t j = 0; j < places.size(); j += 2) {
                            group_ptr->AddPlace(places[j], places[j + 1], true);  // was saved so we set canBeSaved to true
                        }
                    }
                }
            }
        }
    }
}

bool IGFD::PlacesFeature::AddPlacesGroup(const std::string& vGroupName, const size_t& vDisplayOrder, const bool& vCanBeEdited, const bool& vOpenedByDefault) {
    if (vGroupName.empty()) {
        return false;
    }
    auto group_ptr          = std::make_shared<GroupStruct>();
    group_ptr->displayOrder = vDisplayOrder;
    group_ptr->name         = vGroupName;
    group_ptr->defaultOpened = vOpenedByDefault;
    if (group_ptr->defaultOpened) {
        group_ptr->collapsingHeaderFlag = ImGuiTreeNodeFlags_DefaultOpen;
    }
    group_ptr->canBeSaved = group_ptr->canBeEdited = vCanBeEdited; // can be user edited mean can be saved
    m_Groups[vGroupName]     = group_ptr;
    m_OrderedGroups[group_ptr->displayOrder] = group_ptr; // an exisitng display order will be overwrote for code simplicity
    return true;
}

bool IGFD::PlacesFeature::RemovePlacesGroup(const std::string& vGroupName) {
    for (auto it = m_Groups.begin(); it != m_Groups.end(); ++it) {
        if ((*it).second->name == vGroupName) {
            m_Groups.erase(it);
            return true;
        }
    }
    return false;
}

IGFD::PlacesFeature::GroupStruct* IGFD::PlacesFeature::GetPlacesGroupPtr(const std::string& vGroupName) {
    if (m_Groups.find(vGroupName) != m_Groups.end()) {
        return m_Groups.at(vGroupName).get();
    }
    return nullptr;
}

bool IGFD::PlacesFeature::GroupStruct::AddPlace(const std::string& vPlaceName, const std::string& vPlacePath, const bool& vCanBeSaved, const FileStyle& vStyle) {
    if (vPlaceName.empty() || vPlacePath.empty()) {
        return false;
    }
    canBeSaved |= vCanBeSaved; // if one place must be saved so we mark the group to be saved
    PlaceStruct place;
    place.name       = vPlaceName;
    place.path       = vPlacePath;
    place.canBeSaved = vCanBeSaved;
    place.style      = vStyle;
    places.push_back(place);
    return true;
}

void IGFD::PlacesFeature::GroupStruct::AddPlaceSeparator(const float& vThickness) {
    PlaceStruct place;
    place.thickness = vThickness;
    places.push_back(place);
}

bool IGFD::PlacesFeature::GroupStruct::RemovePlace(const std::string& vPlaceName) {
    if (vPlaceName.empty()) {
        return false;
    }
    for (auto places_it = places.begin(); places_it != places.end(); ++places_it) {
        if ((*places_it).name == vPlaceName) {
            places.erase(places_it);
            return true;
        }
    }
    return false;
}
#endif  // USE_PLACES_FEATURE

#pragma endregion

#pragma region KeyExplorerFeature

IGFD::KeyExplorerFeature::KeyExplorerFeature() = default;

#ifdef USE_EXPLORATION_BY_KEYS
bool IGFD::KeyExplorerFeature::m_LocateItem_Loop(FileDialogInternal& vFileDialogInternal, ImWchar vC) {
    bool found = false;

    auto& fdi = vFileDialogInternal.fileManager;
    if (!fdi.IsFilteredListEmpty()) {
        auto countFiles = fdi.GetFilteredListSize();
        for (size_t i = m_LocateFileByInputChar_lastFileIdx; i < countFiles; i++) {
            auto nfo = fdi.GetFilteredFileAt(i);
            if (nfo.use_count()) {
                if (nfo->fileNameExt_optimized[0] == vC ||  // lower case search //-V522
                    nfo->fileNameExt[0] == vC)              // maybe upper case search
                {
                    // float p = ((float)i) * ImGui::GetTextLineHeightWithSpacing();
                    float p = (float)((double)i / (double)countFiles) * ImGui::GetScrollMaxY();
                    ImGui::SetScrollY(p);
                    m_LocateFileByInputChar_lastFound   = true;
                    m_LocateFileByInputChar_lastFileIdx = i;
                    m_StartFlashItem(m_LocateFileByInputChar_lastFileIdx);

                    auto infos = fdi.GetFilteredFileAt(m_LocateFileByInputChar_lastFileIdx);
                    if (infos.use_count()) {
                        if (infos->fileType.isDir())  //-V522
                        {
                            if (fdi.dLGDirectoryMode)  // directory chooser
                            {
                                fdi.SelectFileName(vFileDialogInternal, infos);
                            }
                        } else {
                            fdi.SelectFileName(vFileDialogInternal, infos);
                        }

                        found = true;
                        break;
                    }
                }
            }
        }
    }

    return found;
}

void IGFD::KeyExplorerFeature::m_LocateByInputKey(FileDialogInternal& vFileDialogInternal) {
    ImGuiContext& g = *GImGui;
    auto& fdi       = vFileDialogInternal.fileManager;
    if (!g.ActiveId && !fdi.IsFilteredListEmpty()) {
        auto& queueChar = ImGui::GetIO().InputQueueCharacters;
        auto countFiles = fdi.GetFilteredListSize();

        // point by char
        if (!queueChar.empty()) {
            ImWchar c = queueChar.back();
            if (m_LocateFileByInputChar_InputQueueCharactersSize != queueChar.size()) {
                if (c == m_LocateFileByInputChar_lastChar)  // next file starting with same char until
                {
                    if (m_LocateFileByInputChar_lastFileIdx < countFiles - 1U)
                        m_LocateFileByInputChar_lastFileIdx++;
                    else
                        m_LocateFileByInputChar_lastFileIdx = 0;
                }

                if (!m_LocateItem_Loop(vFileDialogInternal, c)) {
                    // not found, loop again from 0 this time
                    m_LocateFileByInputChar_lastFileIdx = 0;
                    m_LocateItem_Loop(vFileDialogInternal, c);
                }

                m_LocateFileByInputChar_lastChar = c;
            }
        }

        m_LocateFileByInputChar_InputQueueCharactersSize = queueChar.size();
    }
}

void IGFD::KeyExplorerFeature::m_ExploreWithkeys(FileDialogInternal& vFileDialogInternal, ImGuiID vListViewID) {
    auto& fdi = vFileDialogInternal.fileManager;
    if (!fdi.IsFilteredListEmpty()) {
        bool canWeExplore = false;
        bool hasNav       = (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard);

        ImGuiContext& g = *GImGui;
        if (!hasNav && !g.ActiveId)  // no nav and no activated inputs
            canWeExplore = true;

        if (g.NavId && g.NavId == vListViewID) {
            if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter) || ImGui::IsKeyPressed(ImGuiKey_Space)) {
                ImGui::ActivateItemByID(vListViewID);
                ImGui::SetActiveID(vListViewID, g.CurrentWindow);
            }
        }

        if (vListViewID == g.LastActiveId - 1)  // if listview id is the last acticated nav id (ImGui::ActivateItemByID(vListViewID);)
            canWeExplore = true;

        if (canWeExplore && ImGui::IsWindowFocused()) {
            if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
                ImGui::ClearActiveID();
                g.LastActiveId = 0;
            }

            auto countFiles = fdi.GetFilteredListSize();

            // explore
            bool exploreByKey     = false;
            bool enterInDirectory = false;
            bool exitDirectory    = false;

            if ((hasNav && ImGui::IsKeyPressed(ImGuiKey_UpArrow)) || (!hasNav && ImGui::IsKeyPressed(ImGuiKey_UpArrow))) {
                exploreByKey = true;
                if (m_LocateFileByInputChar_lastFileIdx > 0)
                    m_LocateFileByInputChar_lastFileIdx--;
                else
                    m_LocateFileByInputChar_lastFileIdx = countFiles - 1U;
            } else if ((hasNav && ImGui::IsKeyPressed(ImGuiKey_DownArrow)) || (!hasNav && ImGui::IsKeyPressed(ImGuiKey_DownArrow))) {
                exploreByKey = true;
                if (m_LocateFileByInputChar_lastFileIdx < countFiles - 1U)
                    m_LocateFileByInputChar_lastFileIdx++;
                else
                    m_LocateFileByInputChar_lastFileIdx = 0U;
            } else if (ImGui::IsKeyReleased(ImGuiKey_Enter)) {
                exploreByKey     = true;
                enterInDirectory = true;
            } else if (ImGui::IsKeyReleased(ImGuiKey_Backspace)) {
                exploreByKey  = true;
                exitDirectory = true;
            }

            if (exploreByKey) {
                // float totalHeight = m_FilteredFileList.size() * ImGui::GetTextLineHeightWithSpacing();
                float p = (float)((double)m_LocateFileByInputChar_lastFileIdx / (double)(countFiles - 1U)) * ImGui::GetScrollMaxY();  // seems not udpated in tables version outside tables
                // float p = ((float)locateFileByInputChar_lastFileIdx) * ImGui::GetTextLineHeightWithSpacing();
                ImGui::SetScrollY(p);
                m_StartFlashItem(m_LocateFileByInputChar_lastFileIdx);

                auto infos = fdi.GetFilteredFileAt(m_LocateFileByInputChar_lastFileIdx);
                if (infos.use_count()) {
                    if (infos->fileType.isDir())  //-V522
                    {
                        if (!fdi.dLGDirectoryMode || enterInDirectory) {
                            if (enterInDirectory) {
                                if (fdi.SelectDirectory(infos)) {
                                    // changement de repertoire
                                    vFileDialogInternal.fileManager.OpenCurrentPath(vFileDialogInternal);
                                    if (m_LocateFileByInputChar_lastFileIdx > countFiles - 1U) {
                                        m_LocateFileByInputChar_lastFileIdx = 0;
                                    }
                                }
                            }
                        } else  // directory chooser
                        {
                            fdi.SelectFileName(vFileDialogInternal, infos);
                        }
                    } else {
                        fdi.SelectFileName(vFileDialogInternal, infos);

                        if (enterInDirectory) {
                            vFileDialogInternal.isOk = true;
                        }
                    }

                    if (exitDirectory) {
                        auto nfo         = std::make_shared<FileInfos>();
                        nfo->fileNameExt = "..";

                        if (fdi.SelectDirectory(nfo)) {
                            // changement de repertoire
                            vFileDialogInternal.fileManager.OpenCurrentPath(vFileDialogInternal);
                            if (m_LocateFileByInputChar_lastFileIdx > countFiles - 1U) {
                                m_LocateFileByInputChar_lastFileIdx = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}

bool IGFD::KeyExplorerFeature::m_FlashableSelectable(const char* label, bool selected, ImGuiSelectableFlags flags, bool vFlashing, const ImVec2& size_arg) {
    using namespace ImGui;

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g         = *GImGui;
    const ImGuiStyle& style = g.Style;

    // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
    ImGuiID id        = window->GetID(label);
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize(size, 0.0f);

    // Fill horizontal space
    // We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
    const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
    const float min_x           = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
    const float max_x           = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
    if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth)) size.x = ImMax(label_size.x, max_x - min_x);

    // Text stays at the submission position, but bounding box may be extended on both sides
    const ImVec2 text_min = pos;
    const ImVec2 text_max(min_x + size.x, pos.y + size.y);

    // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
    ImRect bb(min_x, pos.y, text_max.x, text_max.y);
    if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0) {
        const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
        const float spacing_y = style.ItemSpacing.y;
        const float spacing_L = IM_TRUNC(spacing_x * 0.50f);
        const float spacing_U = IM_TRUNC(spacing_y * 0.50f);
        bb.Min.x -= spacing_L;
        bb.Min.y -= spacing_U;
        bb.Max.x += (spacing_x - spacing_L);
        bb.Max.y += (spacing_y - spacing_U);
    }
    // if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

    // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackgroundChannel for every Selectable..
    const float backup_clip_rect_min_x = window->ClipRect.Min.x;
    const float backup_clip_rect_max_x = window->ClipRect.Max.x;
    if (span_all_columns) {
        window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
        window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
    }

    const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
    const bool item_add      = ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
    if (span_all_columns) {
        window->ClipRect.Min.x = backup_clip_rect_min_x;
        window->ClipRect.Max.x = backup_clip_rect_max_x;
    }

    if (!item_add) return false;

    const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
    if (disabled_item && !disabled_global)  // Only testing this as an optimization
        BeginDisabled();

    // FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
    // which would be advantageous since most selectable are not selected.
    if (span_all_columns) {
        if (g.CurrentTable)
            TablePushBackgroundChannel();
        else if (window->DC.CurrentColumns)
            PushColumnsBackground();
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasClipRect;
        g.LastItemData.ClipRect = window->ClipRect;
    }

    // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
    ImGuiButtonFlags button_flags = 0;
    if (flags & ImGuiSelectableFlags_NoHoldingActiveID) {
        button_flags |= ImGuiButtonFlags_NoHoldingActiveId;
    }
    if (flags & ImGuiSelectableFlags_NoSetKeyOwner) {
        button_flags |= ImGuiButtonFlags_NoSetKeyOwner;
    }
    if (flags & ImGuiSelectableFlags_SelectOnClick) {
        button_flags |= ImGuiButtonFlags_PressedOnClick;
    }
    if (flags & ImGuiSelectableFlags_SelectOnRelease) {
        button_flags |= ImGuiButtonFlags_PressedOnRelease;
    }
    if (flags & ImGuiSelectableFlags_AllowDoubleClick) {
        button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
    }
    if ((flags & ImGuiSelectableFlags_AllowOverlap) || (g.LastItemData.InFlags & ImGuiItemFlags_AllowOverlap)) {
        button_flags |= ImGuiButtonFlags_AllowOverlap;
    }

    const bool was_selected = selected;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

    // Auto-select when moved into
    // - This will be more fully fleshed in the range-select branch
    // - This is not exposed as it won't nicely work with some user side handling of shift/control
    // - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
    //   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g. BeginSelection() calling PushFocusScope())
    //   - (2) usage will fail with clipped items
    //   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
    if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId)
        if (g.NavJustMovedToId == id) selected = pressed = true;

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover))) {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent) {
            SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel(window, bb));  // (bb == NavRect)
            g.NavDisableHighlight = true;
        }
    }
    if (pressed) MarkItemEdited(id);

    // In this branch, Selectable() cannot toggle the selection so this will never trigger.
    if (selected != was_selected)  //-V547
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

    //////////////////////////////////////////////////////////////////
    // this function copy ImGui::Selectable just for this line....
    hovered |= vFlashing;
    //////////////////////////////////////////////////////////////////

    // Render
    if (hovered || selected) {
        const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
        RenderFrame(bb.Min, bb.Max, col, false, 0.0f);
    }
    if (g.NavId == id) RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_Compact | ImGuiNavHighlightFlags_NoRounding);

    if (span_all_columns) {
        if (g.CurrentTable)
            TablePopBackgroundChannel();
        else if (window->DC.CurrentColumns)
            PopColumnsBackground();
    }

    RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);

    // Automatically close popups
    if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup)) CloseCurrentPopup();

    if (disabled_item && !disabled_global) EndDisabled();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;  //-V1020
}

void IGFD::KeyExplorerFeature::m_StartFlashItem(size_t vIdx) {
    m_FlashAlpha  = 1.0f;
    m_FlashedItem = vIdx;
}

bool IGFD::KeyExplorerFeature::m_BeginFlashItem(size_t vIdx) {
    bool res = false;

    if (m_FlashedItem == vIdx && std::abs(m_FlashAlpha - 0.0f) > 0.00001f) {
        m_FlashAlpha -= m_FlashAlphaAttenInSecs * ImGui::GetIO().DeltaTime;
        if (m_FlashAlpha < 0.0f) m_FlashAlpha = 0.0f;

        ImVec4 hov = ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered);
        hov.w      = m_FlashAlpha;
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, hov);
        res = true;
    }

    return res;
}

void IGFD::KeyExplorerFeature::m_EndFlashItem() {
    ImGui::PopStyleColor();
}

void IGFD::KeyExplorerFeature::SetFlashingAttenuationInSeconds(float vAttenValue) {
    m_FlashAlphaAttenInSecs = 1.0f / ImMax(vAttenValue, 0.01f);
}
#endif  // USE_EXPLORATION_BY_KEYS

#pragma endregion

#pragma endregion

#pragma region FileDialog

IGFD::FileDialog::FileDialog() : PlacesFeature(), KeyExplorerFeature(), ThumbnailFeature() {
#ifdef USE_PLACES_FEATURE
    m_InitPlaces(m_FileDialogInternal);
#endif
}
IGFD::FileDialog::~FileDialog() = default;

//////////////////////////////////////////////////////////////////////////////////////////////////
///// FILE DIALOG STANDARD DIALOG ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

// path and fileNameExt can be specified
void IGFD::FileDialog::OpenDialog(const std::string& vKey, const std::string& vTitle, const char* vFilters, const FileDialogConfig& vConfig) {
    if (m_FileDialogInternal.showDialog)  // if already opened, quit
        return;
    m_FileDialogInternal.configureDialog(vKey, vTitle, vFilters, vConfig);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
///// FILE DIALOG DISPLAY FUNCTION ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

bool IGFD::FileDialog::Display(const std::string& vKey, ImGuiWindowFlags vFlags, ImVec2 vMinSize, ImVec2 vMaxSize) {
    bool res = false;

    if (m_FileDialogInternal.showDialog && m_FileDialogInternal.dLGkey == vKey) {
        if (m_FileDialogInternal.puUseCustomLocale) setlocale(m_FileDialogInternal.localeCategory, m_FileDialogInternal.localeBegin.c_str());

        auto& fdFile   = m_FileDialogInternal.fileManager;
        auto& fdFilter = m_FileDialogInternal.filterManager;

        static ImGuiWindowFlags flags;  // todo: not a good solution for multi instance, to fix

        // to be sure than only one dialog is displayed per frame
        ImGuiContext& g = *GImGui;
        if (g.FrameCount == m_FileDialogInternal.lastImGuiFrameCount)  // one instance was displayed this frame before
                                                                       // for this key +> quit
            return res;
        m_FileDialogInternal.lastImGuiFrameCount = g.FrameCount;  // mark this instance as used this frame

        std::string name = m_FileDialogInternal.dLGtitle + "##" + m_FileDialogInternal.dLGkey;
        if (m_FileDialogInternal.name != name) {
            fdFile.ClearComposer();
            fdFile.ClearFileLists();
            flags = vFlags;
        }

        m_NewFrame();

#ifdef IMGUI_HAS_VIEWPORT
        if (!ImGui::GetIO().ConfigViewportsNoDecoration) {
            // https://github.com/ocornut/imgui/issues/4534
            ImGuiWindowClass window_class;
            window_class.ViewportFlagsOverrideClear = ImGuiViewportFlags_NoDecoration;
            ImGui::SetNextWindowClass(&window_class);
        }
#endif  // IMGUI_HAS_VIEWPORT

        bool beg = false;
        if (m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_NoDialog)  // disable our own dialog system (standard or modal)
        {
            beg = true;
        } else {
            ImGui::SetNextWindowSizeConstraints(vMinSize, vMaxSize);

            if (m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_Modal && !m_FileDialogInternal.okResultToConfirm)  // disable modal because the confirm dialog for overwrite is
                                                                                                                        // a new modal
            {
                ImGui::OpenPopup(name.c_str());
                beg = ImGui::BeginPopupModal(name.c_str(), (bool*)nullptr, flags | ImGuiWindowFlags_NoScrollbar);
            } else {
                beg = ImGui::Begin(name.c_str(), (bool*)nullptr, flags | ImGuiWindowFlags_NoScrollbar);
            }
        }
        if (beg) {
#ifdef IMGUI_HAS_VIEWPORT
            // if decoration is enabled we disable the resizing feature of imgui for avoid crash with SDL2 and GLFW3
            if (ImGui::GetIO().ConfigViewportsNoDecoration) {
                flags = vFlags;
            } else {
                auto win = ImGui::GetCurrentWindowRead();
                if (win->Viewport->Idx != 0)
                    flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
                else
                    flags = vFlags;
            }
#endif  // IMGUI_HAS_VIEWPORT

            ImGuiID _frameId = ImGui::GetID(name.c_str());
            ImVec2 frameSize = ImVec2(0, 0);
            if (m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_NoDialog) frameSize = vMaxSize;
            if (ImGui::BeginChild(_frameId, frameSize, false, flags | ImGuiWindowFlags_NoScrollbar)) {
                m_FileDialogInternal.name = name;  //-V820

                if (fdFile.dLGpath.empty()) fdFile.dLGpath = ".";  // defaut path is '.'

                fdFilter.SetDefaultFilterIfNotDefined();

                // init list of files
                if (fdFile.IsFileListEmpty()) {
                    if (fdFile.dLGpath != ".")                                                      // Removes extension seperator in filename if we don't check
                        IGFD::Utils::ReplaceString(fdFile.dLGDefaultFileName, fdFile.dLGpath, "");  // local path

                    if (!fdFile.dLGDefaultFileName.empty()) {
                        fdFile.SetDefaultFileName(fdFile.dLGDefaultFileName);
                        fdFilter.SetSelectedFilterWithExt(fdFilter.dLGdefaultExt);
                    } else if (fdFile.dLGDirectoryMode)  // directory mode
                        fdFile.SetDefaultFileName(".");
                    fdFile.ScanDir(m_FileDialogInternal, fdFile.dLGpath);
                }

                // draw dialog parts
                m_DrawHeader();        // place, directory, path
                m_DrawContent();       // place, files view, side pane
                res = m_DrawFooter();  // file field, filter combobox, ok/cancel buttons

                m_EndFrame();
            }
            ImGui::EndChild();

            // for display in dialog center, the confirm to overwrite dlg
            m_FileDialogInternal.dialogCenterPos = ImGui::GetCurrentWindowRead()->ContentRegionRect.GetCenter();

            // when the confirm to overwrite dialog will appear we need to
            // disable the modal mode of the main file dialog
            // see prOkResultToConfirm under
            if (m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_Modal && !m_FileDialogInternal.okResultToConfirm) {
                ImGui::EndPopup();
            }
        }

        if (m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_NoDialog) {  // disable our own dialog system (standard or modal)
        } else {
            // same things here regarding prOkResultToConfirm
            if (!(m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_Modal) || m_FileDialogInternal.okResultToConfirm) {
                ImGui::End();
            }
        }
        // confirm the result and show the confirm to overwrite dialog if needed
        res = m_Confirm_Or_OpenOverWriteFileDialog_IfNeeded(res, vFlags);

        if (m_FileDialogInternal.puUseCustomLocale) setlocale(m_FileDialogInternal.localeCategory, m_FileDialogInternal.localeEnd.c_str());
    }

    return res;
}

void IGFD::FileDialog::m_NewFrame() {
    m_FileDialogInternal.NewFrame();
    m_NewThumbnailFrame(m_FileDialogInternal);
}

void IGFD::FileDialog::m_EndFrame() {
    m_EndThumbnailFrame(m_FileDialogInternal);
    m_FileDialogInternal.EndFrame();
}
void IGFD::FileDialog::m_QuitFrame() {
    m_QuitThumbnailFrame(m_FileDialogInternal);
}

void IGFD::FileDialog::m_DrawHeader() {
#ifdef USE_PLACES_FEATURE
    if (!(m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_DisablePlaceMode)) {
        m_DrawPlacesButton();
        ImGui::SameLine();
    }

#endif  // USE_PLACES_FEATURE

    m_FileDialogInternal.fileManager.DrawDirectoryCreation(m_FileDialogInternal);

    if (
#ifdef USE_PLACES_FEATURE
        !(m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_DisablePlaceMode) ||
#endif  // USE_PLACES_FEATURE
        !(m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_DisableCreateDirectoryButton)) {
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
        ImGui::SameLine();
    }
    m_FileDialogInternal.fileManager.DrawPathComposer(m_FileDialogInternal);

#ifdef USE_THUMBNAILS
    if (!(m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_DisableThumbnailMode)) {
        m_DrawDisplayModeToolBar();
        ImGui::SameLine();
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
        ImGui::SameLine();
    }
#endif  // USE_THUMBNAILS

    m_FileDialogInternal.searchManager.DrawSearchBar(m_FileDialogInternal);
}

void IGFD::FileDialog::m_DrawContent() {
    ImVec2 size = ImGui::GetContentRegionAvail() - ImVec2(0.0f, m_FileDialogInternal.footerHeight);

#ifdef USE_PLACES_FEATURE
    if (!(m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_DisablePlaceMode)) {
        if (m_PlacesPaneShown) {
            float otherWidth = size.x - m_PlacesPaneWidth;
            ImGui::PushID("##splitterplaces");
            IGFD::Utils::ImSplitter(true, 4.0f, &m_PlacesPaneWidth, &otherWidth, 10.0f, 10.0f + m_FileDialogInternal.getDialogConfig().sidePaneWidth, size.y);
            ImGui::PopID();
            size.x -= otherWidth;
            m_DrawPlacesPane(m_FileDialogInternal, size);
            ImGui::SameLine();
        }
    }
#endif  // USE_PLACES_FEATURE

    size.x = ImGui::GetContentRegionAvail().x - m_FileDialogInternal.getDialogConfig().sidePaneWidth;

    if (m_FileDialogInternal.getDialogConfig().sidePane) {
        ImGui::PushID("##splittersidepane");
        IGFD::Utils::ImSplitter(true, 4.0f, &size.x, &m_FileDialogInternal.getDialogConfigRef().sidePaneWidth, 10.0f, 10.0f, size.y);
        ImGui::PopID();
    }

#ifdef USE_THUMBNAILS
    if (m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_DisableThumbnailMode) {
        m_DrawFileListView(size);
    } else {
        switch (m_DisplayMode) {
            case DisplayModeEnum::FILE_LIST: m_DrawFileListView(size); break;
            case DisplayModeEnum::THUMBNAILS_LIST: m_DrawThumbnailsListView(size); break;
            case DisplayModeEnum::THUMBNAILS_GRID: m_DrawThumbnailsGridView(size);
        }
    }
#else   // USE_THUMBNAILS
    m_DrawFileListView(size);
#endif  // USE_THUMBNAILS

    if (m_FileDialogInternal.getDialogConfig().sidePane) {
        m_DrawSidePane(size.y);
    }

    if (!(m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_DisableQuickPathSelection)) {
        m_DisplayPathPopup(size);
    }
}

void IGFD::FileDialog::m_DisplayPathPopup(ImVec2 vSize) {
    ImVec2 size = ImVec2(vSize.x * 0.5f, vSize.y * 0.5f);
    if (ImGui::BeginPopup("IGFD_Path_Popup")) {
        auto& fdi = m_FileDialogInternal.fileManager;

        ImGui::PushID(this);

        static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoHostExtendY;
        auto listViewID              = ImGui::GetID("##FileDialog_pathTable");
        if (ImGui::BeginTableEx("##FileDialog_pathTable", listViewID, 1, flags, size, 0.0f))  //-V112
        {
            ImGui::TableSetupScrollFreeze(0, 1);  // Make header always visible
            ImGui::TableSetupColumn(tableHeaderFileNameString, ImGuiTableColumnFlags_WidthStretch | (defaultSortOrderFilename ? ImGuiTableColumnFlags_PreferSortAscending : ImGuiTableColumnFlags_PreferSortDescending), -1, 0);

            ImGui::TableHeadersRow();

            if (!fdi.IsPathFilteredListEmpty()) {
                std::string _str;
                ImFont* _font   = nullptr;
                bool _showColor = false;

                m_PathListClipper.Begin((int)fdi.GetPathFilteredListSize(), ImGui::GetTextLineHeightWithSpacing());
                while (m_PathListClipper.Step()) {
                    for (int i = m_PathListClipper.DisplayStart; i < m_PathListClipper.DisplayEnd; i++) {
                        if (i < 0) continue;

                        auto infos = fdi.GetFilteredPathAt((size_t)i);
                        if (!infos.use_count()) continue;

                        m_BeginFileColorIconStyle(infos, _showColor, _str, &_font);

                        bool selected = fdi.IsFileNameSelected(infos->fileNameExt);  // found

                        ImGui::TableNextRow();

                        if (ImGui::TableNextColumn())  // file name
                        {
                            if (ImGui::Selectable(infos->fileNameExt.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SpanAvailWidth)) {
                                fdi.SetCurrentPath(fdi.ComposeNewPath(fdi.GetCurrentPopupComposedPath()));
                                fdi.puPathClicked = fdi.SelectDirectory(infos);
                                ImGui::CloseCurrentPopup();
                            }
                        }

                        m_EndFileColorIconStyle(_showColor, _font);
                    }
                }
                m_PathListClipper.End();
            }

            ImGui::EndTable();
        }

        ImGui::PopID();

        ImGui::EndPopup();
    }
}

bool IGFD::FileDialog::m_DrawOkButton() {
    auto& fdFile = m_FileDialogInternal.fileManager;
    if (m_FileDialogInternal.canWeContinue && strlen(fdFile.fileNameBuffer)) {
        if (IMGUI_BUTTON(okButtonString "##validationdialog", ImVec2(okButtonWidth, 0.0f)) || m_FileDialogInternal.isOk) {
            m_FileDialogInternal.isOk = true;
            return true;
        }

#if !invertOkAndCancelButtons
        ImGui::SameLine();
#endif
    }

    return false;
}

bool IGFD::FileDialog::m_DrawCancelButton() {
    if (IMGUI_BUTTON(cancelButtonString "##validationdialog", ImVec2(cancelButtonWidth, 0.0f)) || m_FileDialogInternal.needToExitDialog)  // dialog exit asked
    {
        m_FileDialogInternal.isOk = false;
        return true;
    }

#if invertOkAndCancelButtons
    ImGui::SameLine();
#endif

    return false;
}

bool IGFD::FileDialog::m_DrawValidationButtons() {
    bool res = false;

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - prOkCancelButtonWidth) * okCancelButtonAlignement);

    ImGui::BeginGroup();

    if (invertOkAndCancelButtons) {
        res |= m_DrawCancelButton();
        res |= m_DrawOkButton();
    } else {
        res |= m_DrawOkButton();
        res |= m_DrawCancelButton();
    }

    ImGui::EndGroup();

    prOkCancelButtonWidth = ImGui::GetItemRectSize().x;

    return res;
}

bool IGFD::FileDialog::m_DrawFooter() {
    auto& fdFile = m_FileDialogInternal.fileManager;

    float posY = ImGui::GetCursorPos().y;  // height of last bar calc
    ImGui::AlignTextToFramePadding();
    if (!fdFile.dLGDirectoryMode)
        ImGui::Text(fileNameString);
    else  // directory chooser
        ImGui::Text(dirNameString);
    ImGui::SameLine();

    // Input file fields
    float width = ImGui::GetContentRegionAvail().x;
    if (!fdFile.dLGDirectoryMode) {
        ImGuiContext& g = *GImGui;
        width -= m_FileDialogInternal.filterManager.GetFilterComboBoxWidth() + g.Style.ItemSpacing.x;
    }

    ImGui::PushItemWidth(width);
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
    if (m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_ReadOnlyFileNameField) {
        flags |= ImGuiInputTextFlags_ReadOnly;
    }
    if (ImGui::InputText("##FileName", fdFile.fileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, flags)) {
        m_FileDialogInternal.isOk = true;
    }
    if (ImGui::GetItemID() == ImGui::GetActiveID()) m_FileDialogInternal.fileInputIsActive = true;
    ImGui::PopItemWidth();

    // combobox of filters
    m_FileDialogInternal.filterManager.DrawFilterComboBox(m_FileDialogInternal);

    bool res                          = m_DrawValidationButtons();
    m_FileDialogInternal.footerHeight = ImGui::GetCursorPosY() - posY;
    return res;
}

void IGFD::FileDialog::m_SelectableItem(int vidx, std::shared_ptr<FileInfos> vInfos, bool vSelected, const char* vFmt, ...) {
    if (!vInfos.use_count()) return;

    auto& fdi = m_FileDialogInternal.fileManager;

    static ImGuiSelectableFlags selectableFlags = ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SpanAvailWidth;

    va_list args;
    va_start(args, vFmt);
    vsnprintf(fdi.variadicBuffer, MAX_FILE_DIALOG_NAME_BUFFER, vFmt, args);
    va_end(args);

    float h = 0.0f;
#ifdef USE_THUMBNAILS
    if (m_DisplayMode == DisplayModeEnum::THUMBNAILS_LIST && !(m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_DisableThumbnailMode)) {
        h = DisplayMode_ThumbailsList_ImageHeight;
    }
#endif  // USE_THUMBNAILS
#ifdef USE_EXPLORATION_BY_KEYS
    bool flashed = m_BeginFlashItem((size_t)vidx);
    bool res     = m_FlashableSelectable(fdi.variadicBuffer, vSelected, selectableFlags, flashed, ImVec2(-1.0f, h));
    if (flashed) m_EndFlashItem();
#else   // USE_EXPLORATION_BY_KEYS
    (void)vidx;  // remove a warnings ofr unused var

    bool res = ImGui::Selectable(fdi.variadicBuffer, vSelected, selectableFlags, ImVec2(-1.0f, h));
#endif  // USE_EXPLORATION_BY_KEYS
    if (res) {
        if (vInfos->fileType.isDir()) {
            // nav system, selectable cause open directory or select directory
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard) {
                // little fix for get back the mouse behavior in nav system
                if (ImGui::IsMouseDoubleClicked(0))  // 0 -> left mouse button double click
                {
                    fdi.puPathClicked = fdi.SelectDirectory(vInfos);
                } else if (fdi.dLGDirectoryMode)  // directory chooser
                {
                    fdi.SelectFileName(m_FileDialogInternal, vInfos);
                } else {
                    fdi.puPathClicked = fdi.SelectDirectory(vInfos);
                }
            } else  // no nav system => classic behavior
            {
                if (ImGui::IsMouseDoubleClicked(0))  // 0 -> left mouse button double click
                {
                    fdi.puPathClicked = fdi.SelectDirectory(vInfos);
                } else if (fdi.dLGDirectoryMode)  // directory chooser
                {
                    fdi.SelectFileName(m_FileDialogInternal, vInfos);
                }
            }
        } else {
            fdi.SelectFileName(m_FileDialogInternal, vInfos);

            if (ImGui::IsMouseDoubleClicked(0)) {
                m_FileDialogInternal.isOk = true;
            }
        }
    }
}

void IGFD::FileDialog::m_DisplayFileInfosTooltip(const int32_t& vRowIdx, const int32_t& vColumnIdx, std::shared_ptr<FileInfos> vFileInfos) {
    if (ImGui::IsItemHovered()) {
        if (vFileInfos != nullptr && vFileInfos->tooltipColumn == vColumnIdx) {
            if (!vFileInfos->tooltipMessage.empty()) {
                ImGui::SetTooltip("%s", vFileInfos->tooltipMessage.c_str());
            }
        }
    }
}

void IGFD::FileDialog::m_BeginFileColorIconStyle(std::shared_ptr<FileInfos> vFileInfos, bool& vOutShowColor, std::string& vOutStr, ImFont** vOutFont) {
    vOutStr.clear();
    vOutShowColor = false;

    if (vFileInfos->fileStyle.use_count())  //-V807 //-V522
    {
        vOutShowColor = true;

        *vOutFont = vFileInfos->fileStyle->font;
    }

    if (vOutShowColor && !vFileInfos->fileStyle->icon.empty())
        vOutStr = vFileInfos->fileStyle->icon;
    else if (vFileInfos->fileType.isDir())
        vOutStr = dirEntryString;
    else if (vFileInfos->fileType.isLinkToUnknown())
        vOutStr = linkEntryString;
    else if (vFileInfos->fileType.isFile())
        vOutStr = fileEntryString;

    vOutStr += " " + vFileInfos->fileNameExt;

    if (vOutShowColor) ImGui::PushStyleColor(ImGuiCol_Text, vFileInfos->fileStyle->color);
    if (*vOutFont) ImGui::PushFont(*vOutFont);
}

void IGFD::FileDialog::m_EndFileColorIconStyle(const bool& vShowColor, ImFont* vFont) {
    if (vFont) ImGui::PopFont();
    if (vShowColor) ImGui::PopStyleColor();
}

void IGFD::FileDialog::m_DrawFileListView(ImVec2 vSize) {
    auto& fdi = m_FileDialogInternal.fileManager;

    ImGui::PushID(this);

    static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoHostExtendY
#ifndef USE_CUSTOM_SORTING_ICON
                                   | ImGuiTableFlags_Sortable
#endif  // USE_CUSTOM_SORTING_ICON
        ;
    auto listViewID = ImGui::GetID("##FileDialog_fileTable");
    if (ImGui::BeginTableEx("##FileDialog_fileTable", listViewID, 4, flags, vSize, 0.0f)) { 
        ImGui::TableSetupScrollFreeze(0, 1);  // Make header always visible
        ImGui::TableSetupColumn(fdi.headerFileName.c_str(), ImGuiTableColumnFlags_WidthStretch | (defaultSortOrderFilename ? ImGuiTableColumnFlags_PreferSortAscending : ImGuiTableColumnFlags_PreferSortDescending), -1, 0);
        ImGui::TableSetupColumn(fdi.headerFileType.c_str(),
                                ImGuiTableColumnFlags_WidthFixed | (defaultSortOrderType ? ImGuiTableColumnFlags_PreferSortAscending : ImGuiTableColumnFlags_PreferSortDescending) |
                                    ((m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_HideColumnType) ? ImGuiTableColumnFlags_DefaultHide : 0),
                                -1, 1);
        ImGui::TableSetupColumn(fdi.headerFileSize.c_str(),
                                ImGuiTableColumnFlags_WidthFixed | (defaultSortOrderSize ? ImGuiTableColumnFlags_PreferSortAscending : ImGuiTableColumnFlags_PreferSortDescending) |
                                    ((m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_HideColumnSize) ? ImGuiTableColumnFlags_DefaultHide : 0),
                                -1, 2);
        ImGui::TableSetupColumn(fdi.headerFileDate.c_str(),
                                ImGuiTableColumnFlags_WidthFixed | (defaultSortOrderDate ? ImGuiTableColumnFlags_PreferSortAscending : ImGuiTableColumnFlags_PreferSortDescending) |
                                    ((m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_HideColumnDate) ? ImGuiTableColumnFlags_DefaultHide : 0),
                                -1, 3);

#ifndef USE_CUSTOM_SORTING_ICON
        // Sort our data if sort specs have been changed!
        if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs()) {
            if (sorts_specs->SpecsDirty && !fdi.IsFileListEmpty()) {
                bool direction = sorts_specs->Specs->SortDirection == ImGuiSortDirection_Ascending;

                if (sorts_specs->Specs->ColumnUserID == 0) {
                    fdi.sortingField        = IGFD::FileManager::SortingFieldEnum::FIELD_FILENAME;
                    fdi.sortingDirection[0] = direction;
                    fdi.SortFields(m_FileDialogInternal);
                } else if (sorts_specs->Specs->ColumnUserID == 1) {
                    fdi.sortingField        = IGFD::FileManager::SortingFieldEnum::FIELD_TYPE;
                    fdi.sortingDirection[1] = direction;
                    fdi.SortFields(m_FileDialogInternal);
                } else if (sorts_specs->Specs->ColumnUserID == 2) {
                    fdi.sortingField        = IGFD::FileManager::SortingFieldEnum::FIELD_SIZE;
                    fdi.sortingDirection[2] = direction;
                    fdi.SortFields(m_FileDialogInternal);
                } else  // if (sorts_specs->Specs->ColumnUserID == 3) => alwayd true for the moment, to uncomment if we
                        // add a fourth column
                {
                    fdi.sortingField        = IGFD::FileManager::SortingFieldEnum::FIELD_DATE;
                    fdi.sortingDirection[3] = direction;
                    fdi.SortFields(m_FileDialogInternal);
                }

                sorts_specs->SpecsDirty = false;
            }
        }

        ImGui::TableHeadersRow();
#else   // USE_CUSTOM_SORTING_ICON
        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
        for (int column = 0; column < 4; column++)  //-V112
        {
            ImGui::TableSetColumnIndex(column);
            const char* column_name = ImGui::TableGetColumnName(column);  // Retrieve name passed to TableSetupColumn()
            ImGui::PushID(column);
            ImGui::TableHeader(column_name);
            ImGui::PopID();
            if (ImGui::IsItemClicked()) {
                if (column == 0) {
                    if (fdi.sortingField == IGFD::FileManager::SortingFieldEnum::FIELD_FILENAME)
                        fdi.sortingDirection[0] = !fdi.sortingDirection[0];
                    else
                        fdi.sortingField = IGFD::FileManager::SortingFieldEnum::FIELD_FILENAME;

                    fdi.SortFields(m_FileDialogInternal);
                } else if (column == 1) {
                    if (fdi.sortingField == IGFD::FileManager::SortingFieldEnum::FIELD_TYPE)
                        fdi.sortingDirection[1] = !fdi.sortingDirection[1];
                    else
                        fdi.sortingField = IGFD::FileManager::SortingFieldEnum::FIELD_TYPE;

                    fdi.SortFields(m_FileDialogInternal);
                } else if (column == 2) {
                    if (fdi.sortingField == IGFD::FileManager::SortingFieldEnum::FIELD_SIZE)
                        fdi.sortingDirection[2] = !fdi.sortingDirection[2];
                    else
                        fdi.sortingField = IGFD::FileManager::SortingFieldEnum::FIELD_SIZE;

                    fdi.SortFields(m_FileDialogInternal);
                } else  // if (column == 3) => alwayd true for the moment, to uncomment if we add a fourth column
                {
                    if (fdi.sortingField == IGFD::FileManager::SortingFieldEnum::FIELD_DATE)
                        fdi.sortingDirection[3] = !fdi.sortingDirection[3];
                    else
                        fdi.sortingField = IGFD::FileManager::SortingFieldEnum::FIELD_DATE;

                    fdi.SortFields(m_FileDialogInternal);
                }
            }
        }
#endif  // USE_CUSTOM_SORTING_ICON
        if (!fdi.IsFilteredListEmpty()) {
            std::string _str;
            ImFont* _font   = nullptr;
            bool _showColor = false;

            int column_id = 0;
            m_FileListClipper.Begin((int)fdi.GetFilteredListSize(), ImGui::GetTextLineHeightWithSpacing());
            while (m_FileListClipper.Step()) {
                for (int i = m_FileListClipper.DisplayStart; i < m_FileListClipper.DisplayEnd; i++) {
                    if (i < 0) continue;

                    auto infos = fdi.GetFilteredFileAt((size_t)i);
                    if (!infos.use_count()) continue;

                    m_BeginFileColorIconStyle(infos, _showColor, _str, &_font);

                    bool selected = fdi.IsFileNameSelected(infos->fileNameExt);  // found

                    ImGui::TableNextRow();

                    column_id = 0;
                    if (ImGui::TableNextColumn()) {  // file name
                        if (!infos->deviceInfos.empty()) {
                            _str += " " + infos->deviceInfos;
                        }
                        m_SelectableItem(i, infos, selected, _str.c_str());
                        m_DisplayFileInfosTooltip(i, column_id++, infos);
                    }
                    if (ImGui::TableNextColumn()) {  // file type
                        ImGui::Text("%s", infos->fileExtLevels[0].c_str());
                        m_DisplayFileInfosTooltip(i, column_id++, infos);
                    }
                    if (ImGui::TableNextColumn()) {  // file size
                        if (!infos->fileType.isDir()) {
                            ImGui::Text("%s ", infos->formatedFileSize.c_str());
                        } else {
                            ImGui::TextUnformatted("");
                        }
                        m_DisplayFileInfosTooltip(i, column_id++, infos);
                    }
                    if (ImGui::TableNextColumn()) {  // file date + time
                        ImGui::Text("%s", infos->fileModifDate.c_str());
                        m_DisplayFileInfosTooltip(i, column_id++, infos);
                    }

                    m_EndFileColorIconStyle(_showColor, _font);
                }
            }
            m_FileListClipper.End();
        }

#ifdef USE_EXPLORATION_BY_KEYS
        if (!fdi.inputPathActivated) {
            m_LocateByInputKey(m_FileDialogInternal);
            m_ExploreWithkeys(m_FileDialogInternal, listViewID);
        }
#endif  // USE_EXPLORATION_BY_KEYS

        ImGuiContext& g = *GImGui;
        if (g.LastActiveId - 1 == listViewID || g.LastActiveId == listViewID) {
            m_FileDialogInternal.fileListViewIsActive = true;
        }

        ImGui::EndTable();
    }

    ImGui::PopID();
}

#ifdef USE_THUMBNAILS
void IGFD::FileDialog::m_DrawThumbnailsListView(ImVec2 vSize) {
    auto& fdi = m_FileDialogInternal.fileManager;

    ImGui::PushID(this);

    static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoHostExtendY
#ifndef USE_CUSTOM_SORTING_ICON
                                   | ImGuiTableFlags_Sortable
#endif  // USE_CUSTOM_SORTING_ICON
        ;
    auto listViewID = ImGui::GetID("##FileDialog_fileTable");
    if (ImGui::BeginTableEx("##FileDialog_fileTable", listViewID, 5, flags, vSize, 0.0f)) {
        ImGui::TableSetupScrollFreeze(0, 1);  // Make header always visible
        ImGui::TableSetupColumn(fdi.headerFileName.c_str(), ImGuiTableColumnFlags_WidthStretch | (defaultSortOrderFilename ? ImGuiTableColumnFlags_PreferSortAscending : ImGuiTableColumnFlags_PreferSortDescending), -1, 0);
        ImGui::TableSetupColumn(fdi.headerFileType.c_str(),
                                ImGuiTableColumnFlags_WidthFixed | (defaultSortOrderType ? ImGuiTableColumnFlags_PreferSortAscending : ImGuiTableColumnFlags_PreferSortDescending) |
                                    ((m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_HideColumnType) ? ImGuiTableColumnFlags_DefaultHide : 0),
                                -1, 1);
        ImGui::TableSetupColumn(fdi.headerFileSize.c_str(),
                                ImGuiTableColumnFlags_WidthFixed | (defaultSortOrderSize ? ImGuiTableColumnFlags_PreferSortAscending : ImGuiTableColumnFlags_PreferSortDescending) |
                                    ((m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_HideColumnSize) ? ImGuiTableColumnFlags_DefaultHide : 0),
                                -1, 2);
        ImGui::TableSetupColumn(fdi.headerFileDate.c_str(),
                                ImGuiTableColumnFlags_WidthFixed | (defaultSortOrderDate ? ImGuiTableColumnFlags_PreferSortAscending : ImGuiTableColumnFlags_PreferSortDescending) |
                                    ((m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_HideColumnDate) ? ImGuiTableColumnFlags_DefaultHide : 0),
                                -1, 3);
        // not needed to have an option for hide the thumbnails since this is why this view is used
        ImGui::TableSetupColumn(fdi.headerFileThumbnails.c_str(), ImGuiTableColumnFlags_WidthFixed | (defaultSortOrderThumbnails ? ImGuiTableColumnFlags_PreferSortAscending : ImGuiTableColumnFlags_PreferSortDescending), -1, 4);  //-V112

#ifndef USE_CUSTOM_SORTING_ICON
        // Sort our data if sort specs have been changed!
        if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs()) {
            if (sorts_specs->SpecsDirty && !fdi.IsFileListEmpty()) {
                bool direction = sorts_specs->Specs->SortDirection == ImGuiSortDirection_Ascending;

                if (sorts_specs->Specs->ColumnUserID == 0) {
                    fdi.sortingField        = IGFD::FileManager::SortingFieldEnum::FIELD_FILENAME;
                    fdi.sortingDirection[0] = direction;
                    fdi.SortFields(m_FileDialogInternal);
                } else if (sorts_specs->Specs->ColumnUserID == 1) {
                    fdi.sortingField        = IGFD::FileManager::SortingFieldEnum::FIELD_TYPE;
                    fdi.sortingDirection[1] = direction;
                    fdi.SortFields(m_FileDialogInternal);
                } else if (sorts_specs->Specs->ColumnUserID == 2) {
                    fdi.sortingField        = IGFD::FileManager::SortingFieldEnum::FIELD_SIZE;
                    fdi.sortingDirection[2] = direction;
                    fdi.SortFields(m_FileDialogInternal);
                } else if (sorts_specs->Specs->ColumnUserID == 3) {
                    fdi.sortingField        = IGFD::FileManager::SortingFieldEnum::FIELD_DATE;
                    fdi.sortingDirection[3] = direction;
                    fdi.SortFields(m_FileDialogInternal);
                } else  // if (sorts_specs->Specs->ColumnUserID == 4) = > always true for the moment, to uncomment if we
                        // add another column
                {
                    fdi.sortingField        = IGFD::FileManager::SortingFieldEnum::FIELD_THUMBNAILS;
                    fdi.sortingDirection[4] = direction;
                    fdi.SortFields(m_FileDialogInternal);
                }

                sorts_specs->SpecsDirty = false;
            }
        }

        ImGui::TableHeadersRow();
#else   // USE_CUSTOM_SORTING_ICON
        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
        for (int column = 0; column < 5; column++) {
            ImGui::TableSetColumnIndex(column);
            const char* column_name = ImGui::TableGetColumnName(column);  // Retrieve name passed to TableSetupColumn()
            ImGui::PushID(column);
            ImGui::TableHeader(column_name);
            ImGui::PopID();
            if (ImGui::IsItemClicked()) {
                if (column == 0) {
                    if (fdi.sortingField == IGFD::FileManager::SortingFieldEnum::FIELD_FILENAME)
                        fdi.sortingDirection[0] = !fdi.sortingDirection[0];
                    else
                        fdi.sortingField = IGFD::FileManager::SortingFieldEnum::FIELD_FILENAME;

                    fdi.SortFields(m_FileDialogInternal);
                } else if (column == 1) {
                    if (fdi.sortingField == IGFD::FileManager::SortingFieldEnum::FIELD_TYPE)
                        fdi.sortingDirection[1] = !fdi.sortingDirection[1];
                    else
                        fdi.sortingField = IGFD::FileManager::SortingFieldEnum::FIELD_TYPE;

                    fdi.SortFields(m_FileDialogInternal);
                } else if (column == 2) {
                    if (fdi.sortingField == IGFD::FileManager::SortingFieldEnum::FIELD_SIZE)
                        fdi.sortingDirection[2] = !fdi.sortingDirection[2];
                    else
                        fdi.sortingField = IGFD::FileManager::SortingFieldEnum::FIELD_SIZE;

                    fdi.SortFields(m_FileDialogInternal);
                } else if (column == 3) {
                    if (fdi.sortingField == IGFD::FileManager::SortingFieldEnum::FIELD_DATE)
                        fdi.sortingDirection[3] = !fdi.sortingDirection[3];
                    else
                        fdi.sortingField = IGFD::FileManager::SortingFieldEnum::FIELD_DATE;

                    fdi.SortFields(m_FileDialogInternal);
                } else  // if (sorts_specs->Specs->ColumnUserID == 4) = > always true for the moment, to uncomment if we
                        // add another column
                {
                    if (fdi.sortingField == IGFD::FileManager::SortingFieldEnum::FIELD_THUMBNAILS)
                        fdi.sortingDirection[4] = !fdi.sortingDirection[4];
                    else
                        fdi.sortingField = IGFD::FileManager::SortingFieldEnum::FIELD_THUMBNAILS;

                    fdi.SortFields(m_FileDialogInternal);
                }
            }
        }
#endif  // USE_CUSTOM_SORTING_ICON
        if (!fdi.IsFilteredListEmpty()) {
            std::string _str;
            ImFont* _font   = nullptr;
            bool _showColor = false;

            ImGuiContext& g        = *GImGui;
            const float itemHeight = ImMax(g.FontSize, DisplayMode_ThumbailsList_ImageHeight) + g.Style.ItemSpacing.y;

            int column_id = 0;
            m_FileListClipper.Begin((int)fdi.GetFilteredListSize(), itemHeight);
            while (m_FileListClipper.Step()) {
                for (int i = m_FileListClipper.DisplayStart; i < m_FileListClipper.DisplayEnd; i++) {
                    if (i < 0) continue;

                    auto infos = fdi.GetFilteredFileAt((size_t)i);
                    if (!infos.use_count()) continue;

                    m_BeginFileColorIconStyle(infos, _showColor, _str, &_font);

                    bool selected = fdi.IsFileNameSelected(infos->fileNameExt);  // found

                    ImGui::TableNextRow();

                    column_id = 0;
                    if (ImGui::TableNextColumn()) {  // file name
                        if (!infos->deviceInfos.empty()) {
                            _str += " " + infos->deviceInfos;
                        }
                        m_SelectableItem(i, infos, selected, _str.c_str());
                        m_DisplayFileInfosTooltip(i, column_id++, infos);
                    }
                    if (ImGui::TableNextColumn()) 
                    {  // file type
                        ImGui::Text("%s", infos->fileExtLevels[0].c_str());
                        m_DisplayFileInfosTooltip(i, column_id++, infos);
                    }
                    if (ImGui::TableNextColumn()) {  // file size
                        if (!infos->fileType.isDir()) {
                            ImGui::Text("%s ", infos->formatedFileSize.c_str());
                        } else {
                            ImGui::TextUnformatted("");
                        }
                        m_DisplayFileInfosTooltip(i, column_id++, infos);
                    }
                    if (ImGui::TableNextColumn()) {  // file date + time
                        ImGui::Text("%s", infos->fileModifDate.c_str());
                        m_DisplayFileInfosTooltip(i, column_id++, infos);
                    }
                    if (ImGui::TableNextColumn()) {  // file thumbnails
                        auto th = &infos->thumbnailInfo;

                        if (!th->isLoadingOrLoaded) {
                            m_AddThumbnailToLoad(infos);
                        }
                        if (th->isReadyToDisplay && th->textureID) {
                            ImGui::Image((ImTextureID)th->textureID, ImVec2((float)th->textureWidth, (float)th->textureHeight));
                        }
                        m_DisplayFileInfosTooltip(i, column_id++, infos);
                    }

                    m_EndFileColorIconStyle(_showColor, _font);
                }
            }
            m_FileListClipper.End();
        }

#ifdef USE_EXPLORATION_BY_KEYS
        if (!fdi.inputPathActivated) {
            m_LocateByInputKey(m_FileDialogInternal);
            m_ExploreWithkeys(m_FileDialogInternal, listViewID);
        }
#endif  // USE_EXPLORATION_BY_KEYS

        ImGuiContext& g = *GImGui;
        if (g.LastActiveId - 1 == listViewID || g.LastActiveId == listViewID) {
            m_FileDialogInternal.fileListViewIsActive = true;
        }

        ImGui::EndTable();
    }

    ImGui::PopID();
}

void IGFD::FileDialog::m_DrawThumbnailsGridView(ImVec2 vSize) {
    if (ImGui::BeginChild("##thumbnailsGridsFiles", vSize)) {
        // todo
    }

    ImGui::EndChild();
}

#endif

void IGFD::FileDialog::m_DrawSidePane(float vHeight) {
    ImGui::SameLine();

    ImGui::BeginChild("##FileTypes", ImVec2(0, vHeight));

    m_FileDialogInternal.getDialogConfig().sidePane(
        m_FileDialogInternal.filterManager.GetSelectedFilter().getFirstFilter().c_str(), 
        m_FileDialogInternal.getDialogConfigRef().userDatas, 
        &m_FileDialogInternal.canWeContinue);
    ImGui::EndChild();
}

void IGFD::FileDialog::Close() {
    m_FileDialogInternal.dLGkey.clear();
    m_FileDialogInternal.showDialog = false;
}

bool IGFD::FileDialog::WasOpenedThisFrame(const std::string& vKey) const {
    bool res = m_FileDialogInternal.showDialog && m_FileDialogInternal.dLGkey == vKey;
    if (res) {
        res &= m_FileDialogInternal.lastImGuiFrameCount == GImGui->FrameCount;  // return true if a dialog was displayed in this frame
    }
    return res;
}

bool IGFD::FileDialog::WasOpenedThisFrame() const {
    bool res = m_FileDialogInternal.showDialog;
    if (res) {
        res &= m_FileDialogInternal.lastImGuiFrameCount == GImGui->FrameCount;  // return true if a dialog was displayed in this frame
    }
    return res;
}

bool IGFD::FileDialog::IsOpened(const std::string& vKey) const {
    return (m_FileDialogInternal.showDialog && m_FileDialogInternal.dLGkey == vKey);
}

bool IGFD::FileDialog::IsOpened() const {
    return m_FileDialogInternal.showDialog;
}

std::string IGFD::FileDialog::GetOpenedKey() const {
    if (m_FileDialogInternal.showDialog) {
        return m_FileDialogInternal.dLGkey;
    }
    return "";
}

std::string IGFD::FileDialog::GetFilePathName(IGFD_ResultMode vFlag) {
    return m_FileDialogInternal.fileManager.GetResultingFilePathName(m_FileDialogInternal, vFlag);
}

std::string IGFD::FileDialog::GetCurrentPath() {
    return m_FileDialogInternal.fileManager.GetResultingPath();
}

std::string IGFD::FileDialog::GetCurrentFileName(IGFD_ResultMode vFlag) {
    return m_FileDialogInternal.fileManager.GetResultingFileName(m_FileDialogInternal, vFlag);
}

std::string IGFD::FileDialog::GetCurrentFilter() {
    return m_FileDialogInternal.filterManager.GetSelectedFilter().title;
}

std::map<std::string, std::string> IGFD::FileDialog::GetSelection(IGFD_ResultMode vFlag) {
    return m_FileDialogInternal.fileManager.GetResultingSelection(m_FileDialogInternal, vFlag);
}

IGFD::UserDatas IGFD::FileDialog::GetUserDatas() const {
    return m_FileDialogInternal.getDialogConfig().userDatas;
}

bool IGFD::FileDialog::IsOk() const {
    return m_FileDialogInternal.isOk;
}

void IGFD::FileDialog::SetFileStyle(const IGFD_FileStyleFlags& vFlags, const char* vCriteria, const FileStyle& vInfos) {
    m_FileDialogInternal.filterManager.SetFileStyle(vFlags, vCriteria, vInfos);
}

void IGFD::FileDialog::SetFileStyle(const IGFD_FileStyleFlags& vFlags, const char* vCriteria, const ImVec4& vColor, const std::string& vIcon, ImFont* vFont) {
    m_FileDialogInternal.filterManager.SetFileStyle(vFlags, vCriteria, vColor, vIcon, vFont);
}

void IGFD::FileDialog::SetFileStyle(FileStyle::FileStyleFunctor vFunctor) {
    m_FileDialogInternal.filterManager.SetFileStyle(vFunctor);
}

bool IGFD::FileDialog::GetFileStyle(const IGFD_FileStyleFlags& vFlags, const std::string& vCriteria, ImVec4* vOutColor, std::string* vOutIcon, ImFont** vOutFont) {
    return m_FileDialogInternal.filterManager.GetFileStyle(vFlags, vCriteria, vOutColor, vOutIcon, vOutFont);
}

void IGFD::FileDialog::ClearFilesStyle() {
    m_FileDialogInternal.filterManager.ClearFilesStyle();
}

void IGFD::FileDialog::SetLocales(const int& /*vLocaleCategory*/, const std::string& vLocaleBegin, const std::string& vLocaleEnd) {
    m_FileDialogInternal.puUseCustomLocale = true;
    m_FileDialogInternal.localeBegin       = vLocaleBegin;
    m_FileDialogInternal.localeEnd         = vLocaleEnd;
}

//////////////////////////////////////////////////////////////////////////////
//// OVERWRITE DIALOG ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

bool IGFD::FileDialog::m_Confirm_Or_OpenOverWriteFileDialog_IfNeeded(bool vLastAction, ImGuiWindowFlags vFlags) {
    // if confirmation => return true for confirm the overwrite et quit the dialog
    // if cancel => return false && set IsOk to false for keep inside the dialog

    // if IsOk == false => return false for quit the dialog
    if (!m_FileDialogInternal.isOk && vLastAction) {
        m_QuitFrame();
        return true;
    }

    // if IsOk == true && no check of overwrite => return true for confirm the dialog
    if (m_FileDialogInternal.isOk && vLastAction && !(m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_ConfirmOverwrite)) {
        m_QuitFrame();
        return true;
    }

    // if IsOk == true && check of overwrite => return false and show confirm to overwrite dialog
    if ((m_FileDialogInternal.okResultToConfirm || (m_FileDialogInternal.isOk && vLastAction)) && (m_FileDialogInternal.getDialogConfig().flags & ImGuiFileDialogFlags_ConfirmOverwrite)) {
        if (m_FileDialogInternal.isOk)  // catched only one time
        {
            if (!m_FileDialogInternal.fileManager.GetFileSystemInstance()->IsFileExist(GetFilePathName()))  // not existing => quit dialog
            {
                m_QuitFrame();
                return true;
            } else  // existing => confirm dialog to open
            {
                m_FileDialogInternal.isOk              = false;
                m_FileDialogInternal.okResultToConfirm = true;
            }
        }

        std::string name = OverWriteDialogTitleString "##" + m_FileDialogInternal.dLGtitle + m_FileDialogInternal.dLGkey + "OverWriteDialog";

        bool res = false;

        ImGui::OpenPopup(name.c_str());
        if (ImGui::BeginPopupModal(name.c_str(), (bool*)0, vFlags | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
            ImGui::SetWindowPos(m_FileDialogInternal.dialogCenterPos - ImGui::GetWindowSize() * 0.5f);  // next frame needed for GetWindowSize to work

            ImGui::Text("%s", OverWriteDialogMessageString);

            if (IMGUI_BUTTON(OverWriteDialogConfirmButtonString)) {
                m_FileDialogInternal.okResultToConfirm = false;
                m_FileDialogInternal.isOk              = true;
                res                                    = true;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (IMGUI_BUTTON(OverWriteDialogCancelButtonString)) {
                m_FileDialogInternal.okResultToConfirm = false;
                m_FileDialogInternal.isOk              = false;
                res                                    = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if (res) {
            m_QuitFrame();
        }
        return res;
    }

    return false;
}

#pragma endregion

#pragma endregion

#endif  // __cplusplus

#pragma region IGFD_C_API

// return an initialized IGFD_FileDialog_Config
IGFD_C_API IGFD_FileDialog_Config IGFD_FileDialog_Config_Get() {
    IGFD_FileDialog_Config res = {};
    res.path                   = "";
    res.fileName               = "";
    res.filePathName           = "";
    res.countSelectionMax      = 1;
    res.userDatas              = nullptr;
    res.sidePane               = nullptr;
    res.sidePaneWidth = 250.0f;     
    res.flags         = ImGuiFileDialogFlags_Default;
    return res;
}

// Return an initialized IGFD_Selection_Pair
IGFD_C_API IGFD_Selection_Pair IGFD_Selection_Pair_Get(void) {
    IGFD_Selection_Pair res = {};
    res.fileName            = nullptr;
    res.filePathName        = nullptr;
    return res;
}

// destroy only the content of vSelection_Pair
IGFD_C_API void IGFD_Selection_Pair_DestroyContent(IGFD_Selection_Pair* vSelection_Pair) {
    if (vSelection_Pair) {
        delete[] vSelection_Pair->fileName;
        delete[] vSelection_Pair->filePathName;
    }
}

// Return an initialized IGFD_Selection
IGFD_C_API IGFD_Selection IGFD_Selection_Get(void) {
    return {nullptr, 0U};
}

// destroy only the content of vSelection
IGFD_C_API void IGFD_Selection_DestroyContent(IGFD_Selection* vSelection) {
    if (vSelection) {
        if (vSelection->table) {
            for (size_t i = 0U; i < vSelection->count; i++) {
                IGFD_Selection_Pair_DestroyContent(&vSelection->table[i]);
            }
            delete[] vSelection->table;
        }
        vSelection->count = 0U;
    }
}

// create an instance of ImGuiFileDialog
IGFD_C_API ImGuiFileDialog* IGFD_Create(void) {
    return new ImGuiFileDialog();
}

// destroy the instance of ImGuiFileDialog
IGFD_C_API void IGFD_Destroy(ImGuiFileDialog* vContextPtr) {
    if (vContextPtr != nullptr) {
        delete vContextPtr;
        vContextPtr = nullptr;
    }
}

IGFD_C_API void IGFD_OpenDialog(             // open a standard dialog
    ImGuiFileDialog* vContextPtr,            // ImGuiFileDialog context
    const char* vKey,                        // key dialog
    const char* vTitle,                      // title
    const char* vFilters,                    // filters/filter collections. set it to null for directory mode
    const IGFD_FileDialog_Config vConfig) {  // path
    if (vContextPtr != nullptr) {
        IGFD::FileDialogConfig config;
        config.path              = vConfig.path;
        config.fileName          = vConfig.fileName;
        config.filePathName      = vConfig.filePathName;
        config.countSelectionMax = vConfig.countSelectionMax;
        config.userDatas         = vConfig.userDatas;
        config.flags             = vConfig.flags;
        config.sidePane          = vConfig.sidePane;
        config.sidePaneWidth     = vConfig.sidePaneWidth;
        vContextPtr->OpenDialog(vKey, vTitle, vFilters, config);
    }
}

IGFD_C_API bool IGFD_DisplayDialog(ImGuiFileDialog* vContextPtr, const char* vKey, ImGuiWindowFlags vFlags, ImVec2 vMinSize, ImVec2 vMaxSize) {
    if (vContextPtr != nullptr) {
        return vContextPtr->Display(vKey, vFlags, vMinSize, vMaxSize);
    }
    return false;
}

IGFD_C_API void IGFD_CloseDialog(ImGuiFileDialog* vContextPtr) {
    if (vContextPtr != nullptr) {
        vContextPtr->Close();
    }
}

IGFD_C_API bool IGFD_IsOk(ImGuiFileDialog* vContextPtr) {
    if (vContextPtr != nullptr) {
        return vContextPtr->IsOk();
    }
    return false;
}

IGFD_C_API bool IGFD_WasKeyOpenedThisFrame(ImGuiFileDialog* vContextPtr, const char* vKey) {
    if (vContextPtr != nullptr) {
        return vContextPtr->WasOpenedThisFrame(vKey);
    }
    return false;
}

IGFD_C_API bool IGFD_WasOpenedThisFrame(ImGuiFileDialog* vContextPtr) {
    if (vContextPtr != nullptr) {
        return vContextPtr->WasOpenedThisFrame();
    }

    return false;
}

IGFD_C_API bool IGFD_IsKeyOpened(ImGuiFileDialog* vContextPtr, const char* vCurrentOpenedKey) {
    if (vContextPtr != nullptr) {
        return vContextPtr->IsOpened(vCurrentOpenedKey);
    }

    return false;
}

IGFD_C_API bool IGFD_IsOpened(ImGuiFileDialog* vContextPtr) {
    if (vContextPtr != nullptr) {
        return vContextPtr->IsOpened();
    }

    return false;
}

IGFD_C_API IGFD_Selection IGFD_GetSelection(ImGuiFileDialog* vContextPtr, IGFD_ResultMode vMode) {
    IGFD_Selection res = IGFD_Selection_Get();
    if (vContextPtr != nullptr) {
        auto sel = vContextPtr->GetSelection(vMode);
        if (!sel.empty()) {
            res.count = sel.size();
            res.table = new IGFD_Selection_Pair[res.count];

            size_t idx = 0U;
            for (const auto& s : sel) {
                IGFD_Selection_Pair* pair = res.table + idx++;

                // fileNameExt
                if (!s.first.empty()) {
                    size_t siz     = s.first.size() + 1U;
                    pair->fileName = new char[siz];
#ifndef _MSC_VER
                    strncpy(pair->fileName, s.first.c_str(), siz);
#else   // _MSC_VER
                    strncpy_s(pair->fileName, siz, s.first.c_str(), siz);
#endif  // _MSC_VER
                    pair->fileName[siz - 1U] = '\0';
                }

                // filePathName
                if (!s.second.empty()) {
                    size_t siz         = s.second.size() + 1U;
                    pair->filePathName = new char[siz];
#ifndef _MSC_VER
                    strncpy(pair->filePathName, s.second.c_str(), siz);
#else   // _MSC_VER
                    strncpy_s(pair->filePathName, siz, s.second.c_str(), siz);
#endif  // _MSC_VER
                    pair->filePathName[siz - 1U] = '\0';
                }
            }

            return res;
        }
    }

    return res;
}

IGFD_C_API char* IGFD_GetFilePathName(ImGuiFileDialog* vContextPtr, IGFD_ResultMode vMode) {
    char* res = nullptr;

    if (vContextPtr != nullptr) {
        auto s = vContextPtr->GetFilePathName(vMode);
        if (!s.empty()) {
            size_t siz = s.size() + 1U;
            res        = (char*)malloc(siz);
            if (res) {
#ifndef _MSC_VER
                strncpy(res, s.c_str(), siz);
#else   // _MSC_VER
                strncpy_s(res, siz, s.c_str(), siz);
#endif  // _MSC_VER
                res[siz - 1U] = '\0';
            }
        }
    }

    return res;
}

IGFD_C_API char* IGFD_GetCurrentFileName(ImGuiFileDialog* vContextPtr, IGFD_ResultMode vMode) {
    char* res = nullptr;

    if (vContextPtr != nullptr) {
        auto s = vContextPtr->GetCurrentFileName(vMode);
        if (!s.empty()) {
            size_t siz = s.size() + 1U;
            res        = (char*)malloc(siz);
            if (res) {
#ifndef _MSC_VER
                strncpy(res, s.c_str(), siz);
#else   // _MSC_VER
                strncpy_s(res, siz, s.c_str(), siz);
#endif  // _MSC_VER
                res[siz - 1U] = '\0';
            }
        }
    }

    return res;
}

IGFD_C_API char* IGFD_GetCurrentPath(ImGuiFileDialog* vContextPtr) {
    char* res = nullptr;

    if (vContextPtr != nullptr) {
        auto s = vContextPtr->GetCurrentPath();
        if (!s.empty()) {
            size_t siz = s.size() + 1U;
            res        = (char*)malloc(siz);
            if (res) {
#ifndef _MSC_VER
                strncpy(res, s.c_str(), siz);
#else   // _MSC_VER
                strncpy_s(res, siz, s.c_str(), siz);
#endif  // _MSC_VER
                res[siz - 1U] = '\0';
            }
        }
    }

    return res;
}

IGFD_C_API char* IGFD_GetCurrentFilter(ImGuiFileDialog* vContextPtr) {
    char* res = nullptr;

    if (vContextPtr != nullptr) {
        auto s = vContextPtr->GetCurrentFilter();
        if (!s.empty()) {
            size_t siz = s.size() + 1U;
            res        = (char*)malloc(siz);
            if (res) {
#ifndef _MSC_VER
                strncpy(res, s.c_str(), siz);
#else   // _MSC_VER
                strncpy_s(res, siz, s.c_str(), siz);
#endif  // _MSC_VER
                res[siz - 1U] = '\0';
            }
        }
    }

    return res;
}

IGFD_C_API void* IGFD_GetUserDatas(ImGuiFileDialog* vContextPtr) {
    if (vContextPtr != nullptr) {
        return vContextPtr->GetUserDatas();
    }

    return nullptr;
}

IGFD_C_API void IGFD_SetFileStyle(ImGuiFileDialog* vContextPtr, IGFD_FileStyleFlags vFlags, const char* vCriteria, ImVec4 vColor, const char* vIcon,
                                  ImFont* vFont)  //-V813
{
    if (vContextPtr != nullptr) {
        vContextPtr->SetFileStyle(vFlags, vCriteria, vColor, vIcon, vFont);
    }
}

IGFD_C_API void IGFD_SetFileStyle2(ImGuiFileDialog* vContextPtr, IGFD_FileStyleFlags vFlags, const char* vCriteria, float vR, float vG, float vB, float vA, const char* vIcon, ImFont* vFont) {
    if (vContextPtr != nullptr) {
        vContextPtr->SetFileStyle(vFlags, vCriteria, ImVec4(vR, vG, vB, vA), vIcon, vFont);
    }
}

IGFD_C_API bool IGFD_GetFileStyle(ImGuiFileDialog* vContextPtr, IGFD_FileStyleFlags vFlags, const char* vCriteria, ImVec4* vOutColor, char** vOutIconText, ImFont** vOutFont) {
    if (vContextPtr != nullptr) {
        std::string icon;
        bool res = vContextPtr->GetFileStyle(vFlags, vCriteria, vOutColor, &icon, vOutFont);
        if (!icon.empty() && vOutIconText) {
            size_t siz    = icon.size() + 1U;
            *vOutIconText = (char*)malloc(siz);
            if (*vOutIconText) {
#ifndef _MSC_VER
                strncpy(*vOutIconText, icon.c_str(), siz);
#else   // _MSC_VER
                strncpy_s(*vOutIconText, siz, icon.c_str(), siz);
#endif  // _MSC_VER
                (*vOutIconText)[siz - 1U] = '\0';
            }
        }
        return res;
    }

    return false;
}

IGFD_C_API void IGFD_ClearFilesStyle(ImGuiFileDialog* vContextPtr) {
    if (vContextPtr != nullptr) {
        vContextPtr->ClearFilesStyle();
    }
}

IGFD_C_API void SetLocales(ImGuiFileDialog* vContextPtr, const int vCategory, const char* vBeginLocale, const char* vEndLocale) {
    if (vContextPtr != nullptr) {
        vContextPtr->SetLocales(vCategory, (vBeginLocale ? vBeginLocale : ""), (vEndLocale ? vEndLocale : ""));
    }
}

#ifdef USE_EXPLORATION_BY_KEYS
IGFD_C_API void IGFD_SetFlashingAttenuationInSeconds(ImGuiFileDialog* vContextPtr, float vAttenValue) {
    if (vContextPtr != nullptr) {
        vContextPtr->SetFlashingAttenuationInSeconds(vAttenValue);
    }
}
#endif

#ifdef USE_PLACES_FEATURE
IGFD_C_API char* IGFD_SerializePlaces(ImGuiFileDialog* vContextPtr, bool vDontSerializeCodeBasedPlaces) {
    char* res = nullptr;

    if (vContextPtr != nullptr) {
        auto s = vContextPtr->SerializePlaces(vDontSerializeCodeBasedPlaces);
        if (!s.empty()) {
            size_t siz = s.size() + 1U;
            res        = (char*)malloc(siz);
            if (res) {
#ifndef _MSC_VER
                strncpy(res, s.c_str(), siz);
#else   // _MSC_VER
                strncpy_s(res, siz, s.c_str(), siz);
#endif  // _MSC_VER
                res[siz - 1U] = '\0';
            }
        }
    }

    return res;
}

IGFD_C_API void IGFD_DeserializePlaces(ImGuiFileDialog* vContextPtr, const char* vPlaces) {
    if (vContextPtr != nullptr) {
        vContextPtr->DeserializePlaces(vPlaces);
    }
}

IGFD_C_API bool IGFD_AddPlacesGroup(ImGuiFileDialog* vContextPtr, const char* vGroupName, size_t vDisplayOrder, bool vCanBeEdited) {
    if (vContextPtr != nullptr) {
        return vContextPtr->AddPlacesGroup(vGroupName, vDisplayOrder, vCanBeEdited);
    }
    return false;
}

IGFD_C_API bool IGFD_RemovePlacesGroup(ImGuiFileDialog* vContextPtr, const char* vGroupName) {
    if (vContextPtr != nullptr) {
        return vContextPtr->RemovePlacesGroup(vGroupName);
    }
    return false;
}

IGFD_C_API bool IGFD_AddPlace(ImGuiFileDialog* vContextPtr, const char* vGroupName, const char* vPlaceName, const char* vPlacePath, bool vCanBeSaved, const char* vIconText) {
    if (vContextPtr != nullptr) {
        auto group_ptr = vContextPtr->GetPlacesGroupPtr(vGroupName);
        if (group_ptr != nullptr) {
            IGFD::FileStyle style;
            style.icon = vIconText;
            return group_ptr->AddPlace(vPlaceName, vPlacePath, vCanBeSaved, style);
        }
    }
    return false;
}

IGFD_C_API bool IGFD_RemovePlace(ImGuiFileDialog* vContextPtr, const char* vGroupName, const char* vPlaceName) {
    if (vContextPtr != nullptr) {
        auto group_ptr = vContextPtr->GetPlacesGroupPtr(vGroupName);
        if (group_ptr != nullptr) {
            return group_ptr->RemovePlace(vPlaceName);
        }
    }
    return false;
}     

#endif

#ifdef USE_THUMBNAILS
IGFD_C_API void SetCreateThumbnailCallback(ImGuiFileDialog* vContextPtr, const IGFD_CreateThumbnailFun vCreateThumbnailFun) {
    if (vContextPtr != nullptr) {
        vContextPtr->SetCreateThumbnailCallback(vCreateThumbnailFun);
    }
}

IGFD_C_API void SetDestroyThumbnailCallback(ImGuiFileDialog* vContextPtr, const IGFD_DestroyThumbnailFun vDestroyThumbnailFun) {
    if (vContextPtr != nullptr) {
        vContextPtr->SetDestroyThumbnailCallback(vDestroyThumbnailFun);
    }
}

IGFD_C_API void ManageGPUThumbnails(ImGuiFileDialog* vContextPtr) {
    if (vContextPtr != nullptr) {
        vContextPtr->ManageGPUThumbnails();
    }
}
#endif  // USE_THUMBNAILS

#pragma endregion
