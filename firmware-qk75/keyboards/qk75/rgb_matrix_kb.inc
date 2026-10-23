// Прямой режим: цвет каждого светодиода задаёт программа на компьютере (RGB Control) через VIA-команду,
// см. via_custom_value_command_kb в qk75.c
RGB_MATRIX_EFFECT(DIRECT)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

extern uint8_t g_direct_rgb[RGB_MATRIX_LED_COUNT][3];

static bool DIRECT(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i = led_min; i < led_max; i++) {
        // Как у встроенных эффектов, не выше RGB_MATRIX_MAXIMUM_BRIGHTNESS — бережём питание от USB
        rgb_matrix_set_color(i,
            (uint16_t)g_direct_rgb[i][0] * RGB_MATRIX_MAXIMUM_BRIGHTNESS / 255,
            (uint16_t)g_direct_rgb[i][1] * RGB_MATRIX_MAXIMUM_BRIGHTNESS / 255,
            (uint16_t)g_direct_rgb[i][2] * RGB_MATRIX_MAXIMUM_BRIGHTNESS / 255);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif
