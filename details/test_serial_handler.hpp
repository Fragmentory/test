/**
 * \file test_serial_handler.hpp
 * \author Roman Koch, koch.roman@gmail.com
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "bytering.hpp"

namespace test::collection::details
{
    namespace handler
    {
        static void black_hole(buffer_t *const _rx, buffer_t *const _tx)
        {
            (void)_tx;

            uint8_t value = 0;
            while (bytering_read(_rx, &value))
            {
                /* drop away any input */
            }
        }

        static void echo(buffer_t *const _rx, buffer_t *const _tx)
        {
            uint8_t value = 0;
            bytering_peek(_rx, 0, &value);
            bytering_copy(_rx, _tx);
        }
    }
}
