/* This program creates state changes using a push button 
Requirements: Insert serial prints to see in which the states the LEDs are in
Write debounce code for the button's button.isPressed()s
*/

class Button {
private:
    byte pin;

public:
    Button(byte p) : pin(p) {
        pinMode(pin, INPUT_PULLUP);
    }
    ~Button() {}
    void on() {
        digitalWrite(pin, HIGH);
    }
    void off() {
        digitalWrite(pin, LOW);
    }
    boolean isPressed() {
        return !digitalRead(pin);
    }    
};

class LEDs {
private:
    byte pin1;
    byte pin2;

public:
    LEDs(byte p1, byte p2) : pin1(p1), pin2(p2) {
        pinMode(pin1, OUTPUT);
        pinMode(pin2, OUTPUT);
    }

    ~LEDs() {}

    void on() {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, HIGH);
    }

    void off() {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
    }

    void blinkTogether() {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, HIGH);
        delay(500);
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        delay(500);
    }

    void blinkAlternately() {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
        delay(500);
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
        delay(500);
    }
};

class StateHandler {
private:
    LEDs &ledRow;

public:
    StateHandler(LEDs& ledRow) : ledRow(ledRow) {}

    void state01() {
        ledRow.on();
        Serial.println("State 1: LEDs ON");
    }

    void state02() {
        ledRow.blinkTogether();
        Serial.println("State 2: LEDs Blinking Together");
    }

    void state03() {
        ledRow.blinkAlternately();
        Serial.println("State 3: LEDs Blinking Alternately");
    }
};

int currentState = 0;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

LEDs ledRow(6, 7);
Button button(13);
StateHandler stateHandler(ledRow);

void setup(){
    Serial.begin(9600);
    Serial.println("Starting...");
    ledRow.on();
}

void loop(){
    bool reading = button.isPressed();

    if(reading != lastButtonState){
        lastDebounceTime = millis();
    }

    if((millis() - lastDebounceTime) > debounceDelay){
        if(reading == LOW && lastButtonState == HIGH){
            currentState = (currentState + 1) % 3;
        }
    }

    lastButtonState = reading;

    switch(currentState){
        case 0:
            stateHandler.state01();
            break;
        case 1:
            stateHandler.state02();
            break;
        case 2:
            stateHandler.state03();
            break;
    }
}