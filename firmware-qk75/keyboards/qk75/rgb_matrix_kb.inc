// Прямой режим: цвет каждого светодиода задаёт программа на компьютере (RGB Control) через VIA-команду,
// см. via_custom_value_command_kb в qk75.c
RGB_MATRIX_EFFECT(DIRECT)
// «Сердечки»: от нажатой клавиши вырастает красное сердечко и гаснет; фон — выбранный цвет
RGB_MATRIX_EFFECT(HEARTS)
// «Пламя» — как в RGB Control: каждая клавиша сама по себе тлеет и вспыхивает оттенками выбранного цвета
RGB_MATRIX_EFFECT(FIRE)
// «Змейка» — сама ищет путь к красному яблоку по клавишам, растёт, после 20 клеток или 90 секунд — заново
RGB_MATRIX_EFFECT(SNAKE)

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


// Внутри ли сердечка точка (x, y) в его собственных единицах: (x² + y² − 1)³ − x²·y³ ≤ 0, ось y — вверх
static bool heart_inside(float x, float y) {
    float a = x * x + y * y - 1.0f;
    return a * a * a - x * x * y * y * y <= 0.0f;
}

static bool HEARTS(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    uint8_t v = rgb_matrix_config.hsv.v;
    // Фон — выбранный цвет, приглушённый, чтобы сердечко было видно
    HSV bg_hsv = {rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, v / 3};
    RGB bg = rgb_matrix_hsv_to_rgb(bg_hsv);
    // Сердечко живёт от 1,6 с (скорость на минимуме) до 0,4 с: растёт от ~половины клавиши до ~двух с половиной и гаснет
    uint16_t life = 1600 - (uint16_t)rgb_matrix_config.speed * 1200 / 255;
    float    grow = 28.8f / life;   // за всю жизнь сердечко вырастает на одинаковую величину
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        uint8_t heat = 0;     // насколько ярко сердечко на этой клавише, 0..255
        for (uint8_t j = 0; j < g_last_hit_tracker.count; j++) {
            uint16_t tick = g_last_hit_tracker.tick[j];
            if (tick >= life) continue;
            float size = 7.0f + tick * grow;        // в координатах QMK: клавиша — около 14 по x и 10 по y
            float x = (float)((int16_t)g_led_config.point[i].x - g_last_hit_tracker.x[j]) / size;
            float y = -(float)((int16_t)g_led_config.point[i].y - g_last_hit_tracker.y[j]) / size * 1.4f + 0.35f;
            if (!heart_inside(x, y)) continue;
            uint8_t h = 255 - (uint8_t)((uint32_t)tick * 255 / life);
            if (h > heat) heat = h;
        }
        // Сердечко всегда красное: смешиваем фон с красным по яркости сердечка
        uint8_t r = bg.r + (uint16_t)(v - (bg.r > v ? v : bg.r)) * heat / 255;
        uint8_t g = (uint16_t)bg.g * (255 - heat) / 255;
        uint8_t b = (uint16_t)bg.b * (255 - heat) / 255;
        rgb_matrix_set_color(i, r, g, b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

// ---------- Пламя ----------

// Тот же хеш и плавный шум, что в программе (Effects.Hash / Effects.Noise)
static float fx_hash(int32_t x) {
    uint32_t h = (uint32_t)x * 2654435761u;
    h ^= h >> 15;
    h *= 2246822519u;
    h ^= h >> 13;
    return (h & 0xFFFFFF) / 16777216.0f;
}

static float fx_noise(float x) {
    float   i = floorf(x);
    float   f = x - i;
    float   u = f * f * (3.0f - 2.0f * f);
    int32_t a = (int32_t)i;
    return fx_hash(a) * (1.0f - u) + fx_hash(a + 1) * u;
}

static uint8_t fx_lerp8(uint8_t a, uint8_t b, float k) {
    return (uint8_t)(a + (b - a) * k);
}

static bool FIRE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    // Скорость как у программы: от 0,03 до 2 циклов в секунду по экспоненте
    float speed = rgb_matrix_config.speed / 255.0f;
    float phase = g_rgb_timer / 1000.0f * 0.03f * powf(2.0f / 0.03f, speed);
    RGB   base  = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv);
    RGB   ember = {base.r / 3, base.g / 6, base.b / 3};
    RGB   tip   = {fx_lerp8(base.r, 255, 0.4f), fx_lerp8(base.g, 255, 0.4f), fx_lerp8(base.b, 255, 0.4f)};
    // Языки светлее цвета, но не ярче, чем позволено яркостью
    uint8_t v = rgb_matrix_config.hsv.v;
    tip.r = (uint16_t)tip.r * v / 255;
    tip.g = (uint16_t)tip.g * v / 255;
    tip.b = (uint16_t)tip.b * v / 255;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        float heat = 0.6f * fx_noise(phase * 5.0f + fx_hash(i) * 10.0f) + 0.4f * fx_noise(phase * 9.0f + i * 1.3f + 50.0f);
        heat = (heat - 0.5f) * 1.6f + 0.55f;
        if (heat < 0.0f) heat = 0.0f;
        if (heat > 1.0f) heat = 1.0f;
        RGB c;
        if (heat < 0.5f) {
            float k = heat / 0.5f;
            c.r = fx_lerp8(ember.r, base.r, k), c.g = fx_lerp8(ember.g, base.g, k), c.b = fx_lerp8(ember.b, base.b, k);
        } else {
            float k = (heat - 0.5f) / 0.5f;
            c.r = fx_lerp8(base.r, tip.r, k), c.g = fx_lerp8(base.g, tip.g, k), c.b = fx_lerp8(base.b, tip.b, k);
        }
        rgb_matrix_set_color(i, c.r, c.g, c.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

// ---------- Змейка ----------
// Соседи клавиши — ближайшие слева и справа в том же ряду и ближайшие по горизонтали в рядах выше и ниже
// (по координатам светодиодов). Путь к яблоку — поиск в ширину в обход тела

#define SNAKE_MAX 20
#define SNAKE_ROUND_MS 90000
#define SNAKE_NB 6

static uint8_t  snake_nb[RGB_MATRIX_LED_COUNT][SNAKE_NB];
static uint8_t  snake_nb_count[RGB_MATRIX_LED_COUNT];
static bool     snake_nb_ready;
static uint8_t  snake_body[SNAKE_MAX + 2];  // голова первая
static uint8_t  snake_len, snake_grow, snake_apple;
static uint32_t snake_last_step, snake_game_start;

static bool snake_is_key(uint8_t i) {
    return HAS_ANY_FLAGS(g_led_config.flags[i], (LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER));
}

static void snake_add_nb(uint8_t a, uint8_t b) {
    if (b == 0xFF) return;
    for (uint8_t k = 0; k < snake_nb_count[a]; k++)
        if (snake_nb[a][k] == b) return;
    if (snake_nb_count[a] < SNAKE_NB) snake_nb[a][snake_nb_count[a]++] = b;
}

static void snake_build_nb(void) {
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) snake_nb_count[i] = 0;
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        if (!snake_is_key(i)) continue;
        int16_t ax = g_led_config.point[i].x, ay = g_led_config.point[i].y;
        uint8_t left = 0xFF, right = 0xFF, up = 0xFF, down = 0xFF;
        int16_t bl = 9999, br = 9999, bu = 9999, bd = 9999;
        for (uint8_t j = 0; j < RGB_MATRIX_LED_COUNT; j++) {
            if (j == i || !snake_is_key(j)) continue;
            int16_t dx = g_led_config.point[j].x - ax, dy = g_led_config.point[j].y - ay;
            int16_t adx = dx < 0 ? -dx : dx, ady = dy < 0 ? -dy : dy;
            if (ady < 3) {
                if (dx > 0 && dx < br) br = dx, right = j;
                if (dx < 0 && -dx < bl) bl = -dx, left = j;
            } else if (ady < 16 && adx < 16) {
                if (dy < 0 && adx < bu) bu = adx, up = j;
                if (dy > 0 && adx < bd) bd = adx, down = j;
            }
        }
        snake_add_nb(i, left), snake_add_nb(i, right), snake_add_nb(i, up), snake_add_nb(i, down);
    }
    // Соседство взаимное: ряды сдвинуты, и ближайшие клавиши не всегда видят друг друга
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++)
        for (uint8_t k = 0; k < snake_nb_count[i]; k++) snake_add_nb(snake_nb[i][k], i);
    snake_nb_ready = true;
}

static bool snake_has(uint8_t led, bool tail_leaves) {
    uint8_t n = tail_leaves && snake_grow == 0 ? snake_len - 1 : snake_len;
    for (uint8_t k = 0; k < n; k++)
        if (snake_body[k] == led) return true;
    return false;
}

static uint8_t snake_random_key(bool free_only) {
    uint8_t keys[RGB_MATRIX_LED_COUNT], n = 0;
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++)
        if (snake_is_key(i) && (!free_only || !snake_has(i, false))) keys[n++] = i;
    return n ? keys[random8() % n] : 0xFF;
}

static void snake_new_game(void) {
    snake_body[0]    = snake_random_key(false);
    snake_len        = 1;
    snake_grow       = 2;
    snake_game_start = g_rgb_timer;
    snake_apple      = snake_random_key(true);
}

// Первый шаг кратчайшего пути от головы к яблоку; 0xFF — пути нет
static uint8_t snake_path_step(void) {
    static uint8_t from[RGB_MATRIX_LED_COUNT], queue[RGB_MATRIX_LED_COUNT];
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) from[i] = 0xFF;
    uint8_t head = snake_body[0], qh = 0, qt = 0;
    from[head]  = head;
    queue[qt++] = head;
    while (qh < qt) {
        uint8_t c = queue[qh++];
        if (c == snake_apple) {
            while (from[c] != head) c = from[c];
            return c;
        }
        for (uint8_t k = 0; k < snake_nb_count[c]; k++) {
            uint8_t nb = snake_nb[c][k];
            if (from[nb] != 0xFF || snake_has(nb, true)) continue;
            from[nb]    = c;
            queue[qt++] = nb;
        }
    }
    return 0xFF;
}

static void snake_step(void) {
    if (snake_len == 0 || snake_apple == 0xFF || snake_len >= SNAKE_MAX || g_rgb_timer - snake_game_start > SNAKE_ROUND_MS) {
        snake_new_game();
        return;
    }
    uint8_t next = snake_path_step();
    if (next == 0xFF) {
        // До яблока не добраться — ползём на любую свободную соседнюю клавишу, а если таких нет, начинаем заново
        uint8_t options[SNAKE_NB], n = 0, head = snake_body[0];
        for (uint8_t k = 0; k < snake_nb_count[head]; k++)
            if (!snake_has(snake_nb[head][k], true)) options[n++] = snake_nb[head][k];
        if (n == 0) {
            snake_new_game();
            return;
        }
        next = options[random8() % n];
    }
    for (uint8_t k = snake_len; k > 0; k--) snake_body[k] = snake_body[k - 1];
    snake_body[0] = next;
    snake_len++;
    if (next == snake_apple) {
        snake_grow++;
        snake_apple = snake_random_key(true);
    }
    if (snake_grow > 0) snake_grow--;
    else snake_len--;
}

static bool SNAKE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    if (!snake_nb_ready) snake_build_nb();
    if (params->init) {
        snake_new_game();
        snake_last_step = g_rgb_timer;
    }
    // Ход игры — один раз за кадр, на первой порции светодиодов. От 2,5 шага в секунду (медленно) до 20 (быстро)
    if (led_min == 0) {
        uint16_t interval = 400 - (uint16_t)rgb_matrix_config.speed * 350 / 255;
        if (g_rgb_timer - snake_last_step > 2000) snake_last_step = g_rgb_timer;
        while (g_rgb_timer - snake_last_step >= interval) {
            snake_last_step += interval;
            snake_step();
        }
    }
    uint8_t v    = rgb_matrix_config.hsv.v;
    RGB     body = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv);
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        uint8_t r = 0, g = 0, b = 0;
        if (i == snake_apple) r = v;
        for (uint8_t k = 0; k < snake_len; k++) {
            if (snake_body[k] != i) continue;
            if (k == 0) {
                // Голова светлее тела
                r = (body.r + v) / 2, g = (body.g + v) / 2, b = (body.b + v) / 2;
            } else {
                // Тело к хвосту тускнеет до 35%
                uint16_t fade = 255 - (uint16_t)166 * k / (snake_len > 1 ? snake_len - 1 : 1);
                r = body.r * fade / 255, g = body.g * fade / 255, b = body.b * fade / 255;
            }
            break;
        }
        rgb_matrix_set_color(i, r, g, b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif
