#pragma once
extern DMA_HandleTypeDef h_dma_spi1tx;
extern DMA_HandleTypeDef h_dma_usart1tx;
extern DMA_HandleTypeDef h_dma_usart1rx;
int usartRxDmaInit(void);
int usartTxDmaInit(void);
int spiTxDmaInit(void);
