cppcon_add_test(testing
                SOURCES allocator.cpp
                        main.cpp
                LIBRARIES Boost::boost
                          Boost::system
                          Catch2::Catch2
                          Threads::Threads)
