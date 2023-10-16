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

#define ROTATE_DEFAULT 0
#define ROTATE_RIGHT 1
#define ROTATE_UPSIDE_DOWN 2
#define ROTATE_LEFT 3

void Lcd_Write(uint16_t, uint8_t);
void Lcd_Write_Reg(uint16_t index);
void Lcd_Write_Data(uint16_t data);

void Init_R61529(void);
void R61529_SetRotation(unsigned char r);
void R61529_Flood(uint16_t color, uint32_t len);
void R61529_FillScreen(uint16_t color);

void R61529_FillRect(uint16_t color, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2);
void R61529_DrawCircle(uint16_t x0, uint16_t y0, int r, uint16_t color);
void R61529_DrawPixel(int x, int y, uint16_t color);
void R61529_DrawLine(uint16_t color, uint16_t x1, uint16_t y1,	uint16_t x2, uint16_t y2);
void R61529_DrawRect(uint16_t color, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2);
void R61529_Char_Universale(int x0, int y0, unsigned int color, unsigned int phone, unsigned char charcode);
void R61529_String_to_Char_universale(int x0, int y0, unsigned int color, unsigned int phone, char *string);
void Draw_Image(const short *image_array, uint16_t x_coordinat, uint16_t y_coordinat, uint16_t img_width, uint16_t img_height, uint32_t s_img);
void R61529_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);

void R61529_rotation_test(void);

#endif
