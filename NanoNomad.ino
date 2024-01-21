#include <Wire.h>

//motor pins
int enA = 10;//engineA (right)
int in1 = 9;
int in2 = 8;//these two are for A

const int turning_radius = 10;

int in3 = 7;//input is to control direction
int in4 = 6;//these are for B
int enB = 5;//engineB (left)

//const, determine default direction
const bool defaultLeft = true;
//const, fixed values for speed
const int fastSpeed = 75;
const int defaultSpeed = 1;

//for ultrasonic sensor
int trigPin = 4;
int echoPin = 11;
double duration = 0;
int distance = 0;

//IR sensors 
//only digital is used bc we LOVE high contrast. analog is dogshit and useless
const int irLeft_A = A0; // analog input
const int irLeft_D = 12; // digital input

const int irRight_A = A0; // analog input
const int irRight_D = 13; // digital input

//set the tick rate, might be 
const int tickRate = 5;

//Debugging variables
String turningStatus = "No status yet";

//
void setup() {
  //ir sensor
  pinMode (irLeft_A, INPUT);
  pinMode (irLeft_D, INPUT);
  //ultrasonic
  pinMode (trigPin, OUTPUT);
  pinMode (echoPin, INPUT);
  // Set up other pins
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

//sets speed of motors, does not turn them ON 0-255
  digitalWrite(enA, defaultSpeed); 
  digitalWrite(enB, defaultSpeed);

  Serial.begin(9600);
}

void turnDefaultDirection(bool turnLeft) {
  if (turnLeft) {
    left();
  } else {
    right();
  }
}

void avoidWall()
{
  while (true)
    {
      turnDefaultDirection(defaultLeft);
      int distance = getDistance(trigPin, echoPin);
      if (distance > turning_radius + turning_radius/2)
        break;
    }
}

void getAroundObject(){
  bool objectAvoided = false;
  while (true)
  {
    //avoid the object until you cant see
    avoidWall();
    //go forward a bit
    forward();
    delay(500);
    //turn opposite direction and check for obstacle
    turnDefaultDirection(!(defaultLeft));
    delay(1000);//need to change this number to get a 90 degree turn
    //if object still in view, go back
    if (getDistance(trigPin,echoPin) < turning_radius+turning_radius/2)
    {
      avoidWall();
    }
    else//don't see object, go forward and exit loop as if nothing happened
    {
      //make a full 90 degree right turn tho
      break;
    }
  }
}

//RUN THIS FUNCTION FOR THE OBJECT TRAVERSAL LOGIC IDK
void objectTraversalLoop()
{
  //read sensor data
  int distance = getDistance(trigPin, echoPin);
  int lineDetectionState = getLineState();

  //make a decision based on what's ahead
  if (lineDetectionState != 0)
  {
    //execute line following correction code
    if(lineDetectionState == 1){
      left();
      turningStatus = "left";
      }   
    if(lineDetectionState == 2){
      right();
      turningStatus = "right";
      } 
  }

  //if too close
  if (distance < turning_radius) {
    //execute code to go backward a little until it isnt
    backward();
  }
  //if kind of close, should be aware, should turn
  else if (distance < turning_radius + turning_radius/2) {
    //stop, specific code for this might not be necessary
    getAroundObject();
  }
  //very far, just go forward until it gets close
  else {
    forward();
  }
}

//ultrasonic sensor to determine distance
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds (2);
  digitalWrite (trigPin, HIGH);
  delayMicroseconds (10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration*0.034/2;
}

int getLineState(){
  //hold the values of whether or not the IR sensors are triggered
  bool valueIrLeft_D;
  bool valueIrRight_D;
  // Step 1: Get the state of the sensor at the beginning of the loop
  valueIrLeft_D = digitalRead(irLeft_D);// reads the digital input from the IR distance sensor
  valueIrRight_D = digitalRead(irRight_D);// reads the digital input from the IR distance sensor
  //1 = no line
  //0 = line detected
  if((valueIrLeft_D == 0)&&(valueIrRight_D == 0)){
    return 0;
    }   //if Right Sensor and Left Sensor are at White color then it will call forword function
  if((valueIrLeft_D == 1)&&(valueIrRight_D == 0)){
    return 1;
    }   
  if((valueIrLeft_D == 0)&&(valueIrRight_D == 1)){
    return 2;
    } 
  if((valueIrLeft_D == 1)&&(valueIrRight_D == 1)){//this one wont be used probably
    return 3;
    }
}

void mazeLoop() {  //hug right wall algorithm
  //check if something is in front, otherwise get close to the rightmost wall
  while (true) {
    //read sensor data
    int distance = getDistance(trigPin, echoPin);
    if (distance < turning_radius) {
      //execute code to go backward a little until it isnt
      backward(); 
    }
    //if kind of close, should be aware, should turn
    else if (distance < turning_radius + turning_radius / 2) {
      //at good distance, turn away from it properly
      avoidWall();
      break;
    }
    //very far, just go forward until it gets close
    else {
      forward();
    }
  }
  //then run the hug right algorithm
  while (true)  //we going to run this permanently now
  {
    //avoid the object until you cant see
    
    //go forward a bit
    forward();
    delay(500);
    //turn opposite direction and check for obstacle
    turnDefaultDirection(!(defaultLeft));
    delay(1000);  //need to change this number to get a 90 degree turn
    //if object/wall still in view, go back
    if (getDistance(trigPin, echoPin) < turning_radius + turning_radius / 2) {
      //turn back
      turnDefaultDirection(defaultLeft);
      delay(1000); 
    } 
    //if nothing happens then just go forward
  }
}


//drive
void forward() {//go FORWARD
 digitalWrite(enA, defaultSpeed);
 digitalWrite(enB, defaultSpeed);

  //forward for left
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  //because motor is rotated so then we reverse for the right motor
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

}

//reversed the motor movements for forward
void backward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void left() {
 digitalWrite(enA, 0);
 digitalWrite(enB, defaultSpeed);

 digitalWrite(in1, LOW);
 digitalWrite(in2, HIGH);
 digitalWrite(in3, LOW);
 digitalWrite(in4, HIGH);
}

void right() {
 digitalWrite(enA, defaultSpeed);
 digitalWrite(enB, 0);

 digitalWrite(in1, HIGH);
 digitalWrite(in2, LOW);
 digitalWrite(in3, HIGH);
 digitalWrite(in4, LOW);
}

void stopMotors() { //will not be used because the rover will be perfect
 analogWrite(enA, 0);
 analogWrite(enB, 0);
}

//debugging
void printLineStatus(bool leftSensor, bool rightSensor, String turnStatus) {
  Serial.println("Left Sensor: " + String(leftSensor));
  Serial.println("Right Sensor: " + String(rightSensor));
  Serial.println("Turning Status: "+ turnStatus);
}


void loop() {

  //0 = no lines detected
  //1 = line detected from right side
  //2 = line detected from left side
  //3 = line detected from both sides
  //this variable uses the sensors to determine a state
  int lineDetectionState = getLineState();

  
  mazeLoop();

  //check what it sees
  //1 = i see no line (one green light). 0 = i see line (second green light triggers)!!!
  if(lineDetectionState == 0){
    forward();
    turningStatus = "forward";
    }
  if(lineDetectionState == 1){
    left();
    turningStatus = "left";
    }   
  if(lineDetectionState == 2){
    right();
    turningStatus = "right";
    } 
  /* if(lineDetectionState == 3){//this one wont be used probably
    stopMotors();
    turningStatus = "stopped D:";
    } */
  //printLineStatus(valueIrLeft_D, valueIrRight_D, turningStatus);


  //set the rate of pausing
  delay(tickRate);
  
  digitalWrite(enA, 0); 
  digitalWrite(enB, 0); 

  delay(10);

  digitalWrite(enA, defaultSpeed); 
  digitalWrite(enB, defaultSpeed); 

}