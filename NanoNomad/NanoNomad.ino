#include <Wire.h>

int enA = 10;
int in1 = 9;
int in2 = 8;
int in3 = 7;
int in4 = 6;
int enB = 5;
int sensor1 = 12;
int sensorState; // Declare sensorState variable
unsigned long starttime;
unsigned long endtime;

const int irLeft_A = A0; // analog input
const int irLeft_D = 12; // digital input

const int irRight_A = A0; // analog input
const int irRight_D = 13; // digital input

void setup() {
 pinMode(sensor1, INPUT);
 pinMode (irLeft_A, INPUT);
 pinMode (irLeft_D, INPUT);

 // Set up other pins
 pinMode(enA, OUTPUT);
 pinMode(in1, OUTPUT);
 pinMode(in2, OUTPUT);
 pinMode(in3, OUTPUT);
 pinMode(in4, OUTPUT);
 pinMode(enB, OUTPUT);

digitalWrite(enA, 75); 
digitalWrite(enB, 75);

  Serial.begin(9600);

}

void forward() {
/*analogWrite(enA, 75);
 digitalWrite(in1, LOW);
 digitalWrite(in2, HIGH);
 analogWrite(enB, 75);
 digitalWrite(in3, LOW);
 digitalWrite(in4, HIGH);*/
   
 digitalWrite(in1, LOW);
 digitalWrite(in2, HIGH);
 digitalWrite(in3, HIGH);
 digitalWrite(in4, LOW);
}

void left() {
/* analogWrite(enA, 100);
 digitalWrite(in1, LOW);
 digitalWrite(in2, HIGH);
 digitalWrite(in3, HIGH);
 digitalWrite(in4, LOW);
 analogWrite(enB, 100);*/

 digitalWrite(in1, LOW);
 digitalWrite(in2, HIGH);
 digitalWrite(in3, LOW);
 digitalWrite(in4, HIGH);
}

void right() {
/*   analogWrite(enA, 100); // Stop motor A
 analogWrite(enB, 100);
 digitalWrite(in1, HIGH);
 digitalWrite(in2, LOW);
 digitalWrite(in3, LOW);
 digitalWrite(in4, HIGH); */

 digitalWrite(in1, HIGH);
 digitalWrite(in2, LOW);
 digitalWrite(in3, HIGH);
 digitalWrite(in4, LOW);
}

void stopMotors() {
 analogWrite(enA, 0);
 analogWrite(enB, 0);
}

//int valueIrLeft_A;
bool valueIrLeft_D;
bool valueIrRight_D;

void loop() {

  //valueIrLeft_A = analogRead(irLeft_A); // reads the analog input from the IR distance sensor
  valueIrLeft_D = digitalRead(irLeft_D);// reads the digital input from the IR distance sensor
  valueIrRight_D = digitalRead(irRight_D);// reads the digital input from the IR distance sensor

  // Step 1: Get the state of the sensor at the beginning of the loop
  sensorState = digitalRead(sensor1);

  if((valueIrLeft_D == 1)&&(valueIrRight_D == 1)){
    forward();
    }   //if Right Sensor and Left Sensor are at White color then it will call forword function
  if((valueIrLeft_D == 0)&&(valueIrRight_D == 1)){
    left();
    }   
  if((valueIrLeft_D == 1)&&(valueIrRight_D == 0)){
    right();
    } 
  if((valueIrLeft_D == 0)&&(valueIrRight_D == 0)){
    stopMotors();
    }     

/*   //valueIrLeft_A = analogRead(irLeft_A); // reads the analog input from the IR distance sensor
  valueIrLeft_D = digitalRead(irLeft_D);// reads the digital input from the IR distance sensor

 // Step 1: Get the state of the sensor at the beginning of the loop
 sensorState = digitalRead(sensor1);

 // Step 2: Decide what to do based on the input from the sensor
 if (valueIrLeft_D == 1 && valueIrRight_D == 1) {

   // This means that the IR sensor detects the black line
   forward();
 }

 else {
   //test left
   starttime = millis();
   left();

   while (millis() - starttime < 250){
       if (valueIrLeft_D == 1){
           break;
       }
   }
 
   //test right
   if (valueIrRight_D == 1){
       return;
   }

   else{
       starttime = millis();
       right();

       while (millis() - starttime < 250){

           if (valueIrRight_D == 1){

               break;

           }

       }

   } 

 } */

}