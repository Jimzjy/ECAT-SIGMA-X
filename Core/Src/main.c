/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "w5500.h"
#include "ecat_master.h"
#include "mprint.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "oled.h"
#include "font.h"
#include "math.h"
#include "ecat_params.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RESET_ABS_ENC_INDEX 0x2710
#define RESET_ABS_ENC_W 0x01
#define RESET_ABS_ENC_R 0x03

#define MENU_ITEM_HEIGHT 18
#define MENU_TOTAL_HEIGHT 64
#define MAIN_MENU_SIZE 5
#define IMPORT_MENU_SIZE 2

typedef struct MainMenu {
  char items[MAIN_MENU_SIZE][32];
  uint8_t selectIndex;
} MainMenu;
typedef struct ImportMenu {
  char items[IMPORT_MENU_SIZE][32];
  uint8_t selectIndex;
} ImportMenu;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static int slaveCount = 0;        /* 从站数量 */
static char displayBuf[32];       /* 显示缓冲区 */
static char paramDisplayBuf[32]; /* 参数显示缓冲区 */
static uint8_t ecatInitialized = 0;  /* 初始化标志 */

volatile uint8_t btn_state = 0b000000;
PageStatus currentPage = PAGE_MAIN;
uint8_t dialogSelectIndex = 0;
ECAT_Slave_t *slave;

static MainMenu mainMenu = {
  .items = {
    "重新连接",
    "参数查看/修改",
    "参数批量写入",
    "编码器复位",
    "参数初始化"
  },
  .selectIndex = 0
};

static ImportMenu importMenu = {
  .items = {
    "CNCMC800-X",
    "CNCMC800-Y"
  },
  .selectIndex = 0
};

static Parameter currentParam = {
  .paramIndex = 0x2000,
  .selectParamBit = 0,
  .rawData = 0,
  .data_min = 0,
  .data_max = 0xFFFFFFFE,
  .dataSize = 2,
  .displayFormat = DISPLAY_FORMAT_BYTE_ARRAY,
  .selectDisplayBit = 0
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void ECAT_DisplayStatus(const char *status, int progress);
static int ECAT_InitAndScan(void);

static int showRSTENCStatus(void);
static int ECAT_2710_StopStep(ECAT_Slave_t *slave, uint8_t* data16);
static void showDialog(uint8_t dialogType, const char *text0, const char *text1);
static void btnLock(void);
static void btnUnlock(void);
static const ParameterRule *FindParameterRule(uint16_t paramIndex);

void refreshScreen(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void ECAT_DisplayStatus(const char *status, int progress)
{
    OLED_NewFrame();
    OLED_PrintCenterString(0, 128, 0, (char *)status, &font16x16, OLED_COLOR_NORMAL);
    if (progress >= 0)
    {
        /* 绘制进度条背景 */
        OLED_DrawRectangle(4, 35, 120, 12, OLED_COLOR_NORMAL);
        /* 绘制进度条 */
        OLED_DrawFilledRectangle(4, 35, (progress / 100.0) * 120, 12, OLED_COLOR_NORMAL);
    }
    OLED_ShowFrame();
}

/**
* @brief 显示对话框
* @param dialogType 对话框类型，0：确认/取消，1：仅确认
* @param text0 第一行文本
* @param text1 第二行文本
*/
static void showDialog(uint8_t dialogType, const char *text0, const char *text1) {
  OLED_NewFrame();
  OLED_PrintCenterString(0, 128, 2, (char *)text0, &font16x16, OLED_COLOR_NORMAL);
  OLED_PrintCenterString(0, 128, 18, (char *)text1, &font16x16, OLED_COLOR_NORMAL);

  switch (dialogType) {
  case 0:
    OLED_PrintCenterString(0, 64, 44, "取消", &font16x16, (dialogSelectIndex == 0) ? OLED_COLOR_REVERSED : OLED_COLOR_NORMAL);
    OLED_PrintCenterString(64, 64, 44, "确认", &font16x16, (dialogSelectIndex == 1) ? OLED_COLOR_REVERSED : OLED_COLOR_NORMAL);
    break;
  case 1:
    OLED_PrintCenterString(0, 128, 44, "确认", &font16x16, OLED_COLOR_REVERSED);
    break;
  }

  OLED_ShowFrame();
}

static int ECAT_InitAndScan(void)
{
  btnLock();

  int ret;
  int w5500_retry = 0;
  static char initmsg[] = "启动网络中...";
  
  /* 显示初始化状态 */
  ECAT_DisplayStatus(initmsg, 10);
  
  /* 检查W5500 */
  if (W5500_GetVersion() != 0x04)
  {
    w5500_retry++;
    if (w5500_retry > 3) {
      ECAT_DisplayStatus("网络启动失败", -1);
      HAL_Delay(2000);
    }
    return ECAT_ERROR;
  }
  
  ECAT_DisplayStatus(initmsg, 30);
  
  /* 初始化EtherCAT主站 */
  ret = ECAT_Init();
  if (ret != ECAT_OK)
  {
    ECAT_DisplayStatus("ECAT启动失败", -1);
    HAL_Delay(2000);
    return ret;
  }

  ECAT_DisplayStatus(initmsg, 50);
  
  /* 检查链接状态 */
  if (!W5500_GetLinkStatus())
  {
    ECAT_DisplayStatus("连接异常", -1);
    HAL_Delay(2000);
    return ECAT_ERROR;
  }
  
  /* 扫描从站 */
  ECAT_DisplayStatus(initmsg, 80);
  slaveCount = ECAT_ScanSlaves();
  if (slaveCount == 0)
  {
    ECAT_DisplayStatus("未连接到从站", -1);
    HAL_Delay(2000);
    return ECAT_NO_SLAVE;
  }
  
  ECAT_DisplayStatus(initmsg, 90);
  slave = ECAT_GetSlave(0);
  if (slave == NULL)
  {
    ECAT_DisplayStatus("从站连接异常", -1);
    HAL_Delay(2000);
    return ECAT_NO_SLAVE;
  }
  
  ecatInitialized = 1;
  
  currentPage = PAGE_MAIN;
  refreshScreen();
  btnUnlock();
  return ECAT_OK;
}

static void btnLock(void) {
  btn_state |= 0b000001;
}

static void btnUnlock(void) {
  btn_state &= 0b111110;
}

static const ParameterRule *FindParameterRule(uint16_t paramIndex) {
  uint16_t low = 0;
  uint16_t high = (parameterRules_size == 0) ? 0 : (uint16_t)(parameterRules_size - 1);

  while (low <= high && parameterRules_size > 0) {
    uint16_t mid = (uint16_t)(low + ((high - low) / 2));
    uint16_t midIndex = parameterRules[mid].paramIndex;

    if (midIndex == paramIndex) {
      return &parameterRules[mid];
    }
    if (midIndex < paramIndex) {
      low = (uint16_t)(mid + 1);
    } else {
      if (mid == 0) {
        break;
      }
      high = (uint16_t)(mid - 1);
    }
  }

  return NULL;
}

int showRSTENCStatus(void) {
  btnLock();

  int ret;
  int dataSize = 16;
  uint8_t data16[16] = {0,0,1,2,0,0x20,0,0,0x08,0x10,0,0,0,0,0x00,0x00};
  int readRetry = 10;

  ECAT_DisplayStatus("编码器复位中...", 10);

  // 0 请求复位
  ret = ECAT_SDOWrite(slave->configAddr, RESET_ABS_ENC_INDEX, RESET_ABS_ENC_W, data16, 10, 2000);
  if (ret != ECAT_OK)
  {
      sprintf(displayBuf, "ABSRST 0 Err:%d", ret);
      ECAT_DisplayStatus(displayBuf, -1);
      return ret;
  }

  ECAT_DisplayStatus("编码器复位中...", 20);
  
  // 1 读取请求是否成功
  for (int i = 0; i <= readRetry; i++) {
    HAL_Delay(500);

    dataSize = 16;
    ret = ECAT_SDORead(slave->configAddr, RESET_ABS_ENC_INDEX, RESET_ABS_ENC_R, &data16, &dataSize, 2000);
    if (ret != ECAT_OK)
    {
      sprintf(displayBuf, "ABSRST 1 Err:%d", ret);
      ECAT_DisplayStatus(displayBuf, -1);
      return ret;
    }
    if (data16[0] <= 3 && dataSize > 4) {
      break;
    }

    // 超时
    if (i >= readRetry) {
      ECAT_2710_StopStep(slave, data16);
      ECAT_DisplayStatus("ABSRST 1 Timeout", -1);
      return ECAT_ERROR;
    }
  }
  if (data16[0] != 1) {
    ECAT_2710_StopStep(slave, data16);
    sprintf(displayBuf, "ABSRST 1 StErr%d", data16[0]);
    ECAT_DisplayStatus(displayBuf, -1);
    return ECAT_ERROR;
  }

  ECAT_DisplayStatus("编码器复位中...", 30);

  // 2 执行复位前准备
  data16[0] = 0;
  data16[1] = 0;
  data16[2] = 1;
  data16[3] = 2;
  data16[4] = 0x01;
  data16[5] = 0x20;
  data16[6] = 0x00;
  data16[7] = 0x00;
  data16[8] = 0x02;
  data16[9] = 0x00;
  ret = ECAT_SDOWrite(slave->configAddr, RESET_ABS_ENC_INDEX, RESET_ABS_ENC_W, data16, 10, 2000);
  if (ret != ECAT_OK)
  {
      sprintf(displayBuf, "ABSRST 2 Err:%d", ret);
      ECAT_DisplayStatus(displayBuf, -1);
      return ret;
  }

  ECAT_DisplayStatus("编码器复位中...", 40);

  // 3 读取请求是否成功
  for (int i = 0; i <= readRetry; i++) {
    HAL_Delay(500);

    dataSize = 16;
    ret = ECAT_SDORead(slave->configAddr, RESET_ABS_ENC_INDEX, RESET_ABS_ENC_R, &data16, &dataSize, 2000);
    if (ret != ECAT_OK)
    {
      sprintf(displayBuf, "ABSRST 3 Err:%d", ret);
      ECAT_DisplayStatus(displayBuf, -1);
      return ret;
    }
    if (data16[0] <= 3 && dataSize > 4) {
      break;
    }

    // 超时
    if (i >= readRetry) {
      ECAT_2710_StopStep(slave, data16);
      ECAT_DisplayStatus("ABSRST 3 Timeout", -1);
      return ECAT_ERROR;
    }
  }
  if (data16[0] != 1) {
    ECAT_2710_StopStep(slave, data16);
    sprintf(displayBuf, "ABSRST 3 StErr%d", data16[0]);
    ECAT_DisplayStatus(displayBuf, -1);
    return ECAT_ERROR;
  }

  ECAT_DisplayStatus("编码器复位中...", 60);

  // 4 执行复位
  data16[0] = 0;
  data16[1] = 0;
  data16[2] = 1;
  data16[3] = 2;
  data16[4] = 0x01;
  data16[5] = 0x20;
  data16[6] = 0x00;
  data16[7] = 0x00;
  data16[8] = 0x01;
  data16[9] = 0x00;
  ret = ECAT_SDOWrite(slave->configAddr, RESET_ABS_ENC_INDEX, RESET_ABS_ENC_W, data16, 10, 2000);
  if (ret != ECAT_OK)
  {
      sprintf(displayBuf, "ABSRST 4 Err:%d", ret);
      ECAT_DisplayStatus(displayBuf, -1);
      return ret;
  }

  ECAT_DisplayStatus("编码器复位中...", 80);

  // 5 读取请求是否成功
  for (int i = 0; i <= readRetry; i++) {
    HAL_Delay(500);

    dataSize = 16;
    ret = ECAT_SDORead(slave->configAddr, RESET_ABS_ENC_INDEX, RESET_ABS_ENC_R, &data16, &dataSize, 2000);
    if (ret != ECAT_OK)
    {
      sprintf(displayBuf, "ABSRST 5 Err:%d", ret);
      ECAT_DisplayStatus(displayBuf, -1);
      return ret;
    }
    if (data16[0] <= 3 && dataSize > 4) {
      break;
    }

    // 超时
    if (i >= readRetry) {
      ECAT_2710_StopStep(slave, data16);
      ECAT_DisplayStatus("ABSRST 5 Timeout", -1);
      return ECAT_ERROR;
    }
  }
  if (data16[0] != 1) {
    ECAT_2710_StopStep(slave, data16);
    sprintf(displayBuf, "ABSRST 5 StErr%d", data16[0]);
    ECAT_DisplayStatus(displayBuf, -1);
    return ECAT_ERROR;
  }

  ECAT_DisplayStatus("编码器复位中...", 90);

  // 6 Stop
  ECAT_2710_StopStep(slave, data16);
  
  // 成功
  dialogSelectIndex = 0;
  showDialog(1, "编码器复位成功", "");
  btnUnlock();
  return ret;
}

int ECAT_2710_StopStep(ECAT_Slave_t *slave, uint8_t* data16) {
  data16[0] = 0;
  data16[1] = 0;
  data16[2] = 1;
  data16[3] = 2;
  data16[4] = 0x00;
  data16[5] = 0x20;
  data16[6] = 0x00;
  data16[7] = 0x00;
  data16[8] = 0x00;
  data16[9] = 0x00;
  return ECAT_SDOWrite(slave->configAddr, RESET_ABS_ENC_INDEX, RESET_ABS_ENC_W, data16, 10, 2000);
}

void showMainMenu(void) {
  uint8_t i = 0;
  int8_t pos = 0;

  OLED_NewFrame();
  if ((mainMenu.selectIndex * MENU_ITEM_HEIGHT) > (MENU_TOTAL_HEIGHT - MENU_ITEM_HEIGHT)) {
    pos = MENU_TOTAL_HEIGHT - MENU_ITEM_HEIGHT;
    OLED_PrintString(0, pos, mainMenu.items[mainMenu.selectIndex], &font16x16, OLED_COLOR_REVERSED);
    pos -= MENU_ITEM_HEIGHT;
    i = mainMenu.selectIndex - 1;
    while (pos > -MENU_ITEM_HEIGHT) {
      OLED_PrintString(0, pos, mainMenu.items[i], &font16x16, OLED_COLOR_NORMAL);
      pos -= MENU_ITEM_HEIGHT;
      i--;
    }
  } else {
    while (pos < MENU_TOTAL_HEIGHT && i < MAIN_MENU_SIZE) {
      if (i == mainMenu.selectIndex) {
        OLED_PrintString(0, pos, mainMenu.items[i], &font16x16, OLED_COLOR_REVERSED);
      } else {
        OLED_PrintString(0, pos, mainMenu.items[i], &font16x16, OLED_COLOR_NORMAL);
      }
      pos += MENU_ITEM_HEIGHT;
      i++;
    }
  }
  OLED_ShowFrame();
}

void paramIndexUpDownHandle(int8_t dir) {
  if (dir > 0) {
    currentParam.paramIndex += 1 << (currentParam.selectParamBit * 4);
    currentParam.paramIndex %= 0x2661;
  } else {
    currentParam.paramIndex -= 1 << (currentParam.selectParamBit * 4);
    if (currentParam.paramIndex < 0x2000) {
      currentParam.paramIndex = 0x2000;
    }
  }
}

void paramDataUpDownHandle(int8_t dir) {
  if (dir > 0) {
    switch (currentParam.displayFormat) {
      case DISPLAY_FORMAT_BYTE_ARRAY:
        currentParam.rawData += 1 << (currentParam.selectDisplayBit * 4);
        currentParam.rawData %= (currentParam.data_max + 1);
        break;
      case DISPLAY_FORMAT_DEC:
        currentParam.rawData += 1 * pow(10, currentParam.selectDisplayBit);
        currentParam.rawData %= (currentParam.data_max + 1);
        break;
    }
  } else {
    switch (currentParam.displayFormat) {
      case DISPLAY_FORMAT_BYTE_ARRAY:
        if ((currentParam.rawData - (1 << (currentParam.selectDisplayBit * 4))) >= currentParam.data_min) {
          currentParam.rawData -= 1 << (currentParam.selectDisplayBit * 4);
        }
        break;
      case DISPLAY_FORMAT_DEC:
        if (currentParam.rawData - (1 * pow(10, currentParam.selectDisplayBit)) >= currentParam.data_min) {
          currentParam.rawData -= 1 * pow(10, currentParam.selectDisplayBit);
        }
        break;
    }
  }
}

int readParam(void) {
  currentParam.selectParamBit = 0;
  currentParam.selectDisplayBit = 0;

  int ret;
  ret = ECAT_SDORead(slave->configAddr, currentParam.paramIndex, 0, &currentParam.rawData, &currentParam.dataSize, 2000);
  if (ret != ECAT_OK)
  {
    ECAT_DisplayStatus("读取失败", -1);
    return ret;
  }

  return ret;
}

int writeParam(void) {
  int ret;
  ret = ECAT_SDOWrite(slave->configAddr, currentParam.paramIndex, 0, &currentParam.rawData, currentParam.dataSize, 2000);
  if (ret != ECAT_OK) {
    ECAT_DisplayStatus("写入失败", -1);
    return ret;
  }

  uint32_t saveFlag = 0x65766173;
  ret = ECAT_SDOWrite(slave->configAddr, 0x1010, 1, &saveFlag, 4, 2000);
  if (ret != ECAT_OK) {
    ECAT_DisplayStatus("写入失败", -1);
    return ret;
  }

  return ret;
}

void showParamMenu(void) {
  OLED_NewFrame();

  sprintf(displayBuf, "Pn%03X", currentParam.paramIndex & 0xDFFF);
  OLED_PrintASCIIString(2, 2, displayBuf, &afont24x12, OLED_COLOR_NORMAL);
  OLED_DrawLine(50 - currentParam.selectParamBit * 12, 28, 62 - currentParam.selectParamBit * 12, 28, OLED_COLOR_NORMAL);

  OLED_ShowFrame();
}

void showParamReadStatus(void) {
  btnLock();

  OLED_NewFrame();
  sprintf(displayBuf, "Pn%03X", currentParam.paramIndex & 0xDFFF);
  OLED_PrintASCIIString(2, 2, displayBuf, &afont24x12, OLED_COLOR_NORMAL);
  OLED_PrintString(2, 32, "读取中...", &font16x16, OLED_COLOR_NORMAL);
  OLED_ShowFrame();

  const ParameterRule *rule = FindParameterRule(currentParam.paramIndex);
  if (rule == NULL) {
    ECAT_DisplayStatus("不支持的参数", -1);
    btnUnlock();
    return;
  }

  currentParam.data_min = rule->data_min;
  currentParam.data_max = rule->data_max;
  currentParam.dataSize = rule->dataSize;
  currentParam.displayFormat = rule->displayFormat;

  int ret = readParam();
  if (ret != ECAT_OK) {
    OLED_NewFrame();
    sprintf(displayBuf, "Pn%03X", currentParam.paramIndex & 0xDFFF);
    OLED_PrintASCIIString(2, 2, displayBuf, &afont24x12, OLED_COLOR_NORMAL);
    OLED_PrintString(2, 32, "读取失败", &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();

    btnUnlock();
    return;
  }

  currentPage = PAGE_PARAMS_EDIT;
  refreshScreen();

  btnUnlock();
}

void showParamEdit(void) {
  OLED_NewFrame();

  sprintf(displayBuf, "Pn%03X", currentParam.paramIndex & 0xDFFF);
  OLED_PrintASCIIString(2, 2, displayBuf, &afont24x12, OLED_COLOR_NORMAL);
  switch (currentParam.displayFormat) {
    case DISPLAY_FORMAT_BYTE_ARRAY:
      switch (currentParam.dataSize) {
      case 2:
        sprintf(paramDisplayBuf, "%04X", currentParam.rawData);
        OLED_PrintASCIIString(2, 32, paramDisplayBuf, &afont24x12, OLED_COLOR_NORMAL);
        OLED_DrawLine(38 - currentParam.selectDisplayBit * 12, 58, 50 - currentParam.selectDisplayBit * 12, 58, OLED_COLOR_NORMAL);
        break;
      case 4:
        sprintf(paramDisplayBuf, "%08X", currentParam.rawData);
        OLED_PrintASCIIString(2, 32, paramDisplayBuf, &afont24x12, OLED_COLOR_NORMAL);
        OLED_DrawLine(86 - currentParam.selectDisplayBit * 12, 58, 98 - currentParam.selectDisplayBit * 12, 58, OLED_COLOR_NORMAL);
        break;
      }
      break;
    case DISPLAY_FORMAT_DEC:
      switch (currentParam.dataSize) {
      case 2:
        sprintf(paramDisplayBuf, "%05d", currentParam.rawData);
        OLED_PrintASCIIString(2, 32, paramDisplayBuf, &afont24x12, OLED_COLOR_NORMAL);
        OLED_DrawLine(50 - currentParam.selectDisplayBit * 12, 58, 62 - currentParam.selectDisplayBit * 12, 58, OLED_COLOR_NORMAL);
        break;
      case 4:
        sprintf(paramDisplayBuf, "%010d", currentParam.rawData);
        OLED_PrintASCIIString(2, 32, paramDisplayBuf, &afont24x12, OLED_COLOR_NORMAL);
        OLED_DrawLine(110 - currentParam.selectDisplayBit * 12, 58, 112 - currentParam.selectDisplayBit * 12, 58, OLED_COLOR_NORMAL);
        break;
      }
      break;
  }

  OLED_ShowFrame();
}

void showParamEditResult(void) {
  btnLock();

  OLED_NewFrame();
  OLED_PrintCenterString(0, 128, 24, "写入参数中...", &font16x16, OLED_COLOR_NORMAL);
  OLED_ShowFrame();

  int ret = writeParam();
  if (ret == ECAT_OK) {
    showDialog(1, "写入成功", "");
  } else {
    showDialog(1, "写入失败", "");
  }

  btnUnlock();
}

void showImportMenu(void) {
  uint8_t i = 0;
  int8_t pos = 0;

  OLED_NewFrame();
  if ((importMenu.selectIndex * MENU_ITEM_HEIGHT) > (MENU_TOTAL_HEIGHT - MENU_ITEM_HEIGHT)) {
    pos = MENU_TOTAL_HEIGHT - MENU_ITEM_HEIGHT;
    OLED_PrintString(0, pos, importMenu.items[importMenu.selectIndex], &font16x16, OLED_COLOR_REVERSED);
    pos -= MENU_ITEM_HEIGHT;
    i = importMenu.selectIndex - 1;
    while (pos > -MENU_ITEM_HEIGHT) {
      OLED_PrintString(0, pos, importMenu.items[i], &font16x16, OLED_COLOR_NORMAL);
      pos -= MENU_ITEM_HEIGHT;
      i--;
    }
  } else {
    while (pos < MENU_TOTAL_HEIGHT && i < IMPORT_MENU_SIZE) {
      if (i == importMenu.selectIndex) {
        OLED_PrintString(0, pos, importMenu.items[i], &font16x16, OLED_COLOR_REVERSED);
      } else {
        OLED_PrintString(0, pos, importMenu.items[i], &font16x16, OLED_COLOR_NORMAL);
      }
      pos += MENU_ITEM_HEIGHT;
      i++;
    }
  }
  OLED_ShowFrame();
}

void showImportDialog(void) {
  showDialog(0, "参数写入?", "");
}

void showImportDataStatus(void) {
  btnLock();

  dialogSelectIndex = 0;
  ECAT_DisplayStatus("写入参数中...", 5);

  const ImportParameter *importParamStore = NULL;
  uint8_t importParamStore_size = 0;
  switch (importMenu.selectIndex) {
  case 0:
    importParamStore = importParamStore_0;
    importParamStore_size = importParamStore_0_size;
    break;
  case 1:
    importParamStore = importParamStore_1;
    importParamStore_size = importParamStore_1_size;
    break;
  default:
    break;
  }

  if (importParamStore == NULL || importParamStore_size == 0) {
    showDialog(1, "Parameter NULL", "");
    btnUnlock();
    return;
  }

  ECAT_DisplayStatus("写入参数中...", 10);

  int ret;
  for (int i = 0; i < importParamStore_size; i++) {
    ret = ECAT_SDOWrite(slave->configAddr, importParamStore[i].paramIndex, 0, &importParamStore[i].rawData, importParamStore[i].dataSize, 2000);
    if (ret != ECAT_OK) {
      sprintf(displayBuf, "%04X写入失败", importParamStore[i].paramIndex);
      ECAT_DisplayStatus(displayBuf, -1);
      break;
    }
    ECAT_DisplayStatus("写入参数中...", 10 + (i + 1) * 90 / importParamStore_size);
  }

  if (ret != ECAT_OK) {
    btnUnlock();
    return;
  }

  uint32_t saveFlag = 0x65766173;
  ret = ECAT_SDOWrite(slave->configAddr, 0x1010, 1, &saveFlag, 4, 2000);
  if (ret != ECAT_OK) {
    ECAT_DisplayStatus("写入失败", -1);
  } else {
    showDialog(1, "参数写入成功", "");
  }

  btnUnlock();
}

void showRSTENCDialog() {
  showDialog(0, "编码器复位?", "");
}

void showFactoryRSTDialog() {
  showDialog(0, "参数初始化?", "");
}

void showFactoryRSTStatus() {
  btnLock();

  dialogSelectIndex = 0;
  ECAT_DisplayStatus("初始化中...", -1);

  uint32_t rstFlag = 0x64616F6C;
  int ret = ECAT_SDOWrite(slave->configAddr, 0x1011, 1, &rstFlag, 4, 2000);
  if (ret != ECAT_OK) {
    ECAT_DisplayStatus("初始化失败", -1);
  } else {
    showDialog(1, "初始化成功", "");
  }

  btnUnlock();
}

void refreshScreen(void) {
  if (ecatInitialized == 0) {
    return;
  }

  switch (currentPage) {
    case PAGE_MAIN:
      showMainMenu();
      break;
    case PAGE_PARAMS:
      showParamMenu();
      break;
    case PAGE_PARAMS_READ:
      break;
    case PAGE_PARAMS_EDIT:
      showParamEdit();
      break;
    case PAGE_PARAMS_EDIT_RESULT:
      break;
    case PAGE_IMPORT:
      showImportMenu();
      break;
    case PAGE_IMPORT_DIALOG:
      showImportDialog();
      break;
    case PAGE_IMPORT_STATUS:
      break;
    case PAGE_RSTENC_DIALOG:
      showRSTENCDialog();
      break;
    case PAGE_RSTENC_STATUS:
      break;
    case PAGE_FACTORYRST_DIALOG:
      showFactoryRSTDialog();
      break;
    case PAGE_FACTORYRST_STATUS:
      break;
  }
}

void btnStateHandle(void) {
  if (!(btn_state & 0b111110)) {
    return;
  }

  if (btn_state & 0b000010) {
    // BACK
    switch (currentPage) {
      case PAGE_MAIN:
        break;
      case PAGE_PARAMS:
        currentPage = PAGE_MAIN;
        break;
      case PAGE_PARAMS_READ:
        currentPage = PAGE_PARAMS;
        break;
      case PAGE_PARAMS_EDIT:
        currentPage = PAGE_PARAMS;
        break;
      case PAGE_PARAMS_EDIT_RESULT:
        currentPage = PAGE_PARAMS;
        break;
      case PAGE_IMPORT:
        currentPage = PAGE_MAIN;
        break;
      case PAGE_IMPORT_DIALOG:
        currentPage = PAGE_IMPORT;
        break;
      case PAGE_IMPORT_STATUS:
        currentPage = PAGE_IMPORT;
        break;
      case PAGE_RSTENC_DIALOG:
        currentPage = PAGE_MAIN;
        break;
      case PAGE_RSTENC_STATUS:
        currentPage = PAGE_MAIN;
        break;
      case PAGE_FACTORYRST_DIALOG:
        currentPage = PAGE_MAIN;
        break;
      case PAGE_FACTORYRST_STATUS:
        currentPage = PAGE_MAIN;
        break;
    }
  }
  if (btn_state & 0b000100) {
    // UP
    switch (currentPage) {
      case PAGE_MAIN:
        if (mainMenu.selectIndex > 0) {
          mainMenu.selectIndex--;
        }
        break;
      case PAGE_PARAMS:
        paramIndexUpDownHandle(1);
        break;
      case PAGE_PARAMS_READ:
        break;
      case PAGE_PARAMS_EDIT:
        paramDataUpDownHandle(1);
        break;
      case PAGE_PARAMS_EDIT_RESULT:
        break;
      case PAGE_IMPORT:
        if (importMenu.selectIndex > 0) {
          importMenu.selectIndex--;
        }
        break;
      case PAGE_IMPORT_DIALOG:
        break;
      case PAGE_IMPORT_STATUS:
        break;
      case PAGE_RSTENC_DIALOG:
        break;
      case PAGE_RSTENC_STATUS:
        break;
      case PAGE_FACTORYRST_DIALOG:
        break;
      case PAGE_FACTORYRST_STATUS:
        break;
    }
  }
  if (btn_state & 0b001000) {
    // DOWN
    switch (currentPage) {
      case PAGE_MAIN:
        mainMenu.selectIndex++;
        mainMenu.selectIndex %= MAIN_MENU_SIZE;
        break;
      case PAGE_PARAMS:
        paramIndexUpDownHandle(-1);
        break;
      case PAGE_PARAMS_READ:
        break;
      case PAGE_PARAMS_EDIT:
        paramDataUpDownHandle(-1);
        break;
      case PAGE_PARAMS_EDIT_RESULT:
        break;
      case PAGE_IMPORT:
        importMenu.selectIndex++;
        importMenu.selectIndex %= IMPORT_MENU_SIZE;
        break;
      case PAGE_IMPORT_DIALOG:
        break;
      case PAGE_IMPORT_STATUS:
        break;
      case PAGE_RSTENC_DIALOG:
        break;
      case PAGE_RSTENC_STATUS:
        break;
      case PAGE_FACTORYRST_DIALOG:
        break;
      case PAGE_FACTORYRST_STATUS:
        break;
    }
  }
  if (btn_state & 0b010000) {
    // SELECT
    switch (currentPage) {
      case PAGE_MAIN:
        break;
      case PAGE_PARAMS:
        currentParam.selectParamBit++;
        currentParam.selectParamBit %= 3;
        break;
      case PAGE_PARAMS_READ:
        break;
      case PAGE_PARAMS_EDIT:
        currentParam.selectDisplayBit++;
        switch (currentParam.displayFormat) {
        case DISPLAY_FORMAT_BYTE_ARRAY:
          if (currentParam.dataSize == 2) {
            currentParam.selectDisplayBit %= 4;
          } else if (currentParam.dataSize == 4) {
            currentParam.selectDisplayBit %= 8;
          }
          break;
        case DISPLAY_FORMAT_DEC:
          if (currentParam.dataSize == 2) {
            currentParam.selectDisplayBit %= 5;
          } else if (currentParam.dataSize == 4) {
            currentParam.selectDisplayBit %= 10;
          }
        }
        break;
      case PAGE_PARAMS_EDIT_RESULT:
        break;
      case PAGE_IMPORT:
        break;
      case PAGE_IMPORT_DIALOG:
        dialogSelectIndex++;
        dialogSelectIndex %= 2;
        break;
      case PAGE_IMPORT_STATUS:
        break;
      case PAGE_RSTENC_DIALOG:
        dialogSelectIndex++;
        dialogSelectIndex %= 2;
        break;
      case PAGE_RSTENC_STATUS:
        break;
      case PAGE_FACTORYRST_DIALOG:
        dialogSelectIndex++;
        dialogSelectIndex %= 2;
        break;
      case PAGE_FACTORYRST_STATUS:
        break;
    }
  }
  if (btn_state & 0b100000) {
    // CONFIRM
    switch (currentPage) {
      case PAGE_MAIN:
        switch (mainMenu.selectIndex) {
          case 0:
            ecatInitialized = 0;
            ECAT_InitAndScan();
            break;
          case 1:
            currentPage = PAGE_PARAMS;
            break;
          case 2:
            currentPage = PAGE_IMPORT;
            break;
          case 3:
            dialogSelectIndex = 0;
            currentPage = PAGE_RSTENC_DIALOG;
            break;
          case 4:
            dialogSelectIndex = 0;
            currentPage = PAGE_FACTORYRST_DIALOG;
            break;
        }
        break;
      case PAGE_PARAMS:
        currentPage = PAGE_PARAMS_READ;
        showParamReadStatus();
        break;
      case PAGE_PARAMS_READ:
        break;
      case PAGE_PARAMS_EDIT:
        currentPage = PAGE_PARAMS_EDIT_RESULT;
        showParamEditResult();
        break;
      case PAGE_PARAMS_EDIT_RESULT:
        currentPage = PAGE_PARAMS;
        break;
      case PAGE_IMPORT:
        dialogSelectIndex = 0;
        currentPage = PAGE_IMPORT_DIALOG;
        break;
      case PAGE_IMPORT_DIALOG:
        if (dialogSelectIndex == 0) {
          currentPage = PAGE_IMPORT;
        } else {
          currentPage = PAGE_IMPORT_STATUS;
          showImportDataStatus();
        }
        break;
      case PAGE_IMPORT_STATUS:
        currentPage = PAGE_IMPORT;
        break;
      case PAGE_RSTENC_DIALOG:
        if (dialogSelectIndex == 0) {
          currentPage = PAGE_MAIN;
        } else {
          currentPage = PAGE_RSTENC_STATUS;
          showRSTENCStatus();
        }
        break;
      case PAGE_RSTENC_STATUS:
        currentPage = PAGE_MAIN;
        break;
      case PAGE_FACTORYRST_DIALOG:
        if (dialogSelectIndex == 0) {
          currentPage = PAGE_MAIN;
        } else {
          currentPage = PAGE_FACTORYRST_STATUS;
          showFactoryRSTStatus();
        }
        break;
      case PAGE_FACTORYRST_STATUS:
        currentPage = PAGE_MAIN;
        break;
    }
  }

  // 清空按钮状态
  btn_state &= 0b000001;
  mprintf("%d", currentPage);
  refreshScreen();
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  OLED_Init();
  ECAT_InitAndScan();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (ecatInitialized != 1)
    {
      ECAT_InitAndScan();
    }

    btnStateHandle();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
