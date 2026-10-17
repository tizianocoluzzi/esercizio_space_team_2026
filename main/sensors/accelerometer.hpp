#ifndef ACCELEROMETER_HPP
#define ACCELEROMETER_HPP

#include "esp_err.h"
#include <math.h>

#define kGravity 9.80665f  // m/s^2
#define kPi 3.14159265358979323846f


class Accelerometer {
public:
    struct accel_data_t {
        double accel_x;
        double accel_y;
        double accel_z;

        double gyro_x;
        double gyro_y;
        double gyro_z;
    };

    Accelerometer();

    esp_err_t read_data();

    accel_data_t get_data() const {
        return data[cnt];
    }

private:
    static constexpr int kSampleCount = 2048;
    accel_data_t data[kSampleCount];

    // Generates a synthetic 2048-sample flight reaching an 800 m apogee.
    void generate_rocket_profile_800m(int count);

    int cnt = 0;
};

#endif // ACCELEROMETER_HPP

