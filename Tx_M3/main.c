/*
 * Tx_M3.c
 *
 * Created: 23/01/2019 10:56:50
 * Author : Jhonatan
 * Este transmisor envia datos cada segundo.
 * Se envia un identificador para avanzar o retroceder y el valor del ADC mapeado de 0 a 255.
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "utils/ADC.h"
#include "utils/UART.h"


uint8_t vel, dir;
int main(void)
{
	/* Inicializa componentes */
    ADC_Init();
	USART_Init(MYUBRR);
	
	/* Habilita interrupciones globales */
	sei();
	
    while (1) 
    {
		/* Empieza Conversión ADC (8 bits) */
		ADC_Start();
		
		/* Parametros para velocidad y dirección */
		vel = ADC_GetValue();
		if (vel>=128)		// Adelante
		{
			dir = 0x0A;					// Establece dirección
			vel = (vel-128)<<1;		// Escala el valor de 0 a 255
		}
		else if (vel <127) // Atras
		{
			dir = 0x0B;					// Establece dirección
			vel = 126-vel;
			vel = vel<<1;				// Escala el valor de 0 a 255
		}
		
		if (vel<=5){vel = 0;}
		if (vel>=250){vel = 255;}
		
		USART_Transmit(dir);
		USART_Transmit(vel);	
		_delay_ms(100);
    }
}

