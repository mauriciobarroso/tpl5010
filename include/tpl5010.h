/**
 ******************************************************************************
 * @file           : tpl5010.h
 * @author         : Mauricio Barroso Benavides
 * @date           : Jul 17, 2023
 * @brief          : todo: write brief
 ******************************************************************************
 * @attention
 *
 * MIT License
 *
 * Copyright (c) 2023 Mauricio Barroso Benavides
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TPL5010_H_
#define TPL5010_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 ||                    \
    CONFIG_IDF_TARGET_ESP32S3 || CONFIG_IDF_TARGET_ESP32C2 ||                  \
    CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6 ||                  \
    CONFIG_IDF_TARGET_ESP32P4
#define ESP32_TARGET
#endif

#ifdef ESP32_TARGET
#include "driver/gpio.h"
#else
#include "main.h"
#endif /* ESP32_TARGET */

/* Exported Macros -----------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/
/**
 * @brief Millisecond delay function prototype.
 * @param ms : Delay time in milliseconds.
 */
typedef void (*tpl5010_delay_t)(uint32_t ms);

/**
 * @brief Callback function prototype invoked on WAKE pulse.
 * @param arg: User data pointer provided at registration.
 */
typedef void (*tpl5010_cb_t)(void *arg);

/**
 * @brief Driver configuration structure for the TPL5010 system wake-up timer.
 */
typedef struct {
  int wake_gpio; /*!< GPIO connected to TPL5010 WAKE output pin; pulses high
                    every tIP period to wake the MCU */
  int done_gpio; /*!< GPIO connected to TPL5010 DONE input pin; MCU must assert
                    a low-to-high transition to reset the watchdog counter. */
  tpl5010_delay_t delay_ms; /*!< User-supplied function to delay execution by
                               `ms` milliseconds. */
  tpl5010_cb_t cb_fn;       /*!< User callback invoked upon each WAKE pulse. */
  void *cb_arg; /*!< User-defined context pointer passed to `cb_fn`. */
} tpl5010_t;

/* Exported variables --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
/**
 * @brief Initialize the TPL5010 driver instance.
 *
 * @param me[in]        : Pointer to a tpl5010_t structure (must be initialized
 * before use)
 * @param wake_gpio[in] : MCU GPIO pin number connected to the TPL5010 WAKE
 * output; configure this pin as an input to detect the 20 ms WAKE pulse
 * @param done_gpio[in] : MCU GPIO pin number connected to the TPL5010 DONE
 * input; configure this pin as an output to issue the low-to-high DONE
 * transition that resets the watchdog counter
 *
 * @return 0 on success, -1 on failure
 */
int tpl5010_init(tpl5010_t *const me, int wake_gpio, int done_gpio);

/**
 * @brief Register a user callback to be invoked on each WAKE pulse.
 *
 * @param me[in]     : Pointer to a tpl5010_t structure previously initialized
 * with tpl5010_init()
 * @param cb_fn[in]  : Function pointer to call when a WAKE pulse is detected;
 * signature must be `void (*callback)(void *arg)`
 * @param cb_arg[in] : User-defined context pointer passed to the callback when
 * invoked
 *
 * @return 0 on success, -1 on failure
 */
int tpl5010_register_callback(tpl5010_t *const me, tpl5010_cb_t cb_fn,
                              void *cb_arg);

/**
 * @brief Issue the DONE signal to the TPL5010, indicating successful processing
 * of the last WAKE pulse.
 *
 * @param me[in] : Pointer to a tpl5010_t structure previously initialized with
 * tpl5010_init()
 *
 * @return 0 on success, -1 on failure
 */
int tpl5010_done(tpl5010_t *const me);

#ifdef __cplusplus
}
#endif

#endif /* TPL5010_H_ */

/***************************** END OF FILE ************************************/
