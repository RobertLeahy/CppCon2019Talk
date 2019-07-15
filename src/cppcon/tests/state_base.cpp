#include <cppcon/state_base.hpp>

#include <experimental/executor>
#include <type_traits>

namespace cppcon {
namespace tests {
namespace {

class completion_handler {};

using state_base = cppcon::state_base<completion_handler>;

static_assert(!std::is_default_constructible_v<state_base>);
static_assert(!std::is_copy_constructible_v<state_base>);
static_assert(!std::is_copy_assignable_v<state_base>);
static_assert(!std::is_move_constructible_v<state_base>);
static_assert(!std::is_move_assignable_v<state_base>);
static_assert(std::is_constructible_v<state_base,
                                      completion_handler>);
static_assert(std::is_nothrow_constructible_v<state_base,
                                              completion_handler>);

class throwing_completion_handler {
public:
  throwing_completion_handler(throwing_completion_handler&&);
};

using throwing_state_base = cppcon::state_base<throwing_completion_handler>;

static_assert(std::is_constructible_v<throwing_state_base,
                                      throwing_completion_handler>);
static_assert(!std::is_nothrow_constructible_v<throwing_state_base,
                                               throwing_completion_handler>);

}
}
}
