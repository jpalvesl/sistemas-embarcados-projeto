#include <stdio.h>

#include "sensor-movimento.h"


uint8_t hasPresence(gpio_num_t pin) {
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    
    return (uint8_t) gpio_get_level(pin);
}