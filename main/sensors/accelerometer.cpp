
#include "accelerometer.hpp"
#include "esp_log.h"

Accelerometer::Accelerometer() {
    ESP_LOGI("ACCEL", "in init accelerometer/gyroscope");
    generate_rocket_profile_800m(2048);
    ESP_LOGI("ACCEL", "fine accelerometer/gyroscope");
}

void Accelerometer::generate_rocket_profile_800m(int count) {
    // Keep writes within the fixed-size buffer.
    if (count < 2) return;
    if (count > kSampleCount) count = kSampleCount;

    for (int i = 0; i < count; ++i) {
        const float t = static_cast<float>(i) / (count - 1);
        float ax = 0.0f, ay = 0.0f, az = 0.0f;
        float gx = 0.0f, gy = 0.0f, gz = 0.0f;

        // 1. Pre-launch, 0%-10%: motionless on the launch pad.
        // An accelerometer measures +1g along the upward-pointing Z axis.
        if (t < 0.10f) {
            az = kGravity;
        }
        // 2. Ascent, 10%-45%: powered launch followed by ballistic coast.
        else if (t < 0.45f) {
            const float phase = (t - 0.10f) / 0.35f;
            constexpr float kBurnFraction = 0.20f;

            if (phase < kBurnFraction) {
                // Motor burning: strong upward specific force and vibration.
                const float burn = phase / kBurnFraction;
                const float thrust = sinf(kPi * burn); // smooth thrust pulse
                az = kGravity + 55.0f * thrust;
                ax = 1.6f * thrust * sinf(i * 0.21f);
                ay = 1.4f * thrust * cosf(i * 0.18f);
                gx = 2.0f * sinf(burn * 5.0f * kPi);
                gy = 2.5f * cosf(burn * 4.0f * kPi);
                gz = 8.0f * thrust; // slight axial rotation
            } else {
                // Ballistic coast: near-weightlessness in the rocket frame.
                // Gravity accelerates the rocket but is not measured as
                // specific force by an ideal accelerometer.
                az = 0.0f;
                gx = 0.7f * sinf(i * 0.025f);
                gy = 0.6f * cosf(i * 0.022f);
                gz = 1.0f;
            }
        }
        // 3. Apogee, 45%-50%: still in near-free-fall before deployment.
        else if (t < 0.50f) {
            az = 0.0f;
            gx = 0.5f * sinf(i * 0.025f);
            gy = 0.5f * cosf(i * 0.022f);
            gz = 0.5f;
        }
        // 4. Parachute descent, 50%-92%: opening shock, then swaying.
        else if (t < 0.92f) {
            const float phase = (t - 0.50f) / 0.42f;
            const float opening = expf(-phase * 65.0f);

            az = kGravity + 40.0f * opening
                 + 0.35f * sinf(phase * 12.0f * kPi);
            ax = 0.8f * sinf(phase * 10.0f * kPi);
            ay = 0.7f * cosf(phase * 9.0f * kPi);

            gx = 65.0f * opening + 7.0f * sinf(phase * 10.0f * kPi);
            gy = 40.0f * opening + 6.0f * cosf(phase * 9.0f * kPi);
            gz = 10.0f * opening + 2.0f * sinf(phase * 7.0f * kPi);
        }
        // 5. Landing, 92%-100%: touchdown pulse, then at rest.
        else {
            const float since_landing = t - 0.92f;
            const float impact = expf(-since_landing * 500.0f);

            az = kGravity + 75.0f * impact;
            ax = 12.0f * impact;
            ay = -8.0f * impact;
            gx = 30.0f * impact;
            gy = -20.0f * impact;
            gz = 8.0f * impact;
        }

        // Small deterministic sensor noise (no RNG required).
        const float n1 = sinf(i * 0.40f);
        const float n2 = cosf(i * 0.31f);
        const float n3 = sinf(i * 0.27f + 1.0f);

        data[i].accel_x = ax + 0.025f * n1;
        data[i].accel_y = ay + 0.025f * n2;
        data[i].accel_z = az + 0.035f * n3;
        data[i].gyro_x  = gx + 0.06f * n2;
        data[i].gyro_y  = gy + 0.06f * n3;
        data[i].gyro_z  = gz + 0.06f * n1;
    }
}

esp_err_t Accelerometer::read_data() {
    cnt = (cnt + 1) % kSampleCount; // wrap around the 2048-sample buffer
    return ESP_OK;
}

