#include "src/MarlinCore.h"
#if HAS_PRUSA_MMU3
#include "mmu2_log.h"
namespace MMU2 {

void LogErrorEvent_P(const char *msg){
  SERIAL_ECHO_START; //!@todo Decide MMU errors on serial line
  SERIAL_MMU2();
  SERIAL_ECHOLN_P(msg);
}

void LogEchoEvent_P(const char *msg){
  SERIAL_ECHO_START;
  SERIAL_MMU2();
  SERIAL_ECHOLN_P(msg);
}

} // namespace MMU2
#endif // HAS_PRUSA_MMU3
