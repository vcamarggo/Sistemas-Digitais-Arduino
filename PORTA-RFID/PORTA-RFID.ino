#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10     // Pino do slave select
#define RST_PIN 9     // Pino do reset
#define SAIDA_RELAY 8 // Pino do relay
#define LED_VERMELHO 7 // Pino da LED Vermelha
#define LED_VERDE 6    // Pino da LED Verde

MFRC522 leitor(SS_PIN, RST_PIN); // Classe e variavel do leitor MFRC522

byte storedCard[4];   // Guarda a id lida da EEPROM
byte cartaoLido[4];   // Guarda a id lida do RFID
byte cartaoMestre[4] = {0xE3, 0x67, 0x3C, 0x5B};   // Numero do cartao Master

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  leitor.PCD_Init(); // Init leitor
  piscaLedAmbas(2000); //Pisca LEDs para indicar inicio do sistema
}

void loop() {
  Serial.println("Insira o RFID: ");
  while (!getID());

  if (eMestre(cartaoLido)) {
    trataMaster();
    return;
  }

  if (encontraID(cartaoLido)) {
    Serial.println("Cartao cadastrado. Acesso liberado.");
    piscaLedIndividual(LED_VERDE, 1500); // Pisca LEDs para indicar acesso liberado
    abrirPorta();
  } else {
    Serial.println("Cartao nao cadastrado. Favor inserir o cartao mestre");
    piscaLedIndividual(LED_VERMELHO, 1000); // Pisca LEDs para indicar acesso proibido
  }

}

void trataMaster() {
  piscaLedIndividual(LED_VERDE, 500); // Pisca LEDs 3x para indicar modo-mestre
  delay(300);
  piscaLedIndividual(LED_VERDE, 500);
  delay(300);
  piscaLedIndividual(LED_VERDE, 500);

  Serial.println("Insira o cartao nao-mestre: ");
  delay(1500);
  while (!getID() || eMestre(cartaoLido));
  if (!encontraID(cartaoLido))
    escreveID(cartaoLido);
  else {
    apagaID(cartaoLido);
  }

  piscaLedAmbas(600); // Pisca LEDs para indicar fim do modo-mestre
}

void piscaLedIndividual(int LED, int millisegundos) {
  digitalWrite(LED, HIGH);
  delay(millisegundos);
  digitalWrite(LED, LOW);
}

void piscaLedAmbas(int millisegundos) {
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_VERDE, HIGH);
  delay(millisegundos);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, LOW);
}

void abrirPorta() {
  digitalWrite(SAIDA_RELAY, HIGH); // ativa rele, abre a trava
  delay(2000);           // espera 2 segundos
  digitalWrite(SAIDA_RELAY, LOW);  // desativa rele, fecha a trava
}

uint8_t getID() { // Le o ID do cartao no modulo RFID
  if ( ! leitor.PICC_IsNewCardPresent()) { //Se nao ha cartao no leitor
    return 0;
  }
  if ( ! leitor.PICC_ReadCardSerial()) { //Se cartao nao e do padrao esperado
    return 0;
  }
  Serial.print("ID: ");
  for ( uint8_t i = 0; i < 4; i++) {
    cartaoLido[i] = leitor.uid.uidByte[i]; //Le em hexadecimal
    Serial.print(cartaoLido[i], HEX);
  }
  Serial.println("");
  leitor.PICC_HaltA(); // termina a leitura
  return 1;
}

void leID( uint8_t number ) { //Le ID da EEPROM
  uint8_t start = (number * 4 ) + 1;
  for ( uint8_t i = 0; i < 4; i++ ) {
    storedCard[i] = EEPROM.read(start + i);
  }
}

void escreveID( byte a[] ) { //Escreve ID na EEPROM
  if ( !encontraID( a ) ) {
    uint8_t num = EEPROM.read(0);
    uint8_t start = ( num * 4 ) + 1;
    num++;
    EEPROM.write( 0, num );
    for ( uint8_t j = 0; j < 4; j++ ) {
      EEPROM.write( start + j, a[j] );
    }
    Serial.println(F("ID cadastrado na EEPROM"));
  }
  else {
    Serial.println(F("Falha! Ha um problema na ID ou na EEPROM"));
  }
}

boolean encontraID( byte find[] ) { //Encontra se um ID esta na EEPROM
  uint8_t count = EEPROM.read(0);
  for ( uint8_t i = 0; i <= count; i++ ) {
    leID(i);
    if ( testaChaves( find, storedCard ) ) {
      return true;
    }
  }
  return false;
}


boolean testaChaves ( byte a[], byte b[] ) { //testa se dois IDs são iguais
  boolean match = true;
  if ( a[0] != 0 )
    for ( uint8_t k = 0; k < 4; k++ ) {
      if ( a[k] != b[k] )
        match = false;
    }
  return match;
}

void apagaID( byte a[] ) { //Remove ID da EEPROM
  if ( !encontraID( a ) ) {
    Serial.println(F("Falha! Ha um problema na ID ou na EEPROM"));
  }
  else {
    int num = EEPROM.read(0);
    int slot;
    int start;
    int looping;
    int j;
    int count = EEPROM.read(0);
    slot = encontraIDSLOT( a ); //Encontra o slot que o ID esta alocado na EEPROM
    start = (slot * 4) + 1;
    looping = ((num - slot) * 4);
    num--;
    EEPROM.write( 0, num );
    for ( j = 0; j < looping; j++ ) { //Grava os dados do prox no slot anterior, ate atingir o ultimo registro
      EEPROM.write( start + j, EEPROM.read(start + 4 + j));
    }
    for ( int k = 0; k < 4; k++ ) { ///Grava zeros na ultima posicao restante
      EEPROM.write( start + j + k, 0);
    }
    Serial.println(F("ID removido da EEPROM"));
  }
}

int encontraIDSLOT( byte find[] ) { //Encontra o slot que o ID esta alocado na EEPROM
  int count = EEPROM.read(0);
  for ( int i = 0; i <= count; i++ ) {
    leID(i);
    if ( testaChaves( find, storedCard ) ) {
      return i;
    }
  }
}

boolean eMestre( byte test[] ) { //Verifica se cartaoLido e cartaoMestre
  return testaChaves( test, cartaoMestre );
}

