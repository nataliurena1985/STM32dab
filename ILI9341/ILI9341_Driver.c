#include "ff.h"
#include "characters.h"
#include "stm32f3xx_hal_dma.h"
#include "stm32f3xx_hal.h"
#include "ILI9341_Driver.h"
#include "Recursos.h"


static void ILI9341_SPI_Init(void);
static void Before_Sending_Data(void);
static void Before_Sending_Command(void);
static void ILI9341_Send_Data(unsigned char data);
static void ILI9341_Send_Command(unsigned char command);


void ILI9341_Send_Burst(unsigned short color, unsigned long len);
//static void ILI9341_Draw_Double_Pixel(int x, int y, unsigned int color1, unsigned int color2);
void ILI9341_Draw_Char(int x, int y, unsigned int color, unsigned int phone, unsigned char charcode, unsigned char size);
void ILI9341_Draw_String(unsigned int x, unsigned int y, unsigned int color, unsigned int phone, char* str, unsigned char size);

static unsigned int X_SIZE = 240;
static unsigned int Y_SIZE = 320;
unsigned char m_rotacion;

unsigned char hh;

SPI_HandleTypeDef lcd_spi;

void ILI9341_Init()
{

	SD_CS_OFF;
	TFT_CS_ON;

	ILI9341_SPI_Init();
	/* Reset The Screen */
	ILI9341_Reset();
	ILI9341_Send_Command(0x01);

	/* Power Control A */
	ILI9341_Send_Command(0xCB);
	ILI9341_Send_Data(0x39);
	ILI9341_Send_Data(0x2C);
	ILI9341_Send_Data(0x00);
	ILI9341_Send_Data(0x34);
	ILI9341_Send_Data(0x02);

	/* Power Control B */
	ILI9341_Send_Command(0xCF);
	ILI9341_Send_Data(0x00);
	ILI9341_Send_Data(0xC1);
	ILI9341_Send_Data(0x30);

	/* Driver timing control A */
	ILI9341_Send_Command(0xE8);
	ILI9341_Send_Data(0x85);
	ILI9341_Send_Data(0x00);
	ILI9341_Send_Data(0x78);

	/* Driver timing control B */
	ILI9341_Send_Command(0xEA);
	ILI9341_Send_Data(0x00);
	ILI9341_Send_Data(0x00);

	/* Power on Sequence control */
	ILI9341_Send_Command(0xED);
	ILI9341_Send_Data(0x64);
	ILI9341_Send_Data(0x03);
	ILI9341_Send_Data(0x12);
	ILI9341_Send_Data(0x81);

	/* Pump ratio control */
	ILI9341_Send_Command(0xF7);
	ILI9341_Send_Data(0x20);

	/* Power Control 1 */
	ILI9341_Send_Command(0xC0);
	ILI9341_Send_Data(0x10);

	/* Power Control 2 */
	ILI9341_Send_Command(0xC1);
	ILI9341_Send_Data(0x10);

	/* VCOM Control 1 */
	ILI9341_Send_Command(0xC5);
	ILI9341_Send_Data(0x3E);
	ILI9341_Send_Data(0x28);

	/* VCOM Control 2 */
	ILI9341_Send_Command(0xC7);
	ILI9341_Send_Data(0x86);

	/* VCOM Control 2 */
	ILI9341_Send_Command(0x36);
	ILI9341_Send_Data(0x48);

	/* Pixel Format Set */
	ILI9341_Send_Command(0x3A);
	ILI9341_Send_Data(0x55);    //16bit

	ILI9341_Send_Command(0xB1);
	ILI9341_Send_Data(0x00);
	ILI9341_Send_Data(0x18);

	/* Display Function Control */
	ILI9341_Send_Command(0xB6);
	ILI9341_Send_Data(0x08);
	ILI9341_Send_Data(0x82);
	ILI9341_Send_Data(0x27);

	/* 3GAMMA FUNCTION DISABLE */
	ILI9341_Send_Command(0xF2);
	ILI9341_Send_Data(0x00);

	/* GAMMA CURVE SELECTED */
	ILI9341_Send_Command(0x26); //Gamma set
	ILI9341_Send_Data(0x01); 	//Gamma Curve (G2.2)

	//Positive Gamma  Correction
	ILI9341_Send_Command(0xE0);
	ILI9341_Send_Data(0x0F);
	ILI9341_Send_Data(0x31);
	ILI9341_Send_Data(0x2B);
	ILI9341_Send_Data(0x0C);
	ILI9341_Send_Data(0x0E);
	ILI9341_Send_Data(0x08);
	ILI9341_Send_Data(0x4E);
	ILI9341_Send_Data(0xF1);
	ILI9341_Send_Data(0x37);
	ILI9341_Send_Data(0x07);
	ILI9341_Send_Data(0x10);
	ILI9341_Send_Data(0x03);
	ILI9341_Send_Data(0x0E);
	ILI9341_Send_Data(0x09);
	ILI9341_Send_Data(0x00);

	//Negative Gamma  Correction
	ILI9341_Send_Command(0xE1);
	ILI9341_Send_Data(0x00);
	ILI9341_Send_Data(0x0E);
	ILI9341_Send_Data(0x14);
	ILI9341_Send_Data(0x03);
	ILI9341_Send_Data(0x11);
	ILI9341_Send_Data(0x07);
	ILI9341_Send_Data(0x31);
	ILI9341_Send_Data(0xC1);
	ILI9341_Send_Data(0x48);
	ILI9341_Send_Data(0x08);
	ILI9341_Send_Data(0x0F);
	ILI9341_Send_Data(0x0C);
	ILI9341_Send_Data(0x31);
	ILI9341_Send_Data(0x36);
	ILI9341_Send_Data(0x0F);

	//EXIT SLEEP
	ILI9341_Send_Command(0x11);

	//TURN ON DISPLAY
	ILI9341_Send_Command(0x29);
	ILI9341_Send_Data(0x2C);
}

/**
 * @brief SPI3 Initialization Function
 * @param None
 * @retval None
 */
static void ILI9341_SPI_Init()
{
	lcd_spi.Instance 				= SPI3;
	lcd_spi.Init.Mode 				= SPI_MODE_MASTER;
	lcd_spi.Init.Direction 			= SPI_DIRECTION_2LINES;
	lcd_spi.Init.DataSize 			= SPI_DATASIZE_8BIT;
	lcd_spi.Init.CLKPolarity 		= SPI_POLARITY_LOW;
	lcd_spi.Init.CLKPhase 			= SPI_PHASE_1EDGE;
	lcd_spi.Init.NSS 				= SPI_NSS_SOFT;
	lcd_spi.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_2;
	lcd_spi.Init.FirstBit 			= SPI_FIRSTBIT_MSB;
	lcd_spi.Init.TIMode 			= SPI_TIMODE_DISABLE;
	lcd_spi.Init.CRCCalculation 	= SPI_CRCCALCULATION_DISABLE;
	lcd_spi.Init.CRCPolynomial 		= 10;
	if (HAL_SPI_Init(&lcd_spi) != HAL_OK)
	{
		Error_Handler();
	}
}

void ILI9341_SPI_Send(unsigned char data)
{
	HAL_SPI_Transmit(&lcd_spi, &data, 1, 1);
}

void ILI9341_SPI_Send_Multiple(unsigned char data, int size)
{
	SD_CS_OFF;
	TFT_CS_ON;
	HAL_SPI_Transmit(&lcd_spi, &data, size, 10);
}

void ILI9341_Reset()
{
	SD_CS_OFF;

	TFT_RESET_ON;
	HAL_Delay(50);
	TFT_RESET_OFF;
	TFT_CS_ON;
	HAL_Delay(50);
	ILI9341_Send_Command(0x01);
	TFT_CS_OFF;
}

void ILI9341_Set_Rotation(unsigned char rotation)
{
	SD_CS_OFF;
	TFT_CS_ON;
	m_rotacion=rotation;
	ILI9341_Send_Command(0x36);
	switch (rotation)
	{
	case 0:
	//	ILI9341_Send_Data(0x048);
		ILI9341_Send_Data(0x088);
		X_SIZE = 240;
		Y_SIZE = 320;
		break;
	case 1:
		ILI9341_Send_Data(0x28);
		X_SIZE = 320;
		Y_SIZE = 240;
		break;
	case 2:
		ILI9341_Send_Data(0x88);
		X_SIZE = 240;
		Y_SIZE = 320;
		break;
	case 3:
		ILI9341_Send_Data(0xE8);
		X_SIZE = 320;
		Y_SIZE = 240;
		break;
	case 4:
		ILI9341_Send_Data(0xB8);
		X_SIZE = 320;
		Y_SIZE = 240;
		break;
	case 5:
		ILI9341_Send_Data(0x6c);
		X_SIZE = 320;
		Y_SIZE = 240;
		break;
	case 6:
		ILI9341_Send_Data(0x08);
		X_SIZE = 240;
		Y_SIZE = 320;
		break;
	}
}

void ILI9341_SPI_Send_32(unsigned char command, unsigned long data)
{
	SD_CS_OFF;
	TFT_CS_ON;

	TFT_DC_COMMAND;
	ILI9341_SPI_Send(command);

	TFT_DC_DATA;
	ILI9341_SPI_Send(data >> 24);
	ILI9341_SPI_Send(data >> 16);
	ILI9341_SPI_Send(data >> 8);
	ILI9341_SPI_Send(data);
}

void ILI9341_Set_Address(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	unsigned char data[3];

	SD_CS_OFF;
	TFT_CS_ON;

	TFT_DC_COMMAND;
	data[0]=0x2A;
	HAL_SPI_Transmit(&lcd_spi, data,1, 5);

	TFT_DC_DATA;
	data[0]=x1>>8;
	data[1]=x1;
	data[2]=x2>>8;
	data[3]=x2;
	HAL_SPI_Transmit(&lcd_spi,data,4,5);

	TFT_DC_COMMAND;
	data[0]=0x2B;
	HAL_SPI_Transmit(&lcd_spi,data,1,1);

	TFT_DC_DATA;
	data[0]=y1>>8;
	data[1]=y1;
	data[2]=y2>>8;
	data[3]=y2;
	HAL_SPI_Transmit(&lcd_spi,data,4,5);
}
/*
void ILI9341_Fill_Screen(unsigned int color)
{
	ILI9341_Set_Address(0,0,X_SIZE-1,Y_SIZE-1);
	ILI9341_Send_Burst(color, (long)X_SIZE * (long)Y_SIZE);
}
*/
void ILI9341_Draw_Pixel(int x, int y, unsigned int color)
{
	SD_CS_OFF;
	TFT_CS_ON;

	y=239-y;
	ILI9341_Set_Address(x,y,X_SIZE-1,Y_SIZE-1);
	TFT_DC_COMMAND;
	ILI9341_SPI_Send(0x2C);
	TFT_DC_DATA;
	ILI9341_SPI_Send(color>>8);
	ILI9341_SPI_Send(color);
}

void ILI9341_Draw_Empty_Rectangle(unsigned int color,unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	SD_CS_OFF;
	TFT_CS_ON;

	ILI9341_Draw_Line(color, x1, y1, x2, y1);
	ILI9341_Draw_Line(color, x2, y1, x2, y2);
	ILI9341_Draw_Line(color, x1, y1, x1, y2);
	ILI9341_Draw_Line(color, x1, y2, x2, y2);
}

void ILI9341_Draw_Line(unsigned int color, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	SD_CS_OFF;
	TFT_CS_ON;

	int steep = abs(y2-y1) > abs(x2-x1);

	if (steep)
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
	dx = (x2 - x1);
	dy = abs(y2 - y1);
	int err = dx / 2;
	int ystep;
	if(y1 < y2)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}
	for (; x1 <= x2; x1++)
	{
		if (steep)
		{
			ILI9341_Draw_Pixel(y1, x1, color);
		}
		else
		{
			ILI9341_Draw_Pixel(x1, y1, color);
		}
		err -= dy;
		if (err < 0)
		{
			y1 += ystep;
			err = dx;
		}
	}
}

void ILI9341_Draw_Circle(unsigned int x0, unsigned int y0, int r, unsigned int color, unsigned char flood)
{
	SD_CS_OFF;
	TFT_CS_ON;

	y0=240-y0;

	int f = 1 - r;
	int ddF_x = 1;
	int ddF_y = -2 * r;
	int x = 0;
	int y = r;
	if (flood == 0)
	{
		ILI9341_Draw_Pixel(x0, y0 + r, color);
		ILI9341_Draw_Pixel(x0, y0 - r, color);
		ILI9341_Draw_Pixel(x0 + r, y0, color);
		ILI9341_Draw_Pixel(x0 - r, y0, color);
		while (x < y) {
			if (f >= 0) {
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			x++;
			ddF_x += 2;
			f += ddF_x;
			ILI9341_Draw_Pixel(x0 + x, y0 + y, color);
			ILI9341_Draw_Pixel(x0 - x, y0 + y, color);
			ILI9341_Draw_Pixel(x0 + x, y0 - y, color);
			ILI9341_Draw_Pixel(x0 - x, y0 - y, color);
			ILI9341_Draw_Pixel(x0 + y, y0 + x, color);
			ILI9341_Draw_Pixel(x0 - y, y0 + x, color);
			ILI9341_Draw_Pixel(x0 + y, y0 - x, color);
			ILI9341_Draw_Pixel(x0 - y, y0 - x, color);
		}
	}
	else
	{
		ILI9341_Draw_Pixel(x0, y0 + r, color);
		ILI9341_Draw_Pixel(x0, y0 - r, color);
		ILI9341_Set_Address(x0 - r, y0, x0 + r, y0);
		TFT_DC_COMMAND;
		ILI9341_SPI_Send(0x2C);
		TFT_DC_DATA;
		for (uint32_t fff = 0; fff < r * 2 + 1; fff++)
		{
			ILI9341_SPI_Send(color >> 8);
			ILI9341_SPI_Send(color);
		}
		while (x < y) {
			if (f >= 0) {
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			x++;
			ddF_x += 2;
			f += ddF_x;
			ILI9341_Set_Address(x0 - x, y0 + y, x0 + x, y0 + y);
			TFT_DC_COMMAND;
			ILI9341_SPI_Send(0x2C);
			TFT_DC_DATA;
			for (uint32_t fff = 0; fff < x * 2 + 1; fff++) {
				ILI9341_SPI_Send(color >> 8);
				ILI9341_SPI_Send(color);
			}
			ILI9341_Set_Address(x0 - x, y0 - y, x0 + x, y0 - y);
			TFT_DC_COMMAND;
			ILI9341_SPI_Send(0x2C);
			TFT_DC_DATA;
			for (uint32_t fff = 0; fff < x * 2 + 1; fff++) {
				ILI9341_SPI_Send(color >> 8);
				ILI9341_SPI_Send(color);
			}
			ILI9341_Set_Address(x0 - y, y0 + x, x0 + y, y0 + x);
			TFT_DC_COMMAND;
			ILI9341_SPI_Send(0x2C);
			TFT_DC_DATA;
			for (uint32_t fff = 0; fff < y * 2 + 1; fff++) {
				ILI9341_SPI_Send(color >> 8);
				ILI9341_SPI_Send(color);
			}
			ILI9341_Set_Address(x0 - y, y0 - x, x0 + y, y0 - x);
			TFT_DC_COMMAND;
			ILI9341_SPI_Send(0x2C);
			TFT_DC_DATA;
			for (uint32_t fff = 0; fff < y * 2 + 1; fff++) {
				ILI9341_SPI_Send(color >> 8);
				ILI9341_SPI_Send(color);
			}
		}
	}
}

void ILI9341_Draw_String(unsigned int x, unsigned int y, unsigned int color, unsigned int phone, char* str, unsigned char size)
{
	SD_CS_OFF;
	TFT_CS_ON;

m_rotacion=0;

	if(m_rotacion!=6) y=239-y-size*8;
	else 			y=319-y-size*8;
	switch (size)
	{
		case 1:
		while (*str)
		{
			if ((x+(size*8))>320)
			{
				x = 1;
				y = y + (size*8);
			}
			ILI9341_Draw_Char(x, y, color, phone, *str, size);
			x += size*8-2;
			str++;
		}
		break;

		case 2:
		hh=1;
		while (*str)
		{
			if ((x+(size*8))>320)
			{
				x = 1;
				y = y + (size*8);
			}
			ILI9341_Draw_Char(x,y,color,phone,*str,size);
			x += hh*8;
			str++;
		}
		break;

		case 4:
		hh=1;
		while (*str)
		{
				if ((x+(size*8))>320)
				{
					x = 1;
					y = y + (size*8);
				}
				ILI9341_Draw_Char(x,y,color,phone,*str,size);
				x += hh*14;
				str++;
		}
		break;
	}
}

static void ILI9341_Send_Command(unsigned char command)
{
	SD_CS_OFF;
	TFT_CS_ON;

	Before_Sending_Command();
	ILI9341_SPI_Send(command);
}

static void ILI9341_Send_Data(unsigned char data)
{
	SD_CS_OFF;
	TFT_CS_ON;

	Before_Sending_Data();
	ILI9341_SPI_Send(data);
}


void ILI9341_Draw_Char(int x, int y, unsigned int colorT, unsigned int colorF, unsigned char charcode, unsigned char size)
{
	SD_CS_OFF;
	TFT_CS_ON;

	int i,h;
	unsigned char data[512];
	unsigned char Ch=colorT>>8;
	unsigned char Cl=colorT;
	unsigned char Fh=colorF>>8;
	unsigned char Fl=colorF;

	switch(size)
	{
		case 1:
		ILI9341_Set_Address(x, y, x+5, y+8);
		TFT_DC_COMMAND;
		ILI9341_SPI_Send(0x2C);
		TFT_DC_DATA;
		//for(h=0;h<8;h++)
		for(h=7;h>-1;h--)
		{
			for(i=2;i<8;i++)
			{
				if ((chars8[charcode-0x20][h]>>(7-i))&0x01)
				{
					ILI9341_SPI_Send(colorT>8);
					ILI9341_SPI_Send(colorT);
				}
				else
				{
					ILI9341_SPI_Send(colorF>>8);
					ILI9341_SPI_Send(colorF);
				}
			}
		}
		break;

		case 2:
		ILI9341_Set_Address(x, y, x+7, y+26);
		TFT_DC_COMMAND;
		ILI9341_SPI_Send(0x2C);
		TFT_DC_DATA;
		//for(h=0;h<16;h++)
		for(h=15;h>-1;h--)
		{
			for(i=0;i<8;i++)
			{
				if ((chars16[charcode-0x20][h]>>(7-i))&0x01)
				{
					ILI9341_SPI_Send(colorT>>8);
					ILI9341_SPI_Send(colorT);
				}
				else
				{
					ILI9341_SPI_Send(colorF>>8);
					ILI9341_SPI_Send(colorF);
				}
			}
		}
		break;

		case 4:
		ILI9341_Set_Address(x, y, x+7, y+32);
		TFT_DC_COMMAND;
		ILI9341_SPI_Send(0x2C);
		TFT_DC_DATA;
		for(h=0;h<16;h++)
		{
			for(i=0;i<8;i++)
			{
				if( (chars16[charcode-0x20][15-h]>>(7-i) )&0x01) data[i*2+32*h]=Ch,data[i*2+1+32*h]=Cl,data[i*2+16+32*h]=Ch,data[i*2+17+32*h]=Cl;
				else data[i*2+32*h]=Fh,data[i*2+1+32*h]=Fl,data[i*2+16+32*h]=Fh,data[i*2+17+32*h]=Fl;
			}
		}
		HAL_SPI_Transmit(&lcd_spi,data,512,10);
		break;
	}
}

static void Before_Sending_Data()
{
	SD_CS_OFF;
	TFT_CS_ON;

	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);
}

static void Before_Sending_Command()
{
	SD_CS_OFF;
	TFT_CS_ON;

	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);
}


