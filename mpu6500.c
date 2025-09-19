#include "mpu6500.h"
#include "pico/stdlib.h"

// --- Registers and I2C address ---
#define MPU6500_ADDR     0x68 // Default I2C address when AD0 pin is at GND.
#define MPU6500_PWR_MGMT 0x6B // Power Management Recorder.
#define MPU6500_ACCEL_X  0x3B // Initial accelerometer data logger.

void mpu6500_init(i2c_inst_t* i2c) {
    // To "wake up" the MPU-6500, we need to write 0x00 to the register
    // power management (PWR_MGMT_1), disabling "sleep" mode.
    uint8_t buf[] = {MPU6500_PWR_MGMT, 0x00};
    i2c_write_blocking(i2c, MPU6500_ADDR, buf, 2, false);
}

void mpu6500_read_raw(i2c_inst_t* i2c, mpu6500_data_t* data) {
    uint8_t buffer[14];

    // We point to the first data register (ACCEL_XOUT_H at 0x3B).
    // The sensor will send the 14 bytes of data in sequence from this point onwards.
    uint8_t reg = MPU6500_ACCEL_X;
    i2c_write_blocking(i2c, MPU6500_ADDR, &reg, 1, true); // 'true' to keep the bus active.
    i2c_read_blocking(i2c, MPU6500_ADDR, buffer, 14, false);

    // Converts the read byte pairs (8 bits each) into 16-bit numbers.
    // The MPU-6500 sends data in "big-endian" format (most significant byte first).
    data->accel[0] = (buffer[0] << 8) | buffer[1];
    data->accel[1] = (buffer[2] << 8) | buffer[3];
    data->accel[2] = (buffer[4] << 8) | buffer[5];
    data->temp     = (buffer[6] << 8) | buffer[7];
    data->gyro[0]  = (buffer[8] << 8) | buffer[9];
    data->gyro[1]  = (buffer[10] << 8) | buffer[11];
    data->gyro[2]  = (buffer[12] << 8) | buffer[13];
}