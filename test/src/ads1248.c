#include "ads1248.h"

#include <stdio.h>
#include <stdint.h>

struct ads1248_options_t {
    uint32_t pin_reset;
    int pin_drdy;
    int pin_start;
    int *spi_module;
    int cs_id;
};

ads1248_options_t* ads1248_init ()
{
    printf("ads1248_init call\n");
}

void ads1248_destroy (ads1248_options_t* ads)
{
}
