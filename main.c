#include "main.h"
void SystemClock_Config(void);
#include "spi.h"
#include "lcd.h"
#include "led.h"
#include "uart.h"
#define DMASTEP 25
 int lcd_dma_transmit_step_counter = 0;
 int dma_image_Tx_switch = 0;
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    dma_image_Tx_switch = 1;
     sendDatas_dma( usart1_buf.buf, USART1BUFSIZE);
     HAL_UART_Receive_DMA(&h_usart1, usart1_buf.buf, USART1BUFSIZE);
 }

 void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *huart)
{
    if(! dma_image_Tx_switch)
        return;
    lcd_dma_transmit_step_counter++;
    if(lcd_dma_transmit_step_counter >=DMASTEP){
        serial_print_dma();
        lcd_dma_transmit_step_counter = 0;
    }
    dma_image_Tx_switch = 0;
 }

int main(void)
{

    HAL_Init();
    SystemClock_Config();

    usart_Init();
    spiInit();

    lcdInit();
    print("hello world");
    serial_print_dma();

    HAL_UART_Receive_DMA(&h_usart1, usart1_buf.buf, USART1BUFSIZE);
    while(1)
    {

    }
  /* USER CODE END 3 */
}

void SystemClock_Config(void)
{
RCC_OscInitTypeDef RCC_OscInitStruct = {0};
RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
RCC_OscInitStruct.HSIState = RCC_HSI_ON;
RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
{
    Error_Handler();
}

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

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
