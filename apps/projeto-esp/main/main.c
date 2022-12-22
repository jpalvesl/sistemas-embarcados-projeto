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


#define SENSOR_TYPE DHT_TYPE_DHT11
#define PIR_PIN GPIO_NUM_25
#define LUZ_PIN GPIO_NUM_32
#define ALARME_LED_PIN GPIO_NUM_33
#define BUZZER_PIN GPIO_NUM_26
#define DHT_PIN GPIO_NUM_27
#define SENSOR_C GPIO_NUM_14

int isLuzLigada = 0;
int isAlarmeLigado = 0;
float umidade, temperatura;
int deveLerTemperaturaUmidade = 1;
int alarmeDisparado = 0;


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
    mqtt_subscribe("/embarcados/dados");



    //Definir variaveis e configurar GPIO
    gpio_set_direction(LUZ_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ALARME_LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ALARME_LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIR_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(DHT_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(DHT_PIN, GPIO_PULLUP_ONLY);

    gpio_set_direction(SENSOR_C, GPIO_MODE_INPUT);


    // Pooling
    while (1)
    {
        //Leitura dos dados
        if (deveLerTemperaturaUmidade) {
            dht_read_float_data(SENSOR_TYPE, DHT_PIN, &umidade, &temperatura);
            deveLerTemperaturaUmidade = 0;
        }

        // Leitura do sensor capacitivo
        if (gpio_get_level(SENSOR_C)) {
            if (isAlarmeLigado) {
                gpio_set_level(ALARME_LED_PIN, 0);
                isAlarmeLigado = 0;
            } else {
                gpio_set_level(ALARME_LED_PIN, 1);
                isAlarmeLigado = 1;
            }
            while (gpio_get_level(SENSOR_C));
            
        }
        
        if (isLuzLigada) {
            gpio_set_level(LUZ_PIN, 1);
        } else{
            gpio_set_level(LUZ_PIN, 0);
        }
        if (isAlarmeLigado) {
            gpio_set_level(ALARME_LED_PIN, 1);
            if (hasPresence(PIR_PIN)) {
                gpio_set_level(BUZZER_PIN, 1);
            }
        } else{
            gpio_set_level(ALARME_LED_PIN, 0);
            gpio_set_level(BUZZER_PIN, 0);
        }
        vTaskDelay(1);
    }
}


