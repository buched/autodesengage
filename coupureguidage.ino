////////////////////////////////////////////////////////////////////////////  
 //  
 // ino pour coupure du guidage AgopenGPS 
 // le seuil se définit avec la position des dip switch sur le pcb
 // credits manu@couvercelle.eu
 //  
 
#include "Adafruit_INA260.h"

const byte Ledetat = 12;
const byte Relay = 7;
unsigned long ampfinal;
unsigned long powermax = 25000; // valeur en milliwatts. exemple 120w = 120000
int method = 1; //0 = jumper, 1 potentiometre, 2 valeur directe
bool button = 1; //0 = steer switch, 1 = steer momentary button or remote pin
// déclaration des variables
volatile int etat = LOW;
unsigned long int reel;

Adafruit_INA260 ina260 = Adafruit_INA260();

void setup() {
  Serial.begin(38400);
  // Wait until serial port is opened
  while (!Serial) { delay(10); }

  Serial.println("Adafruit INA260 Test");

  if (!ina260.begin()) {
    Serial.println("Couldn't find INA260 chip");
    while (1);
  }
  Serial.println("Found INA260 chip");

    // Configure la broche du bouton en entrée

    pinMode(Relay, OUTPUT);
    pinMode(Ledetat, OUTPUT);
    pinMode(A3,INPUT_PULLUP);

}

void loop() {
if (method == 1)
                  {
                        ampfinal = analogRead(A3); // valeur potard 0>1024 = 0 à 204.8 w
                        ampfinal = ampfinal * 200;
                        Serial.print("consigne : ");
                        Serial.println(ampfinal);//pour debug
                  }
                else if (method == 2)
                {
                    ampfinal = powermax; // valeur en milliwatts. exemple 120w = 120000
                    Serial.println(ampfinal);//pour debug
                } 

//gestion de la coupure

digitalWrite(Relay,LOW);
digitalWrite(Ledetat,HIGH);
reel = ina260.readPower();
Serial.print("conso : ");
Serial.println(reel);
//while (digitalRead(authsteer)== LOW) // future option
//  {
     if ((abs(reel)/ 1000) >= ampfinal)
        {
              // Allume la LED en fonction de l'état du bouton
//        etat = 1;
                digitalWrite(Ledetat,LOW);// led allumée
                digitalWrite(Relay,HIGH);// impulsion coupure guidage
                if (button == 1)
                    {
                       delay(500);
                       digitalWrite(Relay,LOW);//retour etat initial
                       digitalWrite(Ledetat,HIGH);// led allumée
                    }
               
        }
//  }
}// fin loop
