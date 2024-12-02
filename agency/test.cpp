/**
 * \file test.cpp
 * \author Roman Koch, koch.roman@gmail.com
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#include "test_record.hpp"
#include "unit_identifier.hpp"
#include "unity.h"

#include <cstdio>
#include <hardware/flash.h>
#include <hardware/uart.h>
#include <hardware/watchdog.h>
#include <pico/bootrom.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/time.h>

/**
 * \brief Test group and test identifier
 *
 * ATTENTION: do not change order of the test_group and test_identifier
 */
/**@{ */
volatile uint16_t test_group __attribute__((section(".uninitialized_data"), used));
volatile uint16_t test_identifier __attribute__((section(".uninitialized_data"), used));
/**}@ */

void setUp(void) {}
void tearDown(void) {}

namespace test
{
    extern void execute(void)
    {
        stdio_init_all();
        sleep_ms(100);

        if (watchdog_caused_reboot())
            printf("REBOOT CAUSED BY WATCHDOG\n");

        const test::GROUP group = static_cast<test::GROUP>(test_group);
        const uint16_t identifier = test_identifier;
        // const test::GROUP group = test::GROUP::VISUAL;
        // const uint16_t identifier = test::visual::SMOOTH;

        test_group = 0xffff;
        test_identifier = 0xffff;

        int number_failed = collection::__run_test__(group, identifier);
        if (number_failed)
        {
            printf("FAILED: %d\n\n", number_failed);
        }
        else
        {
            printf("PASSED\n\n");
        }
        printf("__UNIT_TEST_DONE__\n");

        /* reboot rp2040 in BOOTSEL mode */
        reset_usb_boot((0x00000001 << PICO_DEFAULT_LED_PIN), 0);
    }
}
