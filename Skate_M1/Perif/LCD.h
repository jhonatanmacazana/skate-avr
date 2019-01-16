/*
 * LCD.h
 *
 * Created: 21/12/2018 08:50:07
 *  Author: Jhonatan
 */ 


#ifndef LCD_H_
#define LCD_H_



#define F_CPU 1000000UL //Frec. Clk
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

/*PUERTOS:
	-El programa trabaja utilizando 6 pines de un mismo puerto:
	-Los pines D7:4 del LCD se consideran los menos significativos del puerto*/
#define EN			PB5 
#define RS			PB4 
#define LCD_BUS		PORTB 
#define LCD_DDR		DDRB

#define LCD_Cursor_Up() (LCD_cmd(0x80))
#define LCD_Cursor_Down() (LCD_cmd(0xC0))


/* Prototypes */

/*Funci�n de env�o:
	-Permite el envi� de data al activar y desactivar el ENABLE*/
void Toggle_EN (void);

/*Funci�n de escritura de comandos:
	-Permite escribir comandos al LCD*/
void LCD_cmd(uint8_t CMD);
/*Funci�n de escritura de caracter:
	-Permite escribir un caracter en la pantalla del LCD*/
void LCD_Write (char LCDCHAR);

/*Funci�n de escritura de cadena de caracteres:
	-Permite escribir Strings*/
void LCD_WriteString (char *S);

/*Funci�n de configuraci�n de puertos del LCD 
	-Configura los puertos del LCD */
void LCD_Init();

/*Funci�n de configuraci�n del LCD:
	-Configura el LCD para funcionar mediante 4bits*/
void LCD_Start(void);

/*Funci�n para escribir Integers en LCD:
	-Utiliza la funci�n "sprintf" para convertir entero a string*/
void LCD_WriteInt (int8_t Num);

/*Funci�n para limpiar LCD:*/
void LCD_Clear (void);


#endif /* LCD_H_ */