/*
    Este programa destina-se a simular um receiver em uma USART,
    recebendo caractere a caractere se houver uma porta serial enviando.
*/
char caractereRecebido;

void setup() {
  Serial.begin(9600);                     // Inicia a comunição serial
}

void loop() {
  while (Serial.available())              // testa se algum dispositvo está enviando sinal serial
  {
    caractereRecebido = Serial.read();    // lê o caractere enviado via porta serial
  }
  Serial.println(caractereRecebido);      // printa caractere recebido
  delay(400);
}
