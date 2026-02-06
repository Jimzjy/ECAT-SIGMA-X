/**
 * @file ecat_master.c
 * @brief 简化版EtherCAT主站实现
 * 针对STM32F103C8T6 + W5500优化
 * 专注于SDO读取功能
 */

#include "ecat_master.h"
#include "w5500.h"
#include <string.h>

/* 帧缓冲区 */
static uint8_t txFrame[ECAT_MAX_FRAME];
static uint8_t rxFrame[ECAT_MAX_FRAME];

/* 从站列表 */
#define MAX_SLAVES 4
static ECAT_Slave_t slaves[MAX_SLAVES];
static int slaveCount = 0;

/* 帧索引计数 */
static uint8_t frameIdx = 0;

/* 源MAC地址 */
static const uint8_t srcMac[6] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10};

/* 辅助函数 - 字节序转换 */
static inline uint16_t swapU16(uint16_t val)
{
    return ((val & 0xFF) << 8) | ((val >> 8) & 0xFF);
}

/**
 * @brief 构建EtherCAT帧头
 */
static void buildEthHeader(uint8_t *frame)
{
    ECAT_EthHeader_t *hdr = (ECAT_EthHeader_t *)frame;
    
    /* 广播目的地址 */
    memset(hdr->destMac, 0xFF, 6);
    
    /* 源地址 */
    memcpy(hdr->srcMac, srcMac, 6);
    
    /* EtherCAT类型 (大端) */
    hdr->etherType = swapU16(ECAT_ETHERTYPE);
}

/* 数据报头大小 */
#define ECAT_DGRAM_HEADER_SIZE  10

/**
 * @brief 发送EtherCAT数据报
 * @return WKC (工作计数器) 或负数错误码
 */
int ECAT_SendDatagram(uint8_t cmd, uint16_t adp, uint16_t ado, 
                      uint8_t *data, uint16_t dataLen, int timeout)
{
    int frameLen;
    ECAT_Header_t *ecatHdr;
    ECAT_DatagramHeader_t *dgram;
    uint16_t wkc;
    int rxLen;
    int elapsed = 0;
    uint16_t dataOffset;
    
    /* 构建以太网头 */
    buildEthHeader(txFrame);
    
    /* EtherCAT协议头位置 (以太网头之后) */
    ecatHdr = (ECAT_Header_t *)&txFrame[14];
    
    /* 数据报头位置 (EtherCAT头之后) */
    dgram = (ECAT_DatagramHeader_t *)&txFrame[16];
    
    /* 数据起始位置 */
    dataOffset = 16 + ECAT_DGRAM_HEADER_SIZE;
    
    /* 设置EtherCAT长度字段: 11位长度 + 类型1 */
    uint16_t ecatLen = ECAT_DGRAM_HEADER_SIZE + dataLen + 2;  /* 数据报头 + 数据 + WKC */
    ecatHdr->length = (ecatLen & 0x07FF) | 0x1000;  /* 类型1 = EtherCAT命令 */
    
    /* 设置数据报头 */
    dgram->cmd = cmd;
    dgram->idx = frameIdx++;
    dgram->adp = adp;
    dgram->ado = ado;
    dgram->dlc = dataLen & 0x07FF;  /* 11位数据长度，无后续数据报 */
    dgram->irq = 0;
    
    /* 复制数据 */
    if (data && dataLen > 0)
    {
        memcpy(&txFrame[dataOffset], data, dataLen);
    }
    else
    {
        memset(&txFrame[dataOffset], 0, dataLen);
    }
    
    /* WKC初始化为0 */
    txFrame[dataOffset + dataLen] = 0;
    txFrame[dataOffset + dataLen + 1] = 0;
    
    /* 帧总长度: 以太网头(14) + EtherCAT头(2) + 数据报头(10) + 数据 + WKC(2) */
    frameLen = 14 + 2 + ECAT_DGRAM_HEADER_SIZE + dataLen + 2;
    
    /* 最小帧长度60字节 */
    if (frameLen < 60)
    {
        memset(&txFrame[frameLen], 0, 60 - frameLen);
        frameLen = 60;
    }
    
    /* 发送帧 */
    if (W5500_SendFrame(txFrame, frameLen) < 0)
    {
        return ECAT_ERROR;
    }
    
    /* 等待响应 */
    while (elapsed < timeout)
    {
        rxLen = W5500_RecvFrame(rxFrame, ECAT_MAX_FRAME);
        
        if (rxLen > 0)
        {
            /* 检查是否是EtherCAT响应 */
            ECAT_EthHeader_t *rxHdr = (ECAT_EthHeader_t *)rxFrame;
            
            if (rxHdr->etherType == swapU16(ECAT_ETHERTYPE))
            {
                /* 检查索引匹配 (数据报头在偏移16处) */
                ECAT_DatagramHeader_t *rxDgram = (ECAT_DatagramHeader_t *)&rxFrame[16];
                
                if (rxDgram->idx == dgram->idx)
                {
                    /* 复制接收数据 */
                    if (data && dataLen > 0)
                    {
                        memcpy(data, &rxFrame[16 + ECAT_DGRAM_HEADER_SIZE], dataLen);
                    }
                    
                    /* 获取WKC (在数据之后) */
                    wkc = rxFrame[16 + ECAT_DGRAM_HEADER_SIZE + dataLen];
                    wkc |= (rxFrame[16 + ECAT_DGRAM_HEADER_SIZE + dataLen + 1] << 8);
                    
                    return wkc;
                }
            }
        }
        
        HAL_Delay(1);
        elapsed++;
    }
    
    return ECAT_TIMEOUT;
}

/**
 * @brief 读取从站寄存器 (APRD命令，使用自增量地址)
 */
int ECAT_ReadReg(uint16_t adp, uint16_t addr, uint8_t *data, uint16_t len, int timeout)
{
    int wkc;
    
    /* 使用APRD命令 - adp是自增量地址 */
    wkc = ECAT_SendDatagram(EC_CMD_APRD, adp, addr, data, len, timeout);
    
    if (wkc < 0)
    {
        return wkc;
    }
    
    if (wkc == 0)
    {
        return ECAT_NO_SLAVE;
    }
    
    return ECAT_OK;
}

/**
 * @brief 写入从站寄存器 (APWR命令，使用自增量地址)
 */
int ECAT_WriteReg(uint16_t adp, uint16_t addr, uint8_t *data, uint16_t len, int timeout)
{
    int wkc;
    
    /* 使用APWR命令 - adp是自增量地址 */
    wkc = ECAT_SendDatagram(EC_CMD_APWR, adp, addr, data, len, timeout);
    
    if (wkc < 0)
    {
        return wkc;
    }
    
    if (wkc == 0)
    {
        return ECAT_NO_SLAVE;
    }
    
    return ECAT_OK;
}

/**
 * @brief 广播读取 (BRD命令)
 */
static int ECAT_BroadcastRead(uint16_t addr, uint8_t *data, uint16_t len, int timeout)
{
    return ECAT_SendDatagram(EC_CMD_BRD, 0, addr, data, len, timeout);
}

/**
 * @brief 自增量写入 (APWR命令)
 */
static int ECAT_AutoIncWrite(uint16_t adp, uint16_t addr, uint8_t *data, uint16_t len, int timeout)
{
    return ECAT_SendDatagram(EC_CMD_APWR, adp, addr, data, len, timeout);
}

/**
 * @brief 初始化EtherCAT主站
 */
int ECAT_Init(void)
{
    /* 初始化W5500 */
    W5500_Init();
    
    /* 等待链接建立 */
    int timeout = 30;
    while (!W5500_GetLinkStatus() && timeout > 0)
    {
        HAL_Delay(100);
        timeout--;
    }
    
    if (timeout <= 0)
    {
        return ECAT_ERROR;
    }
    
    /* 打开MACRAW模式 */
    if (W5500_OpenMACRAW() != 0)
    {
        return ECAT_ERROR;
    }
    
    /* 清空从站列表 */
    slaveCount = 0;
    memset(slaves, 0, sizeof(slaves));
    
    return ECAT_OK;
}

/**
 * @brief 读取从站EEPROM
 * @param slave 配置地址
 * @param eepAddr EEPROM字地址
 * @param data 数据缓冲区（至少4字节）
 * @param timeout 超时时间(ms)
 * @return ECAT_OK 或错误码
 */
static int ECAT_ReadEEPROM(uint16_t slave, uint16_t eepAddr, uint8_t *data, int timeout)
{
    uint8_t cmd[4];
    uint8_t status;
    int elapsed = 0;
    
    /* 等待EEPROM空闲 */
    do {
        if (ECAT_ReadReg(slave, ECT_REG_EEPCTL, &status, 1, 50) != ECAT_OK)
        {
            return ECAT_ERROR;
        }
        if (!(status & 0x80))  /* 不忙 */
        {
            break;
        }
        HAL_Delay(1);
        elapsed++;
    } while (elapsed < timeout);
    
    if (elapsed >= timeout)
    {
        return ECAT_TIMEOUT;
    }
    
    /* 写入EEPROM地址 */
    cmd[0] = eepAddr & 0xFF;
    cmd[1] = (eepAddr >> 8) & 0xFF;
    cmd[2] = 0;
    cmd[3] = 0;
    if (ECAT_WriteReg(slave, ECT_REG_EEPADR, cmd, 4, 50) != ECAT_OK)
    {
        return ECAT_ERROR;
    }
    
    /* 发送读取命令 (0x0100 = read) */
    cmd[0] = 0x00;
    cmd[1] = 0x01;
    if (ECAT_WriteReg(slave, ECT_REG_EEPCTL, cmd, 2, 50) != ECAT_OK)
    {
        return ECAT_ERROR;
    }
    
    /* 等待完成 */
    elapsed = 0;
    do {
        if (ECAT_ReadReg(slave, ECT_REG_EEPCTL, &status, 1, 50) != ECAT_OK)
        {
            return ECAT_ERROR;
        }
        if (!(status & 0x80))  /* 完成 */
        {
            break;
        }
        HAL_Delay(1);
        elapsed++;
    } while (elapsed < timeout);
    
    if (elapsed >= timeout)
    {
        return ECAT_TIMEOUT;
    }
    
    /* 读取数据 (4字节) */
    if (ECAT_ReadReg(slave, ECT_REG_EEPDAT, data, 4, 50) != ECAT_OK)
    {
        return ECAT_ERROR;
    }
    
    return ECAT_OK;
}

/**
 * @brief 读取从站邮箱配置
 */
static int ECAT_ReadMbxConfig(ECAT_Slave_t *slave)
{
    uint8_t data[4];
    uint16_t adp = (uint16_t)slave->adp;  /* 使用自增量地址 */
    
    /* 读取Mailbox Receive offset (EEPROM字地址0x18) */
    if (ECAT_ReadEEPROM(adp, 0x18, data, 100) == ECAT_OK)
    {
        slave->mbxRxAddr = data[0] | (data[1] << 8);
        slave->mbxRxSize = data[2] | (data[3] << 8);
    }
    else
    {
        /* 使用默认值 */
        slave->mbxRxAddr = 0x1000;
        slave->mbxRxSize = 128;
    }
    
    /* 读取Mailbox Send offset (EEPROM字地址0x1A) */
    if (ECAT_ReadEEPROM(adp, 0x1A, data, 100) == ECAT_OK)
    {
        slave->mbxTxAddr = data[0] | (data[1] << 8);
        slave->mbxTxSize = data[2] | (data[3] << 8);
    }
    else
    {
        slave->mbxTxAddr = 0x1080;
        slave->mbxTxSize = 128;
    }
    
    /* 读取Mailbox Protocol (EEPROM字地址0x1C) */
    if (ECAT_ReadEEPROM(adp, 0x1C, data, 100) == ECAT_OK)
    {
        slave->mbxProtocol = data[0] | (data[1] << 8);
    }
    else
    {
        slave->mbxProtocol = 0x04;  /* CoE */
    }
    
    return ECAT_OK;
}

/**
 * @brief 扫描从站
 */
int ECAT_ScanSlaves(void)
{
    int wkc;
    uint8_t data[2];
    uint16_t configAddr = 0x1001;  /* 从站配置地址从0x1001开始 */
    
    slaveCount = 0;
    
    /* 使用广播读取DL状态寄存器来获取从站数量 */
    memset(data, 0, sizeof(data));
    wkc = ECAT_BroadcastRead(ECT_REG_TYPE, data, 2, 500);  /* 增加超时到500ms */
    
    if (wkc <= 0)
    {
        return 0;
    }
    
    /* 分配配置地址并收集从站信息 */
    for (int i = 0; i < wkc && i < MAX_SLAVES; i++)
    {
        /* 自增量地址：第一个从站=0，第二个=1，... */
        int16_t adp = i;
        
        /* 写入配置地址到从站 (使用APWR) */
        data[0] = configAddr & 0xFF;
        data[1] = (configAddr >> 8) & 0xFF;
        
        int writeWkc = ECAT_AutoIncWrite((uint16_t)adp, ECT_REG_STAESSION, data, 2, 500);
        
        /* 保存从站信息 */
        slaves[slaveCount].configAddr = configAddr;
        slaves[slaveCount].adp = adp;  /* 保存自增量地址 */
        slaves[slaveCount].state = EC_STATE_INIT;
        
        /* 从EEPROM读取邮箱配置 */
        ECAT_ReadMbxConfig(&slaves[slaveCount]);
        
        slaveCount++;
        configAddr++;
        
        (void)writeWkc;  /* 忽略返回值 */
    }
    
    return slaveCount;
}

/**
 * @brief 获取从站数量
 */
int ECAT_GetSlaveCount(void)
{
    return slaveCount;
}

/**
 * @brief 获取从站信息
 */
ECAT_Slave_t* ECAT_GetSlave(int index)
{
    if (index >= 0 && index < slaveCount)
    {
        return &slaves[index];
    }
    return NULL;
}

/**
 * @brief 设置从站状态
 */
int ECAT_SetState(uint16_t slave, uint8_t state)
{
    uint8_t data[2];
    int ret;
    int timeout = 200;  /* 增加超时 */
    int retryCount = 3;
    
    data[0] = state;
    data[1] = 0;
    
retry:
    /* 写入AL控制寄存器 */
    ret = ECAT_WriteReg(slave, ECT_REG_ALCTL, data, 2, 100);
    if (ret != ECAT_OK)
    {
        return ret;
    }
    
    /* 等待状态转换 */
    while (timeout > 0)
    {
        HAL_Delay(10);
        
        ret = ECAT_ReadReg(slave, ECT_REG_ALSTAT, data, 2, 100);
        if (ret == ECAT_OK)
        {
            if ((data[0] & 0x0F) == state)
            {
                return ECAT_OK;
            }
            
            if (data[0] & EC_STATE_ERROR)
            {
                if (retryCount > 0)
                {
                    retryCount--;
                    /* 清除错误 */
                    uint8_t ackData[2] = {(data[0] & 0x0F) | EC_STATE_ERROR, 0};
                    ECAT_WriteReg(slave, ECT_REG_ALCTL, ackData, 2, 100);
                    HAL_Delay(50);
                    
                    /* 重新请求目标状态 */
                    data[0] = state;
                    data[1] = 0;
                    goto retry;
                }
                /* 重试耗尽，继续等待 */
            }
        }
        
        timeout--;
    }
    
    return ECAT_TIMEOUT;
}

/**
 * @brief 获取从站状态
 */
uint8_t ECAT_GetState(uint16_t slave)
{
    uint8_t data[2];
    
    if (ECAT_ReadReg(slave, ECT_REG_ALSTAT, data, 2, 100) == ECAT_OK)
    {
        return data[0] & 0x0F;
    }
    
    return EC_STATE_NONE;
}

/**
 * @brief 配置邮箱SM
 */
static int ECAT_ConfigMbx(ECAT_Slave_t *slave)
{
    uint8_t smData[8];
    uint16_t adp = (uint16_t)slave->adp;
    
    /* 配置SM0 (邮箱接收) */
    smData[0] = slave->mbxRxAddr & 0xFF;
    smData[1] = (slave->mbxRxAddr >> 8) & 0xFF;
    smData[2] = slave->mbxRxSize & 0xFF;
    smData[3] = (slave->mbxRxSize >> 8) & 0xFF;
    smData[4] = 0x26;  /* 控制: 操作使能，PDI读写，邮箱 */
    smData[5] = 0x00;
    smData[6] = 0x01;  /* 激活 */
    smData[7] = 0x00;
    
    if (ECAT_WriteReg(adp, ECT_REG_SM0, smData, 8, 100) != ECAT_OK)
    {
        return ECAT_SM0_ERR;
    }
    
    /* 配置SM1 (邮箱发送) */
    smData[0] = slave->mbxTxAddr & 0xFF;
    smData[1] = (slave->mbxTxAddr >> 8) & 0xFF;
    smData[2] = slave->mbxTxSize & 0xFF;
    smData[3] = (slave->mbxTxSize >> 8) & 0xFF;
    smData[4] = 0x22;  /* 控制: 操作使能，PDI读写，邮箱 */
    smData[5] = 0x00;
    smData[6] = 0x01;  /* 激活 */
    smData[7] = 0x00;
    
    if (ECAT_WriteReg(adp, ECT_REG_SM1, smData, 8, 100) != ECAT_OK)
    {
        return ECAT_SM1_ERR;
    }
    
    return ECAT_OK;
}

/**
 * @brief 等待邮箱响应
 */
static int ECAT_WaitMbxResponse(uint16_t slave, uint16_t mbxAddr, 
                                 uint8_t *data, int maxLen, int timeout)
{
    uint8_t smStatus;
    int elapsed = 0;
    
    while (elapsed < timeout)
    {
        /* 读取SM1状态 */
        if (ECAT_ReadReg(slave, ECT_REG_SM1 + 5, &smStatus, 1, 50) == ECAT_OK)
        {
            /* 检查邮箱满标志 */
            if (smStatus & 0x08)
            {
                /* 读取邮箱数据 */
                if (ECAT_ReadReg(slave, mbxAddr, data, maxLen, 100) == ECAT_OK)
                {
                    return ECAT_OK;
                }
            }
        }
        
        HAL_Delay(10);
        elapsed += 10;
    }
    
    return ECAT_TIMEOUT;
}

/* 邮箱计数器 */
static uint8_t mbxCnt = 1;

/**
 * @brief SDO读取
 * @param slave 从站配置地址
 * @param index SDO索引
 * @param subIndex SDO子索引
 * @param data 数据缓冲区
 * @param dataSize 输入：缓冲区大小，输出：实际数据大小
 * @param timeout 超时时间(ms)
 * @return ECAT_OK成功，其他失败
 */
int ECAT_SDORead(uint16_t slave, uint16_t index, uint8_t subIndex, 
                 void *data, int *dataSize, int timeout)
{
    ECAT_Slave_t *slv = NULL;
    uint8_t mbxData[128];
    ECAT_MbxHeader_t *mbxHdr;
    int ret;
    uint8_t smStatus;
    
    /* 查找从站 */
    for (int i = 0; i < slaveCount; i++)
    {
        if (slaves[i].configAddr == slave)
        {
            slv = &slaves[i];
            break;
        }
    }
    
    if (slv == NULL)
    {
        return ECAT_NO_SLAVE;
    }
    
    uint16_t adp = (uint16_t)slv->adp;  /* 使用自增量地址 */
    
    /* 确保从站在PreOP或更高状态 */
    if (slv->state < EC_STATE_PREOP)
    {
        uint8_t alData[2];
        
        /* 1. 先清除任何错误 - 写入INIT + Error Ack */
        alData[0] = EC_STATE_INIT | EC_STATE_ERROR;
        alData[1] = 0;
        ECAT_WriteReg(adp, ECT_REG_ALCTL, alData, 2, 100);
        HAL_Delay(10);
        
        /* 2. 请求INIT状态 */
        alData[0] = EC_STATE_INIT;
        alData[1] = 0;
        ECAT_WriteReg(adp, ECT_REG_ALCTL, alData, 2, 100);
        HAL_Delay(50);
        
        /* 3. 配置SM0和SM1用于邮箱 */
        ret = ECAT_ConfigMbx(slv);
        if (ret != ECAT_OK)
        {
            return ret;
        }
        
        /* 4. 请求PreOP状态 */
        ret = ECAT_SetState(adp, EC_STATE_PREOP);
        if (ret != ECAT_OK)
        {
            return ECAT_STATE_ERR;
        }
        
        slv->state = EC_STATE_PREOP;
        
        /* 给从站一些时间来配置 */
        HAL_Delay(50);
    }
    
    /* 等待邮箱空 */
    int waitTime = 0;
    do {
        if (ECAT_ReadReg(adp, ECT_REG_SM0 + 5, &smStatus, 1, 50) != ECAT_OK)
        {
            return ECAT_MBX_READ_ERR;
        }
        
        if (!(smStatus & 0x08))  /* 邮箱空 */
        {
            break;
        }
        
        HAL_Delay(10);
        waitTime += 10;
    } while (waitTime < timeout);
    
    if (waitTime >= timeout)
    {
        return ECAT_TIMEOUT;
    }
    
    /* 构建SDO上传请求 */
    memset(mbxData, 0, sizeof(mbxData));
    
    /* 邮箱头 (6字节) */
    mbxHdr = (ECAT_MbxHeader_t *)mbxData;
    mbxHdr->length = 10;  /* CoE头(2) + SDO命令(1) + 索引(2) + 子索引(1) + 数据(4) = 10 */
    mbxHdr->address = 0;
    mbxHdr->channel_priority = 0;  /* Channel=0, Priority=0 */
    mbxHdr->type_cnt = (EC_MAILBOX_COE & 0x0F) | ((mbxCnt << 4) & 0xF0);  /* Type=3(CoE), Cnt */
    if (++mbxCnt > 7) mbxCnt = 1;
    
    /* SDO请求头 (在邮箱头之后) */
    ECAT_SDOHeader_t *sdoHdr = (ECAT_SDOHeader_t *)&mbxData[sizeof(ECAT_MbxHeader_t)];
    sdoHdr->coe_header = (COE_SDOREQ << 12);  /* CoE 类型: SDO请求 */
    sdoHdr->command = SDO_INIT_UPLOAD_REQ;    /* 初始化上传请求 0x40 */
    sdoHdr->index = index;
    sdoHdr->subIndex = subIndex;
    sdoHdr->size = 0;
    
    /* 写入邮箱 */
    ret = ECAT_WriteReg(adp, slv->mbxRxAddr, mbxData, slv->mbxRxSize, 100);
    if (ret != ECAT_OK)
    {
        slv->state = EC_STATE_INIT;  /* 失败后重置状态 */
        return ECAT_MBX_WRITE_ERR;
    }
    
    /* 等待响应 */
    ret = ECAT_WaitMbxResponse(adp, slv->mbxTxAddr, mbxData, sizeof(mbxData), timeout);
    if (ret != ECAT_OK)
    {
        slv->state = EC_STATE_INIT;  /* 失败后重置状态 */
        return ECAT_MBX_READ_ERR;
    }
    
    /* 解析响应 */
    mbxHdr = (ECAT_MbxHeader_t *)mbxData;
    sdoHdr = (ECAT_SDOHeader_t *)&mbxData[sizeof(ECAT_MbxHeader_t)];
    
    /* 检查是否是中止响应 */
    if ((sdoHdr->command & 0xE0) == SDO_ABORT)
    {
        return ECAT_SDO_ERROR;
    }
    
    /* 检查是否是上传响应 */
    uint8_t coeType = (sdoHdr->coe_header >> 12) & 0x0F;
    if (coeType != COE_SDORES)
    {
        return ECAT_ERROR;
    }
    
    /* 提取数据 */
    int size = 0;
    uint8_t sizeIndicator = sdoHdr->command & 0x0F;
    
    if (sdoHdr->command & 0x01)  /* 加速传输 */
    {
        /* 数据在size字段中 */
        size = 4 - ((sizeIndicator >> 2) & 0x03);
        if (size > *dataSize)
        {
            size = *dataSize;
        }
        memcpy(data, &sdoHdr->size, size);
    }
    else  /* 正常传输 */
    {
        size = sdoHdr->size;
        if (size > *dataSize)
        {
            size = *dataSize;
        }
        /* 数据在SDO头之后 */
        memcpy(data, &mbxData[sizeof(ECAT_MbxHeader_t) + sizeof(ECAT_SDOHeader_t)], size);
    }
    
    *dataSize = size;
    
    return ECAT_OK;
}
/**
 * @brief SDO写入 (下载)
 * @param slave 从站地址
 * @param index SDO索引
 * @param subIndex SDO子索引
 * @param data 要写入的数据
 * @param dataSize 数据大小(最大16字节)
 * @param timeout 超时时间(ms)
 * @return ECAT_OK成功，其他失败
 */
int ECAT_SDOWrite(uint16_t slave, uint16_t index, uint8_t subIndex, 
                  void *data, int dataSize, int timeout)
{
    ECAT_Slave_t *slv = NULL;
    uint8_t mbxData[128];
    ECAT_MbxHeader_t *mbxHdr;
    int ret;
    uint8_t smStatus;
    
    /* 支持1-16字节传输 */
    if (dataSize < 1 || dataSize > 16)
    {
        return ECAT_ERROR;
    }
    
    /* 查找从站 */
    for (int i = 0; i < slaveCount; i++)
    {
        if (slaves[i].configAddr == slave)
        {
            slv = &slaves[i];
            break;
        }
    }
    
    if (slv == NULL)
    {
        return ECAT_NO_SLAVE;
    }
    
    uint16_t adp = (uint16_t)slv->adp;
    
    /* 确保从站在PreOP或更高状态 */
    if (slv->state < EC_STATE_PREOP)
    {
        uint8_t alData[2];
        
        /* 1. 先清除任何错误 */
        alData[0] = EC_STATE_INIT | EC_STATE_ERROR;
        alData[1] = 0;
        ECAT_WriteReg(adp, ECT_REG_ALCTL, alData, 2, 100);
        HAL_Delay(10);
        
        /* 2. 请求INIT状态 */
        alData[0] = EC_STATE_INIT;
        alData[1] = 0;
        ECAT_WriteReg(adp, ECT_REG_ALCTL, alData, 2, 100);
        HAL_Delay(50);
        
        /* 3. 配置SM0和SM1用于邮箱 */
        ret = ECAT_ConfigMbx(slv);
        if (ret != ECAT_OK)
        {
            return ret;
        }
        
        /* 4. 请求PreOP状态 */
        ret = ECAT_SetState(adp, EC_STATE_PREOP);
        if (ret != ECAT_OK)
        {
            return ECAT_STATE_ERR;
        }
        
        slv->state = EC_STATE_PREOP;
        HAL_Delay(50);
    }
    
    /* 等待邮箱空 */
    int waitTime = 0;
    do {
        if (ECAT_ReadReg(adp, ECT_REG_SM0 + 5, &smStatus, 1, 50) != ECAT_OK)
        {
            return ECAT_MBX_READ_ERR;
        }
        
        if (!(smStatus & 0x08))
        {
            break;
        }
        
        HAL_Delay(10);
        waitTime += 10;
    } while (waitTime < timeout);
    
    if (waitTime >= timeout)
    {
        return ECAT_TIMEOUT;
    }
    
    /* 构建SDO下载请求 */
    memset(mbxData, 0, sizeof(mbxData));
    
    /* 邮箱头 (6字节) */
    mbxHdr = (ECAT_MbxHeader_t *)mbxData;
    mbxHdr->address = 0;
    mbxHdr->channel_priority = 0;
    mbxHdr->type_cnt = (EC_MAILBOX_COE & 0x0F) | ((mbxCnt << 4) & 0xF0);
    if (++mbxCnt > 7) mbxCnt = 1;
    
    /* SDO下载请求头 */
    ECAT_SDOHeader_t *sdoHdr = (ECAT_SDOHeader_t *)&mbxData[sizeof(ECAT_MbxHeader_t)];
    sdoHdr->coe_header = (COE_SDOREQ << 12);  /* CoE 类型: SDO请求 */
    sdoHdr->index = index;
    sdoHdr->subIndex = subIndex;
    
    if (dataSize <= 4)
    {
        /* 加速传输 (Expedited): 数据放在size字段 */
        /* 命令: 0x2F(1字节), 0x2B(2字节), 0x27(3字节), 0x23(4字节) */
        sdoHdr->command = SDO_INIT_DOWNLOAD_EXP | ((4 - dataSize) << 2);
        sdoHdr->size = 0;
        memcpy(&sdoHdr->size, data, dataSize);
        mbxHdr->length = 10;  /* CoE头(2) + SDO头(8) = 10 */
    }
    else
    {
        /* 正常传输 (Normal): 数据跟在SDO头之后 */
        sdoHdr->command = SDO_INIT_DOWNLOAD_NOR;  /* e=0, s=1 */
        sdoHdr->size = dataSize;  /* 数据大小 */
        /* 复制数据到SDO头之后 */
        memcpy(&mbxData[sizeof(ECAT_MbxHeader_t) + sizeof(ECAT_SDOHeader_t)], data, dataSize);
        mbxHdr->length = 10 + dataSize;  /* CoE头(2) + SDO头(8) + 数据 */
    }
    
    /* 写入邮箱 */
    ret = ECAT_WriteReg(adp, slv->mbxRxAddr, mbxData, slv->mbxRxSize, 100);
    if (ret != ECAT_OK)
    {
        slv->state = EC_STATE_INIT;
        return ECAT_MBX_WRITE_ERR;
    }
    
    /* 等待响应 */
    ret = ECAT_WaitMbxResponse(adp, slv->mbxTxAddr, mbxData, sizeof(mbxData), timeout);
    if (ret != ECAT_OK)
    {
        slv->state = EC_STATE_INIT;
        return ECAT_MBX_READ_ERR;
    }
    
    /* 解析响应 */
    mbxHdr = (ECAT_MbxHeader_t *)mbxData;
    sdoHdr = (ECAT_SDOHeader_t *)&mbxData[sizeof(ECAT_MbxHeader_t)];
    
    /* 检查是否是中止响应 */
    if ((sdoHdr->command & 0xE0) == SDO_ABORT)
    {
        return ECAT_SDO_ERROR;
    }
    
    /* 检查是否是下载响应 (命令应为0x60) */
    if ((sdoHdr->command & 0xE0) != 0x60)
    {
        return ECAT_ERROR;
    }
    
    return ECAT_OK;
}