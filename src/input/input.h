#pragma once

#include <SDL2/SDL.h>

class Input
{
public:
    Input();
    ~Input();

    void updateEvents();
    bool isQuit() const;
    bool getKey(const SDL_Scancode key) const;
    bool getButton(const Uint8 button) const;
    bool isCursorMoving() const;
    int getX() const;
    int getY() const;
    int getXRel() const;
    int getYRel() const;
    void displayCursor(bool state) const;
    void catchCursor(bool state) const;

private:
    SDL_Event events;
    bool keys[SDL_NUM_SCANCODES];
    bool buttons[8];

    int x;
    int y;
    int xRel;
    int yRel;

    bool quit;
};
