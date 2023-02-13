/*
 * File      : rtdef.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2011, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2007-01-10     Bernard      the first version
 * 2008-07-12     Bernard      remove all rt_int8, rt_uint32_t etc typedef
 * 2010-10-26     yi.qiu       add module support
 * 2010-11-10     Bernard      add cleanup callback function in thread exit.
 * 2011-05-09     Bernard      use builtin va_arg in GCC 4.x
 */

#ifndef __RT_DEF_H__
#define __RT_DEF_H__

// liao
// #include <rtconfig.h>
#define RT_NAME_MAX 12

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup BasicDef
 */
/*@{*/

/**
 * RT-Thread version information
 */
#define RT_VERSION 1L	 /**< major version number */
#define RT_SUBVERSION 0L /**< minor version number */
#define RT_REVISION 0L	 /**< revise version number */

	/**
	 * RT-Thread basic data type definitions
	 */
	typedef signed char rt_int8_t;		/**<  8bit integer type */
	typedef signed short rt_int16_t;	/**< 16bit integer type */
	typedef signed long rt_int32_t;		/**< 32bit integer type */
	typedef unsigned char rt_uint8_t;	/**<  8bit unsigned integer type */
	typedef unsigned short rt_uint16_t; /**< 16bit unsigned integer type */
	typedef unsigned long rt_uint32_t;	/**< 32bit unsigned integer type */
	typedef int rt_bool_t;				/**< boolean type */

	/* 32bit CPU */
	typedef long rt_base_t;			  /**< Nbit CPU related date type */
	typedef unsigned long rt_ubase_t; /**< Nbit unsigned CPU related data type */

	typedef rt_base_t rt_err_t;	   /**< Type for error number      */
	typedef rt_uint32_t rt_time_t; /**< Type for time stamp        */
	typedef rt_uint32_t rt_tick_t; /**< Type for tick count        */
	typedef rt_base_t rt_flag_t;   /**< Type for flags             */
	typedef rt_ubase_t rt_size_t;  /**< Type for size number       */
	typedef rt_ubase_t rt_dev_t;   /**< Type for device            */
	typedef rt_base_t rt_off_t;	   /**< Type for offset            */

/**
 * boolean type definitions
 */
#define RT_TRUE 1  /**< boolean true  */
#define RT_FALSE 0 /**< boolean fails */
/*@}*/

/* maximum value of base type */
#define RT_UINT8_MAX 0xff		  /* Maxium number of UINT8   */
#define RT_UINT16_MAX 0xffff	  /* Maxium number of UINT16  */
#define RT_UINT32_MAX 0xffffffff  /* Maxium number of UINT32  */
#define RT_TICK_MAX RT_UINT32_MAX /* Maxium number of tick    */

/* Compiler Related Definitions */
#ifdef __CC_ARM /* ARM Compiler */
#include <stdarg.h>
#define SECTION(x) __attribute__((section(x)))
#define UNUSED __attribute__((unused))
#define ALIGN(n) __attribute__((aligned(n)))
#define rt_inline static __inline
/* module compiling */
#ifdef RT_USING_MODULE
#define RTT_API __declspec(dllimport)
#else
#define RTT_API __declspec(dllexport)
#endif

#elif defined(__ICCARM__) /* for IAR Compiler */
#include <stdarg.h>
#define SECTION(x) @x
// #define UNUSED  /*��xinchang*/
#define PRAGMA(x) _Pragma(#x)
#define ALIGN(n) PRAGMA(data_alignment = n)
#define rt_inline static inline
#define RTT_API

#elif defined(__GNUC__) /* GNU GCC Compiler */
#ifdef RT_USING_NEWLIB
#include <stdarg.h>
#else
#if __GNUC__ < 4
typedef void *__sys_va_list;
typedef __sys_va_list va_list;
#define __va_rounded_size(type) \
	(((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(ap, lastarg) \
	(ap = ((char *)&(lastarg) + __va_rounded_size(lastarg)))
#define va_end(ap) ((void)0)
/*	little endian */
#define va_arg(ap, type)                                           \
	(ap = (__sys_va_list)((char *)(ap) + __va_rounded_size(type)), \
	 *((type *)(void *)((char *)(ap)-__va_rounded_size(type))))
#else
typedef __builtin_va_list __gnuc_va_list;
typedef __gnuc_va_list va_list;
#define va_start(v, l) __builtin_va_start(v, l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v, l) __builtin_va_arg(v, l)
#endif
#endif

#define SECTION(x) __attribute__((section(x)))
#define UNUSED __attribute__((unused))
#define ALIGN(n) __attribute__((aligned(n)))
#define rt_inline static __inline
#define RTT_API
#endif

/* event length */
#define RT_EVENT_LENGTH 32

/* memory management option */
#define RT_MM_PAGE_SIZE 4096
#define RT_MM_PAGE_MASK (RT_MM_PAGE_SIZE - 1)
#define RT_MM_PAGE_BITS 12

/**
 * @addtogroup Error
 */
/*@{*/
/** RT-Thread error code definitions */
#define RT_EOK 0	  /**< There is no error       */
#define RT_ERROR 1	  /**< A generic error happens */
#define RT_ETIMEOUT 2 /**< Timed out               */
#define RT_EFULL 3	  /**< The resource is full    */
#define RT_EEMPTY 4	  /**< The resource is empty   */
#define RT_ENOMEM 5	  /**< No memory               */
#define RT_ENOSYS 6	  /**< No system               */
#define RT_EBUSY 7	  /**< Busy                    */
#define RT_EIO 8	  /**< IO error 				 */
/*@}*/

/**
 * @ingroup BasicDef
 *
 * @def RT_ALIGN(size, align)
 * Return the most contiguous size aligned at specified width. RT_ALIGN(13, 4)
 * would return 16.
 */
#define RT_ALIGN(size, align) (((size) + (align)-1) & ~((align)-1))
/**
 * @ingroup BasicDef
 *
 * @def RT_ALIGN_DOWN(size, align)
 * Return the down number of aligned at specified width. RT_ALIGN_DOWN(13, 4)
 * would return 12.
 */
#define RT_ALIGN_DOWN(size, align) ((size) & ~((align)-1))

/**
 * @ingroup BasicDef
 *
 * @def RT_NULL
 * Similar as the \c NULL in C library.
 */
#define RT_NULL ((void *)0)

	struct rt_list_node
	{
		struct rt_list_node *next; /* point to next node. */
		struct rt_list_node *prev; /* point to prev node. */
	};
	typedef struct rt_list_node rt_list_t; /* Type for lists.     */

/**
 * @addtogroup KernelObject
 */
/*@{*/

/*
 * kernel object macros
 */
#define RT_OBJECT_FLAG_MODULE 0x80 /* is module object.   */

	/**
	 * Base structure of Kernel object
	 */
	struct rt_object
	{
		char name[RT_NAME_MAX]; /**< name of kernel object      */
		rt_uint8_t type;		/**< type of kernel object      */
		rt_uint8_t flag;		/**< flag of kernel object      */

#ifdef RT_USING_MODULE
		void *module_id; /**< id of application module   */
#endif
		rt_list_t list; /**< list node of kernel object */
	};
	typedef struct rt_object *rt_object_t; /* Type for kernel objects.     */

	/**
	 *  The object type can be one of the follows with specific
	 *  macros enabled:
	 *  - Thread
	 *  - Semaphore
	 *  - Mutex
	 *  - Event
	 *  - MailBox
	 *  - MessageQueue
	 *  - MemPool
	 *  - Device
	 *  - Timer
	 *  - Unknown
	 *  - Static
	 */
	enum rt_object_class_type
	{
		RT_Object_Class_Thread = 0, /**< The object is a thread.                */
#ifdef RT_USING_SEMAPHORE
		RT_Object_Class_Semaphore, /**< The object is a semaphore.             */
#endif
#ifdef RT_USING_MUTEX
		RT_Object_Class_Mutex, /**< The object is a mutex.                 */
#endif
#ifdef RT_USING_EVENT
		RT_Object_Class_Event, /**< The object is a event.                 */
#endif
#ifdef RT_USING_MAILBOX
		RT_Object_Class_MailBox, /**< The object is a mail box.              */
#endif
#ifdef RT_USING_MESSAGEQUEUE
		RT_Object_Class_MessageQueue, /**< The object is a message queue.         */
#endif
#ifdef RT_USING_MEMPOOL
		RT_Object_Class_MemPool, /**< The object is a memory pool.           */
#endif
#ifdef RT_USING_DEVICE
		RT_Object_Class_Device, /**< The object is a device                 */
#endif
		RT_Object_Class_Timer, /**< The object is a timer.                 */
#ifdef RT_USING_MODULE
		RT_Object_Class_Module, /**< The object is a module.                */
#endif
		RT_Object_Class_Unknown,	  /**< The object is unknown.                 */
		RT_Object_Class_Static = 0x80 /**< The object is a static object.         */
	};

	/**
	 * The information of the kernel object
	 */
	struct rt_object_information
	{
		enum rt_object_class_type type; /**< object class type                      */
		rt_list_t object_list;			/**< object list                            */
		rt_size_t object_size;			/**< object size                            */
	};

/**
 * The hook function call macro
 */
#ifdef RT_USING_HOOK
#define RT_OBJECT_HOOK_CALL(func, argv) \
	do                                  \
	{                                   \
		if ((func) != RT_NULL)          \
			func argv;                  \
	} while (0)
#else
#define RT_OBJECT_HOOK_CALL(func, argv)
#endif

/*@}*/

/**
 * @addtogroup Clock
 */
/*@{*/

/**
 * clock & timer macros
 */
#define RT_TIMER_FLAG_DEACTIVATED 0x0 /**< timer is deactive                      */
#define RT_TIMER_FLAG_ACTIVATED 0x1	  /**< timer is active                        */
#define RT_TIMER_FLAG_ONE_SHOT 0x0	  /**< one shot timer                         */
#define RT_TIMER_FLAG_PERIODIC 0x2	  /**< periodic timer                         */

#define RT_TIMER_FLAG_HARD_TIMER 0x0 /**< hard timer,the timer's callback function will be called in tick isr.     */
#define RT_TIMER_FLAG_SOFT_TIMER 0x4 /**< soft timer,the timer's callback function will be called in timer thread. */

#define RT_TIMER_CTRL_SET_TIME 0x0	   /**< set timer control command              */
#define RT_TIMER_CTRL_GET_TIME 0x1	   /**< get timer control command              */
#define RT_TIMER_CTRL_SET_ONESHOT 0x2  /**< change timer to one shot               */
#define RT_TIMER_CTRL_SET_PERIODIC 0x3 /**< change timer to periodic               */

	/**
	 * timer structure
	 */
	struct rt_timer
	{
		struct rt_object parent; /**< inherit from rt_object                 */

		rt_list_t list; /**< the node of timer list                 */

		void (*timeout_func)(void *parameter); /**< timeout function                       */
		void *parameter;					   /**< timeout function's parameter           */

		rt_tick_t init_tick;	/**< timer timeout tick                     */
		rt_tick_t timeout_tick; /**< timeout tick                           */
	};
	typedef struct rt_timer *rt_timer_t;
/*@}*/

/**
 * @addtogroup Thread
 */
/*@{*/

/*
 * Thread
 */

/*
 * thread state definitions
 */
#define RT_THREAD_INIT 0x00				  /**< Initialized status                 */
#define RT_THREAD_READY 0x01			  /**< Ready status                       */
#define RT_THREAD_SUSPEND 0x02			  /**< Suspend status                     */
#define RT_THREAD_RUNNING 0x03			  /**< Running status                     */
#define RT_THREAD_BLOCK RT_THREAD_SUSPEND /**< Blocked status                     */
#define RT_THREAD_CLOSE 0x04			  /**< Closed status                      */

/**
 * thread control command definitions
 */
#define RT_THREAD_CTRL_STARTUP 0x00			/**< Starup thread.                     */
#define RT_THREAD_CTRL_CLOSE 0x01			/**< Close thread.                      */
#define RT_THREAD_CTRL_CHANGE_PRIORITY 0x02 /**< Change thread priority.            */
#define RT_THREAD_CTRL_INFO 0x03			/**< Get thread information.            */

	typedef struct rt_thread *rt_thread_t;

	/**
	 * Thread structure
	 *
	 */
	struct rt_thread
	{
		/* rt object */
		char name[RT_NAME_MAX]; /**< the name of thread                     */
		rt_uint8_t type;		/**< type of object                         */
		rt_uint8_t flags;		/**< thread's flags                         */

#ifdef RT_USING_MODULE
		void *module_id; /**< id of application module               */
#endif

		rt_list_t list;	 /**< the object list                        */
		rt_list_t tlist; /**< the thread list                        */

		/* stack point and entry */
		void *sp;				/**< stack point                            */
		void *entry;			/**< entry                                  */
		void *parameter;		/**< parameter                              */
		void *stack_addr;		/**< stack address                          */
		rt_uint16_t stack_size; /**< stack size                             */

		/* error code */
		rt_err_t error; /**< error code                             */

		rt_uint8_t stat; /**< thread stat                            */

		/* priority */
		rt_uint8_t current_priority; /**< current priority                       */
		rt_uint8_t init_priority;	 /**< initialized priority                   */
#if RT_THREAD_PRIORITY_MAX > 32
		rt_uint8_t number;
		rt_uint8_t high_mask;
#endif
		rt_uint32_t number_mask;

#if defined(RT_USING_EVENT)
		/* thread event */
		rt_uint32_t event_set;
		rt_uint8_t event_info;
#endif

		rt_ubase_t init_tick;	   /**< thread's initialized tick              */
		rt_ubase_t remaining_tick; /**< remaining tick                         */

		struct rt_timer thread_timer; /**< thread timer                           */

		void (*cleanup)(struct rt_thread *tid); /**< cleanup function when thread exit      */

		rt_uint32_t user_data; /**< user data                              */
	};
/*@}*/

/**
 * @addtogroup IPC
 */
/*@{*/

/**
 * IPC flags and control command defitions
 */
#define RT_IPC_FLAG_FIFO 0x00 /**< FIFOed IPC. @ref IPC.                  */
#define RT_IPC_FLAG_PRIO 0x01 /**< PRIOed IPC. @ref IPC.                  */

#define RT_IPC_CMD_UNKNOWN 0x00 /**< unknown IPC command                    */
#define RT_IPC_CMD_RESET 0x01	/**< reset IPC object                       */

#define RT_WAITING_FOREVER -1 /**< Block forever until get resource.      */
#define RT_WAITING_NO 0		  /**< Non-block.                             */

	/**
	 * Base structure of IPC object
	 */
	struct rt_ipc_object
	{
		struct rt_object parent; /**< inherit from rt_object                 */

		rt_list_t suspend_thread; /**< threads pended on this resource        */
	};

#ifdef RT_USING_SEMAPHORE
	/**
	 * Semaphore structure
	 */
	struct rt_semaphore
	{
		struct rt_ipc_object parent; /**< inherit from ipc_object                */

		rt_uint16_t value; /**< value of semaphore.                    */
	};
	typedef struct rt_semaphore *rt_sem_t;
#endif

#ifdef RT_USING_MUTEX
	/**
	 * Mutual exclusion (mutex) structure
	 */
	struct rt_mutex
	{
		struct rt_ipc_object parent; /**< inherit from ipc_object                */

		rt_uint16_t value; /**< value of mutex                         */

		rt_uint8_t original_priority; /**< priority of last thread hold the mutex */
		rt_uint8_t hold;			  /**< numbers of thread hold the mutex       */

		struct rt_thread *owner; /**< current owner of mutex                 */
	};
	typedef struct rt_mutex *rt_mutex_t;
#endif

#ifdef RT_USING_EVENT
/**
 * flag defintions in event
 */
#define RT_EVENT_FLAG_AND 0x01	 /**< logic and                              */
#define RT_EVENT_FLAG_OR 0x02	 /**< logic or                               */
#define RT_EVENT_FLAG_CLEAR 0x04 /**< clear flag                             */

	/*
	 * event structure
	 */
	struct rt_event
	{
		struct rt_ipc_object parent; /**< inherit from ipc_object                */

		rt_uint32_t set; /**< event set                              */
	};
	typedef struct rt_event *rt_event_t;
#endif

#ifdef RT_USING_MAILBOX
	/**
	 * mailbox structure
	 *
	 */
	struct rt_mailbox
	{
		struct rt_ipc_object parent; /**< inherit from ipc_object                */

		rt_uint32_t *msg_pool; /**< start address of message buffer        */

		rt_uint16_t size; /**< size of message pool                   */

		rt_uint16_t entry;				   /**< index of messages in msg_pool          */
		rt_uint16_t in_offset, out_offset; /**< in/output offset of the message buffer */

		rt_list_t suspend_sender_thread; /**< sender thread suspended on this mb     */
	};
	typedef struct rt_mailbox *rt_mailbox_t;
#endif

#ifdef RT_USING_MESSAGEQUEUE
	/**
	 * message queue structure
	 */
	struct rt_messagequeue
	{
		struct rt_ipc_object parent; /**< inherit from ipc_object                */

		void *msg_pool; /**< start address of message queue         */

		rt_uint16_t msg_size; /**< message size of each message           */
		rt_uint16_t max_msgs; /**< max number of messages                 */

		rt_uint16_t entry; /**< index of messages in the queue         */

		void *msg_queue_head; /**< list head                              */
		void *msg_queue_tail; /**< list tail                              */
		void *msg_queue_free; /**< pointer indicated the free node of queue */
	};
	typedef struct rt_messagequeue *rt_mq_t;
#endif
/*@}*/

/**
 * @addtogroup MM
 */
/*@{*/
/*
 * memory management
 * heap & partition
 */
#ifdef RT_USING_MEMPOOL
	/**
	 * Base structure of Memory pool object
	 */
	struct rt_mempool
	{
		struct rt_object parent; /**< inherit from rt_object                     */

		void *start_address; /**< memory pool start                          */
		rt_size_t size;		 /**< size of memory pool                        */

		rt_size_t block_size;	/**< size of memory blocks                      */
		rt_uint8_t *block_list; /**< memory blocks list                         */

		rt_size_t block_total_count; /**< numbers of memory block                    */
		rt_size_t block_free_count;	 /**< numbers of free memory block               */

		rt_list_t suspend_thread;		/**< threads pended on this resource            */
		rt_size_t suspend_thread_count; /**< numbers of thread pended on this resource  */
	};
	typedef struct rt_mempool *rt_mp_t;
#endif
	/*@}*/

#ifdef RT_USING_DEVICE_
	/**
	 * @addtogroup Device
	 */
	/*@{*/

	/**
	 * device (I/O) class type
	 */
	enum rt_device_class_type
	{
		RT_Device_Class_Char = 0,  /**< character device                           */
		RT_Device_Class_Block,	   /**< block device                               */
		RT_Device_Class_NetIf,	   /**< net interface                              */
		RT_Device_Class_MTD,	   /**< memory device                              */
		RT_Device_Class_CAN,	   /**< CAN device                                 */
		RT_Device_Class_RTC,	   /**< RTC device                                 */
		RT_Device_Class_Sound,	   /**< Sound device                               */
		RT_Device_Class_Graphic,   /**< Graphic device                             */
		RT_Device_Class_I2C,	   /**< I2C device                                 */
		RT_Device_Class_USBDevice, /**< USB slave device                           */
		RT_Device_Class_USBHost,   /**< USB host bus                               */
		RT_Device_Class_SPIBUS,	   /**< SPI bus device                             */
		RT_Device_Class_SPIDevice, /**< SPI device                                 */
		RT_Device_Class_SDIO,	   /**< SDIO bus device                            */
		RT_Device_Class_Unknown	   /**< unknown device                             */
	};

/**
 * device flags defitions
 */
#define RT_DEVICE_FLAG_DEACTIVATE 0x000 /**< device is not not initialized              */

#define RT_DEVICE_FLAG_RDONLY 0x001 /**< read only                                  */
#define RT_DEVICE_FLAG_WRONLY 0x002 /**< write only                                 */
#define RT_DEVICE_FLAG_RDWR 0x003	/**< read and write                             */

#define RT_DEVICE_FLAG_REMOVABLE 0x004	/**< removable device                           */
#define RT_DEVICE_FLAG_STANDALONE 0x008 /**< standalone device                          */
#define RT_DEVICE_FLAG_ACTIVATED 0x010	/**< device is activated                        */
#define RT_DEVICE_FLAG_SUSPENDED 0x020	/**< device is suspended                        */
#define RT_DEVICE_FLAG_STREAM 0x040		/**< stream mode                                */

#define RT_DEVICE_FLAG_INT_RX 0x100 /**< INT mode on Rx                             */
#define RT_DEVICE_FLAG_DMA_RX 0x200 /**< DMA mode on Rx                             */
#define RT_DEVICE_FLAG_INT_TX 0x400 /**< INT mode on Tx                             */
#define RT_DEVICE_FLAG_DMA_TX 0x800 /**< DMA mode on Tx                             */

#define RT_DEVICE_OFLAG_CLOSE 0x000	 /**< device is closed                           */
#define RT_DEVICE_OFLAG_RDONLY 0x001 /**< read only access                           */
#define RT_DEVICE_OFLAG_WRONLY 0x002 /**< write only access                          */
#define RT_DEVICE_OFLAG_RDWR 0x003	 /**< read and write                             */
#define RT_DEVICE_OFLAG_OPEN 0x008	 /**< device is opened                           */

/**
 * general device commands
 */
#define RT_DEVICE_CTRL_RESUME 0x01	/**< resume device                              */
#define RT_DEVICE_CTRL_SUSPEND 0x02 /**< suspend device                             */

/**
 * special device commands
 */
#define RT_DEVICE_CTRL_CHAR_STREAM 0x10	 /**< stream mode on char device                 */
#define RT_DEVICE_CTRL_BLK_GETGEOME 0x10 /**< get geometry information                   */
#define RT_DEVICE_CTRL_NETIF_GETMAC 0x10 /**< get mac address                            */
#define RT_DEVICE_CTRL_MTD_FORMAT 0x10	 /**< format a MTD device                        */
#define RT_DEVICE_CTRL_RTC_GET_TIME 0x10 /**< get time                                   */
#define RT_DEVICE_CTRL_RTC_SET_TIME 0x11 /**< set time                                   */

	typedef struct rt_device *rt_device_t;

	/**
	 * Device structure
	 */
	struct rt_device
	{
		struct rt_object parent; /**< inherit from rt_object                     */

		enum rt_device_class_type type; /**< device type                                */
		rt_uint16_t flag, open_flag;	/**< device flag and device open flag           */

		/* device call back */
		rt_err_t (*rx_indicate)(rt_device_t dev, rt_size_t size);
		rt_err_t (*tx_complete)(rt_device_t dev, void *buffer);

		/* common device interface */
		rt_err_t (*init)(rt_device_t dev);
		rt_err_t (*open)(rt_device_t dev, rt_uint16_t oflag);
		rt_err_t (*close)(rt_device_t dev);
		rt_size_t (*read)(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
		rt_size_t (*write)(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size);
		rt_err_t (*control)(rt_device_t dev, rt_uint8_t cmd, void *args);

#ifdef RT_USING_DEVICE_SUSPEND
		rt_err_t (*suspend)(rt_device_t dev);
		rt_err_t (*resumed)(rt_device_t dev);
#endif

		void *user_data; /**< device private data                        */
	};

	/**
	 * block device geometry structure
	 */
	struct rt_device_blk_geometry
	{
		rt_uint32_t sector_count;	  /**< count of sectors                           */
		rt_uint32_t bytes_per_sector; /**< number of bytes per sector                 */
		rt_uint32_t block_size;		  /**< size to erase one block                    */
	};

/**
 * graphic device control command
 */
#define RTGRAPHIC_CTRL_RECT_UPDATE 0
#define RTGRAPHIC_CTRL_POWERON 1
#define RTGRAPHIC_CTRL_POWEROFF 2
#define RTGRAPHIC_CTRL_GET_INFO 3
#define RTGRAPHIC_CTRL_SET_MODE 4

	/* graphic deice */
	enum
	{
		RTGRAPHIC_PIXEL_FORMAT_MONO = 0,
		RTGRAPHIC_PIXEL_FORMAT_GRAY4,
		RTGRAPHIC_PIXEL_FORMAT_GRAY16,
		RTGRAPHIC_PIXEL_FORMAT_RGB332,
		RTGRAPHIC_PIXEL_FORMAT_RGB444,
		RTGRAPHIC_PIXEL_FORMAT_RGB565,
		RTGRAPHIC_PIXEL_FORMAT_RGB565P,
		RTGRAPHIC_PIXEL_FORMAT_RGB666,
		RTGRAPHIC_PIXEL_FORMAT_RGB888,
		RTGRAPHIC_PIXEL_FORMAT_ARGB888
	};
/**
 * build a pixel position according to (x, y) coordinates.
 */
#define RTGRAPHIC_PIXEL_POSITION(x, y) ((x << 16) | y)

	/**
	 * graphic device information structure
	 */
	struct rt_device_graphic_info
	{
		rt_uint8_t pixel_format;   /**< graphic format           */
		rt_uint8_t bits_per_pixel; /**< bits per pixel           */
		rt_uint16_t reserved;	   /**< reserved field           */

		rt_uint16_t width;	/**< width of graphic device  */
		rt_uint16_t height; /**< height of graphic device */

		rt_uint8_t *framebuffer; /**< frame buffer             */
	};

	/**
	 * rectangle information structure
	 */
	struct rt_device_rect_info
	{
		rt_uint16_t x, y;		   /**< x, y coordinate          */
		rt_uint16_t width, height; /**< width and height         */
	};

	/**
	 * graphic operations
	 */
	struct rt_device_graphic_ops
	{
		void (*set_pixel)(const char *pixel, int x, int y);
		void (*get_pixel)(char *pixel, int x, int y);

		void (*draw_hline)(const char *pixel, int x1, int x2, int y);
		void (*draw_vline)(const char *pixel, int x, int y1, int y2);

		void (*blit_line)(const char *pixel, int x, int y, rt_size_t size);
	};
#define rt_graphix_ops(device) ((struct rt_device_graphic_ops *)(device->user_data))
/*@}*/

/**
 * @addtogroup Module
 */
/*@{*/
#ifdef RT_USING_MODULE
	/*
	 * module system
	 */

#define RT_MODULE_FLAG_WITHENTRY 0x00	 /* with entry point                             */
#define RT_MODULE_FLAG_WITHOUTENTRY 0x01 /* without entry point                          */

	/**
	 * Application Module structure
	 */
	struct rt_module
	{
		struct rt_object parent; /**< inherit from object                        */

		rt_uint8_t *module_space; /**< module memory space                        */

		void *module_entry;		   /**< entry address of module's thread           */
		rt_thread_t module_thread; /**< stack size of module's thread              */
		rt_uint32_t stack_size;	   /**< priority of module's thread                */
		rt_uint32_t thread_priority;

		/* module memory allocator */
		void *mem_list;		  /**< module's free memory list                  */
		void *page_array;	  /**< module's using pages                       */
		rt_uint32_t page_cnt; /**< module's using pages count                 */

		rt_uint32_t nsym;				 /**< number of symbol in the module             */
		struct rt_module_symtab *symtab; /**< module symbol table                        */

		rt_uint32_t nref; /**< reference count                            */

		/**< object in this module, module object is the last basic object type */
		struct rt_object_information module_object[RT_Object_Class_Unknown];
	};
	typedef struct rt_module *rt_module_t;
#endif

/*@}*/
#endif

#ifdef __cplusplus
}
#endif

#endif
