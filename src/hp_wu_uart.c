/*
 * @Author “Zero” “zero_bbx@qq.com”
 * @Date 2023-01-04 16:28:21
 * @LastEditors “Zero” “zero_bbx@qq.com”
 * @LastEditTime 2023-01-05 13:03:17
 * @FilePath \mdf_meshc:\Users\Wu\Documents\PlatformIO\Projects\esp32idf_gpio\src\hp_wu_uart.c
 * @Description 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "hp_wu_uart.h"

#define UART_PIN_TXD       (16)
#define UART_PIN_RXD       (17)
#define UART_PIN_RTS       (UART_PIN_NO_CHANGE)
#define UART_PIN_CTS       (UART_PIN_NO_CHANGE)
#define UART_BUFF_SIZE     (128)

static const uart_mode_t uart_mode = UART_MODE_UART;
static uart_func_t uart_callback_func = NULL; 

/**
 * @brief 串口通信初始化
 * @param  uart_port UART port number, can be UART_NUM_0 ~ (UART_NUM_MAX -1).
 * @return {*}
 */
void uart_driver_init(uart_port_t uart_port)
{
    uart_config_t uart_config = {
        .baud_rate = 256000,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk= UART_SCLK_APB,
    };
 
    // Install UART driver (we don't need an event queue here)
    // In this example we don't even use a buffer for sending data.
    ESP_ERROR_CHECK(uart_driver_install(uart_port, UART_BUFF_SIZE * 2, 0, 0, NULL, 0));

    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_port, &uart_config));
if (uart_port != UART_NUM_0) {
    // Set UART pins as per KConfig settings
    ESP_ERROR_CHECK(uart_set_pin(uart_port, UART_PIN_TXD, UART_PIN_RXD, UART_PIN_RTS, UART_PIN_CTS));

    // Set RS485 half duplex mode
    ESP_ERROR_CHECK(uart_set_mode(uart_port, uart_mode));
}
    // Set read timeout of UART TOUT feature
    // ESP_ERROR_CHECK(uart_set_rx_timeout(uart_port, UART_READ_TOUT));
}

// An example of echo test with hardware flow control on UART
static void uart_read_task(void *arg)
{
    uart_port_t uart_port = *((uint8_t *)arg) - '0';

    // Allocate buffers for UART
    uint8_t *data = (uint8_t *) malloc(UART_BUFF_SIZE);
    if (data == NULL) vTaskDelete(NULL);

    //ESP_LOGI(TAG, "UART[%d] start recieve loop.\r\n", uart_port); 
    while ( true ) {
        // Read data from UART
        int len = uart_read_bytes(uart_port, data, 45, 100 / portTICK_RATE_MS);
        if (len > 0) {
            
            printf("Size----%d\r\n", len);
            for (size_t i = 0; i < len; i++)
            {
                printf("%X ", data[i]);
            }
            printf("\r\n");
            
            // ESP_LOGI(TAG, "Received %u bytes:", len);
            // if (uart_callback_func != NULL) {
            //     uart_callback_func(uart_port, data, len);
            // }
        }  
    }
    vTaskDelete(NULL);
}

void app_uart_init(uart_func_t callback_func)
{
    uart_driver_init(UART_NUM_2); 
    //uart_register_callback(callback_func);   
    xTaskCreatePinnedToCore(uart_read_task, "uart1_read_task", 3 * 1024, (void * const)"2", 6, NULL, 0);   

}
