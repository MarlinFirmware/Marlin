// /* syscalls.c - AT32F403A için Syscalls (C dili) */
// #include <errno.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <reent.h>

// // AT32 başlık dosyaları
// #include "at32f403a_407.h"
// #include "at32f403a_407_usart.h"

// /* AT32 USART1 register adresleri */

// #define USART_STS      (*((volatile uint32_t *)(USART1_BASE + 0x00)))
// #define USART_DT       (*((volatile uint32_t *)(USART1_BASE + 0x04)))


// /* Linker sembolleri */
// extern char _end;
// extern char _estack;
// extern unsigned int _Min_Stack_Size;

// /* MSP okuma fonksiyonu (ARM Cortex-M4) */
// static uint32_t get_msp(void) {
//     uint32_t result;
//     __asm volatile ("MRS %0, msp" : "=r" (result));
//     return result;
// }

// /* ========== _sbrk ========== */
// void *_sbrk(int incr) {
//     static char *heap_end = NULL;
//     char *prev_heap_end;
//     char *stack_limit;
    
//     /* İlk çağrıda başlat */
//     if (heap_end == NULL) {
//         heap_end = &_end;
//     }
    
//     /* Stack limitini hesapla: _estack - _Min_Stack_Size */
//     stack_limit = (char *)((uintptr_t)&_estack - (uintptr_t)&_Min_Stack_Size);
    
//     /* Heap ve stack çakışması kontrolü */
//     if ((uintptr_t)heap_end + incr > (uintptr_t)stack_limit) {
//         errno = ENOMEM;
//         return (void *)-1;
//     }
    
//     prev_heap_end = heap_end;
//     heap_end += incr;
//     return (void *)prev_heap_end;
// }

// /* ========== _sbrk_r ========== */
// void *_sbrk_r(struct _reent *ptr, int incr) {
//     (void)ptr;
//     return _sbrk(incr);
// }

// /* ========== _write_r ========== */
// int _write_r(struct _reent *ptr, int fd, const void *buf, size_t cnt) {
//     const char *buffer;
//     size_t i;
    
//     (void)ptr; /* Kullanılmıyor */
    
//     /* stdout (1) veya stderr (2) */
//     if (fd == 1 || fd == 2) {
//         buffer = (const char *)buf;
        
//         for (i = 0; i < cnt; i++) {
//             /* TX buffer boşalana kadar bekle */
//             while ((USART_STS & USART_TDBE_FLAG) == 0) {
//                 /* busy wait */
//             }
//             USART_DT = buffer[i];
//         }
//         return (int)cnt;
//     }
    
//     errno = EBADF;
//     return -1;
// }

// /* ========== Diğer syscalls ========== */
// int _read_r(struct _reent *ptr, int fd, void *buf, size_t cnt) {
//     (void)ptr; (void)fd; (void)buf; (void)cnt;
//     return 0;
// }

// int _close_r(struct _reent *ptr, int fd) {
//     (void)ptr; (void)fd;
//     return -1;
// }

// int _fstat_r(struct _reent *ptr, int fd, struct stat *pstat) {
//     (void)ptr; (void)fd;
//     pstat->st_mode = 0x2000; /* S_IFCHR */
//     return 0;
// }

// int _isatty_r(struct _reent *ptr, int fd) {
//     (void)ptr; (void)fd;
//     return 1;
// }

// long _lseek_r(struct _reent *ptr, int fd, long pos, int whence) {
//     (void)ptr; (void)fd; (void)pos; (void)whence;
//     return 0;
// }

// /* ========== Non-reentrant syscalls ========== */
// int _write(int fd, const void *buf, size_t cnt) {
//     return _write_r(_REENT, fd, buf, cnt);
// }

// int _read(int fd, void *buf, size_t cnt) {
//     return _read_r(_REENT, fd, buf, cnt);
// }

// int _close(int fd) {
//     return _close_r(_REENT, fd);
// }

// int _fstat(int fd, struct stat *pstat) {
//     return _fstat_r(_REENT, fd, pstat);
// }

// int _isatty(int fd) {
//     return _isatty_r(_REENT, fd);
// }

// long _lseek(int fd, long pos, int whence) {
//     return _lseek_r(_REENT, fd, pos, whence);
// }

// void _exit(int status) {
//     (void)status;
//     while (1) {}
// }

// int _kill(int pid, int sig) {
//     (void)pid; (void)sig;
//     errno = EINVAL;
//     return -1;
// }

// int _getpid(void) {
//     return 1;
// }






/**
  * \file syscalls_at32.c
  *
  * Implementation of newlib syscall.
  *
  */
// #include "at32f403a_407.h"
// #include "at32f403a_407_conf.h"
// #if defined (  __GNUC__  ) /* GCC CS3 */
//   #include <sys/stat.h>
// #endif
// #include <errno.h>
// #undef errno
// extern int errno;


// // Helper macro to mark unused parameters and prevent compiler warnings.
// // Appends _UNUSED to the variable name to prevent accidentally using them.
// #ifdef UNUSED
//   #undef UNUSED
// #endif
// #ifdef __GNUC__
//   #define UNUSED(x) x ## _UNUSED __attribute__((__unused__))
// #else
//   #define UNUSED(x) x ## _UNUSED
// #endif

// __attribute__((weak))
// caddr_t _sbrk(int incr)
// {
//   extern char _estack; /* Defined in the linker script */
//   extern char _Min_Stack_Size; /* Defined in the linker script */
//   extern char _end; /* Defined by the linker */
//   static char *heap_end = &_end ;
//   char *prev_heap_end = heap_end;

//   if (heap_end + incr > (char *)__get_MSP()) {
//     /* Heap and stack collision */
//     errno = ENOMEM;
//     return (caddr_t) -1;
//   }
//   /* Ensure to keep minimum stack size defined in the linker script */
//   if (heap_end + incr >= (char *)(&_estack - &_Min_Stack_Size)) {
//     errno = ENOMEM;
//     return (caddr_t) -1;
//   }

//   heap_end += incr ;
//   return (caddr_t) prev_heap_end ;
// }

// __attribute__((weak))
// int _close(UNUSED(int file))
// {
//   return -1;
// }

// __attribute__((weak))
// int _fstat(UNUSED(int file), struct stat *st)
// {
//   st->st_mode = S_IFCHR ;
//   return 0;
// }

// __attribute__((weak))
// int _isatty(UNUSED(int file))
// {
//   return 1;
// }

// __attribute__((weak))
// int _lseek(UNUSED(int file), UNUSED(int ptr), UNUSED(int dir))
// {
//   return 0;
// }

// __attribute__((weak))
// int _read(UNUSED(int file), UNUSED(char *ptr), UNUSED(int len))
// {
//   return 0;
// }

// /* Moved to Print.cpp to support Print::printf()
// __attribute__((weak))
// int _write(UNUSED(int file), char *ptr, int len)
// {
// }
// */

// __attribute__((weak))
// void _exit(UNUSED(int status))
// {
//   for (; ;) ;
// }

// __attribute__((weak))
// int _kill(UNUSED(int pid), UNUSED(int sig))
// {
//   errno = EINVAL;
//   return -1;
// }

// __attribute__((weak))
// int _getpid(void)
// {
//   return 1;
// }

// #include "at32f403a_407.h"
// #include "at32f403a_407_conf.h"
// #if defined (  __GNUC__  ) /* GCC CS3 */
//   #include <sys/stat.h>
// #endif
// #include <errno.h>
// #undef errno
// extern int errno;

// #include <sys/types.h>
// #include <errno.h>
// #ifdef UNUSED
//   #undef UNUSED
// #endif
// #ifdef __GNUC__
//   #define UNUSED(x) x ## _UNUSED __attribute__((__unused__))
// #else
//   #define UNUSED(x) x ## _UNUSED
// #endif

// extern char _end;           // RAM'deki bss sonu (otomatik gelir)
// extern char _estack;        // RAM'in en üstü (stack başlangıcı)

// #define STACK_SAFETY  512   // Stack için 512 byte kenarda bırakalım

// void *_sbrk(int incr) {
//     static char *heap_end = NULL;
//     char *prev_heap_end;
//     uintptr_t stack_limit;
    
//     /* İlk çağrıda başlat */
//     if (heap_end == NULL) {
//         heap_end = &_end;
//     }
    
//     /* Stack limitini uintptr_t olarak hesapla (array subscript hatasını önler) */
//     stack_limit = (uintptr_t)&_estack - STACK_SAFETY;
    
//     /* Heap ve stack çakışması kontrolü */
//     if ((uintptr_t)heap_end + incr > stack_limit) {
//         errno = ENOMEM;
//         return (void *)-1;
//     }
    
//     prev_heap_end = heap_end;
//     heap_end += incr;
//     return (void *)prev_heap_end;
// }


// int _close(UNUSED(int file))
// {
//   return -1;
// }


// int _fstat(UNUSED(int file), struct stat *st)
// {
//   st->st_mode = S_IFCHR ;
//   return 0;
// }


// int _isatty(UNUSED(int file))
// {
//   return 1;
// }


// int _lseek(UNUSED(int file), UNUSED(int ptr), UNUSED(int dir))
// {
//   return 0;
// }


// int _read(UNUSED(int file), UNUSED(char *ptr), UNUSED(int len))
// {
//   return 0;
// }

// /* Moved to Print.cpp to support Print::printf()
// __attribute__((weak))
// int _write(UNUSED(int file), char *ptr, int len)
// {
// }
// */


// void _exit(UNUSED(int status))
// {
//   for (; ;) ;
// }


// int _kill(UNUSED(int pid), UNUSED(int sig))
// {
//   errno = EINVAL;
//   return -1;
// }


// int _getpid(void)
// {
//   return 1;
// }

// #ifdef __cplusplus
// extern "C" {
// #endif

// syscalls.c
// AT32F403A / STM32F4 benzeri bare-metal projeler için
// _sbrk + _exit + _kill + diğer temel syscall stub'ları
// Rıza - 2026

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>
// // AT32 başlık dosyaları
#include "at32f403a_407.h"
#include "at32f403a_407_usart.h"

/* AT32 USART1 register adresleri */

#define USART_STS      (*((volatile uint32_t *)(USART1_BASE + 0x00)))
#define USART_DT       (*((volatile uint32_t *)(USART1_BASE + 0x04)))

// Linker tarafından otomatik tanımlanan semboller
extern uint8_t _end;       // .bss sonu → heap başlangıcı
extern uint8_t _estack;    // RAM'in en üstü (stack aşağı büyür)

// Stack için ayıracağımız güvenlik bölgesi (bayt cinsinden)
#define MIN_STACK_REMAIN  2048   // 2 KB genelde yeter, gerekirse artır

// Heap pointer'ı (statik değişken)
//static uint8_t* heap_end = NULL;

// ---------------------------------------------
// En önemli: Heap yönetimi (_sbrk)
// ---------------------------------------------
// caddr_t _sbrk_r(struct _reent* r, ptrdiff_t incr)
// {
//     uint8_t* prev_heap_end;

//     (void)r;  // kullanılmıyor uyarısını engellemek için

//     if (heap_end == NULL)
//         heap_end = &_end;           // İlk çağrıda .bss sonundan başla

//     prev_heap_end = heap_end;

//     // Heap + istenen artış > RAM sonu - stack güvenlik alanı ?
//     if ((uintptr_t)(heap_end + incr) > (uintptr_t)&_estack - MIN_STACK_REMAIN)
//     {
//         r->_errno = ENOMEM;
//         return (caddr_t)-1;         // Bellek hatası
//     }

//     heap_end += incr;
//     return (caddr_t)prev_heap_end;
// }

// Basit versiyonu da ekliyoruz (bazı eski kodlar bunu çağırır)



// caddr_t _sbrk_r(struct _reent *r, ptrdiff_t incr)
// {
//     uint8_t *prev_heap_end;

//     (void)r;  // kullanılmıyor uyarısını engelle

//     if (heap_end == NULL)
//         heap_end = (uint8_t *)&_end;

//     prev_heap_end = heap_end;

//     if ((uintptr_t)(heap_end + incr) > (uintptr_t)&_estack - MIN_STACK_REMAIN)
//     {
//         errno = ENOMEM;
//         if (r) r->_errno = ENOMEM;
//         return (caddr_t)-1;
//     }

//     heap_end += incr;
//     return (caddr_t)prev_heap_end;
// }

/* Basit wrapper – bazı eski kodlar bunu çağırır */
// Sadece prototip (tanım yok!)
// extern caddr_t _sbrk_r(struct _reent *r, ptrdiff_t incr);

// // Basit wrapper – newlib bunu çağırır
// caddr_t _sbrk(ptrdiff_t incr)
// {
//     return _sbrk_r(NULL, incr);
// }

static uint8_t *heap_end;

caddr_t _sbrk(ptrdiff_t incr)
{
    if (heap_end == 0)
        heap_end = &_end;

    uintptr_t next = (uintptr_t)heap_end + incr;
    uintptr_t stack_limit = (uintptr_t)&_estack - 512;

    if (next >= stack_limit) {
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    caddr_t prev = (caddr_t)heap_end;
    heap_end = (uint8_t *)next;

    return prev;
}



// ---------------------------------------------
// Program sonlandırma (_exit)
// ---------------------------------------------
void _exit(int status)
{
    // Bare-metal ortamda çıkış yok → sonsuz döngü
    // İstersen buraya LED yakma, hata gösterme vs. koyabilirsin
    while (1)
    {
        __asm volatile ("nop");   // işlemciyi çok yormamak için
    }
    (void)status;                 // kullanılmadı uyarısını engelle
}

// ---------------------------------------------
// Process öldürme (_kill)
// ---------------------------------------------
int _kill(pid_t pid, int sig)
{
    // Bare-metal'de process yok → hata döndür
    errno = EINVAL;
    return -1;

    (void)pid;
    (void)sig;
}

// ---------------------------------------------
// Process ID alma (_getpid)
// ---------------------------------------------
pid_t _getpid(void)
{
    return 1;   // Tek process varmış gibi davranıyoruz
}

// ---------------------------------------------
// Diğer sık çıkan dummy implementasyonlar
// (İhtiyacınız olursa yorum satırını kaldırın)
// ---------------------------------------------

//#if 0   // Genelde gerekmez, ama hata alırsanız açabilirsiniz

int _close(int file)
{
    (void)file;
    return -1;
}

int _fstat(int file, struct stat* st)
{
    (void)file;
    st->st_mode = S_IFCHR;  // karakter cihazı gibi davran
    return 0;
}

int _isatty(int file)
{
    (void)file;
    return 1;   // her şey terminalmiş gibi
}

int _lseek(int file, int ptr, int dir)
{
    (void)file; (void)ptr; (void)dir;
    return -1;
}

int _read(int file, char* ptr, int len)
{
    (void)file; (void)ptr; (void)len;
    return -1;   // okuma desteklenmiyor
}

int _write(struct _reent *ptr, int fd, const void *buf, size_t cnt) {
    const char *buffer;
    size_t i;
    
    (void)ptr; /* Kullanılmıyor */
    
    /* stdout (1) veya stderr (2) */
    if (fd == 1 || fd == 2) {
        buffer = (const char *)buf;
        
        for (i = 0; i < cnt; i++) {
            /* TX buffer boşalana kadar bekle */
            while ((USART_STS & USART_TDBE_FLAG) == 0) {
                /* busy wait */
            }
            USART_DT = buffer[i];
        }
        return (int)cnt;
    }
    
    errno = EBADF;
    return -1;
}



// int _write(int file, const char* ptr, int len)
// {
//     // İsterseniz buraya UART gönderme koyabilirsiniz
//     // Örnek:
//     // for(int i = 0; i < len; i++) UART_SendChar(ptr[i]);
//     (void)file;
//     return len;   // "yazmış gibi" yap
// }

//#endif