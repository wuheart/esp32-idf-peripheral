/*
 * @Author “Zero” “zero_bbx@qq.com”
 * @Date 2022-12-06 15:55:43
 * @LastEditors “Zero” “zero_bbx@qq.com”
 * @LastEditTime 2023-01-28 17:34:30
 * @FilePath \esp32idf_gpio\src\main.c
 * @Description 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "driver/gpio.h"
//#include "freertos/task.h"
//#include "freertos/queue.h"
//#include "freertos/FreeRTOS.h"
//#include "sdkconfig.h"
//#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "soc/gpio_reg.h"

#include "hp_wu_gpio.h"
#include "hp_wu_adc.h"
#include "hp_wu_timer.h"
#include "hp_wu_uart.h"
#include "hp_wu_wifi.h"

void xTaskOne(void *arg){
    while (1)
    {
        printf("taskone is running !\n");
        gpio_set_level(GPIO_NUM_19,0);
        gpio_set_level(GPIO_NUM_18,1);
        vTaskDelay(1500 / portTICK_PERIOD_MS);
    }
    
}

void print_free_ram(){
 //获取剩余的dram大小
	size_t dram = heap_caps_get_free_size(MALLOC_CAP_8BIT);

	//获取剩余的iram大小
	size_t iram = heap_caps_get_free_size(MALLOC_CAP_32BIT) - heap_caps_get_free_size(MALLOC_CAP_8BIT);

	//获取剩余的dram大小与heap_caps_get_free_size函数一样
	uint32_t data = xPortGetFreeHeapSize();

	//获取最大的连续的堆区空间
	size_t heapmax = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
	int* p = (int*)malloc(heapmax);
	free(p);

	//获取栈区的最高水位线(也就是栈顶，意味着最大栈区空间)
	int stackmark=uxTaskGetStackHighWaterMark(NULL);
	
	printf("data=%d\n", data);
	printf("dram=%d\n", dram);
	printf("iram=%d\n", iram);
	printf("max=%d\n", heapmax);
	printf("stackmark=%d\n", stackmark);
    //获取芯片可用内存
    printf(" esp_get_free_heap_size : %d  \n", esp_get_free_heap_size());
    //获取从未使用过的最小内存
    printf(" esp_get_minimum_free_heap_size : %d  \n", esp_get_minimum_free_heap_size());
}

void app_main(){
    //gpio_Init();
    //xTaskCreate(xTaskOne, "xTask1", 1000, NULL, 1, NULL );
    //adc_init();
    //hp_timer_init();
    // app_uart_init(print_free_ram);
    wu_wifi_init();
    while (1)
    {
        //gpio_set_level(GPIO_NUM_19,1);
        //gpio_set_level(GPIO_NUM_18,0);
        printf("main is running ! \n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
}
