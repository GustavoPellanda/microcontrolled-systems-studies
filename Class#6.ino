/* This code displays an int on a LCD Display,
being that the int can be increased or decreased by push buttons */

class Display {
    // TO DO: Display code
};

class ButtonCounter {
private:
    int counter;
    byte pinB1, pinB2;
    bool lastStateB1, lastStateB2, currentStateB1, currentStateB2;

public:
    ButtonCounter(byte pb1, byte pb2): pinB1(pb1), pinB2(pb2), counter(0){
        pinMode(pinB1, INPUT_PULLUP);
        pinMode(pinB2, INPUT_PULLUP);
        
        lastStateB1 = digitalRead(pinB1);
        lastStateB2 = digitalRead(pinB2);
        currentStateB1 = lastStateB1;
        currentStateB2 = lastStateB2;
    }

    void update(){
        currentStateB1 = digitalRead(pinB1);
        currentStateB2 = digitalRead(pinB2);

        if(lastStateB1 == HIGH && currentStateB1 == LOW) counter++;
        if(lastStateB2 == HIGH && currentStateB2 == LOW) counter--;

        lastStateB1 = currentStateB1;
        lastStateB2 = currentStateB2;
    }

    int getCounter(){
        return counter;
    }
};

ButtonCounter buttonCounter(6, 7);

void setup(){
  Serial.begin(9600);
}

void loop(){
    buttonCounter.update();
    Serial.println(buttonCounter.getCounter());
    delay(100);
}