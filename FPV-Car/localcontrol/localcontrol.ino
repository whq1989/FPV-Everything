//motor connected between A01 and A02

int STBY = 10; //standby   停止状态
int PWMA = 3; //Speed control  速度
int AIN1 = 9; //Direction  方向
int AIN2 = 8; //Direction  方向

void setup(){
pinMode(STBY, OUTPUT);
pinMode(PWMA, OUTPUT);
pinMode(AIN1, OUTPUT);
pinMode(AIN2, OUTPUT);
}

void loop(){
  moveMotor(90, 1);
  stopMotor();
  moveMotor(90, 0);
  delay(1000);
}


//------------moveMotor控制电机转速和方向
void moveMotor(int speed, int direction){
  //speed: 0 is off, and 255 is full speed 选择速度 75-200
  //direction: 0 clockwise, 1 counter-clockwise 选择旋转方向： 0表示顺时针，1表示逆时针
  digitalWrite(STBY, HIGH); //disable standby取消空置状态
  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;//电机默认的转动方向

  if(direction == 1){//方向置为1时，改变转向
    inPin1 = HIGH;
    inPin2 = LOW;
  }
  digitalWrite(AIN1, inPin1);//设置方向
  digitalWrite(AIN2, inPin2);
  analogWrite(PWMA, speed);//设置速度
}
//------------stopMotor控制电机转速和方向
void stopMotor(){//电机待机
  digitalWrite(STBY, LOW);
}
