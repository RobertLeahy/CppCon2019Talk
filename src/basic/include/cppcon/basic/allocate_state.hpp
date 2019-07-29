#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <boost/asio/ts/executor.hpp>

namespace cppcon {
namespace basic {

namespace detail {

template<typename CompletionHandler,
         typename Head,
         typename... Tail>
decltype(auto) allocate_shared_find_completion_handler(const Head& head,
                                                       const Tail&... tail) noexcept
{
  if constexpr (std::is_same_v<CompletionHandler,
                               std::decay_t<Head>>)
  {
    return head;
  } else {
    return detail::allocate_shared_find_completion_handler<CompletionHandler>(tail...);
  }
}

}

template<typename State,
         typename... Args>
std::shared_ptr<State> allocate_state(Args&&... args) {
  using completion_handler_type = typename State::completion_handler_type;
  const auto& handler = detail::allocate_shared_find_completion_handler<completion_handler_type>(args...);
  auto a = boost::asio::get_associated_allocator(handler);
  return std::allocate_shared<State>(a,
                                     std::forward<Args>(args)...);
}

}
}
