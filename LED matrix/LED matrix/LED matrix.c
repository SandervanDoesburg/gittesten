/*
 * Project		: Microcontrollers eindopdracht
 * Bestand		: LED_matrix.c
 * Auteur		: Matthijs 
 * Datum		: 13-1-2014 10:45:18
 * Omschrijving	: LED Matrix
 *
 */ 

#define F_CPU 2000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "spi.h"


const uint8_t lookupcol[][8] = {							
	{0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01},
};



const uint8_t lookuprow[][8] = {							
	{0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11, 0x0E},		// A
	{0x1E, 0x11, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E},		// B
	{0x0E, 0x11, 0x10, 0x10, 0x10, 0x10, 0x11, 0x0E},		// C
	{0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E},		// D
	{0x1F, 0x10, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F},		// E
	{0x10, 0x10, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F},		// F
	{0x0E, 0x11, 0x11, 0x17, 0x10, 0x10, 0x10, 0x0E},		// G
	{0x11, 0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11},		// H
	{0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E},		// I
	{0x08, 0x14, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E},		// J
	{0x11, 0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11},		// K
	{0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},		// L
	{0x11, 0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x1B},		// M
	{0x11, 0x11, 0x11, 0x11, 0x13, 0x15, 0x19, 0x11},		// N
	{0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E},		// O
	{0x10, 0x10, 0x10, 0x10, 0x1E, 0x11, 0x11, 0x1E},		// P
	{0x0F, 0x13, 0x15, 0x11, 0x11, 0x11, 0x11, 0x0E},		// Q
	{0x11, 0x12, 0x14, 0x1E, 0x11, 0x11, 0x11, 0x0E},		// R
	{0x1E, 0x01, 0x01, 0x01, 0x0E, 0x10, 0x10, 0x0F},		// S
	{0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1F},		// T
	{0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11},		// U
	{0x04, 0x0A, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11},		// V
	{0x0E, 0x15, 0x15, 0x15, 0x11, 0x11, 0x11, 0x11},		// W
	{0x11, 0x0A, 0x0A, 0x04, 0x04, 0x0A, 0x0A, 0x11},		// X
	{0x04, 0x04, 0x04, 0x04, 0x04, 0x0A, 0x0A, 0x11},		// Y
	{0x1F, 0x10, 0x08, 0x04, 0x02, 0x02, 0x01, 0x1F},		// Z
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},		// Spatie	
};



int main(void)
{
	spi_init();
	
	PORTC.DIRSET = PIN0_bm;														
	
	int stringlength, temp, index, rowcol = 0;
	
	unsigned int buffer[] = {0,0,0,0,0,0,0,0};
		
	char message[] = " MATTHIJS VISSER ";
		
	stringlength = strlen(message);
	
	while(1) 
	{
		for (int a=0; a<stringlength; a++)
		{
			for (int scroll = 0; scroll < 8; scroll++)
			{
				for (int shift = 0; shift < 8; shift++)
					{
						index = message[a];
						
						if (index == 32)
						{
							index = index - 6;			// spatie
						}
						else
						{
							index = index - 65;			// A t/m Z
						}
						
						temp = lookuprow[index][shift];
						
						buffer[shift] = (buffer[shift] << 1)|(temp >> ((7)-scroll));

						
							for(int i=0; i<8; i++)
							{
								
								spi_transfer((lookupcol[rowcol][i]));

								spi_transfer((buffer[i]));

			
								PORTC_OUT = PIN0_bm;
								_delay_ms(1.5);
								
							}
					}
		}
	}
}
}