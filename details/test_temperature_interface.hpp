/**
 * \file test_temperature_interface.hpp
 * \author Roman Koch, koch.roman@gmail.com
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "temperature_interface.hpp"

namespace test::collection::details
{
    struct TemperaturInterface : pulp::temperature::Interface
    {
        virtual float get_value(void) const { return 22.5; };
        virtual uint16_t get_raw_value(void) const { return 0x00ff; };
    };
}
