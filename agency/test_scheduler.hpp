/**
 * \file test_details.hpp
 * \author Roman Koch, koch.roman@gmail.com
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "unity.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <hardware/irq.h>
#include <hardware/sync.h>
#include <hardware/timer.h>
#include <pico/stdlib.h>

namespace test::collection
{
    using doing_function_t = std::function<void(void)>;

    struct until_timer;

    namespace details
    {
        struct until_timer
        {
            static constexpr bool VERBOSITY_ON = false;
            static constexpr bool TOGGLE_PIN_ON = false;

            static until_timer &get_instance()
            {
                static until_timer instance;
                return instance;
            }

            until_timer() { execution_flag = true; }
            ~until_timer() { cancel_repeating_timer(&timer); }

            void start(int64_t _tick_us, uint32_t _duration_ms = 1000)
            {
                const bool success = add_repeating_timer_us(_tick_us, until_timer::repeating_timer_callback, NULL, &timer);
                assert(success && "failed to add repeating timer");

                duration = static_cast<uint64_t>(1000.0 * _duration_ms / static_cast<double>(std::abs(_tick_us)));
                VERBOSITY_ON &&printf("tick:     %lld us\n", _tick_us);
                VERBOSITY_ON &&printf("duration: %lu ms\n", _duration_ms);
                VERBOSITY_ON &&printf("steps:    %llu\n", duration);
            }

            inline static doing_function_t perform_callback{nullptr};

            void perform(doing_function_t _doing)
            {
                ticker_callback = _doing;
                while (execution_flag)
                {
                    if (perform_callback)
                        perform_callback();
                    __wfi();
                }
            }

            static void stop() { until_timer::get_instance().execution_flag = false; }

            static bool repeating_timer_callback(struct repeating_timer *t)
            {
                (void)t;
                static uint64_t counter = 0;
                if (ticker_callback)
                    ticker_callback();
                if (++counter > duration)
                {
                    ticker_callback = nullptr;
                    perform_callback = nullptr;
                    until_timer::stop();
                }
                return true;
            }

          private:
            friend class test::collection::until_timer;
            bool execution_flag;
            inline static uint64_t duration{0};
            struct repeating_timer timer;
            inline static doing_function_t ticker_callback{nullptr};
        };
    }

    struct until_timer
    {
        uint64_t steps{0};

        static constexpr bool VERBOSITY_ON = false;
        /**
         * \brief Construct a new until timer object
         *
         * \param _tick_us tick duration
         * \param _duration_ms loop duration
         */
        until_timer(int64_t _tick_us, uint32_t _duration_ms = 1000)
        {
            steps = 1000 * _duration_ms / std::abs(_tick_us);

            VERBOSITY_ON &&printf("enter\n");
            details::until_timer::get_instance().start(_tick_us, _duration_ms);
        }
        ~until_timer()
        {
            cancel_repeating_timer(&details::until_timer::get_instance().timer);
            VERBOSITY_ON &&printf("exit\n");
        }

        void perform(doing_function_t _doing)
        {
            VERBOSITY_ON &&printf("perform\n");
            details::until_timer::get_instance().perform(_doing);
            VERBOSITY_ON &&printf("done\n");
        }
    };
}
