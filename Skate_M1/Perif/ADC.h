/*
 * ADC.h
 *
 * Created: 22/12/2018 09:50:00
 *  Author: Jhonatan
 */ 


#ifndef ADC_H_
#define ADC_H_

#define F_CPU 1000000UL //Frec. Clk
#include <avr/io.h>


/* Macros */
#define sbi(port,bit)			(port |=  (1 << bit))
#define cbi(port,bit)			(port &= ~(1 << bit))
#define tog(port,bit)			(port ^ (1 << bit))
#define motor_avanza(pwm)		(OCR0A = pwm)
#define motor_retrocede(pwm)	(OCR0B = pwm)

/* Prototypes */

void ADC_Init(void);
void lee_bateria(void);
void Motor_Init(void);


#endif /* ADC_H_ */