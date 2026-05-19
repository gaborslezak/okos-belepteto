#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void udvozles() {
  lcd.setCursor(0, 0);
  lcd.print("Udvozoljuk!");

  lcd.setCursor(0, 1);
  lcd.print("Kerem, adja meg a PIN kodot!");
}

void setup() {
  lcd.init();
  lcd.backlight();
  udvozles();
}

void loop() {
  lcd.scrollDisplayLeft();
  delay(450);
}