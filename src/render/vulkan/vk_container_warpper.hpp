#include "vk_allocator.hpp"

// std
#include "string"
#include "unordered_map"
#include "unordered_set"
#include "vector"

namespace wabby::render::vulkan
{
  template <typename T>
  using vk_vector = std::vector<T, vk_allocator<T>>;

  using vk_string = std::basic_string<char, std::char_traits<char>, vk_allocator<char>>;

  template <typename K, typename V>
  using vk_unordered_map = std::unordered_map<K, V, std::hash<K>, std::equal_to<K>, vk_allocator<std::pair<const K, V>>>;

  template <typename K>
  using vk_unordered_set = std::unordered_set<K, std::hash<K>, std::equal_to<K>, vk_allocator<K>>;

}  // namespace wabby::render::vulkan