#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

enum Screen {
  SCREEN_WELCOME,
  SCREEN_PIN
};

const byte ROWS = 4;
const byte COLUMNS = 4;
bool isClearNeeded = true;
String enteredPIN = "";
String openPIN = "1234";
String closePIN = "8965";

Screen currScreen = SCREEN_WELCOME;
Screen prevScreen = SCREEN_WELCOME;

char keys[ROWS][COLUMNS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPin[ROWS] = {11, 10, 9, 8};
byte colPin[COLUMNS] = {7, 6, 5, 4};

Keypad keypad = Keypad(makeKeymap(keys), rowPin, colPin, ROWS, COLUMNS);

Servo lock;

void welcomeMsg(char key) {
  lcd.setCursor(0, 0);
  lcd.print("Udvozoljuk!");
  if (key) {
    currScreen = SCREEN_PIN;
  }
}

void pinScreen() {
  if(isClearNeeded) {
    lcd.clear();
    isClearNeeded = false;
  }
  lcd.setCursor(0, 0);
  lcd.print("Kerem a PINt:");
  lcd.setCursor(0, 1);
  lcd.print("PIN:");

  for (int i = 0; i < enteredPIN.length(); i++) {
    lcd.print("*");
  }
}

void pinInputScreen(char key) {
  if (key >= '0' && key <= '9') {
    if(enteredPIN.length() < 4) {
    	enteredPIN += key;	
    }
  }
}

void closeDoor() {
  lock.write(180);  
}

void openDoor() {
  lock.write(90);  
}

void setup() {
  lcd.init();
  lcd.backlight();
  lock.attach(3);
}

void loop() {
  char key = keypad.getKey();
  if (currScreen != prevScreen) {
    isClearNeeded = true;
    prevScreen = currScreen;
  }
  
  switch (currScreen) {
    case SCREEN_WELCOME:
      welcomeMsg(key);
      break;
    case SCREEN_PIN:
      pinScreen();
      pinInputScreen(key);
      break;
  }
}