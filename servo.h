#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

// Define the GPIO pin connected to the servo
#define SERVO_PIN 2

// Initializes PWM for the servo
void init_pwm_servo(void);

// Convert angle (0° to 180°) to corresponding PWM value
uint16_t slope(float angulo);

// Move servo based on accelerometer axis values
float move_servo_for_axis(float acc_x, float acc_y, float acc_z);


#endif // SERVO_H