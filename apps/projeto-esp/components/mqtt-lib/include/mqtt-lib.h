#ifndef MQTT_LIB_H
#define MQTT_LIB_H

void mqtt_start(void);

void mqtt_publish(char* topic, char* data);

void mqtt_subscribe(char* topic);


#endif