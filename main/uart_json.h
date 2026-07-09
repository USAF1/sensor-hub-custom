#ifndef UART_JSON_H
#define UART_JSON_H

#include "esp_err.h"

esp_err_t uart_json_init(void);
void uart_json_task(void *pvParameters);
esp_err_t uart_json_send_sensor_status(const char *status, int sensor_count);

#endif // UART_JSON_H