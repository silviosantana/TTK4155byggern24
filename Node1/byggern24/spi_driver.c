#include "spi_driver.h"

#include "util.h"
#include "avr/io.h"
#include "util/delay.h"

//TODO - define for register bit (reusable code)

void SPI_MasterInit(void)
{
    /* Set MOSI, SCK output and SS all others input */
    DDRB = (1<<DDB5)|(1<<DDB7)|(1<<DDB4);
	
	//Set MISO as input 
	clear_bit(DDRB, DDB6);
	
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(char cData)
{
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));
}

void spi_init()
{
	SPI_MasterInit();
}

void spi_send(char data)
{
	SPI_MasterTransmit(data);	
	
}

char spi_read()
{
	SPI_MasterTransmit(0x00);
	return SPDR;
}

void spi_test()
{
	char data = 0xd5;
	clear_bit(PORTB, 4);
	spi_send(data);
	set_bit(PORTB, 4);
}