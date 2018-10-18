#ifndef USART_DRIVER_H_
#define USART_DRIVER_H_

#include <stdio.h>

void USART_Init(unsigned int ubrr);

void USART_Transmit(unsigned char data);

unsigned char USART_Receive(void);

#endif