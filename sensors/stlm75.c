/***
 * STLM75 temperature sensor
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

#include "stlm75.h"

#if defined __GNUC__
# define UNUSED(x) x __attribute__ ((unused))
#else
# define UNUSED(x) x
#endif

typedef struct {
    uint8_t addr;
    I2CDriver *i2cp;
    i2cblock_t tx[2];
    i2cblock_t rx[2];
    uint16_t temperature;
} STLM75;

static STLM75 d = {
    0x90, /* Default */
    NULL,
    { 0, },
    { 0, },
    0,
};

static BinarySemaphore readSem;

void stlm75Start(I2CDriver *i2cp, uint8_t address)
{
    d.i2cp = i2cp;
    if (address > 0)
        d.addr = address;
    chBSemInit(&readSem, TRUE);

    /* Default config of the STLM75 is:
        * mode = comparator
        * polarity = active-low
        * fault tolerance = 1 fault
        * watchdog limits:
            upper = 80C
            lower = 75C
        * register pointer = 0 (temperature)
      */

}

static void stlm75DataReady (I2CDriver *UNUSED(i2cp),
                             const I2CSlaveConfig *UNUSED(i2cscfg))
{
    chSysLockFromIsr();

    /* Signal that we have new data */
    chBSemSignalI(&readSem);
    
    chSysUnlockFromIsr();
}

static I2CSlaveConfig readCfg = { stlm75DataReady, NULL };

uint16_t stlm75ReadSync()
{
    i2cAcquireBus(d.i2cp);

    /* Read temperature from 8-bit registers (2 bytes) */
    d.rx[0] = d.rx[1] = 0;
    i2cMasterReceive(d.i2cp, &readCfg, d.addr, d.rx, 2);

    i2cReleaseBus(d.i2cp);

    /* Wait for the data to arrive */
    chBSemWait(&readSem);

    return (d.rx[0] << 8) | d.rx[1];
}

