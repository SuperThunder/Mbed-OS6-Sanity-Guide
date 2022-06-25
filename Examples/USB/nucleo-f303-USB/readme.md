# Nucleo F303RE USB
USB support can be added fairly easy for the Nucleo F303RE, since the F3 family has Mbed OS USB support on the same pins and the clock configuration is already correct. The USB pins are PA_12 (USB_DP) and PA_11 (USB_DM). Remember a 1.5kOhm pullup to 3v3 on USB_DP is required for F3 series, although other STM32 devices [have internal pullups](https://www.st.com/resource/en/application_note/an4879-usb-hardware-and-pcb-guidelines-using-stm32-mcus-stmicroelectronics.pdf).

## USBSerial Basic Example
- Connect GND, USP_DP, and USB_DM to a USB breakout board
- Connect USB_DP to a 1.5kOhm pullup to 3v3
- Copy the included mbed_app.json which adds Mbed USBDEVICE support needed
- Upload the USBSerial example to your F303
- Connect your PC to the USB breakout board, a new COM port should appear
- Connect to the COM port at 115200 baud


