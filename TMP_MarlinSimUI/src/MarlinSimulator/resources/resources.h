#pragma once

#include <filesystem>
#include <map>
#include <vector>
#include <string_view>

namespace resource {

class Resource {
public:
  Resource(const char * buffer = nullptr) {
     if (buffer) m_buffer = { buffer };
  }
  virtual ~Resource() {}
  std::string_view m_buffer;
  virtual bool reload() { return false; };
};

class FileResource : public Resource {
public:
  FileResource(std::filesystem::path path);
  ~FileResource() { }

  void load();
  virtual bool reload() override;

  std::vector<char> m_data;
  std::filesystem::path m_path;
  std::filesystem::file_time_type m_modified;
};

class ResourceManager {
public:
  static std::shared_ptr<Resource> get(std::filesystem::path path);
  static const char* get_as_cstr(std::filesystem::path path);
  static std::string_view get_as_sv(std::filesystem::path path);
private:
  static std::map<std::filesystem::path, std::shared_ptr<Resource>> s_embedded_resource;
  static std::map<std::filesystem::path, std::shared_ptr<Resource>> s_loaded_resource;
  ResourceManager();
};

}
