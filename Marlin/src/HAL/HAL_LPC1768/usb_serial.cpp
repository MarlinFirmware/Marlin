#ifdef TARGET_LPC1768
#include "../../inc/MarlinConfigPre.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/emergency_parser.h"
  EmergencyParser::State emergency_state;
  bool CDC_RecvCallback(const char buffer) {
    emergency_parser.update(emergency_state, buffer);
    return true;
  }
#endif // ENABLED(EMERGENCY_PARSER)

#endif // TARGET_LPC1768
