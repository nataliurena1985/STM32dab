#define MENU_PRINCIPAL                  4

#define ARANCAR                         10
#define CONFIGURACION                   20

#define VISOR							2000
#define DES_DAB							2100
#define DES_MPPT						2200
#define DES_INVERTER 					2300
#define VISOR3							2400
#define VISOR4							2500
#define VISOR5							2600

#define OSCILOSCOPIO					3000

#define CONFIGURAR						5000

#define HARDWARE						5100
#define HARDWARE_PASS					5110
#define HARDWARE_VERIF_PASS				5120
#define HARDWARE_SET_SISTEMA			5130
#define HARDWARE_SIS_POTENCIA			5140
#define HARDWARE_SIS_COMPONENTES		5150

#define HARDWARE_PRESET_INVERTER		5160
#define HARDWARE_PRESET_INVERTER0		5162
#define HARDWARE_PRESET_MPPT			5170
#define HARDWARE_PRESET_MPPT0			5172
#define HARDWARE_PRESET_DAB				5180
#define HARDWARE_PRESET_DAB0			5182
#define HARDWARE_PRESET_DAB1			5184

#define WIFI							5200
#define WIFI_SSID						5210
#define WIFI_PASS						5220
#define WIFI_CONNECT					5230
#define WIFI_CLR_CONF					5240
#define WIFI_MANDAR_SSID				5250
#define WIFI_MANDAR_PASS				5260

extern char m_Conf;

void CHECK_CIRC(unsigned int x,unsigned int y,char* a,char* b,unsigned char Est);
unsigned char  CHECK_CIRC_Activo(unsigned int x,unsigned int y);

void DPMChico(unsigned int x,unsigned int y,unsigned char LargoBoton,unsigned int ColorFont,unsigned int ColorFondo,char* str);

void f_TECLADO_ALFA_open(FIL* f);
void f_TECLADO_ALFA_fondo(FIL* f);
void f_TECLADO_ALFA_Input(FIL* fi,unsigned int PantallaSI,unsigned int PantallaNO);

void f_PEL_open(FIL* f,char* Dir,char* Name,unsigned int QEsc);
void f_PEL(FIL* Pel,unsigned int x,unsigned int y,unsigned int offx,unsigned int offy,unsigned char Esc);
void f_PEL_close(FIL* f,unsigned int QEsc);


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

unsigned long Vin2Res(unsigned long in,unsigned long R);
unsigned long Ajustar(unsigned long Valor,unsigned long Numerador,unsigned long Denominador);

void MenuSet(unsigned char MenuNro,char* Buffer,unsigned int PantallaProx);
void MenuPrint(void);
void MenuAtencion(void);
extern unsigned int m_MenuMax;
extern char m_MenuStr[30][25];

char BuscarCodigo(long punt);
char BusquedaStr(char *a, char *b);
extern char m_TecladoStr[50];
extern unsigned int m_TecladoIndice;

unsigned char LeerSDAbs(unsigned long Pos);
unsigned char LeerSDRel();

unsigned char LeerFlashAbs(unsigned long Pos);
unsigned char LeerFlashRel();
extern unsigned char * m_Memoria;

extern char m_cache[2001];
extern unsigned long m_pcache;

void DibujarTeclasFlechas(void);
void DibujarConexionado(void);
void DibujarIcon(unsigned char Brillo);
void FondoTeclas(void);
void CalculoXY(void);

extern unsigned int m_IndicePel;

unsigned char Display(unsigned char fil,unsigned char col,unsigned char mode,char* a);

void Encabezamiento3(char* str);
void GraficarOSC(void);
void Graficar5(void);
void Graficar6(void);
void Graficar5G(void);
void Graficar5E(void);
void Graficar6E(void);
void GraficarPWM(void);
void Graficar(unsigned int x,unsigned int y,unsigned int orix,unsigned int oriy,unsigned int color);

void DPM(  unsigned char* ID,
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
           unsigned char ST);

void DibujarIcon(unsigned char Brillo);
void Fondo();
void FondoAzul();
void FondoBlanco();
void FondoTeclas();
void FondoCompletoAzul();
void MedAPunto(void);
void PrintCentrado( unsigned char* Titulo,
                    unsigned char y,
                    unsigned int ColorChar,
                    unsigned int ColorFondo
                   );

void FondoCartel(unsigned char Y, unsigned int ColorFondo);

void CalcularMyB(void); //  Calculo de las pendientes y ordenadas de calobración del panel tactil

void PanTecladoCodigo(unsigned char* Titulo,unsigned char QRen);
void AtencionTecladoCodigo();
void PanTecladoNumerico(unsigned char* Titulo);
void AtencionTecladoNumerico();

extern float  Mx, My, Bx, By;
extern unsigned int ValX,ValY;

void MuestraXY(void);
void CalculoXY(void);

extern unsigned long m_PosFlashSD;
extern unsigned long m_PosFlashST;
extern unsigned long m_PosBK;

void 	PIC(unsigned char* ID,unsigned int x,unsigned int y,unsigned char Brillo);

unsigned char BOW_PX(unsigned int x,unsigned int y,unsigned int PantallaDest,unsigned char LargoBoton,char* str);

unsigned char  StaticBox2_Activo(unsigned int x,unsigned int y,unsigned char LargoBoton);
unsigned char  Zona_Activo(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);

void StaticBox(unsigned int x,unsigned int y,unsigned char LargoBoton,char* str);
void StaticBox2(unsigned int x,unsigned int y,unsigned char Est,unsigned char LargoBoton,char* str,char* tit);

void PEL(unsigned char* ID,unsigned int x,unsigned int y,unsigned char Est);

void BOT(unsigned char* ID,unsigned int x,unsigned int y,unsigned char Est);
unsigned char BOT_Activo(unsigned char* ID,unsigned int x,unsigned int y);

void BTT(unsigned char* ID,unsigned int x,unsigned int y,unsigned char Est);
unsigned char BTT_Activo(unsigned char* ID,unsigned int x,unsigned int y,unsigned char Est);

void BCO(unsigned char* Bot,unsigned int x,unsigned int y,unsigned char Est,unsigned char Largo,unsigned char Pos,char* str);
unsigned char BCO_Activo(unsigned int x,unsigned int y,unsigned char Est,unsigned char Largo);

void	BTX(unsigned char* ID,unsigned int x,unsigned int y,unsigned char Est,unsigned char Largo,unsigned char* str);

void	BOW(unsigned int x,unsigned int y,unsigned char Est,unsigned char LargoBoton,char* str);
unsigned char BOW_Activo(unsigned int x,unsigned int y,unsigned char LargoBoton);

unsigned char BOR(unsigned char fil,unsigned char col,unsigned char mo,char* a);



void FillReg(unsigned int x,unsigned int y,
			 unsigned int wx,unsigned int wy,
		     unsigned int color);

unsigned long	ReconocerInt(char *chr);
unsigned long	ReconocerFnInt(char *chr);
unsigned long	ResolverInt(char *chr);
double	ReconocerDouble(char *chSal);
double	ReconocerFnDouble(char* chSal);
double	ResolverDouble(char* orig);
unsigned long	ReconocerString(char* str,char ChSalida);
unsigned char TextOut(unsigned int x,unsigned int y,char *a);

uint8_t SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t Data);
void SPI_TxByte(uint8_t data);
uint8_t SPI_RxByte(void);
void SPI_RxBytePtr(uint8_t *buff);

void ChicharraError(void);
unsigned int MedirVerde(void);
unsigned int MedirBlanco(void);
unsigned int MedirCorriente(void);



