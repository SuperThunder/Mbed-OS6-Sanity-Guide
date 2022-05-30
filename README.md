# Mbed OS6 Sanity-Preserving Introduction and Notes Collection
A sanity-preserving introduction to Mbed OS 6 and my collection of notes. Aiming to collect the reasonable documentation that does exist for Mbed OS 6 but can only be found by ignoring the thousands of outdated docs, examples, and forum discussions.

Mbed OS is overall a very solid RTOS with good device support and arduino-like abstraction while being much more powerful. However, it is still obscure and at least in large part it has to be due to how frustrating it is to start. This guide aims to give an easy getting started path, and also collect resources on the easy and correct way to do things with Mbed OS 6.


# Introduction: A Sane Start
Mbed has many options for development: legacy compiler, keil studio online, mbed studio, mbed cli 1, mbed cli 2. This guide will use **Mbed Studio with Mbed 6.15**. The board used in writing this guide is the Nucleo F303RE. Some STM32-specific details will be present but overall it should be the same with other vendors' boards.

Note that I am not a particularly experienced C, C++, Mbed, or embedded developer, so this guide is not meant to be authoritative or contain perfect code and practices. It is meant to help in getting started with Mbed OS 6 without getting confused and giving up, particularly for someone that has spent some time with classic Arduino (AVR, 2K SRAM, one big loop, many globals, etc) and wants to try some projects on powerful 32 bit MCUs with more advanced abstractions (threads, queues, semaphores, etc).

## 1: Software and blinky
Reference: [Mbed docs quick start](https://os.mbed.com/docs/mbed-studio/current/getting-started/index.html)
- Acquire an mbed-capable board, ideally one of the better supported boards [with official mbed studio debug support](https://os.mbed.com/platforms/?q=&Mbed+Studio+support=Debug). Suggestion: Nucleo F303 or F411, they are <$15USD, widely used, and have a generous amount of flash and RAM.
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

Serial on mbed can get a lot more complicated (see feature section on serial) but this is a good place to start to have basic printf / scanf.

## 3: Mbed RTOS basics: Threads, Sleep, and Timers
TODO
TODO include a callback to a function
TODO mention how to handle callback to member function with state (object instance) -> platform concepts callback page


## 4: Mbed RTOS basics: Thread Communication
TODO
...how do we share information between threads?...
- global (unsafe)
- queue / mail

## 5: Mbed RTOS basics: Thread Synchronization and Safety
TODO
...how can a thread cause something to happen?...
...how can 2 threads access the same resource?...
...how can a thread wait for another thread?...

## 7: Mbed Studio: Debugging basics
Reference on debugging [here](https://os.mbed.com/docs/mbed-studio/current/monitor-debug/debugging-with-mbed-studio.html).

...todo some basic app with a couple of threads, button or interrupt ISRs, ticker events to show debugging...
...todo running debug...
...todo thread view...
...todo continue / step / pause...
...todo breakpoints...
...some basic commands for the debug cmd...

#### Variable display issue
As discussed [here](https://forums.mbed.com/t/mbed-studio-debugger-not-showing-variables/12931), Mbed studio sometimes doesn't show your variables and gives a cryptic error message. You can try the workaround there of using `-stack-list-locals <0 or 1 or 2>`.

### Optimization level
As described [here](https://forums.mbed.com/t/mbed-studio-debugging-show-wrong-source-file-and-line/13599), the default debug build profile in Mbed Studio (ARM C6) still uses -O1. This can result in odd behaviour from lines being optimized out, so your breakpoint will not stop where you expect it to. You may need to change the profile temporarily to get (closer to) the expected debugger behaviour.

## 8: Mbed hardware APIs: GPIO, Interrupts, ADC, PWM, I2C, SPI
Note: Called drivers by Mbed.
TODO

## 8: How do I figure out how to do something in Mbed OS I could do with Arduino?
One way to design RTOS apps is threads with responsibilities for specific tasks that communicate with each other as necessary. Blocked or sleeping threads are pre-empted, allowing for other tasks to run.  Using RTOS primitives (threads, queues, events, flags, mutexes) and C++ methods (objects with state rather than globals, functionality split up by classes) generally results in more modern and clean designs. 

So your initial plan will list the various things you want your program to do, and then which thread will be responsible for each thing. Then, by what means threads will communicate and synchronize. Most RTOS have very similar primitives that match concepts discussed in RTOS textbooks, so discussion and tutorials on how to do things with other RTOS (FreeRTOS, Mynewt, Zephyr, etc) will be very relevant to Mbed OS too. 

Another major way to design RTOS apps is to use events in event queues to structure how the system reacts to inputs. Events can quickly be registered in ISRs or other threads and then handled later by the relevant function (which run in a thread created by the EventQueue class).

However, this can all be hard to think about when starting out with Mbed. For example, in Arduino projects it is fairly common to have a large set of global variables, a superloop, and have different sections of code communicate through global variables. When starting with Mbed it may be easier to think about putting each section of superloop code in its own thread that runs at the appropriate interval using ThisThread::sleep and keep the same structure of global variables, but now use Mutexes to synchronize thread access. Later, an appropriate RTOS communication method can replace the majority of the global variables, particularly the ones used for communication. Globals used for ISRs can be enclosed in objects that control external access to the volatile variable.

As for how to literally do things in Mbed OS that are possible in Arduino with respect to libraries and peripheral support, you will need to track down a library written for Mbed OS 6, port one written for Mbed OS 5 (probably minimal or no changes), or port the Arduino library (likely medium effort, you can also write your own library using the Arduino one as a guide).

## 9: Further Reading
The Mbed OS [full API reference](https://os.mbed.com/docs/mbed-os/v6.15/apis/index.html) contains information about most parts of Mbed OS with some small explanations and examples. It's worth skimming through the list to see what sort of things are available. If you are still using Mbed in a month, then skim the entire documentation - Mbed OS has so many neat features and customizable parameters.
There is also a section on [platform concepts](https://os.mbed.com/docs/mbed-os/v6.15/apis/platform-concepts.html) that explains some of the more important mbed-specific concepts.
Finally, figuring out things in mbed sometimes involves searching through the [forums](https://forums.mbed.com/c/mbed-os/6) or [class hierarchy](https://os.mbed.com/docs/mbed-os/v6.15/mbed-os-api-doxy/hierarchy.html) or even checking the [source code](https://github.com/ARMmbed/mbed-os/tree/mbed-os-6.15.1).

The rest of this guide describes specific features of Mbed OS 6 and some examples of their use can be found in the Examples directory. The [Mbed OS documentation](https://os.mbed.com/docs/mbed-os/v6.15/apis/index.html) contains examples for standalone usage of most APIs, so the examples here focus more on practical usage or what is unclear in the Mbed docs. Also, there are various tips on how to find information and examples for Mbed and some bits of useful information I have found on the forums and in bug reports / PRs.


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
- timer (ticker / timeout + LP versions)
- mention EventQueue API + docs page (event loop example)
    - interesting example of deferring printf calls

# Feature: Callbacks
#### (std::function ish)
Mbed has a special class called Callback that gives functionality like std::function. 
- As a type declaration it is written as Callback<ret_type(argtype1, argtype2, ...)>
- when actually creating a Callback it is written as callback(myFunction) for a standalone function 
- or callback(class_instance, ClassName::myFunction) for a member function with the state of a specific object it can act on.

# Feature: DAC
TODO

# Feature: I2C
TODO
- mention master and slave modes

Note that the I2C implementation is a little odd and [unbelievably, they didn't properly support using multiple devices on a single I2C bus](https://github.com/ARMmbed/mbed-os/issues/14004). More details [here](https://github.com/mbed-ce/mbed-os/issues/13). A long saga ensued which may have finally been fixed around July 2021 as seen [here](https://github.com/ARMmbed/mbed-os/pull/14805) and [here](https://github.com/ARMmbed/mbed-os/issues/14735). To use multiple devices, make sure the bus is operating at a speed suitable for the slowest device.

# Feature: SPI
- mention master and slave modes

# Feature: SPI, I2C, and SD Storage devices
Mbed actually has pretty good support for storage devices (eg winbond W25Q series for SPI, atmel AT24 series for I2C, SD cards). See [here for overview](https://os.mbed.com/docs/mbed-os/v6.15/apis/blockdevice-apis.html).
- [I2C, not in docs but in the class hierarchy](https://os.mbed.com/docs/mbed-os/v6.15/mbed-os-api-doxy/class_i2_c_e_e_block_device.html)
- [SPI](https://os.mbed.com/docs/mbed-os/v6.15/apis/spi-flash-block-device.html)
- SD TODO

# Feature: Filesystems
As can be seen in [this section of the docs](https://os.mbed.com/docs/mbed-os/v6.15/apis/file-system-apis.html), FAT and LittleFS are both supported as well as some other interesting APIs like KVStore (key value) and a generic directory / file system.

# Feature: Serial output and printf/scanf
See [Hitchhiker’s Guide to Printf in Mbed 6](https://forums.mbed.com/t/hitchhikers-guide-to-printf-in-mbed-6/12492) by MonomialOutput. Take note of the target.printf_lib directive if you need full printf/scanf. A summary of "std" vs "minimal" printf is found [here](https://github.com/ARMmbed/mbed-os/tree/master/platform/source/minimal-printf#size-comparison).

## Buffered Serial
The buffered serial class can be used directly rather than through printf / scanf. If you want to access the FileHandle of a BufferedSerial std in/out, see [this post](https://forums.mbed.com/t/hitchhikers-guide-to-printf-in-mbed-6/12492/28)
TODO

## Unbuffered Serial
TODO

## USB Serial
Note: Over USB, not using USBTX / USBRX connected to onboard STLink on Nucleo boards.
TODO

## Direct use of FILE object
TODO

# Feature: Time APIs
- Timer (~microseconds)
- LowPowerTimer (milliseconds + can go into deep sleep)
- time(NULL) -> seconds since epoch
- Kernel whatever

# Feature: USB
USB is [surprisingly fully featured](https://os.mbed.com/docs/mbed-os/v6.15/apis/usb-apis.html) but it has to be supported on your target (This means that boards with USB pins but no header by default will need additional configuration). Many USB things are supported, including:
- Keyboard (and media keys)
- Mouse
- Virtual serial device
- Mass storage device
- Ethernet over USB
- Audio

More details on how USB is implemented in Mbed are [given here](https://os.mbed.com/docs/mbed-os/v6.15/apis/drivers-architecture.html).

# Feature: Wifi (ESP8266)
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

Discussion on how to find the specific configurable parameters (eg UART buffer size) found [here](https://forums.mbed.com/t/hitchhikers-guide-to-printf-in-mbed-6/12492/19)

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
There are actually a decent number of mbed os6 examples, but they are easy to miss. They are summarized in the [tutorials and examples section](https://os.mbed.com/docs/mbed-os/v6.15/tutorials/index.html) of the docs. Repositories for them can be found on the [ARMmbed github account](https://github.com/orgs/ARMmbed/repositories?q=example&type=all&language=&sort=).

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
Zoltan Hudak has a custom target specifically [for F411 BlackPill](https://os.mbed.com/users/hudakz/code/BLACKPILL_Custom_Target/).

# Extra Info: Core-Coupled Memory (CCM)
See discussion [here](https://github.com/ARMmbed/mbed-os/issues/8220). Mbed seems to generally ignore the CCM available for a target. Some linker script editing allows you to specify if memory should be placed in the CCM area, which can only be accessed by the CPU (no DMA). For example, creating a large buffer to use for a processing algorithm. Since the memory is core-coupled, there is meant to be a significant reduction in latency.

# Example: Basic interactive console application
TODO

# Example: Serial over USB
## Board with built in USB (Black Pill F411)
TODO
## Board without built in USB (Nucleo F303RE)
TODO



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
    
    
# Other tutorials / resources
- Hackaday mbed USB volume control with F411: https://hackaday.com/2022/04/19/arm-pumps-up-the-volume-with-mbed-and-a-potentiometer/
- EFI ECU project written in mbed: https://github.com/FL0WL0W/EFIGenie