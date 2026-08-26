#ifndef CAMROBO_H
#define CAMROBO_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <Joystick.h>

class CamRobo {

private:

    // -------------------------
    // JOYSTICK
    // -------------------------

    Joystick joy;


    // -------------------------
    // SERVOS
    // -------------------------

    Servo hser;
    Servo vser;


    // -------------------------
    // SERVO PINS
    // -------------------------

    int hpin;
    int vpin;


    // -------------------------
    // CURRENT POSITIONS
    // -------------------------

    int pha;
    int pva;


    // -------------------------
    // SERVO LIMITS
    // -------------------------

    int hl = 0;
    int hr = 180;

    int vu = 0;
    int vd = 120;


    // -------------------------
    // MOVEMENT SPEED
    // -------------------------

    const int del = 10;

    unsigned long lastMoveTime = 0;


public:

    // -------------------------
    // CONSTRUCTOR
    // -------------------------

    CamRobo(int horizontalPin, int verticalPin)
        : joy(34, 35, 25) {

        hpin = horizontalPin;
        vpin = verticalPin;

        pha = 90;
        pva = 90;
    }


    // -------------------------
    // INITIALIZE
    // -------------------------

    void begin() {

        joy.begin();

        hser.attach(hpin);
        vser.attach(vpin);

        reset();
    }


    // -------------------------
    // MAIN CONTROL
    // -------------------------

    void update() {

        String state = joy.getState();

        unsigned long currentTime = millis();


        // Don't move faster than our fixed delay
        if (currentTime - lastMoveTime < del) {
            return;
        }

        lastMoveTime = currentTime;


        // -------------------------
        // FORWARD
        // -------------------------

        if (state == "FORWARD") {

            if (pva > vu) {

                pva--;

                vser.write(pva);
            }
        }


        // -------------------------
        // BACKWARD
        // -------------------------

        else if (state == "BACKWARD") {

            if (pva < vd) {

                pva++;

                vser.write(pva);
            }
        }


        // -------------------------
        // LEFT
        // -------------------------

        else if (state == "LEFT") {

            if (pha > hl) {

                pha--;

                hser.write(pha);
            }
        }


        // -------------------------
        // RIGHT
        // -------------------------

        else if (state == "RIGHT") {

            if (pha < hr) {

                pha++;

                hser.write(pha);
            }
        }


        // -------------------------
        // NORMAL
        // -------------------------

        else if (state == "NORMAL") {

            // DO NOTHING
            // Servo remains at its current position.
        }


        // -------------------------
        // PRESSED
        // -------------------------

        else if (state == "PRESSED") {

            // Reserved for future functionality.
            reset();
        }
    }


    // -------------------------
    // RESET TO CENTER
    // -------------------------

    void reset() {

        pha = 90;
        pva = 90;

        hser.write(pha);
        vser.write(pva);
    }


    // -------------------------
    // HORIZONTAL SWEEP
    // -------------------------

    void hspan() {

        reset();

        for (int i = hl; i <= hr; i++) {

            pha = i;

            hser.write(i);

            delay(del);
        }


        for (int i = hr; i >= hl; i--) {

            pha = i;

            hser.write(i);

            delay(del);
        }
    }


    // -------------------------
    // VERTICAL SWEEP
    // -------------------------

    void vspan() {

        reset();

        for (int i = vu; i <= vd; i++) {

            pva = i;

            vser.write(i);

            delay(del);
        }


        for (int i = vd; i >= vu; i--) {

            pva = i;

            vser.write(i);

            delay(del);
        }
    }


    // -------------------------
    // GET HORIZONTAL ANGLE
    // -------------------------

    int getHorizontalAngle() {

        return pha;
    }


    // -------------------------
    // GET VERTICAL ANGLE
    // -------------------------

    int getVerticalAngle() {

        return pva;
    }
};

#endif