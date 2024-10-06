#include "light_sensor.h"
#include <BH1750.h>
#include <Wire.h>

BH1750 lightMeter;

void initLightSensor() {
    Wire.begin();
    lightMeter.configure(BH1750::CONTINUOUS_LOW_RES_MODE);
    lightMeter.begin();
}

int readlight() {
    // https://github.com/claws/BH1750
    // Low Resolution Mode    - (4 lx precision, 16ms measurement time)
    // High Resolution Mode   - (1 lx precision, 120ms measurement time)
    // High Resolution Mode 2 - (0.5 lx precision, 120ms measurement time)
    // lightMeter.configure(BH1750::ONE_TIME_HIGH_RES_MODE);
    // lightMeter.configure(BH1750::CONTINUOUS_HIGH_RES_MODE);

    while (!lightMeter.measurementReady(true)) {
        yield();
    }
    return (int)lightMeter.readLightLevel();
}
