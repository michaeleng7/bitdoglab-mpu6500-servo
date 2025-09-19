// Libraries padrão e específicas do Raspberry Pi Pico
#include <stdio.h>
#include "pico/stdlib.h"        // Basic I/O functions
#include "hardware/pwm.h"       // PWM control
#include "servo.h"              // File of servo.h
#include <math.h>               // Mathematical functions like atan2f, sqrtf

// Define value of PI if not defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Convert an angle (0° to 180°) to corresponding PWM value
uint16_t slope(float angle) {
    float pulso = 0.5f + (angle / 180.0f) * 2.0f; // Calculates pulse width in ms
    return (uint16_t)((pulso / 20.0f) * 20000.0f); // Convert to PWM value (0–20000)
}

// Inicializa o PWM no pino do servo
void init_pwm_servo() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM); // Define function as PWM on the pin
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN); // Gets the PWM slice for the pin
    pwm_config config = pwm_get_default_config(); // Carrys default config
    pwm_config_set_clkdiv(&config, 125.0f);       // Define divisor of clock
    pwm_config_set_wrap(&config, 20000);          // Define cicle of PWM (20ms)
    pwm_init(slice_num, &config, true);           // Initializes PWM with config
}

// Move servo based on accelerometer axis values
float move_servo_for_axis(float acc_x, float acc_y, float acc_z) {
    static float last_angle = -1; // stores the last used angle

    // Calculates the tilt angle based on axes
    float raw_angle = atan2f(acc_x, sqrtf(acc_y * acc_y + acc_z * acc_z)) * (180.0f / M_PI);
    float angle = 90.0f + raw_angle; // Adjusts to range 0° to 180°

    // Guarantees that the angle stays within valid limits
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    // Rounds the angle to discrete ranges (each range ≈ 5.625°)
    int range = angle / 5.625f;
    switch (range) {
        case 0: angle = 0; break;
        case 1: angle = 10; break;
        case 2: angle = 15; break;
        case 3: angle = 20; break;
        case 4: angle = 25; break;
        case 5: angle = 30; break;
        case 6: angle = 35; break;
        case 7: angle = 40; break;
        case 8: angle = 45; break;
        case 9: angle = 50; break;
        case 10: angle = 55; break;
        case 11: angle = 65; break;
        case 12: angle = 75; break;
        case 13: angle = 80; break;
        case 14: angle = 85; break;
        case 15: angle = 90; break;
        case 16: angle = 95; break;
        case 17: angle = 100; break;
        case 18: angle = 105; break;
        case 19: angle = 110; break;
        case 20: angle = 115; break;
        case 21: angle = 120; break;
        case 22: angle = 125; break;
        case 23: angle = 130; break;
        case 24: angle = 135; break;
        case 25: angle = 140; break;
        case 26: angle = 145; break;
        case 27: angle = 150; break;
        case 28: angle = 155; break;
        case 29: angle = 160; break;
        case 30: angle = 165; break;
        case 31: angle = 170; break;
        case 32: angle = 175; break;
        default: angle = 180; break;
    }

    // Define the movement direction based on the Y axis
    float direction = (acc_y > 0.5f || acc_y < -0.5f) ? -1.0f : 1.0f;

    // Calculates the movement step of the servo
    float step = ((angle > last_angle) ? 1 : -1) * direction;

    // If it's the first run, set the last angle
    if (last_angle < 0) last_angle = angle;

    // Move the servo gradually to the new angle
    if (angle != last_angle) {
        for (float a = last_angle + step; step > 0 ? a <= angle : a >= angle; a += step) {
            pwm_set_gpio_level(SERVO_PIN, slope(a)); // Aplicate PWM
            sleep_ms(10); // Delay for smooth movement
        }
        last_angle = angle; // Updates last angle
    }
    return angle; // Returns the final angle
}