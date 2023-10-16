#include "stm32h7xx_hal.h"
#include "R61529.h"
#include<stdio.h>
#include<string.h>

// static unsigned int const FONT16_INFO_1[] = {31,2};
//extern const short belk[];

uint16_t X_SIZE = 480;
uint16_t Y_SIZE = 320;

//  =====================================================================
/*
PD14   ------> FMC_D0
PD15   ------> FMC_D1
PD0   ------> FMC_D2
PD1   ------> FMC_D3
PE7   ------> FMC_D4
PE8   ------> FMC_D5
PE9   ------> FMC_D6
PE10   ------> FMC_D7
PE11   ------> FMC_D8
PE12   ------> FMC_D9
PE13   ------> FMC_D10
PE14   ------> FMC_D11
PE15   ------> FMC_D12
PD8   ------> FMC_D13
PD9   ------> FMC_D14
PD10   ------> FMC_D15
PD11   ------> FMC_A16
PC7   ------> FMC_NE1
PD4   ------> FMC_NOE
PD5   ------> FMC_NWE
*/

__inline void Lcd_Write(uint16_t index, uint8_t type){
	uint32_t MASK_GPIOD = 0x0;
	uint32_t MASK_GPIOE = 0x0;

	if(type == COMMAND){
		RS_ACTIVE;
	}else if(type == DATA){
		RS_IDLE;
	}

	//Chip select
	CS_ACTIVE;

	//Write enable
	WR_ACTIVE;


	//BIT 0
	if(index & 0x0001)
		MASK_GPIOD |= GPIO_PIN_14;
	else
		MASK_GPIOD |= (uint32_t)GPIO_PIN_14 << 16;

	//BIT 1
	if(index & 0x0002)
		MASK_GPIOD |= GPIO_PIN_15;
	else
		MASK_GPIOD |= (uint32_t)GPIO_PIN_15 << 16;

	//BIT 2
	if(index & 0x0004)
		MASK_GPIOD |= GPIO_PIN_0;
	else
		MASK_GPIOD |= (uint32_t)GPIO_PIN_0 << 16;

	//BIT 3
	if(index & 0x0008)
		MASK_GPIOD |= GPIO_PIN_1;
	else
		MASK_GPIOD |= (uint32_t)GPIO_PIN_1 << 16;

	//BIT 13
	if(index & 0x2000)
		MASK_GPIOD |= GPIO_PIN_8;
	else
		MASK_GPIOD |= (uint32_t)GPIO_PIN_8 << 16;

	//BIT 14
	if(index & 0x4000)
		MASK_GPIOD |= GPIO_PIN_9;
	else
		MASK_GPIOD |= (uint32_t)GPIO_PIN_9 << 16;

	//BIT 15
	if(index & 0x8000)
		MASK_GPIOD |= GPIO_PIN_10;
	else
		MASK_GPIOD |= (uint32_t)GPIO_PIN_10 << 16;

	//BITS 4 - 12
	for(uint8_t i = 7; i <= 15; i++){
		if(index & (1 << (i - 3))){
			MASK_GPIOE |= ((uint32_t)1 << i);
		}else{
			MASK_GPIOE |= ((uint32_t)1 << (i + 16));
		}
	}

	GPIOD->BSRR = MASK_GPIOD;
	GPIOE->BSRR = MASK_GPIOE;

	//delay
	for(int i = 0; i < 10; i++);

	//Chip un-select
	CS_IDLE;

	//Write disable
	WR_IDLE;
}

__inline void Lcd_Write_Reg(uint16_t index){
	//*(uint16_t *) (LCD_REG) = index;
	Lcd_Write(index, COMMAND);
}
__inline void Lcd_Write_Data(uint16_t data){
    //*(uint16_t *) (LCD_DATA)= data;
	Lcd_Write(data, DATA);
}

//  =====================================================================
void R61529_SetAddrWindow(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	Lcd_Write_Reg(0x2A);//Column Addres Set
	Lcd_Write_Data(x1 >> 8);
	Lcd_Write_Data(x1 & 0xFF);
	Lcd_Write_Data(x2 >> 8);
	Lcd_Write_Data(x2 & 0xFF);

	Lcd_Write_Reg(0x2B);//Page Addres Set
	Lcd_Write_Data(y1 >> 8);
	Lcd_Write_Data(y1 & 0xFF);
	Lcd_Write_Data(y2 >> 8);
	Lcd_Write_Data(y2 & 0xFF);
	Lcd_Write_Reg(0x2C);
}

//  =====================================================================
void Init_R61529(){
	RD_IDLE;
	RESET_IDLE;
	HAL_Delay(100);
	RESET_ACTIVE;
	HAL_Delay(800);
	RESET_IDLE;
	HAL_Delay(800);

	Lcd_Write_Reg(0x01); // reset
	HAL_Delay(20);
	// digitalWrite(PIN_CSX, LOW); //chip enable
	Lcd_Write_Reg(0x11); //exit sleep mode;
	HAL_Delay(200);
	Lcd_Write_Reg(0xB0); //manufacturer command access protect
	Lcd_Write_Data(0x04); //allow access to additional manufacturer's commands
	HAL_Delay(1);

	Lcd_Write_Reg(0xB3); //Frame Memory Access and Interface Setting
	Lcd_Write_Data(0x02); // reset start position of a window area address...
	Lcd_Write_Data(0x00); //TE pin is used. TE signal is output every frame.
	Lcd_Write_Data(0x00); // empty according to the datasheet - does nothing;
	Lcd_Write_Data(0x00); // convert 16/18 bits to 24bits data by writing zeroes to LSBs. Sets image data write/read format(?)
	Lcd_Write_Data(0x00);  // ???? (not needed?)
	HAL_Delay(1);

	Lcd_Write_Reg(0xB4); //Display Mode
	Lcd_Write_Data(0x00); //Uses internal oscillator
	HAL_Delay(1);

	/**/
	Lcd_Write_Reg(0xC0); // Panel Driving Setting;
	Lcd_Write_Data(0x03); // Output polarity is inverted. Left/right interchanging scan. Forward scan. BGR mode (depends on other settings). S960 ? S1 (depends)
	Lcd_Write_Data(0xDF); // Number of lines for driver to drive - 480.
	Lcd_Write_Data(0x40); // Scan start position - Gate1. (depend on other param);
	Lcd_Write_Data(0x10); // Dot inversion. Dot inversion in not-lit display area. If 0x13 - both will be set to 'column inversion'.
	Lcd_Write_Data(0x00); // settings for non-lit display area...
	Lcd_Write_Data(0x01); // 3 frame scan interval in non-display area...
	Lcd_Write_Data(0x00); // Source output level in retrace period...
	Lcd_Write_Data(0x55);//54 . Internal clock divider = 5 (low and high periods).

	Lcd_Write_Reg(0xC1); //Display Timing Setting for Normal Mode
	Lcd_Write_Data(0x07); // Clock devider = 12. 14MHz/12. Used by display circuit and step-up circuit.
	Lcd_Write_Data(0x27); // These bits set the number of clocks in 1 line period. 0x27 - 39 clocks.
	Lcd_Write_Data(0x08); // Number of back porch lines. 0x08 - 8 lines.
	Lcd_Write_Data(0x08); // Number of front porch lines. 0x08 - 8lines.
	Lcd_Write_Data(0x00); // Spacial configuriation mode 1 (?). 1 line inversion mode (?).

	Lcd_Write_Reg(0xC4); // Source/Gate Driving Timing Setting
	Lcd_Write_Data(0x57); // falling position (stop) of gate driver - 4 clocks... gate start position - 8 clocks...
	Lcd_Write_Data(0x00); // nothing to set up according to the datasheet
	Lcd_Write_Data(0x05); // Source precharge period (GND) - 5 clocks.
	Lcd_Write_Data(0x03); // source precharge period (VCI) - 3 clocks.

	Lcd_Write_Reg(0xC6); //DPI polarity control
	Lcd_Write_Data(0x04); // VSYNC -Active Low. HSYNC - Active Low. DE pin enable data write in when DE=1. Reads data on the rising edge of the PCLK signal.

	//----Gamma setting start-----
	Lcd_Write_Reg(0xC8);
	Lcd_Write_Data(0x03);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x1A);
	Lcd_Write_Data(0x24);
	Lcd_Write_Data(0x32);
	Lcd_Write_Data(0x4B);
	Lcd_Write_Data(0x3B);
	Lcd_Write_Data(0x29);
	Lcd_Write_Data(0x1F);
	Lcd_Write_Data(0x18);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x04);

	Lcd_Write_Data(0x03);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x1A);
	Lcd_Write_Data(0x24);
	Lcd_Write_Data(0x32);
	Lcd_Write_Data(0x4B);
	Lcd_Write_Data(0x3B);
	Lcd_Write_Data(0x29);
	Lcd_Write_Data(0x1F);
	Lcd_Write_Data(0x18);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x04);

	Lcd_Write_Reg(0xC9);
	Lcd_Write_Data(0x03);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x1A);
	Lcd_Write_Data(0x24);
	Lcd_Write_Data(0x32);
	Lcd_Write_Data(0x4B);
	Lcd_Write_Data(0x3B);
	Lcd_Write_Data(0x29);
	Lcd_Write_Data(0x1F);
	Lcd_Write_Data(0x18);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x04);

	Lcd_Write_Data(0x03);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x1A);
	Lcd_Write_Data(0x24);
	Lcd_Write_Data(0x32);
	Lcd_Write_Data(0x4B);
	Lcd_Write_Data(0x3B);
	Lcd_Write_Data(0x29);
	Lcd_Write_Data(0x1F);
	Lcd_Write_Data(0x18);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x04);

	Lcd_Write_Reg(0xCA);
	Lcd_Write_Data(0x03);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x1A);
	Lcd_Write_Data(0x24);
	Lcd_Write_Data(0x32);
	Lcd_Write_Data(0x4B);
	Lcd_Write_Data(0x3B);
	Lcd_Write_Data(0x29);
	Lcd_Write_Data(0x1F);
	Lcd_Write_Data(0x18);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x04);

	Lcd_Write_Data(0x03);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x1A);
	Lcd_Write_Data(0x24);
	Lcd_Write_Data(0x32);
	Lcd_Write_Data(0x4B);
	Lcd_Write_Data(0x3B);
	Lcd_Write_Data(0x29);
	Lcd_Write_Data(0x1F);
	Lcd_Write_Data(0x18);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x04);
	/**/
	//---Gamma setting end--------

	/**/
	Lcd_Write_Reg(0xD0); // Power (charge pump) settings
	Lcd_Write_Data(0x99);//DC4~1//A5. Set up clock cycle of the internal step up controller.
	Lcd_Write_Data(0x06);//BT // Set Voltage step up factor.
	Lcd_Write_Data(0x08);// default according to the datasheet - does nothing.
	Lcd_Write_Data(0x20);// VCN step up cycles.
	Lcd_Write_Data(0x29);//VC1, VC2// VCI3 voltage = 2.70V;  VCI2 voltage = 3.8V.
	Lcd_Write_Data(0x04);// default
	Lcd_Write_Data(0x01);// default
	Lcd_Write_Data(0x00);// default
	Lcd_Write_Data(0x08);// default
	Lcd_Write_Data(0x01);// default
	Lcd_Write_Data(0x00);// default
	Lcd_Write_Data(0x06);// default
	Lcd_Write_Data(0x01);// default
	Lcd_Write_Data(0x00);// default
	Lcd_Write_Data(0x00);// default
	Lcd_Write_Data(0x20);// default
	/**/

	Lcd_Write_Reg(0xD1);//VCOM setting
	Lcd_Write_Data(0x00);//disable write to VDC[7:0].
	Lcd_Write_Data(0x20);//45 38 VPLVL// voltage of ? correction registers for positive polarity
	Lcd_Write_Data(0x20);//45 38 VNLVL// voltage of ? correction registers for negative polarity
	Lcd_Write_Data(0x15);//32 2A VCOMDC// VNLVL x 0.063

	Lcd_Write_Reg(0xE0);//NVM Access Control
	Lcd_Write_Data(0x00);//NVM access is disabled
	Lcd_Write_Data(0x00);//Erase operation (disabled).
	Lcd_Write_Data(0x00);//TE pin works as tearing effect pin.
	// should be one more Lcd_Write_Data(0x00); according to the datasheet.

	Lcd_Write_Reg(0xE1); //set_DDB_write_control
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);

	Lcd_Write_Reg(0xE2); //NVM Load Control
	Lcd_Write_Data(0x00); // does not execute data load from the NVM to each command

	Lcd_Write_Reg(0x36); //set_address_mode
	Lcd_Write_Data(0x20|0x00|0x01); // data is not flipped in any way?

	Lcd_Write_Reg(0x3A); // set_pixel_format
	Lcd_Write_Data(0x55);// 16-Bit/pixel = 55h, 24-bit/pixel = 77h

	Lcd_Write_Reg(0x2A); //set_column_address
	Lcd_Write_Data(0x00); // starts from 0th frame buffer address
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x01);
	Lcd_Write_Data(0xDF);//320 - uses all columns

	Lcd_Write_Reg(0x2B); //set_page_address
	Lcd_Write_Data(0x00); // starts from 0th frame buffer address
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x01);
	Lcd_Write_Data(0x3F);//480 - uses all lines in the frame buffer

	Lcd_Write_Reg(0x29); //set_display_on - This command causes the display module to start displaying the image data on the display device.
	HAL_Delay(20);
}

//  =====================================================================
void R61529_SetRotation(unsigned char r){
	Lcd_Write_Reg(0x36);

	switch(r){
		case ROTATE_DEFAULT:
			Lcd_Write_Data(0x21);
			X_SIZE = 480;
			Y_SIZE = 320;
			break;
		case ROTATE_RIGHT:
			Lcd_Write_Data(0x00);
			X_SIZE = 320;
			Y_SIZE = 480;
			break;
		case ROTATE_UPSIDE_DOWN:
			Lcd_Write_Data(0x60);
			X_SIZE = 480;
			Y_SIZE = 320;
			break;
		case ROTATE_LEFT:
			Lcd_Write_Data(0xC0);
			X_SIZE = 320;
			Y_SIZE = 480;
			break;
	}
}

void R61529_Flood(uint16_t color,uint32_t len){
	uint16_t blocks;
	uint8_t i;

	//HAL_SRAM_Write_16b(&hsram1, (uint32_t*)(LCD_REG), &reg1, 1);
	//HAL_SRAM_Write_16b(&hsram1, (uint32_t*)(LCD_DATA), color_arr, 1);
	Lcd_Write_Reg(0x2C);
	Lcd_Write_Data (color);

	len--;
	blocks=(uint16_t)(len/64);//64pixels/blocks
	while(blocks--)
	{
		i=16;
		do
		{
			//HAL_SRAM_Write_16b(&hsram1, (uint32_t*)(LCD_DATA), color_arr, 5);
			Lcd_Write_Data (color);
			Lcd_Write_Data (color);
			Lcd_Write_Data (color);
			Lcd_Write_Data (color);
			Lcd_Write_Data (color);
		}while(--i);
	}
	//Fill any remaining pixels (1 to 64);
	for(i=(uint8_t)len&63;i--;){
		Lcd_Write_Data(color);
	}
	//HAL_SRAM_Write_16b(&hsram1, (uint32_t*)(LCD_DATA), color_arr, len & 63);
}
// =====================================================================
void R61529_FillScreen(uint16_t color)
{
        R61529_SetAddrWindow(0,0,X_SIZE-1,Y_SIZE-1); //
        R61529_Flood(color,(long)X_SIZE*(long)Y_SIZE);
}
// =====================================================================
void R61529_FillRect(uint16_t color, uint16_t x1, uint16_t y1,
											uint16_t x2, uint16_t y2)
{
	if(x2==480) x2=479;
	if(y2==320) y2=319;
	R61529_SetAddrWindow(x1,y1,x2,y2);
	R61529_Flood(color,(uint16_t)(x2-x1+1)*(uint16_t)(y2-y1+1));
}
// =====================================================================
void R61529_DrawPixel(int x, int y, uint16_t color)
{
	if((x<0)||(y<0)||(x>=X_SIZE)||(y>=Y_SIZE)) return;
	R61529_SetAddrWindow(x,y,x,y);
	Lcd_Write_Reg(0x2C);
	Lcd_Write_Data(color);

	//HAL_SRAM_Write_16b(&hsram1, (uint32_t*)(LCD_DATA), &color, 1);
}
// =====================================================================
void R61529_DrawLine(uint16_t color, uint16_t x1, uint16_t y1,
											uint16_t x2, uint16_t y2)
{
	int steep = abs(y2-y1)>abs(x2-x1);
	if(steep)
	{
		swap(x1,y1);
		swap(x2,y2);
	}
	if(x1>x2)
	{
		swap(x1,x2);
		swap(y1,y2);
	}
	int dx,dy;
	dx=x2-x1;
	dy=abs(y2-y1);
	int err=dx/2;
	int ystep;
	if(y1<y2) ystep=1;
	else ystep=-1;
	for(;x1<=x2;x1++)
	{
		if(steep) R61529_DrawPixel(y1,x1,color);
		else R61529_DrawPixel(x1,y1,color);
		err-=dy;
		if(err<0)
		{
			y1 += ystep;
			err=dx;
		}
	}
}

// =====================================================================
void R61529_DrawRect(uint16_t color, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2)
{
	R61529_DrawLine(color,x1,y1,x2,y1);
	R61529_DrawLine(color,x2,y1,x2,y2);
	R61529_DrawLine(color,x1,y1,x1,y2);
	R61529_DrawLine(color,x1,y2,x2,y2);
}

// =====================================================================
void R61529_DrawCircle(uint16_t x0, uint16_t y0, int r, uint16_t color)
{
	int f = 1-r;
	int ddF_x=1;
	int ddF_y=-2*r;
	int x = 0;
	int y = r;
	R61529_DrawPixel(x0,y0+r,color);
	R61529_DrawPixel(x0,y0-r,color);
	R61529_DrawPixel(x0+r,y0,color);
	R61529_DrawPixel(x0-r,y0,color);
	while (x<y)
	{
		if (f>=0)
		{
			y--;
			ddF_y+=2;
			f+=ddF_y;
		}
		x++;
		ddF_x+=2;
		f+=ddF_x;
		R61529_DrawPixel(x0+x,y0+y,color);
		R61529_DrawPixel(x0-x,y0+y,color);
		R61529_DrawPixel(x0+x,y0-y,color);
		R61529_DrawPixel(x0-x,y0-y,color);
		R61529_DrawPixel(x0+y,y0+x,color);
		R61529_DrawPixel(x0-y,y0+x,color);
		R61529_DrawPixel(x0+y,y0-x,color);
		R61529_DrawPixel(x0-y,y0-x,color);
	}
}

static void R61529_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor){
    uint32_t i, b, j;

    R61529_SetAddrWindow(x, y, x+font.width-1, y+font.height-1);

    for(i = 0; i < font.height; i++){
        b = font.data[(ch - 32) * font.height + i];
        for(j = 0; j < font.width; j++){
            if((b << j) & 0x8000){
                Lcd_Write_Data(color);
            }else{
                Lcd_Write_Data(bgcolor);
            }
        }
    }
}

void R61529_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor){
    while(*str){
        if(x + font.width >= X_SIZE){
            x = 0;
            y += font.height;

            if(y + font.height >= Y_SIZE){
                break;
            }

            if(*str == ' '){
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        R61529_WriteChar(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }
}

void Draw_Image(const short *image_array, uint16_t x_coordinat, uint16_t y_coordinat, uint16_t img_width, uint16_t img_height, uint32_t s_img){
	R61529_SetAddrWindow(x_coordinat, y_coordinat, img_width + x_coordinat - 1, img_height + y_coordinat - 1);

	for(uint32_t i = 0; i < s_img; i++){
		Lcd_Write_Data(image_array[i]);
	}
}

// =====================================================================
void R61529_rotation_test(){
	char text[20] = {0};
	R61529_FillScreen(BLACK);

	for(uint8_t i = 0; i < 4; i++){
		sprintf(text, "Hello, World! Rt: %d", i + 1);
		R61529_SetRotation(i);
		R61529_WriteString(10, 10, text, Font_11x18, GREEN, BLACK);
		HAL_Delay(750);
		R61529_FillRect(BLACK, 10, 10, X_SIZE, 10 + 20);
		memset(text, 0, 19);
	}

	R61529_SetRotation(ROTATE_DEFAULT);
}


