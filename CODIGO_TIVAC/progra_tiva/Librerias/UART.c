/*
 * UART.c
 *
 *  Created on: 13/03/2022
 *      Author: Charlie tito
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "UART.h"

void UART_init(void)
{
    //---------------------------------------INICIALIZACION DE PERIFERICOS---------------------------------------
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    //---------------------------------------SE INICIALIZAN UART0----------------------------------
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE );
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

}
void descon_UART(struct ENVIO_STRUCT *self, double dato)  // FUNCION DESCONCATENAR DATOS IMU
{
    self->primer_byte = ((uint32_t)dato >> 56) & 0xff;  //se extrae los bits 24-31 de la señal de referencia de posicion y se almacenan en la primera
                                                     //posicion del array data

    self->segundo_byte = ((uint32_t)dato>> 48) & 0xff;  //se extrae los bits 16-23 de la señal de referencia de posicion y se almacenan en la segunda
                                                     //posicion del array data

    self->tercer_byte = ((uint32_t)dato >> 40) & 0xff;  //se extrae los bits 8-15 de la señal de referencia de posicion y se almacenan en la tercera
                                                     //posicion del array data

    self->cuarto_byte = ((uint32_t)dato>>32)& 0xff; //se extrae los bits 0-7 de la señal de referencia de posicion y se almacenan en la cuarta
                                            //posicion del array data

    self->quinto_byte = ((uint32_t)dato >> 24) & 0xff;  //se extrae los bits 24-31 de la señal de referencia de posicion y se almacenan en la primera
                                                         //posicion del array data

    self->sexto_byte = ((uint32_t)dato>> 16) & 0xff;  //se extrae los bits 16-23 de la señal de referencia de posicion y se almacenan en la segunda
                                                         //posicion del array data

    self->septimo_byte = ((uint32_t)dato >>  8) & 0xff;  //se extrae los bits 8-15 de la señal de referencia de posicion y se almacenan en la tercera
                                                         //posicion del array data

    self->octavo_byte = (uint32_t)dato & 0xff; //se extrae los bits 0-7 de la señal de referencia de posicion y se almacenan en la cuarta
                                                //posicion del array data

}
void send_uart (unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4, unsigned char byte5, unsigned char byte6, unsigned char byte7, unsigned char byte8)
{
    char N = 0;
    UARTCharPut(UART0_BASE,'1');
    N = UARTCharGet(UART0_BASE);
    if(N=='1')
    {
        UARTCharPut(UART0_BASE,byte1);              // ENVIA LOS DATOS DESCONCATENADOS DE LA IMU A PYTHON
        UARTCharPut(UART0_BASE,byte2);
        UARTCharPut(UART0_BASE,byte3);
        UARTCharPut(UART0_BASE,byte4);
        UARTCharPut(UART0_BASE,byte5);              // ENVIA LOS DATOS DESCONCATENADOS DE LA IMU A PYTHON
        UARTCharPut(UART0_BASE,byte6);
        UARTCharPut(UART0_BASE,byte7);
        UARTCharPut(UART0_BASE,byte8);
        N = 0;
    }

}


