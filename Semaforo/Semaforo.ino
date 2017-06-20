// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
#define VERDE1  3
#define VERMELHO1 4
#define AMARELO1 2

#define VERDE2  5
#define VERMELHO2 6
#define AMARELO2 7

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(VERDE1, OUTPUT);
  pinMode(VERMELHO1, OUTPUT);
  pinMode(AMARELO1, OUTPUT);
    pinMode(VERDE2, OUTPUT);
  pinMode(VERMELHO2, OUTPUT);
  pinMode(AMARELO2, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(VERMELHO1, LOW);
  digitalWrite(VERMELHO2, HIGH);
  digitalWrite(VERDE1, HIGH);
  delay(3000);
              
  digitalWrite(VERDE1, LOW);
  digitalWrite(AMARELO1, HIGH);
  delay(3000);
  
   digitalWrite(AMARELO1, LOW);
  digitalWrite(VERMELHO1, HIGH);
   digitalWrite(VERMELHO2, LOW);
   digitalWrite(VERDE2, HIGH);
  delay(3000);
  
  digitalWrite(VERDE2, LOW);
  digitalWrite(AMARELO2, HIGH);

  delay(3000);
  
  digitalWrite(AMARELO2, LOW);   
}
