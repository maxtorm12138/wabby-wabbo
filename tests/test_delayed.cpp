#include "wabby/container/delayed.hpp"

#include "gtest/gtest.h"

class delayed_test : public ::testing::Test
{
};

class Foo
{
public:
  Foo( int a )
  {
    a_ = a;
  }
  ~Foo()
  {
    a_ = INT_MAX;
  }

public:
  uint32_t a_{ UINT32_MAX };
};

TEST_F( delayed_test, test_construct )
{
  wabby::container::delayed<Foo> foo;
  foo.construct( UINT32_MAX );
  ASSERT_EQ( foo->a_, UINT32_MAX );
}

TEST_F( delayed_test, test_vector )
{
  wabby::container::delayed<std::vector<int>> i;
  i.construct( 10, 1 );
  i[0] = 2;
  EXPECT_EQ( i[0], 2 );
  EXPECT_EQ( i[1], 1 );
}