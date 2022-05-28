//Convenience functions you can use with BufferedSerial to make it easier to do common tasks

void bf_print(BufferedSerial* bf, char s[])
{
    int len = strlen(s);
    bf->write(s, len);
}

void bf_println(BufferedSerial* bf, char s[])
{
    int len = strlen(s);
    bf->write(s, len);
    bf->write(&("\n"), 1);
}

//variadic function so printf can be used with buffered serial (alternatively, you can get the FILE* handle for the BufferedSerial with fdopen and use fprintf)
void bf_printf(BufferedSerial* bf, const char fmt[], ...)
{
    //sized to RX buffer size in case entire line needs to be written
    char buffer[MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE] = {0};
    uint32_t vsn_retval = 0;

    va_list args;
    va_start(args, fmt);
    
    //non negative and less than specified buffer size: entire string written to buffer
    vsn_retval = vsnprintf(buffer,MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE,fmt,args);
    
    va_end(args);
    
    if(vsn_retval > 0 && vsn_retval < MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE)
    {
        bf->write(buffer, vsn_retval);
    }
    else if(vsn_retval >= MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE)
    {
        error("E: vsnprintf return value exceeded buffer size! (%d)\r\n", vsn_retval);
    }
    else
    {
        error("E: vsnprintf negative value (%d)\r\n", vsn_retval);
    }
}