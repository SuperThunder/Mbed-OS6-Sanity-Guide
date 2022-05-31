# Mbed OS Features
Notes on built in features of MbedOS (hardware related like ADC, PWM, etc but also software like filesystem support)
## GPIO
TODO
- also mention port / bus

### PWM
TODO

### ADC
TODO

### Interrupts
TODO
- pin
- timer (ticker / timeout + LP versions)
- mention EventQueue API + docs page (event loop example)
    - interesting example of deferring printf calls

### Callbacks
#### (std::function ish)
Mbed has a special class called Callback that gives functionality like std::function. 
- As a type declaration it is written as Callback<ret_type(argtype1, argtype2, ...)>
- when actually creating a Callback it is written as callback(myFunction) for a standalone function 
- or callback(class_instance, ClassName::myFunction) for a member function with the state of a specific object it can act on.

### DAC
TODO

### I2C
TODO
- mention master and slave modes

Note that the I2C implementation is a little odd and [unbelievably, they didn't properly support using multiple devices on a single I2C bus](https://github.com/ARMmbed/mbed-os/issues/14004). More details [here](https://github.com/mbed-ce/mbed-os/issues/13). A long saga ensued which may have finally been fixed around July 2021 as seen [here](https://github.com/ARMmbed/mbed-os/pull/14805) and [here](https://github.com/ARMmbed/mbed-os/issues/14735). To use multiple devices, make sure the bus is operating at a speed suitable for the slowest device.

### SPI
- mention master and slave modes

### SPI, I2C, and SD Storage devices
Mbed actually has pretty good support for storage devices (eg winbond W25Q series for SPI, atmel AT24 series for I2C, SD cards). See [here for overview](https://os.mbed.com/docs/mbed-os/v6.15/apis/blockdevice-apis.html).
- [I2C, not in docs but in the class hierarchy](https://os.mbed.com/docs/mbed-os/v6.15/mbed-os-api-doxy/class_i2_c_e_e_block_device.html)
- [SPI](https://os.mbed.com/docs/mbed-os/v6.15/apis/spi-flash-block-device.html)
- SD TODO

### Filesystems
As can be seen in [this section of the docs](https://os.mbed.com/docs/mbed-os/v6.15/apis/file-system-apis.html), FAT and LittleFS are both supported as well as some other interesting APIs like KVStore (key value) and a generic directory / file system.

### Serial output and printf/scanf
See [Hitchhiker’s Guide to Printf in Mbed 6](https://forums.mbed.com/t/hitchhikers-guide-to-printf-in-mbed-6/12492) by MonomialOutput. Take note of the target.printf_lib directive if you need full printf/scanf. A summary of "std" vs "minimal" printf is found [here](https://github.com/ARMmbed/mbed-os/tree/master/platform/source/minimal-printf#size-comparison).

### Buffered Serial
The buffered serial class can be used directly rather than through printf / scanf. If you want to access the FileHandle of a BufferedSerial std in/out, see [this post](https://forums.mbed.com/t/hitchhikers-guide-to-printf-in-mbed-6/12492/28)
TODO

### Unbuffered Serial
TODO

### USB Serial
Note: Over USB, not using USBTX / USBRX connected to onboard STLink on Nucleo boards.
TODO

### Direct use of FILE object
TODO

### Time APIs
- Timer (~microseconds)
- LowPowerTimer (milliseconds + can go into deep sleep)
- time(NULL) -> seconds since epoch
- Kernel whatever

### USB
USB is [surprisingly fully featured](https://os.mbed.com/docs/mbed-os/v6.15/apis/usb-apis.html) but it has to be supported on your target (This means that boards with USB pins but no header by default will need additional configuration). Many USB things are supported, including:
- Keyboard (and media keys)
- Mouse
- Virtual serial device
- Mass storage device
- Ethernet over USB
- Audio

More details on how USB is implemented in Mbed are [given here](https://os.mbed.com/docs/mbed-os/v6.15/apis/drivers-architecture.html).

### Wifi (ESP8266)
TODO

  
# Extra Information
### GCC compiler
By default, Mbed Studio uses the ARM compiler. In some cases you may want or need to [switch to GCC](https://os.mbed.com/docs/mbed-studio/current/installing/switching-to-gcc.html). In one case I had to switch to be able to work on an "old" (1 year...) project as the ARM compiler license for my version of Mbed studio had expired.
  
### Mbed OS Custom targets (custom boards)
To use a custom target with mbed, you need a set of files that describes to mbed how to treat the board (what architecture is it, clock configuration, is USB present, what pin is the LED connected to, etc)
JojoS62 has a [GitHub repo with custom targets](https://github.com/JojoS62/custom_targets) for several common third party boards, including the powerful and affordable F411 BlackPill.
Zoltan Hudak has a custom target specifically [for F411 BlackPill](https://os.mbed.com/users/hudakz/code/BLACKPILL_Custom_Target/).

### Target pin assignment information
Want to know how to refer to the pins and peripherals of your microcontroller? The easiest way is to go straight to the source code that mbed uses to identify them. This can be found in mbed-os/targets/TARGET_<VENDOR>/TARGET_<FAMILY>. For example, for F303RE:
- [PeripheralNames.h](https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_STM/TARGET_STM32F3/PeripheralNames.h)
- [PinNames.h](https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_STM/TARGET_STM32F3/TARGET_STM32F303xE/TARGET_NUCLEO_F303RE/PinNames.h)
- [PeripheralPinMaps.h](https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_STM/TARGET_STM32F3/TARGET_STM32F303xE/TARGET_NUCLEO_F303RE/PeripheralPinMaps.h)
    
The possible options for default targets in mbed_app.json can be found [here](https://github.com/ARMmbed/mbed-os/blob/master/targets/targets.json).
  
On Nucleo boards, pins are labelled like "D3" and then the pinout diagram will refer to that as "PB3". Looking at PinNames.h you can immediately see the format Mbed is expecting is PB_3 or ARDUINO_UNO_D3.



# Mbed Studio Notes
#### Note: Debugger Variable display issue
As discussed [here](https://forums.mbed.com/t/mbed-studio-debugger-not-showing-variables/12931), Mbed studio sometimes doesn't show your variables and gives a cryptic error message. You can try the workaround there of using `-stack-list-locals <0 or 1 or 2>`.

### Note: Debugger Optimization level
As described [here](https://forums.mbed.com/t/mbed-studio-debugging-show-wrong-source-file-and-line/13599), the default debug build profile in Mbed Studio (ARM C6) still uses -O1. This can result in odd behaviour from lines being optimized out, so your breakpoint will not stop where you expect it to. You may need to change the profile temporarily to get (closer to) the expected debugger behaviour.

# STM32 Specific Notes
### Hazard: (STM32) Calibrated ADC Readings
You might think that a fancy abstracted RTOS like mbed would use the ADC calibration available to each platform, however it does not. Check the datasheet of your STM32 to find out how to read your ADC calibration values and apply the correction factor. You will need to also read the internal temperature sensor ADC.
See [this gist](https://gist.github.com/SuperThunder/83fa784e13f39cfd27ca0ebee7cb5165) for an example for F303.

### Core-Coupled Memory (CCM)
See discussion [here](https://github.com/ARMmbed/mbed-os/issues/8220). Mbed seems to generally ignore the CCM available for a target. Some linker script editing allows you to specify if memory should be placed in the CCM area, which can only be accessed by the CPU (no DMA). For example, creating a large buffer to use for a processing algorithm. Since the memory is core-coupled, there is meant to be a significant reduction in latency.

# Misc mbed lore
### Why are there 3 versions of mbed, and most google results are for the most outdated one?
Mbed went through 2 sudden changes when it went from 2 to 5 (becoming an RTOS with IoT functionality) and 5 to 6 (making large changes to the way Mbed OS worked). For years everyone had been happily using Mbed 2, and the initial release of 5 was a mess so everyone kept using 2. Once 5 was actually working quite well they introduced a shift to 6 which broke just enough things (some intentional, some bugs) to be seriously frustrating. This is why almost all the libraries and projects that show up in google results seem to be for Mbed 2 or 5: every major update actually seriously incentivized not updating for years.

### Bare metal vs RTOS?
There is a bare metal option in Mbed 5 and 6 that will use less flash / RAM but isn't able to use all functionality as it isn't an RTOS. It seems to be recommended as the migration path for Mbed 2 applications, or for using lower-end microcontrollers (eg F103C8 tier stuff). Details [here](https://os.mbed.com/docs/mbed-os/v6.15/bare-metal/index.html).

### What vendors have good mbed support?
As far as I can tell, the vendors with the best Mbed support are 
- STMicro
- NXP
- Cypress (probably - The PSoC 6 Wifi/BT has a decent number of examples [here](https://iotexpert.com/mouser-psoc-6-wifi-bt-mbed-l1-developer-resources/)). Incidentally the PSoC 6 wifi / BT seems like great value for getting Wifi, Bluetooth, 2M flash, 1M SRAM, and M4 150Mhz, and a bunch of built in IO.
- Maxim (probably - haven't found much discussion)
- (up to Mbed 5.15) Nordic, who have now discontinued Mbed in favour of Zephyr

Check in the boards list that it [supports OS 6.15](https://os.mbed.com/platforms/?q=&Mbed+OS+support=Mbed+OS+6.15) (or whatever is the latest version now).