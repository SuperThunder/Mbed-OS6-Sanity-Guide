//callback of a C++ class member
#include <mbed.h>

InterruptIn button(BUTTON1);

class Counter
{
    public:
        void inc(){_n++;}
        int get(){return _n;};
    private:
        int _n = 0;
};

Counter ctr;


int main()
{
    //You pass callback with a pointer to the object instance, and to the function of the class to call of that instance
    button.rise(callback(&ctr, &Counter::inc));

    while(true)
    {
        printf("Counter value: %d\n", ctr.get());
        ThisThread::sleep_for(1s);
    }
}