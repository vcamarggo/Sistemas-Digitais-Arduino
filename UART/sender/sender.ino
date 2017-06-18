/*
  Este programa destina-se a simular um sender em uma USART, enviando todos os caracteres entre o caractere 33 até o 127 da tabela ASCII
*/
void setup()
{
  Serial.begin(9600);                               // Inicia a comunição serial
}

void loop()
{
  static char caractereEnviado = 33;                // inicia com o caractere espaço
  
  Serial.print(caractereEnviado);                   // enviado o caractere via porta serial

  if(caractereEnviado == 127) caractereEnviado=32;  // se caractere é o último asc printável, reseta
  caractereEnviado = caractereEnviado + 1;          // incrementa caractere

  delay(400);
}
