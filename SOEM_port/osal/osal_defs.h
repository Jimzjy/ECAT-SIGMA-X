/*
 * OSAL定义头文件 - STM32裸机实现
 */

#ifndef _osal_defs_
#define _osal_defs_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* 时间结构体 */
typedef struct
{
   int32_t tv_sec;    /* 秒 */
   int32_t tv_nsec;   /* 纳秒 */
} ec_timet;

/* 互斥量类型 (STM32裸机简化版，不实际使用) */
typedef int osal_mutext;

/* 线程句柄 (STM32裸机不支持) */
#define OSAL_THREAD_HANDLE void*
#define OSAL_THREAD_FUNC void
#define OSAL_THREAD_FUNC_RT void

/* 打包属性 */
#define OSAL_PACKED_BEGIN
#define OSAL_PACKED_END
#define OSAL_PACKED __attribute__((packed))

/* 时间相加宏 */
#define osal_timespecadd(a, b, result) \
   do { \
      (result)->tv_sec = (a)->tv_sec + (b)->tv_sec; \
      (result)->tv_nsec = (a)->tv_nsec + (b)->tv_nsec; \
      if ((result)->tv_nsec >= 1000000000L) { \
         ++(result)->tv_sec; \
         (result)->tv_nsec -= 1000000000L; \
      } \
   } while (0)

#ifdef __cplusplus
}
#endif

#endif /* _osal_defs_ */
