#include <Arduino.h>
#if !defined(ARDUINO_ESP32C3_DEV) // This is due to a bug in RISC-V compiler, which requires unused function sections :-(.
#define DISABLE_CODE_FOR_RECEIVER // Disables static receiver code like receive timer ISR handler and static IRReceiver and irparams data. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not required.
#endif
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library

const int LIGHT_SENSOR_PIN = 7; // пин датчика света 
const int BUTTON_PIN = 8; // пин кнопки
const int LED_PIN = 13; // пин индикационного светодиода
// IR Sender pin - пин ИК передатчика на УНО и НАНО = 2

int send_count = 0; // счетчик отправок

void setup() {
    pinMode(LED_PIN, OUTPUT); 
    pinMode(BUTTON_PIN, INPUT);
    pinMode(LIGHT_SENSOR_PIN, INPUT);

    Serial.begin(115200);

    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
    Serial.print(F("Send IR signals at pin "));
    Serial.println(IR_SEND_PIN);
    IrSender.begin(); // Start with IR_SEND_PIN as send pin and disable feedback LED at default feedback LED pin
    disableLEDFeedback(); // Disable feedback LED at default feedback LED pin
}

void loop() {
  if (digitalRead(LIGHT_SENSOR_PIN) == LOW or digitalRead(BUTTON_PIN) == HIGH) { // Если сработал датчик или нажата кнопка
    // Отправляем ИК код
    Serial.println("Sending IR code");
    Serial.println("Counter: " + String(send_count));
    // IrSender.sendSamsung(0x707, 0xE6, 5); // Отправка ИК кода для ТВ Самсунг
    IrSender.sendNEC(0x0, 0x8, 5); // Отправка ИК кода (Пульт FullHD Съемка)
    send_count += 1; // Увеличиваем счетчик
    
    // for(int i = 0; i < 2; i++){
    //   // Моргаем светодиодом
    //   digitalWrite(LED_PIN, HIGH);
    //   delay(30);
    //   digitalWrite(LED_PIN, LOW);
    //   delay(30);
    // }
    
    while (digitalRead(LIGHT_SENSOR_PIN) == LOW or digitalRead(BUTTON_PIN) == HIGH){
      // Ждем пока сигнал пропадет
      delay(10);
    }
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}
