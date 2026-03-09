#include <WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"

const char* ssid = "Realme 12+";
const char* password = "cris2310";

//configuracion de ThingSpeak
unsigned long channelID = 3292213;
const char* writeAPIKey = "XXFZSM4U1RZMZ9T9";

WiFiClient client;

//configuración del sensor DHT22
#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

unsigned long intervalo = 15000;
unsigned long tiempoAnterior = 0;

//calcular punto de rocio
float calcularPuntoRocio(float temperatura, float humedad) {
    float a = 17.27;
    float b = 237.7;
    
    float alpha = ((a * temperatura) / (b + temperatura)) + log(humedad / 100);
    float puntoRocio = (b * alpha) / (a - alpha);

    return puntoRocio;
}

void setup() {

    Serial.begin(115200);

    dht.begin();

    WiFi.begin(ssid, password);
    Serial.print("Conectando a WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi conectado");

    ThingSpeak.begin(client);
}

void loop() {

    if (millis() - tiempoAnterior >= intervalo) {

        tiempoAnterior = millis();

        float humedad = dht.readHumidity();
        float temperatura = dht.readTemperature();

        if (isnan(humedad) || isnan(temperatura)) {
            Serial.println("Error al leer el sensor DHT22");
            return;
        }

        float heatIndex = dht.computeHeatIndex(temperatura, humedad, false);

        float puntoRocio = calcularPuntoRocio(temperatura, humedad);

        Serial.println("----- DATOS DEL SENSOR -----");
        Serial.print("Temperatura: ");
        Serial.println(temperatura);

        Serial.print("Humedad: ");
        Serial.println(humedad);

        Serial.print("Sensacion Termica: ");
        Serial.println(heatIndex);

        Serial.print("Punto de Rocio: ");
        Serial.println(puntoRocio);

        ThingSpeak.setField(1, temperatura);
        ThingSpeak.setField(2, humedad);
        ThingSpeak.setField(3, heatIndex);
        ThingSpeak.setField(4, puntoRocio);

        int respuesta = ThingSpeak.writeFields(channelID, writeAPIKey);

        if (respuesta == 200) {
            Serial.println("Datos enviados correctamente a ThingSpeak");
        } else {
            Serial.print("Error al enviar datos. Codigo: ");
            Serial.println(respuesta);
        }

        Serial.println("----------------------------");
    }
}