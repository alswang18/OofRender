#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

const int VERSION_MAJOR = 3;
const int VERSION_MINOR = 3;
namespace Core
{

    Window::Window(int width, int height, const std::string &title)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        wnd = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (wnd == NULL)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
            return;
        }
        glfwMakeContextCurrent(wnd);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
            return;
        }
    }

    Window::~Window()
    {
        glfwTerminate();
    }

    std::pair<int, int> Window::getWindowSize() const noexcept
    {
        int width, height;
        glfwGetWindowSize(wnd, &width, &height);
        return std::make_pair(width, height);
    }

    bool Window::shouldClose() const noexcept
    {
        return glfwWindowShouldClose(wnd) != 0;
    }

    void Window::pollEvents() const noexcept
    {
        glfwPollEvents();
    }

    void Window::swapBuffers() const noexcept
    {
        glfwSwapBuffers(wnd);
    }

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void Window::processInput()
    {
        if (glfwGetKey(wnd, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(wnd, true);
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void Window::framebuffer_size_callback(int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

}