#ifndef _SERIAL_H_
#define _SERIAL_H_



/*
 * Output a single byte to the serial port.
 */
void serial_putc(const char c);

/*
 * Test whether a character is in the RX buffer
 */
int serial_tstc(void);

/* 
 * Output a string to the serial port
 */
void serial_puts(const char *s);



#endif /* _SERIAL_H_ */
