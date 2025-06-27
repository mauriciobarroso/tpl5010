/**
 ******************************************************************************
 * @file           : tpl5010.c
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

/* Includes ------------------------------------------------------------------*/
#include "tpl5010.h"

#ifdef ESP32_TARGET
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#else
/* todo: write for STM32 devices */
#endif /* ESP32_TARGET */

/* Private macros ------------------------------------------------------------*/
#define NOP() asm volatile("nop")

/* External variables --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static const char *TAG = "tpl5010";

/* Private function prototypes -----------------------------------------------*/
static int set_done_level(int done_gpio, bool level);

/* Exported functions definitions --------------------------------------------*/
/**
 * @brief Initialize the TPL5010 driver instance.
 */
int tpl5010_init(tpl5010_t *const me, int wake_gpio, int done_gpio) {
  int ret = 0;

  /* Configure wake GPIO */
  me->wake_gpio = wake_gpio;

#ifdef ESP32_TARGET
  gpio_config_t gpio_conf;
  gpio_conf.intr_type = GPIO_INTR_POSEDGE;
  gpio_conf.mode = GPIO_MODE_INPUT;
  gpio_conf.pin_bit_mask = 1ULL << me->wake_gpio;
  gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  gpio_conf.pull_up_en = GPIO_PULLUP_ENABLE;
  ret = gpio_config(&gpio_conf);

  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to configure wake GPIO");
    return ret;
  }
#else
  /* todo: write for STM32 devices */
#endif /* ESP32_TARGET */

  /* Configure done GPIO */
  me->done_gpio = done_gpio;

#ifdef ESP32_TARGET
  gpio_conf.intr_type = GPIO_INTR_DISABLE;
  gpio_conf.mode = GPIO_MODE_OUTPUT;
  gpio_conf.pin_bit_mask = 1ULL << me->done_gpio;
  gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  gpio_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  ret = gpio_config(&gpio_conf);

  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to configure done GPIO");
    return ret;
  }
#else
  /* todo: write for STM32 devices */
#endif /* ESP32_TARGET */

  /* Install GPIO ISR service */
  ret = gpio_install_isr_service(0);

  if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
    ESP_LOGE(TAG, "Failed to install wake GPIO ISR service");
    return ret;
  }

  /* Send done signal */
  tpl5010_done(me);

  /* Return success */
  return ret;
}

/**
 * @brief Register a user callback to be invoked on each WAKE pulse.
 */
int tpl5010_register_callback(tpl5010_t *const me, tpl5010_cb_t cb_fn,
                              void *cb_arg) {
  int ret = 0;

  /* Check if the callback functions is valid */
  if (cb_fn == NULL) {
    return -1;
  }

  /* Assign callback function and argument to structure members */
  me->cb_fn = cb_fn;
  me->cb_arg = cb_arg;

/* Add GPIO ISR handler */
#ifdef ESP32_TARGET
  ret = gpio_isr_handler_add(me->wake_gpio, me->cb_fn, me->cb_arg);

  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to add GPIO ISR handler");
    return ret;
  }
#else
  /* todo: write for STM32 devices */
#endif /* ESP32_TARGET */

  /* Return success */
  return ret;
}

/**
 * @brief Function to write a byte
 */
esp_err_t tpl5010_done(tpl5010_t *const me) {
  esp_err_t ret = ESP_OK;

  /* Put in high level the done GPIO */
  ret = set_done_level(me->done_gpio, 1);

  if (ret != 0) {
    return ret;
  }

  /* Wait for 100 ms */
  me->delay_ms(100);

  /* Put in low level the done GPIO */
  ret = set_done_level(me->done_gpio, 0);

  if (ret != 0) {
    return ret;
  }

  /* Return ESP_OK */
  return ret;
}

/* Private function definitions ----------------------------------------------*/
static int set_done_level(int done_gpio, bool level) {
#ifdef ESP32_TARGET
  return gpio_set_level(done_gpio, level);
#else
  /* todo: write for STM32 devices */
#endif /* ESP32_TARGET */
}

/***************************** END OF FILE ************************************/
