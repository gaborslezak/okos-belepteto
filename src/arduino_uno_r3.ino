#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

enum Screen {
  SCREEN_WELCOME,
  SCREEN_PIN,
  SCREEN_OPENED_DOOR,
  SCREEN_CLOSED_DOOR,
  SCREEN_ERROR,
  SCREEN_LEAVING,
  SCREEN_LEFT
};

enum ErrorCode {
  NO_ERROR,
  ERROR_INVALID_PIN,
  ERROR_CLOSE_THE_DOOR
};

const byte ROWS = 4;
const byte COLUMNS = 4;
bool isClearNeeded = true;
String enteredPIN = "";
String openPIN = "1234";
String closePIN = "8965";
int lastCloserButtonState = HIGH;
int currCloserButtonState = HIGH;
bool isDoorClosed = true;
bool fullLock = false;
int doorButton = 2;
int doorLockPin = 3;
int windowButton = 13;
int windowLockPin = 12;
int lastWindowButtonState = HIGH;
int currWindowButtonState = HIGH;
Screen screenBeforeError = SCREEN_PIN;
bool isWindowOpen = true;
int potPin = A0;

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

Servo doorLock;
Servo windowLock;

void welcomeMsg(char key) {
  if(isClearNeeded) {
    lcd.clear();
    isClearNeeded = false;
  }
  lcd.setCursor(0, 0);
  lcd.print("Udvozoljuk!");
  if (key) {
    currScreen = SCREEN_PIN;
  }
}

void pinScreen(char key, String pin, String reason) {
  if(isClearNeeded) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(reason);
    lcd.setCursor(0, 1);
    lcd.print("PINt:");
    isClearNeeded = false;
  }


  if (key >= '0' && key <= '9') {
    if(enteredPIN.length() < 4) {
    	enteredPIN += key;	

      lcd.setCursor(5 + enteredPIN.length() - 1, 1);
      lcd.print("*");
    }
  }

  if (enteredPIN.length() == 4 && enteredPIN != pin) {
    screenBeforeError = currScreen;
    currScreen = SCREEN_ERROR;
    currErrorCode = ERROR_INVALID_PIN;
    enteredPIN = "";
  }
  else if (enteredPIN.length() == 4 && enteredPIN == pin) {
    if (pin == openPIN) {
      fullLock = false;
      currScreen = SCREEN_OPENED_DOOR;
    } else {
      fullLock = true;
      currScreen = SCREEN_LEFT;
    }
    enteredPIN = "";
  }
}

void closeDoor() {
  if(isClearNeeded) {
    lcd.clear();
    isClearNeeded = false;
  }
  currScreen = SCREEN_CLOSED_DOOR;
  lcd.setCursor(0, 0);
  lcd.print("Ajto zarodik.");
  doorLock.write(0);
}

void openDoor() {
  if(isClearNeeded) {
    lcd.clear();
    isClearNeeded = false;
  }
  currScreen = SCREEN_OPENED_DOOR;
  lcd.setCursor(0, 0);
  lcd.print("Ajto nyilik.");
  doorLock.write(90);
}

void errorScreen() {
  if(isClearNeeded) {
    lcd.clear();
    isClearNeeded = false;
  }
  switch (currErrorCode) {
    case ERROR_INVALID_PIN:
      lcd.setCursor(0, 0);
      lcd.print("A PIN hibas.");
      lcd.setCursor(0, 1);
      lcd.print("Probald ujra!");
      currScreen = screenBeforeError;
      currErrorCode = NO_ERROR;
      break;
    case ERROR_CLOSE_THE_DOOR:
      lcd.setCursor(0, 0);
      lcd.print("Ajto nyitva!");
      lcd.setCursor(0, 1);
      lcd.print("Kerem zarja be!");
      currScreen = screenBeforeError;
      currErrorCode = NO_ERROR;
      break;
    default:
      break;
  }
}

void openedDoorSequence() {
  if (isDoorClosed) {
    openDoor();
    isDoorClosed = false;
  }
  if (lastCloserButtonState == HIGH && currCloserButtonState == LOW) {
    closeDoor();
    isDoorClosed = true;
    currScreen = SCREEN_CLOSED_DOOR;
  }
}

void closedDoorSequence() {
  if (!fullLock && lastCloserButtonState == HIGH && currCloserButtonState == LOW) {
    openDoor();
    isDoorClosed = false;
    currScreen = SCREEN_OPENED_DOOR;
  }
}

void guestsLeft(){
  if(isClearNeeded) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Viszlat!");
    lcd.setCursor(0, 1);
    lcd.print("Visszavarjuk!");
    isClearNeeded = false;
  }
  doorLock.write(0);
  isDoorClosed = true;
  delay(1500);
  currScreen = SCREEN_WELCOME;
}

void windowHandler(int percentage) {
  if (lastWindowButtonState == HIGH && currWindowButtonState == LOW) {
    isWindowOpen = !isWindowOpen;
    Serial.println(isWindowOpen);
    if (isWindowOpen) {
      windowLock.write(90);
    } else {
      windowLock.write(0);
    }
  }
  lastWindowButtonState = currWindowButtonState;
  if (isWindowOpen && percentage > 30) {
    Serial.print("RAIN");
    Serial.print("RAIN");
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  windowLock.attach(windowLockPin);
  windowLock.write(90);
  doorLock.attach(doorLockPin);
  doorLock.write(0);
  pinMode(doorButton, INPUT_PULLUP);
  pinMode(windowButton, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(potPin);
  int percentage = map(value, 0, 1023, 100, 0);
  // Serial.print("Potmeter: ");
  // Serial.print(percentage);
  // Serial.println("%");

  char key = keypad.getKey();
  //Serial.println(key);
  currCloserButtonState = digitalRead(doorButton);
  currWindowButtonState = digitalRead(windowButton);
  Serial.println(isDoorClosed);
  if (key == 'C' && isDoorClosed && !fullLock) {
    currScreen = SCREEN_LEAVING;
  }
  else if (key == 'C' && !isDoorClosed && !fullLock) {
    screenBeforeError = currScreen;
    currScreen = SCREEN_ERROR;
    currErrorCode = ERROR_CLOSE_THE_DOOR;
  }
  if (currScreen != prevScreen) {
    isClearNeeded = true;
    prevScreen = currScreen;
  }
  
  switch (currScreen) {
    case SCREEN_WELCOME:
      welcomeMsg(key);
      break;
    case SCREEN_PIN:
      pinScreen(key, openPIN, "Nyitas, kerem a");
      break;
    case SCREEN_ERROR:
      errorScreen();
      break;
    case SCREEN_OPENED_DOOR:
      openedDoorSequence();
      break;
    case SCREEN_CLOSED_DOOR:
      closedDoorSequence();
      break;
    case SCREEN_LEAVING:
      pinScreen(key, closePIN, "Zaras, kerem a");
      break;
    case SCREEN_LEFT:
      guestsLeft();
      break;
  }
  lastCloserButtonState = currCloserButtonState;
  windowHandler(percentage);
}