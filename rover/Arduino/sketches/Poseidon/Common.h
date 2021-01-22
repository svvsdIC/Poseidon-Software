#pragma once

/*
A shared header to ensure that the surface and the rover
both use a single common definition of values used for
communication.
*/


/* TODO: define enum for joystick commands:
    TRANSX, TRANSY, ... etc ...
*/

enum inputChannels {
    TRANSX, 
    TRANSY,
    TRANSZ,
    YAW,
    ROLL,
    PITCH
};
