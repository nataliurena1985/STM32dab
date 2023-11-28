#ifndef APP_H
#define APP_H

extern FIL fil;
extern FIL fPIC_BOB;//Bobina con diodo
//extern FIL fPIC_BOBI_DOB;
//extern FIL fPIC_BOBINA;//Bobina sin diodo
extern FIL fPIC_CORO_AZ;
extern FIL fPIC_CORO_VE;
extern FIL fPIC_CORO_BL;
extern FIL fPIC_CORO_AM;
extern FIL fPIC_POT_DOB;
extern FIL fPIC_CONE_VEC;
extern FIL fPIC_CONE_ROC;
extern FIL fPIC_CONE_NEC;
extern FIL fPIC_CONE_BLC;
extern FIL fPIC_ERROR;
extern FIL fPIC_RETI_CHI;
extern FIL fPEL1[24];
extern FIL fBOT[8];

extern FIL fCORREDERA[2];
extern unsigned int m_Frec;

extern unsigned char m_Canal;
extern unsigned char m_GenEolico;
extern unsigned char m_GrupoElectr;
extern unsigned int ValorPista1;
extern unsigned int ValorPista2;
extern void AppInit(void);
extern void AppLoop(void);
extern int opcion;
extern unsigned int m_ActivarPWM;
#endif

extern unsigned int m_Pantalla;
extern unsigned int m_PantallaExt;
extern unsigned int m_PantallaAnt;
extern unsigned int m_PantallaProx[];

extern unsigned char m_ManPag;
extern unsigned char m_ManUltimaPag;

extern unsigned char m_AnchoIcon; 
extern unsigned char m_TimerIter;
extern unsigned char m_NroPistas;
extern unsigned char m_Seleccion;
extern int opcion;
extern int EnPrueba;
extern int Time;
extern int m_CC;

extern int TiempoPruebaMin;
extern int TiempoPruebaSeg;
extern int andando;
extern int rot;
extern char Estado0,Estado1;
extern char m_Conf;

extern unsigned int m_PWM;
extern unsigned int m_FREC;		

extern unsigned  int m_x,m_y,m_z;
extern unsigned int m_yAnt,m_zAnt;
extern unsigned int m_PistaAscendente;
extern unsigned int m_ActivarPWM;
extern unsigned int m_Max;
extern unsigned int m_Min;
extern unsigned int m_RPM;
extern unsigned int m_ti;

extern int m_IndiceGel;
extern int m_IndiceSol;
extern int m_IndiceBat;
extern int m_IndicePot1;
extern int m_IndicePot2;

void GraficarPWM();

unsigned int dis(unsigned int a,unsigned int b);
unsigned int MedirPistaAsc(void);
unsigned int MedirPistaDes(void);

void NOP(void);


extern unsigned int m_PWMInicialAsc;		
extern unsigned int m_PWMInicialDes;		
extern unsigned int m_PWMFinalAsc;		
extern unsigned int m_PWMFinalDes;		
extern float m_mAsc;
extern float m_mDes;
