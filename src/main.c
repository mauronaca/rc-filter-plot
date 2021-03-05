
/*----------------------------
PD6----------PB0	

		      220		  led
PD3----/\/\/\/\--->|---GND

PB0 pull up
PD6 salida OC0A

		        220			10uF
PD6----/\/\/\/\-----| |------GND
				 |
				 |
			    PC2
AREF----VCC
-----------------------------
		
*/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/delay.h>
#include "utils.h"
#include "adc.h"
#include "uart.h"

int main(void)
{
	extern uint8_t delta_tiempo_low, delta_tiempo_high, ancho_low, ancho_high;
	int buffer = 900;
	uint16_t muestra[buffer];
	
	SET(DDRD, 3); // PD3 conectado a un LED
	pwmInit(); // Inicializo la senial pwm en el pin OC0A usando el timer0 a 61 Hz con ciclo del 50%
	uart_init(103); // Inicializo el puerto serie con baudrate 9600 bps
	adc_init(); // Inicializo el ADC 
	
	//----------------------------------
	/* Medir periodo de la senial PWM */
	//----------------------------------
	
	SET(PORTB,0);// Habilito resistencia de pull up interna de PB0 del pin como entrada para medir la frecuencia.
	pwmMedir();  // Devuelve la diferencia de los tiempos y el duty cycle. 
				// para calcular el periodo = delta_tiempo*N/F_CPU . En este el prescaler N=1024. Lo mismo con el ancho multiplicar por 100%
	uart_sendString("Periodo: \n");
	uart_sendInt(((uint16_t) delta_tiempo_high << 8) | (delta_tiempo_low));
	uart_sendString("Ciclo de trabajo: \n");
	uart_sendInt(((uint16_t) ancho_high << 8) | (ancho_low));
	
	_delay_1s();
	_delay_1s();
	_delay_1s();

	
	_delay_ms(164); // Espero 10 periodos de senial
	
    while (1) 
    {
		// Muestreo a 16MHz/128/13 = 9615 Hz. Simulado y medido, aproximadamente entre muestra y muestra 111.5 us => fs = 8969
		for(int i = 0 ; i < buffer ; i++){
			startConversion();
			muestra[i] = (uint16_t) ADCL;
			muestra[i] |= (uint16_t) ADCH << 8;
		}
		for(int i = 0; i < buffer ; i++){
			uart_sendInt(muestra[i]); // Mando 3 bytes + byte de \n + 2 de bits de stop por cada byte = 40 bits a 9600 bps
									  // De todas formas esto no influye en el tiempo de muestreo.
		}
    }
}

