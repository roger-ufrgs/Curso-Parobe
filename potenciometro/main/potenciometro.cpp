/*
    @Author : Roger Moraes de Moura
    @Version: 1.0.1
    @Data   : 20/08/2025

*/

#include <Arduino.h>
#include "LiquidCrystal_I2C.h"

// Inicializa LCD no endereço I2C 0x27 com 16 colunas e 2 linhas
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pino do potenciômetro
#define potPin 34
// Constantes
#define sensorPino 34
#define leituras 50

// Variáveis
long sensor;
float temperatura;

void lcdConfig(){
  lcd.init();      
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Testando A/D");
}

void ADConfig(){
   // configurações do A/D
    //analogSetAttenuation (ADC_0db);  // Para todos os pinos
  // Atenuação: ADC_0db, ADC_2_5db, ADC_6db, ADC_11db
  // 0dB: 100 - 950mV # 2.5dB: 100 - 1250mV # 6dB: 150 - 1750mV # 11dB: 150 - 2450mV
 // Atenuação de 6db no pino sensorPino
  //analogSetCycles(16); // define o número de ciclos por amostra. O padrão é 8. Intervalo: 1 a 255.
  //analogSetSamples(1); // define o número de amostras no intervalo. O padrão é 1 amostra. Tem um efeito de aumentar a sensibilidade
  analogReadResolution ( 12 );
  analogSetPinAttenuation (sensorPino, ADC_6db);
}


void setup() {
  Serial.begin(115200);
  ADConfig();
  lcdConfig();

}
void loop() {
  // Leitura do sensor
  sensor = 0;  // zera as leituras
  for (int i = 0; i < leituras; i++) {
    sensor = sensor + analogRead(sensorPino); // acumula as leituras
    delay(1); // tempo para o conversor
  }
  sensor = sensor / leituras; // calcula a média
  float volts =( ((float)sensor / 4095) * 3300) / 1000; // Calcula o valor obtido do conversor para mV e converte paara Volts
 char buffer[10];  // cria um buffer para armazenar o número formatado
dtostrf(volts, 7, 3, buffer); 

lcd.setCursor(0, 2);
lcd.print(buffer);
lcd.print(" v");
  delay(1000);  
}



extern "C" void app_main() {
    initArduino();  
    setup();
    while (true) {
        loop();
        delay(1);  // Pequeno delay para evitar watchdog
    }
}
