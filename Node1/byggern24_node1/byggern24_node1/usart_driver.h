#ifndef USART_DRIVER_H_
#define USART_DRIVER_H_

void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data);
unsigned char USART_Receive(void);
void USART_test_transmit_receive();

#endif // USART_DRIVER_H_