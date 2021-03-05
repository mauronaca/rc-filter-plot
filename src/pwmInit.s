
/*
 * pwmInit.s
 *
 * Created: 26/2/2021 20:40:34
 *  Author: mnaca
 */ 
 
 #define __SFR_OFFSET 0
 #include <avr/io.h>

.global pwmInit
.section .text

pwmInit:
	LDI R16, (1<<PD6)
	OUT DDRD, R16 // Pin PD6 salida
	; DC = 50%, tendra 128 valores en 1 y otros 128 en 0
	LDI R16, 127
	OUT OCR0A, R16
	;LDI R16, 127
	;OUT OCR0B, R16
	; Modo 3 - Fast PWM Mode - Non-Inverted - TOP = 0xFF
	LDI R16, (1<<COM0A1)|(1<<WGM01)|(1<<WGM00)
	OUT TCCR0A, R16
	; Prescaler 1024; Frecuencia = 16MHz/(256*1024) = 61 Hz
	LDI R16, (1<<CS02)|(1<<CS00) 
	OUT TCCR0B, R16
	RET

.end
