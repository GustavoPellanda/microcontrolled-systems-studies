/* This program manages LED states via push buttons, 
whereas an LCD display presents a menu for state selection.
*/

#include <LiquidCrystal.h>

class Display {
private:
    LiquidCrystal lcd;
    const int columns = 16, rows = 2;

public:
    Display(byte p1, byte p2, byte p3, byte p4, byte p5, byte p6):
        lcd(p1, p2, p3, p4, p5, p6) {
        lcd.begin(columns, rows);
        lcd.setCursor(0, 0);
    }

};

class ButtonSelector {
private:
    byte pinB1, pinB2, pinB3;
    bool lastStateB1, lastStateB2, currentStateB1, currentStateB2;

public:
    ButtonCounter(byte pb1, byte pb2, byte pb3): pinB1(pb1), pinB2(pb2),  pinB2(pb3){
        pinMode(pinB1, INPUT_PULLUP);
        pinMode(pinB2, INPUT_PULLUP);
        pinMode(pinB2, INPUT_PULLUP);
    }

    // TO DO: return the command according to which button is pressed
   
};

    // TO DO: use the state handler to create the display menu

ButtonSelector buttonSelector(5, 6, 7);
Display display(13, 12, 11, 10, 9, 8);

void setup(){

}

void loop(){
    // TO DO:
    // The button selector will be responsible for reading which button is pressed and informing main
    // The state handler will change the state of the LEDs according to the return from the button selector method
    // The display class is only responsible for managing what the display will show, nothing else.
    // One of the buttons is up, one is down and one is select.
}