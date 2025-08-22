#include <Wire.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define POT_PIN   25
#define LED_PIN   26

#define PWM_CANAL      0
#define PWM_FREQ       5000
#define PWM_RESOLUCAO  8

int leituraPot = 0;
int dutyPercentual = 0;
int dutyValor = 0;
int configuracao = 0;

void lcdConfig(){
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Iniciando o Sistema!");
    lcd.setCursor(0,1);
    lcd.print(configuracao ? "Config Ok." : "Config Fail.");
    delay(3000);
    lcd.clear();
}

void setup(){
    pinMode(LED_PIN,OUTPUT);
    configuracao = ledcAttachChannel(LED_PIN,PWM_FREQ,PWM_RESOLUCAO,PWM_CANAL);
    lcdConfig();    
}
void loop() {
    leituraPot = analogRead(POT_PIN);
    dutyPercentual = map(leituraPot,0,4095,0,100);
    // Converte para o valor PWM
    dutyValor = map(dutyPercentual,0,100,0,255);
    ledcWrite(LED_PIN,dutyPercentual);
    lcd.setCursor(0,0);
    lcd.print(dutyPercentual);
    lcd.print(" %");
    lcd.setCursor(0,1);
    lcd.print(dutyValor);
    delay(1000);
    lcd.clear();

    

}

// Ponto de entrada principal do ESP-IDF com Arduino
extern "C" void app_main() {
  initArduino();  
  setup();
  while (true) {
    loop();
    delay(1);  // Evita o watchdog
  }
}
