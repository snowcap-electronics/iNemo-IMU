/***
 * IMU code for iNemo v1
 *
 * Copyright 2011 Kalle Vahlman, <zuh@iki.fi>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */
/* ChibiOS includes */
#include "ch.h"
#include "hal.h"

/* Project includes */
#include "utils.h"

#include "hmc5843.h"
#include "stlm75.h"

/*

 Peripherals for iNemo v1:

 UART 2

 I²C 1:

    HMC5843  - Magnetometer

 I²C 2:

    LPS001DL - Pressure sensor

    STLM75   - Temperature sensor

 SPI 1:

    LIS331DLH - Accelerometer

 SPI 2:

    Zigbee (not fitted)

 Analog input
 
    LPR530AL - X/Y Gyroscope
    LPR530AL - X/Z Gyroscope

*/

static
void out(char *msg)
{
    size_t c = 0;
    while (msg[c] != '\0')
        chIOPut(&SD2, msg[c++]);
}

static
void outInt(uint16_t n, size_t magnitude)
{
    char msg[64];
    size_t c = 0;
    size_t mag = magnitude;
    while (c < 63) {
        msg[c++] = 48 + (n / mag) % 10;
        if (mag <= 1)
            break;
        mag = mag / 10;
    }
    msg[c] = '\0';
    out(msg);
}

static
void outln(char *msg)
{
    out(msg);
    chIOPut(&SD2, '\n');
    chIOPut(&SD2, '\r');
}

/*
 * I²C driver configuration
 */
static I2CConfig i2c_cfg = {
    OPMODE_I2C, /* Bus mode */
    100000, /* Clock speed */
    STD_DUTY_CYCLE,
    0,
    0,
    0,
};

static WORKING_AREA(reader, 64);
static msg_t readThread(void *UNUSED(arg)) {

    while (TRUE) {

        /* TODO: how frequently does it make sense to read? */
        chThdSleepMilliseconds(750);
    }
    return 0;
}

        static int16_t temperature = 0;

int main(void) {

    /* Initialize ChibiOS HAL and core */
    halInit();
    chSysInit();

    /* Start serial driver */
    sdStart(&SD2, NULL);

    i2cStart(&I2CD1, &i2c_cfg);

    hmc5843Init(&I2CD1);

/* FIXME: I²C is busted, why? */
#if 0
    /* I²C 1 pins */
    palSetPadMode(GPIOB, 6, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
    palSetPadMode(GPIOB, 7, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
    /* HMC5843 data ready pin */
    palSetPadMode(GPIOB, 8, PAL_MODE_INPUT);


    rccDisableUSART3(TRUE);

/*
    palSetPadMode(GPIOB, 10, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
    palSetPadMode(GPIOB, 11, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
*/
    
#endif

    i2cStart(&I2CD2, &i2c_cfg);
    /* STLM75 has it's address pins grounded, so the address is 0x90 */
    stlm75Start(&I2CD2, 0x90);

    palSetPadMode(GPIOB, GPIOB_LED, PAL_MODE_OUTPUT_PUSHPULL);

    /* This is our main loop */
    while (TRUE) {

        /* With 100ms intervals */
        chThdSleepMilliseconds(1000);

        /* Toggle LED on/off */
        palTogglePad(GPIOB, GPIOB_LED);
 
 
        if (palReadPad(GPIOC, GPIOC_BUTTON) == PAL_LOW)
            outln("Button pressed");
        if (palReadPad(GPIOB, 8) == PAL_LOW)
            outln("Mag data ready");
{
    int16_t xh, yh, zh, xl, yl, zl, s;
    xh = yh = zh = xl = yl = zl = s = 0;
    s = hmc5843Read(&xh, &yh, &zh, &xl, &yl, &zl);
    outInt((xh << 8) | xl, 10000);
    out(" ");
    outInt(xh, 100);
    out(" ");
    outInt(xl, 100);
    out(" ");
    outInt(yh, 100);
    out(" ");
    outInt(yl, 100);
    out(" ");
    outInt(zh, 100);
    out(" ");
    outInt(zl, 100);
    outln("");
} 
#if 0
    out("Temperature: ");
    outInt(stlm75ReadSync(), 100);
    outln("");
#endif
    }
    return 0;
}
