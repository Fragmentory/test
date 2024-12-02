# Copyright (c) 2023, Roman Koch, koch.roman@gmail.com
# SPDX-License-Identifier: MIT

function(usage)
    message("Usage:")
    message("  do_test(group_name)")
    message("    - The part before the first underscore '_' is the group name.")
    message("    - The part after the first underscore is the test name.")
    message("")
    message("  do_test(group)")
    message("    - The group name is specified, and test names are generated as 'TEST_N', where N is 0, 1, etc.")
    message("    - Underscores are not allowed in the group name.")
    message("")
    message("  do_test(group@)")
    message("    - The group name is specified, and test names are generated as 'test_N', where N is 0, 1, etc.")
    message("    - Underscores are allowed in the group name.")
    message("")
    message("  do_test(group_name@test_name)")
    message("    - The part before the '@' is the group name (with underscores allowed).")
    message("    - The part after the '@' is the test name (with underscores allowed).")
    message("")
    message("Examples:")
    message("  do_test(my_group_name)")
    message("  do_test(mygroup)")
    message("  do_test(mygroup@)")
    message("  do_test(my_group_name@my_test_name)")
endfunction()

function(do_test TEST_DESCRIPTION)
        set(binary "")

        if(ARGN)
                list(GET ARGN 0 binary)
        endif()

        string(REGEX MATCH "^[a-zA-Z0-9_@]+$" MATCH_RESULT ${TEST_DESCRIPTION})
        if(NOT MATCH_RESULT)
                usage()
                message(FATAL_ERROR "Input string '${TEST_DESCRIPTION}' contains shit.")
        endif()

        set(TNI_VAR "TNI")
        if(NOT DEFINED ${TNI_VAR})
                set(TNI 0)
                set(${TNI_VAR} ${TNI} PARENT_SCOPE)
        endif()

        set(TG "")
        set(TN "")

        string(FIND ${TEST_DESCRIPTION} "@" AT_INDEX)
        if(NOT AT_INDEX EQUAL -1)
                string(SUBSTRING ${TEST_DESCRIPTION} 0 ${AT_INDEX} TG)
                if (NOT TG)
                        set(TG "GROUP_${TNI}")
                
                        math(EXPR II "${${TNI_VAR}} + 1")
                        set(${TNI_VAR} ${II} PARENT_SCOPE)
                        set(TNI ${II})       
                endif()

                math(EXPR TN_START "${AT_INDEX} + 1")
                string(SUBSTRING ${TEST_DESCRIPTION} ${TN_START} -1 TN)

                if (NOT TN)
                        set(TN "TEST_${TNI}")
                
                        math(EXPR II "${${TNI_VAR}} + 1")
                        set(${TNI_VAR} ${II} PARENT_SCOPE)
                        set(TNI ${II})       
                endif()
        else()
                string(FIND ${TEST_DESCRIPTION} "_" UNDERSCORE_INDEX)
                if(NOT UNDERSCORE_INDEX EQUAL -1)
                        string(SUBSTRING ${TEST_DESCRIPTION} 0 ${UNDERSCORE_INDEX} TG)
                        if (NOT TG)
                                set(TG "GROUP_${TNI}")
                        
                                math(EXPR II "${${TNI_VAR}} + 1")
                                set(${TNI_VAR} ${II} PARENT_SCOPE)
                                set(TNI ${II})       
                        endif()

                        math(EXPR TN_START "${UNDERSCORE_INDEX} + 1")
                        string(SUBSTRING ${TEST_DESCRIPTION} ${TN_START} -1 TN)

                        if (NOT TN)
                                set(TN "TEST_${TNI}")
                        
                                math(EXPR II "${${TNI_VAR}} + 1")
                                set(${TNI_VAR} ${II} PARENT_SCOPE)
                                set(TNI ${II})       
                        endif()
                else()
                        set(TG ${TEST_DESCRIPTION})
                        set(TN "TEST_${TNI}")
        
                        math(EXPR II "${${TNI_VAR}} + 1")
                        set(${TNI_VAR} ${II} PARENT_SCOPE)
                        set(TNI ${II})       
                endif()                
        endif()

        string(TOUPPER ${TG} TEST_GROUP_NAME)
        string(TOUPPER ${TN} TEST_NAME)

        # Split the TEST_DESCRIPTION into parts
        #string(REPLACE "_" ";" TEST_DESCRIPTION_PARTS ${TEST_DESCRIPTION})
        #list(LENGTH TEST_DESCRIPTION_PARTS LIST_LENGTH)
        #list(GET TEST_DESCRIPTION_PARTS 0 GROUP_NAME_low)
        #string(TOUPPER ${GROUP_NAME_low} TEST_GROUP_NAME)

        # Get the rest of the parts of the TEST_DESCRIPTION
        #list(SUBLIST TEST_DESCRIPTION_PARTS 1 -1 REST_OF_PARTS)
        #string(REPLACE ";" "_" TEST_NAME_low "${REST_OF_PARTS}")
        #string(TOUPPER ${TEST_NAME_low} TEST_NAME)

        # Generate a unique group ID
        string(MD5 GROUP_HASH ${TEST_GROUP_NAME})
        string(SUBSTRING ${GROUP_HASH} 0 4 GROUP_HEX)
        math(EXPR GROUP_HASH "0x${GROUP_HEX} % 65536")

        set(GLOBAL_LAST_HASH_VAR "LAST_TEST_GROUP_HASH")
        if(NOT DEFINED ${GLOBAL_LAST_HASH_VAR})
                set(LAST_HASH 0)
                set(${GLOBAL_LAST_HASH_VAR} ${LAST_HASH} PARENT_SCOPE)
        else()
                set(LAST_HASH ${${GLOBAL_LAST_HASH_VAR}})
        endif()

        # Variable to store global group counter
        set(GLOBAL_GROUP_COUNTER_VAR "TEST_GROUP")

        # Variable to store test group ID
        set(TEST_IDENTIFIER_VAR "TEST_IDENTIFIER")

        if(NOT ${LAST_HASH} EQUAL ${GROUP_HASH})
                # Check if global group counter is defined, if not, initialize it
                if(NOT DEFINED ${GLOBAL_GROUP_COUNTER_VAR})
                        set(TEST_GROUP 0)
                        set(${GLOBAL_GROUP_COUNTER_VAR} ${TEST_GROUP} PARENT_SCOPE)
                else()
                        math(EXPR NEW_GLOBAL_GROUP_COUNTER "${TEST_GROUP} + 1")
                        set(${GLOBAL_GROUP_COUNTER_VAR} ${NEW_GLOBAL_GROUP_COUNTER} PARENT_SCOPE)
                        set(TEST_GROUP ${NEW_GLOBAL_GROUP_COUNTER})
                endif()

                set(IDENTIFIER 0)
                set(${TEST_IDENTIFIER_VAR} ${IDENTIFIER} PARENT_SCOPE)
                set(${GLOBAL_LAST_HASH_VAR} ${GROUP_HASH} PARENT_SCOPE)
                set(LAST_HASH ${GLOBAL_LAST_HASH_VAR})
        else()
                # Set IDENTIFIER to the current value of the test group ID
                math(EXPR NEW_ID "${${TEST_IDENTIFIER_VAR}} + 1")
                set(${TEST_IDENTIFIER_VAR} ${NEW_ID} PARENT_SCOPE)
                set(IDENTIFIER ${NEW_ID})
        endif()

        # Path to the test script
        set(PICOTOOL_TEST_SCRIPT "${PROJECT_SOURCE_DIR}/build/test.sh")

        set(TEST_CALL_NAME "${TEST_GROUP}_${TEST_NAME}")

        # Add the test command
        if(NOT ${binary} STREQUAL "")
                add_test(NAME ${TEST_CALL_NAME} COMMAND ${PICOTOOL_TEST_SCRIPT} ${TEST_GROUP} ${IDENTIFIER} ${binary} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
        else()
                add_test(NAME ${TEST_CALL_NAME} COMMAND ${PICOTOOL_TEST_SCRIPT} ${TEST_GROUP} ${IDENTIFIER} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
        endif()

        # Set test properties for pass and fail regex and timeout
        set(passRegex "PASSED|OK")
        set(failRegex "FAIL|FAILED|TIMEOUT")
        set_tests_properties(${TEST_CALL_NAME} PROPERTIES PASS_REGULAR_EXPRESSION "${passRegex}")
        set_tests_properties(${TEST_CALL_NAME} PROPERTIES FAIL_REGULAR_EXPRESSION "${failRegex}")
        set_tests_properties(${TEST_CALL_NAME} PROPERTIES TIMEOUT 120)

        # Append the test details to the global test list
        list(APPEND GLOBAL_TEST_LIST "${TEST_GROUP_NAME}/${TEST_NAME}/${GROUP_HASH}/${IDENTIFIER}/${TEST_GROUP}")
        set(GLOBAL_TEST_LIST ${GLOBAL_TEST_LIST} PARENT_SCOPE)
endfunction()
