
#include "exercise_sensor.h"


namespace esphome {
namespace exercise_sensor {


static const char *TAG = "exercise_sensor.sensor";
static bool connected = false;
static bool doScan = true;
static bool doConnect = false;
static BLEUUID serviceUUID("1826"); // service UUID for Bowflex C6 bike
static BLEUUID charUUID("2ad2"); // characteristic UUID for BowFlex c6 Bike Data
static BLERemoteCharacteristic* istic;
static BLEAdvertisedDevice* btbike;

float myspeed = 0.0f;
float mycadence = 0.0f;
float mypower = 0.0f; 
float myheart = 80.0f;
float totalDistance = 0.0f;
float calories = 0.0f;

extern int user_weight;
extern int user_age;
extern int user_sex;
int weight = 100; 
int age = 30; 
int sex = 0; 

std::chrono::steady_clock::time_point lastTime;
float timePassed = 0.0f;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks 
{
    void onResult(BLEAdvertisedDevice advertisedDevice) 
    {
        // We have found a device, let us now see if it contains the service we are looking for.
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) 
        {
        BLEDevice::getScan()->stop();
        btbike = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
        doScan = false;
        } 
    } 
};

class MyClientCallback : public BLEClientCallbacks 
{
    void onConnect(BLEClient* pclient) 
    {
        //Serial.println("Connected to Bluetooth Bike");
        connected = true;
    }

    void onDisconnect(BLEClient* pclient) 
    {
        connected = false;
        doScan = true;
    }
};

void ExerciseSensor::update() {
    if (this->speed_ != nullptr)
      this->speed_->publish_state(myspeed);
    if (this->cadence_ != nullptr)
      this->cadence_->publish_state(mycadence);
    if (this->power_ != nullptr)
      this->power_->publish_state(mypower);
    if (this->distance_ != nullptr)
      this->distance_->publish_state(totalDistance);
    if (this->heart_ != nullptr)
      this->heart_->publish_state(myheart);
}

void ExerciseSensor::dump_config() {
    ESP_LOGCONFIG(TAG, "Exercise sensor");
}

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{
    //Serial.print("Notify callback for characteristic ");
    //Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    //Serial.print(" of data length ");
    //Serial.println(length);
    //Serial.print("data: ");
    uint16_t flags = (pData[1] << 8) | pData[0];
    uint16_t xspeed = (pData[3] << 8) | pData[2];
    uint16_t xcadence = (pData[5] << 8) | pData[4];
    uint16_t xpower = (pData[7] << 8) | pData[6];
    uint8_t xheartrate = pData[8];
    float speedo = (xspeed * 0.01 * 0.64419665777);
    myspeed = speedo;
    mycadence = xcadence;
    mypower = xpower;
    myheart = xheartrate;


    auto current_time = std::chrono::steady_clock::now();
    if (lastTime != std::chrono::steady_clock::time_point()) 
    {
      std::chrono::duration<float, std::ratio<3600>> time_span = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<3600>>>(current_time - lastTime);
      timePassed = time_span.count();
      totalDistance += speedo * timePassed;
      //calories += 



    }
    lastTime = current_time;
    // need to do some math and add up the total distance traveled. I want that tracked. 
    // i want heard rate tracked too, but maybe I should use another esp32? either that or bring in some switches to enable one or the other. hmmm
}

float calculateCalorieIncrement(float powerOutput, float heartRate, float timeIncrement,  bool userSex, int userAge, int userWeight)
{
    float output = 0.0f;

    /*
     calories_power = power_output * 3.6 * time_increment_hours

        # Heart Rate Method
        gender_factor = 0.86 if self.gender.lower() == 'female' else 1
        calories_hr = (
            (self.age * 0.2017)
            + (self.weight_kg * 0.09036)
            + (heart_rate * 0.6309)
            - 55.0969
        ) * gender_factor * (time_increment_hours * 60)  # Convert hours to minutes

        # Average of both methods
        calories_increment = (calories_power + calories_hr) / 2

        # Update totals
        self.total_calories += calories_increment
        self.total_time_hours += time_increment_hours

        return {
            "increment_calories": round(calories_increment, 4),
            "total_calories": round(self.total_calories, 2),
            "total_time_hours": round(self.total_time_hours, 4)
        }
    */

    return output;
}

void BTScan()
{
    // Retrieve a Scanner and set the callback we want to use to be informed when we
    // have detected a new device.  Specify that we want active scanning and start the
    // scan to run for 5 seconds.
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
}

bool ConnectToBike() 
{
    //Serial.print("Forming a connection to ");
    //Serial.println(btbike->getAddress().toString().c_str());
    BLEClient*  pClient  = BLEDevice::createClient();
    //Serial.println(" - Created client");
    pClient->setClientCallbacks(new MyClientCallback());
    // Connect to the remove BLE Server.
    pClient->connect(btbike);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    //Serial.println(" - Connected to server");
    pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) 
    {
        //Serial.print("Failed to find our service UUID: ");
        //Serial.println(serviceUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }
    //Serial.println(" - Found our service");

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    istic = pRemoteService->getCharacteristic(charUUID);
    if (istic == nullptr) 
    {
        //Serial.print("Failed to find our characteristic UUID: ");
        //Serial.println(charUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }
    //Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if(istic->canRead()) 
    {
        std::string value = istic->readValue();
        //Serial.print("The characteristic value was: ");
        //Serial.println(value.c_str());
    }

    if(istic->canNotify())
        istic->registerForNotify(notifyCallback);

    connected = true;
    return true;
}

void ExerciseSensor::setup() 
{
    BLEDevice::init("");
    weight = id(user_weight);
    age = id(user_age);
    sex = id(user_sex);
}

void ExerciseSensor::loop() 
{
    if (doConnect) 
        if (ConnectToBike()) 
        {
            //ESP_LOGD("Connected to Bowflex C6.");
            doConnect = false;
        } 
      
  if(doScan)
  {
      //ESP_LOGD("BT Scanning...");
      BTScan();
  }
}



} //namespace empty_compound_sensor
} //namespace esphome