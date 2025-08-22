#include <Wire.h>                   // Comunicação I2C
#include <LiquidCrystal_I2C.h>      // LCD 16x2 com I2C
#include <EEPROM.h>                 // EEPROM para salvar valor

#define EEPROM_SIZE 10              // Tamanho da EEPROM
#define ENDERECO 0                  // Endereço onde salva o número

// Pinos dos botões
#define BT_INCREMENTO 32
#define BT_DECREMENTO 33
#define BT_GRAVAR 12
#define BT_LEITURA 14
#define BT_RESET 13

int valores[5] = {0, 0, 0, 0, 0};

int numero = 0;
int capacidade = 30;

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
  if(bt < capacidade) funcao();
}

void mediaValores(){
  //faz 100 leituras de cada sensor touch e calcula a média do valor lido
  for(int i=0; i< 100; i++)
  {
    valores[0]+= touchRead(BT_INCREMENTO);
    valores[1]+= touchRead(BT_DECREMENTO);
    valores[2]+= touchRead(BT_GRAVAR);
    valores[3]+= touchRead(BT_LEITURA);
    valores[4]+= touchRead(BT_RESET);
  }
  for (int i = 0; i < 5; i++)   valores[i] = valores[i] / 100;
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
  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(115200);
  lcdConfig();
}

void loop() {
  mediaValores();
  verifica(valores[0],testaBotaoMais);
  verifica(valores[1],testaBotaoMenos);
  verifica(valores[2],testaBotaoGrava);
  verifica(valores[3],testaBotaoLe);
  verifica(valores[4],testaBotaoReset); 
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