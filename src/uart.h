/*
 * uart.h
 *
 * Created: 26/2/2021 20:37:51
 *  Author: mnaca
 */ 


#ifndef UART_H_
#define UART_H_




#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void uart_init(unsigned int);
void uart_tx(unsigned char ch);
void uart_sendString(char * str);

void uart_init(unsigned int baudrate){
	UBRR0 = baudrate;// baudrate. Lo paso por parametro
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // Envio 8 bits
	UCSR0B = (1<<TXEN0); // Habilito la transmision
}

// Enviar un byte:
void uart_tx(unsigned char c){
	while(!(UCSR0A & (1<<UDRE0))); // Aguardo a que este habilitado el registro cuando UDRE0 se setea
	UDR0 = c; // Envio el byte
}

// Rutina para enviar arreglos de bytes:
void uart_sendString(char * str){
	while(*str != '\0'){ // Envio todos los bytes que componen al string hasta encontrar el \0
		uart_tx(*str);
		str++;
	}
}


// Rutina para enviar un entero de 2 bytes seguido por el \n
void uart_sendInt(uint16_t num){
	char buffer[8];
	sprintf(buffer, "%u", num); // Convierto el numero entero de 2 bytes a una cadena de bytes en formato entero sin signo. 
	uart_sendString(buffer);
	uart_tx('\n');
}

#endif /* UART_H_ */