#include <mbed.h>

Thread thr1;
Thread thr2(osPriorityLow, OS_STACK_SIZE); //different priority, default stack size
Thread thr3(osPriorityNormal, 1024); //use non-default amount of stack for the thread

//For a ticker example, see the LED flipper at https://os.mbed.com/docs/mbed-os/v6.15/apis/ticker.html

void example_thread_loop()
{
    printf("Hello from example thread\n");

    //We need an infinite loop, or the thread would exit
    while(true)
    {
        ThisThread::sleep_for(1s);
    }
}

void example_thread_param(int* n)
{
    printf("Got number in param example thread %d\n", *n);

    while(true)
    {
        ThisThread::sleep_for(1s);
    }
}

class ClassThreadExample
{
    public:
        ClassThreadExample(int n)
        {
            some_var = n;
        }
        void thread_example()
        {
            printf("Hello from class thread example\nValue is %d", some_var);
            while(true)
            {
                ThisThread::sleep_for(1s);
            }
        }
    private:
        int some_var;
};

// main() runs in its own thread in the OS
int main()
{

    printf("Hello from main thread\n");

    //Function reference casted to Callback
    thr1.start(&example_thread_loop);

    int num = 56;
    thr2.start(callback(example_thread_param, &num));

    ClassThreadExample cte(645);
    thr3.start(callback(&cte, &ClassThreadExample::thread_example));

    //If we don't loop infinitely at the end of main, then the main thread exits
    //The main downside of this is the wasted stack space of the main thread (default 4KB, can be changed in mbed_app.json)
    while (true) {
        //Time constants like "1s", "150ms", etc can be used and come from Mbed Chrono::Duration
        ThisThread::sleep_for(1s);
    }
}

