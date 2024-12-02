/**
 * \file test_application.hpp
 * \author Roman Koch, koch.roman@gmail.com
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "default_device_validator.hpp"
#include "engine_defines.hpp"
#include "event_bus.hpp"
#include "pulp.hpp"
#include "pulp_event.hpp"
#include "test_serial_handler.hpp"
#include "usb_descriptors.hpp"

#include <cstdio>
#include <hardware/gpio.h>
#include <pico/stdlib.h>

namespace test::collection::details
{
    namespace application
    {
        struct Instance
        {
            Instance() :
                core_instance(event_bus),
                validator(event_bus),
                pulp_instance(event_bus, validator, core_instance)
            {
            }

            pulp::COMPONENT_STATE state{pulp::COMPONENT_STATE::UNDEFINED};
            pulp::event::Bus event_bus;
            core::Instance core_instance;
            pulp::check::DefaultValidator validator;
            pulp::Instance pulp_instance;

            void initialize()
            {
                assert(state == pulp::COMPONENT_STATE::UNDEFINED);
                state = pulp::COMPONENT_STATE::IDLE;

                core_instance.set_first_handler(details::handler::black_hole);
                core_instance.set_second_handler(details::handler::black_hole);
                core_instance.assembly.soc.ticker.set_callback(pulp_instance.get_scheduler());

                event_bus.initialize();
                core::usb::set_event_bus(event_bus);

                core_instance.initialize();
                validator.initialize();
                pulp_instance.initialize();
            }

            void shutdown()
            {
                assert(state != pulp::COMPONENT_STATE::UNDEFINED);
                state = pulp::COMPONENT_STATE::UNDEFINED;

                core_instance.shutdown();
                pulp_instance.shutdown();
                validator.shutdown();
                event_bus.shutdown();
            }

            void perform()
            {
                assert(state != pulp::COMPONENT_STATE::UNDEFINED);

                event_bus.notify(pulp::event::Enable());
                pulp_instance.perform();
            }
        };

        template <typename Event>
        struct Trigger
        {
            Trigger(pulp::event::Bus &_event_bus, core::Instance &_core, const uint64_t _duration_ms) :
                event_bus(_event_bus),
                core_instance(_core)
            {
                gpio_init(22);
                gpio_set_dir(22, true);
                gpio_put(22, 1);

                event_bus.subscribe<pulp::event::Perform>([this](const pulp::event::Item &_event) { this->on_event_perform(_event); });
                steps = core_instance.time_ms2tick(static_cast<float>(_duration_ms)) / pulp::event::Perform::HEARTBEAT_COUNTER;
                // printf("duration %llu steps %lu\n", _duration_ms, steps);
            }

            ~Trigger()
            {
                gpio_put(22, 0);
                gpio_deinit(22);
            }

          private:
            void on_event_perform(const pulp::event::Item &_event)
            {
                (void)_event;

                /* ATTENTION: ENGINE NEEDS UP TO 2 ROUNDS EXTRA TO STOP APPLICATION */
                constexpr uint32_t EXTRA_STEPS_TO_STOP_APPLICATION = 2;

                static uint32_t counter = 0;
                if (counter >= ((steps > EXTRA_STEPS_TO_STOP_APPLICATION) ? steps - EXTRA_STEPS_TO_STOP_APPLICATION : steps))
                {
                    counter = 0;
                    event_bus.notify(Event());
                }
                counter++;
            }

            pulp::event::Bus &event_bus;
            core::Instance &core_instance;
            uint32_t steps;
        };
    }
}
