#pragma once

#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>
#include "state_base.hpp"

namespace cppcon {

template<typename State>
class op_base {
private:
  using completion_handler_type = typename State::completion_handler_type;
public:
  using executor_type = typename State::executor_type;
  using allocator_type = typename State::allocator_type;
  op_base() = delete;
  op_base(const op_base&) = delete;
  op_base(op_base&&) = default;
  op_base& operator=(const op_base&) = delete;
  op_base& operator=(op_base&&) = default;
  explicit op_base(std::shared_ptr<State> state) noexcept
    : state_(std::move(state))
  {
    assert(state_);
  }
  auto get_executor() const noexcept {
    assert(state_);
    return state_->get_executor();
  }
  auto get_allocator() const noexcept {
    assert(state_);
    return state_->get_allocator();
  }
protected:
  State& state() const noexcept {
    assert(state_);
    return *state_;
  }
  template<typename... Args>
  void upcall(Args&&... args) noexcept(std::is_nothrow_move_constructible_v<completion_handler_type> &&
                                       noexcept(std::declval<completion_handler_type&>()(std::forward<Args>(args)...)))
  {
    assert(state_);
    assert(state_.use_count() == 1);
    auto io_g = std::move(state_->io_g_);
    auto g = std::move(state_->g_);
    auto alloc = state_->get_allocator();
    auto handler = std::move(state_->handler_);
    state_.reset();
    handler(std::forward<Args>(args)...);
  }
private:
  std::shared_ptr<State> state_;
};

}
