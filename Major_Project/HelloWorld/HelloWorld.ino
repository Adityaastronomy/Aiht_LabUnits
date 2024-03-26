#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
	// initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();
  lcd.print("7");
  lcd.clear();
}

void loop()
{
	// Do nothing here...
  lcd.clear();
  lcd.print("0.65");
  lcd.clear();
  delay(3000);
}
