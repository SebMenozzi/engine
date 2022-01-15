#include "window.h"

namespace window
{
    // MARK: Public

    Window::Window(uint32 width, uint32 height, const char* title): 
        width_(width), 
        height_(height),
        title_(title)
    {}

    static void setWindowSizeCallback_(GLFWwindow* window, int width, int height)
    {
        void *ptr = glfwGetWindowUserPointer(window);
        if (Window *windowPtr = static_cast<Window*>(ptr))
            windowPtr->setUpdateViewport(true);
    }

    void Window::init()
    {
        window_ = glfwCreateWindow(width_, height_, title_, NULL, NULL);

        if (window_ == NULL)
        {
            std::cerr << "Error while opening a GLFW3 window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window_);
        glfwSetWindowUserPointer(window_, this);
        glfwSetWindowSizeCallback(window_, setWindowSizeCallback_);

        monitor_ =  glfwGetPrimaryMonitor();
        glfwGetWindowSize(window_, &windowSize_[0], &windowSize_[1]);
        glfwGetWindowPos(window_, &windowPos_[0], &windowPos_[1]);

        // Setup Dear ImGui binding

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();

        const char* glslVersion = "#version 330 core";
        ImGui_ImplGlfw_InitForOpenGL(window_, true);
		ImGui_ImplOpenGL3_Init(glslVersion);
    }

    void Window::setUpdateViewport(bool update)
    {
        updateViewport_ = update;
    }

    void Window::swapBuffers()
    {
        glfwSwapBuffers(window_);
    }

    void Window::resize()
    {
        int width, height;
        glfwGetFramebufferSize(window_, &width, &height);
        glViewport(0, 0, width, height);
    }

    void Window::setInputMode(int mode, int value)
    {
        glfwSetInputMode(window_, mode, value);
    }

    void Window::shouldClose()
    {
        glfwSetWindowShouldClose(window_, true);
    }

    void Window::setFullscreen(bool fullscreen)
    {
        if (isFullscreen() == fullscreen)
            return;

        if (fullscreen)
        {
            // Backup window position and window size
            glfwGetWindowPos(window_, &windowPos_[0], &windowPos_[1] );
            glfwGetWindowSize(window_, &windowSize_[0], &windowSize_[1] );
            
            // Get resolution of monitor
            const GLFWvidmode * mode = glfwGetVideoMode(monitor_);

            // Switch to full screen
            glfwSetWindowMonitor(window_, monitor_, 0, 0, mode->width, mode->height, 0 );
        }
        else
        {
            // Restore last window size and position
            glfwSetWindowMonitor(window_, nullptr,  windowPos_[0], windowPos_[1], windowSize_[0], windowSize_[1], 0);
        }
    }

    void Window::update()
    {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        xPrev_ = x_;
        yPrev_ = y_;
        x_ = x;
        y_ = y;
    }

    bool Window::getShouldClose() const
    {
        return glfwWindowShouldClose(window_);
    }

    bool Window::isFullscreen() const
    {
        return glfwGetWindowMonitor(window_) != nullptr;
    }

    bool Window::isKeyPressed(int key) const
    {
        return glfwGetKey(window_, key) == GLFW_PRESS;
    }

    bool Window::isCursorMoving() const
    {
        double xRel = x_ - xPrev_;
        double yRel = y_ - yPrev_;

        return !(xRel == 0 && yRel == 0);
    }

    double Window::getX() const
    {
        return x_;
    }

    double Window::getY() const
    {
        return y_;
    }

    double Window::getXRel() const
    {
        return x_ - xPrev_;
    }

    double Window::getYRel() const
    {
        return y_ - yPrev_;
    }

    bool Window::getUpdateViewport() const
    {
        return updateViewport_;
    }
}