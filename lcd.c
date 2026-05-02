
#include "main.h"
#include "config.h"
#include "font.h"   
#include "delay.h" 
#include "spi.h"  
#define LCDx 240
#define LCDy 320

#define CURSORCOLOR 0x1f
#define IMAGEX 200
#define IMAGEY 100
void
addrSet(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2 );

struct Cursor {
    unsigned int x ;
    unsigned int  y;
};
#define cursor_max_x 29
#define cursor_max_y 24
struct Cursor cursor = {0};
void Cursor_incream(void){
    ++(cursor.x);
    if(cursor.x > cursor_max_x){
        cursor.x = 0;
        ++(cursor.y);
        if(cursor.y > cursor_max_y)
            cursor.y = 0;
    }
}
void Cursor_newLine(void){
    cursor.x = 0;
    ++(cursor.y);
    if(cursor.y > cursor_max_y)
        cursor.y = 0;
}
void
Cursor_show(void){
    unsigned int x1 = cursor.x * FONTWIDTH;
    unsigned int y1 = cursor.y * FONTSIZE;
    unsigned int x2 = x1 + FONTWIDTH;
    unsigned int y2 = y1 + FONTSIZE;
    addrSet(x1,y1,x2-1,y2-1);
    unsigned char data [] = {CURSORCOLOR>>8, CURSORCOLOR&0xff};
    for(int i = 0 ; i<FONTSIZE; ++i){
        for( int j = 0; j<8; ++j){
            sendDatas_dma(data, 2);
        }
    }
}
void
lcdInit(void){

    HAL_GPIO_WritePin(GPIOA, RES, GPIO_PIN_RESET);
    mydelay(100);
    HAL_GPIO_WritePin(GPIOA, RES, GPIO_PIN_SET);
    mydelay(100);
    sendByte(0x11, CODE);
    mydelay(100);

    sendByte(0x36, CODE);
    mydelay(100);
    sendByte(0x00, DATA);

    sendByte(0x3a, CODE);
    sendByte(0x05, DATA);

    sendByte(0xb2, CODE);
    sendByte(0x0c, DATA);
    sendByte(0x0c, DATA);
    sendByte(0x00, DATA);
    sendByte(0x33, DATA);
    sendByte(0x33, DATA);

    sendByte(0xb7, CODE);
    sendByte(0x35, DATA);

    sendByte(0xbb, CODE);
    sendByte(0x19, DATA);

    sendByte(0xc0, CODE);
    sendByte(0x2c, DATA);

    sendByte(0xc2, CODE);
    sendByte(0x01, DATA);

    sendByte(0xc3, CODE);
    sendByte(0x12, DATA);

    sendByte(0xc4, CODE);
    sendByte(0x20, DATA);

    sendByte(0xc6, CODE);
    sendByte(0x0f, DATA);

    sendByte(0xd0, CODE);
    sendByte(0xa4, DATA);
    sendByte(0xa1, DATA);

    sendByte(0xe0, CODE);
    unsigned char data[] ={0xd0,0x04,0x0d,0x11,0x13,0x2b,0x3f,0x54,0x4c,0x18,0x0d,0x0b,0x1f,0x23};
    for(int i = 0 ; i<14; ++i){
        sendByte(data[i], DATA);
    }

    sendByte(0xe1, CODE);
    unsigned char data2[] = {0xd0,0x04,0x0c,0x11,0x13,0x2c,0x3f,0x44,0x51,0x2f,0x1f,0x1f,0x20,0x23};
    for(int i =0 ; i<14 ; ++i){
        sendByte(data2[i], DATA);
    }

    sendByte(0x21, CODE);

    sendByte(0x29, CODE);
}
void
addrSet(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2 ){
    sendByte(0x2a, CODE);
    sendWord(x1);
    sendWord(x2);
    sendByte(0x2b, CODE);
    sendWord(y1);
    sendWord(y2);
    sendByte(0x2c, CODE);
}
void
lcd_clear(void){
    addrSet(0,0,239,319);
    for(int i = 0 ; i < 320; ++i){
        for(int j = 0 ; j < 240; ++j){
            unsigned char data[] = {0,0};
            sendDatas_dma(data, 2);
        }
    }0
}


void
__putchar(unsigned int x, unsigned int y, char c){
    u8 * data = font_get(c);
    if(!data){
        return;  // character no found
    }
    addrSet(x, y, x +7 , y +FONTSIZE-1);
    for(int i = 0 ; i<FONTSIZE; ++i){
        unsigned char line = data[i];
        for( int j = 0; j<8; ++j){
            u8 bit = line >> 7;
            line <<= 1;
            unsigned int color = bit ? RGBWHITE : RGBBLACK;
            sendDatas_dma((unsigned char *)(&color), 2);
        }
    }
}
void
putchar( char c){
    if(c == '\n'){
        Cursor_newLine();
        Cursor_show();
        return;
    }
    __putchar((FONTWIDTH * cursor.x), (FONTSIZE * cursor.y), c);
    Cursor_incream();
    Cursor_show();
}
void newLine(void){
    Cursor_newLine();
}
void print( char * string){
    for(unsigned int i = 0; string[i] != 0; i++){
        if(string[i] == '\n'){
            Cursor_newLine();
            continue;
        }
        __putchar((FONTWIDTH * cursor.x), (FONTSIZE * cursor.y), string[i]);
        Cursor_incream();
    }
    Cursor_show();
}
#define serial_print_x 200
#define serial_print_y 100
void
serial_print_dma(){
    addrSet(0, 0, serial_print_x-1, serial_print_y-1);
    HAL_GPIO_WritePin(GPIOA, DC, DATA);
}
