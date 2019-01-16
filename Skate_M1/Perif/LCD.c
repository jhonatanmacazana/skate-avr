/*
 * LCD.c
 *
 * Created: 21/12/2018 08:50:14
 *  Author: Jhonatan
 */ 

#define F_CPU 1000000UL		// 1MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "LCD.h"



/*Función de envío:
	-Permite el envió de data al activar y desactivar el ENABLE*/
void Toggle_EN (void) {
	LCD_BUS |= (1<<EN);
	_delay_us(750);
	LCD_BUS &=~((1<<EN));
	_delay_us(750);
	return;
}

/*Función de escritura de comandos:
	-Permite escribir comandos al LCD*/
void LCD_cmd(uint8_t CMD){
	LCD_BUS &=~((1<<RS)); //Habilita uso de comandos
	LCD_BUS &= 0xF0;	
	LCD_BUS |= ((CMD & 0xF0)>>4); //Mandar nibble superior
	Toggle_EN();
	LCD_BUS &= 0xF0;
	LCD_BUS |= CMD & 0x0F; //Mandar nibble inferior
	Toggle_EN();
	return;
}

/*Función de escritura de caracter:
	-Permite escribir un caracter en la pantalla del LCD*/
void LCD_Write (char LCDCHAR){
	LCD_BUS |= (1<<RS);
	LCD_BUS &= 0xF0;	
	LCD_BUS |= ((LCDCHAR&0xF0)>>4); //Mandar nibble superior
	Toggle_EN();
	LCD_BUS &= 0xF0;
	LCD_BUS |= LCDCHAR&0x0F; //Mandar nibble inferior
	Toggle_EN();
	return;	
}

/*Función de escritura de cadena de caracteres:
	-Permite escribir Strings*/
void LCD_WriteString (char *S){
	while (*S!='\0') {
		LCD_Write(*S);
		S++;
	}
	return;
}


/*Función de configuración de puertos del LCD 
	-Configura los puertos del LCD */
void LCD_Init(){
	LCD_DDR = 0xFF;
	return;
}

/*Función de configuración del LCD:
	-Configura el LCD para funcionar mediante 4bits*/
void LCD_Start(void){
	LCD_BUS &= 0xF0;
	//Secuencia para habilitar LCD 4bits
	LCD_BUS |= 0x03;
	Toggle_EN();
	_delay_ms(6);
	LCD_BUS &= 0xF0;
	LCD_BUS |= 0x03;
	Toggle_EN();
	_delay_us(64); //?
	LCD_BUS &= 0xF0;
	LCD_BUS |=0x03;
	Toggle_EN();
	LCD_BUS &= 0xF0;
	LCD_BUS |= 0x02;
	Toggle_EN();
	_delay_ms(2);
	//Fin
	LCD_cmd(0x28); //4bits
	LCD_cmd(0x0C);
	LCD_cmd(0x01); //Borrar LCD
	LCD_cmd(0x06);
	return;
}

/*Función para escribir Integers en LCD:
	-Utiliza la función "sprintf" para convertir entero a string*/
void LCD_WriteInt (int8_t Num){
	char str[2];
	sprintf(str,"%d",Num);
	LCD_WriteString(str);
	return;
}

/*Función para limpiar LCD:*/
void LCD_Clear (void) {
	LCD_cmd(0x01); //Borrar LCD	
	return;
}