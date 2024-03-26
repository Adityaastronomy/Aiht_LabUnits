#include <TimeLib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DHTPIN 2       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);
int pulsePin = A0;  // Pulse Sensor purple wire connected to analog pin A0

const int xPin = A1;  // X-axis of the accelerometer connected to analog pin A1
const int yPin = A2;  // Y-axis of the accelerometer connected to analog pin A2
const int zPin = A3;  // Z-axis of the accelerometer connected to analog pin A3

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;             // holds the incoming raw data
volatile int IBI = 600;          // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false;  // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false;     // becomes true when Arduino finds a beat.
volatile int steps = 0;          // Variable to count steps


int prevX = 0;             // Previous X-axis value
int prevY = 0;             // Previous Y-axis value
int prevZ = 0;             // Previous Z-axis value
const int threshold = 10;  // Threshold for detecting a step

void setup() {
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  setTime(22, 32, 0, 21, 3, 24);
}

void printDigits(int digits) {
  // Add leading 0 if the number is less than 10
  if (digits < 10)
    lcd.print(0);
  lcd.print(digits);
}



void digitalClockDisplay() {
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
  lcd.setCursor(0, 0);
  lcd.print("Date: ");
  lcd.print(day());
  lcd.print("/");
  lcd.print(month());
  lcd.print("/");
  lcd.print(year());

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  printDigits(hour());
  lcd.print(":");
  printDigits(minute());
  lcd.print(":");
  printDigits(second());

  delay(2000);
  lcd.clear();
}

void loop() {
  digitalClockDisplay();
  delay(1000);

  // Read temperature and humidity
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  float lin = (-0.01896 + (0.0191381 * h) + (-0.09338002 * t));
  float Po = (1 / (1 + exp(-(lin))));
  float xp = Po * 100;
  // Accelerometer Step Detection
  int xAcc = analogRead(xPin);
  int yAcc = analogRead(yPin);
  int zAcc = analogRead(zPin);

  if (abs(xAcc - prevX) > threshold || abs(yAcc - prevY) > threshold || abs(zAcc - prevZ) > threshold) {
    steps++;
    prevX = xAcc;
    prevY = yAcc;
    prevZ = zAcc;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));


  Serial.print(F("percentage probability of rainfall "));
  Serial.println(xp);

  // Display temperature and humidity
  lcd.setCursor(0, 0);
  lcd.print("Temperature: ");
  lcd.setCursor(12, 0);
  lcd.print(t);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.setCursor(12, 1);
  lcd.print(h);
  lcd.print("%");

  delay(2000);
  lcd.clear();

  // Display probability of rainfall
  lcd.setCursor(0, 0);
  lcd.print("P(rain)%: ");
  lcd.print(xp);

  delay(2000);
  lcd.clear();

  // Print Step Count on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Steps: ");
  lcd.print(steps);
  Serial.println(steps);
  delay(2000);
}


// for DHT11,
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
