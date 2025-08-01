/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "bsp/esp-bsp.h"
#include "bsp/display.h"
#include "bsp/touch.h"
#include "esp_brookesia.hpp"
#include "esp_brookesia_app_squareline_demo.hpp"

using namespace esp_brookesia::apps;

#define EXAMPLE_SHOW_MEM_INFO             (1)

#define LVGL_PORT_INIT_CONFIG() \
    {                               \
        .task_priority = 4,       \
        .task_stack = 10 * 1024,       \
        .task_affinity = -1,      \
        .task_max_sleep_ms = 500, \
        .timer_period_ms = 5,     \
    }

static const char *TAG = "app_main";

static void on_clock_update_timer_cb(struct _lv_timer_t *t);

extern "C" void app_main(void)
{
    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = LVGL_PORT_INIT_CONFIG()
    };
    lv_display_t *disp = bsp_display_start_with_config(&cfg);

    ESP_LOGI(TAG, "Display ESP-Brookesia phone demo");
    /**
     * To avoid errors caused by multiple tasks simultaneously accessing LVGL,
     * should acquire a lock before operating on LVGL.
     */
    lvgl_port_lock(0);

    /* Create a phone object */
    ESP_Brookesia_Phone *phone = new ESP_Brookesia_Phone(disp);
    assert(phone && "Create phone failed");

    /* Try using a stylesheet that corresponds to the resolution */
    ESP_Brookesia_PhoneStylesheet_t *stylesheet = nullptr;
    if ((BSP_LCD_H_RES == 480) && (BSP_LCD_V_RES == 480)) {
        stylesheet = new ESP_Brookesia_PhoneStylesheet_t(ESP_BROOKESIA_PHONE_480_480_DARK_STYLESHEET());
        assert(stylesheet && "Create stylesheet failed");
    } else if ((BSP_LCD_H_RES == 800) && (BSP_LCD_V_RES == 480)) {
        stylesheet = new ESP_Brookesia_PhoneStylesheet_t(ESP_BROOKESIA_PHONE_800_480_DARK_STYLESHEET());
        assert(stylesheet && "Create stylesheet failed");
    }
    if (stylesheet != nullptr) {
        ESP_LOGI(TAG, "Using stylesheet (%s)", stylesheet->core.name);
        assert(phone->addStylesheet(stylesheet) && "Add stylesheet failed");
        assert(phone->activateStylesheet(stylesheet) && "Activate stylesheet failed");
        delete stylesheet;
    }

    /* Configure and begin the phone */
    assert(phone->setTouchDevice(bsp_display_get_input_dev()) && "Set touch device failed");
    phone->registerLvLockCallback((ESP_Brookesia_GUI_LockCallback_t)(lvgl_port_lock), 0);
    phone->registerLvUnlockCallback((ESP_Brookesia_GUI_UnlockCallback_t)(lvgl_port_unlock));
    assert(phone->begin() && "Begin failed");
    // assert(phone->getCoreHome().showContainerBorder(), "Show container border failed");

    /* Install apps */
    SquarelineDemo *app_squareline = SquarelineDemo::requestInstance();
    assert(app_squareline && "Create app squareline failed");
    assert((phone->installApp(app_squareline) >= 0) && "Install app squareline failed");

    /* Create a timer to update the clock */
    assert(lv_timer_create(on_clock_update_timer_cb, 1000, phone) && "Create clock update timer failed");

    /* Release the lock */
    lvgl_port_unlock();

#if EXAMPLE_SHOW_MEM_INFO
    char buffer[128];    /* Make sure buffer is enough for `sprintf` */
    size_t internal_free = 0;
    size_t internal_total = 0;
    size_t external_free = 0;
    size_t external_total = 0;
    while (1) {
        internal_free = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
        internal_total = heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
        external_free = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
        external_total = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
        sprintf(buffer, "   Biggest /     Free /    Total\n"
                "\t  SRAM : [%8d / %8d / %8d]\n"
                "\t PSRAM : [%8d / %8d / %8d]",
                heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL), internal_free, internal_total,
                heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM), external_free, external_total);
        ESP_LOGI(TAG, "%s", buffer);

        /**
         * The `lockLv()` and `unlockLv()` functions are used to lock and unlock the LVGL task.
         * They are registered by the `registerLvLockCallback()` and `registerLvUnlockCallback()` functions.
         */
        phone->lockLv();
        // Update memory label on "Recents Screen"
        if (!phone->getHome().getRecentsScreen()->setMemoryLabel(internal_free / 1024, internal_total / 1024,
                external_free / 1024, external_total / 1024)) {
            ESP_LOGE(TAG, "Set memory label failed");
        }
        phone->unlockLv();

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
#endif
}

static void on_clock_update_timer_cb(struct _lv_timer_t *t)
{
    time_t now;
    struct tm timeinfo;
    ESP_Brookesia_Phone *phone = (ESP_Brookesia_Phone *)t->user_data;

    time(&now);
    localtime_r(&now, &timeinfo);

    /* Since this callback is called from LVGL task, it is safe to operate LVGL */
    // Update clock on "Status Bar"
    assert(phone->getHome().getStatusBar()->setClock(timeinfo.tm_hour, timeinfo.tm_min) && "Refresh status bar failed");
}
