/*
  Este programa destina-se a simular um slave do protocolo de comunição TWI.
  Ele opera por um loop no qual a cada 300ms ele realiza uma operação (5x escrita - 5x leitura)
*/
#include <Wire.h>

//Variável de controle do loop
int nrLoop = 1;

void setup() {
  Wire.begin(8);                  // inicia o protocolo i2c na porta 8
  Serial.begin(9600);             // Inicia a saída serial
  Wire.onRequest(requestEvent);   // onRequeste, execute o método passado no parâmetro
}

void loop() {

  //Controla a leitura após as cinco primeiras escritas
  if (nrLoop > 5) {
    Serial.print(nrLoop);
    Wire.requestFrom(9, 12);      // requisita 11 bytes do dispositvo 9
    while (Wire.available()) {
      char c = Wire.read();       // converte o byte recebido em caractere
      Serial.print(c);            // printa o caractere recebido, um a um
    }
    Serial.println();             // printa linha em branco
    delay(300);
  } else Serial.println(nrLoop);  // printa o numero sem mensagem caso esteja recebendo

  nrLoop++;                       // incrementa o contador


  if (nrLoop > 10) {
    nrLoop = 1;                   // reseta o contador
  }

  delay(300);
}

// Função escreve na saída serial uma mensagem
// ela responde a um evento após ser setada onRequest() em setup()
void requestEvent() {
  Wire.write(" I'm slave ");      // Escreve uma mensagem qualquer,
                                  // sua mudança altera a expectativa de recepção de bytes do master
}
