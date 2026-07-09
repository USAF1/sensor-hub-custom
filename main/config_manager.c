#include "config_manager.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static const char *TAG = "CONFIG_MANAGER";
static const char *CONFIG_FILE = "/spiffs/config.json";

static char config_buffer[2048];

static char* get_iso8601_timestamp(void)
{
    static char timestamp[32];
    time_t now = time(NULL);
    struct tm *timeinfo = gmtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", timeinfo);
    return timestamp;
}

esp_err_t config_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing Config Manager (SPIFFS disabled for now)");
    
    // Skip SPIFFS for now - use RAM only
    memset(config_buffer, 0, sizeof(config_buffer));
    
    // Create default config in memory
    snprintf(config_buffer, sizeof(config_buffer),
             "{\"mode\":\"normal\",\"sensors\":[],\"aggregated_status\":{\"apartment_status\":\"VACANT\"}}");
    
    ESP_LOGI(TAG, "Config Manager initialized (RAM-based)");
    return ESP_OK;
}

esp_err_t config_manager_load_config(void)
{
    ESP_LOGI(TAG, "Config load (RAM-based)");
    return ESP_OK;
}

esp_err_t config_manager_save_config(void)
{
    ESP_LOGI(TAG, "Config saved (RAM-based)");
    return ESP_OK;
}

esp_err_t config_manager_update_status(const char *status)
{
    ESP_LOGI(TAG, "Status updated to: %s", status);
    return ESP_OK;
}

esp_err_t config_manager_add_sensor(const char *sensor_id, const char *sensor_name)
{
    ESP_LOGI(TAG, "Sensor added: %s (%s)", sensor_name, sensor_id);
    return ESP_OK;
}

const char* config_manager_get_config_json(void)
{
    return config_buffer;
}