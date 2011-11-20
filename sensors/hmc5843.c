
/* ChibiOS includes */
#include "ch.h"
#include "hal.h"

#include "utils.h"
#include "hmc5843.h"


typedef struct {
    uint8_t addr;
    I2CSlaveConfig cfg;
    I2CDriver *i2cp;
    i2cblock_t tx[2];
    i2cblock_t rx[7];
    uint16_t x;
    uint16_t y;
    uint16_t z;
} HMC5843;


static void hmc5843Ok (I2CDriver *i2cp, const I2CSlaveConfig *i2cscfg);
static void hmc5843Error (I2CDriver *i2cp, const I2CSlaveConfig *i2cscfg);

static HMC5843 d = {
    HMC5843_I2C_ADDRESS,
    { hmc5843Ok, hmc5843Error },
    NULL,
    { 0, },
    { 0, },
    0,
    0,
    0,
};

static I2CSlaveConfig dummycfg = { NULL, NULL };

void hmc5843Init(I2CDriver *i2cp)
{
    /* Make sure the power-up has finished (spec says 8.3ms) */
    chThdSleepMilliseconds(9);

    d.i2cp = i2cp;

    i2cAcquireBus(d.i2cp);

    /* Set up continuous measurement mode */
    d.tx[0] = HMC5843_MODE;
    d.tx[1] = HMC5843_MODE_SINGLE;
    i2cMasterTransmit(d.i2cp, &dummycfg, d.addr, d.tx, 2, NULL, 0);

    i2cReleaseBus(d.i2cp);

}

static void hmc5843Ok (I2CDriver *UNUSED(i2cp), const I2CSlaveConfig *UNUSED(i2cscfg))
{
    chSysLockFromIsr();
    
    chSysUnlockFromIsr();
}

static void hmc5843Error (I2CDriver *UNUSED(i2cp), const I2CSlaveConfig *UNUSED(i2cscfg))
{
    chSysLockFromIsr();

    chSysUnlockFromIsr();
}

i2cblock_t dummy = 0x00;

int hmc5843Read (int16_t *xouth, int16_t *youth, int16_t *zouth,
                 int16_t *xoutl, int16_t *youtl, int16_t *zoutl)
{
    int s = 0;
    i2cAcquireBus(d.i2cp);

    /* Read X/Y/Z high/low bits and status from 8-bit registers */
    d.rx[0] = d.rx[1] = d.rx[2] = d.rx[3] = d.rx[4] = d.rx[5] = d.rx[6] = 0;
    i2cMasterReceive(d.i2cp, &d.cfg, d.addr, d.rx, 2);

#if 1
    d.tx[0] = HMC5843_MODE;
    d.tx[1] = HMC5843_MODE_SINGLE;
    i2cMasterTransmit(d.i2cp, &dummycfg, d.addr, d.tx, 2, NULL, 0);
#endif

    i2cReleaseBus(d.i2cp);

    chThdSleepMilliseconds(60);

    *xouth = d.rx[0];
    *xoutl = d.rx[1];
    *youth = d.rx[2];
    *youtl = d.rx[3];
    *zouth = d.rx[4];
    *zoutl = d.rx[5];

    return s * 5;
}

