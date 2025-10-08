#include <dht.h>

/*
The DHTRead class returns the temperature and humidity separately.
It receives a pin to be read and a reference to a dht object.
The shouldReadSensor() function ensures calling the getter methods will not trigger 
the readSensor() function faster than the interval defined by readInterval.
*/
class DHTRead {
private:
  const byte pin;
  dht &sensor;
  int temperature;
  int humidity;
  unsigned long lastReadTime;
  const unsigned long readInterval = 2000; // Reads every two seconds

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
  DHTRead(byte p, dht& dhtRef) : pin(p), sensor(dhtRef), temperature(0), humidity(0), lastReadTime(0) {}
  
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

  void printState(byte state) {
    Serial.print("State: ");
    if(state == 1) Serial.println("green");
    if(state == 2) Serial.println("yellow");
    if(state == 3) Serial.println("red");
  }
};

/*
Handles which light should be on in the stacklight.
*/
class StackLight {
private:
  byte Rpin, Ypin, Gpin;
  unsigned long previousBlinkTime = 0;
  const unsigned long blinkInterval = 500; // 500ms blink interval

public:
  StackLight(byte r, byte y, byte g) : Rpin(r), Ypin(y), Gpin(g) {}

  void begin() {
    pinMode(Rpin, OUTPUT);
    pinMode(Ypin, OUTPUT);
    pinMode(Gpin, OUTPUT);
  }

  void setRedLightOn() {
    digitalWrite(Ypin, LOW);
    digitalWrite(Gpin, LOW);
    
    // Handle blinking
    unsigned long currentTime = millis();
    if (currentTime - previousBlinkTime >= blinkInterval) {
      previousBlinkTime = currentTime;
      digitalWrite(Rpin, !digitalRead(Rpin)); // Toggle red LED
    }
  }

  void setYellowLightOn() {
    digitalWrite(Rpin, LOW);
    digitalWrite(Ypin, HIGH);
    digitalWrite(Gpin, LOW);
  }

  void setGreenLightOn() {
    digitalWrite(Rpin, LOW);
    digitalWrite(Ypin, LOW);
    digitalWrite(Gpin, HIGH);
  }
};

/*
Handles the states of the system's operations 
based on the temperature and humidity:
*/
class StateHandler {
private:
  SerialMonitorHandler &serialMonitor;
  StackLight &stackLight;

public:
  StateHandler(SerialMonitorHandler &serialRef, StackLight &stackLightRef) : serialMonitor(serialRef), stackLight(stackLightRef) {}

  void analiseState(int temp, int humidity) {
    if(temp >= 30) {
      redAlert();
      return;
    }
    if(temp >= 25 && humidity < 20) {
      yellowOperation();
      return;
    }
    if(temp < 30) {
      greenOperation();
      return;
    }
  }

  void greenOperation() {
    stackLight.setGreenLightOn();
    serialMonitor.printState(1);
  }

  void yellowOperation() {
    stackLight.setYellowLightOn();
    serialMonitor.printState(2);
  }

  void redAlert() {
    stackLight.setRedLightOn();
    serialMonitor.printState(3);
  }
};

/*
dht DHT creates an object of the DHT class from the dht.h library.
The read11() function from the library populates its attributes with float values
of temperature in °C and humidity in relative percentage.
*/
dht DHT;
DHTRead dhtRead(8, DHT);
SerialMonitorHandler serialMonitor;
StackLight stackLight(12, 11, 10);
StateHandler stateHandler(serialMonitor, stackLight);

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(2000);
  stateHandler.analiseState(dhtRead.getTemperature(), dhtRead.getHumidity());
  serialMonitor.printSensorValues(dhtRead.getTemperature(), dhtRead.getHumidity());
}