#ifndef ZIGBEE_COORDINATOR_H
#define ZIGBEE_COORDINATOR_H

#include <time.h>
#include "esp_err.h"

typedef struct {
    uint16_t short_addr;
    uint64_t ieee_addr;
    char device_name[64];
    uint8_t occupancy_status;
    time_t last_polled;
    uint8_t connected;
    uint8_t timeout_counter;
} sensor_device_t;

// Zigbee Coordinator Functions
esp_err_t zigbee_coordinator_init(void);
esp_err_t zigbee_coordinator_start_discovery(void);
esp_err_t zigbee_coordinator_stop_discovery(void);
esp_err_t zigbee_coordinator_read_sensor(sensor_device_t *device);
int zigbee_coordinator_get_sensor_count(void);
sensor_device_t* zigbee_coordinator_get_sensor(int index);

#endif // ZIGBEE_COORDINATOR_H