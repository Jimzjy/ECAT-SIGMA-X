/*
 * OSAL头文件 - STM32裸机适配版本
 * 基于SOEM-2.0.0/osal/osal.h
 */

#ifndef _osal_
#define _osal_

#ifdef __cplusplus
extern "C" {
#endif

#include "osal_defs.h"
#include <stdint.h>
#include <stddef.h>

/* General types */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
typedef uint8_t boolean;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef float float32;
typedef double float64;

typedef struct osal_timer
{
   ec_timet stop_time;
} osal_timert;

/**
 * @brief 初始化OSAL
 */
void osal_init(void);

/**
 * @brief 获取单调递增时间
 */
void osal_get_monotonic_time(ec_timet *ts);

/**
 * @brief 获取当前时间
 */
ec_timet osal_current_time(void);

/**
 * @brief 计算时间差
 */
void osal_time_diff(ec_timet *start, ec_timet *end, ec_timet *diff);

/**
 * @brief 启动定时器
 */
void osal_timer_start(osal_timert *self, uint32 timeout_usec);

/**
 * @brief 检查定时器是否超时
 */
boolean osal_timer_is_expired(osal_timert *self);

/**
 * @brief 微秒延时
 */
int osal_usleep(uint32 usec);

/**
 * @brief 睡眠到指定单调时间
 */
int osal_monotonic_sleep(ec_timet *ts);

/**
 * @brief 内存分配
 */
void *osal_malloc(size_t size);

/**
 * @brief 内存释放
 */
void osal_free(void *ptr);

/**
 * @brief 创建线程 (STM32裸机不支持)
 */
int osal_thread_create(void *thandle, int stacksize, void *func, void *param);

/**
 * @brief 创建实时线程 (STM32裸机不支持)
 */
int osal_thread_create_rt(void *thandle, int stacksize, void *func, void *param);

/**
 * @brief 创建互斥锁
 */
osal_mutext *osal_mutex_create(void);

/**
 * @brief 销毁互斥锁
 */
void osal_mutex_destroy(osal_mutext *mutex);

/**
 * @brief 锁定互斥锁
 */
int osal_mutex_lock(osal_mutext *mutex);

/**
 * @brief 解锁互斥锁
 */
int osal_mutex_unlock(osal_mutext *mutex);

/* 时间转换宏 */
#ifndef osal_timespec_from_usec
#define osal_timespec_from_usec(usec, result)      \
   do                                              \
   {                                               \
      (result)->tv_sec = usec / 1000000;           \
      (result)->tv_nsec = (usec % 1000000) * 1000; \
   } while (0)
#endif

#ifndef osal_timespeccmp
#define osal_timespeccmp(a, b, CMP)      \
   (((a)->tv_sec == (b)->tv_sec)         \
        ? ((a)->tv_nsec CMP(b)->tv_nsec) \
        : ((a)->tv_sec CMP(b)->tv_sec))
#endif

#ifndef osal_timespecadd
#define osal_timespecadd(a, b, result)                 \
   do                                                  \
   {                                                   \
      (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;    \
      (result)->tv_nsec = (a)->tv_nsec + (b)->tv_nsec; \
      if ((result)->tv_nsec >= 1000000000)             \
      {                                                \
         ++(result)->tv_sec;                           \
         (result)->tv_nsec -= 1000000000;              \
      }                                                \
   } while (0)
#endif

#ifndef osal_timespecsub
#define osal_timespecsub(a, b, result)                 \
   do                                                  \
   {                                                   \
      (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;    \
      (result)->tv_nsec = (a)->tv_nsec - (b)->tv_nsec; \
      if ((result)->tv_nsec < 0)                       \
      {                                                \
         --(result)->tv_sec;                           \
         (result)->tv_nsec += 1000000000;              \
      }                                                \
   } while (0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _osal_ */
