//A simple way of having a button call a function is to use it as the ISR callback for the button's rise or fall
//https://os.mbed.com/docs/mbed-os/v6.15/apis/interruptin.html

InterruptIn button(BUTTON1);

volatile int counter = 0;

//This function runs in ISR context, so no blocking or memory or printf
void button_isr_function()
{
    counter++;
}

int main()
{
    button.rise(callback(button_isr_function)); //for simple functions like this you can implicitly cast to Callback too

    while(true)
    {
        ThisThread::sleep_for(1s);
    }
}