/*
 * XPT2046_touch.c
 *
 *  Created on: 20 sep. 2019 ð.
 *      Author: Andriy Honcharenko
 */

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "ILI9341_Driver.h"
#include <XPT2046_touch.h>
#include "stm32f3xx_hal_dma.h"
#include "stm32f3xx_hal.h"


unsigned int m_XPanel;
unsigned int m_YPanel;

#define XPT2046_SPI_PORT hspi3
// change depending on screen orientation
#define XPT2046_SCALE_X 320
#define XPT2046_SCALE_Y 240

// to calibrate uncomment UART_Printf line in ili9341_touch.c
#define XPT2046_MIN_RAW_X 3001
#define XPT2046_MAX_RAW_X 30943
#define XPT2046_MIN_RAW_Y 1769
#define XPT2046_MAX_RAW_Y 30371

#define TP_READ_X 0x90
#define TP_READ_Y 0xD0

void XPT2046_TouchGetCoordinates();

void XPT2046_TouchGetCoordinates()
{
	uint8_t calibrar=0;

    static const uint8_t cmd_read_x[] = { TP_READ_X };
    static const uint8_t cmd_read_y[] = { TP_READ_Y };
    static const uint8_t zeroes_tx[] = { 0x00, 0x00 };

    MODIFY_REG(hspi3.Instance->CR1, SPI_BAUDRATEPRESCALER_256, SPI_BAUDRATEPRESCALER_256);

    char str[30];

    SD_CS_OFF;
   	TFT_CS_OFF;
   	TP_CS_ON;

    uint32_t avg_x = 0;
    uint32_t avg_y = 0;
    uint8_t nsamples = 0;

    for(uint8_t i = 0; i < 16; i++)
    {
        if(HAL_GPIO_ReadPin(TP_IRQ_GPIO_Port, TP_IRQ_Pin)!=0  )
            break;

        nsamples++;

        HAL_SPI_Transmit(&XPT2046_SPI_PORT, (uint8_t*)cmd_read_y, sizeof(cmd_read_y), HAL_MAX_DELAY);
        uint8_t y_raw[2];
        HAL_SPI_TransmitReceive(&XPT2046_SPI_PORT, (uint8_t*)zeroes_tx, y_raw, sizeof(y_raw), HAL_MAX_DELAY);

        HAL_SPI_Transmit(&XPT2046_SPI_PORT, (uint8_t*)cmd_read_x, sizeof(cmd_read_x), HAL_MAX_DELAY);
        uint8_t x_raw[2];
        HAL_SPI_TransmitReceive(&XPT2046_SPI_PORT, (uint8_t*)zeroes_tx, x_raw, sizeof(x_raw), HAL_MAX_DELAY);

        avg_x += (((uint16_t)x_raw[0]) << 8) | ((uint16_t)x_raw[1]);
        avg_y += (((uint16_t)y_raw[0]) << 8) | ((uint16_t)y_raw[1]);
    }

   	TP_CS_OFF;
   	MODIFY_REG(hspi3.Instance->CR1, SPI_BAUDRATEPRESCALER_256, SPI_BAUDRATEPRESCALER_2);

    if(nsamples==16)
    {
    	uint32_t raw_x = (avg_x / 16);
    	uint32_t raw_y = (avg_y / 16);

    	if(calibrar==1)
    	{
    		TFT_CS_ON;
    		FillReg(0,0,320,240,0xFC00);
    		sprintf(str,"CRUDO");
    		ILI9341_Draw_String(10,10, 0, 0xFC00,str,4);
    		sprintf(str,"x=%d y=%d",(int)raw_x,(int)raw_y);
    		ILI9341_Draw_String(10,40, 0, 0xFC00,str,4);
    	}
    	if(raw_x < XPT2046_MIN_RAW_X) raw_x = XPT2046_MIN_RAW_X;
    	if(raw_x > XPT2046_MAX_RAW_X) raw_x = XPT2046_MAX_RAW_X;

    	if(raw_y < XPT2046_MIN_RAW_Y) raw_y = XPT2046_MIN_RAW_Y;
    	if(raw_y > XPT2046_MAX_RAW_Y) raw_y = XPT2046_MAX_RAW_Y;

    	m_XPanel = (raw_x - XPT2046_MIN_RAW_X) * XPT2046_SCALE_X / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
    	m_YPanel = (raw_y - XPT2046_MIN_RAW_Y) * XPT2046_SCALE_Y / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);

    	if(calibrar==1)
    	{
    		sprintf(str,"COCIDO");
    		ILI9341_Draw_String(10,100, 0,0xFC00,str,4);
    		sprintf(str,"x=%d y=%d",m_XPanel,m_YPanel);
    		ILI9341_Draw_String(10,140, 0,0xFC00,str,4);
    	}
    }
    else m_XPanel=0,m_YPanel=0;
}
