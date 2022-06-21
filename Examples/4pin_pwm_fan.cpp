#include "mbed.h"

/*
Circuit setup: Same as here https://github.com/SuperThunder/Mbed-OS6-4PinFanPWM
- 5k pullup to 12v on fan tachometer pin
- 1/4 voltage division from tachometer to MCU pin, resistors used in the 10s of kOhm range

*/

#define PWM_PIN PB_3
#define TACH_PIN PA_0
#define PWM_FREQUENCY 25000.0f

InterruptIn button_int(BUTTON1);
InterruptIn tach_int(TACH_PIN);
PwmOut fan_pwm(PWM_PIN);

volatile int tach_pulse_count = 0;

EventFlags app_events;
#define APP_FLAG_BUTTON (1UL << 0)

Thread thr_pwm_duty_mgmt(osPriorityNormal, 1024, nullptr, nullptr);


void button_isr()
{ 
    app_events.set(APP_FLAG_BUTTON);
}

void tach_pulse_isr()
{
    tach_pulse_count++;
}

void pwm_duty_mgmt_loop()
{
    //cycle in 20% increments so 0, 20, 40, 60, 80, 100%
    //so this variable will be mod 6 with 0 for 0% and 5 for 100%
    int pwm_setting = 0; 

    while(true)
    {        
        app_events.wait_all(APP_FLAG_BUTTON);

        pwm_setting = (pwm_setting+1) % 6; //increment pwm_setting by 1 when button pushed
        float pwm_value = (float)pwm_setting / 5.0f;
        fan_pwm.write(pwm_value);

        // printf("PWM setting: %d\n", pwm_setting);
        // printf("PWM value: %f\n", pwm_value);
        printf("Set PWM duty to %3.1f\n", (pwm_value*100.0));
    }
}

int main()
{
    printf("Starting fan RPM demo\n");

    fan_pwm.period(1/PWM_FREQUENCY);
    fan_pwm.write(0.0f);

    button_int.rise(button_isr);
    tach_int.mode(PullNone);
    tach_int.rise(tach_pulse_isr);

    app_events.clear();
    thr_pwm_duty_mgmt.start(pwm_duty_mgmt_loop);

    //timer to keep exact track of how long it's been since the pulse counter was reset (assuming 1 second will be a bit inaccurate)
    Timer rpm_timer;
    rpm_timer.start();

    //main thread used for output
    while(true)
    {
        //Calculate RPM
        chrono::microseconds elapsed = rpm_timer.elapsed_time();
        uint64_t fan_current_rpm = (uint64_t)tach_pulse_count * 60 / 2 * 1000000 / elapsed.count();
        tach_pulse_count = 0; //the ISR will do its read and increment without interruption, so this is fine
        rpm_timer.reset();
        printf("Fan RPM: %llu\n", fan_current_rpm);

        ThisThread::sleep_for(1s);
    }
}