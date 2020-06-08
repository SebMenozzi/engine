#include "input.h"

Input::Input()
{
    this->x = 0;
    this->y = 0;

    this->xRel = 0;
    this->yRel = 0;

    this->quit = false;

    for(int i = 0; i < SDL_NUM_SCANCODES; ++i)
        this->keys[i] = false;

    for(int i = 0; i < 8; ++i)
        this->buttons[i] = false;
}

void Input::updateEvents()
{
    this->xRel = 0;
    this->yRel = 0;

    while(SDL_PollEvent(&this->events))
    {
        switch(this->events.type)
        {
            case SDL_KEYDOWN:
                this->keys[this->events.key.keysym.scancode] = true;
                break;

            case SDL_KEYUP:
                this->keys[this->events.key.keysym.scancode] = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                this->buttons[this->events.button.button] = true;
                break;

            case SDL_MOUSEBUTTONUP:
                this->buttons[this->events.button.button] = false;
                break;

            case SDL_MOUSEMOTION:
                this->x = this->events.motion.x;
                this->y = this->events.motion.y;

                this->xRel = this->events.motion.xrel;
                this->yRel = this->events.motion.yrel;
                break;

            case SDL_WINDOWEVENT:
                if(this->events.window.event == SDL_WINDOWEVENT_CLOSE)
                    this->quit = true;
                break;

            default:
                break;
        }
    }
}

bool Input::isQuit() const
{
    return this->quit;
}

bool Input::getKey(const SDL_Scancode key) const
{
    return this->keys[key];
}

bool Input::getButton(const Uint8 button) const
{
    return this->buttons[button];
}

bool Input::isCursorMoving() const
{
    return !(this->xRel == 0 && this->yRel == 0);
}

int Input::getX() const
{
    return this->x;
}

int Input::getY() const
{
    return this->y;
}

int Input::getXRel() const
{
    return this->xRel;
}

int Input::getYRel() const
{
    return this->yRel;
}

void Input::displayCursor(bool state) const
{
    if(state)
        SDL_ShowCursor(SDL_ENABLE);
    else
        SDL_ShowCursor(SDL_DISABLE);
}

void Input::catchCursor(bool state) const
{
    if(state)
        SDL_SetRelativeMouseMode(SDL_TRUE);
    else
        SDL_SetRelativeMouseMode(SDL_FALSE);
}
