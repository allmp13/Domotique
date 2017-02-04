
#define VOLET_HAUT 2
#define VOLET_STOP 3
#define VOLET_BAS 4
#define VOLET_CHANGE 5
 
long pi=0; //position Initiale des volets
long p;


void setup() {                
  // initialize the digital pin as an output.
  pinMode(VOLET_HAUT, OUTPUT);  
  pinMode(VOLET_STOP, OUTPUT);
  pinMode(VOLET_BAS, OUTPUT);
  pinMode(VOLET_CHANGE, OUTPUT);  
  Serial.begin(9600);  
}

void loop() {   
 if (Serial.available())
  {
  p = Serial.parseInt(); 
  pi=volet(p);
  Serial.print(p);
  Serial.print(" % --> ");
  Serial.println(pi);
  }
}

void monte()
{
  Serial.println("Monte");
  digitalWrite(VOLET_HAUT, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_HAUT, LOW);    // set the LED off
}

void baisse()
{
  Serial.println("Baisse");
  digitalWrite(VOLET_BAS, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_BAS, LOW);    // set the LED off
}

void arret()
{
  Serial.println("Arret");
  digitalWrite(VOLET_STOP, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_STOP, LOW);    // set the LED off
}

long volet(long niveau)
{
long deb;
long delta;
 if (niveau >= 0 && niveau <= 100)
 {   
   niveau=niveau*260;
   if (niveau < pi)
   {
    baisse();
    deb=millis();
    do
    {
      delta=millis()-deb;
    }while (niveau <= (pi-delta));
    arret();
  } 
  else if (niveau > pi)
  {
    monte();
    deb=millis();
    do
    {
      delta=millis()-deb;
    }while (niveau >= (pi+delta));
    arret();
  }
 }
   return niveau;
}
