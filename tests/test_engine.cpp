#include "core/engine.hpp"

int main(int argc, char *argv[])
{
    wawy::core::engine engine("test_engine", WAWY_MAKE_VERSION(1, 0, 0));
    engine.run();
}