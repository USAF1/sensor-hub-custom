#include "sensor_manager.h"
#include "esp_log.h"
#include "zigbee_coordinator.h"
#include "uart_json.h"
#include "config_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <time.h>
#include <string.h>

static const char *TAG = "SENSOR_MANAGER";

#define POLLING_INTERVAL_MS 5000
#define SENSOR_TIMEOUT_MS 10000

static sensor_manager_state_t manager_state = {
    .current_status = SENSOR_STATUS_UNKNOWN,
    .previous_status = SENSOR_STATUS_UNKNOWN,
    .last_status_change = 0,
    .last_poll_time = 0
};

esp_err_t sensor_manager_init(void)
{
    ESP_LOGI(TAG, "Sensor Manager initialized");
    return ESP_OK;
}

void sensor_manager_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Sensor polling task started");
    
    while (1) {
        time_t now = time(NULL);
        manager_state.last_poll_time = now;
        
        // Poll all paired sensors
        int sensor_count = zigbee_coordinator_get_sensor_count();
        uint8_t any_occupied = 0;
        
        for (int i = 0; i < sensor_count; i++) {
            sensor_device_t *sensor = zigbee_coordinator_get_sensor(i);
            if (sensor) {
                // Simulate sensor read (TODO: actual Zigbee read)
                // For now, read from config
                if (sensor->occupancy_status == 1) {
                    any_occupied = 1;
                }
                
                // Check timeout
                if ((now - sensor->last_polled) > (SENSOR_TIMEOUT_MS / 1000)) {
                    sensor->connected = 0;
                    ESP_LOGW(TAG, "Sensor %d timeout", i);
                }
            }
        }
        
        // Aggregate status
        manager_state.current_status = any_occupied ? SENSOR_STATUS_OCCUPIED : SENSOR_STATUS_VACANT;
        
        // Send to Master only on change
        if (manager_state.current_status != manager_state.previous_status) {
            ESP_LOGI(TAG, "Status changed to: %s", 
                     manager_state.current_status == SENSOR_STATUS_OCCUPIED ? "OCCUPIED" : "VACANT");
            
            // Send to Master via UART
            uart_json_send_sensor_status(
                manager_state.current_status == SENSOR_STATUS_OCCUPIED ? "OCCUPIED" : "VACANT",
                sensor_count
            );
            
            manager_state.previous_status = manager_state.current_status;
            manager_state.last_status_change = now;
            
            // Update config file
            config_manager_update_status(
                manager_state.current_status == SENSOR_STATUS_OCCUPIED ? "OCCUPIED" : "VACANT"
            );
        }
        
        vTaskDelay(POLLING_INTERVAL_MS / portTICK_PERIOD_MS);
    }
}

apartment_status_t sensor_manager_get_status(void)
{
    return manager_state.current_status;
}

sensor_manager_state_t sensor_manager_get_state(void)
{
    return manager_state;
}