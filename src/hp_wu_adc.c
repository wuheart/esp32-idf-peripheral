/*
 * @Author “Zero” “zero_bbx@qq.com”
 * @Date 2022-12-09 11:01:02
 * @LastEditors “Zero” “zero_bbx@qq.com”
 * @LastEditTime 2023-01-28 17:56:12
 * @FilePath \esp32idf_gpio\src\hp_wu_adc.c
 * @Description 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

//ADC Channels
#if CONFIG_IDF_TARGET_ESP32
#define ADC1_EXAMPLE_CHAN0          ADC1_CHANNEL_6
#define ADC2_EXAMPLE_CHAN5          ADC2_CHANNEL_5
static const char *TAG_CH[2][10] = {{"ADC1_CH6"}, {"ADC2_CH5"}};
#else
#define ADC1_EXAMPLE_CHAN0          ADC1_CHANNEL_2
#define ADC2_EXAMPLE_CHAN0          ADC2_CHANNEL_0
static const char *TAG_CH[2][10] = {{"ADC1_CH2"}, {"ADC2_CH0"}};
#endif

//ADC Attenuation
#define ADC_EXAMPLE_ATTEN           ADC_ATTEN_DB_11

//ADC Calibration
#if CONFIG_IDF_TARGET_ESP32
#define ADC_EXAMPLE_CALI_SCHEME     ESP_ADC_CAL_VAL_EFUSE_VREF
#elif CONFIG_IDF_TARGET_ESP32S2
#define ADC_EXAMPLE_CALI_SCHEME     ESP_ADC_CAL_VAL_EFUSE_TP
#elif CONFIG_IDF_TARGET_ESP32C3
#define ADC_EXAMPLE_CALI_SCHEME     ESP_ADC_CAL_VAL_EFUSE_TP
#elif CONFIG_IDF_TARGET_ESP32S3
#define ADC_EXAMPLE_CALI_SCHEME     ESP_ADC_CAL_VAL_EFUSE_TP_FIT
#endif


static int adc_raw[2][10];
static const char *TAG = "ADC SINGLE";

static esp_adc_cal_characteristics_t adc1_chars;
static esp_adc_cal_characteristics_t adc2_chars;

static bool adc_calibration_init(void)
{
    esp_err_t ret;
    bool cali_enable = false;

    ret = esp_adc_cal_check_efuse(ADC_EXAMPLE_CALI_SCHEME);
    if (ret == ESP_ERR_NOT_SUPPORTED) {
        ESP_LOGW(TAG, "Calibration scheme not supported, skip software calibration");
    } else if (ret == ESP_ERR_INVALID_VERSION) {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else if (ret == ESP_OK) {
        cali_enable = true;
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
        //esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc2_chars);
    } else {
        ESP_LOGE(TAG, "Invalid arg");
    }

    return cali_enable;
}

void adc_init()
{   
    esp_err_t ret = ESP_OK;
    uint8_t ld_out = 0;
    uint32_t voltage_vh = 0;
    uint32_t voltage_vb = 0;
    bool cali_enable = adc_calibration_init();
    //gpio config
    gpio_config_t gpio_ld = {
        .pin_bit_mask = (1ull << 5),
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
    };
    gpio_config(&gpio_ld);
    //ADC1 config
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11));//GPIO35VB
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11));//GPIO34VH
    //ADC2 config
    //ESP_ERROR_CHECK(adc2_config_channel_atten(ADC2_EXAMPLE_CHAN5, ADC_EXAMPLE_ATTEN));

    while (1) {

        adc_raw[1][0] = adc1_get_raw(ADC1_CHANNEL_6);  //VH
        adc_raw[1][1] = adc1_get_raw(ADC1_CHANNEL_7);  //VB
        ld_out = gpio_get_level(GPIO_NUM_5);
        //ESP_LOGI(TAG_CH[1][0], "raw  data: %d", adc_raw[1][0]);

        if (cali_enable) {
            voltage_vh = esp_adc_cal_raw_to_voltage(adc_raw[1][0], &adc1_chars);
            voltage_vb = esp_adc_cal_raw_to_voltage(adc_raw[1][1], &adc1_chars);
            //ESP_LOGI(TAG_CH[1][0], "cali data: %d mV", voltage_vh);
            //ESP_LOGI(TAG_CH[1][0], "cali data: %d mV", voltage_vb);
            printf("%d,%d,%d\r\n",voltage_vh, voltage_vb, ld_out);
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
