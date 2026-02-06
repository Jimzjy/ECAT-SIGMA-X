/**
 * @file ecat_master.h
 * @brief 简化版EtherCAT主站头文件
 * 针对STM32F103C8T6 + W5500优化
 * 专注于SDO读取功能
 */

#ifndef __ECAT_MASTER_H__
#define __ECAT_MASTER_H__

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

/* EtherCAT帧类型 */
#define ECAT_ETHERTYPE          0x88A4

/* EtherCAT命令类型 */
#define EC_CMD_NOP              0x00
#define EC_CMD_APRD             0x01  /* Auto Increment Read */
#define EC_CMD_APWR             0x02  /* Auto Increment Write */
#define EC_CMD_APRW             0x03  /* Auto Increment Read Write */
#define EC_CMD_FPRD             0x04  /* Configured Address Read */
#define EC_CMD_FPWR             0x05  /* Configured Address Write */
#define EC_CMD_FPRW             0x06  /* Configured Address Read Write */
#define EC_CMD_BRD              0x07  /* Broadcast Read */
#define EC_CMD_BWR              0x08  /* Broadcast Write */
#define EC_CMD_BRW              0x09  /* Broadcast Read Write */
#define EC_CMD_LRD              0x0A  /* Logical Memory Read */
#define EC_CMD_LWR              0x0B  /* Logical Memory Write */
#define EC_CMD_LRW              0x0C  /* Logical Memory Read Write */
#define EC_CMD_ARMW             0x0D  /* Auto Increment Read Multiple Write */
#define EC_CMD_FRMW             0x0E  /* Configured Read Multiple Write */

/* EtherCAT寄存器地址 */
#define ECT_REG_TYPE            0x0000
#define ECT_REG_PORTDES         0x0007
#define ECT_REG_ESCSUP          0x0008
#define ECT_REG_STAESSION       0x0010
#define ECT_REG_DLCTL           0x0100
#define ECT_REG_DLPORT          0x0101
#define ECT_REG_DLALIAS         0x0012
#define ECT_REG_DLSTAT          0x0110
#define ECT_REG_ALCTL           0x0120
#define ECT_REG_ALSTAT          0x0130
#define ECT_REG_ALSTACODE       0x0134
#define ECT_REG_PDICTL          0x0140
#define ECT_REG_IRQMASK         0x0200
#define ECT_REG_RXERR           0x0300
#define ECT_REG_FRXL            0x0310
#define ECT_REG_EEPCFG          0x0500
#define ECT_REG_EEPCTL          0x0502
#define ECT_REG_EEPSTAT         0x0502
#define ECT_REG_EEPADR          0x0504
#define ECT_REG_EEPDAT          0x0508
#define ECT_REG_FMMU0           0x0600
#define ECT_REG_SM0             0x0800
#define ECT_REG_SM1             0x0808
#define ECT_REG_SM2             0x0810
#define ECT_REG_SM3             0x0818
#define ECT_REG_DCTIME0         0x0900
#define ECT_REG_DCTIME1         0x0904
#define ECT_REG_DCTIME2         0x0908
#define ECT_REG_DCTIME3         0x090C
#define ECT_REG_DCSYSTIME       0x0910

/* EtherCAT状态 */
#define EC_STATE_NONE           0x00
#define EC_STATE_INIT           0x01
#define EC_STATE_PREOP          0x02
#define EC_STATE_BOOT           0x03
#define EC_STATE_SAFEOP         0x04
#define EC_STATE_OPERATIONAL    0x08
#define EC_STATE_ERROR          0x10

/* CoE邮箱类型 */
#define EC_MAILBOX_COE          0x03

/* CoE命令 */
#define COE_SDOREQ              0x02
#define COE_SDORES              0x03
#define COE_SDOINFO             0x08

/* SDO命令 */
#define SDO_INIT_UPLOAD_REQ     0x40
#define SDO_INIT_DOWNLOAD_EXP   0x23  /* 加速下载请求(≤4字节) */
#define SDO_INIT_DOWNLOAD_NOR   0x21  /* 正常下载请求(>4字节) */
#define SDO_ABORT               0x80

/* 返回值 */
#define ECAT_OK                 0
#define ECAT_ERROR              -1
#define ECAT_TIMEOUT            -2
#define ECAT_NO_SLAVE           -3
#define ECAT_SDO_ERROR          -4
#define ECAT_SM0_ERR            -5
#define ECAT_SM1_ERR            -6
#define ECAT_STATE_ERR          -7
#define ECAT_MBX_WRITE_ERR      -8
#define ECAT_MBX_READ_ERR       -9

/* 最大帧大小 */
#define ECAT_MAX_FRAME          1518

/* 从站信息结构 */
typedef struct {
    uint16_t configAddr;        /* 配置地址 */
    uint16_t aliasAddr;         /* 别名地址 */
    int16_t  adp;               /* 自增量地址 (负数: -1, -2, ...) */
    uint8_t  state;             /* 当前状态 */
    uint16_t alStatusCode;      /* AL状态码 */
    uint16_t mbxRxAddr;         /* 接收邮箱地址 */
    uint16_t mbxRxSize;         /* 接收邮箱大小 */
    uint16_t mbxTxAddr;         /* 发送邮箱地址 */
    uint16_t mbxTxSize;         /* 发送邮箱大小 */
    uint8_t  mbxProtocol;       /* 支持的邮箱协议 */
} ECAT_Slave_t;

/* EtherCAT帧头 (以太网头) */
typedef struct __attribute__((packed)) {
    uint8_t  destMac[6];
    uint8_t  srcMac[6];
    uint16_t etherType;
} ECAT_EthHeader_t;

/* EtherCAT协议头 (紧跟以太网头) */
typedef struct __attribute__((packed)) {
    uint16_t length;            /* 11位长度 + 4位保留 + 1位类型 */
} ECAT_Header_t;

/* EtherCAT数据报头 (10字节) */
typedef struct __attribute__((packed)) {
    uint8_t  cmd;               /* 命令 */
    uint8_t  idx;               /* 索引 */
    uint16_t adp;               /* 地址指针 (从站地址) */
    uint16_t ado;               /* 地址偏移 (寄存器地址) */
    uint16_t dlc;               /* 11位数据长度 + R + C + M + 后续标志 */
    uint16_t irq;               /* 中断 */
} ECAT_DatagramHeader_t;

/* 邮箱头 (6字节) */
typedef struct __attribute__((packed)) {
    uint16_t length;            /* 邮箱数据长度 (不包括邮箱头) */
    uint16_t address;           /* 站地址 */
    uint8_t  channel_priority;  /* Channel:6 + Priority:2 */
    uint8_t  type_cnt;          /* Type:4 + Cnt:4 */
} ECAT_MbxHeader_t;

/* CoE头 (2字节 + SDO数据) */
typedef struct __attribute__((packed)) {
    uint16_t header;            /* CoE服务类型 */
} ECAT_CoEHeader_t;

/* SDO请求头 */
typedef struct __attribute__((packed)) {
    uint16_t coe_header;        /* CoE头 */
    uint8_t  command;           /* SDO命令 */
    uint16_t index;             /* 对象索引 */
    uint8_t  subIndex;          /* 子索引 */
    uint32_t size;              /* 数据大小/数据 */
} ECAT_SDOHeader_t;

/* 函数声明 */
int ECAT_Init(void);
int ECAT_ScanSlaves(void);
int ECAT_GetSlaveCount(void);
ECAT_Slave_t* ECAT_GetSlave(int index);
int ECAT_SetState(uint16_t slave, uint8_t state);
int ECAT_SDORead(uint16_t slave, uint16_t index, uint8_t subIndex, 
                 void *data, int *dataSize, int timeout);
int ECAT_SDOWrite(uint16_t slave, uint16_t index, uint8_t subIndex, 
                 void *data, int dataSize, int timeout);

#endif /* __ECAT_MASTER_H__ */
