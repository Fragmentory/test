/**
 * \file test_record.hpp
 * \author Roman Koch, koch.roman@gmail.com
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "unity.h"

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <utility>

namespace test::collection
{
    namespace details
    {
        static void __test_unknown__(void) { TEST_ASSERT_MESSAGE(0, "unknown test"); }

        /**
         * \brief Singleton to store the test function map with function pointers
         *
         * test definition usage:
         *
         * test::collection::record::Item<1, 0> group1_test0([]() { std::cout << "Running Group 1, Test 0 (lambda)\n"; });
         * test::collection::record::Item<1, 1> group1_test1([]() { std::cout << "Running Group 1, Test 1 (lambda)\n"; });
         * test::collection::record::Item<2, 0> group2_test0([]() { std::cout << "Running Group 2, Test 0 (lambda)\n"; });
         *
         * test call usage:
         *
         * const uint16_t group = 1;
         * const uint16_t identifier = 1;
         *
         * int number_failed = __run_test__(group, identifier);
         */
        struct Record
        {

            static Record &get_instance()
            {
                static Record instance;
                return instance;
            }

            /**
             * \brief Store raw function pointers (void (*)())
             *
             * \param _id
             * \param _test_function
             */
            void register_test(std::pair<uint16_t, uint16_t> _id, void (*_test_function)()) { test_map[_id] = _test_function; }

            void (*get_test(std::pair<uint16_t, uint16_t> _id))()
            {
                auto it = test_map.find(_id);
                if (it != test_map.end())
                {
                    return it->second;
                }
                return nullptr; // Return null if no test is found
            }

            /**
             * \brief Run test by group and identifier
             *
             * \param _group
             * \param _test
             * \return int
             */
            int run_test(uint16_t _group, uint16_t _test)
            {
                UNITY_BEGIN();

                auto test_function = get_test({_group, _test});
                if (test_function)
                {
                    RUN_TEST(test_function);
                }
                else
                {
                    std::cout << "wrong group or test identifier: (" << _group << ", " << _test << ")" << std::endl;
                    RUN_TEST(__test_unknown__);
                }

                return UNITY_END();
            }

          private:
            std::map<std::pair<uint16_t, uint16_t>, void (*)()> test_map;
            Record() = default;
        };
    }

    namespace record
    {
        /**
         * \brief Helper class to register the tests via static initialization
         *
         * Template for a test definition:
         * record::Item<test::GROUP::XXX, test::xxx::IDENTIFIER::YYY> test_template([](){});
         *
         * @tparam _group
         * @tparam _test
         */
        template <uint16_t _group, uint16_t _test>
        struct Item
        {
            Item(void (*_test_function)()) { test::collection::details::Record::get_instance().register_test({_group, _test}, _test_function); }
        };
    }

    extern inline int __run_test__(uint16_t group, uint16_t test) { return test::collection::details::Record::get_instance().run_test(group, test); }
}
