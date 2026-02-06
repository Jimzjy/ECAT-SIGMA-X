/*
 * NIC驱动实现 - STM32 + W5500
 */

#include "nicdrv.h"
#include "oshw.h"
#include "w5500.h"
#include <string.h>

/* EtherCAT主站MAC地址 */
const uint16 priMAC[3] = {0x1010, 0x1010, 0x1010};
const uint16 secMAC[3] = {0x1011, 0x1010, 0x1010};

/* EtherCAT以太网类型 */
#define ETH_P_ECAT 0x88A4

/**
 * @brief 设置以太网帧头
 */
void ec_setupheader(void *p)
{
    ec_etherheadert *bp = (ec_etherheadert *)p;
    
    /* 目的MAC: 广播地址 */
    bp->da0 = oshw_htons(0xFFFF);
    bp->da1 = oshw_htons(0xFFFF);
    bp->da2 = oshw_htons(0xFFFF);
    
    /* 源MAC */
    bp->sa0 = oshw_htons(priMAC[0]);
    bp->sa1 = oshw_htons(priMAC[1]);
    bp->sa2 = oshw_htons(priMAC[2]);
    
    /* EtherCAT类型 */
    bp->etype = oshw_htons(ETH_P_ECAT);
}

/**
 * @brief 初始化网络接口
 */
int ecx_setupnic(ecx_portt *port, const char *ifname, int secondary)
{
    (void)ifname;
    (void)secondary;
    
    /* 初始化W5500 */
    W5500_Init();
    
    /* 等待链接建立 */
    int timeout = 50;
    while (!W5500_GetLinkStatus() && timeout > 0)
    {
        HAL_Delay(100);
        timeout--;
    }
    
    if (timeout <= 0)
    {
        return 0;  /* 链接失败 */
    }
    
    /* 打开MACRAW模式 */
    if (W5500_OpenMACRAW() != 0)
    {
        return 0;
    }
    
    /* 初始化端口结构 */
    memset(port->rxbuf, 0, sizeof(port->rxbuf));
    memset(port->rxbufstat, 0, sizeof(port->rxbufstat));
    memset(port->txbuf, 0, sizeof(port->txbuf));
    memset(port->txbuflength, 0, sizeof(port->txbuflength));
    
    port->lastidx = 0;
    port->redstate = 0;
    port->redport = NULL;
    
    /* 初始化栈结构 */
    port->stack.txbuf = &port->txbuf;
    port->stack.txbuflength = &port->txbuflength;
    port->stack.tempbuf = &port->tempinbuf;
    port->stack.rxbuf = &port->rxbuf;
    port->stack.rxbufstat = &port->rxbufstat;
    port->stack.rxsa = &port->rxsa;
    port->stack.rxcnt = 0;
    
    /* 创建互斥锁 */
    port->getindex_mutex = osal_mutex_create();
    port->tx_mutex = osal_mutex_create();
    port->rx_mutex = osal_mutex_create();
    
    return 1;
}

/**
 * @brief 关闭网络接口
 */
int ecx_closenic(ecx_portt *port)
{
    (void)port;
    W5500_CloseMACRAW();
    return 1;
}

/**
 * @brief 设置缓冲区状态
 */
void ecx_setbufstat(ecx_portt *port, uint8 idx, int bufstat)
{
    port->rxbufstat[idx] = bufstat;
}

/**
 * @brief 获取空闲帧索引
 */
uint8 ecx_getindex(ecx_portt *port)
{
    uint8 idx;
    uint8 cnt = 0;
    
    osal_mutex_lock(port->getindex_mutex);
    
    idx = port->lastidx + 1;
    if (idx >= EC_MAXBUF)
    {
        idx = 0;
    }
    
    /* 查找空闲缓冲区 */
    while ((port->rxbufstat[idx] != EC_BUF_EMPTY) && (cnt < EC_MAXBUF))
    {
        idx++;
        cnt++;
        if (idx >= EC_MAXBUF)
        {
            idx = 0;
        }
    }
    
    port->rxbufstat[idx] = EC_BUF_ALLOC;
    port->lastidx = idx;
    
    osal_mutex_unlock(port->getindex_mutex);
    
    return idx;
}

/**
 * @brief 发送帧
 */
int ecx_outframe(ecx_portt *port, uint8 idx, int stacknumber)
{
    int rval;
    
    (void)stacknumber;
    
    osal_mutex_lock(port->tx_mutex);
    
    rval = W5500_SendFrame(port->txbuf[idx], port->txbuflength[idx]);
    
    osal_mutex_unlock(port->tx_mutex);
    
    if (rval > 0)
    {
        port->rxbufstat[idx] = EC_BUF_TX;
        return 1;
    }
    
    return 0;
}

/**
 * @brief 发送帧（冗余模式）
 */
int ecx_outframe_red(ecx_portt *port, uint8 idx)
{
    return ecx_outframe(port, idx, 0);
}

/**
 * @brief 接收帧处理
 */
static int ecx_recvpkt(ecx_portt *port, int stacknumber)
{
    int rval;
    int bytesrx;
    
    (void)stacknumber;
    
    bytesrx = W5500_RecvFrame(port->tempinbuf, EC_BUFSIZE);
    
    if (bytesrx > 0)
    {
        rval = bytesrx;
        port->tempinbufs = bytesrx;
        port->stack.rxcnt++;
    }
    else
    {
        rval = 0;
        port->tempinbufs = 0;
    }
    
    return rval;
}

/**
 * @brief 等待接收帧
 */
int ecx_waitinframe(ecx_portt *port, uint8 idx, int timeout)
{
    osal_timert timer;
    int wkc = EC_NOFRAME;
    uint8 rxidx;
    ec_etherheadert *ehp;
    ec_comt *ecp;
    uint16 l;
    
    osal_timer_start(&timer, timeout);
    
    osal_mutex_lock(port->rx_mutex);
    
    do
    {
        /* 检查是否已接收 */
        if (port->rxbufstat[idx] == EC_BUF_RCVD)
        {
            wkc = EC_NOFRAME;
            ecp = (ec_comt *)&port->rxbuf[idx][ETH_HEADERSIZE];
            l = etohs(ecp->elength) & 0x0FFF;
            if (l > 0)
            {
                wkc = (int16)etohs(*((uint16 *)&port->rxbuf[idx][ETH_HEADERSIZE + EC_HEADERSIZE + l]));
            }
            port->rxbufstat[idx] = EC_BUF_COMPLETE;
            break;
        }
        
        /* 尝试接收新帧 */
        if (ecx_recvpkt(port, 0) > 0)
        {
            /* 检查是否是EtherCAT帧 */
            ehp = (ec_etherheadert *)port->tempinbuf;
            
            if (ehp->etype == oshw_htons(ETH_P_ECAT))
            {
                ecp = (ec_comt *)&port->tempinbuf[ETH_HEADERSIZE];
                rxidx = ecp->index;
                
                /* 检查索引是否在等待列表中 */
                if (rxidx < EC_MAXBUF)
                {
                    if (port->rxbufstat[rxidx] == EC_BUF_TX)
                    {
                        /* 复制到对应缓冲区 */
                        memcpy(port->rxbuf[rxidx], port->tempinbuf, port->tempinbufs);
                        port->rxbufstat[rxidx] = EC_BUF_RCVD;
                        
                        if (rxidx == idx)
                        {
                            /* 找到目标帧 */
                            l = etohs(ecp->elength) & 0x0FFF;
                            if (l > 0)
                            {
                                wkc = (int16)etohs(*((uint16 *)&port->rxbuf[idx][ETH_HEADERSIZE + EC_HEADERSIZE + l]));
                            }
                            port->rxbufstat[idx] = EC_BUF_COMPLETE;
                            break;
                        }
                    }
                }
            }
        }
        
        osal_usleep(10);
        
    } while (!osal_timer_is_expired(&timer));
    
    osal_mutex_unlock(port->rx_mutex);
    
    return wkc;
}

/**
 * @brief 发送接收确认
 */
int ecx_srconfirm(ecx_portt *port, uint8 idx, int timeout)
{
    int wkc;
    
    /* 发送帧 */
    ecx_outframe(port, idx, 0);
    
    /* 等待响应 */
    wkc = ecx_waitinframe(port, idx, timeout);
    
    return wkc;
}
