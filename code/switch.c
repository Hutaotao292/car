#include "switch.h"

void switch_init(){
    gpio_init(P22_0, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(P22_1, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(P22_2, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(P22_3, GPI, GPIO_HIGH, GPI_PULL_UP);
}
