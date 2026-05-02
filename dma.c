/*
 * DMA initalizations for USART1TX USART1RX SPI1TX ;
 */


#include"main.h"
#include "config.h"
DMA_HandleTypeDef h_dma_spi1tx = {0};
DMA_HandleTypeDef h_dma_usart1tx = {0};
DMA_HandleTypeDef h_dma_usart1rx = {0};

int usartRxDmaInit(void){
    __DMA1_CLK_ENABLE();
    h_dma_usart1rx.Instance = DMA1_Channel5; //usart1  rx
    h_dma_usart1rx.Init.Direction = DMA_PERIPH_TO_MEMORY ;
    h_dma_usart1rx.Init.PeriphInc= DMA_PINC_DISABLE ;
    h_dma_usart1rx.Init.MemInc= DMA_MINC_ENABLE ;
    h_dma_usart1rx.Init.PeriphDataAlignment= DMA_PDATAALIGN_BYTE ;
    h_dma_usart1rx.Init.MemDataAlignment= DMA_MDATAALIGN_BYTE ;
    h_dma_usart1rx.Init.Mode= DMA_NORMAL ;
    h_dma_usart1rx.Init.Priority= DMA_PRIORITY_MEDIUM ;
    HAL_DMA_Init(&(h_dma_usart1rx));

    return 1;
}
int usartTxDmaInit(void){
    __DMA1_CLK_ENABLE();
    h_dma_usart1tx.Instance = DMA1_Channel4; //usart1  tx
    h_dma_usart1tx.Init.Direction = DMA_MEMORY_TO_PERIPH ;
    h_dma_usart1tx.Init.PeriphInc= DMA_PINC_DISABLE ;
    h_dma_usart1tx.Init.MemInc= DMA_MINC_ENABLE ;
    h_dma_usart1tx.Init.PeriphDataAlignment= DMA_PDATAALIGN_BYTE ;
    h_dma_usart1tx.Init.MemDataAlignment= DMA_MDATAALIGN_BYTE ;
    h_dma_usart1tx.Init.Mode= DMA_NORMAL ;
    h_dma_usart1tx.Init.Priority= DMA_PRIORITY_MEDIUM ;
    HAL_DMA_Init(&(h_dma_usart1tx));

    return 1;
}
int spiTxDmaInit(void){
    __DMA1_CLK_ENABLE();
    h_dma_spi1tx.Instance = DMA1_Channel3; //spi1
    h_dma_spi1tx.Init.Direction = DMA_MEMORY_TO_PERIPH ;
    h_dma_spi1tx.Init.PeriphInc= DMA_PINC_DISABLE ;
    h_dma_spi1tx.Init.MemInc= DMA_MINC_ENABLE ;
    h_dma_spi1tx.Init.PeriphDataAlignment= DMA_PDATAALIGN_BYTE ;
    h_dma_spi1tx.Init.MemDataAlignment= DMA_MDATAALIGN_BYTE ;
    h_dma_spi1tx.Init.Mode= DMA_NORMAL ;
    h_dma_spi1tx.Init.Priority= DMA_PRIORITY_LOW ;
    HAL_DMA_Init( &(h_dma_spi1tx) );
    return 1;
}
