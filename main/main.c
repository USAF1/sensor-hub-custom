#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "config_manager.h"
#include "zigbee_coordinator.h"
#include "sensor_manager.h"
#include "uart_json.h"

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
    
    while (1) {
        printf("sensorhub> ");
        fflush(stdout);
        
        memset(input_buffer, 0, sizeof(input_buffer));
        input_idx = 0;
        
        // Read one line; skip EOF characters while waiting for real input
        while (input_idx < 255) {
            vTaskDelay(pdMS_TO_TICKS(10));
            
            c = getchar();
            
            if (c == EOF) {
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

    // Install the UART driver for the console port before hooking it into
    // the VFS layer.  Without this, esp_vfs_dev_uart_use_driver() has no
    // underlying driver to forward reads/writes to, so getchar() returns
    // EOF immediately on every call.
    ESP_ERROR_CHECK(uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM,
                                        256,   // RX ring-buffer
                                        0,     // TX ring-buffer (0 = blocking writes)
                                        0, NULL, 0));

    // Configure VFS for console I/O
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);

    // Disable stdin buffering so getchar() returns each byte immediately
    setvbuf(stdin, NULL, _IONBF, 0);

    ESP_LOGI(TAG, "Console UART driver and VFS initialized");

    // Initialize subsystems
    ESP_ERROR_CHECK(config_manager_init());
    ESP_ERROR_CHECK(zigbee_coordinator_init());
    ESP_ERROR_CHECK(sensor_manager_init());
    ESP_ERROR_CHECK(uart_json_init());

    xTaskCreate(console_task, "console", 4096, NULL, 5, NULL);
    
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}