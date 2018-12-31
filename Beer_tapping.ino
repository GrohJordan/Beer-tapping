/*
Copyright (c) 2018, Jordan Groh.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

int ledPin = 3;
int taste = 4;
int druck = A0;
int dauer = 2000;

int durchschnitt = 0;
int trigger = abs(analogRead(druck) - durchschnitt);
int ausloesen = 2;
int ausgeloest = false;

unsigned long time;
#define Dtime 250       //Zeit Zwischen Werten (in µs) 

#define wert2N 9
int  wert[513];         //wertarray
int  werti=0;           //wertzaehler
int  wertN=1<<wert2N;   //anzahl der Werte
int  wertT = 0;         //wert beim triggern
int  wertS = wertN;     //stopwert nach dem trigger
int  wertPT = wertN>>2; //pretrigger

int var = 0;
long sum = 0;


void setup() {
 Serial.begin(115200);
 pinMode(ledPin,OUTPUT);
 pinMode(taste,INPUT);
 digitalWrite(ledPin, LOW);
 var = 0;
// Serial.println("------------");
// Serial.println(wertN);
// Serial.println("------------");
 delay(1000); 
}

void loop() {

switch(var) {

  case 0:                             //durchschnitt bestimmen
//Serial.println("------------");
//   Serial.println("case 0");
//Serial.println("------------");
    while(werti < wertN){
      wert[werti] = analogRead(druck);
      werti++;}
    werti = 0;
    while(werti < wertN){
      sum = sum + wert[werti];
      werti++;
//      Serial.println(sum); 
      }
    durchschnitt = sum>>wert2N;
// Serial.println("------------");
//    Serial.println(durchschnitt);
// Serial.println("------------");
    var++;
    break;  

  
  case 1:                             //scharf stellen
//Serial.println("------------");
//    Serial.println("case 1 ");
//Serial.println("------------");
   if (digitalRead(taste)==HIGH){
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    var++;}
    break;
       
  case 2:                             //auf trigger warten -> (wertN-wertPT)weriter Werte aufzeichnen
/*Serial.println("------------");
//  Serial.println("case 2");
//Serial.println("------------");*/
    digitalWrite(ledPin, HIGH);
    while(ausgeloest == false){
      time = micros();
      wert[werti] = analogRead(druck);
      wertT = werti;
      werti++;
      if(werti >= wertN){werti = 0;}
      delayMicroseconds(Dtime-(micros()-time));         
      trigger = abs(analogRead(druck) - durchschnitt);
      if(trigger > ausloesen){
        ausgeloest = true;
        wertS = wertT - wertPT;
        if(wertS > wertN-1){wertS = wertS-wertN;}
// Serial.println("------------");
//  Serial.println("ausgelöst");
//      Serial.println(wertT);
//        Serial.println(wertS);     
//      Serial.println(werti);
// Serial.println("------------");
      }
    }
    var++;
  break;  

  
  case 3:                             //Werte uebertragen
/*Serial.println("------------");
     Serial.println("case 3 ");
Serial.println("------------");*/
     while(werti != wertS){
      time = micros();
      wert[werti] = analogRead(druck);
      werti++;
      if(werti >= wertN){werti = 0;}
      delayMicroseconds(Dtime-(micros()-time));
      }
     while(werti < wertN){
//          Serial.print(werti);Serial.print(" ");
          Serial.println(wert[werti]);
          werti++;}  
     werti=0;
     while(werti < wertS){
//          Serial.print(werti);Serial.print(" ");
          Serial.println(wert[werti]);
          werti++;}  
    digitalWrite(ledPin, LOW);
    ausgeloest = false;
    werti=0;
    sum=0;
    var=0;
    delay(1000);
    break;  
  }
}
