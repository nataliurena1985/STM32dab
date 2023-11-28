#include "ff.h"
#include "fatfs.h"
#include "main.h"
#include "stm32f3xx_hal_dma.h"
#include "stm32f3xx_hal.h"
#include "ILI9341_Driver.h"
#include <stdlib.h>
#include <string.h>
#include "Memoria.h"
#include "fatfs_sd.h"
#include "user_diskio.h"
#include "Recursos.h"
#include "Aplicacion.h"
#include "XPT2046_touch.h"
#include "stdio.h"
#include "OSC.h"


char m_Conf;
unsigned char* m_Memoria;
char BuscarCodigo(long punt);
char BusquedaStr(char *a, char *b);
char m_TecladoStr[50];
unsigned int m_TecladoIndice=0;
unsigned int m_IndicePel=0;
char m_Teclado=0;

unsigned int m_MenuMax;
char m_MenuStr[30][25];
unsigned int MedirVerde(void);
unsigned int MedirBlanco(void);
unsigned int MedirCorriente(void);

void DPMChico(unsigned int x,unsigned int y,unsigned char LargoBoton,unsigned int ColorFont,unsigned int ColorFondo,char* str);

void GraficarOSC(void);
void Graficar5(void);
void Graficar6(void);
void Graficar5E(void);
void Graficar5G(void);
void Graficar6E(void);
void GraficarPWM(void);
void Graficar(unsigned int x,unsigned int y,unsigned int orix,unsigned int oriy,unsigned int color);

void PanTecladoCodigo(unsigned char* Titulo,unsigned char QRen);
void MuestraXY(void);

void f_TECLADO_ALFA_open(FIL* f);
void f_TECLADO_ALFA_Input(FIL* fi,unsigned int PantallaSI,unsigned int PantallaNO);
void f_TECLADO_ALFA_fondo(FIL* f);

void f_PEL_open(FIL* f,char* Dir,char* Name,unsigned int QEsc);
void f_PEL(FIL* Pel,unsigned int x,unsigned int y,unsigned int offx,unsigned int offy,unsigned char Esc);
void f_PEL_close(FIL* f,unsigned int QEsc);

unsigned int f_BOT_Ancho[6];
unsigned int f_BOT_Alto[6];
void f_BOT_open(FIL* f,char* Name);
void f_BOT(FIL* Bot,unsigned int x,unsigned int y,unsigned char Est);
void f_BOT_close(FIL* f);
unsigned char f_BOT_Activo(FIL* Bot,unsigned int x,unsigned int y);

void f_BCO_open(FIL* f,char* Name);
void f_BCO(FIL* Bot,unsigned int x,unsigned int y,unsigned char Est,unsigned char Largo,unsigned char Pos,char* str);
void f_BCO_close(FIL* f);
unsigned char f_BCO_Activo(unsigned int x,unsigned int y,unsigned char Est,unsigned char Largo);

void f_BOP(FIL* f,unsigned int x,unsigned int y,unsigned int wx,unsigned int wy,unsigned char Est);

void f_PIC_open(FIL* f,char* Name);
void f_PIC(FIL* f,unsigned int x,unsigned int y);
void f_PIC_close(FIL* f);

void f_BMP(char* File,unsigned int x,unsigned int y);

unsigned char  StaticBox2_Activo(unsigned int x,unsigned int y,unsigned char LargoBoton);

unsigned char  Zona_Activo(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);

void PIC(unsigned char* File,unsigned int x,unsigned int y,unsigned char Brillo);

void BOP(FIL* f,unsigned int x,unsigned int y,unsigned int wx,unsigned int wy,unsigned char Est);
void BOT(unsigned char* ID,unsigned int x,unsigned int y,unsigned char Est);
unsigned char  BOT_Activo(unsigned char* ID,unsigned int x,unsigned int y);

unsigned char BOW_PX(unsigned int x,unsigned int y,unsigned int m_PantallaDest,unsigned char LargoBoton,char* str);
unsigned char BOW_Activo(unsigned int x,unsigned int y,unsigned char LargoBoton);

unsigned long Vin2Res(unsigned long in,unsigned long R);
unsigned long Ajustar(unsigned long Valor,unsigned long Numerador,unsigned long Denominador);

void MenuSet(unsigned char MenuNro,char* Buffer,unsigned int PantallaProx);
void MenuPrint();
void MenuAtencion(void);


void CHECK_CIRC(unsigned int x,unsigned int y,char* a,char* b,unsigned char Est);
unsigned char  CHECK_CIRC_Activo(unsigned int x,unsigned int y);


//////////////////////////////////////////////////////////
// PELICULAS DESDE SD FILE
//////////////////////////////////////////////////////////
void f_PEL_open(FIL* f,char* Dir,char* Name,unsigned int QEsc)
{
	int i;
	TP_CS_OFF;
	TFT_CS_OFF;
	SD_CS_ON;

	char File[50];
	for(i=0;i<=QEsc;i++)
	{
		sprintf(File,"%s/PEL %s/PEL %s %d.bmp",Dir,Name,Name,i);
		f_open(&f[i],File,FA_READ);
	}
	SD_CS_OFF;
}
void f_PEL(FIL* Pel,unsigned int x,unsigned int y,unsigned int offx,unsigned int offy,unsigned char Esc)
{
    unsigned int xx;
    unsigned int yy;

    if(Esc==0)
    {
      xx=x;
      yy=y;
    }
    else
    {
      xx=x+offx;
      yy=y+offy;
    }
    f_PIC(&Pel[Esc],xx,yy);
}
void f_PEL_close(FIL* f,unsigned int QEsc)
{
	TP_CS_OFF;
	TFT_CS_OFF;
	SD_CS_ON;
	int i;

	for(i=0;i<QEsc;i++)
	{

		f_close(&f[i]);
	}
	SD_CS_OFF;
}
//////////////////////////////////////////////////////////
// BOTON CORREDERA DESDE SD FILE
//////////////////////////////////////////////////////////
void f_BCO_open(FIL* f,char* Name)
{
	int i;
	TP_CS_OFF;
	TFT_CS_OFF;
	SD_CS_ON;
	char File[50];
	sprintf(File,"BOT/BOT %s V.bmp",Name);
	f_open(&f[0],File,FA_READ);
	sprintf(File,"BOT/BOT %s H.bmp",Name);
	f_open(&f[1],File,FA_READ);
	SD_CS_OFF;
}
void f_BCO(FIL* Bot,unsigned int x,unsigned int y,unsigned char Est,unsigned char Largo,unsigned char Pos,char* str)
{
	TP_CS_OFF;
	SD_CS_OFF;
	TFT_CS_ON;
    uint16_t colorF=0x64DF;
    uint16_t colorT=0x0000;
    uint16_t xt=x+6+(Largo*3-(unsigned char)strlen(str))*7;
    uint16_t yt=y;
    uint16_t w=Largo*14*3;
    Pos--;

    switch(Est)
    {
    	case 0://H
   		ILI9341_Draw_String(xt,yt,colorT,colorF,str,4);
   		FillReg(x   ,40+y   ,w  ,8 ,0xFC00);
   		FillReg(x+8 ,40+y+8 ,w-8,8 ,BLACK);
   		FillReg(x	,40+y	,8	,24,0xFC00);
   		FillReg(x+w ,40+y	,8	,24,0xFC00);
   		FillReg(x	,40+y+16,w  ,8 ,0xFC00);
    	f_PIC(&fCORREDERA[1],x+14*3*Pos+12,y+30);
    	break;

    	case 1://H
   		FillReg(x   ,40+y   ,w  ,8 ,0xFC00);
   		FillReg(x+8 ,40+y+8 ,w-8,8 ,BLACK);
   		FillReg(x	,40+y	,8	,24,0xFC00);
   		FillReg(x+w ,40+y	,8	,24,0xFC00);
   		FillReg(x	,40+y+16,w  ,8 ,0xFC00);
   		FillReg(x   ,30+y   ,w  ,10,0x64df);
   		FillReg(x   ,64+y   ,w  ,10,0x64df);
   		f_PIC(&fCORREDERA[1],x+14*3*Pos+12,y+30);
   		while( HAL_GPIO_ReadPin(TP_IRQ_GPIO_Port, TP_IRQ_Pin) == 0 );
    	break;

    	case 2://V
   		FillReg(x+2,y+2,w-2,35,colorF);
   		ILI9341_Draw_String(xt+1,yt+1,colorT,colorF,str,4);
   		FillReg(x   ,y   ,8 ,w  ,0xFC00);
  		FillReg(x+8 ,y+8 ,8 ,w-8,BLACK);
   		FillReg(x	,y	 ,24,8  ,0xFC00);
  		FillReg(x   ,y+w ,24,8  ,0xFC00);
   		FillReg(x+16,y   ,8 ,w  ,0xFC00);
    	break;

    	case 3://H
    	f_PIC(&fCORREDERA[1],x+30,y+14*3*Pos+12);
    	break;

    }
}
unsigned char f_BCO_Activo(unsigned int x,unsigned int y,unsigned char Est,unsigned char Largo)
{
	return BCO_Activo(x,y,Est,Largo);
}
void f_BCO_close(FIL* f)
{
	TP_CS_OFF;
	TFT_CS_OFF;
	SD_CS_ON;
	f_close(&f[0]);
	f_close(&f[1]);
	SD_CS_OFF;
}
//////////////////////////////////////////////////////////
// BOTON PIC DESDE SD FILE
//////////////////////////////////////////////////////////
void f_BOT_open(FIL* f,char* Name)
{
	TP_CS_OFF;
	TFT_CS_OFF;
	SD_CS_ON;
	char File[50];
	sprintf(File,"REC/BOT/BOT %s Up.bmp",Name);
	f_open(&f[0],File,FA_READ);
	sprintf(File,"REC/BOT/BOT %s Dn.bmp",Name);
	f_open(&f[1],File,FA_READ);
	SD_CS_OFF;
}
void f_BOT(FIL* Bot,unsigned int x,unsigned int y,unsigned char Est)
{
    switch(Est)
    {
    	case 0://Arriba
    	f_PIC(&Bot[0],x,y);
    	break;

    	case 1://Abajo
       	f_PIC(&Bot[1],x,y);
    	break;

    	case 2: //Baja, espera y va arriba)
       	f_PIC(&Bot[1],x,y);
       	while( HAL_GPIO_ReadPin(TP_IRQ_GPIO_Port, TP_IRQ_Pin) == 0 );
       	f_PIC(&Bot[0],x,y);
    	break;
    }
}
void f_BOT_close(FIL* f)
{
	TP_CS_OFF;
	TFT_CS_OFF;
	SD_CS_ON;
	f_close(&f[0]);
	f_close(&f[1]);
	SD_CS_OFF;
}
unsigned char f_BOT_Activo(FIL* Bot,unsigned int x,unsigned int y)
{

	uint8_t data[55];
	UINT 	br;

	TFT_CS_OFF;
	SD_CS_ON;
	f_lseek(&Bot[0],0);
	f_read(&Bot[0],data,25,&br);
	SD_CS_OFF;

	unsigned int AnchoBMP=data[0x12]+data[0x13]*256;
	unsigned int AltoBMP=data[0x16]+data[0x17]*256;

    if
    (
      (m_XPanel>x && m_XPanel<(x+AnchoBMP))
      &&
      (m_YPanel>y && m_YPanel<(y+AltoBMP))
    ) return 1;
    else return 0;
}
//////////////////////////////////////////////////////////
// BOTON BMP FONDO BLANCO CON BORDE DESDE FILE
//////////////////////////////////////////////////////////
void f_BOP(FIL* f,unsigned int x,unsigned int y,unsigned int wx,unsigned int wy,unsigned char Est)
{
	SD_CS_OFF;
	TFT_CS_ON;
	UINT br;
	unsigned char data[30];

	switch(Est)
    {
    	case 0://Arriba
   		//FillReg(x+2,y+2,wx-2,35,colorF);
   		FillReg(x   ,y	   ,wx  ,2   ,BLACK);
   		FillReg(x	,y	   ,2	  ,wy  ,BLACK);
   		FillReg(x+wx,y	   ,2	  ,wy  ,BLACK);
   		FillReg(x	,y+wy-2,wx  ,2   ,BLACK);
       	FillReg(x+2 ,y+wy-4,wx-4,2   ,DARKGREY);
      	FillReg(x+wx-2,y+2,2    ,wy-4,DARKGREY);
      	FillReg(x+2 ,y+2   ,wx-4 ,2   ,WHITE);
      	FillReg(x+2	,y+2   ,2	 ,wy-4,WHITE);
    	break;

    	case 1://Abajo
   		//FillReg(x+2,y+2,wx-2,35,colorF);
     	FillReg(x   ,y	   ,wx  ,2   ,BLACK);
      	FillReg(x	,y	   ,2	,wy  ,BLACK);
      	FillReg(x+wx,y	   ,2	,wy  ,BLACK);
      	FillReg(x	,y+wy-2,wx  ,2   ,BLACK);
       	FillReg(x+2 ,y+wy-4,wx-4,2   ,WHITE);
     	FillReg(x+wx-2,y+2,2 ,wy-4,WHITE);
   		FillReg(x+2 ,y+2   ,wx-4 ,2   ,DARKGREY);
   		FillReg(x+2	,y+2   ,2	 ,wy-4,DARKGREY);
    	break;

    	case 2://Baja, espera y va arriba)
    	//FillReg(x+2,y+2,wx-2,35,colorF);
       	FillReg(x   ,y	 ,wx    ,2   ,BLACK);
       	FillReg(x	,y	 ,2	    ,wy  ,BLACK);
       	FillReg(x+wx,y	 ,2	    ,wy  ,BLACK);
       	FillReg(x	,y+wy-2,wx  ,2   ,BLACK);

       	FillReg(x+2 ,y+wy-4,wx-4,2   ,WHITE);
     	FillReg(x+wx-2,y+2,2 ,wy-4,WHITE);
   		FillReg(x+2 ,y+2   ,wx-4 ,2   ,DARKGREY);
   		FillReg(x+2	,y+2   ,2	 ,wy-4,DARKGREY);
   		while( HAL_GPIO_ReadPin(TP_IRQ_GPIO_Port, TP_IRQ_Pin) == 0 );

       	HAL_Delay(255);

       	//FillReg(x+2,y+2,wx-2,35,colorF);
   		FillReg(x   ,y	   ,wx   ,2   ,BLACK);
   		FillReg(x	,y	   ,2	 ,wy  ,BLACK);
   		FillReg(x+wx,y	   ,2	 ,wy  ,BLACK);
   		FillReg(x	,y+wy-2,wx   ,2   ,BLACK);

       	FillReg(x+2 ,y+wy-4,wx-4,2   ,DARKGREY);
      	FillReg(x+wx-2,y+2,2    ,wy-4,DARKGREY);
      	FillReg(x+2 ,y+2   ,wx-4 ,2   ,WHITE);
      	FillReg(x+2	,y+2   ,2	 ,wy-4,WHITE);

   		HAL_Delay(55);
    	break;
    }

    TFT_CS_OFF;
    SD_CS_ON;
	f_lseek(f,0);
    f_read(f,data,25,&br);
    unsigned int dx=(wx-((unsigned int)data[0x12]+(unsigned int)data[0x13]*256))/2;
    unsigned int dy=(wy-((unsigned int)data[0x16]+(unsigned int)data[0x17]*256))/2;
    f_PIC(f,x+dx,y+dy);
}

//////////////////////////////////////////////////////////
// TECLADO_ALFA DESDE SD FILE
//////////////////////////////////////////////////////////
void f_TECLADO_ALFA_Input(FIL* fi,unsigned int PantallaSI,unsigned int PantallaNO)
{
   int f,c;
   char str[50];
   char Tecla[41];

   if(m_Teclado==0) sprintf(Tecla,"0123456789QWERTYUIOPASDFGHJKLN#^ZXCVBNM<");
   if(m_Teclado==1) sprintf(Tecla,"0123456789qwertyuiopasdfghjkLn#^zxcvbnm<");
   if(m_Teclado==2) sprintf(Tecla,"~!#$%c&@_?!=+-*%c%c^|xX<>()[]{}oO#^%c'.,:;!<",'%','/','\\','"');


   for(f=0;f<4;f++)
   {
	   for(c=0;c<10;c++)
	   {
		   if( m_XPanel>c*32 && m_XPanel<(c+1)*32 && m_YPanel>80+f*32 && m_YPanel<80+(f+1)*32 )
		   {
	   		    f_PIC(&fi[1],c*32,80+f*32);

	   		    ILI9341_Draw_Char(c*32+12,240-f*32-112,0xffff,0x5AF7,Tecla[f*10+c],4);
	   		    while( HAL_GPIO_ReadPin(TP_IRQ_GPIO_Port, TP_IRQ_Pin) == 0 );
		       	HAL_Delay(255);
 			    f_PIC(&fi[0],c*32,80+f*32);

		    	ILI9341_Draw_Char(c*32+12,240-f*32-112,0xffff,0x0010,Tecla[f*10+c],4);
 			    if(f*10+c != 30 && f*10+c != 31 && f*10+c != 39 && m_TecladoIndice<22)
 			    {
 			    	m_TecladoStr[m_TecladoIndice]=Tecla[f*10+c];
 			    	m_TecladoStr[m_TecladoIndice+1]=0;
 			    	m_TecladoIndice++;
 			    }
 			    if(f*10+c == 39 && m_TecladoIndice>0) m_TecladoIndice--,m_TecladoStr[m_TecladoIndice]=0;
 			    if(f*10+c == 31)
 			    {
 			    	if(m_Teclado!=0) m_Teclado=0;
 			    	else			 m_Teclado=1;
 			    }
 			    if(f*10+c == 30)
 			    {
 			      	if(m_Teclado!=2) m_Teclado=2;
 			       	else			 m_Teclado=0;
 			    }
 			    if(f*10+c == 30 || f*10+c == 31) f_TECLADO_ALFA_fondo(fi);
		   }
	   }
   }
   if( m_XPanel<80 && m_YPanel>208 )
   {
	    f_PIC(&fi[7],0,208);
		ILI9341_Draw_String(26,207,0xffff,0x5AF7, "<-",4);
	    while( HAL_GPIO_ReadPin(TP_IRQ_GPIO_Port, TP_IRQ_Pin) == 0 );
      	HAL_Delay(255);
        f_PIC(&fi[6],0,208);
        ILI9341_Draw_String(26,207,0xffff,0x0010, "<-",4);
        m_Pantalla=PantallaNO;
   }
   if( m_XPanel>80 && m_XPanel<240 && m_YPanel>208 )
   {
	    f_PIC(&fi[5],80,208);
	    while( HAL_GPIO_ReadPin(TP_IRQ_GPIO_Port, TP_IRQ_Pin) == 0 );
      	HAL_Delay(255);
        f_PIC(&fi[4],80,208);
   }
   if( m_XPanel>240 && m_YPanel>208 )
   {
	    f_PIC(&fi[7],240,208);
		ILI9341_Draw_String(266,207,0xffff,0x5AF7, "OK",4);
	    while( HAL_GPIO_ReadPin(TP_IRQ_GPIO_Port, TP_IRQ_Pin) == 0 );
      	HAL_Delay(255);
        f_PIC(&fi[6],240,208);
        ILI9341_Draw_String(266,207,0xffff,0x0010, "OK",4);
        m_Pantalla=PantallaSI;
   }
   sprintf(str,"%s                      ",m_TecladoStr);
   m_TecladoStr[22]=0;
   str[21]=0;
   ILI9341_Draw_String(0,40,0,0x64DF,str,4);
}

void f_TECLADO_ALFA_open(FIL* fi)
{
   int f,c;
   char str[35];

   m_TecladoIndice=0;

   f_PIC_open(&fi[0],"REC/TECLADO ALFA/BOT TECLA UP.bmp");
   f_PIC_open(&fi[1],"REC/TECLADO ALFA/BOT TECLA DN.bmp");
   f_PIC_open(&fi[2],"REC/TECLADO ALFA/BOT BUSCAR UP.bmp");
   f_PIC_open(&fi[3],"REC/TECLADO ALFA/BOT BUSCAR DN.bmp");
   f_PIC_open(&fi[4],"REC/TECLADO ALFA/BOT SP UP.bmp");
   f_PIC_open(&fi[5],"REC/TECLADO ALFA/BOT SP DN.bmp");
   f_PIC_open(&fi[6],"REC/TECLADO ALFA/BOT BARRA UP.bmp");
   f_PIC_open(&fi[7],"REC/TECLADO ALFA/BOT BARRA DN.bmp");
// ILI9341_Draw_String(0,80,WHITE,0x0010,str, 4);

   f_TECLADO_ALFA_fondo(fi);
}

void f_TECLADO_ALFA_fondo(FIL* fi)
{
	int f,c;

	char Tecla[41];

	if(m_Teclado==0) sprintf(Tecla,"0123456789QWERTYUIOPASDFGHJKLN#^ZXCVBNM<");
	if(m_Teclado==1) sprintf(Tecla,"0123456789qwertyuiopasdfghjkLn#^zxcvbnm<");
	if(m_Teclado==2) sprintf(Tecla,"~!#$%c&@_?!=+-*%c%c^|xX<>()[]{}oO#^%c'.,:;!<",'%','/','\\','"');

	for(f=0;f<4;f++)
	{
	   	for(c=0;c<10;c++)
	   	{
	   	   f_PIC(&fi[0],c*32,80+f*32);
	   	   ILI9341_Draw_Char(c*32+12,240-f*32-112,0xffff,0x0010,Tecla[f*10+c],4);
	   	}
	}
	f_PIC(&fi[6],0,208);
	ILI9341_Draw_String(26,207,0xffff,0x0010, "<-",4);
	f_PIC(&fi[4],80,208);
	f_PIC(&fi[6],240,208);
	ILI9341_Draw_String(266,207,0xffff,0x0010,"OK",4);
}

//////////////////////////////////////////////////////////
// PIC DESDE SD FILE
//////////////////////////////////////////////////////////
void f_PIC_open(FIL* f,char* Name)
{
	char File[50];
	TP_CS_OFF;
	TFT_CS_OFF;
	SD_CS_ON;
	f_open(f,Name,FA_READ);
	SD_CS_OFF;
}
void f_PIC_close(FIL* f)
{
	TP_CS_OFF;
	TFT_CS_OFF;
	SD_CS_ON;
	f_close(f);
	SD_CS_OFF;
}
void f_PIC(FIL* f,unsigned int x,unsigned int y)
{
	uint32_t		i,ii,iii;
	uint8_t    		dataOut[512];
	unsigned char 	dataIn[256],color[4],c;
	unsigned int 	col_16,TablaCol[256];
	UINT 			br,ERROR_SD;
	uint32_t		resto;

	TFT_CS_OFF;
	SD_CS_ON;

	f_lseek(f,0);
	f_read(f,dataIn,54,&br);

	uint32_t	AnchoBMP=(unsigned int)dataIn[0x12]+(unsigned int)dataIn[0x13]*256;
	uint32_t	AltoBMP=(unsigned int)dataIn[0x16]+(unsigned int)dataIn[0x17]*256;
	uint32_t	OffsetArray=(unsigned int)dataIn[0x0A]+(unsigned int)dataIn[0x0B]*256;
	uint32_t	LargoTablaColores=(unsigned int)dataIn[0x2E]+(unsigned int)dataIn[0x2F]*256;

	for(i=0;i<LargoTablaColores;i++)
	{
		f_read(f,color,4,&br);
		TablaCol[i]=(color[2]/8)*2048+(color[1]/4)*32+(color[0])/8;
	}

	f_lseek(f,OffsetArray);
	uint32_t Peso=AnchoBMP*AltoBMP;

	SD_CS_OFF;
	TFT_CS_ON;
	ILI9341_Set_Rotation(4);
	unsigned int x1=x;
	unsigned int y1=240-(unsigned int)AltoBMP-y;
	unsigned int x2=AnchoBMP+x-1;
	unsigned int y2=239-y;
	ILI9341_Set_Address(x1,y1,x2,y2);
	TFT_DC_COMMAND;
	dataIn[0]=0x2C;
	HAL_SPI_Transmit(&lcd_spi, dataIn, 1, 1);
	DWORD sect ;

	for(i=0;i<Peso;i=i+256)
	{
		TFT_CS_OFF;
		SD_CS_ON;

		resto = Peso-i;
		if(resto>256) resto=256;

		f_read(f,dataIn,resto,&br);
		for(ii=0,iii=0;ii<resto;ii++,iii++)
		{
			dataOut[iii]=TablaCol[dataIn[ii]]>>8;
			iii++;
			dataOut[iii]=TablaCol[dataIn[ii]];
		}

		SD_CS_OFF;
		TFT_CS_ON;
		TFT_DC_DATA;
		HAL_SPI_Transmit(&lcd_spi,dataOut,resto*2,300);
		TFT_CS_OFF;
	}
}

//////////////////////////////////////////////////////////
// BOTON CORREDERA DESDE FLASH INTERNA
//////////////////////////////////////////////////////////
void BCO(unsigned char* Bot,unsigned int x,unsigned int y,unsigned char Est,unsigned char Largo,unsigned char Pos,char* str)
{
	TP_CS_OFF;
	SD_CS_OFF;
	TFT_CS_ON;
    uint16_t colorF=0x64DF;
    uint16_t colorT=0x0000;
    uint16_t xt=x+6+(Largo*3-(unsigned char)strlen(str))*7;
    uint16_t yt=y;
    uint16_t w=Largo*14*3;
    Pos--;

    switch(Est)
    {
    	case 0://H
   		ILI9341_Draw_String(xt,yt,colorT,colorF,str,4);
   		FillReg(x   ,40+y   ,w  ,8 ,0xFC00);
   		FillReg(x+8 ,40+y+8 ,w-8,8 ,BLACK);
   		FillReg(x	,40+y	,8	,24,0xFC00);
   		FillReg(x+w ,40+y	,8	,24,0xFC00);
   		FillReg(x	,40+y+16,w  ,8 ,0xFC00);
    	PIC(Bot,x+14*3*Pos+12,y+30,0);
    	break;

    	case 1://H
   		FillReg(x   ,40+y   ,w  ,8 ,0xFC00);
   		FillReg(x+8 ,40+y+8 ,w-8,8 ,BLACK);
   		FillReg(x	,40+y	,8	,24,0xFC00);
   		FillReg(x+w ,40+y	,8	,24,0xFC00);
   		FillReg(x	,40+y+16,w  ,8 ,0xFC00);
   		FillReg(x   ,30+y   ,w  ,10,0x64df);
   		FillReg(x   ,64+y   ,w  ,10,0x64df);
   		PIC(Bot,x+14*3*Pos+12,y+30,0);
   		while( HAL_GPIO_ReadPin(TP_IRQ_GPIO_Port, TP_IRQ_Pin) == 0 );
    	break;

    	case 2://V
   		FillReg(x+2,y+2,w-2,35,colorF);
   		ILI9341_Draw_String(xt+1,yt+1,colorT,colorF,str,4);
   		FillReg(x   ,y   ,8 ,w  ,0xFC00);
  		FillReg(x+8 ,y+8 ,8 ,w-8,BLACK);
   		FillReg(x	,y	 ,24,8  ,0xFC00);
  		FillReg(x   ,y+w ,24,8  ,0xFC00);
   		FillReg(x+16,y   ,8 ,w  ,0xFC00);
    	break;

    	case 3://H
    	PIC(Bot,x+30,y+14*3*Pos+12,0);
    	break;
    }
}
unsigned char BCO_Activo(unsigned int x,unsigned int y,unsigned char Est,unsigned char Largo)
{
    uint16_t w=Largo*14*3/Largo;
    uint16_t i;

    x=x+8;

    if(Est==0)
    for(i=0;i<Largo;i++)
    {
    	if
		(
			(m_XPanel>(x+i*w) && m_XPanel<(x+(i+1)*w) )
			&&
			(m_YPanel>y && m_YPanel<(y+62) )
		) return i+1;
    }
    else
   	for(i=0;i<Largo;i++)
    {
    	if
		(
			(m_XPanel>x 	  && m_XPanel<(x+62) )
			&&
			(m_YPanel>(y+i*w) && m_YPanel<(y+(i+1)*w) )
		) return i+1;
    }

    return 0;
}
//////////////////////////////////////////////////////////
// BOTON PIC DESDE FLASH INTERNA
//////////////////////////////////////////////////////////
void BOT(unsigned char* ID,unsigned int x,unsigned int y,unsigned char Est)
{
    switch(Est)
    {
    	case 0://Arriba
   		PIC(ID,x,y,0);
    	//HAL_Delay(255);
    	break;

    	case 1://Abajo
       	PIC(ID,x,y,4);
       	//HAL_Delay(255);
    	break;

    	case 2: //Baja, espera y va arriba)
    	PIC(ID,x,y,4);
    	HAL_Delay(255);
    	PIC(ID,x,y,0);
    	//HAL_Delay(200);
    	break;
    }
}

void CHECK_CIRC(unsigned int x,unsigned int y,char* a,char* b,unsigned char Est)
{
	ILI9341_Draw_String(x,y   ,WHITE,0x0010,a,2);
	ILI9341_Draw_String(x,y+15,WHITE,0x0010,b,2);
	x=x+76;
	y=y+16;
	switch(Est)
    {
    	case 0://FALSO
   		ILI9341_Draw_Circle(x,y,16,0x0000,1);
   		ILI9341_Draw_Circle(x,y,12,0xFFFF,1);
    	break;

    	case 1://VERDADERO
   		ILI9341_Draw_Circle(x,y,16,0x0000,1);
  		ILI9341_Draw_Circle(x,y,12,0xFFFF,1);
  		ILI9341_Draw_Circle(x,y,8 ,0x0000,1);
    	break;
    }
}

unsigned char CHECK_CIRC_Activo(unsigned int x,unsigned int y)
{
	x=x+60;
	if
    (
      (m_XPanel>x && m_XPanel<(x+32) )
      &&
      (m_YPanel>y && m_YPanel<(y+32) )
    )
	{
	   	HAL_Delay(200);
		return 1;
	}

    else return 0;
}


//////////////////////////////////////////////////////////
// BOTON TIPO WINDOWS
//////////////////////////////////////////////////////////
unsigned char BOW_PX(unsigned int x,unsigned int y,unsigned int m_PantallaDest,unsigned char LargoBoton,char* str)
{
	if(BOW_Activo(x,y,LargoBoton) )
	{
		BOW(x,y,2,LargoBoton,str);
		m_Pantalla= m_PantallaDest;
		return 1;
	}
	return 0;
}
void BOW(unsigned int x,unsigned int y,unsigned char Est,unsigned char LargoBoton,char* str)
{
	TP_CS_OFF;
	SD_CS_OFF;
	TFT_CS_ON;
    uint16_t colorF=LIGHTGREY;
    uint16_t colorT=0x0000;
    uint16_t xt=x+2+((LargoBoton-(unsigned char)strlen(str)))*7;
    uint16_t yt=y+3;
    uint16_t wx=LargoBoton*14;
    uint16_t wy=35;

    switch(Est)
    {
    	case 0://Arriba
   		FillReg(x+2,y+2,wx-2,35,colorF);
 		ILI9341_Draw_String(xt,yt,colorT,colorF,str,4);
   		FillReg(x   ,y	 ,wx  ,2  ,BLACK);
   		FillReg(x	,y	 ,2	  ,37 ,BLACK);
   		FillReg(x+wx,y	 ,2	  ,37 ,BLACK);
   		FillReg(x	,y+35,wx  ,2  ,BLACK);
   		FillReg(x+2 ,y+2 ,wx-4,2  ,WHITE);
   		FillReg(x+2	,y+2 ,2	  ,33 ,WHITE);
    	break;

    	case 1://Abajo
   		FillReg(x+2,y+2,wx-2,35,colorF);
   		ILI9341_Draw_String(xt+1,yt+1,colorT,colorF,str,4);
     	FillReg(x   ,y	 ,wx  ,2  ,BLACK);
      	FillReg(x	,y	 ,2	  ,37 ,BLACK);
      	FillReg(x+wx,y	 ,2	  ,37 ,BLACK);
      	FillReg(x	,y+35,wx  ,2  ,BLACK);
      	FillReg(x+2 ,y+33,wx-4,2  ,WHITE);
      	FillReg(x+wx-2,y+2,2  ,33 ,WHITE);
    	break;

    	case 2://Baja, espera y va arriba)
       	FillReg(x+2,y+2,wx-2,35,colorF);
       	ILI9341_Draw_String(xt+1,yt+1,colorT,colorF,str,4);
       	FillReg(x   ,y	 ,wx  ,2  ,BLACK);
       	FillReg(x	,y	 ,2	  ,37 ,BLACK);
       	FillReg(x+wx,y	 ,2	  ,37 ,BLACK);
       	FillReg(x	,y+35,wx  ,2  ,BLACK);
       	FillReg(x+2 ,y+33,wx-4,2  ,WHITE);
      	FillReg(x+wx-2,y+2,2  ,33 ,WHITE);

       	while( HAL_GPIO_ReadPin(TP_IRQ_GPIO_Port, TP_IRQ_Pin) == 0 );

       	HAL_Delay(255);

    	FillReg(x+2,y+2,wx-2,35,colorF);
    	ILI9341_Draw_String(xt,yt,colorT,colorF,str,4);
   		FillReg(x   ,y	 ,wx  ,2  ,BLACK);
   		FillReg(x	,y	 ,2	  ,37 ,BLACK);
   		FillReg(x+wx,y	 ,2	  ,37 ,BLACK);
   		FillReg(x	,y+35,wx  ,2  ,BLACK);
   		FillReg(x+2 ,y+2 ,wx-4,2  ,WHITE);
   		FillReg(x+2	,y+2 ,2	  ,33 ,WHITE);
   		HAL_Delay(55);
    	break;
    }
}




//////////////////////////////////////////////////////////
// SPI
//////////////////////////////////////////////////////////
uint8_t SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t Data)
{//Transmit Receive 1 byte
  *(__IO uint8_t *)&hspi->Instance->DR=Data;
  if(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE))
	  return *(__IO uint8_t *)&hspi->Instance->DR;
  return 0;
}

void SPI_TxByte(uint8_t data)
{//Transmit 1 byte
	SPI_TransmitReceive(&SD_SPI_HANDLE,data);
}

uint8_t SPI_RxByte(void)
{//Receive 1 byte
	uint8_t data=0xFF;
	return SPI_TransmitReceive(&SD_SPI_HANDLE,data);
}
void SPI_RxBytePtr(uint8_t *buff)
{
  *buff = SPI_RxByte();
}
//////////////////////////////////////////////////////////
// SD
//////////////////////////////////////////////////////////
unsigned char  readSD1(FIL* fp);
HAL_StatusTypeDef SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
DWORD clmt_clustSD (	/* <2:Error, >=2:Cluster number */
	FIL* fp,		/* Pointer to the file object */
	DWORD ofs		/* File offset to be converted to cluster# */
);
void mem_cpySD(void* dst, const void* src, UINT cnt);

extern FIL fil;
extern FATFS fs;
extern FATFS *pfs;

unsigned long m_PosFlashSD=0;
unsigned long m_PosFlashST=0;
unsigned long m_PosBK;

unsigned char LeerSDAbs(unsigned long Pos);
unsigned char LeerSDRel();



void StaticBox(unsigned int x,unsigned int y,unsigned char LargoBoton,char* str);
void StaticBox2(unsigned int x,unsigned int y,unsigned char Est,unsigned char LargoBoton,char* str,char* tit);

unsigned long	ReconocerInt(char *chr);
unsigned long	ReconocerFnInt(char *chr);
unsigned long	ResolverInt(char *chr);
double			ReconocerDouble(char *chSal);
double			ReconocerFnDouble(char* chSal);
double			ResolverDouble(char* orig);
unsigned long	ReconocerString(char* str,char ChSalida);

unsigned char 	TextOut(unsigned int x,unsigned int y,char *a);

unsigned char BOR(unsigned char fil,unsigned char col,unsigned char mo,char* a);

void DibujarTeclasFlechas(void);
void FondoTeclas(void);
void CalculoXY(void);

void PIC(unsigned char* ID,unsigned int x,unsigned int y,unsigned char Brillo)
{
	uint32_t	ii;
	uint16_t    cRGB[256],R,G,B,RGB;
	unsigned char data[512];
	unsigned int q,c,d,i;

	m_Memoria=ID;

	TP_CS_OFF;
	SD_CS_OFF;
	TFT_CS_ON;
	ILI9341_Set_Rotation(4);

	uint32_t AnchoBMP=LeerFlashAbs(0)<<8;
	AnchoBMP=AnchoBMP | LeerFlashRel();
	uint32_t AltoBMP = LeerFlashRel()<<8;
	AltoBMP=AltoBMP | LeerFlashRel();
	uint32_t Peso=AnchoBMP*AltoBMP;

	//sprintf(data,"Ancho %d Alto %d",AnchoBMP,AltoBMP);
	//ILI9341_Draw_String(0,80,WHITE,0x0010,data,4);

	LeerFlashRel();
	LeerFlashRel();
	LeerFlashRel();

	uint32_t LargoTablaColores= LeerFlashRel();
	if(LargoTablaColores==0) 	LargoTablaColores=256;

	for(i=0;i<LargoTablaColores;i++)
	{
		cRGB[i]=(uint16_t)LeerFlashRel()<<8;
		cRGB[i]=cRGB[i] | LeerFlashRel();

		if(Brillo !=0 )
		{
			R=cRGB[i]>>11;
			G=cRGB[i]>>5 & 0x3F;
			B=cRGB[i] & 0x1F;

			if((R+Brillo)<0x1F) R=R+Brillo;
			else			  	R=0x1f;
			if((G+Brillo)<0x3F) G=G+Brillo;
			else			  	G=0x3f;
			if((B+Brillo)<0x1F) B=B+Brillo;
			else			  	B=0x1f;
			cRGB[i]=R*2048+G*32+B;
			RGB==R*2048+G*32+B;

			//sprintf(data,"%X %X %X %X",R,G,B,RGB);
			//ILI9341_Draw_String(0,80,WHITE,0x0010,data,4);
		}
	}

	unsigned int x1=x;
	unsigned int y1=240-(unsigned int)AltoBMP-y;
	unsigned int x2=AnchoBMP+x-1;
	unsigned int y2=239-y;
	ILI9341_Set_Address(x1,y1,x2,y2);

	TFT_DC_COMMAND;
	data[0]=0x2C;
	HAL_SPI_Transmit(&lcd_spi, data, 1, 1);

	ii=0;
	do
	{
		q=LeerFlashRel();
		c=LeerFlashRel();
		for(i=0;i<q*2;i++)
		{
			data[i]=(uint8_t)(cRGB[c]>>8);
			i++;
			data[i]=(uint8_t)(cRGB[c] & 0x00FF);
		}
		TFT_DC_DATA;
		HAL_SPI_Transmit(&lcd_spi,data,q*2,100);
		ii=ii+q;

	}while(ii<Peso);

//	sprintf(data,"%X %X %X %X",R,G,B,RGB);
//	ILI9341_Draw_String(0,80,WHITE,0x0010,data,4);

	TFT_CS_OFF;
}

void f_BMP(char* File,unsigned int x,unsigned int y)
{
	uint32_t		i,ii,iii;
	uint8_t    		dataOut[512];
	unsigned char 	dataIn[256],color[4],c;
	unsigned int 	col_16,TablaCol[256];
	UINT 			br,ERROR_SD;
	FIL Fil;

	TP_CS_OFF;
	TFT_CS_OFF;
	SD_CS_ON;

	f_open(&Fil,File,FA_READ);
	f_read(&Fil,dataIn,54,&br);

	uint32_t	AnchoBMP=(unsigned int)dataIn[0x12]+(unsigned int)dataIn[0x13]*256;
	uint32_t	AltoBMP=(unsigned int)dataIn[0x16]+(unsigned int)dataIn[0x17]*256;
	uint32_t	OffsetArray=(unsigned int)dataIn[0x0A]+(unsigned int)dataIn[0x0B]*256;
	uint32_t	LargoTablaColores=(unsigned int)dataIn[0x2E]+(unsigned int)dataIn[0x2F]*256;

	for(i=0;i<LargoTablaColores;i++)
	{
		f_read(&Fil,color,4,&br);
		TablaCol[i]=(color[2]/8)*2048+(color[1]/4)*32+(color[0])/8;
	}

	f_lseek(&Fil,OffsetArray);
	uint32_t Peso=AnchoBMP*AltoBMP;

	SD_CS_OFF;
	TFT_CS_ON;
	ILI9341_Set_Rotation(4);
	unsigned int x1=x;
	unsigned int y1=240-(unsigned int)AltoBMP-y;
	unsigned int x2=AnchoBMP+x-1;
	unsigned int y2=239-y;
	ILI9341_Set_Address(x1,y1,x2,y2);
	TFT_DC_COMMAND;
	dataIn[0]=0x2C;
	HAL_SPI_Transmit(&lcd_spi, dataIn, 1, 1);
	TFT_CS_OFF;
	DWORD sect ;

	for(i=0;i<Peso;i=i+256)
	{
		TFT_CS_OFF;
		SD_CS_ON;

		f_read(&Fil,dataIn,256,&br);
		for(ii=0,iii=0;ii<256;ii++,iii++)
		{
			dataOut[iii]=TablaCol[dataIn[ii]]>>8;
			iii++;
			dataOut[iii]=TablaCol[dataIn[ii]];
		}
		SD_CS_OFF;
		TFT_CS_ON;
		TFT_DC_DATA;
		HAL_SPI_Transmit(&lcd_spi,dataOut,512,100);
		TFT_CS_OFF;
	}
	TFT_CS_OFF;
	SD_CS_ON;
	f_close(&Fil);
	SD_CS_OFF;

}


void FillReg(unsigned int x,unsigned int y,
			 unsigned int wx,unsigned int wy,
			 unsigned int color)
{
	SD_CS_OFF;
	TFT_CS_ON;

	uint16_t	i;
	uint32_t	Peso=wx*wy;

	uint8_t data[640];

	ILI9341_Set_Rotation(4);
	unsigned int x1=x;
	unsigned int y1=240-wy-y;
	unsigned int x2=wx+x-1;
	unsigned int y2=239-y;
	ILI9341_Set_Address(x1,y1,x2,y2);

	TFT_DC_COMMAND;
	data[0]=0x2C;
	HAL_SPI_Transmit(&lcd_spi, data, 1, 1);

	for(i=0;i<wx;i++)
	{
			data[i*2]=color>>8;
			data[i*2+1]=color & 0xff;
	}

	TFT_DC_DATA;
	for(i=0;i<wy;i++)
	{
		HAL_SPI_Transmit(&lcd_spi,data,wx*2,10);
	}

}


void PrintCentrado( unsigned char* Titulo,
                    unsigned char y,
                    unsigned int ColorChar,
                    unsigned int ColorFondo
                   )
{
    uint16_t x=(22-(unsigned char)strlen(Titulo))/2;
    ILI9341_Draw_String(x*14, y, ColorChar,ColorFondo,Titulo,4);
}

void FondoCartel(  unsigned char Y,unsigned int ColorFondo)
{
	FillReg(31,Y,280,Y+80,0x0000);
	FillReg(33,Y+2,278,Y+78,ColorFondo);
}
void AtencionTecladoNumerico()
{
    //asmAtencionTecladoNumerico();
}
void Encabezamiento3(char* str)
{
	FillReg(0,0,319,33,0x0010);
    uint16_t x=(22-(unsigned char)strlen(str))/2;
    ILI9341_Draw_String(x*14+7, 0, WHITE, 0x0010, str, 4);
}
void GraficarPWM(void)
{
}

void GraficarOSC(void)
{
  SD_CS_OFF;
  TFT_CS_ON;

  uint16_t i,x;
  uint8_t data[500];
  uint32_t ya0,yp0,ya1,yp1,ya2,yp2,ya3,yp3;

  ILI9341_Set_Rotation(0);
  ILI9341_Set_Address(0,0,239,319);

  TFT_DC_COMMAND;
  data[0]=0x2c;
  HAL_SPI_Transmit(&lcd_spi,data,1,10);
  TFT_DC_DATA;
  for(x=0;x<320;x++)
  {
	  if(x==0||x==50||x==100||x==150||x==200||x==250||x==300)  for(i=0;i<480;i=i+2) data[i]=0xff,data[i+1]=0xff;
	  else for(i=0;i<480;i=i+2) data[i]=0x064,data[i+1]=0xDF;

	  if(x<=300)
	  {
		  data[0]=0xff;
		  data[1]=0xff;
		  data[80]=0xff;
		  data[81]=0xff;
		  data[160]=0xff;
		  data[161]=0xff;
		  data[240]=0xff;
		  data[241]=0xff;
		  data[320]=0xff;
		  data[321]=0xff;
		  data[400]=0xff;
		  data[401]=0xff;
		  data[479]=0xff;
		  data[480]=0xff;
	  }

	  ya0=(OSCCanal1[x]/120+40)*2;
	  yp0=(OSCCanal1[x+1]/120+40)*2;

	  ya1=(OSCCanal2[x]/120+80)*2;
	  yp1=(OSCCanal2[x+1]/120+80)*2;

	  ya2=(OSCCanal3[x]/120+120)*2;
	  yp2=(OSCCanal3[x+1]/120+120)*2;

	  ya3=(OSCCanal4[x]/120+160)*2;
	  yp3=(OSCCanal4[x+1]/120+160)*2;

	  if(yp0>ya0) for(i=ya0;i<=yp0 & i<480;i=i+2) data[i]=RED>>8, data[i+1]=RED;
	  else for(i=yp0;i<=ya0 & i<480;i=i+2) data[i]=RED>>8, data[i+1]=RED;

	  if(yp1>ya1) for(i=ya1;i<=yp1 & i<480;i=i+2) data[i]=DARKGREEN>>8, data[i+1]=DARKGREEN;
	  else for(i=yp1;i<=ya1 & i<480;i=i+2) data[i]=DARKGREEN>>8, data[i+1]=DARKGREEN;

	  if(yp2>ya2) for(i=ya2;i<=yp2 & i<480;i=i+2) data[i]=YELLOW>>8, data[i+1]=YELLOW;
  	  else for(i=yp2;i<=ya2 & i<480;i=i+2) data[i]=YELLOW>>8, data[i+1]=YELLOW;

	  if(yp3>ya3) for(i=ya3;i<=yp3 & i<480;i=i+2) data[i]=PURPLE>>8, data[i+1]=PURPLE;
  	  else for(i=yp3;i<=ya3 & i<480;i=i+2) data[i]=PURPLE>>8, data[i+1]=PURPLE;

	  HAL_SPI_Transmit(&lcd_spi,data,480,10);
  }
}


void DPM( unsigned char* ID,
          unsigned int x,
          unsigned int y,
          unsigned char Signo,
          unsigned int Valor,
          unsigned char LargoEnPixeles,
          unsigned char offset,
          unsigned char QChar,
          unsigned int ColorChar,
          unsigned int ColorBk,
          unsigned char Pto,
          unsigned char ST)
{
  //asm bclr 1,KBISC
  //Reg1=(unsigned char)(x & 0x00FF);//;XL  Start
  //Reg2=(unsigned char)(x >>8);//;XH
  //Reg3=(unsigned char)(y & 0x00FF);//;YL  Start
  //Reg33=(unsigned char)(y >>8);//;YH
  //Reg4=LargoEnPixeles;
  //Reg5=(unsigned char)(ColorChar & 0x00FF);//;L
  //Reg6=(unsigned char)(ColorChar >>8);//;H
  //Reg7=(unsigned char)(ColorBk & 0x00FF);//;L
  //Reg8=(unsigned char)(ColorBk >>8);//;H
  //Reg9=(unsigned char)(Valor & 0x00FF);//;L
  //Reg10=(unsigned char)(Valor >>8);//;H
  //Reg11=QChar;//4 máximo    1 mínimo
  //Reg12=Pto;//lugar del punto  0=sin punto 1=0.0  2=0.00  3=0.000
  //Reg13=ST;//0  primera vez  /  //Reg13>0 imprimir valor
  //Reg14=offset;//0  primera vez  /  //Reg13>0 imprimir valor
  ////Param3=ID[0];
  ////Param2=ID[1];
  ////Param1=ID[2];
  //asm bset 1,KBISC
  //asmDPM();
}

void Fondo()
{
}

void FondoAzul()
{
	FillReg(0,0,319,202,0x64DF);//azul
}
void FondoBlanco()
{
	FillReg(0,0,319,200,WHITE);
}
void FondoTeclas()
{
	FillReg(0,200,319,39,0x64DF);//azul
}
void FondoCompletoAzul()
{
	FillReg(0,0,320,240,0x64DF);//azul
}

void DibujarTeclasFlechas()
{
	char str[4];

	FondoTeclas();
	str[0]='<';//0xA4;
	str[1]='-';//0xA5;
	str[2]=0;
	BOW(4,205,0,5,str);

	str[0]='+';//0xA6;
	str[1]=' ';//0xA7;
	str[2]=0;
	BOW(84,205,0,5,str);

	str[0]='-';//0xA8;
	str[1]=' ';//0xA9;
	str[2]=0;
	BOW(164,205,0,5,str);

	str[0]='-';//0xA5;
	str[1]='>';//0xA3;
	str[2]=0;
	BOW(244,205,0,5,str);
}


void Graficar(unsigned int x,unsigned int y,unsigned int orix,unsigned int oriy,unsigned int color)
{
	SD_CS_OFF;
	TFT_CS_ON;
	ILI9341_Set_Rotation(4);
	ILI9341_Draw_Pixel(orix+x,oriy-y,color);
}
void DPMChico(unsigned int x,unsigned int y,unsigned char LargoBoton,unsigned int ColorFont,unsigned int ColorFondo,char* str)
{

	SD_CS_OFF;
	TFT_CS_ON;

	ILI9341_Set_Rotation(4);

    uint16_t xt=x+2+(LargoBoton-(unsigned char)strlen(str))*7;
    uint16_t yt=y+3;
    uint16_t wx=LargoBoton*14;
    uint16_t wy=35;

    FillReg(x+2,y+2,wx-2,35,ColorFondo);
   	ILI9341_Draw_String(xt,yt,ColorFont,ColorFondo,str,4);
   	FillReg(x   ,y	 ,wx  ,2  ,0);
   	FillReg(x	,y	 ,2	  ,37 ,0);
   	FillReg(x+wx,y	 ,2	  ,37 ,0);
   	FillReg(x	,y+35,wx  ,2  ,0);
}

void StaticBox(unsigned int x,unsigned int y,unsigned char LargoBoton,char* str)
{
    uint16_t xt=x+2+(LargoBoton-(unsigned char)strlen(str))*7;
    uint16_t yt=y+3;
    uint16_t wx=LargoBoton*14;
    uint16_t wy=35;

    FillReg(x+2,y+2,wx-2,35,WHITE);
   	ILI9341_Draw_String(xt,yt,BLACK,WHITE,str,4);
   	FillReg(x   ,y	 ,wx  ,2  ,BLACK);
   	FillReg(x	,y	 ,2	  ,37 ,BLACK);
   	FillReg(x+wx,y	 ,2	  ,37 ,BLACK);
   	FillReg(x	,y+35,wx  ,2  ,BLACK);
}

void StaticBox2(unsigned int x,unsigned int y,unsigned char Est,unsigned char Largo,char* str,char* tit)
{
	uint16_t xt=x+((Largo-(unsigned char)strlen(tit))/2)*7+36;
    uint16_t yt=y-3;

    if(Est==0)  BOT(BOT_AIZQ,x,y+16,0),BOT(BOT_ADER,x+39+14*Largo,y+16,0);
    if(Est==1)  BOT(BOT_AIZQ,x,y+16,2);
    if(Est==2)  BOT(BOT_ADER,x+39+14*Largo,y+16,2);

	ILI9341_Draw_String(x,y, WHITE, 0x0010,tit,2);
	StaticBox(x+35,y+17,Largo,str);
}

unsigned char  StaticBox2_Activo(unsigned int x,unsigned int y,unsigned char LargoBoton)
{
    if
    (
      (m_XPanel>x && m_XPanel<(x+14*LargoBoton+37) )
      &&
      (m_YPanel>y && m_YPanel<(y+62) )
    ) return 1;

    if
    (
       (m_XPanel>(x+14*LargoBoton+37) && m_XPanel<(x+14*LargoBoton+74) )
       &&
       (m_YPanel>y && m_YPanel<(y+62) )
    ) return 2;

    return 0;
}

unsigned char  Zona_Activo(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
    if
    (
      (m_XPanel>x1 && m_XPanel<x2)
      &&
      (m_YPanel>y1 && m_YPanel<y2)
    ) return 1;
    return 0;
}

unsigned char  BOW_Activo(unsigned int x,unsigned int y,unsigned char LargoBoton)
{
    if
    (
      (m_XPanel>x && m_XPanel<(x+7+14*LargoBoton ) )
      &&
      (m_YPanel>y && m_YPanel<(y+30 ) )
    ) return 1;
    else return 0;
}
unsigned char  BOR_Activo(unsigned int fil,unsigned int col,unsigned char LargoBoton)
{
	return BOW_Activo(col*14,38*fil,LargoBoton*14);
}

unsigned char  BOT_Activo(unsigned char* ID,unsigned int x,unsigned int y)
{
	int	AnchoBMP=(unsigned int)ID[0]*256+(unsigned int)ID[1];
	int AltoBMP=(unsigned int)ID[2]*256+(unsigned int)ID[3];
    if
    (
      (m_XPanel>x && m_XPanel<(x+AnchoBMP) )
      &&
      (m_YPanel>y && m_YPanel<(y+AltoBMP) )
    ) return 1;
    else return 0;
}

unsigned char  BTT_Activo(unsigned char* ID,unsigned int x,unsigned int y,unsigned char Est)
{
    if(Est==0)
    {
        if
        (
            (m_XPanel>x && m_XPanel<(x+ID[0]) )
            &&
            (m_YPanel>y && m_YPanel<(y+ID[1]/2) )
        )   return 1;
        else return 0;
    }
    else
    {
        if
        (
            (m_XPanel>x && m_XPanel<(x+ID[0]) )
            &&
            (m_YPanel>(y+ID[1]/2) && m_YPanel<(y+ID[1]) )
        )   return 1;
        else return 0;
    }
}

void PEL(unsigned char* ID,unsigned int x,unsigned int y,unsigned char Esc)
{
    unsigned int xx;
    unsigned int yy;

    if(Esc==0)
    {
      xx=x;
      yy=y;
    }
    else
    {
      xx=x+ID[1];
      yy=y+ID[2];

    }
    f_PIC(&ID[3+3*Esc],xx,yy);
}

void CalcularMyB(void)
{/*
    Mx= 256./(((XXXXH*256)+XXXXL)-((//Reg18*256)+//Reg17));
    My= 136./(((YYYYH*256)+YYYYL)-((//Reg20*256)+//Reg19));
    Bx= 32.5-(Mx * ((//Reg18*256)+//Reg17));
    By= 44.5-(My * ((//Reg20*256)+//Reg19));*/
}


/////////////////////
// MEMORIA SD
/////////////////////
unsigned char LeerSDAbs(unsigned long Pos)
{
	TFT_CS_OFF;
	SD_CS_ON;
	m_PosFlashSD=Pos;
	f_lseek(&fil,m_PosFlashSD);
	m_PosFlashSD++;
	return readSD1(&fil);
}
unsigned char LeerSDRel()
{
	TFT_CS_OFF;
	SD_CS_ON;
	m_PosFlashSD++;
	return readSD1(&fil);
}
unsigned char readSD1(FIL* fp)
{
	DWORD clst, sect;
	UINT rcnt;
	BYTE csect,a;

	if ((fp->fptr % 512) == 0)
	{/* On the sector boundary? */
		csect = (BYTE)(fp->fptr / 512 & (fp->fs->csize - 1));	/* Sector offset in the cluster */
		if (!csect)
		{/* On the cluster boundary? */
			if (fp->fptr == 0)
			{/* On the top of the file? */
				clst = fp->sclust;/* Follow from the origin*/
			}
			else
			{/* Middle or end of the file */
				if (fp->cltbl) clst = clmt_clustSD(fp, fp->fptr);/*Get cluster# from the CLMT */
				else		   clst = get_fat(fp->fs, fp->clust);/*Follow cluster chain on the FAT*/
			}
			fp->clust = clst;	/* Update current cluster */
		}
		sect = clust2sect(fp->fs,fp->clust);	/* Get current sector */

		sect += csect;
		if (fp->dsect != sect)
		{/* Load data sector if not in cache */
			if (fp->flag & FA__DIRTY)
			{/* Write-back dirty sector cache */
				disk_write(fp->fs->drv, fp->buf.d8, fp->dsect, 1);
				fp->flag &= ~FA__DIRTY;
			}
			disk_read(fp->fs->drv, fp->buf.d8, sect, 1) ;
		}
		fp->dsect = sect;
	}
	a=fp->buf.d8[fp->fptr % 512];
	fp->fptr++;
	return a;
}
DWORD clmt_clustSD(	/* <2:Error, >=2:Cluster number */
	FIL* fp,		/* Pointer to the file object */
	DWORD ofs		/* File offset to be converted to cluster# */
)
{
	DWORD cl, ncl, *tbl;

	tbl = fp->cltbl + 1;	/* Top of CLMT */
	cl = ofs / 512 / fp->fs->csize;	/* Cluster order from top of the file */
	for (;;) {
		ncl = *tbl++;			/* Number of cluters in the fragment */
		if (!ncl) return 0;		/* End of table? (error) */
		if (cl < ncl) break;	/* In this fragment? */
		cl -= ncl; tbl++;		/* Next fragment */
	}
	return cl + *tbl;	/* Return the cluster number */
}

/////////////////////////////
// FUNCIONES SOBRE SD
/////////////////////////////
double	s(long renglon,long pos);
double	r(long renglon,long pos);
double	t(long renglon,long pos);
double	l(long renglon,long puntero);
double	um(double ref,double dato);
double	us(double ref,double dato);
double	ul(double ref,double dato);
double	CA2(double valor);
long	G();
long	H(long pos);
long	L(long pos);
long	I(long pos);
long	UM(long ref,long dato);
long	US(long ref,long dato);
long	UL(long ref,long dato);
long	BI(long renglon,long puntero);
void	TeclaArriba();
void	TeclaAbajo();
void	TeclaAtras();
void	TeclaAdelante();

unsigned long ReconocerString(char* str1,char ChSalida)
{
	unsigned long col=0;

	do
	{
		str1[col]=LeerSDRel();
		col++;

	}while(	str1[col-1]!=ChSalida && str1[col-1]!=0);
	str1[col-1]=0;
	return col;
}

unsigned long ReconocerInt(char *chr)
{

	unsigned long Decimal=1,P0=0;
	char ch;
	do
	{
		ch=LeerSDRel();

		if(ch=='x') Decimal=0,P0=0;
		if(ch=='X') Decimal=0,P0=0;
		if
		(
			Decimal==0	&&
			(
				(ch>=0x30 && ch<=0x39)	||
				ch=='a'	|| ch=='A'	||
				ch=='b'	|| ch=='B'	||
				ch=='c'	|| ch=='C'	||
				ch=='d'	|| ch=='D'	||
				ch=='e'	|| ch=='E'	||
				ch=='f'	|| ch=='F'
			)
		)
		{
			if(ch=='a'||ch=='A') P0=P0*16+10;
			if(ch=='b'||ch=='B') P0=P0*16+11;
			if(ch=='c'||ch=='C') P0=P0*16+12;
			if(ch=='d'||ch=='D') P0=P0*16+13;
			if(ch=='e'||ch=='E') P0=P0*16+14;
			if(ch=='f'||ch=='F') P0=P0*16+15;
   			if((ch>=0x30 && ch<=0x39)) P0=P0*16+(long)ch-0x30;
		}
		if
		(
			Decimal==1 &&
			ch>=0x30 &&
			ch<=0x39

		)	P0=P0*10+(long)ch-0x30;

	}while
	(
		(ch>=0x30 && ch<=0x39) ||
		ch=='a'	|| 	ch=='b'	||	ch=='c'	||
    	ch=='d'	||	ch=='e'	||	ch=='f'	||
		ch=='A'	|| 	ch=='B'	||	ch=='C'	||
		ch=='D'	||	ch=='E'	||	ch=='F'	||
		ch=='x'	||	ch=='X'	||	ch==' '
	);
	*chr=ch;
	return P0;
}

unsigned long ReconocerFnInt(char *chr)
{
	unsigned long fin=0,i=0,P0,P1,S;
		char ch;

		if((S=ReconocerInt(&ch))==0)
		{
			if(ch=='U')
			{
				ch=LeerSDRel();
				if(ch=='M')
				{
					ch=LeerSDRel();
					P0=ResolverInt(&ch);
					P1=ResolverInt(&ch);
					S=UM(P0,P1);
					ch=LeerSDRel();
					*chr=ch;
					return S;
				}
				if(ch=='S')
				{
					ch=LeerSDRel();
					P0=ResolverInt(&ch);
					P1=ResolverInt(&ch);
					S=US(P0,P1);
					ch=LeerSDRel();
					*chr=ch;
					return S;
				}
				if(ch=='L')
				{
					ch=LeerSDRel();
					P0=ResolverInt(&ch);
					P1=ResolverInt(&ch);
					S=UL(P0,P1);
					ch=LeerSDRel();
					*chr=ch;
					return S;
				}
			}

			if(ch=='I')
			{
				ch=LeerSDRel();
				P0=ResolverInt(&ch);
				S=I(P0);
				ch=LeerSDRel();
				*chr=ch;
		 		return S;
			}
			if(ch=='H')
			{
				ch=LeerSDRel();
				P0=ResolverInt(&ch);
				S=H(P0);
				ch=LeerSDRel();
				*chr=ch;
		 		return S;
			}
			if(ch=='L')
			{
				ch=LeerSDRel();
				P0=ResolverInt(&ch);
				S=L(P0);
				ch=LeerSDRel();
				*chr=ch;
		 		return S;
			}

			if(ch=='G')
			{
				ch=LeerSDRel();
				S=G();
				ch=LeerSDRel();
				*chr=ch;
		 		return S;
			}

			if(ch=='(')
			{
				S=ResolverInt(&ch);
				ch=LeerSDRel();
				*chr=ch;
		 		return S;
			}
		}
		*chr=ch;
 		return S;
}

unsigned long	ResolverInt(char *chr)
{
	unsigned long	T,ST,in=0;
	char ch;

	ST=ReconocerFnInt(&ch);
	T=0;

	do
	{

		if(ch=='&')	ST=ST&ReconocerFnInt(&ch);
		if(ch=='|')	ST=ST|ReconocerFnInt(&ch);
		if(ch=='*')	ST=ST*ReconocerFnInt(&ch);
		if(ch=='/')	ST=ST/ReconocerFnInt(&ch);
		if(ch=='+')	T=T+ST,ST=ReconocerFnInt(&ch);
		if(ch=='-')	T=T+ST,ST=-ReconocerFnInt(&ch);
		if(ch==')' || ch==',')	T=T+ST;

	}while(ch!=',' && ch!=')');

	*chr=ch;

	return T;
}

double ReconocerDouble(char *chSal)
{

			long Decimal=1,fr=0,frs=0,i;
			double	P0=0;
			char ch;
			do
			{
				ch=LeerSDRel();

				if(ch=='x') Decimal=0,P0=0;
				if(ch=='X') Decimal=0,P0=0;
				if(ch=='.') fr=1;
				if
				(
					Decimal==0	&&
					(
						(ch>=0x30 && ch<=0x39)	||
						ch=='a'	|| ch=='A'	||
						ch=='b'	|| ch=='B'	||
						ch=='c'	|| ch=='C'	||
						ch=='d'	|| ch=='D'	||
						ch=='e'	|| ch=='E'	||
						ch=='f'	|| ch=='F'
					)
				)
				{
					if(ch=='a'||ch=='A') P0=P0*16+10;
					if(ch=='b'||ch=='B') P0=P0*16+11;
					if(ch=='c'||ch=='C') P0=P0*16+12;
					if(ch=='d'||ch=='D') P0=P0*16+13;
					if(ch=='e'||ch=='E') P0=P0*16+14;
					if(ch=='f'||ch=='F') P0=P0*16+15;
					if((ch>=0x30 && ch<=0x39)) P0=P0*16+(long)ch-0x30;

					if(fr==1) frs++;
				}
				if
				(
					Decimal==1 &&
					ch>=0x30 &&
					ch<=0x39
				)
				{
					P0=P0*10+(long)ch-0x30;
					if(fr==1) frs++;
				}

			}while
			(
				(ch>=0x30 && ch<=0x39) ||
				ch=='a'	|| 	ch=='b'	||	ch=='c'	||
				ch=='d'	||	ch=='e'	||	ch=='f'	||
				ch=='A'	|| 	ch=='B'	||	ch=='C'	||
				ch=='D'	||	ch=='E'	||	ch=='F'	||
				ch=='x'	||	ch=='X'	||	ch==' '	||
				ch=='.'
			);
			*chSal=ch;

			for(i=frs;i>0 && Decimal==1;i--) P0=P0/10;
			for(i=frs;i>0 && Decimal==0;i--) P0=P0/16;

			return P0;
}
double	ReconocerFnDouble(char *chr)
{
		long	fin=0,i=0,P0,P1;
		double	R0,R1;
		double S;
		char ch;

		if((S=ReconocerDouble(&ch))==0)
		{
			if(ch=='s')
			{
				ch=LeerSDRel();
				P0=ResolverInt(&ch);
				P1=ResolverInt(&ch);
				S=s(P0,P1);
				ch=LeerSDRel();
			}
			if(ch=='r')
			{
				ch=LeerSDRel();
				P0=ResolverInt(&ch);
				P1=ResolverInt(&ch);
				S=r(P0,P1);
				ch=LeerSDRel();
			}
			if(ch=='t')
			{
				ch=LeerSDRel();
				P0=ResolverInt(&ch);
				P1=ResolverInt(&ch);
				S=t(P0,P1);
				ch=LeerSDRel();
			}
			if(ch=='p')
			{
				ch=LeerSDRel();
				P0=ResolverInt(&ch);
				S=p(P0);
				ch=LeerSDRel();
			}

			if(ch=='u')
			{
				ch=LeerSDRel();
				if(ch=='m')
				{
					ch=LeerSDRel();
					R0=ResolverDouble(&ch);
					R1=ResolverDouble(&ch);
					S=um(R0,R1);
					ch=LeerSDRel();
				}
				if(ch=='s')
				{
					ch=LeerSDRel();
					R0=ResolverDouble(&ch);
					R1=ResolverDouble(&ch);
					S=us(R0,R1);
					ch=LeerSDRel();
				}
				if(ch=='l')
				{
					ch=LeerSDRel();
					R0=ResolverDouble(&ch);
					R1=ResolverDouble(&ch);
					S=ul(R0,R1);
					ch=LeerSDRel();
				}
			}
			if(ch=='l')
			{
				ch=LeerSDRel();
				P0=ResolverInt(&ch);
				P1=ResolverInt(&ch);
				S=l(P0,P1);
				ch=LeerSDRel();
			}
			if(ch=='(')
			{
				S=ResolverDouble(&ch);
				ch=LeerSDRel();
			}
		}

		*chr=ch;
		return S;
}

double	ResolverDouble(char *chr)
{
	double	T,ST;
	char ch;

	ST=ReconocerFnDouble(&ch);
	T=0;
	do
	{
		if(ch=='*')	ST=ST*ReconocerFnDouble(&ch);
		if(ch=='/')	ST=ST/ReconocerFnDouble(&ch);
		if(ch=='+')	T=T+ST,ST=ReconocerFnDouble(&ch);
		if(ch=='-')	T=T+ST,ST=-ReconocerFnDouble(&ch);
		if(ch==')' || ch==',')	T=T+ST;

	}while(ch!=',' && ch!=')');

	*chr=ch;

	return T;
}

unsigned char TextOut(unsigned int x,unsigned int y,char *a)
{
	ILI9341_Draw_String(x,y,WHITE,0x643F,a,4);
	return 0;
}

unsigned char Display(unsigned char fila,unsigned char columna,unsigned char modo, char *a)
{
    unsigned int  x,y,anch_car,anch_ren;
    unsigned int  ColorCaracter,ColorFondo;
    int   pt=0,i;
    char  code_char, resalte;
    unsigned int  coli;

    anch_car=14;
    anch_ren=24;

    x=(unsigned int)columna*anch_car;
    y=240-(unsigned int)fila*anch_ren+5;

    switch (modo)
    {
            case 0:
            ColorCaracter=0xFFFF;
            ColorFondo=0x64DF;

            for(pt=0;pt!=23 && a[pt]!=0; pt++)   /*envío los datos al display*/
            {
                code_char = a[pt];
                if (x<320 && y<180)
                ILI9341_Draw_Char(x,y,ColorCaracter,ColorFondo,code_char,4);
                x=x+anch_car;
            }
            for(;pt!=23; pt++)   /*envío los datos al display*/
            {
                code_char = 0x20;
                if (x<320 && y<180) ILI9341_Draw_Char(x,y,ColorCaracter,ColorFondo,code_char,4);
                x=x+anch_car;
            }
            break;

            case 1:
            ColorCaracter=0x001F;
            ColorFondo=0xFFFF;

            for(pt=0;pt!=23 && a[pt]!=0; pt++)   /*envío los datos al display*/
            {
                code_char = a[pt];
                if (x<320 && y<180) ILI9341_Draw_Char(x,y,ColorCaracter,ColorFondo,code_char,4);
                x=x+anch_car;
            }
            for(;pt!=23; pt++)   /*envío los datos al display*/
            {
                code_char = 0x20;
                if (x<320 && y<180) ILI9341_Draw_Char(x,y,ColorCaracter,ColorFondo,code_char,4);
                x=x+anch_car;
            }
            break;

            case 2:
            ColorCaracter=0xFFFF;
            ColorFondo=0x64DF;

            do
            {
                code_char = a[pt];
                if (x<320 && y<180) ILI9341_Draw_Char(x,y,ColorCaracter,ColorFondo,code_char,4);
                columna++;

                if ( (columna>18 && a[pt]==0x20) || columna>21)
                {
                    fila++;
                    columna=0;
                }
                x=(unsigned int) columna*anch_car;
                y=(unsigned int) fila*anch_ren+5;
                pt++;

            }while  (a[pt]!=0 && fila<8);
            break;

            case 3:
            ColorCaracter=0xFFFF;
            ColorFondo=0x64DF;

            do
            {
                x=(unsigned int)columna*anch_car;
                y=(unsigned int)fila*anch_ren+5;
                code_char = a[pt];
                if(code_char!=0 && x<320 && y<180) ILI9341_Draw_Char(x,y,ColorCaracter,ColorFondo,code_char,4);
                if(code_char==0 && x<320 && y<180) ILI9341_Draw_Char(x,y,ColorCaracter,ColorFondo,0x20,4);
                pt++;
                columna++;
                if( (columna>18 && a[pt]==0x20) || columna>21 || a[pt]==0)
                {
                    for(i=columna;i<23;i++)
                    {
                        x=(unsigned int)i*anch_car;
                        if(x<320 && y<180) ILI9341_Draw_Char(x,y,ColorCaracter,ColorFondo,code_char,4);
                    }
                    columna=0;
                    if(a[pt]==0x20) pt++;
                    if(a[pt]!=0) fila++;
                }

            }while( a[pt]!=0 && fila<9 );
            break;

            case 4:
            ColorCaracter=0xfFf0;
            ColorFondo=0x00f0;

            for(pt=0;pt!=23 && a[pt]!=0; pt++)   /*envío los datos al display*/
            {
                code_char = a[pt];
                if (x<320 && y<180) ILI9341_Draw_Char(x,y,ColorCaracter,ColorFondo,code_char,4);
                x=x+anch_car;
            }
            for(;pt!=23; pt++)   /*envío los datos al display*/
            {
                code_char = 0x20;
                if (x<320 && y<180) ILI9341_Draw_Char(x,y,ColorCaracter,ColorFondo,code_char,4);
                x=x+anch_car;
            }
            break;
   }
   return  fila;
}


/*

char BuscarCodigo(long punt)
{
	char    str[100],ch;
	long	fin=0,i=0;
	char	a,b,c,d,e,enc=0,TipoBusq=0;
	enc=0;

	FondoCartel(46,0x643F);
	TextOut(40,95,"BUSCANDO");

    HAL_Delay(1000);

 	e=LeerSDAbs(punt);
	do
	{
		a=b;
		b=c;
		c=d;
		d=e;
		e=LeerSDRel();

		if( b=='C' && c=='O' && d=='D' && e=='(')
		{
			ReconocerString(str,',');

			for(i=0;i<15;i++)
			{
				if(BufferBus[i]==0x20) BufferBus[i]=0;
			}
			if(BusquedaStr(str,m_TecladoStr) ) enc=1;

			if(enc==1)
			{
				sprintf(m_CodSel,"%s",str);
				ReconocerString(str,',');
				m_IDPrg=ReconocerInt(&ch);
				m_Bmp=ReconocerInt(&ch);
                m_BmpB=ReconocerInt(&ch);
    			m_BmpC=ReconocerInt(&ch);
				m_BmpD=ReconocerInt(&ch);
				m_Var0=ReconocerInt(&ch);
				m_Var1=ReconocerInt(&ch);
                m_Var2=ReconocerInt(&ch);
                m_Var3=ReconocerInt(&ch);
				m_punteroBus=m_PosFlashSD;
			}
			else
			{
				m_PosFlashSD=m_PosFlashSD+100;
			}
		}
		if( b=='F' && c=='I' && d=='G' && e=='(')
		{
			TipoBusq=1;
		}
		if( b=='M' && c=='A' && d=='R' && e=='(' && TipoBusq==1)
		{
			enc=2;
		}
		if( b=='C' && c=='I' && d=='L')
		{
			enc=2;
		}
		if( b=='B' && c=='M' && d=='P')
		{
			enc=2;
		}


	}while(	enc==0 );

	if(enc==1)	return 1;
	else		return 0;
}
*/
char BusquedaStr(char *a, char *b)
{
	int ia,ib,c=0,lb;

	lb=strlen(b);

	for(ia=0;ia+lb<21 && c==0;ia++)
	{
		 c=1;
		 for(ib=0;ib<lb && c==1;ib++)
		 {
			if(a[ia+ib]!=b[ib]) c=0;
		 }
	}
	return c;
}

/////////////////////
// MEMORIA FLASH
/////////////////////

unsigned char LeerFlashAbs(unsigned long Pos);
unsigned char LeerFlashRel();

unsigned char LeerFlashAbs(unsigned long Pos)
{
	m_PosFlashST=Pos;
	m_PosFlashST++;
	return m_Memoria[Pos];
}

unsigned char LeerFlashRel()
{
	unsigned char a=m_Memoria[m_PosFlashST];
	m_PosFlashST++;
	return a;
}
unsigned long Vin2Res(unsigned long in,unsigned long R)
{
	unsigned long Num=R*in;
	unsigned long Den=4070-in;
	return Num/Den;
}

unsigned long Ajustar(unsigned long Valor,unsigned long Numerador,unsigned long Denominador)
{
	return Valor*Numerador/Denominador;
}

void Presentacion(char* Marca,char* Modelo)
{

}

void MenuSet(unsigned char MenuNro,char* Buffer,unsigned int PantallaProx)
{
	int i;

	if(MenuNro==0) m_MenuMax=0,m_PantallaExt=0;
	for(i=0;Buffer[i]!=0 && i<20;i++) m_MenuStr[MenuNro][i]=Buffer[i];
	for(;i<20;i++) m_MenuStr[MenuNro][i]=' ';
	if(MenuNro>m_MenuMax) m_MenuMax=MenuNro;
	m_PantallaProx[MenuNro]=PantallaProx;
}

void MenuPrint()
{

	int i;

	for(i=0;(i+m_PantallaExt*6)<=m_MenuMax && i<6;i++)
		BOW(5,35+33*i,0,22,m_MenuStr[i+m_PantallaExt*6]);

	//BOW(5,202,0,7,"SUBIR");
   	BOW(5,202,0,22,"VOLVER");
   	//BOW(215,202,0,7,"BAJAR");
}

void MenuAtencion(void)
{
	for(int i=0;i<6 && (i+m_PantallaExt*6)<=m_MenuMax;i++)
	{
		if(BOW_Activo(6,35+33*i,22) )
		{
			BOW(5,35+33*i,2,22,m_MenuStr[6*m_PantallaExt+i]);
			m_Pantalla=m_PantallaProx[6*m_PantallaExt+i];
		}
	}
	/*
	if(BOW_Activo(5,202,7) )
	{
		if(m_PantallaExt>0) m_PantallaExt--;
		MenuPrint();
		BOW(5,202,2,7,"SUBIR");
	}*/
	if(BOW_Activo(5,202,22) )
	{
		BOW(5,202,2,22,"VOLVER");
		m_Pantalla=MENU_PRINCIPAL;
	}
	/*
	if(BOW_Activo(215,202,7) )
	{
		if(m_PantallaExt < m_MenuMax/6) m_PantallaExt++;
		MenuPrint();
		BOW(215,202,2,7,"BAJAR");
	}
	*/
}
void ChicharraError(void)
{

}
unsigned int MedirVerde(void)
{
	return 0;
}
unsigned int MedirBlanco(void)
{
	return 0;
}
unsigned int MedirCorriente(void)
{
	return 0;
}

