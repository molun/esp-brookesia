/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "systems/phone/widgets/recents_screen/esp_brookesia_recents_screen.hpp"
#include "systems/phone/assets/esp_brookesia_phone_assets.h"

constexpr ESP_Brookesia_RecentsScreenSnapshotData_t ESP_BROOKESIA_PHONE_800_1280_DARK_RECENTS_SCREEN_SNAPSHOT_DATA()
{
    return {
        .main_size = ESP_BROOKESIA_STYLE_SIZE_RECT(500, 860),
        .title = {
            .main_size = ESP_BROOKESIA_STYLE_SIZE_RECT(500, 60),
            .main_layout_column_pad = 10,
            .icon_size = ESP_BROOKESIA_STYLE_SIZE_SQUARE(36),
            .text_font = ESP_BROOKESIA_STYLE_FONT_SIZE(22),
            .text_color = ESP_BROOKESIA_STYLE_COLOR(0xFFFFFF),
        },
        .image = {
            .main_size = ESP_BROOKESIA_STYLE_SIZE_RECT(500, 800),
            .radius = 22,
        },
    };
}

constexpr ESP_Brookesia_RecentsScreenData_t ESP_BROOKESIA_PHONE_800_1280_DARK_RECENTS_SCREEN_DATA()
{
    return {
        .main = {
            .size = ESP_BROOKESIA_STYLE_SIZE_RECT_PERCENT(100, 100),
            .layout_row_pad = 20,
            .layout_top_pad = 0,
            .layout_bottom_pad = 20,
            .background_color = ESP_BROOKESIA_STYLE_COLOR(0x1A1A1A),
        },
        .memory = {
            .main_size = ESP_BROOKESIA_STYLE_SIZE_RECT_W_PERCENT(100, 22),
            .main_layout_x_right_offset = 10,
            .label_text_font = ESP_BROOKESIA_STYLE_FONT_HEIGHT_PERCENT(100),
            .label_text_color = ESP_BROOKESIA_STYLE_COLOR(0xFFFFFF),
            .label_unit_text = "KB",
        },
        .snapshot_table = {
            .main_size = ESP_BROOKESIA_STYLE_SIZE_RECT_PERCENT(100, 100),
            .main_layout_column_pad = 40,
            .snapshot = ESP_BROOKESIA_PHONE_800_1280_DARK_RECENTS_SCREEN_SNAPSHOT_DATA(),
        },
        .trash_icon = {
            .default_size = ESP_BROOKESIA_STYLE_SIZE_SQUARE(48),
            .press_size = ESP_BROOKESIA_STYLE_SIZE_SQUARE(43),
            .image = ESP_BROOKESIA_STYLE_IMAGE(&esp_brookesia_image_large_recents_screen_trash_64_64),
        },
        .flags = {
            .enable_memory = 1,
            .enable_table_height_flex = 1,
            .enable_table_snapshot_use_icon_image = 0,
        },
    };
}
