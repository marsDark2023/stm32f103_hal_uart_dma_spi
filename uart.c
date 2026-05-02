/*
 * configure of USART1TX USART1RX USART1RX_DMA USART1TX_DMA;
 * functions for USART1 t or r;
 */
#include "main.h"
#include "config.h"
#define uartport GPIOA
#define uartpin_r GPIO_PIN_10
#define uartpin_t GPIO_PIN_9
#define baudrate 500000

#define USART1BUFSIZE 1600
struct UsartBuf{
    int counter;
    unsigned char buf[USART1BUFSIZE];
};
struct UsartBuf usart1_buf={             // for usart1 -> usart1_buf -> dma -> spi1
    .counter = 0
};
void UsartBuf_clear(struct UsartBuf * ths){
    ths->counter = 0;
}
int UsartBuf_write(struct UsartBuf * ths, unsigned char data){
    if(ths->counter == USART1BUFSIZE)
        return 0;
    ths->buf[ths->counter] = data;
    ++(ths->counter);
    return 1;
}
int UsartBuf_read(struct UsartBuf * ths, unsigned char * ret){
    if(ths->counter == 0)
        return 0;
    *ret = ths->buf[(ths->counter) -1];
    return 1;
}
int UsartBuf_isEmpty(struct UsartBuf * ths){
    return ths->counter == 0;
}
int UsartBuf_isFul(struct UsartBuf * ths){
    return (ths->counter) == (USART1BUFSIZE);
}

UART_HandleTypeDef h_usart1 = {0};
 #ifdef use_dma_for_usart
 #include "dma.h"  
 void
 usart1_dma_tx_setUp(void){
     usartTxDmaInit();
 }
 void
 usart1_dma_rx_setUp(void){
     usartRxDmaInit();
 }
 #endif
 void
 HAL_UART_MspInit(UART_HandleTypeDef * huart){
     
     __USART1_CLK_ENABLE();    
     __HAL_RCC_GPIOA_CLK_ENABLE();
     GPIO_InitTypeDef usart1_rx_pin = {0};
     usart1_rx_pin.Pin = uartpin_r ;
     usart1_rx_pin.Mode= GPIO_MODE_AF_INPUT;
     usart1_rx_pin.Pull=GPIO_NOPULL ;
     HAL_GPIO_Init(uartport, &usart1_rx_pin);
     GPIO_InitTypeDef usart1_tx_pin = {0};
     usart1_tx_pin.Pin = uartpin_t;
     usart1_tx_pin.Mode = GPIO_MODE_AF_PP;
     usart1_tx_pin.Speed= GPIO_SPEED_FREQ_HIGH;
     HAL_GPIO_Init(uartport, &usart1_tx_pin);

     #ifdef use_dma_for_usart
     usart1_dma_tx_setUp();
     usart1_dma_rx_setUp();
     #endif

     HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
     HAL_NVIC_EnableIRQ(USART1_IRQn);
 }
 int
 usart1_conf(void){
    
     h_usart1.Instance = USART1 ;
     h_usart1.Init.BaudRate  = baudrate;
     h_usart1.Init.WordLength= UART_WORDLENGTH_8B ;
     h_usart1.Init.StopBits  = UART_STOPBITS_1;
     h_usart1.Init.Parity    = UART_PARITY_NONE;
     h_usart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
     h_usart1.Init.Mode      = UART_MODE_TX_RX;
     h_usart1.Init.OverSampling = UART_OVERSAMPLING_16;
     HAL_UART_Init(&h_usart1);
     __HAL_UART_ENABLE_IT(&h_usart1, UART_IT_RXNE);

     #ifdef use_dma_for_usart
     __HAL_LINKDMA(&h_usart1, hdmatx, h_dma_usart1tx);
     __HAL_LINKDMA(&h_usart1, hdmarx, h_dma_usart1rx);

     HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
     HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

     HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
     HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
     #endif

    return 0;
 }


void usart_Init(void)
{
    usart1_conf();
}

void usart_transmit(unsigned char * datas, int size){
    HAL_UART_Transmit_DMA(&h_usart1, datas, size);
}
