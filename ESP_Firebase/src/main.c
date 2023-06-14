#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi_connect.h"
#include "http_transmit.h"

const char* ESP_WIFI_SSID =                             "Ambi";
const char* ESP_WIFI_PASS =                             "1234ambi";
const char* URL =                                       "https://firestore.googleapis.com/v1/projects/esp-idf-project/databases/(default)/documents/Data";
char* data =                                            "{\"fields\":{\"Humidity\":{\"integerValue\":\"30\"},\"Temperature\":{\"integerValue\":\"10\"},\"Co2\":{\"integerValue\":\"30\"},\"Lux\":{\"integerValue\":\"35\"}}}";
// char* data;
int temp = 50;

void app_main(void)
{
    wifi_connect(ESP_WIFI_SSID, ESP_WIFI_PASS);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    // client_get_rest_function(URL);
    // for(int i = 0; i<10; i++){
    //     temp += 1;
        // char data[100] = "{\"fields\":";
        // strcat(data, "{\"Temperature\":{\"integerValue\":");
        // strcat(data, "\"245\"");
        // strcat(data,"}}}");
        // client_post_rest_function(URL, data); 
    // client_patch_rest_function(URL, data);
}
