// Lab07_BMP180_OLED.ino

#include <Wire.h>
#include <Adafruit_BMP085.h>     
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_W    128
#define OLED_H     64
#define OLED_ADDR  0x3C

Adafruit_BMP085 bmp;
Adafruit_SSD1306 display(OLED_W, OLED_H, &Wire, -1);

const float PRESION_MAR = 1013.25;

// ─────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("ERROR: OLED no encontrada");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 28);
  display.println("Iniciando BMP180...");
  display.display();

  if (!bmp.begin()) {
    Serial.println("ERROR: BMP180 no encontrado");
    display.clearDisplay();
    display.setCursor(10, 28);
    display.println("ERROR: BMP180");
    display.display();
    while (true);
  }

  Serial.println("BMP180 y OLED listos.");
  Serial.println("Tiempo\tTemp(C)\tPresion(hPa)\tAltitud(m)");
}

void loop() {
  float temp    = bmp.readTemperature();
  float presion = bmp.readPressure() / 100.0;  
  float altitud = bmp.readAltitude(PRESION_MAR);

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(25, 0);
  display.println("-- BMP180 --");

  // Temperatura 
  display.setTextSize(2);
  display.setCursor(0, 14);
  display.print(temp, 1);
  display.println(" C");

  // Presión
  display.setTextSize(1);
  display.setCursor(0, 38);
  display.print("P: ");
  display.print(presion, 1);
  display.println(" hPa");

  // Altitud
  display.setCursor(0, 50);
  display.print("Alt: ");
  display.print(altitud, 0);
  display.println(" m");

  display.display();

  // ── Monitor Serial ─────────────────────
  Serial.print(millis() / 1000); Serial.print("\t");
  Serial.print(temp, 2);         Serial.print("\t");
  Serial.print(presion, 2);      Serial.print("\t");
  Serial.println(altitud, 1);

  delay(1000);
}