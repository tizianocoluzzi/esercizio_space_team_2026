#ifndef BAROMETER_HPP
#define BAROMETER_HPP

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>

#define P0 101.325   // kPa
#define T0 20.0      // °C (293.15 K)
#define RH0 50.0     // % RH

class Barometer {
    public:

        struct baro_data_t{
            double temperature;
            double pressure;   // in kPa
            double humidity;
        };

        Barometer();

        esp_err_t read_data();
        baro_data_t get_data() const{
            return data[cnt];
        }

    private: 
        baro_data_t data[2048]; 

// Generates a 2048-sample profile for a rocket reaching an 800m apogee
        void generate_rocket_profile_800m( int count); 
        int cnt = 0;

};

#endif // COMMANDER_HPP
