#include <cppcon/allocate_state.hpp>

#include <cppcon/state_base.hpp>
#include <catch2/catch.hpp>

namespace cppcon {
namespace tests {
namespace {

class completion_handler {};

TEST_CASE("allocate_state w/std::allocator<void>",
          "[allocate_state]")
{
  using state_type = state_base<completion_handler>;
  auto ptr = allocate_state<state_type>(completion_handler());
  REQUIRE(ptr);
}

}
}
}
