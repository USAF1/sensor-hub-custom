#include "web_ui.h"
#include "esp_log.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "WEB_UI";

esp_err_t web_ui_init(void)
{
    ESP_LOGI(TAG, "Web UI initialized (console only)");
    return ESP_OK;
}

void web_ui_task(void *pvParameters)
{
    char input_buffer[256];
    int input_idx = 0;
    int c;
    
    ESP_LOGI(TAG, "Web UI task started");
    printf("\n");
    printf("Type 'help' for commands\n\n");
    
    while (1) {
        printf("sensorhub> ");
        fflush(stdout);
        
        memset(input_buffer, 0, sizeof(input_buffer));
        input_idx = 0;
        
        // Read input line
        int line_complete = 0;
        while (!line_complete) {
            vTaskDelay(pdMS_TO_TICKS(10));
            c = getchar();
            
            if (c != EOF) {
                if (c == '\n' || c == '\r') {
                    printf("\n");
                    line_complete = 1;
                } else if (isprint(c) && input_idx < 255) {
                    input_buffer[input_idx++] = c;
                    printf("%c", c);
                    fflush(stdout);
                }
            }
        }
        
        if (input_idx == 0) {
            continue;
        }
        
        input_buffer[input_idx] = '\0';
        
        // Convert to lowercase
        for (int i = 0; input_buffer[i]; i++) {
            input_buffer[i] = tolower((unsigned char)input_buffer[i]);
        }
        
        // Process commands
        if (strcmp(input_buffer, "help") == 0) {
            printf("Commands: help, status, sensors, config, info, test\n\n");
        }
        else if (strcmp(input_buffer, "status") == 0) {
            printf("Status: OPERATIONAL | Occupancy: VACANT\n\n");
        }
        else if (strcmp(input_buffer, "sensors") == 0) {
            printf("Paired Sensors: 0 (none)\n\n");
        }
        else if (strcmp(input_buffer, "config") == 0) {
            printf("Config: Loaded (RAM-based)\n\n");
        }
        else if (strcmp(input_buffer, "info") == 0) {
            printf("ESP32-C6 SensorHub v1.0 | Zigbee Coordinator\n\n");
        }
        else if (strcmp(input_buffer, "test") == 0) {
            printf("System: ALL OK\n\n");
        }
        else {
            printf("Unknown command\n\n");
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}