const int BTN_A = 2;
const int BTN_B = 3;

const int LED_VERDE = 8;
const int LED_ROJO = 9;

void setup() {
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  if (digitalRead(BTN_A) == LOW) {
    digitalWrite(LED_VERDE, HIGH);
    Serial.println("Boton A presionado - LED VERDE");
  } else {
    digitalWrite(LED_VERDE, LOW);
  }

  if (digitalRead(BTN_B) == LOW) {
    digitalWrite(LED_ROJO, HIGH);
    Serial.println("Boton B presionado - LED ROJO");
  } else {
    digitalWrite(LED_ROJO, LOW);
  }

  delay(200);
}