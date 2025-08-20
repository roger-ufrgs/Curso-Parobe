/*
    @Author : Roger Moraes de Moura
    @Version: 1.0.0
    @Data   : 17/08/2025

*/

#include <Arduino.h>
#include "LiquidCrystal_I2C.h"

// Inicializa LCD no endereço I2C 0x27 com 16 colunas e 2 linhas
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pino do potenciômetro
const int potPin = 34;
// Constantes
const int sensorPino = 34;
const int leituras = 50;
// Variáveis
long sensorValor;
float temperatura;

void lcdConfig(){
  lcd.init();      
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sistema Iniciando!");
}


void setup() {
  Serial.begin(115200);
  // configurações do A/D
  analogReadResolution ( 12 ); // resolução de 12 bits
  //analogSetAttenuation (ADC_0db);  // Para todos os pinos
  // Atenuação: ADC_0db, ADC_2_5db, ADC_6db, ADC_11db
  // 0dB: 100 - 950mV # 2.5dB: 100 - 1250mV # 6dB: 150 - 1750mV # 11dB: 150 - 2450mV
  analogSetPinAttenuation (sensorPino, ADC_6db); // Atenuação de 6db no pino sensorPino
  //analogSetCycles(16); // define o número de ciclos por amostra. O padrão é 8. Intervalo: 1 a 255.
  //analogSetSamples(1); // define o número de amostras no intervalo. O padrão é 1 amostra. Tem um efeito de aumentar a sensibilidade
  lcdConfig();

}
void loop() {
  // Leitura do sensor
  sensorValor = 0;  // zera as leituras
  for (int i = 0; i < leituras; i++) {
    sensorValor = sensorValor + analogRead(sensorPino); // acumula as leituras
    delay(1); // tempo para o conversor
  }
  sensorValor = sensorValor / leituras; // calcula a média
  Serial.print(sensorValor);  // imprime a media da conversão
  float millivolts = ((float)sensorValor / 4095) * 3300; // Calcula o valor obtido do conversor para mV
  Serial.print(" -> ");
  Serial.print(millivolts); // imprime o valor em mV
  float temperatura = millivolts / 10;  // calucla a temperatura
  Serial.print("mV -> Temperatura = ");
  Serial.print(temperatura);  // imprime a temperatura
  Serial.println("ºC");
  delay(1000);  // tempo entre cada medida
}



extern "C" void app_main() {
    initArduino();  
    setup();
    while (true) {
        loop();
        delay(1);  // Pequeno delay para evitar watchdog
    }
}
