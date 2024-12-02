/**
 * \file test_details.hpp
 * \author Roman Koch koch.roman@gmail.com
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>

namespace test::collection
{
    namespace details
    {
        static void test_init_gpio()
        {
            /*
                ATTENTION: do not initialize, perform or shutdown the peach core here.
            */

            stdio_init_all();

            gpio_init(12);
            gpio_init(13);
            gpio_init(14);
            gpio_init(15);

            gpio_set_dir(12, GPIO_OUT);
            gpio_set_dir(13, GPIO_OUT);
            gpio_set_dir(14, GPIO_OUT);
            gpio_set_dir(15, GPIO_OUT);

            printf("*NOTE*: USE SALEAE TOOL TO SNIFF TICKER DATA ON GPIO12, GPIO3, GPIO14 and GPIO15\n");
        }
    }
}
