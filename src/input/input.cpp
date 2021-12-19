#include "input.h"

namespace input
{
    Input::Input():
        x_(0),
        y_(0),
        xRel_(0),
        yRel_(0),
        quit_(false)
    {
        for (uint16 i = 0; i < SDL_NUM_SCANCODES; ++i)
            this->keys_[i] = false;

        for (uint8 i = 0; i < 8; ++i)
            this->buttons_[i] = false;
    }

    void Input::updateEvents()
    {
        xRel_ = 0;
        yRel_ = 0;

        while (SDL_PollEvent(&events_))
        {
            switch (events_.type) {
                case SDL_KEYDOWN:
                    keys_[events_.key.keysym.scancode] = true;
                    break;

                case SDL_KEYUP:
                    keys_[events_.key.keysym.scancode] = false;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    buttons_[events_.button.button] = true;
                    break;

                case SDL_MOUSEBUTTONUP:
                    buttons_[events_.button.button] = false;
                    break;

                case SDL_MOUSEMOTION:
                    x_ = events_.motion.x;
                    y_ = events_.motion.y;

                    xRel_ = events_.motion.xrel;
                    yRel_ = events_.motion.yrel;
                    break;

                case SDL_WINDOWEVENT:
                    if (events_.window.event == SDL_WINDOWEVENT_CLOSE)
                        quit_ = true;
                    break;

                default:
                    break;
            }
        }
    }

    bool Input::hasQuit() const
    {
        return quit_;
    }

    bool Input::getKey(const SDL_Scancode key) const
    {
        return keys_[key];
    }

    bool Input::getButton(const Uint8 button) const
    {
        return buttons_[button];
    }

    bool Input::isCursorMoving() const
    {
        return !(xRel_ == 0 && yRel_ == 0);
    }

    int Input::getX() const
    {
        return x_;
    }

    int Input::getY() const
    {
        return y_;
    }

    int Input::getXRel() const
    {
        return xRel_;
    }

    int Input::getYRel() const
    {
        return yRel_;
    }

    void Input::displayCursor(bool state) const
    {
        if (state)
            SDL_ShowCursor(SDL_ENABLE);
        else
            SDL_ShowCursor(SDL_DISABLE);
    }

    void Input::catchCursor(bool state) const
    {
        if (state)
            SDL_SetRelativeMouseMode(SDL_TRUE);
        else
            SDL_SetRelativeMouseMode(SDL_FALSE);
    }
}