// ******************************************************************************
// PROGRAMA	: pinmux.c
// TARGET	: CC3200
// DESCRIPCION	: Configuraci�n de los pines de la UART y GPIO
// AUTOR	: Jos� Manuel Cano Garc�a y Eva Gonz�lez Parada
// FECHA	: 10-12-16
// ESQUEMA  : Modo Activo-->Clock 80Mhz
// ******************************************************************************
#include "pinmux.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "pin.h"
#include "rom.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"

//*****************************************************************************

//*****************************************************************************
//
//! Configuraci�n de los pines utilizados en el programa
//!
//! \Par�mentros: ninguno
//!
//! \Retorno: ninguno
//
//*****************************************************************************


void PinMuxConfig(void)
{
    //
    // Habilitaci�n de los relojes del GPIO.
	// Aunque el perif�rico GPIO no necesita reloj s� lo necesitan sus buses de interconexi�n
    //
    PRCMPeripheralClkEnable(PRCM_GPIOA1, PRCM_RUN_MODE_CLK|PRCM_SLP_MODE_CLK);

    //
    // Configuraci�n del PIN_64 en modo GPIO y salida.
    // Es necesario programar el modo del pin entre  las distintas funcionalidades que tiene,
    // tambi�n es necesario la configuraci�n el�ctrica, as� como especificar su direcci�n (entrada/salida)
    // El modo viene indicado en el Datasheet del CC3200 (p�gina 20, tabla 3.1),  el modo 0 corresponde al GPIO
    //
    // El pin 64 corresponde al terminal 1 del puerto A1, que est� conectado al LED Rojo de la placa CC3200
    // Esta �ltima informaci�n se puede consultar los esquem�ticos de la placa (Hojas 1 y 4) y en el documento "User�s Guide" de
    // la placa de desarrollo CC3200 SimpleLink Wi-Fi (p�gina 14, tabla 9). En ambas documentaciones el pin 64 se identifica como GPIO_09
    //
   	PinTypeGPIO(PIN_64, PIN_MODE_0, false);
    GPIODirModeSet(GPIOA1_BASE, GPIO_PIN_1, GPIO_DIR_MODE_OUT);

    //// PREGUNTA EC43
    // Configuraci�n del pin correspondiente al LED Amarillo
    //
    PinTypeGPIO(PIN_01, PIN_MODE_0, false);
    GPIODirModeSet(GPIOA1_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT);

    //// PREGUNTA EC43
    // Configuraci�n del pin correspondiente al LED verde
    //
    PinTypeGPIO(PIN_02, PIN_MODE_0, false);
    GPIODirModeSet(GPIOA1_BASE, GPIO_PIN_3, GPIO_DIR_MODE_OUT);

    //
    //Habilitaci�n de los relojes para la UART....
    //
    PRCMPeripheralClkEnable(PRCM_UARTA0, PRCM_RUN_MODE_CLK|PRCM_SLP_MODE_CLK|PRCM_DSLP_MODE_CLK);

    //
    // Configuraci�n del pin 55 correspondiente al transmisor de la UART0 UART0_TX
    // Datasheet del CC3200 (p�gina 18, tabla 3.1), modo 3, correspondiente a UART
    //
    PinTypeUART(PIN_55, PIN_MODE_3);

    //
    // Configuraci�n del pin 57 correspondiente al receptor de la UART0 UART0_RX
    // Datasheet del CC3200 (p�gina 18, tabla 3.1), modo 3, correspondiente a UART
    //
    PinTypeUART(PIN_57, PIN_MODE_3);


}
