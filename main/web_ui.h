#ifndef WEB_UI_H
#define WEB_UI_H

#include "esp_err.h"

esp_err_t web_ui_init(void);
void web_ui_task(void *pvParameters);

#endif // WEB_UI_H