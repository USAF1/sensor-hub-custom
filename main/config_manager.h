#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "esp_err.h"

esp_err_t config_manager_init(void);
esp_err_t config_manager_load_config(void);
esp_err_t config_manager_save_config(void);
esp_err_t config_manager_update_status(const char *status);
esp_err_t config_manager_add_sensor(const char *sensor_id, const char *sensor_name);
const char* config_manager_get_config_json(void);

#endif // CONFIG_MANAGER_H