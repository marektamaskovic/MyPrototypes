#include <Servo.h>
#include "LiquidCrystal.h"


struct sensor{
	int minimum,maximum;
	bool avl;
};
sensor sensors[4];

const int trigPin = 23;
const int echoPin = 25;

LiquidCrystal lcd(22,24,26,28,30,32);
Servo head;

int motor [4][3] = {	
						{5 , 2, 4},
						{3 , 0, 1},
						{9 , 8, 11},
						{10, 6, 7}
				};

int sensors[4];
bool flag;


void motorControl(int enable,int input2,int input1,int speed){
	//Single motor controll function
	if (speed>0) {
		digitalWrite(input1,HIGH);
		digitalWrite(input2, LOW);
	}
	else if (speed<0){
		digitalWrite(input1, LOW);
		digitalWrite(input2, HIGH);
	}
  	else{
    	digitalWrite(input1, LOW);
    	digitalWrite(input2, LOW);
  	}

	analogWrite(enable, abs(speed));
}

void moveControl(int left, int right){
	motorControl(motor[0][0],motor[0][1],motor[0][2],left);
	motorControl(motor[1][0],motor[1][1],motor[1][2],right);
	motorControl(motor[2][0],motor[2][1],motor[2][2],left);
	motorControl(motor[3][0],motor[3][1],motor[3][2],right);
}

void calibration(){
	//initialize duration of calibration
	randomSeed(analogRead(A8););
	int calibrationTime = random(10, 26);; //20 seconds is calibration time
	int startTime = millis();

  	Serial.print("Calibration started \n calibrationTime: ");
  	Serial.println(calibrationTime);

  	//initialize sensors variable
  	for (int i = 0; i<4; i++){
		sensors[i].min = 1023;
		sensors[i].max = 0;
	}

	//start spinning
	moveControl(150,-150);

	while (((millis()-startTime)/1000) < calibrationTime){
		//inicializovanie senzorov, priemerna hodnota min, max
		int tmp = analogRead(A0);
		if (tmp<sensors[0].minimum)
			sensors[0].minimum=tmp;
		if (tmp>sensors[0].maximum)
			sensors[0].maximum=tmp;
		
		tmp = analogRead(A1);
		if (tmp<sensors[1].minimum)
			sensors[1].minimum=tmp;
		if (tmp>sensors[1].maximum)
			sensors[1].maximum=tmp;
		
		tmp = analogRead(A3);
		if (tmp<sensors[2].minimum)
			sensors[2].minimum=tmp;
		if (tmp>sensors[2].maximum)
			sensors[2].maximum=tmp;
		
		tmp = analogRead(A4);
		if (tmp<sensors[3].minimum)
			sensors[3].minimum=tmp;
		if (tmp>sensors[3].maximum)
			sensors[3].maximum=tmp;
	}

	//stop spinning
	moveControl(0,0);

	for (int i = 0; i < 3; i++){ 
		sensors;
    	Serial.print(i);
    	Serial.print(": ");
    	Serial.print(sensors[i].minimum);
    	Serial.println(sensors[i].maximum)
	}

}

void measure(){
	//first
	if ((analogread(A0)<=sensors[0].maximum) && ( analogread(A0)>=sensors[0].minimum)){
		sensors[0].avl = true;
	}
	else{
		sensors[0].avl = false;
	}

	//second
	if ((analogread(A1)<=sensors[1].maximum) && ( analogread(A1)>=sensors[1].minimum)){
		sensors[1].avl = true;
	}
	else{
		sensors[1].avl = false;
	}

	//third
	if ((analogread(A3)<=sensors[2].maximum) && ( analogread(A3)>=sensors[2].minimum)){
		sensors[2].avl = true;
	}
	else{
		sensors[2].avl = false;
	}

	//fourth
	if ((analogread(A4)<=sensors[3].maximum) && ( analogread(A4)>=sensors[3].minimum)){
		sensors[3].avl = true;
	}
	else{
		sensors[3].avl = false;
	}

}


void showReadings(){
	//IR readings
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(analogRead(A3));
	lcd.setCursor(16,0);
	lcd.print(analogRead(A4));
	lcd.setCursor(16,3);
	lcd.print(analogRead(A1));
	lcd.setCursor(0,3);
	lcd.print(analogRead(A0));

	//servo and SRF05 sensor readings
	lcd.setCursor(6,0);
	lcd.print("cm: ");
	
	long dist = distance();

	if (dist <> 0) lcd.print(dist)
	else lcd.print("Na");

	lcd.setCursor(5,1);
	lcd.print("servo: ");
	lcd.print(head.read());
}

int distance(){
	long duration, inches, cm;

  	digitalWrite(trigPin, HIGH);
  	delayMicroseconds(10);
  	digitalWrite(trigPin, LOW);

  	duration = pulseIn(echoPin, HIGH,2000); // 2000 is 45cm

  	return duration/29/2;

	//2000 is <0 <-> 35 / 40>cm

}

void setup(){
	head.attach(A2);
	head.write(88); // 97 default 180 left 30 right
	lcd.begin(20,4);
	lcd.clear();
	lcd.print("hi there");
	Serial.begin(9600);

  	pinMode(A0, INPUT);
  	digitalWrite(A0,HIGH); //pull-up for nondegenerating data
  	pinMode(A1, INPUT);
  	pinMode(A3, INPUT);
  	pinMode(A4, INPUT);

  	for (int i = 0;i<11;i++){
  		pinMode(i, OUTPUT);
  		digitalWrite(i, LOW);
  	}

  	pinMode(trigPin, OUTPUT);
  	pinMode(echoPin, INPUT);
  	digitalWrite(trigPin, LOW); 

  	//calibration
  	calibration();
}

void loop(){

	
	// recalibration
	if (caliButton){
		calibration();
	}

	Serial.print(millis());

	showReadings();

	measure();
	long dist = distance();

	//logic
	// if is everything ok and distance is in good shape then go straight
	if (sensors[0].avl && sensors[1].avl && ((dist >= 30) || (dist == 0)) ) {
		moveControl(255,255);
	}

	//front left side is not on table
	if (!sensors[0].avl && sensors[1].avl && sensors[4].avl) {

		head.write(180);
		dist = distance();

		if ((dist>=35) || (dist == 0))
		moveControl(-255,255);

		if(sensors[0].avl){
			head.write(97);   
		}
	}

	//front left side is not on table
	if (sensors[0].avl && !sensors[1].avl && sensors[3].avl) {
		
		head.write(30);
		dist = distance();

		if ((dist>=35) || (dist == 0))
		moveControl(255,-255);

		if(sensors[0].avl){
			head.write(97);   
		}
	}

	//front side is not on table and we need to go backwards

	while (!sensors[0].avl && !sensors[1].avl && sensors[2].avl && sensors[3].avl){
		motorControl(-150,-150);
		delay(100);
		measure();
		if (sensors[0].avl && sensors[1].avl) flag = true;
	}

	// after reverse spin 
	if (flag) {
		randomSeed(analogRead(A8));
		long randNum = random(0, 2);
		if (randNum == 0) {
			motorControl(-255,255);
		}
		else {
			motorControl(255,-255);
		}
		flag = false;
		delay(750);
	}

	// error because vehicle is not on table or recalibrate!
	if !(sensors[0].avl && sensors[1].avl && sensors[2].avl && sensors[3].avl) {
		lcd.setCursor(4,3);
		lcd.print("ERROR");
	}


	Serial.print(" - ");
	Serial.println(millis());
}