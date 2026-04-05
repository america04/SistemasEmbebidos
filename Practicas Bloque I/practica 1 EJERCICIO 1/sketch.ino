const int PIN_LED = 13;
int contador = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(9600);
  Serial.println("=== LAB01 - EJERCICIO DE MODIFICACION 1===");
}

void loop() {
  // put your main code here, to run repeatedly:
  contador++;
  digitalWrite(PIN_LED, HIGH);
  Serial.print("[#");
  Serial.print(contador);
  Serial.println("] LED ->  ENCENDIDO");
  delay(200);

  digitalWrite(PIN_LED, LOW);
  Serial.print("[#");
  Serial.print(contador);
  Serial.println("] LED ->  APAGADO");
  delay(800);
}
