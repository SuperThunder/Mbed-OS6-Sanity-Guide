//This is in the style of the offical Mbed example
//The queue has pointers to your type
//You have memory somewhere to store values (eg memorypool or could also be locations in an array)
//And the queue will send pointers to these values
//However, when you use try_get, you must pass a double pointer

#include <mbed.h>

Thread get_thread(osPriorityNormal, OS_STACK_SIZE, nullptr, "get thread");
Thread put_thread(osPriorityNormal, OS_STACK_SIZE, nullptr, "put thread");

Queue<int, 8> q1;
MemoryPool<int, 8> mp;

void put_loop()
{
    int count = 0;
    while(true)
    {

        int* mp_slot_ptr = mp.try_alloc_for(Kernel::wait_for_u32_forever);
        *mp_slot_ptr = count;
        bool put_status = q1.try_put(mp_slot_ptr);

        if(!put_status)
        {
            printf("Queue full at size %d\n", q1.count());
        }

        count++;

        ThisThread::sleep_for(1s);
    }
}

void get_loop()
{
    //"The message is stored in the location pointed to be the parameter data_out"
    int* data_in;
    //Yes, we provide a pointer to a pointer so they know where to store the pointer
    int** queue_received = &data_in;

    while(true)
    {
        bool get_status = q1.try_get(queue_received);
        
        if(get_status)
        {
            printf("\nGet loop got: %d\n", *data_in);
            printf("Mempool free: %d\n", mp.free(data_in));
            //OR:
            //printf("Get loop got: %d\n", *queue_received);
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