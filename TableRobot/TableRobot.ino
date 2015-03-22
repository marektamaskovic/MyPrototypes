#include <Servo.h>
#include <LiquidCrystal.h>

Servo head;
LiquidCrystal lcd(31,33,35,37,39,41);

const int trigPin = 23;
const int echoPin = 25;
int sensors[3];
int motor[4][3] = {	
					{5 , 2, 4},
					{3 , 0, 1},
					{9 , 8, 11},
					{10, 6, 7}
				};



int distance(){
	long duration, cm;
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH);
	cm = duration/29/2;
	return cm;
}

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

	int calibrationTime = 20; //20 seconds is calibration time
	int startTime = millis();
	int counter;

  Serial.print("Calibration started \n calibrationTime: ");
  Serial.println(calibrationTime);

	//start spinning
	moveControl(255,-255);

	while (((millis()-startTime)/1000) < calibrationTime){
		sensors[0]+=analogRead(A0);
		sensors[1]+=analogRead(A1);
		sensors[2]+=analogRead(A3);
		sensors[3]+=analogRead(A4);
		++counter;
	}

	//stop spinning
	moveControl(0,0);

	for (int i = 0; i < 3; i++){ 
		sensors[i]/=counter;
    Serial.print(i);
    Serial.print(": ");
    Serial.println(sensors[i]);
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
	//lcd.print(distance());
	lcd.setCursor(5,1);
	lcd.print("servo: ");
	lcd.print(head.read());
}

void setup(){
	head.attach(A2);
	head.write(85);
	lcd.begin(20,4);
	lcd.clear();
  Serial.begin(9600);

	pinMode(A0, INPUT);
  digitalWrite(A0,HIGH); //pull-up pre !degenerovanie dat
  pinMode(A1, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW); 
   motorControl(motor[0][0],motor[0][1],motor[0][2],255);
}

void loop(){
	showReadings();

	//write distance to 11,0 display position
	// try{
	// 	distance();
	// }
	delay(100);
}