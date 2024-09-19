#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/log.h"
#include <BLEScan.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEAdvertisedDevice.h>
#include <chrono>

namespace esphome {
namespace exercise_sensor {

class ExerciseSensor : public sensor::Sensor, public PollingComponent {
  public:  
    void set_sensor1(sensor::Sensor *speed) { speed_ = speed; }
    void set_sensor2(sensor::Sensor *cadence) { cadence_ = cadence; }
    void set_sensor3(sensor::Sensor *power) { power_ = power; }
    void set_sensor4(sensor::Sensor *distance) { distance_ = distance; }
    void set_sensor5(sensor::Sensor *heart) { heart_ = heart; }

    void setup() override;
    void loop() override;
    void update() override;
    void dump_config() override;

    static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);

  protected:
    sensor::Sensor *speed_;
    sensor::Sensor *cadence_;
    sensor::Sensor *power_;
    sensor::Sensor *distance_;
    sensor::Sensor *heart_;

  private:

};

} //namespace empty_compound_sensor
} //namespace esphome