idf_component_register(SRCS "main.c" 
                            "zigbee_coordinator.c"
                            "sensor_manager.c"
                            "uart_json.c"
                            "config_manager.c"
                            "web_ui.c"
                       INCLUDE_DIRS "."
                       REQUIRES freertos
                               nvs_flash
                               spiffs
                               esp_driver_uart)