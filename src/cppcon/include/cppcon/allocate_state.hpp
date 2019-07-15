#pragma once

#include <memory>
#include <type_traits>
#include <utility>

namespace cppcon {

namespace detail {

template<typename T,
         typename... Args>
T allocate_state_get_impl(const std::false_type&,
                          Args&&...) noexcept
{
  return T();
}

template<typename T,
         typename U,
         typename... Args>
T allocate_state_get_impl(const std::true_type& tag,
                          U&& u,
                          Args&&... args) noexcept
{
  if constexpr (std::is_same_v<std::decay_t<U>>) {
    return T(std::forward<U>(u));
  } else {
    return detail::allocate_state_get_impl<T>(tag,
                                              std::forward<Args>(args)...);
  }
}

template<typename T,
         typename... Args>
T allocate_state_get(Args&&... args) noexcept {
  using tag_type = typename std::disjunction<std::is_same<T,
                                                          std::decay_t<Args>>...>::type;
  tag_type tag;
  return detail::allocate_state_get_impl<T>(tag,
                                            std::forward<Args>(args)...);
}

}

template<typename State,
         typename... Args>
std::shared_ptr<State> allocate_state(Args&&... args) {
  auto alloc = detail::allocate_state_get<typename State::default_allocator_type>(args...);
  return std::allocate_shared<State>(alloc,
                                     std::forward<Args>(args)...);
}

}
