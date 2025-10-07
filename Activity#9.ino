#include <dht.h>

/*
DHTRead is a singleton class.
It receives a pin to be read and a reference to a dht object.
The shouldReadSensor() function ensures calling the getter methods will not trigger 
the readSensor() function faster than the interval defined by readInterval.
Returns the temperature and humidity separately.
*/
class DHTRead {
private:
  static DHTRead* instance;
  const byte pin;
  dht &sensor;
  int temperature;
  int humidity;
  unsigned long lastReadTime;
  const unsigned long readInterval = 2000; // Reads every two seconds

  DHTRead(byte p, dht& dhtRef) : pin(p), sensor(dhtRef), temperature(0), humidity(0), lastReadTime(0) {}

  void readSensor() {
    sensor.read11(pin);
    temperature = sensor.temperature;
    humidity = sensor.humidity;
    lastReadTime = millis();
  }

  bool shouldReadSensor() {
    return (millis() - lastReadTime) >= readInterval;
  }

public:
  DHTRead(const DHTRead&) = delete;
  DHTRead& operator=(const DHTRead&) = delete;

  static DHTRead* getInstance(byte p, dht& dhtRef) {
    if (instance == nullptr) {
      instance = new DHTRead(p, dhtRef);
    }
    return instance;
  }

  static DHTRead* getInstance() {
    return instance;
  }

  static void deleteInstance() {
    if (instance != nullptr) {
      delete instance;
      instance = nullptr;
    }
  }

  int getTemperature() {
    if (shouldReadSensor()) {
      readSensor();
    }
    return temperature;
  }

  int getHumidity() {
    if (shouldReadSensor()) {
      readSensor();
    }
    return humidity;
  }
};

// Initializes static member
DHTRead* DHTRead::instance = nullptr;

/*
Handles the serial prints:
*/
class SerialMonitorHandler {
public:
  SerialMonitorHandler() {}

  void printSensorValues(int temp, int humidity) {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.println("-------------------");
  }
};

/*
dht DHT creates an object of the DHT class from the dht.h library.
The read11() function from the library populates its attributes with float values
of temperature in °C and humidity in relative percentage.
*/
dht DHT;
SerialMonitorHandler serialMonitor;

void setup() {
  Serial.begin(9600);
  DHTRead::getInstance(8, DHT); // Initializes the singleton instance
}

void loop() {
  delay(2000);
  DHTRead* dhtRead = DHTRead::getInstance(); // Ensures there is only one instance of the DHTRead class
  serialMonitor.printSensorValues(dhtRead->getTemperature(), dhtRead->getHumidity());
}