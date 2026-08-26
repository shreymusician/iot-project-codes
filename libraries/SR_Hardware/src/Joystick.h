#ifndef SR_JOYSTICK_H
#define SR_JOYSTICK_H

#include <Arduino.h>

class Joystick {

private:
    int xPin;
    int yPin;
    int buttonPin;

public:

    // Constructor
    Joystick(int xPin, int yPin, int buttonPin) {
        this->xPin = xPin;
        this->yPin = yPin;
        this->buttonPin = buttonPin;
    }

    // Initialize joystick
    void begin() {
        pinMode(buttonPin, INPUT_PULLUP);
        analogReadResolution(12);
    }

    // Read joystick and return its current state
    String getState() {

        int x = analogRead(xPin);
        int y = analogRead(yPin);
        int button = digitalRead(buttonPin);

        if (x == 4095) {
            return "FORWARD";
        }

        else if (x == 0) {
            return "BACKWARD";
        }

        else if (y == 0) {
            return "LEFT";
        }

        else if (y == 4095) {
            return "RIGHT";
        }

        else if (button == 0) {
            return "PRESSED";
        }

        else {
            return "NORMAL";
        }
    }
};

#endif