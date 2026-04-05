const int PIN_LED = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(9600);
  Serial.println("=== LAB01 ===");
}
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PIN_LED, HIGH);
  Serial.println("LED Encendido");
  delay(1000);

  digitalWrite(PIN_LED, LOW);
  Serial.println("LED Apagado");
  delay(1000);
}

