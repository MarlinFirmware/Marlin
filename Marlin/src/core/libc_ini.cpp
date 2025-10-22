#include <stdint.h>

extern "C" {

// Секция: массив указателей на void-функции
extern void (*__init_array_start[])(void);
extern void (*__init_array_end[])(void);

void safe_init_array(void) {
  for (void (**func)() = __init_array_start; func < __init_array_end; func++) {
    if (*func &&
        (uintptr_t)(*func) > 0x08000000 &&
        (uintptr_t)(*func) < 0x08080000) {
      (*func)();
    }
  }
}

}