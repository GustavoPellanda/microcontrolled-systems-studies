/* This program recives inputs from a keypad and prints them on the LCD Display.
Requirements: The numbers inputed must be stored in a buffer.
The buffer must be a rotational array.
*/

class Display {
private:
    LiquidCrystal lcd;
    const int columns = 16, rows = 2;

public:
    Display(byte p1, byte p2, byte p3, byte p4, byte p5, byte p6)
        : lcd(p1, p2, p3, p4, p5, p6) {
        lcd.begin(columns, rows);
        lcd.setCursor(0, 0);
    }

    void printNumber(int number){}

};

class Keypad {

};

class Buffer {
private:
    char inputBuffer[16];

public:
    getLastKey() {} // Returns the lastKey

};

void setup() {
    
}

void loop() {
    
}