cppcon_add_library(testing SOURCES allocator.cpp
                                   async_write_stream.cpp
                                   pending_service.cpp
                           LIBRARIES Boost::boost
                                     Boost::system)
add_subdirectory(tests)
