
/*
 * pwmMedir.s
 *
 * Created: 26/2/2021 20:40:42
 *  Author: mnaca
 */ 
 
 #define __SFR_OFFSET 0
 #include <avr/io.h>

 /*
 Entrada: ICP1 pin PB0
 Salida: Registro R21 byte bajo y R22 byte alto
 */
 
.global pwmMedir
.comm delta_tiempo_low, 2 // Reservo 1 byte para las variables
.comm delta_tiempo_high, 2
.comm ancho_low, 2
.comm ancho_high, 2
.global delta_tiempo_low // Variables globales
.global delta_tiempo_high
.global ancho_low
.global ancho_high
.section .text

pwmMedir:

inicio_periodo:
	sbi PORTD, PD3
	ldi r16, 0x00
	sts TCCR1A, r16 // timer1 modo Normal
	ldi r16, (1<<ICES1)|(1<<CS12)|(0<<CS11)|(1<<CS10) //  Prescaler en 1024 ICES1 Flanco ascendente
	sts TCCR1B, r16 // Flanco ascendente
primerFlanco_periodo:
	in r16, TIFR1
	sbrs r16, ICF1 // Salta si ICF1 esta seteado - El flag se setea si se capturo el tiempo
	rjmp primerFlanco_periodo 
	lds r23, ICR1L
	lds r24, ICR1H
	out TIFR1, r16 // ICF1 = 0
segundoFlanco_periodo:
	in r16, TIFR1
	sbrs r16, ICF1 // salta siguiente si ICF1 esta seteado
	rjmp segundoFlanco_periodo
	out TIFR1, r16 // ICF1 = 0
	lds r21, ICR1L
	lds r22, ICR1H
	sub r21, r23 // r21 = r21 - r23
	sbc r22, r24 // r22 = r22 - r24
fin_periodo:
	sts delta_tiempo_low, r21
	sts delta_tiempo_high, r22

inicio_ancho:
primerFlanco_ancho:
	in r16, TIFR1
	sbrs r16, ICF1 // Salta si ICF1 esta seteado
	rjmp primerFlanco_ancho
	lds r23, ICR1L
	lds r24, ICR1H
	out TIFR1, r16 // ICF1 = 0
	ldi r17, 0x05 // 1024 presacler ICES1=0 flanco bajada
	sts TCCR1B, r17 
segundoFlanco_ancho:
	in r16, TIFR1
	sbrs r16, ICF1 // salta siguiente si ICF1 esta seteado
	rjmp segundoFlanco_ancho
	out TIFR1, r16 // ICF1 = 0
	lds r21, ICR1L
	lds r22, ICR1H
	sub r21, r23 // r21 = r21 - r23
	sbc r22, r24 // r22 = r22 - r24
fin_ancho:
	sts ancho_low, r21
	sts ancho_high, r22
	cbi PORTD, 3
	ret
.end