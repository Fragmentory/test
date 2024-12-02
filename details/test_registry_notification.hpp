/**
 * \file test_registry_notification.hpp
 * \author Roman Koch, koch.roman@gmail.com
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "registry_notification.hpp"

#include <cstdio>

namespace test::collection::details
{
    namespace registry
    {
        struct NotificationHandler : public pulp::registry::notification::Interface
        {
            virtual void handle(const pulp::registry::notification::Event &_event)
            {
                printf("registry notice: ");
                switch (_event.cause)
                {
                    case pulp::registry::notification::CAUSE::RESET:
                        printf("RESET ");
                        break;
                    case pulp::registry::notification::CAUSE::UNDEFINED:
                        printf("UNDEFINED ");
                        break;
                    default:
                        printf("? ");
                        break;
                }
                switch (_event.identifier)
                {
                    case pulp::registry::notification::EVENT::BACKUP_CREATED:
                        printf("BACKUP_CREATED ");
                        break;
                    case pulp::registry::notification::EVENT::CRITICAL_ERROR:
                        printf("CRITICAL_ERROR ");
                        break;
                    case pulp::registry::notification::EVENT::PARAMETER_MISSED:
                        printf("PARAMETER_MISSED ");
                        break;
                    case pulp::registry::notification::EVENT::UNDEFINED:
                        printf("UNDEFINED ");
                        break;
                    default:
                        printf("? ");
                        break;
                }
                printf("\n");
            }
        };
    }
}
