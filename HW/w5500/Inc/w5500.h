/**
 * @file w5500.h
 * @brief W5500以太网控制器驱动头文件
 */

#ifndef __W5500_H__
#define __W5500_H__

#include "main.h"
#include <stdint.h>

/* W5500引脚定义 */
#define W5500_SCS_PORT      GPIOB
#define W5500_SCS_PIN       GPIO_PIN_12
#define W5500_RST_PORT      GPIOA
#define W5500_RST_PIN       GPIO_PIN_12
#define W5500_INT_PORT      GPIOA
#define W5500_INT_PIN       GPIO_PIN_11

/* W5500块选择位 */
#define W5500_BSB_COMMON    (0x00 << 3)
#define W5500_BSB_SOCKET0   (0x01 << 3)
#define W5500_BSB_S0_TX     (0x02 << 3)
#define W5500_BSB_S0_RX     (0x03 << 3)

/* W5500读写模式 */
#define W5500_READ          (0x00 << 2)
#define W5500_WRITE         (0x01 << 2)

/* W5500 VDM (可变数据长度模式) */
#define W5500_VDM           0x00

/* 通用寄存器地址 */
#define W5500_MR            0x0000  /* 模式寄存器 */
#define W5500_GAR           0x0001  /* 网关地址 */
#define W5500_SUBR          0x0005  /* 子网掩码 */
#define W5500_SHAR          0x0009  /* 源MAC地址 */
#define W5500_SIPR          0x000F  /* 源IP地址 */
#define W5500_INTLEVEL      0x0013  /* 中断低水平定时器 */
#define W5500_IR            0x0015  /* 中断寄存器 */
#define W5500_IMR           0x0016  /* 中断掩码寄存器 */
#define W5500_SIR           0x0017  /* Socket中断寄存器 */
#define W5500_SIMR          0x0018  /* Socket中断掩码寄存器 */
#define W5500_RTR           0x0019  /* 重试时间 */
#define W5500_RCR           0x001B  /* 重试计数 */
#define W5500_PHYCFGR       0x002E  /* PHY配置寄存器 */
#define W5500_VERSIONR      0x0039  /* 版本寄存器 */

/* Socket寄存器偏移 */
#define W5500_Sn_MR         0x0000  /* Socket模式寄存器 */
#define W5500_Sn_CR         0x0001  /* Socket命令寄存器 */
#define W5500_Sn_IR         0x0002  /* Socket中断寄存器 */
#define W5500_Sn_SR         0x0003  /* Socket状态寄存器 */
#define W5500_Sn_PORT       0x0004  /* 源端口 */
#define W5500_Sn_DHAR       0x0006  /* 目的MAC地址 */
#define W5500_Sn_DIPR       0x000C  /* 目的IP地址 */
#define W5500_Sn_DPORT      0x0010  /* 目的端口 */
#define W5500_Sn_MSSR       0x0012  /* 最大分段大小 */
#define W5500_Sn_TOS        0x0015  /* TOS */
#define W5500_Sn_TTL        0x0016  /* TTL */
#define W5500_Sn_RXBUF_SIZE 0x001E  /* 接收缓冲区大小 */
#define W5500_Sn_TXBUF_SIZE 0x001F  /* 发送缓冲区大小 */
#define W5500_Sn_TX_FSR     0x0020  /* TX空闲大小 */
#define W5500_Sn_TX_RD      0x0022  /* TX读指针 */
#define W5500_Sn_TX_WR      0x0024  /* TX写指针 */
#define W5500_Sn_RX_RSR     0x0026  /* RX接收大小 */
#define W5500_Sn_RX_RD      0x0028  /* RX读指针 */
#define W5500_Sn_RX_WR      0x002A  /* RX写指针 */
#define W5500_Sn_IMR        0x002C  /* Socket中断掩码 */
#define W5500_Sn_FRAG       0x002D  /* 分片偏移 */
#define W5500_Sn_KPALVTR    0x002F  /* 保活时间 */

/* Socket模式 */
#define W5500_Sn_MR_CLOSE   0x00
#define W5500_Sn_MR_TCP     0x01
#define W5500_Sn_MR_UDP     0x02
#define W5500_Sn_MR_MACRAW  0x04  /* 原始以太网帧模式 */
#define W5500_Sn_MR_MF      0x40  /* MAC过滤使能 (MACRAW下为多播) */
#define W5500_Sn_MR_MFEN    0x80  /* MAC过滤使能 */

/* Socket命令 */
#define W5500_Sn_CR_OPEN    0x01
#define W5500_Sn_CR_CLOSE   0x10
#define W5500_Sn_CR_SEND    0x20
#define W5500_Sn_CR_RECV    0x40

/* Socket状态 */
#define W5500_Sn_SR_SOCK_CLOSED     0x00
#define W5500_Sn_SR_SOCK_INIT       0x13
#define W5500_Sn_SR_SOCK_MACRAW     0x42

/* Socket中断 */
#define W5500_Sn_IR_SEND_OK 0x10
#define W5500_Sn_IR_RECV    0x04

/* 缓冲区大小 */
#define W5500_TX_BUF_SIZE   2048
#define W5500_RX_BUF_SIZE   2048

/* 最大帧大小 */
#define W5500_MAX_FRAME_SIZE 1518

/* 函数声明 */
void W5500_Init(void);
void W5500_Reset(void);
uint8_t W5500_ReadReg(uint16_t addr, uint8_t bsb);
void W5500_WriteReg(uint16_t addr, uint8_t bsb, uint8_t data);
void W5500_ReadBuf(uint16_t addr, uint8_t bsb, uint8_t *buf, uint16_t len);
void W5500_WriteBuf(uint16_t addr, uint8_t bsb, uint8_t *buf, uint16_t len);

void W5500_SetMAC(uint8_t *mac);
void W5500_GetMAC(uint8_t *mac);

int W5500_OpenMACRAW(void);
void W5500_CloseMACRAW(void);

int W5500_SendFrame(uint8_t *frame, uint16_t len);
int W5500_RecvFrame(uint8_t *frame, uint16_t maxlen);
int W5500_GetRxSize(void);

uint8_t W5500_GetLinkStatus(void);
uint8_t W5500_GetVersion(void);

#endif /* __W5500_H__ */
