#include <Wire.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define pot 34                
#define leituras 10           
#define led 26                 
#define canalPWM 0            
#define frequenciaPWM 1000    
#define resoluPWM 10          

// Variáveis globais
int valorAnalogico;
int percPWM;

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Endereço I2C e tamanho do LCD

void atualizaPWM() {
  percPWM = map(valorAnalogico, 0, 1023, 0, 100);     // Percentual para LCD
  ledcWrite(canalPWM, valorAnalogico);                // Brilho do LED
}
void atualizaLcd() {
  lcd.setCursor(0, 1);
  lcd.print(percPWM);
  lcd.print("%   ");  // Apaga sobras de valores antigos (ex: de 100% para 9%)
}
void leituraAnalogica() {
  valorAnalogico = 0;
  for (int i = 0; i < leituras; i++) {
    valorAnalogico += analogRead(pot);
    delay(2);
  }
  valorAnalogico /= leituras;
}

void setup() {
  lcd.init();              // Inicializa o LCD
  lcd.backlight();         // Liga a luz de fundo do LCD
  lcd.setCursor(0, 0);
  lcd.print("LUZ LED");

  analogReadResolution(10);                  // Resolução de leitura analógica
  analogSetPinAttenuation(pot, ADC_11db);    // Atenuação para leitura (ESP32)

  ledcSetup(canalPWM, frequenciaPWM, resoluPWM);  // Configura PWM
  ledcAttachPin(led, canalPWM);                   // Associa pino ao canal PWM
}

void loop() {
  leituraAnalogica();  // Lê o potenciômetro
  atualizaPWM();       // Atualiza brilho do LED
  atualizaLcd();       // Mostra no display
}



