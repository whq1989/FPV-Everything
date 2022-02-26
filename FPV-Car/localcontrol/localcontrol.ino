#include <Servo.h>

Servo myservo;
int sta_1_num=0;//前后速度缓存
String sta_1_sta="q";//前后方向缓存
int sta_2_num=0;//左右速度缓存
String sta_2_sta="z";//左右方向缓存
String readString="";
String doCar="";
int num=0;

int STBY = 10; //standby   停止状态
int PWMA = 3; //Speed control  速度
int AIN1 = 9; //Direction  方向
int AIN2 = 8; //Direction  方向

void setup(){
pinMode(STBY, OUTPUT);
pinMode(PWMA, OUTPUT);
pinMode(AIN1, OUTPUT);
pinMode(AIN2, OUTPUT);

Serial.begin(9600); //设定硬串口波特率
Serial.println("BT is ready!");
Serial.setTimeout(10);
myservo.attach(11);
}

void loop(){
  //MotorMove(90, 1); 控制电机速度和方向
  //MotorStop(); 电机待机
  //myservo.write(pos); 控制舵机角度
  while(Serial.available()>0){
    char doCar = Serial.read();
    readString += doCar;
    num = Serial.parseInt();
    Serial.println(readString);
    Serial.println(num);
    
    if(readString=="q"){
      sta_1_num=num; 
      if(readString=sta_1_sta){
        MotorMove(sta_1_num, 1);
      }else{
        sta_1_sta=readString;
        MotorStop();
        MotorMove(sta_1_num, 1);
      }
    }else if(readString=="h"){
      sta_1_num=num;
      if(readString=sta_1_sta){
        MotorMove(sta_1_num, 0);
      }else{
        sta_1_sta=readString;
        MotorStop();
        MotorMove(sta_1_num, 0);
      }
    }else if(readString=="z"){
      myservo.write(num);
    }else if(readString=="y"){
      myservo.write(num);
    }else if(readString=="t"){
      MotorStop();
    }
    num=0;
    readString="";
  }

//------------控制舵机的角度
  //if (Serial.available()) { //如果HC-06发来数据  
    //int k=Serial.parseInt();//获取蓝牙传输的角度值
    //int k=Serial.read();
    //Serial.println(k);
    //if(k!=0){
     // old=k;
      //Serial.println(old);  
     // myservo.write(old);
   // }
    
    //myservo.write(k);
    //delayMicroseconds(5000);
    //    if(k!=0&&k!=old){//如果角度值有变化时，开始执行
    //      Serial.println(k); 
    //      if(k>old){
    //       for (pos = old; pos <= k; pos += 1) { // goes from 0 degrees to 180 degrees
    //         myservo.write(pos);              // tell servo to go to position in variable 'pos'
    //         delayMicroseconds(100);                       // waits 15ms for the servo to reach the position
    //       }
    //       }else{
    //         for (pos = old; pos >= k; pos -= 1) { // goes from 0 degrees to 180 degrees
    //           myservo.write(pos);              // tell servo to go to position in variable 'pos'
    //           delayMicroseconds(100);                       // waits 15ms for the servo to reach the position
    //         }
    //       }
    //      old=k;
    //    }
  //}
  
}


//------------moveMotor控制电机转速和方向
void MotorMove(int speed, int direction){
  //speed: 0 is off, and 255 is full speed 选择速度 75-200
  //direction: 0 clockwise, 1 counter-clockwise 选择旋转方向： 0表示顺时针，1表示逆时针
  digitalWrite(STBY, HIGH); //disable standby取消空置状态
  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;//电机默认的转动方向

  if(direction == 1){//方向置为1时，改变转向
    inPin1 = HIGH;
    inPin2 = LOW;
  }else{
    inPin2 = HIGH;
    inPin1 = LOW;
  }
  digitalWrite(AIN1, inPin1);//设置方向
  digitalWrite(AIN2, inPin2);
  analogWrite(PWMA, speed);//设置速度
}
//------------stopMotor控制电机转速和方向
void MotorStop(){//电机待机
  digitalWrite(STBY, LOW);
}
