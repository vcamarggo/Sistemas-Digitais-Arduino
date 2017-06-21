#include <EEPROM.h>      
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9     // Pino do reset
#define SS_PIN 10     // Pino do slave select

MFRC522 leitor(SS_PIN, RST_PIN); // Classe e variavel do leitor MFRC522

byte storedCard[4];   // Guarda a id lida da EEPROM
byte cartaoLido[4];   // Guarda a id lida do RFID
byte cartaoMestre[4] = {0xE3, 0x67, 0x3C, 0x5B};   // Numero do cartao Master

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  leitor.PCD_Init(); // Init leitor

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
    abrirPorta();
  } else {
    Serial.println("Cartao nao cadastrado. Favor inserir o cartao mestre");
  }

}

void trataMaster() {
  Serial.println("Insira o cartao nao-mestre: ");
  delay(1500);
  while (!getID() || eMestre(cartaoLido));             
  if (!encontraID(cartaoLido))
    escreveID(cartaoLido);
  else {
    apagaID(cartaoLido);
  }
}

void abrirPorta() {
}

uint8_t getID() {
  if ( ! leitor.PICC_IsNewCardPresent()) {
    return 0;
  }
  if ( ! leitor.PICC_ReadCardSerial()) {
    return 0;
  }
  for ( uint8_t i = 0; i < 4; i++) {
    cartaoLido[i] = leitor.uid.uidByte[i];
    Serial.print(cartaoLido[i], HEX);
  }
  Serial.println("");
  leitor.PICC_HaltA(); // Stop reading
  return 1;
}

void leID( uint8_t number ) {
  uint8_t start = (number * 4 ) + 1;
  for ( uint8_t i = 0; i < 4; i++ ) {
    storedCard[i] = EEPROM.read(start + i);
  }
}

void escreveID( byte a[] ) {
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

boolean encontraID( byte find[] ) {
  uint8_t count = EEPROM.read(0);
  for ( uint8_t i = 0; i <= count; i++ ) {
    leID(i);
    if ( testaChaves( find, storedCard ) ) {
      return true;
    }
  }
  return false;
}


///////////////////////////////////////// Check Bytes   ///////////////////////////////////
boolean testaChaves ( byte a[], byte b[] ) {
  boolean match = true;
  if ( a[0] != 0 )      
    for ( uint8_t k = 0; k < 4; k++ ) {    
      if ( a[k] != b[k] )    
        match = false;
    }
  return match;
}

///////////////////////////////////////// Remove ID from EEPROM   ///////////////////////////////////
void apagaID( byte a[] ) {
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
    slot = encontraIDSLOT( a );   
    start = (slot * 4) + 1;
    looping = ((num - slot) * 4);
    num--;     
    EEPROM.write( 0, num );   
    for ( j = 0; j < looping; j++ ) {          
      EEPROM.write( start + j, EEPROM.read(start + 4 + j));   
    }
    for ( int k = 0; k < 4; k++ ) {         
      EEPROM.write( start + j + k, 0);
    }
    Serial.println(F("ID removido da EEPROM"));
  }
}

///////////////////////////////////////// Find Slot   ///////////////////////////////////
int encontraIDSLOT( byte find[] ) {
  int count = EEPROM.read(0);       
  for ( int i = 0; i <= count; i++ ) {     
    leID(i);              
    if ( testaChaves( find, storedCard ) ) {    
      return i;         
    }
  }
}

////////////////////// Check cartaoLido IF is cartaoMestre   ///////////////////////////////////
boolean eMestre( byte test[] ) {
  return testaChaves( test, cartaoMestre );
}

