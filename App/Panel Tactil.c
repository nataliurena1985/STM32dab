#include "ff.h"
#include "fatfs.h"
#include <stdio.h>
#include <string.h>
#include "stm32f3xx_HAL.h"
#include <math.h>
#include "Aplicacion.h"
#include "Idioma.h"
#include "main.h"
#include "Memoria.h"
#include "Recursos.h"
#include "XPT2046_touch.h"
#include "OSC.h"

unsigned char PedidoPanel=0;

void AtencionTecla();
void Volver(char i);

//*********************************************************************************************
// TOUCH
//*********************************************************************************************

void AtencionTecla()
{
	char str[23],i;

	unsigned char l_m_Pantalla=m_Pantalla;
	unsigned int ValorPista1,ValorPista2;

	XPT2046_TouchGetCoordinates();

	if(m_Pantalla==1 || m_Pantalla==3)
	{
		if(BOW_Activo(34,200,18) )
		{ 
			BOW(34,200,2,18,INGRESAR);									
			FondoTeclas();
			m_Pantalla++;
		}	 
	}
	if(m_Pantalla== MENU_PRINCIPAL+1)
	{
		MenuAtencion();
	}
	if(m_Pantalla==CONFIGURAR+1)
	{
		MenuAtencion();
	}
	if(m_Pantalla==WIFI+1)
	{
		MenuAtencion();
	}

	if(m_Pantalla==WIFI_SSID+1)
	{
		f_TECLADO_ALFA_Input(fBOT,WIFI_MANDAR_SSID,WIFI);
	}
	if(m_Pantalla==WIFI_PASS+1 )
	{
		f_TECLADO_ALFA_Input(fBOT,WIFI_MANDAR_PASS,WIFI);
	}





	if(m_Pantalla==HARDWARE_PASS+1)
	{
		f_TECLADO_ALFA_Input(fBOT,HARDWARE_VERIF_PASS,MENU_PRINCIPAL);
	}
	if(m_Pantalla==HARDWARE+1)
	{
		MenuAtencion();
	}
	if(m_Pantalla == HARDWARE_PRESET_INVERTER+1)
	{
		MenuAtencion();
	}
	if(m_Pantalla == HARDWARE_PRESET_MPPT+1)
	{
		MenuAtencion();
	}
	if(m_Pantalla == HARDWARE_PRESET_DAB+1)
	{
		MenuAtencion();
	}
	if(m_Pantalla == HARDWARE_SET_SISTEMA+1)
	{
		//CONFIGURACION VERTICAL CANAL X

		if(BCO_Activo(4,55,0,4)==1)
			BCO(CORREDERA,4,55,1,4,1,"NO DC AC GND");
		if(BCO_Activo(4,55,0,4)==2)
			BCO(CORREDERA,4,55,1,4,2,"NO DC AC GND");
		if(BCO_Activo(4,55,0,4)==3)
			BCO(CORREDERA,4,55,1,4,3,"NO DC AC GND");
		if(BCO_Activo(4,55,0,4)==4)
			BCO(CORREDERA,4,55,1,4,4,"NO DC AC GND");
		if(BCO_Activo(210,55,0,2)==1)
			BCO(CORREDERA,200,55,1,2,1,"SI  NO");
		if(BCO_Activo(210,55,0,2)==2)
			BCO(CORREDERA,200,55,1,2,2,"SI  NO");

		if(CHECK_CIRC_Activo(4,160))
		{
			if(m_GrupoElectr==0) m_GrupoElectr=1;
			else				 m_GrupoElectr=0;
			CHECK_CIRC(4,160,"GRUPO","ELECTR.",m_GrupoElectr);
		}
		if(CHECK_CIRC_Activo(114,160))
		{
			if(m_GenEolico==0) 	m_GenEolico=1;
			else				m_GenEolico=0;
			CHECK_CIRC(114,160,"GENER.","EOLICO",m_GenEolico);
		}
		if(CHECK_CIRC_Activo(224,160))
		{
			if(m_GrupoElectr==0) m_GrupoElectr=1;
			else				 m_GrupoElectr=0;
			CHECK_CIRC(224,160,"RESERV.","",0);
		}
		if(BOW_Activo(3,202,11) )
		{
			BOW(3,202,2,11,"OK");
			//Grabar Datos en SD
		}

		if(BOW_Activo(162,202,11) )
		{
			BOW(162,202,2,11,SALIR);
			m_Pantalla=HARDWARE;
		}

	}

	if
	(
		m_Pantalla == HARDWARE_PRESET_INVERTER0+1 	||
		m_Pantalla == HARDWARE_PRESET_MPPT0+1  		||
		m_Pantalla == HARDWARE_PRESET_DAB0+1 		||
		m_Pantalla == HARDWARE_PRESET_DAB1+1
	)
	{
		if( BOT_Activo(BOT_AIZQ,2,83))
		{
			BOT(BOT_AIZQ,2,83,1);
			do
			{
				XPT2046_TouchGetCoordinates();
				if(m_IndicePot1>1) m_IndicePot1--;
				f_PEL(fPEL1,35,95,2,0,m_IndicePot1);

			}while(BOT_Activo(BOT_AIZQ,2,83) );
	        BOT(BOT_AIZQ,2,83,0);
		}
		if( BOT_Activo(BOT_ADER,127,83))
		{
			BOT(BOT_ADER,127,83,1);
			do
			{
				XPT2046_TouchGetCoordinates();
				if(m_IndicePot1<18) m_IndicePot1++;
				f_PEL(fPEL1,35,95,2,0,m_IndicePot1);

			}while(BOT_Activo(BOT_ADER,127,83) );
	        BOT(BOT_ADER,127,83,0);
		}
		if( BOT_Activo(BOT_AIZQ,162,83))
		{
			BOT(BOT_AIZQ,162,83,1);
			do
			{
				XPT2046_TouchGetCoordinates();
				if(m_IndicePot2>1) m_IndicePot2--;
				f_PEL(fPEL1,195,95,2,0,m_IndicePot2);

			}while(BOT_Activo(BOT_AIZQ,162,83) );
	        BOT(BOT_AIZQ,162,83,0);
		}
		if( BOT_Activo(BOT_ADER,287,83))
		{
			BOT(BOT_ADER,287,83,1);
			do
			{
				XPT2046_TouchGetCoordinates();
				if(m_IndicePot2<18) m_IndicePot2++;
				f_PEL(fPEL1,195,95,2,0,m_IndicePot2);

			}while(BOT_Activo(BOT_ADER,287,83) );
			BOT(BOT_ADER,287,83,0);
		}

		if(BOW_Activo(51,202,4) )
		{
			BOW(51,202,2,4,"OK");
		}
		if(BOW_Activo(211,202,4) )
		{
			BOW(211,202,2,4,"OK");
		}

		if(BOW_Activo(117,202,6) )
		{
			BOW(117,202,2,6,SALIR);
			m_Pantalla=HARDWARE;
		}
	}
	if(	m_Pantalla == VISOR+1)
	{
		if(BOW_Activo(118,3,6) )
		{
			BOW(118,3,2,6,SALIR);
			m_Pantalla=MENU_PRINCIPAL;
		}
	}

	if(	m_Pantalla == DES_DAB+1)
	{
		m_Pantalla=MENU_PRINCIPAL;
	}

	if(	m_Pantalla == DES_MPPT+1)
	{
		m_Pantalla=MENU_PRINCIPAL;
	}

	if(	m_Pantalla == DES_INVERTER+1)
	{
		if(m_XPanel<100 && m_BTO>0 ) m_BTO--;
		if(m_XPanel>210 && m_BTO<14) m_BTO++;
		if(m_XPanel>100 && m_XPanel<210  ) m_Pantalla=MENU_PRINCIPAL;
		while( HAL_GPIO_ReadPin(TP_IRQ_GPIO_Port, TP_IRQ_Pin) == 0 );
	}

		/*
	if(Pantalla==253)
	{
		if(BOW_Activo(34,200,18) )
		{
			BOW(60,208,2,14,VOLVER);
			FondoTeclas();
			Pantalla= 2;
		}
	}

    if(Pantalla==255)
	{
		if(BOW_Activo(7,205,5))
		{
			 str[0]=0xA4;
			 str[1]=0xA5;
			 str[2]=0;
			 BOW(4,205,2,5,str);
			 TeclaAtrasMAN();
		}
		if(BOW_Activo(87,205,5))
		{
			 str[0]=0xA6;
			 str[1]=0xA7;
			 str[2]=0;
			 BOW(84,205,2,5,str);
			 TeclaArribaMAN();
		}
		if(BOW_Activo(167,205,5))
		{
			 str[0]=0xA8;
			 str[1]=0xA9;
			 str[2]=0;
			 BOW(164,205,2,5,str);
			 TeclaAbajoMAN();
		}
		if(BOW_Activo(247,205,5))
		{
			 str[0]=0xA5;
			 str[1]=0xA3;
			 str[2]=0;
			 BOW(244,205,2,5,str);
			 TeclaAdelanteMAN();
		}
		if( ValY<192 && m_Pag==0)
		{
			 if( ValY<24)			m_renglonSel[m_Pag]=0;
			 if( ValY>24  && ValY<48 )	 m_renglonSel[m_Pag]=1;
			 if( ValY>48  && ValY<72 )	 m_renglonSel[m_Pag]=2;
			 if( ValY>72  && ValY<96 )	 m_renglonSel[m_Pag]=3;
			 if( ValY>96  && ValY<120)	 m_renglonSel[m_Pag]=4;
			 if( ValY>120 && ValY<144)	 m_renglonSel[m_Pag]=5;
			 if( ValY>144 && ValY<168)	 m_renglonSel[m_Pag]=6;
			 if( ValY>168 && ValY<192)	 m_renglonSel[m_Pag]=7;
		 }
		 m_NucleoMAN=0;
	 }
*/



	PedidoPanel=0;
}
