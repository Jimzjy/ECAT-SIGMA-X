/*
 * NIC驱动头文件 - STM32 + W5500实现
 */

#ifndef _nicdrvh_
#define _nicdrvh_

#ifdef __cplusplus
extern "C" {
#endif

#include "soem/ec_type.h"

/** 指针结构体用于Tx和Rx栈 */
typedef struct
{
   /** 发送缓冲区 */
   ec_bufT (*txbuf)[EC_MAXBUF];
   /** 发送缓冲区长度 */
   int (*txbuflength)[EC_MAXBUF];
   /** 临时接收缓冲区 */
   ec_bufT *tempbuf;
   /** 接收缓冲区 */
   ec_bufT (*rxbuf)[EC_MAXBUF];
   /** 接收缓冲区状态 */
   int (*rxbufstat)[EC_MAXBUF];
   /** 接收MAC源地址 */
   int (*rxsa)[EC_MAXBUF];
   /** 接收帧计数 */
   uint64 rxcnt;
} ec_stackT;

/** 冗余端口缓冲区结构 */
typedef struct
{
   ec_stackT stack;
   /** 接收缓冲区 */
   ec_bufT rxbuf[EC_MAXBUF];
   /** 接收缓冲区状态 */
   int rxbufstat[EC_MAXBUF];
   /** 接收MAC源地址 */
   int rxsa[EC_MAXBUF];
   /** 临时接收缓冲区 */
   ec_bufT tempinbuf;
} ecx_redportt;

/** 端口实例结构 */
typedef struct
{
   ec_stackT stack;
   /** 接收缓冲区 */
   ec_bufT rxbuf[EC_MAXBUF];
   /** 接收缓冲区状态 */
   int rxbufstat[EC_MAXBUF];
   /** 接收MAC源地址 */
   int rxsa[EC_MAXBUF];
   /** 临时接收缓冲区 */
   ec_bufT tempinbuf;
   /** 临时接收缓冲区状态 */
   int tempinbufs;
   /** 发送缓冲区 */
   ec_bufT txbuf[EC_MAXBUF];
   /** 发送缓冲区长度 */
   int txbuflength[EC_MAXBUF];
   /** 临时发送缓冲区 */
   ec_bufT txbuf2;
   /** 临时发送缓冲区长度 */
   int txbuflength2;
   /** 最后使用的帧索引 */
   uint8 lastidx;
   /** 当前冗余状态 */
   int redstate;
   /** 冗余端口指针 */
   ecx_redportt *redport;
   osal_mutext *getindex_mutex;
   osal_mutext *tx_mutex;
   osal_mutext *rx_mutex;
} ecx_portt;

extern const uint16 priMAC[3];
extern const uint16 secMAC[3];

void ec_setupheader(void *p);
int ecx_setupnic(ecx_portt *port, const char *ifname, int secondary);
int ecx_closenic(ecx_portt *port);
void ecx_setbufstat(ecx_portt *port, uint8 idx, int bufstat);
uint8 ecx_getindex(ecx_portt *port);
int ecx_outframe(ecx_portt *port, uint8 idx, int stacknumber);
int ecx_outframe_red(ecx_portt *port, uint8 idx);
int ecx_waitinframe(ecx_portt *port, uint8 idx, int timeout);
int ecx_srconfirm(ecx_portt *port, uint8 idx, int timeout);

#ifdef __cplusplus
}
#endif

#endif /* _nicdrvh_ */
