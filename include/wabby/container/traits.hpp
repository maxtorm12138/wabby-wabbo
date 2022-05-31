#ifndef WABBY_CONTAINER_TRAITS_HPP
#define WABBY_CONTAINER_TRAITS_HPP

#include "type_traits"

namespace wabby::container
{
  template <typename T, typename = void>
  struct has_subscript : std::false_type
  {
  };

  template <typename T>
  struct has_subscript<T, std::void_t<decltype( ( *std::declval<T *>() )[std::declval<size_t>()] )>> : std::true_type
  {
  };
}  // namespace wabby::container

#endif