
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "sensorlib/i2cm_drv.h"
#include "sensorlib/hw_mpu6050.h"
#include "sensorlib/mpu6050.h"

#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_i2c.h"
#include "inc/hw_sysctl.h"

#include "driverlib/uart.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/i2c.h"



#include "Librerias/MPU6050.h"
#include "Librerias/uartstdio.h"
#include "Librerias/timer.h"
#include "Librerias/controlador.h"
#include "Librerias/UART.h"
#include <math.h>

#define dt 0.001
#define alpha 0.5

/**
 * main.c
 */
int update;
int n;
int serial;
void Timer0Handler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    update = 1;

}

void UART0IntHandler(void)
{
    UARTIntClear(UART0_BASE,UARTIntStatus(UART0_BASE, true)); //se limpia la bandera de la interrupcion
    n = UARTCharGet(UART0_BASE);//se lee lo que entra al UART0
    serial=1;
}

struct DATOS_STRUCT imu;
struct PID_STRUCT posicion;
struct PID_STRUCT velocida;
struct ENVIO_STRUCT python;

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_XTAL_16MHZ);
    InitI2C0();
    MPU6050INIT();
    timer_init();
    UART_init();
    while(1)
    {
        if(update==1)
        {
            imu.acelerometro = MPU6050READDATA(&imu);

            //imu.filtro_posicion = filtro_complementario_posicion(&imu, imu.Filtrox, imu.gx, imu.Accx, imu.Filtroy, imu.gy, imu.Accy, dt, alpha);
            //imu.filtro_velocidad = filtro_complementario_velocidad(&imu, imu.Velocidadx, imu.gx, imu.Velocidady, imu.gy, imu.Velocidadz, imu.gz, alpha);
            //pid.control_pid = PID_control(&pid, imu.Filtrox, 0.00(angulo balancin quieto), pid.Ek, pid.ek,  1, .2, 0.0005);
            update=0;
        }
        python.envio=descon_UART(&python,imu.Accx);
        if(serial ==0)
        {
            UARTCharPut(UART0_BASE,"6");
        }
        if(n==1)
        {
            UARTCharPut(UART0_BASE,"1");
            send_uart(python.primer_byte,python.segundo_byte,python.tercer_byte,python.cuarto_byte,python.quinto_byte,python.sexto_byte,python.septimo_byte,python.octavo_byte);
            n=0;
        }

    }
    //return 0;
}
