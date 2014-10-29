#define F_CPU 2000000UL

#define SPI_SS_bm 0x10								// SS pin 4
#define SPI_MISO_bm 0x20							// DATA pin 5
#define SPI_SCK_bm	0x80							// Clock pin 7 

void spi_init(void);
uint8_t spi_transfer(uint8_t data);