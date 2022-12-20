#include <stdio.h>
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi.h"
#include "mqtt-lib.h"
#include "dht.h"
#include "sensor-movimento.h"

#define SENSOR_TYPE DHT_TYPE_AM2301
#define DHT_GPIO GPIO_NUM_2
#define PIR_PIN GPIO_NUM_9

int isLuzLigada = 0;

// typedef struct estadoVariaveis {
//     // estados que podem ser alterados tanto por botoes quanto pelo mqtt
//     int isAlarmeLigado = 0;
//     int isLuzLigada = 0;

//     // sensores
//     float temperatura = 0;
//     int hasPresenca = 0;
// } estadoVariaveis;


void app_main(void)
{
    // esp_err_t ret = nvs_flash_init();
    // if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //     ESP_ERROR_CHECK(nvs_flash_erase());
    //     ret = nvs_flash_init();
    // }
    // ESP_ERROR_CHECK(ret);
    // wifi_start();

    // mqtt_start();

    // vTaskDelay(1000 / portTICK_PERIOD_MS);
    // mqtt_subscribe("/embarcados/luz");
    // mqtt_subscribe("/embarcados/alarme");

    // mqtt_publish("/embarcados/temperatura", "40");
    // mqtt_publish("/embarcados/movimento", "1");

    // while(1) {
    //     printf("Luz: %d\n", isLuzLigada);
    //     vTaskDelay(200 / portTICK_PERIOD_MS);
    // }

    // float umidade, temperatura;
    // gpio_set_direction(DHT_GPIO, GPIO_MODE_INPUT);
    // gpio_set_pull_mode(DHT_GPIO, GPIO_PULLUP_ONLY);

    // while (1)
    // {
    //     if (dht_read_float_data(SENSOR_TYPE, DHT_GPIO, &umidade, &temperatura) == ESP_OK) {
    //         printf("Umidade: %f\nTemperatura: %f\n", umidade, temperatura);
    //     }
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }

    while (1)
    {
        if(hasPresence(PIR_PIN)) {
            printf("Presente\n");
        }else {
            printf("Ausente\n");
        }
    }
    
    
}


