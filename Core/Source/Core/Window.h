#include <string>
#include <memory>

// Forward declare GLFWwindow to avoid including glfw3.h
struct GLFWwindow;

namespace Core
{
    class Window
    {
    public:
        // default constructor
        Window() = default;
        Window(int width, int height, const std::string &title);
        ~Window();
        bool shouldClose() const noexcept;

        void pollEvents() const noexcept;

        void swapBuffers() const noexcept;

        std::pair<int, int> getWindowSize() const noexcept;

        void processInput();

        void framebuffer_size_callback(int width, int height);

    private:
        GLFWwindow *wnd;
    };
}