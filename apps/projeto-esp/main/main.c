#include <stdio.h>
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#include "wifi.h"
#include "mqtt-lib.h"
#include "dht.h"
#include "sensor-movimento.h"

// #define LEDC_TIMER              LEDC_TIMER_0
// #define LEDC_MODE               LEDC_LOW_SPEED_MODE
// #define LEDC_OUTPUT_IO          (26) // Define the output GPIO
// #define LEDC_CHANNEL            LEDC_CHANNEL_0
// #define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
// #define LEDC_DUTY               (4095) // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
// #define LEDC_FREQUENCY          (5000) // Frequency in Hertz. Set frequency at 5 kHz


// int32_t dutyled1 = 0;


// static void example_ledc_init(int frequencia)
// {
//     // Prepare and then apply the LEDC PWM timer configuration
//     ledc_timer_config_t ledc_timer = {
//         .speed_mode       = LEDC_MODE,
//         .timer_num        = LEDC_TIMER,
//         .duty_resolution  = LEDC_DUTY_RES,
//         .freq_hz          = frequencia,  // Set output frequency at 5 kHz
//         //.clk_cfg          = LEDC_AUTO_CLK
//     };
//     ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

//     // Prepare and then apply the LEDC PWM channel configuration
//     ledc_channel_config_t ledc_channel = {
//         .speed_mode     = LEDC_MODE,
//         .channel        = LEDC_CHANNEL,
//         .timer_sel      = LEDC_TIMER,
//         .intr_type      = LEDC_INTR_DISABLE,
//         .gpio_num       = LEDC_OUTPUT_IO,
//         .duty           = 0, // Set duty to 0%
//         //.hpoint         = 0
//     };
//     ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
// }


#define SENSOR_TYPE DHT_TYPE_DHT11
#define DHT_PIN GPIO_NUM_2
#define PIR_PIN GPIO_NUM_25
#define LUZ_PIN GPIO_NUM_32
#define ALARME_LED_PIN GPIO_NUM_33
#define BUZZER_PIN GPIO_NUM_26

int isLuzLigada = 0;
int isAlarmeLigado = 0;

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    wifi_start();

    mqtt_start();

    vTaskDelay(1000 / portTICK_PERIOD_MS);
    mqtt_subscribe("/embarcados/luz");
    mqtt_subscribe("/embarcados/alarme");



    //Definir variaveis e configurar GPIO
    gpio_set_direction(LUZ_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ALARME_LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ALARME_LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIR_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(DHT_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(DHT_PIN, GPIO_PULLUP_ONLY);

    float umidade, temperatura;

    // Pooling
    while (1)
    {
        // Leitura dos dados
        // if (dht_read_float_data(SENSOR_TYPE, DHT_PIN, &umidade, &temperatura) == ESP_OK) {
        //     printf("Umidade: %f\nTemperatura: %f\n", umidade, temperatura);
        // }
        
        if (isLuzLigada) {
            gpio_set_level(LUZ_PIN, 1);
        } else{
            gpio_set_level(LUZ_PIN, 0);
        }
        if (isAlarmeLigado) {
            gpio_set_level(ALARME_LED_PIN, 1);
            if (hasPresence(PIR_PIN)) {
                gpio_set_level(BUZZER_PIN, 1);
            } else {
                gpio_set_level(BUZZER_PIN, 0);
            }
        } else{
            gpio_set_level(ALARME_LED_PIN, 0);
        }
    }
    
    // while (1)
    // {
    //     if(hasPresence(PIR_PIN)) {
    //         printf("Presente\n");
    //     }else {
    //         printf("Ausente\n");
    //     }
    // }
    
    
}


