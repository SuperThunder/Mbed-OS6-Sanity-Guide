# Mbed OS6 Sanity-Preserving Introduction and Notes Collection
A sanity-preserving introduction to Mbed OS 6 and my collection of notes. Aiming to collect the reasonable documentation that does exist for Mbed OS 6 but can only be found by ignoring the years of outdated docs, examples, and forum discussions.

Mbed OS is overall a very solid RTOS with good device support and arduino-like abstraction while being much more powerful. However, it is still obscure and at least in large part it has to be due to how frustrating it is to start. This guide aims to give an easy getting started path, and also collect resources on the easy and correct way to do things with Mbed OS 6.

![The Nucleo F303RE](Notes/resource/nucleof303re.jpg)

# Introduction: A Sane Start
Mbed has many options for development: legacy compiler, keil studio online, mbed studio, mbed cli 1, mbed cli 2, dozens of development boards, and 5 distinct versions. This guide will use **Mbed Studio with Mbed 6.15**. The board used in writing this guide is the **Nucleo F303RE**.

This guide is meant to help in getting started with Mbed OS 6 without getting confused and giving up, particularly for someone that has spent some time with classic Arduino (AVR, 2K SRAM, one big loop, many globals, etc) and wants to try some projects on powerful 32 bit MCUs with more advanced abstractions (threads, queues, mutexes, etc).

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


TODO start with basic threaded program
TODO explain how main thread keeps running
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



## 8: Mbed hardware APIs: GPIO, Interrupts, ADC, PWM, I2C, SPI
Note: Called drivers by Mbed.
TODO

## 8: How do I figure out how to do something in Mbed OS I could do with Arduino?
One way to design RTOS apps is threads with responsibilities for specific tasks that communicate with each other as necessary. Blocked or sleeping threads are pre-empted, allowing for other tasks to run.  Using RTOS primitives (threads, queues, events, flags, mutexes) and C++ methods (objects with state rather than globals, functionality split up by classes) generally results in more modern and clean designs. 

So your initial plan will list the various things you want your program to do, and then which thread will be responsible for each thing. Then, by what means threads will communicate and synchronize. Most RTOS have very similar primitives that match concepts discussed in RTOS textbooks, so discussion and tutorials on how to do things with other RTOS (FreeRTOS, Mynewt, Zephyr, etc) will be very relevant to Mbed OS too. 

Another major way to design RTOS apps is to use events in event queues to structure how the system reacts to inputs. Events can quickly be registered in ISRs or other threads and then handled later by the relevant function (which run in a thread created by the EventQueue class).

However, this can all be hard to think about when starting out with Mbed. For example, in Arduino projects it is fairly common to have a large set of global variables, a superloop, and have different sections of code communicate through global variables. When starting with Mbed it may be easier to think about putting each section of superloop code in its own thread that runs at the appropriate interval using ThisThread::sleep and keep the same structure of global variables, but now use Mutexes to synchronize thread access. Later, an appropriate RTOS communication method can replace the majority of the global variables, particularly the ones used for communication. Globals used for ISRs can be enclosed in objects that control external access to the volatile variable.

As for how to literally do things in Mbed OS that are possible in Arduino with respect to libraries and peripheral support, you will need to track down a library written for Mbed OS 6, port one written for Mbed OS 5 (probably minimal or no changes), or port the Arduino library (likely medium effort, you can also write your own library using the Arduino one as a guide).

## 9: What do I do when I get *really* stuck?
Now is a good time to explain what it takes to make reasonable progress on an Mbed project (really, any embedded project) when you run into issues:
1. Read the Mbed OS [docs](https://os.mbed.com/docs/mbed-os/v6.15/apis/index.html) related to whatever you are doing thoroughly.
2. Search the forums (the built in search tool and Google for the occasional result in blogs, stackoverflow, etc) for any problems you're having - there will usually be someone else with the question, and they usually did get some kind of answer.
3. Understand your underlying hardware - Mbed OS is a decent abstraction but not a perfect one. If you're doing something that makes non-trivial use of an MCU peripheral, it's a good idea to read the datasheet to find out what you're really using underneath. If you run into problems, you definitely need to check the datasheet and the vendor forums. Check the MCU errata too, silicon vendors make a [shocking amount](https://www.reddit.com/r/embedded/comments/htcdnw/lpt_when_you_are_starting_with_a_new_mcu_read/) of [mistakes in silicon](https://www.reddit.com/r/embedded/comments/qi5f97/are_modern_socs_becoming_less_usable/). While you're on the vendors website, check out their other documents too - application notes are 100 times more readable than datasheets and usually give great instructions on how to get things done. Reference manuals and programming manuals will give a much more straightforward answer of "how" to use an MCU than the datasheet.
4. See what clues you can find with the debugger.
5. If all of the above fails, ask nicely on the Mbed forums or r/embedded. Make sure to explain what you are trying to do overall, what your problem is, and what you've tried so far to solve it.

# 10: Configuration: mbed_app.json
This file is very important as it allows you to enable and disable features and change your c_lib and printf_lib. However, it is not created by default in Mbed Studio!
Here is an example mbed_app.json with a few things configured:
```
{
    "target_overrides": {
        "*": {
            "target.components_add": [],
            "target.features_add": [],
            "target.features_remove": []
            "target.c_lib": "std",
            "target.printf_lib": "minimal",
            "platform.stdio-baud-rate": 115200,
            "platform.stdio-buffered-serial": 1
        }
    }
}
```

Docs on mbed_app.json can be found [here](https://os.mbed.com/docs/mbed-os/v6.15/program-setup/advanced-configuration.html#configuration-parameters-in-mbed-app-json-mbed-lib-json).
- [RTOS configuration params](https://os.mbed.com/docs/mbed-os/v6.15/apis/scheduling-options-and-config.html)
- [Mbed Driver params](https://os.mbed.com/docs/mbed-os/v6.15/apis/drivers-options-and-config.html)
- [Mbed platform params](https://os.mbed.com/docs/mbed-os/v6.15/apis/drivers-options-and-config.html)
- [Mbed data store params](https://os.mbed.com/docs/mbed-os/v6.15/program-setup/api-specific-configuration.html)
- [Mbed connectivity params](https://os.mbed.com/docs/mbed-os/v6.15/apis/connectivity-options-and-config.html) (see the protocol specific params too)

Configuration can get quite advanced, such as setting [target-specific parameters](https://os.mbed.com/docs/mbed-os/v6.15/program-setup/advanced-configuration.html).

Discussion on how to find the specific configurable parameters (eg UART buffer size) found [here](https://forums.mbed.com/t/hitchhikers-guide-to-printf-in-mbed-6/12492/19)

## 11: Further Reading
The Mbed OS [full API reference](https://os.mbed.com/docs/mbed-os/v6.15/apis/index.html) contains information about most parts of Mbed OS with some small explanations and examples. It's worth skimming through the list to see what sort of things are available. If you are still using Mbed in a month, then skim the entire documentation - Mbed OS has so many neat features and customizable aspects.
There is also a section on [platform concepts](https://os.mbed.com/docs/mbed-os/v6.15/apis/platform-concepts.html) that explains some of the more important mbed-specific concepts.
Finally, figuring out things in mbed sometimes involves searching through the [forums](https://forums.mbed.com/c/mbed-os/6) or [class hierarchy](https://os.mbed.com/docs/mbed-os/v6.15/mbed-os-api-doxy/hierarchy.html) or even checking the [source code](https://github.com/ARMmbed/mbed-os/tree/mbed-os-6.15.1).



# Tips for Mbed / Mbed Studio
### Studio: Shared mbed library to save 1GB per application
Re-use the mbed os library from a single program (eg, the blinky program) where possible. Otherwise you are duplicating 1GB of Mbed files for each application in Mbed Studio.

### Reducing compile time (initial, and whenever a library or mbed config file is changed)
By default mbed has a LOT of extra stuff that it compiles for absolutely no reason on most projects. Zoltan Hudak has written a [.mbedignore file](https://forums.mbed.com/t/how-to-shorten-compilation-time-up-to-six-times/11203/22) that cuts out a lot of the unneeded libraries, drastically decreasing full-build compile time. However, make sure you do read through all the excluded directories and comment out any that you actually need.
TODO: put mbedignore example in repo

# Mbed examples
There are actually a decent number of mbed os6 examples, but they are easy to miss. They are summarized in the [tutorials and examples section](https://os.mbed.com/docs/mbed-os/v6.15/tutorials/index.html) of the docs. Repositories for them can be found on the [ARMmbed github account](https://github.com/orgs/ARMmbed/repositories?q=example&type=all&language=&sort=).

# Mbed libraries
Finding mbed libraries for components can be frustrating compared to the ease of arduino but libraries do exist for many components. A good place to start is the [mbed repository search](https://os.mbed.com/code/).


# Examples
A number of examples can be found in the Examples/ directory of this repository. The [Mbed OS documentation](https://os.mbed.com/docs/mbed-os/v6.15/apis/index.html) contains examples for standalone usage of most APIs, so the examples here focus more on practical usage or what is unclear in the Mbed docs.


    
# Other tutorials / resources
- Hackaday mbed USB volume control with F411: https://hackaday.com/2022/04/19/arm-pumps-up-the-volume-with-mbed-and-a-potentiometer/
- EFI ECU project written in mbed: https://github.com/FL0WL0W/EFIGenie
- Mbed audio player example: 

# Other suitable STM32 boards for starting off with Mbed
Some other potentially suitable beginner-friendly options would be the Nucleo F411RE, F429ZI, F446ZE, or F401RE. Many other boards will work but may require additional configuration and/or installing a custom target.