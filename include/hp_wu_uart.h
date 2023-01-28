/*
 * @Author “Zero” “zero_bbx@qq.com”
 * @Date 2023-01-04 16:28:50
 * @LastEditors “Zero” “zero_bbx@qq.com”
 * @LastEditTime 2023-01-04 17:20:44
 * @FilePath \mdf_meshc:\Users\Wu\Documents\PlatformIO\Projects\esp32idf_gpio\include\hp_wu_uart.h
 * @Description 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "driver/uart.h"

#ifndef _HP_WU_UART_H_
#define _HP_WU_UART_H_

typedef void (*uart_func_t)(uart_port_t uart_port, uint8_t *data, uint8_t len);

void app_uart_init(uart_func_t callback_func);
void uart_driver_init(uart_port_t uart_port);

#endif //
