/**
 * \file test_reboot.hpp
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
    namespace reboot
    {
        void check_reset_reason()
        {
            // Lesen Sie den Scratch-Register
            uint32_t reset_cause = watchdog_hw->scratch[0];

            if (reset_cause & (1u << 30))
            { // Beispiel für Power-On Reset Bit
                printf("Reset cause: Power-On Reset\n");
            }
            else if (reset_cause & (1u << 31))
            { // Beispiel für Watchdog Reset Bit
                printf("Reset cause: Watchdog Reset\n");
            }
            else
            {
                printf("Reset cause: Unknown\n");
            }
        }

        static void pico_reboot() { watchdog_reboot(0, 0, 0); }

    }
}