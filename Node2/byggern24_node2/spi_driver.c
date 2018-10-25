#include "spi_driver.h"

#include "avr/io.h"
#include "util/delay.h"
#include "util.h"

//TODO - define for register bit (reusable code)

void SPI_MasterInit(void)
{
	/* Set MOSI, SCK and SS output all others input */
	DDRB |= (1<<PB2)|(1<<PB1)|(1<<PB7)|(1<<PB0);
	//Set MISO as input
	DDRB &= ~(1<<PB3);
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

// void SPI_SlaveInit(void)
// {
//     /* Set MISO output, all others input */
//     DDRB = (1<<DDB6);
//     /* Enable SPI */
//     SPCR = (1<<SPE);
// }
//
// char SPI_SlaveReceive(void)
// {
//     /* Wait for reception complete */
//     while(!(SPSR & (1<<SPIF)))
//         ;
//     /* Return data register */
//     return SPDR;
// }


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
	PORTB &= ~(1 << 4);
	spi_send(data);
	PORTB |= (1 << 4);
}