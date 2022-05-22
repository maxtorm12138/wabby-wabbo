#include "core/engine.hpp"

int main( int argc, char * argv[] )
{
  wabby::core::engine engine;

  wabby::core::engine_setup_info setup_info{ .application_name = "asdf", .application_version = 1, .width = 800, .height = 600 };

  engine.setup( setup_info );

  engine.run();

  engine.teardown();
}