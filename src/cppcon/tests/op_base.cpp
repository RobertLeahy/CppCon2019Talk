#include <cppcon/op_base.hpp>

#include <experimental/io_context>
#include <memory>
#include <stdexcept>
#include <system_error>
#include <utility>
#include <cppcon/state_base.hpp>

#include <catch2/catch.hpp>

namespace cppcon {
namespace tests {
namespace {

class completion_handler_state {
public:
  completion_handler_state() noexcept
    : invoked(false)
  {}
  completion_handler_state(const completion_handler_state&) = delete;
  completion_handler_state& operator=(const completion_handler_state&) = delete;
  bool            invoked;
  std::error_code ec;
};

class completion_handler {
public:
  completion_handler() = delete;
  completion_handler(const completion_handler&) = delete;
  completion_handler(completion_handler&&) = default;
  completion_handler& operator=(const completion_handler&) = delete;
  completion_handler& operator=(completion_handler&&) = delete;
  explicit completion_handler(completion_handler_state& state) noexcept
    : state_(state)
  {}
  void operator()(std::error_code ec) {
    if (state_.invoked) {
      throw std::logic_error("Double invoke");
    }
    state_.invoked = true;
    state_.ec = ec;
  }
private:
  completion_handler_state& state_;
};

template<typename State>
class op : public op_base<State> {
private:
  using base = op_base<State>;
public:
  using base::base;
  using base::upcall;
  using base::state;
};

TEST_CASE("op_base::state",
          "[op_base]")
{
  completion_handler_state state;
  using state_type = state_base<completion_handler>;
  auto ptr = std::make_shared<state_type>(completion_handler(state));
  op<state_type> o(ptr);
  CHECK(&o.state() == ptr.get());
}

TEST_CASE("op_base::upcall",
          "[op_base]")
{
  completion_handler_state state;
  using execution_context_type = std::experimental::net::io_context;
  execution_context_type ctx;
  using state_type = state_base<completion_handler,
                                execution_context_type::executor_type>;
  auto ptr = std::make_shared<state_type>(completion_handler(state),
                                          ctx.get_executor());
  auto count = ctx.poll();
  CHECK(count == 0);
  REQUIRE_FALSE(ctx.stopped());
  op<state_type> o(std::move(ptr));
  o.upcall(std::error_code());
  count = ctx.poll();
  //  TODO: Assert count is one?
  CHECK(state.invoked);
  CHECK_FALSE(state.ec);
  CHECK(ctx.stopped());
}

TEST_CASE("op_base::get_executor",
          "[op_base]")
{
  completion_handler_state state;
  using state_type = state_base<completion_handler>;
  auto ptr = std::make_shared<state_type>(completion_handler(state));
  op<state_type> o(ptr);
  CHECK(ptr->get_executor() == o.get_executor());
}

TEST_CASE("op_base::get_allocator",
          "[op_base]")
{
  completion_handler_state state;
  using state_type = state_base<completion_handler>;
  auto ptr = std::make_shared<state_type>(completion_handler(state));
  op<state_type> o(ptr);
  CHECK(ptr->get_allocator() == o.get_allocator());
}

}
}
}
