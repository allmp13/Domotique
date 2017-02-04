
#define VOLET_HAUT 2
#define VOLET_STOP 3
#define VOLET_BAS 4
#define VOLET_CHANGE 5

int photocellPin = 0; // the cell and 10K pulldown are connected to a0
int lum; // the analog reading from the analog resistor divider
void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);
  pinMode(VOLET_HAUT, OUTPUT);  
  pinMode(VOLET_STOP, OUTPUT);
  pinMode(VOLET_BAS, OUTPUT);
  pinMode(VOLET_CHANGE, OUTPUT);  
}

void loop(void) {
  lum = analogRead(photocellPin);
  Serial.print("initial Analog reading = ");
  Serial.println(lum); // the raw analog reading
  if (lum <400 || lum > 450)
  {
    if ( lum < 400)
    {
      digitalWrite(VOLET_HAUT, HIGH);   // set the LED on
      delay(100);              // wait for a second
      digitalWrite(VOLET_HAUT, LOW);    // set the LED off 
      do
      {
        lum = analogRead(photocellPin);
          Serial.print("Analog reading Monte = ");
          Serial.println(lum); // the raw analog reading
      } while (lum < 425);        
    }
      if ( lum > 450)
    {
      digitalWrite(VOLET_BAS, HIGH);   // set the LED on
      delay(100);              // wait for a second
      digitalWrite(VOLET_BAS, LOW);    // set the LED off 
      do
      {
        lum = analogRead(photocellPin);
         Serial.print("Analog reading Baisse = ");
         Serial.println(lum); // the raw analog reading
      } while (lum > 425);        
    }
  digitalWrite(VOLET_STOP, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_STOP, LOW);    // set the LED off
  }
  
  delay(2000);
  
  
  /*
  do
  {
  if ((t == 0) && (lum > 800))
  {
  digitalWrite(VOLET_BAS, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_BAS, LOW);    // set the LED off
  t=1;
  }
  lum = analogRead(photocellPin);
  }while (lum > 800);
  if (t ==1)
  {
  digitalWrite(VOLET_STOP, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_STOP, LOW);    // set the LED off
   t=0;
  }
  */
}
