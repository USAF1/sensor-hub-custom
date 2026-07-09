#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    printf("\n\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  HELLO WORLD - System Started!                             ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    fflush(stdout);

    ESP_LOGI(TAG, "App main running");

    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}