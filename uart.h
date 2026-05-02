#pragma once
extern UART_HandleTypeDef h_usart1;
int
usart1_conf(void);
void usart_Init(void);
void usart_transmit(unsigned char * datas, int size);
void
usart1_dma_tx_link(void);
void
usart1_dma_rx_link(void);

#define USART1BUFSIZE 1600
struct UsartBuf{
    int counter;
    unsigned char buf[USART1BUFSIZE];
};
extern struct UsartBuf usart1_buf;
void UsartBuf_clear(struct UsartBuf * ths);
int UsartBuf_write(struct UsartBuf * ths, unsigned char data);
int UsartBuf_read(struct UsartBuf * ths, unsigned char * ret);
int UsartBuf_isEmpty(struct UsartBuf * ths);
int UsartBuf_isFul(struct UsartBuf * ths);
