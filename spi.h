#pragma once
#define RES GPIO_PIN_2
#define DC GPIO_PIN_3
#define CS GPIO_PIN_4
#define CLK GPIO_PIN_5
#define IN GPIO_PIN_6
#define OUT GPIO_PIN_7

#define DATA 1
#define CODE 0
extern SPI_HandleTypeDef h_spi1;
void
spi_config(void);
void
spi_dma_tx_link(void);
void
spiInit(void);
void
sendByte(unsigned char byte, int dc);
void
sendWord(unsigned int word);
void
sendDatas_dma(unsigned char * p_data, int size);
