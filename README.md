# Mbed-OS6-Sanity-Guide
A sanity-preserving introduction to Mbed OS 6. Aiming to filter out the many outdated discussion threads and documentation posts online and link to the reasonable documentation that does exist for Mbed OS 6 but can only be found through hard searching.

Mbed OS is overall a very solid RTOS with good device support and arduino-like abstraction while being much more powerful. However, it is still obscure and at least in large part it has to be due to how frustrating it is to start. This guide aims to give an easy getting started path, and also collect resources on the easy and correct way to do things with Mbed OS 6.


# Introduction: A Sane Start (Mbed Studio)
Mbed has many options for development: legacy compiler, keil studio online, mbed studio, mbed cli 1, mbed cli 2. This guide will use **Mbed Studio with Mbed 6.15**. The board used in writing this guide is the Nucleo F303RE. Some STM32-specific details will be present but overall it should be the same on other platforms.

## 1: Software and blinky
Reference: [Mbed docs quick start](https://os.mbed.com/docs/mbed-studio/current/getting-started/index.html)
- Install Mbed Studio
- Make a new project with the os 6 blinky code and mbed 6.15 (or later)
- Plug in your board and select yes when it asks if you want it to be the active board. You should see your board with a green USB icon under "Target" in the top left section of the interface.
- Build the blinky code (hammer icon). This will take a little while.
- Run the code (play button). Observe the blinky.

## 2: Printing Hello World
To get started with serial output, set up mbed_app.json to enable buffered serial and baudrate 115200:
```
{
    "target_overrides": {
        "*": {
            "target.components_add": [],
            "target.features_add": [],
            "target.features_remove": [],
            "target.c_lib": "std", 
            "target.printf_lib": "minimal", 
            "platform.stdio-buffered-serial": 1, 
            "platform.stdio-baud-rate": 115200
        }
    }
}
```
Modify the blinky example to include `printf("Hello world\n");` in the while loop. Set mbed studio to 115200 in the output tab for the board. Remember to include newlines with any printf that should display immediately.

## 3: RTOS basics: Threads and Sleep
TODO

## 4: RTOS basics: Thread Communication
TODO

## 5: RTOS basics: Thread Synchronization and Safety
TODO

## 6: Debugging in Mbed Studio
TODO

## 7: Further Reading
TODO

# Feature: GPIO
TODO
- also mention port / bus

# Feature: PWM
TODO

# Feature: ADC
TODO

# Feature: Interrupts
TODO
- pin
- timer (ticker)

# Feature: DAC
TODO

# Feature: I2C
TODO
- mention master and slave modes

Note that the I2C implementation is a little odd and [unbelievably, they didn't properly support using multiple devices on a single I2C bus](https://github.com/ARMmbed/mbed-os/issues/14004). More details [here](https://github.com/mbed-ce/mbed-os/issues/13). A long saga ensued which may have finally been fixed around July 2021 as seen [here](https://github.com/ARMmbed/mbed-os/pull/14805) and [here](https://github.com/ARMmbed/mbed-os/issues/14735). To use multiple devices, make sure the bus is operating at a speed suitable for the slowest device.

# Feature: SPI
- mention master and slave modes

# Feature: SPI or I2C Storage devices
Mbed actually has pretty good support for storage devices (eg winbond W25Q series for SPI, atmel AT24 series for I2C, SD cards). See [here for overview](https://os.mbed.com/docs/mbed-os/v6.15/apis/blockdevice-apis.html).
- [I2C, not in docs but in the class hierarchy](https://os.mbed.com/docs/mbed-os/v6.15/mbed-os-api-doxy/class_i2_c_e_e_block_device.html)
- [SPI](https://os.mbed.com/docs/mbed-os/v6.15/apis/spi-flash-block-device.html)

# Feature: Filesystems
As can be seen in [this section of the docs](https://os.mbed.com/docs/mbed-os/v6.15/apis/file-system-apis.html), FAT and LittleFS are both supported as well as some other interesting APIs like KVStore (key value) and a generic directory / file system.

# Feature: Serial output and printf/scanf
See [Hitchhiker’s Guide to Printf in Mbed 6](https://forums.mbed.com/t/hitchhikers-guide-to-printf-in-mbed-6/12492) by MonomialOutput. Take note of the target.printf_lib directive if you need full printf/scanf. A summary of "std" vs "minimal" printf is found [here](https://github.com/ARMmbed/mbed-os/tree/master/platform/source/minimal-printf#size-comparison).

## Buffered Serial
The buffered serial class can be used directly rather than through printf / scanf.
TODO

## Unbuffered Serial
TODO

## USB Serial
Note: Over USB, not using USBTX / USBRX connected to onboard STLink on Nucleo boards.
TODO

## Direct use of FILE object
TODO


# Feature: USB
USB is [surprisingly fully featured](https://os.mbed.com/docs/mbed-os/v6.15/apis/usb-apis.html) but it has to be supported on your target (This means that boards with USB pins but no header by default will need additional configuration). Many USB things are supported, including:
- Keyboard (and media keys)
- Mouse
- Virtual serial device
- Mass torage device
- Ethernet over USB
- Audio

More details on how USB is implemented in Mbed are [given here](https://os.mbed.com/docs/mbed-os/v6.15/apis/drivers-architecture.html).

# Feature: Wifi (ESP8266)
TODO

# Feature: Ethernet
TODO

# Configuration: mbed_app.json
This file is very important as it allows you to enable and disable features and change your c_lib and printf_lib. However, it is not created by default in Mbed Studio!
Here is an example mbed_app.json with a few things configured:
```
{
    "target_overrides": {
        "*": {
            "target.components_add": ["SPIF"],
            "target.features_add": ["STORAGE"],
            "target.features_remove": [],
            "target.c_lib": "std",
            "target.printf_lib": "minimal",
            "platform.stdio-baud-rate": 115200
        }
    }
}
```
Docs on mbed_app.json can be found [here](https://os.mbed.com/docs/mbed-os/v6.15/program-setup/advanced-configuration.html#configuration-parameters-in-mbed-app-json-mbed-lib-json).

# Hazard: (STM32) Calibrated ADC Readings
You might think that a fancy abstracted RTOS like mbed would use the ADC calibration available to each platform, however it does not. Check the datasheet of your STM32 to find out how to read your ADC calibration values and apply the correction factor. You will need to also read the internal temperature sensor ADC.
See [this gist](https://gist.github.com/SuperThunder/83fa784e13f39cfd27ca0ebee7cb5165) for an example for F303.

# QoL: Shared mbed library to save 1GB per application
Re-use the mbed os library from a single program (eg, the blinky program) where possible. Otherwise you are duplicating 1GB of Mbed files for each application in Mbed Studio.

# QoL: Reducing compile time (initial, and whenever a library or mbed config file is changed)
By default mbed has a LOT of extra stuff that it compiles for absolutely no reason on most projects. Zoltan Hudak has written a [.mbedignore file](https://forums.mbed.com/t/how-to-shorten-compilation-time-up-to-six-times/11203/22) that cuts out a lot of the unneeded libraries, drastically decreasing full-build compile time. However, make sure you do read through all the excluded directories and comment out any that you actually need.
TODO: put mbedignore example in repo

# QoL: Sane defaults Mbed app to clone
TODO
- mbed_app.json already created, filled in with common values to change, and annotated
- .mbedignore already written
- 115200 baud

# Good to know: Finding an Mbed feature
The compete list of APIs is [here](https://os.mbed.com/docs/mbed-os/v6.15/apis/index.html). Ctrl-f on this page is very helpful to finding a relevant feature (eg timer, or sleep, or queue).

# Good to know: Mbed forums
The only active place on the internet for mbed (as far as I am aware) is the [mbed OS forums](https://forums.mbed.com/c/mbed-os/6). Be nice to the community and ARM employees there, they probably didn't want to rush into whatever update has broken things either.

# Good to know: Mbed examples
There are actually a decent number of mbed os6 examples, but they are hard to find. One source is the official examples on the [mbed website](https://os.mbed.com/teams/mbed-os-examples/code/) or from the [ARMmbed github account](https://github.com/orgs/ARMmbed/repositories?q=example&type=all&language=&sort=).

# Good to know: Mbed libraries
Finding mbed libraries for components can be frustrating compared to the ease of arduino but libraries do exist for many components. A good place to start is the [mbed repository search](https://os.mbed.com/code/).

# Good to know: Target information
Want to know how to refer to the pins and peripherals of your microcontroller? The easiest way is to go straight to the source code that mbed uses to identify them. This can be found in mbed-os/targets/TARGET_<VENDOR>/TARGET_<FAMILY>. For example, for F303RE:
- [PeripheralNames.h](https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_STM/TARGET_STM32F3/PeripheralNames.h)
- [PinNames.h](https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_STM/TARGET_STM32F3/TARGET_STM32F303xE/TARGET_NUCLEO_F303RE/PinNames.h)
- [PeripheralPinMaps.h](https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_STM/TARGET_STM32F3/TARGET_STM32F303xE/TARGET_NUCLEO_F303RE/PeripheralPinMaps.h)
    
The possible options for default targets in mbed_app.json can be found [here](https://github.com/ARMmbed/mbed-os/blob/master/targets/targets.json).
  
On Nucleo boards, pins are labelled like "D3" and then the pinout diagram will refer to that as "PB3". Looking at PinNames.h you can immediately see the format Mbed is expecting is PB_3 or ARDUINO_UNO_D3.
  

# Good to know: GCC compiler
  By default, Mbed Studio uses the ARM compiler. In some cases you may want or need to [switch to GCC](https://os.mbed.com/docs/mbed-studio/current/installing/switching-to-gcc.html). In one case I had to switch to be able to work on an "old" (1 year...) project as the ARM compiler license for my version of Mbed studio had expired.
  
# Extra Info: Other suitable STM32 boards for starting off with Mbed
  Some other potentially suitable beginner-friendly options would be the Nucleo F411RE, F429ZI, F446ZE, or F401RE. Many other boards will work but may require additional configuration and/or installing a custom target.
    
# Extra Info: Custom targets
To use a custom target with mbed, you need a set of files that describes to mbed how to treat the board (what architecture is it, clock configuration, is USB present, what pin is the LED connected to, etc)
JojoS62 has a [GitHub repo with custom targets](https://github.com/JojoS62/custom_targets) for several common third party boards, including the powerful and affordable F411 BlackPill.
Zoltan Hudak has a custom target specifically [for F411 BlackPill](https://os.mbed.com/users/hudakz/code/BLACKPILL_Custom_Target/)


# The Big List of misc mbed lore
### Why are there 3 versions of mbed, and most google results are for the most outdated one?
Mbed went through 2 sudden changes when it went from 2 to 5 (becoming an RTOS with IoT functionality) and 5 to 6 (making large changes to the way Mbed OS worked). For years everyone had been happily using Mbed 2, and the initial release of 5 was a mess so everyone kept using 2. Once 5 was actually working quite well they introduced a shift to 6 which broke just enough things (some intentional, some bugs) to be seriously frustrating. This is why almost all the libraries and projects that show up in google results seem to be for Mbed 2 or 5: every major update actually seriously incentivized not updating for years.

### Bare metal vs RTOS?
There is a bare metal option in Mbed 5 and 6 that will use less flash / RAM but isn't able to use all functionality as it isn't an RTOS. It seems to be recommended as the migration path for Mbed 2 applications, or for using lower-end microcontrollers (eg F103C8 tier stuff). Details [here](https://os.mbed.com/docs/mbed-os/v6.15/bare-metal/index.html).
    
    
    
# Other tutorials / resources

