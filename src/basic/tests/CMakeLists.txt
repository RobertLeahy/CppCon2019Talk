cppcon_add_test(basic
                SOURCES allocate_state.cpp
                        main.cpp
                        op_base.cpp
                        state_base.cpp
                LIBRARIES Catch2::Catch2)
target_link_libraries(basic_tests
                      testing)
