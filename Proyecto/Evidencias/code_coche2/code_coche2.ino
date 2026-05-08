#include "Freenove_WS2812B_RGBLED_Controller.h"
#include <IRremote.h>

#define PIN_DIRECTION_LEFT  4
#define PIN_DIRECTION_RIGHT 3
#define PIN_MOTOR_PWM_LEFT  6
#define PIN_MOTOR_PWM_RIGHT 5
#define PIN_IRREMOTE_RECV   9
#define PIN_BUZZER          A0

#define MOTOR_DIRECTION     0
#define IR_CAR_SPEED        180

IRrecv irrecv(PIN_IRREMOTE_RECV);
decode_results results;
u32 lastIRUpdateTime = 0;
bool isStopFromIR = true;

bool alarmaActiva = false;
unsigned long tiempoUltimoPitido = 0;
int pitidosDados = 0;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(PIN_DIRECTION_LEFT, OUTPUT);
  pinMode(PIN_MOTOR_PWM_LEFT, OUTPUT);
  pinMode(PIN_DIRECTION_RIGHT, OUTPUT);
  pinMode(PIN_MOTOR_PWM_RIGHT, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    char comando = Serial.read();
    
    if (comando == 'A') { 
      digitalWrite(PIN_BUZZER, HIGH);
      delay(100); // Pitido muy corto para no bloquear
      digitalWrite(PIN_BUZZER, LOW);
    } 
    else if (comando == 'S') { 
      motorRun(0, 0);  
      alarmaActiva = true; // Activamos el modo alarma
      pitidosDados = 0;
    }
    else if (comando == 'R') { 
      // REINICIO INSTANTÁNEO
      alarmaActiva = false; 
      digitalWrite(PIN_BUZZER, LOW);
      motorRun(0, 0);
      isStopFromIR = true;
    }
  }

  // Logica de alarma sin delay
  if (alarmaActiva && pitidosDados < 10) {
    if (millis() - tiempoUltimoPitido >= 200) {
      digitalWrite(PIN_BUZZER, !digitalRead(PIN_BUZZER)); // Alterna el sonido
      tiempoUltimoPitido = millis();
      pitidosDados++;
    }
  } else if (pitidosDados >= 10) {
    alarmaActiva = false;
    digitalWrite(PIN_BUZZER, LOW);
  }

  // Control Remoto IR
  if (irrecv.decode(&results)) {
    isStopFromIR = false;
    u32 code = results.value;
    if (code == 0xFF02FD) motorRun(IR_CAR_SPEED, IR_CAR_SPEED);
    else if (code == 0xFF9867) motorRun(-IR_CAR_SPEED, -IR_CAR_SPEED);
    else if (code == 0xFFE01F) motorRun(-IR_CAR_SPEED, IR_CAR_SPEED);
    else if (code == 0xFF906F) motorRun(IR_CAR_SPEED, -IR_CAR_SPEED);
    irrecv.resume();
    lastIRUpdateTime = millis();
  } else {
    if (millis() - lastIRUpdateTime > 110 && !isStopFromIR) {
      motorRun(0, 0);
      isStopFromIR = true;
    }
  }
}

void motorRun(int speedl, int speedr) {
  int dirL = (speedl > 0) ? (0 ^ MOTOR_DIRECTION) : (1 ^ MOTOR_DIRECTION);
  int dirR = (speedr > 0) ? (1 ^ MOTOR_DIRECTION) : (0 ^ MOTOR_DIRECTION);
  digitalWrite(PIN_DIRECTION_LEFT, dirL);
  digitalWrite(PIN_DIRECTION_RIGHT, dirR);
  analogWrite(PIN_MOTOR_PWM_LEFT, constrain(abs(speedl), 0, 255));
  analogWrite(PIN_MOTOR_PWM_RIGHT, constrain(abs(speedr), 0, 255));
}