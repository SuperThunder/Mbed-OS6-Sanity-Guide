/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#define SLEEP_RATE_MS     1000

//Remember to set appropriate baudrate in mbed_app.json
//In the example mbed_app.json, buffered serial is enabled to avoid blocking (slow!!!) printf calls
int main()
{
    while (true) {
        printf("Hello world!\n");
        ThisThread::sleep_for(SLEEP_RATE_MS);
    }
}