#include <SoftwareSerial.h>
// 设置软串口使用的针脚
SoftwareSerial softSerial(2, 3); //2为RX, 3为TX
void setup() {
  Serial.begin(9600); //设定硬串口波特率
  softSerial.begin(9600); //设定软串口波特率

  Serial.println("go");
  softSerial.println("AT");
}
void loop() {
  
  if (softSerial.available()) { //如果HC-06发来数据
    //int k = softSerial.parseInt(); 
    char m=softSerial.read();//读取1个字节的数据
    Serial.println(m); //通过硬串口打印输出
  }
}
