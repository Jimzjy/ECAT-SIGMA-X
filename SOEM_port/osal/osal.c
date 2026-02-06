/*
 * OSAL实现 - STM32裸机版本
 */

#include "osal.h"
#include "main.h"
#include <stdlib.h>
#include <string.h>

/* 系统启动时间 (用于单调时间) */
static uint32_t osal_start_tick = 0;

/**
 * @brief 获取单调递增时间
 */
void osal_get_monotonic_time(ec_timet *ts)
{
    uint32_t tick = HAL_GetTick();
    uint32_t elapsed = tick - osal_start_tick;
    
    ts->tv_sec = elapsed / 1000;
    ts->tv_nsec = (elapsed % 1000) * 1000000;
}

/**
 * @brief 获取当前时间
 */
ec_timet osal_current_time(void)
{
    ec_timet ts;
    osal_get_monotonic_time(&ts);
    return ts;
}

/**
 * @brief 计算时间差
 */
void osal_time_diff(ec_timet *start, ec_timet *end, ec_timet *diff)
{
    diff->tv_sec = end->tv_sec - start->tv_sec;
    diff->tv_nsec = end->tv_nsec - start->tv_nsec;
    
    if (diff->tv_nsec < 0)
    {
        diff->tv_sec--;
        diff->tv_nsec += 1000000000L;
    }
}

/**
 * @brief 启动定时器
 */
void osal_timer_start(osal_timert *self, uint32 timeout_usec)
{
    ec_timet now;
    osal_get_monotonic_time(&now);
    
    self->stop_time.tv_sec = now.tv_sec + (timeout_usec / 1000000);
    self->stop_time.tv_nsec = now.tv_nsec + ((timeout_usec % 1000000) * 1000);
    
    if (self->stop_time.tv_nsec >= 1000000000L)
    {
        self->stop_time.tv_sec++;
        self->stop_time.tv_nsec -= 1000000000L;
    }
}

/**
 * @brief 检查定时器是否超时
 */
boolean osal_timer_is_expired(osal_timert *self)
{
    ec_timet now;
    osal_get_monotonic_time(&now);
    
    if (now.tv_sec > self->stop_time.tv_sec)
    {
        return TRUE;
    }
    else if (now.tv_sec == self->stop_time.tv_sec)
    {
        if (now.tv_nsec >= self->stop_time.tv_nsec)
        {
            return TRUE;
        }
    }
    
    return FALSE;
}

/**
 * @brief 微秒延时
 */
int osal_usleep(uint32 usec)
{
    if (usec >= 1000)
    {
        HAL_Delay(usec / 1000);
    }
    else
    {
        /* 对于小于1ms的延时，使用简单的循环 */
        volatile uint32_t count = usec * 9;  /* 72MHz时钟大约 */
        while (count--)
        {
            __NOP();
        }
    }
    return 0;
}

/**
 * @brief 睡眠到指定单调时间
 */
int osal_monotonic_sleep(ec_timet *ts)
{
    ec_timet now;
    
    while (1)
    {
        osal_get_monotonic_time(&now);
        
        if (now.tv_sec > ts->tv_sec)
        {
            break;
        }
        else if (now.tv_sec == ts->tv_sec && now.tv_nsec >= ts->tv_nsec)
        {
            break;
        }
        
        /* 短延时 */
        osal_usleep(100);
    }
    
    return 0;
}

/**
 * @brief 内存分配
 * 注意：STM32F103C8T6内存有限，谨慎使用动态分配
 */
void *osal_malloc(size_t size)
{
    return malloc(size);
}

/**
 * @brief 内存释放
 */
void osal_free(void *ptr)
{
    free(ptr);
}

/**
 * @brief 创建线程 (STM32裸机不支持)
 */
int osal_thread_create(void *thandle, int stacksize, void *func, void *param)
{
    (void)thandle;
    (void)stacksize;
    (void)func;
    (void)param;
    return 0;  /* 不支持 */
}

/**
 * @brief 创建实时线程 (STM32裸机不支持)
 */
int osal_thread_create_rt(void *thandle, int stacksize, void *func, void *param)
{
    (void)thandle;
    (void)stacksize;
    (void)func;
    (void)param;
    return 0;  /* 不支持 */
}

/**
 * @brief 创建互斥锁 (STM32裸机简化版)
 */
osal_mutext *osal_mutex_create(void)
{
    static osal_mutext dummy_mutex = 0;
    return &dummy_mutex;
}

/**
 * @brief 销毁互斥锁
 */
void osal_mutex_destroy(osal_mutext *mtx)
{
    (void)mtx;
}

/**
 * @brief 锁定互斥锁
 */
int osal_mutex_lock(osal_mutext *mtx)
{
    (void)mtx;
    __disable_irq();  /* 简单使用中断禁用 */
    return 0;
}

/**
 * @brief 解锁互斥锁
 */
int osal_mutex_unlock(osal_mutext *mtx)
{
    (void)mtx;
    __enable_irq();
    return 0;
}

/**
 * @brief 初始化OSAL
 */
void osal_init(void)
{
    osal_start_tick = HAL_GetTick();
}
