#pragma once
#define LCDx 240
#define LCDy 320
//font attributes
#define RGBWHITE 0x0000ffff
#define RGBBLACK 0x00000000
#define FONTSIZE 0x0c
#define FONTWIDTH 0x08
#define CURSORCOLOR 0x1f
#define IMAGEX 200
#define IMAGEY 100
void
addrSet(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2 );
void
lcdInit(void);
void
addrSet(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2 );
void
lcd_clear(void);
void
putchar( char c);
void newLine(void);
void print( char * string);
void
serial_print_dma();
