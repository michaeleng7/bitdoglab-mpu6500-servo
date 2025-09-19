// Libraries default and specific for calculations and communication
#include <stdio.h>              // Input and output functions 
#include <math.h>               // Mathematical functions like atan2f, sqrtf
#include "pico/stdlib.h"        // Basic I/O functions
#include "hardware/i2c.h"       // Lib for comunication I2C
#include "mpu6500.h"            //Lib for MPU6500 sensor
#include "lib/ssd1306.h"        // Control for display OLED SSD1306
#include "lib/ssd1306_fonts.h"  // Fonts for OLED display

#include "servo.h"              // Lib for servo control

// Definitions for I2C pins
#define I2C_PORT i2c0           // Uses the I2C0 port
#define I2C_SDA 0               // Pin GPIO 0 as SDA
#define I2C_SCL 1               // Pin GPIO 1 as SCL

int main() {
    // Initialization of I2C, MPU6500, Servo, and OLED Display 
    stdio_init_all();
    while (!stdio_usb_connected()) sleep_ms(100); // Waits for USB connection
    printf("System Ready: MPU6500 + Servo\n");

    // Initialization of I2C barrier with 400kHz
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); 
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); 
    gpio_pull_up(I2C_SDA); 
    gpio_pull_up(I2C_SCL);

    // Initialization of OLED Display
    ssd1306_Init();
    ssd1306_Fill(Black);        // Clears the screen
    ssd1306_UpdateScreen();     // Updates the screen

    // Initializes the MPU6500 sensor
    mpu6500_init(I2C_PORT);


    // Initializes the servo motor
    init_pwm_servo();

    char buffer[32]; // Buffer to format strings for display

    // Main loop
    while (1) {
        mpu6500_data_t data; // Struct to store sensor data
        mpu6500_read_raw(I2C_PORT, &data); // Reads raw accelerometer data

        // Converts raw data to g (gravity) values
        float acc_x = data.accel[0] / 16384.0f;
        float acc_y = data.accel[1] / 16384.0f;
        float acc_z = data.accel[2] / 16384.0f;


        // Move the servo based on detected tilt
        float angle = move_servo_for_axis(acc_x, acc_y, acc_z);


    // Decides direction based on axis values
    const char* direction;
    if (acc_x > 0.5f) direction = "Right";       
    else if (acc_x < -0.5f) direction = "Left";
    else if (acc_y > 0.5f) direction = "Front";
    else if (acc_y < -0.5f) direction = "Back";
    else if (acc_z > 0.5f) direction = "Up";
    else if (acc_z < -0.5f) direction = "Down";
    else direction = "Neutral"; // Without significant movement


     // Prints data to the terminal
     printf("Acc X: %.2f | Y: %.2f | Z: %.2f | Servo: %.0f° | Direction: %s\n",
         acc_x, acc_y, acc_z, angle, direction);


    // Updates the OLED display with the data
    ssd1306_Fill(Black); // Clears screen

    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("MPU/SERVO Monitor", Font_7x10, White);

    ssd1306_SetCursor(0, 12);
    snprintf(buffer, sizeof(buffer), "Servo: %.0f degrees", angle);
    ssd1306_WriteString(buffer, Font_7x10, White);

    snprintf(buffer, sizeof(buffer), "X: %.2f", acc_x);
    ssd1306_SetCursor(0, 24);
    ssd1306_WriteString(buffer, Font_7x10, White);

    snprintf(buffer, sizeof(buffer), "Y: %.2f Dir: %s", acc_y, direction);
    ssd1306_SetCursor(0, 36);
    ssd1306_WriteString(buffer, Font_7x10, White);

    snprintf(buffer, sizeof(buffer), "Z: %.2f", acc_z);
    ssd1306_SetCursor(0, 48);
    ssd1306_WriteString(buffer, Font_7x10, White);

    ssd1306_UpdateScreen(); // Updates the screen with new data

        sleep_ms(500); // Waits half a second before the next reading
    }

    return 0; 
}