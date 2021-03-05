/*
 * adc.h
 *
 * Created: 26/2/2021 20:37:42
 *  Author: mnaca
 */ 


#ifndef ADC_H_
#define ADC_H_

#include "utils.h"

void adc_init(){
	// Habilito el adc en modo libre con prescaler . Cada conversion tarda (16MHz/N)/13
	ADMUX |= (1<<MUX1)|(0<<MUX0); // AREF, MUX[3:0]=0010 canal adc2, adlar=0 8 bits en el registro adcl
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // Prescaler 128 y habilito el ADC
}

void startConversion(){
	SET(ADCSRA, ADSC);
	while((ADCSRA & (1<<ADSC))); // Mientras que adsc sea 1 no termino la conversion
}


#endif /* ADC_H_ */