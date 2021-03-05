#pragma once

/*
A shared header to ensure that the surface and the rover
both use a single common definition of values used for
communication.
*/

/*
Input channels for surface and rover 
*/
enum inputChannels {
    TRANSX = 0, 
    TRANSY,
    TRANSZ,
    YAW,
    ROLL,
    PITCH
};
