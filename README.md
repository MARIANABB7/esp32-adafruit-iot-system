# ESP32 IoT Home System 

Este proyecto implementa un sistema domótico e IoT basado en el microcontrolador **ESP32**. Permite controlar la iluminación de forma local (mediante un teclado matricial) y remota, además de enviar datos de presencia y detección de gas a la nube con **Adafruit IO**.

---

## Componentes y Hardware
* **Microcontrolador:** ESP32
* **Sensores:**
  * Sensor de movimiento PIR (Pin 34)
  * Sensor de gas MQ / Analógico (Pin 35)
* **Actuadores y Entradas:**
  * Módulo Relevador / Foco (Pin 22)
  * Teclado Matricial 4x4 (Filas: 19, 18, 5, 17 | Columnas: 16, 4, 32, 33)

---

## Librerías Requeridas
Asegúrate de instalar las siguientes librerías desde el Gestor de Librerías de Arduino IDE:
* `WiFi.h` y `WiFiManager`
* `Adafruit IO Arduino`
* `Keypad`

---

## Configuración y Uso

1. Abre el archivo `.ino` en Arduino IDE.
2. Reemplaza las credenciales de Adafruit en el código con tus datos personales:
   ```cpp
   #define IO_USERNAME "TU_USUARIO"
   #define IO_KEY      "TU_IO_KEY"
