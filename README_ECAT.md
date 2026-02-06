# EtherCAT SDO读取项目

基于STM32F103C8T6 + W5500的EtherCAT主站实现，用于读取从站SDO数据并显示在OLED上。

## 硬件连接

### W5500连接
| W5500引脚 | STM32引脚 | 功能 |
|-----------|-----------|------|
| SCLK | PB13 | SPI时钟 |
| MISO | PB14 | SPI数据输入 |
| MOSI | PB15 | SPI数据输出 |
| SCS | PB12 | SPI片选 |
| INT | PA11 | 中断信号 |
| RST | PA12 | 复位信号 |

### OLED连接
使用I2C1接口连接OLED显示屏。

## 项目结构

```
ECAT_M/
├── Core/                    # STM32 HAL库核心代码
│   ├── Inc/
│   └── Src/
│       └── main.c          # 主程序
├── HW/                      # 硬件驱动
│   ├── oled/               # OLED驱动
│   ├── w5500/              # W5500以太网驱动
│   │   ├── Inc/w5500.h
│   │   └── Src/w5500.c
│   └── ecat/               # EtherCAT主站实现
│       ├── Inc/ecat_master.h
│       └── Src/ecat_master.c
├── Drivers/                 # STM32 HAL驱动
└── CMakeLists.txt          # CMake构建文件
```

## 功能说明

### 主要功能
1. 初始化W5500以太网控制器，使用MACRAW模式收发原始以太网帧
2. 扫描EtherCAT网络，发现从站设备
3. 将从站配置到Pre-Operational状态
4. 使用CoE (CANopen over EtherCAT) 协议读取SDO数据
5. 在OLED上显示读取的数据

### SDO配置
- 索引 (Index): 0x2015
- 子索引 (SubIndex): 0x00
- 数据大小: 4字节

## 代码说明

### W5500驱动 (w5500.c)
- `W5500_Init()`: 初始化W5500
- `W5500_OpenMACRAW()`: 打开MACRAW模式
- `W5500_SendFrame()`: 发送原始以太网帧
- `W5500_RecvFrame()`: 接收原始以太网帧

### EtherCAT主站 (ecat_master.c)
- `ECAT_Init()`: 初始化EtherCAT主站
- `ECAT_ScanSlaves()`: 扫描从站
- `ECAT_SetState()`: 设置从站状态
- `ECAT_SDORead()`: 读取SDO数据

### 主程序 (main.c)
程序流程:
1. 初始化外设 (GPIO, SPI, I2C, OLED)
2. 初始化EtherCAT主站
3. 扫描从站
4. 每500ms读取一次SDO数据
5. 在OLED上显示数据

## 编译方法

### 使用CMake
```bash
cmake --preset Debug
cmake --build build/Debug
```

### 使用STM32CubeIDE
1. 导入项目
2. 添加源文件到工程
3. 配置Include路径
4. 编译

## 修改SDO参数

在 `main.c` 中修改以下宏定义:
```c
#define SDO_INDEX       0x2015    // SDO索引
#define SDO_SUBINDEX    0x00      // SDO子索引
#define SDO_SIZE        4         // 数据大小(字节)
#define READ_INTERVAL   500       // 读取间隔(ms)
```

## 注意事项

1. W5500使用MACRAW模式，可以收发原始以太网帧，这是EtherCAT通信的基础
2. EtherCAT使用专用的以太网类型0x88A4
3. SDO读取需要从站处于Pre-Operational或更高状态
4. STM32F103C8T6内存有限(20KB RAM)，代码已针对内存进行优化
5. 邮箱默认配置可能需要根据具体从站进行调整

## 故障排除

### OLED显示"Init Failed!"
- 检查W5500硬件连接
- 检查SPI配置是否正确
- 确认W5500供电正常

### OLED显示"No Slave Found"
- 检查以太网线连接
- 确认从站设备上电
- W5500到从站之间不需要交换机

### OLED显示"SDO Err: -4"
- SDO访问被从站拒绝
- 检查索引和子索引是否正确
- 确认从站支持该SDO对象
