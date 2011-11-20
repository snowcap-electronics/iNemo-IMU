
#ifndef HMC5843_H
#define HMC5843_H

/* ChibiOS includes */
#include "ch.h"
#include "hal.h"

#define HMC5843_I2C_SPEED           100000
#define HMC5843_I2C_ADDRESS         0x1E

#define HMC5843_MODE                0x02
#define HMC5843_MODE_CONTINUOUS     0x00
#define HMC5843_MODE_SINGLE         0x01
#define HMC5843_MODE_IDLE           0x02
#define HMC5843_MODE_SLEEP          0x03

#define HMC5843_X_MSB               0x04
#define HMC5843_X_LSB               0x05
#define HMC5843_Y_MSB               0x06
#define HMC5843_Y_LSB               0x07
#define HMC5843_Z_MSB               0x08
#define HMC5843_Z_LSB               0x09


void hmc5843Init(I2CDriver *i2cp);

int hmc5843Read(int16_t *xouth, int16_t *youth, int16_t *zouth,
                int16_t *xoutl, int16_t *youtl, int16_t *zoutl);

#endif /* HMC5843_H */
