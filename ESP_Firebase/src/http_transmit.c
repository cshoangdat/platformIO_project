#include "http_transmit.h"

static esp_err_t client_event_get_handler(esp_http_client_event_handle_t evt){
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA: %.*s\n", evt->data_len, (char*)evt->data);
        break;
    default:
        break;
    }
    return ESP_OK;
}

void client_post_rest_function(const char *url, char* data){
    esp_http_client_config_t post_config = {
        .url = url,
        .method = HTTP_METHOD_POST,
        .cert_pem = (const char*) certificate_pem_start,
        .event_handler = client_event_get_handler
    };
    esp_http_client_handle_t client = esp_http_client_init(&post_config);

    char *post_data = data;
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_http_client_set_header(client, "Content-Type", "application/json");

    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

void client_get_rest_function(const char *url){
    esp_http_client_config_t get_config = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .cert_pem = (const char*) certificate_pem_start,
        .event_handler = client_event_get_handler
    };
    esp_http_client_handle_t client = esp_http_client_init(&get_config);

    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

void client_patch_rest_function(const char *url, char* data){
    esp_http_client_config_t post_config = {
        .url = url,
        .method = HTTP_METHOD_PATCH,
        .cert_pem = (const char*) certificate_pem_start,
        .event_handler = client_event_get_handler
    };
    esp_http_client_handle_t client = esp_http_client_init(&post_config);

    char *post_data = data;
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_http_client_set_header(client, "Content-Type", "application/json");

    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

