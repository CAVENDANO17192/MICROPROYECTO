/*
 * UART.h
 *
 *  Created on: 13/03/2022
 *      Author: Charlie tito
 */

#ifndef UART_H_
#define UART_H_


extern void UART_init(void);
void send_uart (unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4, unsigned char byte5, unsigned char byte6, unsigned char byte7, unsigned char byte8);
typedef struct ENVIO_STRUCT{
    unsigned char primer_byte;
    unsigned char segundo_byte;
    unsigned char tercer_byte;
    unsigned char cuarto_byte;
    unsigned char quinto_byte;
    unsigned char sexto_byte;
    unsigned char septimo_byte;
    unsigned char octavo_byte;
    void (*envio)(struct ENVIO_STRUCT *self, double dato);

}enviar_T;
#endif /* UART_H_ */
