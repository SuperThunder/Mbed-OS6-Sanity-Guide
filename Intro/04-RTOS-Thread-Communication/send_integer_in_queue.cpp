//How to send an int directly
//This is a misuse of pointers to send 32 bit values directly in the queue

#include <mbed.h>

Thread get_thread(osPriorityNormal, OS_STACK_SIZE, nullptr, "get thread");
Thread put_thread(osPriorityNormal, OS_STACK_SIZE, nullptr, "put thread");

Queue<int, 32> q1;

void put_loop()
{
    int count = 0;
    while(true)
    {
        //All of the queue entries will point to this one integer pointer, but both threads are waiting 1 second between get/put
        //This is only for example purposes
        int* forced_value_into_pointer;
        // Store the value of count in the int pointer
        forced_value_into_pointer = (int*)count;

        //The queue copies the value of the pointer, so you can re-use your pointer here
        bool put_status = q1.try_put(forced_value_into_pointer);

        count++;

        ThisThread::sleep_for(1s);
    }
}

void get_loop()
{
    //Class ref: "The message is stored in the location pointed to be the parameter data_out"
    int* queue_ptr;
    //Provide a pointer to a pointer so the queue knows where to store the pointer
    int** queue_ptr_ptr = &queue_ptr;

    while(true)
    {
        bool get_status = q1.try_get(queue_ptr_ptr);
        
        if(get_status)
        {
            printf("Get loop got: %d\n", queue_ptr);
            //OR:
            //printf("Get loop got: %d\n", *queue_ptr_ptr);
        }

        ThisThread::sleep_for(1s);
    }
}

int main()
{
    printf("Queue example\n");

    put_thread.start(put_loop);
    get_thread.start(get_loop);

    while(true)
    {
        ThisThread::sleep_for(1s);
    }
}
