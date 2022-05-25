#ifdef WABBY_ENGINE_ENTRY

#  include "wabby/core/engine.hpp"

int main( int argc, char ** argv )
{
  try
  {
    wabby::core::engine_setup_info setup_info{ .application_name = "asdf", .application_version = 1, .width = 800, .height = 600 };
    wabby::core::engine            engine;

    engine.setup( setup_info );

    engine.run();

    engine.teardown();
  }
  catch ( ... )
  {
  }
  return 0;
}

#endif