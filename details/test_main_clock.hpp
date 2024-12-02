/**
 * \file test_main_clock.hpp
 * \author Roman Koch, koch.roman@gmail.com
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>
#include <cstdio>
#include <hardware/clocks.h>
#include <hardware/timer.h>
#include <hardware/watchdog.h>
#include <pico/stdlib.h>

namespace test::collection::details
{
    namespace clock
    {
        void init()
        {
            uint32_t freq = clock_get_hz(clk_sys);
            printf("Aktuelle Systemtaktfrequenz: %u Hz\n", freq);
            sleep_ms(100);
            if (freq < 133000000)
            {
                set_sys_clock_khz(133000, true);
                // printf("Systemtakt auf 133 MHz eingestellt\n");
            }

            // UART initialisieren
            uart_init(uart0, 115200); // gewünschte Baudrate ist 115200
            gpio_set_function(0, GPIO_FUNC_UART);
            gpio_set_function(1, GPIO_FUNC_UART);

            // UART-Baudrate anpassen
            uint baudrate = uart_set_baudrate(uart0, 115200);
            printf("UART Baudrate eingestellt: %u\n", baudrate);

            sleep_ms(100);

            // Neue Taktfrequenz abrufen
            freq = clock_get_hz(clk_sys);
            printf("Neue Systemtaktfrequenz: %u Hz\n", freq);

            check_reset_reason();

            // gpio_init(12);
            // gpio_init(13);
            // gpio_init(14);
            // gpio_init(15);
            // gpio_set_dir(12, GPIO_OUT);
            // gpio_set_dir(13, GPIO_OUT);
            // gpio_set_dir(14, GPIO_OUT);
            // gpio_set_dir(15, GPIO_OUT);
        }
    }
}