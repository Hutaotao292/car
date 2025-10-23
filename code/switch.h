#ifndef SWITCH_H
#define SWITCH_H

#include "zf_driver_gpio.h"

#define SWITCH_1 (!gpio_get_level(P22_0))
#define SWITCH_2 (!gpio_get_level(P22_1))
#define SWITCH_3 (!gpio_get_level(P22_2))
#define SWITCH_4 (!gpio_get_level(P22_3))


void switch_init();

#endif //SWITCH_H