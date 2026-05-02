
#include "main.h"
#include "config.h"
#define RES GPIO_PIN_2
#define DC GPIO_PIN_3
#define CS GPIO_PIN_4
#define CLK GPIO_PIN_5
#define IN GPIO_PIN_6
#define OUT GPIO_PIN_7

#define DATA 1
#define CODE 0
SPI_HandleTypeDef h_spi1 ={0};


#ifdef use_dma_for_spi
#include "dma.h"  
void
spi_dma_tx_setUP(void){
    spiTxDmaInit();
}
#endif
void
HAL_SPI_MspInit(SPI_HandleTypeDef *hspi){
    GPIO_InitTypeDef init;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();
    init.Pin = CS ;        //CS DC RES CLK IN OUT;
    init.Mode = GPIO_MODE_OUTPUT_PP ;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &init);

    init.Pin = DC ;
    HAL_GPIO_Init(GPIOA, &init);
    init.Pin = RES ;
    HAL_GPIO_Init(GPIOA, &init);

    init.Mode = GPIO_MODE_AF_PP ;
    init.Pin = CLK|IN|OUT ;
    HAL_GPIO_Init( GPIOA, &init);

    #ifdef use_dma_for_spi
    spi_dma_tx_setUP();
    #endif
}
void
spi_config(void){
    h_spi1.Instance = SPI1 ;
    h_spi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    h_spi1.Init.Direction = SPI_DIRECTION_2LINES;
    h_spi1.Init.CLKPhase = SPI_PHASE_2EDGE;
    h_spi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
    h_spi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    h_spi1.Init.CRCPolynomial =  7;
    h_spi1.Init.DataSize = SPI_DATASIZE_8BIT ;
    h_spi1.Init.FirstBit = SPI_FIRSTBIT_MSB ;
    h_spi1.Init.NSS = SPI_NSS_SOFT ;        // cs soft
    h_spi1.Init.TIMode = SPI_TIMODE_DISABLE ;
    h_spi1.Init.Mode = SPI_MODE_MASTER ;
    HAL_SPI_Init(&h_spi1);
    __HAL_SPI_ENABLE(&h_spi1);

    #ifdef use_dma_for_spi
    __HAL_LINKDMA(&h_spi1, hdmatx, h_dma_spi1tx);
    HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
    #endif
}

void
sendByte(unsigned char byte, int dc){
    HAL_GPIO_WritePin(GPIOA, CS, 0);
    HAL_GPIO_WritePin(GPIOA, DC, dc);

    uint8_t tmp = byte & 0xff;
    HAL_SPI_Transmit( &h_spi1, &tmp, 1, 0x0fffff);
}
void
sendWord(unsigned int word){
    HAL_GPIO_WritePin(GPIOA, CS, 0);
    HAL_GPIO_WritePin(GPIOA, DC, DATA);

    uint8_t tmp = (uint8_t)((word>>8)&0xff);
    HAL_SPI_Transmit(&h_spi1, &tmp, 1, 0x0fffff);

    tmp = (uint8_t)(word&0xff);
    HAL_SPI_Transmit(&h_spi1, &tmp, 1, 0x0fffff);
}

void
sendDatas_dma(unsigned char * p_data, int size){
    HAL_GPIO_WritePin(GPIOA, CS, 0);
    HAL_GPIO_WritePin(GPIOA, DC, DATA);

    HAL_SPI_Transmit_DMA(&h_spi1, p_data, size);

}

void
spiInit(void){
    spi_config();
}
