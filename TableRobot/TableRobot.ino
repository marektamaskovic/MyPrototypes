const int trigPin = 23;
const int echoPin = 25;

int motor[3][2] = {	
					{5 , 2, 4},
					{3 , 0, 1},
					{9 , 8, 11},
					{10, 6, 7}
				};


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
	//lcd.begin(20,4);
	//lcd.clear();
  Serial.begin(9600);

	pinMode(A0, INPUT);
  digitalWrite(A0,HIGH); //pull-up pre !degenerovanie dat
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
   //motorControl(motor[0][0],motor[0][1],motor[0][2],255);
   moveControl(255,255);
}

void loop(){
	//showReadings();

	//write distance to 11,0 display position
	// try{
	// 	distance();
	// }
	moveControl(255,255);
	delay(100);
}