int motor[4][3] = {	
					{5 , 2, 4},
					{3 , 0, 1},
					{9 , 8, 7},
					{10, 6, 11}
				};

void motorControl(int enable,int input1,int input2,int speed){
	//Single motor controll function
	if (speed>0) {
		digitalWrite(input1,HIGH);
		digitalWrite(input2, LOW);
	}
	if (speed<0){
		digitalWrite(input1, LOW);
		digitalWrite(input2, HIGH);
	}
	if (speed=0){
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

void setup(){
  //Serial.begin(9600);
  for (int i = 0;i<13;i++){
  	pinMode(i, OUTPUT);
  	digitalWrite(i, LOW);
  }
  //moveControl(255,255);
}

void loop(){
	//digitalWrite(2,HIGH);
	//digitalWrite(4, LOW);
	//analogWrite(5, 255);
	moveControl(255,255);
	delay(1000);
}