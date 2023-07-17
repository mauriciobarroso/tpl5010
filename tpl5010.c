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

#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"

/* Private macros ------------------------------------------------------------*/
#define NOP() asm volatile ("nop")

/* External variables --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static const char *TAG = "tpl5010";

/* Private function prototypes -----------------------------------------------*/
/**
 * @brief Function to write a byte
 *
 * @param me        : Pointer to a structure instance of at24cs0x_t
 * @param data_addr : Address to write data
 * @param data      : Data to write
 *
 * @return ESP_OK on success
 */
static void delay_us(uint32_t period_us)

/**
 * @brief Function to write a byte
 *
 * @param me        : Pointer to a structure instance of at24cs0x_t
 * @param data_addr : Address to write data
 * @param data      : Data to write
 *
 * @return ESP_OK on success
 */
static void IRAM_ATTR gpio_isr_handler(void *arg);

/* Exported functions definitions --------------------------------------------*/
/**
 * @brief Function to write a byte
 */
esp_err_t tpl5010_init(tpl5010_t *const me, int wake_gpio, int done_gpio) {
	esp_err_t ret = ESP_OK;

	/* Configure wake GPIO */
	me->wake_gpio = wake_gpio;

	gpio_config_t gpio_conf;
	gpio_conf.intr_type = GPIO_INTR_POSEDGE;
	gpio_conf.mode = GPIO_MODE_INPUT;
	gpio_conf.pin_bit_mask = 1ULL << me->wake_gpio;
	gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	gpio_conf.pull_up_en = GPIO_PULLUP_DISABLE;

	ret = gpio_config(&gpio_conf);

	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Failed to configure wake GPIO");
		return ret;
	}

	/* Configure done GPIO */
	me->done_gpio = done_gpio;

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

	/* Install GPIO ISR service */
	ret = gpio_install_isr_service(0);

	if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
		ESP_LOGE(TAG, "Failed to install wake GPIO ISR service");
		return ret;
	}

	/* Add GPIO ISR handler */
	ret = gpio_isr_handler_add(me->wake_gpio, gpio_isr_handler, me);

	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Failed to add GPIO ISR handler");
		return ret;
	}

	ret = tpl5010_done(me);

	if (ret != ESP_OK) {
		return ret;
	}

	/* Return ESP_OK */
	return ret;
}

/**
 * @brief Function to write a byte
 */
esp_err_t tpl5010_done(tpl5010_t *const me) {
	esp_err_t ret = ESP_OK;

	/* Send an initial pulse to TPL5010 */
	ret = gpio_set_level(me->done_gpio, true);

	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Failed to set done GPIO level");
		return ret;
	}

	delay_us(100);

	ret = gpio_set_level(me->done_gpio, false);

	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Failed to set done GPIO level");
		return ret;
	}

	/* Return ESP_OK */
	return ret;
}

/* Private function definitions ----------------------------------------------*/
/**
 * @brief Function to write a byte
 */
static void delay_us(uint32_t period_us) {
	uint64_t m = (uint64_t)esp_timer_get_time();

  if (period_us) {
  	uint64_t e = (m + period_us);

  	if (m > e) { /* overflow */
  		while ((uint64_t)esp_timer_get_time() > e) {
  			NOP();
  		}
  	}

  	while ((uint64_t)esp_timer_get_time() < e) {
  		NOP();
  	}
  }
}

/**
 * @brief Function to write a byte
 */
static void IRAM_ATTR gpio_isr_handler(void *arg) {
	tpl5010_t *tpl5010 = (tpl5010_t *)arg;

	ESP_DRAM_LOGI(TAG, "Wake");
	tpl5010_done(tpl5010);
}

/***************************** END OF FILE ************************************/
