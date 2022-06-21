//The synchronization methods in Mbed are the same as found in POSIX / other RTOS / general RTOS theory

//Mbed mutex example: https://os.mbed.com/docs/mbed-os/v6.15/apis/mutex.html
//Mbed semaphore example: https://os.mbed.com/docs/mbed-os/v6.15/apis/semaphore.html
//Mbed conditionvariable example: https://os.mbed.com/docs/mbed-os/v6.15/apis/rtos-apis.html
//Mbed eventflags example: https://os.mbed.com/docs/mbed-os/v6.15/apis/eventflags.html

//CMSIS-RTOS docs on Mutex: https://www.keil.com/pack/doc/CMSIS/RTOS2/html/group__CMSIS__RTOS__MutexMgmt.html
//CMSIS-RTOS docs on EventFlags: https://www.keil.com/pack/doc/CMSIS/RTOS2/html/group__CMSIS__RTOS__EventFlags.html
//Producer-Consumer Condition variable usage example from a different RTOS: https://micro-os-plus.github.io/reference/cmsis-plus/classos_1_1rtos_1_1condition__variable.html


#include <mbed.h>

//This example is a little contrived, but imagine you need threads to pause and wait for some reason or another
//      - having threads wait for conditions to be set at startup or wake from sleep
//      - having a thread use wait_any to wait for one of several others to activate it
//      - signal something to a thread from an ISR context (eg an interrupt fires from a button or UART or other peripheral and records some data while also setting the flag to inform the thread there is data to process)

EventFlags task_flags; //https://os.mbed.com/docs/mbed-os/v6.15/apis/eventflags.html
Thread first_task;
Thread second_task;

//set up a bit 16 flag (max is 31). Basically, pick each bit to mean some truth condition.
#define TASK_FLAG1 (1UL << 16)
//and bit 4
#define TASK_FLAG2 (1UL << 4)

//Suppose this task needs to run first
//eg, this is meant to collect some data
//we will also suppose it waits for the other thread before running again
void first_task_loop()
{
    printf("First task running\n");
    while(true)
    {
        //do some stuff...
        printf("First task doing some work (1s)\n");
        ThisThread::sleep_for(1s);
        
        //setting flags is just an int of what flags to set
        task_flags.set(TASK_FLAG1);

        //wait for TASK_FLAG2 (bit 4) to be set
        //you can also wait for flags for a certain duration or until an absolute time
        //you can also use wait_any to wait for just one flag instead of all
        //Note that by default, the specified flags will be cleared after waiting
        task_flags.wait_all(TASK_FLAG2);
        printf("\n\n");
    }
}

//And this task needs to run only after the first one is done
//eg, it waits for the data to be collected (perhaps from multiple sources)
//and then signals to the original task it can run again
void second_task_loop()
{
    printf("Second task running\n");
    while(true)
    {
        task_flags.wait_all(TASK_FLAG1);

        //do some stuff...
        printf("Second task doing some work (3s)\n");
        ThisThread::sleep_for(3s);

        task_flags.set(TASK_FLAG2);
    }
}


int main()
{
    task_flags.clear();

    first_task.start(first_task_loop);
    second_task.start(second_task_loop);

    while(true)
    {
        ThisThread::sleep_for(10s);
    }
}