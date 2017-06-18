#include <LiquidCrystal.h>
 

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);              // define os pinos que serão utilizados para ligação ao display
 

String line1 = "Sistemas";                          // line1 = Frase superior - "rolável"
String line2 = "Digitais";                          // line2 = Frase inferior - estatica
  
int stringCursor = 0;                                // flag de controle cursor
int stringStart = line1.length() - 1;                // flag de controle inicio da string
int stringStop = line1.length();                     // flag de controle fim da string
   
void setup() {  
  lcd.begin(16,2);                                    // inicializacao do display
}  
   
void loop() {  
  lcd.setCursor(stringCursor, 0);     
  lcd.print(line1.substring(stringStart,stringStop)); // printa substring da flag inicio ate flag fim
  lcd.setCursor(4, 1);  
  lcd.print(line2);                                   // printa a linha estatica
  
  delay(350);  
  
  scroll_conf();                                      // chama a rotina que configura o scroll
  
  if(stringCursor == 17){
    stringCursor = 0;                                 // reseta flags
    stringStart = line1.length() - 1;
    stringStop = line1.length();
  }
}  
   
void scroll_conf() {  
  lcd.clear();
  if(stringStart > 0){
    stringStart--;                    // decrementa a posição de início
  }else{
    stringCursor++;                   // aumenta a posição do cursor se string inteira já foi printada
  }
  lcd.setCursor(stringCursor,0);
 }  
