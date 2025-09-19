#ifndef MPU6500_H
#define MPU6500_H

#include <stdint.h>
#include "hardware/i2c.h"

// Structure to hold raw sensor data
typedef struct {
    int16_t accel[3]; // Aceleration X, Y, Z
    int16_t gyro[3];  // Gyroscope X, Y, Z
    int16_t temp;     // Temperature
} mpu6500_data_t;

// Function prototypes
void mpu6500_init(i2c_inst_t* i2c_port);
void mpu6500_read_raw(i2c_inst_t* i2c_port, mpu6500_data_t* data);

#endif