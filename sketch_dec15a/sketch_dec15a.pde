byte pinUp = 2;      // Bouton Haut de la telecommande
byte pinStop = 3;    // Bouton Start/Stop de la telecommande
byte pinDown = 4;    // Bouton Bas de la telecommande
byte pinSwitch = 5;  // Bouton changement de canal de la telecommande

byte pinLED = 13;    // LED d'activite

int delayms = 100;    // Duree pendant laquelle on reste appuye sur un bouton

void setup()
{
  // Emulation serie a 9600 bauds
  Serial.begin(9600);
  //configuration des sorties vers l'optocoupleur
  pinMode(pinUp,     OUTPUT);
  pinMode(pinStop,   OUTPUT);
  pinMode(pinDown,   OUTPUT);
  pinMode(pinSwitch, OUTPUT);
  // configuration de la sortie de la LED d'activite
  pinMode(pinLED,    OUTPUT);
  // envois d'un charactere pour signaler la fin de l'initialisation
  Serial.println("X");
}

void loop()
{
  char inChar = ' ';
  // Si quelquechose attend sur le port serie, on le lit
  if (Serial.available() > 0) 
  {
    // recuperation du caractere:
    inChar = Serial.read();

    // selon le charactere on active ou desactive des sorties
    switch (inChar) {
    case 'A':
      digitalWrite(pinUp,HIGH);
      digitalWrite(pinLED,HIGH);
      Serial.println(inChar);
      break;
    case 'a':
      digitalWrite(pinUp,LOW);
      digitalWrite(pinLED,LOW);
      Serial.println(inChar);
      break;
    case '1':
      digitalWrite(pinUp,HIGH);
      digitalWrite(pinLED,HIGH);
      delay(delayms);
      digitalWrite(pinUp,LOW);
      digitalWrite(pinLED,LOW);
      Serial.println(inChar);
      break;
    case 'B':
      digitalWrite(pinStop,HIGH);
      digitalWrite(pinLED,HIGH);
      Serial.println(inChar);
      break;
    case 'b':
      digitalWrite(pinStop,LOW);
      digitalWrite(pinLED,LOW);
      Serial.println(inChar);
      break;
    case '2':
      digitalWrite(pinStop,HIGH);
      digitalWrite(pinLED,HIGH);
      delay(delayms);
      digitalWrite(pinStop,LOW);
      digitalWrite(pinLED,LOW);
      Serial.println(inChar);
      break;
    case 'C':
      digitalWrite(pinDown,HIGH);
      digitalWrite(pinLED,HIGH);
      Serial.println(inChar);
      break;
    case 'c':
      digitalWrite(pinDown,LOW);
      digitalWrite(pinLED,LOW);
      Serial.println(inChar);
      break;
      break;
    case '3':
      digitalWrite(pinDown,HIGH);
      digitalWrite(pinLED,HIGH);
      delay(delayms);
      digitalWrite(pinDown,LOW);
      digitalWrite(pinLED,LOW);
      Serial.println(inChar);
      break;      
    case 'D':
      digitalWrite(pinSwitch,HIGH);
      digitalWrite(pinLED,HIGH);
      Serial.println(inChar);
      break;
    case 'd':
      digitalWrite(pinSwitch,LOW);
      digitalWrite(pinLED,LOW);
      Serial.println(inChar);
      break;
    case '4':
      digitalWrite(pinSwitch,HIGH);
      digitalWrite(pinLED,HIGH);
      delay(delayms);
      digitalWrite(pinSwitch,LOW);
      digitalWrite(pinLED,LOW);
      Serial.println(inChar);
      break;            
    }
  }
}



