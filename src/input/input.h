#pragma once

#include <SDL2/SDL.h>

class Input
{
public:
    /**
     * @brief Define the Input object
     * @param void
     * @return void
    */
    Input();

    /**
     * @brief Update the Input object according SDL events
     * @param void
     * @return void
    */
    void updateEvents();

    /**
     * @brief Retrieve the boolean quit state of the Input object,
     * "true" if the window manager requests that the window be closed
     * @param void
     * @return isQuit
    */
    bool isQuit() const;

    /**
     * @brief Retrieve the boolean key state from the keys array,
     * "true" if the key is being pressed
     * @param key
     * @return key state
    */
    bool getKey(const SDL_Scancode key) const;

    /**
     * @brief Retrieve the boolean button state from the buttons array,
     * "true" if the button is being pressed
     * @param key
     * @return key state
    */
    bool getButton(const Uint8 button) const;

    /**
     * @brief Retrieve the boolean that is "true" if the cursor is moving
     * @param void
     * @return boolean
    */
    bool isCursorMoving() const;

    /**
     * @brief Retrieve the X coordinate, relative to window
     * @param void
     * @return int
    */
    int getX() const;

    /**
     * @brief Retrieve the Y coordinate, relative to window
     * @param void
     * @return int
    */
    int getY() const;

    /**
     * @brief Retrieve the relative motion in the X direction
     * @param void
     * @return y
    */
    int getXRel() const;

    /**
     * @brief Retrieve the relative motion in the Y direction
     * @param void
     * @return x relative
    */
    int getYRel() const;

    /**
     * @brief Display/Hide the cursor
     * @param boolean
     * @return y relative
    */
    void displayCursor(bool state) const;

    /**
     * @brief Enable or not that the cursor can be displayed
     * when it is not anymore pointing at the window
     * @param boolean
     * @return void
    */
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
