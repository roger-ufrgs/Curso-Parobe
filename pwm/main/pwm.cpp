#include <Wire.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>


// ======================= LCD =======================
#define LCD_ENDERECO  0x27
#define LCD_COLUNAS   20
#define LCD_LINHAS    4
LiquidCrystal_I2C lcd(LCD_ENDERECO, LCD_COLUNAS, LCD_LINHAS);

// =================== Pinos / HW ===================
#define POT_PIN   25   // Analógico (somente entrada)
#define BT_PIN    18   // Botão (com pull-up interno)
#define PWM_PIN   25   // LED / saída PWM

// =================== PWM ESP32 ====================
#define PWM_CANAL      0
#define PWM_FREQ       5000  // Hz
#define PWM_RESOLUCAO  8     // 0-255

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
    configuracao = ledcAttachChannel(POT_PIN,PWM_FREQ,PWM_RESOLUCAO,PWM_CANAL);
    lcdConfig();

    
}
void loop() {
    

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
