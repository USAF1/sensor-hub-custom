#include "uart_json.h"
#include "esp_log.h"
#include "driver/uart.h"
#include <time.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static const char *TAG = "UART_JSON";

#define UART_NUM UART_NUM_1
#define UART_TX_GPIO 16
#define UART_RX_GPIO 17
#define UART_BAUD_RATE 115200
#define UART_BUF_SIZE 1024

static char* get_iso8601_timestamp(void)
{
    static char timestamp[32];
    time_t now = time(NULL);
    struct tm *timeinfo = gmtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", timeinfo);
    return timestamp;
}

esp_err_t uart_json_init(void)
{
    ESP_LOGI(TAG, "Initializing UART JSON communication");
    
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, UART_BUF_SIZE * 2, 
                                        UART_BUF_SIZE * 2, 20, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, UART_TX_GPIO, UART_RX_GPIO, 
                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    
    ESP_LOGI(TAG, "UART configured: GPIO16(TX) → Master GPIO17, GPIO17(RX) ← Master GPIO16");
    return ESP_OK;
}

esp_err_t uart_json_send_sensor_status(const char *status, int sensor_count)
{
    char json_msg[512];
    
    snprintf(json_msg, sizeof(json_msg),
             "{\"msg_type\":\"SENSOR_STATUS\",\"timestamp\":\"%s\",\"apartment_status\":\"%s\",\"sensor_count\":%d}\n",
             get_iso8601_timestamp(), status, sensor_count);
    
    uart_write_bytes(UART_NUM, (const char*)json_msg, strlen(json_msg));
    
    ESP_LOGI(TAG, "Sent: %s", json_msg);
    
    return ESP_OK;
}

void uart_json_task(void *pvParameters)
{
    // UART_BUF_SIZE + 1 so that writing the null terminator at data[len]
    // (where len can equal UART_BUF_SIZE) is always in bounds.
    uint8_t data[UART_BUF_SIZE + 1];
    
    ESP_LOGI(TAG, "UART communication task started");
    
    while (1) {
        int len = uart_read_bytes(UART_NUM, data, UART_BUF_SIZE, 100 / portTICK_PERIOD_MS);
        
        if (len > 0) {
            data[len] = '\0';
            ESP_LOGI(TAG, "Received from Master: %s", data);
        }
        
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}