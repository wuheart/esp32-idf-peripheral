/*
 * @Author “Zero” “zero_bbx@qq.com”
 * @Date 2023-01-28 15:29:35
 * @LastEditors “Zero” “zero_bbx@qq.com”
 * @LastEditTime 2023-01-28 17:38:52
 * @FilePath \esp32idf_gpio\src\hp_wu_wifi.c
 * @Description 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "hp_wu_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_log.h"

/**
 * @brief 用于初始化nvs 
 * @return {*}
 */
void nvs_init(){
    esp_err_t err = nvs_flash_init();
    if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND){
        // 初始化失败，检查错误，重新初始化
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void wu_wifi_init(){
    nvs_init();
    
    esp_netif_init();  //初始化网络接口
    esp_event_loop_create_default();   
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t cfg_sta = {
        .sta ={
            .ssid = "yiree-zhulao",
            .password = "yiree2021",
        }
    };

    esp_wifi_set_config(WIFI_IF_STA, &cfg_sta);
    esp_wifi_set_mode(WIFI_MODE_STA);

    esp_wifi_start();

}