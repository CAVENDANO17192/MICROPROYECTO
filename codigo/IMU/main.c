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

#include <math.h>

#define dt 0.001
#define alpha 0.5

/**
 * main.c
 */
int update;

void Timer0Handler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    update = 1;

}

struct DATOS_STRUCT imu;
struct PID_STRUCT pid;

void ConfigureUART(void) { // Função retirada do exemplo hello.c
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE );
    UARTStdioConfig(0, 115200, SysCtlClockGet());
}
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_XTAL_16MHZ);
    InitI2C0();
    MPU6050INIT();
    ConfigureUART();
    timer_init();
    while(1)
    {
        if(update==1)
        {
            imu.acelerometro = MPU6050READDATA(&imu);
            UARTprintf("Ang x: %d| Ang. y: %d |Ang. z %d | Gyro. x %d | Gyro. y %d | Gyro. z %d | Accx. %d | Accy. %d \n", (int)imu.ax, (int)imu.ay, (int)imu.az, (int)imu.gx, (int)imu.gy, (int)imu.gz, (int)imu.Accx, (int)imu.Accy);
            imu.filtro_posicion = filtro_complementario_posicion(&imu, imu.Filtrox, imu.gx, imu.Accx, imu.Filtroy, imu.gy, imu.Accy, dt, alpha);
            imu.filtro_velocidad = filtro_complementario_velocidad(&imu, imu.Velocidadx, imu.gx, imu.Velocidady, imu.gy, imu.Velocidadz, imu.gz, alpha);
            pid.control_pid = PID_control(&pid, imu.Filtrox, 0.00, pid.Ek, pid.ek,  1, .2, 0.0005);
            update=0;
        }
    }
	return 0;
}
