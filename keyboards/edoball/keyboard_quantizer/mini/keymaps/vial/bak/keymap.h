#pragma once

typedef union {
    uint32_t raw;
    struct {
        uint8_t key_os_override : 4;
    };
} user_config_t;

enum custom_keycodes {
    KEY_OS_OVERRIDE_DISABLE = 0,
    US_KEY_JP_OS_OVERRIDE_DISABLE,
    JP_KEY_US_OS_OVERRIDE_DISABLE,
};
