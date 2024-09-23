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
    void set_sensor6(sensor::Sensor *workdout_time) { workout_time_ = workdout_time; }
    void set_sensor7(sensor::Sensor *calories) { calories_ = calories; }
    void set_age(int user_age) {user_age_ = user_age; }
    void set_weight(int user_weight) {user_weight_ = user_weight; }
    void set_sex(int user_sex) {user_sex_ = user_sex; }

    void setup() override;
    void loop() override;
    void update() override;
    void dump_config() override;

    static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
    static float calculateCalorieIncrement(float powerOutput, float heartRate, float timeIncrement, int uage, int weight, int usex);

    int get_user_age() const { return user_age_; }
    int get_user_weight() const { return user_weight_; }
    int get_user_sex() const { return user_sex_; }

  protected:
    sensor::Sensor *speed_;
    sensor::Sensor *cadence_;
    sensor::Sensor *power_;
    sensor::Sensor *distance_;
    sensor::Sensor *heart_;
    sensor::Sensor *workout_time_;
    sensor::Sensor *calories_;
    int user_age_;
    int user_weight_;
    int user_sex_;

  private:

};

} //namespace empty_compound_sensor
} //namespace esphome