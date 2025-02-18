
//
// Constants.h
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

class Constants
{
    public:
    //static const bool LOW = false;
    //static const bool HIGH = true;

    static const byte MOTOR1_A = 2;
    static const byte MOTOR1_B = 3;
    static const byte MOTOR2_A = 1;
    static const byte MOTOR2_B = 4;
    static const byte MOTOR4_A = 0;
    static const byte MOTOR4_B = 6;
    static const byte MOTOR3_A = 5;
    static const byte MOTOR3_B = 7;

    static const byte FORWARD = 1;
    static const byte BACKWARD = 2;
    static const byte BRAKE = 3;
    static const byte RELEASE = 4;

    static const byte SINGLE = 1;
    static const byte DOUBLE = 2;
    static const byte INTERLEAVE = 3;
    static const byte MICROSTEP = 4;
};

#endif

