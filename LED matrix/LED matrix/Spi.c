#include <avr/io.h>
#include "spi.h"

void spi_init(void)
{
	PORTC.DIR |= SPI_SCK_bm|SPI_MOSI_bm|SPI_SS_bm;
	SPIC.CTRL = (!SPI_CLK2X_bm) |				// no double clock speed
	SPI_ENABLE_bm |								// SPI enable
	(!SPI_DORD_bm) |							// data order;: MSB first
	SPI_MASTER_bm |								// master
	SPI_MODE_0_gc |								// mode 0
	SPI_PRESCALER_DIV4_gc;						// Presc. 4 (@2 MHz,500kHz)
}


	
uint16_t spi_transfer(uint16_t data)
{
	SPIC.DATA = data;
	while ( ! (SPIC.STATUS & (SPI_IF_bm)) );
	
	return SPIC.DATA;
}