#include "barometer.hpp"
#include "esp_log.h"

Barometer::Barometer(){
    ESP_LOGI("BARO", "in init baro");
    generate_rocket_profile_800m(2048);
    ESP_LOGI("BARO", "fine barometer");
}
void Barometer::generate_rocket_profile_800m( int count) {
    for (int i = 0; i < count; i++) {
        double t = (double)i / (count - 1); // Normalized flight time (0.0 to 1.0)
        double altitude_m = 0.0;

        // 1. Pre-Launch Pad State (0% - 10% of total time)
        if (t < 0.10) {
            altitude_m = 0.0;
        }
        // 2. Powered Ascent & Coast to Apogee (10% - 45% of total time)
        else if (t < 0.45) {
            double phase = (t - 0.10) / 0.35; // 0.0 to 1.0
                                              // Smooth parabolic ascent up to 800m peak
            altitude_m = 800.0 * sin(phase * (M_PI / 2.0));
        }
        // 3. Apogee / Parachute Deployment (45% - 50% of total time)
        else if (t < 0.50) {
            altitude_m = 800.0; 
        }
        // 4. Descent under Parachute (50% - 92% of total time)
        else if (t < 0.92) {
            double phase = (t - 0.50) / 0.42; // 0.0 to 1.0
                                              // Linear descent back to ground level (0m)
            altitude_m = 800.0 * (1.0 - phase);
        }
        // 5. Post-Landing / Ground (92% - 100% of total time)
        else {
            altitude_m = 0.0;
        }

        // --- Physics Calculations based on Altitude ---

        // Temperature drop: Standard troposphere lapse rate (6.5 °C per 1000m)
        double temp_c = T0 - (6.5 * (altitude_m / 1000.0));

        // Barometric formula for low altitude: P = P0 * (1 - L*h/T0)^(g*M/(R*L))
        // Simplified barometric approximation for <1km: ΔP ≈ -0.012 kPa per meter
        double press_kpa = P0 * pow(1.0 - (0.0065 * altitude_m / 293.15), 5.255);

        // Humidity drop with altitude (slight drop up to 800m)
        double hum_pct = RH0 - (altitude_m * 0.005); 
        if (hum_pct < 0.0) hum_pct = 0.0;

        // Add subtle sensor fluctuation/noise
        double noise = sin(i * 0.4) * 0.02;

        data[i].temperature = temp_c + noise;
        data[i].pressure    = press_kpa + (noise * 0.01);
        data[i].humidity    = hum_pct;
    }
}

esp_err_t Barometer:: read_data(){
    cnt = (cnt +1)%2048 ; //per evitare buffer overflow
    return ESP_OK;
}
