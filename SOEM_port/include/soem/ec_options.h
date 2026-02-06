/*
 * SOEM配置文件 - STM32F103C8T6优化版
 * 针对有限的RAM和Flash进行裁剪
 */

#ifndef _ec_options_
#define _ec_options_

#ifdef __cplusplus
extern "C" {
#endif

/* 标准帧缓冲区大小 (字节) */
#define EC_BUFSIZE 1536

/* 每通道帧缓冲区数量 */
#define EC_MAXBUF 4

/* EEPROM位图缓存大小 */
#define EC_MAXEEPBITMAP 16

/* EEPROM缓存缓冲区大小 */
#define EC_MAXEEPBUF 128

/* 默认组大小 (2^x) */
#define EC_LOGGROUPOFFSET 12

/* EtherCAT错误列表最大条目 */
#define EC_MAXELIST 4

/* 从站列表和对象描述列表中可读名称的最大长度 */
#define EC_MAXNAME 32

/* 最大从站数量 */
#define EC_MAXSLAVE 4

/* 最大组数量 */
#define EC_MAXGROUP 2

/* 每组最大IO段数 */
#define EC_MAXIOSEGMENTS 1

/* 最大邮箱大小 */
#define EC_MAXMBX 128

/* 邮箱池大小 */
#define EC_MBXPOOLSIZE 2

/* 最大EEPROM PDO条目 */
#define EC_MAXEEPDO 8

/* 最大使用的SM */
#define EC_MAXSM 4

/* 最大使用的FMMU */
#define EC_MAXFMMU 4

/* 最大适配器名称长度 */
#define EC_MAXLEN_ADAPTERNAME 16

/* 映射中最大并发线程数 */
#define EC_MAX_MAPT 1

/* 对象描述列表最大条目 */
#define EC_MAXODLIST 16

/* 对象条目列表最大条目 */
#define EC_MAXOELIST 16

/* SoE名称最大长度 */
#define EC_SOE_MAXNAME 32

/* SoE映射最大数量 */
#define EC_SOE_MAXMAPPING 8

/* 超时和重试 */

/* tx帧返回rx的超时值 (微秒) */
#define EC_TIMEOUTRET 2000

/* 安全数据传输超时值 (微秒)，最多三次重试 */
#define EC_TIMEOUTRET3 6000

/* "安全"变体返回超时值 (微秒) */
#define EC_TIMEOUTSAFE 20000

/* EEPROM访问超时值 (微秒) */
#define EC_TIMEOUTEEP 20000

/* tx邮箱周期超时值 (微秒) */
#define EC_TIMEOUTTXM 20000

/* rx邮箱周期超时值 (微秒) */
#define EC_TIMEOUTRXM 700000

/* 状态变更检查超时值 (微秒) */
#define EC_TIMEOUTSTATE 2000000

/* wkc <= 0时的默认重试次数 */
#define EC_DEFAULTRETRIES 3

/* MAC地址 */
#define EC_PRIMARY_MAC_ARRAY {0x10, 0x10, 0x10, 0x10, 0x10, 0x10}
#define EC_SECONDARY_MAC_ARRAY {0x10, 0x10, 0x10, 0x10, 0x10, 0x11}

#ifdef __cplusplus
}
#endif

#endif /* _ec_options_ */
