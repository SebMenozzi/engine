#pragma once

#include <SDL.h>

#include "types.h"

namespace input
{
    class Input
    {
        public:
            Input();
            void updateEvents();
            bool hasQuit() const;
            bool getKey(const SDL_Scancode key) const;
            bool getButton(const uint8 button) const;
            bool isCursorMoving() const;
            int getX() const;
            int getY() const;
            int getXRel() const;
            int getYRel() const;
            void displayCursor(bool state) const;
            void catchCursor(bool state) const;

        private:
            SDL_Event events_;
            bool keys_[SDL_NUM_SCANCODES];
            bool buttons_[8];

            int x_;
            int y_;
            int xRel_;
            int yRel_;

            bool quit_;
    };
}