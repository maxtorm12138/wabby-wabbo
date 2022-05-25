#include "wabby/container/delayed.hpp"

namespace wabby::container::detail
{
  std::unordered_map<void *, bool> delayed_tracer::constructed_;

  void delayed_tracer::test_constructed()
  {
#ifndef NDEBUG
    assert( constructed_[this] && "object shold be constructed" );
#endif  // !NDEBUG
  }

  void delayed_tracer::test_not_constructed()
  {
#ifndef NDEBUG
    assert( !constructed_[this] && "construct shold not be constructed" );
#endif  // !NDEBUG
  }

  void delayed_tracer::set_constructed()
  {
#ifndef NDEBUG
    constructed_[this] = true;
#endif  // !NDEBUG
  }
}  // namespace wabby::container::detail