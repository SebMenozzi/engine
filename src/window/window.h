#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <array>
// imgui
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "types.h"

namespace window
{
    class Window
    {
    public:
        Window(uint32 width, uint32 height, const char* title);

        void init();
        void setUpdateViewport(bool update);
        void swapBuffers();
        void resize();
        void setInputMode(int mode, int value);
        void shouldClose();
        void setFullscreen(bool fullscreen);
        void update();

        bool isKeyPressed(int key) const;
        bool isFullscreen() const;
        bool getShouldClose() const;
        bool isCursorMoving() const;
        double getX() const;
        double getY() const;
        double getXRel() const;
        double getYRel() const;
        bool getUpdateViewport() const;

    private:
        uint32 width_;
        uint32 height_;
        const char* title_;

        GLFWwindow* window_;
        GLFWmonitor* monitor_;

        double x_ = 0;
        double y_ = 0;
        double xPrev_ = 0;
        double yPrev_ = 0;

        std::array<int, 2> windowPos_ { 0, 0 };
        std::array<int, 2> windowSize_ { 0, 0 };

        bool updateViewport_ = true;
    };
}
