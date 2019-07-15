#pragma once

#include <cassert>
#include <experimental/executor>
#include <memory>
#include <type_traits>
#include <utility>

namespace cppcon {

template<typename CompletionHandler,
         typename Executor = std::experimental::net::system_executor,
         typename Allocator = std::allocator<void>>
class state_base : private Allocator {
template<typename>
friend class op_base;
public:
  using completion_handler_type = CompletionHandler;
  using io_executor_type = Executor;
  using executor_type = std::experimental::net::associated_executor_t<completion_handler_type,
                                                                      io_executor_type>;
  using default_allocator_type = Allocator;
  using allocator_type = std::experimental::net::associated_allocator_t<completion_handler_type,
                                                                        default_allocator_type>;
  state_base() = delete;
  state_base(const state_base&) = delete;
  state_base& operator=(const state_base&) = delete;
  state_base(CompletionHandler handler,
             const io_executor_type& ex,
             const Allocator& alloc = Allocator()) noexcept(std::is_nothrow_move_constructible_v<CompletionHandler>)
    : io_g_   (ex),
      g_      (std::experimental::net::get_associated_executor(handler,
                                                               ex)),
      handler_(std::move(handler))
  {}
  explicit state_base(CompletionHandler handler,
                      const Allocator& alloc = Allocator()) noexcept(std::is_nothrow_move_constructible_v<CompletionHandler>)
    : state_base(std::move(handler),
                 std::experimental::net::system_executor())
  {}
  auto get_executor() const noexcept {
    assert(g_.owns_work());
    return g_.get_executor();
  }
  auto get_allocator() const noexcept {
    auto&& alloc = static_cast<const Allocator&>(*this);
    return std::experimental::net::get_associated_allocator(handler_,
                                                            alloc);
  }
private:
  std::experimental::net::executor_work_guard<io_executor_type> io_g_;
  std::experimental::net::executor_work_guard<executor_type>    g_;
  CompletionHandler                                             handler_;
};

}
