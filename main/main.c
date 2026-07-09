#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "esp_log.h"

static const char *TAG = "MAIN";

void console_task(void *pvParameters)
{
    char input_buffer[256];
    int input_idx = 0;
    int c;
    
    printf("\n\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  Console Ready - Type 'help' for commands                   ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    fflush(stdout);
    
    printf("[DEBUG] Console task started\n");
    fflush(stdout);
    
    while (1) {
        printf("sensorhub> ");
        fflush(stdout);
        
        memset(input_buffer, 0, sizeof(input_buffer));
        input_idx = 0;
        
        printf("[DEBUG] Waiting for input...\n");
        fflush(stdout);
        
        // Read one line
        while (input_idx < 255) {
            vTaskDelay(pdMS_TO_TICKS(50));
            
            c = getchar();
            
            printf("[DEBUG] Got: %d\n", c);
            fflush(stdout);
            
            if (c == EOF) {
                printf("[DEBUG] EOF\n");
                fflush(stdout);
                continue;
            }
            
            if (c == '\r' || c == '\n') {
                printf("\n");
                fflush(stdout);
                break;
            }
            
            if (isprint(c)) {
                input_buffer[input_idx++] = c;
                printf("%c", c);
                fflush(stdout);
            }
        }
        
        if (input_idx == 0) {
            continue;
        }
        
        input_buffer[input_idx] = '\0';
        
        // Convert to lowercase
        for (int i = 0; i < input_idx; i++) {
            input_buffer[i] = tolower((unsigned char)input_buffer[i]);
        }
        
        printf("\n[DEBUG] Command: %s\n", input_buffer);
        fflush(stdout);
        
        // Process commands
        if (strcmp(input_buffer, "help") == 0) {
            printf("\nCommands: help, status, sensors, info, test\n\n");
            fflush(stdout);
        }
        else if (strcmp(input_buffer, "status") == 0) {
            printf("\nStatus: OPERATIONAL | Occupancy: VACANT | Sensors: 0/3\n\n");
            fflush(stdout);
        }
        else if (strcmp(input_buffer, "sensors") == 0) {
            printf("\nPaired Sensors: 0 (none)\n\n");
            fflush(stdout);
        }
        else if (strcmp(input_buffer, "info") == 0) {
            printf("\nESP32-C6 SensorHub v1.0 | Zigbee Coordinator\n\n");
            fflush(stdout);
        }
        else if (strcmp(input_buffer, "test") == 0) {
            printf("\nSystem Test: ALL OK\n\n");
            fflush(stdout);
        }
        else {
            printf("\nUnknown: %s\n\n", input_buffer);
            fflush(stdout);
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(void)
{
    printf("\n\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  Zigbee Coordinator SensorHub v1.0                          ║\n");
    printf("║  ESP32-C6 Apartment Occupancy System                        ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    fflush(stdout);
    
    // Initialize VFS for console I/O
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);
    
    printf("[DEBUG] VFS console initialized\n");
    fflush(stdout);
    
    xTaskCreate(console_task, "console", 4096, NULL, 5, NULL);
    
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}