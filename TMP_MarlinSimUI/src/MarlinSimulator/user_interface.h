#pragma once

#include <map>
#include <vector>
#include <deque>
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>
#include <filesystem>

#include <gl.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuiFileDialog.h>

#include "RingBuffer.h"
#include "logger.h"
#include "execution_control.h"

class UiWindow {
public:
  UiWindow(std::string name, std::function<void(UiWindow*)> show_callback = {}, std::function<void(UiWindow*)> menu_callback = {}) :
      name(name),
      show_callback(show_callback),
      menu_callback(menu_callback) { }

  virtual void show() {
    if (!active) return;
    if (!ImGui::Begin((char*)name.c_str(), &active, flags)) {
      ImGui::End();
      return;
    }
    if (menu_callback) menu_callback(this); else menu();
    if (show_callback) show_callback(this); else panel();
    ImGui::End();
  }

  virtual void panel() {

  }

  virtual void menu() {

  }

  virtual void enable() {
    active = true;
  }

  virtual void disable() {
    active = false;
  }

  virtual void select() {
    ImGuiWindow* window = ImGui::FindWindowByName(name.c_str());
    if (window == NULL || window->DockNode == NULL || window->DockNode->TabBar == NULL) return;
    window->DockNode->TabBar->NextSelectedTabId = window->ID;
    ;
  }

  bool listed_on_main_menu = true;
  std::string name;
  bool active            = true;
  ImGuiWindowFlags flags = 0;
  std::function<void(UiWindow*)> show_callback;
  std::function<void(UiWindow*)> menu_callback;
};

class UiPopup : public UiWindow {
public:
  template<class... Args> UiPopup(std::string name, bool is_modal, Args... args) : UiWindow(name, args...), m_is_modal{is_modal} {
    listed_on_main_menu = false;
   }

  virtual void show() override {
    if (m_will_open) {
      ImGui::OpenPopup(name.c_str());
      m_will_open = false;
    }

    if (((m_is_modal && ImGui::BeginPopupModal(name.c_str(), &active, flags)) || ImGui::BeginPopup(name.c_str(), flags))) {
      if (menu_callback) menu_callback(this);
      if (show_callback) show_callback(this);
      ImGui::EndPopup();
    }
  }

  void enable() override {
    m_will_open = true;
    UiWindow::enable();
  }
  bool m_is_modal = false;
  bool m_will_open = false;
};

class UserInterface {
public:
  UserInterface() = default;
  ~UserInterface();

  template <typename T, class... Args>
  std::shared_ptr<T> addElement(std::string id, Args&&... args) {
    if (ui_elements.find(id) != ui_elements.end()) throw std::runtime_error("UI IDs must be unique"); //todo: gracefulness
    auto element = std::make_shared<T>(id, args...);
    ui_elements[id] = element;
    return element;
  }

  template <typename T>
  static std::shared_ptr<T> getElement(const std::string id) {
    if (ui_elements.count(id) == 0) return nullptr;
    return std::dynamic_pointer_cast<T>(ui_elements.at(id));
  }

  void init(std::filesystem::path config_path);
  void show();
  void render();

  bool post_init_complete = false;
  std::function<void(void)> post_init;
  std::function<void(void)> m_main_menu;

  static std::map<std::string, std::shared_ptr<UiWindow>> ui_elements;
};

struct SerialMonitor : public UiWindow {
  SerialMonitor(std::string name) : UiWindow(name) {};
  char InputBuf[256] = {};
  std::string working_buffer;
  struct line_meta {
    std::size_t count = {};
    std::string text = {};
  };
  std::deque<line_meta> line_buffer{};
  std::deque<std::string> command_history{};
  std::size_t history_index = 0;
  std::string input_buffer = {};
  bool scroll_follow = true;
  uint8_t scroll_follow_state = false;
  bool streaming = false;
  std::size_t stream_sent = 0, stream_total = 0;
  bool copy_buffer_signal = false;
  std::ifstream input_file;
  const std::string file_dialog_key = "ChooseFileDlgKey";
  const std::string file_dialog_title = "Choose File";
  char const* file_dialog_filters     = "GCode(*.gcode *.gc *.g){.gcode,.gc,.g},.*";
  const std::string file_dialog_path = ".";

  InOutRingBuffer<uint8_t, 32768> serial_buffer;
  std::vector<InOutRingBuffer<uint8_t, 32768>*> serial_endpoints;

  void register_endpoint(InOutRingBuffer<uint8_t, 32768>* endpoint) {
    serial_endpoints.push_back(endpoint);
  }

  int input_callback(ImGuiInputTextCallbackData* data) {
    switch (data->EventFlag) {
      case ImGuiInputTextFlags_CallbackCompletion:   // TODO: just search history?
        break;

      case ImGuiInputTextFlags_CallbackHistory: {
        std::size_t history_index_prev = history_index;

        if (data->EventKey == ImGuiKey_UpArrow) {
          if (history_index < command_history.size()) history_index ++;
        }
        else if (data->EventKey == ImGuiKey_DownArrow) {
          if (history_index > 0) history_index--;
        }

        if (history_index > 0 && history_index != history_index_prev) {
          if (history_index_prev == 0)
            input_buffer = std::string{data->Buf};

          data->DeleteChars(0, data->BufTextLen);
          data->InsertChars(0, (char *)command_history[history_index - 1].c_str());
        }
        else if (history_index == 0) {
          data->DeleteChars(0, data->BufTextLen);
          data->InsertChars(0, (char *)input_buffer.c_str());
        }
      } break;
    }
    return 0;
  }

  void insert_text(std::string text) {
    working_buffer.append(text);
    std::size_t index = working_buffer.find('\n');
    while (index != std::string::npos) {
      if (line_buffer.size() == 0 || line_buffer.back().text != working_buffer.substr(0, index)) {
        line_buffer.push_back({1, working_buffer.substr(0, index)});
      } else line_buffer.back().count++;
      working_buffer = working_buffer.substr(index + 1);
      index = working_buffer.find('\n');
    }
  }

  void show() {
    // File read into serial port
    if (input_file.is_open() && serial_buffer.out.free() && streaming) {
      static uint8_t buffer[32768];
      size_t read_size = std::min(serial_buffer.out.free(), stream_total - stream_sent);
      read_size = std::min(read_size, size_t(32768));
      input_file.read((char*)buffer, read_size);
      serial_buffer.out.write(buffer, read_size);
      stream_sent += read_size;
      if (stream_sent >= stream_total) {
        input_file.close();
        streaming = false;
        stream_total = 0;
        stream_sent = 0;
      }
    }

    while (serial_buffer.in.available()) {
      static char buffer[32768 + 1];
      auto count = serial_buffer.in.read((uint8_t*)buffer, 32768);
      for (auto endpoint : serial_endpoints) {
        endpoint->in.write((uint8_t*)buffer, count);
      }
      buffer[count] = '\0';
      insert_text(buffer);
    }
    for (auto endpoint : serial_endpoints) {
      endpoint->out.read(serial_buffer.out);
    }


    if (!ImGui::Begin((char *)name.c_str(), nullptr, ImGuiWindowFlags_MenuBar)) {
      ImGui::End();
      return;
    }

    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("Stream")) {
        if (ImGui::MenuItem("Select GCode File")) {
          IGFD::FileDialogConfig config { file_dialog_path };
          config.flags |= ImGuiFileDialogFlags_Modal;
          ImGuiFileDialog::Instance()->OpenDialog(file_dialog_key, file_dialog_title, file_dialog_filters, config);
        }
        if (input_file.is_open() && streaming)
          if (ImGui::MenuItem("Pause")) {
            streaming = false;
          }
        if (input_file.is_open() && !streaming) {
          if (ImGui::MenuItem("Resume")) {
            streaming = true;
          }
        }
        if (input_file.is_open()) {
          if (ImGui::MenuItem("Cancel")) {
            input_file.close();
            streaming = false;
            stream_total = 0;
            stream_sent = 0;
          }
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Copy Buffer")) {
          copy_buffer_signal = true;
          ImGui::LogToClipboard();
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    if (ImGuiFileDialog::Instance()->IsOpened() &&  ImGuiFileDialog::Instance()->Display(file_dialog_key, ImGuiWindowFlags_NoDocking))  {
      if (ImGuiFileDialog::Instance()->IsOk()) {
        std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
        logger::info("Streaming file: %s", filePathName.c_str());
        input_file.open(filePathName);
        input_file.seekg(0, std::ios::end);
        stream_total = input_file.tellg();
        input_file.seekg(0, std::ios::beg);
        streaming = true;
      }
      ImGuiFileDialog::Instance()->Close();
    }
    if (stream_total) {
      ImGui::ProgressBar((float)stream_sent / stream_total);
    }
    ImGui::BeginGroup();
    const ImGuiWindowFlags child_flags = 0;
    const ImGuiID child_id = ImGui::GetID((void*)(intptr_t)0);
    auto size = ImGui::GetContentRegionAvail();
    size.y -= 25; // TODO: there must be a better way to fill 2 items on a line
    if (ImGui::BeginChild(child_id, size, true, child_flags)) {
      if (copy_buffer_signal) {
        ImGui::LogToClipboard();
      }
      ImGuiListClipper clipper;
      clipper.Begin(line_buffer.size());
      while(clipper.Step()) {
        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
          if (line_buffer.at(line_no).count > 1) ImGui::TextWrapped("[%ld] %s", line_buffer.at(line_no).count, (char *)line_buffer.at(line_no).text.c_str());
          else  ImGui::TextWrapped("%s", (char *)line_buffer.at(line_no).text.c_str());
        }
        //ImGui::TextWrapped("%s", (char *)working_buffer.c_str());
      }
      clipper.End();

      if (copy_buffer_signal) {
        copy_buffer_signal = false;
        ImGui::LogFinish();
      }

      // Automatically set follow when scrolled to max
      if (ImGui::GetScrollY() != ImGui::GetScrollMaxY() || scroll_follow_state == 2) scroll_follow = false;
      else scroll_follow = true;

      // Automatically rescroll to max when follow clicked
      if (scroll_follow || scroll_follow_state == 1) {
        scroll_follow_state = 0;
        ImGui::SetScrollHereY(1.0f);
      }
    }
    ImGui::EndChild();
    ImGui::EndGroup();

    // Command-line
    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
    ImGui::PushItemWidth(-27);
    if (ImGui::InputText("##SerialInput", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, [](ImGuiInputTextCallbackData* data) -> int { return ((SerialMonitor*)(data->UserData))->input_callback(data); }, (void*)this)) {
        std::string input(InputBuf);
        input.erase(std::find_if(input.rbegin(), input.rend(), [](int ch) { return !std::isspace(ch); }).base(), input.end()); //trim whitespace
        if (input.size()) {
          if (command_history.size() == 0 || command_history.front() != input) command_history.push_front(input);
          history_index = 0;
          input.push_back('\n');
          std::size_t count = serial_buffer.out.free();
          serial_buffer.out.write((uint8_t *)input.c_str(), std::min({count, input.size()}));
        }
        strcpy((char*)InputBuf, "");
        reclaim_focus = true;
    }
    ImGui::PopItemWidth();

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus) ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
    ImGui::SameLine();

    bool last_follow_state = scroll_follow;
    if (ImGui::Checkbox("##scroll_follow", &scroll_follow)) {
      if (last_follow_state != scroll_follow) {
        scroll_follow_state = last_follow_state == false ? 1 : 2;
      } else {
        scroll_follow_state = 0;
      }
    }
    ImGui::End();
  }

};

struct SerialController : public UiWindow {
  SerialController(std::string name) : UiWindow(name) {
    UserInterface::getElement<SerialMonitor>("Serial Monitor(0)")->register_endpoint(&serial_buffer);
  };
  InOutRingBuffer<uint8_t, 32768> serial_buffer;

  void process_response(std::string_view buffer) {
    // logger::info(buffer.data());

    if (buffer.substr(0, 2).compare("ok") == 0) {
      if (buffer.size() > 2) {
        // logger::info("is advanced ok");
      } else {
        // logger::info("is ok");
      }
    }

    if (buffer.substr(0, 2).compare("//") == 0) {
      // logger::info("is info for host");
    }

    if (buffer.substr(0, 4).compare("echo") == 0) {
      // logger::info("is echo");
    }

    if (buffer.substr(0, 2).compare("X:") == 0) {
      // logger::info("is position report");
    }

    if (buffer.substr(0, 3).compare(" T:") == 0) {
      // logger::info("is temp report");
    }

  }

  virtual void panel() override {
    static std::function<size_t(const char*)> serial_write = [this](const char* str){
      return serial_buffer.out.write((uint8_t*)str, strlen(str));
    };

    std::size_t index = 0;
    while (serial_buffer.in.find_next_index_of(uint8_t('\n'), index)) {
      static char buffer[513] {};
      serial_buffer.in.read((uint8_t*)buffer, index);
      serial_buffer.in.drop(1);
      buffer[index] = 0;
      process_response(buffer);
    }

    if (ImGui::Button("Home All")) {
      serial_write("G28\n");
    }

    // X Control Buttons
    if (ImGui::Button("-10##X")) {
      serial_write("G91\nG0X-10\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("-1##X")) {
      serial_write("G91\nG0X-1\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("-0.1##X")) {
      serial_write("G91\nG0X-0.1\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("Home X##X")) {
      serial_write("G28 X\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("+0.1##X")) {
      serial_write("G91\nG0X+0.1\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("+1##X")) {
      serial_write("G91\nG0X+1\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("+10##X")) {
      serial_write("G91\nG0X+10\nG90\n");
    }

    // Y Control Buttons
    if (ImGui::Button("-10##Y")) {
      serial_write("G91\nG0Y-10\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("-1##Y")) {
      serial_write("G91\nG0Y-1\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("-0.1##Y")) {
      serial_write("G91\nG0Y-0.1\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("Home Y##Y")) {
      serial_write("G28 Y\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("+0.1##Y")) {
      serial_write("G91\nG0Y+0.1\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("+1##Y")) {
      serial_write("G91\nG0Y+1\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("+10##Y")) {
      serial_write("G91\nG0Y+10\nG90\n");
    }

    // Z Control Buttons
    if (ImGui::Button("-10##Z")) {
      serial_write("G91\nG0Z-10\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("-1##Z")) {
      serial_write("G91\nG0Z-1\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("-0.1##Z")) {
      serial_write("G91\nG0Z-0.1\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("Home Z##Z")) {
      serial_write("G28 Z\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("+0.1##Z")) {
      serial_write("G91\nG0Z+0.1\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("+1##Z")) {
      serial_write("G91\nG0Z+1\nG90\n");
    }
    ImGui::SameLine();
    if (ImGui::Button("+10##Z")) {
      serial_write("G91\nG0Z+10\nG90\n");
    }
  }
};

struct TextureWindow : public UiWindow {
  bool hovered = false;
  bool focused = false;
  GLuint texture_id = 0;
  float aspect_ratio = 0.0f;

  template<class... Args>
  TextureWindow(std::string name, GLuint texture_id, float aratio, Args... args) : UiWindow(name, args...), texture_id{texture_id}, aspect_ratio(aratio) {}
  void show() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{2, 2});
    ImGui::Begin((char *)name.c_str(), nullptr);
    auto size = ImGui::GetContentRegionAvail();
    size.y = size.x / aspect_ratio;
    ImGui::Image((ImTextureID)(intptr_t)texture_id, size, ImVec2(0,0), ImVec2(1,1));
    hovered = ImGui::IsItemHovered();
    focused = ImGui::IsWindowFocused();
    if (show_callback) show_callback((UiWindow*)this);
    ImGui::End();
    ImGui::PopStyleVar();
  }
};

// todo: write a better one, taken from the demo just added colour
struct LoggerWindow : public UiWindow {
  ImGuiTextBuffer     Buf;
  ImGuiTextFilter     Filter;
  ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
  bool                AutoScroll;  // Keep scrolling if already at the bottom.

  template<class... Args>
  LoggerWindow(std::string name, Args... args) : UiWindow(name, args...) {
    AutoScroll = true;
    clear();
  }

  void clear() {
    Buf.clear();
    LineOffsets.clear();
    LineOffsets.push_back(0);
  }

  // todo: return span information and colour only the log level indicator?
  ImVec4 get_color(const char* start, const char* end) {
    const ImVec4 colors[6] = {
      ImVec4{1.0f, 1.0f, 1.0f, 1.0f},
      ImVec4{0.2f, 0.6f, 1.0f, 1.0f},
      ImVec4{0.0f, 1.0f, 0.0f, 1.0f},
      ImVec4{1.0f, 1.0f, 0.4f, 1.0f},
      ImVec4{1.0f, 0.0f, 0.0f, 1.0f},
      ImVec4{1.0f, 0.0f, 0.0f, 1.0f}
    };

    std::string_view view(start, end - start);
    auto result = view.find("trace]");
    if (result != std::string::npos) return colors[0];
    result = view.find("debug]");
    if (result != std::string::npos) return colors[1];
    result = view.find("info]");
    if (result != std::string::npos) return colors[2];
    result = view.find("warning]");
    if (result != std::string::npos) return colors[3];
    result = view.find("error]");
    if (result != std::string::npos) return colors[4];
    result = view.find("critical]");
    if (result != std::string::npos) return colors[5];

    return colors[0];
  }

  void add_log(const std::string_view value) {
    int old_size = Buf.size();
    Buf.append(value.cbegin(), value.cend());
    Buf.append("\n");
    for (int new_size = Buf.size(); old_size < new_size; old_size++) {
      if (Buf[old_size] == '\n') LineOffsets.push_back(old_size + 1);
    }
  }

  void show() {
    if (!active) return;
    if (!ImGui::Begin((char *)name.c_str(), &active, flags)) {
      ImGui::End();
      return;
    }

    // Options menu
    if (ImGui::BeginPopup("Options")) {
      ImGui::Checkbox("Auto-scroll", &AutoScroll);
      ImGui::EndPopup();
    }

    // Main window
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::SameLine();
    bool clear_button = ImGui::Button("Clear");
    ImGui::SameLine();
    bool copy_button = ImGui::Button("Copy");
    ImGui::SameLine();
    Filter.Draw("Filter", -100.0f);

    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    if (clear_button)
      clear();
    if (copy_button)
      ImGui::LogToClipboard();


    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    const char* buf = Buf.begin();
    const char* buf_end = Buf.end();
    if (Filter.IsActive()) {
      for (int line_no = 0; line_no < LineOffsets.Size; line_no++) {
        const char* line_start = buf + LineOffsets[line_no];
        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
        if (Filter.PassFilter(line_start, line_end)) {
          ImGui::PushStyleColor(ImGuiCol_Text, get_color(line_start, line_end));
          ImGui::TextUnformatted(line_start, line_end);
          ImGui::PopStyleColor();
        }
      }
    } else {
      ImGuiListClipper clipper;
      clipper.Begin(LineOffsets.Size);
      while (clipper.Step()) {
        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
          const char* line_start = buf + LineOffsets[line_no];
          const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
          ImGui::PushStyleColor(ImGuiCol_Text, get_color(line_start, line_end));
          ImGui::TextUnformatted(line_start, line_end);
          ImGui::PopStyleColor();
        }
      }
      clipper.End();
    }
    ImGui::PopStyleVar();

    if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
      ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
  }
};

struct GraphWindow : public UiWindow {
  bool hovered = false;
  bool focused = false;
  GLuint texture_id = 0;
  float aspect_ratio = 0.0f;

  template<class... Args>
  GraphWindow(std::string name, GLuint texture_id, float aratio, Args... args) : UiWindow(name, args...), texture_id{texture_id}, aspect_ratio(aratio) {}
  void show() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{2, 2});
    ImGui::Begin((char *)name.c_str(), nullptr);
    auto size = ImGui::GetContentRegionAvail();
    size.y = size.x / aspect_ratio;
    ImGui::Image((ImTextureID)(intptr_t)texture_id, size, ImVec2(0,0), ImVec2(1,1));
    hovered = ImGui::IsItemHovered();
    focused = ImGui::IsWindowFocused();
    ImGui::End();
    ImGui::PopStyleVar();
  }
};
