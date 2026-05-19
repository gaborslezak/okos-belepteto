#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLUMNS = 4;
bool isScrollingStarted = false;

char keys[ROWS][COLUMNS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPin[ROWS] = {11, 10, 9, 8};
byte colPin[COLUMNS] = {7, 6, 5, 4};

Keypad keypad = Keypad(makeKeymap(keys), rowPin, colPin, ROWS, COLUMNS);

void udvozles() {
  lcd.setCursor(0, 0);
  lcd.print("Udvozoljuk!");

  lcd.setCursor(0, 1);
  lcd.print("Kerem, adja meg a PIN kodot!");
}

void setup() {
  lcd.init();
  lcd.backlight();

}

void loop() {
  char key = keypad.getKey();
  if (key) {
    udvozles();
    isScrollingStarted = true;
  }
  if (isScrollingStarted) {
    delay(1000);
    lcd.scrollDisplayLeft();
    delay(300);
  }
}