const int PIN_LDR = A1;
const int PIN_LED = 9;
const int PIN_BUZZER  = 6;

const int LDR_OSCURO = 800;
const int LDR_LUZ = 100;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("LDR_raw\tLuz%\tEstado");

}

void loop() {
  // put your main code here, to run repeatedly:
  int lectura_ldr = analogRead(PIN_LDR);

  int luz_pct = map(lectura_ldr, LDR_OSCURO, LDR_LUZ, 0, 100);
  luz_pct = constrain(luz_pct, 0, 100);

  int brillo_led = map(luz_pct, 0, 100, 255, 0);
  analogWrite(PIN_LED, brillo_led);

  String estado;
  if(luz_pct < 20){
    estado = "MUY OSCURO - ALARMA";
    tone(PIN_BUZZER, 1000, 100);
  } else if(luz_pct < 50){
    estado = "PENUMBRA";
  } else{
    estado = "ILUMINADO";
  }

  Serial.print(lectura_ldr); Serial.print("\t");
  Serial.print(luz_pct); Serial.print("\t");
  Serial.println(estado);
  delay(200);
}
