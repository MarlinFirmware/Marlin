/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Copyright (c) 2021 X-Ryl669
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <limits.h>
#include <stdlib.h>
#include <memory.h>
#include <new>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
// std stuff
#include <string>
#include <functional>
#include <vector>

#if defined(__linux__) || defined(__APPLE__)
  #define _POSIX 1
#endif

#ifdef _WIN32
  #include <windows.h>
  #include <Mmsystem.h>
  #include <Nb30.h>
  #include <ws2tcpip.h>
  #include <tchar.h>
  #include <iphlpapi.h>
#elif defined(_POSIX)
  #include <errno.h>
  #include <execinfo.h>
  #include <fcntl.h>
  #include <netdb.h>
  #include <netinet/in.h>
  #include <pthread.h>
  #include <signal.h>
  #include <sys/poll.h>
  #include <sys/socket.h>
  #include <sys/time.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/ptrace.h>
  #include <aio.h>
  #include <ifaddrs.h>

  #if defined(__linux__)
    #include <semaphore.h>
  #elif defined(__APPLE__)
    #include <sys/sysctl.h>
    #include <sys/stat.h>
    #include <sys/uio.h>
    #include <net/route.h>
    #include <net/if_dl.h>
  #endif
#endif

/** All testing code frameword is declared inside this namespace */
namespace Test {
  inline bool isUnderDebugger() {
    #ifdef _WIN32
      return (IsDebuggerPresent() == TRUE);
    #elif defined(__linux__)
      static signed char testResult = 0;
      if (testResult == 0) {
        testResult = (char) ptrace (PT_TRACE_ME, 0, 0, 0);
        if (testResult >= 0) {
          ptrace (PT_DETACH, 0, (caddr_t) 1, 0);
          testResult = 1;
        }
      }
      return (testResult < 0);
    #elif defined (__APPLE__)
      static signed char testResult = 0;
      if (testResult == 0) {
        struct kinfo_proc info;
        int m[] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
        size_t sz = sizeof (info);
        sysctl (m, 4, &info, &sz, 0, 0);
        testResult = ((info.kp_proc.p_flag & P_TRACED) != 0) ? 1 : -1;
      }
      return testResult > 0;
    #else
      return false;
    #endif
  }
    
  /** This is used to trigger the debugger when called */
  inline void breakUnderDebugger() {
    if (isUnderDebugger()) {
      #ifdef _WIN32
        DebugBreak();
      #elif defined(__linux__)
        raise(SIGTRAP);
      #elif defined (__APPLE__)
        __asm__("int $3\n" : : );
      #else
        #error "Put your break into debugger code here."
      #endif
    }
  }

  // This can be changed by the Test wrapper to avoid stopping in debugged each time a test fails
  static bool & shouldWeBreak() { static bool BreakInDebug = true; return BreakInDebug; }
  
  /** A line and file path error tracker */
  struct _False {
    const char *  file;
    int           line;
    operator bool() const { fprintf(stderr, "> Failed test here: %s(%d)\n", file, line); if (shouldWeBreak()) breakUnderDebugger(); return false; }
    bool operator()(const char * msg) const { fprintf(stderr, "> Failed test because \"%s\" here: %s(%d)\n", msg, file, line); if (shouldWeBreak()) breakUnderDebugger(); return false; }
    _False(const char * file, const int line) : file(file), line(line) {}
  };
  #define False ::Test::_False(__FILE__, __LINE__)

  /** A unit test.
      You are unlikely to manipulate the Unit class directly, but instead you'll use the MakeUnitTest macro that's building the right Unit child class depending on the provided arguments
      @sa Test::MakeUnitTest */
  struct Unit {
    std::string testName;
    std::function<bool()> execute;
    bool launchTest() { return execute(); }
    std::string getTestName(const std::string & str = "") const { return testName + str; }

    Unit(std::string && name, std::function<bool()> && func) : testName(std::move(name)), execute(std::move(func))  {}
  };

  // The only instance of this object
  std::vector<Unit> testVectors;

  struct AutoRegisterTest {
    AutoRegisterTest(std::string && name, std::function<bool()> && func) { testVectors.emplace_back(std::move(name), std::move(func)); }
  };

  #define DeclareTest__(A,L) A##L 
  #define DeclareTest_(A,L) DeclareTest__(A,L)
  #define DeclareTest DeclareTest_(static ::Test::AutoRegisterTest test,__LINE__)
}
