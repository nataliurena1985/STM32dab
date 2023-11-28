#include <stdio.h>
#include <string.h>
//#include "Recursos.h"
#include <math.h>
#include "Panel Tactil.h"

char Idioma(void);

//Seleciono ac� el idioma
#define ESP//ESP  //POR  //ING  //ITA

const char BIOS[]="BIOS";  

#ifdef ESP

unsigned int m_NroIdioma=0;

const char PRUEBA[]="PRUEBA";
const char PROBAR_AUTOPARTE[]="PROBAR AUTOPARTE";

const char BUSCAR_POR_CODIGO[]="BUSCAR POR CODIGO";
const char CODIGO[]="CODIGO";
const char BUSCAR[]="BUSCAR";
const char BUSCANDO[]=		   "     BUSCANDO    ";
const char SIN_COINCIDENCIAS[]="SIN COINCIDENCIAS";
const char REPETIR[]="REPETIR";
const char INGRESAR[]="INGRESAR";
const char MENU_PRINCIPAL[]="MENU PRINCIPAL";
const char CONECTAR_AL_REGULADOR[]="CONECTAR AL REGULADOR";
const char SOPORTE[]="SOPORTE";
const char MANUAL_DE_USO[]="MANUAL DE USO";
const char SOPORTE_TECNICO[]="SOPORTE TECNICO";
const char SOLO_PARA_EL_USO_DEL[]="SOLO PARA EL USO DEL";
const char PERSONAL_TECNICO[]="PERSONAL TECNICO.   ";
const char TENSION_MEDIDA[]="TENSION MEDIDA";
const char TENSION_DESEADA[]="TENSION DESEADA";
const char CLAVE[]="CLAVE";
const char CONECTAR[]="CONECTAR";
const char BORRAR_CONFIGURACION[]="BORRAR CONFIGURACION";


const char PWM_[]="PWM";
const char FREC_[]="FREC";

const char VOLVER[]="VOLVER";
const char SALIR[]="SALIR";
char Idioma()
{
    return 'S';
}

#endif

#ifdef ING

unsigned int m_NroIdioma=1;

const char INGRESAR[]="ENTER";
const char MENU_PRINCIPAL[]="MAIN MENU";
const char CONECTAR_AL_REGULADOR[]="CONNECT REGULATOR";
const char SOPORTE[]="SUPPORT";
const char MANUAL_DE_USO[]="USER MANUAL";
const char SOPORTE_TECNICO[]="TECHNICAL SUPPORT";
const char SOLO_PARA_EL_USO_DEL[]="ONLY FOR THE USE OF ";
const char PERSONAL_TECNICO[]    ="TECHNICAL STAFF.    ";
const char TENSION_MEDIDA[]="MEASURED VOLTAGE";
const char TENSION_DESEADA[]="SET VOLTAGE";
const char VOLVER[]="RETURN";
char Idioma()
{
    return 'E';
}

#endif

#ifdef ITA

unsigned int m_NroIdioma=2;

const char INGRESAR[]="ACCEDERE";
const char MENU_PRINCIPAL[]="MENU PRINCIPALE";
const char CONECTAR_AL_REGULADOR[]="COLLEGARE REGOLATORE";
const char SOPORTE[]="SUPPORTO";
const char MANUAL_DE_USO[]="MANUALE D'USO";
const char SOPORTE_TECNICO[]="SUPPORTO TECNICO";
const char SOLO_PARA_EL_USO_DEL[]="SOLO PER L'USO DI   ";
const char PERSONAL_TECNICO[]="PERSONALE TECNICO   ";
const char TENSION_MEDIDA[]="TENSIONE MISURATA";
const char TENSION_DESEADA[]="TENSIONE DESIDERATA";
const char VOLVER[]="TORNARE";
char Idioma()
{
    return 'I';
}
#endif

#ifdef POR

unsigned int m_NroIdioma=3;

const char PRUEBA[]="TESTE";

const char BUSCAR_POR_CODIGO[]="PESQUISA X CODIGO"; 
const char CODIGO[]="CODIGO"; 
const char BUSCAR[]="PESQUISAR";
const char BUSCANDO[]		  ="   PESQUISANDO   "; 
const char SIN_COINCIDENCIAS[]="SEM COINCIDENCIAS"; 
const char REPETIR[]="REPETIR"; 

const char INGRESAR[]="ENTRAR";                                            
const char MENU_PRINCIPAL[]="MENU PRINCIPAL";
const char CONECTAR_AL_REGULADOR[]="LIGAR AO REGULADOR";
const char SOPORTE[]="SUPORTE";                         
const char MANUAL_DE_USO[]="MANUAL DO USUARIO";         
const char SOPORTE_TECNICO[]="SUPORTE TECNICO";         
const char SOLO_PARA_EL_USO_DEL[]="SOMENTE USO DE      ";     
const char PERSONAL_TECNICO[]    ="PESSOAL TECNICO     ";         
const char TENSION_MEDIDA[]="TENSAO MEDIDA";            
const char TENSION_DESEADA[]="TENSAO DESEJADA";         
const char VOLVER[]="VOLTAR ";
char Idioma()
{
    return 'P';
}
#endif



