const int BTN_A = 2;
const int BTN_B = 3;
const int LED = 13;

int conteo = 0;
int estadoLED = LOW;

int ultimoA = HIGH;
int ultimoB = HIGH;

void setup() {
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
  Serial.println("Ejercicio 1");
}

void loop() {
  int lecturaA = digitalRead(BTN_A);
  int lecturaB = digitalRead(BTN_B);

  if (lecturaA == LOW && ultimoA == HIGH) {
    conteo++;
    estadoLED = !estadoLED;
    digitalWrite(LED, estadoLED);

    Serial.print("Conteo: ");
    Serial.println(conteo);
  }

  if (lecturaB == LOW && ultimoB == HIGH) {
    conteo = 0;
    Serial.println("CONTEO REINICIADO");
  }

  ultimoA = lecturaA;
  ultimoB = lecturaB;

  delay(200);
}