/**
 * @file w5500.c
 * @brief W5500以太网控制器驱动源文件
 */

#include "w5500.h"
#include "spi.h"

extern SPI_HandleTypeDef hspi2;

/* 片选控制宏 */
#define W5500_CS_LOW()   HAL_GPIO_WritePin(W5500_SCS_PORT, W5500_SCS_PIN, GPIO_PIN_RESET)
#define W5500_CS_HIGH()  HAL_GPIO_WritePin(W5500_SCS_PORT, W5500_SCS_PIN, GPIO_PIN_SET)

/* 复位控制宏 */
#define W5500_RST_LOW()  HAL_GPIO_WritePin(W5500_RST_PORT, W5500_RST_PIN, GPIO_PIN_RESET)
#define W5500_RST_HIGH() HAL_GPIO_WritePin(W5500_RST_PORT, W5500_RST_PIN, GPIO_PIN_SET)

/* 默认MAC地址 (EtherCAT主站使用) */
static uint8_t default_mac[6] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10};

/**
 * @brief SPI发送接收单字节
 */
static uint8_t W5500_SPI_ReadWrite(uint8_t data)
{
    uint8_t rx_data;
    HAL_SPI_TransmitReceive(&hspi2, &data, &rx_data, 1, HAL_MAX_DELAY);
    return rx_data;
}

/**
 * @brief 硬件复位W5500
 */
void W5500_Reset(void)
{
    W5500_RST_LOW();
    HAL_Delay(10);
    W5500_RST_HIGH();
    HAL_Delay(100);
}

/**
 * @brief 读取单个寄存器
 */
uint8_t W5500_ReadReg(uint16_t addr, uint8_t bsb)
{
    uint8_t data;
    
    W5500_CS_LOW();
    
    W5500_SPI_ReadWrite((addr >> 8) & 0xFF);
    W5500_SPI_ReadWrite(addr & 0xFF);
    W5500_SPI_ReadWrite(bsb | W5500_READ | W5500_VDM);
    data = W5500_SPI_ReadWrite(0x00);
    
    W5500_CS_HIGH();
    
    return data;
}

/**
 * @brief 写入单个寄存器
 */
void W5500_WriteReg(uint16_t addr, uint8_t bsb, uint8_t data)
{
    W5500_CS_LOW();
    
    W5500_SPI_ReadWrite((addr >> 8) & 0xFF);
    W5500_SPI_ReadWrite(addr & 0xFF);
    W5500_SPI_ReadWrite(bsb | W5500_WRITE | W5500_VDM);
    W5500_SPI_ReadWrite(data);
    
    W5500_CS_HIGH();
}

/**
 * @brief 读取多个字节
 */
void W5500_ReadBuf(uint16_t addr, uint8_t bsb, uint8_t *buf, uint16_t len)
{
    W5500_CS_LOW();
    
    W5500_SPI_ReadWrite((addr >> 8) & 0xFF);
    W5500_SPI_ReadWrite(addr & 0xFF);
    W5500_SPI_ReadWrite(bsb | W5500_READ | W5500_VDM);
    
    for (uint16_t i = 0; i < len; i++)
    {
        buf[i] = W5500_SPI_ReadWrite(0x00);
    }
    
    W5500_CS_HIGH();
}

/**
 * @brief 写入多个字节
 */
void W5500_WriteBuf(uint16_t addr, uint8_t bsb, uint8_t *buf, uint16_t len)
{
    W5500_CS_LOW();
    
    W5500_SPI_ReadWrite((addr >> 8) & 0xFF);
    W5500_SPI_ReadWrite(addr & 0xFF);
    W5500_SPI_ReadWrite(bsb | W5500_WRITE | W5500_VDM);
    
    for (uint16_t i = 0; i < len; i++)
    {
        W5500_SPI_ReadWrite(buf[i]);
    }
    
    W5500_CS_HIGH();
}

/**
 * @brief 设置MAC地址
 */
void W5500_SetMAC(uint8_t *mac)
{
    W5500_WriteBuf(W5500_SHAR, W5500_BSB_COMMON, mac, 6);
}

/**
 * @brief 获取MAC地址
 */
void W5500_GetMAC(uint8_t *mac)
{
    W5500_ReadBuf(W5500_SHAR, W5500_BSB_COMMON, mac, 6);
}

/**
 * @brief 获取链接状态
 */
uint8_t W5500_GetLinkStatus(void)
{
    uint8_t phycfg = W5500_ReadReg(W5500_PHYCFGR, W5500_BSB_COMMON);
    return (phycfg & 0x01);  /* Bit0: Link状态 */
}

/**
 * @brief 获取W5500版本号
 */
uint8_t W5500_GetVersion(void)
{
    return W5500_ReadReg(W5500_VERSIONR, W5500_BSB_COMMON);
}

/**
 * @brief 初始化W5500
 */
void W5500_Init(void)
{
    /* 硬件复位 */
    W5500_Reset();
    
    /* 检查版本号 (应该返回0x04) */
    uint8_t version = W5500_GetVersion();
    (void)version;  /* 防止未使用警告 */
    
    /* 设置MAC地址 */
    W5500_SetMAC(default_mac);
    
    /* 禁用所有Socket中断 */
    W5500_WriteReg(W5500_SIMR, W5500_BSB_COMMON, 0x00);
    
    /* 设置Socket 0的TX/RX缓冲区大小为2KB */
    W5500_WriteReg(W5500_Sn_RXBUF_SIZE, W5500_BSB_SOCKET0, 2);
    W5500_WriteReg(W5500_Sn_TXBUF_SIZE, W5500_BSB_SOCKET0, 2);
}

/**
 * @brief 打开MACRAW模式Socket
 */
int W5500_OpenMACRAW(void)
{
    /* 关闭Socket 0 */
    W5500_WriteReg(W5500_Sn_CR, W5500_BSB_SOCKET0, W5500_Sn_CR_CLOSE);
    HAL_Delay(1);
    
    /* 等待关闭完成 */
    while (W5500_ReadReg(W5500_Sn_SR, W5500_BSB_SOCKET0) != W5500_Sn_SR_SOCK_CLOSED)
    {
        HAL_Delay(1);
    }
    
    /* 设置MACRAW模式，禁用MAC地址过滤（混杂模式） */
    /* MFEN=0: 接收所有帧 */
    W5500_WriteReg(W5500_Sn_MR, W5500_BSB_SOCKET0, W5500_Sn_MR_MACRAW);
    
    /* 打开Socket */
    W5500_WriteReg(W5500_Sn_CR, W5500_BSB_SOCKET0, W5500_Sn_CR_OPEN);
    HAL_Delay(1);
    
    /* 等待打开完成 */
    int timeout = 100;
    while (W5500_ReadReg(W5500_Sn_SR, W5500_BSB_SOCKET0) != W5500_Sn_SR_SOCK_MACRAW)
    {
        if (--timeout <= 0)
        {
            return -1;
        }
        HAL_Delay(1);
    }
    
    return 0;
}

/**
 * @brief 关闭MACRAW模式Socket
 */
void W5500_CloseMACRAW(void)
{
    W5500_WriteReg(W5500_Sn_CR, W5500_BSB_SOCKET0, W5500_Sn_CR_CLOSE);
    HAL_Delay(1);
}

/**
 * @brief 发送以太网帧
 * @param frame 帧数据
 * @param len 帧长度
 * @return 成功返回发送字节数，失败返回-1
 */
int W5500_SendFrame(uint8_t *frame, uint16_t len)
{
    uint16_t freesize;
    uint16_t ptr;
    
    if (len > W5500_MAX_FRAME_SIZE)
    {
        return -1;
    }
    
    /* 获取TX缓冲区空闲大小 */
    freesize = (W5500_ReadReg(W5500_Sn_TX_FSR, W5500_BSB_SOCKET0) << 8);
    freesize |= W5500_ReadReg(W5500_Sn_TX_FSR + 1, W5500_BSB_SOCKET0);
    
    if (freesize < len)
    {
        return -1;
    }
    
    /* 获取TX写指针 */
    ptr = (W5500_ReadReg(W5500_Sn_TX_WR, W5500_BSB_SOCKET0) << 8);
    ptr |= W5500_ReadReg(W5500_Sn_TX_WR + 1, W5500_BSB_SOCKET0);
    
    /* 直接使用指针作为地址（W5500内部处理循环） */
    W5500_WriteBuf(ptr, W5500_BSB_S0_TX, frame, len);
    
    /* 更新TX写指针 */
    ptr += len;
    W5500_WriteReg(W5500_Sn_TX_WR, W5500_BSB_SOCKET0, (ptr >> 8) & 0xFF);
    W5500_WriteReg(W5500_Sn_TX_WR + 1, W5500_BSB_SOCKET0, ptr & 0xFF);
    
    /* 发送命令 */
    W5500_WriteReg(W5500_Sn_CR, W5500_BSB_SOCKET0, W5500_Sn_CR_SEND);
    
    /* 等待发送完成 */
    int timeout = 1000;
    while ((W5500_ReadReg(W5500_Sn_IR, W5500_BSB_SOCKET0) & W5500_Sn_IR_SEND_OK) == 0)
    {
        if (--timeout <= 0)
        {
            return -1;
        }
    }
    
    /* 清除中断标志 */
    W5500_WriteReg(W5500_Sn_IR, W5500_BSB_SOCKET0, W5500_Sn_IR_SEND_OK);
    
    return len;
}

/**
 * @brief 获取接收缓冲区数据大小
 */
int W5500_GetRxSize(void)
{
    uint16_t rxsize;
    
    rxsize = (W5500_ReadReg(W5500_Sn_RX_RSR, W5500_BSB_SOCKET0) << 8);
    rxsize |= W5500_ReadReg(W5500_Sn_RX_RSR + 1, W5500_BSB_SOCKET0);
    
    return rxsize;
}

/**
 * @brief 接收以太网帧
 * @param frame 帧数据缓冲区
 * @param maxlen 最大接收长度
 * @return 成功返回接收字节数，无数据返回0，失败返回-1
 */
int W5500_RecvFrame(uint8_t *frame, uint16_t maxlen)
{
    uint16_t rxsize;
    uint16_t ptr;
    uint16_t frame_len;
    uint8_t header[2];
    
    /* 获取RX缓冲区接收数据大小 */
    rxsize = W5500_GetRxSize();
    
    if (rxsize == 0)
    {
        return 0;
    }
    
    /* 获取RX读指针 */
    ptr = (W5500_ReadReg(W5500_Sn_RX_RD, W5500_BSB_SOCKET0) << 8);
    ptr |= W5500_ReadReg(W5500_Sn_RX_RD + 1, W5500_BSB_SOCKET0);
    
    /* 读取帧长度头 (MACRAW模式下前2字节是长度) */
    W5500_ReadBuf(ptr, W5500_BSB_S0_RX, header, 2);
    frame_len = (header[0] << 8) | header[1];
    frame_len -= 2;  /* 减去长度头本身 */
    
    if (frame_len > maxlen)
    {
        frame_len = maxlen;
    }
    
    /* 读取帧数据 */
    W5500_ReadBuf(ptr + 2, W5500_BSB_S0_RX, frame, frame_len);
    
    /* 更新RX读指针 */
    ptr += frame_len + 2;
    W5500_WriteReg(W5500_Sn_RX_RD, W5500_BSB_SOCKET0, (ptr >> 8) & 0xFF);
    W5500_WriteReg(W5500_Sn_RX_RD + 1, W5500_BSB_SOCKET0, ptr & 0xFF);
    
    /* 接收命令 (更新指针) */
    W5500_WriteReg(W5500_Sn_CR, W5500_BSB_SOCKET0, W5500_Sn_CR_RECV);
    
    return frame_len;
}
