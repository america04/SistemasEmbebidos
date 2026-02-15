#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

/* ---------- LCD ---------- */
LiquidCrystal lcd(12, 11, 10, A0, A1, A2);

/* ---------- SERVO ---------- */
Servo puerta;

/* ---------- SENSOR ULTRASONICO ---------- */
const int trigPin = A3;
const int echoPin = A4;

/* ---------- TECLADO ---------- */
const byte filas = 4;
const byte columnas = 4;

char teclas[filas][columnas] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pinesFilas[filas] = {9, 8, 7, 6};
byte pinesColumnas[columnas] = {5, 4, 3, 2};

Keypad teclado = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, filas, columnas);

/* ---------- CONTRASEÑAS ---------- */
String password = "1234";
String input = "";
String newPassword = "";

bool cambiarClave = false;
bool puertaAbierta = false;

/* ---------- TIEMPO SENSOR ---------- */
unsigned long tiempoSinPresencia = 0;
const unsigned long tiempoCierre = 5000; // 5 segundos
const int distanciaMaxima = 25; // cm

/* ---------- SETUP ---------- */
void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Ingrese clave");

  puerta.attach(13);
  puerta.write(0); // puerta cerrada

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

/* ---------- LOOP ---------- */
void loop() {
  char tecla = teclado.getKey();

  if (tecla) {

    /* ----- MODO CAMBIAR CONTRASEÑA ----- */
    if (cambiarClave) {
      if (tecla == '#') {
        password = newPassword;
        newPassword = "";
        cambiarClave = false;
        lcd.clear();
        lcd.print("Clave guardada");
        delay(1500);
        lcd.clear();
        lcd.print("Ingrese clave");
      } else {
        newPassword += tecla;
        lcd.setCursor(0, 1);
        lcd.print(newPassword);
      }
      return;
    }

    /* ----- ENTRAR A CAMBIO DE CLAVE ----- */
    if (tecla == '*') {
      cambiarClave = true;
      newPassword = "";
      lcd.clear();
      lcd.print("Nueva clave:");
      return;
    }

    /* ----- INGRESO NORMAL ----- */
    if (tecla == '#') {
      lcd.clear();
      if (input == password) {
        lcd.print("Acceso correcto");
        abrirPuerta();
      } else {
        lcd.print("Clave incorrecta");
      }
      input = "";
      delay(1500);
      lcd.clear();
      lcd.print("Ingrese clave");
    } else {
      input += tecla;
      lcd.setCursor(input.length() - 1, 1);
      lcd.print("*");
    }
  }

  /* ----- CONTROL DE CIERRE CON SENSOR ----- */
  if (puertaAbierta) {
    long distancia = medirDistancia();

    if (distancia > 0 && distancia <= distanciaMaxima) {
      // Hay presencia
      tiempoSinPresencia = millis();
    } else {
      // No hay presencia
      if (millis() - tiempoSinPresencia >= tiempoCierre) {
        cerrarPuerta();
        lcd.clear();
        lcd.print("Puerta cerrada");
        delay(1500);
        lcd.clear();
        lcd.print("Ingrese clave");
      }
    }
  }
}

/* ---------- FUNCIONES ---------- */
void abrirPuerta() {
  puerta.write(90);
  puertaAbierta = true;
  tiempoSinPresencia = millis();
}

void cerrarPuerta() {
  puerta.write(0);
  puertaAbierta = false;
}

long medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH, 30000);
  if (duracion == 0) return 0;

  long distancia = duracion * 0.034 / 2;
  return distancia;
}
