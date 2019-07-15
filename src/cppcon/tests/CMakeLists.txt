cppcon_add_test(cppcon
                SOURCES allocate_state.cpp
                        main.cpp
                        op_base.cpp
                        state_base.cpp
                LIBRARIES Catch2::Catch2)
