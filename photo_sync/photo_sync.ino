#include <Arduino.h>
#if !defined(ARDUINO_ESP32C3_DEV) // This is due to a bug in RISC-V compiler, which requires unused function sections :-(.
#define DISABLE_CODE_FOR_RECEIVER // Disables static receiver code like receive timer ISR handler and static IRReceiver and irparams data. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not required.
#endif
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library

const bool VIDEO_BUTTON_ENABLED = true; // подключена ли видео-кнопка?

const int LIGHT_SENSOR_PIN = 7; // пин датчика света 
const int PHOTO_BUTTON_PIN = 8; // пин кнопки (фото-съемка)
const int VIDEO_BUTTON_PIN = 9; // пин кнопки (видео-съемка)

const int LED_PIN = 13; // пин индикационного светодиода
// IR Sender pin - пин ИК передатчика на УНО и НАНО = 2

int send_count = 0; // счетчик отправок

void setup() {
    pinMode(LED_PIN, OUTPUT); 
    pinMode(PHOTO_BUTTON_PIN, INPUT);
    pinMode(VIDEO_BUTTON_PIN, INPUT);
    pinMode(LIGHT_SENSOR_PIN, INPUT);

    Serial.begin(115200);

    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
    Serial.print(F("Send IR signals at pin "));
    Serial.println(IR_SEND_PIN);
    IrSender.begin(); // Start with IR_SEND_PIN as send pin and disable feedback LED at default feedback LED pin
    disableLEDFeedback(); // Disable feedback LED at default feedback LED pin
}

void loop() {

  // ФОТО
  if (digitalRead(LIGHT_SENSOR_PIN) == LOW or digitalRead(PHOTO_BUTTON_PIN) == HIGH) { // Если сработал датчик или нажата кнопка фото
    // Отправляем ИК код (фото)
    Serial.println("Sending IR code (Photo)");
    Serial.println("Counter: " + String(send_count));
    IrSender.sendNEC(0x0, 0x8, 5); // Отправка ИК кода (Пульт FullHD Съемка фото)
    send_count += 1; // Увеличиваем счетчик
    
    while (digitalRead(LIGHT_SENSOR_PIN) == LOW or digitalRead(PHOTO_BUTTON_PIN) == HIGH){
      // Ждем пока сигнал пропадет
      delay(10);
    }

  } else {
    digitalWrite(LED_PIN, LOW); // в любой непонятной ситуации гасим светодиод
  }

  // ВИДЕО
  if (digitalRead(VIDEO_BUTTON_PIN) == HIGH and VIDEO_BUTTON_ENABLED == true) { // Если доступна и нажата кнопка видео
    // Отправляем ИК код (видео)
    Serial.println("Sending IR code (Video)");
    IrSender.sendNEC(0x0, 0x1C, 5); // Отправка ИК кода (Пульт FullHD Съемка видео)
    
    while (digitalRead(VIDEO_BUTTON_PIN) == HIGH and VIDEO_BUTTON_ENABLED == true){
      // Ждем пока сигнал пропадет
      delay(10);
    }

  } else {
    digitalWrite(LED_PIN, LOW); // в любой непонятной ситуации гасим светодиод
  }

}


