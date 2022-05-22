# Mbed-OS6-Sanity-Guide
A sanity-preserving introduction to Mbed OS 6. Aiming to filter out the many outdated discussion threads and documentation posts online and link to the reasonable documentation that does exist for Mbed OS 6 but can only be found through hard searching.

Mbed OS is overall a very solid RTOS with good device support and arduino-like abstraction while being much more powerful. However, it is still obscure and at least in large part it has to be due to how frustrating it is to start. This guide aims to give an easy getting started path, and also collect resources on the easy and correct way to do things with Mbed OS 6.


# Introduction: A Sane Start (Mbed Studio)
Mbed has many options for development: legacy compiler, keil studio online, mbed studio, mbed cli 1, mbed cli 2. This guide will use **Mbed Studio with Mbed 6.15**. The board used in writing this guide is the Nucleo F303RE.

## 1: Software and blinky
- Install Mbed Studio
- Make a new project with the os 6 blinky code and mbed 6.15 (or later)
- Plug in your board and select yes when it asks if you want it to be the active board. You should see your board with a green USB icon under "Target" in the top left section of the interface.
- Build the blinky code (hammer icon). This will take a little while.
- Run the code (play button). Observe the blinky.

## 2: Printing Hello World
TODO

## 3: RTOS features
TODO

# Feature: Serial output
See [Hitchhiker’s Guide to Printf in Mbed 6](https://forums.mbed.com/t/hitchhikers-guide-to-printf-in-mbed-6/12492) by MonomialOutput.

# Feature: SPI or I2C Storage devices
Mbed actually has pretty good support for storage devices (eg winbond W25Q series for SPI, atmel AT24 series for I2C, SD cards). See [here for overview](https://os.mbed.com/docs/mbed-os/v6.15/apis/blockdevice-apis.html).
- [I2C, not in docs but in the class hierarchy](https://os.mbed.com/docs/mbed-os/v6.15/mbed-os-api-doxy/class_i2_c_e_e_block_device.html)
- [SPI](https://os.mbed.com/docs/mbed-os/v6.15/apis/spi-flash-block-device.html)

# Feature: Filesystems
As can be seen in [this section of the docs](https://os.mbed.com/docs/mbed-os/v6.15/apis/file-system-apis.html), FAT and LittleFS are both supported as well as some other interesting APIs like KVStore (key value) and a generic directory / file system.

# Feature: Wifi (ESP8266)
TODO.


# Hazard: (STM32) Calibrated ADC Readings
You might think that a fancy abstracted RTOS like mbed would use the ADC calibration available to each platform, however it does not. Check the datasheet of your STM32 to find out how to read your ADC calibration values and apply the correction factor. You will need to also read the internal temperature sensor ADC.
See [this gist](https://gist.github.com/SuperThunder/83fa784e13f39cfd27ca0ebee7cb5165) for an example for F303.

# QoL: Shared mbed library to save 1GB per application
Re-use the mbed os library from a single program (eg, the blinky program) where possible. Otherwise you are duplicating 1GB of Mbed files for each application in Mbed Studio.

# QoL: Reducing compile time (initial, and whenever a library or mbed config file is changed)
By default mbed has a LOT of extra stuff that it compiles for absolutely no reason on most projects. Zoltan Hudak has written a [.mbedignore file](https://forums.mbed.com/t/how-to-shorten-compilation-time-up-to-six-times/11203/22) that cuts out a lot of the unneeded libraries, drastically decreasing full-build compile time. However, make sure you do read through all the excluded directories and comment out any that you actually need.
TODO: put mbedignore example in repo

# Good to know: Mbed forums
The only active place on the internet for mbed (as far as I am aware) is the [mbed OS forums](https://forums.mbed.com/c/mbed-os/6).

# Good to know: Mbed examples
There are actually a decent number of mbed os6 examples, but they are hard to find. One place to find them is from the [ARMmbed github account](https://github.com/orgs/ARMmbed/repositories?q=example&type=all&language=&sort=).

# Good to know: Target information
Want to know how to refer to the pins and peripherals of your microcontroller? The easiest way is to go straight to the source code that mbed uses to identify them. This can be found in mbed-os/targets/TARGET_<VENDOR>/TARGET_<FAMILY>. For example, for F303RE:
- [PeripheralNames.h](https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_STM/TARGET_STM32F3/PeripheralNames.h)
- [PinNames.h](https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_STM/TARGET_STM32F3/TARGET_STM32F303xE/TARGET_NUCLEO_F303RE/PinNames.h)
- [PeripheralPinMaps.h](https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_STM/TARGET_STM32F3/TARGET_STM32F303xE/TARGET_NUCLEO_F303RE/PeripheralPinMaps.h)
  
On Nucleo boards, pins are labelled like "D3" and then the pinout diagram will refer to that as "PB3". Looking at PinNames.h you can immediately see the format Mbed is expecting is PB_3 or ARDUINO_UNO_D3.
  



# The Big List of misc mbed lore
### Why are there 3 versions of mbed, and most google results are for the most outdated one?
Mbed went through 2 sudden changes when it went from 2 to 5 (becoming much more of an RTOS with IoT functionality) and 5 to 6 (making large changes to the way Mbed OS worked). For years everyone had been happily using Mbed 2, and the initial release of 5 was a mess so everyone kept using 2. Once 5 was actually working quite well they introduced a shift to 6 which breaks just enough things to be seriously frustrating. This is why almost all the libraries and projects that show up in google results seem to be for Mbed 2 or 5: every major update actually seriously incentivized not updating for years.

### Bare metal vs RTOS?
There is a bare metal option in Mbed 5 and 6 that will use less flash / RAM but isn't able to use all functionality. It seems to be recommended as the migration path for Mbed 2 applications, or for using lower-end microcontrollers (eg F103C8 tier stuff).
