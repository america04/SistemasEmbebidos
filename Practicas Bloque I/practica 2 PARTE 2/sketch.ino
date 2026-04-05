const int PIN_BOTON = 2;
const int PIN_LED = 13;

int estado_led = LOW;
int conteo = 0;

unsigned long ultimoTiempo = 0;
const int debounceDelay = 50;

int ultimoEstadoBoton = HIGH;

void setup() {
  pinMode(PIN_BOTON, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(9600);
  Serial.println("Lab 02 - Debounce funcionando");
}

void loop() {
  int lectura = digitalRead(PIN_BOTON);

  if (lectura != ultimoEstadoBoton) {
    if ((millis() - ultimoTiempo) > debounceDelay) {

      // Detecta cuando presionas
      if (lectura == LOW) {
        conteo++;

        estado_led = !estado_led;
        digitalWrite(PIN_LED, estado_led);

        Serial.print("Pulsacion #");
        Serial.print(conteo);
        Serial.print(" LED: ");
        Serial.println(estado_led ? "ON" : "OFF");
      }

      ultimoTiempo = millis();
    }
  }

  ultimoEstadoBoton = lectura;
}