#define CATCH_CONFIG_MAIN
#include <Catch.hpp>
#include <Core/Window.h>

TEST_CASE("Window tests", "[Core]")
{
    Core::Window wnd = Core::Window(800, 600, "Test");
    REQUIRE(wnd.shouldClose() == false);

    std::pair<int, int> size = wnd.getWindowSize();
    REQUIRE(size.first == 800);
    REQUIRE(size.second == 600);
}