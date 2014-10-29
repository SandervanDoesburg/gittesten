/*
 * main.c
 *
 * Created: 1-5-2014 22:10:08
 *  Author: Matthijs
 */

#define F_CPU 2000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd.h"
#include "spi.h"
#define FOO 0

uint8_t spi_read_byte(void)
{
	uint8_t data;
	PORTC.OUTCLR = SPI_SS_bm;
	data = spi_transfer(FOO);
	PORTC.OUTSET = SPI_SS_bm;
	return data;
}

int main(void){
	lcd_init();
	spi_init();
	PORTE.DIRSET = PIN0_bm;
	char buffer[3];
				 
	while(1)
	{
		uint8_t data = spi_read_byte();
		
		lcd_puts("RFID data:");
		lcd_gotoxy(0, 1);
		
		utoa( data, buffer, 10 );
		lcd_puts(buffer);
		_delay_ms(2000);
		lcd_clear();
	}
}