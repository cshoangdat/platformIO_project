#ifndef _WIFI_CONNECT_DRIVER_H_
#define _WIFI_CONNECT_DRIVER_H_

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define ESP_MAXIMUM_RETRY                           30
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD           WIFI_AUTH_WPA2_PSK

static EventGroupHandle_t s_wifi_event_group;

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1


void wifi_init_sta(const char* ssid, const char* password);
void wifi_connect(const char* wifi_ssid, const char* wifi_pass);

#endif
