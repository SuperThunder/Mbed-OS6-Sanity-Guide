//The synchronization methods in Mbed are the same as found in POSIX / other RTOS / general RTOS theory

//Mbed mutex example: https://os.mbed.com/docs/mbed-os/v6.15/apis/mutex.html
//Mbed semaphore example: https://os.mbed.com/docs/mbed-os/v6.15/apis/semaphore.html
//Mbed conditionvariable example: https://os.mbed.com/docs/mbed-os/v6.15/apis/rtos-apis.html
//Mbed eventflags example: https://os.mbed.com/docs/mbed-os/v6.15/apis/eventflags.html

//CMSIS-RTOS docs on Mutex: https://www.keil.com/pack/doc/CMSIS/RTOS2/html/group__CMSIS__RTOS__MutexMgmt.html
//CMSIS-RTOS docs on EventFlags: https://www.keil.com/pack/doc/CMSIS/RTOS2/html/group__CMSIS__RTOS__EventFlags.html
//Producer-Consumer Condition variable usage example from a different RTOS: https://micro-os-plus.github.io/reference/cmsis-plus/classos_1_1rtos_1_1condition__variable.html


//TODO example with EventFlags (one thread waits for signal from another)