const int PIN_POT = A0;
const int PIN_LED = 9;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Lectura\tVoltaje\tPorcentaje\tPWM");

}

void loop() {
  // put your main code here, to run repeatedly:
  int lectura = analogRead(PIN_POT);
  float voltaje = lectura * (5.0 / 1023.0);
  int porcentaje = map(lectura, 0, 1023, 0, 100);
  int pwm_val = map(lectura, 0, 1023, 0, 255);

  analogWrite(PIN_LED, pwm_val);

  Serial.print(lectura); Serial.print("\t");
  Serial.print(voltaje, 2); Serial.print("\t");
  Serial.print(porcentaje); Serial.print("\t");
  Serial.println(pwm_val);

  delay(500);

}
