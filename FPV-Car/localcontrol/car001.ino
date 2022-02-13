
#include <Servo.h>

// 设置软串口使用的针脚

Servo myservo;
int old=0;//外设数值的缓存

void setup() {
  Serial.begin(9600); //设定硬串口波特率
  Serial.println("BT is ready!");
  Serial.setTimeout(1);
  myservo.attach(9);
}
void loop() {  
  if (Serial.available()) { //如果HC-06发来数据  
    //int k=Serial.parseInt();//获取蓝牙传输的角度值
    int k=Serial.read();
    //Serial.println(k);
    if(k!=0){
      old=k;
      Serial.println(old);  
      myservo.write(old);
    }
    
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
  }
}
