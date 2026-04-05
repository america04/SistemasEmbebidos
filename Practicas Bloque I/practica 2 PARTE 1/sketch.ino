const int PIN_BOTON = 2;
const int PIN_LED = 8;

void setup() {
  pinMode(PIN_BOTON, INPUT);   
  pinMode(PIN_LED, OUTPUT);    

  Serial.begin(9600);
  Serial.println("Lab 02 — Lectura de pulsador");
}

void loop() {
  int estado = digitalRead(PIN_BOTON);

  if (estado == HIGH) {
    digitalWrite(PIN_LED, HIGH);
    Serial.println("Boton: PRESIONADO");
  } else {
    digitalWrite(PIN_LED, LOW);
  }

  delay(50);
}