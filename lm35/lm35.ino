//////////////////////////////////////////////////////
// Objet  : Mesure de temperature avec un LM35 DZ    //
// lm35_phm_temperature.ino                          //
// Auteur : PHM                                      //
// Date création : Le 10 juin 2011                   //
// Date dernière mise à jour : Le 27 juin 2011       //
///////////////////////////////////////////////////////

//Déclaration et initialisation des variables
  float mesure=0;        //Variable pour le stockage mesure retournée par le capteur
  float tension=0;       //Variable pour le stockage de la tension 
  float temperature=0;   //Variable pour le stockage de la température 
  float tempmin=100;     //Temperature min
  float tempmax=0;       //Temperature max
  int portana0 = 5;      //Numéro du port analogique sur lequel la température est mesurée

//Traitements initialisation
  void setup()
  {
    analogReference(INTERNAL); //Permet de fixer la temperature de refernce à 1,1 volt
    Serial.begin(9600);        //initialisation de la liaison série à 9 600 bauds
  }

//Traitements itératifs
  void loop()
  {
    mesure = analogRead(portana0);  //Lecture de la valeur fournie par le capteur de température
    tension = mesure * 1.1 / 1024;  //Conversion en tension (en volt)
    
    temperature = tension * 100;    //Conversion en température (en degré Celsius)
    if (temperature < tempmin) {tempmin = temperature;} //Calcul temperature mini
    if (temperature > tempmax) {tempmax = temperature;} //Calcul temperature maxi
    
    //Envoi de la tension et des températures au PC par la liaison série pour affichage dans la console
    Serial.print("Tension : "); Serial.print (tension); Serial.print(" Volts - ");         
    Serial.print("Temperatures :"); 
    Serial.print(" >Actuelle : "); Serial.print (temperature); 
    Serial.print(" >Mini : "); Serial.print (tempmin); 
    Serial.print (" >Max : "); Serial.println (tempmax);
    //Attente de 5 secondes avant de faire une nouvelle mesure
    delay(5000);                    
  }
  
  
