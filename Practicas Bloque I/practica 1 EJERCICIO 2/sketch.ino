const int PIN_LED = 13;
int contador = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(9600);
  Serial.println("=== LAB01 - EJERCICIO DE MODIFICACION 2===");
  Serial.println("Sistema SOS Iniciado");
  }

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("ENVIANDO: S");
  for(int i = 0; i < 3; i++){
    digitalWrite(PIN_LED, HIGH);
    delay(200);
    digitalWrite(PIN_LED, LOW);
    delay(200);
  }
  delay(400);

  Serial.println("ENVIANDO: O");
  for(int i = 0; i < 3; i++){
    digitalWrite(PIN_LED, HIGH);
    delay(600);
    digitalWrite(PIN_LED, LOW);
    delay(200);
  }
  delay(400);

  Serial.println("ENVIANDO: S");
  for(int i = 0; i < 3; i++){
    digitalWrite(PIN_LED, HIGH);
    delay(200);
    digitalWrite(PIN_LED, LOW);
    delay(200);
  }

  Serial.println("PAUSA DE CICLO...");
  delay(2000);
}
