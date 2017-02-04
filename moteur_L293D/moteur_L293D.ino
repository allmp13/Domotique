int motor1_enablePin = 11; //pwm
int motor1_in1Pin = 13;
int motor1_in2Pin = 12;
 
int motor2_enablePin = 10; //pwm
int motor2_in1Pin = 8;
int motor2_in2Pin = 7;
int bp=4;
int ETAT_BP;
boolean sens = 1;
 
void setup()
{
  //on initialise les pins du moteur 1
  pinMode(motor1_in1Pin, OUTPUT);
  pinMode(motor1_in2Pin, OUTPUT);
  pinMode(motor1_enablePin, OUTPUT);
 
  //on initialise les pins du moteur 2
  pinMode(motor2_in1Pin, OUTPUT);
  pinMode(motor2_in2Pin, OUTPUT);
  pinMode(motor2_enablePin, OUTPUT);
  Serial.begin(9600);
  pinMode(bp, INPUT);
}
 
void loop()
{
  
  int valeur = analogRead(0);
  ETAT_BP=digitalRead(bp);
  if (ETAT_BP==1) 
  {
    sens=!sens;
    delay(500);
  }
  valeur = map(valeur, 0, 1023, 0, 255);
  Serial.print(sens);
  Serial.print("     ");
  Serial.println(valeur);
  SetMotor1(valeur, sens);
}
 
//Fonction qui set le moteur1
void SetMotor1(int speed, boolean reverse)
{
  analogWrite(motor1_enablePin, speed);
  digitalWrite(motor1_in1Pin, ! reverse);
  digitalWrite(motor1_in2Pin, reverse);
}
 
//Fonction qui set le moteur2
void SetMotor2(int speed, boolean reverse)
{
  analogWrite(motor2_enablePin, speed);
  digitalWrite(motor2_in1Pin, ! reverse);
  digitalWrite(motor2_in2Pin, reverse);
}
