const int PIN_LED = 13;
int contador = 0;
float tiempoActual=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(9600);
  Serial.println("=== LAB01 - EJERCICIO DE MODIFICACION 3===");
  }

void loop() {
  // put your main code here, to run repeatedly:
  contador++;

  digitalWrite(PIN_LED, HIGH);
  Serial.print("[#");
  Serial.print(contador);
  Serial.print("] t=");
  Serial.print(tiempoActual);
  Serial.println("ms LED -> ENCENDIDO");
  delay(500);

  tiempoActual = millis();

  digitalWrite(PIN_LED, LOW);
  Serial.print("[#");
  Serial.print(contador);
  Serial.print("] t=");
  Serial.print(tiempoActual);
  Serial.println("ms LED -> APAGADO");
  delay(500);
}
