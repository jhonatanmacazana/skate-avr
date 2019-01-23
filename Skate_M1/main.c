/*
 * Skate_M1.c
 *
 * Created: 20/12/2018 19:05:27
 * Author : Jhonatan
 */ 

#define F_CPU		1000000UL			/* F_CPU = 1 MHz */
#define BAUD		9600
#define MYUBRR		F_CPU/8/BAUD-1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Perif/LCD.h"
#include "Perif/ULTRASONIC.h"
#include "Perif/ADC.h"
#include "Perif/USART_irq.h"




uint16_t bateria_p,bateriaLecturaAdc,bateria_mV;
uint8_t distancia=0;

uint8_t estado = 0;


int main(void)
{
	uint8_t Vel = 0;
	Motor_Init();
	ADC_Init();
	Buzzer_Init();
	Ultrasonic_Init();
	USART_Init(MYUBRR);
	LCD_Init();
	
	sei();
	
	LCD_Start();
	
	estado = 0;

	while (1) // Bucle infinito
	{
		/* Motor */
		Vel = USART_Receive();
		if (Vel == 0x0A)
		{
			motor_avanza(USART_Receive());
			motor_retrocede(0);
		}
		else 
		{
			motor_avanza(0);
			motor_retrocede(USART_Receive());
		}
		
		/* Ultrasonido */
		distancia = GetDist();
		if (distancia < 0){distancia = 0;}
		else if (distancia >255){ distancia = 255;}
		if(distancia < 15)
		{
			Buzzer_On(NOTE_A4, 500);
			motor_avanza(0);
			motor_retrocede(0);
		}
		
		/* Estados*/
		switch (estado)
		{
			case 0:
				/* Imprime en LCD */
				LCD_Cursor_Up();
				LCD_WriteString("  Bienvenidos!  ");
				LCD_Cursor_Down();
				LCD_WriteString("    a UTEC ");
				break;
			case 1:
			
				/* Calcula el porcentaje de la bateria */
				lee_bateria();
				/* Imprime en LCD */
				LCD_Cursor_Up();
				LCD_WriteString("Nivel:          ");
				LCD_Cursor_Down();
				LCD_WriteInt(bateria_p);
				LCD_WriteString(" %      ");
				
				break;
			case 2:
				/* Imprime en LCD */
				LCD_Cursor_Up();
				LCD_WriteString("Distancia:    ");
				LCD_Cursor_Down();
				if (distancia <30)
				{
					LCD_WriteInt(distancia);
					LCD_WriteString(" cm    ");
				}
				else
				{
					LCD_WriteString(" > 30 cm    ");
					
				}
				break;
				
			default:
				break;
		}
		_delay_ms(700);
	}
}



/**********************/
/*** Interrupciones ***/
/**********************/

/* INT0 cambia de estado para mostrar la pantalla de LCD*/
ISR(INT0_vect)
{
	
	LCD_Clear();
	if (estado ==2)
		estado = 0;
	else
		estado++;
		
}

/* INT0 es el trigger para empezar el ADC. Cuando está listo, modifica el PWM y prepara el valor para el LCD*/
ISR(ADC_vect)
{
	bateriaLecturaAdc = ADC;
	bateria_mV = bateriaLecturaAdc*43/40;
	bateria_p = bateria_mV/11;
	
}