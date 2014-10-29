/*
 * ledblink.c
 *
 * Created: 7-4-2014 13:16:44
 *  Author: Matthijs
 */ 

#define  F_CPU 2000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	PORTE.DIRSET = PIN0_bm;
	
	while (1) {

		PORTE_OUTTGL = PIN0_bm;
		_delay_ms(1000);

	}
}