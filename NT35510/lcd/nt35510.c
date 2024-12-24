#include "lcd/nt35510.h"

struct _LCD_DRIVE_ G_LCD = {0};

int lcd_unlock(uint8_t page){
	LCD_WRITE_REG_DATA(0xF000,0x0055);
	LCD_WRITE_REG_DATA(0xF001,0x00AA);
	LCD_WRITE_REG_DATA(0xF002,0x0052);
	LCD_WRITE_REG_DATA(0xF003,0x0008);
	LCD_WRITE_REG_DATA(0xF004,((page)?0x0001:0x0000));
	return 0;
}

int lcd_set_cursor(uint16_t x,uint16_t y){
	LCD_WRITE_REG_DATA(0x2A00,x>>8);
	LCD_WRITE_REG_DATA(0x2A01,x&0xFF);
	LCD_WRITE_REG_DATA(0x2B00,y>>8);
	LCD_WRITE_REG_DATA(0x2B01,y&0xFF);
	return 0;
}
void lcd_set_dir(struct _LCD_DRIVE_ *drive){
	LCD_WRITE_REG_DATA(0x2A00,0>>8);
	LCD_WRITE_REG_DATA(0x2A01,0&0xFF);
	LCD_WRITE_REG_DATA(0x2A02,drive->wide>>8);
	LCD_WRITE_REG_DATA(0x2A03,drive->wide&0xFF);
	LCD_WRITE_REG_DATA(0x2B00,0>>8);
	LCD_WRITE_REG_DATA(0x2B01,0&0xFF);
	LCD_WRITE_REG_DATA(0x2B02,drive->high>>8);
	LCD_WRITE_REG_DATA(0x2B03,drive->high&0xFF);
	LCD_WRITE_REG_DATA(0x3600,drive->MADCTL);
}

int lcd_init(struct _LCD_DRIVE_ *init){
	if(init == NULL)return 1;
	lcd_unlock(1);
	LCD_READ_REG_DATA(0XC500,init->id[1]);
	LCD_READ_REG_DATA(0XC501,init->id[2]);
	init->id[0] = (init->id[1]<<8)|init->id[2];
	if(init->id[0] != NT35510ID)return 1;
	HAL_Delay(5);
	if(init->MV){
		init->wide = LCD_HEIGHT;
		init->high = LCD_WIDTH;
	}
	else{
		init->wide = LCD_WIDTH;
		init->high = LCD_HEIGHT;
	}
	lcd_unlock(0);
	lcd_set_dir(init);
	LCD_WRITE(LCD_COMMADDRESS,0x2800);
	LCD_WRITE_REG_DATA(0x3A00,0x0055);
	LCD_WRITE(LCD_COMMADDRESS,0x1100);
	LCD_WRITE(LCD_COMMADDRESS,0x2900);

	for(uint16_t i=0;i<init->high;++i){
		for(uint16_t j=0;j<init->wide;++j){
			lcd_set_dot(init,j,i,0xFFFF);
		}
	}
	return 0;
}

int lcd_set_dot(struct _LCD_DRIVE_ *drive,uint16_t x,uint16_t y,uint16_t color){
	lcd_set_cursor(x,y);
	LCD_WRITE(LCD_COMMADDRESS,0x2C00);
	LCD_WRITE(LCD_DATAADDRESS,color);
	return 0;
}

int lcd_get_dot(struct _LCD_DRIVE_ *drive,uint16_t x,uint16_t y,uint16_t *color){
	lcd_set_cursor(x,y);
	uint16_t temp[3];
	LCD_WRITE(LCD_COMMADDRESS,0x2E00);
	LCD_READ(LCD_DATAADDRESS,temp[0]);
	LCD_READ(LCD_DATAADDRESS,temp[1]);
	LCD_READ(LCD_DATAADDRESS,temp[2]);

	*color=(((temp[1]&0xF800) << 8) | ((temp[1]&0x00FC) << 3) | ((temp[2]&0xF800) >> 11));
	return 0;
}

int lcd_fill_color(struct _LCD_DRIVE_ *drive,uint16_t x,uint16_t y,uint16_t width ,uint16_t height,uint16_t color){
    for(uint16_t i=0;i<height;++i){
        lcd_set_cursor(x,y+i);
        LCD_WRITE(LCD_COMMADDRESS,0x2C00);
        for(uint16_t j=0;j<width;++j){
            LCD_WRITE(LCD_DATAADDRESS,color);
        }
    }
    return 0;
}

int lcd_fill_data(struct _LCD_DRIVE_ *drive,uint16_t x,uint16_t y,uint16_t width ,uint16_t height,uint16_t *color){
    for(uint16_t i=0;i<height;++i){
        lcd_set_cursor(x,y+i);
        LCD_WRITE(LCD_COMMADDRESS,0x2C00);
        for(uint16_t j=0;j<width;++j){
            LCD_WRITE(LCD_DATAADDRESS,color[i*width+j]);
        }
    }
    return 0;
}

int lcd_show_ascii(struct _LCD_DRIVE_ *drive,uint16_t x,uint16_t y,char *str,uint16_t f_color,uint16_t b_color,sFONT font){
    char temp = ((*str)-' ');
    uint8_t num =0;
    uint8_t font_x = ((font.Width%8)?(font.Width/8)+1:font.Width);

    while(temp > 0 && temp < '~'){
        for(uint8_t i=0;i<font.Height;++i){
            lcd_set_cursor(x+num*font.Width,y+i);
            LCD_WRITE(LCD_COMMADDRESS,0x2C00);
            for(uint8_t j=0;j<font.Width;++j){
                LCD_WRITE(LCD_DATAADDRESS,(font.table[temp*font.Height*font_x+(j/8)+i*font_x]&(0x80>>(j%8)))?f_color:b_color);
            }
        }
        temp = ((*++str)-' ');
        num++;
    }
    return 0;
}

