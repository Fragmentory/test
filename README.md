# Test definition

Define list with test calls in `test_list.cmake`.

```txt
usage:
  do_test(group_name)             left from first left underscore is the group name
                                  right from first left underscore is the test name
  do_test(group)                  group name, test name are test_N, with N=0,1...
                                  (underscores not allowed) 
  do_test(group@)                 group name, test name are test_N, with N=0,1...
                                  (underscores allowed) 
  do_test(group_name@test_name)   left from @ is the group name (underscores allowed)
                                  right from @ is the test name (underscores allowed)
```

An overview to the current fresh generated group/name enums l.a. 
file "bin/unit_identifier.hpp".
