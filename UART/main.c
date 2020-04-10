// ******************************************************************************
// PROGRAMA	: main.c
// TARGET	: CC3200
// DESCRIPCION	: C�digo ejemplo de UART y LEDs de la placa
// AUTOR	: Jos� Manuel Cano Garc�a y Eva Gonz�lez Parada
// FECHA	: 10-12-16
// ESQUEMA  : Modo Activo-->Clock 80Mhz
// ******************************************************************************
// Includes de funciones est�ndar
#include <stdio.h>

// Includes de la biblioteca Driverlib para CC3200
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "hw_apps_rcm.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "gpio.h"
#include "utils.h"
#include "uart.h"

// Otros includes de ficheros desarrollados
#include "pinmux.h"


//*****************************************************************************
//                VARIABLES GLOBALES -- Inicio
//*****************************************************************************
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif


//*****************************************************************************
//                  VARIABLES GLOBALES -- Fin
//*****************************************************************************


//*****************************************************************************
//                     PROTOTIPOS DE FUNCIONES LOCALES -- Inicio
//*****************************************************************************
static void BoardInit(void);
void UARTIntHandler(void);

//*****************************************************************************
//                     PROTOTIPOS DE FUNCIONES LOCALES -- Fin
//*****************************************************************************

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//
//! Funci�n main
//!
//! Par�mentros: ninguno
//!
//! Retorno: ninguno
//
//*****************************************************************************
int
main()
{
    //
    // Inicializaci�n y configuraci�n de la placa
    //
    BoardInit();

    //
    // Configuraci�n del modo, corriente y direcci�n de los pines implicados
    //
    PinMuxConfig();

    // Apagado de LED1, LED2 y LED3 (rojo, amarillo y verde)
    GPIOPinWrite(GPIOA1_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);


    //Configuraci�n de los par�metros UART
    UARTConfigSetExpClk(UARTA0_BASE,PRCMPeripheralClockGet(PRCM_UARTA0),
                      115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));


    UARTFIFOEnable(UARTA0_BASE);
    UARTFIFOLevelSet(UARTA0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);


	IntRegister(INT_UARTA0,UARTIntHandler); //Asignaci�n en la Tabla de vectores del manejador de la interrupci�n de la UART
    UARTIntEnable(UARTA0_BASE, UART_INT_RX|UART_INT_RT); //Habilitaci�n de las  fuentes de interrupci�n de la UART
    IntEnable(INT_UARTA0); // Habilitaci�n de la interrupciones de la UART
    UARTEnable(UARTA0_BASE);//Habilitaci�n del m�dulo receptor y transmisi�n de la UART

    IntMasterEnable();//Habilita las interrupciones del procesador (relacionado con el registro PRIMASK)

    //while(1){ PRCMSleepEnter();}; // Funci�n para introducir al micro en BAJO CONSUMO
    while(1);
    return 0;
}

//*****************************************************************************
//*****************************************************************************



//*****************************************************************************
//         DEFINICI�N DE FUNCIONES y MENEJADORES DE INTERRUPCI�N -- Inicio
//*****************************************************************************

//*****************************************************************************
//
//! Inicializaci�n de la placa
//!
//! \Par�mentros: ninguno
//!
//! \Retorno: ninguno
//
//*****************************************************************************
static void
BoardInit(void)
{

#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif

    
    //
    // Configuraci�n del microcontrolador tras el encendido, RESET o Hibernaci�n
    //
    PRCMCC3200MCUInit();
}

//*****************************************************************************
//
//! Funci�n de Interrupci�n de la UART0
//!
//! \Par�mentros: ninguno
//!
//! \Retorno: ninguno
//
//*****************************************************************************


void UARTIntHandler(void)
{

	unsigned char rxChar;

	//Interrupci�n UART handler

	while(UARTCharsAvail(UARTA0_BASE))
	{
		rxChar=UARTCharGet(UARTA0_BASE);
		// Escritura de pins asociados a los leds verda, rojo y amarillo
		switch(rxChar)
		{
		case 'R':
		    GPIOPinWrite(GPIOA1_BASE,GPIO_PIN_1,0);
		    break;
		case 'r':
		    GPIOPinWrite(GPIOA1_BASE,GPIO_PIN_1,GPIO_PIN_1);
		    break;
		case 'A':
		    GPIOPinWrite(GPIOA1_BASE,GPIO_PIN_2,0);
		    break;
		case 'a':
		    GPIOPinWrite(GPIOA1_BASE,GPIO_PIN_2,GPIO_PIN_2);
		    break;
		case 'V':
		    GPIOPinWrite(GPIOA1_BASE,GPIO_PIN_3,0);
		    break;
		case 'v':
		    GPIOPinWrite(GPIOA1_BASE,GPIO_PIN_3,GPIO_PIN_3);
		    break;
		}
		// Echo local
		UARTCharPut(UARTA0_BASE, rxChar);
	}
	UARTIntClear(UARTA0_BASE,UART_INT_RX|UART_INT_RT);
}

//*****************************************************************************
//         DEFINICI�N DE FUNCIONES y MENEJADORES DE INTERRUPCI�N -- Fin
//*****************************************************************************


