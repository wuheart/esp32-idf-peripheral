
#include "soc/gpio_reg.h"
#include "driver/gpio.h"


static void IRAM_ATTR ledon(void *arg){
    if(gpio_get_level(12)){
        gpio_set_level(GPIO_NUM_12, 0);
        printf("ON ! \n");
    }else {
        gpio_set_level(GPIO_NUM_12, 1);
        printf("OFF ! \n");
    }
    

}

static void IRAM_ATTR ledoff(void *arg){
    gpio_set_level(GPIO_NUM_12, 0);
    printf("OFF ! \n");
}

void gpio_Init(){

    gpio_config_t gpioconfig = {
        .pin_bit_mask = (1ull << 12),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE
    };
    
    esp_err_t reg = gpio_config(&gpioconfig);
    gpioconfig.pin_bit_mask = (1ull << 14) | (1ull << 15);
    gpioconfig.mode = GPIO_MODE_INPUT;
    gpioconfig.intr_type = GPIO_INTR_NEGEDGE;
    gpioconfig.pull_down_en = GPIO_PULLUP_ENABLE;
    reg = gpio_config(&gpioconfig);
    //中断安装
    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_NUM_14, ledon, NULL);
    //gpio_isr_handler_add(GPIO_NUM_15, ledoff, NULL);
}