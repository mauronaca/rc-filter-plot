/*
 * utils.h
 *
 * Created: 26/2/2021 20:38:01
 *  Author: mnaca
 */ 


#ifndef UTILS_H_
#define UTILS_H_

/*Operaciones de bits*/
#define SET(x,y) x |= (1 << y)
#define CLEAR(x,y) x &= ~(1<< y)
#define READ(x,y) ((0x00 == ((x & (1<<y))>> y))?0x00:0x01)
#define TOGGLE(x,y) (x ^= (1 << y))
#define BYTELOW(v)   (*(((uint16_t *) (&v) + 1)))
#define BYTEHIGH(v)  (*((uint16_t *) (&v)))

/*Llamadas a funciones de assembler*/
extern void pwmInit();
extern void pwmMedir();

void _delay_1s();

void _delay_1s(){
	TCCR1A = (0<<WGM11)|(0<<WGM10); // Modo 4 CTC
	TCCR1B = (1<<WGM12)|(1<<CS12)|(0<<CS11)|(1<<CS10) ; // Prescaler 1024
	OCR1A = 15625; // tiempo = OCR1A*N/F_CPU
	TCNT1 = 0;
	while(!READ(TIFR1,OCF1A)); // Mientras que el flag de ocf1a este en 0...
	SET(TIFR1, OCF1A); // Poner en 0 bit OCF1A
}

#endif /* UTILS_H_ */