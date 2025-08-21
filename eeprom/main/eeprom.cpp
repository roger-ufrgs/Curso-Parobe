#include <Wire.h>                   // Comunicação I2C
#include <LiquidCrystal_I2C.h>      // LCD 16x2 com I2C
#include <EEPROM.h>                 // EEPROM para salvar valor

#define EEPROM_SIZE 10              // Tamanho da EEPROM
#define ENDERECO 0                  // Endereço onde salva o número

// Pinos dos botões
#define BT_INCREMENTO 27
#define BT_DECREMENTO 14
#define BT_GRAVAR 12
#define BT_LEITURA 13
#define BT_RESET 26

int numero = 0;                     // Valor atual do contador

LiquidCrystal_I2C lcd(0x27,20,4);  // Instância do LCD

void testaBotaoMais() {
    if (numero < 999) numero++;
}

void testaBotaoMenos() {
  if (numero > 0) numero--;
}

void testaBotaoGrava() {
  int valorSalvo;
    EEPROM.get(ENDERECO, valorSalvo);           // Lê valor salvo
    if (valorSalvo != numero) {                 // Só grava se for diferente
      EEPROM.put(ENDERECO, numero);             // Grava novo valor
      EEPROM.commit();                          // Confirma gravação
      lcd.clear();
      lcd.print("gravando...");
    } else {
      lcd.clear();
      lcd.print("ja gravado");
    }
    delay(2000); lcd.clear();                   // Aguarda e limpa tela
}

void verifica(int bt, void (*funcao)()){
  if(digitalRead(bt) == LOW){
      while(digitalRead(bt) == LOW);
      funcao();
  }
}

void testaBotaoLe() {
    EEPROM.get(ENDERECO, numero);               // Lê valor da EEPROM
    lcd.clear();
    lcd.print("lendo......");
    delay(2000); lcd.clear();                   // Aguarda e limpa tela
}

void testaBotaoReset() {
  numero = 0;                                 // Zera o valor
 }

void mostraLcd() {
  lcd.setCursor(0, 0); lcd.print("D:");         // Mostra decimal
  lcd.print(numero, DEC); lcd.print("  ");
  
  lcd.setCursor(0, 2); lcd.print("H:");         // Mostra hexadecimal
  lcd.print(numero, HEX); lcd.print(" "); 

  lcd.setCursor(0, 1); lcd.print("B:");         // Mostra binário
  lcd.print(numero, BIN); lcd.print("       ");
}

void lcdConfig(){
  lcd.init();                             
  lcd.backlight();                        
}

void setup() {
  EEPROM.begin(EEPROM_SIZE);              // Inicia EEPROM
  lcdConfig();
  pinMode(BT_INCREMENTO, INPUT_PULLUP);  // Botão de incremento
  pinMode(BT_DECREMENTO, INPUT_PULLUP); // Botão de decremento
  pinMode(BT_GRAVAR, INPUT_PULLUP); // Botão para gravar
  pinMode(BT_LEITURA, INPUT_PULLUP);    // Botão para ler
  pinMode(BT_RESET, INPUT_PULLUP); // Botão para resetar
}

void loop() {
  verifica(BT_INCREMENTO,testaBotaoMais);
  verifica(BT_DECREMENTO,testaBotaoMenos);   
  verifica(BT_DECREMENTO,testaBotaoMenos);
  verifica(BT_GRAVAR,testaBotaoGrava);  
  verifica(BT_LEITURA,testaBotaoLe);
  verifica(BT_RESET,testaBotaoReset);     
  mostraLcd(); 
}
extern "C" void app_main() {
    initArduino();  
    setup();
    while (true) {
        loop();
        delay(1);  // Pequeno delay para evitar watchdog
    }
}