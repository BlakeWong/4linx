#include <config.h>
#include <asm/regs.h>
#include <config/smdk6410.h>
#include <asm/s3c64x0.h>



#ifdef CONFIG_SERIAL1
#define UART_NR	S3C64XX_UART0

#elif defined(CONFIG_SERIAL2)
#define UART_NR	S3C64XX_UART1

#elif defined(CONFIG_SERIAL3)
#define UART_NR	S3C64XX_UART2

#elif defined(CONFIG_SERIAL4)
#define UART_NR	S3C64XX_UART3

#else
#error "Bad: you didn't configure serial ..."
#endif


/*
 * Output a single byte to the serial port.
 */
void serial_putc(const char c)
{
        S3C64XX_UART *const uart = S3C64XX_GetBase_UART(UART_NR);

#ifdef CONFIG_MODEM_SUPPORT
        if (be_quiet)
                return;
#endif

        /* wait for room in the tx FIFO */
        while (!(uart->UTRSTAT & 0x2));

#ifdef CONFIG_HWFLOW
        /* Wait for CTS up */
        while (hwflow && !(uart->UMSTAT & 0x1));
#endif

        uart->UTXH = c;

        /* If \n, also do \r */
        if (c == '\n')
                serial_putc('\r');
}

/*
 * Test whether a character is in the RX buffer
 */
int serial_tstc(void)
{
        S3C64XX_UART *const uart = S3C64XX_GetBase_UART(UART_NR);

        return uart->UTRSTAT & 0x1;
}

/* 
 * Output a string to the serial port
 */
void serial_puts(const char *s)
{
        while (*s) {
                serial_putc(*s++);
        }
}
