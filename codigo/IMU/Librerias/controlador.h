/*
 * controlador.h
 *
 *  Created on: 11/03/2022
 *      Author: juan
 */

#ifndef LIBRERIAS_CONTROLADOR_H_
#define LIBRERIAS_CONTROLADOR_H_


typedef struct PID_STRUCT
{
    double ek;
    double uk;
    double Ek;
    void (*control_pid)(struct PID_STRUCT *self, double imu, double ref, double Ek_1, double ek_1,  double Kp, double Ki, double Kd);
}pid_T;


#endif /* LIBRERIAS_CONTROLADOR_H_ */
