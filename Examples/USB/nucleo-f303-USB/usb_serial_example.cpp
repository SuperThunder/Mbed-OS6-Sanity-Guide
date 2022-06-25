//https://os.mbed.com/docs/mbed-os/v6.15/apis/usbserial.html
#include "mbed.h"
#include "USBSerial.h"

//Virtual serial port over USB
USBSerial serial;

int main(void)
{
    while (1) {
        serial.printf("I am a virtual serial port\r\n");
        ThisThread::sleep_for(1000);
    }
}

