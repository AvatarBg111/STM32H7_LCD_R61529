/*
 * ft5436.c
 *
 *  Created on: Oct 12, 2023
 *      Author: AvatarBg111
 */

#include "ft5436.h"
#include "stm32h7xx_hal.h"
#include "r61529.h"
#include "systick_timer.h"
#include<stdio.h>
#include<string.h>

// Extern variables
extern I2C_HandleTypeDef hi2c4;
extern uint8_t touch_detected;

// Variables
static uint16_t touchX[FT_REG_NUMTOUCHES] = {0}, touchY[FT_REG_NUMTOUCHES] = {0}, touchID[FT_REG_NUMTOUCHES] = {0};

void Init_FT5436(void){
	uint8_t initialization_arr[] = {0x00, 0x00, 0x88, 0x0E};
	uint8_t meta_arr[] = {0xA8, 0xA3, 0x80, 0x80};
	uint8_t panel_id = 0x00, chip_id = 0x00;
	uint8_t counter = 25;
	char screen_text[50] = {0};

	while(counter--){
		FT5436_RESET_ACTIVE;
		HAL_Delay(200);
		FT5436_RESET_IDLE;
		HAL_Delay(200);
		FT5436_RESET_IDLE;

		if(HAL_I2C_Master_Transmit(&hi2c4, 0x38 << 1, initialization_arr, 4, 10) == HAL_OK){
			break;
		}
	}

	// Get panel ID if FT module is a panel
	if(HAL_I2C_Master_Transmit(&hi2c4, 0x38 << 1, meta_arr, 1, 25) != HAL_OK){
		Error_Handler();
	}
	HAL_I2C_Master_Receive(&hi2c4, 0x38 << 1, &panel_id, 1, 20);

	// Get chip ID if FT is a chip
	if(HAL_I2C_Master_Transmit(&hi2c4, 0x38 << 1, meta_arr + 1, 1, 25) != HAL_OK){
		Error_Handler();
	}
	HAL_I2C_Master_Receive(&hi2c4, 0x38 << 1, &chip_id, 1, 20);

	// Format output string and draw it on screen
	sprintf(screen_text, "Panel Id: 0x%X  Chip Id: 0x%X", panel_id, chip_id);
	R61529_WriteString(5, 300, screen_text, Font_7x10, GREEN, BLACK);

	// Set touch detection threshold
	if(HAL_I2C_Master_Transmit(&hi2c4, 0x38 << 1, meta_arr + 2, 2, 25) != HAL_OK){
		Error_Handler();
	}
}

void touch_move(void){
	uint8_t meta_arr[] = {FT_REG_NUMTOUCHES, 0x00};
	uint8_t res_arr[4 + (FT_REG_NUMTOUCHES * 6) + 1] = {0};
	static uint8_t show_coordinates = 0x01;
	char coordinates_str[25] = {0};

	if(touch_detected){
		touch_detected = 0x00;

		if(HAL_I2C_Master_Transmit(&hi2c4, 0x38 << 1, meta_arr, 1, 30) != HAL_OK){
			Error_Handler();
		}
		HAL_I2C_Master_Receive(&hi2c4, 0x38 << 1, res_arr, 1, 5);

		if(res_arr[0] > FT_REG_NUMTOUCHES){
			return;
		}else{
			if(HAL_I2C_Master_Transmit(&hi2c4, 0x38 << 1, meta_arr + 1, 1, 30) != HAL_OK){
				Error_Handler();
			}
			HAL_I2C_Master_Receive(&hi2c4, 0x38 << 1, res_arr, 33, 10);

			if(!((res_arr[2] > FT_REG_NUMTOUCHES) || (res_arr[2] == 0))){
			    for(uint8_t i = 0; i < res_arr[2]; i++){
			        touchX[i] = res_arr[0x03 + i * 6] & 0x0F;
			        touchX[i] <<= 8;
			        touchX[i] |= res_arr[0x04 + i * 6];
			        touchY[i] = res_arr[0x05 + i * 6] & 0x0F;
			        touchY[i] <<= 8;
			        touchY[i] |= res_arr[0x06 + i * 6];
			        touchID[i] = res_arr[0x05 + i * 6] >> 4;
			    }

			    if(!show_coordinates && wait_ms_ch(1, 100)){
					show_coordinates = 0x01;
			    }
			    if(show_coordinates){
					show_coordinates = 0x00;

					for(uint8_t i = 0; i < FT_REG_NUMTOUCHES; i++){
						memset(coordinates_str, 0, 20);

						uint16_t color = 0x0000;
						if(i < res_arr[2]){
							sprintf(coordinates_str, "(x - %3d, y - %3d)", touchX[i], touchY[i]);
							color = RED;
						}else{
							sprintf(coordinates_str, "N/A");
							color = YELLOW;
						}
						R61529_WriteString(60, 30 + (i * 15), coordinates_str, Font_7x10, color, BLACK);
					}
			    }
			}
		}
	}
}
