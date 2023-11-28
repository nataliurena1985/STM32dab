#include <stdio.h>
#include "ff.h"
#include "fatfs.h"
#include "main.h"
#include "stm32f3xx_hal.h"
#include <string.h>
#include <math.h>
#include "Panel Tactil.h"
#include "Idioma.h"
#include "ILI9341_Driver.h"
#include "Memoria.h"
#include "Recursos.h"
#include "XPT2046_touch.h"
#include "Aplicacion.h"
#include "OSC.h"

#define WCH//WCH MARELLI MAHLE

unsigned int m_Frec;
FIL fil;
FIL fPIC_BOB;//Bobina con diodo
FIL fPIC_CORO_AZ;
FIL fPIC_CORO_VE;
FIL fPIC_CORO_BL;
FIL fPIC_CORO_AM;
FIL fPIC_POT_DOB;
FIL fPIC_CONE_VEC;
FIL fPIC_CONE_ROC;
FIL fPIC_CONE_NEC;
FIL fPIC_CONE_BLC;
FIL fPIC_ERROR;
FIL fPIC_RETI_CHI;
FIL fPEL1[24];
FIL fBOT[8];
FIL fCORREDERA[2];

unsigned char m_Canal=1;
unsigned char m_PantallaConfig=1;

unsigned int m_Pantalla= 0;
unsigned int m_PantallaExt=0;
unsigned int m_PantallaAnt=0;
unsigned int m_PantallaProx[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char m_ManPag;
unsigned char m_ManUltimaPag;
unsigned int m_Fase=0;

void AppInit(void);
unsigned int fpwm(unsigned int pwm,unsigned int i) ;

int opcion=0;
int EnPrueba=0;
int Time=9;
int m_CC=0;
int m_IndiceGel=0;
int m_IndiceSol=0;
int m_IndiceBat=0;
int m_IndicePot1=0;
int m_IndicePot2=0;

unsigned char m_GenEolico;
unsigned char m_GrupoElectr;

int TiempoPruebaMin;
int TiempoPruebaSeg;
int andando=0;
int rot;
char Estado0,Estado1;

unsigned int m_x=0,m_y=0,m_z=0;

unsigned int m_yAnt=0,m_zAnt=0;
unsigned int m_PistaAscendente;
unsigned int m_ActivarPWM=1;
unsigned int m_Max=0;
unsigned int m_Min=0;
unsigned int m_RPM=700;
unsigned int m_ti=20;

unsigned int dis(unsigned int a,unsigned int b);
unsigned int MedirPistaAsc(void);
unsigned int MedirPistaDes(void);

void NOP(void);

unsigned int m_PWMInicialAsc;		
unsigned int m_PWMInicialDes;		
unsigned int m_PWMFinalAsc;		
unsigned int m_PWMFinalDes;		

unsigned char m_TimerIter=0;
unsigned char m_NroPistas=0;
unsigned char m_Seleccion=0;

float m_mAsc;
float m_mDes;

char m_Marca=0;//0=WCH 1=Magneti Marelli

unsigned int m_PWM=10;
unsigned int m_FREC=100;
unsigned int ValorPista1;
unsigned int ValorPista2;

void AppInit(void)
{
	int NroPIC=0;
	int NroBOT=0;
	int NroBTX=0;
	int NroPEL=0;
	int i;
	char a,b,c,d,e,ch;
	char File[50];

	m_ActivarPWM=0;
	m_Pantalla=0;
	//ApagoCanales();
}

void AppLoop(void)
{
	int i,x,y;
	char str[100];
/*
	if(m_Pantalla == 0)
	{
		m_Pantalla = DES_INVERTER;
		SPI3->CR1  &= ~ SPI_CR1_SPE;
		MODIFY_REG(SD_SPI_HANDLE.Instance->CR1,SPI_BAUDRATEPRESCALER_256,SPI_BAUDRATEPRESCALER_2);
		SPI3->CR1 |= SPI_CR1_SPE;
	}
*/

	if(m_Pantalla == 0)
	{
		SPI3->CR1  &= ~ SPI_CR1_SPE;
		MODIFY_REG(SD_SPI_HANDLE.Instance->CR1,SPI_BAUDRATEPRESCALER_256,SPI_BAUDRATEPRESCALER_2);
		SPI3->CR1 |= SPI_CR1_SPE;
		//PIC(LOGO_MM,0,0,0);
		f_BMP("REC/LOGO/PIC LOGO_WCH.bmp",0,0);
		sprintf(str,"%d %d",m_Pantalla,ERROR_SD);
		ILI9341_Draw_String(10,10, WHITE, 0xf800,str,2);
        BOW(34,200,0,18,INGRESAR);
        m_Pantalla++;
	}

	if(m_Pantalla == 2)
	{
		SPI3->CR1  &= ~ SPI_CR1_SPE;
		MODIFY_REG(SD_SPI_HANDLE.Instance->CR1,SPI_BAUDRATEPRESCALER_256,SPI_BAUDRATEPRESCALER_2);
		SPI3->CR1 |= SPI_CR1_SPE;
		f_BMP("REC/LOGO/PIC LOGO_ROT.bmp",0,0);
		sprintf(str,"%d %d",m_Pantalla,ERROR_SD);
		ILI9341_Draw_String(10,10, WHITE, 0xf800,str,2);
        BOW(34,200,0,18,INGRESAR);
        m_Pantalla++;
	}

	if(m_Pantalla == MENU_PRINCIPAL)
	{
		FondoCompletoAzul();
    	Encabezamiento3("MENU PRINCIPAL");
    	MenuSet( 0,"ARRANCAR            ",VISOR);
    	MenuSet( 1,"DESARROLLO DAB      ",DES_DAB);
    	MenuSet( 2,"DESARROLLO MPPT     ",DES_MPPT);
    	MenuSet( 3,"DESARROLLO INVERTER ",DES_INVERTER);
    	MenuSet( 4,"CONFIGURAR          ",CONFIGURAR);

    	MenuPrint();
    	m_Pantalla++;
	}

	if(m_Pantalla == CONFIGURAR)
	{
		FondoCompletoAzul();
    	Encabezamiento3("CONFIGURACION");
    	MenuSet( 0,"WIFI                ",WIFI);
    	MenuSet( 1,"HARDWARE            ",HARDWARE_PASS);
    	MenuPrint();
    	m_Pantalla++;
	}
	////////////////////////////////////////////////////////////////////////
	// CONFIGURAR WIFI
	////////////////////////////////////////////////////////////////////////
	if(m_Pantalla == WIFI)
	{
		FondoCompletoAzul();
	   	Encabezamiento3("CONFIGURAR WIFI");

	   	MenuSet( 0,"SSID                ",WIFI_SSID);
	   	MenuSet( 1,"CLAVE               ",WIFI_PASS);
	   	MenuSet( 2,"CONECTAR            ",WIFI_CONNECT);
	   	MenuSet( 3,"BORRAR CONFIGURACION",WIFI_CLR_CONF);
	   	MenuPrint();
	   	m_Pantalla++;
	}
	if(m_Pantalla == WIFI_SSID)
	{
		FondoCompletoAzul();
	   	Encabezamiento3("SSID WIFI");
		f_TECLADO_ALFA_open(fBOT);
		m_Pantalla++;
	}
	if(m_Pantalla == WIFI_PASS)
	{
		FondoCompletoAzul();
	   	Encabezamiento3("CLAVE WIFI");
		f_TECLADO_ALFA_open(fBOT);
		m_Pantalla++;
	}
	if(m_Pantalla==WIFI_MANDAR_SSID)
	{
		m_TecladoStr[44]=0;
		sprintf(str,"SSI%s;",m_TecladoStr);
		//mandarWifi(str);
		m_Pantalla=WIFI;
	}
	if(m_Pantalla==WIFI_MANDAR_PASS)
	{
		m_TecladoStr[44]=0;
		sprintf(str,"PAS%s;",m_TecladoStr);
		//mandarWifi(str);
		m_Pantalla=WIFI;
	}
	if(m_Pantalla == WIFI_CONNECT)
	{
		sprintf(str,"HIG;");
		//mandarWifi(str);
		m_Pantalla=WIFI;
	}
	if(m_Pantalla == WIFI_CLR_CONF)
	{
		sprintf(str,"CLR;");
		//mandarWifi(str);*/
		m_Pantalla=WIFI;
	}
	////////////////////////////////////////////////////////////////////////
	// CONFIGURAR HARDWARE
	////////////////////////////////////////////////////////////////////////
	if(m_Pantalla == HARDWARE_PASS)
	{
		FondoCompletoAzul();
       	Encabezamiento3("INGRESE CLAVE SOPORTE");
		f_TECLADO_ALFA_open(fBOT);
		m_Pantalla++;
	}
	if(m_Pantalla == HARDWARE_VERIF_PASS)
	{
		if(m_TecladoStr[0]=='1' && m_TecladoStr[1]=='2' && m_TecladoStr[2]=='3') m_Pantalla=HARDWARE;
		else m_Pantalla=MENU_PRINCIPAL;
	}
	if(m_Pantalla == HARDWARE)
	{
		FondoCompletoAzul();
    	Encabezamiento3("CONFIGURACION HARDWARE");
    	MenuSet( 0,"CONFIGURACION SISTEMA ",HARDWARE_SET_SISTEMA);
    	MenuSet( 1,"PRESET INVERTER       ",HARDWARE_PRESET_INVERTER);
    	MenuSet( 2,"PRESET MPPT           ",HARDWARE_PRESET_MPPT);
    	MenuSet( 3,"PRESET DAB            ",HARDWARE_PRESET_DAB);
    	MenuPrint();
    	m_Pantalla++;
	}
   	if(m_Pantalla == HARDWARE_SET_SISTEMA)
   	{
		FondoCompletoAzul();
	   	Encabezamiento3("CONFIGURACION SISTEMA");

	   	ILI9341_Draw_String(4,37,WHITE,0x0010,"POTENCIA",2);
		BCO(CORREDERA,4,55,0,4,3,"1K 3K 4K 5K");

		ILI9341_Draw_String(200,37,WHITE,0x0010,"ON-GRID",2);
		BCO(CORREDERA,200,55,0,2,1,"SI  NO");

		CHECK_CIRC(4,160,"GRUPO","ELECTR.",m_GrupoElectr);
		CHECK_CIRC(114,160,"GENER.","EOLICO",m_GenEolico);
		CHECK_CIRC(224,160,"RESERV.","",0);

		BOW(3,202,0,11,"OK");
		BOW(162,202,0,11,"SALIR");
		m_Pantalla++;
   	}
   	if(m_Pantalla == HARDWARE_PRESET_INVERTER)
	{
   		FondoCompletoAzul();
		Encabezamiento3("PRESET INVERTER");
    	MenuSet( 0,"VCA e ICA DE LINEA    ",HARDWARE_PRESET_INVERTER0);
    	MenuPrint();
    	m_Pantalla++;
	}
	if(m_Pantalla == HARDWARE_PRESET_MPPT)
	{
		FondoCompletoAzul();
		Encabezamiento3("PRESET MPPT");
    	MenuSet( 0,"VCC e ICC PANELES     ",HARDWARE_PRESET_MPPT0);
    	MenuPrint();
    	m_Pantalla++;

	}
	if(m_Pantalla == HARDWARE_PRESET_DAB)
	{
		FondoCompletoAzul();
		Encabezamiento3("PRESET DAB");
    	MenuSet( 0,"VCC e ICC DE BATERIA",HARDWARE_PRESET_DAB0);
    	MenuSet( 1,"VCC del BUS         ",HARDWARE_PRESET_DAB1);
    	MenuPrint();
    	m_Pantalla++;
	}
   	if(m_Pantalla == HARDWARE_PRESET_INVERTER0)
	{
		FondoCompletoAzul();
	   	Encabezamiento3("INVERTER VCA/ICA RED");
		f_PEL_open(fPEL1,"INVERTER","POT_CUR",18);//Cargo película EÓLICO desde SD y declaro que tiene 12 fotogramas

		i=0;
	   	ILI9341_Draw_String(27,35,WHITE, 0x0010,"TENSION RED",2);
	   	DPMChico(37+i,55,6,WHITE,RED,"220V");
		BOT(BOT_AIZQ,2+i,83,0),
		BOT(BOT_ADER,127+i,83,0);
		f_PEL(fPEL1,35+i,95,0,0,0);
		BOW(51+i,202,0,4,"OK");

		i=160;
		ILI9341_Draw_String(178,35,WHITE, 0x0010,"CORRIENTE RED",2);
		DPMChico(37+i,55,6,WHITE,RED,"12.4A");

		BOT(BOT_AIZQ,2+i,83,0),
		BOT(BOT_ADER,127+i,83,0);
		f_PEL(fPEL1,35+i,95,0,0,0);
		BOW(51+i,202,0,4,"OK");

		BOW(117,202,0,6,"SALIR");
		m_Pantalla++;
	}
   	if(m_Pantalla == HARDWARE_PRESET_MPPT0)
	{
		FondoCompletoAzul();
	   	Encabezamiento3("MPPT VCC-ICC PANELES");
		f_PEL_open(fPEL1,"INVERTER","POT_CUR",18);//Cargo película EÓLICO desde SD y declaro que tiene 12 fotogramas

		i=0;
	   	ILI9341_Draw_String(27,35,WHITE, 0x0010,"TENSION PANEL",2);
	   	DPMChico(37+i,55,6,WHITE,RED,"168V");
		BOT(BOT_AIZQ,2+i,83,0),
		BOT(BOT_ADER,127+i,83,0);
		f_PEL(fPEL1,35+i,95,0,0,0);
		BOW(51+i,202,0,4,"OK");

		i=160;
		ILI9341_Draw_String(178,35,WHITE, 0x0010,"CORRIENTE PANEL",2);
		DPMChico(37+i,55,6,WHITE,RED,"35.5A");

		BOT(BOT_AIZQ,2+i,83,0),
		BOT(BOT_ADER,127+i,83,0);
		f_PEL(fPEL1,35+i,95,0,0,0);
		BOW(51+i,202,0,4,"OK");

		BOW(117,202,0,6,"SALIR");
   		m_Pantalla++;
   	}
   	if(m_Pantalla == HARDWARE_PRESET_DAB0)
	{
		FondoCompletoAzul();
	   	Encabezamiento3("VCC-ICC BATERIAS");
		f_PEL_open(fPEL1,"INVERTER","POT_CUR",18);//Cargo película EÓLICO desde SD y declaro que tiene 12 fotogramas

		i=0;
	   	ILI9341_Draw_String(17,35,WHITE, 0x0010,"TENSION BATERIA",2);
	   	DPMChico(37+i,55,6,WHITE,RED,"53.4V");
		BOT(BOT_AIZQ,2+i,83,0),
		BOT(BOT_ADER,127+i,83,0);
		f_PEL(fPEL1,35+i,95,0,0,0);
		BOW(51+i,202,0,4,"OK");

		i=160;
		ILI9341_Draw_String(168,35,WHITE, 0x0010,"CORRIENTE BATERIAS",2);
		DPMChico(37+i,55,6,WHITE,RED,"12.1A");

		BOT(BOT_AIZQ,2+i,83,0),
   		BOT(BOT_ADER,127+i,83,0);
   		f_PEL(fPEL1,35+i,95,0,0,0);
   		BOW(51+i,202,0,4,"OK");

   		BOW(117,202,0,6,"SALIR");
   	   	m_Pantalla++;
   	}

   	if(m_Pantalla == HARDWARE_PRESET_DAB1)
	{
		FondoCompletoAzul();
	   	Encabezamiento3("VCC-ICC BUS");
		f_PEL_open(fPEL1,"INVERTER","POT_CUR",18);//Cargo película EÓLICO desde SD y declaro que tiene 12 fotogramas

		i=0;
	   	ILI9341_Draw_String(33,35,WHITE, 0x0010,"TENSION BUS",2);
	   	DPMChico(37+i,55,6,WHITE,RED,"53.4V");
		BOT(BOT_AIZQ,2+i,83,0),
		BOT(BOT_ADER,127+i,83,0);
		f_PEL(fPEL1,35+i,95,0,0,0);
		BOW(51+i,202,0,4,"OK");

		i=160;
		ILI9341_Draw_String(187,35,WHITE, 0x0010,"CORRIENTE BUS",2);
		DPMChico(37+i,55,6,WHITE,RED,"12.1A");

		BOT(BOT_AIZQ,2+i,83,0),
   		BOT(BOT_ADER,127+i,83,0);
   		f_PEL(fPEL1,35+i,95,0,0,0);
   		BOW(51+i,202,0,4,"OK");

   		BOW(117,202,0,6,"SALIR");
   	   	m_Pantalla++;
   	}


	///////////////////////////////////////////////////////////
	// VISORES
	///////////////////////////////////////////////////////////

	if(m_Pantalla == VISOR)//Pantalla PAR inicializa los fondos y variables, pasa la primera vez y luego va a la pantalla impar
	{
		m_IndicePel=1;
		FondoCompletoAzul();
		f_BMP("INVERTER/PIC/PIC VISOR1.bmp",0,0);//Dibujo fondo de pantalla
		f_PEL_open(fPEL1,"INVERTER","EOLICO",12);//Cargo película EÓLICO desde SD y declaro que tiene 12 fotogramas
		f_PEL(fPEL1,204,0,96,33,0);//Dibujo fondo del eólico en la posición 204,0
		BOW(118,03,0,6,SALIR);
		m_Pantalla++;
	}

	if(m_Pantalla == VISOR+1)//Pantallas IMPAR solo actualiza
	{
		m_IndicePel++;
		if(m_IndicePel>12) m_IndicePel=1;
		f_PEL(fPEL1,204,0,0,0,m_IndicePel);

		m_IndiceGel++;
		if(m_IndiceGel==10) f_BMP("INVERTER/PIC/PIC GEL_PREN.bmp",230,188);
		if(m_IndiceGel==20) f_BMP("INVERTER/PIC/PIC GEL_APAG.bmp",230,188),m_IndiceGel=0;

		m_IndiceSol++;
		if(m_IndiceSol==100) f_BMP("INVERTER/PIC/PIC SOL_PREN.bmp",57,0);
		if(m_IndiceSol==200) f_BMP("INVERTER/PIC/PIC SOL_APAG.bmp",57,0),m_IndiceSol=0;

		m_IndiceBat++;
		if(m_IndiceBat==34) m_IndiceBat=0;
		FillReg(23,200,55,34-m_IndiceBat,0x0000);
		FillReg(23,235-m_IndiceBat,55,m_IndiceBat,0xffff);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	// DESARROLLO DAB
	/////////////////////////////////////////////////////////////////////////////
	if(m_Pantalla == DES_DAB)
	{
		m_BTO=0;

		FondoCompletoAzul();

		OSCCanal1=&OSCCanal[0];
		OSCCanal2=&OSCCanal[1024];
		OSCCanal3=&OSCCanal[2048];
		OSCCanal4=&OSCCanal[3072];

		SetBT();
	    TomarCanales();
	    GraficarOSC();
	    m_Pantalla++;

	}
	if(m_Pantalla == DES_DAB+1)
	{
		m_Fase=m_Fase+10;
    	if(m_Fase==800) m_Fase=0;

    	TIM8->CCR1=800-m_Fase;
    	TIM8->CCR2=m_Fase;

    	TIM8->CCR3=m_Fase;
		TIM8->CCR4=800-m_Fase;

    	SetBT();
		TomarCanales();
		GraficarOSC();
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	// DESARROLLO MPPT
	/////////////////////////////////////////////////////////////////////////////
	if(m_Pantalla == DES_MPPT)
	{
		m_BTO=0;
		m_BTG=0;

		FondoCompletoAzul();

		OSCCanal1=&OSCCanal[0];
		OSCCanal2=&OSCCanal[1024];
		OSCCanal3=&OSCCanal[2048];
		OSCCanal4=&OSCCanal[3072];

		SetBT();
	    TomarCanales();
	    GraficarOSC();
	    m_Pantalla++;

	}
	if(m_Pantalla == DES_MPPT+1)
	{
		m_Fase=m_Fase+10;
    	if(m_Fase==800) m_Fase=0;

    	TIM8->CCR1=m_Fase;
    	TIM8->CCR2=500;

    	TIM8->CCR3=m_Fase;
		TIM8->CCR4=800;

    	SetBT();
		TomarCanales();
		GraficarOSC();
	}
	/////////////////////////////////////////////////////////////////////////////
	// DESARROLLO INVERTER
	/////////////////////////////////////////////////////////////////////////////
	if(m_Pantalla == DES_INVERTER)
	{
		for(i=0;i<250;i++)
		{
			SPWM1[i] = (uint32_t)(SPWM[i]*(float)1440);
		}

		m_BTO=3;

		FondoCompletoAzul();
		OSCCanal1=&OSCCanal[0];
		OSCCanal2=&OSCCanal[1024];
		OSCCanal3=&OSCCanal[2048];
		OSCCanal4=&OSCCanal[3072];

		SetBT();
	    TomarCanales();
	    GraficarOSC();
	    m_Pantalla++;
	}
	if(m_Pantalla == DES_INVERTER+1)
	{
//		m_Fase=m_Fase+10;
//  	if(m_Fase==800) m_Fase=0;

    	SetBT();
		TomarCanales();
		GraficarOSC();
	}
}


void NOP(void)
{
	m_Pantalla=2;
}


