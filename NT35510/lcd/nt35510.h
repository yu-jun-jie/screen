#ifndef _NT35510_H_
#define _NT35510_H_
#include "main.h"
#include "Fonts/fonts.h"
#define NT35510ID 0x5510
#define LCD_WIDTH 480
#define LCD_HEIGHT 800

//#define LCD_DATAADDRESS (0x60000000 | (1<<(18+1)))
//#define LCD_COMMADDRESS (LCD_DATAADDRESS-2)

#define LCD_DATAADDRESS (0x60080000)
#define LCD_COMMADDRESS (0x6007FFFE)

#define LCD_WRITE(ADDRESS,DATA) do{(*(uint16_t*)ADDRESS) = DATA;}while(0)
#define LCD_READ(ADDRESS,DATA) do{DATA = (*(uint16_t*)ADDRESS);}while(0)
#define LCD_WRITE_REG_DATA(REGS,DATA) do{LCD_WRITE(LCD_COMMADDRESS,REGS);LCD_WRITE(LCD_DATAADDRESS,DATA);}while(0)
#define LCD_READ_REG_DATA(REGS,DATA) do{LCD_WRITE(LCD_COMMADDRESS,REGS);LCD_READ(LCD_DATAADDRESS,DATA);}while(0)

#define LCD_BL(X) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, X?GPIO_PIN_RESET:GPIO_PIN_SET);}while(0);



struct _LCD_DRIVE_{
	uint16_t id[3];
	union{
		struct{
//			uint8_t retain;
			uint8_t RSMY:1;
			uint8_t RSMX:1;
			uint8_t MH:1;
			uint8_t RGB:1;
			uint8_t ML:1;
			uint8_t MV:1;
			uint8_t MX:1;
			uint8_t MY:1;
		};
		uint16_t MADCTL;
	};
	uint16_t wide;
	uint16_t high;
};

extern struct _LCD_DRIVE_ G_LCD;

int lcd_init(struct _LCD_DRIVE_ *init);
int lcd_set_dot(struct _LCD_DRIVE_ *drive,uint16_t x,uint16_t y,uint16_t color);
int lcd_get_dot(struct _LCD_DRIVE_ *drive,uint16_t x,uint16_t y,uint16_t *color);
int lcd_fill_color(struct _LCD_DRIVE_ *drive,uint16_t x,uint16_t y,uint16_t width ,uint16_t height,uint16_t color);\
int lcd_show_ascii(struct _LCD_DRIVE_ *drive,uint16_t x,uint16_t y,char *str,uint16_t f_color,uint16_t b_color,sFONT font);
#endif /* SCREEN_NT35510_H_ */
