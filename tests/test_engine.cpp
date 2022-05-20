#include "core/engine.hpp"

int main( int argc, char * argv[] )
{
  wabby::core::engine engine( "test_engine", 1 );
  engine.run();
}