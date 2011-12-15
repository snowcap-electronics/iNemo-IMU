/***
 * IMU code for iNemo v1
 *
 * Copyright 2011 Kalle Vahlman <zuh@snowcap.fi>
 *                Tuomas Kulve <tuomas.kulve@snowcap.fi>
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

/*

 Peripherals for iNemo v1:

 UART 2

 I²C 1:

    HMC5843  - Magnetometer (data ready indication on PB8)

 I²C 2:

    LPS001DL - Pressure sensor

    STLM75   - Temperature sensor

 SPI 1:

    LIS331DLH - Accelerometer

 SPI 2:

    Zigbee (not fitted)

 Analog input
 
    LPR530AL - X/Y Gyroscope
    LPY530AL - X/Z Gyroscope

*/

/* ADC conversion callback */
static void samplesReady(ADCDriver *adcp, adcsample_t *buffer, size_t n);

/* We only read one channel (pin?) */
#define ADC_CHANNELS 4
/* How many samples to read before processing */
#define ADC_BUFFER_DEPTH 4
/* How long to sample the voltage
   This is defined in cycles, and thus is dependent on the ADC clock rate.
   The range is 1.5...239.5 cycles, in 8 non-linear steps
   The higher the number, the longer is the sampling time.
 */
#define ADC_SAMPLE_TIME ADC_SAMPLE_239P5

static adcsample_t samples[ADC_CHANNELS * ADC_BUFFER_DEPTH];

static const ADCConversionGroup convCfg = {
    /* Circular buffer mode */
    TRUE,
    /* Sampled channels */
    ADC_CHANNELS,
    /* Conversion completed -callback */
    samplesReady,
    /* Conversion error -callback */
    NULL,
    /* CR1 register setup */
    0,
    /* CR2 register setup, trigger from software */
    0,
    /* Sample time setup for channels 10..17 */
    ADC_SMPR1_SMP_AN10(ADC_SAMPLE_TIME)
    | ADC_SMPR1_SMP_AN11(ADC_SAMPLE_TIME)
    | ADC_SMPR1_SMP_AN14(ADC_SAMPLE_TIME)
    | ADC_SMPR1_SMP_AN15(ADC_SAMPLE_TIME),
    /* Sample rate setup for channels 0..9 */
    0,
    /* Conversion sequence setup register 1
       The sequence length is encoded here */
    ADC_SQR1_NUM_CH(ADC_CHANNELS),
    /* Conversion sequence setup register 2 */
    0,
    /* Conversion sequence setup register 3
       Read from analog inputs 10 (PC0), 11 (PC1), 14 (PC4) and 15 (PC5) */
    ADC_SQR3_SQ1_N(ADC_CHANNEL_IN10)
    | ADC_SQR3_SQ2_N(ADC_CHANNEL_IN11)
    | ADC_SQR3_SQ3_N(ADC_CHANNEL_IN14)
    | ADC_SQR3_SQ4_N(ADC_CHANNEL_IN15)
};

static void samplesReady(ADCDriver *adcp, adcsample_t *buffer, size_t UNUSED(n))
{
    /* The callback gets called when buffer is half-full and full.
       The state is ADC_ACTIVE when half-full and ADC_COMPLETE when full.
     */
    if (adcp->state == ADC_COMPLETE) {
        int i;

        for (i = 0; i < ADC_CHANNELS; i++) {
            int j;
            adcsample_t avg = 0;
            for (j = 0; j < ADC_BUFFER_DEPTH; j++) {
                // do stuff
                avg += buffer[i*j];
            }
            avg = avg / ADC_BUFFER_DEPTH;
        }

    }
}

int main(void) {

    /* Initialize ChibiOS HAL and core */
    halInit();
    chSysInit();

    /* LPR530AL pin setup */
    palSetPadMode(GPIOC, 0, PAL_MODE_INPUT_ANALOG);
    palSetPadMode(GPIOC, 1, PAL_MODE_INPUT_ANALOG);

    /* LPY530AL pin setup */
    palSetPadMode(GPIOC, 4, PAL_MODE_INPUT_ANALOG);
    palSetPadMode(GPIOC, 5, PAL_MODE_INPUT_ANALOG);

    /* Start ADC driver 1 */
    adcStart(&ADCD1, NULL);

    /* LED pin setup */
    palSetPadMode(GPIOB, GPIOB_LED, PAL_MODE_OUTPUT_PUSHPULL);

{
    int i;
    for (i = 0; i < ADC_CHANNELS * ADC_BUFFER_DEPTH; i++)
        samples[i] = 0;
}

    /* This is our main loop */
    while (TRUE) {

        /* Sleep between conversions */
        chThdSleepMilliseconds(100);

        /* Toggle LED on/off */
        palTogglePad(GPIOB, GPIOB_LED);

        /* Start ADC conversion */
        adcStartConversionI(&ADCD1, &convCfg, samples, ADC_CHANNELS * ADC_BUFFER_DEPTH);

    }
    return 0;
}
