/*
 * OSHW实现 - STM32 + W5500实现
 */

#include "oshw.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief 主机字节序转网络字节序 (16位)
 */
uint16 oshw_htons(uint16 hostshort)
{
    /* STM32是小端序，网络是大端序 */
    return ((hostshort & 0xFF) << 8) | ((hostshort >> 8) & 0xFF);
}

/**
 * @brief 网络字节序转主机字节序 (16位)
 */
uint16 oshw_ntohs(uint16 networkshort)
{
    return ((networkshort & 0xFF) << 8) | ((networkshort >> 8) & 0xFF);
}

/**
 * @brief 查找网络适配器
 */
ec_adaptert *oshw_find_adapters(void)
{
    static ec_adaptert adapter;
    
    memset(&adapter, 0, sizeof(adapter));
    strcpy(adapter.name, "w5500");
    strcpy(adapter.desc, "W5500 Ethernet Controller");
    adapter.next = NULL;
    
    return &adapter;
}

/**
 * @brief 释放适配器列表
 */
void oshw_free_adapters(ec_adaptert *adapter)
{
    /* 静态分配，不需要释放 */
    (void)adapter;
}
