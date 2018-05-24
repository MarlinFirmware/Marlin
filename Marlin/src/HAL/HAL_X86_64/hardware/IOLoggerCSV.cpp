#ifdef __PLAT_X86_64__

#include "IOLoggerCSV.h"

IOLoggerCSV::IOLoggerCSV(std::string filename) {
  file.open(filename);
}

IOLoggerCSV::~IOLoggerCSV() {
  file.close();
}

void IOLoggerCSV::log(GpioEvent ev) {
  std::lock_guard<std::mutex> lock(vector_lock);
  events.push_back(ev); //minimal impact to signal handler
}

void IOLoggerCSV::flush() {
  { std::lock_guard<std::mutex> lock(vector_lock);
    while (!events.empty()) {
      file << events.front().timestamp << ", "<< events.front().pin_id << ", " << events.front().event << std::endl;
      events.pop_front();
    }
  }
  file.flush();
}

#endif // __PLAT_X86_64__
