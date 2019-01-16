/*
 * ULTRASONIC.c
 *
 * Created: 21/12/2018 08:59:42
 *  Author: Jhonatan
 *
 *	Librería para controlar el sensor ultrasónico,
 *  Funciones:
 *		- Ultrasonic_Start () : permite inicializar el sensor
 *		- GetDist() : permite calcular la distancia en cm (valor de retorno en uint16_t)
 */ 


#define F_CPU 1000000UL //Frec. Clk
#include <avr/io.h>
#include <util/delay.h> //Libería de Delay
#include <avr/interrupt.h> //Libería de IRQs

#include "ULTRASONIC.h"
uint8_t Ep=0;
volatile int32_t timer2_toggle_count;

/*Función para configurar el Ultrasonido*/
void Ultrasonic_Init(void) {
	/* Trigger = Salida (1), Echo = Entrada(0)*/ 
	sbi(DDR_Ultra, Trig);
	
	/* Interrupción por cambio de estado */
	sbi(PCICR, 1);		/* Puerto C [7:0] */
	sbi(PCMSK1, Echo);	/* Solo el pin ECHO */
	sei();				/* Habilita interrupciones IRQs */
	/* Reinicia Timer */
	TCNT1 = 0x0000;
	return;
}

/*Función para calcular la distancia*/
uint16_t GetDist (void) {
	uint16_t Tiempo;
	cbi(PORT_Ultra,Trig);		/* Pone Trig en bajo */
	sbi(PORT_Ultra,Trig);		/* Pone Trig en alto */
	_delay_us(11);				/* Pulso en Trig por más de 10us */
	cbi(PORT_Ultra, Trig);		/* Pone Trig en bajo */
		
	while (PIN_Ultra & (0<<Echo)) {}	
	while (PIN_Ultra & (1<<Echo)) {}
	Tiempo = TCNT1/7;			/*Calculo de la distancia en cm (entre 116) */
	
	TCCR1B = 0x00;		/* Detiene el timer (Presc = 0)*/
	TCNT1 = 0x0000;		/* Reinicia la cuenta del timer */
	return Tiempo;
}






/*Función para configurar el Buzzer*/
void Buzzer_Init(void)
{
	/* Timer 2. CTC */
	TCCR2A |= (1 << WGM21);
	/* Preescalador = 1 */
	TCCR2B |= (1 << CS20);
	TCNT2 = 0;
	OCR2B = 0;
	return;
}

/* Función que enciende el Buzzer */
void Buzzer_On(uint16_t freq, uint32_t duration)
{
	uint8_t preesc = 0b001;
	int32_t toggle_count = 0;
	uint32_t ocr = 0;
	cbi(PORT_Buzzer, P_Buzzer);		/* Pone en bajo PD6 */
	sbi(DDR_Buzzer, P_Buzzer);		/* Buzzer => salida(PD6)*/
	
	/* Cálculo del pre escalador */
	ocr = F_CPU /freq/2 - 1;
	preesc = 0b001;		/* clk/1 */
	if ( ocr > 255)
	{
		ocr = F_CPU /freq/2/8 - 1;
		preesc = 0b010;		/* clk/8 */
		
		if (ocr > 255)
		{
			ocr = F_CPU /freq/2/32 - 1;
			preesc = 0b011;		/* clk/32 */			
		}
		
		if (ocr > 255)
		{
			ocr = F_CPU /freq/2/64 - 1;
			preesc = 0b100;		/* clk/64 */
			
			if (ocr > 255)
			{
				ocr = F_CPU /freq/2/128 - 1;
				preesc = 0b101;		/* clk/128 */
			}
			if (ocr > 255)
			{
				ocr = F_CPU /freq/2/256 - 1;
				preesc = 0b110;		/* clk/256 */
				
				if (ocr > 255)
				{
					ocr = F_CPU /freq/2/1024 - 1;
					preesc = 0b111;		/* clk/1024 */
				}
			}
			
		}
		
	}
	
	/* Cambio del pre escalador*/
	TCCR2B = (TCCR2B & 0b11111000) | preesc;
	
	
	/* Cálculo de la cantidad de veces que cambiará*/
	toggle_count = 2 * freq * duration / 1000;
	
	
	/* Cambio del OCR*/
	OCR2A = ocr;
	timer2_toggle_count = toggle_count;
	TIMSK2 |=  1<<OCIE2A;
	
	return;
}


/* Función que apaga el Buzzer */
void Buzzer_Off(void)
{
	cbi(TIMSK2, OCIE2A);		/* Deshabilita la interrupcion por el comparador 'A' */
	cbi(PORT_Buzzer,P_Buzzer);	/* Pone en bajo el pin del buzzer */
	return;
	
}

/*Subrutina de interrupción para el pin Buzzer */
ISR(TIMER2_COMPA_vect)
{
	int32_t temp_toggle_count = timer2_toggle_count;
	
	if (temp_toggle_count != 0)
	{
		/* Toggle the pin*/
		toggle(PIN_Buzzer, P_Buzzer);
		
		if (temp_toggle_count > 0)
			temp_toggle_count--;
	}
	
	else
	{
		cbi(TIMSK2, OCIE2A);		/* Deshabilita la interrupcion por el comparador 'A' */
		cbi(PORT_Buzzer,P_Buzzer);	/* Pone en bajo el pin del buzzer*/
	}
	
	timer2_toggle_count = temp_toggle_count;
	
	
}


/*Subrutina de interrupción para el pin ECHO*/
ISR (PCINT1_vect)
{
	if (Ep == 0) { //ECHO flanco de subida
		TCCR1B = 0x02; /* Habilita el timer (Presc = 8) */
		Ep = 1;
		return;
	}
	if (Ep == 1) { //ECHO flanco de bajada
		TCCR1B = 0x00; /* Deshabilita el timer (Presc = 8) */
		Ep = 0;
		return;
	}
}