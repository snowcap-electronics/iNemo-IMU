/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for STMicroelectronics iNemo v1 board.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_INEMO_V1
#define BOARD_NAME              "ST iNemo v1"

/*
 * Board frequencies.
 */
#define STM32_LSECLK            32768
#define STM32_HSECLK            8000000

/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 */
#define STM32F10X_HD

/*
 * IO pins assignments.
 */
#define GPIOC_BUTTON            13

#define GPIOB_LED               9

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 *
 * The digits have the following meaning:
 *   0 - Analog input.
 *   1 - Push Pull output 10MHz.
 *   2 - Push Pull output 2MHz.
 *   3 - Push Pull output 50MHz.
 *   4 - Digital input.
 *   5 - Open Drain output 10MHz.
 *   6 - Open Drain output 2MHz.
 *   7 - Open Drain output 50MHz.
 *   8 - Digital input with PullUp or PullDown resistor depending on ODR.
 *   9 - Alternate Push Pull output 10MHz.
 *   A - Alternate Push Pull output 2MHz.
 *   B - Alternate Push Pull output 50MHz.
 *   C - Reserved.
 *   D - Alternate Open Drain output 10MHz.
 *   E - Alternate Open Drain output 2MHz.
 *   F - Alternate Open Drain output 50MHz.
 * Please refer to the STM32 Reference Manual for details.
 */

/*
 * Port A setup.
 * Everything input with pull-up except:
 * PA2  - Alternate output  (UART2 TX).
 * PA3  - Normal input      (UART2 RX).
 * PA4  - Push pull output  (SPI1 NSS), initially high state.
 * PA5  - Alternate output  (SPI1 SCK).
 * PA6  - Normal input      (SPI1 MISO).
 * PA7  - Alternate output  (SPI1 MOSI).
 */
#define VAL_GPIOACRL            0xB4B34B88      /*  PA7...PA0 */
#define VAL_GPIOACRH            0x88888888      /* PA15...PA8 */
#define VAL_GPIOAODR            0xFFFFFFFF

/*
 * Port B setup.
 * Everything input with pull-up except:
 * PB6  - Alternate output  (I2C1 SCL).
 * PB7  - Alternate output  (I2C1 SDA).
 * PB10 - Alternate output  (I2C2 SCL).
 * PB11 - Alternate output  (I2C2 SDA).
 * PB12 - Push pull output  (SPI2 NSS), initially high state.
 * PB13 - Alternate output  (SPI2 SCK).
 * PB14 - Normal input      (SPI2 MISO).
 * PB15 - Alternate output  (SPI2 MOSI).
 */
#define VAL_GPIOBCRL            0xFF888888      /*  PB7...PB0 */
#define VAL_GPIOBCRH            0xB4B3FF88      /* PB15...PB8 */
#define VAL_GPIOBODR            0xFFFFFFFF

/*
 * Port C setup.
 * Everything input with pull-up except:
 * PC13 - Normal input      (BUTTON)
 */
#define VAL_GPIOCCRL            0x88888888      /*  PC7...PC0 */
#define VAL_GPIOCCRH            0x88888888      /* PC15...PC8 */
#define VAL_GPIOCODR            0xFFFFFFFF

/*
 * Port D setup.
 * Everything input with pull-up
 * PD0  - Normal input (XTAL).
 * PD1  - Normal input (XTAL).
 */
#define VAL_GPIODCRL            0x88888844      /*  PD7...PD0 */
#define VAL_GPIODCRH            0x88888888      /* PD15...PD8 */
#define VAL_GPIODODR            0xFFFFFFFF

/*
 * Port E setup.
 * Everything input with pull-up
 */
#define VAL_GPIOECRL            0x88888888      /*  PE7...PE0 */
#define VAL_GPIOECRH            0x88888888      /* PE15...PE8 */
#define VAL_GPIOEODR            0xFFFFFFFF

/*
 * Port F setup.
 * Everything input with pull-up
 */
#define VAL_GPIOFCRL            0x88888888      /*  PF7...PF0 */
#define VAL_GPIOFCRH            0x88888888      /* PF15...PF8 */
#define VAL_GPIOFODR            0xFFFFFFFF

/*
 * Port G setup.
 * Everything input with pull-up
 */
#define VAL_GPIOGCRL            0x88888888      /*  PG7...PG0 */
#define VAL_GPIOGCRH            0x88888888      /* PG15...PG8 */
#define VAL_GPIOGODR            0xFFFFFFFF

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
