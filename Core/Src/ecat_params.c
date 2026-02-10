#include "ecat_params.h"

const ImportParameter importParamStore_0[] = { // CNCMC800-X
  { .paramIndex = 0x250A, .rawData = 0x8881, .dataSize = 2 },
  { .paramIndex = 0x250B, .rawData = 0x8888, .dataSize = 2 },
  { .paramIndex = 0x2520, .rawData = 219895608, .dataSize = 4 },
  { .paramIndex = 0x2008, .rawData = 0x4001, .dataSize = 2 },
  { .paramIndex = 0x200B, .rawData = 0x0000, .dataSize = 2 },
  { .paramIndex = 0x2170, .rawData = 0x1400, .dataSize = 2 },
  { .paramIndex = 0x220E, .rawData = 8, .dataSize = 4 },
  { .paramIndex = 0x2210, .rawData = 1, .dataSize = 4 },
  { .paramIndex = 0x2100, .rawData = 2500, .dataSize = 2 },
  { .paramIndex = 0x2102, .rawData = 5500, .dataSize = 2 },
};

const uint8_t importParamStore_0_size = sizeof(importParamStore_0) / sizeof(importParamStore_0[0]);

const ImportParameter importParamStore_1[] = { // CNCMC800-Y
  { .paramIndex = 0x250A, .rawData = 0x8881, .dataSize = 2 },
  { .paramIndex = 0x250B, .rawData = 0x8888, .dataSize = 2 },
  { .paramIndex = 0x2520, .rawData = 219895608, .dataSize = 4 },
  { .paramIndex = 0x2008, .rawData = 0x4001, .dataSize = 2 },
  { .paramIndex = 0x200B, .rawData = 0x0000, .dataSize = 2 },
  { .paramIndex = 0x2170, .rawData = 0x1400, .dataSize = 2 },
  { .paramIndex = 0x220E, .rawData = 8, .dataSize = 4 },
  { .paramIndex = 0x2210, .rawData = 1, .dataSize = 4 },
  { .paramIndex = 0x2100, .rawData = 1200, .dataSize = 2 },
  { .paramIndex = 0x2102, .rawData = 3200, .dataSize = 2 },
};

const uint8_t importParamStore_1_size = sizeof(importParamStore_1) / sizeof(importParamStore_1[0]);

const ParameterRule parameterRules[] = {
{ .paramIndex = 0x2000, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x10B1 },
    
    // Pn001(2001h): 功能选择应用开关1
{ .paramIndex = 0x2001, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x1142 },

// Pn002(2002h): 功能选择应用开关2
{ .paramIndex = 0x2002, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x4213 },

// Pn006(2006h): 功能选择应用开关6
{ .paramIndex = 0x2006, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x105F },

// Pn007(2007h): 功能选择应用开关7
{ .paramIndex = 0x2007, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x105F },

// Pn008(2008h): 功能选择应用开关8
{ .paramIndex = 0x2008, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x7121 },

// Pn009(2009h): 功能选择应用开关9
{ .paramIndex = 0x2009, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0141 },

// Pn00A(200Ah): 功能选择应用开关A
{ .paramIndex = 0x200A, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x1244 },

// Pn00B(200Bh): 功能选择应用开关B
{ .paramIndex = 0x200B, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x1121 },

// Pn00C(200Ch): 功能选择应用开关C
{ .paramIndex = 0x200C, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0141 },

// Pn00D(200Dh): 功能选择应用开关D
{ .paramIndex = 0x200D, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2001 },

// Pn00E(200Eh): 功能选择应用开关E
{ .paramIndex = 0x200E, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x4001 },

// Pn00F(200Fh): 功能选择应用开关F
{ .paramIndex = 0x200F, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2021 },

// Pn021(2021h): 预约参数
{ .paramIndex = 0x2021, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn022(2022h): 功能选择应用开关22
{ .paramIndex = 0x2022, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0011 },

// Pn02F(202Fh): 功能选择应用开关2F
{ .paramIndex = 0x202F, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0002 },

// Pn040(2040h): 预约参数
{ .paramIndex = 0x2040, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn050(2050h): Σ-LINK II响应数据选择1
{ .paramIndex = 0x2050, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x00000000, .data_max = 0xFF7EFFFF },

// Pn052(2052h): Σ-LINK II响应数据选择2
{ .paramIndex = 0x2052, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x00000000, .data_max = 0xFF7EFFFF },

// Pn054(2054h): Σ-LINK II响应数据选择3
{ .paramIndex = 0x2054, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x00000000, .data_max = 0xFF7EFFFF },

// Pn056(2056h): Σ-LINK II响应数据选择4
{ .paramIndex = 0x2056, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x00000000, .data_max = 0xFF7EFFFF },

// Pn058(2058h): Σ-LINK II响应数据选择5
{ .paramIndex = 0x2058, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x00000000, .data_max = 0xFF7EFFFF },

// Pn05A(205Ah): Σ-LINK II响应数据选择6
{ .paramIndex = 0x205A, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x00000000, .data_max = 0xFF7EFFFF },

// Pn05C(205Ch): Σ-LINK II响应数据选择7
{ .paramIndex = 0x205C, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x00000000, .data_max = 0xFF7EFFFF },

// Pn05E(205Eh): Σ-LINK II响应数据选择8
{ .paramIndex = 0x205E, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x00000000, .data_max = 0xFF7EFFFF },

// Pn080(2080h): 功能选择应用开关80
{ .paramIndex = 0x2080, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x1111 },

// Pn081(2081h): 功能选择应用开关81
{ .paramIndex = 0x2081, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x1111 },

// Pn090(2090h): Σ-LINK II指令数据选择1
{ .paramIndex = 0x2090, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x00000000, .data_max = 0xFF7EFFFF },

// Pn092(2092h): Σ-LINK II指令数据选择2
{ .paramIndex = 0x2092, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x00000000, .data_max = 0xFF7EFFFF },

// Pn094(2094h): Σ-LINK II指令数据选择3
{ .paramIndex = 0x2094, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x00000000, .data_max = 0xFF7EFFFF },

// Pn096(2096h): Σ-LINK II指令数据选择4
{ .paramIndex = 0x2096, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x00000000, .data_max = 0xFF7EFFFF },

// Pn0B1(20B1h): Σ-LINK II顺控输入分配1
{ .paramIndex = 0x20B1, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn0B2(20B2h): Σ-LINK II顺控输入分配2
{ .paramIndex = 0x20B2, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn0B5(20B5h): Σ-LINK II顺控输出分配1
{ .paramIndex = 0x20B5, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn0D6(20D6h): 预约参数
{ .paramIndex = 0x20D6, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn0DA(20DAh): Σ-LINK II半闭环编码器选择
{ .paramIndex = 0x20DA, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x011E },

// Pn0DB(20DBh): Σ-LINK II全闭环编码器选择
{ .paramIndex = 0x20DB, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x011E },

// Pn0DC(20DCh): Σ-LINK II连接节点变更检出条件选择
{ .paramIndex = 0x20DC, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0003 },

// Pn0DD(20DDh): Σ-LINK II I/O设备异常检出选择
{ .paramIndex = 0x20DD, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xF4F2 },

// Pn100(2100h): 速度环增益
{ .paramIndex = 0x2100, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn101(2101h): 速度环积分时间参数
{ .paramIndex = 0x2101, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 15, .data_max = 51200 },

// Pn102(2102h): 位置环增益
{ .paramIndex = 0x2102, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn103(2103h): 转动惯量比
{ .paramIndex = 0x2103, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn104(2104h): 第2速度环增益
{ .paramIndex = 0x2104, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn105(2105h): 第2速度环积分时间参数
{ .paramIndex = 0x2105, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 15, .data_max = 51200 },

// Pn106(2106h): 第2位置环增益
{ .paramIndex = 0x2106, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn109(2109h): 前馈
{ .paramIndex = 0x2109, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn10A(210Ah): 前馈滤波时间参数
{ .paramIndex = 0x210A, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 6400 },

// Pn10B(210Bh): 增益类应用开关
{ .paramIndex = 0x210B, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x5334 },

// Pn10C(210Ch): 模式开关（转矩指令）
{ .paramIndex = 0x210C, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 800 },

// Pn10D(210Dh): 模式开关（速度指令）
{ .paramIndex = 0x210D, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn10E(210Eh): 模式开关（加速度）
{ .paramIndex = 0x210E, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 30000 },

// Pn10F(210Fh): 模式开关（位置偏差）
{ .paramIndex = 0x210F, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn11F(211Fh): 位置积分时间参数
{ .paramIndex = 0x211F, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 50000 },

// Pn121(2121h): 摩擦补偿增益
{ .paramIndex = 0x2121, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 1000 },

// Pn122(2122h): 第2摩擦补偿增益
{ .paramIndex = 0x2122, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 1000 },

// Pn123(2123h): 摩擦补偿系数
{ .paramIndex = 0x2123, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn124(2124h): 摩擦补偿频率补偿
{ .paramIndex = 0x2124, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 1000 },

// Pn125(2125h): 摩擦补偿增益补偿
{ .paramIndex = 0x2125, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 1000 },

// Pn129(2129h): 第3摩擦补偿增益
{ .paramIndex = 0x2129, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 1000 },

// Pn12A(212Ah): 第4摩擦补偿增益
{ .paramIndex = 0x212A, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 1000 },

// Pn12B(212Bh): 第3速度环增益
{ .paramIndex = 0x212B, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn12C(212Ch): 第3速度环积分时间参数
{ .paramIndex = 0x212C, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 15, .data_max = 51200 },

// Pn12D(212Dh): 第3位置环增益
{ .paramIndex = 0x212D, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn12E(212Eh): 第4速度环增益
{ .paramIndex = 0x212E, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn12F(212Fh): 第4速度环积分时间参数
{ .paramIndex = 0x212F, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 15, .data_max = 51200 },

// Pn130(2130h): 第4位置环增益
{ .paramIndex = 0x2130, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn131(2131h): 增益切换时间1
{ .paramIndex = 0x2131, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn132(2132h): 增益切换时间2
{ .paramIndex = 0x2132, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn135(2135h): 增益切换等待时间1
{ .paramIndex = 0x2135, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn136(2136h): 增益切换等待时间2
{ .paramIndex = 0x2136, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn139(2139h): 自动增益切换类开关1
{ .paramIndex = 0x2139, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0052 },

// Pn13D(213Dh): 电流增益值
{ .paramIndex = 0x213D, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 100, .data_max = 2000 },

// Pn13F(213Fh): 无偏差控制 第2位置积分时间参数
{ .paramIndex = 0x213F, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 50000 },

// Pn140(2140h): 模型追踪控制类开关
{ .paramIndex = 0x2140, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x1121 },

// Pn141(2141h): 模型追踪控制增益
{ .paramIndex = 0x2141, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn142(2142h): 模型追踪控制增益补偿
{ .paramIndex = 0x2142, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 500, .data_max = 2000 },

// Pn143(2143h): 模型追踪控制偏置（正转方向）
{ .paramIndex = 0x2143, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn144(2144h): 模型追踪控制偏置（反转方向）
{ .paramIndex = 0x2144, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn145(2145h): 振动抑制1频率A
{ .paramIndex = 0x2145, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 25000 },

// Pn146(2146h): 振动抑制1频率B
{ .paramIndex = 0x2146, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 25000 },

// Pn147(2147h): 模型追踪控制速度前馈补偿
{ .paramIndex = 0x2147, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn148(2148h): 第2模型追踪控制增益
{ .paramIndex = 0x2148, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn149(2149h): 第2模型追踪控制增益补偿
{ .paramIndex = 0x2149, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 500, .data_max = 2000 },

// Pn14A(214Ah): 振动抑制2频率
{ .paramIndex = 0x214A, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn14B(214Bh): 振动抑制2补偿
{ .paramIndex = 0x214B, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 1000 },

// Pn14F(214Fh): 控制类开关
{ .paramIndex = 0x214F, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0031 },

// Pn15B(215Bh): 第3模型追踪控制增益
{ .paramIndex = 0x215B, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn15C(215Ch): 第3模型追踪控制增益补偿
{ .paramIndex = 0x215C, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 500, .data_max = 2000 },

// Pn15D(215Dh): 第4模型追踪控制增益
{ .paramIndex = 0x215D, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn15E(215Eh): 第4模型追踪控制增益补偿
{ .paramIndex = 0x215E, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 500, .data_max = 2000 },

// Pn160(2160h): 防振控制类开关
{ .paramIndex = 0x2160, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0011 },

// Pn161(2161h): A型抑振频率
{ .paramIndex = 0x2161, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn162(2162h): A型抑振增益补偿
{ .paramIndex = 0x2162, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 1000 },

// Pn163(2163h): A型抑振阻尼增益
{ .paramIndex = 0x2163, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 300 },

// Pn164(2164h): A型抑振滤波时间参数1补偿
{ .paramIndex = 0x2164, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 1000 },

// Pn165(2165h): A型抑振滤波时间参数2补偿
{ .paramIndex = 0x2165, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 1000 },

// Pn166(2166h): A型抑振阻尼增益2
{ .paramIndex = 0x2166, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 1000 },

// Pn170(2170h): 免调整类开关
{ .paramIndex = 0x2170, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2711 },

// Pn173(2173h): 负载波动补偿控制类开关
{ .paramIndex = 0x2173, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0001 },

// Pn174(2174h): 负载波动补偿控制 响应值
{ .paramIndex = 0x2174, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn181(2181h): 模式开关（速度指令）
{ .paramIndex = 0x2181, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn182(2182h): 模式开关（加速度）
{ .paramIndex = 0x2182, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 30000 },

// Pn183(2183h): 低频抑振功能开关
{ .paramIndex = 0x2183, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0011 },

// Pn184(2184h): 低频抑振功能频率
{ .paramIndex = 0x2184, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 1000 },

// Pn185(2185h): 低频抑振功能增益
{ .paramIndex = 0x2185, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 3000 },

// Pn186(2186h): 低频抑振滤波补偿
{ .paramIndex = 0x2186, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = -100, .data_max = 100 },

// Pn190(2190h): 无偏差控制类开关
{ .paramIndex = 0x2190, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x1101 },

// Pn191(2191h): 无偏差控制 速度前馈增益
{ .paramIndex = 0x2191, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn192(2192h): 无偏差控制 正转侧转矩前馈增益
{ .paramIndex = 0x2192, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn193(2193h): 无偏差控制 反转侧转矩前馈增益
{ .paramIndex = 0x2193, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn195(2195h): 无偏差控制 转矩前馈滤波时间参数
{ .paramIndex = 0x2195, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn196(2196h): 无偏差控制 转矩前馈移动平均时间
{ .paramIndex = 0x2196, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 5100 },

// Pn197(2197h): 无偏差控制不完全积分比例
{ .paramIndex = 0x2197, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn198(2198h): 无偏差控制 旋转型伺服电机用黏性摩擦补偿系数
{ .paramIndex = 0x2198, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 8000 },

// Pn199(2199h): 无偏差控制 直线伺服电机用黏性摩擦补偿系数
{ .paramIndex = 0x2199, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 8000 },

// Pn19A(219Ah): 模型追踪控制 无偏差控制转矩FF补偿适用百分比
{ .paramIndex = 0x219A, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn205(2205h): 旋转圈数上限值
{ .paramIndex = 0x2205, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn207(2207h): 位置控制功能开关
{ .paramIndex = 0x2207, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2210 },

// Pn20A(220Ah): 外部编码器的光栅尺节距数
{ .paramIndex = 0x220A, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 4, .data_max = 1048576 },

// Pn20E(220Eh): 电子齿轮比（分子）
{ .paramIndex = 0x220E, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 1073741824 },

// Pn210(2210h): 电子齿轮比（分母）
{ .paramIndex = 0x2210, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 1073741824 },

// Pn212(2212h): 编码器分频脉冲数
{ .paramIndex = 0x2212, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 16, .data_max = 1073741824 },

// Pn21D(221Dh): 编码器分辨率设定
{ .paramIndex = 0x221D, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x00A1 },

// Pn22A(222Ah): 全闭环控制选择开关
{ .paramIndex = 0x222A, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x1003 },

// Pn230(2230h): 位置控制扩展功能开关
{ .paramIndex = 0x2230, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0001 },

// Pn231(2231h): 背隙补偿量
{ .paramIndex = 0x2231, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = -500000, .data_max = 500000 },

// Pn233(2233h): 背隙补偿时间参数
{ .paramIndex = 0x2233, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn281(2281h): 编码器输出分辨率
{ .paramIndex = 0x2281, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 4096 },

// Pn282(2282h): 线性编码器的光栅尺节距
{ .paramIndex = 0x2282, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65536000 },

// Pn304(2304h): 点动(JOG)速度
{ .paramIndex = 0x2304, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn305(2305h): 软起动加速时间
{ .paramIndex = 0x2305, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 12000 },

// Pn306(2306h): 软起动减速时间
{ .paramIndex = 0x2306, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 12000 },

// Pn307(2307h): 速度指令滤波时间参数
{ .paramIndex = 0x2307, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn308(2308h): 速度反馈滤波器时间参数
{ .paramIndex = 0x2308, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn30A(230Ah): 伺服OFF及强制停止时的减速时间
{ .paramIndex = 0x230A, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 12000 },

// Pn30C(230Ch): 速度前馈移动平均时间
{ .paramIndex = 0x230C, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 5100 },

// Pn310(2310h): 振动检测开关
{ .paramIndex = 0x2310, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0002 },

// Pn311(2311h): 振动检测灵敏度
{ .paramIndex = 0x2311, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 50, .data_max = 500 },

// Pn312(2312h): 振动检出值
{ .paramIndex = 0x2312, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 5000 },

// Pn316(2316h): 电机最高速度
{ .paramIndex = 0x2316, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn324(2324h): 转动惯量推定开始值
{ .paramIndex = 0x2324, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 20000 },

// Pn383(2383h): 点动(JOG)速度
{ .paramIndex = 0x2383, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn384(2384h): 振动检出值
{ .paramIndex = 0x2384, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 5000 },

// Pn385(2385h): 电机最高速度
{ .paramIndex = 0x2385, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 100100 },

// Pn401(2401h): 第1段第1转矩指令滤波时间参数
{ .paramIndex = 0x2401, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn402(2402h): 正转转矩限制
{ .paramIndex = 0x2402, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 800 },

// Pn403(2403h): 反转转矩限制
{ .paramIndex = 0x2403, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 800 },

// Pn404(2404h): 正转侧外部转矩限制
{ .paramIndex = 0x2404, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 800 },

// Pn405(2405h): 反转侧外部转矩限制
{ .paramIndex = 0x2405, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 800 },

// Pn406(2406h): 紧急停止转矩
{ .paramIndex = 0x2406, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 800 },

// Pn407(2407h): 转矩控制时的速度限制
{ .paramIndex = 0x2407, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn408(2408h): 转矩类功能开关
{ .paramIndex = 0x2408, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x1111 },

// Pn409(2409h): 第1段陷波滤波器频率
{ .paramIndex = 0x2409, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 50000 },

// Pn40A(240Ah): 第1段陷波滤波器Q值
{ .paramIndex = 0x240A, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 50, .data_max = 1000 },

// Pn40B(240Bh): 第1段陷波滤波器深度
{ .paramIndex = 0x240B, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn40C(240Ch): 第2段陷波滤波器频率
{ .paramIndex = 0x240C, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 50000 },

// Pn40D(240Dh): 第2段陷波滤波器Q值
{ .paramIndex = 0x240D, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 50, .data_max = 1000 },

// Pn40E(240Eh): 第2段陷波滤波器深度
{ .paramIndex = 0x240E, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn40F(240Fh): 第2段第2转矩指令滤波器频率
{ .paramIndex = 0x240F, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 100, .data_max = 50000 },

// Pn410(2410h): 第2段第2转矩指令滤波器Q值
{ .paramIndex = 0x2410, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 50, .data_max = 100 },

// Pn412(2412h): 第1段第2转矩指令滤波时间参数
{ .paramIndex = 0x2412, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn413(2413h): 第1段第3转矩指令滤波时间参数
{ .paramIndex = 0x2413, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn414(2414h): 第1段第4转矩指令滤波时间参数
{ .paramIndex = 0x2414, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn416(2416h): 转矩类功能开关2
{ .paramIndex = 0x2416, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x1111 },

// Pn417(2417h): 第3段陷波滤波器频率
{ .paramIndex = 0x2417, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 50000 },

// Pn418(2418h): 第3段陷波滤波器Q值
{ .paramIndex = 0x2418, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 50, .data_max = 1000 },

// Pn419(2419h): 第3段陷波滤波器深度
{ .paramIndex = 0x2419, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn41A(241Ah): 第4段陷波滤波器频率
{ .paramIndex = 0x241A, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 50000 },

// Pn41B(241Bh): 第4段陷波滤波器Q值
{ .paramIndex = 0x241B, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 50, .data_max = 1000 },

// Pn41C(241Ch): 第4段陷波滤波器深度
{ .paramIndex = 0x241C, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn41D(241Dh): 第5段陷波滤波器频率
{ .paramIndex = 0x241D, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 50000 },

// Pn41E(241Eh): 第5段陷波滤波器Q值
{ .paramIndex = 0x241E, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 50, .data_max = 1000 },

// Pn41F(241Fh): 第5段陷波滤波器深度
{ .paramIndex = 0x241F, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn423(2423h): 速度脉动补偿开关
{ .paramIndex = 0x2423, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x3112 },

// Pn424(2424h): 主电路电压下降时的转矩限制
{ .paramIndex = 0x2424, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn425(2425h): 主电路电压下降时的转矩限制解除时间
{ .paramIndex = 0x2425, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn426(2426h): 转矩前馈移动平均时间
{ .paramIndex = 0x2426, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 5100 },

// Pn427(2427h): 速度脉动补偿有效速度
{ .paramIndex = 0x2427, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn428(2428h): 输出转矩补偿开关
{ .paramIndex = 0x2428, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0001 },

// Pn456(2456h): 扫描转矩指令振幅
{ .paramIndex = 0x2456, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 800 },

// Pn460(2460h): 陷波滤波器调整开关1
{ .paramIndex = 0x2460, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0101 },

// Pn475(2475h): 重力补偿类开关
{ .paramIndex = 0x2475, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0001 },

// Pn476(2476h): 重力补偿转矩
{ .paramIndex = 0x2476, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = -1000, .data_max = 1000 },

// Pn480(2480h): 推力控制时的速度限制
{ .paramIndex = 0x2480, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn481(2481h): 磁极检出速度环增益
{ .paramIndex = 0x2481, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 20000 },

// Pn482(2482h): 磁极检出速度环积分时间
{ .paramIndex = 0x2482, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 15, .data_max = 51200 },

// Pn483(2483h): 正向推力限制
{ .paramIndex = 0x2483, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 800 },

// Pn484(2484h): 反向推力限制
{ .paramIndex = 0x2484, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 800 },

// Pn485(2485h): 磁极检出指令速度
{ .paramIndex = 0x2485, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn486(2486h): 磁极检出指令加减速时间
{ .paramIndex = 0x2486, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn487(2487h): 磁极检出指令恒速时间
{ .paramIndex = 0x2487, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 300 },

// Pn488(2488h): 磁极检出指令等待时间
{ .paramIndex = 0x2488, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 50, .data_max = 500 },

// Pn48E(248Eh): 磁极检出活动范围
{ .paramIndex = 0x248E, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 65535 },

// Pn490(2490h): 磁极检出负载值
{ .paramIndex = 0x2490, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 20000 },

// Pn495(2495h): 磁极检出确认推力指令
{ .paramIndex = 0x2495, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 200 },

// Pn498(2498h): 磁极检出误差容许范围
{ .paramIndex = 0x2498, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 30 },

// Pn49F(249Fh): 速度脉动补偿有效速度（直线电机）
{ .paramIndex = 0x249F, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn501(2501h): 零位固定值
{ .paramIndex = 0x2501, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn502(2502h): 旋转检出值
{ .paramIndex = 0x2502, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 10000 },

// Pn503(2503h): 速度一致信号输出范围
{ .paramIndex = 0x2503, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn506(2506h): 制动器指令—伺服OFF 延迟时间
{ .paramIndex = 0x2506, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 50 },

// Pn507(2507h): 制动器指令输出速度值
{ .paramIndex = 0x2507, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn508(2508h): 伺服OFF — 制动器指令等待时间
{ .paramIndex = 0x2508, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 100 },

// Pn509(2509h): 瞬间停电保持时间
{ .paramIndex = 0x2509, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 20, .data_max = 50000 },

// Pn50A(250Ah): 输入信号选择1
{ .paramIndex = 0x250A, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFF2 },

// Pn50B(250Bh): 输入信号选择2
{ .paramIndex = 0x250B, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn50E(250Eh): 输出信号选择1
{ .paramIndex = 0x250E, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x6666 },

// Pn50F(250Fh): 输出信号选择2
{ .paramIndex = 0x250F, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x6666 },

// Pn510(2510h): 输出信号选择3
{ .paramIndex = 0x2510, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0666 },

// Pn511(2511h): 输入信号选择5
{ .paramIndex = 0x2511, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn512(2512h): 输出信号反转设定
{ .paramIndex = 0x2512, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x1111 },

// Pn514(2514h): 输出信号选择4
{ .paramIndex = 0x2514, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0666 },

// Pn516(2516h): 输入信号选择7
{ .paramIndex = 0x2516, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn518(2518h): 预约参数
{ .paramIndex = 0x2518, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn51B(251Bh): 电机-负载位置间偏差过大检出值
{ .paramIndex = 0x251B, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 4, .data_max = 1073741824 },

// Pn51E(251Eh): 位置偏差过大警告值
{ .paramIndex = 0x251E, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 100 },

// Pn520(2520h): 位置偏差过大警报值
{ .paramIndex = 0x2520, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 1073741823 },

// Pn522(2522h): 定位完成幅宽
{ .paramIndex = 0x2522, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 4, .data_max = 1073741824 },

// Pn524(2524h): NEAR信号范围
{ .paramIndex = 0x2524, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 1073741824 },

// Pn526(2526h): 伺服ON时位置偏差过大警报值
{ .paramIndex = 0x2526, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 1073741823 },

// Pn528(2528h): 伺服ON时位置偏差过大警告值
{ .paramIndex = 0x2528, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 100 },

// Pn529(2529h): 伺服ON时速度限制值
{ .paramIndex = 0x2529, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn52A(252Ah): 全闭环旋转1圈的乘积值
{ .paramIndex = 0x252A, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn52B(252Bh): 过载警告值
{ .paramIndex = 0x252B, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 100 },

// Pn52C(252Ch): 电机过载检出基极电流降低额定值
{ .paramIndex = 0x252C, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 100 },

// Pn530(2530h): 程序JOG运行相关开关
{ .paramIndex = 0x2530, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0005 },

// Pn531(2531h): 程序JOG移动距离
{ .paramIndex = 0x2531, .dataSize = 4, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 4, .data_max = 1073741824 },

// Pn533(2533h): 程序JOG移动速度
{ .paramIndex = 0x2533, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 10000 },

// Pn534(2534h): 程序JOG加减速时间
{ .paramIndex = 0x2534, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 2, .data_max = 10000 },

// Pn535(2535h): 程序JOG等待时间
{ .paramIndex = 0x2535, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn536(2536h): 程序JOG移动次数
{ .paramIndex = 0x2536, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 1000 },

// Pn540(2540h): 最大搜索增益
{ .paramIndex = 0x2540, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 40000 },

// Pn550(2550h): 模拟量监视1偏置量电压
{ .paramIndex = 0x2550, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = -10000, .data_max = 10000 },

// Pn551(2551h): 模拟量监视2偏置量电压
{ .paramIndex = 0x2551, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = -10000, .data_max = 10000 },

// Pn552(2552h): 模拟量监视1倍率
{ .paramIndex = 0x2552, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = -10000, .data_max = 10000 },

// Pn553(2553h): 模拟量监视2倍率
{ .paramIndex = 0x2553, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = -10000, .data_max = 10000 },

// Pn55A(255Ah): 功耗监视单位时间
{ .paramIndex = 0x255A, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 1440 },

// Pn55C(255Ch): 上位机通信异常时指定输出状态功能的开关
{ .paramIndex = 0x255C, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0001 },

// Pn55D(255Dh): 上位机通信异常时指定输出状态
{ .paramIndex = 0x255D, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x001F },

// Pn560(2560h): 残留振动检出幅度
{ .paramIndex = 0x2560, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 3000 },

// Pn561(2561h): 超调检出值
{ .paramIndex = 0x2561, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn562(2562h): 设定增益比率
{ .paramIndex = 0x2562, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 10, .data_max = 100 },

// Pn580(2580h): 零位固定值
{ .paramIndex = 0x2580, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn581(2581h): 零速值
{ .paramIndex = 0x2581, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 10000 },

// Pn582(2582h): 速度一致信号输出范围
{ .paramIndex = 0x2582, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn583(2583h): 制动器指令输出速度值
{ .paramIndex = 0x2583, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn584(2584h): 伺服ON时速度限制值
{ .paramIndex = 0x2584, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 10000 },

// Pn585(2585h): 程序JOG移动速度
{ .paramIndex = 0x2585, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 1, .data_max = 10000 },

// Pn586(2586h): 电机自转冷却率
{ .paramIndex = 0x2586, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn587(2587h): 绝对值线性编码器用磁极检出执行选择
{ .paramIndex = 0x2587, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0001 },

// Pn589(2589h): Σ-LINK II节点检出时间
{ .paramIndex = 0x2589, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 100, .data_max = 10000 },

// Pn590(2590h): 禁止正转侧驱动输入(P-OT)信号的分配
{ .paramIndex = 0x2590, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x3149 },

// Pn591(2591h): 禁止反转侧驱动输入(N-OT)信号的分配
{ .paramIndex = 0x2591, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x3149 },

// Pn593(2593h): 探针1闩锁输入 (/Probe1)信号的分配
{ .paramIndex = 0x2593, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2149 },

// Pn594(2594h): 探针2闩锁输入 (/Probe2)信号的分配
{ .paramIndex = 0x2594, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2149 },

// Pn595(2595h): HOME开关输入(/Home)信号的分配
{ .paramIndex = 0x2595, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2149 },

// Pn597(2597h): 强制停止输入(FSTP)信号的分配
{ .paramIndex = 0x2597, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x3049 },

// Pn598(2598h): 正转侧外部转矩/推力限制输入（/P-CL）信号的分配
{ .paramIndex = 0x2598, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x3149 },

// Pn599(2599h): 反转侧外部转矩/推力限制输入（/N-CL）信号的分配
{ .paramIndex = 0x2599, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x3149 },

// Pn5B0(25B0h): 定位完成输出(/COIN)信号分配
{ .paramIndex = 0x25B0, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2039 },

// Pn5B1(25B1h): 速度一致输出(/V-CMP)信号的分配
{ .paramIndex = 0x25B1, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2039 },

// Pn5B2(25B2h): 旋转检出输出(/TGON)信号的分配
{ .paramIndex = 0x25B2, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2039 },

// Pn5B3(25B3h): 伺服准备就绪输出(/S-RDY)信号的分配
{ .paramIndex = 0x25B3, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2039 },

// Pn5B4(25B4h): 转矩限制检出输出(/CLT)信号的分配
{ .paramIndex = 0x25B4, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2039 },

// Pn5B5(25B5h): 速度限制检测输出(/VLT)信号的分配
{ .paramIndex = 0x25B5, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2039 },

// Pn5B6(25B6h): 制动器控制输出(/BK)信号的分配
{ .paramIndex = 0x25B6, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2039 },

// Pn5B7(25B7h): 警告输出(/WARN)信号的分配
{ .paramIndex = 0x25B7, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2039 },

// Pn5B8(25B8h): 定位接近输出(/NEAR)信号的分配
{ .paramIndex = 0x25B8, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2039 },

// Pn5BC(25BCh): 预防维护输出(/PM)信号的分配
{ .paramIndex = 0x25BC, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2039 },

// Pn5C3(25C3h): 异常检出功能设定
{ .paramIndex = 0x25C3, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0011 },

// Pn5C4(25C4h): 异常检出样本数据集1警告值1
{ .paramIndex = 0x25C4, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100000 },

// Pn5C5(25C5h): 异常检出样本数据集1判定值1
{ .paramIndex = 0x25C5, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = -1520, .data_max = 10000 },

// Pn5C6(25C6h): 异常检出样本数据集1警告值2
{ .paramIndex = 0x25C6, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100000 },

// Pn5C7(25C7h): 异常检出样本数据集1判定值2
{ .paramIndex = 0x25C7, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = -1520, .data_max = 10000 },

// Pn5C8(25C8h): 异常检出样本数据集2警告值1
{ .paramIndex = 0x25C8, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100000 },

// Pn5C9(25C9h): 异常检出样本数据集2判定值1
{ .paramIndex = 0x25C9, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = -1520, .data_max = 10000 },

// Pn5CA(25CAh): 异常检出样本数据集2警告值2
{ .paramIndex = 0x25CA, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100000 },

// Pn5CB(25CBh): 异常检出样本数据集2判定值2
{ .paramIndex = 0x25CB, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = -1520, .data_max = 10000 },

// Pn5D7(25D7h): 定点通过输出信号反转设定
{ .paramIndex = 0x25D7, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x01F7 },

// Pn600(2600h): 再生电阻容量
{ .paramIndex = 0x2600, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn601(2601h): DB电阻容许消耗能量
{ .paramIndex = 0x2601, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn603(2603h): 再生电阻值
{ .paramIndex = 0x2603, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn604(2604h): DB电阻值
{ .paramIndex = 0x2604, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn61A(261Ah): 过热保护功能选择开关
{ .paramIndex = 0x261A, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x0003 },

// Pn61B(261Bh): 过热警报值
{ .paramIndex = 0x261B, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 500 },

// Pn61C(261Ch): 过热警告值
{ .paramIndex = 0x261C, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 100 },

// Pn61D(261Dh): 过热警报滤波器时间
{ .paramIndex = 0x261D, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_DEC, .data_min = 0, .data_max = 65535 },

// Pn621(2621h): 预约参数
{ .paramIndex = 0x2621, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn622(2622h): 预约参数
{ .paramIndex = 0x2622, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn623(2623h): 预约参数
{ .paramIndex = 0x2623, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn624(2624h): 预约参数
{ .paramIndex = 0x2624, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn625(2625h): 预约参数
{ .paramIndex = 0x2625, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn626(2626h): 预约参数
{ .paramIndex = 0x2626, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn628(2628h): 预约参数
{ .paramIndex = 0x2628, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0xFFFF },

// Pn629(2629h): 安全模块监视对象插槽选择
{ .paramIndex = 0x2629, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x110A },

// Pn660(2660h): 定点通过输出功能开关
{ .paramIndex = 0x2660, .dataSize = 2, .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY, .data_min = 0x0000, .data_max = 0x2011 }
};

const uint16_t parameterRules_size = sizeof(parameterRules) / sizeof(parameterRules[0]);