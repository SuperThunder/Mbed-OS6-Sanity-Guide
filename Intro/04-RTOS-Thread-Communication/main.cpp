//TODO in here we will use the whole pointer to a pointer thing (maybe with a mempool)
//TODO mempool

#include <mbed.h>

Thread get_thread(osPriorityNormal, OS_STACK_SIZE, nullptr, "get thread");
Thread put_thread(osPriorityNormal, OS_STACK_SIZE, nullptr, "put thread");

Queue<int, 32> q1;

void put_loop()
{
    int count = 0;
    while(true)
    {
        bool put_status = q1.try_put(&tmp);

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
            printf("Get loop got: %d\n", **queue_received);
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