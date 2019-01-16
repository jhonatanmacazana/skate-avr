/*
 * ADC.c
 *
 * Created: 22/12/2018 09:49:52
 *  Author: Jhonatan
 */ 


#define F_CPU 1000000UL		// 1MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "ADC.h"


void ADC_Init(void)
{
	/* Voltage Reference = 1.1V ; PC1 */
	ADMUX |= (3<<REFS0)|(1<<MUX0);
	/* Enable, Start Conversion, Auto Trigger Enable, ADC Interrupt, Prescaler = 8 (125 kHz)*/
	ADCSRA |= (1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(1<<ADIE)|(3<<ADPS0);
	/* Trigger = INT0 */
	ADCSRB |= (2<<ADTS0);
	/* Rising Edge */
	EICRA |= (3<<ISC00);
	/* Activa INT0*/
	EIMSK |= (1<< INT0);
}

void lee_bateria()
{
	/*Activa ADC*/
	ADCSRA |= (1<<ADSC);
}

void Motor_Init()
{
	cbi(PORTD, 6);
	cbi(PORTD, 5);
	sbi(DDRD, 6);		/* Motor adelante	= salida(PD6)*/
	sbi(DDRD, 5);		/* Motor atras		= salida(PD5)*/
	/* Timer 0 */
	TCCR0A |= (3 << WGM00);		// Fast PWM
	TCCR0B |= (1 << CS01);		// Preescalador = 8
	TCNT0 = 0;					// Reinicia el timer
	/* Avanza */
	TCCR0A |= (1 << COM0A1)|(1 << COM0B1);	// Clear OC0A y OC0B (Avanzar y retroceder)
	OCR0A = 0;
	OCR0B = 0;
}


