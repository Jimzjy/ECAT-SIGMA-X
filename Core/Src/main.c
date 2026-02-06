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
#include "i2c.h"
#include "spi.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "gpio.h"
#include "oled.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "w5500.h"
#include "ecat_master.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SDO_INDEX       0x2015    /* SDO索引 */
#define SDO_SUBINDEX    0x00      /* SDO子索引 */
#define SDO_SIZE        4         /* 数据大小(字节) */
#define SDO_WRITE_VALUE 3         /* 写入的数据值 */
#define READ_INTERVAL   5000       /* 读取间隔(ms) */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static uint32_t sdoData = 0;      /* SDO数据 */
static int slaveCount = 0;        /* 从站数量 */
static char displayBuf[32];       /* 显示缓冲区 */
static uint8_t ecatInitialized = 0;  /* 初始化标志 */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void ECAT_DisplayStatus(const char *status);
static void ECAT_DisplayData(uint32_t data);
static int ECAT_InitAndScan(void);
static int ECAT_ReadAndDisplay(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief 在OLED上显示状态信息
 */
static void ECAT_DisplayStatus(const char *status)
{
    OLED_NewFrame();
    OLED_PrintASCIIString(0, 0, "EtherCAT Master", &afont12x6, OLED_COLOR_NORMAL);
    OLED_PrintASCIIString(0, 16, (char *)status, &afont12x6, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
}

/**
 * @brief 在OLED上显示SDO读写结果
 */
static void ECAT_DisplayData(uint32_t data)
{
    OLED_NewFrame();
    OLED_PrintASCIIString(0, 0, "SDO 2015h:00", &afont12x6, OLED_COLOR_NORMAL);
    
    /* 显示写入值 */
    sprintf(displayBuf, "W:%lu R:%lu", (unsigned long)SDO_WRITE_VALUE, (unsigned long)data);
    OLED_PrintASCIIString(0, 16, displayBuf, &afont12x6, OLED_COLOR_NORMAL);
    
    /* 格式化数据为十六进制 */
    sprintf(displayBuf, "HEX: 0x%08lX", (unsigned long)data);
    OLED_PrintASCIIString(0, 32, displayBuf, &afont12x6, OLED_COLOR_NORMAL);
    
    /* 格式化数据为十进制 */
    sprintf(displayBuf, "DEC: %lu", (unsigned long)data);
    OLED_PrintASCIIString(0, 48, displayBuf, &afont12x6, OLED_COLOR_NORMAL);
    
    OLED_ShowFrame();
}

/**
 * @brief 初始化EtherCAT并扫描从站
 */
static int ECAT_InitAndScan(void)
{
    int ret;
    
    /* 显示初始化状态 */
    ECAT_DisplayStatus("Initializing...");
    
    /* 检查W5500 */
    if (W5500_GetVersion() != 0x04)
    {
        ECAT_DisplayStatus("W5500 Error!");
        return ECAT_ERROR;
    }
    
    /* 初始化EtherCAT主站 */
    ret = ECAT_Init();
    if (ret != ECAT_OK)
    {
        ECAT_DisplayStatus("Init Failed!");
        return ret;
    }
    
    /* 检查链接状态 */
    if (!W5500_GetLinkStatus())
    {
        ECAT_DisplayStatus("Link Down!");
        return ECAT_ERROR;
    }
    
    /* 扫描从站 */
    ECAT_DisplayStatus("Scanning...");
    slaveCount = ECAT_ScanSlaves();
    
    if (slaveCount == 0)
    {
        ECAT_DisplayStatus("No Slave Found");
        return ECAT_NO_SLAVE;
    }
    
    /* 显示从站数量 */
    sprintf(displayBuf, "Found %d Slave", slaveCount);
    ECAT_DisplayStatus(displayBuf);
    HAL_Delay(500);
    
    return ECAT_OK;
}

/**
 * @brief 写入并读取SDO显示
 */
static int ECAT_ReadAndDisplay(void)
{
    int ret;
    int dataSize = SDO_SIZE;
    ECAT_Slave_t *slave;
    uint32_t writeValue = SDO_WRITE_VALUE;
    
    /* 获取第一个从站 */
    slave = ECAT_GetSlave(0);
    if (slave == NULL)
    {
        ECAT_DisplayStatus("Slave Error");
        return ECAT_NO_SLAVE;
    }
    
    /* 先写入SDO */
    ret = ECAT_SDOWrite(slave->configAddr, SDO_INDEX, SDO_SUBINDEX, 
                        &writeValue, SDO_SIZE, 2000);
    
    if (ret != ECAT_OK)
    {
        sprintf(displayBuf, "Write Err:%d", ret);
        ECAT_DisplayStatus(displayBuf);
        return ret;
    }
    
    /* 再读取SDO验证 */
    ret = ECAT_SDORead(slave->configAddr, SDO_INDEX, SDO_SUBINDEX, 
                       &sdoData, &dataSize, 2000);
    
    if (ret == ECAT_OK)
    {
        /* 显示数据 */
        ECAT_DisplayData(sdoData);
    }
    else
    {
        /* 显示错误 */
        sprintf(displayBuf, "Read Err:%d", ret);
        ECAT_DisplayStatus(displayBuf);
    }
    
    return ret;
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
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  /* 初始化OLED */
  OLED_Init();
  ECAT_DisplayStatus("Starting...");
  HAL_Delay(500);
  
  /* 初始化EtherCAT并扫描从站 */
  if (ECAT_InitAndScan() == ECAT_OK)
  {
      ecatInitialized = 1;
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t lastReadTime = 0;
  
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (ecatInitialized)
    {
        /* 定时读取SDO数据 */
        if (HAL_GetTick() - lastReadTime >= READ_INTERVAL)
        {
            lastReadTime = HAL_GetTick();
            ECAT_ReadAndDisplay();
        }
    }
    else
    {
        /* 尝试重新初始化 */
        HAL_Delay(2000);
        if (ECAT_InitAndScan() == ECAT_OK)
        {
            ecatInitialized = 1;
        }
    }
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
