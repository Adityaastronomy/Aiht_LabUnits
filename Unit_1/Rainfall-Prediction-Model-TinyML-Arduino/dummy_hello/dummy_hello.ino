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
}

void loop()
{
	// Do nothing here...
  lcd.setCursor(1,0);
  lcd.print("7 ");
  delay(1000);
  lcd.clear();

  lcd.setCursor(2,0);
  lcd.print("8");
  lcd.setCursor(2,1);
  lcd.print("9");
  delay(1000);
  lcd.clear();
}
