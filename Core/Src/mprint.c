#include "mprint.h"

static char mprintf_buffer[256];
static volatile uint8_t dma_tx_complete = 1;

void mprintf(const char *format, ...) {
#if defined(DEBUG)
  va_list args;
  va_start(args, format);
  int len = vsnprintf(mprintf_buffer, sizeof(mprintf_buffer), format, args);
  va_end(args);

  if (len > 0 && len < 256) {
    uint32_t timeout = 1000000;
    while (!dma_tx_complete && timeout--) {
      __NOP();
    }
    
    dma_tx_complete = 0;
    HAL_UART_Transmit_DMA(&huart1, (uint8_t*)mprintf_buffer, len);
  }
#else
    (void)format;
#endif
}

// DMA 完成回调
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART1) {
    dma_tx_complete = 1;
  }
}
