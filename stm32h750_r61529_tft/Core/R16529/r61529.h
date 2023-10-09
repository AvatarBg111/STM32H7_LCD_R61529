#ifndef _SSD1289_H_
#define _SSD1289_H_

#include "main.h"
#include "stm32h7xx_hal.h"
#include "fonts.h"
#include "stdlib.h"
//#include "string.h"

/*
PD11   ------> R61529_RS
PC7   ------> R61529_CS
PD4   ------> R61529_RD
PD5   ------> R61529_WR
PB13   ------> R61529_RESET
*/
#define  RESET_ACTIVE   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
#define  RESET_IDLE     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
#define  RS_ACTIVE   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
#define  RS_IDLE     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
#define  CS_ACTIVE   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
#define  CS_IDLE     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
#define  RD_ACTIVE   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
#define  RD_IDLE     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
#define  WR_ACTIVE   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
#define  WR_IDLE     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);

#define COMMAND 0
#define DATA 1

#define swap(a,b) {int16_t t=a;a=b;b=t;}

// Определяем адреса
// Для записи данных
#define LCD_DATA    			    0x60080000
// Для записи команд
#define LCD_REG   		  	    0x60000000

#define  BLACK   0x0000
#define  BLUE    0x001F
#define  RED     0xF800
#define  GREEN   0x07E0
#define  CYAN    0x07FF
#define  MAGENTA 0xF81F
#define  YELLOW  0xFFE0
#define  WHITE   0xFFFF


extern const unsigned char simbols [];
void Lcd_Write(uint16_t, uint8_t);
void Lcd_Write_Reg(uint16_t index);
void Lcd_Write_Data(uint16_t data);
//void TFT_FillColor(const uint16_t color);
void Init_Disp(void);
void Init_Disp2(void);
void init_ili9481(void);
void ili9481_SetRotation(unsigned char r);
//void TFT_FillRect(const uint16_t color);
void ili9481_Flood(uint16_t color, uint32_t len) ;
void ili9481_FillScreen(uint16_t color) ;
void ili9481_FillRect(uint16_t color, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2);
void ili9481_DrawCircle(uint16_t x0, uint16_t y0, int r, uint16_t color);
void ili9481_DrawPixel(int x, int y, uint16_t color);
void ili9481_DrawLine(uint16_t color, uint16_t x1, uint16_t y1,	uint16_t x2, uint16_t y2);
void ili9481_DrawRect(uint16_t color, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2);
void ili9481_Char_Universale(int x0, int y0, unsigned int color, unsigned int phone, unsigned char charcode);
void ili9481_String_to_Char_universale(int x0, int y0, unsigned int color, unsigned int phone, char *string);
void Draw_Image(const short *image_array, uint16_t x_coordinat, uint16_t y_coordinat, uint16_t img_width, uint16_t img_height, uint32_t s_img);
void r61529_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);

//void  Draw_Image(const short *image_array, uint16_t x_coordinat, uint16_t y_coordinat, uint16_t img_width, uint16_t img_height, uint32_t s_img);

////void Lcd_Write_Reg(uint16_t lcd_reg, uint16_t lcd_data);
//uint16_t Lcd_Read_Reg(uint16_t reg_addr);

//void Set_Cursor(uint16_t x_kur, uint16_t y_kur);
//void Lcd_Clear(uint16_t color);
////void Get_Lcd_Id(uint16_t x, uint16_t y,uint16_t color, uint16_t phone, uint8_t size);
//void Init_Disp(void);
//void Init_Periph(void);

//void Draw_Simbol(uint16_t x, uint16_t y, uint16_t color, uint16_t phone, const uint8_t *sim, uint8_t size);
//void Draw_String (uint16_t start_x,uint16_t start_y,uint16_t color,uint16_t phone,uint8_t size ,const uint8_t *string);
//void Draw_Point (uint16_t x,uint16_t y,uint16_t color,uint8_t size);
//void Draw_Line (uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color,uint8_t size);
//void Draw_Horizont_Line(uint16_t x1,uint16_t y1,uint16_t y2,uint16_t color,uint8_t size);
//void Draw_Vertical_Line(uint16_t x1,uint16_t x2,uint16_t y1,uint16_t color, uint8_t size);
//void Draw_Reactangle(uint16_t left,uint16_t top,uint16_t right,uint16_t bottom,uint16_t color, uint8_t size);
//void Draw_Triangle( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t size, uint16_t color);
////void Draw_Circle (uint16_t x0,uint16_t y0,uint16_t radius,uint16_t color, uint8_t size);
//void Set_Work_Area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
//void Draw_Area(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, uint16_t color);

//void Draw_Circle1(unsigned int x,unsigned int y,char radius,char fill, char size, unsigned int color);
//void Draw_Picture_Hor(uint16_t start_x,uint16_t start_y,uint16_t end_x,uint16_t end_y,const uint16_t *pic);

//void delay(uint32_t delayTime);



#endif
