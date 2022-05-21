#include "container/string.hpp"

#include "gtest/gtest.h"

class string_test : public ::testing::Test
{
};

TEST_F( string_test, small_string_construct )
{
  wabby::container::string str( "hello world" );
  EXPECT_STREQ( str.c_str(), "hello world" );
}

TEST_F( string_test, large_string_construct )
{
  wabby::container::string str( "hello world, hello world, hello world, hello world" );
  EXPECT_STREQ( str.c_str(), "hello world, hello world, hello world, hello world" );
}