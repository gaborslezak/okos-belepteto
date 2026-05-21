#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

enum Screen {
  SCREEN_WELCOME,
  SCREEN_PIN,
  SCREEN_OPEN_DOOR,
  SCREEN_ERROR
};

enum ErrorCode {
  NO_ERROR,
  INVALID_PIN
};

const byte ROWS = 4;
const byte COLUMNS = 4;
bool isClearNeeded = true;
String enteredPIN = "";
String openPIN = "1234";
String closePIN = "8965";

Screen currScreen = SCREEN_WELCOME;
Screen prevScreen = SCREEN_WELCOME;
ErrorCode currErrorCode = NO_ERROR;

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

void pinScreen(char key) {
  if(isClearNeeded) {
    lcd.clear();
    isClearNeeded = false;
  }
  lcd.setCursor(0, 0);
  lcd.print("Kerem a PINt:");
  lcd.setCursor(0, 1);
  lcd.print("PIN:");

  if (key >= '0' && key <= '9') {
    if(enteredPIN.length() < 4) {
    	enteredPIN += key;	
    }
  }
  for (int i = 0; i < enteredPIN.length(); i++) {
    lcd.print("*");
  }

  if (enteredPIN.length() == 4 && enteredPIN != openPIN) {
    currScreen = SCREEN_ERROR;
    currErrorCode = INVALID_PIN;
    enteredPIN = "";
  }
  else if(enteredPIN.length() == 4 && enteredPIN == openPIN) {
    currScreen = SCREEN_OPEN_DOOR;
    enteredPIN ="";
  }
}

void closeDoor() {
  lock.write(180);  
}

void openDoor() {
  if(isClearNeeded) {
    lcd.clear();
    isClearNeeded = false;
  }
  lcd.setCursor(0, 0);
  lcd.print("PIN helyes.");
  lcd.setCursor(0, 1);
  lcd.print("Ajto nyilik.");
  lock.write(90);  
}

void errorScreen() {
  if(isClearNeeded) {
    lcd.clear();
    isClearNeeded = false;
  }
  switch (currErrorCode) {
    case INVALID_PIN:
      lcd.setCursor(0, 0);
      lcd.print("A PIN hibas.");
      lcd.setCursor(0, 1);
      lcd.print("Probald ujra!");
      delay(500);
      currScreen = SCREEN_PIN;
      currErrorCode = NO_ERROR;
      break;
    default:
      break;
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  lock.attach(3);
  lock.write(0);
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
      pinScreen(key);
      delay(10);
      break;
    case SCREEN_ERROR:
      errorScreen();
      break;
    case SCREEN_OPEN_DOOR:
      openDoor();
      break;
  }
}