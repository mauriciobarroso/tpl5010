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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "esp_err.h"

/* Exported Macros -----------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/
typedef struct {
	int wake_gpio;
	int done_gpio;
} tpl5010_t;

/* Exported variables --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
/**
 * @brief Function to write a byte
 *
 * @param me        : Pointer to a structure instance of at24cs0x_t
 * @param data_addr : Address to write data
 * @param data      : Data to write
 *
 * @return ESP_OK on success
 */
esp_err_t tpl5010_init(tpl5010_t *const me, int wake_gpio, int done_gpio);

/**
 * @brief Function to write a byte
 *
 * @param me        : Pointer to a structure instance of at24cs0x_t
 * @param data_addr : Address to write data
 * @param data      : Data to write
 *
 * @return ESP_OK on success
 */
esp_err_t tpl5010_done(tpl5010_t *const me);

#ifdef __cplusplus
}
#endif

#endif /* TPL5010_H_ */

/***************************** END OF FILE ************************************/
