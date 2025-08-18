/*
    @Author : Roger Moraes de Moura
    @Version: 1.0.0
    @Data   : 17/08/2025

*/

#include <Arduino.h>
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define LED_1 17
#define LED_2 5

#define BT_1 25
#define BT_2 26

bool estadoLED1 = false;
bool botaoAnterior = HIGH;

//Função para acender o número de vezes o led2
void ligaLedVezes(int vezes){
    for(int i =0; i < vezes; i++){
        digitalWrite(LED_2,HIGH);
        delay(1000);
        digitalWrite(LED_2,LOW);
        delay(300);
    } 
}

// LCD
void lcdConfig(){
  lcd.init();      
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Led desligado");
}

void setup() {
  pinMode(LED_1, OUTPUT);
  pinMode(BT_1, INPUT_PULLUP);
  pinMode(LED_2, OUTPUT);
  pinMode(BT_2, INPUT_PULLUP);
  lcdConfig();
}

void loop() {
  bool leituraAtual = digitalRead(BT_1);

  if (botaoAnterior == HIGH && leituraAtual == LOW) {
    estadoLED1 = !estadoLED1;               
    digitalWrite(LED_1, estadoLED1);         
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(estadoLED1 ? "LED ligado!":"LED desligado!");
    delay(50);
  }
    if(digitalRead(BT_2) == LOW){
        ligaLedVezes(3);
    }
    botaoAnterior = leituraAtual;
}
extern "C" void app_main() {
    initArduino();  
    setup();
    while (true) {
        loop();
        delay(1);  // Pequeno delay para evitar watchdog
    }
}
