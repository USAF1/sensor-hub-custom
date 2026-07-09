#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <time.h>
#include "esp_err.h"
#include "zigbee_coordinator.h"

typedef enum {
    SENSOR_STATUS_VACANT = 0,
    SENSOR_STATUS_OCCUPIED = 1,
    SENSOR_STATUS_UNKNOWN = 2
} apartment_status_t;

typedef struct {
    apartment_status_t current_status;
    apartment_status_t previous_status;
    time_t last_status_change;
    time_t last_poll_time;
} sensor_manager_state_t;

esp_err_t sensor_manager_init(void);
void sensor_manager_task(void *pvParameters);
apartment_status_t sensor_manager_get_status(void);
sensor_manager_state_t sensor_manager_get_state(void);

#endif // SENSOR_MANAGER_H