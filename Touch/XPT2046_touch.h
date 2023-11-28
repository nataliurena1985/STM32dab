/*
 * XPT2046_touch.h
 *
 *  Created on: 20 sep. 2019 ð.
 *      Author: Andriy Honcharenko
 */

/*** Redefine if necessary ***/

// Warning! Use SPI bus with < 2.5 Mbit speed, better ~650 Kbit to be save.

extern unsigned int m_XPanel;
extern unsigned int m_YPanel;

extern SPI_HandleTypeDef XPT2046_SPI_PORT;

#define TP_IRQ_Pin 			GPIO_PIN_1
#define TP_IRQ_GPIO_Port 	GPIOB
#define TP_CS_Pin 			GPIO_PIN_13
#define TP_CS_GPIO_Port 	GPIOB
#define TP_SPI_HANDLE 		hspi3

#define TP_CS_OFF 			HAL_GPIO_WritePin(TP_CS_GPIO_Port,TP_CS_Pin,GPIO_PIN_SET)
#define TP_CS_ON 			HAL_GPIO_WritePin(TP_CS_GPIO_Port,TP_CS_Pin,GPIO_PIN_RESET)

// call before initializing any SPI devices
//unsigned char XPT2046_TouchPressed();
void XPT2046_TouchGetCoordinates();
