cppcon_add_test(basic
                SOURCES main.cpp
                        state_base.cpp
                LIBRARIES Catch2::Catch2)
target_link_libraries(basic_tests
                      testing)
