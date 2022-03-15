/*
 * controlador.c
 *
 *  Created on: 11/03/2022
 *      Author: juan
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "Librerias/controlador.h"

void PID_control (struct PID_STRUCT *self, double imu, double ref, double Ek_1, double ek_1,  double Kp, double Ki, double Kd)
{
    double ed;
    double ek;
    double Ek;

    ek  = ref - imu;
    ed = ek - ek_1;
    Ek  = Ek_1+ek;
    self-> uk = (Kp*ek) + (Ki*Ek) + (Kd*ed);
    self-> ek = ek;
    self-> Ek = Ek;
    return self;
}


