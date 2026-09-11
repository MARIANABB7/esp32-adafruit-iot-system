#include <WiFi.h>
#include <WiFiManager.h>      
#include "AdafruitIO_WiFi.h"
#include <Keypad.h>

// ======================================================
// CONFIGURACIÓN DE ADAFRUIT IO (CREDENCIALES OCULTAS)
// ======================================================
#define IO_USERNAME     "TU_USUARIO_ADAFRUIT"
#define IO_KEY          "TU_IO_KEY_AQUI"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, "", "");

// Feeds configurados en el Dashboard
AdafruitIO_Feed *feedFoco = io.feed("foco");
AdafruitIO_Feed *feedPir  = io.feed("pir");       
AdafruitIO_Feed *feedGas  = io.feed("gas2");

// ======================================================
// PINES DE HARDWARE
// ======================================================
const int PIN_RELAY = 22; 
const int PIN_PIR   = 34; 
const int PIN_GAS   = 35; // Pin analógico ADC

// Teclado Matricial 4x4
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {19, 18, 5, 17}; 
byte colPins[COLS] = {16, 4, 32, 33}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

bool estadoFoco = false; 
unsigned long ultimoEnvio = 0;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_PIR, INPUT);
  
  // Apagado por defecto
  digitalWrite(PIN_RELAY, HIGH); 

  // Configuración de red Wi-Fi
  WiFiManager wm;
  wm.setConfigPortalTimeout(30); 

  if(!wm.autoConnect("ESP32-Config-WiFi")) {
    Serial.println("Modo Local (Sin conexión a internet)");
  } else {
    Serial.println("¡Conectado a Wi-Fi!");
    io.connect();
    feedFoco->onMessage(handleMessage);
  }
}

void loop() {
  if(io.status() >= AIO_CONNECTED) {
    io.run(); 
  }

  // Control local por Teclado Matricial (Tecla '1')
  char key = keypad.getKey();
  if (key == '1') { 
    estadoFoco = !estadoFoco;
    digitalWrite(PIN_RELAY, estadoFoco ? LOW : HIGH);
    
    if(io.status() >= AIO_CONNECTED) {
      feedFoco->save(estadoFoco ? 1 : 0); 
    }
    Serial.print("Teclado -> Foco: ");
    Serial.println(estadoFoco ? "ENCENDIDO" : "APAGADO");
  }

  // Lectura y envío de sensores a Adafruit cada 3 segundos
  if (millis() - ultimoEnvio > 3000) {
    ultimoEnvio = millis();

    int estadoPir = digitalRead(PIN_PIR);
    int valorGas  = analogRead(PIN_GAS);

    if(io.status() >= AIO_CONNECTED) {
      feedPir->save(estadoPir);
      feedGas->save(valorGas);
    }

    Serial.print("PIR: "); Serial.print(estadoPir);
    Serial.print(" | Gas (Analógico): "); Serial.println(valorGas);
  }
}

// Recepción de órdenes desde Adafruit IO
void handleMessage(AdafruitIO_Data *data) {
  estadoFoco = data->toBool();
  digitalWrite(PIN_RELAY, estadoFoco ? LOW : HIGH);
  Serial.print("Adafruit -> Foco: ");
  Serial.println(estadoFoco ? "ENCENDIDO" : "APAGADO");
}
    