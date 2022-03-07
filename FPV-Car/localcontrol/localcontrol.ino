#include <Servo.h>

Servo myservo;
//int sta_1_num=0;//前后速度缓存
//String sta_1_sta="q";//前后方向缓存
//int sta_2_num=0;//左右速度缓存
//String sta_2_sta="z";//左右方向缓存
//String readString="";
//String doCar="";
int num=0;
int cw=0;//前进方向定义
int ccw=1;//后退方向定义
int q;//前
int h;//后
int z;//左
int y;//右
int arrayInts[2];//临时存放速度方向数据
int i;//方向速度数据由2字节组成

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
  MotorStop();
  i=0;
}

void loop(){
  //MotorMove(90, 1); 控制电机速度和方向
  //MotorStop(); 电机待机
  //myservo.write(0); //控制舵机角度
  while(Serial.available()>0){
    num = Serial.read();
    arrayInts[i]=num;
    if(i>=1){
      num=arrayInts[1]*256+arrayInts[0];
      //组合方向速度数据
      //0-90是左方向
      //91-180是右方向
      //200-400是前方向，其中200属于0速度即停止
      //600-750是后方向
      if(num<90&&num>0){//数值表示向右偏转N度，例如50表示向右50度
        if(num!=z){//如果数值变化
          z=num;
          num=90+num;
          myservo.write(num);
        }
      }else if(num>90&&num<=180){//数值表示向左偏转N-90度，例如120表示向右30度
        if(num!=y){//如果数值变化
          y=num;
          num=180-num;
          myservo.write(num);
        }
      }else if(num>=200&&num<400){
        if(num!=q){//如果数值变化
          q=num;
          num=num-200;
          //MotorStop();//防止电机频繁正反转交替，方向变化时强制待机
          MotorMove(num, cw);//正方向行驶
        }
      }else if(num>=600&&num<800){
        if(num!=h){//如果数值变化
          h=num;
          num=num-600;
          //MotorStop();//防止电机频繁正反转交替，方向变化时强制待机
          MotorMove(num, ccw);//反方向行驶
        }
      }else{
        MotorStop();
      }
      Serial.println(num,DEC);
      num=0;
      i=0;
    }else{
      i++;
      delay(2);
    }
  }
  
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
