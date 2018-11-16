#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

void spi_init();
void spi_send(char data);
void spi_test();
char spi_read();

#endif // SPI_DRIVER_H_